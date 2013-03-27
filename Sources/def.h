#ifndef DEF_H
#define DEF_H
#define DEBUG 0
#define BYTE uint8_t
#define BYTE_S int8_t
#define FLAG_Z 0x80
#define FLAG_N 0x40
#define FLAG_H 0x20
#define FLAG_C 0x10
#define FLAG_NZ 0x7F
#define FLAG_NN 0xBF
#define FLAG_NH 0xDF
#define FLAG_NC 0xEF
#define BIT_0 0x01
#define BIT_1 0x02
#define BIT_2 0x04
#define BIT_3 0x08
#define BIT_4 0x10
#define BIT_5 0x20
#define BIT_6 0x40
#define BIT_7 0x80
#define ADD 1
#define SUB 0
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
#endif
