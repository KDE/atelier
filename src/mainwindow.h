#pragma once

#include <QMainWindow>
#include "atelier.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Atelier *atelier;
    void initConnectsToAtelier();
};
