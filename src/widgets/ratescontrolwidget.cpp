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
#include "ratescontrolwidget.h"
#include "ui_ratescontrolwidget.h"

RatesControlWidget::RatesControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RatesControlWidget)
{
    ui->setupUi(this);
    connect(ui->flowRateSB, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [ = ](float value){
        emit flowRateChanged(value);
    });
    connect(ui->fanSpeedSB, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [ = ](float value){
        emit fanSpeedChanged(value);
    });
    connect(ui->printSpeedSB, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [ = ](float value){
       emit printSpeedChanged(value);
    });

}

RatesControlWidget::~RatesControlWidget()
{
    delete ui;
}
