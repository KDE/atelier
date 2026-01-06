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
#include "mainwindow.h"
#include "dialogs/choosefiledialog.h"
#include "dialogs/profilesdialog.h"
#include "widgets/3dview/viewer3d.h"
#include "widgets/atcoreinstancewidget.h"
#include "widgets/videomonitorwidget.h"
#include "widgets/welcomewidget.h"
#include <KActionCollection>
#include <KLocalizedString>
#include <KStandardAction>
#include <KXMLGUIFactory>
#include <MachineInfo>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QSplitter>
#include <QToolButton>
#include <memory>

using namespace Qt::StringLiterals;

MainWindow::MainWindow(QWidget *parent)
    : KXmlGuiWindow(parent)
    , m_currInstance(0)
    , m_theme(getTheme())
    , m_instances(new QTabWidget(this))
{
    initWidgets();
    setupActions();
    setAcceptDrops(true);

    connect(m_instances, &QTabWidget::tabCloseRequested, this, [this](int index) {
        auto tempWidget = qobject_cast<AtCoreInstanceWidget *>(m_instances->widget(index));
        if (tempWidget->isPrinting() && !askToClose()) {
            return;
        }

        tempWidget->disconnect();
        tempWidget->close();
        m_instances->removeTab(index);

        if (m_instances->count() == 1) {
            m_instances->setTabsClosable(false);
            m_instances->setMovable(false);
        }
    });
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (!askToSave(m_gcodeEditor->modifiedFiles())) {
        event->ignore();
    }

    bool closePrompt = false;
    for (int i = 0; i < m_instances->count(); i++) {
        auto instance = qobject_cast<AtCoreInstanceWidget *>(m_instances->widget(i));
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
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        processDropEvent(mimeData->urls());
    }
}

void MainWindow::processDropEvent(const QList<QUrl> &fileList)
{
    for (const auto &url : fileList) {
        // Loop thru the urls and only load ones ending our "supported" formats
        QString ext = url.toLocalFile().split('.'_L1).last();
        if (ext.contains(u"gcode"_s, Qt::CaseInsensitive) || ext.contains(u"gco"_s, Qt::CaseInsensitive)) {
            loadFile(url);
        }
    }
}

void MainWindow::initWidgets()
{
    setupLateralArea();
    newAtCoreInstance();
    // View:
    // Sidebar, Sidebar Controls, Printer Tabs.
    // Sidebar Controls and Printer Tabs can be resized, Sidebar can't.
    auto splitter = new QSplitter(this);
    splitter->addWidget(m_lateral.m_stack);
    splitter->addWidget(m_instances);

    auto addTabBtn = new QToolButton(this);
    addTabBtn->setIconSize(QSize(fontMetrics().lineSpacing(), fontMetrics().lineSpacing()));
    addTabBtn->setIcon(QIcon::fromTheme(u"list-add"_s, QIcon(QStringLiteral(":/%1/addTab").arg(m_theme))));
    addTabBtn->setToolTip(i18n("Create new instance"));
    addTabBtn->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_T));
    connect(addTabBtn, &QToolButton::clicked, this, &MainWindow::newAtCoreInstance);
    m_instances->setCornerWidget(addTabBtn, Qt::TopLeftCorner);

    auto *centralLayout = new QHBoxLayout();
    centralLayout->addWidget(m_lateral.m_toolBar);
    centralLayout->addWidget(splitter);

    auto *centralWidget = new QWidget(this);
    centralWidget->setLayout(centralLayout);
    setCentralWidget(centralWidget);
}

