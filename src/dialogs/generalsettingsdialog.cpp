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
#include "generalsettingsdialog.h"
#include "ui_generalsettingsdialog.h"
#include <QMessageBox>
#include <KLocalizedString>

GeneralSettingsDialog::GeneralSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GeneralSettingsDialog)
{
    ui->setupUi(this);

    ui->profileCB->setAutoCompletion(true);
    connect(ui->profileCB, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [ = ] {
        loadSettings();
    });
    updateCBProfiles();

    connect(ui->buttonBox, &QDialogButtonBox::clicked, [ = ](QAbstractButton * btn) {
        switch (ui->buttonBox->buttonRole(btn)) {
        case QDialogButtonBox::ResetRole:
            loadSettings();
            break;
        case QDialogButtonBox::RejectRole:
                close();
            break;
        default:
            break;
        }
    });

    connect(ui->heatedBedCK, &QCheckBox::clicked, [ = ](const bool & status) {
        ui->bedTempSB->setEnabled(status);
    });

    connect(ui->cartesianRB, &QRadioButton::clicked, [ = ]() {
        ui->cartesianGB->setHidden(false);
        ui->deltaGB->setHidden(true);
    });

    connect(ui->deltaRB, &QRadioButton::clicked, [ = ]() {
        ui->cartesianGB->setHidden(true);
        ui->deltaGB->setHidden(false);
    });
}

void GeneralSettingsDialog::setBaudRates(const QStringList &list)
{
    ui->baudCB->addItems(list);
    ui->baudCB->setCurrentText(QLatin1String("115200"));
}

GeneralSettingsDialog::~GeneralSettingsDialog()
{
    delete ui;
}

void GeneralSettingsDialog::saveSettings()
{
    settings.beginGroup(QStringLiteral("GeneralSettings"));
    QStringList groups = settings.childGroups();
    settings.endGroup();
    QString currentProfile = ui->profileCB->currentText();
    if (groups.contains(currentProfile)) {
        int ret = QMessageBox::information(this, i18n("Save?"),
                                           i18n("The settings has been modified. \n Do you want to save your changes?"),
                                           QMessageBox::Save, QMessageBox::Cancel);
        if (ret == QMessageBox::Cancel) {
            return;
        }
    }
    //Add indent to better view of the data
    settings.beginGroup(QStringLiteral("GeneralSettings"));
    settings.beginGroup(currentProfile);
    //BED
    if (ui->cartesianRB->isChecked()) {
        settings.setValue(QStringLiteral("isCartesian"), true);
        settings.setValue(QStringLiteral("dimensionX"), ui->x_dimensionSB->value());
        settings.setValue(QStringLiteral("dimensionY"), ui->y_dimensionSB->value());
        settings.setValue(QStringLiteral("dimensionZ"), ui->z_dimensionSB->value());
    }
    else {
        settings.setValue(QStringLiteral("isCartesian"), false);
        settings.setValue(QStringLiteral("radius"), ui->radiusSB->value());
        settings.setValue(QStringLiteral("z_delta_dimension"), ui->z_dimensionSB->value());
    }

    settings.setValue(QStringLiteral("heatedBed"), ui->heatedBedCK->isChecked());
    settings.setValue(QStringLiteral("temperatureBed"), ui->bedTempSB->value());
    //HOTEND
    settings.setValue(QStringLiteral("temperatureExtruder"), ui->extruderTempSB->value());
    //Baud
    settings.setValue(QStringLiteral("bps"), ui->baudCB->currentText());
    settings.endGroup();
    settings.endGroup();

    //Load new profile
    updateCBProfiles();
    loadSettings(currentProfile);
    emit updateProfiles();
}

void GeneralSettingsDialog::loadSettings(const QString &currentProfile)
{
    settings.beginGroup(QStringLiteral("GeneralSettings"));
    const QString profileName = currentProfile.isEmpty() ? ui->profileCB ->currentText() : currentProfile;
    ui->profileCB->setCurrentText(profileName);
    settings.beginGroup(profileName);

    //BED
    if (settings.value(QStringLiteral("isCartesian")).toBool()) {
        ui->cartesianGB->setHidden(false);
        ui->cartesianRB->setChecked(true);
        ui->deltaRB->setChecked(false);
        ui->deltaGB->setHidden(true);
        ui->x_dimensionSB->setValue(settings.value(QStringLiteral("dimensionX"), QStringLiteral("0")).toInt());
        ui->y_dimensionSB->setValue(settings.value(QStringLiteral("dimensionY"), QStringLiteral("0")).toInt());
        ui->z_dimensionSB->setValue(settings.value(QStringLiteral("dimensionZ"), QStringLiteral("0")).toInt());
    } else {
        ui->deltaGB->setHidden(false);
        ui->deltaRB->setChecked(true);
        ui->cartesianRB->setChecked(false);
        ui->cartesianGB->setHidden(true);
        ui->radiusSB->setValue(settings.value(QStringLiteral("radius"), QStringLiteral("0")).toFloat());
        ui->z_delta_dimensionSB->setValue(settings.value(QStringLiteral("z_delta_dimension"), QStringLiteral("0")).toFloat());
    }

    ui->heatedBedCK->setChecked(settings.value(QStringLiteral("heatedBed"), QStringLiteral("true")).toBool());
    ui->bedTempSB->setEnabled(ui->heatedBedCK->isChecked());
    ui->bedTempSB->setValue(settings.value(QStringLiteral("temperatureBed"), QStringLiteral("0")).toFloat());

    //HOTEND
    ui->extruderTempSB->setValue(settings.value(QStringLiteral("temperatureExtruder"), QStringLiteral("0.0")).toFloat());
    //Baud
    ui->baudCB->setCurrentText(settings.value(QStringLiteral("bps"), QStringLiteral("115200")).toString());
    settings.endGroup();
    settings.endGroup();

}

void GeneralSettingsDialog::updateCBProfiles()
{
    settings.beginGroup(QStringLiteral("GeneralSettings"));
    QStringList groups = settings.childGroups();
    settings.endGroup();
    if (groups.isEmpty()) {
        ui->deltaGB->setHidden(true);
    }
    ui->profileCB->clear();
    ui->profileCB->addItems(groups);
}

void GeneralSettingsDialog::accept()
{
    saveSettings();
}
