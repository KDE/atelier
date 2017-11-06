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
#include <AtCore/SerialLayer>
#include <AtCore/GCodeCommands>

MainWindow::MainWindow(QWidget *parent) :
    KXmlGuiWindow(parent),
    ui(new Ui::MainWindow),
    profilesDialog(new ProfilesDialog(this)),
    connectSettingsDialog(new ConnectSettingsDialog(this))
{
    ui->setupUi(this);
    logWidget = new LogWidget;
    setupActions();
    initConnectsToAtCore();
    initWidgets();
    setCentralWidget(nullptr);
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
        logWidget->addLog("Push " + command);
        core.pushCommand(command);
    });

    // Fan, Flow and Speed management
    connect(ui->ratesControlWidget, &RatesControlWidget::fanSpeedChanged, &core, &AtCore::setFanSpeed);
    connect(ui->ratesControlWidget, &RatesControlWidget::flowRateChanged, &core, &AtCore::setFlowRate);
    connect(ui->ratesControlWidget, &RatesControlWidget::printSpeedChanged, &core, &AtCore::setPrinterSpeed);
    connect(ui->axisViewWidget, &AxisControl::clicked, this, &MainWindow::axisControlClicked);

}

void MainWindow::initWidgets()
{
    // Disable Toolbox to prevent the user to mess up without a printer being connected
    ui->controlDockWidget->setDisabled(true);
    ui->axisDockWidget->setDisabled(true);

    // This dock is of Printing Progress. It only need to be show while printing
    ui->printProgressDockWidget->setVisible(false);

    ui->logDockWidget->setWidget(logWidget);
    ui->statusBar->addWidget(ui->statusBarWidget);

    // When a new profile is added on the Profile Dialog it needs to update the profiles on connection dialog
    connect(profilesDialog, &ProfilesDialog::updateProfiles,
            connectSettingsDialog, &ConnectSettingsDialog::updateProfiles);

    connectSettingsDialog->setFirmwareList(core.availableFirmwarePlugins());
    profilesDialog->setBaudRates(core.serial()->validBaudRates());

    ui->homeAllPB->setIcon(style()->standardIcon(QStyle::SP_DirHomeIcon));
    ui->homeXPB->setIcon(style()->standardIcon(QStyle::SP_DirHomeIcon));
    ui->homeYPB->setIcon(style()->standardIcon(QStyle::SP_DirHomeIcon));
    ui->homeZPB->setIcon(style()->standardIcon(QStyle::SP_DirHomeIcon));

    tabifyDockWidget(ui->axisDockWidget, ui->controlDockWidget);
    tabifyDockWidget(ui->view3DdockWidget, ui->gcodeDockWidget);
    tabifyDockWidget(ui->view3DdockWidget, ui->videoDockWidget);
}

void MainWindow::setupActions()
{
    // Actions for the Toolbar
    QAction *action;
    action = actionCollection()->addAction(QStringLiteral("open_gcode"));
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
            core.setState(AtCore::DISCONNECTED);
        }
    });

    connect(connectSettingsDialog, &ConnectSettingsDialog::setConnectValue, _connect, &QAction::setChecked);

    connect(ui->homeAllPB, &QPushButton::clicked, [=]{
        core.home();
    });

    connect(ui->homeXPB, &QPushButton::clicked, [=]{
       core.home(AtCore::X);
    });

    connect(ui->homeYPB, &QPushButton::clicked, [=]{
       core.home(AtCore::Y);
    });

    connect(ui->homeZPB, &QPushButton::clicked, [=]{
       core.home(AtCore::Z);
    });

    action = actionCollection()->addAction(QStringLiteral("profiles"));
    action->setText(i18n("&Profiles"));
    connect(action, &QAction::triggered, profilesDialog, &ProfilesDialog::show);

    action = actionCollection()->addAction(QStringLiteral("print"));
    action->setText(i18n("&Print"));
    connect(action, &QAction::triggered, this, &MainWindow::printFile);

    action = actionCollection()->addAction(QStringLiteral("pause"));
    action->setText(i18n("&Pause"));
    connect(action, &QAction::triggered, this, &MainWindow::pausePrint);

    action = actionCollection()->addAction(QStringLiteral("stop"));
    action->setText(i18n("&Stop"));
    connect(action, &QAction::triggered, this, &MainWindow::stopPrint);

    #ifdef Q_OS_LINUX
    //only set icons from theme on linux
        actionCollection()->action(QStringLiteral("profiles"))->setIcon(QIcon::fromTheme("emblem-favorite"));
    #endif
    //use style's standardIcon for the icons we can.
    actionCollection()->action(QStringLiteral("open_gcode"))->setIcon(style()->standardIcon(QStyle::SP_DirOpenIcon));
    actionCollection()->action(QStringLiteral("print"))->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    actionCollection()->action(QStringLiteral("pause"))->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    actionCollection()->action(QStringLiteral("stop"))->setIcon(style()->standardIcon(QStyle::SP_MediaStop));

    // Actions for the Docks
    action = actionCollection()->addAction(QStringLiteral("dock_labels"));
    action->setText(i18n("&Show Dock Labels"));
    action->setCheckable(true); 
    action->setChecked(true);
    connect(action, &QAction::triggered, [=](bool checked){
        toggleDockTitles(checked);
    });

    action = actionCollection()->addAction(QStringLiteral("3d"), ui->view3DdockWidget->toggleViewAction());
    action->setText(i18n("&3DView"));

    action = actionCollection()->addAction(QStringLiteral("gcode"), ui->gcodeDockWidget->toggleViewAction());
    action->setText(i18n("&GCode"));

    action = KStandardAction::quit(qApp, SLOT(quit()), actionCollection());

    // Plot
    action = actionCollection()->addAction(QStringLiteral("plot"), ui->plotDockWidget->toggleViewAction());
    action->setText(i18n("Temperature Timeline"));

    action = actionCollection()->addAction(QStringLiteral("controller"), ui->controlDockWidget->toggleViewAction());
    action->setText(i18n("Controller"));

    action = actionCollection()->addAction(QStringLiteral("axis"), ui->axisDockWidget->toggleViewAction());
    action->setText(i18n("Axis"));
    
    action = actionCollection()->addAction(QStringLiteral("video"), ui->videoDockWidget->toggleViewAction());
    action->setText(i18n("Video Monitor"));
    
    action = actionCollection()->addAction(QStringLiteral("log"), ui->logDockWidget->toggleViewAction());
    action->setText(i18n("Log"));

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
        ui->view3DWidget->drawModel(fileName.toString());
    }
}

