#include "printerhostpositionvisualcontroller.h"
#include <QDebug>
#include <QResizeEvent>

PieButton::PieButton(QLatin1Char axis, int value, int size, int angle) : _axis(axis), _value(value) {
    const int delta = 16; // Qt Docs: angle is 16th of a degree.
    setBrush(QBrush(Qt::white));
    setStartAngle(angle * delta);
    setSpanAngle(90 * delta);
    setRect(QRect(QPoint(size * -1, size * -1), QPoint(size, size)));
    setZValue(size * -1);
    setAcceptHoverEvents(true);
    setToolTip(QStringLiteral("Move the hotend to the %1 by %2 units").arg(axis).arg(value));
}

void PieButton::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    emit clicked(_axis, _value);
}

void PieButton::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    setBrush(QBrush(QColor(Qt::white).dark(150)));
}

void PieButton::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    setBrush(QBrush(QColor(Qt::white)));
}

PrinterHotendPositionVisualController::PrinterHotendPositionVisualController(QWidget *parent) :
QGraphicsView(parent)
{
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    setScene(new QGraphicsScene());

    auto createPie = [=](QLatin1Char axis, int value, int size, int angle) {
        auto pie = new PieButton(axis, value, size, angle);
        scene()->addItem(pie);
        connect(pie, &PieButton::clicked, this, &PrinterHotendPositionVisualController::clicked);
    };

    int currPieSize = 25;
    for(auto value : {1, 10, 100, 1000}) {
        createPie(QLatin1Char('X'), value, currPieSize, -45);       // Left
        createPie(QLatin1Char('X'), value * -1, currPieSize, 135);  // Right
        createPie(QLatin1Char('Y'), value, currPieSize, 45);        // Top
        createPie(QLatin1Char('Y'), value * -1, currPieSize, 225);  // Bottom
        currPieSize += 25;
    }
    setSceneRect(scene()->itemsBoundingRect());
}

void PrinterHotendPositionVisualController::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    fitInView(sceneRect(), Qt::KeepAspectRatio);
}
