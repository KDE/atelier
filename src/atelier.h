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
    void percentage(float percent);
    void updateTemperatures(float bed, float ext);
    void updateLog(QString message);
};
