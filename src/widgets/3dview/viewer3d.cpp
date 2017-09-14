/* Atelier KDE Printer Host for 3D Printing
    Copyright (C) <2017>
    Author: Patrick José Pereira - patrickelectric@gmail.com

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

#include "linemesh.h"
#include "gridmesh.h"
#include "viewer3d.h"

#include <QHBoxLayout>
#include <QQmlEngine>
#include <QString>
#include <QtQuick/QQuickView>

#include <QObject>
#include <QDirIterator>
#include <QDebug>
#include <QQmlContext>
#include <QQuickItem>

Viewer3D::Viewer3D(QWidget *parent)
    : QWidget(parent)
    , _lineMesh(new LineMesh)
{
    Q_INIT_RESOURCE(viewer3d);

    QDirIterator it(QStringLiteral(":"), QDirIterator::Subdirectories);
    while (it.hasNext()) {
        qDebug() << it.next();
    }

    qmlRegisterType<GridMesh>("GridMesh", 1, 0, "GridMesh");
    qmlRegisterType<LineMesh>("LineMesh", 1, 0, "LineMesh");

    _view = new QQuickView(&_engine, nullptr);
    _view->setResizeMode(QQuickView::SizeRootObjectToView);
    _view->setSource(QUrl(QStringLiteral("qrc:/viewer3d.qml")));
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(QWidget::createWindowContainer(_view));
    this->setLayout(mainLayout);
}

void Viewer3D::drawModel(QString file)
{

    QObject *object = _view->rootObject();
    QObject *fileName = object->findChild<QObject*>(QStringLiteral("fileName"));
    fileName->setProperty("text", QVariant(file));
}

Viewer3D::~Viewer3D()
{
}
