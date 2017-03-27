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

private:
    Ui::RatesControlWidget *ui;
};
