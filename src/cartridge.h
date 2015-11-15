#ifndef _CARTRIDGE_H_
#define _CARTRIDGE_H_

#include "def.h"
#include <string>

class Cartridge{
public:
    Cartridge();
    virtual ~Cartridge();
    int type() const;
    bool haveRam() const;
    BYTE *ram();
    BYTE *rom();

    void set_rom(BYTE *rom);
    void create_ram(int size);

    virtual void write(int address, BYTE data) = 0;
    virtual BYTE read(int address);
    void open(const std::string &path);

    virtual void update(int cycles);
protected:
    BYTE *rom_;
    BYTE *ram_;
    bool enable_ram_{false};
    int rom_selector_ {1};
    int ram_selector_ {0};
};

#endif
