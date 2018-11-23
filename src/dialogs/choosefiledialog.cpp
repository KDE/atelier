/* Atelier KDE Printer Host for 3D Printing
    Copyright (C) <2017>
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
#include <QDialogButtonBox>
#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>
#include "choosefiledialog.h"

ChooseFileDialog::ChooseFileDialog(QWidget *parent, QList<QUrl> files) :
    QDialog(parent)
{
    const int padding = 30;
    auto listWidget = new QListWidget(this);
    listWidget->setMinimumWidth(fontMetrics().height() / 2  * padding);

    foreach (const auto &url, files) {
        listWidget->addItem(url.toLocalFile());
    }
    listWidget->setCurrentRow(0);
    connect(listWidget, &QListWidget::currentRowChanged, this, [this, &files](const int t) {
        m_choosen_file = files.at(t);
    });

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &ChooseFileDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ChooseFileDialog::reject);

    auto layout = new QVBoxLayout;
    auto label = new QLabel(i18n("Choose a file to print:"), this);
    layout->addWidget(label);
    layout->addWidget(listWidget);
    layout->addWidget(buttonBox);
    setLayout(layout);
}

const QUrl ChooseFileDialog::choosenFile()
{
    return m_choosen_file;
}
