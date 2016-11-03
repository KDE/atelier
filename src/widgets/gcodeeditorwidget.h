#pragma once

#include <QWidget>

namespace Ui {
class GCodeEditorWidget;
}

class GCodeEditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GCodeEditorWidget(QWidget *parent = nullptr);
    ~GCodeEditorWidget();
    void loadFile(const QString &fileName);

private:
    Ui::GCodeEditorWidget *ui;
};
