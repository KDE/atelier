#pragma once

#include <QWidget>

namespace Ui {
class GCodeEditor;
}

class GCodeEditor : public QWidget
{
    Q_OBJECT

public:
    explicit GCodeEditor(QWidget *parent = nullptr);
    ~GCodeEditor();

private:
    Ui::GCodeEditor *ui;
};
