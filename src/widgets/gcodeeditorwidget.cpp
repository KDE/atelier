/* Atelier KDE Printer Host for 3D Printing
    Copyright (C) <2016>
    Author: Lays Rodrigues - laysrodrigues@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
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
    QRegularExpression catchEnd(";(\\s?)(End)(\\s?\\w+\\s?(\\w+)?)");
    catchEnd.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
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
        if (match.hasMatch()) {
            break;
        }
        header += (line + QStringLiteral("\n"));
    }
    int last = list.lastIndexOf(catchEnd);
    if (last != -1) {
        for (int i = last; i <= list.size() - 1; ++i) {
            footer += (list.at(i) + QStringLiteral("\n"));
        }
    }
    ui->headerPT->setPlainText(header);
    ui->footerPT->setPlainText(footer);
}
