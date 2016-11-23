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

#include <QWidget>

namespace Ui {
class TemporaryPrinterControlWidget;
}

class TemporaryPrinterControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TemporaryPrinterControlWidget(QWidget *parent = nullptr);
    ~TemporaryPrinterControlWidget();
    void updateProgressBar(float percent);
    void updateTemperatures(float bed, float ext);
    void updateLog(QString msg);

private:
    Ui::TemporaryPrinterControlWidget *ui;

signals:
    void setHeatBed(int temp);
    void setHeatExtruder(int ext, int temp);
    void changeFanSpeed(int value);
    void changePrintSpeed(int value);
    void homeAll();
    void homeX();
    void homeY();
    void homeZ();
    void moveAxis(QLatin1Char axis, int dist);
};
