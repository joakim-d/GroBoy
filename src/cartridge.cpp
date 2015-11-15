#include "cartridge.h"
#include <assert.h>

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

BYTE Cartridge::read(int addr){
    switch(addr){
    case 0 ... 0x3FFF:
        return rom_[addr];
    case 0x4000 ... 0x7FFF:
        return rom_[0x4000*(rom_selector_ - 1) + addr];
    case 0xA000 ... 0xBFFF:  
        return haveRam() ? ram_[(0x2000*ram_selector_) + addr - 0xA000] : 0;
    default:
        assert(false);
        return 0;
    }
}
