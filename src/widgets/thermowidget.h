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
#pragma once

#include <qwt_dial.h>
#include <qwt_dial_needle.h>

class QKeyEvent;
class QPaintEvent;
class QFocusEvent;
class QWheelEvent;

class ThermoWidget : public QwtDial
{
    Q_OBJECT

public:
    ThermoWidget(QWidget *parent, const QString &name);
    ~ThermoWidget() = default;

    void drawNeedle(QPainter *painter, const QPointF &center,
                    double radius, double dir, QPalette::ColorGroup colorGroup) const;

    void setCurrentTemperature(double temperature);
    void setTargetTemperature(int temperature);

signals:
    void targetTemperatureChanged(double targetTemperature);

protected:
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    bool isEqual(double a = 0, double b = 0);
    QwtDialSimpleNeedle *m_currentTemperatureNeedle;
    QwtDialSimpleNeedle *m_targetTemperatureNeedle;
    QString m_currentTemperatureTextFromEditor = QString("-");
    QString m_name;
    QTimer *m_cursorTimer = nullptr;
    QTimer *m_tempChangedTimer = nullptr;
    bool m_paintCursor = false;
    int m_cursorPos = 0;
    double m_currentTemperature;
    int m_targetTemperature;
    void resetTimer();
};
