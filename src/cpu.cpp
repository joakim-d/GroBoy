#include "cpu.h"
#define V_BLANK 0
#define LCD_STAT 1
#define TIMER 2
#define SERIAL 3
#define JOYPAD 4


extern const uint16_t daa_table[4096];
extern const uint8_t z80_cycles[];
extern const uint8_t z80_cb_cycles[];
extern int sound_cycles;

Cpu::Cpu() : memory_(0), halted_(0), ime_counter_(0), IME_(false){
    z80_.PC = 0x0100;
    z80_.SP = 0xFFFE;
    z80_.A = 0x01;
    z80_.F = 0xB0;
    z80_.B = 0x00;
    z80_.C = 0x13;
    z80_.D = 0x00;
    z80_.E = 0xD8;
    z80_.H = 0x01;
    z80_.L = 0x4D;
}

Cpu::~Cpu(){}

void Cpu::set_memory(Memory *memory){
    memory_ = memory;
}

int Cpu::run(){
    //int interrupt_period;
    //int counter;
    //counter=interrupt_period;
    BYTE op_code;

    op_code = memory_->read(z80_.PC);
    cycles_ = z80_cycles[op_code];
    //sound_cycles -= cycles_ % 8 ;

    if(!halted_){
#ifdef DEBUG
            static int i = 0;
            printf("instruction : %d\n", i++);
            printf("PC: %x \n", z80_.PC);
            printf("Opcode :%x + next 2 bytes: %x, %x\n",op_code, memory_->read(z80_.PC + 1), memory_->read(z80_.PC + 2));
            //printf("The next two bytes in memory are: %x, %x\n", memory_->read(z80_.PC + 1), memory_->read(z80_.PC + 2));
            printf("AF: %x\n", (z80_.A << 8) + z80_.F);
            printf("BC: %x\n", (z80_.B << 8) + z80_.C);
            printf("DE: %x\n", (z80_.D << 8) + z80_.E);
            printf("HL: %x\n", (z80_.H << 8) + z80_.L);
            printf("SP: %x\n", z80_.SP);
            printf("Top of the stack: %x\n\n", (memory_->read(z80_.SP + 1) << 8) + memory_->read(z80_.SP));
#endif
        z80_.PC++;
        switch(op_code){
            //Maxi table d'instructions

            case 0x00:
                //NOP
                break;
            case 0x01:
                //LD BC,d16
                ld_reg(&z80_.C, memory_->read(z80_.PC++));
                ld_reg(&z80_.B, memory_->read(z80_.PC++));
                break;
            case 0x02:
                //LD (BC),A
                //Décalage à gauche pour
                //concaténation des valeurs de registres
                ld_at((z80_.B << 8) + z80_.C);
                break;
            case 0x03:
                //INC BC
                inc_dbl(&z80_.B,&z80_.C);
                break;
            case 0x04:
                //INC B
                inc_smpl(&z80_.B);
                break;
            case 0x05:
                //DEC B
                dec_smpl(&z80_.B);
                break;
            case 0x06:
                //LD B,*
                ld_reg(&z80_.B, memory_->read(z80_.PC++));
                break;
            case 0x07:
                //RLCA
                rlca();
                break;
            case 0x08:
                //LD (A16),SP
                ld_a16_sp();
                break;
            case 0x09:
                //ADD HL,BC
                add_dbl(&z80_.H,&z80_.L, (z80_.B << 8) + z80_.C);
                break;
            case 0x0A:
                //LD A,(BC)
                ld_reg(&z80_.A, memory_->read((z80_.B << 8)+z80_.C));
                break;
            case 0x0B:
                //DEC BC
                dec_dbl(&z80_.B,&z80_.C);
                break;
            case 0x0C:
                //INC C
                inc_smpl(&z80_.C);
                break;
            case 0x0D:
                //DEC C
                dec_smpl(&z80_.C);
                break;
            case 0x0E:
                //LD C,d8
                ld_reg(&z80_.C, memory_->read(z80_.PC++));
                break;
            case 0x0F:
                //RRCA
                rrca();
                break;
            case 0x10:
                //STOP 0
                stop();
                break;
            case 0x11:
                //LD DE, d16
                ld_reg(&z80_.E, memory_->read(z80_.PC++));
                ld_reg(&z80_.D, memory_->read(z80_.PC++));
                break;
            case 0x12:
                //LD (DE), A
                ld_at((z80_.D << 8) + z80_.E);
                break;
            case 0x13:
                //INC DE
                inc_dbl(&z80_.D,&z80_.E);
                break;
            case 0x14:
                //INC D
                inc_smpl(&z80_.D);
                break;
            case 0x15:
                //DEC D
                dec_smpl(&z80_.D);
                break;
            case 0x16:
                //LD D,d8
                ld_reg(&z80_.D, memory_->read(z80_.PC++));
                break;
            case 0x17:
                //RLA
                rla();
                break;
            case 0x18:
                //JR r8
                jr(memory_->read(z80_.PC++));
                break;
            case 0x19:
                //ADD HL,DE
                add_dbl(&z80_.H,&z80_.L, (z80_.D << 8) + z80_.E);
                break;
            case 0x1A:
                //LD A,(DE)
                ld_reg(&z80_.A, memory_->read((z80_.D << 8)+z80_.E));
                break;
            case 0x1B:
                //DEC DE
                dec_dbl(&z80_.D,&z80_.E);
                break;
            case 0x1C:
                //INC E
                inc_smpl(&z80_.E);
                break;
            case 0x1D:
                //DEC E
                dec_smpl(&z80_.E);
                break;
            case 0x1E:
                //LD E,d8
                ld_reg(&z80_.E, memory_->read(z80_.PC++));
                break;
            case 0x1F:
                //RRA
                rra();
                break;
            case 0x20:
                //JR NZ, R8
                jr_n_cond(FLAG_Z,memory_->read(z80_.PC++));
                break;
            case 0x21:
                //LD HL, d16
                ld_reg(&z80_.L, memory_->read(z80_.PC++));
                ld_reg(&z80_.H, memory_->read(z80_.PC++));
                break;
            case 0x22:
                //LD (HL+), A
                ld_at((z80_.H << 8)+z80_.L);
                inc_dbl(&z80_.H,&z80_.L);
                break;
            case 0x23:
                //INC HL
                inc_dbl(&z80_.H,&z80_.L);
                break;
            case 0x24:
                //INC H
                inc_smpl(&z80_.H);
                break;
            case 0x25:
                //DEC H
                dec_smpl(&z80_.H);
                break;
            case 0x26:
                //LD H,d8
                ld_reg(&z80_.H, memory_->read(z80_.PC++));
                break;
            case 0x27:
                //DAA
                daa();
                break;
            case 0x28:
                //JR Z,r8
                jr_cond(FLAG_Z,memory_->read(z80_.PC++));
                break;
            case 0x29:
                //ADD HL,HL
                add_dbl(&z80_.H,&z80_.L, (z80_.H << 8) + z80_.L);
                break;
            case 0x2A:
                //LD A,(HL+)
                ld_reg(&z80_.A, memory_->read((z80_.H << 8)+z80_.L));
                inc_dbl(&z80_.H,&z80_.L);
                break;
            case 0x2B:
                //DEC HL
                dec_dbl(&z80_.H, &z80_.L);
                break;
            case 0x2C:
                //INC L
                inc_smpl(&z80_.L);
                break;
            case 0x2D:
                //DEC L
                dec_smpl(&z80_.L);
                break;
            case 0x2E:
                //LD L,d8
                ld_reg(&z80_.L, memory_->read(z80_.PC++));
                break;
            case 0x2F:
                //CPL
                cpl();
                break;
            case 0x30:
                //JR NC,r8
                jr_n_cond(FLAG_C,memory_->read(z80_.PC++));
                break;
            case 0x31:
                //LD SP,d16
                ld_sp((memory_->read(z80_.PC + 1) << 8)|memory_->read(z80_.PC));
                z80_.PC+=2;
                break;
            case 0x32:
                //LD (HL-), A
                ld_at((z80_.H << 8)+z80_.L);
                dec_dbl(&z80_.H,&z80_.L);
                break;
            case 0x33:
                //INC SP
                inc_sp();
                break;
            case 0x34:
                //INC (HL)
                inc_at((z80_.H << 8) + z80_.L);
                break;
            case 0x35:
                //DEC (HL)
                dec_at((z80_.H << 8) + z80_.L);
                break;
            case 0x36:
                //LD (HL),d8
                ld_mem((z80_.H << 8)+z80_.L, memory_->read(z80_.PC++));
                break;
            case 0x37:
                //SCF
                scf();
                break;
            case 0x38:
                //JR C,r8
                jr_cond(FLAG_C,memory_->read(z80_.PC++));
                break;
            case 0x39:
                //ADD HL,SP
                add_dbl(&z80_.H,&z80_.L, z80_.SP);
                break;
            case 0x3A:
                //LD A,(HL-)
                ld_reg(&z80_.A, memory_->read((z80_.H << 8)+z80_.L));
                dec_dbl(&z80_.H,&z80_.L);
                break;
            case 0x3B:
                //DEC SP
                dec_sp();
                break;
            case 0x3C:
                //INC A
                inc_smpl(&z80_.A);
                break;
            case 0x3D:
                //DEC A
                dec_smpl(&z80_.A);
                break;
            case 0x3E:
                //LD A,d8
                ld_reg(&z80_.A, memory_->read(z80_.PC++));
                break;
            case 0x3F:
                //CCF
                ccf();
                break;
            case 0x40:
                //LD B,B
                ld_reg(&z80_.B,z80_.B);
                break;
            case 0x41:
                //LD B,C
                ld_reg(&z80_.B,z80_.C);
                break;
            case 0x42:
                //LD B,D
                ld_reg(&z80_.B,z80_.D);
                break;
            case 0x43:
                //LD B,E
                ld_reg(&z80_.B,z80_.E);
                break;
            case 0x44:
                //LD B,H
                ld_reg(&z80_.B,z80_.H);
                break;
            case 0x45:
                //LD B,L
                ld_reg(&z80_.B,z80_.L);
                break;
            case 0x46:
                //LD B,(HL)
                ld_reg(&z80_.B, memory_->read((z80_.H << 8)+z80_.L));
                break;
            case 0x47:
                //LD B,A
                ld_reg(&z80_.B,z80_.A);
                break;
            case 0x48:
                //LD C,B
                ld_reg(&z80_.C,z80_.B);
                break;
            case 0x49:
                //LD C,C
                ld_reg(&z80_.C,z80_.C);
                break;
            case 0x4A:
                //LD C,D
                ld_reg(&z80_.C,z80_.D);
                break;
            case 0x4B:
                //LD C,E
                ld_reg(&z80_.C,z80_.E);
                break;
            case 0x4C:
                //LD C,H
                ld_reg(&z80_.C,z80_.H);
                break;
            case 0x4D:
                //LD C,L
                ld_reg(&z80_.C,z80_.L);
                break;
            case 0x4E:
                //LD C,(HL)
                ld_reg(&z80_.C, memory_->read((z80_.H << 8)+z80_.L));
                break;
            case 0x4F:
                //LD C,A
                ld_reg(&z80_.C,z80_.A);
                break;
            case 0x50:
                //LD D,B
                ld_reg(&z80_.D,z80_.B);
                break;
            case 0x51:
                //LD D,C
                ld_reg(&z80_.D,z80_.C);
                break;
            case 0x52:
                //LD D,D
                ld_reg(&z80_.D,z80_.D);
                break;
            case 0x53:
                //LD D,E
                ld_reg(&z80_.D,z80_.E);
                break;
            case 0x54:
                //LD D,H
                ld_reg(&z80_.D,z80_.H);
                break;
            case 0x55:
                //LD D,L
                ld_reg(&z80_.D,z80_.L);
                break;
            case 0x56:
                //LD D,(HL)
                ld_reg(&z80_.D, memory_->read((z80_.H << 8) + z80_.L));
                break;
            case 0x57:
                //LD D,A
                ld_reg(&z80_.D,z80_.A);
                break;
            case 0x58:
                //LD E,B
                ld_reg(&z80_.E,z80_.B);
                break;
            case 0x59:
                //LD E,C
                ld_reg(&z80_.E,z80_.C);
                break;
            case 0x5A:
                //LD E,D
                ld_reg(&z80_.E,z80_.D);
                break;
            case 0x5B:
                //LD E,E
                ld_reg(&z80_.E,z80_.E);
                break;
            case 0x5C:
                //LD E,H
                ld_reg(&z80_.E,z80_.H);
                break;
            case 0x5D:
                //LD E,L
                ld_reg(&z80_.E,z80_.L);
                break;
            case 0x5E:
                //LD E,(HL)
                ld_reg(&z80_.E,memory_->read((z80_.H << 8)+z80_.L));
                break;
            case 0x5F:
                //LD E,A
                ld_reg(&z80_.E,z80_.A);
                break;
            case 0x60:
                //LD H,B
                ld_reg(&z80_.H,z80_.B);
                break;
            case 0x61:
                //LD H,C
                ld_reg(&z80_.H,z80_.C);
                break;
            case 0x62:
                //LD H,D
                ld_reg(&z80_.H,z80_.D);
                break;
            case 0x63:
                //LD H,E
                ld_reg(&z80_.H,z80_.E);
                break;
            case 0x64:
                //LD H,H
                ld_reg(&z80_.H,z80_.H);
                break;
            case 0x65:
                //LD H,L
                ld_reg(&z80_.H,z80_.L);
                break;
            case 0x66:
                //LD H,(HL)
                ld_reg(&z80_.H,memory_->read((z80_.H << 8) + z80_.L));
                break;
            case 0x67:
                //LD H,A
                ld_reg(&z80_.H,z80_.A);
                break;
            case 0x68:
                //LD L,B
                ld_reg(&z80_.L,z80_.B);
                break;
            case 0x69:
                //LD L,C
                ld_reg(&z80_.L,z80_.C);
                break;
            case 0x6A:
                //LD L,D
                ld_reg(&z80_.L,z80_.D);
                break;
            case 0x6B:
                //LD L,E
                ld_reg(&z80_.L,z80_.E);
                break;
            case 0x6C:
                //LD L,H
                ld_reg(&z80_.L,z80_.H);
                break;
            case 0x6D:
                //LD L,L
                ld_reg(&z80_.L,z80_.L);
                break;
            case 0x6E:
                //LD L,(HL)
                ld_reg(&z80_.L,memory_->read((z80_.H << 8)+z80_.L));
                break;
            case 0x6F:
                //LD L,A
                ld_reg(&z80_.L,z80_.A);
                break;
            case 0x70:
                //LD (HL),B
                ld_mem((z80_.H << 8)+z80_.L,z80_.B);
                break;
            case 0x71:
                //LD (HL),C
                ld_mem((z80_.H << 8)+z80_.L,z80_.C);
                break;
            case 0x72:
                //LD (HL),D
                ld_mem((z80_.H << 8)+z80_.L,z80_.D);
                break;
            case 0x73:
                //LD (HL),E
                ld_mem((z80_.H << 8)+z80_.L,z80_.E);
                break;
            case 0x74:
                //LD (HL),H
                ld_mem((z80_.H << 8)+z80_.L,z80_.H);
                break;
            case 0x75:
                //LD (HL),L
                ld_mem((z80_.H << 8)+z80_.L,z80_.L);
                break;
            case 0x76:
                //HALT
                halt();
                break;
            case 0x77:
                //LD (HL),A
                ld_mem((z80_.H << 8)+z80_.L,z80_.A);
                break;
            case 0x78:
                //LD A,B
                ld_reg(&z80_.A,z80_.B);
                break;
            case 0x79:
                //LD A,C
                ld_reg(&z80_.A,z80_.C);
                break;
            case 0x7A:
                //LD A,D
                ld_reg(&z80_.A,z80_.D);
                break;
            case 0x7B:
                //LD A,E
                ld_reg(&z80_.A,z80_.E);
                break;
            case 0x7C:
                //LD A,H
                ld_reg(&z80_.A,z80_.H);
                break;
            case 0x7D:
                //LD A,L
                ld_reg(&z80_.A,z80_.L);
                break;
            case 0x7E:
                //LD A,(HL)
                ld_reg(&z80_.A,memory_->read((z80_.H << 8)+z80_.L));
                break;
            case 0x7F:
                //LD A,A
                ld_reg(&z80_.A,z80_.A);
                break;
            case 0x80:
                //ADD A,B
                add(z80_.B);
                break;
            case 0x81:
                //ADD A,C
                add(z80_.C);
                break;
            case 0x82:
                //ADD A,D
                add(z80_.D);
                break;
            case 0x83:
                //ADD A,E
                add(z80_.E);
                break;
            case 0x84:
                //ADD A,H
                add(z80_.H);
                break;
            case 0x85:
                //ADD A,L
                add(z80_.L);
                break;
            case 0x86:
                //ADD A,(HL)
                add(memory_->read((z80_.H << 8)+z80_.L));
                break;
            case 0x87:
                //ADD A,A
                add(z80_.A);
                break;
            case 0x88:
                //ADC A,B
                adc(z80_.B);
                break;
            case 0x89:
                //ADC A,C
                adc(z80_.C);
                break;
            case 0x8A:
                //ADC A,D
                adc(z80_.D);
                break;
            case 0x8B:
                //ADC A,E
                adc(z80_.E);
                break;
            case 0x8C:
                //ADC A,H
                adc(z80_.H);
                break;
            case 0x8D:
                //ADC A,L
                adc(z80_.L);
                break;
            case 0x8E:
                //ADC A,(HL)
                adc(memory_->read((z80_.H << 8)+z80_.L));
                break;
            case 0x8F:
                //ADC A,A
                adc(z80_.A);
                break;
            case 0x90:
                //SUB B
                sub(z80_.B);
                break;
            case 0x91:
                //SUB C
                sub(z80_.C);
                break;
            case 0x92:
                //SUB D
                sub(z80_.D);
                break;
            case 0x93:
                //SUB E
                sub(z80_.E);
                break;
            case 0x94:
                //SUB H
                sub(z80_.H);
                break;
            case 0x95:
                //SUB L
                sub(z80_.L);
                break;
            case 0x96:
                //SUB (HL)
                sub(memory_->read((z80_.H << 8)+z80_.L));
                break;
            case 0x97:
                //SUB A
                sub(z80_.A);
                break;
            case 0x98:
                //SBC A,B
                sbc(z80_.B);
                break;
            case 0x99:
                //SBC A,C
                sbc(z80_.C);
                break;
            case 0x9A:
                //SBC A,D
                sbc(z80_.D);
                break;
            case 0x9B:
                //SBC A,E
                sbc(z80_.E);
                break;
            case 0x9C:
                //SBC A,H
                sbc(z80_.H);
                break;
            case 0x9D:
                //SBC A,L
                sbc(z80_.L);
                break;
            case 0x9E:
                //SBC A,(HL)
                sbc(memory_->read((z80_.H << 8)+z80_.L));
                break;
            case 0x9F:
                //SBC A,A
                sbc(z80_.A);
                break;
            case 0xA0:
                //AND B
                and_(z80_.B);
                break;
            case 0xA1:
                //AND C
                and_(z80_.C);
                break;
            case 0xA2:
                //AND D
                and_(z80_.D);
                break;
            case 0xA3:
                //AND E
                and_(z80_.E);
                break;
            case 0xA4:
                //AND H
                and_(z80_.H);
                break;
            case 0xA5:
                //AND L
                and_(z80_.L);
                break;
            case 0xA6:
                //AND (HL)
                and_(memory_->read((z80_.H << 8)+z80_.L));
                break;
            case 0xA7:
                //AND A
                and_(z80_.A);
                break;
            case 0xA8:
                //XOR B
                xor_(z80_.B);
                break;
            case 0xA9:
                //XOR C
                xor_(z80_.C);
                break;
            case 0xAA:
                //XOR D
                xor_(z80_.D);
                break;
            case 0xAB:
                //XOR E
                xor_(z80_.E);
                break;
            case 0xAC:
                //XOR H
                xor_(z80_.H);
                break;
            case 0xAD:
                //XOR L
                xor_(z80_.L);
                break;
            case 0xAE:
                //XOR (HL)
                xor_(memory_->read((z80_.H << 8)+z80_.L));
                break;
            case 0xAF:
                //XOR A
                xor_(z80_.A);
                break;

            case 0xB0: //OR B
                or_(z80_.B);
                break;
            case 0xB1: //OR C
                or_(z80_.C);
                break;
            case 0xB2: //OR D
                or_(z80_.D);
                break;
            case 0xB3: //OR E
                or_(z80_.E);
                break;
            case 0xB4: //OR H
                or_(z80_.H);
                break;
            case 0xB5: //OR L
                or_(z80_.L);
                break;
            case 0xB6: //OR (HL)
                or_(memory_->read((z80_.H << 8) + z80_.L));
                break;
            case 0xB7: //OR A
                or_(z80_.A);
                break;
            case 0xB8: //CP B
                cp(z80_.B);
                break;
            case 0xB9: //CP C
                cp(z80_.C);
                break;
            case 0xBA: //CP D
                cp(z80_.D);
                break;
            case 0xBB: //CP E
                cp(z80_.E);
                break;
            case 0xBC: //CP H
                cp(z80_.H);
                break;
            case 0xBD: //CP L
                cp(z80_.L);
                break;
            case 0xBE: //CP (HL)
                cp(memory_->read((z80_.H << 8) + z80_.L));
                break;
            case 0xBF: //CP A
                cp(z80_.A);
                break;
            case 0xC0: //RET NZ
                ret_n_cond(FLAG_Z);
                break;
            case 0xC1: //POP BC
                pop(&z80_.B, &z80_.C);
                break;
            case 0xC2: //JP NZ,a16
                jp_n_cond(FLAG_Z);
                break;
            case 0xC3: //JP a16
                jp((memory_->read(z80_.PC + 1) << 8) + memory_->read(z80_.PC));
                break;
            case 0xC4: //CALL NZ,a16
                call_n_cond(0x80);
                break;
            case 0xC5: //PUSH BC
                push(z80_.B, z80_.C);
                break;
            case 0xC6: //ADD A,d8
                add(memory_->read(z80_.PC++));
                break;
            case 0xC7: //RST 00H
                rst(0);
                break;
            case 0xC8: //RET Z
                ret_cond(0x80);
                break;
            case 0xC9: //RET
                ret();
                break;
            case 0xCA: //JP Z,a16
                jp_cond(0x80);
                break;
            case 0xCB: //Two bytes Opcode
                op_code = memory_->read(z80_.PC++);
                cycles_ = z80_cb_cycles[op_code];
                switch(op_code){
                    case 0x00: //RLC B
                        rlc(&z80_.B);
                        break;
                    case 0x01: //RLC C
                        rlc(&z80_.C);
                        break;
                    case 0x02: //RLC D
                        rlc(&z80_.D);
                        break;
                    case 0x03: //RLC E
                        rlc(&z80_.E);
                        break;
                    case 0x04: //RLC H
                        rlc(&z80_.H);
                        break;
                    case 0x05: //RLC L
                        rlc(&z80_.L);
                        break;
                    case 0x06: //RLC (HL)
                        rlc_hl();
                        break;
                    case 0x07: //RLC A
                        rlc(&z80_.A);
                        break;
                    case 0x08: //RRC B
                        rrc(&z80_.B);
                        break;
                    case 0x09: //RRC C
                        rrc(&z80_.C);
                        break;
                    case 0x0A: //RRC D
                        rrc(&z80_.D);
                        break;
                    case 0x0B: //RRC E
                        rrc(&z80_.E);
                        break;
                    case 0x0C: //RRC H
                        rrc(&z80_.H);
                        break;
                    case 0x0D: //RRC L
                        rrc(&z80_.L);
                        break;
                    case 0x0E: //RRC (HL)
                        rrc_hl();
                        break;
                    case 0x0F: //RRC A
                        rrc(&z80_.A);
                        break;
                    case 0x10: //RL B
                        rl(&z80_.B);
                        break;
                    case 0x11: //RL C
                        rl(&z80_.C);
                        break;
                    case 0x12: //RL D
                        rl(&z80_.D);
                        break;
                    case 0x13: //RL E
                        rl(&z80_.E);
                        break;
                    case 0x14: //RL H
                        rl(&z80_.H);
                        break;
                    case 0x15: //RL L
                        rl(&z80_.L);
                        break;
                    case 0x16: //RL (HL)
                        rl_hl();
                        break;
                    case 0x17: //RL A
                        rl(&z80_.A);
                        break;
                    case 0x18: //RR B
                        rr(&z80_.B);
                        break;
                    case 0x19: //RR C
                        rr(&z80_.C);
                        break;
                    case 0x1A: //RR D
                        rr(&z80_.D);
                        break;
                    case 0x1B: //RR E
                        rr(&z80_.E);
                        break;
                    case 0x1C: //RR H
                        rr(&z80_.H);
                        break;
                    case 0x1D: //RR L
                        rr(&z80_.L);
                        break;
                    case 0x1E: //RR (HL)
                        rr_hl();
                        break;
                    case 0x1F: //RR A
                        rr(&z80_.A);
                        break;
                    case 0x20: //SLA B
                        sla(&z80_.B);
                        break;
                    case 0x21: //SLA C
                        sla(&z80_.C);
                        break;
                    case 0x22: //SLA D
                        sla(&z80_.D);
                        break;
                    case 0x23: //SLA E
                        sla(&z80_.E);
                        break;
                    case 0x24: //SLA H
                        sla(&z80_.H);
                        break;
                    case 0x25: //SLA L
                        sla(&z80_.L);
                        break;
                    case 0x26: //SLA (HL)
                        sla_hl();
                        break;
                    case 0x27: //SLA A
                        sla(&z80_.A);
                        break;
                    case 0x28: //SRA B
                        sra(&z80_.B);
                        break;
                    case 0x29: //SRA C
                        sra(&z80_.C);
                        break;
                    case 0x2A: //SRA D
                        sra(&z80_.D);
                        break;
                    case 0x2B: //SRA E
                        sra(&z80_.E);
                        break;
                    case 0x2C: //SRA H
                        sra(&z80_.H);
                        break;
                    case 0x2D: //SRA L
                        sra(&z80_.L);
                        break;
                    case 0x2E: //SRA (HL)
                        sra_hl();
                        break;
                    case 0x2F: //SRA A
                        sra(&z80_.A);
                        break;
                    case 0x30: //SWAP B
                        swap(&z80_.B);
                        break;
                    case 0x31: //SWAP C
                        swap(&z80_.C);
                        break;
                    case 0x32: //SWAP D
                        swap(&z80_.D);
                        break;
                    case 0x33: //SWAP E
                        swap(&z80_.E);
                        break;
                    case 0x34: //SWAP H
                        swap(&z80_.H);
                        break;
                    case 0x35: //SWAP L
                        swap(&z80_.L);
                        break;
                    case 0x36: //SWAP (HL)
                        swap_hl();
                        break;
                    case 0x37: //SWAP A
                        swap(&z80_.A);
                        break;
                    case 0x38: //SRL B
                        srl(&z80_.B);
                        break;
                    case 0x39: //SRL C
                        srl(&z80_.C);
                        break;
                    case 0x3A: //SRL D
                        srl(&z80_.D);
                        break;
                    case 0x3B: //SRL E
                        srl(&z80_.E);
                        break;
                    case 0x3C: //SRL H
                        srl(&z80_.H);
                        break;
                    case 0x3D: //SRL L
                        srl(&z80_.L);
                        break;
                    case 0x3E: //SRL (HL)
                        srl_hl();
                        break;
                    case 0x3F: //SRL A
                        srl(&z80_.A);
                        break;
                    case 0x40: //BIT 0,B
                        bit(0x01, z80_.B);
                        break;
                    case 0x41: //BIT 0,C
                        bit(0x01, z80_.C);
                        break;
                    case 0x42: //BIT 0,D
                        bit(0x01, z80_.D);
                        break;
                    case 0x43: //BIT 0,E
                        bit(0x01, z80_.E);
                        break;
                    case 0x44: //BIT 0,H
                        bit(0x01, z80_.H);
                        break;
                    case 0x45: //BIT 0,L
                        bit(0x01, z80_.L);
                        break;
                    case 0x46: //BIT 0,(HL)
                        bit(0x01, memory_->read((z80_.H << 8) + z80_.L));
                        break;
                    case 0x47: //BIT 0,A
                        bit(0x01, z80_.A);
                        break;
                    case 0x48: //BIT 1,B
                        bit(0x02, z80_.B);
                        break;
                    case 0x49: //BIT 1,C
                        bit(0x02, z80_.C);
                        break;
                    case 0x4A: //BIT 1,D
                        bit(0x02, z80_.D);
                        break;
                    case 0x4B: //BIT 1,E
                        bit(0x02, z80_.E);
                        break;
                    case 0x4C: //BIT 1,H
                        bit(0x02, z80_.H);
                        break;
                    case 0x4D: //BIT 1,L
                        bit(0x02, z80_.L);
                        break;
                    case 0x4E: //BIT 1,(HL)
                        bit(0x02, memory_->read((z80_.H << 8) + z80_.L));
                        break;
                    case 0x4F: //BIT 1,A
                        bit(0x02, z80_.A);
                        break;
                    case 0x50: //BIT 2,B
                        bit(0x04, z80_.B);
                        break;
                    case 0x51: //BIT 2,C
                        bit(0x04, z80_.C);
                        break;
                    case 0x52: //BIT 2,D
                        bit(0x04, z80_.D);
                        break;
                    case 0x53: //BIT 2,E
                        bit(0x04, z80_.E);
                        break;
                    case 0x54: //BIT 2,H
                        bit(0x04, z80_.H);
                        break;
                    case 0x55: //BIT 2,L
                        bit(0x04, z80_.L);
                        break;
                    case 0x56: //BIT 2,(HL)
                        bit(0x04, memory_->read((z80_.H << 8) + z80_.L));
                        break;
                    case 0x57: //BIT 2,A
                        bit(0x04, z80_.A);
                        break;
                    case 0x58: //BIT 3,B
                        bit(0x08, z80_.B);
                        break;
                    case 0x59: //BIT 3,C
                        bit(0x08, z80_.C);
                        break;
                    case 0x5A: //BIT 3,D
                        bit(0x08, z80_.D);
                        break;
                    case 0x5B: //BIT 3,E
                        bit(0x08, z80_.E);
                        break;
                    case 0x5C: //BIT 3,H
                        bit(0x08, z80_.H);
                        break;
                    case 0x5D: //BIT 3,L
                        bit(0x08, z80_.L);
                        break;
                    case 0x5E: //BIT 3,(HL)
                        bit(0x08, memory_->read((z80_.H << 8) + z80_.L));
                        break;
                    case 0x5F: //BIT 3,A
                        bit(0x08, z80_.A);
                        break;
                    case 0x60: //BIT 4,B
                        bit((0x10),z80_.B);
                        break;
                    case 0x61: //BIT 4,C
                        bit((0x10),z80_.C);
                        break;
                    case 0x62: //BIT 4,D
                        bit((0x10),z80_.D);
                        break;
                    case 0x63: //BIT 4,E
                        bit((0x10),z80_.E);
                        break;
                    case 0x64: //BIT 4,H
                        bit((0x10),z80_.H);
                        break;
                    case 0x65: //BIT 4,L
                        bit((0x10),z80_.L);
                        break;
                    case 0x66: //BIT 4,HL
                        bit((0x10),memory_->read((z80_.H << 8) + z80_.L));
                        break;
                    case 0x67: //BIT 4,A
                        bit((0x10),z80_.A);
                        break;
                    case 0x68: //BIT 5,B
                        bit((0x20),z80_.B);
                        break;
                    case 0x69: //BIT 5,C
                        bit((0x20),z80_.C);
                        break;
                    case 0x6A: //BIT 5,D
                        bit((0x20),z80_.D);
                        break;
                    case 0x6B: //BIT 5,E
                        bit((0x20),z80_.E);
                        break;
                    case 0x6C: //BIT 5,H
                        bit((0x20),z80_.H);
                        break;
                    case 0x6D: //BIT 5,L
                        bit((0x20),z80_.L);
                        break;
                    case 0x6E: //BIT 5,HL
                        bit((0x20),memory_->read((z80_.H << 8) + z80_.L));
                        break;
                    case 0x6F: //BIT 5,A
                        bit((0x20),z80_.A);
                        break;
                    case 0x70: //BIT 6,B
                        bit((0x40),z80_.B);
                        break;
                    case 0x71: //BIT 6,C
                        bit((0x40),z80_.C);
                        break;
                    case 0x72: //BIT 6,D
                        bit((0x40),z80_.D);
                        break;
                    case 0x73: //BIT 6,E
                        bit((0x40),z80_.E);
                        break;
                    case 0x74: //BIT 6,H
                        bit((0x40),z80_.H);
                        break;
                    case 0x75: //BIT 6,L
                        bit((0x40),z80_.L);
                        break;
                    case 0x76: //BIT 6,HL
                        bit((0x40),memory_->read((z80_.H << 8) + z80_.L));
                        break;
                    case 0x77: //BIT 6,A
                        bit((0x40),z80_.A);
                        break;
                    case 0x78: //BIT 7,B
                        bit((0x80),z80_.B);
                        break;
                    case 0x79: //BIT 7,C
                        bit((0x80),z80_.C);
                        break;
                    case 0x7A: //BIT 7,D
                        bit((0x80),z80_.D);
                        break;
                    case 0x7B: //BIT 7,E
                        bit((0x80),z80_.E);
                        break;
                    case 0x7C: //BIT 7,H
                        bit((0x80),z80_.H);
                        break;
                    case 0x7D: //BIT 7,L
                        bit((0x80),z80_.L);
                        break;
                    case 0x7E: //BIT 7,HL
                        bit((0x80),memory_->read((z80_.H << 8) + z80_.L));
                        break;
                    case 0x7F: //BIT 7,A
                        bit((0x80),z80_.A);
                        break;
                    case 0x80: //RES 0,B
                        res(0,&z80_.B);
                        break;
                    case 0x81: //RES 0,C
                        res(0,&z80_.C);
                        break;
                    case 0x82: //RES 0,D
                        res(0,&z80_.D);
                        break;
                    case 0x83: //RES 0,E
                        res(0,&z80_.E);
                        break;
                    case 0x84: //RES 0,H
                        res(0,&z80_.H);
                        break;
                    case 0x85: //RES 0,L
                        res(0,&z80_.L);
                        break;
                    case 0x86: //RES 0,HL
                        res_hl(0);
                        break;
                    case 0x87: //RES 0,A
                        res(0,&z80_.A);
                        break;
                    case 0x88: //RES 1,B
                        res(1,&z80_.B);
                        break;
                    case 0x89: //RES 1,C
                        res(1,&z80_.C);
                        break;
                    case 0x8A: //RES 1,D
                        res(1,&z80_.D);
                        break;
                    case 0x8B: //RES 1,E
                        res(1,&z80_.E);
                        break;
                    case 0x8C: //RES 1,H
                        res(1,&z80_.H);
                        break;
                    case 0x8D: //RES 1,L
                        res(1,&z80_.L);
                        break;
                    case 0x8E: //RES 1,HL
                        res_hl(1);
                        break;
                    case 0x8F: //RES 1,A
                        res(1,&z80_.A);
                        break;
                    case 0x90: //RES 2,B
                        res(2,&z80_.B);
                        break;
                    case 0x91: //RES 2,C
                        res(2,&z80_.C);
                        break;
                    case 0x92: //RES 2,D
                        res(2,&z80_.D);
                        break;
                    case 0x93: //RES 2,E
                        res(2,&z80_.E);
                        break;
                    case 0x94: //RES 2,H
                        res(2,&z80_.H);
                        break;
                    case 0x95: //RES 2,L
                        res(2,&z80_.L);
                        break;
                    case 0x96: //RES 2,HL
                        res_hl(2);
                        break;
                    case 0x97: //RES 2,A
                        res(2,&z80_.A);
                        break;
                    case 0x98: //RES 3,B
                        res(3,&z80_.B);
                        break;
                    case 0x99: //RES 3,C
                        res(3,&z80_.C);
                        break;
                    case 0x9A: //RES 3,D
                        res(3,&z80_.D);
                        break;
                    case 0x9B: //RES 3,E
                        res(3,&z80_.E);
                        break;
                    case 0x9C: //RES 3,H
                        res(3,&z80_.H);
                        break;
                    case 0x9D: //RES 3,L
                        res(3,&z80_.L);
                        break;
                    case 0x9E: //RES 3,HL
                        res_hl(3);
                        break;
                    case 0x9F: //RES 3,A
                        res(3,&z80_.A);
                        break;
                    case 0xA0: //RES 4,B
                        res(4,&z80_.B);
                        break;
                    case 0xA1: //RES 4,C
                        res(4,&z80_.C);
                        break;
                    case 0xA2: //RES 4,D
                        res(4,&z80_.D);
                        break;
                    case 0xA3: //RES 4,E
                        res(4,&z80_.E);
                        break;
                    case 0xA4: //RES 4,H
                        res(4,&z80_.H);
                        break;
                    case 0xA5: //RES 4,L
                        res(4,&z80_.L);
                        break;
                    case 0xA6: //RES 4,HL
                        res_hl(4);
                        break;
                    case 0xA7: //RES 4,A
                        res(4,&z80_.A);
                        break;
                    case 0xA8: //RES 5,B
                        res(5,&z80_.B);
                        break;
                    case 0xA9: //RES 5,C
                        res(5,&z80_.C);
                        break;
                    case 0xAA: //RES 5,D
                        res(5,&z80_.D);
                        break;
                    case 0xAB: //RES 5,E
                        res(5,&z80_.E);
                        break;
                    case 0xAC: //RES 5,H
                        res(5,&z80_.H);
                        break;
                    case 0xAD: //RES 5,L
                        res(5,&z80_.L);
                        break;
                    case 0xAE: //RES 5,HL
                        res_hl(5);
                        break;
                    case 0xAF: //RES 5,A
                        res(5,&z80_.A);
                        break;
                    case 0xB0: //RES 6,B
                        res(6,&z80_.B);
                        break;
                    case 0xB1: //RES 6,C
                        res(6,&z80_.C);
                        break;
                    case 0xB2: //RES 6,D
                        res(6,&z80_.D);
                        break;
                    case 0xB3: //RES 6,E
                        res(6,&z80_.E);
                        break;
                    case 0xB4: //RES 6,H
                        res(6,&z80_.H);
                        break;
                    case 0xB5: //RES 6,L
                        res(6,&z80_.L);
                        break;
                    case 0xB6: //RES 6,Hl
                        res_hl(6);
                        break;
                    case 0xB7: //RES 6,A
                        res(6,&z80_.A);
                        break;
                    case 0xB8: //RES 7,B
                        res(7,&z80_.B);
                        break;
                    case 0xB9: //RES 7,C
                        res(7,&z80_.C);
                        break;
                    case 0xBA: //RES 7,D
                        res(7,&z80_.D);
                        break;
                    case 0xBB: //RES 7,E
                        res(7,&z80_.E);
                        break;
                    case 0xBC: //RES 7,H
                        res(7,&z80_.H);
                        break;
                    case 0xBD: //RES 7,L
                        res(7,&z80_.L);
                        break;
                    case 0xBE: //RES 7,HL
                        res_hl(7);
                        break;
                    case 0xBF: //RES 7,A
                        res(7,&z80_.A);
                        break;
                    case 0xC0: //SET 0,B
                        set(0,&z80_.B);
                        break;
                    case 0xC1: //SET 0,C
                        set(0,&z80_.C);
                        break;
                    case 0xC2: //SET 0,D
                        set(0,&z80_.D);
                        break;
                    case 0xC3: //SET 0,E
                        set(0,&z80_.E);
                        break;
                    case 0xC4: //SET 0,H
                        set(0,&z80_.H);
                        break;
                    case 0xC5: //SET 0,L
                        set(0,&z80_.L);
                        break;
                    case 0xC6: //SET 0,HL
                        set_hl(0);
                        break;
                    case 0xC7: //SET 0,A
                        set(0,&z80_.A);
                        break;
                    case 0xC8: //SET 1,B
                        set(1,&z80_.B);
                        break;
                    case 0xC9: //SET 1,C
                        set(1,&z80_.C);
                        break;
                    case 0xCA: //SET 1,D
                        set(1,&z80_.D);
                        break;
                    case 0xCB: //SET 1,E
                        set(1,&z80_.E);
                        break;
                    case 0xCC: //SET 1,H
                        set(1,&z80_.H);
                        break;
                    case 0xCD: //SET 1,L
                        set(1,&z80_.L);
                        break;
                    case 0xCE: //SET 1,HL
                        set_hl(1);
                        break;
                    case 0xCF: //SET 1,A
                        set(1,&z80_.A);
                        break;
                    case 0xD0: //SET 2,B
                        set(2,&z80_.B);
                        break;
                    case 0xD1: //SET 2,C
                        set(2,&z80_.C);
                        break;
                    case 0xD2: //SET 2,D
                        set(2,&z80_.D);
                        break;
                    case 0xD3: //SET 2,E
                        set(2,&z80_.E);
                        break;
                    case 0xD4: //SET 2,H
                        set(2,&z80_.H);
                        break;
                    case 0xD5: //SET 2,L
                        set(2,&z80_.L);
                        break;
                    case 0xD6: //SET 2,HL
                        set_hl(2);
                        break;
                    case 0xD7: //SET 2,A
                        set(2,&z80_.A);
                        break;
                    case 0xD8: //SET 3,B
                        set(3,&z80_.B);
                        break;
                    case 0xD9: //SET 3,C
                        set(3,&z80_.C);
                        break;
                    case 0xDA: //SET 3,D
                        set(3,&z80_.D);
                        break;
                    case 0xDB: //SET 3,E
                        set(3,&z80_.E);
                        break;
                    case 0xDC: //SET 3,H
                        set(3,&z80_.H);
                        break;
                    case 0xDD: //SET 3,L
                        set(3,&z80_.L);
                        break;
                    case 0xDE: //SET 3,Hl
                        set_hl(3);
                        break;
                    case 0xDF: //SET 3,A
                        set(3,&z80_.A);
                        break;
                    case 0xE0: //SET 4,B
                        set(4,&z80_.B);
                        break;
                    case 0xE1: //SET 4,C
                        set(4,&z80_.C);
                        break;
                    case 0xE2: //SET 4,D
                        set(4,&z80_.D);
                        break;
                    case 0xE3: //SET 4,E
                        set(4,&z80_.E);
                        break;
                    case 0xE4: //SET 4,H
                        set(4,&z80_.H);
                        break;
                    case 0xE5: //SET 4,L
                        set(4,&z80_.L);
                        break;
                    case 0xE6: //SET 4,HL
                        set_hl(4);
                        break;
                    case 0xE7: //SET 4,A
                        set(4,&z80_.A);
                        break;
                    case 0xE8: //SET 5,B
                        set(5,&z80_.B);
                        break;
                    case 0xE9: //SET 5,C
                        set(5,&z80_.C);
                        break;
                    case 0xEA: //SET 5,D
                        set(5,&z80_.D);
                        break;
                    case 0xEB: //SET 5,E
                        set(5,&z80_.E);
                        break;
                    case 0xEC: //SET 5,H
                        set(5,&z80_.H);
                        break;
                    case 0xED: //SET 5,L
                        set(5,&z80_.L);
                        break;
                    case 0xEE: //SET 5,HL
                        set_hl(5);
                        break;
                    case 0xEF: //SET 5,A
                        set(5,&z80_.A);
                        break;
                    case 0xF0: //SET 6,B
                        set(6,&z80_.B);
                        break;
                    case 0xF1: //SET 6,C
                        set(6,&z80_.C);
                        break;
                    case 0xF2: //SET 6,D
                        set(6,&z80_.D);
                        break;
                    case 0xF3: //SET 6,E
                        set(6,&z80_.E);
                        break;
                    case 0xF4: //SET 6,H
                        set(6,&z80_.H);
                        break;
                    case 0xF5: //SET 6,L
                        set(6,&z80_.L);
                        break;
                    case 0xF6: //SET 6,HL
                        set_hl(6);
                        break;
                    case 0xF7: //SET 6,A
                        set(6,&z80_.A);
                        break;
                    case 0xF8: //SET 7,B
                        set(7,&z80_.B);
                        break;
                    case 0xF9: //SET 7,C
                        set(7,&z80_.C);
                        break;
                    case 0xFA: //SET 7,D
                        set(7,&z80_.D);
                        break;
                    case 0xFB: //SET 7,E
                        set(7,&z80_.E);
                        break;
                    case 0xFC: //SET 7,H
                        set(7,&z80_.H);
                        break;
                    case 0xFD: //SET 7,L
                        set(7,&z80_.L);
                        break;
                    case 0xFE: //SET 7,HL
                        set_hl(7);
                        break;
                    case 0xFF: //SET 7,A
                        set(7,&z80_.A);
                        break;
                }
                break;
            case 0xCC: //CALL Z,a16
                call_cond(0x80);
                break;
            case 0xCD: //CALL a16
                call();
                break;
            case 0xCE: //ADC A,d8
                adc(memory_->read(z80_.PC++));
                break;
            case 0xCF: //RST 08H
                rst(0x08);
                break;
            case 0xD0: //RET NC
                ret_n_cond(0x10);
                break;
            case 0xD1: //POP DE
                pop(&z80_.D, &z80_.E);
                break;
            case 0xD2: //JP NC,a16
                jp_n_cond(0x10);
                break;
            case 0xD4: //CALL NC,a16
                call_n_cond(0x10);
                break;
            case 0xD5: //PUSH DE
                push(z80_.D, z80_.E);
                break;
            case 0xD6: //SUB d8
                sub(memory_->read(z80_.PC++));
                break;
            case 0xD7: //RST 10H
                rst(0x10);
                break;
            case 0xD8: //RET C
                ret_cond(FLAG_C);
                break;
            case 0xD9: //RETI
                reti();
                break;
            case 0xDA: //JP C,a16
                jp_cond(0x10);
                break;
            case 0xDC: //CALL C,a16
                call_cond(0x10);
                break;
            case 0xDE: //SBC A,d8
                sbc(memory_->read(z80_.PC++));
                break;
            case 0xDF: //RST 18H
                rst(0x18);
                break;
            case 0xE0: //LDH (a8),A
                ld_at(0xFF00 + memory_->read(z80_.PC++));
                break;
            case 0xE1: //POP HL
                pop(&z80_.H, &z80_.L);
                break;
            case 0xE2: //LD (C), A
                ld_at(0xFF00 + z80_.C);
                break;
            case 0xE5: //PUSH HL
                push(z80_.H, z80_.L);
                break;
            case 0xE6: //AND d8
                and_(memory_->read(z80_.PC++));
                break;
            case 0xE7: //RST 20H
                rst(0x20);
                break;
            case 0xE8: //ADD SP,r8
                add_sp_r8(memory_->read(z80_.PC++));
                break;
            case 0xE9: //JP (HL) -> c'est une feinte l'expression devrait être marqué JP HL
                jp((z80_.H << 8) + z80_.L);
                break;
            case 0xEA: //LD (a16),A
                ld_at((memory_->read(z80_.PC + 1) << 8) + memory_->read(z80_.PC));
                z80_.PC += 2;
                break;
            case 0xEE: //XOR d8
                xor_(memory_->read(z80_.PC++));
                break;
            case 0xEF: //RST 28H
                rst(0x28);
                break;
            case 0xF0: //LDH A,(a8)
                ld_reg(&z80_.A, memory_->read(0xFF00 + memory_->read(z80_.PC++)));
                break;
            case 0xF1: //POP AF
                pop(&z80_.A, &z80_.F);
                z80_.F &= 0xF0;
                break;
            case 0xF2: //LD A,(C)
                ld_reg(&z80_.A, memory_->read(0xFF00 + z80_.C));
                break;
            case 0xF3: //DI
                di();
                break;

            case 0xF5: //PUSH AF
                push(z80_.A, z80_.F);
                break;
            case 0xF6: //OR d8
                or_(memory_->read(z80_.PC++));
                break;
            case 0xF7: //RST 30H
                rst(0x30);
                break;
            case 0xF8: //LD HL,SP+r8
                ld_hl_sp_p_r8();
                break;
            case 0xF9: //LD SP,HL
                ld_sp((z80_.H << 8) + z80_.L);
                break;
            case 0xFA: //LD A,(a16)
                ld_reg(&z80_.A, memory_->read((memory_->read(z80_.PC + 1) << 8) + memory_->read(z80_.PC)));
                z80_.PC +=2;
                break;
            case 0xFB: //EI
                ei();
                break;
            case 0xFE: //CP d8
                cp(memory_->read(z80_.PC++));
                break;

            case 0xFF: //RST 38H
                rst(0x38);
                break;
        }
    }

    handle_interrupts(&z80_);

    if(ime_counter_ > 0){
        ime_counter_ -= cycles_;
        if(ime_counter_ <= 0){
            set_IME(true);
        }
    }

    return cycles_;
}

