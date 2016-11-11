#include "atelier.h"
#include <QCoreApplication>
#include <QFile>
#include <QEventLoop>
#include <QTextStream>
Atelier::Atelier():
    core(new AtCore(this))
{
    connect(core, &AtCore::printerStatusChanged, this, &Atelier::checkPrinterStatus);
}

Atelier::~Atelier()
{

}

void Atelier::startConnectProccess(QString port, QString baud)
{
    core->initFirmware(port, baud.toInt());
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
    if (core->state() == PrinterState::PAUSE) {
        core->setState(PrinterState::IDLE);
    }
    core->setState(PrinterState::PAUSE);
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
