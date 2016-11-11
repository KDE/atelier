#include "temporaryprintercontrolwidget.h"
#include "ui_temporaryprintercontrolwidget.h"

TemporaryPrinterControlWidget::TemporaryPrinterControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TemporaryPrinterControlWidget)
{
    ui->setupUi(this);
    connect(ui->fanSpeedSB, static_cast<void (QSpinBox::*)(const int)>(&QSpinBox::valueChanged), this, &TemporaryPrinterControlWidget::changeFanSpeed);
    connect(ui->printSpeedSB, static_cast<void (QSpinBox::*)(const int)>(&QSpinBox::valueChanged), this, &TemporaryPrinterControlWidget::changePrintSpeed);
    connect(ui->homeAllPB, &QPushButton::clicked, this, &TemporaryPrinterControlWidget::homeAll);
    connect(ui->homeXPB, &QPushButton::clicked, this, &TemporaryPrinterControlWidget::homeX);
    connect(ui->homeYPB, &QPushButton::clicked, this, &TemporaryPrinterControlWidget::homeY);
    connect(ui->homeZPB, &QPushButton::clicked, this, &TemporaryPrinterControlWidget::homeZ);

    connect(ui->heatBedPB, &QPushButton::clicked, [=]{
       emit setHeatBed(ui->bedTempSB->value());
    });

    connect(ui->heatExtruderPB, &QPushButton::clicked, [=]{
        emit setHeatExtruder(ui->extruderCB->currentIndex(), ui->extruderTempSB->value());
    });

    connect(ui->x_leftPB, &QPushButton::clicked, [=]{
       emit moveAxis(QLatin1Char('X'), ui->distanceSB->value());
    });

    connect(ui->x_rightPB, &QPushButton::clicked, [=]{
       emit moveAxis(QLatin1Char('X'), ui->distanceSB->value());
    });

    connect(ui->y_upPB, &QPushButton::clicked, [=]{
       emit moveAxis(QLatin1Char('Y'), ui->distanceSB->value());
    });

    connect(ui->y_downPB, &QPushButton::clicked, [=]{
       emit moveAxis(QLatin1Char('Y'), ui->distanceSB->value());
    });

    connect(ui->z_downPB, &QPushButton::clicked, [=]{
       emit moveAxis(QLatin1Char('Z'), ui->distanceSB->value());
    });

    connect(ui->z_upPB, &QPushButton::clicked, [=]{
       emit moveAxis(QLatin1Char('Z'), ui->distanceSB->value());
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
