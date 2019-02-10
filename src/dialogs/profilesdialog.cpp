/* Atelier KDE Printer Host for 3D Printing
    Copyright (C) <2016>
    Author: Lays Rodrigues - lays.rodrigues@kde.org
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
#include <atcore_default_folders.h>
#include <KLocalizedString>
#include <QDir>
#include <QMessageBox>
#include "profilesdialog.h"
#include "ui_profilesdialog.h"

ProfilesDialog::ProfilesDialog(QWidget *parent) :
    QDialog(parent)
    , ui(new Ui::ProfilesDialog)
    , m_modified(false)
{
    ui->setupUi(this);
    ui->firmwareCB->addItem(QStringLiteral("Auto-Detect"));
    ui->firmwareCB->addItems(detectFWPlugins());
    ui->baudCB->addItems(SERIAL::BAUDS);
    ui->baudCB->setCurrentText(QLatin1String("115200"));
    ui->profileCB->setAutoCompletion(true);
    connect(ui->profileCB, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](const int newIndex) {
        blockSignals(true);
        ui->profileCB->setCurrentIndex(m_prevIndex);
        blockSignals(false);
        askToSave();
        blockSignals(true);
        m_prevIndex = newIndex;
        ui->profileCB->setCurrentIndex(m_prevIndex);
        blockSignals(false);
        loadSettings();
    });
    updateCBProfiles();

    connect(ui->buttonBox, &QDialogButtonBox::clicked, this, &ProfilesDialog::buttonBoxClicked);
    connect(ui->heatedBedCK, &QCheckBox::clicked, this, [this](const bool & status) {
        ui->bedTempSB->setEnabled(status);
    });

    connect(ui->cartesianRB, &QRadioButton::clicked, this, [this] {
        ui->printerTypeStack->setCurrentIndex(1);
    });

    connect(ui->deltaRB, &QRadioButton::clicked, this, [this] {
        ui->printerTypeStack->setCurrentIndex(0);
    });

    connect(ui->removeProfilePB, &QPushButton::clicked, this, &ProfilesDialog::removeProfile);
    ui->removeProfilePB->setIcon(QIcon::fromTheme(QStringLiteral("edit-delete"), style()->standardIcon(QStyle::SP_TrashIcon)));

//if any control is modified and no load / save has happened contents are not saved.
    auto modify = [this] {setModified(true);};
    connect(ui->baudCB, &QComboBox::currentTextChanged, modify);
    connect(ui->radiusSB, &QSpinBox::editingFinished, modify);
    connect(ui->z_delta_dimensionSB, &QSpinBox::editingFinished, modify);
    connect(ui->x_dimensionSB, &QSpinBox::editingFinished, modify);
    connect(ui->y_dimensionSB, &QSpinBox::editingFinished, modify);
    connect(ui->z_dimensionSB, &QSpinBox::editingFinished, modify);
    connect(ui->heatedBedCK, &QCheckBox::stateChanged, modify);
    connect(ui->bedTempSB, &QSpinBox::editingFinished, modify);
    connect(ui->extruderTempSB, &QSpinBox::editingFinished, modify);
    connect(ui->postPauseLE, &QLineEdit::editingFinished, modify);
    connect(ui->firmwareCB, &QComboBox::currentTextChanged, modify);
    connect(ui->autoReportTempCK, &QCheckBox::stateChanged, modify);
}

ProfilesDialog::~ProfilesDialog()
{
    delete ui;
}

void ProfilesDialog::buttonBoxClicked(QAbstractButton *btn)
{
    switch (ui->buttonBox->buttonRole(btn)) {
    case QDialogButtonBox::ResetRole:
        askToSave();
        loadSettings();
        break;
    case QDialogButtonBox::RejectRole:
        askToSave();
        close();
        break;
    case QDialogButtonBox::AcceptRole:
        saveSettings();
        break;
    default:
        break;
    }
}

void ProfilesDialog::saveSettings()
{
    m_settings.beginGroup(QStringLiteral("Profiles"));
    QStringList groups = m_settings.childGroups();
    m_settings.endGroup();
    if (groups.contains(ui->profileCB->currentText())) {
        int ret = QMessageBox::warning(
                      this
                      , i18n("Overwrite Profile?")
                      , i18n("A profile with this name already exists. \n Are you sure you want to overwrite it?")
                      , QMessageBox::Save
                      , QMessageBox::Cancel
                  );

        if (ret == QMessageBox::Cancel) {
            return;
        }
    }
    save();
}

void ProfilesDialog::save()
{
    QString currentProfile = ui->profileCB->currentText();
    //Add indent to better view of the data
    m_settings.beginGroup(QStringLiteral("Profiles"));
    m_settings.beginGroup(currentProfile);
    //BED
    if (ui->cartesianRB->isChecked()) {
        m_settings.setValue(QStringLiteral("isCartesian"), true);
        m_settings.setValue(QStringLiteral("dimensionX"), ui->x_dimensionSB->value());
        m_settings.setValue(QStringLiteral("dimensionY"), ui->y_dimensionSB->value());
        m_settings.setValue(QStringLiteral("dimensionZ"), ui->z_dimensionSB->value());
    } else {
        m_settings.setValue(QStringLiteral("isCartesian"), false);
        m_settings.setValue(QStringLiteral("radius"), ui->radiusSB->value());
        m_settings.setValue(QStringLiteral("z_delta_dimension"), ui->z_delta_dimensionSB->value());
    }

    m_settings.setValue(QStringLiteral("heatedBed"), ui->heatedBedCK->isChecked());
    m_settings.setValue(QStringLiteral("maximumTemperatureBed"), ui->bedTempSB->value());
    //HOTEND
    m_settings.setValue(QStringLiteral("maximumTemperatureExtruder"), ui->extruderTempSB->value());
    m_settings.setValue(QStringLiteral("autoReportTemp"), ui->autoReportTempCK->isChecked());
    //Baud
    m_settings.setValue(QStringLiteral("bps"), ui->baudCB->currentText());
    m_settings.setValue(QStringLiteral("firmware"), ui->firmwareCB->currentText());
    m_settings.setValue(QStringLiteral("postPause"), ui->postPauseLE->text());
    m_settings.endGroup();
    m_settings.endGroup();

    //Load new profile
    setModified(false);
    updateCBProfiles();
    loadSettings(currentProfile);
    emit updateProfiles();
}

void ProfilesDialog::loadSettings(const QString &currentProfile)
{
    m_settings.beginGroup(QStringLiteral("Profiles"));
    const QString profileName = currentProfile.isEmpty() ? ui->profileCB ->currentText() : currentProfile;
    ui->profileCB->setCurrentText(profileName);
    m_settings.beginGroup(profileName);

    //BED
    if (m_settings.value(QStringLiteral("isCartesian")).toBool()) {
        ui->printerTypeStack->setCurrentIndex(1);
        ui->cartesianRB->setChecked(true);
        ui->deltaRB->setChecked(false);
        ui->x_dimensionSB->setValue(m_settings.value(QStringLiteral("dimensionX"), QStringLiteral("0")).toInt());
        ui->y_dimensionSB->setValue(m_settings.value(QStringLiteral("dimensionY"), QStringLiteral("0")).toInt());
        ui->z_dimensionSB->setValue(m_settings.value(QStringLiteral("dimensionZ"), QStringLiteral("0")).toInt());
    } else {
        ui->printerTypeStack->setCurrentIndex(0);
        ui->deltaRB->setChecked(true);
        ui->cartesianRB->setChecked(false);
        ui->radiusSB->setValue(m_settings.value(QStringLiteral("radius"), QStringLiteral("0")).toInt());
        ui->z_delta_dimensionSB->setValue(m_settings.value(QStringLiteral("z_delta_dimension"), QStringLiteral("0")).toInt());
    }

    ui->heatedBedCK->setChecked(m_settings.value(QStringLiteral("heatedBed"), QStringLiteral("true")).toBool());
    ui->bedTempSB->setEnabled(ui->heatedBedCK->isChecked());
    ui->bedTempSB->setValue(m_settings.value(QStringLiteral("maximumTemperatureBed"), QStringLiteral("0")).toInt());
    ui->autoReportTempCK->setChecked(m_settings.value(QStringLiteral("autoReportTemp"), QStringLiteral("false")).toBool());

    //HOTEND
    ui->extruderTempSB->setValue(m_settings.value(QStringLiteral("maximumTemperatureExtruder"), QStringLiteral("0")).toInt());
    //Baud
    ui->baudCB->setCurrentText(m_settings.value(QStringLiteral("bps"), QStringLiteral("115200")).toString());
    ui->firmwareCB->setCurrentText(m_settings.value(QStringLiteral("firmware"), QStringLiteral("Auto-Detect")).toString());
    ui->postPauseLE->setText(m_settings.value(QStringLiteral("postPause"), QString()).toString());
    m_settings.endGroup();
    m_settings.endGroup();
    setModified(false);

}

void ProfilesDialog::updateCBProfiles()
{
    m_settings.beginGroup(QStringLiteral("Profiles"));
    QStringList groups = m_settings.childGroups();
    m_settings.endGroup();
    if (groups.isEmpty()) {
        ui->printerTypeStack->setCurrentIndex(1);
    }
    ui->profileCB->clear();
    ui->profileCB->addItems(groups);
}

void ProfilesDialog::removeProfile()
{
    QString currentProfile = ui->profileCB->currentText();
    m_settings.beginGroup(QStringLiteral("Profiles"));
    m_settings.beginGroup(currentProfile);
    m_settings.remove("");
    m_settings.endGroup();
    m_settings.remove(currentProfile);
    m_settings.endGroup();
    updateCBProfiles();
}

QStringList ProfilesDialog::detectFWPlugins()
{
    QStringList firmwares;
    QStringList paths = AtCoreDirectories::pluginDir;
    //Add our runtime paths
    const QString &path(qApp->applicationDirPath());
    paths.prepend(path + QStringLiteral("/../Plugins/AtCore"));
    paths.prepend(path + QStringLiteral("/AtCore"));
    paths.prepend(path + QStringLiteral("/plugins"));
    for (const QString &path : qAsConst(paths)) {
        firmwares = firmwaresInPath(path);
        if (!firmwares.isEmpty()) {
            //use path where plugins were detected.
            break;
        }
    }
    return firmwares;
}

QStringList ProfilesDialog::firmwaresInPath(const QString &path)
{
    QStringList firmwares;
    QStringList files = QDir(path).entryList(QDir::Files);
    for (const QString &f : files) {
        QString file = f;
#if defined(Q_OS_WIN)
        if (file.endsWith(QStringLiteral(".dll")))
#elif defined(Q_OS_MAC)
        if (file.endsWith(QStringLiteral(".dylib")))
#else
        if (file.endsWith(QStringLiteral(".so")))
#endif
            file = file.split(QChar::fromLatin1('.')).at(0);
        else {
            continue;
        }
        if (file.startsWith(QStringLiteral("lib"))) {
            file = file.remove(QStringLiteral("lib"));
        }
        file = file.toLower().simplified();
        firmwares.append(file);
    }
    return firmwares;
}

void ProfilesDialog::setModified(bool modified)
{
    m_modified = modified;
}

void ProfilesDialog::askToSave()
{
    if (m_modified) {
        int ret = QMessageBox::question(
                      this
                      , i18n("Save?")
                      , i18n("This Profile has been modified, Would you like to Save it?")
                      , QMessageBox::Save
                      , QMessageBox::No
                  );
        if (ret == QMessageBox::Save) {
            save();
        }
    }
}
