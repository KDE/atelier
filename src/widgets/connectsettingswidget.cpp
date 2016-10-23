#include "connectsettingswidget.h"
#include "ui_connectsettingswidget.h"

ConnectSettings::ConnectSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectSettings)
{
    ui->setupUi(this);
}

ConnectSettings::~ConnectSettings()
{
    delete ui;
}
