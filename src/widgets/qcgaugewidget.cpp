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

#include "qcgaugewidget.h"
#include <QPainterPath>

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

QcGaugeWidget::QcGaugeWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(250, 250);
}

QcBackgroundItem *QcGaugeWidget::addBackground(float position)
{
    QcBackgroundItem *item = new QcBackgroundItem(this);
    item->setPosition(position);
    mItems.append(item);
    return item;
}

QcDegreesItem *QcGaugeWidget::addDegrees(float position)
{
    QcDegreesItem *item = new QcDegreesItem(this);
    item->setPosition(position);

    mItems.append(item);
    return item;
}

QcValuesItem *QcGaugeWidget::addValues(float position)
{
    auto *item = new QcValuesItem(this);
    item->setPosition(position);
    mItems.append(item);
    return item;
}

QcArcItem *QcGaugeWidget::addArc(float position)
{
    QcArcItem *item = new QcArcItem(this);
    item->setPosition(position);
    mItems.append(item);
    return item;
}

QcNeedleItem *QcGaugeWidget::addNeedle(float position)
{
    auto *item = new QcNeedleItem(this);
    item->setPosition(position);
    mItems.append(item);
    return item;
}

QcLabelItem *QcGaugeWidget::addLabel(float position)
{
    QcLabelItem *item = new QcLabelItem(this);
    item->setPosition(position);
    mItems.append(item);
    return item;
}

void QcGaugeWidget::addItem(QcItem *item, float position)
{
    // takes parentship of the item
    item->setParent(this);
    item->setPosition(position);
    mItems.append(item);
}

int QcGaugeWidget::removeItem(QcItem *item)
{
    return mItems.removeAll(item);
}

QList<QcItem *> QcGaugeWidget::items()
{
    return mItems;
}

void QcGaugeWidget::paintEvent(QPaintEvent * /*paintEvt*/)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    for (QcItem *item : std::as_const(mItems)) {
        item->draw(&painter);
    }
}
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

QcItem::QcItem(QObject *parent)
    : QObject(parent)
{
    parentWidget = qobject_cast<QWidget *>(parent);
    mPosition = 50;
}

int QcItem::type()
{
    return 50;
}

void QcItem::update()
{
    parentWidget->update();
}

float QcItem::position()
{
    return mPosition;
}

QRectF QcItem::rect()
{
    return mRect;
}

void QcItem::setPosition(float position)
{
    if (position > 100) {
        mPosition = 100;
    } else if (position < 0) {
        mPosition = 0;
    } else {
        mPosition = position;
    }
    update();
}

QRectF QcItem::adjustRect(float percentage)
{
    float r = getRadius(mRect);
    float offset = r - (percentage * r) / 100.0;
    QRectF tmpRect = mRect.adjusted(offset, offset, -offset, -offset);
    return tmpRect;
}

float QcItem::getRadius(const QRectF &tmpRect)
{
    float r = 0;
    if (tmpRect.width() < tmpRect.height()) {
        r = tmpRect.width() / 2.0;
    } else {
        r = tmpRect.height() / 2.0;
    }
    return r;
}

QRectF QcItem::resetRect()
{
    mRect = parentWidget->rect();
    float r = getRadius(mRect);
    mRect.setWidth(2.0 * r);
    mRect.setHeight(2.0 * r);
    mRect.moveCenter(parentWidget->rect().center());
    return mRect;
}

QPointF QcItem::getPoint(float deg, const QRectF &tmpRect)
{
    float r = getRadius(tmpRect);
    float xx = cos(qDegreesToRadians(deg)) * r;
    float yy = sin(qDegreesToRadians(deg)) * r;
    QPointF pt;
    xx = tmpRect.center().x() - xx;
    yy = tmpRect.center().y() - yy;
    pt.setX(xx);
    pt.setY(yy);
    return pt;
}

