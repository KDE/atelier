/* Atelier KDE Printer Host for 3D Printing
    Copyright (C) <2016>
    Author: Lays Rodrigues - lays.rodrigues@kde.org
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
#include <QLabel>
#include <QRadioButton>
#include "bedextruderwidget.h"
#include "thermowidget.h"

BedExtruderWidget::BedExtruderWidget(QWidget *parent) :
    QWidget(parent)
    , m_bedThermo(new ThermoWidget(this, QString(i18n("Bed"))))
    , m_extruderThermo(new ThermoWidget(this, QString(i18n("HotEnd"))))
    , m_extrudersLayout(new QHBoxLayout)
    , m_extruderBox(new QWidget(this))
{
    m_bedThermo->setScale(0, 150);
    m_extruderThermo->setScale(0, 250);
    m_extruderBox->setLayout(m_extrudersLayout);

    auto *label = new QLabel(i18n("Active Extruder:"));
    m_extrudersLayout->addWidget(label);

    auto *layout = new QHBoxLayout;
    layout->addWidget(m_extruderBox);
    layout->addWidget(m_bedThermo);
    layout->addWidget(m_extruderThermo);

    setLayout(layout);
    //Add Default Extruder
    setExtruderCount(1);

    connect(m_bedThermo, &ThermoWidget::targetTemperatureChanged, [this](double v) {
        qDebug() << "Receiving the temperature change for bed";
        emit bedTemperatureChanged((int)v, false);
    });

    connect(m_extruderThermo, &ThermoWidget::targetTemperatureChanged, [this](double v) {
        qDebug() << "Receiving the temperature changed for thermo";
        emit extTemperatureChanged((int)v, currentExtruder(), false);
    });
}

void BedExtruderWidget::setExtruderCount(int value)
{
    value > 1 ? m_extruderBox->setVisible(true) : m_extruderBox->setVisible(false);
    if (value == m_extruderCount) {
        return;
    } else if (m_extruderCount < value) {
        //loop for the new buttons
        for (int i = m_extruderCount; i < value; i++) {
            auto *rb = new QRadioButton(QString::number(i + 1));
            m_extrudersLayout->addWidget(rb);
            extruderMap.insert(i, rb);
        }
    } else {
        //remove buttons - need to test it!
        for (int i = m_extruderCount; i >= value; i--) {
            auto *rb = extruderMap.value(i);
            m_extrudersLayout->removeWidget(rb);
            extruderMap.remove(i);
            delete (rb);
        }
    }
    m_extruderCount = value;
}

void BedExtruderWidget::updateBedTemp(const float temp)
{
    m_bedThermo->setCurrentTemperature(temp);
}

void BedExtruderWidget::updateExtTemp(const float temp)
{
    m_extruderThermo->setCurrentTemperature(temp);
}

void BedExtruderWidget::updateBedTargetTemp(const float temp)
{
    m_bedThermo->setTargetTemperature(temp);
}

void BedExtruderWidget::updateExtTargetTemp(const float temp)
{
    m_extruderThermo->setTargetTemperature(temp);
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

void BedExtruderWidget::setBedMaxTemperature(int value)
{
    m_bedThermo->setScale(0, value);
}

void BedExtruderWidget::setExtruderMaxTemperature(int value)
{
    m_extruderThermo->setScale(0, value);
}

void BedExtruderWidget::hideBedThermo()
{
    m_bedThermo->hide();
}