//Interrupt handling

void Cpu::set_IME(bool value){
    IME_ = value;
}

void Cpu::handle_interrupts(Cpu::z80_t *z80){
    BYTE IE;
    BYTE IF;

    if(IME_){
        IE = memory_->read(0xFFFF);
        IF = memory_->read(0xFF0F);
        if(IE & IF & 0x01){
            memory_->write(0xFF0F, IF & 0xFE);
            execute_interrupt(V_BLANK, z80);
        }
        else if(IE & IF & 0x02){
            memory_->write(0xFF0F, IF & 0xFD);
            execute_interrupt(LCD_STAT, z80);
        }
        else if (IE & IF & 0x04){
            memory_->write(0xFF0F, IF & 0xFB);
            execute_interrupt(TIMER, z80);
        }
        else if(IE & IF & 0x08){
            memory_->write(0xFF0F, IF & 0xF7);
            execute_interrupt(SERIAL, z80);
        }
        else if(IE & IF & 0x10){
            memory_->write(0xFF0F, IF & 0xEF);
            execute_interrupt(JOYPAD, z80);
        }
    }
    else if(halted_){
        IE = memory_->read(0xFFFF);
        IF = memory_->read(0xFF0F);
        if(IE & IF & 0x1F){
            reset_halt();
        }
    }
}

