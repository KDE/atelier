/* Atelier KDE Printer Host for 3D Printing
    Copyright (C) <2016>
    Author: Lays Rodrigues - laysrodriguessilva@gmail.com
            Tomaz Canabraza - tcanabrava@kde.org
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
#pragma once

#include <QHBoxLayout>
#include <QMap>
#include <QRadioButton>
#include <QWidget>

class ThermoWidget;

class BedExtruderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BedExtruderWidget(QWidget *parent = nullptr);
    void setExtruderCount(int value);
    void setBedMaxTemperature(int value);
    void setExtruderMaxTemperature(int value);
    void updateBedTemp(const float temp);
    void updateExtTemp(const float temp);
    void updateBedTargetTemp(const float temp);
    void updateExtTargetTemp(const float temp);

private:
    QMap <int, QRadioButton *> extruderMap;
    ThermoWidget *m_bedThermo = nullptr;
    ThermoWidget *m_extruderThermo = nullptr;
    QWidget *m_extruderBox = nullptr;
    QHBoxLayout *m_extrudersLayout = nullptr;

    int currentExtruder();
    int m_extruderCount = 0;

signals:
    void bedTemperatureChanged(int tmp, bool andWait);
    void extTemperatureChanged(int tmp, int currExt, bool andWait);
};
