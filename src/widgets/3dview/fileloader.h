/* Atelier KDE Printer Host for 3D Printing
    Copyright (C) <2017>
    Author: Patrick José Pereira - patrickjp@kde.org

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
#include <QObject>
#include <QVariant>
#include <QVector>

class QString;
class QVector4D;

class FileLoader : public QObject
{
    Q_OBJECT

public:
    FileLoader(QString &fileName, QObject *parent = nullptr);
    ~FileLoader() = default;

private:
    QFile _file;
    const static QString _commentChar;
    const static QStringList _moveCommands;
    const static QString _space;
    const static QString _E;
    const static QString _X;
    const static QString _Y;
    const static QString _Z;

Q_SIGNALS:
    void percentUpdate(QVariant var);
    void posFinished(const QVector<QVector4D> &pos);

public Q_SLOTS:
    void run();
};
