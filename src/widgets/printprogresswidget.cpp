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

#include "printprogresswidget.h"
#include <QVBoxLayout>
#include <QLabel>
#include <KLocalizedString>

PrintProgressWidget::PrintProgressWidget(QWidget *parent) :
    QWidget(parent),
    progressbar(new QProgressBar(this))
{
    QVBoxLayout *layout = new QVBoxLayout();
    progressbar->setValue(0);
    layout->addWidget(progressbar);
    this->setLayout(layout);
}

void PrintProgressWidget::updateProgressBar(const float value)
{
    progressbar->setValue(value);
}
