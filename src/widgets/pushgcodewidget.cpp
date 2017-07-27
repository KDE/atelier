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

#include "pushgcodewidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <KLocalizedString>

PushGCodeWidget::PushGCodeWidget(QWidget *parent) : QWidget(parent),
    input(new QLineEdit(this))
{
    QVBoxLayout *layout = new QVBoxLayout();
    QHBoxLayout *items = new QHBoxLayout();
    QLabel *lb = new QLabel(i18n("Push Gcode"));
    lb->setAlignment(Qt::AlignHCenter);
    QPushButton *bt = new QPushButton(i18n("Send"));
    items->setSpacing(3);
    layout->addWidget(lb);
    items->addWidget(input);
    items->addWidget(bt);
    layout->addItem(items);
    this->setLayout(layout);

    connect(bt, &QPushButton::clicked, [ = ] {
        push(input->text());
    });
}

