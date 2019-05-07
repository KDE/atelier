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

#include <KLocalizedString>
#include <QDialogButtonBox>
#include <ProfileManager>

#include "profilesdialog.h"

ProfilesDialog::ProfilesDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(i18n("Profiles"));
    ProfileManager *pm = new ProfileManager(this);
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::close);
    QLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(pm);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
}
