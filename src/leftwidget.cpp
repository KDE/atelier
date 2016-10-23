#include "leftwidget.h"
#include <QVBoxLayout>
LeftWidget::LeftWidget(QWidget *parent) : QWidget(parent),
  connectSettingsPB(new QPushButton),
  gcodeEditorPB(new QPushButton),
  maintanceMenuPB(new QPushButton)
{
    auto *layout = new QVBoxLayout;
    initButtons();
    layout->addWidget(connectSettingsPB);
    layout->addWidget(gcodeEditorPB);
    layout->addWidget(maintanceMenuPB);
    layout->addSpacing(400);
    this->setLayout(layout);
}

void LeftWidget::initButtons()
{
    connectSettingsPB->setText("Connect Settings");
    connectSettingsPB->setCheckable(true);
    gcodeEditorPB->setText("GCode Editor");
    gcodeEditorPB->setCheckable(true);
    maintanceMenuPB->setText("Maintance Menu");
    maintanceMenuPB->setCheckable(true);

    connect(connectSettingsPB, &QPushButton::clicked, [=] {
        if(connectSettingsPB->isChecked()) {
            gcodeEditorPB->setChecked(false);
            maintanceMenuPB->setChecked(false);
            emit loadConnectContainer();
        }else {
            emit hideContainers();
        }
    });

    connect(gcodeEditorPB, &QPushButton::clicked, [=] {
        if(gcodeEditorPB->isChecked()) {
            connectSettingsPB->setChecked(false);
            maintanceMenuPB->setChecked(false);
            emit loadGCodeContainer();
        }else {
            emit hideContainers();
        }
    });

    connect(maintanceMenuPB, &QPushButton::clicked, [=] {
        if(maintanceMenuPB->isChecked()) {
            connectSettingsPB->setChecked(false);
            gcodeEditorPB->setChecked(false);
            emit loadMaintanceContainer();
        }else {
            emit hideContainers();
        }
    });
}

