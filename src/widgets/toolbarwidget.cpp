#include "toolbarwidget.h"
#include <QHBoxLayout>
#include <QFileDialog>

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
       fileName = QFileDialog::getOpenFileName(this, tr("Select a file to print"),
                                                       QDir::homePath(), tr("GCode(*.gco *gcode)"));
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
    settingsTB->setText("Settings");
    openFileTB->setText("Open File");
    connectTB->setText("Connect");
    startTB->setText("Start");
    pauseTB->setText("Pause");
    stopTB->setText("Stop");
}