void Cpu::execute_interrupt(BYTE type, Cpu::z80_t *z80){
    set_IME(false);
    reset_halt();
    memory_->write(z80->SP - 1, (z80->PC & 0xFF00) >> 8);
    memory_->write(z80->SP - 2, (z80->PC & 0x00FF));
    z80->SP -= 2;

    switch(type){
        case V_BLANK:
            z80->PC = 0x40;
            break;
        case LCD_STAT:
            z80->PC = 0x48;
            break;
        case TIMER:
            z80->PC = 0x50;
            break;
        case SERIAL:
            z80->PC = 0x58;
            break;
        case JOYPAD:
            z80->PC = 0x60;
            break;
    }
}

void Cpu::make_request(int type){
    switch(type){
        case V_BLANK:
            memory_->write(0xFF0F, memory_->read(0xFF0F) | 0x01);
            break;
        case LCD_STAT:
            memory_->write(0xFF0F, memory_->read(0xFF0F) | 0x02);
            break;
        case TIMER:
            memory_->write(0xFF0F, memory_->read(0xFF0F) | 0x04);
            break;
        case SERIAL:
            memory_->write(0xFF0F, memory_->read(0xFF0F) | 0x08);
            break;
        case JOYPAD:
            memory_->write(0xFF0F, memory_->read(0xFF0F) | 0x10);
            break;
    }
}

