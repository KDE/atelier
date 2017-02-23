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
#include <QVBoxLayout>
#include <QPushButton>

GCodeEditorWidget::GCodeEditorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GCodeEditorWidget)
{
    ui->setupUi(this);
    editor = KTextEditor::Editor::instance();
    doc = editor->createDocument(this);
    doc->setMode("G-Code");
    view = doc->createView(ui->containerWidget);
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(view);
    ui->containerWidget->setLayout(layout);
    setupInterface();
}

GCodeEditorWidget::~GCodeEditorWidget()
{
    delete ui;
}

void GCodeEditorWidget::loadFile(const QUrl &fileName)
{
    doc->openUrl(fileName);
    doc->setHighlightingMode(QString("G-Code"));
}

KTextEditor::View* GCodeEditorWidget::gcodeView() const
{
    return view;
}

void GCodeEditorWidget::setupInterface()
{
    interface = qobject_cast<KTextEditor::ConfigInterface*>(view);
    interface->setConfigValue("line-numbers", true);
}
