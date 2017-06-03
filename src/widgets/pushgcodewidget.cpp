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

