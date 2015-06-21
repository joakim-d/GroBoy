#include "gameboy.h"
#include <tr1/functional>
Gameboy::Gameboy(){

}

void Gameboy::setGame(const std::string &path){
    game_ = path;
}

void Gameboy::play(){
    if(game_ == ""){
        return;
    }
    memory_.load_cartridge(game_);

    cpu_.set_memory(&memory_);
    gpu_.set_memory(&memory_);
    timer_.set_memory(&memory_);

    gpu_.set_ready_callback(std::tr1::bind(&Gameboy::handle_ready, this));
    gpu_.set_request_callback(std::tr1::bind(&Cpu::make_request, &cpu_, std::tr1::placeholders::_1));
    timer_.set_request_callback(std::tr1::bind(&Cpu::make_request, &cpu_, std::tr1::placeholders::_1));

    int cycles;
    int max_cycles = 69905 ;
    for(;;){
        cycles = cpu_.run();
        gpu_.update(cycles);
        timer_.update(cycles);
        max_cycles -= cycles;
        if(max_cycles < 0){
            max_cycles += 69905;
            usleep(16000);
        }
    }
}

void Gameboy::handle_ready(){
    uint8_t *buf = gpu_.get_buffer();
    update_screen(buf, Gpu::BUFFER_WIDTH, Gpu::BUFFER_HEIGHT);
}
