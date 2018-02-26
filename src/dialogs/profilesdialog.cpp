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
#include "profilesdialog.h"
#include "ui_profilesdialog.h"
#include <QMessageBox>
#include <KLocalizedString>
#include <QDir>

//Do not include for windows/mac os
#ifndef Q_OS_WIN
    #ifndef Q_OS_MAC
        #include <AtCore/atcore_default_folders.h>
    #endif
#endif

ProfilesDialog::ProfilesDialog(const QStringList &baudList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProfilesDialog)
{
    ui->setupUi(this);
    ui->firmwareCB->addItem(QStringLiteral("Auto-Detect"));
    ui->firmwareCB->addItems(detectFWPlugins());
    ui->baudCB->addItems(baudList);
    ui->baudCB->setCurrentText(QLatin1String("115200"));
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

    connect(ui->removeProfilePB, &QPushButton::clicked, this, &ProfilesDialog::removeProfile);
#ifdef Q_OS_LINUX
    ui->removeProfilePB->setIcon(QIcon::fromTheme("edit-delete"));
#else
    ui->removeProfilePB->setIcon(style()->standardIcon(QStyle::SP_TrashIcon));
#endif
}

ProfilesDialog::~ProfilesDialog()
{
    delete ui;
}

void ProfilesDialog::saveSettings()
{
    settings.beginGroup(QStringLiteral("GeneralSettings"));
    QStringList groups = settings.childGroups();
    settings.endGroup();
    QString currentProfile = ui->profileCB->currentText();
    if (groups.contains(currentProfile)) {
        int ret = QMessageBox::information(this, i18n("Save?"),
                                           i18n("A profile with this name already exists. \n Are you sure you want to overwrite it?"),
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
    settings.setValue(QStringLiteral("maximumTemperatureBed"), ui->bedTempSB->value());
    //HOTEND
    settings.setValue(QStringLiteral("maximumTemperatureExtruder"), ui->extruderTempSB->value());
    //Baud
    settings.setValue(QStringLiteral("bps"), ui->baudCB->currentText());
    settings.setValue(QStringLiteral("firmware"),ui->firmwareCB->currentText());
    settings.endGroup();
    settings.endGroup();

    //Load new profile
    updateCBProfiles();
    loadSettings(currentProfile);
    emit updateProfiles();
}

void ProfilesDialog::loadSettings(const QString &currentProfile)
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
    ui->bedTempSB->setValue(settings.value(QStringLiteral("maximumTemperatureBed"), QStringLiteral("0")).toInt());

    //HOTEND
    ui->extruderTempSB->setValue(settings.value(QStringLiteral("maximumTemperatureExtruder"), QStringLiteral("0")).toInt());
    //Baud
    ui->baudCB->setCurrentText(settings.value(QStringLiteral("bps"), QStringLiteral("115200")).toString());
    ui->firmwareCB->setCurrentText(settings.value(QStringLiteral("firmware"), QStringLiteral("Auto-Detect")).toString());
    settings.endGroup();
    settings.endGroup();

}

void ProfilesDialog::updateCBProfiles()
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

void ProfilesDialog::accept()
{
    saveSettings();
}

void ProfilesDialog::removeProfile(){
    QString currentProfile = ui->profileCB->currentText();
    settings.beginGroup(QStringLiteral("GeneralSettings"));
    settings.beginGroup(currentProfile);
    settings.remove("");
    settings.endGroup();
    settings.remove(currentProfile);
    settings.endGroup();
    updateCBProfiles();
}

QStringList ProfilesDialog::detectFWPlugins() const
{
    //Path used if for windows/ mac os only.
    QDir pluginDir(qApp->applicationDirPath() + QStringLiteral("/plugins"));

#if defined(Q_OS_WIN)
    pluginDir.setNameFilters(QStringList() << "*.dll");

#elif defined(Q_OS_MAC)
    pluginDir.setNameFilters(QStringList() << "*.dylib");

#else //Not Windows || Not MAC
    QStringList pathList = AtCoreDirectories::pluginDir;
    pathList.append(QLibraryInfo::location(QLibraryInfo::PluginsPath) + QStringLiteral("/AtCore"));

    for (const auto &path : pathList) {
        if (QDir(path).exists()) {
            //use path where plugins were detected.
            pluginDir = QDir(path);
            break;
        }
    }
    pluginDir.setNameFilters(QStringList() << "*.so");
#endif

    QStringList firmwares;
    QStringList files = pluginDir.entryList(QDir::Files);
    foreach (const QString &f, files) {
        QString file = f;
            file = file.split(QChar::fromLatin1('.')).at(0);
        if (file.startsWith(QStringLiteral("lib"))) {
            file = file.remove(QStringLiteral("lib"));
        }
        file = file.toLower().simplified();
        firmwares.append(file);
    }
    return firmwares;
}
