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
#pragma once

#include <QDialog>
#include <Solid/DeviceNotifier>
#include <QSettings>

namespace Ui
{
class ConnectSettingsDialog;
}

class ConnectSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectSettingsDialog(QWidget *parent = nullptr);
    void updateProfiles();
    void setFirmwareList(const QStringList fw);
    ~ConnectSettingsDialog();

private:
    Ui::ConnectSettingsDialog *ui;
    void locateSerialPort();
    QStringList serialPortList;
    Solid::DeviceNotifier *deviceNotifier;
    QSettings settings;
    QMap<QString, QVariant> profileData();
    void accept();

signals:
    void startConnection(const QString port, const QMap<QString, QVariant> data);
    void setConnectValue(bool b);
};
