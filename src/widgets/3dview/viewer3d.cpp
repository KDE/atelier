/* Atelier KDE Printer Host for 3D Printing
    Copyright (C) <2017>
    Author: Patrick José Pereira - patrickjp@kde.org
            Chris Rizzitello - rizzitello@kde.org

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 3 of
    the License or any later version accepted by the membership of
    KDE e.V. (or its successor approved by the membership of KDE
    e.V.), which shall act as a proxy defined in Section 14 of
    version 3 of the license.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <QDebug>
#include <QDirIterator>
#include <QHBoxLayout>
#include <QObject>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickItem>
#include <QQuickView>
#include <QString>

#include "axisgnomonentity.h"
#include "bedproperties.h"
#include "cameracontroller.h"
#include "gridmesh.h"
#include "linemesh.h"
#include "viewer3d.h"

Viewer3D::Viewer3D(QWidget *parent)
    : QWidget(parent)
    , _lineMesh(new LineMesh)
{
    Q_INIT_RESOURCE(viewer3d);

    qmlRegisterType<AxisGnomonEntity>("Atelier", 1, 0, "AxisGnomonEntity");
    qmlRegisterType<CameraController>("Atelier", 1, 0, "CameraController");
    qmlRegisterType<GridMesh>("Atelier", 1, 0, "GridMesh");
    qmlRegisterType<LineMesh>("Atelier", 1, 0, "LineMesh");
    qmlRegisterType<BedProperties>("Atelier", 1, 0, "BedProperties");

    _view = new QQuickView(&_engine, nullptr);

    auto format = QSurfaceFormat();
    format.setVersion(3, 1);
    format.setProfile(QSurfaceFormat::CoreProfile);
    _view->setFormat(format);

    _view->rootContext()->setContextProperty(QStringLiteral("viewer3d"), this);
    _view->setResizeMode(QQuickView::SizeRootObjectToView);
    _view->setSource(QUrl(QStringLiteral("qrc:/viewer3d.qml")));
    auto mainLayout = new QHBoxLayout;
    mainLayout->addWidget(QWidget::createWindowContainer(_view));
    QObject *item = _view->rootObject();
    // Connect the drop pass from the QML part.
    connect(item, SIGNAL(droppedUrls(QVariant)), this, SLOT(dropCatch(QVariant)));
    this->setLayout(mainLayout);
}

void Viewer3D::dropCatch(const QVariant &var)
{
    Q_EMIT droppedUrls(var.value<QList<QUrl>>());
}

void Viewer3D::drawModel(const QString &file)
{
    QObject *object = _view->rootObject();
    auto fileName = object->findChild<QObject *>(QStringLiteral("fileName"));
    fileName->setProperty("text", QVariant(file));
}

void Viewer3D::setBedSize(const QSize &newBedSize)
{
    if (newBedSize != _bedSize) {
        _bedSize = newBedSize;
        Q_EMIT bedSizeChanged(_bedSize);
    }
}

QSize Viewer3D::bedSize()
{
    return _bedSize;
}