void MainWindow::printFile()
{
    if (!fileName.isEmpty() && (core.state() == AtCore::IDLE)) {
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

void MainWindow::handlePrinterStatusChanged(AtCore::STATES newState)
{
    QString stateString;
    switch (newState) {
    case AtCore::CONNECTING: {
        stateString = i18n("Connecting...");
        connect(&core, &AtCore::receivedMessage, this, &MainWindow::checkReceivedCommand);
        connect(core.serial(), &SerialLayer::pushedCommand, this, &MainWindow::checkPushedCommands);
    } break;
    case AtCore::IDLE: {
        stateString = i18n("Connected to ") + core.serial()->portName();
        ui->controlDockWidget->setEnabled(true);
        ui->axisDockWidget->setEnabled(true);
        emit extruderCountChanged(core.extruderCount());
        logWidget->addLog(i18n("Serial connected"));
        _connect->setText(i18n("&Disconnect"));
    } break;
    case AtCore::DISCONNECTED: {
        stateString = i18n("Not Connected");
        ui->controlDockWidget->setEnabled(false);
        ui->axisDockWidget->setEnabled(false);
        disconnect(&core, &AtCore::receivedMessage, this, &MainWindow::checkReceivedCommand);
        disconnect(core.serial(), &SerialLayer::pushedCommand, this, &MainWindow::checkPushedCommands);
        logWidget->addLog(i18n("Serial disconnected"));

    } break;
    case AtCore::STARTPRINT: {
        stateString = i18n("Starting Print");
        ui->printProgressDockWidget->setVisible(true);
        connect(&core, &AtCore::printProgressChanged, ui->printProgressWidget, &PrintProgressWidget::updateProgressBar);
    } break;
    case AtCore::FINISHEDPRINT: {
        stateString = i18n("Finished Print");
        ui->printProgressDockWidget->setVisible(false);
        disconnect(&core, &AtCore::printProgressChanged, ui->printProgressWidget, &PrintProgressWidget::updateProgressBar);
    } break;
    case AtCore::BUSY: {
        stateString = i18n("Printing");
    } break;
    case AtCore::PAUSE: {
        stateString = i18n("Paused");
    } break;
    case AtCore::STOP: {
        stateString = i18n("Stoping Print");
    } break;
    case AtCore::ERRORSTATE: {
        stateString = i18n("Error");
    } break;
    }
    ui->lblState->setText(stateString);
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

    logWidget->addRLog(msg);
}

void MainWindow::checkReceivedCommand(const QByteArray &message)
{
    logWidget->addRLog(QString::fromUtf8(message));
}

void MainWindow::checkPushedCommands(QByteArray bmsg)
{
    QString msg = QString::fromUtf8(bmsg);
    QRegExp _newLine(QChar::fromLatin1('\n'));
    QRegExp _return(QChar::fromLatin1('\r'));
    msg.replace(_newLine, QStringLiteral("\\n"));
    msg.replace(_return, QStringLiteral("\\r"));
    logWidget->addSLog(msg);
}

void MainWindow::axisControlClicked(QChar axis, int value)
{
    core.setRelativePosition();
    core.pushCommand(GCode::toCommand(GCode::G1, QStringLiteral("%1%2").arg(axis, QString::number(value))));
    core.setAbsolutePosition();
}

void MainWindow::toggleDockTitles(bool checked)
{
    if(checked){
        delete ui->axisDockWidget->titleBarWidget();
        delete ui->view3DdockWidget->titleBarWidget();
        delete ui->plotDockWidget->titleBarWidget();
        delete ui->logDockWidget->titleBarWidget();
        delete ui->controlDockWidget->titleBarWidget();
        delete ui->gcodeDockWidget->titleBarWidget();
    }else{
        ui->axisDockWidget->setTitleBarWidget(new QWidget());
        ui->view3DdockWidget->setTitleBarWidget(new QWidget());
        ui->plotDockWidget->setTitleBarWidget(new QWidget());
        ui->logDockWidget->setTitleBarWidget(new QWidget());
        ui->controlDockWidget->setTitleBarWidget(new QWidget());
        ui->gcodeDockWidget->setTitleBarWidget(new QWidget());
    }
}
