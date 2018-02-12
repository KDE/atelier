#include "thermowidget.h"
#include <QKeyEvent>
#include <QPaintEvent>
#include <QFocusEvent>
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QWheelEvent>

ThermoWidget::ThermoWidget(QWidget *parent) : QwtDial(parent),
    m_currentTemperatureNeedle(new QwtDialSimpleNeedle(QwtDialSimpleNeedle::Arrow)),
    m_targetTemperatureNeedle(new QwtDialSimpleNeedle(QwtDialSimpleNeedle::Arrow, Qt::red, Qt::darkRed)),
    m_currentTemperature(0),
    m_targetTemperature(0)
{
    setScaleArc(40, 320);
    setNeedle(m_currentTemperatureNeedle);
    setReadOnly(false);
    setFocusPolicy(Qt::StrongFocus);
}

void ThermoWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9) {
        auto tmp = m_currentTemperatureTextFromEditor + event->key();
        if (tmp.toInt() <= upperBound() && tmp.toInt() >= lowerBound()) {
            m_currentTemperatureTextFromEditor = tmp;
        }
    } else if (event->key() == Qt::Key_Backspace && m_currentTemperatureTextFromEditor.count()) {
        m_currentTemperatureTextFromEditor.remove(m_currentTemperatureTextFromEditor.count() - 1, 1);
    } else if (event->key() == Qt::Key_Enter) {
        m_targetTemperature = m_currentTemperatureTextFromEditor.toInt();
    } else if (event->key() == Qt::Key_Escape) {
        m_currentTemperatureTextFromEditor = '0';
    } else if (event->key() == Qt::Key_Up || event->key() == Qt::Key_Right) {
        if (m_targetTemperature != upperBound()) {
            m_currentTemperatureTextFromEditor = QString::number(m_targetTemperature + 1);
        }
    } else if (event->key() == Qt::Key_Down || event->key() == Qt::Key_Left) {
        if (m_targetTemperature != lowerBound()) {
            m_currentTemperatureTextFromEditor = QString::number(m_targetTemperature - 1);
        }
    } else if (event->key() == Qt::Key_PageUp) {
        if (m_targetTemperature + 10 > upperBound()) {
            m_currentTemperatureTextFromEditor = QString::number(upperBound());
        } else {
            m_currentTemperatureTextFromEditor = QString::number(m_targetTemperature + 10);
        }
    } else if (event->key() == Qt::Key_PageDown) {
        if (m_targetTemperature - 10 < lowerBound()) {
            m_currentTemperatureTextFromEditor = QString::number(lowerBound());
        } else {
            m_currentTemperatureTextFromEditor = QString::number(m_targetTemperature - 10);
        }
    }

    else {
        QwtDial::keyPressEvent(event);
        return;
    }

    if (m_targetTemperature != m_currentTemperatureTextFromEditor.toInt()) {
        m_targetTemperature = m_currentTemperatureTextFromEditor.toInt();
        emit targetTemperatureChanged(m_targetTemperature);
        update();
        event->accept();
    }
}

void ThermoWidget::wheelEvent(QWheelEvent* event)
{
    if (event->angleDelta().y() > 0) {
        if (m_targetTemperature + 10 > upperBound()) {
            m_currentTemperatureTextFromEditor = QString::number(upperBound());
        } else {
            m_currentTemperatureTextFromEditor = QString::number(m_targetTemperature + 10);
        }
    } else if (event->angleDelta().y() < 0) {
        if (m_targetTemperature - 10 < lowerBound()) {
            m_currentTemperatureTextFromEditor = QString::number(lowerBound());
        } else {
            m_currentTemperatureTextFromEditor = QString::number(m_targetTemperature - 10);
        }
    }

    if (m_targetTemperature != m_currentTemperatureTextFromEditor.toInt()) {
        m_targetTemperature = m_currentTemperatureTextFromEditor.toInt();
        emit targetTemperatureChanged(m_targetTemperature);
        update();
    }
    event->accept();
}

void ThermoWidget::focusOutEvent(QFocusEvent* event)
{
    if (m_targetTemperature != m_currentTemperatureTextFromEditor.toInt()) {
        m_targetTemperature = m_currentTemperatureTextFromEditor.toInt();
        emit targetTemperatureChanged(m_targetTemperature);
        update();
        event->accept();
    }
}


void ThermoWidget::paintEvent(QPaintEvent* event)
{
    QwtDial::paintEvent(event);
    const QString currentText = QString::number(m_currentTemperature);

    QFontMetrics fm(font());
    const double targetWidth = fm.width(m_currentTemperatureTextFromEditor);
    const double currentWidth = fm.width(currentText);

    const double height = fm.height();
    const double halfWidth = geometry().width() / 2;
    const double xposTarget = halfWidth - (targetWidth / 2);
    const double xposCurrent = halfWidth - (currentWidth / 2);
    const double ypos = geometry().height() - height * 2  - 2;

    QPainter p(this);

    p.setBrush(Qt::red);
    p.setPen(Qt::red);
    p.drawText(xposTarget, ypos - height, m_currentTemperatureTextFromEditor);

    p.setBrush(Qt::white);
    p.setPen(Qt::white);
    p.drawText(xposCurrent, ypos, QString::number(m_currentTemperature));
}

void ThermoWidget::drawNeedle( QPainter *painter, const QPointF &center, double radius, double dir, QPalette::ColorGroup colorGroup ) const
{
    Q_UNUSED( dir );

    const double relativePercent = upperBound() - lowerBound();
    const double currentTemperaturePercent = (m_currentTemperature - lowerBound()) / relativePercent;
    const double targetTemperaturePercent = (m_targetTemperature - lowerBound()) / relativePercent;
    const double currentTemperatureAngle = (maxScaleArc() - minScaleArc()) * currentTemperaturePercent + minScaleArc();
    const double targetTemperatureAngle = (maxScaleArc() - minScaleArc()) * targetTemperaturePercent + minScaleArc();

    m_targetTemperatureNeedle->draw(painter, center, radius, 360 - targetTemperatureAngle - origin(), colorGroup);
    m_currentTemperatureNeedle->draw(painter, center, radius, 360 - currentTemperatureAngle - origin(), colorGroup);
}

void ThermoWidget::setCurrentTemperature(double temperature)
{
    if (m_currentTemperature != temperature) {
        m_currentTemperature = temperature;
        update();
    }
}

void ThermoWidget::setTargetTemperature(double temperature)
{
    if (m_targetTemperature != temperature) {
        m_currentTemperatureTextFromEditor = QString::number(temperature);
        m_targetTemperature = temperature;
        emit targetTemperatureChanged(m_targetTemperature);
        update();
    }
}
