#include "timer.h"
#include "interrupts.h"

#define CLOCK_SPEED 4194304
#define DIVIDER_TIMER_SPEED 256

static const int tac_speed[] = {1024, 16, 64, 256};

Timer::Timer(){}

void Timer::update(uint8_t cycles){
    uint8_t timer_control;
	uint8_t timer_counter;
    div_timer_ += cycles;
    if(div_timer_ / DIVIDER_TIMER_SPEED >= 1){
        memory_->set_force_write(true);
        memory_->write(0xFF04, memory_->read(0xFF04) + 1);
        memory_->set_force_write(false);
        div_timer_ -= DIVIDER_TIMER_SPEED;
	}
    timer_control = memory_->read(0xFF07);
	if(timer_control & 0x04){
        tac_timer_ += cycles;
        while(tac_timer_ >= tac_speed[timer_control & 0x03]){
            tac_timer_ -= tac_speed[timer_control & 0x03];
            timer_counter = memory_->read(0xFF05);
            timer_counter++;
            if(timer_counter == 0){
              request_callback_(TIMER);
              timer_counter = memory_->read(0xFF06);
            }
            memory_->write(0xFF05, timer_counter);
		}
	}
}

void Timer::set_request_callback(const std::tr1::function<void (int)> &callback){
    request_callback_ = callback;
}

void Timer::set_memory(Memory *memory){
    memory_ = memory;
}

void Timer::reset(){
    div_timer_ = 0;
    tac_timer_ = 0;
}
