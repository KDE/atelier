/* Atelier KDE Printer Host for 3D Printing
    Copyright (C) <2016>
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
#include <QRadioButton>
#include <QMap>

namespace Ui
{
class BedExtruderWidget;
}

class BedExtruderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BedExtruderWidget(QWidget *parent = nullptr);
    ~BedExtruderWidget();
    void setExtruderCount(int value);
    void updateBedTemp(const float temp);
    void updateExtTemp(const float temp);
    void updateBedTargetTemp(const float temp);
    void updateExtTargetTemp(const float temp);
    void stopHeating();

private:
    Ui::BedExtruderWidget *ui;
    QMap <int, QRadioButton *> extruderMap;
    void heatExtruderClicked(bool clicked);
    void heatBedClicked(bool clicked);
    int currentExtruder();
    int extruderCount = 0;

signals:
    void bedTemperatureChanged(int tmp);
    void extTemperatureChanged(int tmp, int currExt);
};