void Cpu::reset_halt(){
    halted_ = 0;
}

//-----------Misc/control instructions------------------

 void Cpu::di(){
    set_IME(false);
    ime_counter_ = 0;
}
 void Cpu::ei(){
    set_IME(true);
    ime_counter_ = z80_cycles[0xFB] * 4;
}
 void Cpu::halt(){
    if( ime_counter_ > 0){
        set_IME(true);
        ime_counter_ = 0;
        z80_.PC--;
    }
    else{
        halted_ = 1;
    }

}
 void Cpu::stop(){
    printf("appel de stop\n");
}
//----------Jumps/calls--------------------------------

 void Cpu::call(){
    memory_->write(z80_.SP - 1, ((z80_.PC + 2) & 0xFF00) >> 8);
    memory_->write(z80_.SP - 2, (z80_.PC + 2) & 0x00FF);
    z80_.SP -= 2;
    z80_.PC = (memory_->read(z80_.PC + 1) << 8) + memory_->read(z80_.PC);
}

 void Cpu::call_cond(BYTE cond){
    if(z80_.F & cond){
        memory_->write(z80_.SP - 1, ((z80_.PC + 2) & 0xFF00) >> 8);
        memory_->write(z80_.SP - 2, (z80_.PC + 2) & 0x00FF);
        z80_.SP -= 2;
        z80_.PC = (memory_->read(z80_.PC + 1) << 8) + memory_->read(z80_.PC);
        cycles_ += 12;
    }
    else z80_.PC += 2;
}

 void Cpu::call_n_cond(BYTE cond){
    if(!(z80_.F & cond)){
        memory_->write(z80_.SP - 1, ((z80_.PC + 2) & 0xFF00) >> 8);
        memory_->write(z80_.SP - 2, (z80_.PC + 2) & 0xFF);
        z80_.SP -= 2;
        z80_.PC = (memory_->read(z80_.PC + 1) << 8) + memory_->read(z80_.PC);
        cycles_ += 12;
    }
    else z80_.PC += 2;
}

 void Cpu::jp(unsigned short addr){
    z80_.PC = addr;
}

 void Cpu::jp_cond(BYTE cond){
    if(z80_.F & cond){
        z80_.PC = (memory_->read(z80_.PC + 1) << 8) + memory_->read(z80_.PC);
        cycles_ += 4;
    }
    else z80_.PC += 2;
}

 void Cpu::jp_n_cond(BYTE cond){
    if((z80_.F & cond) == 0){
        z80_.PC = (memory_->read(z80_.PC + 1) << 8) + memory_->read(z80_.PC);
        cycles_ += 4;
    }
    else z80_.PC += 2;
}

 void Cpu::jr(BYTE_S d){
    // - - - -
    //Jump Relatif
    z80_.PC += d;
}

 void Cpu::jr_cond(BYTE cond, BYTE_S d){
    // - - - -
    if(z80_.F & cond){
        z80_.PC += d;
        cycles_ += 4;
    }
}

 void Cpu::jr_n_cond(BYTE cond, BYTE_S d){
    // - - - -
    if(!(z80_.F & cond)){
        z80_.PC += d;
        cycles_ += 4;
    }
}

 void Cpu::ret(){
    z80_.PC = (memory_->read(z80_.SP + 1) << 8) + memory_->read(z80_.SP);
    z80_.SP += 2;
}

 void Cpu::ret_cond(BYTE cond){
    if(z80_.F & cond){
        z80_.PC = (memory_->read(z80_.SP + 1) << 8) + memory_->read(z80_.SP);
        z80_.SP += 2;
        cycles_ += 12;
    }
}

 void Cpu::ret_n_cond(BYTE cond){
    if(!(z80_.F & cond)){
        z80_.PC = (memory_->read(z80_.SP + 1) << 8) + memory_->read(z80_.SP);;
        z80_.SP += 2;
        cycles_ += 12;
    }
}

 void Cpu::reti(){
    //PLUS FLAG IF
    z80_.PC = (memory_->read(z80_.SP + 1) << 8) + memory_->read(z80_.SP);
    z80_.SP += 2;
    set_IME(true);
}

 void Cpu::rst(BYTE addr){
    memory_->write(z80_.SP - 1, (z80_.PC & 0xFF00) >> 8);
    memory_->write(z80_.SP - 2, z80_.PC & 0xFF);
    z80_.PC = addr;
    z80_.SP -= 2;
}

//--------8bit load/store/move instructions

 void Cpu::ld_reg(BYTE *reg, BYTE data){
    *reg = data;
}
 void Cpu::ld_mem(unsigned short addr, BYTE data){
    memory_->write(addr, data);

}
//--------16bit load/store/move instructions

 void Cpu::ld_a16_sp(){
    unsigned short addr = (memory_->read(z80_.PC + 1) << 8) + memory_->read(z80_.PC);
    memory_->write(addr, (z80_.SP & 0x00FF));
    memory_->write(addr + 1, (z80_.SP & 0xFF00) >> 8);
    z80_.PC += 2;
}

 void Cpu::ld_at(unsigned short addr){
    memory_->write(addr, z80_.A);
}

 void Cpu::ld_from_a8(unsigned short addr){
    z80_.A = memory_->read(addr);
}

 void Cpu::ld_hl_sp_p_r8(){
    BYTE_S r8 = memory_->read(z80_.PC);
    unsigned short op = z80_.SP + r8;
    z80_.F = ((((z80_.SP^r8^op) & 0x100) == 0x100) ? FLAG_C:0) | ((((z80_.SP^r8^op) & 0x10) == 0x10) ? FLAG_H:0);
    z80_.H = (op & 0xFF00) >> 8 ;
    z80_.L = (op & 0xFF);
    z80_.PC++;
}

 void Cpu::ld_sp(unsigned short data){
    z80_.SP = data;
}

 void Cpu::pop(BYTE *reg1, BYTE *reg2){
    *reg1 = memory_->read(z80_.SP + 1);
    *reg2 = memory_->read(z80_.SP);
    z80_.SP += 2;
}

 void Cpu::push(BYTE reg1, BYTE reg2){
    memory_->write(z80_.SP - 1, reg1);
    memory_->write(z80_.SP - 2, reg2);
    z80_.SP -= 2;
}

//--------8bit arithmetic/logical instructions


 void Cpu::adc(BYTE data){
    //Z 0 H C
    BYTE op = z80_.A + data + ((z80_.F & FLAG_C)? 1:0);
    z80_.F = ((op == 0) ? FLAG_Z:0) | (((z80_.A^data^op) & 0x10) ? FLAG_H:0) | ((z80_.A + data + ((z80_.F & FLAG_C)? 1:0) > 0xFF) ? FLAG_C:0);
    z80_.A = op;
}

 void Cpu::add(BYTE data){
    //Z 0 H C
    BYTE op = z80_.A + data;
    z80_.F = ((op == 0) ? FLAG_Z:0) | (((z80_.A^data^op) & 0x10) ? FLAG_H:0) | ((z80_.A + data > 0xFF) ? FLAG_C:0);
    z80_.A = op;
}

 void Cpu::and_(BYTE data){
    z80_.A &= data;
    z80_.F = FLAG_H | ((z80_.A == 0) ? FLAG_Z:0);
}

 void Cpu::ccf(){
    // - 0 0 C
    //Inverse le Carry Flag
    //Clear le N flag et le H flag
    z80_.F = (z80_.F & FLAG_Z) | ((z80_.F & FLAG_C) ? 0:FLAG_C);
}

 void Cpu::cp(BYTE data){
    BYTE op = z80_.A - data;
    z80_.F = FLAG_N | ((op == 0) ? FLAG_Z:0) | (((z80_.A^data^op) & 0x10) ? FLAG_H:0) | ((z80_.A - data < 0) ? FLAG_C : 0);
}

 void Cpu::cpl(){
    // - 1 1 -
    //Inverse tous les bits de A
    //( == A XOR 0xFF)
    z80_.A = ~z80_.A;
    z80_.F |= 0x60; // H + N
}

 void Cpu::daa(){
    unsigned short temp = z80_.A;
    if(z80_.F & FLAG_C) {temp |= 256;}
    if(z80_.F & FLAG_H) {temp |= 512;}
    if(z80_.F & FLAG_N) {temp |= 1024;}
    if(z80_.F & FLAG_Z) {temp |= 2048;}
    temp = daa_table[temp];
    z80_.A = temp >> 8;
    z80_.F = temp;
}

 void Cpu::dec_at(unsigned short addr){
    // Z 1 H -
    BYTE d8 = memory_->read(addr) - 1;
    z80_.F = FLAG_N | (z80_.F & FLAG_C) | (((d8 & 0x0F) == 0x0F) ? FLAG_H:0) | ((d8 == 0) ? FLAG_Z:0);
    memory_->write(addr, d8);
}

 void Cpu::dec_sp(){
    // - - - -
    z80_.SP -= 1;
}

 void Cpu::dec_smpl(BYTE *reg1){
    // Z 1 H -
    BYTE d8 = *reg1 - 1;
    z80_.F = FLAG_N | (z80_.F & FLAG_C) | (((d8 & 0x0F) == 0x0F) ? FLAG_H:0) | ((d8 == 0) ? FLAG_Z:0);
    *reg1 = d8;
}

 void Cpu::inc_at(unsigned short addr){
    // Z 0 H -
    BYTE d8 = memory_->read(addr) + 1;
    z80_.F = (z80_.F & FLAG_C) | ((d8 & 0x0F) ? 0:FLAG_H) | ((d8 == 0) ? FLAG_Z:0);
    memory_->write(addr, d8);
}

 void Cpu::inc_smpl(BYTE *reg1){
    // Z 0 H -
    BYTE d8 = *reg1 + 1;
    z80_.F = (z80_.F & FLAG_C) | ((d8 & 0x0F) ? 0:FLAG_H) | ((d8 == 0) ? FLAG_Z:0);
    *reg1 = d8;
}

 void Cpu::inc_sp(){
    // - - - -
    z80_.SP += 1;
}
 void Cpu::sbc(BYTE data){
    BYTE op = z80_.A - data - ((z80_.F & FLAG_C)? 1:0);
    z80_.F = FLAG_N | ((op == 0) ? FLAG_Z:0) | (((z80_.A^data^op) & 0x10) ? FLAG_H:0) | ((z80_.A - data - ((z80_.F & FLAG_C) ? 1:0) < 0) ? FLAG_C : 0);
    z80_.A = op;
}

 void Cpu::scf(){
    // - 0 0 1
    z80_.F = (z80_.F & FLAG_Z) | FLAG_C;
}

 void Cpu::sub(BYTE data){
    //Z 1 H C
    BYTE op = z80_.A - data;
    z80_.F = FLAG_N | ((op == 0) ? FLAG_Z:0) | (((z80_.A^data^op) & 0x10) ? FLAG_H:0) | ((z80_.A - data < 0) ? FLAG_C : 0);
    z80_.A = op;
}

 void Cpu::or_(BYTE data){
    z80_.A |= data;
    z80_.F = (z80_.A == 0) ? FLAG_Z:0;
}

 void Cpu::xor_(BYTE data){
    z80_.A ^= (data);
    z80_.F = (z80_.A == 0) ? FLAG_Z:0;
}

//--------16bit arithmetic/logical instructions

 void Cpu::add_dbl(BYTE *reg1, BYTE *reg2, unsigned short data){
    // - 0 H C
    unsigned short r1r2 = (*reg1 << 8) + *reg2;
    unsigned short op = r1r2 + data;
    z80_.F = ((z80_.F & FLAG_Z) ? FLAG_Z:0) | (((r1r2^data^op) & 0x1000) ? FLAG_H:0) | (((r1r2+data)&0x10000)? FLAG_C:0);
    *reg1 =( 0xFF00 & op) >> 8;
    *reg2 =( 0x00FF & op);
}

 void Cpu::add_sp_r8(BYTE_S data){
    unsigned short op = z80_.SP + data;
    z80_.F = ((((z80_.SP^data^op) & 0x100) == 0x100) ? FLAG_C:0) | ((((z80_.SP^data^op) & 0x10) == 0x10) ? FLAG_H:0);
    z80_.SP = op;
}

 void Cpu::dec_dbl(BYTE *reg1, BYTE *reg2){
    // - - - -
    unsigned short temp;
    temp = (*reg1 << 8) + *reg2 - 0x01;
    *reg1 =( 0xFF00 & temp) >> 8;
    *reg2 =( 0x00FF & temp);
}
 void Cpu::inc_dbl(BYTE *reg1, BYTE *reg2){
    // - - - -
    unsigned short temp;
    temp = (*reg1 << 8) + *reg2 + 0x01;
    *reg1 =( 0xFF00 & temp) >> 8;
    *reg2 =( 0x00FF & temp);
}


