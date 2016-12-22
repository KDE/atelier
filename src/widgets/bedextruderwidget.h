#ifndef BEDEXTRUDERWIDGET_H
#define BEDEXTRUDERWIDGET_H

#include <QWidget>

namespace Ui {
class BedExtruderWidget;
}

class BedExtruderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BedExtruderWidget(QWidget *parent = 0);
    ~BedExtruderWidget();

private:
    Ui::BedExtruderWidget *ui;
};

#endif // BEDEXTRUDERWIDGET_H
