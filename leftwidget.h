#ifndef LEFTWIDGET_H
#define LEFTWIDGET_H
#include <QWidget>
#include <QPushButton>

class LeftWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LeftWidget(QWidget *parent = 0);

private:
    QPushButton *connectSettingsPB;
    QPushButton *gcodeEditorPB;
    QPushButton *maintanceMenuPB;
    void initButtons();
};

#endif // LEFTWIDGET_H
