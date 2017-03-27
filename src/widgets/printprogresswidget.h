#pragma once
#include <QWidget>
#include <QProgressBar>

class PrintProgressWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PrintProgressWidget(QWidget *parent = nullptr);
    void updateProgressBar(const float value);

private:
    QProgressBar *progressbar;

};
