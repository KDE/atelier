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
#include <QCloseEvent>

ConnectSettingsDialog::ConnectSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectSettingsDialog),
    deviceNotifier(Solid::DeviceNotifier::instance())
{
    ui->setupUi(this);
    connect(deviceNotifier, &Solid::DeviceNotifier::deviceAdded, this, &ConnectSettingsDialog::locateSerialPort);
    connect(deviceNotifier, &Solid::DeviceNotifier::deviceRemoved, this, &ConnectSettingsDialog::locateSerialPort);
    locateSerialPort();

    connect(ui->buttonBox, &QDialogButtonBox::clicked, [ = ](QAbstractButton * btn) {
        if (ui->buttonBox->buttonRole(btn) == QDialogButtonBox::RejectRole) {
            close();
            emit setConnectValue(false);
        }
    });

    updateProfiles();
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
#ifdef Q_OS_MAC
            //Mac os has callout serial ports starting with cu. they can only recv data. filter them out
            if (!serialPortInfo.portName().startsWith(QStringLiteral("cu."), Qt::CaseInsensitive)) {
                ports.append(serialPortInfo.portName());
            }
#else
            ports.append(serialPortInfo.portName());
#endif
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

void ConnectSettingsDialog::updateProfiles()
{
    settings.beginGroup("GeneralSettings");
    QStringList groups = settings.childGroups();
    settings.endGroup();
    ui->profileCB->clear();
    ui->profileCB->addItems(groups);
}

void ConnectSettingsDialog::setFirmwareList(QStringList fw)
{
    fw.prepend(i18n("Auto-Detect"));
    ui->firmwareCB->clear();
    ui->firmwareCB->addItems(fw);
}

QMap<QString, QVariant> ConnectSettingsDialog::profileData()
{
    settings.beginGroup("GeneralSettings");
    settings.beginGroup(ui->profileCB->currentText());
    QMap<QString, QVariant> data;
    data["bps"] = settings.value(QStringLiteral("bps"), QStringLiteral("115200"));
    data["bedTemp"] = settings.value(QStringLiteral("temperatureBed"), QStringLiteral("0"));
    data["hotendTemp"] = settings.value(QStringLiteral("temperatureExtrude"), QStringLiteral("0"));
    settings.endGroup();
    settings.endGroup();
    return data;
}

void ConnectSettingsDialog::accept()
{
    if (ui->profileCB->currentText().isEmpty()) {
        QMessageBox::warning(this, i18n("Warning"), i18n("Please, create a profile to connect on Settings!"));
        emit setConnectValue(false);
    } else if (ui->serialPortCB->currentText().isEmpty()) {
        QMessageBox::warning(this, i18n("Warning"), i18n("Please, connect a serial device to continue!"));
        emit setConnectValue(false);
    } else {
        emit startConnection(ui->serialPortCB->currentText(), profileData());
        this->close();
    }
}
