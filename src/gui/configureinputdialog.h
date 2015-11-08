#ifndef CONFIGUREINPUTDIALOG_H
#define CONFIGUREINPUTDIALOG_H

#include <QDialog>
#include "input.h"

namespace Ui {
class ConfigureInputDialog;
}

enum Button{
    BUTTON_UP = 0,
    BUTTON_DOWN,
    BUTTON_LEFT,
    BUTTON_RIGHT,
    BUTTON_SELECT,
    BUTTON_START,
    BUTTON_B,
    BUTTON_A,

    BUTTON_COUNT
};

class QLabel;

class ConfigureInputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigureInputDialog(QWidget *parent = 0);
    ~ConfigureInputDialog();

    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    const Input& input() const;

private slots:
    void handleConfigureInput();
private:
    void initTouchLabels();
    void printTouchLabels();

    Ui::ConfigureInputDialog *ui;
    bool configuring_;
    int current_touch_index_;
    Input input_;
    QVector<QLabel *> touch_labels_;
};

#endif // CONFIGUREINPUTDIALOG_H
