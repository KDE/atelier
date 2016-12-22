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
    extruderCountChanged(1);
    connect(ui->heatBedPB, &QPushButton::clicked, [ = ](bool clicked) {
        int temp = ui->bedTempSB->value()*clicked;
        emit setBedTemperature(temp);
        ui->bedTargetTempLB->setText(QString::number(temp));
    });

    connect(ui->heatExtPB, &QPushButton::clicked, this, &BedExtruderWidget::heatExtruderClicked);
}

BedExtruderWidget::~BedExtruderWidget()
{
    delete ui;
}

void BedExtruderWidget::extruderCountChanged(int value)
{
    //To prevent errors
    if (extruderCount == value) {
        return;
    } else if (extruderCount < value) {
        //loop for the new buttons
        for (int i = extruderCount; i <= value; i++) {
            auto *rb = new QRadioButton(QString::number(i+1));
            ui->extRadioButtonLayout->addWidget(rb);
            extruderMap.insert(i, rb);
        }
    } else {
        //remove buttons
        for (int i = extruderCount; i >= value; i--) {
            auto *rb = extruderMap.value(i);
            ui->extRadioButtonLayout->removeWidget(rb);
            extruderMap.remove(i);
            delete(rb);
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

void BedExtruderWidget::heatExtruderClicked(bool clicked)
{
    int currExt = 1;
    if (extruderMap.size() > 1) {
        for (int i = 0; i < extruderMap.size(); i++) {
            if (extruderMap.value(i)->isChecked()) {
                currExt = i;
                break;
            }
        }
    }

    if (clicked) {
        int tmp = ui->extTempSB->value();
        emit setExtTemperature(currExt, tmp);
        ui->extTargetTempLB->setText(QString::number(tmp));

    }else {
        emit setExtTemperature(currExt, 0);
        ui->extTargetTempLB->setText(QString::number(0));
    }
}

void BedExtruderWidget::reset()
{
    for (int i = 1; i <= extruderMap.size(); i++) {
        ui->extRadioButtonLayout->removeWidget(extruderMap[i]);
    }
    extruderMap.clear();
}
