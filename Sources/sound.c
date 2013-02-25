#include "sound.h"

apu_t apu;

void init(){
	apu.channel1.sweep_period = 0x00;
	apu.channel1.sweep_shift = 0x00;
	apu.channel1.sweep_regulation = 0; 
	apu.channel1.wave_duty = 0x00;
	apu.channel1.sound_length = 0x00;
	apu.channel1.initial_volume = 0x00;
	apu.channel1.nb_sweep_env = 0x00;
	apu.channel1.env_direction = 0; 
	apu.channel1.freq_low = 0x00;
	apu.channel1.freq_high = 0x00;
	apu.channel1.initier = 0; 
	apu.channel1.counter_consec = 0; 

	apu.channel2.sound_length = 0x00;
	apu.channel2.wave_duty = 0x00;
	apu.channel2.initial_env_volume = 0x00;
	apu.channel2.env_direction = 0; 
	apu.channel2.nb_sweep_env = 0x00;
	apu.channel2.freq_low = 0x00;
	apu.channel2.freq_high = 0x00;
	apu.channel2.initier = 0; 
	apu.channel2.counter_consec = 0; 

	apu.channel3.sound_trigger = 0; 
	apu.channel3.sound_length = 0x00;
	apu.channel3.output_level = 0x00;
	apu.channel3.freq_low = 0x00;
	apu.channel3.freq_high = 0x00;
	apu.channel3.initier = 0; 
	apu.channel3.counter_consec = 0; 

	apu.channel4.sound_length = 0x00;
	apu.channel4.initial_env_volume = 0x00;
	apu.channel4.sweep_number = 0x00;
	apu.channel4.env_direction = 0; 
	apu.channel4.freq_shift = 0x00;
	apu.channel4.freq_division_ratio = 0x00;
	apu.channel4.step_counter = 0; 
	apu.channel4.initier = 0; 
	apu.channel4.counter_consec = 0; 

	apu.sound_controller.so2_output_level = 0x00;
	apu.sound_controller.so1_output_level = 0x00;
	apu.sound_controller.output_to_so2 = 0; 
	apu.sound_controller.output_to_so1 = 0; 
	apu.sound_controller.all_sounds_trigger = 0; 

	apu.sound_controller.output_sound_so1 = (int *)malloc(3*sizeof(int));
	apu.sound_controller.output_sound_so2 = (int *)malloc(3*sizeof(int));
	apu.sound_controller.sound_flags = (int *)malloc(3*sizeof(int));
	for(int i=0;i<4;i++){
		apu.sound_controller.output_sound_so1[i] = 0;
		apu.sound_controller.output_sound_so2[i] = 0;
		apu.sound_controller.sound_flags[i] = 0;
	}
}

void sound_run(){
	unsigned short address;
	switch(address){
		case NR10:
			if((memory_read(address) & BIT_3) > 0)apu.channel1.sweep_regulation = 1;else apu.channel1.sweep_regulation = 0;
		break;
		case NR11:
		break;
		case NR12:
			if((memory_read(address) & BIT_3)>0) apu.channel1.env_direction = 1;else apu.channel1.env_direction = 0;
		break;
		case NR13:
		break;
		case NR14:
			if((memory_read(address) & BIT_7)>0) apu.channel1.initier = 1;else apu.channel1.initier = 0;
			if((memory_read(address) & BIT_6)>0) apu.channel1.counter_consec = 1;else apu.channel1.counter_consec = 0;
		break;
		//SC2
		case NR21:
		break;
		case NR22:
			if((memory_read(address) & BIT_3)>0) apu.channel2.env_direction = 1;else apu.channel2.env_direction = 0;
		break;
		case NR23:
		break;
		case NR24:
			if((memory_read(address) & BIT_7)>0) apu.channel2.initier = 1;else apu.channel2.initier = 0;
			if((memory_read(address) & BIT_6)>0) apu.channel2.counter_consec = 1;else apu.channel2.counter_consec = 0;
		break;
		//SC3
		case NR30:
			if((memory_read(address) & BIT_7)>0) apu.channel3.sound_trigger = 1;else apu.channel3.sound_trigger = 0;
		break;
		case NR31:
		break;
		case NR32:
		break;
		case NR33:
		break;
		case NR34:
			if((memory_read(address) & BIT_7)>0) apu.channel3.initier = 1;else apu.channel3.initier = 0;
			if((memory_read(address) & BIT_6)>0) apu.channel3.counter_consec = 1;else apu.channel3.counter_consec = 0;
		break;
		//SC4
		case NR41:
		break;
		case NR42:
			if((memory_read(address) & BIT_3)>0) apu.channel4.env_direction = 1;else apu.channel4.env_direction = 0;
		break;
		case NR43:
			if((memory_read(address) & BIT_3)>0) apu.channel4.step_counter = 1;else apu.channel4.step_counter = 0;
		break;
		case NR44:
			if((memory_read(address) & BIT_7)>0) apu.channel4.initier = 1;else apu.channel4.initier = 0;
			if((memory_read(address) & BIT_6)>0) apu.channel4.counter_consec = 1;else apu.channel4.counter_consec = 0;
		break;
		//SOUND CONTROLL
		case NR50:
			if((memory_read(address) & BIT_7)>0) apu.sound_controller.output_to_so2 = 1;else apu.sound_controller.output_to_so2 = 0;
			if((memory_read(address) & BIT_3)>0) apu.sound_controller.output_to_so1 = 1;else apu.sound_controller.output_to_so1 = 0;
		break;
		case NR51:
		break;
		case NR52:
			if((memory_read(address) & BIT_7)>0) apu.sound_controller.all_sounds_trigger = 1;else apu.sound_controller.all_sounds_trigger = 0;
		break;
		default:
		break;
	}
}

