#include "gcodeeditorwidget.h"
#include "ui_gcodeeditorwidget.h"
#include <QTextStream>
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

void GCodeEditorWidget::loadFile(const QString &fileName)
{
    QRegularExpression cacthMov("G(?<command>.) .*\\bX(?<axisX>[0-9.-]+) Y(?<axisY>[0-9.-]+) Z(?<axisZ>[0-9].+)");
    QRegularExpression catchEnd(";(End)(\\s?\\w+\\s?(\\w+)?)");
    QRegularExpressionMatch match;
    QFile gcode(fileName);
    if (!gcode.open(QFile::ReadOnly)) {
        return;
    }
    QTextStream gcodeStream(&gcode);
    QString content = gcodeStream.readAll();
    //TODO: Send this content to 3DView
    QStringList list = content.split(QStringLiteral("\n"), QString::SkipEmptyParts);
    QString header, footer;
    foreach (const auto &line, list) {
        match = cacthMov.match(line);
        if(match.hasMatch()){
            break;
        }
        header += (line + QStringLiteral("\n"));
    }
    int last = list.lastIndexOf(catchEnd);
    for(int i = last; i!= list.size() -1; ++i) {
        footer += (list.at(i) + QStringLiteral("\n"));
    }
    ui->headerPT->setPlainText(header);
    ui->footerPT->setPlainText(footer);
}
