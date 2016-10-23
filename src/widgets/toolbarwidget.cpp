#include "toolbarwidget.h"
#include "../dialogs/generalsettingsdialog.h"
#include <QHBoxLayout>

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
