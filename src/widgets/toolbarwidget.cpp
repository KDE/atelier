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
#include "toolbarwidget.h"
#include <QHBoxLayout>
#include <QFileDialog>
#include <KLocalizedString>

ToolBarWidget::ToolBarWidget(QWidget *parent) : QWidget(parent),
    settingsTB(new QToolButton),
    openFileTB(new QToolButton),
    connectTB(new QToolButton),
    startTB(new QToolButton),
    pauseTB(new QToolButton),
    stopTB(new QToolButton)
{
    auto *layout = new QHBoxLayout;
    initButtons();
    layout->addWidget(settingsTB);
    layout->addWidget(openFileTB);
    layout->addWidget(connectTB);
    layout->addSpacing(400);
    layout->addWidget(startTB);
    layout->addWidget(pauseTB);
    layout->addWidget(stopTB);
    this->setLayout(layout);

    connect(settingsTB, &QToolButton::clicked, [=]{
        auto *dialog = new GeneralSettingsDialog();
        dialog->show();
    });

    connect(connectTB, &QToolButton::clicked, [=]{
       auto *dialog = new ConnectSettingsDialog(firmwaresList);
       dialog->show();
       connect(dialog, &ConnectSettingsDialog::_connect, this, &ToolBarWidget::_connect);
    });

    connect(openFileTB, &QToolButton::clicked, [=]{
       fileName = QFileDialog::getOpenFileName(this, i18n("Select a file to print"),
                                                       QDir::homePath(), i18n("GCode(*.gco *gcode)"));
       emit loadFile(fileName);
    });

    connect(startTB, &QToolButton::clicked, [=]{
        emit printFile(fileName);
    });

    connect(pauseTB, &QToolButton::clicked, this, &ToolBarWidget::pausePrint);
    connect(stopTB, &QToolButton::clicked, this, &ToolBarWidget::stopPrint);
}

void ToolBarWidget::setFirmwaresList(QStringList fw)
{
    firmwaresList = fw;
}

void ToolBarWidget::initButtons()
{
    settingsTB->setText(i18n("Settings"));
    openFileTB->setText(i18n("Open File"));
    connectTB->setText(i18n("Connect"));
    startTB->setText(i18n("Start"));
    pauseTB->setText(i18n("Pause"));
    stopTB->setText(i18n("Stop"));
}
