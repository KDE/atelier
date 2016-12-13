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
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &GeneralSettingsDialog::saveSettings);

    settings.beginGroup(i18n("GeneralSettings"));
    QStringList groups = settings.childGroups();
    settings.endGroup();

    ui->profileCB->addItems(groups);
    loadSettings(ui->profileCB->currentText());
    connect(ui->profileCB, &QComboBox::editTextChanged, this, &GeneralSettingsDialog::loadSettings);

    connect(ui->heatedBedCK, &QCheckBox::clicked, [ = ](const bool & status) {
        if (status) {
            ui->bedTempLE->setEnabled(true);
        } else {
            ui->bedTempLE->setEnabled(false);
        }
    });
}

GeneralSettingsDialog::~GeneralSettingsDialog()
{
    delete ui;
}

void GeneralSettingsDialog::saveSettings()
{
    settings.beginGroup(i18n("GeneralSettings"));
    QStringList groups = settings.childGroups();
    settings.endGroup();
    QString currentProfile = ui->profileCB->currentText();
    if (groups.contains(currentProfile)) {
        QMessageBox msgBox;
        msgBox.setText(i18n("The settings has been modified."));
        msgBox.setInformativeText(i18n("Do you want to save your changes?"));
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
        int ret = msgBox.exec();
        if (ret == QMessageBox::Cancel) {
            return;
        }
    }
    //Add indent to better view of the data
    settings.beginGroup(QLatin1String("GeneralSettings"));
    settings.beginGroup(currentProfile);
    //BED
    settings.setValue(QLatin1String("dimensionX"), ui->x_dimensionLE->text());
    settings.setValue(QLatin1String("dimensionY"), ui->y_dimensionLE->text());
    settings.setValue(QLatin1String("dimensionZ"), ui->z_dimensionLE->text());
    settings.setValue(QLatin1String("heatedBed"), ui->heatedBedCK->isChecked());
    settings.setValue(QLatin1String("temperatureBed"), ui->bedTempLE->text());
    //HOTEND
    settings.setValue(QLatin1String("nozzleSize"), ui->nozzleSizeLE->text());
    settings.setValue(QLatin1String("temperatureExtruder"), ui->extruderTempLE->text());
    settings.setValue(QLatin1String("maxTemperatureExtruder"), ui->extruderMaxTempLE->text());
    settings.endGroup();
    settings.endGroup();
    return; //TODO Add something to alert the user that the settings where saved
}

void GeneralSettingsDialog::loadSettings(QString currentProfile)
{
    settings.beginGroup(QLatin1String("GeneralSettings"));
    settings.beginGroup(currentProfile);
    //BED
    ui->x_dimensionLE->setText(settings.value(QLatin1String("dimensionX"), QLatin1String("200")).toString());
    ui->y_dimensionLE->setText(settings.value(QLatin1String("dimensionY"), QLatin1String("200")).toString());
    ui->z_dimensionLE->setText(settings.value(QLatin1String("dimensionZ"), QLatin1String("200")).toString());
    ui->heatedBedCK->setChecked(settings.value(QLatin1String("heatedBed"), QLatin1String("200")).toBool());
    ui->bedTempLE->setText(settings.value(QLatin1String("temperatureBed"), QLatin1String("0")).toString());
    //HOTEND
    ui->nozzleSizeLE->setText(settings.value(QLatin1String("nozzleSize"), QLatin1String("0.4")).toString());
    ui->extruderTempLE->setText(settings.value(QLatin1String("temperatureExtruder"), QLatin1String("0")).toString());
    ui->extruderMaxTempLE->setText(settings.value(QLatin1String("maxTemperatureExtruder"), QLatin1String("250")).toString());
    settings.endGroup();
    settings.endGroup();

}
