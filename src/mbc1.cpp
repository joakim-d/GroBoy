#include "mbc1.h"

MBC1::MBC1() : Cartridge(), enable_ram_(false), rom_selector_(1), ram_selector_(0){
}

void MBC1::write(int address, BYTE data){
    switch(address){
    case 0 ... 0x1FFF:
        if((data & 0x0A) == 0x0A) enable_ram_ = true;
        else enable_ram_ = false;
        break;
    case 0x2000 ... 0x3FFF:
        rom_selector_ = data & 0x1F;
        if(rom_selector_ == 0 || rom_selector_ == 0x20 || rom_selector_ == 0x40 || rom_selector_ == 0x60) {
            rom_selector_++;
        }
        break;
    case 0x4000 ... 0x5FFF:
        ram_selector_ = (data & 0x60) >> 5;
        break;
    case 0x6000 ... 0x7FFF:
        if(data == 0) rom_mode_ = 1; //rom mode enabled
        else rom_mode_ = 0; // ram mode enabled
        break;
    default:
        if(haveRam()){
            ram_[(0x2000*ram_selector_) + address - 0xA000] = data;
        }
    }
}

BYTE MBC1::read(int addr){
    switch(addr){
    case 0 ... 0x3FFF:
        return *(rom_ + addr);
    case 0x4000 ... 0x7FFF:
        return *(rom_ + (0x4000*(rom_selector_ - 1) + addr));
    case 0x8000 ... 0xBFFF:
        return *(ram_ + ((0x2000*ram_selector_) + addr - 0xA000));
    default:
        exit(EXIT_FAILURE);
    }
}
