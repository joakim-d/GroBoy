#ifndef _MBC1_H_
#define _MBC1_H_

#include "cartridge.h"

class MBC1 : public Cartridge{
public:
    MBC1();

    virtual void write(int address, BYTE data);

private:
    bool rom_mode_ {true};
};

#endif
