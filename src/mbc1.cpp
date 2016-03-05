#include "mbc1.h"
#include <iostream>
#include <assert.h>

MBC1::MBC1() : Cartridge(){}

void MBC1::write(int address, uint8_t data){
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
        if(data == 0) {
            rom_mode_ = true; //rom mode enabled
            if(ram_selector_ != 0){
                std::cout << "Ram selector = " << ram_selector_ << "while in rom mode" << std::endl;
                assert(false);
            }
        }
        else {
            rom_mode_ = false; // ram mode enabled
            if(rom_selector_ > 0x1F){
                std::cout << "Rom selector = " << rom_selector_ << "while in ram mode" << std::endl;
                assert(false);
            }
        }
        break;
    default:
        if(haveRam()){
            ram_[(0x2000*ram_selector_) + address - 0xA000] = data;
        }
    }
}

