/* Atelier KDE Printer Host for 3D Printing
    Copyright (C) <2018>
    Author: Kevin Ottens - ervin@kde.org

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

#include <QVector2D>
#include <Qt3DCore/QEntity>

namespace Qt3DRender
{
class QParameter;
}

class AxisGnomonEntity : public Qt3DCore::QEntity
{
    Q_OBJECT
    Q_PROPERTY(QVector2D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(float scale READ scale WRITE setScale NOTIFY scaleChanged)
public:
    explicit AxisGnomonEntity(Qt3DCore::QNode *parent = nullptr);
    ~AxisGnomonEntity();

    QVector2D position() const;
    float scale() const;

public Q_SLOTS:
    void setPosition(const QVector2D &position);
    void setScale(float scale);

Q_SIGNALS:
    void positionChanged(const QVector2D &position);
    void scaleChanged(float scale);

private:
    void applyModelMatrix();

    Qt3DRender::QParameter *_modelMatrixParameter;
    QVector2D _position;
    float _scale;
};
