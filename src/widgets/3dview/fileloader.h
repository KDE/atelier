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

#include <QFile>
#include <QList>
#include <QObject>
#include <QVariant>

class QString;
class QVector4D;

class FileLoader : public QObject
{
    Q_OBJECT

public:
    FileLoader(QString &fileName, QObject *parent = nullptr);
    ~FileLoader();

private:
    QFile _file;

signals:
    void percentUpdate(QVariant var);
    void posFinished(const QList<QVector4D> &pos);

public slots:
    void run();
};
