#include "temporaryprintercontrolwidget.h"
#include "ui_temporaryprintercontrolwidget.h"

TemporaryPrinterControlWidget::TemporaryPrinterControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TemporaryPrinterControlWidget)
{
    ui->setupUi(this);
}

TemporaryPrinterControlWidget::~TemporaryPrinterControlWidget()
{
    delete ui;
}
