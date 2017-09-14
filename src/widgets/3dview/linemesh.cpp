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

#include "gcodeto4d.h"
#include "linemesh.h"
#include "linemeshgeometry.h"

#include <QVector3D>
#include <QVector4D>
#include <Qt3DRender/QGeometryRenderer>

LineMesh::LineMesh(Qt3DCore::QNode *parent)
    : Qt3DRender::QGeometryRenderer(parent)
    , _lineMeshGeo(nullptr)
{
    setInstanceCount(1);
    setIndexOffset(0);
    setFirstInstance(0);
    setPrimitiveType(Qt3DRender::QGeometryRenderer::LineStrip);

    qRegisterMetaType<QList<QVector4D> >("QList<QVector4D>");
    connect(&_gcode, &GcodeTo4D::posFinished, this, &LineMesh::posUpdate);
}

void LineMesh::readAndRun(const QString &path)
{
    _gcode.read(path);
}

void LineMesh::read(const QString &path)
{
    emit run(path);
}

void LineMesh::posUpdate(const QList<QVector4D> &pos)
{
    _vertices = pos;
    _lineMeshGeo = new LineMeshGeometry(_vertices, this);
    setVertexCount(_lineMeshGeo->vertexCount());
    setGeometry(_lineMeshGeo);
    emit finished();
}

LineMesh::~LineMesh()
{
}
