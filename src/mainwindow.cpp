/* Atelier KDE Printer Host for 3D Printing
    Copyright (C) <2016>
    Author: Lays Rodrigues - laysrodrigues@gmail.com
            Chris Rizzitello - rizzitello@kde.org

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <KLocalizedString>
#include <KStandardAction>
#include <KActionCollection>
#include <KXMLGUIFactory>
#include <dialogs/connectsettingsdialog.h>
#include <dialogs/profilesdialog.h>
#include <widgets/gcodeeditorwidget.h>
#include <set>
#include <QToolButton>
#include <memory>
#include <QSplitter>
#include <QHBoxLayout>
#include <widgets/atcoreinstancewidget.h>
#include <QStackedWidget>
#include <widgets/3dview/viewer3d.h>
#include <widgets/videomonitorwidget.h>

MainWindow::MainWindow(QWidget *parent) :
    KXmlGuiWindow(parent),
    ui(new Ui::MainWindow),
    m_curr_editor_view(nullptr),
    m_instances(new QTabWidget(this))
{
    ui->setupUi(this);
    initWidgets();
    setupActions();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initWidgets()
{
    auto newInstanceWidget = new AtCoreInstanceWidget();
    m_instances->addTab(newInstanceWidget, i18n("Connect your printer"));

    setupLateralArea();

    // View:
    // Sidebar, Sidevar Controls, Printer Tabs.
    // Sidevar Controls and Printer Tabs can be resized, Sidebar cant.
    auto *centralLayout = new QHBoxLayout();
    auto splitter = new QSplitter();
    splitter->addWidget(m_lateral.m_stack);
    splitter->addWidget(m_instances);
    centralLayout->addWidget(m_lateral.m_toolBar);
    centralLayout->addWidget(splitter);
    ui->centralwidget->setLayout(centralLayout);
}

// Move to LateralArea.
void MainWindow::setupLateralArea()
{
    m_lateral.m_toolBar = new QWidget();
    m_lateral.m_stack = new QStackedWidget();
    auto *buttonLayout = new QVBoxLayout();

    auto setupButton = [this, buttonLayout](const QString& key, const QString& text, const QIcon& icon, QWidget *w) {
        auto *btn = new QPushButton(m_lateral.m_toolBar);
        btn->setToolTip(text);
        btn->setAutoExclusive(true);
        btn->setCheckable(true);
        btn->setIcon(icon);
        btn->setIconSize(QSize(64,64));
        btn->setFlat(true);
        m_lateral.m_stack->addWidget(w);
        m_lateral.m_map[key] = {btn, w};

        buttonLayout->addWidget(btn);
        connect(btn, &QToolButton::toggled, [this, w](bool checked) {
            if (checked)
                m_lateral.m_stack->setCurrentWidget(w);
        });
    };

    auto *gcodeEditor = new GCodeEditorWidget(this);
     connect(gcodeEditor, &GCodeEditorWidget::updateClientFactory, this, [&](KTextEditor::View* view){
         guiFactory()->removeClient(m_curr_editor_view);
         guiFactory()->addClient(view);
         m_curr_editor_view = view;
     });

    setupButton("3d",    i18n("&3D"), QIcon(":/icon/atelier"), new Viewer3D(this));
    setupButton("gcode", i18n("&GCode"), QIcon(":/icon/atelier"), gcodeEditor);
    setupButton("video", i18n("&Video"), QIcon(":/icon/atelier"), new VideoMonitorWidget(this));
    buttonLayout->addStretch();
    m_lateral.m_toolBar->setLayout(buttonLayout);
}

void MainWindow::setupActions()
{
    // Actions for the Toolbar
    QAction *action;
    action = actionCollection()->addAction(QStringLiteral("open_gcode"));
    action->setText(i18n("&Open GCode"));
    connect(action, &QAction::triggered, this, &MainWindow::openFile);

    action = actionCollection()->addAction(QStringLiteral("connect"));
    action->setText(i18n("&Connect"));
    connect(action, &QAction::triggered, [ & ]{
            std::unique_ptr<ConnectSettingsDialog> csd(new ConnectSettingsDialog);
            connect(csd.get(), &ConnectSettingsDialog::startConnection, [ & ](const QString& port, const QMap<QString, QVariant>& data) {
                newConnection(port, data);
            });
            csd->exec();
    });

    action = actionCollection()->addAction(QStringLiteral("profiles"));
    action->setText(i18n("&Profiles"));
    connect(action, &QAction::triggered, [ & ] {
        std::unique_ptr<ProfilesDialog> pd(new ProfilesDialog);
        pd->exec();
    });

    #ifdef Q_OS_LINUX
    //only set icons from theme on linux
        actionCollection()->action(QStringLiteral("profiles"))->setIcon(QIcon::fromTheme("emblem-favorite"));
    #endif
    //use style's standardIcon for the icons we can.
    actionCollection()->action(QStringLiteral("open_gcode"))->setIcon(style()->standardIcon(QStyle::SP_DirOpenIcon));

    action = KStandardAction::quit(qApp, SLOT(quit()), actionCollection());

    setupGUI(Default, ":/atelierui");
}

void MainWindow::openFile()
{
    QUrl fileName = QFileDialog::getOpenFileUrl(this, i18n("Open GCode"),
                              QDir::homePath(), i18n("GCode (*.gco *.gcode)"));

    if (!fileName.isEmpty()) {

        m_lateral.get<GCodeEditorWidget>("gcode")->loadFile(fileName);
        m_lateral.get<Viewer3D>("3d")->drawModel(fileName.toString());

        const int tabs = m_instances->count();
        m_openFiles.append(fileName);

        for(int i=0; i < tabs; ++i){
            auto instance = qobject_cast<AtCoreInstanceWidget*>(m_instances->widget(i));
            instance->setOpenFiles(m_openFiles);
        }
    }
}
void MainWindow::newConnection(const QString& port, const QMap<QString, QVariant>& profile)
{
    const int tabs = m_instances->count();
    if(tabs == 1){
        auto instance = qobject_cast<AtCoreInstanceWidget*>(m_instances->currentWidget());
        if(!instance->connected()){
            instance->startConnection(port, profile);
            m_instances->setTabText(m_instances->currentIndex(), profile["name"].toString());
            return;
        }
    }
    auto newInstanceWidget = new AtCoreInstanceWidget();
    m_instances->addTab(newInstanceWidget, profile["name"].toString());
    newInstanceWidget->startConnection(port, profile);
}
