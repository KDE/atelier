/* Atelier KDE Printer Host for 3D Printing
    Copyright (C) <2016>
    Author: Lays Rodrigues - lays.rodrigues@kde.org

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
#pragma once

#include <QDialog>
#include <QSettings>

namespace Ui
{
class ProfilesDialog;
}

class ProfilesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProfilesDialog(QWidget *parent = nullptr);
    ~ProfilesDialog();

private:
    Ui::ProfilesDialog *ui;
    QSettings settings;
    QStringList detectFWPlugins() const;
    void accept();
    void loadSettings(const QString &currentProfile = QString());
    void removeProfile();
    void saveSettings();
    void updateCBProfiles();

signals:
    void updateProfiles();
};

namespace SERIAL
{
static const QStringList BAUDS = {
    QStringList() <<
                  QStringLiteral("9600") <<
                  QStringLiteral("14400") <<
                  QStringLiteral("19200") <<
                  QStringLiteral("28800") <<
                  QStringLiteral("38400") <<
                  QStringLiteral("57600") <<
                  QStringLiteral("76800") <<
                  QStringLiteral("115200") <<
                  QStringLiteral("230400") <<
                  QStringLiteral("250000") <<
                  QStringLiteral("500000") <<
                  QStringLiteral("1000000")
};
}
