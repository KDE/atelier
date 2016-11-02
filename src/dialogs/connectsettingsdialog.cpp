#include "connectsettingsdialog.h"
#include "ui_connectsettingsdialog.h"

ConnectSettingsDialog::ConnectSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectSettingsDialog)
{
    ui->setupUi(this);
}

ConnectSettingsDialog::~ConnectSettingsDialog()
{
    delete ui;
}
