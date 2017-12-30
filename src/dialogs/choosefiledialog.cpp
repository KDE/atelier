/* Atelier KDE Printer Host for 3D Printing
    Copyright (C) <2017>
    Author: Lays Rodrigues - laysrodriguessilva@gmail.com

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
#include "choosefiledialog.h"
#include <KLocalizedString>
#include <QDialogButtonBox>
#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>

ChooseFileDialog::ChooseFileDialog(QWidget *parent, QList<QUrl> files) :
    QDialog(parent)
{
    auto layout = new QVBoxLayout;
    auto label = new QLabel(i18n("Choose a file to print: "));
    auto listWigdet = new QListWidget();
    QStringList files_list;
    foreach(const auto &file, files){
        files_list.append(file.toLocalFile());
    }
    listWigdet->addItems(files_list);
    connect(listWigdet, &QListWidget::currentTextChanged, [ & ](const QString& t){
        m_choosen_file = t;
    });
    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &ChooseFileDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ChooseFileDialog::reject);
    layout->addWidget(label);
    layout->addWidget(listWigdet);
    layout->addWidget(buttonBox);
    setLayout(layout);
}

ChooseFileDialog::~ChooseFileDialog()
{
}

const QString& ChooseFileDialog::choosenFile()
{
    return m_choosen_file;
}
