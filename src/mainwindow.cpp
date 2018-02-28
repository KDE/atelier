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
#include <memory>
MainWindow::MainWindow(QWidget *parent) :
    KXmlGuiWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupActions();
}

MainWindow::~MainWindow()
{
    delete ui;
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
    QUrl fileNameFromDialog = QFileDialog::getOpenFileUrl(this, i18n("Open GCode"),
                              QDir::homePath(), i18n("GCode (*.gco *.gcode)"));
    if (!fileNameFromDialog.isEmpty()) {
        m_fileName = fileNameFromDialog;
        ui->gcodeEditorWidget->loadFile(m_fileName);
        guiFactory()->addClient(ui->gcodeEditorWidget->gcodeView());
        ui->view3DWidget->drawModel(m_fileName.toString());
    }
}

void MainWindow::newConnection(const QString& port, const QMap<QString, QVariant>& profile)
{
    const int tabs = ui->tabWidget->count();
    if(tabs == 1){
        auto instance = qobject_cast<AtCoreInstanceWidget*>(ui->tabWidget->currentWidget());
        if(!instance->connected()){
            instance->startConnection(port, profile);
            return;
        }
    }
    auto newInstance = new AtCoreInstanceWidget();
    ui->tabWidget->addTab(newInstance, QString::number(tabs+1));
    newInstance->startConnection(port, profile);
}
