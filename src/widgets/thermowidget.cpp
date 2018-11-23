/* Atelier KDE Printer Host for 3D Printing
    Copyright (C) <2018>
    Author: Tomaz Canabrava - tcanabrava@kde.org
            Chris Rizzitello - rizzitello@kde.org
            Lays Rodrigues - lays.rodrigues@kde.org

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <QBrush>
#include <QFocusEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QPen>
#include <QTimer>
#include <QWheelEvent>
#include "thermowidget.h"

ThermoWidget::ThermoWidget(QWidget *parent, const QString &name) :
    QwtDial(parent)
    , m_targetTemperatureNeedle(new QwtDialSimpleNeedle(QwtDialSimpleNeedle::Arrow, false, Qt::red, Qt::darkRed))
    , m_name(name)
    , m_tempChangedTimer(new QTimer(this))
    , m_currentTemperature(0)
    , m_targetTemperature(0)
{
    setScaleArc(40, 320);
    //make our current temperature needle here so we can set it to match text color.
    m_currentTemperatureNeedle = new QwtDialSimpleNeedle(QwtDialSimpleNeedle::Arrow, true, palette().text().color());
    setNeedle(m_currentTemperatureNeedle);
    setReadOnly(false);
    setFocusPolicy(Qt::StrongFocus);

    m_cursorTimer = new QTimer(this);
    connect(m_cursorTimer, &QTimer::timeout, this, [this] {
        m_paintCursor = !m_paintCursor;
        update();
    });

    m_tempChangedTimer->setSingleShot(true);
    connect(m_tempChangedTimer, &QTimer::timeout, this, [this] {
        emit targetTemperatureChanged(m_targetTemperature);
    });
}

void ThermoWidget::keyPressEvent(QKeyEvent *event)
{
    //set our target text length.
    int slen = m_currentTemperatureTextFromEditor.length() - 1;
    // be sure our cursor position is valid.
    if (slen < 0) {
        m_currentTemperatureTextFromEditor = '-';
        m_cursorPos = 0;
    } else if (slen > 2) {
        m_cursorPos = 2;
    }
    //parse the key events.
    if (event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9) {
        auto tmp = m_currentTemperatureTextFromEditor;

        if (m_cursorPos == slen) {
            tmp.append(event->key());
        } else {
            tmp.insert(m_cursorPos, event->key());
        }

        if (tmp.startsWith('0')) {
            tmp.remove(0, 1);
        }

        if (tmp.contains('-')) {
            tmp.remove('-');
            //push the cursor back to negate advancement;
            m_cursorPos--;
        }
        if (tmp.toInt() <= upperBound() && tmp.toInt() >= lowerBound()) {
            m_currentTemperatureTextFromEditor = tmp;
            if (m_cursorPos <= slen) {
                m_cursorPos++;
            }
        }
    } else if (event->key() == Qt::Key_Delete && m_currentTemperatureTextFromEditor.count()) {
        m_currentTemperatureTextFromEditor.remove(m_cursorPos, 1);
        if (m_cursorPos < slen) {
            m_cursorPos = slen;
        }
        m_cursorPos--;
    } else if (event->key() == Qt::Key_Backspace && m_currentTemperatureTextFromEditor.count()) {
        if (m_cursorPos <= slen) {
            m_cursorPos--;
            m_currentTemperatureTextFromEditor.remove(m_cursorPos, 1);
        }
    } else if (event->key() == Qt::Key_Enter) {
        m_targetTemperature = m_currentTemperatureTextFromEditor.toInt();
    } else if (event->key() == Qt::Key_Escape) {
        m_currentTemperatureTextFromEditor = '0';
    } else if (event->key() == Qt::Key_Up || event->key() == Qt::Key_Plus) {
        if (!isEqual(m_targetTemperature, upperBound())) {
            m_currentTemperatureTextFromEditor = QString::number(m_targetTemperature + 1);
        }
    } else if (event->key() == Qt::Key_Down || event->key() == Qt::Key_Minus) {
        if (!isEqual(m_targetTemperature, lowerBound())) {
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
    } else if (event->key() == Qt::Key_Right) {
        if (m_cursorPos < slen) {
            m_cursorPos++;
        }
    } else if (event->key() == Qt::Key_Left) {
        if (m_cursorPos > 0) {
            m_cursorPos--;
        }
    } else {
        QwtDial::keyPressEvent(event);
        return;
    }

    if (m_targetTemperature != m_currentTemperatureTextFromEditor.toInt()) {
        m_targetTemperature = m_currentTemperatureTextFromEditor.toInt();
        resetTimer();
        update();
        event->accept();
    }
}

void ThermoWidget::wheelEvent(QWheelEvent *event)
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
        resetTimer();
        update();
    }
    event->accept();
}

void ThermoWidget::focusOutEvent(QFocusEvent *event)
{
    if (m_targetTemperature != m_currentTemperatureTextFromEditor.toInt()) {
        m_targetTemperature = m_currentTemperatureTextFromEditor.toInt();
        resetTimer();
        event->accept();
    }
    m_cursorTimer->stop();
    m_paintCursor = false;
    update();
}

void ThermoWidget::focusInEvent(QFocusEvent *event)
{
    m_cursorTimer->start(1000);
    event->accept();
}

void ThermoWidget::paintEvent(QPaintEvent *event)
{
    QwtDial::paintEvent(event);
    const QString currentText = QString::number(m_currentTemperature);

    QFontMetrics fm(font());
    const int targetWidth = fm.width(m_currentTemperatureTextFromEditor);
    const int currentWidth = fm.width(currentText);
    const int nameWidth = fm.width(m_name);
    const int wWidth = fm.width('W');
    const int cursorWidth = fm.width('0');
    const int height = fm.height();
    const int halfWidth = geometry().width() / 2;
    const int xposTarget = int(halfWidth - (targetWidth / 2));
    const int xposCurrent = int(halfWidth - (currentWidth / 2));
    const int xposName = halfWidth - (nameWidth / 2);
    const int xposCursor = xposTarget + (cursorWidth * m_cursorPos);
    int ypos = geometry().height() / 2 + height * 2;
    QPainter p(this);
    QColor color = palette().color(QPalette::Text);
    //draw a box to put our target into as a user hint.
    p.fillRect(QRect(int(halfWidth - wWidth), int(ypos - (height * 0.66)), int(wWidth * 2), int(height * 0.9)), palette().color(QPalette::AlternateBase));

    if (m_paintCursor) {
        p.setPen(palette().color(QPalette::Text));
        p.drawText(xposCursor, ypos, QChar('_'));
    }

    p.setPen(Qt::red);
    p.drawText(xposTarget, ypos, m_currentTemperatureTextFromEditor);
    ypos += height + 2;

    p.setPen(color);
    p.drawText(xposCurrent, ypos, QString::number(m_currentTemperature));

    p.setPen(color);
    if (size().height() <= height * 6.5 && innerRect().width() <= nameWidth * 4) {
        p.drawText(xposName, 0 + height, m_name);
    } else if (size().height() >= height * 8) {
        p.drawText(xposName, ypos + height + 2, m_name);
    } else {
        p.drawText(xposName, geometry().height() / 2 - 12, m_name);
    }
}

void ThermoWidget::drawNeedle(QPainter *painter, const QPointF &center, double radius, double dir, QPalette::ColorGroup colorGroup) const
{
    Q_UNUSED(dir);
    //save a copy of radius as int to avoid casting it several times.
    int radiusAsInt = int(radius);
    const double relativePercent = upperBound() - lowerBound();
    const double currentTemperaturePercent = (m_currentTemperature - lowerBound()) / relativePercent;
    const double targetTemperaturePercent = (m_targetTemperature - lowerBound()) / relativePercent;
    const double currentTemperatureAngle = (maxScaleArc() - minScaleArc()) * currentTemperaturePercent + minScaleArc();
    const double targetTemperatureAngle = (maxScaleArc() - minScaleArc()) * targetTemperaturePercent + minScaleArc();
    // Qt coordinates and Qwt coordinates differ.
    // the "begin" of our coordinates in Qt: -130
    // the "span" of our coordinates in Qt: -180
    // Negative values means clockwise in Qt dialect.
    const int qtBeginAngle = -130;
    const int coolZone =  int (- (targetTemperatureAngle - minScaleArc()));
    int yPos = geometry().height() / 2 - radiusAsInt;
    int xPos = geometry().width() / 2 - radiusAsInt;

    QRadialGradient grad(center, radius);
    grad.setColorAt(0.75, QColor(0, 0, 0, 0));
    grad.setColorAt(0.85, QColor(255, 0, 0, 196));
    grad.setColorAt(0.95, QColor(255, 110, 60, 196));

    painter->setBrush(grad);
    painter->drawPie(xPos, yPos, radiusAsInt * 2, radiusAsInt * 2, qtBeginAngle * 16, coolZone * 16);

    m_targetTemperatureNeedle->draw(painter, center, radius * 1.3, 360 - targetTemperatureAngle - origin(), colorGroup);
    m_currentTemperatureNeedle->draw(painter, center, radius, 360 - currentTemperatureAngle - origin(), colorGroup);
}

void ThermoWidget::setCurrentTemperature(double temperature)
{
    if (!isEqual(m_currentTemperature, temperature)) {
        m_currentTemperature = temperature;
        update();
    }
}

void ThermoWidget::setTargetTemperature(int temperature)
{
    if (m_targetTemperature != temperature) {
        m_currentTemperatureTextFromEditor = QString::number(temperature);
        m_targetTemperature = temperature;
        resetTimer();
        update();
    }
}

void ThermoWidget::resetTimer()
{
    m_tempChangedTimer->start(500);
}

bool ThermoWidget::isEqual(double a, double b)
{
//qFuzzyCompare always returns false if a || b ==0
    if (qFuzzyIsNull(a) || qFuzzyIsNull(b)) {
        if (a < 0.0 || b < 0.0) {
            //One number is 0 and the other negative
            //to prevent a issue if a or b == -1 and the other 0
            //we will subtract one from each value
            a -= 1;
            b -= 1;
        } else {
            a += 1;
            b += 1;
        }
    }
    return qFuzzyCompare(a, b);
}
