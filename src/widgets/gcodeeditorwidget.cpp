#include "gcodeeditorwidget.h"
#include "ui_gcodeeditorwidget.h"
GCodeEditorWidget::GCodeEditorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GCodeEditorWidget)
{
    ui->setupUi(this);
}

GCodeEditorWidget::~GCodeEditorWidget()
{
    delete ui;
}
