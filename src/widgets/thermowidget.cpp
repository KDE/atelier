#include "thermowidget.h"

ThermoWidget::ThermoWidget(QWidget *parent) : QwtDial(parent),
    m_currentTemperatureNeedle(new QwtDialSimpleNeedle(QwtDialSimpleNeedle::Arrow)),
    m_targetTemperatureNeedle(new QwtDialSimpleNeedle(QwtDialSimpleNeedle::Arrow))
{
    setScaleArc(40, 320);
    setNeedle(m_currentTemperatureNeedle);

    m_currentTemperature = 100;
    m_targetTemperature = 120;

}

void ThermoWidget::drawNeedle( QPainter *painter, const QPointF &center, double radius, double dir, QPalette::ColorGroup colorGroup ) const
{
    Q_UNUSED( dir );

    const double startAngle = minScaleArc();
    const double endAngle = maxScaleArc();


    const double minValue = lowerBound();
    const double maxValue = upperBound();

    const double relativePercent = maxValue - minValue;

    const double currentTemperaturePercent = (m_currentTemperature - minValue) / relativePercent;
    const double targetTemperaturePercent = (m_targetTemperature - minValue) / relativePercent;

    const double currentTemperatureAngle = (endAngle - startAngle) * currentTemperaturePercent + startAngle;
    const double targetTemperatureAngle = (endAngle - startAngle) * targetTemperatureAngle + startAngle;

    m_targetTemperatureNeedle->draw(painter, center, radius, currentTemperatureAngle, colorGroup);
    m_currentTemperatureNeedle->draw(painter, center, radius, targetTemperatureAngle, colorGroup);
}

void ThermoWidget::setCurrentTemperature(double temperature)
{
    m_currentTemperature = temperature;
}

void ThermoWidget::setTargetTemperature(double temperature)
{
    m_targetTemperature = temperature;
}
