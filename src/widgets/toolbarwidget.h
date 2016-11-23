/* Atelier KDE Printer Host for 3D Printing
    Copyright (C) <2016>
    Author: Lays Rodrigues - laysrodrigues@gmail.com

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
#include <QWidget>
#include <QToolButton>
#include "../dialogs/generalsettingsdialog.h"
#include "../dialogs/connectsettingsdialog.h"
class ToolBarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ToolBarWidget(QWidget *parent = nullptr);
    void setFirmwaresList(QStringList fw);

private:
    QToolButton *settingsTB;
    QToolButton *openFileTB;
    QToolButton *connectTB;
    QToolButton *startTB;
    QToolButton *pauseTB;
    QToolButton *stopTB;
    void initButtons();
    QString fileName;
    QStringList firmwaresList;


signals:
    void loadFile(QString file);
    void _connect(QString port, QString baud);
    void printFile(QString fileName);
    void pausePrint();
    void stopPrint();

};
