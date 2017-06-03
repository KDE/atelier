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
#include <KXMLGUIFactory>
#include <KAtCore/SerialLayer>
#include <KAtCore/GCodeCommands>

MainWindow::MainWindow(QWidget *parent) :
    KXmlGuiWindow(parent),
    ui(new Ui::MainWindow),
    generalSettingsDialog(new GeneralSettingsDialog(this)),
    connectSettingsDialog(new ConnectSettingsDialog(this)),
    logDialog(new LogDialog(this))
{
    ui->setupUi(this);
    setupActions();
    initConnectsToAtCore();
    initWidgets();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initConnectsToAtCore()
{
    // Start Connection to the 3DPrinter
    connect(connectSettingsDialog, &ConnectSettingsDialog::startConnection, [ = ](QString port, QMap<QString, QVariant> data) {
        core.initSerial(port, data["bps"].toInt());
    });

    // Handle AtCore status change
    connect(&core, &AtCore::stateChanged, this, &MainWindow::handlePrinterStatusChanged);

    // If the number of extruders from the printer change, we need to update the radiobuttons on the widget
    connect(this, &MainWindow::extruderCountChanged, ui->bedExtWidget, &BedExtruderWidget::setExtruderCount);

    // Bed and Extruder temperatures management
    connect(ui->bedExtWidget, &BedExtruderWidget::bedTemperatureChanged, &core, &AtCore::setBedTemp);
    connect(ui->bedExtWidget, &BedExtruderWidget::extTemperatureChanged, &core, &AtCore::setExtruderTemp);

    // Connect AtCore temperatures changes on Atelier Plot
    connect(&core.temperature(), &Temperature::bedTemperatureChanged, [ = ](float temp) {
        checkTemperature(0x00, 0, temp);
        ui->plotWidget->appendPoint(i18n("Actual Bed"), temp);
        ui->plotWidget->update();
        ui->bedExtWidget->updateBedTemp(temp);
    });
    connect(&core.temperature(), &Temperature::bedTargetTemperatureChanged, [ = ](float temp) {
        checkTemperature(0x01, 0, temp);
        ui->plotWidget->appendPoint(i18n("Target Bed"), temp);
        ui->plotWidget->update();
        ui->bedExtWidget->updateBedTargetTemp(temp);
    });
    connect(&core.temperature(), &Temperature::extruderTemperatureChanged, [ = ](float temp) {
        checkTemperature(0x02, 0, temp);
        ui->plotWidget->appendPoint(i18n("Actual Ext.1"), temp);
        ui->plotWidget->update();
        ui->bedExtWidget->updateExtTemp(temp);
    });
    connect(&core.temperature(), &Temperature::extruderTargetTemperatureChanged, [ = ](float temp) {
        checkTemperature(0x03, 0, temp);
        ui->plotWidget->appendPoint(i18n("Target Ext.1"), temp);
        ui->plotWidget->update();
        ui->bedExtWidget->updateExtTargetTemp(temp);
    });

    connect(ui->pushGCodeWidget, &PushGCodeWidget::push, [ = ](QString command) {
        logDialog->addLog("Push " + command);
        core.pushCommand(command);
    });

    // Fan, Flow and Speed management
    connect(ui->ratesControlWidget, &RatesControlWidget::fanSpeedChanged, &core, &AtCore::setFanSpeed);
    connect(ui->ratesControlWidget, &RatesControlWidget::flowRateChanged, &core, &AtCore::setFlowRate);
    connect(ui->ratesControlWidget, &RatesControlWidget::printSpeedChanged, &core, &AtCore::setPrinterSpeed);

}

void MainWindow::initWidgets()
{
    // Disable Toolbox to prevent the user to mess up without a printer being connected
    ui->toolboxTabWidget->setDisabled(true);

    // This dock is of Printing Progress. It only need to be show while printing
    ui->printProgressDockWidget->setVisible(false);

    // When a new profile is added on the Profile Dialog it needs to update the profiles on connection dialog
    connect(generalSettingsDialog, &GeneralSettingsDialog::updateProfiles,
            connectSettingsDialog, &ConnectSettingsDialog::updateProfiles);

    connectSettingsDialog->setFirmwareList(core.availablePlugins());
    generalSettingsDialog->setBaudRates(core.serial()->validBaudRates());
}

void MainWindow::setupActions()
{
    // Actions for the Toolbar
    QAction *action;
    action = actionCollection()->addAction(QStringLiteral("open_gcode"));
    action->setIcon(QIcon::fromTheme("open"));
    action->setText(i18n("&Open GCode"));
    connect(action, &QAction::triggered, this, &MainWindow::openFile);

    _connect = actionCollection()->addAction(QStringLiteral("connect"));
    _connect->setText(i18n("&Connect"));
    _connect->setCheckable(true);
    connect(_connect, &QAction::toggled, [ = ](bool checked) {
        if (checked) {
            connectSettingsDialog->show();
        } else {
            core.closeConnection();
            _connect->setText(i18n("&Connect"));
            ui->bedExtWidget->stopHeating();
            core.setState(PrinterState::DISCONNECTED);
        }
    });

    connect(connectSettingsDialog, &ConnectSettingsDialog::setConnectValue, _connect, &QAction::setChecked);

    action = actionCollection()->addAction(QStringLiteral("settings"));
    action->setText(i18n("&Settings"));
    connect(action, &QAction::triggered, generalSettingsDialog, &GeneralSettingsDialog::show);

    action = actionCollection()->addAction(QStringLiteral("print"));
    action->setText(i18n("&Print"));
    connect(action, &QAction::triggered, this, &MainWindow::printFile);

    action = actionCollection()->addAction(QStringLiteral("pause"));
    action->setText(i18n("&Pause"));
    connect(action, &QAction::triggered, this, &MainWindow::pausePrint);

    action = actionCollection()->addAction(QStringLiteral("stop"));
    action->setText(i18n("&Stop"));
    connect(action, &QAction::triggered, this, &MainWindow::stopPrint);

    // Actions for the Docks

    action = actionCollection()->addAction(QStringLiteral("gcode"), ui->gcodeDockWidget->toggleViewAction());
    action->setText(i18n("&GCode"));
    /*
     * Known BUG:
     * If you try to move the dock, the else on this connect
     * will enter in infinite loop until the program
     * crashes.
     * I have no idea why this happens. =/
    */
    connect(action, &QAction::toggled, [ = ](bool status) {
        if (status) {
            guiFactory()->addClient(ui->gcodeEditorWidget->gcodeView());
        } else {
            guiFactory()->removeClient(ui->gcodeEditorWidget->gcodeView());
        }
    });

    action = KStandardAction::quit(qApp, SLOT(quit()), actionCollection());

    // Plot
    action = actionCollection()->addAction(QStringLiteral("plot"), ui->plotDockWidget->toggleViewAction());
    action->setText(i18n("Temperature Timeline"));

    // Toolbox
    action = actionCollection()->addAction(QStringLiteral("toolbox"), ui->toolboxDockWidget->toggleViewAction());
    action->setText(i18n("Toolbox"));

    // Actions for the Dialogs
    action = actionCollection()->addAction(QStringLiteral("log"));
    action->setText(i18n("Log Dialog"));
    connect(action, &QAction::triggered, logDialog, &LogDialog::show);

    setupGUI(Default, "atelierui.rc");
}

void MainWindow::openFile()
{
    QUrl fileNameFromDialog = QFileDialog::getOpenFileUrl(this, i18n("Open GCode"),
                              QDir::homePath(), i18n("GCode (*.gco *.gcode)"));
    if (!fileNameFromDialog.isEmpty()) {
        fileName = fileNameFromDialog;
        ui->gcodeEditorWidget->loadFile(fileName);
        guiFactory()->addClient(ui->gcodeEditorWidget->gcodeView());
    }
}

void MainWindow::printFile()
{
    if (!fileName.isEmpty() && (core.state() == PrinterState::IDLE)) {
        QString f = fileName.toLocalFile();
        core.print(f);
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

void MainWindow::handlePrinterStatusChanged(PrinterState newState)
{
    switch (newState) {
    case PrinterState::CONNECTING: {
        connect(core.serial(), &SerialLayer::receivedCommand, this, &MainWindow::checkReceivedCommand);
        connect(core.serial(), &SerialLayer::pushedCommand, this, &MainWindow::checkPushedCommands);
    } break;
    case PrinterState::IDLE: {
        ui->toolboxTabWidget->setEnabled(true);
        emit extruderCountChanged(core.extruderCount());
        logDialog->addLog(i18n("Serial connected"));
        _connect->setText(i18n("&Disconnect"));
    } break;
    case PrinterState::DISCONNECTED: {
        ui->toolboxTabWidget->setEnabled(false);
        disconnect(core.serial(), &SerialLayer::receivedCommand, this, &MainWindow::checkReceivedCommand);
        disconnect(core.serial(), &SerialLayer::pushedCommand, this, &MainWindow::checkPushedCommands);
        logDialog->addLog(i18n("Serial disconnected"));

    } break;
    case PrinterState::STARTPRINT: {
        ui->printProgressDockWidget->setVisible(true);
        connect(&core, &AtCore::printProgressChanged, ui->printProgressWidget, &PrintProgressWidget::updateProgressBar);
    } break;
    case PrinterState::FINISHEDPRINT: {
        ui->printProgressDockWidget->setVisible(false);
        disconnect(&core, &AtCore::printProgressChanged, ui->printProgressWidget, &PrintProgressWidget::updateProgressBar);
    } break;

    default:
        return;
    }
}

void MainWindow::checkTemperature(uint sensorType, uint number, uint temp)
{
    QString msg;
    switch (sensorType) {
    case 0x00: // bed
        msg = QString::fromLatin1("Bed Temperature ");
        break;

    case 0x01: // bed target
        msg = QString::fromLatin1("Bed Target Temperature ");
        break;

    case 0x02: // extruder
        msg = QString::fromLatin1("Extruder Temperature ");
        break;

    case 0x03: // extruder target
        msg = QString::fromLatin1("Extruder Target Temperature ");
        break;

    case 0x04: // enclosure
        msg = QString::fromLatin1("Enclosure Temperature ");
        break;

    case 0x05: // enclosure target
        msg = QString::fromLatin1("Enclosure Target Temperature ");
        break;
    }

    msg.append(QString::fromLatin1("[%1] : %2"));
    msg = msg.arg(QString::number(number))
          .arg(QString::number(temp));

    logDialog->addRLog(msg);
}

void MainWindow::checkReceivedCommand()
{
    logDialog->addRLog(QString::fromUtf8(core.popCommand()));
}

void MainWindow::checkPushedCommands(QByteArray bmsg)
{
    QString msg = QString::fromUtf8(bmsg);
    QRegExp _newLine(QChar::fromLatin1('\n'));
    QRegExp _return(QChar::fromLatin1('\r'));
    msg.replace(_newLine, QStringLiteral("\\n"));
    msg.replace(_return, QStringLiteral("\\r"));
    logDialog->addSLog(msg);
}
