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
#include "atelier.h"
#include <QCoreApplication>
#include <QFile>
#include <QEventLoop>
#include <QTextStream>
#include <QDir>
#include <QTime>
#include <KAtCore/seriallayer.h>

Atelier::Atelier():
    core(new AtCore(this)),
    logFile(new QTemporaryFile(QDir::tempPath() + QStringLiteral("/AtCore_")))
{
    connect(core, &AtCore::printerStatusChanged, this, &Atelier::checkPrinterStatus);
    connect(core, &AtCore::printProgressChanged, this, &Atelier::percentage);
}

Atelier::~Atelier()
{
    delete logFile;
}

void Atelier::startConnectProccess(QString port, QString baud)
{
    core->initFirmware(port, baud.toInt());
    connect(core->serial(), &SerialLayer::receivedCommand, this, &Atelier::checkReceivedCommand);
    connect(core->serial(), &SerialLayer::pushedCommand, this, &Atelier::checkPushedCommands);
}

void Atelier::checkPrinterStatus(PrinterStatus status)
{
   QString msg = QString::fromLatin1("Bed:%1/%2  Extruder:%3/%4")
                      .arg(QString::number(status.bedTemp))
                      .arg(QString::number(status.bedTargetTemp))
                      .arg(QString::number(status.extruderTemp))
                      .arg(QString::number(status.extruderTargetTemp));
    emit updateTemperatures(status.bedTemp,status.extruderTemp);
    addRLog(msg);
}

void Atelier::pausePrint()
{
   core->pause();
}

void Atelier::stopPrint()
{
    core->stop();
}

void Atelier::emergencyStopPrint()
{
    core->stop();
}

QStringList Atelier::availablePlugins()
{
    return core->availablePlugins();
}

void Atelier::homeAll()
{
    core->home();
}

void Atelier::homeX()
{
    core->home(AXIS::X);
}

void Atelier::homeY()
{
    core->home(AXIS::Y);
}

void Atelier::homeZ()
{
    core->home(AXIS::Z);
}

void Atelier::setBedTemp(int temp)
{
    core->setBedTemp(temp);
}

void Atelier::setExtTemp(int ext, int temp)
{
    core->setExtruderTemp(temp, ext);
}

void Atelier::moveAxis(QLatin1Char axis, int dist)
{
    if (axis == QLatin1Char('X'))
        core->move(AXIS::X, dist);
    else if (axis == QLatin1Char('Y'))
        core->move(AXIS::Y, dist);
    else
        core->move(AXIS::Z, dist);
}

void Atelier::fanSpeed(int speed)
{
    core->setFanSpeed(speed);
}

void Atelier::printSpeed(int speed)
{
    core->setPrinterSpeed(speed);
}

void Atelier::printFile(const QString &fileName)
{
    core->print(fileName);
}

void Atelier::percentagePrinted(){
    emit percentage(core->percentagePrinted());
}

void Atelier::writeTempFile(QString text)
{
    /*
    A QTemporaryFile will always be opened in QIODevice::ReadWrite mode,
    this allows easy access to the data in the file. This function will
    return true upon success and will set the fileName() to the unique
    filename used.
    */
    logFile->open();
    logFile->seek(logFile->size());
    logFile->write(text.toLatin1());
    logFile->putChar('\n');
    logFile->close();
}

QString Atelier::getTime()
{
    return QTime::currentTime().toString(QStringLiteral("hh:mm:ss:zzz"));
}

QString Atelier::logHeader()
{
    return QStringLiteral("[%1]  ").arg(getTime());
}

QString Atelier::rLogHeader()
{
    return QStringLiteral("[%1]< ").arg(getTime());
}

QString Atelier::sLogHeader()
{
    return QStringLiteral("[%1]> ").arg(getTime());
}

void Atelier::addLog(QString msg)
{
    QString message(logHeader() + msg);
    emit updateLog(message);
    writeTempFile(message);
}

void Atelier::addRLog(QString msg)
{
    QString message(rLogHeader() + msg);
    emit updateLog(message);
    writeTempFile(message);
}

void Atelier::addSLog(QString msg)
{
    QString message(sLogHeader() + msg);
    emit updateLog(message);
    writeTempFile(message);
}

void Atelier::checkReceivedCommand()
{
    addRLog(QString::fromUtf8(core->popCommand()));
}

void Atelier::checkPushedCommands(QByteArray bmsg)
{
    QString msg = QString::fromUtf8(bmsg);
    QRegExp _newLine(QChar::fromLatin1('\n'));
    QRegExp _return(QChar::fromLatin1('\r'));
    msg.replace(_newLine, QStringLiteral("\\n"));
    msg.replace(_return, QStringLiteral("\\r"));
    addSLog(msg);
}
