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
#include "bedextruderwidget.h"
#include "ui_bedextruderwidget.h"
#include <QRadioButton>
#include <KLocalizedString>

BedExtruderWidget::BedExtruderWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BedExtruderWidget)
{
    ui->setupUi(this);

    //Add Default Extruder
    setExtruderCount(1);
    connect(ui->heatBedPB, &QPushButton::clicked, this, &BedExtruderWidget::heatBedClicked);
    connect(ui->heatExtPB, &QPushButton::clicked, this, &BedExtruderWidget::heatExtruderClicked);

    connect(ui->bedTempSB, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [ = ](double tmp) {
        if (ui->heatBedPB->isChecked()) {
            emit bedTemperatureChanged(tmp);
        }
    });

    connect(ui->extTempSB, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [ = ](double tmp) {
        if (ui->heatExtPB->isChecked()) {
            emit extTemperatureChanged(tmp, currentExtruder());
        }
    });
}

BedExtruderWidget::~BedExtruderWidget()
{
    delete ui;
}

void BedExtruderWidget::setExtruderCount(int value)
{
    if (value == extruderCount) {
        return;
    } else if (extruderCount < value) {
        //loop for the new buttons
        for (int i = extruderCount; i < value; i++) {
            auto *rb = new QRadioButton(QString::number(i + 1));
            ui->extRadioButtonLayout->addWidget(rb);
            extruderMap.insert(i, rb);
        }
    } else {
        //remove buttons - need to test it!
        for (int i = extruderCount; i >= value; i--) {
            auto *rb = extruderMap.value(i);
            ui->extRadioButtonLayout->removeWidget(rb);
            extruderMap.remove(i);
            delete (rb);
        }
    }
    extruderCount = value;
}

void BedExtruderWidget::updateBedTemp(const float temp)
{
    ui->bedCurrTempLB->setText(QString::number(temp));
}

void BedExtruderWidget::updateExtTemp(const float temp)
{
    ui->extCurrTempLB->setText(QString::number(temp));
}

void BedExtruderWidget::updateBedTargetTemp(const float temp)
{
    ui->bedTargetTempLB->setText(QString::number(temp) + " ºC");
}

void BedExtruderWidget::updateExtTargetTemp(const float temp)
{
    ui->extTargetTempLB->setText(QString::number(temp) + " ºC");
}

void BedExtruderWidget::stopHeating()
{
    emit bedTemperatureChanged(0);
    for (int i = 0; i < extruderCount; i++) {
        emit extTemperatureChanged(0, i);
    }
    ui->heatBedPB->setChecked(false);
    ui->heatExtPB->setChecked(false);
}

void BedExtruderWidget::heatExtruderClicked(bool clicked)
{
    int temp = ui->extTempSB->value() * clicked;
    emit extTemperatureChanged(temp, currentExtruder());
}

void BedExtruderWidget::heatBedClicked(bool clicked)
{
    int temp = ui->bedTempSB->value() * clicked;
    emit bedTemperatureChanged(temp);

}

int BedExtruderWidget::currentExtruder()
{
    int currExt = 0;
    if (extruderMap.size() > 1) {
        for (int i = 0; i < extruderMap.size(); i++) {
            if (extruderMap.value(i)->isChecked()) {
                currExt = i;
                break;
            }
        }
    }
    return currExt;
}
