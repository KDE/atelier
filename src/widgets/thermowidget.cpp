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
#include "thermowidget.h"
#include <QApplication>
#include <QBrush>
#include <QFocusEvent>
#include <QKeyEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPen>
#include <QTimer>
#include <QWheelEvent>

ThermoWidget::ThermoWidget(QWidget *parent, const QString &name)
    : QcGaugeWidget(parent)
    , m_tempChangedTimer(new QTimer(this))
    , m_currentTemperature(0)
    , m_targetTemperature(0)
{
    setFocusPolicy(Qt::StrongFocus);
    QcBackgroundItem *bkground = addBackground(100);
    bkground->clearrColors();
    bkground->addColor(0.0, palette().light().color());

    auto bk2 = addBackground(98);
    bk2->clearrColors();
    bk2->addColor(0.0, palette().dark().color());

    auto arc = addArc(70);
    arc->setColor(palette().midlight().color());
    m_ticksItem = addDegrees(70);
    m_ticksItem->setColor(Qt::red);

    m_subTicksItem = addDegrees(70);
    m_subTicksItem->setSubDegree(true);
    m_subTicksItem->setStep(5);
    m_subTicksItem->setColor(palette().text().color());

    m_nameLabel = addLabel(80);
    m_nameLabel->setFont(font());
    m_nameLabel->setText(name);
    m_nameLabel->setColor(palette().text().color());
    m_valueDisplay = addValues(85);
    m_valueDisplay->setFont(font());
    m_valueDisplay->setColor(palette().text().color());

    m_targetTemperatureNeedle = addNeedle(60);
    m_targetTemperatureNeedle->setPosition(95);
    m_targetTemperatureNeedle->setColor(Qt::red);

    QcLabelItem *lab = addLabel(60);
    lab->setText(QString::number(m_currentTemperature));
    lab->setColor(palette().text().color());

    m_currentTemperatureNeedle = addNeedle(60);
    m_currentTemperatureNeedle->setPosition(65);
    m_currentTemperatureNeedle->setColor(palette().text().color());
    m_currentTemperatureNeedle->setLabel(lab);
    addBackground(7);

    m_cursorTimer = new QTimer(this);
    connect(m_cursorTimer, &QTimer::timeout, this, [this] {
        m_paintCursor = !m_paintCursor;
        update();
    });

    m_tempChangedTimer->setSingleShot(true);
    connect(m_tempChangedTimer, &QTimer::timeout, this, [this] { emit targetTemperatureChanged(m_targetTemperature); });
}

void ThermoWidget::setRange(int min, int max)
{
    m_maxValue = max;
    m_minValue = min;
    m_currentTemperatureNeedle->setValueRange(min, max);
    m_targetTemperatureNeedle->setValueRange(min, max);
    m_valueDisplay->setValueRange(min, max);
    int range = max - min;
    if (range <= 150) {
        m_valueDisplay->setStep(10);
        m_ticksItem->setStep(10);
    } else if (range <= 300) {
        m_valueDisplay->setStep(25);
        m_ticksItem->setStep(25);
    } else {
        m_valueDisplay->setStep((max - min) / 10);
        m_ticksItem->setStep((max - min) / 10);
    }
    m_ticksItem->setValueRange(min, max);
}

void ThermoWidget::setName(const QString &name)
{
    m_nameLabel->setText(name);
}

void ThermoWidget::setCurrentTemperature(double temperature)
{
    if (!isEqual(m_currentTemperature, temperature)) {
        m_currentTemperature = temperature;
        m_currentTemperatureNeedle->setCurrentValue(float(temperature));
        update();
    }
}

