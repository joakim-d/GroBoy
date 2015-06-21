#ifndef _CARTRIDGE_H_
#define _CARTRIDGE_H_

#include "def.h"
#include <string>

class Cartridge{
public:
    Cartridge();
    ~Cartridge();
    int type() const;
    bool haveRam() const;
    BYTE *ram();
    BYTE *rom();

    void set_rom(BYTE *rom);
    void create_ram(int size);

    virtual void write(int address, BYTE data) = 0;
    virtual BYTE read(int address) = 0;
    void open(const std::string &path);
protected:
    BYTE *rom_;
    BYTE *ram_;
};

#endif
