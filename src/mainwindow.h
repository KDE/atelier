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

#include <KTextEditor/View>
#include <KXmlGui/KXmlGuiWindow>
#include <QMap>
#include <QPair>
#include <QPushButton>
#include <QStackedWidget>
#include <QUrl>
#include "widgets/gcodeeditorwidget.h"

struct LateralArea {
    // Area with the the lateral buttons that will open the views.
    // Kind like the KDevelop stuff but way simpler.
    using Btn2Widget = QPair<QPushButton *, QWidget *>;
    using WidgetMap = QMap<QString, Btn2Widget>;

    QWidget *m_toolBar;
    QStackedWidget *m_stack;
    WidgetMap m_map;
    template<typename T> T *get(const QString &s)
    {
        return qobject_cast<T *>(m_map[s].second);
    }
};

class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    void closeEvent(QCloseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private:
    GCodeEditorWidget *m_gcodeEditor;
    KTextEditor::View *m_currEditorView;
    LateralArea m_lateral;
    QList<QUrl> m_openFiles;
    QString m_theme;
    QTabWidget *m_instances;
    bool askToClose();
    void atCoreInstanceNameChange(const QString &name);
    QString getTheme();
    void initWidgets();
    void loadFile(const QUrl &fileName);
    void newAtCoreInstance();
    void openActionTriggered();
    void processDropEvent(const QList<QUrl> &fileList);
    void setupActions();
    void setupLateralArea();
    void toggleGCodeActions();
    void updateClientFactory(KTextEditor::View *view);

signals:
    void extruderCountChanged(int count);
    void profilesChanged();
};
