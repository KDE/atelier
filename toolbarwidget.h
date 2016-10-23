#pragma once
#include <QWidget>
#include <QToolButton>
class ToolBarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ToolBarWidget(QWidget *parent = nullptr);

private:
    QToolButton *settingsTB;
    QToolButton *openFileTB;
    QToolButton *connectTB;
    QToolButton *startTB;
    QToolButton *pauseTB;
    QToolButton *stopTB;
    void initButtons();
};
