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

#pragma once

#include <QList>
#include <QObject>
#include <Qt3DCore/QNode>
#include <Qt3DRender/QGeometryRenderer>

#include "gcodeto4d.h"

class LineMeshGeometry;
class QString;
class QVector4D;

class LineMesh : public Qt3DRender::QGeometryRenderer
{
    Q_OBJECT
public:
    explicit LineMesh(Qt3DCore::QNode *parent = Q_NULLPTR);
    ~LineMesh();
    Q_INVOKABLE void readAndRun(const QString &path);
    void read(const QString &path);
    void posUpdate(const QList<QVector4D> &pos);

signals:
    void finished();
    void run(const QString &path);

private:
    QList<QVector4D> _vertices;
    LineMeshGeometry *_lineMeshGeo;
    GcodeTo4D _gcode;
};
