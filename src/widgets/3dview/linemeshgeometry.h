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

#pragma once

#include <QAttribute>
#include <QGeometry>
#include <Qt3DCore/QBuffer>

class LineMeshGeometry : public Qt3DCore::QGeometry
{
    Q_OBJECT

public:
    LineMeshGeometry(const QVector<QVector3D> &vertices, Qt3DCore::QNode *parent = Q_NULLPTR);
    ~LineMeshGeometry() = default;
    int vertexCount();

private:
    Qt3DCore::QAttribute *_positionAttribute;
    Qt3DCore::QBuffer *_vertexBuffer;
};
