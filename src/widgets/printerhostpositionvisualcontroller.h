#pragma once

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>

/* Usage:
 *
 * Create a instance of PrinterHotendPositionVisualController and
 * connect the clicked signal, it will give you the axis and value
 * that was clicked.
 */

class PieButton : public QObject, public QGraphicsEllipseItem {
    Q_OBJECT
public:
    PieButton(QLatin1Char axis, int value, int size, int angle);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
signals:
    void clicked(QLatin1Char axis, int value);
private:
    QLatin1Char _axis;
    int _value;
};

class PrinterHotendPositionVisualController : public QGraphicsView
{
    Q_OBJECT

public:
    explicit PrinterHotendPositionVisualController(QWidget *parent = 0);

protected:
    void resizeEvent(QResizeEvent *event);

signals:
    void clicked(QLatin1Char axis, int value);

};
