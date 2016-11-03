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

private:
    Ui::TemporaryPrinterControlWidget *ui;
};
