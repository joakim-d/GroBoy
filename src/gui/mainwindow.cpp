#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QThread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->open_rom, &QAction::triggered, this, &MainWindow::handle_open_rom);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handle_open_rom(){
    QString rom = QFileDialog::getOpenFileName(this,
        tr("Open Rom"), "", tr("Image Files (*.rom *.gb)"));
    gb_.setGame(rom.toStdString());
    gb_.start();
}
