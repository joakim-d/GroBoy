#ifndef CPU_H
#define CPU_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "def.h"
#include "memory.h"

class Cpu {
public:
    Cpu();
    ~Cpu();
    int run();
    void set_memory(Memory *memory);
    void make_request(int type);
    /*
    BYTE is_halted();
    int save_cpu(FILE* file);
    void restore_cpu(FILE* file);*/
private:
    struct z80_t {
        unsigned short PC;
        unsigned short SP;
        unsigned char A;
        unsigned char F;
        unsigned char B;
        unsigned char C;
        unsigned char D;
        unsigned char E;
        unsigned char H;
        unsigned char L;
    };
    z80_t z80_;
    Memory *memory_;
    int cycles_;
    int ime_counter_;
    BYTE halted_;
    BYTE skip_;
    bool IME_;

    //Interrupt handling
    void interrupts_init();
    void set_IME(bool);
    void handle_interrupts(Cpu::z80_t *z80);
    void execute_interrupt(BYTE type, Cpu::z80_t *z80);

    void reset_halt();

    //Misc/control instructions
    void di();
    void ei();
    void halt();
    void stop();
    //Jumps/calls
    void call_n_cond(BYTE cond);
    void call_cond(BYTE cond);
    void call();
    void jp_n_cond(BYTE cond);
    void jp(unsigned short addr);
    void jp_cond(BYTE cond);
    void jr(BYTE_S d);
    void jr_cond(BYTE cond, BYTE_S d);
    void jr_n_cond(BYTE cond, BYTE_S d);
    void ret();
    void ret_cond(BYTE cond);
    void ret_n_cond(BYTE cond);
    void reti();
    void rst(BYTE addr);
    //8bit load/store/move instructions
    void ld_reg(BYTE *reg, BYTE data);
    void ld_mem(unsigned short addr, BYTE data);
    //16bit load/store/move instructions
    void ld_a16_sp();
    void ld_at(unsigned short addr);
    void ld_from_a8(unsigned short addr);
    void ld_hl_sp_p_r8();
    void ld_sp(unsigned short data);
    void pop(BYTE *reg1, BYTE *reg2);
    void push(BYTE reg1, BYTE reg2);

    //8bit arithmetic/logical instructions
    void adc(BYTE data);
    void add(BYTE data);
    void and_(BYTE data);
    void ccf();
    void cp(BYTE data);
    void cpl();
    void daa();
    void dec_at(unsigned short addr);
    void dec_smpl(BYTE *reg1);
    void dec_sp();
    void inc_at(unsigned short addr);
    void inc_smpl(BYTE *reg1);
    void inc_sp();
    void sbc(BYTE data);
    void scf();
    void sub(BYTE data);
    void or_(BYTE data);
    void xor_(BYTE data);

    //16 bit arithmetic/logical instructions
    void add_dbl(BYTE *reg1, BYTE *reg2, unsigned short data);
    void add_sp_r8(BYTE_S data);
    void dec_dbl(BYTE *reg1, BYTE *reg2);
    void inc_dbl(BYTE *reg1, BYTE *reg2);

    //8bit rotations/shifts and bit instructions
    void bit(BYTE bit, BYTE data);
    void res(BYTE b, BYTE *a);
    void res_hl(BYTE b);
    void rl(BYTE *data);
    void rl_hl();
    void rla();
    void rlc(BYTE *data);
    void rlc_hl();
    void rlca();
    void rr(BYTE *data);
    void rr_hl();
    void rra();
    void rrc(BYTE *data);
    void rrc_hl();
    void rrca();
    void set(BYTE b, BYTE *a);
    void set_hl(BYTE b);
    void srl(BYTE *data);
    void srl_hl();
    void sla(BYTE *data);
    void sla_hl();
    void sra(BYTE *data);
    void sra_hl();
    void swap(BYTE *data);
    void swap_hl();
};






#endif

