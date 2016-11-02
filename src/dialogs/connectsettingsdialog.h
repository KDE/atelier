#pragma once

#include <QDialog>

namespace Ui {
class ConnectSettingsDialog;
}

class ConnectSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectSettingsDialog(QWidget *parent = nullptr);
    ~ConnectSettingsDialog();

private:
    Ui::ConnectSettingsDialog *ui;
};
