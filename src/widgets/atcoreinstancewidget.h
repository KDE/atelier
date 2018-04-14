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
#include <AtCore/AxisControl>
#include <AtCore/LogWidget>
#include <AtCore/PlotWidget>
#include <AtCore/SdWidget>
#include <AtCore/StatusWidget>
#include <QComboBox>
#include <QList>
#include <QPushButton>
#include <QSettings>
#include <QToolBar>
#include <QUrl>
#include <QWidget>
namespace Ui
{
class AtCoreInstanceWidget;
}

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
    AtCoreInstanceWidget(QWidget* parent = nullptr);

    /**
     * Destructor
     */
    ~AtCoreInstanceWidget();
    void startConnection(const QString& serialPort, const QMap<QString, QVariant>& profile);
    bool connected();
    void setOpenFiles(const QList<QUrl>& files);

public slots:
    void updateProfileData();

private:
    Ui::AtCoreInstanceWidget* ui;
    AtCore m_core;
    QToolBar *m_toolBar;
    QComboBox *m_comboPort;
    QComboBox *m_comboProfile;
    QPushButton *m_connectButton;
    QToolBar *m_connectToolBar;
    QWidget *m_connectWidget;
    QMap<QString, QVariant> profileData;
    QList<QUrl> m_files;
    QAction *m_printAction;
    QAction *m_stopAction;
    QSettings m_settings;
    QString m_theme;
    AxisControl *m_axisControl;
    LogWidget *m_logWidget;
    PlotWidget *m_plotWidget;
    SdWidget *m_sdWidget;
    StatusWidget *m_statusWidget;
    void initConnectsToAtCore();
    void printFile(const QUrl& fileName);
    void pausePrint();
    void stopPrint();
    void disableMotors();
    void handlePrinterStatusChanged(AtCore::STATES newState);
    void checkTemperature(uint sensorType, uint number, uint temp);
    void axisControlClicked(QChar axis, int value);
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
};
