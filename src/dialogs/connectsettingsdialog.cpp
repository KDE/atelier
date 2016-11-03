#include "connectsettingsdialog.h"
#include "ui_connectsettingsdialog.h"
#include <QSerialPortInfo>

ConnectSettingsDialog::ConnectSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectSettingsDialog),
    deviceNotifier(Solid::DeviceNotifier::instance())
{
    ui->setupUi(this);
    connect(deviceNotifier, &Solid::DeviceNotifier::deviceAdded, this, &ConnectSettingsDialog::locateSerialPort);
    connect(deviceNotifier, &Solid::DeviceNotifier::deviceRemoved, this, &ConnectSettingsDialog::locateSerialPort);
}

ConnectSettingsDialog::~ConnectSettingsDialog()
{
    delete ui;
}

void ConnectSettingsDialog::locateSerialPort()
{
    QStringList ports;
    QList<QSerialPortInfo> serialPortInfoList = QSerialPortInfo::availablePorts();
    if (!serialPortInfoList.isEmpty()) {
        foreach (const QSerialPortInfo &serialPortInfo, serialPortInfoList) {
            ports.append(QStringLiteral("/dev/") + serialPortInfo.portName());
        }
        if (ports == serialPortList) {
            return;
        } else {
            serialPortList.clear();
            serialPortList = ports;
            ui->serialPortCB->clear();
            ui->serialPortCB->addItems(serialPortList);
        }
    } else {
        serialPortList.clear();
        ui->serialPortCB->clear();
    }
}