void ThermoWidget::setTargetTemperature(int temperature)
{
    if (m_targetTemperature != temperature) {
        m_targetTemperatureText = QString::number(temperature);
        m_targetTemperature = temperature;
        m_targetTemperatureNeedle->setCurrentValue(temperature);
        resetTimer();
        update();
    }
}
void ThermoWidget::keyPressEvent(QKeyEvent *event)
{
    // set our target text length.
    int slen = m_targetTemperatureText.length() - 1;
    // be sure our cursor position is valid.
    if (slen < 0) {
        m_targetTemperatureText = QStringLiteral("0");
        m_cursorPos = 0;
    } else if (slen > 2) {
        m_cursorPos = 2;
    }
    // parse the key events.
    if (event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9) {
        auto tmp = QString::number(m_targetTemperature);

        if (m_cursorPos == slen) {
            tmp.append(QChar(event->key()));
        } else {
            tmp.insert(m_cursorPos, QChar(event->key()));
        }

        if (tmp.toInt() <= m_maxValue && tmp.toInt() >= m_minValue) {
            m_targetTemperatureText = tmp;
            if (m_cursorPos <= slen) {
                m_cursorPos++;
            }
        }
    } else if (event->key() == Qt::Key_Delete && m_targetTemperatureText.count()) {
        m_targetTemperatureText.remove(m_cursorPos, 1);
        if (m_cursorPos < slen) {
            m_cursorPos = slen;
        }
        m_cursorPos--;
    } else if (event->key() == Qt::Key_Backspace && m_targetTemperatureText.count()) {
        if (m_cursorPos <= slen) {
            m_cursorPos--;
            m_targetTemperatureText.remove(m_cursorPos, 1);
        }
    } else if (event->key() == Qt::Key_Enter) {
        m_targetTemperature = m_targetTemperatureText.toInt();
    } else if (event->key() == Qt::Key_Escape) {
        m_targetTemperatureText = QString::number(m_targetTemperature);
    } else if (event->key() == Qt::Key_Up || event->key() == Qt::Key_Plus) {
        if (!isEqual(m_targetTemperature, m_maxValue)) {
            m_targetTemperatureText = QString::number(m_targetTemperature + 1);
        }
    } else if (event->key() == Qt::Key_Down || event->key() == Qt::Key_Minus) {
        if (!isEqual(m_targetTemperature, m_minValue)) {
            m_targetTemperatureText = QString::number(m_targetTemperature - 1);
        }
    } else if (event->key() == Qt::Key_PageUp) {
        if (m_targetTemperature + 10 > m_maxValue) {
            m_targetTemperatureText = QString::number(m_maxValue);
        } else {
            m_targetTemperatureText = QString::number(m_targetTemperature + 10);
        }
    } else if (event->key() == Qt::Key_PageDown) {
        if (m_targetTemperature - 10 < m_minValue) {
            m_targetTemperatureText = QString::number(m_minValue);
        } else {
            m_targetTemperatureText = QString::number(m_targetTemperature - 10);
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
        QcGaugeWidget::keyPressEvent(event);
    }

    if (m_cursorPos < 0) {
        m_cursorPos = 0;
    } else if (m_cursorPos > 2) {
        m_cursorPos = 2;
    }

    if (m_targetTemperatureText.simplified().isEmpty()) {
        m_targetTemperatureText = QStringLiteral("0");
    }

    if (m_targetTemperature != m_targetTemperatureText.toInt()) {
        setTargetTemperature(m_targetTemperatureText.toInt());
        resetTimer();
        update();
    }
    event->accept();
}

void ThermoWidget::wheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y() > 0) {
        if (m_targetTemperature + 10 > m_maxValue) {
            m_targetTemperatureText = QString::number(m_maxValue);
        } else {
            m_targetTemperatureText = QString::number(m_targetTemperature + 10);
        }
    } else if (event->angleDelta().y() < 0) {
        if (m_targetTemperature - 10 < m_minValue) {
            m_targetTemperatureText = QString::number(m_minValue);
        } else {
            m_targetTemperatureText = QString::number(m_targetTemperature - 10);
        }
    }

    if (m_targetTemperature != m_targetTemperatureText.toInt()) {
        setTargetTemperature(m_targetTemperatureText.toInt());
        resetTimer();
        update();
    }
    event->accept();
}

void ThermoWidget::focusOutEvent(QFocusEvent *event)
{
    if (m_targetTemperature != m_targetTemperatureText.toInt()) {
        setTargetTemperature(m_targetTemperatureText.toInt());
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

void ThermoWidget::resetTimer()
{
    m_tempChangedTimer->start(500);
}

void ThermoWidget::paintEvent(QPaintEvent *event)
{
    QcGaugeWidget::paintEvent(event);

    int radiusAsInt = 0;
    if (rect().width() < rect().height()) {
        radiusAsInt = int(rect().width() / 2.0);
    } else {
        radiusAsInt = int(rect().height() / 2.0);
    }

    QFont font(QApplication::font().family(), radiusAsInt / 10, QFont::Normal);
    QFontMetrics fm(font);
    const int targetWidth = fm.horizontalAdvance(m_targetTemperatureText);
    const int wWidth = fm.horizontalAdvance('W');
    const int cursorWidth = fm.horizontalAdvance('0');
    const int height = fm.height();
    const int halfWidth = geometry().width() / 2;
    const int xposTarget = int(halfWidth - (targetWidth / 2));
    const int xposCursor = xposTarget + (cursorWidth * m_cursorPos);
    int ypos = geometry().height() / 2 + height * 2;
    const double relativePercent = m_maxValue - m_minValue;
    const double targetTemperaturePercent = (m_targetTemperature - m_minValue) / relativePercent;
    const double targetTemperatureAngle = (225 - -45) * targetTemperaturePercent + -45;
    const int coolZone = int(-(targetTemperatureAngle - -45));
    const int qtBeginAngle = -135;
    int PieYPos = geometry().height() / 2 - radiusAsInt;
    int PieXPos = geometry().width() / 2 - radiusAsInt;

    QPainter p(this);
    p.setFont(font);
    // draw a box to put our target into as a user hint.
    p.fillRect(QRect(int(halfWidth - wWidth), int(ypos - (height * 1.2)), int(wWidth * 2), int(height * 0.8)), palette().color(QPalette::AlternateBase));

    if (m_paintCursor) {
        p.setPen(palette().color(QPalette::Text));
        p.drawText(xposCursor, int(ypos - (height * 0.60)), QChar('_'));
    }

    p.setPen(Qt::red);
    p.drawText(xposTarget, int(ypos - (height * 0.60)), m_targetTemperatureText);

    QRadialGradient grad(rect().center(), radiusAsInt);
    grad.setColorAt(0.35, QColor(0, 0, 0, 0));
    grad.setColorAt(0.45, QColor(255, 110, 60, 96));
    grad.setColorAt(0.55, QColor(255, 0, 0, 96));
    grad.setColorAt(0.70, QColor(0, 0, 0, 0));
    p.setPen(Qt::transparent);
    p.setBrush(grad);
    p.drawPie(PieXPos, PieYPos, radiusAsInt * 2, radiusAsInt * 2, qtBeginAngle * 16, coolZone * 16);
}

bool ThermoWidget::isEqual(double a, double b)
{
    // qFuzzyCompare always returns false if a || b ==0
    if (qFuzzyIsNull(a) || qFuzzyIsNull(b)) {
        if (a < 0.0 || b < 0.0) {
            // One number is 0 and the other negative
            // to prevent a issue if a or b == -1 and the other 0
            // we will subtract one from each value
            a -= 1;
            b -= 1;
        } else {
            a += 1;
            b += 1;
        }
    }
    return qFuzzyCompare(a, b);
}
