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
#include <atcore_version.h>
#include <KAboutData>
#include <KLocalizedString>
#include <QApplication>
#include "config.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("KDE");

    QCoreApplication::setOrganizationDomain("kde.org");

    QCoreApplication::setApplicationName("atelier");

    KAboutData aboutData(
        // The program name used internally. (componentName)
        QStringLiteral("atelier"),
        // A displayable program name string. (displayName)
        i18n("Atelier"),
        // The program version string. (version)
        QStringLiteral(ATELIER_VERSION),
        // Short description of what the app does. (shortDescription)
        i18n("Printer Host for 3DPrinters"),
        // The license this code is released under
        KAboutLicense::GPL_V3,
        // Copyright Statement (copyrightStatement = QString())
        i18n("(c) 2016"),
        // Optional text shown in the About box.
        // Can contain any information desired. (otherText)
        QString(),
        // The program homepage string. (homePageAddress = QString())
        QStringLiteral("http://atelier.kde.org"),
        // The bug report email address
        QStringLiteral("atelier@bugs.kde.org"));
    aboutData.addAuthor(i18n("Lays Rodrigues"), i18n("Developer"), QStringLiteral("lays.rodrigues@kde.org"),
                        QStringLiteral("http://laysrodriguesdev.wordpress.com"));
    aboutData.addAuthor(i18n("Chris Rizzitello"), i18n("Developer"), QStringLiteral("rizzitello@kde.org"),
                        QStringLiteral("http://rizzitello.wordpress.com"));
    aboutData.addAuthor(i18n("Patrick Pereira"), i18n("Developer"), QStringLiteral("patrickjp@kde.org"),
                        QStringLiteral("http://patrickjp.com"));
    aboutData.addAuthor(i18n("Tomaz Canabrava"), i18n("Contributor"), QStringLiteral("tcanabrava@kde.org"),
                        QStringLiteral("http://angrycane.com.br"));
    aboutData.setOtherText(i18n("Using AtCore:%1", QString(ATCORE_VERSION_STRING)));
    KAboutData::setApplicationData(aboutData);

    MainWindow *m = new MainWindow();
    m->setWindowIcon(QIcon(":/icon/atelier"));
    m->show();
    return app.exec();
}
