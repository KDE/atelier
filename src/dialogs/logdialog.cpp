/* Atelier KDE Printer Host for 3D Printing
    Copyright (C) <2017>
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
#include "logdialog.h"
#include <QVBoxLayout>

#include <QTime>
#include <QFileDialog>
#include <QTextStream>
#include <KLocalizedString>

LogDialog::LogDialog(QWidget *parent) : QDialog(parent),
    logFile(new QTemporaryFile(QDir::tempPath() + QStringLiteral("/Atelier_")))
{
    this->setWindowTitle(i18n("Log Dialog"));
    QVBoxLayout *layout = new QVBoxLayout();
    buttonGroup = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Close);
    log = new QPlainTextEdit();
    log->setReadOnly(true);
    log->setMaximumBlockCount(1000);

    layout->addWidget(log);
    layout->addWidget(buttonGroup);
    this->setLayout(layout);

    connect(buttonGroup, &QDialogButtonBox::rejected, this, &LogDialog::reject);
    connect(buttonGroup, &QDialogButtonBox::accepted, this, &LogDialog::saveLog);

}

void LogDialog::saveLog()
{
    QString fileName = QDir::homePath() + QChar::fromLatin1('/') + QFileInfo(logFile->fileName()).fileName();
    QString saveFileName = QFileDialog::getSaveFileName(this, i18n("Save Log to file"), fileName);
    QFile::copy(logFile->fileName(), saveFileName);
    logFile->close();
}

QString LogDialog::logHeader()
{
    return QStringLiteral("[%1]  ").arg(getTime());
}

QString LogDialog::rLogHeader()
{
    return QStringLiteral("[%1]< ").arg(getTime());
}

QString LogDialog::sLogHeader()
{
    return QStringLiteral("[%1]> ").arg(getTime());
}

void LogDialog::writeTempFile(QString text)
{
    /*
    A QTemporaryFile will always be opened in QIODevice::ReadWrite mode,
    this allows easy access to the data in the file. This function will
    return true upon success and will set the fileName() to the unique
    filename used.
    */
    logFile->open();
    logFile->seek(logFile->size());
    logFile->write(text.toLatin1());
    logFile->putChar('\n');
    logFile->close();
}

void LogDialog::addLog(QString msg)
{
    QString message(logHeader() + msg);
    log->appendPlainText(message);
    writeTempFile(message);
}

void LogDialog::addRLog(QString msg)
{
    QString message(rLogHeader() + msg);
    log->appendPlainText(message);
    writeTempFile(message);
}

void LogDialog::addSLog(QString msg)
{
    QString message(sLogHeader() + msg);
    log->appendPlainText(message);
    writeTempFile(message);
}

QString LogDialog::getTime()
{
    return QTime::currentTime().toString(QStringLiteral("hh:mm:ss:zzz"));
}
