#ifndef _GAMEBOY_GUI_H_
#define _GAMEBOY_GUI_H_

#include "../gameboy.h"
#include <QLabel>
#include <QThread>

class GameboyGui : public Gameboy, public QThread{
public:
    GameboyGui();
    void run();
    void update_screen(const unsigned char *buffer, unsigned int width, unsigned int height);
private:
    QLabel label_;
};

#endif
