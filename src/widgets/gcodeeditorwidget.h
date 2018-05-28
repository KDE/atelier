/* Atelier KDE Printer Host for 3D Printing
    Copyright (C) <2016>
    Author: Lays Rodrigues - lays.rodrigues@kde.org
            Chris Rizzitello - rizzitello@kde.org

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
#pragma once

#include <KTextEditor/ConfigInterface>
#include <KTextEditor/Document>
#include <KTextEditor/Editor>
#include <KTextEditor/View>
#include <QTabWidget>
#include <QWidget>

class GCodeEditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GCodeEditorWidget(QWidget *parent = nullptr);
    void loadFile(const QUrl &file);

private:
    QMap<QUrl, KTextEditor::Document *> urlDoc;
    QMap<QUrl, QWidget *> urlTab;
    KTextEditor::ConfigInterface *m_interface;
    KTextEditor::Document *newDoc(const QUrl &file);
    KTextEditor::Editor *m_editor;
    KTextEditor::View *newView(KTextEditor::Document *doc);
    QTabWidget *m_tabwidget;
    void closeTab(int index);
    void currentIndexChanged(int index);
    void setupInterface(const KTextEditor::View *view);
    void setupTabWidget();

signals:
    void currentFileChanged(const QUrl &file);
    void updateClientFactory(KTextEditor::View *view);
    void fileClosed(const QUrl &file);
};
