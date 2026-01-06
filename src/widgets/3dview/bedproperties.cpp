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

#include "bedproperties.h"

#include "viewer3d.h"

#include <QQmlContext>
#include <QTimer>

BedProperties::BedProperties(QObject *parent)
    : QObject(parent)
    , m_width(200)
    , m_depth(200)
{
    QTimer::singleShot(0, [=] {
        auto context = qmlContext(this);
        if (!context) {
            return;
        }

        auto viewer = context->contextProperty("viewer3d").value<Viewer3D *>();
        if (!viewer) {
            return;
        }

        updateBedSize(viewer->bedSize());
        connect(viewer, &Viewer3D::bedSizeChanged, this, &BedProperties::updateBedSize);
    });
}

int BedProperties::width() const
{
    return m_width;
}

int BedProperties::depth() const
{
    return m_depth;
}

void BedProperties::updateBedSize(const QSize &size)
{
    if (size.width() != m_width) {
        m_width = size.width();
        Q_EMIT widthChanged(m_width);
    }

    if (size.height() != m_depth) {
        m_depth = size.height();
        Q_EMIT depthChanged(m_depth);
    }
}
