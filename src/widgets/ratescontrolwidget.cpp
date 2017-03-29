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
