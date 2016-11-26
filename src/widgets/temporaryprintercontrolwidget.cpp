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
#include "temporaryprintercontrolwidget.h"
#include "ui_temporaryprintercontrolwidget.h"

TemporaryPrinterControlWidget::TemporaryPrinterControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TemporaryPrinterControlWidget)
{
    ui->setupUi(this);
    connect(ui->fanSpeedSB, static_cast<void (QSpinBox::*)(const int)>(&QSpinBox::valueChanged), this, &TemporaryPrinterControlWidget::changeFanSpeed);
    connect(ui->printSpeedSB, static_cast<void (QSpinBox::*)(const int)>(&QSpinBox::valueChanged), this, &TemporaryPrinterControlWidget::changePrintSpeed);
    connect(ui->homeAllPB, &QPushButton::clicked, this, [=]{
        emit homeAll();
    });
    connect(ui->homeXPB, &QPushButton::clicked, this, [=]{
        emit home('X');
    });
    connect(ui->homeYPB, &QPushButton::clicked, this, [=]{
        emit home('Y');
    });
    connect(ui->homeZPB, &QPushButton::clicked, this, [=]{
        emit home('Z');
    });

    connect(ui->heatBedPB, &QPushButton::clicked, [=]{
       emit setHeatBed(ui->bedTempSB->value());
    });

    connect(ui->heatExtruderPB, &QPushButton::clicked, [=]{
        emit setHeatExtruder(ui->extruderCB->currentIndex(), ui->extruderTempSB->value());
    });

    connect(ui->x_leftPB, &QPushButton::clicked, [=]{
       emit moveAxis('X', ui->distanceSB->value());
    });

    connect(ui->x_rightPB, &QPushButton::clicked, [=]{
       emit moveAxis('X', ui->distanceSB->value());
    });

    connect(ui->y_upPB, &QPushButton::clicked, [=]{
       emit moveAxis('Y', ui->distanceSB->value());
    });

    connect(ui->y_downPB, &QPushButton::clicked, [=]{
       emit moveAxis('Y', ui->distanceSB->value());
    });

    connect(ui->z_downPB, &QPushButton::clicked, [=]{
       emit moveAxis('Z', ui->distanceSB->value());
    });

    connect(ui->z_upPB, &QPushButton::clicked, [=]{
       emit moveAxis('Z', ui->distanceSB->value());
    });
}

TemporaryPrinterControlWidget::~TemporaryPrinterControlWidget()
{
    delete ui;
}

void TemporaryPrinterControlWidget::updateProgressBar(float percent)
{
    ui->printProgressPB->setValue(percent);
}

void TemporaryPrinterControlWidget::updateTemperatures(float bed, float ext)
{
    ui->currentBedTempLB->setText(QVariant(bed).toString().append("ºC"));
    ui->currentExtTempLB->setText(QVariant(ext).toString().append("ºC"));
}

void TemporaryPrinterControlWidget::updateLog(QString msg){
    ui->logPT->appendPlainText(msg);
}
