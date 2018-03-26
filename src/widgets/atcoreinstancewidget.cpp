/* Atelier KDE Printer Host for 3D Printing
    Copyright (C) <2017>
    Author: Lays Rodrigues - laysrodriguessilva@gmail.com

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

#include "atcoreinstancewidget.h"
#include "ui_atcoreinstancewidget.h"
#include <QToolBar>
#include <AtCore/SerialLayer>
#include <AtCore/GCodeCommands>
#include <KLocalizedString>
#include "choosefiledialog.h"

AtCoreInstanceWidget::AtCoreInstanceWidget(QWidget *parent):
    QWidget(parent),
    m_mainToolBar(nullptr),
    m_toolBar(nullptr),
    m_printAction(nullptr)
{
    ui = new Ui::AtCoreInstanceWidget;
    ui->setupUi(this);

    ui->printProgressWidget->setVisible(false);
    buildMainToolbar();
    buildToolbar();
    enableControls(false);
    initConnectsToAtCore();
}

AtCoreInstanceWidget::~AtCoreInstanceWidget()
{
    delete ui;
}

void AtCoreInstanceWidget::buildToolbar()
{
    m_toolBar = new QToolBar();

    auto lb = new QAction;
    lb->setIcon(style()->standardIcon(QStyle::SP_DirHomeIcon));
    lb->setDisabled(true);
    m_toolBar->addAction(lb);

    auto homeAll = new QAction("All");
    connect(homeAll, &QAction::triggered, [this]{
       m_core.home();
    });
    m_toolBar->addAction(homeAll);

    for(auto homes : std::map<QString, int>{{"X", AtCore::X}, {"Y", AtCore::Y}, {"Z", AtCore::Z}}) {
        auto home = new QAction(homes.first);
        connect(home, &QAction::triggered, [this, homes] {
            m_core.home(homes.second);
        });
        m_toolBar->addAction(home);
    }

    m_toolBar->addSeparator();

    auto *axis = new QAction("Axis");
    axis->setCheckable(true);
    axis->setChecked(true);
    connect(axis, &QAction::toggled, ui->axisViewWidget, &AxisControl::setVisible);

    auto controls = new QAction("Controls");
    controls->setCheckable(true);
    controls->setChecked(true);
    connect(controls, &QAction::toggled, ui->bedExtWidget, &BedExtruderWidget::setVisible);

    auto plot = new QAction("Temperature Plot");
    plot->setCheckable(true);
    plot->setChecked(true);
    connect(plot, &QAction::toggled, ui->plotWidget, &PlotWidget::setVisible);

    m_toolBar->addAction(axis);
    m_toolBar->addAction(controls);
    m_toolBar->addAction(plot);
    ui->toolBarLayout->addWidget(m_toolBar);
    ui->toolBarLayout->addStretch();
}

void AtCoreInstanceWidget::buildMainToolbar(){
    m_mainToolBar = new QToolBar();
    m_mainToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    auto disconnectAction = new QAction(style()->standardIcon(QStyle::SP_DialogCloseButton), i18n("Disconnect"));
    connect(this, &AtCoreInstanceWidget::disableDisconnect, disconnectAction, &QAction::setDisabled);
    connect(disconnectAction, &QAction::triggered, [this](){
        m_core.closeConnection();
    });
    m_mainToolBar->addAction(disconnectAction);

    m_printAction = new QAction(style()->standardIcon(QStyle::SP_MediaPlay),i18n("Print"));
    connect(m_printAction, &QAction::triggered, [ this ](){
        if(m_core.state() == AtCore::BUSY) {
            pausePrint();
            return;
        }
        if (m_core.state() == AtCore::IDLE){
            print();
        } else if (m_core.state() == AtCore::PAUSE) {
            m_core.resume();
        }
    });
    m_mainToolBar->addAction(m_printAction);

    auto stopAction = new QAction(style()->standardIcon(QStyle::SP_MediaStop),i18n("Stop"));
    connect(stopAction, &QAction::triggered, this, &AtCoreInstanceWidget::stopPrint);
    connect(stopAction, &QAction::triggered, [this](){
        m_printAction->setText(i18n("Print"));
        m_printAction->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    });
    m_mainToolBar->addAction(stopAction);

    auto disableMotorsAction = new QAction(style()->standardIcon(QStyle::SP_MediaStop),i18n("Disable Motors"));
    connect(disableMotorsAction, &QAction::triggered, this, &AtCoreInstanceWidget::disableMotors);
    m_mainToolBar->addAction(disableMotorsAction);

    ui->mainToolBarLayout->addWidget(m_mainToolBar);
    ui->mainToolBarLayout->addStretch();
}
void AtCoreInstanceWidget::startConnection(const QString& serialPort, const QMap<QString, QVariant>& profiles){
    m_core.initSerial(serialPort, profiles["bps"].toInt());
    if(m_core.state() == AtCore::CONNECTING){
        QString fw = profiles["firmware"].toString();
        if( fw != QString("Auto-Detect")){
            m_core.loadFirmwarePlugin(fw);
        }
    }
}

void AtCoreInstanceWidget::initConnectsToAtCore()
{
    // Handle AtCore status change
    connect(&m_core, &AtCore::stateChanged, this, &AtCoreInstanceWidget::handlePrinterStatusChanged);

    // If the number of extruders from the printer change, we need to update the radiobuttons on the widget
    connect(this, &AtCoreInstanceWidget::extruderCountChanged, ui->bedExtWidget, &BedExtruderWidget::setExtruderCount);

    // Bed and Extruder temperatures management
    connect(ui->bedExtWidget, &BedExtruderWidget::bedTemperatureChanged, &m_core, &AtCore::setBedTemp);
    connect(ui->bedExtWidget, &BedExtruderWidget::extTemperatureChanged, &m_core, &AtCore::setExtruderTemp);

    // Connect AtCore temperatures changes on Atelier Plot
    connect(&m_core.temperature(), &Temperature::bedTemperatureChanged, [ this ](const float& temp) {
        checkTemperature(0x00, 0, temp);
        ui->plotWidget->appendPoint(i18n("Actual Bed"), temp);
        ui->plotWidget->update();
        ui->bedExtWidget->updateBedTemp(temp);
    });
    connect(&m_core.temperature(), &Temperature::bedTargetTemperatureChanged, [ this ](const float& temp) {
        checkTemperature(0x01, 0, temp);
        ui->plotWidget->appendPoint(i18n("Target Bed"), temp);
        ui->plotWidget->update();
        ui->bedExtWidget->updateBedTargetTemp(temp);
    });
    connect(&m_core.temperature(), &Temperature::extruderTemperatureChanged, [ this ](const float& temp) {
        checkTemperature(0x02, 0, temp);
        ui->plotWidget->appendPoint(i18n("Actual Ext.1"), temp);
        ui->plotWidget->update();
        ui->bedExtWidget->updateExtTemp(temp);
    });
    connect(&m_core.temperature(), &Temperature::extruderTargetTemperatureChanged, [ this ](const float& temp) {
        checkTemperature(0x03, 0, temp);
        ui->plotWidget->appendPoint(i18n("Target Ext.1"), temp);
        ui->plotWidget->update();
        ui->bedExtWidget->updateExtTargetTemp(temp);
    });

    connect(ui->pushGCodeWidget, &PushGCodeWidget::push, [ this ](QString command) {
        ui->logWidget->addLog("Push " + command);
        m_core.pushCommand(command);
    });

    // Fan, Flow and Speed management
    connect(ui->ratesControlWidget, &RatesControlWidget::fanSpeedChanged, &m_core, &AtCore::setFanSpeed);
    connect(ui->ratesControlWidget, &RatesControlWidget::flowRateChanged, &m_core, &AtCore::setFlowRate);
    connect(ui->ratesControlWidget, &RatesControlWidget::printSpeedChanged, &m_core, &AtCore::setPrinterSpeed);
    connect(ui->axisViewWidget, &AxisControl::clicked, this, &AtCoreInstanceWidget::axisControlClicked);
}

void AtCoreInstanceWidget::printFile(const QUrl& fileName)
{
    if (!fileName.isEmpty() && (m_core.state() == AtCore::IDLE)) {
        m_core.print(fileName.toLocalFile());
    }
}

void AtCoreInstanceWidget::print(){
    switch (m_files.size()){
        case 0:
            QMessageBox::warning(this, i18n("Error"),
                                 i18n("There's no GCode File open. \n Please select a file and try again."),
                                 QMessageBox::Ok);
            break;
        case 1:
            printFile(m_files.at(0));
            break;
        default:
            auto dialog = new ChooseFileDialog(this, m_files);
            if(dialog->exec() == QDialog::Accepted){
                    printFile(dialog->choosenFile());
            }
    }
}
void AtCoreInstanceWidget::pausePrint()
{
    if(m_core.state() == AtCore::BUSY) {
        m_core.pause(profileData["postPause"].toString());
    } else if (m_core.state() == AtCore::PAUSE) {
        m_core.resume();
    }
}

void AtCoreInstanceWidget::stopPrint()
{
    m_core.stop();
}

void AtCoreInstanceWidget::disableMotors()
{
    m_core.disableMotors(0);
}

void AtCoreInstanceWidget::handlePrinterStatusChanged(AtCore::STATES newState)
{
    static QString stateString;
    switch (newState) {
        case AtCore::CONNECTING: {
            stateString = i18n("Connecting...");
            connect(&m_core, &AtCore::receivedMessage, this, &AtCoreInstanceWidget::checkReceivedCommand);
            connect(m_core.serial(), &SerialLayer::pushedCommand, this, &AtCoreInstanceWidget::checkPushedCommands);
        } break;
        case AtCore::IDLE: {
            stateString = i18n("Connected to ") + m_core.serial()->portName();
            emit extruderCountChanged(m_core.extruderCount());
            ui->logWidget->addLog(i18n("Serial connected"));
            emit disableDisconnect(false);
            enableControls(true);
        } break;
        case AtCore::DISCONNECTED: {
            stateString = i18n("Not Connected");
            disconnect(&m_core, &AtCore::receivedMessage, this, &AtCoreInstanceWidget::checkReceivedCommand);
            disconnect(m_core.serial(), &SerialLayer::pushedCommand, this, &AtCoreInstanceWidget::checkPushedCommands);
            ui->logWidget->addLog(i18n("Serial disconnected"));
            enableControls(false);
        } break;
        case AtCore::STARTPRINT: {
            stateString = i18n("Starting Print");
            ui->printProgressWidget->setVisible(true);
            connect(&m_core, &AtCore::printProgressChanged, ui->printProgressWidget, &PrintProgressWidget::updateProgressBar);
        } break;
        case AtCore::FINISHEDPRINT: {
            stateString = i18n("Finished Print");
            ui->printProgressWidget->setVisible(false);
            disconnect(&m_core, &AtCore::printProgressChanged, ui->printProgressWidget, &PrintProgressWidget::updateProgressBar);
            m_printAction->setText(i18n("Print"));
            m_printAction->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        } break;
        case AtCore::BUSY: {
            stateString = i18n("Printing");
            emit disableDisconnect(true);
            m_printAction->setText(i18n("Pause"));
            m_printAction->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        } break;
        case AtCore::PAUSE: {
            stateString = i18n("Paused");
            m_printAction->setText(i18n("Resume"));
            m_printAction->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        } break;
        case AtCore::STOP: {
            stateString = i18n("Stoping Print");
        } break;
        case AtCore::ERRORSTATE: {
            stateString = i18n("Error");
        } break;
        default:
          qWarning("AtCore State not Recognized.");
          break;
    }
     ui->lblState->setText(stateString);
}

void AtCoreInstanceWidget::checkTemperature(uint sensorType, uint number, uint temp)
{
    static QString msg;
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

    ui->logWidget->addRLog(msg);
}

void AtCoreInstanceWidget::checkReceivedCommand(const QByteArray &message)
{
    ui->logWidget->addRLog(QString::fromUtf8(message));
}

void AtCoreInstanceWidget::checkPushedCommands(const QByteArray &bmsg)
{
    QString msg = QString::fromUtf8(bmsg);
    QRegExp _newLine(QChar::fromLatin1('\n'));
    QRegExp _return(QChar::fromLatin1('\r'));
    msg.replace(_newLine, QStringLiteral("\\n"));
    msg.replace(_return, QStringLiteral("\\r"));
    ui->logWidget->addSLog(msg);
}

void AtCoreInstanceWidget::axisControlClicked(QChar axis, int value)
{
  m_core.setRelativePosition();
  m_core.pushCommand(GCode::toCommand(GCode::G1, QStringLiteral("%1%2").arg(axis, QString::number(value))));
  m_core.setAbsolutePosition();
}

void AtCoreInstanceWidget::enableControls(bool b)
{
    ui->mainTab->setEnabled(b);
    m_mainToolBar->setEnabled(b);
    m_toolBar->setEnabled(b);
}

bool AtCoreInstanceWidget::connected()
{
    return (m_core.state() != AtCore::DISCONNECTED);
}

void AtCoreInstanceWidget::setOpenFiles(const QList<QUrl>& files)
{
    m_files = files;
}
