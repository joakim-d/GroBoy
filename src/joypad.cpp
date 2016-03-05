#include "joypad.h"
#include <iostream>

Joypad::Joypad(){}

void Joypad::update(int cycles){
    joypad_counter_ += cycles;

    if(joypad_counter_ > 80000){
        joypad_counter_ -= 80000;
        key_buttons_ = ~keystate_;
    }

    uint8_t joy_cur = memory_->read(0xFF00);
    joy_new_ = 0x0F;

    if(0x10 & joy_cur) {
        joy_new_ &= ((key_buttons_ & 0xF0)>>4);
    }
    if(0x20 & joy_cur) {
        joy_new_ &= (key_buttons_ & 0x0F);
    }

    memory_->write(0xFF00, (joy_cur & 0xF0) | joy_new_);
    if(joy_new_ != joy_old_){
        joy_old_ = joy_new_;
        request_callback_(JOYPAD);
    }
}

void Joypad::set_memory(Memory *memory){
    memory_ = memory;
}

void Joypad::set_request_callback(const std::tr1::function<void (int)> &callback){
    request_callback_ = callback;
}

void Joypad::updateInput(uint8_t input){
    keystate_ = input;
}

void Joypad::reset(){
    joypad_counter_ = 0;
    keystate_ = 0;
    joy_new_ = 0;
    joy_old_ = 0;
    key_buttons_ = 0xFF;
}
