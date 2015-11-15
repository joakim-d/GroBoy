#include "mbc3.h"
#include <iostream>
#include <assert.h>

void MBC3::write(int address, BYTE data){
    switch(address){
    case 0 ... 0x1FFF:
        if((data & 0x0A) == 0x0A) enable_ram_ = true;
        else enable_ram_ = false;
        break;
    case 0x2000 ... 0x3FFF:
        rom_selector_ = data & 0x7F;
        break;
    case 0x4000 ... 0x5FFF:
        switch(data){
        case 0x0 ... 0x3 :
            ram_selector_ = data;
            break;
        case 0x8:
            address_map_ = (0xA000 + (address - 0x4000));
            mapped_register_ = &rtc_regs_.s;
            break;
        case 0x9:
            address_map_ = (0xA000 + (address - 0x4000));
            mapped_register_ = &rtc_regs_.m;
            break;
        case 0xA:
            address_map_ = (0xA000 + (address - 0x4000));
            mapped_register_ = &rtc_regs_.h;
            break;
        case 0xB:
            address_map_ = (0xA000 + (address - 0x4000));
            mapped_register_ = &rtc_regs_.dl;
            break;
        case 0xC:
            address_map_ = (0xA000 + (address - 0x4000));
            mapped_register_ = &rtc_regs_.dh;
            break;
        default:
            break;
        }

        break;
    case 0x6000 ... 0x7FFF:
        if(data == 0 && !half_latch_){
            half_latch_ = true;
        }
        else {
            if(data == 1 && half_latch_){
                latchTimer();
            }
            half_latch_ = false;
        }
        break;
    case 0xA000 ... 0xBFFF:
        if(address == address_map_){
            assert(mapped_register_ != nullptr);
            *mapped_register_ = data;
        }
        else if(haveRam()){
            ram_[(0x2000*ram_selector_) + address - 0xA000] = data;
        }
        break;
    default:
        std::cout << "MBC3::write Unexpected behavior writing at address " << address << std::endl;
        break;
    }
}

uint8_t MBC3::read(int address){
    if(address == address_map_){
        assert(mapped_register_ != nullptr);
        return *mapped_register_;
    }
    else{
        return Cartridge::read(address);
    }
}

void MBC3::latchTimer(){
    latch_ = !latch_;
}

void MBC3::update(int cycles){
    if(!latch_){
        cycles_ += cycles;
        while(cycles_ >= 128 ){
            cycles_ -= 128;
            rtc_regs_.s++;
            if(rtc_regs_.s == 60){
                rtc_regs_.s = 0;
                rtc_regs_.m++;
                if(rtc_regs_.m == 60){
                    rtc_regs_.m = 0;
                    rtc_regs_.h++;
                    if(rtc_regs_.h == 24){
                        rtc_regs_.h = 0;
                        rtc_regs_.dl++;
                        if(rtc_regs_.dl == 0){
                            rtc_regs_.dh |= 1;
                        }
                    }
                }
            }
        }
    }
}
