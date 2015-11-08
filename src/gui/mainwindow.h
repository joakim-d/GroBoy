#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gameboygui.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void handle_open_rom();
    void handle_configure_input();
private:
    Ui::MainWindow *ui;
    GameboyGui gb_;
};

#endif // MAINWINDOW_H