//-----------8bit rotations/shifts and_ bit instructions

 void Cpu::bit(BYTE bit, BYTE data){
    z80_.F = (z80_.F&FLAG_C) | FLAG_H | ((data&bit) ? 0:FLAG_Z);
}

 void Cpu::res(BYTE b, BYTE *a)
{
    *a &= ~(1 << b);
}

 void Cpu::res_hl(BYTE b)
{
    BYTE hl = memory_->read((z80_.H << 8) + z80_.L);
    hl &= ~(1 << b);
    memory_->write((z80_.H << 8) + z80_.L, hl);
}

 void Cpu::rl(BYTE *data){
    if(*data & 0x80){
        *data = (*data << 1) | (((z80_.F & FLAG_C) ? 1:0));
        z80_.F = (((*data == 0) ? FLAG_Z:0)) | FLAG_C;
    }
    else{
        *data = (*data << 1) | (((z80_.F & FLAG_C) ? 1:0));
        z80_.F = (((*data == 0) ? FLAG_Z:0));
    }
}

 void Cpu::rl_hl(){
    BYTE hl = memory_->read((z80_.H << 8) + z80_.L);
    if(hl & 0x80){
        hl = (hl << 1) | (((z80_.F & FLAG_C) ? 1:0));
        z80_.F = (((hl == 0) ? FLAG_Z:0)) | FLAG_C;
    }
    else{
        hl = (hl << 1) | (((z80_.F & FLAG_C) ? 1:0));
        z80_.F = (((hl == 0) ? FLAG_Z:0));
    }
    memory_->write((z80_.H << 8) + z80_.L, hl);
}

 void Cpu::rla(){
    BYTE F = z80_.F;
    z80_.F = ((z80_.A & 0x80) ? FLAG_C:0);
    z80_.A = (z80_.A << 1) | ((F & FLAG_C)? 1:0);
}

 void Cpu::rlc(BYTE *data){
    z80_.F = ((*data & 0x80) ? FLAG_C:0);
    *data = (*data << 1) | ((z80_.F) ? 1:0);
    z80_.F |= ((*data == 0)? FLAG_Z:0);
}

 void Cpu::rlc_hl(){
    BYTE hl = memory_->read((z80_.H << 8) + z80_.L);

    z80_.F = ((hl & 0x80) ? FLAG_C:0);
    hl = (hl << 1) | ((z80_.F) ? 1:0);
    z80_.F |= ((hl == 0)? FLAG_Z:0);

    memory_->write((z80_.H << 8) + z80_.L, hl);
}
 void Cpu::rlca(){
    z80_.F = ((z80_.A & 0x80) ? FLAG_C:0);
    z80_.A = (z80_.A << 1) | ((z80_.F)? 1 : 0);
}
 void Cpu::rr(BYTE *data){
    if(*data & 0x01){
        *data = (*data >> 1) | (((z80_.F & FLAG_C) ? 0x80:0));
        z80_.F = (((*data == 0) ? FLAG_Z:0)) | FLAG_C;
    }
    else{
        *data = (*data >> 1) | (((z80_.F & FLAG_C) ? 0x80:0));
        z80_.F = (((*data == 0) ? FLAG_Z:0));
    }
}

 void Cpu::rr_hl(){
    BYTE hl = memory_->read((z80_.H << 8) + z80_.L);
    if(hl & 0x01){
        hl = (hl >> 1) | (((z80_.F & FLAG_C) ? 0x80:0));
        z80_.F = (((hl == 0) ? FLAG_Z:0)) | FLAG_C;
    }
    else{
        hl = (hl >> 1) | (((z80_.F & FLAG_C) ? 0x80:0));
        z80_.F = (((hl == 0) ? FLAG_Z:0));
    }
    memory_->write((z80_.H << 8) + z80_.L, hl);
}

 void Cpu::rra(){
    BYTE F = z80_.F;
    z80_.F = ((z80_.A & 0x01) ? FLAG_C:0);
    z80_.A = (z80_.A >> 1) | ((F & FLAG_C) ? 0x80:0);
}

 void Cpu::rrc(BYTE *data){
    z80_.F = ((*data & 0x01) ? FLAG_C:0);
    *data = (*data >> 1) | ((z80_.F) ? 0x80:0);
    z80_.F |= ((*data == 0)? FLAG_Z:0);
}

 void Cpu::rrc_hl(){
    BYTE hl = memory_->read((z80_.H << 8) + z80_.L);

    z80_.F = ((hl & 0x01) ? FLAG_C:0);
    hl = (hl >> 1) | ((z80_.F) ? 0x80:0);
    z80_.F |= ((hl == 0)? FLAG_Z:0);

    memory_->write((z80_.H << 8) + z80_.L, hl);
}


 void Cpu::rrca(){
    z80_.F = ((z80_.A & 0x01) ? FLAG_C: 0);
    z80_.A = (z80_.A >> 1) | (z80_.F ? 0x80:0);
}

 void Cpu::set(BYTE b, BYTE *a)
{
    *a |= (1 << b);
}

 void Cpu::set_hl(BYTE b)
{
    BYTE hl = memory_->read((z80_.H << 8) + z80_.L);
    hl |= (1 << b);
    memory_->write((z80_.H << 8) + z80_.L, hl);
}

 void Cpu::srl(BYTE *data){
    z80_.F = (((*data & 0x01) ? FLAG_C:0));
    *data >>= 1;
    z80_.F |= ((*data == 0) ? FLAG_Z:0);
}

 void Cpu::srl_hl(){
    BYTE hl = memory_->read((z80_.H << 8) + z80_.L);
    z80_.F = (((hl & 0x01) ? FLAG_C:0));
    hl >>= 1;
    z80_.F |= ((hl == 0) ? FLAG_Z:0);
    memory_->write((z80_.H << 8) + z80_.L, hl);
}

 void Cpu::sla(BYTE *data){
    z80_.F = ((*data & 0x80) ? FLAG_C:0);
    *data <<= 1;
    z80_.F |= ((*data == 0) ? FLAG_Z:0);
}

 void Cpu::sla_hl(){
    BYTE hl = memory_->read((z80_.H << 8) + z80_.L);
    z80_.F = ((hl & 0x80) ? FLAG_C:0);
    hl <<= 1;
    z80_.F |= ((hl == 0) ? FLAG_Z:0);
    memory_->write((z80_.H << 8) + z80_.L, hl);
}

 void Cpu::sra(BYTE *data){
    z80_.F = ((*data & 0x01) ? FLAG_C:0);
    *data = (*data >> 1) | ((*data & 0x80) ? 0x80:0);
    z80_.F |= ((*data == 0) ? FLAG_Z:0);
}

 void Cpu::sra_hl(){
    BYTE hl = memory_->read((z80_.H << 8) +z80_.L);
    z80_.F = ((hl & 0x01) ? FLAG_C:0);
    hl = (hl >> 1) | ((hl & 0x80) ? 0x80:0);
    z80_.F |= ((hl == 0) ? FLAG_Z:0);
    memory_->write((z80_.H << 8) + z80_.L, hl);
}

 void Cpu::swap(BYTE *data){
    BYTE low_nibble = (*data & 0xF0) >> 4;
    BYTE high_nibble = (*data & 0x0F) << 4;
    *data = high_nibble + low_nibble;
    z80_.F = ((*data == 0) ? FLAG_Z:0);
}

 void Cpu::swap_hl(){
    BYTE hl = memory_->read((z80_.H << 8) + z80_.L);

    BYTE low_nibble = (hl & 0xF0) >> 4;
    BYTE high_nibble = (hl & 0x0F) << 4;
    hl = high_nibble + low_nibble;
    z80_.F = ((hl == 0) ? FLAG_Z:0);

    memory_->write((z80_.H << 8) + z80_.L, hl);
}


/*int save_cpu(FILE* file)
{
    int nb = 0;
    int nb_elements =14;
    nb += fwrite(&cycles_,sizeof(int),1,file);
        nb += fwrite(&ime_counter,sizeof(int),1,file);
        nb += fwrite(&halted,sizeof(uint8_t),1,file);
        nb += fwrite(&skip,sizeof(uint8_t),1,file);
        nb += fwrite(&z80_.PC,sizeof(unsigned short),1,file);
        nb += fwrite(&z80_.SP,sizeof(unsigned short),1,file);
        nb += fwrite(&z80_.A,sizeof(unsigned char),1,file);
        nb += fwrite(&z80_.F,sizeof(unsigned char),1,file);
        nb += fwrite(&z80_.B,sizeof(unsigned char),1,file);
        nb += fwrite(&z80_.C,sizeof(unsigned char),1,file);
        nb += fwrite(&z80_.D,sizeof(unsigned char),1,file);
        nb += fwrite(&z80_.E,sizeof(unsigned char),1,file);
        nb += fwrite(&z80_.H,sizeof(unsigned char),1,file);
        nb += fwrite(&z80_.L,sizeof(unsigned char),1,file);
    if(nb!=nb_elements) printf("Error when writing cpu variables\n");
    return nb;
}

void Cpu::restore_cpu(FILE* file)
{
    int nb = 0;
    int nb_elements = 14;
    nb += fread(&cycles_,sizeof(int),1,file);
    nb += fread(&ime_counter,sizeof(int),1,file);
    nb += fread(&halted,sizeof(uint8_t),1,file);
    nb += fread(&skip,sizeof(uint8_t),1,file);
    nb += fread(&z80_.PC,sizeof(unsigned short),1,file);
    nb += fread(&z80_.SP,sizeof(unsigned short),1,file);
    nb += fread(&z80_.A,sizeof(unsigned char),1,file);
    nb += fread(&z80_.F,sizeof(unsigned char),1,file);
    nb += fread(&z80_.B,sizeof(unsigned char),1,file);
    nb += fread(&z80_.C,sizeof(unsigned char),1,file);
    nb += fread(&z80_.D,sizeof(unsigned char),1,file);
    nb += fread(&z80_.E,sizeof(unsigned char),1,file);
    nb += fread(&z80_.H,sizeof(unsigned char),1,file);
    nb += fread(&z80_.L,sizeof(unsigned char),1,file);
    if(nb!=nb_elements) printf("Error when reading cpu variables\n");
}*/

