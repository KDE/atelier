/* Atelier KDE Printer Host for 3D Printing
    Copyright (C) <2017>
    Author: Lays Rodrigues - laysrodriguessilva@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include <QDialog>
#include <QList>
#include <QUrl>

class ChooseFileDialog : public QDialog
{
    Q_OBJECT
public:
    ChooseFileDialog(QWidget *parent=nullptr, QList<QUrl> files = QList<QUrl>());
    virtual ~ChooseFileDialog() {}
    const QString& choosenFile();
private:
    QString m_choosen_file;
};
