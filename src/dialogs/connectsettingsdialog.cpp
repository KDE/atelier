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
#include "connectsettingsdialog.h"
#include <QCloseEvent>
#include <KLocalizedString>
#include <QLabel>
#include <QLayout>
#include <QMessageBox>

ConnectSettingsDialog::ConnectSettingsDialog(QWidget *parent) :
    QDialog(parent),
    atcore(new AtCore)
{
    initDisplay();
    initData();
    setWindowTitle(i18n("Connect to Printer"));
    atcore->setSerialTimerInterval(100);
    connect(atcore, &AtCore::portsChanged, this, &ConnectSettingsDialog::updateSerialPort);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &ConnectSettingsDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ConnectSettingsDialog::close);
}

void ConnectSettingsDialog::initDisplay()
{
    serialPortCB = new QComboBox;
    serialPortCB->setEditable(true);
    QLabel *deviceLabel = new QLabel(i18n("Device"));
    QHBoxLayout *deviceLayout = new QHBoxLayout;
    deviceLayout->addWidget(deviceLabel);
    deviceLayout->addWidget(serialPortCB);

    deviceProfileCB = new QComboBox;
    QHBoxLayout *profileLayout = new QHBoxLayout;
    QLabel *profileLabel = new QLabel(i18n("Profile"));
    profileLayout->addWidget(profileLabel);
    profileLayout->addWidget(deviceProfileCB);

    buttonBox = new QDialogButtonBox();
    buttonBox->addButton(i18n("Connect"),QDialogButtonBox::AcceptRole);
    buttonBox->addButton(QDialogButtonBox::Cancel);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(deviceLayout);
    mainLayout->addLayout(profileLayout);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);
}

void ConnectSettingsDialog::initData()
{
    settings.beginGroup("GeneralSettings");
    QStringList profiles = settings.childGroups();
    settings.endGroup();
    deviceProfileCB->addItems(profiles);
}

void ConnectSettingsDialog::updateSerialPort(const QStringList &ports)
{
    serialPortCB->clear();
    if(!ports.isEmpty()) {
        serialPortCB->addItems(ports);
    }
}

QMap<QString, QVariant> ConnectSettingsDialog::profileData()
{
    QString profile = deviceProfileCB->currentText();
    settings.beginGroup("GeneralSettings");
    settings.beginGroup(profile);
    QMap<QString, QVariant> data;
    data["bps"] = settings.value(QStringLiteral("bps"), QStringLiteral("115200"));
    data["bedTemp"] = settings.value(QStringLiteral("maximumTemperatureBed"), QStringLiteral("0"));
    data["hotendTemp"] = settings.value(QStringLiteral("maximumTemperatureExtruder"), QStringLiteral("0"));
    data["firmware"] = settings.value(QStringLiteral("firmware"),QStringLiteral("Auto-Detect"));
    data["postPause"] = settings.value(QStringLiteral("postPause"),QStringLiteral(""));
    data["name"] = profile;
    settings.endGroup();
    settings.endGroup();
    return data;
}

void ConnectSettingsDialog::accept()
{
    if (deviceProfileCB->currentText().isEmpty()) {
        QMessageBox::critical(this, i18n("Error"), i18n("Please, create a profile to connect on Settings!"));
        return;
    }
    if (serialPortCB->currentText().isEmpty()) {
        QMessageBox::critical(this, i18n("Error"), i18n("Please, connect a serial device to continue!"));
        return;
    }
    emit startConnection(serialPortCB->currentText(), profileData());
    close();
}