void MainWindow::newAtCoreInstance()
{
    auto newInstanceWidget = new AtCoreInstanceWidget(this);
    QString name = QString::number(m_instances->addTab(newInstanceWidget, i18n("Connect a printer")));
    newInstanceWidget->setObjectName(name);
    newInstanceWidget->setFileCount(m_openFiles.size());
    connect(MachineInfo::instance(), &MachineInfo::profilesChanged, newInstanceWidget, &AtCoreInstanceWidget::updateProfileData);
    connect(newInstanceWidget, &AtCoreInstanceWidget::requestProfileDialog, this, [this] {
        std::unique_ptr<ProfilesDialog> pd(new ProfilesDialog(this));
        pd->exec();
        Q_EMIT(profilesChanged());
    });

    connect(newInstanceWidget, &AtCoreInstanceWidget::requestFileChooser, this, [newInstanceWidget, this] {
        QUrl file;
        switch (m_openFiles.size()) {
        case 0:
            QMessageBox::warning(this, i18n("Error"), i18n("There's no GCode file open.\nPlease select a file and try again."), QMessageBox::Ok);
            break;
        case 1:
            file = m_openFiles.at(0);
            break;
        default:
            ChooseFileDialog dialog(this, m_openFiles);
            if (dialog.exec() != QDialog::Accepted) {
                return;
            }
            file = dialog.choosenFile();
            break;
        }
        if (m_gcodeEditor->modifiedFiles().contains(file)) {
            int result = QMessageBox::question(
                this,
                i18n("Document Modified"),
                i18n("%1 \nContains unsaved changes that will not be in the print.\nWould you like to save them before printing?", file.toLocalFile()),
                QMessageBox::Save | QMessageBox::Cancel | QMessageBox::Ignore);
            if (result == QMessageBox::Cancel) {
                return;
            }
            if (result == QMessageBox::Save) {
                m_gcodeEditor->saveFile(file);
            }
        }
        newInstanceWidget->printFile(file);
    });

    connect(newInstanceWidget, &AtCoreInstanceWidget::bedSizeChanged, this, [this](const QSize &newSize) {
        if (m_currInstance == m_instances->currentIndex()) {
            updateBedSize(newSize);
        }
    });

    connect(newInstanceWidget, &AtCoreInstanceWidget::connectionChanged, this, [this, newInstanceWidget](const QString &newName) {
        m_instances->setTabText(m_instances->indexOf(newInstanceWidget), newName);
    });

    if (m_instances->count() > 1) {
        m_instances->setTabsClosable(true);
        m_instances->setMovable(true);
        m_instances->setCurrentIndex(m_instances->count() - 1);
    }
}
// Move to LateralArea.
void MainWindow::setupLateralArea()
{
    m_lateral.m_toolBar = new QWidget(this);
    m_lateral.m_stack = new QStackedWidget(this);
    auto buttonLayout = new QVBoxLayout();

    auto setupButton = [this, buttonLayout](const QString &key, const QString &text, const QIcon &icon, QWidget *w) {
        auto *btn = new QPushButton(m_lateral.m_toolBar);
        btn->setToolTip(text);
        btn->setAutoExclusive(true);
        btn->setCheckable(true);
        // Check the top most widget, so users see its selected at startup time.
        btn->setChecked(key == QStringLiteral("welcome"));
        btn->setIcon(icon);
        // Set an iconSize based on the DPI.
        // 96 was considered to be the "standard" DPI for years.
        // Hi-dpi monitors have a higher DPI; 150+
        // Tiny or old screen could have a lower DPI.
        // Start our iconSize at 16 so with a low DPI we get a sane iconsize.
        // Use 72 to better scale for less dense Hi-Dpi screens
        int iconSize = 16 + ((logicalDpiX() / 72) * 16);
        btn->setIconSize(QSize(iconSize, iconSize));
        btn->setFixedSize(btn->iconSize());
        btn->setFlat(true);
        m_lateral.m_stack->addWidget(w);
        m_lateral.m_map[key] = {btn, w};
        buttonLayout->addWidget(btn);

        connect(btn, &QPushButton::clicked, this, [this, w, btn] {
            if (m_lateral.m_stack->currentWidget() == w) {
                m_lateral.m_stack->setHidden(m_lateral.m_stack->isVisible());
                if (m_lateral.m_stack->isHidden()) {
                    btn->setCheckable(false);
                    btn->setCheckable(true);
                }
            } else {
                m_lateral.m_stack->setHidden(false);
                m_lateral.m_stack->setCurrentWidget(w);
            }
            toggleGCodeActions();
        });
    };

    m_gcodeEditor = new GCodeEditorWidget(this);
    connect(m_gcodeEditor, &GCodeEditorWidget::updateClientFactory, this, &MainWindow::updateClientFactory);
    connect(m_gcodeEditor, &GCodeEditorWidget::droppedUrls, this, &MainWindow::processDropEvent);
    connect(m_gcodeEditor, &GCodeEditorWidget::fileClosed, this, [this](const QUrl &file) {
        m_openFiles.removeAll(file);
    });

    auto *viewer3D = new Viewer3D(this);
    connect(viewer3D, &Viewer3D::droppedUrls, this, &MainWindow::processDropEvent);
    // Connect for bed size
    connect(m_instances, &QTabWidget::currentChanged, this, [this](int index) {
        m_currInstance = index;
        auto tempWidget = qobject_cast<AtCoreInstanceWidget *>(m_instances->widget(index));
        updateBedSize(tempWidget->bedSize());
    });

    connect(m_gcodeEditor, &GCodeEditorWidget::currentFileChanged, this, [viewer3D](const QUrl &url) {
        viewer3D->drawModel(url.toLocalFile());
    });

    setupButton(u"welcome"_s, i18n("Welcome"), QIcon::fromTheme(u"go-home"_s, QIcon(QStringLiteral(":/%1/home").arg(m_theme))), new WelcomeWidget(this));
    setupButton(u"3d"_s, i18n("3D"), QIcon::fromTheme(u"draw-cuboid"_s, QIcon(QStringLiteral(":/%1/3d").arg(m_theme))), viewer3D);
    setupButton(u"gcode"_s, i18n("GCode"), QIcon::fromTheme(u"accessories-text-editor"_s, QIcon(u":/icon/edit"_s)), m_gcodeEditor);
    setupButton(u"video"_s, i18n("Video"), QIcon::fromTheme(u"camera-web"_s, QIcon(u":/icon/video"_s)), new VideoMonitorWidget(this));
    buttonLayout->addStretch();
    m_lateral.m_toolBar->setLayout(buttonLayout);
}

