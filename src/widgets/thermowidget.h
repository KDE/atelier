#pragma once

#include <qwt_dial.h>
#include <qwt_dial_needle.h>

class QKeyEvent;
class QPaintEvent;
class QFocusEvent;
class QWheelEvent;

class ThermoWidget : public QwtDial {
    Q_OBJECT

public:
    ThermoWidget(QWidget *parent, QString name);

    void drawNeedle( QPainter *painter, const QPointF &center,
                     double radius, double dir, QPalette::ColorGroup colorGroup ) const;

    void setCurrentTemperature(double temperature);
    void setTargetTemperature(double temperature);

signals:
    void targetTemperatureChanged(double targetTemperature);

protected:
    void focusOutEvent(QFocusEvent *event);
    void keyPressEvent (QKeyEvent *event);
    void paintEvent(QPaintEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    QwtDialSimpleNeedle *m_currentTemperatureNeedle;
    QwtDialSimpleNeedle *m_targetTemperatureNeedle;

    QString m_currentTemperatureTextFromEditor;
    QString m_name;

    double m_currentTemperature;
    double m_targetTemperature;
};
