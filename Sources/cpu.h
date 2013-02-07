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
void run();
void init();
unsigned char *rom_buffer;
typedef struct {
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
} z80_t;
BYTE z80_cycles[0x100] = 
{
4, 12, 8, 8, 4, 4, 8, 4, 20, 8, 8, 8, 4, 4, 8, 4, 
4, 12, 8, 8, 4, 4, 8, 4, 12, 8, 8, 8, 4, 4, 8, 4,
8, 12, 8, 8, 4, 4, 8, 4, 8, 8, 8, 8, 4, 4, 8, 4,
8, 12, 8, 8, 12, 12, 12, 4, 8, 8, 8, 8, 4, 4, 8, 4,
4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
8, 8, 8, 8, 8, 8, 4, 8, 4, 4, 4, 4, 4, 4, 8, 4,
4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
4, 4, 4, 4, 4, 4, 8, 4, 4, 4, 4, 4, 4, 4, 8, 4,
8, 12, 12, 16, 12, 16, 8, 16, 8, 16, 12, 4, 12, 24, 8, 16,
8, 12, 12, 0, 12, 16, 8, 16, 8, 16, 12, 0, 12, 0, 8, 16,
12, 12, 8, 0, 0, 16, 8, 16, 16, 4, 16, 0, 0, 0, 8, 16,
12, 12, 8, 4, 0, 16, 8, 16, 12, 8, 16, 4, 0, 0, 8, 16
};
BYTE z80_cb_cycles[0x100] = 
{
8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
};
//Misc/control instructions
static inline void di();
static inline void ei();
static inline void halt();
static inline void stop();
//Jumps/calls
static inline void call_n_cond(BYTE cond);
static inline void call_cond(BYTE cond);
static inline void call();
static inline void jp_n_cond(BYTE cond);
static inline void jp(short addr);
static inline void jp_cond(BYTE cond);
static inline void jr(BYTE_S d);
static inline void jr_cond(BYTE cond, BYTE_S d);
static inline void jr_n_cond(BYTE cond, BYTE_S d);
static inline void ret();
static inline void ret_cond(BYTE cond);
static inline void ret_n_cond(BYTE cond);
static inline void reti();
static inline void rst(BYTE addr);
//8bit load/store/move instructions
static inline void ld_reg_data(BYTE *reg, BYTE data);
static inline void ld_reg(BYTE *reg1, BYTE data);
static inline void ld_mem(BYTE reg1, BYTE reg2);
//16bit load/store/move instructions
static inline void ld_at(short addr);
static inline void ld_from_a8(short addr);
static inline void ld_hl_sp_p_r8();
static inline void ld_sp_hl();
static inline void pop(BYTE *reg1, BYTE *reg2);
static inline void push(BYTE reg1, BYTE reg2);

//8bit arithmetic/logical instructions
static inline void adc(BYTE data);
static inline void add(BYTE data);
static inline void and(BYTE data);
static inline void ccf();
static inline void cp(BYTE data);
static inline void cpl();
static inline void daa();
static inline void dec_at(short addr);
static inline void dec_smpl(BYTE *reg1);
static inline void dec_sp();
static inline void inc_at(short addr);
static inline void inc_smpl(BYTE *reg1);
static inline void inc_sp();
static inline void sbc(BYTE data);
static inline void scf();
static inline void sub(BYTE data);
static inline void or(BYTE data);
static inline void xor(BYTE data);

//16 bit arithmetic/logical instructions
static inline void add_dbl(BYTE *reg1, BYTE *reg2, short data);
static inline void add_sp_r8(BYTE_S data);
static inline void dec_dbl(BYTE *reg1, BYTE *reg2);
static inline void inc_dbl(BYTE *reg1, BYTE *reg2);

//8bit rotations/shifts and bit instructions
static inline void bit(BYTE bit, BYTE data);
static inline void res(BYTE b, BYTE *a);
static inline void res_hl(BYTE b);
static inline void rl(BYTE *data);
static inline void rl_hl();
static inline void rla();
static inline void rlc(BYTE *data);
static inline void rlc_hl();
static inline void rlca();
static inline void rr(BYTE *data);
static inline void rr_hl();
static inline void rra();
static inline void rrc(BYTE *data);
static inline void rrc_hl();
static inline void rrca();
static inline void set(BYTE b, BYTE *a); 
static inline void set_hl(BYTE b);
static inline void srl(BYTE *data);
static inline void srl_hl();
static inline void sla(BYTE *data);
static inline void sla_hl();
static inline void sra(BYTE *data);
static inline void sra_hl();
static inline void swap(BYTE *data);
static inline void swap_hl();

#endif
