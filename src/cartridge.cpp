#include "cartridge.h"

Cartridge::Cartridge() : rom_(0), ram_(0){}

Cartridge::~Cartridge(){
    if(ram_){
        delete ram_;
    }
}

BYTE *Cartridge::ram(){
    return ram_;
}

BYTE *Cartridge::rom(){
    return rom_;
}

void Cartridge::set_rom(uint8_t *rom){
    rom_ = rom;
}

void Cartridge::create_ram(int size){
    ram_ = new BYTE[size];
}

bool Cartridge::haveRam() const{
    return ram_ != 0;
}
