#pragma once

#include <QWidget>
#include <QRadioButton>
#include <QMap>

namespace Ui {
class BedExtruderWidget;
}

class BedExtruderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BedExtruderWidget(QWidget *parent = nullptr);
    ~BedExtruderWidget();
    void extruderCountChanged(int value);
    void updateBedTemp(const float temp);
    void updateExtTemp(const float temp);

private:
    Ui::BedExtruderWidget *ui;
    QMap <int, QRadioButton*> extruderMap;
    void heatExtruderClicked(bool clicked);
    int extruderCount = 0;
    void reset();

signals:
    void setBedTemperature(int tmp);
    void setExtTemperature(int currExt, int tmp);
};