float QcItem::getAngle(const QPointF &pt, const QRectF &tmpRect)
{
    float xx = tmpRect.center().x() - pt.x();
    float yy = tmpRect.center().y() - pt.y();
    return qRadiansToDegrees(atan2(yy, xx));
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

QcScaleItem::QcScaleItem(QObject *parent)
    : QcItem(parent)
{
    mMinDegree = -45;
    mMaxDegree = 225;
    mMinValue = 0;
    mMaxValue = 100;
}

void QcScaleItem::setValueRange(float minValue, float maxValue)
{
    if (!(minValue < maxValue)) {
        mMaxValue = minValue;
        mMinValue = maxValue;
    } else {
        mMinValue = minValue;
        mMaxValue = maxValue;
    }
}

void QcScaleItem::setDgereeRange(float minDegree, float maxDegree)
{
    if (!(minDegree < maxDegree)) {
        mMinDegree = maxDegree;
        mMaxDegree = minDegree;
    } else {
        mMinDegree = minDegree;
        mMaxDegree = maxDegree;
    }
}

float QcScaleItem::getDegFromValue(float v)
{
    float a = (mMaxDegree - mMinDegree) / (mMaxValue - mMinValue);
    float b = -a * mMinValue + mMinDegree;
    return a * v + b;
}

void QcScaleItem::setMinValue(float minValue)
{
    if (minValue > mMaxValue) {
        return;
    }
    mMinValue = minValue;
    update();
}

void QcScaleItem::setMaxValue(float maxValue)
{
    if (maxValue < mMinValue) {
        return;
    }
    mMaxValue = maxValue;
    update();
}

void QcScaleItem::setMinDegree(float minDegree)
{
    if (minDegree > mMaxDegree) {
        return;
    }
    mMinDegree = minDegree;
    update();
}
void QcScaleItem::setMaxDegree(float maxDegree)
{
    if (maxDegree < mMinDegree) {
        return;
    }
    mMaxDegree = maxDegree;
    update();
}

float QcScaleItem::minValue()
{
    return mMinValue;
}

float QcScaleItem::maxValue()
{
    return mMaxValue;
}

float QcScaleItem::minDegree()
{
    return mMinDegree;
}

float QcScaleItem::maxDegree()
{
    return mMaxDegree;
}
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

QcBackgroundItem::QcBackgroundItem(QObject *parent)
    : QcItem(parent)
{
    setPosition(88);
    mPen = Qt::NoPen;
    setPosition(88);

    addColor(0.4, Qt::darkGray);
    addColor(0.8, Qt::black);
}

void QcBackgroundItem::draw(QPainter *painter)
{
    QRectF tmpRect = resetRect();
    painter->setBrush(Qt::NoBrush);
    QLinearGradient linearGrad(tmpRect.topLeft(), tmpRect.bottomRight());
    for (int i = 0; i < mColors.size(); i++) {
        linearGrad.setColorAt(mColors[i].first, mColors[i].second);
    }
    painter->setPen(mPen);
    painter->setBrush(linearGrad);
    painter->drawEllipse(adjustRect(position()));
}

void QcBackgroundItem::addColor(float position, const QColor &color)
{
    if (position < 0 || position > 1) {
        return;
    }
    QPair<float, QColor> pair;
    pair.first = position;
    pair.second = color;
    mColors.append(pair);
    update();
}

void QcBackgroundItem::clearrColors()
{
    mColors.clear();
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

QcLabelItem::QcLabelItem(QObject *parent)
    : QcItem(parent)
{
    setPosition(50);
    mAngle = 270;
    mText = QStringLiteral("%");
    mColor = Qt::black;
}

void QcLabelItem::draw(QPainter *painter)
{
    resetRect();
    QRectF tmpRect = adjustRect(position());
    painter->setFont(mFont);
    painter->setPen(QPen(mColor));

    QPointF txtCenter = getPoint(mAngle, tmpRect);
    QFontMetrics fMetrics = painter->fontMetrics();
    QSize sz = fMetrics.size(Qt::TextSingleLine, mText);
    QRectF txtRect(QPointF(0, 0), sz);
    txtRect.moveCenter(txtCenter);

    painter->drawText(txtRect, Qt::TextSingleLine, mText);
}

void QcLabelItem::setAngle(float a)
{
    mAngle = a;
    update();
}

float QcLabelItem::angle()
{
    return mAngle;
}

void QcLabelItem::setFont(const QFont &font)
{
    mFont = font;
}

void QcLabelItem::setText(const QString &text, bool repaint)
{
    mText = text;
    if (repaint) {
        update();
    }
}

QString QcLabelItem::text()
{
    return mText;
}

void QcLabelItem::setColor(const QColor &color)
{
    mColor = color;
    update();
}

QColor QcLabelItem::color()
{
    return mColor;
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

QcArcItem::QcArcItem(QObject *parent)
    : QcScaleItem(parent)
{
    setPosition(80);
    mColor = Qt::black;
}

void QcArcItem::draw(QPainter *painter)
{
    resetRect();
    QRectF tmpRect = adjustRect(position());
    float r = getRadius(tmpRect);

    QPen pen;
    pen.setColor(mColor);
    pen.setWidthF(r / 40);
    painter->setPen(pen);
    painter->drawArc(tmpRect, -16 * (minDegree() + 180), -16 * (maxDegree() - minDegree()));
}

void QcArcItem::setColor(const QColor &color)
{
    mColor = color;
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

QcDegreesItem::QcDegreesItem(QObject *parent)
    : QcScaleItem(parent)
{
    mStep = 10;
    mColor = Qt::black;
    mSubDegree = false;
    setPosition(90);
}

void QcDegreesItem::draw(QPainter *painter)
{
    resetRect();
    QRectF tmpRect = adjustRect(position());

    painter->setPen(mColor);
    float r = getRadius(tmpRect);
    for (float val = minValue(); val <= maxValue(); val += mStep) {
        float deg = getDegFromValue(val);
        QPointF pt = getPoint(deg, tmpRect);
        QPainterPath path;
        path.moveTo(pt);
        path.lineTo(tmpRect.center());
        pt = path.pointAtPercent(0.03);
        QPointF newPt = path.pointAtPercent(0.13);

        QPen pen;
        pen.setColor(mColor);
        if (!mSubDegree) {
            pen.setWidthF(r / 25.0);
        }

        painter->setPen(pen);
        painter->drawLine(pt, newPt);
    }
}

void QcDegreesItem::setStep(float step)
{
    mStep = step;
    update();
}

void QcDegreesItem::setColor(const QColor &color)
{
    mColor = color;
    update();
}

void QcDegreesItem::setSubDegree(bool b)
{
    mSubDegree = b;
    update();
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

QcNeedleItem::QcNeedleItem(QObject *parent)
    : QcScaleItem(parent)
    , mCurrentValue(0)
    , mColor(Qt::black)
    , mLabel(nullptr)
{
}

void QcNeedleItem::draw(QPainter *painter)
{
    resetRect();
    QRectF tmpRect = adjustRect(position());
    painter->save();
    painter->translate(tmpRect.center());
    float deg = getDegFromValue(mCurrentValue);
    painter->rotate(deg + 90.0);
    painter->setBrush(QBrush(mColor));
    painter->setPen(Qt::NoPen);

    QLinearGradient grad;
    createDiamonNeedle(getRadius(tmpRect));
    painter->drawConvexPolygon(mNeedlePoly);
    painter->restore();
}

void QcNeedleItem::setCurrentValue(float value)
{
    if (value < minValue()) {
        mCurrentValue = minValue();
    } else if (value > maxValue()) {
        mCurrentValue = maxValue();
    } else {
        mCurrentValue = value;
    }

    if (mLabel) {
        mLabel->setText(QString::number(mCurrentValue), false);
    }
    update();
}

float QcNeedleItem::currentValue()
{
    return mCurrentValue;
}

void QcNeedleItem::setColor(const QColor &color)
{
    mColor = color;
    update();
}

QColor QcNeedleItem::color()
{
    return mColor;
}

void QcNeedleItem::setLabel(QcLabelItem *label)
{
    mLabel = label;
    update();
}

QcLabelItem *QcNeedleItem::label()
{
    return mLabel;
}

void QcNeedleItem::createDiamonNeedle(float r)
{
    QVector<QPointF> tmpPoints;
    tmpPoints.append(QPointF(0.0, 0.0));
    tmpPoints.append(QPointF(-r / 20.0, r / 20.0));
    tmpPoints.append(QPointF(0.0, r));
    tmpPoints.append(QPointF(r / 20.0, r / 20.0));
    mNeedlePoly = tmpPoints;
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

QcValuesItem::QcValuesItem(QObject *parent)
    : QcScaleItem(parent)
    , mStep(10)
    , mColor(Qt::black)
{
    setPosition(70);
}

void QcValuesItem::draw(QPainter *painter)
{
    QRectF tmpRect = resetRect();
    painter->setFont(mFont);
    painter->setPen(mColor);
    for (float val = minValue(); val <= maxValue(); val += mStep) {
        float deg = getDegFromValue(val);
        QPointF pt = getPoint(deg, tmpRect);
        QPainterPath path;
        path.moveTo(pt);
        path.lineTo(tmpRect.center());
        QString strVal = QString::number(val);
        QFontMetrics fMetrics = painter->fontMetrics();
        QSize sz = fMetrics.size(Qt::TextSingleLine, strVal);
        QRectF txtRect(QPointF(0, 0), sz);
        QPointF textCenter = path.pointAtPercent(1.0 - position() / 100.0);
        txtRect.moveCenter(textCenter);

        painter->drawText(txtRect, Qt::TextSingleLine, strVal);
    }
}

void QcValuesItem::setFont(const QFont &font)
{
    mFont = font;
}

void QcValuesItem::setStep(float step)
{
    mStep = step;
}

void QcValuesItem::setColor(const QColor &color)
{
    mColor = color;
}
