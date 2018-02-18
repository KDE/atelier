/* Atelier KDE Printer Host for 3D Printing
    Copyright (C) <2018>
    Author: Tomaz Canabrava - tcanabrava@kde.org
            Chris Rizzitello - rizzitello@kde.org
            Lays Rodrigues - laysrodriguessilva@gmail.com

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
