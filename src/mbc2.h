#ifndef _MBC2_H_
#define _MBC2_H_

#include "cartridge.h"

class MBC2 : public Cartridge{
public:
    virtual void write(int address, uint8_t data);

};

#endif //_MBC2_H_
