#include "mbc2.h"

void MBC2::write(int address, uint8_t data){
    switch(address){
    case 0x0000 ... 0x1FFF:
        if(address & 0x100) enable_ram_ = true;
        else enable_ram_ = false;
        break;
    case 0x2000 ... 0x3FFF:
        rom_selector_ = data & 0x0F;
        if(rom_selector_ == 0) rom_selector_++;
    default:
        ram_[address - 0xA000] = data;
    }
}

