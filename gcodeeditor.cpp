#include "gcodeeditor.h"
#include "ui_gcodeeditor.h"

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
