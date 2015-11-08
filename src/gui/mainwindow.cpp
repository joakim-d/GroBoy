#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QThread>
#include "gameboywidget.h"
#include "configureinputdialog.h"
#include "config.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->open_rom, &QAction::triggered, this, &MainWindow::handle_open_rom);
    QObject::connect(ui->configure_input, &QAction::triggered, this, &MainWindow::handle_configure_input);
    QObject::connect(&gb_, &GameboyGui::frameReady, ui->gameboywidget, &GameboyWidget::updateImage);
    QObject::connect(ui->gameboywidget, SIGNAL(newInput(uint8_t)), &gb_, SLOT(handleNewInput(uint8_t)));
    ui->gameboywidget->setFocus();
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

void MainWindow::handle_configure_input(){
    ConfigureInputDialog input_dialog;
    input_dialog.exec();
    if(input_dialog.result() == QDialog::Accepted){
        Config config;
        config.saveInput(input_dialog.input());
        ui->gameboywidget->setInput(input_dialog.input());
    }
}
