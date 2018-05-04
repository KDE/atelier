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
#include <KLocalizedString>
#include <QLabel>
#include <QVBoxLayout>
#include "gcodeeditorwidget.h"

GCodeEditorWidget::GCodeEditorWidget(QWidget *parent) :
    QWidget(parent)
    , m_tabwidget(new QTabWidget())
{
    m_editor = KTextEditor::Editor::instance();
    setupTabWidget();
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(m_tabwidget);
    setLayout(layout);
}

void GCodeEditorWidget::setupTabWidget()
{
    connect(m_tabwidget, &QTabWidget::tabCloseRequested, this, &GCodeEditorWidget::closeTab);
    connect(m_tabwidget, &QTabWidget::currentChanged, this, &GCodeEditorWidget::currentIndexChanged);
    m_tabwidget->setTabsClosable(true);
}

KTextEditor::View *GCodeEditorWidget::gcodeView() const
{
    return qobject_cast<KTextEditor::View *>(m_editor->documents().first()->views().first());
}

void GCodeEditorWidget::loadFile(const QUrl &file)
{
    //if the file is loaded then reload the document.
    for (const auto &doc : m_editor->documents())
    {
        if (doc->url() == file) {
                doc->documentReload();
                m_tabwidget->setCurrentIndex(m_tabwidget->indexOf(urlTab[doc->url()]));
                return;
        }
    }
    auto doc = newDoc();
    int t = m_tabwidget->addTab(newView(doc), file.fileName());
    m_tabwidget->setCurrentIndex(t);
    doc->openUrl(file);
    doc->setHighlightingMode(QString("G-Code"));
    emit updateClientFactory(doc->views().first());
    urlTab.insert(doc->url(), m_tabwidget->widget(t));
    urlDoc.insert(doc->url(),m_editor->documents().count()-1);
}

void GCodeEditorWidget::setupInterface(const KTextEditor::View *view)
{
    m_interface = qobject_cast<KTextEditor::ConfigInterface *>(view);
    m_interface->setConfigValue("line-numbers", true);
}

KTextEditor::Document *GCodeEditorWidget::newDoc()
{
    KTextEditor::Document *doc = m_editor->createDocument(this);
    doc->setMode("G-Code");
    return doc;
}

KTextEditor::View *GCodeEditorWidget::newView(KTextEditor::Document *doc)
{
    auto view = doc->createView(this);
    setupInterface(view);
    return view;
}

void GCodeEditorWidget::closeTab(int index)
{
    QUrl url = urlTab.key(m_tabwidget->widget(index));
    m_tabwidget->removeTab(index);
    urlTab.remove(url);
    urlDoc.remove(url);
}

void GCodeEditorWidget::currentIndexChanged(int index)
{
    if (index != -1) {
        emit updateClientFactory(qobject_cast<KTextEditor::View *>(m_tabwidget->currentWidget()));
    }
}
