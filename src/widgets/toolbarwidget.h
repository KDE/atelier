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
