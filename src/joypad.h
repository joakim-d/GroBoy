#ifndef JOYPAD_H
#define JOYPAD_H

#include "memory.h"
#include "interrupts.h"
#include <tr1/functional>

class Joypad{
public:
    enum Input{
        RIGHT   = 0x01,
        LEFT    = 0x02,
        UP      = 0x04,
        DOWN    = 0x08,
        A       = 0x10,
        B       = 0x20,
        SELECT  = 0x40,
        START   = 0x80
    };
    Joypad();
    void update(int cycles);
    void reset();
    void updateInput(uint8_t input);
    void set_request_callback(std::tr1::function<void(int)> const &callback);
    void set_memory(Memory *memory);

private:
    Memory *memory_;
    std::tr1::function<void(int)> request_callback_;
    int joypad_counter_ {0};
    uint8_t keystate_ {0};
    uint8_t joy_new_ {0};
    uint8_t joy_old_ {0};
    uint8_t key_buttons_{0xFF};
};

#endif
