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

#include "linemeshgeometry.h"
#include "gridmesh.h"

#include <Qt3DRender/QGeometryRenderer>
#include <QVector4D>
#include <QVector3D>
#include <QVector2D>

GridMesh::GridMesh(Qt3DCore::QNode *parent)
    : Qt3DRender::QGeometryRenderer(parent)
{
    setInstanceCount(1);
    setIndexOffset(0);
    setFirstInstance(0);
    setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);

    QVector2D s(20, 20);
    QList<QVector4D> vertices;
    for (uint i = 0; i <= s.x(); i++) {
        for (uint j = 0; j <= s.y(); j++) {
            vertices.append(QVector4D(i, 0, 0, 0));
            vertices.append(QVector4D(i, j, 0, 0));

            vertices.append(QVector4D(0, j, 0, 0));
            vertices.append(QVector4D(i, j, 0, 0));
        }
    }

    auto geometry = new LineMeshGeometry(vertices, this);
    setVertexCount(geometry->vertexCount());
    setGeometry(geometry);
}

GridMesh::~GridMesh()
{
}
