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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->gcodeEditorWidget->setVisible(false);

    connect(ui->actionGCode, &QAction::triggered, [=]{
        ui->gcodeEditorWidget->setVisible(true);
    });

    connect(ui->actionOpenGCode, &QAction::triggered, [=]{
        QString fileName = QFileDialog::getOpenFileName(this, i18n("Select a file to print"),
                                                        QDir::homePath(), i18n("GCode(*.gco *gcode)"));
        ui->gcodeEditorWidget->loadFile(fileName);
    });

    connect(ui->toolbarWidget, &ToolBarWidget::loadFile, ui->gcodeEditorWidget, &GCodeEditorWidget::loadFile);
    initConnectsToAtCore();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initConnectsToAtCore()
{
    connect(ui->toolbarWidget, &ToolBarWidget::_connect, &core, &AtCore::initFirmware);

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


    connect(ui->toolbarWidget, &ToolBarWidget::printFile, &core, &AtCore::print);
    connect(ui->toolbarWidget, &ToolBarWidget::pausePrint, this, [=]{
        core.pause(QString());
    });
    connect(ui->toolbarWidget, &ToolBarWidget::stopPrint, &core, &AtCore::stop);
    ui->toolbarWidget->setFirmwaresList(core.availablePlugins());
}
