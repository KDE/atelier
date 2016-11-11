#include "connectsettingsdialog.h"
#include "ui_connectsettingsdialog.h"
#include <QSerialPortInfo>

ConnectSettingsDialog::ConnectSettingsDialog(QStringList firmwaresList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectSettingsDialog),
    deviceNotifier(Solid::DeviceNotifier::instance())
{
    ui->setupUi(this);
    connect(deviceNotifier, &Solid::DeviceNotifier::deviceAdded, this, &ConnectSettingsDialog::locateSerialPort);
    connect(deviceNotifier, &Solid::DeviceNotifier::deviceRemoved, this, &ConnectSettingsDialog::locateSerialPort);

    connect(ui->buttonBox, &QDialogButtonBox::accepted,[=]{
        if(ui->profileCB->currentText().isEmpty()) {
            QMessageBox msg;
            msg.setText("Please, create a profile to connect on Settings!");
            msg.setIcon(QMessageBox::Information);
            msg.exec();
        } else if(!ui->serialPortCB->currentText().isEmpty()) {
            emit _connect(ui->serialPortCB->currentText(), ui->baudCB->currentText());
        } else {
            QMessageBox msg;
            msg.setText("Please, connect a serial device to continue!");
            msg.setIcon(QMessageBox::Information);
            msg.exec();
        }

    });
    initBaudRateComboBox();
    initFirmwareComboBox(firmwaresList);
    initProfileComboBox();
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

void ConnectSettingsDialog::initFirmwareComboBox(QStringList fw)
{
    fw.prepend("Auto-Detect");
    ui->firmwareCB->addItems(fw);
}

void ConnectSettingsDialog::initBaudRateComboBox()
{
    QStringList br;
    br.append("115200");
    br.append("250000");
    ui->baudCB->addItems(br);
}
