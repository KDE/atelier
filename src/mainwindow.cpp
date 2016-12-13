/* Atelier KDE Printer Host for 3D Printing
    Copyright (C) <2016>
    Author: Lays Rodrigues - laysrodrigues@gmail.com

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

MainWindow::MainWindow(QWidget *parent) :
    KXmlGuiWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->gcodeEditorWidget->setVisible(false);
    setupActions();
    initConnectsToAtCore();
    initLocalVariables();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initConnectsToAtCore()
{
    connect(ui->rightWidget, &TemporaryPrinterControlWidget::homeAll, this, [=]{
        core.home();
    });
    connect(ui->rightWidget, &TemporaryPrinterControlWidget::home, this, [=]{
        core.home(AXIS::X);
    });
    connect(ui->rightWidget, &TemporaryPrinterControlWidget::home, this, [=]{
        core.home(AXIS::Y);
    });
    connect(ui->rightWidget, &TemporaryPrinterControlWidget::home, this, [=]{
        core.home(AXIS::Z);
    });
    connect(ui->rightWidget, &TemporaryPrinterControlWidget::moveAxis, &core, &AtCore::move);
    //TODO Handle more then one fan - Maybe when the final controls are finished?
    connect(ui->rightWidget, &TemporaryPrinterControlWidget::changeFanSpeed, this, [=](const int value){
        core.setFanSpeed(value);
    });
    connect(ui->rightWidget, &TemporaryPrinterControlWidget::changePrintSpeed, &core, &AtCore::setPrinterSpeed);
    connect(ui->rightWidget, &TemporaryPrinterControlWidget::setHeatBed, &core, &AtCore::setBedTemp);
    connect(ui->rightWidget, &TemporaryPrinterControlWidget::setHeatExtruder, &core, &AtCore::setExtruderTemp);
}

void MainWindow::initLocalVariables()
{
    firmwaresList = core.availablePlugins();
}

void MainWindow::setupActions()
{
    //Toolbar Actions
    QAction *action;
    action = actionCollection()->addAction(QStringLiteral("open_gcode"));
    action->setIcon(QIcon::fromTheme("open"));
    action->setText(i18n("&Open GCode"));
    connect(action, &QAction::triggered, this, &MainWindow::openFile);

    action = actionCollection()->addAction(QStringLiteral("connect"));
    action->setText(i18n("&Connect"));
    connect(action, &QAction::triggered, this, &MainWindow::startConnection);

    action = actionCollection()->addAction(QStringLiteral("print"));
    action->setText(i18n("&Print"));
    connect(action, &QAction::triggered, this, &MainWindow::printFile);

    action = actionCollection()->addAction(QStringLiteral("pause"));
    action->setText(i18n("&Pause"));
    connect(action, &QAction::triggered, this, &MainWindow::pausePrint);

    action = actionCollection()->addAction(QStringLiteral("stop"));
    action->setText(i18n("&Stop"));
    connect(action, &QAction::triggered, this, &MainWindow::stopPrint);

    action = actionCollection()->addAction(QStringLiteral("edit_gcode"));
    action->setText(i18n("&Edit GCode"));
    connect(action, &QAction::triggered, this, [ = ] {
        ui->gcodeEditorWidget->setVisible(!ui->gcodeEditorWidget->isVisible());
    });

    QAction *quit = KStandardAction::quit(qApp, SLOT(quit()), actionCollection());

    setupGUI(Default, "atelierui.rc");
}

void MainWindow::openFile()
{
    QUrl fileNameFromDialog = QFileDialog::getOpenFileUrl(this, i18n("Open GCode"),
                                                          QDir::homePath(), i18n("GCode (*.gco *.gcode)"));
    if (!fileNameFromDialog.isEmpty()) {
        fileName = fileNameFromDialog.toLocalFile();
    }
}

void MainWindow::startConnection()
{
   auto *dialog = new ConnectSettingsDialog(firmwaresList);
   connect(dialog, &ConnectSettingsDialog::_connect, &core, &AtCore::initFirmware);
}

void MainWindow::printFile()
{
    if(!fileName.isEmpty() && (core.state() == PrinterState::IDLE)) {
        core.print(fileName);
    }
}

void MainWindow::pausePrint()
{
    core.pause(QString());
}

void MainWindow::stopPrint()
{
    core.stop();
}
