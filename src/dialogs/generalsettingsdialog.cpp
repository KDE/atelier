#include "generalsettingsdialog.h"
#include "ui_generalsettingsdialog.h"
#include <QMessageBox>

GeneralSettingsDialog::GeneralSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GeneralSettingsDialog)
{
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &GeneralSettingsDialog::saveSettings);

    settings.beginGroup("GeneralSettings");
    QStringList groups = settings.childGroups();
    settings.endGroup();

    ui->profileCB->addItems(groups);
    loadSettings(ui->profileCB->currentText());
    connect(ui->profileCB, &QComboBox::editTextChanged, this, &GeneralSettingsDialog::loadSettings);
}

GeneralSettingsDialog::~GeneralSettingsDialog()
{
    delete ui;
}

void GeneralSettingsDialog::saveSettings()
{
    settings.beginGroup("GeneralSettings");
    QStringList groups = settings.childGroups();
    settings.endGroup();
    QString currentProfile = ui->profileCB->currentText();
    if(groups.contains(currentProfile)) {
        QMessageBox msgBox;
        msgBox.setText("The settings has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
        int ret = msgBox.exec();
        if(ret == QMessageBox::Cancel)
            return;
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
        ui->x_dimensionLE->setText(settings.value(QLatin1String("dimensionX"),QLatin1String("200")).toString());
        ui->y_dimensionLE->setText(settings.value(QLatin1String("dimensionY"),QLatin1String("200")).toString());
        ui->z_dimensionLE->setText(settings.value(QLatin1String("dimensionZ"),QLatin1String("200")).toString());
        ui->heatedBedCK->setChecked(settings.value(QLatin1String("heatedBed"),QLatin1String("200")).toBool());
        ui->bedTempLE->setText(settings.value(QLatin1String("temperatureBed"),QLatin1String("0")).toString());
        //HOTEND
        ui->nozzleSizeLE->setText(settings.value(QLatin1String("nozzleSize"),QLatin1String("0.4")).toString());
        ui->extruderTempLE->setText(settings.value(QLatin1String("temperatureExtruder"),QLatin1String("0")).toString());
        ui->extruderMaxTempLE->setText(settings.value(QLatin1String("maxTemperatureExtruder"),QLatin1String("250")).toString());
    settings.endGroup();
    settings.endGroup();

}
