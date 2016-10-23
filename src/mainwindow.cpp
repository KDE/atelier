#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->connectContainer->setVisible(false);
    ui->gcodeContainer->setVisible(false);
    ui->maintanceContainer->setVisible(false);

    connect(ui->leftWidget, &LeftWidget::loadConnectContainer,[=]{
        ui->gcodeContainer->setVisible(false);
        ui->maintanceContainer->setVisible(false);
        ui->connectContainer->setVisible(true);
    });

    connect(ui->leftWidget, &LeftWidget::loadGCodeContainer,[=]{
        ui->maintanceContainer->setVisible(false);
        ui->connectContainer->setVisible(false);
        ui->gcodeContainer->setVisible(true);
    });


    connect(ui->leftWidget, &LeftWidget::loadMaintanceContainer,[=]{
        ui->connectContainer->setVisible(false);
        ui->gcodeContainer->setVisible(false);
        ui->maintanceContainer->setVisible(true);
    });

    connect(ui->leftWidget, &LeftWidget::hideContainers, [=]{
        ui->connectContainer->setVisible(false);
        ui->gcodeContainer->setVisible(false);
        ui->maintanceContainer->setVisible(false);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
