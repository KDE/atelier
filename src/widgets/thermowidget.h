#pragma once

#include <qwt_dial.h>
#include <qwt_dial_needle.h>

class ThermoWidget : public QwtDial {
    Q_OBJECT
public:
    ThermoWidget(QWidget *parent);
    void drawNeedle( QPainter *painter, const QPointF &center,
                     double radius, double dir, QPalette::ColorGroup colorGroup ) const;

    void setCurrentTemperature(double temperature);
    void setTargetTemperature(double temperature);
private:
    QwtDialSimpleNeedle *m_currentTemperatureNeedle;
    QwtDialSimpleNeedle *m_targetTemperatureNeedle;

    double m_currentTemperature;
    double m_targetTemperature;
};
