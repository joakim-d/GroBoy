#include "memory.h"
#include "mbc1.h"
#include <string.h>
#include <fstream>
#include <iostream>
Memory::Memory(){
    int i;
    //read_rom_info(rom_path);
    force_write_ = false;
    for(i = 0; i < 0x10000; i++){
        internal_ram_[i] = 0;
    }
    write(0xFF05,0x00);
    write(0xFF06,0x00);
    write(0xFF07,0x00);
    write(0xFF10,0x80);
    write(0xFF11,0xBF);
    write(0xFF12,0xF3);
    write(0xFF13,0xFF);
    write(0xFF14,0xBF);
    write(0xFF16,0x3F);
    write(0xFF17,0x00);
    write(0xFF18,0xFF);
    write(0xFF19,0xBF);
    write(0xFF1A,0x7F);
    write(0xFF1B,0xFF);
    write(0xFF1C,0x9F);
    write(0xFF1D,0xFF);
    write(0xFF1E,0xBF);
    write(0xFF20,0xFF);
    write(0xFF21,0x00);
    write(0xFF22,0x00);
    write(0xFF23,0xBF);
    write(0xFF24,0x77);
    write(0xFF25,0xF3);
    write(0xFF26,0xF1);
    write(0xFF40,0x91);
    write(0xFF42,0x00);
    write(0xFF43,0x00);
    write(0xFF45,0x00);
    write(0xFF47,0xFC);
    write(0xFF48,0xFF);
    write(0xFF49,0xFF);
    write(0xFF4A,0x00);
    write(0xFF4B,0x00);
    write(0xFFFF,0x00);
}

void Memory::write(int addr, BYTE data){
    switch(addr){
    case 0 ... 0x7FFF:
    case 0xA000 ... 0xBFFF:
        cartridge_->write(addr, data);
        break;
    case 0xC000 ... 0xDDFF:
        internal_ram_[addr] = data;
        internal_ram_[0xE000 + (addr - 0xC000)] = data;
        break;
    case 0xE000 ... 0xFDFF:
        internal_ram_[addr] = data;
        internal_ram_[0xC000 + (addr - 0xE000)] = data;
        break;
    default:
        if(!force_write_){
            switch(addr){
            case 0xFF04:
            case 0xFF44:
                internal_ram_[addr] = 0;//reset counter
                break;
            case 0xFF10 ... 0xFF26:
                //write_sound(addr,data);
                break;
            case 0xFF30 ... 0xFF3F:
                //write_wave(addr,data);
            case 0xFF46:
                dma_transfer(data); //dma transfer
            default:
                internal_ram_[addr] = data;
            }
        }
        else{
            internal_ram_[addr] = data;
        }
    }
}

void Memory::dma_transfer(BYTE data){
    int i;
    unsigned short source = data << 8;
    for(i = 0; i < 0x9F; i++){
        write(0xFE00 + i, read(source + i));
    }
}

BYTE Memory::read(int addr){
    switch(addr){
    case 0 ... 0x7FFF:
        return cartridge_->read(addr);
    case 0x8000 ... 0x9FFF:
        return internal_ram_[addr];
    case 0xA000 ... 0xBFFF:
        return cartridge_->read(addr);
    default:
        return internal_ram_[addr];
    }
}

