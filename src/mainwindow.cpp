#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->gcodeEditorWidget->setVisible(false);

    connect(ui->actionGCode, &QAction::triggered, [=]{
        ui->gcodeEditorWidget->setVisible(true);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
