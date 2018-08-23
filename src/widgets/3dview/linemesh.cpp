/* Atelier KDE Printer Host for 3D Printing
    Copyright (C) <2017-2018>
    Author: Patrick José Pereira - patrickjp@kde.org
            Kevin Ottens - ervin@kde.org

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

#include <QGeometryRenderer>
#include <QVector3D>
#include <QVector4D>
#include "gcodeto4d.h"
#include "linemesh.h"
#include "linemeshgeometry.h"

LineMesh::LineMesh(Qt3DCore::QNode *parent) :
    Qt3DRender::QGeometryRenderer(parent)
    , _lineMeshGeo(nullptr)
{
    setInstanceCount(1);
    setIndexOffset(0);
    setFirstInstance(0);
    setPrimitiveType(Qt3DRender::QGeometryRenderer::LineStrip);

    qRegisterMetaType<QVector<QVector4D>>("QVector<QVector4D>");
    connect(&_gcode, &GcodeTo4D::posFinished, this, &LineMesh::posUpdate);
}

LineMesh::~LineMesh()
{
}

void LineMesh::readAndRun(const QString &path)
{
    _gcode.read(path);
}

void LineMesh::read(const QString &path)
{
    emit run(path);
}

void LineMesh::posUpdate(const QVector<QVector4D> &pos)
{
    QVector<QVector3D> vertices;
    vertices.reserve(pos.size());
    std::transform(pos.cbegin(), pos.cend(),
                   std::back_inserter(vertices),
                   [](const QVector4D & x) {
                       return x.toVector3D();
                   });

    _lineMeshGeo = new LineMeshGeometry(vertices, this);
    setVertexCount(_lineMeshGeo->vertexCount());
    setGeometry(_lineMeshGeo);
    emit finished();
}
