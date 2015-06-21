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
    void update(BYTE cycles);

private:
    unsigned int div_timer_;
    unsigned int tac_timer_;
    Memory *memory_;

    std::tr1::function<void (int)> request_callback_;
};

#endif
