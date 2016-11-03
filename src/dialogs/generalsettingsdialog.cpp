#include "generalsettingsdialog.h"
#include "ui_generalsettingsdialog.h"
#include <QSettings>
#include <QMessageBox>

GeneralSettingsDialog::GeneralSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GeneralSettingsDialog)
{
    ui->setupUi(this);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &GeneralSettingsDialog::saveSettings);
}

GeneralSettingsDialog::~GeneralSettingsDialog()
{
    delete ui;
}

void GeneralSettingsDialog::saveSettings()
{
    QSettings settings;
    QStringList groups;

    settings.beginGroup("GeneralSettings");
        groups = settings.childGroups();
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
