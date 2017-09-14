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

#include "gcodeto4d.h"
#include "fileloader.h"

#include <QFile>
#include <QThread>
#include <QUrl>
#include <QVector4D>
#include <QVariant>

GcodeTo4D::GcodeTo4D(QObject *parent) :
    QObject(parent)
{
}

void GcodeTo4D::read(const QString &url)
{
    _thread = new QThread;
    QString path = QUrl(url).path();
    auto fileLoader = new FileLoader(path);
    fileLoader->moveToThread(_thread);
    connect(fileLoader, &FileLoader::percentUpdate, this, &GcodeTo4D::percentUpdate);
    connect(fileLoader, &FileLoader::posFinished, this, &GcodeTo4D::posFinished);
    connect(fileLoader, &FileLoader::posFinished, _thread, &QThread::quit);
    connect(_thread, &QThread::started, fileLoader, &FileLoader::run);
    connect(_thread, &QThread::finished, fileLoader, &FileLoader::deleteLater);
    _thread->start();
}

GcodeTo4D::~GcodeTo4D()
{
}