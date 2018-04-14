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
#include <dialogs/profilesdialog.h>
#include <KLocalizedString>
#include <KStandardAction>
#include <KActionCollection>
#include <KXMLGUIFactory>
#include <memory>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QSplitter>
#include <QToolButton>
#include <widgets/3dview/viewer3d.h>
#include <widgets/atcoreinstancewidget.h>
#include <widgets/gcodeeditorwidget.h>
#include <widgets/videomonitorwidget.h>


MainWindow::MainWindow(QWidget *parent) :
    KXmlGuiWindow(parent),
    m_currEditorView(nullptr),
    m_instances(new QTabWidget(this))
{
    m_theme = palette().text().color().value() >= QColor(Qt::lightGray).value() ? QString("dark") : QString("light");

    initWidgets();
    setupActions();
    connect(m_instances, &QTabWidget::tabCloseRequested, [this] (int index){
        QWidget *tempWidget= m_instances->widget(index);
        delete tempWidget;

        if(m_instances->count() == 1) {
            m_instances->setTabsClosable(false);
            m_instances->setMovable(false);
        }
    });
}

void MainWindow::initWidgets()
{

    setupLateralArea();
    newAtCoreInstance();

    // View:
    // Sidebar, Sidevar Controls, Printer Tabs.
    // Sidevar Controls and Printer Tabs can be resized, Sidebar cant.
    auto *centralLayout = new QHBoxLayout();
    auto splitter = new QSplitter();
    splitter->addWidget(m_lateral.m_stack);
    splitter->addWidget(m_instances);

    auto addTabBtn =new QToolButton();
    addTabBtn->setText("+");
    addTabBtn->setToolTip(i18n("Create new instance"));
    addTabBtn->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_T));
    connect(addTabBtn, &QToolButton::clicked, this, &MainWindow::newAtCoreInstance);
    m_instances->setCornerWidget(addTabBtn, Qt::TopLeftCorner);

    centralLayout->addWidget(m_lateral.m_toolBar);
    centralLayout->addWidget(splitter);
    auto *centralWidget = new QWidget();
    centralWidget->setLayout(centralLayout);
    setCentralWidget(centralWidget);
}

void MainWindow::newAtCoreInstance()
{
    auto newInstanceWidget = new AtCoreInstanceWidget();
    QString name = QString::number(m_instances->addTab(newInstanceWidget, i18n("Connect a printer")));
    newInstanceWidget->setObjectName(name);
    newInstanceWidget->setOpenFiles(m_openFiles);
    connect(this, &MainWindow::profilesChanged, newInstanceWidget, &AtCoreInstanceWidget::updateProfileData);
    connect(newInstanceWidget, &AtCoreInstanceWidget::requestProfileDialog, [ this ] {
        std::unique_ptr<ProfilesDialog> pd(new ProfilesDialog);
        pd->exec();
        emit(profilesChanged());
    });
    connect(newInstanceWidget, &AtCoreInstanceWidget::connectionChanged, this, &MainWindow::atCoreInstanceNameChange);

    if(m_instances->count() > 1) {
        m_instances->setTabsClosable(true);
        m_instances->setMovable(true);
        m_instances->setCurrentIndex(m_instances->count()-1);
    }
}
// Move to LateralArea.
void MainWindow::setupLateralArea()
{
    m_lateral.m_toolBar = new QWidget();
    m_lateral.m_stack = new QStackedWidget();
    auto buttonLayout = new QVBoxLayout();

    auto setupButton = [this, buttonLayout](const QString& key, const QString& text, const QIcon& icon, QWidget *w) {
        auto *btn = new QPushButton(m_lateral.m_toolBar);
        btn->setToolTip(text);
        btn->setAutoExclusive(true);
        btn->setCheckable(true);
        //3d view is on top set it checked so users see its selected.
        btn->setChecked(key == "3d");
        btn->setIcon(icon);
        btn->setFixedSize(48,48);
        btn->setIconSize(QSize(48,48));
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
     connect(gcodeEditor, &GCodeEditorWidget::updateClientFactory, this, [this](KTextEditor::View* view){
         guiFactory()->removeClient(m_currEditorView);
         guiFactory()->addClient(view);
         m_currEditorView = view;
     });
    setupButton("3d",    i18n("&3D"), QIcon::fromTheme("draw-cuboid", QIcon(QString(":/%1/3d").arg(m_theme))), new Viewer3D(this));
    setupButton("gcode", i18n("&GCode"), QIcon::fromTheme("accessories-text-editor", QIcon(":/icon/edit")), gcodeEditor);
    setupButton("video", i18n("&Video"), QIcon::fromTheme("camera-web", QIcon(":/icon/video")), new VideoMonitorWidget(this));
    buttonLayout->addStretch();
    m_lateral.m_toolBar->setLayout(buttonLayout);
}

void MainWindow::setupActions()
{
    // Actions for the Toolbar
    QAction *action;
    action = actionCollection()->addAction(QStringLiteral("open"));
    action->setIcon(QIcon::fromTheme("document-open", QIcon(QString(":/%1/open").arg(m_theme))));

    action->setText(i18n("&Open"));
    actionCollection()->setDefaultShortcut(action, QKeySequence::Open);
    connect(action, &QAction::triggered, this, &MainWindow::openFile);

    action = actionCollection()->addAction(QStringLiteral("new_instance"));
    action->setIcon(QIcon::fromTheme("list-add", QIcon(QString(":/%1/addTab").arg(m_theme))));

    action->setText(i18n("&New Connection"));
    actionCollection()->setDefaultShortcut(action, QKeySequence::AddTab);
    connect(action, &QAction::triggered, this, &MainWindow::newAtCoreInstance);

    action = actionCollection()->addAction(QStringLiteral("profiles"));
    action->setIcon(QIcon::fromTheme("document-properties", QIcon(QString(":/%1/configure").arg(m_theme))));

    action->setText(i18n("&Profiles"));
    connect(action, &QAction::triggered, [this] {
        std::unique_ptr<ProfilesDialog> pd(new ProfilesDialog);
        pd->exec();
        emit(profilesChanged());
    });

    action = actionCollection()->addAction(QStringLiteral("quit"));
    action->setIcon(QIcon::fromTheme("application-exit", QIcon(":/icon/exit")));

    action->setText(i18n("&Quit"));
    actionCollection()->setDefaultShortcut(action, QKeySequence::Quit);
    connect(action, &QAction::triggered, this, &MainWindow::close);

    setupGUI(Default, "atelierui");
}

void MainWindow::openFile()
{
    QUrl fileName = QFileDialog::getOpenFileUrl(this, i18n("Open GCode"),
                        QUrl::fromLocalFile(QDir::homePath()), i18n("GCode(*.gco *.gcode);;All Files(*.*)"));

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

void MainWindow::atCoreInstanceNameChange(const QString &name)
{
    m_instances->setTabText(sender()->objectName().toInt(),name);
}
