#include "connectsettings.h"
#include "ui_connectsettings.h"

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
