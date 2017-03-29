#pragma once

#include <QWidget>

namespace Ui {
class RatesControlWidget;
}

class RatesControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RatesControlWidget(QWidget *parent = nullptr);
    ~RatesControlWidget();
signals:
    void flowRateChanged(int value);
    void printSpeedChanged(int value);
    void fanSpeedChanged(int value, uint fan = 0);

private:
    Ui::RatesControlWidget *ui;
};
