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
    QLabel *lb = new QLabel(i18n("Print Progress"));
    layout->addWidget(lb);
    layout->addWidget(progressbar);
    this->setLayout(layout);
}

void PrintProgressWidget::updateProgressBar(const float value)
{
    progressbar->setValue(value);
}