const uint16_t daa_table[4096] =
{

0x80,0x100,0x200,0x300,0x400,0x500,0x600,0x700,
0x800,0x900,0x1000,0x1100,0x1200,0x1300,0x1400,0x1500,
0x1000,0x1100,0x1200,0x1300,0x1400,0x1500,0x1600,0x1700,
0x1800,0x1900,0x2000,0x2100,0x2200,0x2300,0x2400,0x2500,
0x2000,0x2100,0x2200,0x2300,0x2400,0x2500,0x2600,0x2700,
0x2800,0x2900,0x3000,0x3100,0x3200,0x3300,0x3400,0x3500,
0x3000,0x3100,0x3200,0x3300,0x3400,0x3500,0x3600,0x3700,
0x3800,0x3900,0x4000,0x4100,0x4200,0x4300,0x4400,0x4500,
0x4000,0x4100,0x4200,0x4300,0x4400,0x4500,0x4600,0x4700,
0x4800,0x4900,0x5000,0x5100,0x5200,0x5300,0x5400,0x5500,
0x5000,0x5100,0x5200,0x5300,0x5400,0x5500,0x5600,0x5700,
0x5800,0x5900,0x6000,0x6100,0x6200,0x6300,0x6400,0x6500,
0x6000,0x6100,0x6200,0x6300,0x6400,0x6500,0x6600,0x6700,
0x6800,0x6900,0x7000,0x7100,0x7200,0x7300,0x7400,0x7500,
0x7000,0x7100,0x7200,0x7300,0x7400,0x7500,0x7600,0x7700,
0x7800,0x7900,0x8000,0x8100,0x8200,0x8300,0x8400,0x8500,
0x8000,0x8100,0x8200,0x8300,0x8400,0x8500,0x8600,0x8700,
0x8800,0x8900,0x9000,0x9100,0x9200,0x9300,0x9400,0x9500,
0x9000,0x9100,0x9200,0x9300,0x9400,0x9500,0x9600,0x9700,
0x9800,0x9900,0x90,0x110,0x210,0x310,0x410,0x510,
0x90,0x110,0x210,0x310,0x410,0x510,0x610,0x710,
0x810,0x910,0x1010,0x1110,0x1210,0x1310,0x1410,0x1510,
0x1010,0x1110,0x1210,0x1310,0x1410,0x1510,0x1610,0x1710,
0x1810,0x1910,0x2010,0x2110,0x2210,0x2310,0x2410,0x2510,
0x2010,0x2110,0x2210,0x2310,0x2410,0x2510,0x2610,0x2710,
0x2810,0x2910,0x3010,0x3110,0x3210,0x3310,0x3410,0x3510,
0x3010,0x3110,0x3210,0x3310,0x3410,0x3510,0x3610,0x3710,
0x3810,0x3910,0x4010,0x4110,0x4210,0x4310,0x4410,0x4510,
0x4010,0x4110,0x4210,0x4310,0x4410,0x4510,0x4610,0x4710,
0x4810,0x4910,0x5010,0x5110,0x5210,0x5310,0x5410,0x5510,
0x5010,0x5110,0x5210,0x5310,0x5410,0x5510,0x5610,0x5710,
0x5810,0x5910,0x6010,0x6110,0x6210,0x6310,0x6410,0x6510,
0x6010,0x6110,0x6210,0x6310,0x6410,0x6510,0x6610,0x6710,
0x6810,0x6910,0x7010,0x7110,0x7210,0x7310,0x7410,0x7510,
0x7010,0x7110,0x7210,0x7310,0x7410,0x7510,0x7610,0x7710,
0x7810,0x7910,0x8010,0x8110,0x8210,0x8310,0x8410,0x8510,
0x8010,0x8110,0x8210,0x8310,0x8410,0x8510,0x8610,0x8710,
0x8810,0x8910,0x9010,0x9110,0x9210,0x9310,0x9410,0x9510,
0x9010,0x9110,0x9210,0x9310,0x9410,0x9510,0x9610,0x9710,
0x9810,0x9910,0xa010,0xa110,0xa210,0xa310,0xa410,0xa510,
0xa010,0xa110,0xa210,0xa310,0xa410,0xa510,0xa610,0xa710,
0xa810,0xa910,0xb010,0xb110,0xb210,0xb310,0xb410,0xb510,
0xb010,0xb110,0xb210,0xb310,0xb410,0xb510,0xb610,0xb710,
0xb810,0xb910,0xc010,0xc110,0xc210,0xc310,0xc410,0xc510,
0xc010,0xc110,0xc210,0xc310,0xc410,0xc510,0xc610,0xc710,
0xc810,0xc910,0xd010,0xd110,0xd210,0xd310,0xd410,0xd510,
0xd010,0xd110,0xd210,0xd310,0xd410,0xd510,0xd610,0xd710,
0xd810,0xd910,0xe010,0xe110,0xe210,0xe310,0xe410,0xe510,
0xe010,0xe110,0xe210,0xe310,0xe410,0xe510,0xe610,0xe710,
0xe810,0xe910,0xf010,0xf110,0xf210,0xf310,0xf410,0xf510,
0xf010,0xf110,0xf210,0xf310,0xf410,0xf510,0xf610,0xf710,
0xf810,0xf910,0x90,0x110,0x210,0x310,0x410,0x510,
0x90,0x110,0x210,0x310,0x410,0x510,0x610,0x710,
0x810,0x910,0x1010,0x1110,0x1210,0x1310,0x1410,0x1510,
0x1010,0x1110,0x1210,0x1310,0x1410,0x1510,0x1610,0x1710,
0x1810,0x1910,0x2010,0x2110,0x2210,0x2310,0x2410,0x2510,
0x2010,0x2110,0x2210,0x2310,0x2410,0x2510,0x2610,0x2710,
0x2810,0x2910,0x3010,0x3110,0x3210,0x3310,0x3410,0x3510,
0x3010,0x3110,0x3210,0x3310,0x3410,0x3510,0x3610,0x3710,
0x3810,0x3910,0x4010,0x4110,0x4210,0x4310,0x4410,0x4510,
0x4010,0x4110,0x4210,0x4310,0x4410,0x4510,0x4610,0x4710,
0x4810,0x4910,0x5010,0x5110,0x5210,0x5310,0x5410,0x5510,
0x5010,0x5110,0x5210,0x5310,0x5410,0x5510,0x5610,0x5710,
0x5810,0x5910,0x6010,0x6110,0x6210,0x6310,0x6410,0x6510,
0x600,0x700,0x800,0x900,0xa00,0xb00,0xc00,0xd00,
0xe00,0xf00,0x1000,0x1100,0x1200,0x1300,0x1400,0x1500,
0x1600,0x1700,0x1800,0x1900,0x1a00,0x1b00,0x1c00,0x1d00,
0x1e00,0x1f00,0x2000,0x2100,0x2200,0x2300,0x2400,0x2500,
0x2600,0x2700,0x2800,0x2900,0x2a00,0x2b00,0x2c00,0x2d00,
0x2e00,0x2f00,0x3000,0x3100,0x3200,0x3300,0x3400,0x3500,
0x3600,0x3700,0x3800,0x3900,0x3a00,0x3b00,0x3c00,0x3d00,
0x3e00,0x3f00,0x4000,0x4100,0x4200,0x4300,0x4400,0x4500,
0x4600,0x4700,0x4800,0x4900,0x4a00,0x4b00,0x4c00,0x4d00,
0x4e00,0x4f00,0x5000,0x5100,0x5200,0x5300,0x5400,0x5500,
0x5600,0x5700,0x5800,0x5900,0x5a00,0x5b00,0x5c00,0x5d00,
0x5e00,0x5f00,0x6000,0x6100,0x6200,0x6300,0x6400,0x6500,
0x6600,0x6700,0x6800,0x6900,0x6a00,0x6b00,0x6c00,0x6d00,
0x6e00,0x6f00,0x7000,0x7100,0x7200,0x7300,0x7400,0x7500,
0x7600,0x7700,0x7800,0x7900,0x7a00,0x7b00,0x7c00,0x7d00,
0x7e00,0x7f00,0x8000,0x8100,0x8200,0x8300,0x8400,0x8500,
0x8600,0x8700,0x8800,0x8900,0x8a00,0x8b00,0x8c00,0x8d00,
0x8e00,0x8f00,0x9000,0x9100,0x9200,0x9300,0x9400,0x9500,
0x9600,0x9700,0x9800,0x9900,0x9a00,0x9b00,0x9c00,0x9d00,
0x9e00,0x9f00,0x90,0x110,0x210,0x310,0x410,0x510,
0x610,0x710,0x810,0x910,0xa10,0xb10,0xc10,0xd10,
0xe10,0xf10,0x1010,0x1110,0x1210,0x1310,0x1410,0x1510,
0x1610,0x1710,0x1810,0x1910,0x1a10,0x1b10,0x1c10,0x1d10,
0x1e10,0x1f10,0x2010,0x2110,0x2210,0x2310,0x2410,0x2510,
0x2610,0x2710,0x2810,0x2910,0x2a10,0x2b10,0x2c10,0x2d10,
0x2e10,0x2f10,0x3010,0x3110,0x3210,0x3310,0x3410,0x3510,
0x3610,0x3710,0x3810,0x3910,0x3a10,0x3b10,0x3c10,0x3d10,
0x3e10,0x3f10,0x4010,0x4110,0x4210,0x4310,0x4410,0x4510,
0x4610,0x4710,0x4810,0x4910,0x4a10,0x4b10,0x4c10,0x4d10,
0x4e10,0x4f10,0x5010,0x5110,0x5210,0x5310,0x5410,0x5510,
0x5610,0x5710,0x5810,0x5910,0x5a10,0x5b10,0x5c10,0x5d10,
0x5e10,0x5f10,0x6010,0x6110,0x6210,0x6310,0x6410,0x6510,
0x6610,0x6710,0x6810,0x6910,0x6a10,0x6b10,0x6c10,0x6d10,
0x6e10,0x6f10,0x7010,0x7110,0x7210,0x7310,0x7410,0x7510,
0x7610,0x7710,0x7810,0x7910,0x7a10,0x7b10,0x7c10,0x7d10,
0x7e10,0x7f10,0x8010,0x8110,0x8210,0x8310,0x8410,0x8510,
0x8610,0x8710,0x8810,0x8910,0x8a10,0x8b10,0x8c10,0x8d10,
0x8e10,0x8f10,0x9010,0x9110,0x9210,0x9310,0x9410,0x9510,
0x9610,0x9710,0x9810,0x9910,0x9a10,0x9b10,0x9c10,0x9d10,
0x9e10,0x9f10,0xa010,0xa110,0xa210,0xa310,0xa410,0xa510,
0xa610,0xa710,0xa810,0xa910,0xaa10,0xab10,0xac10,0xad10,
0xae10,0xaf10,0xb010,0xb110,0xb210,0xb310,0xb410,0xb510,
0xb610,0xb710,0xb810,0xb910,0xba10,0xbb10,0xbc10,0xbd10,
0xbe10,0xbf10,0xc010,0xc110,0xc210,0xc310,0xc410,0xc510,
0xc610,0xc710,0xc810,0xc910,0xca10,0xcb10,0xcc10,0xcd10,
0xce10,0xcf10,0xd010,0xd110,0xd210,0xd310,0xd410,0xd510,
0xd610,0xd710,0xd810,0xd910,0xda10,0xdb10,0xdc10,0xdd10,
0xde10,0xdf10,0xe010,0xe110,0xe210,0xe310,0xe410,0xe510,
0xe610,0xe710,0xe810,0xe910,0xea10,0xeb10,0xec10,0xed10,
0xee10,0xef10,0xf010,0xf110,0xf210,0xf310,0xf410,0xf510,
0xf610,0xf710,0xf810,0xf910,0xfa10,0xfb10,0xfc10,0xfd10,
0xfe10,0xff10,0x90,0x110,0x210,0x310,0x410,0x510,
0x610,0x710,0x810,0x910,0xa10,0xb10,0xc10,0xd10,
0xe10,0xf10,0x1010,0x1110,0x1210,0x1310,0x1410,0x1510,
0x1610,0x1710,0x1810,0x1910,0x1a10,0x1b10,0x1c10,0x1d10,
0x1e10,0x1f10,0x2010,0x2110,0x2210,0x2310,0x2410,0x2510,
0x2610,0x2710,0x2810,0x2910,0x2a10,0x2b10,0x2c10,0x2d10,
0x2e10,0x2f10,0x3010,0x3110,0x3210,0x3310,0x3410,0x3510,
0x3610,0x3710,0x3810,0x3910,0x3a10,0x3b10,0x3c10,0x3d10,
0x3e10,0x3f10,0x4010,0x4110,0x4210,0x4310,0x4410,0x4510,
0x4610,0x4710,0x4810,0x4910,0x4a10,0x4b10,0x4c10,0x4d10,
0x4e10,0x4f10,0x5010,0x5110,0x5210,0x5310,0x5410,0x5510,
0x5610,0x5710,0x5810,0x5910,0x5a10,0x5b10,0x5c10,0x5d10,
0x5e10,0x5f10,0x6010,0x6110,0x6210,0x6310,0x6410,0x6510,
0xc0,0x140,0x240,0x340,0x440,0x540,0x640,0x740,
0x840,0x940,0xa40,0xb40,0xc40,0xd40,0xe40,0xf40,
0x1040,0x1140,0x1240,0x1340,0x1440,0x1540,0x1640,0x1740,
0x1840,0x1940,0x1a40,0x1b40,0x1c40,0x1d40,0x1e40,0x1f40,
0x2040,0x2140,0x2240,0x2340,0x2440,0x2540,0x2640,0x2740,
0x2840,0x2940,0x2a40,0x2b40,0x2c40,0x2d40,0x2e40,0x2f40,
0x3040,0x3140,0x3240,0x3340,0x3440,0x3540,0x3640,0x3740,
0x3840,0x3940,0x3a40,0x3b40,0x3c40,0x3d40,0x3e40,0x3f40,
0x4040,0x4140,0x4240,0x4340,0x4440,0x4540,0x4640,0x4740,
0x4840,0x4940,0x4a40,0x4b40,0x4c40,0x4d40,0x4e40,0x4f40,
0x5040,0x5140,0x5240,0x5340,0x5440,0x5540,0x5640,0x5740,
0x5840,0x5940,0x5a40,0x5b40,0x5c40,0x5d40,0x5e40,0x5f40,
0x6040,0x6140,0x6240,0x6340,0x6440,0x6540,0x6640,0x6740,
0x6840,0x6940,0x6a40,0x6b40,0x6c40,0x6d40,0x6e40,0x6f40,
0x7040,0x7140,0x7240,0x7340,0x7440,0x7540,0x7640,0x7740,
0x7840,0x7940,0x7a40,0x7b40,0x7c40,0x7d40,0x7e40,0x7f40,
0x8040,0x8140,0x8240,0x8340,0x8440,0x8540,0x8640,0x8740,
0x8840,0x8940,0x8a40,0x8b40,0x8c40,0x8d40,0x8e40,0x8f40,
0x9040,0x9140,0x9240,0x9340,0x9440,0x9540,0x9640,0x9740,
0x9840,0x9940,0x9a40,0x9b40,0x9c40,0x9d40,0x9e40,0x9f40,
0xa040,0xa140,0xa240,0xa340,0xa440,0xa540,0xa640,0xa740,
0xa840,0xa940,0xaa40,0xab40,0xac40,0xad40,0xae40,0xaf40,
0xb040,0xb140,0xb240,0xb340,0xb440,0xb540,0xb640,0xb740,
0xb840,0xb940,0xba40,0xbb40,0xbc40,0xbd40,0xbe40,0xbf40,
0xc040,0xc140,0xc240,0xc340,0xc440,0xc540,0xc640,0xc740,
0xc840,0xc940,0xca40,0xcb40,0xcc40,0xcd40,0xce40,0xcf40,
0xd040,0xd140,0xd240,0xd340,0xd440,0xd540,0xd640,0xd740,
0xd840,0xd940,0xda40,0xdb40,0xdc40,0xdd40,0xde40,0xdf40,
0xe040,0xe140,0xe240,0xe340,0xe440,0xe540,0xe640,0xe740,
0xe840,0xe940,0xea40,0xeb40,0xec40,0xed40,0xee40,0xef40,
0xf040,0xf140,0xf240,0xf340,0xf440,0xf540,0xf640,0xf740,
0xf840,0xf940,0xfa40,0xfb40,0xfc40,0xfd40,0xfe40,0xff40,
0xa050,0xa150,0xa250,0xa350,0xa450,0xa550,0xa650,0xa750,
0xa850,0xa950,0xaa50,0xab50,0xac50,0xad50,0xae50,0xaf50,
0xb050,0xb150,0xb250,0xb350,0xb450,0xb550,0xb650,0xb750,
0xb850,0xb950,0xba50,0xbb50,0xbc50,0xbd50,0xbe50,0xbf50,
0xc050,0xc150,0xc250,0xc350,0xc450,0xc550,0xc650,0xc750,
0xc850,0xc950,0xca50,0xcb50,0xcc50,0xcd50,0xce50,0xcf50,
0xd050,0xd150,0xd250,0xd350,0xd450,0xd550,0xd650,0xd750,
0xd850,0xd950,0xda50,0xdb50,0xdc50,0xdd50,0xde50,0xdf50,
0xe050,0xe150,0xe250,0xe350,0xe450,0xe550,0xe650,0xe750,
0xe850,0xe950,0xea50,0xeb50,0xec50,0xed50,0xee50,0xef50,
0xf050,0xf150,0xf250,0xf350,0xf450,0xf550,0xf650,0xf750,
0xf850,0xf950,0xfa50,0xfb50,0xfc50,0xfd50,0xfe50,0xff50,
0xd0,0x150,0x250,0x350,0x450,0x550,0x650,0x750,
0x850,0x950,0xa50,0xb50,0xc50,0xd50,0xe50,0xf50,
0x1050,0x1150,0x1250,0x1350,0x1450,0x1550,0x1650,0x1750,
0x1850,0x1950,0x1a50,0x1b50,0x1c50,0x1d50,0x1e50,0x1f50,
0x2050,0x2150,0x2250,0x2350,0x2450,0x2550,0x2650,0x2750,
0x2850,0x2950,0x2a50,0x2b50,0x2c50,0x2d50,0x2e50,0x2f50,
0x3050,0x3150,0x3250,0x3350,0x3450,0x3550,0x3650,0x3750,
0x3850,0x3950,0x3a50,0x3b50,0x3c50,0x3d50,0x3e50,0x3f50,
0x4050,0x4150,0x4250,0x4350,0x4450,0x4550,0x4650,0x4750,
0x4850,0x4950,0x4a50,0x4b50,0x4c50,0x4d50,0x4e50,0x4f50,
0x5050,0x5150,0x5250,0x5350,0x5450,0x5550,0x5650,0x5750,
0x5850,0x5950,0x5a50,0x5b50,0x5c50,0x5d50,0x5e50,0x5f50,
0x6050,0x6150,0x6250,0x6350,0x6450,0x6550,0x6650,0x6750,
0x6850,0x6950,0x6a50,0x6b50,0x6c50,0x6d50,0x6e50,0x6f50,
0x7050,0x7150,0x7250,0x7350,0x7450,0x7550,0x7650,0x7750,
0x7850,0x7950,0x7a50,0x7b50,0x7c50,0x7d50,0x7e50,0x7f50,
0x8050,0x8150,0x8250,0x8350,0x8450,0x8550,0x8650,0x8750,
0x8850,0x8950,0x8a50,0x8b50,0x8c50,0x8d50,0x8e50,0x8f50,
0x9050,0x9150,0x9250,0x9350,0x9450,0x9550,0x9650,0x9750,
0x9850,0x9950,0x9a50,0x9b50,0x9c50,0x9d50,0x9e50,0x9f50,
0xfa40,0xfb40,0xfc40,0xfd40,0xfe40,0xff40,0xc0,0x140,
0x240,0x340,0x440,0x540,0x640,0x740,0x840,0x940,
0xa40,0xb40,0xc40,0xd40,0xe40,0xf40,0x1040,0x1140,
0x1240,0x1340,0x1440,0x1540,0x1640,0x1740,0x1840,0x1940,
0x1a40,0x1b40,0x1c40,0x1d40,0x1e40,0x1f40,0x2040,0x2140,
0x2240,0x2340,0x2440,0x2540,0x2640,0x2740,0x2840,0x2940,
0x2a40,0x2b40,0x2c40,0x2d40,0x2e40,0x2f40,0x3040,0x3140,
0x3240,0x3340,0x3440,0x3540,0x3640,0x3740,0x3840,0x3940,
0x3a40,0x3b40,0x3c40,0x3d40,0x3e40,0x3f40,0x4040,0x4140,
0x4240,0x4340,0x4440,0x4540,0x4640,0x4740,0x4840,0x4940,
0x4a40,0x4b40,0x4c40,0x4d40,0x4e40,0x4f40,0x5040,0x5140,
0x5240,0x5340,0x5440,0x5540,0x5640,0x5740,0x5840,0x5940,
0x5a40,0x5b40,0x5c40,0x5d40,0x5e40,0x5f40,0x6040,0x6140,
0x6240,0x6340,0x6440,0x6540,0x6640,0x6740,0x6840,0x6940,
0x6a40,0x6b40,0x6c40,0x6d40,0x6e40,0x6f40,0x7040,0x7140,
0x7240,0x7340,0x7440,0x7540,0x7640,0x7740,0x7840,0x7940,
0x7a40,0x7b40,0x7c40,0x7d40,0x7e40,0x7f40,0x8040,0x8140,
0x8240,0x8340,0x8440,0x8540,0x8640,0x8740,0x8840,0x8940,
0x8a40,0x8b40,0x8c40,0x8d40,0x8e40,0x8f40,0x9040,0x9140,
0x9240,0x9340,0x9440,0x9540,0x9640,0x9740,0x9840,0x9940,
0x9a40,0x9b40,0x9c40,0x9d40,0x9e40,0x9f40,0xa040,0xa140,
0xa240,0xa340,0xa440,0xa540,0xa640,0xa740,0xa840,0xa940,
0xaa40,0xab40,0xac40,0xad40,0xae40,0xaf40,0xb040,0xb140,
0xb240,0xb340,0xb440,0xb540,0xb640,0xb740,0xb840,0xb940,
0xba40,0xbb40,0xbc40,0xbd40,0xbe40,0xbf40,0xc040,0xc140,
0xc240,0xc340,0xc440,0xc540,0xc640,0xc740,0xc840,0xc940,
0xca40,0xcb40,0xcc40,0xcd40,0xce40,0xcf40,0xd040,0xd140,
0xd240,0xd340,0xd440,0xd540,0xd640,0xd740,0xd840,0xd940,
0xda40,0xdb40,0xdc40,0xdd40,0xde40,0xdf40,0xe040,0xe140,
0xe240,0xe340,0xe440,0xe540,0xe640,0xe740,0xe840,0xe940,
0xea40,0xeb40,0xec40,0xed40,0xee40,0xef40,0xf040,0xf140,
0xf240,0xf340,0xf440,0xf540,0xf640,0xf740,0xf840,0xf940,
0x9a50,0x9b50,0x9c50,0x9d50,0x9e50,0x9f50,0xa050,0xa150,
0xa250,0xa350,0xa450,0xa550,0xa650,0xa750,0xa850,0xa950,
0xaa50,0xab50,0xac50,0xad50,0xae50,0xaf50,0xb050,0xb150,
0xb250,0xb350,0xb450,0xb550,0xb650,0xb750,0xb850,0xb950,
0xba50,0xbb50,0xbc50,0xbd50,0xbe50,0xbf50,0xc050,0xc150,
0xc250,0xc350,0xc450,0xc550,0xc650,0xc750,0xc850,0xc950,
0xca50,0xcb50,0xcc50,0xcd50,0xce50,0xcf50,0xd050,0xd150,
0xd250,0xd350,0xd450,0xd550,0xd650,0xd750,0xd850,0xd950,
0xda50,0xdb50,0xdc50,0xdd50,0xde50,0xdf50,0xe050,0xe150,
0xe250,0xe350,0xe450,0xe550,0xe650,0xe750,0xe850,0xe950,
0xea50,0xeb50,0xec50,0xed50,0xee50,0xef50,0xf050,0xf150,
0xf250,0xf350,0xf450,0xf550,0xf650,0xf750,0xf850,0xf950,
0xfa50,0xfb50,0xfc50,0xfd50,0xfe50,0xff50,0xd0,0x150,
0x250,0x350,0x450,0x550,0x650,0x750,0x850,0x950,
0xa50,0xb50,0xc50,0xd50,0xe50,0xf50,0x1050,0x1150,
0x1250,0x1350,0x1450,0x1550,0x1650,0x1750,0x1850,0x1950,
0x1a50,0x1b50,0x1c50,0x1d50,0x1e50,0x1f50,0x2050,0x2150,
0x2250,0x2350,0x2450,0x2550,0x2650,0x2750,0x2850,0x2950,
0x2a50,0x2b50,0x2c50,0x2d50,0x2e50,0x2f50,0x3050,0x3150,
0x3250,0x3350,0x3450,0x3550,0x3650,0x3750,0x3850,0x3950,
0x3a50,0x3b50,0x3c50,0x3d50,0x3e50,0x3f50,0x4050,0x4150,
0x4250,0x4350,0x4450,0x4550,0x4650,0x4750,0x4850,0x4950,
0x4a50,0x4b50,0x4c50,0x4d50,0x4e50,0x4f50,0x5050,0x5150,
0x5250,0x5350,0x5450,0x5550,0x5650,0x5750,0x5850,0x5950,
0x5a50,0x5b50,0x5c50,0x5d50,0x5e50,0x5f50,0x6050,0x6150,
0x6250,0x6350,0x6450,0x6550,0x6650,0x6750,0x6850,0x6950,
0x6a50,0x6b50,0x6c50,0x6d50,0x6e50,0x6f50,0x7050,0x7150,
0x7250,0x7350,0x7450,0x7550,0x7650,0x7750,0x7850,0x7950,
0x7a50,0x7b50,0x7c50,0x7d50,0x7e50,0x7f50,0x8050,0x8150,
0x8250,0x8350,0x8450,0x8550,0x8650,0x8750,0x8850,0x8950,
0x8a50,0x8b50,0x8c50,0x8d50,0x8e50,0x8f50,0x9050,0x9150,
0x9250,0x9350,0x9450,0x9550,0x9650,0x9750,0x9850,0x9950,
0x80,0x100,0x200,0x300,0x400,0x500,0x600,0x700,
0x800,0x900,0x1000,0x1100,0x1200,0x1300,0x1400,0x1500,
0x1000,0x1100,0x1200,0x1300,0x1400,0x1500,0x1600,0x1700,
0x1800,0x1900,0x2000,0x2100,0x2200,0x2300,0x2400,0x2500,
0x2000,0x2100,0x2200,0x2300,0x2400,0x2500,0x2600,0x2700,
0x2800,0x2900,0x3000,0x3100,0x3200,0x3300,0x3400,0x3500,
0x3000,0x3100,0x3200,0x3300,0x3400,0x3500,0x3600,0x3700,
0x3800,0x3900,0x4000,0x4100,0x4200,0x4300,0x4400,0x4500,
0x4000,0x4100,0x4200,0x4300,0x4400,0x4500,0x4600,0x4700,
0x4800,0x4900,0x5000,0x5100,0x5200,0x5300,0x5400,0x5500,
0x5000,0x5100,0x5200,0x5300,0x5400,0x5500,0x5600,0x5700,
0x5800,0x5900,0x6000,0x6100,0x6200,0x6300,0x6400,0x6500,
0x6000,0x6100,0x6200,0x6300,0x6400,0x6500,0x6600,0x6700,
0x6800,0x6900,0x7000,0x7100,0x7200,0x7300,0x7400,0x7500,
0x7000,0x7100,0x7200,0x7300,0x7400,0x7500,0x7600,0x7700,
0x7800,0x7900,0x8000,0x8100,0x8200,0x8300,0x8400,0x8500,
0x8000,0x8100,0x8200,0x8300,0x8400,0x8500,0x8600,0x8700,
0x8800,0x8900,0x9000,0x9100,0x9200,0x9300,0x9400,0x9500,
0x9000,0x9100,0x9200,0x9300,0x9400,0x9500,0x9600,0x9700,
0x9800,0x9900,0x90,0x110,0x210,0x310,0x410,0x510,
0x90,0x110,0x210,0x310,0x410,0x510,0x610,0x710,
0x810,0x910,0x1010,0x1110,0x1210,0x1310,0x1410,0x1510,
0x1010,0x1110,0x1210,0x1310,0x1410,0x1510,0x1610,0x1710,
0x1810,0x1910,0x2010,0x2110,0x2210,0x2310,0x2410,0x2510,
0x2010,0x2110,0x2210,0x2310,0x2410,0x2510,0x2610,0x2710,
0x2810,0x2910,0x3010,0x3110,0x3210,0x3310,0x3410,0x3510,
0x3010,0x3110,0x3210,0x3310,0x3410,0x3510,0x3610,0x3710,
0x3810,0x3910,0x4010,0x4110,0x4210,0x4310,0x4410,0x4510,
0x4010,0x4110,0x4210,0x4310,0x4410,0x4510,0x4610,0x4710,
0x4810,0x4910,0x5010,0x5110,0x5210,0x5310,0x5410,0x5510,
0x5010,0x5110,0x5210,0x5310,0x5410,0x5510,0x5610,0x5710,
0x5810,0x5910,0x6010,0x6110,0x6210,0x6310,0x6410,0x6510,
0x6010,0x6110,0x6210,0x6310,0x6410,0x6510,0x6610,0x6710,
0x6810,0x6910,0x7010,0x7110,0x7210,0x7310,0x7410,0x7510,
0x7010,0x7110,0x7210,0x7310,0x7410,0x7510,0x7610,0x7710,
0x7810,0x7910,0x8010,0x8110,0x8210,0x8310,0x8410,0x8510,
0x8010,0x8110,0x8210,0x8310,0x8410,0x8510,0x8610,0x8710,
0x8810,0x8910,0x9010,0x9110,0x9210,0x9310,0x9410,0x9510,
0x9010,0x9110,0x9210,0x9310,0x9410,0x9510,0x9610,0x9710,
0x9810,0x9910,0xa010,0xa110,0xa210,0xa310,0xa410,0xa510,
0xa010,0xa110,0xa210,0xa310,0xa410,0xa510,0xa610,0xa710,
0xa810,0xa910,0xb010,0xb110,0xb210,0xb310,0xb410,0xb510,
0xb010,0xb110,0xb210,0xb310,0xb410,0xb510,0xb610,0xb710,
0xb810,0xb910,0xc010,0xc110,0xc210,0xc310,0xc410,0xc510,
0xc010,0xc110,0xc210,0xc310,0xc410,0xc510,0xc610,0xc710,
0xc810,0xc910,0xd010,0xd110,0xd210,0xd310,0xd410,0xd510,
0xd010,0xd110,0xd210,0xd310,0xd410,0xd510,0xd610,0xd710,
0xd810,0xd910,0xe010,0xe110,0xe210,0xe310,0xe410,0xe510,
0xe010,0xe110,0xe210,0xe310,0xe410,0xe510,0xe610,0xe710,
0xe810,0xe910,0xf010,0xf110,0xf210,0xf310,0xf410,0xf510,
0xf010,0xf110,0xf210,0xf310,0xf410,0xf510,0xf610,0xf710,
0xf810,0xf910,0x90,0x110,0x210,0x310,0x410,0x510,
0x90,0x110,0x210,0x310,0x410,0x510,0x610,0x710,
0x810,0x910,0x1010,0x1110,0x1210,0x1310,0x1410,0x1510,
0x1010,0x1110,0x1210,0x1310,0x1410,0x1510,0x1610,0x1710,
0x1810,0x1910,0x2010,0x2110,0x2210,0x2310,0x2410,0x2510,
0x2010,0x2110,0x2210,0x2310,0x2410,0x2510,0x2610,0x2710,
0x2810,0x2910,0x3010,0x3110,0x3210,0x3310,0x3410,0x3510,
0x3010,0x3110,0x3210,0x3310,0x3410,0x3510,0x3610,0x3710,
0x3810,0x3910,0x4010,0x4110,0x4210,0x4310,0x4410,0x4510,
0x4010,0x4110,0x4210,0x4310,0x4410,0x4510,0x4610,0x4710,
0x4810,0x4910,0x5010,0x5110,0x5210,0x5310,0x5410,0x5510,
0x5010,0x5110,0x5210,0x5310,0x5410,0x5510,0x5610,0x5710,
0x5810,0x5910,0x6010,0x6110,0x6210,0x6310,0x6410,0x6510,
0x600,0x700,0x800,0x900,0xa00,0xb00,0xc00,0xd00,
0xe00,0xf00,0x1000,0x1100,0x1200,0x1300,0x1400,0x1500,
0x1600,0x1700,0x1800,0x1900,0x1a00,0x1b00,0x1c00,0x1d00,
0x1e00,0x1f00,0x2000,0x2100,0x2200,0x2300,0x2400,0x2500,
0x2600,0x2700,0x2800,0x2900,0x2a00,0x2b00,0x2c00,0x2d00,
0x2e00,0x2f00,0x3000,0x3100,0x3200,0x3300,0x3400,0x3500,
0x3600,0x3700,0x3800,0x3900,0x3a00,0x3b00,0x3c00,0x3d00,
0x3e00,0x3f00,0x4000,0x4100,0x4200,0x4300,0x4400,0x4500,
0x4600,0x4700,0x4800,0x4900,0x4a00,0x4b00,0x4c00,0x4d00,
0x4e00,0x4f00,0x5000,0x5100,0x5200,0x5300,0x5400,0x5500,
0x5600,0x5700,0x5800,0x5900,0x5a00,0x5b00,0x5c00,0x5d00,
0x5e00,0x5f00,0x6000,0x6100,0x6200,0x6300,0x6400,0x6500,
0x6600,0x6700,0x6800,0x6900,0x6a00,0x6b00,0x6c00,0x6d00,
0x6e00,0x6f00,0x7000,0x7100,0x7200,0x7300,0x7400,0x7500,
0x7600,0x7700,0x7800,0x7900,0x7a00,0x7b00,0x7c00,0x7d00,
0x7e00,0x7f00,0x8000,0x8100,0x8200,0x8300,0x8400,0x8500,
0x8600,0x8700,0x8800,0x8900,0x8a00,0x8b00,0x8c00,0x8d00,
0x8e00,0x8f00,0x9000,0x9100,0x9200,0x9300,0x9400,0x9500,
0x9600,0x9700,0x9800,0x9900,0x9a00,0x9b00,0x9c00,0x9d00,
0x9e00,0x9f00,0x90,0x110,0x210,0x310,0x410,0x510,
0x610,0x710,0x810,0x910,0xa10,0xb10,0xc10,0xd10,
0xe10,0xf10,0x1010,0x1110,0x1210,0x1310,0x1410,0x1510,
0x1610,0x1710,0x1810,0x1910,0x1a10,0x1b10,0x1c10,0x1d10,
0x1e10,0x1f10,0x2010,0x2110,0x2210,0x2310,0x2410,0x2510,
0x2610,0x2710,0x2810,0x2910,0x2a10,0x2b10,0x2c10,0x2d10,
0x2e10,0x2f10,0x3010,0x3110,0x3210,0x3310,0x3410,0x3510,
0x3610,0x3710,0x3810,0x3910,0x3a10,0x3b10,0x3c10,0x3d10,
0x3e10,0x3f10,0x4010,0x4110,0x4210,0x4310,0x4410,0x4510,
0x4610,0x4710,0x4810,0x4910,0x4a10,0x4b10,0x4c10,0x4d10,
0x4e10,0x4f10,0x5010,0x5110,0x5210,0x5310,0x5410,0x5510,
0x5610,0x5710,0x5810,0x5910,0x5a10,0x5b10,0x5c10,0x5d10,
0x5e10,0x5f10,0x6010,0x6110,0x6210,0x6310,0x6410,0x6510,
0x6610,0x6710,0x6810,0x6910,0x6a10,0x6b10,0x6c10,0x6d10,
0x6e10,0x6f10,0x7010,0x7110,0x7210,0x7310,0x7410,0x7510,
0x7610,0x7710,0x7810,0x7910,0x7a10,0x7b10,0x7c10,0x7d10,
0x7e10,0x7f10,0x8010,0x8110,0x8210,0x8310,0x8410,0x8510,
0x8610,0x8710,0x8810,0x8910,0x8a10,0x8b10,0x8c10,0x8d10,
0x8e10,0x8f10,0x9010,0x9110,0x9210,0x9310,0x9410,0x9510,
0x9610,0x9710,0x9810,0x9910,0x9a10,0x9b10,0x9c10,0x9d10,
0x9e10,0x9f10,0xa010,0xa110,0xa210,0xa310,0xa410,0xa510,
0xa610,0xa710,0xa810,0xa910,0xaa10,0xab10,0xac10,0xad10,
0xae10,0xaf10,0xb010,0xb110,0xb210,0xb310,0xb410,0xb510,
0xb610,0xb710,0xb810,0xb910,0xba10,0xbb10,0xbc10,0xbd10,
0xbe10,0xbf10,0xc010,0xc110,0xc210,0xc310,0xc410,0xc510,
0xc610,0xc710,0xc810,0xc910,0xca10,0xcb10,0xcc10,0xcd10,
0xce10,0xcf10,0xd010,0xd110,0xd210,0xd310,0xd410,0xd510,
0xd610,0xd710,0xd810,0xd910,0xda10,0xdb10,0xdc10,0xdd10,
0xde10,0xdf10,0xe010,0xe110,0xe210,0xe310,0xe410,0xe510,
0xe610,0xe710,0xe810,0xe910,0xea10,0xeb10,0xec10,0xed10,
0xee10,0xef10,0xf010,0xf110,0xf210,0xf310,0xf410,0xf510,
0xf610,0xf710,0xf810,0xf910,0xfa10,0xfb10,0xfc10,0xfd10,
0xfe10,0xff10,0x90,0x110,0x210,0x310,0x410,0x510,
0x610,0x710,0x810,0x910,0xa10,0xb10,0xc10,0xd10,
0xe10,0xf10,0x1010,0x1110,0x1210,0x1310,0x1410,0x1510,
0x1610,0x1710,0x1810,0x1910,0x1a10,0x1b10,0x1c10,0x1d10,
0x1e10,0x1f10,0x2010,0x2110,0x2210,0x2310,0x2410,0x2510,
0x2610,0x2710,0x2810,0x2910,0x2a10,0x2b10,0x2c10,0x2d10,
0x2e10,0x2f10,0x3010,0x3110,0x3210,0x3310,0x3410,0x3510,
0x3610,0x3710,0x3810,0x3910,0x3a10,0x3b10,0x3c10,0x3d10,
0x3e10,0x3f10,0x4010,0x4110,0x4210,0x4310,0x4410,0x4510,
0x4610,0x4710,0x4810,0x4910,0x4a10,0x4b10,0x4c10,0x4d10,
0x4e10,0x4f10,0x5010,0x5110,0x5210,0x5310,0x5410,0x5510,
0x5610,0x5710,0x5810,0x5910,0x5a10,0x5b10,0x5c10,0x5d10,
0x5e10,0x5f10,0x6010,0x6110,0x6210,0x6310,0x6410,0x6510,
0xc0,0x140,0x240,0x340,0x440,0x540,0x640,0x740,
0x840,0x940,0xa40,0xb40,0xc40,0xd40,0xe40,0xf40,
0x1040,0x1140,0x1240,0x1340,0x1440,0x1540,0x1640,0x1740,
0x1840,0x1940,0x1a40,0x1b40,0x1c40,0x1d40,0x1e40,0x1f40,
0x2040,0x2140,0x2240,0x2340,0x2440,0x2540,0x2640,0x2740,
0x2840,0x2940,0x2a40,0x2b40,0x2c40,0x2d40,0x2e40,0x2f40,
0x3040,0x3140,0x3240,0x3340,0x3440,0x3540,0x3640,0x3740,
0x3840,0x3940,0x3a40,0x3b40,0x3c40,0x3d40,0x3e40,0x3f40,
0x4040,0x4140,0x4240,0x4340,0x4440,0x4540,0x4640,0x4740,
0x4840,0x4940,0x4a40,0x4b40,0x4c40,0x4d40,0x4e40,0x4f40,
0x5040,0x5140,0x5240,0x5340,0x5440,0x5540,0x5640,0x5740,
0x5840,0x5940,0x5a40,0x5b40,0x5c40,0x5d40,0x5e40,0x5f40,
0x6040,0x6140,0x6240,0x6340,0x6440,0x6540,0x6640,0x6740,
0x6840,0x6940,0x6a40,0x6b40,0x6c40,0x6d40,0x6e40,0x6f40,
0x7040,0x7140,0x7240,0x7340,0x7440,0x7540,0x7640,0x7740,
0x7840,0x7940,0x7a40,0x7b40,0x7c40,0x7d40,0x7e40,0x7f40,
0x8040,0x8140,0x8240,0x8340,0x8440,0x8540,0x8640,0x8740,
0x8840,0x8940,0x8a40,0x8b40,0x8c40,0x8d40,0x8e40,0x8f40,
0x9040,0x9140,0x9240,0x9340,0x9440,0x9540,0x9640,0x9740,
0x9840,0x9940,0x9a40,0x9b40,0x9c40,0x9d40,0x9e40,0x9f40,
0xa040,0xa140,0xa240,0xa340,0xa440,0xa540,0xa640,0xa740,
0xa840,0xa940,0xaa40,0xab40,0xac40,0xad40,0xae40,0xaf40,
0xb040,0xb140,0xb240,0xb340,0xb440,0xb540,0xb640,0xb740,
0xb840,0xb940,0xba40,0xbb40,0xbc40,0xbd40,0xbe40,0xbf40,
0xc040,0xc140,0xc240,0xc340,0xc440,0xc540,0xc640,0xc740,
0xc840,0xc940,0xca40,0xcb40,0xcc40,0xcd40,0xce40,0xcf40,
0xd040,0xd140,0xd240,0xd340,0xd440,0xd540,0xd640,0xd740,
0xd840,0xd940,0xda40,0xdb40,0xdc40,0xdd40,0xde40,0xdf40,
0xe040,0xe140,0xe240,0xe340,0xe440,0xe540,0xe640,0xe740,
0xe840,0xe940,0xea40,0xeb40,0xec40,0xed40,0xee40,0xef40,
0xf040,0xf140,0xf240,0xf340,0xf440,0xf540,0xf640,0xf740,
0xf840,0xf940,0xfa40,0xfb40,0xfc40,0xfd40,0xfe40,0xff40,
0xa050,0xa150,0xa250,0xa350,0xa450,0xa550,0xa650,0xa750,
0xa850,0xa950,0xaa50,0xab50,0xac50,0xad50,0xae50,0xaf50,
0xb050,0xb150,0xb250,0xb350,0xb450,0xb550,0xb650,0xb750,
0xb850,0xb950,0xba50,0xbb50,0xbc50,0xbd50,0xbe50,0xbf50,
0xc050,0xc150,0xc250,0xc350,0xc450,0xc550,0xc650,0xc750,
0xc850,0xc950,0xca50,0xcb50,0xcc50,0xcd50,0xce50,0xcf50,
0xd050,0xd150,0xd250,0xd350,0xd450,0xd550,0xd650,0xd750,
0xd850,0xd950,0xda50,0xdb50,0xdc50,0xdd50,0xde50,0xdf50,
0xe050,0xe150,0xe250,0xe350,0xe450,0xe550,0xe650,0xe750,
0xe850,0xe950,0xea50,0xeb50,0xec50,0xed50,0xee50,0xef50,
0xf050,0xf150,0xf250,0xf350,0xf450,0xf550,0xf650,0xf750,
0xf850,0xf950,0xfa50,0xfb50,0xfc50,0xfd50,0xfe50,0xff50,
0xd0,0x150,0x250,0x350,0x450,0x550,0x650,0x750,
0x850,0x950,0xa50,0xb50,0xc50,0xd50,0xe50,0xf50,
0x1050,0x1150,0x1250,0x1350,0x1450,0x1550,0x1650,0x1750,
0x1850,0x1950,0x1a50,0x1b50,0x1c50,0x1d50,0x1e50,0x1f50,
0x2050,0x2150,0x2250,0x2350,0x2450,0x2550,0x2650,0x2750,
0x2850,0x2950,0x2a50,0x2b50,0x2c50,0x2d50,0x2e50,0x2f50,
0x3050,0x3150,0x3250,0x3350,0x3450,0x3550,0x3650,0x3750,
0x3850,0x3950,0x3a50,0x3b50,0x3c50,0x3d50,0x3e50,0x3f50,
0x4050,0x4150,0x4250,0x4350,0x4450,0x4550,0x4650,0x4750,
0x4850,0x4950,0x4a50,0x4b50,0x4c50,0x4d50,0x4e50,0x4f50,
0x5050,0x5150,0x5250,0x5350,0x5450,0x5550,0x5650,0x5750,
0x5850,0x5950,0x5a50,0x5b50,0x5c50,0x5d50,0x5e50,0x5f50,
0x6050,0x6150,0x6250,0x6350,0x6450,0x6550,0x6650,0x6750,
0x6850,0x6950,0x6a50,0x6b50,0x6c50,0x6d50,0x6e50,0x6f50,
0x7050,0x7150,0x7250,0x7350,0x7450,0x7550,0x7650,0x7750,
0x7850,0x7950,0x7a50,0x7b50,0x7c50,0x7d50,0x7e50,0x7f50,
0x8050,0x8150,0x8250,0x8350,0x8450,0x8550,0x8650,0x8750,
0x8850,0x8950,0x8a50,0x8b50,0x8c50,0x8d50,0x8e50,0x8f50,
0x9050,0x9150,0x9250,0x9350,0x9450,0x9550,0x9650,0x9750,
0x9850,0x9950,0x9a50,0x9b50,0x9c50,0x9d50,0x9e50,0x9f50,
0xfa40,0xfb40,0xfc40,0xfd40,0xfe40,0xff40,0xc0,0x140,
0x240,0x340,0x440,0x540,0x640,0x740,0x840,0x940,
0xa40,0xb40,0xc40,0xd40,0xe40,0xf40,0x1040,0x1140,
0x1240,0x1340,0x1440,0x1540,0x1640,0x1740,0x1840,0x1940,
0x1a40,0x1b40,0x1c40,0x1d40,0x1e40,0x1f40,0x2040,0x2140,
0x2240,0x2340,0x2440,0x2540,0x2640,0x2740,0x2840,0x2940,
0x2a40,0x2b40,0x2c40,0x2d40,0x2e40,0x2f40,0x3040,0x3140,
0x3240,0x3340,0x3440,0x3540,0x3640,0x3740,0x3840,0x3940,
0x3a40,0x3b40,0x3c40,0x3d40,0x3e40,0x3f40,0x4040,0x4140,
0x4240,0x4340,0x4440,0x4540,0x4640,0x4740,0x4840,0x4940,
0x4a40,0x4b40,0x4c40,0x4d40,0x4e40,0x4f40,0x5040,0x5140,
0x5240,0x5340,0x5440,0x5540,0x5640,0x5740,0x5840,0x5940,
0x5a40,0x5b40,0x5c40,0x5d40,0x5e40,0x5f40,0x6040,0x6140,
0x6240,0x6340,0x6440,0x6540,0x6640,0x6740,0x6840,0x6940,
0x6a40,0x6b40,0x6c40,0x6d40,0x6e40,0x6f40,0x7040,0x7140,
0x7240,0x7340,0x7440,0x7540,0x7640,0x7740,0x7840,0x7940,
0x7a40,0x7b40,0x7c40,0x7d40,0x7e40,0x7f40,0x8040,0x8140,
0x8240,0x8340,0x8440,0x8540,0x8640,0x8740,0x8840,0x8940,
0x8a40,0x8b40,0x8c40,0x8d40,0x8e40,0x8f40,0x9040,0x9140,
0x9240,0x9340,0x9440,0x9540,0x9640,0x9740,0x9840,0x9940,
0x9a40,0x9b40,0x9c40,0x9d40,0x9e40,0x9f40,0xa040,0xa140,
0xa240,0xa340,0xa440,0xa540,0xa640,0xa740,0xa840,0xa940,
0xaa40,0xab40,0xac40,0xad40,0xae40,0xaf40,0xb040,0xb140,
0xb240,0xb340,0xb440,0xb540,0xb640,0xb740,0xb840,0xb940,
0xba40,0xbb40,0xbc40,0xbd40,0xbe40,0xbf40,0xc040,0xc140,
0xc240,0xc340,0xc440,0xc540,0xc640,0xc740,0xc840,0xc940,
0xca40,0xcb40,0xcc40,0xcd40,0xce40,0xcf40,0xd040,0xd140,
0xd240,0xd340,0xd440,0xd540,0xd640,0xd740,0xd840,0xd940,
0xda40,0xdb40,0xdc40,0xdd40,0xde40,0xdf40,0xe040,0xe140,
0xe240,0xe340,0xe440,0xe540,0xe640,0xe740,0xe840,0xe940,
0xea40,0xeb40,0xec40,0xed40,0xee40,0xef40,0xf040,0xf140,
0xf240,0xf340,0xf440,0xf540,0xf640,0xf740,0xf840,0xf940,
0x9a50,0x9b50,0x9c50,0x9d50,0x9e50,0x9f50,0xa050,0xa150,
0xa250,0xa350,0xa450,0xa550,0xa650,0xa750,0xa850,0xa950,
0xaa50,0xab50,0xac50,0xad50,0xae50,0xaf50,0xb050,0xb150,
0xb250,0xb350,0xb450,0xb550,0xb650,0xb750,0xb850,0xb950,
0xba50,0xbb50,0xbc50,0xbd50,0xbe50,0xbf50,0xc050,0xc150,
0xc250,0xc350,0xc450,0xc550,0xc650,0xc750,0xc850,0xc950,
0xca50,0xcb50,0xcc50,0xcd50,0xce50,0xcf50,0xd050,0xd150,
0xd250,0xd350,0xd450,0xd550,0xd650,0xd750,0xd850,0xd950,
0xda50,0xdb50,0xdc50,0xdd50,0xde50,0xdf50,0xe050,0xe150,
0xe250,0xe350,0xe450,0xe550,0xe650,0xe750,0xe850,0xe950,
0xea50,0xeb50,0xec50,0xed50,0xee50,0xef50,0xf050,0xf150,
0xf250,0xf350,0xf450,0xf550,0xf650,0xf750,0xf850,0xf950,
0xfa50,0xfb50,0xfc50,0xfd50,0xfe50,0xff50,0xd0,0x150,
0x250,0x350,0x450,0x550,0x650,0x750,0x850,0x950,
0xa50,0xb50,0xc50,0xd50,0xe50,0xf50,0x1050,0x1150,
0x1250,0x1350,0x1450,0x1550,0x1650,0x1750,0x1850,0x1950,
0x1a50,0x1b50,0x1c50,0x1d50,0x1e50,0x1f50,0x2050,0x2150,
0x2250,0x2350,0x2450,0x2550,0x2650,0x2750,0x2850,0x2950,
0x2a50,0x2b50,0x2c50,0x2d50,0x2e50,0x2f50,0x3050,0x3150,
0x3250,0x3350,0x3450,0x3550,0x3650,0x3750,0x3850,0x3950,
0x3a50,0x3b50,0x3c50,0x3d50,0x3e50,0x3f50,0x4050,0x4150,
0x4250,0x4350,0x4450,0x4550,0x4650,0x4750,0x4850,0x4950,
0x4a50,0x4b50,0x4c50,0x4d50,0x4e50,0x4f50,0x5050,0x5150,
0x5250,0x5350,0x5450,0x5550,0x5650,0x5750,0x5850,0x5950,
0x5a50,0x5b50,0x5c50,0x5d50,0x5e50,0x5f50,0x6050,0x6150,
0x6250,0x6350,0x6450,0x6550,0x6650,0x6750,0x6850,0x6950,
0x6a50,0x6b50,0x6c50,0x6d50,0x6e50,0x6f50,0x7050,0x7150,
0x7250,0x7350,0x7450,0x7550,0x7650,0x7750,0x7850,0x7950,
0x7a50,0x7b50,0x7c50,0x7d50,0x7e50,0x7f50,0x8050,0x8150,
0x8250,0x8350,0x8450,0x8550,0x8650,0x8750,0x8850,0x8950,
0x8a50,0x8b50,0x8c50,0x8d50,0x8e50,0x8f50,0x9050,0x9150,
0x9250,0x9350,0x9450,0x9550,0x9650,0x9750,0x9850,0x9950
};

const uint8_t z80_cycles[] = {
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

const uint8_t z80_cb_cycles[] = {
    8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
    8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
    8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
    8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
    8, 8, 8, 8, 8, 8, 12, 8, 8, 8, 8, 8, 8, 8, 12, 8,
    8, 8, 8, 8, 8, 8, 12, 8, 8, 8, 8, 8, 8, 8, 12, 8,
    8, 8, 8, 8, 8, 8, 12, 8, 8, 8, 8, 8, 8, 8, 12, 8,
    8, 8, 8, 8, 8, 8, 12, 8, 8, 8, 8, 8, 8, 8, 12, 8,
    8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
    8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
    8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
    8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
    8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
    8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
    8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8,
    8, 8, 8, 8, 8, 8, 16, 8, 8, 8, 8, 8, 8, 8, 16, 8
};
