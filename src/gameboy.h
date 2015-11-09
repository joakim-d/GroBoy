#ifndef _GAMEBOY_H_
#define _GAMEBOY_H_

#include "cpu.h"
#include "memory.h"
#include "gpu.h"
#include "timer.h"
#include "joypad.h"

#include <string>

class Gameboy{
public:
    Gameboy();
    ~Gameboy();
    void setGame(const std::string &path);
    void play();
    void updateInput(BYTE touches);
    virtual void updateScreen(const unsigned char *buffer, unsigned int width, unsigned int height) = 0;

private:
    Cpu cpu_;
    Gpu gpu_;
    Timer timer_;
    Memory memory_;
    Joypad joypad_;
    std::string game_;

    void handle_ready();
};

#endif
