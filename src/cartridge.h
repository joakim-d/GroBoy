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
    uint8_t *ram();
    uint8_t *rom();
    uint8_t *getBuf(int address);

    void set_rom(uint8_t *rom);
    void create_ram(int size);

    virtual void write(int address, uint8_t data) = 0;
    virtual uint8_t read(int address);
    void open(const std::string &path);

    virtual void update(int cycles);
protected:
    uint8_t *rom_;
    uint8_t *ram_;
    bool enable_ram_{false};
    int rom_selector_ {1};
    int ram_selector_ {0};
};

#endif
