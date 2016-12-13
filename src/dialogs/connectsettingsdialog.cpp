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
#include "connectsettingsdialog.h"
#include "ui_connectsettingsdialog.h"
#include <QSerialPortInfo>
#include <QMessageBox>
#include <KLocalizedString>

ConnectSettingsDialog::ConnectSettingsDialog(QStringList firmwaresList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectSettingsDialog),
    deviceNotifier(Solid::DeviceNotifier::instance())
{
    ui->setupUi(this);
    connect(deviceNotifier, &Solid::DeviceNotifier::deviceAdded, this, &ConnectSettingsDialog::locateSerialPort);
    connect(deviceNotifier, &Solid::DeviceNotifier::deviceRemoved, this, &ConnectSettingsDialog::locateSerialPort);
    locateSerialPort();

    connect(ui->buttonBox, &QDialogButtonBox::accepted, [ = ] {
        if (ui->profileCB->currentText().isEmpty())
        {
            QMessageBox msg;
            msg.setText(i18n("Please, create a profile to connect on Settings!"));
            msg.setIcon(QMessageBox::Information);
            msg.exec();
        } else if (!ui->serialPortCB->currentText().isEmpty())
        {
            emit _connect(ui->serialPortCB->currentText(), ui->baudCB->currentText().toInt());
        } else {
            QMessageBox msg;
            msg.setText(i18n("Please, connect a serial device to continue!"));
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
    fw.prepend(i18n("Auto-Detect"));
    ui->firmwareCB->addItems(fw);
}

void ConnectSettingsDialog::initBaudRateComboBox()
{
    QStringList br;
    br.append("115200");
    br.append("250000");
    ui->baudCB->addItems(br);
}

void ConnectSettingsDialog::initProfileComboBox()
{

    settings.beginGroup(i18n("GeneralSettings"));
    QStringList groups = settings.childGroups();
    settings.endGroup();
    ui->profileCB->addItems(groups);
}
