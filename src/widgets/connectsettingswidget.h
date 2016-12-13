#pragma once
#include <QWidget>

namespace Ui
{
class ConnectSettings;
}

class ConnectSettings : public QWidget
{
    Q_OBJECT

public:
    explicit ConnectSettings(QWidget *parent = nullptr);
    ~ConnectSettings();

private:
    Ui::ConnectSettings *ui;
};
