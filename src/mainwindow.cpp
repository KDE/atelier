/* Atelier KDE Printer Host for 3D Printing
    Copyright (C) <2016>
    Author: Lays Rodrigues - lays.rodrigues@kde.org
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
#include <KActionCollection>
#include <KLocalizedString>
#include <KStandardAction>
#include <KXMLGUIFactory>
#include <memory>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QSplitter>
#include <QToolButton>
#include "dialogs/choosefiledialog.h"
#include "dialogs/profilesdialog.h"
#include "mainwindow.h"
#include "widgets/3dview/viewer3d.h"
#include "widgets/atcoreinstancewidget.h"
#include "widgets/videomonitorwidget.h"

MainWindow::MainWindow(QWidget *parent) :
    KXmlGuiWindow(parent)
    , m_currEditorView(nullptr)
    , m_theme(getTheme())
    , m_instances(new QTabWidget(this))
{
    initWidgets();
    setupActions();
    connect(m_instances, &QTabWidget::tabCloseRequested, [this](int index) {
        auto tempWidget = qobject_cast<AtCoreInstanceWidget *>(m_instances->widget(index));
        if (tempWidget->isPrinting()) {
            if (askToClose()) {
                delete tempWidget;
            } else {
                return;
            }
        } else {
            delete tempWidget;
        }
        if (m_instances->count() == 1) {
            m_instances->setTabsClosable(false);
            m_instances->setMovable(false);
        }
    });
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    bool closePrompt = false;
    for (int i = 0; i < m_instances->count(); i++) {
        AtCoreInstanceWidget *instance = qobject_cast<AtCoreInstanceWidget *>(m_instances->widget(i));
        if (instance->isPrinting()) {
            closePrompt = true;
            break;
        }
    }
    if (closePrompt) {
        if (askToClose()) {
            event->accept();
        } else {
            event->ignore();
        }
    }
    disconnect(m_gcodeEditor, &GCodeEditorWidget::updateClientFactory, this, &MainWindow::updateClientFactory);
}

void MainWindow::initWidgets()
{
    setupLateralArea();
    newAtCoreInstance();
    // View:
    // Sidebar, Sidebar Controls, Printer Tabs.
    // Sidebar Controls and Printer Tabs can be resized, Sidebar can't.
    auto splitter = new QSplitter();
    splitter->addWidget(m_lateral.m_stack);
    splitter->addWidget(m_instances);

    auto addTabBtn = new QToolButton();
    addTabBtn->setText("+");
    addTabBtn->setToolTip(i18n("Create new instance"));
    addTabBtn->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_T));
    connect(addTabBtn, &QToolButton::clicked, this, &MainWindow::newAtCoreInstance);
    m_instances->setCornerWidget(addTabBtn, Qt::TopLeftCorner);

    auto *centralLayout = new QHBoxLayout();
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
    newInstanceWidget->setFileCount(m_openFiles.size());
    connect(this, &MainWindow::profilesChanged, newInstanceWidget, &AtCoreInstanceWidget::updateProfileData);
    connect(newInstanceWidget, &AtCoreInstanceWidget::requestProfileDialog, [this] {
        std::unique_ptr<ProfilesDialog> pd(new ProfilesDialog);
        pd->exec();
        emit(profilesChanged());
    });

    connect(newInstanceWidget, &AtCoreInstanceWidget::requestFileChooser, [newInstanceWidget, this] {
        switch (m_openFiles.size())
        {
        case 0:
            QMessageBox::warning(this, i18n("Error"),
                                 i18n("There's no GCode File open. \n Please select a file and try again."),
                                 QMessageBox::Ok);
            break;
        case 1:
            newInstanceWidget->printFile(m_openFiles.at(0));
            break;
        default:
            ChooseFileDialog dialog(this, m_openFiles);
            if (dialog.exec() == QDialog::Accepted) {
                newInstanceWidget->printFile(dialog.choosenFile());
            }
        }
    });

    connect(newInstanceWidget, &AtCoreInstanceWidget::connectionChanged, this, &MainWindow::atCoreInstanceNameChange);

    if (m_instances->count() > 1) {
        m_instances->setTabsClosable(true);
        m_instances->setMovable(true);
        m_instances->setCurrentIndex(m_instances->count() - 1);
    }
}
// Move to LateralArea.
void MainWindow::setupLateralArea()
{
    m_lateral.m_toolBar = new QWidget();
    m_lateral.m_stack = new QStackedWidget();
    auto buttonLayout = new QVBoxLayout();

    auto setupButton = [this, buttonLayout](const QString & key, const QString & text, const QIcon & icon, QWidget * w) {
        auto *btn = new QPushButton(m_lateral.m_toolBar);
        btn->setToolTip(text);
        btn->setAutoExclusive(true);
        btn->setCheckable(true);
        //3d view is on top set it checked so users see its selected.
        btn->setChecked(key == "3d");
        btn->setIcon(icon);
        btn->setFixedSize(48, 48);
        btn->setIconSize(QSize(48, 48));
        btn->setFlat(true);
        m_lateral.m_stack->addWidget(w);
        m_lateral.m_map[key] = {btn, w};
        buttonLayout->addWidget(btn);

        connect(btn, &QPushButton::clicked, [this, w, btn] {
            if (m_lateral.m_stack->currentWidget() == w)
            {
                m_lateral.m_stack->setHidden(m_lateral.m_stack->isVisible());
                if (m_lateral.m_stack->isHidden()) {
                    btn->setCheckable(false);
                    btn->setCheckable(true);
                }
            } else
            {
                m_lateral.m_stack->setHidden(false);
                m_lateral.m_stack->setCurrentWidget(w);
            }
            toggleGCodeActions();
        });
    };

    m_gcodeEditor = new GCodeEditorWidget(this);
    connect(m_gcodeEditor, &GCodeEditorWidget::updateClientFactory, this, &MainWindow::updateClientFactory);
    setupButton("3d", i18n("&3D"), QIcon::fromTheme("draw-cuboid", QIcon(QString(":/%1/3d").arg(m_theme))), new Viewer3D(this));
    setupButton("gcode", i18n("&GCode"), QIcon::fromTheme("accessories-text-editor", QIcon(":/icon/edit")), m_gcodeEditor);
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
    connect(action, &QAction::triggered, this, &MainWindow::openActionTriggered);

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

void MainWindow::openActionTriggered()
{
    QUrl fileName = QFileDialog::getOpenFileUrl(
                        this
                        , i18n("Open GCode")
                        , QUrl::fromLocalFile(QDir::homePath())
                        , i18n("GCode(*.gco *.gcode);;All Files(*.*)")
                    );

    if (!fileName.isEmpty()) {
        loadFile(fileName);
    }
}

void MainWindow::loadFile(const QUrl &fileName)
{
    if (!fileName.isEmpty()) {

        m_lateral.get<GCodeEditorWidget>("gcode")->loadFile(fileName);
        m_lateral.get<Viewer3D>("3d")->drawModel(fileName.toString());

        const int tabs = m_instances->count();
        m_openFiles.append(fileName);

        for (int i = 0; i < tabs; ++i) {
            auto instance = qobject_cast<AtCoreInstanceWidget *>(m_instances->widget(i));
            instance->setFileCount(m_openFiles.size());
        }
    }
}

void MainWindow::atCoreInstanceNameChange(const QString &name)
{
    m_instances->setTabText(sender()->objectName().toInt(), name);
}

QString MainWindow::getTheme()
{
    return palette().text().color().value() >= QColor(Qt::lightGray).value() ? \
           QString("dark") : QString("light");
}

bool MainWindow::askToClose()
{
    bool rtn = false;
    int result = QMessageBox::question(
                     this
                     , i18n("Printing")
                     , i18n("Currently printing! \nAre you sure you want to close?")
                     , QMessageBox::Close
                     , QMessageBox::Cancel
                 );

    switch (result) {
    case QMessageBox::Close:
        rtn = true;
        break;
    default:
        break;
    }
    return rtn;
}

void MainWindow::toggleGCodeActions()
{
    if (m_lateral.m_stack->currentWidget() == m_lateral.m_map["gcode"].second && m_lateral.m_stack->isVisible()) {
        if (m_currEditorView) {
            guiFactory()->addClient(m_currEditorView);
        }
    } else {
        guiFactory()->removeClient(m_currEditorView);
    }
}

void MainWindow::updateClientFactory(KTextEditor::View *view)
{
    if (m_lateral.m_stack->currentWidget() == m_lateral.m_map["gcode"].second) {
        if (m_currEditorView) {
            guiFactory()->removeClient(m_currEditorView);
        }
        if (view) {
            guiFactory()->addClient(view);
        }
    }
    m_currEditorView = view;
}
