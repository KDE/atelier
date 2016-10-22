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
    gcodeEditorPB->setText("GCode Editor");
    maintanceMenuPB->setText("Maintance Menu");
}
