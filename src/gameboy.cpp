#include "gameboy.h"
#include <tr1/functional>
#include <chrono>

Gameboy::Gameboy(){}

Gameboy::~Gameboy(){
    gpu_.set_ready_callback(NULL);
    gpu_.set_request_callback(NULL);
    timer_.set_request_callback(NULL);
    joypad_.set_request_callback(NULL);
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
    joypad_.set_memory(&memory_);

    gpu_.set_ready_callback(std::tr1::bind(&Gameboy::handle_ready, this));
    gpu_.set_request_callback(std::tr1::bind(&Cpu::make_request, &cpu_, std::tr1::placeholders::_1));
    timer_.set_request_callback(std::tr1::bind(&Cpu::make_request, &cpu_, std::tr1::placeholders::_1));
    joypad_.set_request_callback(std::tr1::bind(&Cpu::make_request, &cpu_, std::tr1::placeholders::_1));

    int cycles;
    int max_cycles = 69905 ;
    long long time_elapsed;
    for(;;){
        auto time_before = std::chrono::system_clock::now();
        cycles = cpu_.run();
        gpu_.update(cycles);
        timer_.update(cycles);
        joypad_.update(cycles);

        max_cycles -= cycles;
        if(max_cycles < 0){
            time_elapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - time_before).count();
            max_cycles += 69905;
            if(time_elapsed < 16000){
                usleep(16000 - time_elapsed);
            }
        }
    }
}

void Gameboy::handle_ready(){
    uint8_t *buf = gpu_.get_buffer();
    updateScreen(buf, Gpu::BUFFER_WIDTH, Gpu::BUFFER_HEIGHT);
}

void Gameboy::updateInput(uint8_t input){
    joypad_.updateInput(input);
}
