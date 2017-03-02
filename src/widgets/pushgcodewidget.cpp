#include "pushgcodewidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <KLocalizedString>

PushGCodeWidget::PushGCodeWidget(QWidget *parent) : QWidget(parent),
    input(new QLineEdit(this))
{
    QHBoxLayout *layout = new QHBoxLayout();
    QLabel *lb = new QLabel(i18n("Push Gcode"));
    QPushButton *bt = new QPushButton(i18n("Send"));
    layout->addWidget(lb);
    layout->addWidget(input);
    layout->addWidget(bt);
    this->setLayout(layout);

    connect(bt, &QPushButton::clicked, [=]{
        push(input->text());
    });
}

