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
#include <KLocalizedString>
#include <QBoxLayout>
#include <QRadioButton>
#include "bedextruderwidget.h"
#include "thermowidget.h"
#include "ui_bedextruderwidget.h"

#include "thermowidget.h"

BedExtruderWidget::BedExtruderWidget(QWidget *parent) :
    QWidget(parent),
    m_bedThermo(new ThermoWidget(this)),
    m_exturderThermo(new ThermoWidget(this))
{
    m_bedThermo->setScale(0, 150);
    m_exturderThermo->setScale(0, 250);

    auto centralLayout = new QBoxLayout(QBoxLayout::Direction::LeftToRight);
    centralLayout->addWidget(m_bedThermo);
    centralLayout->addWidget(m_exturderThermo);

    setLayout(centralLayout);
    //Add Default Extruder
    setExtruderCount(1);

    connect(m_bedThermo, &ThermoWidget::targetTemperatureChanged, [this](double v) {
        qDebug() << "Receiving the temperature change for bed";
        emit bedTemperatureChanged((int)v, false);
    });

    connect(m_exturderThermo, &ThermoWidget::targetTemperatureChanged, [this](double v) {
        qDebug() << "Receiving the temperature changed for thermo";
        emit extTemperatureChanged((int)v, currentExtruder(), false);
    });
}

BedExtruderWidget::~BedExtruderWidget()
{
}

void BedExtruderWidget::setExtruderCount(int value)
{
    if (value == extruderCount) {
        return;
    } else if (extruderCount < value) {
        //loop for the new buttons
        for (int i = extruderCount; i < value; i++) {
            auto *rb = new QRadioButton(QString::number(i + 1));
//            ui->extRadioButtonLayout->addWidget(rb);
            extruderMap.insert(i, rb);
        }
    } else {
        //remove buttons - need to test it!
        for (int i = extruderCount; i >= value; i--) {
            auto *rb = extruderMap.value(i);
//            ui->extRadioButtonLayout->removeWidget(rb);
            extruderMap.remove(i);
            delete (rb);
        }
    }
    extruderCount = value;
}

void BedExtruderWidget::updateBedTemp(const float temp)
{
    m_bedThermo->setCurrentTemperature(temp);
}

void BedExtruderWidget::updateExtTemp(const float temp)
{
    m_exturderThermo->setCurrentTemperature(temp);
}

void BedExtruderWidget::updateBedTargetTemp(const float temp)
{
    m_bedThermo->setTargetTemperature(temp);
}

void BedExtruderWidget::updateExtTargetTemp(const float temp)
{
    m_exturderThermo->setTargetTemperature(temp);
}

void BedExtruderWidget::stopHeating()
{
    /*
    emit bedTemperatureChanged(0,ui->bedAndWaitCB->isChecked());
    for (int i = 0; i < extruderCount; i++) {
        emit extTemperatureChanged(0, i,ui->extAndWaitCB->isChecked());
    }
    ui->heatBedPB->setChecked(false);
    ui->heatExtPB->setChecked(false);

    */
}

void BedExtruderWidget::heatExtruderClicked(bool clicked)
{
    /*
    int temp = ui->extTempSB->value() * clicked;
    emit extTemperatureChanged(temp, currentExtruder(),ui->extAndWaitCB->isChecked());

    */
}

void BedExtruderWidget::heatBedClicked(bool clicked)
{
    /*
    int temp = ui->bedTempSB->value() * clicked;
    emit bedTemperatureChanged(temp,ui->bedAndWaitCB->isChecked());
    */
}

int BedExtruderWidget::currentExtruder()
{
    int currExt = 0;
    for (int i = 0; i < extruderMap.size(); i++) {
        if (extruderMap.value(i)->isChecked()) {
            currExt = i;
            break;
        }
    }
    return currExt;
}
