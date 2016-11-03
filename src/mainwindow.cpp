#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->gcodeEditorWidget->setVisible(false);

    connect(ui->actionGCode, &QAction::triggered, [=]{
        ui->gcodeEditorWidget->setVisible(true);
    });

    connect(ui->actionOpenGCode, &QAction::triggered, [=]{
        QString fileName = QFileDialog::getOpenFileName(this, tr("Select a file to print"),
                                                        QDir::homePath(), tr("GCode(*.gco *gcode)"));
        ui->gcodeEditorWidget->loadFile(fileName);
    });

    connect(ui->toolbarWidget, &ToolBarWidget::loadFile, ui->gcodeEditorWidget, &GCodeEditorWidget::loadFile);
}

MainWindow::~MainWindow()
{
    delete ui;
}
