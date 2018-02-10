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
#pragma once

#include <QMainWindow>
#include <QUrl>
#include <KTextEditor/View>
#include <KXmlGui/KXmlGuiWindow>
#include <dialogs/connectsettingsdialog.h>
#include <dialogs/profilesdialog.h>
#include <QStackedWidget>
#include <QToolButton>
#include <QMap>
#include <QPair>
#include <QToolButton>

namespace Ui
{
class MainWindow;
}

struct LateralArea {
    // Area with the the lateral buttons that will open the views.
    // Kind like the KDevelop stuff but way simpler.
    using Btn2Widget = QPair<QToolButton*, QWidget*>;
    using WidgetMap = QMap<QString, Btn2Widget>;

    QWidget *m_toolBar;
    QStackedWidget *m_stack;
    WidgetMap m_map;
};

class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QList<QUrl> m_openFiles;
    KTextEditor::View *m_currEditorView;
    LateralArea m_lateral;
    QTabWidget *m_instances;

    void setupLateralArea();
    void initWidgets();
    void setupActions();
    void openFile();
    void newConnection(const QString& port, const QMap<QString, QVariant>& profile);

signals:
    void extruderCountChanged(int count);
};
