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
#include <AtCore/AtCore>
#include <AtCore/CommandWidget>
#include <AtCore/LogWidget>
#include <AtCore/MovementWidget>
#include <AtCore/PlotWidget>
#include <AtCore/PrintWidget>
#include <AtCore/SdWidget>
#include <AtCore/StatusWidget>
#include "bedextruderwidget.h"
#include <QComboBox>
#include <QList>
#include <QPushButton>
#include <QSettings>
#include <QToolBar>
#include <QUrl>
#include <QWidget>

/**
 * @todo write docs
 */
class AtCoreInstanceWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * Default constructor
     */
    AtCoreInstanceWidget(QWidget *parent = nullptr);

    /**
     * Destructor
     */
    ~AtCoreInstanceWidget();
    void startConnection(const QString &serialPort, const QMap<QString, QVariant> &profile);
    bool connected();
    void setFileCount(int count);

public slots:
    void updateProfileData();
    void printFile(const QUrl &fileName);
    bool isPrinting();

private:
    AtCore m_core;
    int m_fileCount;
    QToolBar *m_toolBar;
    QComboBox *m_comboPort;
    QComboBox *m_comboProfile;
    QPushButton *m_connectButton;
    QToolBar *m_connectToolBar;
    QWidget *m_connectWidget;
    QMap<QString, QVariant> profileData;
    QAction *m_printAction;
    QAction *m_stopAction;
    QSettings m_settings;
    QString m_theme;
    QTabWidget *m_tabWidget;
    QWidget *m_advancedTab;
    BedExtruderWidget *m_bedExtWidget;
    CommandWidget *m_commandWidget;
    LogWidget *m_logWidget;
    MovementWidget *m_movementWidget;
    PlotWidget *m_plotWidget;
    PrintWidget *m_printWidget;
    SdWidget *m_sdWidget;
    StatusWidget *m_statusWidget;
    void initConnectsToAtCore();
    void pausePrint();
    void stopPrint();
    void disableMotors();
    void handlePrinterStatusChanged(AtCore::STATES newState);
    void checkTemperature(uint sensorType, uint number, uint temp);
    void enableControls(bool b);
    void buildToolbar();
    void print();
    void updateSerialPort(const QStringList &ports);
    void buildConnectionToolbar();
    void connectButtonClicked();
    void togglePrintButtons(bool shown);
signals:
    void connectionChanged(QString name);
    void disableDisconnect(bool b);
    void extruderCountChanged(int count);
    void requestProfileDialog();
    void requestFileChooser();
};
