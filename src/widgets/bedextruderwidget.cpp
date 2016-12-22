#include "bedextruderwidget.h"
#include "ui_bedextruderwidget.h"

BedExtruderWidget::BedExtruderWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BedExtruderWidget)
{
    ui->setupUi(this);
}

BedExtruderWidget::~BedExtruderWidget()
{
    delete ui;
}
