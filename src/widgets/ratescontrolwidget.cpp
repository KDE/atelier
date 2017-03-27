#include "ratescontrolwidget.h"
#include "ui_ratescontrolwidget.h"

RatesControlWidget::RatesControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RatesControlWidget)
{
    ui->setupUi(this);
}

RatesControlWidget::~RatesControlWidget()
{
    delete ui;
}
