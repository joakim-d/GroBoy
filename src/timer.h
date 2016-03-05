#ifndef TIMER_H
#define TIMER_H
#include "def.h"
#include "cpu.h"
#include "memory.h"
#include <tr1/functional>
class Timer{
public:
    Timer();
    void set_memory(Memory *memory);
    void set_request_callback(std::tr1::function<void (int)> const &callback);
    void update(uint8_t cycles);
    void reset();

private:
    int div_timer_ {0};
    int tac_timer_ {0};
    Memory *memory_ {};

    std::tr1::function<void (int)> request_callback_;
};

#endif
