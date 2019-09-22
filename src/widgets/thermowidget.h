/* Atelier KDE Printer Host for 3D Printing
    Copyright (C) <2019>
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
#include "qcgaugewidget.h"

class QKeyEvent;
class QPaintEvent;
class QFocusEvent;
class QWheelEvent;

class ThermoWidget : public QcGaugeWidget
{
    Q_OBJECT
public:
    ThermoWidget(QWidget *parent, const QString &name);
    ~ThermoWidget() override = default;
    void setCurrentTemperature(double temperature);
    void setTargetTemperature(int temperature);
    void setRange(int min, int max);
    void setName(const QString &name);

signals:
    void targetTemperatureChanged(double targetTemperature);
protected:
    void keyPressEvent(QKeyEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QcNeedleItem *m_currentTemperatureNeedle = nullptr;
    QcNeedleItem *m_targetTemperatureNeedle = nullptr;
    QcDegreesItem *m_ticksItem = nullptr;
    QcDegreesItem *m_subTicksItem = nullptr;
    QcLabelItem *m_nameLabel = nullptr;
    QcValuesItem *m_valueDisplay = nullptr;
    QTimer *m_cursorTimer = nullptr;
    QTimer *m_tempChangedTimer = nullptr;
    QString m_targetTemperatureText = QStringLiteral("0");
    bool m_paintCursor = false;
    int m_cursorPos = 0;
    double m_currentTemperature;
    int m_targetTemperature;
    int m_maxValue = 100;
    int m_minValue = 0;
    void resetTimer();
    bool isEqual(double a = 0, double b = 0);
};
