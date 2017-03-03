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
    void setExtruderCount(int value);
    void updateBedTemp(const float temp);
    void updateExtTemp(const float temp);
    void updateBedTargetTemp(const float temp);
    void updateExtTargetTemp(const float temp);
    void stopHeating();

private:
    Ui::BedExtruderWidget *ui;
    QMap <int, QRadioButton*> extruderMap;
    void heatExtruderClicked(bool clicked);
    void heatBedClicked(bool clicked);
    int currentExtruder();
    int extruderCount = 0;

signals:
    void bedTemperatureChanged(int tmp);
    void extTemperatureChanged(int tmp, int currExt);
};
