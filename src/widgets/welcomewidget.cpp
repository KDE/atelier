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

#include <KLocalizedString>
#include <QDomDocument>
#include <QHBoxLayout>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QScrollArea>
#include <QVBoxLayout>

#include "welcomewidget.h"

#define POSTS_LIMIT 5

WelcomeWidget::WelcomeWidget(QWidget *parent): QWidget(parent), m_newsFeedWidget(new QWidget)
{
    QFont appFont = font();

    auto hlayout = new QHBoxLayout;
    auto layout = new QVBoxLayout;
    auto label = new QLabel;


    label->setText(i18n("Welcome to Atelier!"));
    appFont.setPointSize(font().pointSize() + 4);
    label->setFont(appFont);
    hlayout->addWidget(label);

    label = new QLabel;
    label->setPixmap(QPixmap(":/icon/logo"));
    label->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
    hlayout->addWidget(label);
    layout->addItem(hlayout);

    auto line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    layout->addWidget(line);

    appFont.setPointSize(font().pointSize() + 2);
    label = new QLabel(i18n("Quick Connect Guide"));
    label->setFont(appFont);
    layout->addWidget(label);

    for(QString sentence : {i18n("1 - Create a Profile."), i18n("2 - Select the device."), i18n("3 - Select the profile and connect.")}){
        label = new QLabel(sentence);
        layout->addWidget(label);
    }

    label = new QLabel(i18n("Having a connection problem?"));
    label->setFont(appFont);
    layout->addWidget(label);

    label = new QLabel(i18n("See the info section of the Log for common problems."));
    layout->addWidget(label);

    label = new QLabel(i18n("Check our <a href=\"http://atelier.kde.org/documentation/atelier\">Atelier Docs</a> for more information."));
    label->setOpenExternalLinks(true);
    layout->addWidget(label);

    label = new QLabel(i18n("Check our last news!"));
    label->setFont(appFont);
    layout->addWidget(label);
    retrieveRssFeed(QUrl("https://rizzitello.wordpress.com/author/sithlord48/feed/"));
    layout->addWidget(m_newsFeedWidget);

    label = new QLabel(i18n("Get Involved"));
    label->setFont(appFont);
    layout->addWidget(label);

    label = new QLabel(QString("<a href=\"http://t.me/KDEAtelier\">")+i18n("Join our Telegram Group!")+QString("</a>"));
    label->setOpenExternalLinks(true);
    layout->addWidget(label);

    label = new QLabel(i18n("You can also find us on Freenode IRC #kde-atelier"));
    layout->addWidget(label);

    layout->addStretch();

    auto infoWidget = new QWidget();
    infoWidget->setLayout(layout);
    
    auto scrollArea = new QScrollArea(this);
    scrollArea->setWidget(infoWidget);
    scrollArea->setWidgetResizable(true);
    
    layout = new QVBoxLayout;
    layout->addWidget(scrollArea);
    setLayout(layout);
}

WelcomeWidget::~WelcomeWidget(){

}

void WelcomeWidget::retrieveRssFeed(const QUrl& url){

    auto manager = new QNetworkAccessManager();
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", "Atelier 1.0");

    connect(manager, &QNetworkAccessManager::finished, [&](QNetworkReply *reply){
        if(reply->error()){
            auto layout = new QHBoxLayout;
            layout->addWidget(new QLabel(i18n("Failed to fetch News.")));
            m_newsFeedWidget->setLayout(layout);
        }else{
            QDomDocument document;
            if(document.setContent(reply->readAll())){
                parseRss(document);
            }
        }
    });

    manager->get(request);
}

void WelcomeWidget::parseRss(const QDomDocument& document){
    QList<Post> postList;
    auto itemList = document.elementsByTagName("item");
    QRegularExpression dateRegex("(?<date>\\d{2} \\w{3} \\d{4})");

    for(int i=0; i < itemList.count(); ++i){
        auto node = itemList.at(i);
        if(node.isElement()){
            //Sample of date format Wed, 24 May 2017 13:46:07 +0000
            QString pDate = node.firstChildElement("pubDate").toElement().text();
            QRegularExpressionMatch match = dateRegex.match(pDate);
            if(match.hasMatch()){
                Post p;
                p.title = node.firstChildElement("title").toElement().text();
                p.url = node.firstChildElement("link").toElement().text();
                p.date = match.captured("date");
                postList.append(p);
            }
        }
    }
    auto layout = new QVBoxLayout;
    int count = postList.count() > POSTS_LIMIT ? POSTS_LIMIT : postList.count();
    for(int i =0; i < count; ++i){
        Post item = postList.at(i);
        QString url = QString("<a href=\"%1\">%2</a>").arg(item.url).arg(QString(item.title + " - " + item.date));
        auto lb = new QLabel(url);
        lb->setOpenExternalLinks(true);
        layout->addWidget(lb);
    }
    m_newsFeedWidget->setLayout(layout);
}
