#ifndef _GAMEBOY_H_
#define _GAMEBOY_H_

#include "cpu.h"
#include "memory.h"
#include "gpu.h"
#include "timer.h"

#include <string>

class Gameboy{
public:
    Gameboy();
    void setGame(const std::string &path);
    void play();
    virtual void update_screen(const unsigned char *buffer, unsigned int width, unsigned int height) = 0;
    void handle_ready();
private:
    Cpu cpu_;
    Gpu gpu_;
    Timer timer_;
    Memory memory_;
    std::string game_;
};

#endif
