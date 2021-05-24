/* Atelier KDE Printer Host for 3D Printing
    Copyright (C) <2018>
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
#include "welcomewidget.h"
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
#include <QStyle>
#include <QTimer>
#include <QToolButton>
#include <QVBoxLayout>
#include <algorithm>

#define POSTS_LIMIT 5
const QString WelcomeWidget::m_telegramLink = QStringLiteral(R"(<a href="http://t.me/KDEAtelier">)");
const QString WelcomeWidget::m_documentsLink = QStringLiteral(R"(<a href="http://atelier.kde.org/documentation/atelier">)");
const QString WelcomeWidget::m_linkClose = QStringLiteral("</a>");

WelcomeWidget::WelcomeWidget(QWidget *parent)
    : QWidget(parent)
    , m_newsFeedWidget(new QWidget(this))
{
    QFont appFont = font();

    auto hlayout = new QHBoxLayout;
    auto layout = new QVBoxLayout;
    auto label = new QLabel(this);

    label->setText(i18n("Welcome to Atelier!"));
    appFont.setPointSize(font().pointSize() + 4);
    label->setFont(appFont);
    hlayout->addWidget(label);

    label = new QLabel(this);
    label->setPixmap(QPixmap(":/icon/logo"));
    label->setLayoutDirection(Qt::LayoutDirection::RightToLeft);
    hlayout->addWidget(label);
    layout->addItem(hlayout);

    auto line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    layout->addWidget(line);

    appFont.setPointSize(font().pointSize() + 2);
    label = new QLabel(i18n("Quick Connect Guide"), this);
    label->setFont(appFont);
    layout->addWidget(label);

    for (const QString &sentence : {i18n("1 - Create a Profile."), i18n("2 - Select the device."), i18n("3 - Select the profile and connect.")}) {
        label = new QLabel(sentence, this);
        layout->addWidget(label);
    }

    label = new QLabel(i18n("Having a connection problem?"), this);
    label->setFont(appFont);
    layout->addWidget(label);

    label = new QLabel(i18n("See the info section of the Log for common problems."), this);
    layout->addWidget(label);

    label = new QLabel(i18n("Check our %1Atelier Docs%2 for more information.", m_documentsLink, m_linkClose), this);
    label->setOpenExternalLinks(true);
    layout->addWidget(label);

    label = new QLabel(i18n("Atelier news!"), this);
    label->setFont(appFont);
    layout->addWidget(label);
    retrieveRssFeed();
    layout->addWidget(m_newsFeedWidget);

    label = new QLabel(i18n("Get Involved"), this);
    label->setFont(appFont);
    layout->addWidget(label);

    label = new QLabel(i18n("Join our %1Telegram Group%2!", m_telegramLink, m_linkClose), this);
    label->setOpenExternalLinks(true);
    layout->addWidget(label);

    label = new QLabel(i18n("You can also find us on Libera Chat IRC #kde-atelier"), this);
    layout->addWidget(label);

    layout->addStretch();

    auto infoWidget = new QWidget(this);
    infoWidget->setLayout(layout);

    auto scrollArea = new QScrollArea(this);
    scrollArea->setWidget(infoWidget);
    scrollArea->setWidgetResizable(true);

    layout = new QVBoxLayout;
    layout->addWidget(scrollArea);
    setLayout(layout);
}

void WelcomeWidget::retrieveRssFeed()
{
    m_postList.clear();
    auto manager = new QNetworkAccessManager(this);
    for (const QUrl &url : {QUrl("https://rizzitello.wordpress.com/category/atelier/feed/"), QUrl("https://laysrodriguesdev.wordpress.com/category/atelier/feed/")}) {
        QNetworkRequest request(url);
        request.setRawHeader("User-Agent", "Atelier 1.0");
        manager->get(request);
        connect(manager, &QNetworkAccessManager::finished, this, [&](QNetworkReply *reply) {
            if (reply->error()) {
                return;
            }
            QDomDocument document;
            if (document.setContent(reply->readAll())) {
                parseRss(document);
            }
        });
    }
}

void WelcomeWidget::parseRss(const QDomDocument &document)
{
    auto itemList = document.elementsByTagName("item");
    QRegularExpression dateRegex(QStringLiteral(R"((?<date>\d{2} \w{3} \d{4}))"));

    for (int i = 0; i < itemList.count(); ++i) {
        auto node = itemList.at(i);
        if (node.isElement()) {
            // Sample of date format Wed, 24 May 2017 13:46:07 +0000
            QString pDate = node.firstChildElement("pubDate").toElement().text();
            QRegularExpressionMatch match = dateRegex.match(pDate);
            if (match.hasMatch()) {
                Post p;
                p.title = node.firstChildElement("title").toElement().text();
                p.url = node.firstChildElement("link").toElement().text();
                p.date = match.captured("date");
                m_postList.append(p);
            }
        }
    }
    setupRssFeed();
}

void WelcomeWidget::setupRssFeed()
{
    if (m_postList.empty()) {
        fallback();
        return;
    }
    QLocale locale(QLocale::English);
    std::sort(m_postList.begin(), m_postList.end(), [locale](const Post &p1, const Post &p2) { return locale.toDate(p1.date, "dd MMM yyyy") > locale.toDate(p2.date, "dd MMM yyyy"); });
    auto layout = new QVBoxLayout;
    int count = m_postList.count() > POSTS_LIMIT ? POSTS_LIMIT : m_postList.count();
    for (int i = 0; i < count; ++i) {
        Post item = m_postList.at(i);
        QString url = QString("<a href=\"%1\">%2</a>").arg(item.url, QString(item.title + " - " + item.date));
        auto lb = new QLabel(url);
        lb->setOpenExternalLinks(true);
        layout->addWidget(lb);
    }
    setNewsLayout(layout);
}

void WelcomeWidget::fallback()
{
    auto theme = palette().text().color().value() >= QColor(Qt::lightGray).value() ? QStringLiteral("dark") : QStringLiteral("light");
    auto layout = new QHBoxLayout;
    layout->addWidget(new QLabel(i18n("Failed to fetch the News.")));
    auto button = new QToolButton;
    button->setIcon(QIcon::fromTheme("view-refresh", QIcon(QString(":/%1/refresh").arg(theme))));
    button->setIconSize(QSize(fontMetrics().lineSpacing(), fontMetrics().lineSpacing()));
    layout->addWidget(button);
    layout->addStretch();
    connect(button, &QToolButton::clicked, this, &WelcomeWidget::retrieveRssFeed);
    setNewsLayout(layout);
}

void WelcomeWidget::setNewsLayout(QLayout *newLayout)
{
    if (m_newsFeedWidget->layout()) {
        qDeleteAll(m_newsFeedWidget->children());
    }
    m_newsFeedWidget->setLayout(newLayout);
}
