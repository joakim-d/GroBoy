#ifndef _MBC3_H_
#define _MBC3_H_

#include "cartridge.h"

class MBC3 : public Cartridge{
public:
    struct RtcRegisters {
        uint8_t s {};   //Seconds
        uint8_t m {};   //Minutes
        uint8_t h {};   //Hours
        uint8_t dl {};  //Lower 8 bits of Day Counter
        uint8_t dh {};  //Upper 1 bit of Day Counter, Carry Bit, Halt Flag
                        //Bit 0  Most significant bit of Day Counter (Bit 8)
                        //Bit 6  Halt (0=Active, 1=Stop Timer)
                        //Bit 7  Day Counter Carry Bit (1=Counter Overflow)
    };
    virtual void write(int address, uint8_t data);
    virtual uint8_t read(int address);
private:
    void latchTimer();

    RtcRegisters rtc_regs_;
    uint16_t address_map_ {};
    uint8_t* mapped_register_ {};
    bool latch_{false};
    bool half_latch_{false};

};

#endif //_MBC3_H_
