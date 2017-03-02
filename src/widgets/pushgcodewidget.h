#pragma once
#include <QWidget>
#include <QLineEdit>

class PushGCodeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PushGCodeWidget(QWidget *parent = nullptr);

private:
    QLineEdit *input;

signals:
    void push(QString command);
};
