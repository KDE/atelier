/***************************************************************************
**                                                                        **
**  QcGauge, for instrumentation, and real time data measurement          **
**  visualization widget for Qt.                                          **
**  Copyright (C) 2015 Hadj Tahar Berrima                                 **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU Lesser General Public License as        **
**  published by the Free Software Foundation, either version 3 of the    **
**  License, or (at your option) any later version.                       **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU Lesser General Public License for more details.                   **
**                                                                        **
**  You should have received a copy of the GNU Lesser General Public      **
**  License along with this program.                                      **
**  If not, see http://www.gnu.org/licenses/.                             **
**                                                                        **
****************************************************************************
**           Author:  Hadj Tahar Berrima                                  **
**           Website: http://pytricity.com/                               **
**           Contact: berrima_tahar@yahoo.com                             **
**           Date:    1 dec 2014                                          **
**           Version:  1.0                                                **
****************************************************************************
**           Modified: Chris Rizzitello 2019                              **
**           Website: http://atelier.kde.org                              **
**           Contact: rizzitello@kde.org                                  **
**           Version:  Atelier-Custom                                     **
****************************************************************************/
#pragma once

#include <QWidget>
#include <QPainter>
#include <QObject>
#include <QRectF>
#include <QtMath>

#if defined(QCGAUGE_COMPILE_LIBRARY)
#  define QCGAUGE_DECL  Q_DECL_EXPORT
#elif defined(QCGAUGE_USE_LIBRARY)
#  define QCGAUGE_DECL Q_DECL_IMPORT
#else
#  define QCGAUGE_DECL
#endif

class QcGaugeWidget;
class QcItem;
class QcBackgroundItem;
class QcDegreesItem;
class QcValuesItem;
class QcArcItem;
class QcNeedleItem;
class QcLabelItem;

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
class QCGAUGE_DECL QcGaugeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QcGaugeWidget(QWidget *parent = nullptr);
    ~QcGaugeWidget() override = default;

    QcBackgroundItem *addBackground(float position);
    QcDegreesItem *addDegrees(float position);
    QcValuesItem *addValues(float position);
    QcArcItem *addArc(float position);
    QcNeedleItem *addNeedle(float position);
    QcLabelItem *addLabel(float position);

    void addItem(QcItem *item, float position);
    int removeItem(QcItem *item);
    QList <QcItem *> items();

private:
    QList <QcItem *> mItems;

protected:
    void paintEvent(QPaintEvent *) override;

};

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

class QCGAUGE_DECL QcItem : public QObject
{
    Q_OBJECT
public:
    explicit QcItem(QObject *parent = nullptr);
    virtual void draw(QPainter *) = 0;
    virtual int type();

    void setPosition(float percentage);
    float position();
    QRectF rect();
    enum Error {InvalidValueRange, InvalidDegreeRange, InvalidStep};

protected:
    QRectF adjustRect(float percentage);
    float getRadius(const QRectF &);
    float getAngle(const QPointF &, const QRectF &tmpRect);
    QPointF getPoint(float deg, const QRectF &tmpRect);
    QRectF resetRect();
    void update();

private:
    QRectF mRect;
    QWidget *parentWidget;
    float mPosition;
};
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

class QCGAUGE_DECL QcScaleItem : public QcItem
{
    Q_OBJECT
public:
    explicit QcScaleItem(QObject *parent = nullptr);

    void setValueRange(float minValue, float maxValue);
    void setDgereeRange(float minDegree, float maxDegree);
    void setMinValue(float minValue);
    void setMaxValue(float maxValue);
    void setMinDegree(float minDegree);
    void setMaxDegree(float maxDegree);
    float minValue();
    float maxValue();
    float minDegree();
    float maxDegree();

protected:
    float getDegFromValue(float);

private:
    float mMinValue;
    float mMaxValue;
    float mMinDegree;
    float mMaxDegree;

};
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

class QCGAUGE_DECL QcBackgroundItem : public QcItem
{
    Q_OBJECT
public:
    explicit QcBackgroundItem(QObject *parent = nullptr);
    void draw(QPainter *) override;
    void addColor(float position, const QColor &color);
    void clearrColors();

private:
    QPen mPen;
    QList<QPair<float, QColor> > mColors;
    QLinearGradient mLinearGrad;
};

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

class QCGAUGE_DECL QcLabelItem : public QcItem
{
    Q_OBJECT
public:
    explicit QcLabelItem(QObject *parent = nullptr);
    void draw(QPainter *) override;
    void setAngle(float);
    float angle();
    void setFont(const QFont &font);
    void setText(const QString &text, bool repaint = true);
    QString text();
    void setColor(const QColor &color);
    QColor color();

private:
    float mAngle;
    QString mText;
    QColor mColor;
    QFont mFont;
};

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

class QCGAUGE_DECL QcArcItem : public QcScaleItem
{
    Q_OBJECT
public:
    explicit QcArcItem(QObject *parent = nullptr);
    void draw(QPainter *) override;
    void setColor(const QColor &color);

private:
    QColor mColor;

signals:

public slots:

};
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

class QCGAUGE_DECL QcDegreesItem : public QcScaleItem
{
    Q_OBJECT
public:
    explicit QcDegreesItem(QObject *parent = nullptr);
    void draw(QPainter *painter) override;
    void setStep(float step);
    void setColor(const QColor &color);
    void setSubDegree(bool);
private:
    float mStep;
    QColor mColor;
    bool mSubDegree;
};
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

class QCGAUGE_DECL QcNeedleItem : public QcScaleItem
{
    Q_OBJECT
public:
    explicit QcNeedleItem(QObject *parent = nullptr);
    void draw(QPainter *) override;
    void setCurrentValue(float value);
    float currentValue();
    void setColor(const QColor &color);
    QColor color();

    void setLabel(QcLabelItem *);
    QcLabelItem *label();

private:
    void createDiamonNeedle(float r);
    QPolygonF mNeedlePoly;
    float mCurrentValue;
    QColor mColor;
    QcLabelItem *mLabel;
};
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

class QCGAUGE_DECL QcValuesItem : public QcScaleItem
{
    Q_OBJECT
public:
    explicit QcValuesItem(QObject *parent = nullptr);
    void draw(QPainter *) override;
    void setFont(const QFont &font);
    void setStep(float step);
    void setColor(const QColor &color);
private:
    float mStep{10};
    QFont mFont;
    QColor mColor;
};
