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

#include <QObject>
#include <QTemporaryFile>
#include <KAtCore/AtCore>

namespace Ui {
class Atelier;
}

class Atelier : public QObject
{
    Q_OBJECT

public:
    explicit Atelier();
    ~Atelier();
    void startConnectProccess(QString port, QString baud);
    void homeAll();
    void homeX();
    void homeY();
    void homeZ();
    void setBedTemp(int temp);
    void setExtTemp(int ext, int temp);
    void moveAxis(QLatin1Char axis, int dist);
    void fanSpeed(int speed);
    void printSpeed(int speed);
    void printFile(const QString &fileName);
    void checkPrinterStatus(PrinterStatus status);
    void pausePrint();
    void stopPrint();
    void emergencyStopPrint();
    QStringList availablePlugins();

private:
    Ui::Atelier *ui;
    AtCore *core;
    QTemporaryFile *logFile;
    void percentagePrinted();
    void addLog(QString msg);
    void addRLog(QString msg);
    void addSLog(QString msg);
    void checkReceivedCommand();
    void checkPushedCommands(QByteArray bmsg);
    void writeTempFile(QString text);
    QString getTime();
    QString logHeader();
    QString rLogHeader();
    QString sLogHeader();

signals:
    void percentage(const float &percent);
    void updateTemperatures(float bed, float ext);
    void updateLog(QString message);
};
