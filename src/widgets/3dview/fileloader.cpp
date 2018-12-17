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
#include <QString>
#include <QTextStream>
#include <QVariant>
#include <QVector>
#include <QVector4D>
#include "fileloader.h"

const QString FileLoader::_commentChar = QStringLiteral(";");
const QStringList FileLoader::_moveCommands = {QStringLiteral("G0"), QStringLiteral("G1")};
const QString FileLoader::_space = QStringLiteral(" ");
const QString FileLoader::_E = QStringLiteral("E");
const QString FileLoader::_X = QStringLiteral("X");
const QString FileLoader::_Y = QStringLiteral("Y");
const QString FileLoader::_Z = QStringLiteral("Z");

FileLoader::FileLoader(QString &fileName, QObject *parent) :
    QObject(parent)
    , _file(fileName)
{
}

void FileLoader::run()
{
    QVector<QVector4D> pos;
    qint64 totalSize = _file.bytesAvailable();
    qint64 stillSize = totalSize;

    if (_file.open(QIODevice::ReadOnly)) {
        int lastPerc = 0;
        QTextStream in(&_file);
        while (!in.atEnd()) {
            //Get each line
            QString line = in.readLine();
            stillSize -= line.size() + 1; // +1 endl
            const int perc = int((totalSize -  stillSize) * 100.0 / totalSize);
            if (perc - lastPerc > 1) {
                emit percentUpdate(perc);
                lastPerc = perc;
            }
            line = line.simplified();
            //Is it a comment ? Drop it
            if (line.isEmpty()) {
                continue;
            }
            //Remove comment in the end of command
            if (line.indexOf(_commentChar) != -1) {
                line.resize(line.indexOf(_commentChar));
                //Remove trailing spaces
                line = line.simplified();
            }

            //Split command and args
            QStringList commAndArgs = line.split(_space);

            if (_moveCommands.contains(commAndArgs[0])) {
                QVector4D actualPos;
                //Compute args
                commAndArgs.removeFirst();
                for (QString element : commAndArgs) {
                    if (element.contains(_X)) {
                        actualPos.setX(element.remove(0, 1).toFloat() / 10);
                    }

                    if (element.contains(_Y)) {
                        actualPos.setY(element.remove(0, 1).toFloat() / 10);
                    }

                    if (element.contains(_Z)) {
                        actualPos.setZ(element.remove(0, 1).toFloat() / 10);
                    }

                    if (element.contains(_E)) {
                        actualPos.setW(element.remove(0, 1).toFloat() / 10);
                    }
                }

                if (!pos.isEmpty()) {
                    if (!line.contains(_X)) {
                        actualPos.setX(pos.last().x());
                    }

                    if (!line.contains(_Y)) {
                        actualPos.setY(pos.last().y());
                    }

                    if (!line.contains(_Z)) {
                        actualPos.setZ(pos.last().z());
                    }

                    if (!line.contains(_E)) {
                        actualPos.setW(pos.last().w());
                    }
                }

                pos.append(actualPos);
            }
        }
    }
    emit percentUpdate(100);
    emit posFinished(pos);
};
