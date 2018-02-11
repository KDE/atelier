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

    qDebug() << "############################";
    qDebug() << "Drawing for" << minScaleArc() << " to " << maxScaleArc() << "and values" << lowerBound() << "and" << upperBound();

    const double startAngle = minScaleArc();
    const double endAngle = maxScaleArc();


    const double minValue = lowerBound();
    const double maxValue = upperBound();

    const double relativePercent = maxValue - minValue;

    const double currentTemperaturePercent = (m_currentTemperature - minValue) / relativePercent;
    const double targetTemperaturePercent = (m_targetTemperature - minValue) / relativePercent;

    qDebug() << "Values" << m_currentTemperature << m_targetTemperature;
    qDebug() << "Percentages" << currentTemperaturePercent << targetTemperaturePercent;

    const double currentTemperatureAngle = (endAngle - startAngle) * currentTemperaturePercent + startAngle;
    const double targetTemperatureAngle = (endAngle - startAngle) * targetTemperaturePercent + startAngle;

    qDebug() << "Angles" << currentTemperatureAngle << targetTemperatureAngle;

    m_targetTemperatureNeedle->draw(painter, center, radius, 360 - currentTemperatureAngle - origin(), colorGroup);
    m_currentTemperatureNeedle->draw(painter, center, radius, 360 - targetTemperatureAngle - origin(), colorGroup);
}

void ThermoWidget::setCurrentTemperature(double temperature)
{
    m_currentTemperature = temperature;
}

void ThermoWidget::setTargetTemperature(double temperature)
{
    m_targetTemperature = temperature;
}
