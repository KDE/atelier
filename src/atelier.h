#pragma once

#include <QObject>
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
    void percentagePrinted();

signals:
    void percentage(float percent);
    void updateTemperatures(float bed, float ext);
};