void Memory::load_cartridge(const std::string &path){
    std::ifstream file(path, std::ios::binary);
    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    BYTE *cartridge_rom_buffer = new BYTE[size];
    if (!file.read(reinterpret_cast<char *>(cartridge_rom_buffer), size))
    {
        std::cout << "Error while reading rom file" << std::endl;
        exit(EXIT_FAILURE);
    }

    if(memcmp(cartridge_rom_buffer + 0x0100, "\x00\xC3", 2) != 0){
        printf("Error reading rom magic code\n");
        exit(-1);
    }

    BYTE cartridge_type = *(cartridge_rom_buffer + 0x0147);

    switch(cartridge_type){
    case 0x00:

        break;
    case 0x01:
    case 0x02:
    case 0x03:
        cartridge_ = new MBC1();
        break;
    case 0x05:
    case 0x06:

        break;
    case 0x08:
    case 0x09:

        break;
    default:
        break;
    }

    if(!cartridge_){
        exit(-1);
    }

    cartridge_->set_rom(cartridge_rom_buffer);

    printf("Game title: %s\n", cartridge_rom_buffer + 0x0134);
    printf("Cartridge type: ");
    switch(*(cartridge_rom_buffer + 0x0147)){
        case 0x00: printf("ROM ONLY");break;
        case 0x01: printf("MBC1");break;
        case 0x02: printf("MBC1+RAM");break;
        case 0x03: printf("MBC1+RAM+BATTERY");break;
        case 0x05: printf("MBC2");break;
        case 0x06: printf("MBC2+BATTERY");break;
        case 0x08: printf("ROM+RAM");break;
        case 0x09: printf("ROM+RAM+BATTERY");break;
        case 0x0B: printf("MMM01");break;
        case 0x0C: printf("MMM01+RAM");break;
        case 0x0D: printf("MMM01+RAM+BATTERY");break;
        case 0x0F: printf("MBC3+TIMER+BATTERY");break;
        case 0x10: printf("MBC3+TIMER+RAM+BATTERY");break;
        case 0x11: printf("MBC3");break;
        case 0x12: printf("MBC3+RAM");break;
        case 0x13: printf("MBC3+RAM+BATTERY");break;
        case 0x15: printf("MBC4");break;
        case 0x16: printf("MBC4+RAM");break;
        case 0x17: printf("MBC4+RAM+BATTERY");break;
        case 0x19: printf("MBC5");break;
        case 0x1A: printf("MBC5+RAM");break;
        case 0x1B: printf("MBC5+RAM+BATTERY");break;
        case 0x1C: printf("MBC5+RUMBLE");break;
        case 0x1D: printf("MBC5+RUMBLE+RAM");break;
        case 0x1E: printf("MBC5+RUMBLE+RAM+BATTERY");break;
        case 0xFC: printf("POCKET CAMERA");break;
        case 0xFD: printf("BANDAI TAMA5");break;
        case 0xFE: printf("HuC3");break;
        case 0xFF: printf("HuC1+RAM+BATTERY");break;
        default: printf("UNKNOWN");break;
    }
    printf("\nRom Size ");
    switch(*(cartridge_rom_buffer + 0x0148)){
        case 0x00: printf("32KB: 2 Banks");break;
        case 0x01: printf("64KB: 4 Banks");break;
        case 0x02: printf("128KB: 8 Banks");break;
        case 0x03: printf("256KB: 16 Banks");break;
        case 0x04: printf("512KB: 32 Banks");break;
        case 0x05: printf("1MB: 64 Banks");break;
        case 0x06: printf("2MB: 128 Banks");break;
        case 0x07: printf("4MB: 256 Banks");break;
        case 0x52: printf("1.1MB: 72 Banks");break;
        case 0x53: printf("1.2MB: 80 Banks");break;
        case 0x54: printf("1.5MB: 96 Banks");break;
    }

    printf("\nRam Size ");
    switch(*(cartridge_rom_buffer + 0x0149)){
        case 0: printf("None");break;
        case 1: printf("2KB: 1 Bank");break;
        case 2: printf("8KB: 1 Bank");break;
        case 3: printf("32KB: 4 Banks");break;
    }
    printf("\nLanguage ");
    switch(*(cartridge_rom_buffer + 0x014A)){
        case 0: printf("Japanese Game");break;
        case 1: printf("English Game");break;
    }
    printf("\nManufacturer");
    switch(*(cartridge_rom_buffer + 0x014B)){
        case 0x33: printf("Nintendo or extended");break;
        case 0x79: printf("Accolade");break;
        case 0xA4: printf("Konami");break;
    }
    printf("\nVersion number %d\n", cartridge_rom_buffer[0x014C]);


    switch(*(cartridge_rom_buffer + 0x0149)){
        case 1: cartridge_->create_ram(0x100);break;
        case 2: cartridge_->create_ram(0x2000);break;
        case 3: cartridge_->create_ram(0x8000);break;
    }

    switch(*(cartridge_rom_buffer + 0x147)){
        case 5: cartridge_->create_ram(0x100);break;
        case 6: cartridge_->create_ram(0x100);break;
    }
}

