#ifndef _MBC1_H_
#define _MBC1_H_

#include "cartridge.h"

class MBC1 : public Cartridge{
public:
    MBC1();

    //virtual BYTE read_rom(int address) const;
    virtual void write(int address, BYTE data);
    virtual BYTE read(int address);

private:
    bool rom_mode_;
    bool enable_ram_;
    int rom_selector_;
    int ram_selector_;

};

#endif
