#include "gcodeeditorwidget.h"
#include "ui_gcodeeditorwidget.h"

GCodeEditor::GCodeEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GCodeEditor)
{
    ui->setupUi(this);
}

GCodeEditor::~GCodeEditor()
{
    delete ui;
}
