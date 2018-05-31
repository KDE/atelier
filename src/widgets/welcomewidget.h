/* Atelier KDE Printer Host for 3D Printing
    Copyright (C) <2018>
    Author: Lays Rodrigues - lays.rodrigues@kde.org

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

#include <QWidget>

class QDomDocument;
class QUrl;

struct Post{
    QString title;
    QString url;
    QString date;
};

class WelcomeWidget : public QWidget
{
    Q_OBJECT

public:
    WelcomeWidget(QWidget *parent=nullptr);
    ~WelcomeWidget();

private:
    void retrieveRssFeed(const QUrl& url);
    void parseRss(const QDomDocument& document);
    QWidget *m_newsFeedWidget;
};
