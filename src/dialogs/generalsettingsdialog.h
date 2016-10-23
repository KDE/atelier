#pragma once

#include <QDialog>

namespace Ui {
class GeneralSettingsDialog;
}

class GeneralSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GeneralSettingsDialog(QWidget *parent = nullptr);
    ~GeneralSettingsDialog();

private:
    Ui::GeneralSettingsDialog *ui;
};

