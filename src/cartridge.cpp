#include "cartridge.h"
#include <assert.h>

Cartridge::Cartridge() : rom_(0), ram_(0){}

Cartridge::~Cartridge(){
    if(ram_){
        delete ram_;
    }
}

uint8_t *Cartridge::ram(){
    return ram_;
}

uint8_t *Cartridge::rom(){
    return rom_;
}

void Cartridge::set_rom(uint8_t *rom){
    rom_ = rom;
}

void Cartridge::create_ram(int size){
    ram_ = new uint8_t[size];
}

bool Cartridge::haveRam() const{
    return ram_ != 0;
}

uint8_t Cartridge::read(int addr){
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

void Cartridge::update(int){}

uint8_t* Cartridge::getBuf(int addr){
  if(addr <= 0x3FFF){
    return rom_ + addr;
  }
  else if(addr <= 0x7FFF){
    return rom_ + 0x4000*(rom_selector_ - 1) + addr;
  }
  else if(addr >= 0xA000 && addr <= 0xBFFF){
    if(haveRam()) return ram_ + (0x2000*ram_selector_) + addr - 0xA000;
    else return nullptr;
  }
  return nullptr;
}

