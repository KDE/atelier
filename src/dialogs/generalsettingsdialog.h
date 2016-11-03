#pragma once

#include <QDialog>
#include <QSettings>

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
    void saveSettings();
    void loadSettings(QString currentProfile);
    QSettings settings;
};