void MainWindow::setupActions()
{
    // Actions for the Toolbar
    QAction *action;
    action = actionCollection()->addAction(QStringLiteral("open"));
    action->setIcon(QIcon::fromTheme(u"document-open"_s, QIcon(QStringLiteral(":/%1/open").arg(m_theme))));

    action->setText(i18n("&Open"));
    actionCollection()->setDefaultShortcut(action, QKeySequence::Open);
    connect(action, &QAction::triggered, this, &MainWindow::openActionTriggered);

    action = actionCollection()->addAction(QStringLiteral("new_instance"));
    action->setIcon(QIcon::fromTheme(u"list-add"_s, QIcon(QStringLiteral(":/%1/addTab").arg(m_theme))));

    action->setText(i18n("&New Connection"));
    actionCollection()->setDefaultShortcut(action, QKeySequence::AddTab);
    connect(action, &QAction::triggered, this, &MainWindow::newAtCoreInstance);

    action = actionCollection()->addAction(QStringLiteral("profiles"));
    action->setIcon(QIcon::fromTheme(u"document-properties"_s, QIcon(QStringLiteral(":/%1/configure").arg(m_theme))));

    action->setText(i18n("&Profiles"));
    connect(action, &QAction::triggered, this, [this] {
        std::unique_ptr<ProfilesDialog> pd(new ProfilesDialog);
        pd->exec();
        Q_EMIT(profilesChanged());
    });

    action = actionCollection()->addAction(QStringLiteral("quit"));
    action->setIcon(QIcon::fromTheme(u"application-exit"_s, QIcon(u":/icon/exit"_s)));

    action->setText(i18n("&Quit"));
    actionCollection()->setDefaultShortcut(action, QKeySequence::Quit);
    connect(action, &QAction::triggered, this, &MainWindow::close);

    setupGUI(Default, u"atelierui"_s);
}

void MainWindow::openActionTriggered()
{
    QList<QUrl> fileList =
        QFileDialog::getOpenFileUrls(this, i18n("Open GCode"), QUrl::fromLocalFile(QDir::homePath()), i18n("GCode(*.gco *.gcode);;All Files(*.*)"));
    for (const auto &url : fileList) {
        loadFile(url);
    }
}

void MainWindow::loadFile(const QUrl &fileName)
{
    if (!fileName.isEmpty()) {
        m_lateral.get<GCodeEditorWidget>(u"gcode"_s)->loadFile(fileName);
        m_lateral.get<Viewer3D>(u"3d"_s)->drawModel(fileName.toLocalFile());
        // Make 3dview focused when opening a file
        if (m_openFiles.isEmpty() && m_lateral.m_stack->currentWidget() == m_lateral.get<WelcomeWidget>(u"welcome"_s)) {
            m_lateral.getButton<QPushButton>(u"3d"_s)->setChecked(true);
            m_lateral.m_stack->setCurrentWidget(m_lateral.get<Viewer3D>(u"3d"_s));
        }

        const int tabs = m_instances->count();
        if (!m_openFiles.contains(fileName)) {
            m_openFiles.append(fileName);
        }

        for (int i = 0; i < tabs; ++i) {
            auto instance = qobject_cast<AtCoreInstanceWidget *>(m_instances->widget(i));
            instance->setFileCount(m_openFiles.size());
        }
    }
}

QString MainWindow::getTheme()
{
    return palette().text().color().value() >= QColor(Qt::lightGray).value() ? u"dark"_s : u"light"_s;
}

bool MainWindow::askToClose()
{
    bool rtn = false;
    int result =
        QMessageBox::question(this, i18n("Printing"), i18n("Currently printing! \nAre you sure you want to close?"), QMessageBox::Close, QMessageBox::Cancel);

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
    if (m_lateral.m_stack->currentWidget() == m_lateral.m_map[u"gcode"_s].second && m_lateral.m_stack->isVisible()) {
        if (m_currEditorView) {
            guiFactory()->addClient(m_currEditorView);
        }
    } else {
        guiFactory()->removeClient(m_currEditorView);
    }
}

