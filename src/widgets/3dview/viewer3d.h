/* Atelier KDE Printer Host for 3D Printing
    Copyright (C) <2017>
    Author: Patrick José Pereira - patrickjp@kde.org
            Chris Rizzitello - rizzitello@kde.org

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

#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QWidget>

class QString;

class Viewer3D : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QSize bedSize READ bedSize WRITE setBedSize NOTIFY bedSizeChanged)

public Q_SLOTS:
    void dropCatch(const QVariant &var);
    void setBedSize(const QSize &newBedSize);

public:
    explicit Viewer3D(QWidget *parent = nullptr);
    ~Viewer3D() override = default;
    QSize bedSize();
    void drawModel(const QString &file);

private:
    QQmlApplicationEngine _engine;
    QQuickView *_view;
    QSize _bedSize = QSize(50, 50);

Q_SIGNALS:
    void droppedUrls(QList<QUrl> fileList);
    void bedSizeChanged(QSize bedSize);
};
