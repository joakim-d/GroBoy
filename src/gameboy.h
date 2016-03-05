#ifndef _GAMEBOY_H_
#define _GAMEBOY_H_

#include "cpu.h"
#include "memory.h"
#include "gpu.h"
#include "timer.h"
#include "joypad.h"

#include <string>
#include <mutex>

class Gameboy{
public:
    Gameboy();
    ~Gameboy();
    void setGame(const std::string &path);
    void play();
    void reset();
    void updateInput(uint8_t touches);
    virtual void updateScreen(const unsigned char *buffer, unsigned int width, unsigned int height) = 0;
    void stop();
    bool isRunning();
private:
    Cpu cpu_;
    Gpu gpu_;
    Timer timer_;
    Memory memory_;
    Joypad joypad_;
    std::string game_;
    bool running_ {false};
    std::mutex run_mutex_;

    void handle_ready();
    void update(int cycles);
};

#endif
