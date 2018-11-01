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

#include <QObject>

class BedProperties : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int width READ width NOTIFY widthChanged)
    Q_PROPERTY(int depth READ depth NOTIFY depthChanged)
public:
    explicit BedProperties(QObject *parent = nullptr);
    ~BedProperties();

    int width() const;
    int depth() const;

signals:
    void widthChanged(int width);
    void depthChanged(int depth);

private:
    void updateBedSize(const QSize &size);

    int m_width;
    int m_depth;
};
