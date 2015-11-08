#include "gameboygui.h"

GameboyGui::GameboyGui(QObject *parent) : QThread(parent){
}

void GameboyGui::run(){
    play();
}

void GameboyGui::updateScreen(const unsigned char *buffer, unsigned int width, unsigned int height){
    QImage img = QImage(buffer, width, height, QImage::Format_Indexed8);
    QVector<QRgb> color_table;
    color_table
            << qRgb(156,189,15)
            << qRgb(140,173,15)
            << qRgb(48,98,48)
            << qRgb(15,56,15);
    img.setColorTable(color_table);
    emit frameReady(img);
}

void GameboyGui::handleNewInput(uint8_t input){
    Gameboy::updateInput(input);
}