void MainWindow::updateClientFactory(KTextEditor::View *view)
{
    if (m_lateral.m_stack->currentWidget() == m_lateral.m_map[u"gcode"_s].second) {
        if (m_currEditorView) {
            guiFactory()->removeClient(m_currEditorView);
        }
        if (view) {
            guiFactory()->addClient(view);
        }
    }
    m_currEditorView = view;
}

bool MainWindow::askToSave(const QVector<QUrl> &fileList)
{
    if (fileList.isEmpty()) {
        return true;
    }
    QSize iconSize = QSize(fontMetrics().lineSpacing(), fontMetrics().lineSpacing());
    auto dialog = new QDialog(this);
    const int padding = 30;
    auto listWidget = new QListWidget(dialog);
    listWidget->setMinimumWidth(fontMetrics().height() / 2 * padding);
    for (const auto &url : fileList) {
        listWidget->addItem(url.toLocalFile() + u" [*]"_s);
    }

    auto hLayout = new QHBoxLayout();
    auto saveBtn = new QPushButton(QIcon::fromTheme(u"document-save"_s, QIcon(QStringLiteral(":/%1/save").arg(m_theme))), i18n("Save Selected"), dialog);
    saveBtn->setIconSize(iconSize);
    saveBtn->setEnabled(false);

    connect(listWidget, &QListWidget::currentRowChanged, this, [saveBtn](const int currentRow) {
        saveBtn->setEnabled(currentRow >= 0);
    });

    connect(saveBtn, &QPushButton::clicked, this, [this, listWidget, &fileList, dialog] {
        if (!m_gcodeEditor->saveFile(fileList.at(listWidget->currentRow()))) {
            QMessageBox::information(this, i18n("Save Failed"), i18n("Failed to save file: %1", fileList.at(listWidget->currentRow()).toLocalFile()));
        } else {
            listWidget->item(listWidget->currentRow())->setText(listWidget->item(listWidget->currentRow())->text().remove(u" [*]"_s));
            for (int i = 0; i < listWidget->count(); i++) {
                if (listWidget->item(i)->text().endsWith(u" [*]"_s)) {
                    return;
                }
            }
            dialog->accept();
        }
    });
    hLayout->addWidget(saveBtn);

    auto saveAllBtn = new QPushButton(QIcon::fromTheme(u"document-save-all"_s, QIcon(QStringLiteral(":/%1/saveAll").arg(m_theme))), i18n("Save All"), dialog);
    saveAllBtn->setIconSize(iconSize);
    connect(saveAllBtn, &QPushButton::clicked, this, [this, listWidget, &fileList, dialog] {
        for (int i = 0; i < listWidget->count(); i++) {
            if (!m_gcodeEditor->saveFile(fileList.at(i))) {
                QMessageBox::information(this, i18n("Save Failed"), i18n("Failed to save file: %1", fileList.at(i).toLocalFile()));
                dialog->reject();
            } else {
                listWidget->item(i)->setText(listWidget->item(i)->text().remove(u" [*]"_s));
            }
        }
        dialog->accept();
    });
    hLayout->addWidget(saveAllBtn);

    auto cancelBtn = new QPushButton(QIcon::fromTheme(u"dialog-cancel"_s, QIcon(QStringLiteral(":/%1/cancel").arg(m_theme))), i18n("Cancel"), dialog);
    cancelBtn->setIconSize(iconSize);
    cancelBtn->setDefault(true);
    connect(cancelBtn, &QPushButton::clicked, this, [dialog] {
        dialog->reject();
    });
    hLayout->addWidget(cancelBtn);

    auto ignoreBtn = new QPushButton(QIcon::fromTheme(u"edit-delete"_s, style()->standardIcon(QStyle::SP_TrashIcon)), i18n("Discard Changes"), dialog);
    ignoreBtn->setIconSize(iconSize);
    connect(ignoreBtn, &QPushButton::clicked, this, [dialog] {
        dialog->accept();
    });
    hLayout->addWidget(ignoreBtn);

    auto layout = new QVBoxLayout;
    auto label = new QLabel(i18n("Files with Unsaved Changes."), dialog);
    layout->addWidget(label);
    layout->addWidget(listWidget);
    layout->addItem(hLayout);
    dialog->setLayout(layout);

    return dialog->exec();
}

void MainWindow::updateBedSize(const QSize &newSize)
{
    m_lateral.get<Viewer3D>(u"3d"_s)->setBedSize(newSize);
}
