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

void GCodeEditorWidget::loadFile(const QUrl &file)
{
    //if the file is loaded then reload the document.
    if (urlDoc.contains(file)) {
        urlDoc[file]->documentReload();
        m_tabwidget->setCurrentIndex(m_tabwidget->indexOf(urlTab[file]));
        return;
    }
    auto doc = newDoc();
    int t = m_tabwidget->addTab(newView(doc), file.fileName());
    doc->openUrl(file);
    doc->setHighlightingMode(QString("G-Code"));
    urlTab[doc->url()] = m_tabwidget->widget(t);
    urlDoc[doc->url()] = doc;
    m_tabwidget->setCurrentIndex(t);
    //connect our new document's modified state changed signal.
    connect(doc, &KTextEditor::Document::modifiedChanged, [this, t](const KTextEditor::Document * document) {
        QString filename = document->url().fileName(QUrl::FullyDecoded);
        if (document->isModified()) {
            filename.append(" *");
        }
        m_tabwidget->setTabText(t, filename);
    });
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
    auto doc = urlDoc[url];
    if (doc->closeUrl()) {
        m_tabwidget->removeTab(index);
        urlTab.remove(url);
        urlDoc.remove(url);
        emit fileClosed(url);
    }
}

void GCodeEditorWidget::currentIndexChanged(int index)
{
    if (index != -1) {
        emit updateClientFactory(qobject_cast<KTextEditor::View *>(m_tabwidget->currentWidget()));
    } else {
        emit(updateClientFactory(nullptr));
    }
}
