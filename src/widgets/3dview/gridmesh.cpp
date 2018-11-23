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
#include <QSize>
#include <QVector3D>
#include "gridmesh.h"
#include "linemeshgeometry.h"

GridMesh::GridMesh(Qt3DCore::QNode *parent)
    : Qt3DRender::QGeometryRenderer(parent)
{
    setInstanceCount(1);
    setIndexOffset(0);
    setFirstInstance(0);
    setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);
    setMeshResolution(QSize(20, 20));
}

QSize GridMesh::meshResolution() const
{
    return m_meshResolution;
}

void GridMesh::setMeshResolution(const QSize &meshResolution)
{
    if (meshResolution == m_meshResolution) {
        return;
    }

    QVector<QVector3D> vertices;

    const float dx = 1.0f / static_cast<float>(meshResolution.width());
    for (int col = 0; col <= meshResolution.width(); col++) {
        const float x = col * dx;
        vertices.append(QVector3D(x, 0.0f, 0.0f));
        vertices.append(QVector3D(x, 1.0f, 0.0f));
    }

    const float dy = 1.0f / static_cast<float>(meshResolution.height());
    for (int row = 0; row <= meshResolution.height(); row++) {
        const float y = row * dy;
        vertices.append(QVector3D(0.0f, y, 0.0f));
        vertices.append(QVector3D(1.0f, y, 0.0f));
    }

    if (geometry()) {
        auto geom = geometry();
        setGeometry(nullptr);
        delete geom;
    }

    auto geom = new LineMeshGeometry(vertices, this);
    setVertexCount(geom->vertexCount());
    setGeometry(geom);

    m_meshResolution = meshResolution;
    emit meshResolutionChanged(m_meshResolution);
}
