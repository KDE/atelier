#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    atelier(new Atelier())
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
    initConnectsToAtelier();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initConnectsToAtelier()
{
    connect(ui->toolbarWidget, &ToolBarWidget::_connect, atelier, &Atelier::startConnectProccess);

    connect(ui->rightWidget, &TemporaryPrinterControlWidget::homeAll, atelier, &Atelier::homeAll);
    connect(ui->rightWidget, &TemporaryPrinterControlWidget::homeX, atelier, &Atelier::homeX);
    connect(ui->rightWidget, &TemporaryPrinterControlWidget::homeY, atelier, &Atelier::homeY);
    connect(ui->rightWidget, &TemporaryPrinterControlWidget::homeZ, atelier, &Atelier::homeZ);
    connect(ui->rightWidget, &TemporaryPrinterControlWidget::moveAxis, atelier, &Atelier::moveAxis);
    connect(ui->rightWidget, &TemporaryPrinterControlWidget::changeFanSpeed, atelier, &Atelier::fanSpeed);
    connect(ui->rightWidget, &TemporaryPrinterControlWidget::changePrintSpeed, atelier, &Atelier::printSpeed);
    connect(ui->rightWidget, &TemporaryPrinterControlWidget::setHeatBed, atelier, &Atelier::setBedTemp);
    connect(ui->rightWidget, &TemporaryPrinterControlWidget::setHeatExtruder, atelier, &Atelier::setExtTemp);

    connect(atelier, &Atelier::percentage, ui->rightWidget, &TemporaryPrinterControlWidget::updateProgressBar);
    connect(atelier, &Atelier::updateTemperatures, ui->rightWidget, &TemporaryPrinterControlWidget::updateTemperatures);

    connect(ui->toolbarWidget, &ToolBarWidget::printFile, atelier, &Atelier::printFile);

    connect(ui->toolbarWidget, &ToolBarWidget::pausePrint, atelier, &Atelier::pausePrint);
    connect(ui->toolbarWidget, &ToolBarWidget::stopPrint, atelier, &Atelier::stopPrint);
    ui->toolbarWidget->setFirmwaresList(atelier->availablePlugins());
}
