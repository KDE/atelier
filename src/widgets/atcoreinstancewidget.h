/* Atelier KDE Printer Host for 3D Printing
    Copyright (C) <2017>
    Author: Lays Rodrigues - lays.rodrigues@kde.org
            Chris Rizzitello - rizzitello@kde.org

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
#include <AtCore>
#include <CommandWidget>
#include <LogWidget>
#include <MovementWidget>
#include <PlotWidget>
#include <PrintWidget>
#include <SdWidget>
#include <StatusWidget>
#include <QComboBox>
#include <QList>
#include <QPushButton>
#include <QSettings>
#include <QToolBar>
#include <QUrl>
#include <QWidget>
#include "bedextruderwidget.h"

/**
 * @todo write docs
 */
class AtCoreInstanceWidget : public QWidget
{
    Q_OBJECT

public:
    AtCoreInstanceWidget(QWidget *parent = nullptr);
    ~AtCoreInstanceWidget();
    bool connected();
    void setFileCount(int count);
    void startConnection(const QString &serialPort, const QMap<QString, QVariant> &profile);

public slots:
    bool isPrinting();
    QSize bedSize();
    void printFile(const QUrl &fileName);
    void updateProfileData();

private:
    AtCore m_core;
    BedExtruderWidget *m_bedExtWidget = nullptr;
    CommandWidget *m_commandWidget = nullptr;
    int m_fileCount = 0;
    LogWidget *m_logWidget = nullptr;
    MovementWidget *m_movementWidget = nullptr;
    PlotWidget *m_plotWidget = nullptr;
    PrintWidget *m_printWidget = nullptr;
    SdWidget *m_sdWidget = nullptr;
    StatusWidget *m_statusWidget = nullptr;
    QAction *m_printAction = nullptr;
    QAction *m_stopAction = nullptr;
    QComboBox *m_comboPort = nullptr;
    QComboBox *m_comboProfile = nullptr;
    QMap<QString, QVariant> m_profileData;
    QPushButton *m_connectButton = nullptr;
    QSettings m_settings;
    QSize m_iconSize;
    QString m_theme;
    QTabWidget *m_tabWidget = nullptr;
    QToolBar *m_connectToolBar = nullptr;
    QToolBar *m_toolBar = nullptr;
    QWidget *m_advancedTab = nullptr;
    QWidget *m_connectWidget = nullptr;
    QSize m_bedSize;
    QTimer *m_connectionTimer = nullptr;
    void buildConnectionToolbar();
    void buildToolbar();
    void checkTemperature(uint sensorType, uint number, float temp);
    void connectButtonClicked();
    void connectBedTemperatureData(bool connected);
    void connectExtruderTemperatureData(bool connected);
    void disableMotors();
    void enableControls(bool b);
    void handlePrinterStatusChanged(AtCore::STATES newState);
    void initConnectsToAtCore();
    void stopPrint();
    QMap<QString, QVariant> readProfile();
    void saveProfile();
    void pausePrint();
    void print();
    void updateSerialPort(QStringList ports);
    void togglePrintButtons(bool shown);

signals:
    void bedSizeChanged(QSize bedSize);
    void connectionChanged(QString name);
    void disableDisconnect(bool b);
    void extruderCountChanged(int count);
    void requestProfileDialog();
    void requestFileChooser();
};
