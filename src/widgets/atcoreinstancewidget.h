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
    void printFile(const QUrl &fileName);
    void updateProfileData();

private:
    AtCore m_core;
    BedExtruderWidget *m_bedExtWidget;
    CommandWidget *m_commandWidget;
    int m_fileCount;
    LogWidget *m_logWidget;
    MovementWidget *m_movementWidget;
    PlotWidget *m_plotWidget;
    PrintWidget *m_printWidget;
    SdWidget *m_sdWidget;
    StatusWidget *m_statusWidget;
    QAction *m_printAction;
    QAction *m_stopAction;
    QComboBox *m_comboPort;
    QComboBox *m_comboProfile;
    QMap<QString, QVariant> m_profileData;
    QPushButton *m_connectButton;
    QSettings m_settings;
    QString m_theme;
    QTabWidget *m_tabWidget;
    QToolBar *m_connectToolBar;
    QToolBar *m_toolBar;
    QWidget *m_advancedTab;
    QWidget *m_connectWidget;
    void buildConnectionToolbar();
    void buildToolbar();
    void checkTemperature(uint sensorType, uint number, uint temp);
    void connectButtonClicked();
    void disableMotors();
    void enableControls(bool b);
    void handlePrinterStatusChanged(AtCore::STATES newState);
    void initConnectsToAtCore();
    void stopPrint();
    void pausePrint();
    void print();
    void updateSerialPort(QStringList ports);
    void togglePrintButtons(bool shown);

signals:
    void connectionChanged(QString name);
    void disableDisconnect(bool b);
    void extruderCountChanged(int count);
    void requestProfileDialog();
    void requestFileChooser();
};
