/* Atelier KDE Printer Host for 3D Printing
    Copyright (C) <2017>
    Author: Lays Rodrigues - laysrodriguessilva@gmail.com

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
#include <QWidget>
#include <QUrl>
#include <QToolBar>
#include <AtCore/AtCore>

namespace Ui
{
class AtCoreInstanceWidget;
}

/**
 * @todo write docs
 */
class AtCoreInstanceWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * Default constructor
     */
    AtCoreInstanceWidget(QWidget* parent = nullptr);

    /**
     * Destructor
     */
    ~AtCoreInstanceWidget();
    void startConnection(const QString& serialPort, const QMap<QString, QVariant>& profiles);
    bool connected();

private:
    Ui::AtCoreInstanceWidget* ui;
    AtCore m_core;
    QToolBar *toolBar;
    void initConnectsToAtCore();
    void printFile(const QUrl& fileName);
    void pausePrint();
    void stopPrint();
    void disableMotors();
    void checkReceivedCommand(const QByteArray &message);
    void checkPushedCommands(const QByteArray &bmsg);
    void handlePrinterStatusChanged(AtCore::STATES newState);
    void checkTemperature(uint sensorType, uint number, uint temp);
    void axisControlClicked(QChar axis, int value);
    void enableControls(bool b);
    void buildToolbar();
    void setupConnections();

signals:
    void extruderCountChanged(int count);
};