void Memory::set_force_write(bool value){
    force_write_ = value;
}

/*
static void read_rom_info(char* rom_path);
static void alloc_ram_mem(size_t size);
static inline void write_mbc1(unsigned short addr, BYTE data);
static inline void write_mbc2(unsigned short addr, BYTE data);
static inline void dma_transfer(BYTE data);
void set_force_write(){
	force_write = 1;
}

void reset_force_write(){
	force_write = 0;
}

static void read_rom_info(char* rom_path){
	int file_d;
	struct stat file_stat;
		
	file_d = open(rom_path, O_RDONLY);
	fstat(file_d, &file_stat);

	cartridge_rom_buffer = (BYTE *) malloc(file_stat.st_size);
	if(read(file_d, cartridge_rom_buffer, file_stat.st_size) == -1){
		printf("Error while reading rom file\n");
		exit(-1);
	}	
	close(file_d);

    if(memcmp(cartridge_rom_buffer + 0x0100, "\x00\xC3", 2) != 0){
		printf("Error reading rom magic code\n");
		exit(-1);
	}

	cartridge_type = *(cartridge_rom_buffer + 0x0147);
#ifdef DEBUG
		printf("Game title: %s\n", cartridge_rom_buffer + 0x0134);
		printf("Cartridge type: ");	
		switch(*(cartridge_rom_buffer + 0x0147)){
			case 0x00: printf("ROM ONLY");break;
			case 0x01: printf("MBC1");break;
			case 0x02: printf("MBC1+RAM");break;
			case 0x03: printf("MBC1+RAM+BATTERY");break;
			case 0x05: printf("MBC2");break;
			case 0x06: printf("MBC2+BATTERY");break;
			case 0x08: printf("ROM+RAM");break;
			case 0x09: printf("ROM+RAM+BATTERY");break;
			case 0x0B: printf("MMM01");break;
			case 0x0C: printf("MMM01+RAM");break;
			case 0x0D: printf("MMM01+RAM+BATTERY");break;
			case 0x0F: printf("MBC3+TIMER+BATTERY");break;
			case 0x10: printf("MBC3+TIMER+RAM+BATTERY");break;
			case 0x11: printf("MBC3");break;
			case 0x12: printf("MBC3+RAM");break;
			case 0x13: printf("MBC3+RAM+BATTERY");break;
			case 0x15: printf("MBC4");break;
			case 0x16: printf("MBC4+RAM");break;
			case 0x17: printf("MBC4+RAM+BATTERY");break;
			case 0x19: printf("MBC5");break;
			case 0x1A: printf("MBC5+RAM");break;
			case 0x1B: printf("MBC5+RAM+BATTERY");break;
			case 0x1C: printf("MBC5+RUMBLE");break;
			case 0x1D: printf("MBC5+RUMBLE+RAM");break;
			case 0x1E: printf("MBC5+RUMBLE+RAM+BATTERY");break;
			case 0xFC: printf("POCKET CAMERA");break;
			case 0xFD: printf("BANDAI TAMA5");break;
			case 0xFE: printf("HuC3");break;
			case 0xFF: printf("HuC1+RAM+BATTERY");break;
			default: printf("UNKNOWN");break;
		}
		printf("\nRom Size ");
		switch(*(cartridge_rom_buffer + 0x0148)){
			case 0x00: printf("32KB: 2 Banks");break;
			case 0x01: printf("64KB: 4 Banks");break;
			case 0x02: printf("128KB: 8 Banks");break;
			case 0x03: printf("256KB: 16 Banks");break;
			case 0x04: printf("512KB: 32 Banks");break;
			case 0x05: printf("1MB: 64 Banks");break;
			case 0x06: printf("2MB: 128 Banks");break;
			case 0x07: printf("4MB: 256 Banks");break;
			case 0x52: printf("1.1MB: 72 Banks");break;
			case 0x53: printf("1.2MB: 80 Banks");break;
			case 0x54: printf("1.5MB: 96 Banks");break;
		}

		printf("\nRam Size ");
		switch(*(cartridge_rom_buffer + 0x0149)){
			case 0: printf("None");break;
			case 1: printf("2KB: 1 Bank");break;
			case 2: printf("8KB: 1 Bank");break;
			case 3: printf("32KB: 4 Banks");break;
		}
		printf("\nLanguage ");
		switch(*(cartridge_rom_buffer + 0x014A)){
			case 0: printf("Japanese Game");break;
			case 1: printf("English Game");break;
		}
		printf("\nManufacturer");
		switch(*(cartridge_rom_buffer + 0x014B)){
			case 0x33: printf("Nintendo or extended");break;
			case 0x79: printf("Accolade");break;
			case 0xA4: printf("Konami");break;
		}
		printf("\nVersion number %d\n", cartridge_rom_buffer[0x014C]);
	}
#endif

	switch(*(cartridge_rom_buffer + 0x0149)){
		case 0: cartridge_ram_enabled = 0;break;
		case 1: alloc_ram_mem(0x100);break;
		case 2: alloc_ram_mem(0x2000);break;
		case 3: alloc_ram_mem(0x8000);break;
	}

	switch(*(cartridge_rom_buffer + 0x147)){
		case 5: alloc_ram_mem(0x100);break;
		case 6: alloc_ram_mem(0x100);break;
	}
}

void memory_init(char *rom_path){
	int i;
	read_rom_info(rom_path);
	rom_selector = 1;
	ram_selector = 0;
	force_write = 0;
	for(i = 0; i < 0x10000; i++){
		internal_ram[i] = 0;
	}
    write(0xFF05,0x00);
    write(0xFF06,0x00);
    write(0xFF07,0x00);
    write(0xFF10,0x80);
    write(0xFF11,0xBF);
    write(0xFF12,0xF3);
    write(0xFF13,0xFF);
    write(0xFF14,0xBF);
    write(0xFF16,0x3F);
    write(0xFF17,0x00);
    write(0xFF18,0xFF);
    write(0xFF19,0xBF);
    write(0xFF1A,0x7F);
    write(0xFF1B,0xFF);
    write(0xFF1C,0x9F);
    write(0xFF1D,0xFF);
    write(0xFF1E,0xBF);
    write(0xFF20,0xFF);
    write(0xFF21,0x00);
    write(0xFF22,0x00);
    write(0xFF23,0xBF);
    write(0xFF24,0x77);
    write(0xFF25,0xF3);
    write(0xFF26,0xF1);
    write(0xFF40,0x91);
    write(0xFF42,0x00);
    write(0xFF43,0x00);
    write(0xFF45,0x00);
    write(0xFF47,0xFC);
    write(0xFF48,0xFF);
    write(0xFF49,0xFF);
    write(0xFF4A,0x00);
    write(0xFF4B,0x00);
    write(0xFFFF,0x00);
}





static void alloc_ram_mem(size_t size){
	cartridge_ram_enabled = 1;
	cartridge_ram_buffer = (BYTE *) malloc(size);
	restore_cartridge(size);
}

inline void write(unsigned short addr, BYTE data){
	if((addr <= 0x7FFF) || (addr >= 0xA000 && addr <= 0xBFFF)){
		if(cartridge_type >= 0x01 && cartridge_type <= 0x03){//MBC1
			write_mbc1(addr, data);		
		}
		else if(cartridge_type == 0x05 || cartridge_type == 0x06)//MBC2
		{
			write_mbc2(addr,data);
		}

	}
	else if(addr >= 0xC000 && addr <= 0xDDFF){
		internal_ram[addr] = data;
		internal_ram[0xE000 + (addr - 0xC000)] = data;
	}
	else if(addr >= 0xE000 && addr <= 0xFDFF){
		internal_ram[addr] = data;
		internal_ram[0xC000 + (addr - 0xE000)] = data;
	}
	else{
		if(!force_write){
			if(addr == 0xFF04 || addr == 0xFF44) {internal_ram[addr] = 0;}	//reset counter
			else if(addr >= 0xFF10 && addr <= 0xFF26 && SOUND){
				write_sound(addr,data);	
			}
			else if(addr >= 0xFF30 && addr <= 0xFF3F && SOUND){
				write_wave(addr,data);
			}
			else if(addr == 0xFF46) {dma_transfer(data);}					//dma transfer
			else {internal_ram[addr] = data;}
		}
		else{
			internal_ram[addr] = data;	
		}
	}

}

static inline void write_mbc2(unsigned short addr, BYTE data){
	if(addr <= 0x1FFF){
		if(!(addr & 0x100)) enable_ram = 1;
		else enable_ram = 0;
	}
	else if(addr <= 0x3FFF){
		rom_selector = data & 0x0F;
		if(rom_selector == 0) rom_selector++;
	}
	else
		cartridge_ram_buffer[addr - 0xA000] = data;
}

static inline void write_mbc1(unsigned short addr, BYTE data){
	if(addr <= 0x1FFF){
		if((data & 0x0A) == 0x0A) enable_ram = 1;
		else enable_ram = 0;
	}
	else if(addr <= 0x3FFF){
		rom_selector = data & 0x1F;
		if(rom_selector == 0 || rom_selector == 0x20 || rom_selector == 0x40 || rom_selector == 0x60) rom_selector++;
	}
	else if(addr <= 0x5FFF){
		ram_selector = (data & 0x60) >> 5; //sélection ram
	}
	else if(addr <= 0x7FFF){
		if(data == 0) rom_mode = 1; //rom mode enabled
		else rom_mode = 0; // ram mode enabled
	}
	else if(cartridge_ram_enabled)
		cartridge_ram_buffer[0x2000*(ram_selector) + addr - 0xA000] = data;
}



void memory_dump(int part){
	int i;
	int diff=0;
	switch(part){
		case ALL:
			for(i=0x00;i<=0xFFFF;i++){
				if(!diff)printf("----- ALL MEMORY -----\n");
                printf("%x : %x\n", i, read(i));
			}
			break;
		case FIRST_CART:
			for(i=0x0000;i<=0x3FFF;i++){
				if(!diff)printf("----- 16KB ROM BANK 00 -----\n");
                printf("%x : %x\n", i, read(i));
			}
			break;
		case SWITCH_CART:
			for(i=0x4000;i<=0x7FFF;i++){
				if(!diff)printf("----- 16KB ROM BANK 01..NN -----\n");
                printf("%x : %x\n", i, read(i));
			}
			break;
		case VRAM:
			for(i=0x8000;i<=0x9FFF;i++){
				if(!diff)printf("----- 8KB VIDEO RAM (VRAM) -----\n");
                printf("%x : %x\n", i, read(i));
			}
			break;
		case EXTRAM:
			for(i=0xA000;i<=0xBFFF;i++){
				if(!diff)printf("----- 8KB EXTERNAL RAM -----\n");
                printf("%x : %x\n", i, read(i));
			}
			break;
		case FIRST_WRAM:
			for(i=0xC000;i<=0xCFFF;i++){
				if(!diff)printf("----- 4KB WORK RAM BANK 0 -----\n");
                printf("%x : %x\n", i, read(i));
			}
			break;
		case SWITCH_WRAM:
			for(i=0xD000;i<=0xDFFF;i++){
				if(!diff)printf("----- 4KB WORK RAM BANK 1 -----\n");
                printf("%x : %x\n", i, read(i));
			}
			break;
		case ECHO:
			for(i=0xE000;i<=0xFDFF;i++){
				if(!diff)printf("----- SAME AS COOO-DDFF (ECHO) -----\n");
                printf("%x : %x\n", i, read(i));
			}
			break;
		case OAM:
			for(i=0xFE00;i<=0xFE9F;i++){
				if(!diff)printf("----- SPRITE ATTRIBUTE TABLE (OAM) -----\n");
                printf("%x : %x\n", i, read(i));
			}
			break;
		case NONUSED:
			for(i=0xFEA0;i<=0xFEFF;i++){
				if(!diff)printf("----- NOT USABLE -----\n");
                printf("%x : %x\n", i, read(i));
			}
			break;
		case IO:
			for(i=0xFF00;i<=0xFF7F;i++){
				if(!diff)printf("----- I/O PORTS -----\n");
                printf("%x : %x\n", i, read(i));
			}
			break;
		case HRAM:
			for(i=0xFF80;i<=0xFFFE;i++){
				if(!diff)printf("----- HIGH RAM (HRAM) -----\n");
                printf("%x : %x\n", i, read(i));
			}
			break;
		case IER:
			i = 0xFFFF;
			if(!diff)printf("----- INTERRUP ENABLE REGISTER -----\n");
            printf("%x : %x\n", i, read(i));
			break;
		case DETAIL_VIDEO:
			if(!diff)printf("LCDC - LCD Control\n");
            printf("FF40 : %x\n", read(0xFF40));
			if(!diff)printf("STAT - LCDC Status\n");
            printf("FF41 : %x\n", read(0xFF41));
			if(!diff)printf("SCY - Scroll Y\n");
            printf("FF42 : %x\n", read(0xFF42));
			if(!diff)printf("SCX - Scroll X\n");
            printf("FF43 : %x\n", read(0xFF43));
			if(!diff)printf("LY - LCDC Y-Coordinate\n");
            printf("FF44 : %x\n", read(0xFF44));
			if(!diff)printf("LYC - LY Compare\n");
            printf("FF45 : %x\n", read(0xFF45));
			if(!diff)printf("WY - Window Y Position\n");
            printf("FF4A : %x\n", read(0xFF4A));
			if(!diff)printf("WX - Window X Position minus 7\n");
            printf("FF4B : %x\n", read(0xFF4B));
			if(!diff)printf("BGP - BG Palette Data\n");
            printf("FF47 : %x\n", read(0xFF47));
			if(!diff)printf("OBP0 Object Palette 0 Data\n");
            printf("FF48 : %x\n", read(0xFF48));
			if(!diff)printf("OBP1 Object Palette 1 Data\n");
            printf("FF49 : %x\n", read(0xFF49));
			if(!diff)printf("BCPS/BGPI Background Palette Index\n");
            printf("FF68 : %x\n", read(0xFF68));
			if(!diff)printf("BCPD/BGPD Background Palette Data\n");
            printf("FF69 : %x\n", read(0xFF69));
			if(!diff)printf("OCPS/OBPI Sprite Palette Index\n");
            printf("FF6A : %x\n", read(0xFF6A));
			if(!diff)printf("OCPD/OBPD Sprite Palette Data\n");
            printf("FF6B : %x\n", read(0xFF6B));
			if(!diff)printf("VBK VRAM Bank\n");
            printf("FF4F : %x\n", read(0xFF4F));
			if(!diff)printf("DMA Transfert And Start Address\n");
            printf("FF46 : %x\n", read(0xFF46));
			if(!diff)printf("New DMA Source, High\n");
            printf("FF51 : %x\n", read(0xFF51));
			if(!diff)printf("New DMA Source, Low\n");
            printf("FF52 : %x\n", read(0xFF52));
			if(!diff)printf("New DMA Destination, High\n");
            printf("FF53 : %x\n", read(0xFF53));
			if(!diff)printf("New DMA Destination, Low\n");
            printf("FF54 : %x\n", read(0xFF54));
			if(!diff)printf("New DMA Length/Mode/Start\n");
            printf("FF55 : %x\n", read(0xFF55));
			break;
		case DETAIL_SOUND:
			if(!diff)printf("Channel 1 Sweep Register\n");
            printf("FF10 : %x\n", read(0xFF10));
			if(!diff)printf("Channel 1 Sound Length/ Wave Pattern Duty\n");
            printf("FF11 : %x\n", read(0xFF11));
			if(!diff)printf("Channel 1 Volume Envelope\n");
            printf("FF12 : %x\n", read(0xFF12));
			if(!diff)printf("Channel 1 Frequency lo\n");
            printf("FF13 : %x\n", read(0xFF13));
			if(!diff)printf("Channel 1 Frequency hi\n");
            printf("FF14 : %x\n", read(0xFF14));
			if(!diff)printf("Channel 2 Sound Length/ Wave Pattern Duty\n");
            printf("FF16 : %x\n", read(0xFF16));
			if(!diff)printf("Channel 2 Volume Envelope\n");
            printf("FF17 : %x\n", read(0xFF17));
			if(!diff)printf("Channel 2 Frequency lo data\n");
            printf("FF18 : %x\n", read(0xFF18));
			if(!diff)printf("Channel 2 Frequency hi data\n");
            printf("FF19 : %x\n", read(0xFF19));
			if(!diff)printf("Channel 3 Sound On/Off\n");
            printf("FF1A : %x\n", read(0xFF1A));
			if(!diff)printf("Channel 3 Sound Length\n");
            printf("FF1B : %x\n", read(0xFF1B));
			if(!diff)printf("Channel 3 Select Output Level\n");
            printf("FF1C : %x\n", read(0xFF1C));
			if(!diff)printf("Channel 3 Frequency's lower data\n");
            printf("FF1D : %x\n", read(0xFF1D));
			if(!diff)printf("Channel 3 Frequency's higher data\n");
            printf("FF1E : %x\n", read(0xFF1E));
			for(i=0xFF30;i<=0xFF3F;i++){
				if(!diff)printf("----- WAVE PATTERN RAM STOCK -----\n");
                printf("%x : %x\n", i, read(i));
			}
			if(!diff)printf("Channel 4 Sound Length\n");
            printf("FF20 : %x\n", read(0xFF20));
			if(!diff)printf("Channel 4 Volume Envelope\n");
            printf("FF21 : %x\n", read(0xFF21));
			if(!diff)printf("Channel 4 Polynomial Counter\n");
            printf("FF22 : %x\n", read(0xFF22));
			if(!diff)printf("Channel 4 Counter/consecutive; Initial\n");
            printf("FF23 : %x\n", read(0xFF23));
			if(!diff)printf("Channel control / On-Off / Volume\n");
            printf("FF24 : %x\n", read(0xFF24));
			if(!diff)printf("Selection of sound output terminal\n");
            printf("FF25 : %x\n", read(0xFF25));
			if(!diff)printf("Sound On/Off\n");
            printf("FF26 : %x\n", read(0xFF26));
			break;
		case DETAIL_JOY:
			if(!diff)printf("Joypad\n");
            printf("FF00 : %x\n", read(0xFF00));
			break;
		case DETAIL_TIMER:
			if(!diff)printf("Divider Register\n");
            printf("FF04 : %x\n", read(0xFF04));
			if(!diff)printf("Timer Counter\n");
            printf("FF05 : %x\n", read(0xFF05));
			if(!diff)printf("Timer Modulo\n");
            printf("FF06 : %x\n", read(0xFF06));
			if(!diff)printf("Timer Control\n");
            printf("FF07 : %x\n", read(0xFF07));
			break;
		case DETAIL_INTERRUPTS:
			if(!diff)printf("----- DETAILS INTERRUPTIONS REGISTERS -----\n");
			if(!diff)printf("Interrupt Enable\n");
            printf("FFFF : %x\n", read(0xFFFF));
			if(!diff)printf("Interrupt Flag\n");
            printf("FF0F : %x\n", read(0xFF0F));
			break;
		default : 
			break;
	}
}

static inline void print_error(int type){
	if(type == 0) {printf("Error when writing memory variables\n");}
	else {printf("Error when reading memory variables\n");}
}

//save_state
int save_memory(FILE* file)
{
	int nb=0;
	size_t size;
	switch(*(cartridge_rom_buffer + 0x0149)){
                case 0:
                        switch(*(cartridge_rom_buffer + 0x147)){
                                case 5: size = 0x100;break;
                                case 6: size = 0x100;break;
				default: size = 0;break;
                                }
                                break;
                case 1: size = 0x100;break;
                case 2: size = 0x2000;break;
                case 3: size = 0x8000;break;
		default: size = 0;
        }
	
	if(fwrite(internal_ram,sizeof(BYTE),0x10000,file) != 0x10000) print_error(0);
	if(fwrite(&cartridge_type,sizeof(BYTE),1,file) != 1) print_error(0);
	if(fwrite(&enable_ram, sizeof(BYTE),1,file) != 1) print_error(0);
	if(fwrite(&cartridge_ram_enabled,sizeof(BYTE),1,file) != 1) print_error(0);
	if(fwrite(&rom_mode,sizeof(BYTE),1,file) != 1) print_error(0);
	if(fwrite(&rom_selector,sizeof(BYTE),1,file) != 1) print_error(0);
	if(fwrite(&ram_selector,sizeof(BYTE),1,file) != 1) print_error(0);
	if(fwrite(&force_write,sizeof(BYTE),1,file) != 1) print_error(0);
	if(size != 0){
		if(fwrite(cartridge_ram_buffer,sizeof(BYTE),size,file) != size) print_error(0);
	}
	return nb;	
}


//restore state
void restore_memory(FILE* file)
{
	size_t size;
	switch(*(cartridge_rom_buffer + 0x0149)){
                case 0:
                        switch(*(cartridge_rom_buffer + 0x147)){
                                case 5: size = 0x100;break;
                                case 6: size = 0x100;break;
				default: size = 0;break;
                                }
                                break;
                case 1: size = 0x100;break;
                case 2: size = 0x2000;break;
                case 3: size = 0x8000;break;
        }
	if(fread(internal_ram,sizeof(BYTE),0x10000,file) != 0x10000) print_error(1);
	if(fread(&cartridge_type,sizeof(BYTE),1,file) != 1) print_error(1);
	if(fread(&enable_ram, sizeof(BYTE),1,file) != 1) print_error(1);
	if(fread(&cartridge_ram_enabled,sizeof(BYTE),1,file) != 1) print_error(1);
	if(fread(&rom_mode,sizeof(BYTE),1,file) != 1) print_error(1);
	if(fread(&rom_selector,sizeof(BYTE),1,file) != 1) print_error(1);
	if(fread(&ram_selector, sizeof(BYTE),1,file) != 1) print_error(1);
	if(fread(&force_write,sizeof(BYTE),1,file) != 1) print_error(1);
	if(size != 0){
		if(fread(cartridge_ram_buffer,sizeof(BYTE),size,file) != size) print_error(1);
	}
}

//sauvegarde normale
void save_cartridge(FILE* file)
{
	size_t size;
        switch(*(cartridge_rom_buffer + 0x0149)){
                case 0:
                        switch(*(cartridge_rom_buffer + 0x147)){
                                case 5: size = 0x100;break;
                                case 6: size = 0x100;break;
                                default: size = 0;break;
                                }
                                break;
                case 1: size = 0x100;break;
                case 2: size = 0x2000;break;
                case 3: size = 0x8000;break;
                default: size = 0;
        }

	if(size != 0){
                if(fwrite(cartridge_ram_buffer,sizeof(BYTE),size,file) != size) print_error(0);
        }
}

void restore_cartridge(size_t size)
{
	FILE* file;
	char name[0x10000];
        char path[0x1000C];
        strcpy(path, "saves/");
        get_gamename(name);
        strcat(path,name);
        strcat(path,".save");
        file = fopen(path,"r");
        if(file == NULL) printf("Error when opening save file\n");
        else
	{
		if(size != 0){
        	        if(fread(cartridge_ram_buffer,sizeof(BYTE),size,file) != size) print_error(1);
	        }
		fclose(file);
	}
}
*/

