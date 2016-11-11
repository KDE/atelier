#pragma once

#include <QWidget>

namespace Ui {
class TemporaryPrinterControlWidget;
}

class TemporaryPrinterControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TemporaryPrinterControlWidget(QWidget *parent = nullptr);
    ~TemporaryPrinterControlWidget();
    void updateProgressBar(float percent);
    void updateTemperatures(float bed, float ext);
    void updateLog(QString msg);

private:
    Ui::TemporaryPrinterControlWidget *ui;

signals:
    void setHeatBed(int temp);
    void setHeatExtruder(int ext, int temp);
    void changeFanSpeed(int value);
    void changePrintSpeed(int value);
    void homeAll();
    void homeX();
    void homeY();
    void homeZ();
    void moveAxis(QLatin1Char axis, int dist);
};
