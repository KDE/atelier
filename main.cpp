#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("KDE");

    QCoreApplication::setOrganizationDomain("kde.org");

    QCoreApplication::setApplicationName("atelier");

    MainWindow m;
    m.show();
    return app.exec();
}
