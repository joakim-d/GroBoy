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
			apu.channel1.sweep_period = (memory_read(address) & 0x70);
			apu.channel1.sweep_shift = (memory_read(address) & 0x07);
			if((memory_read(address) & BIT_3) > 0)apu.channel1.sweep_regulation = 1;else apu.channel1.sweep_regulation = 0;
		break;
		case NR11:
			apu.channel1.wave_duty = (memory_read(address) & 0xC0);
			apu.channel1.sound_length = (memory_read(address) & 0x3F);
		break;
		case NR12:
			apu.channel1.initial_volume = (memory_read(address) & 0xF0);
			apu.channel1.nb_sweep_env = (memory_read(address) & 0x07);
			if((memory_read(address) & BIT_3)>0) apu.channel1.env_direction = 1;else apu.channel1.env_direction = 0;
		break;
		case NR13:
		// REMPLIR NR13
		apu.channel1.freq_low = memory_read(address);
		break;
		case NR14:
		//REMPLIR NR14
			apu.channel1.freq_high = (memory_read(address) & 0x07);
			if((memory_read(address) & BIT_7)>0) apu.channel1.initier = 1;else apu.channel1.initier = 0;
			if((memory_read(address) & BIT_6)>0) apu.channel1.counter_consec = 1;else apu.channel1.counter_consec = 0;
		break;
		//SC2
		case NR21:
			apu.channel2.wave_duty = (memory_read(address) & 0xC0);
			apu.channel2.sound_length = (memory_read(address) & 0x3F);
		break;
		case NR22:
			apu.channel2.initial_env_volume = (memory_read(address) & 0xF0);
			apu.channel2.nb_sweep_env = (memory_read(address) & 0x07);
			if((memory_read(address) & BIT_3)>0) apu.channel2.env_direction = 1;else apu.channel2.env_direction = 0;
		break;
		case NR23:
			apu.channel2.freq_low = memory_read(address);
		break;
		case NR24:
			apu.channel2.freq_high = (memory_read(address) & 0x07);
			if((memory_read(address) & BIT_7)>0) apu.channel2.initier = 1;else apu.channel2.initier = 0;
			if((memory_read(address) & BIT_6)>0) apu.channel2.counter_consec = 1;else apu.channel2.counter_consec = 0;
		break;
		//SC3
		case NR30:
			if((memory_read(address) & BIT_7)>0) apu.channel3.sound_trigger = 1;else apu.channel3.sound_trigger = 0;
		break;
		case NR31:
			apu.channel3.sound_length = memory_read(address);
		break;
		case NR32:
			apu.channel3.output_level = (memory_read(address) & 0x60);
		break;
		case NR33:
			apu.channel3.freq_low = memory_read(address);
		break;
		case NR34:
			apu.channel3.freq_high = (memory_read(address) & 0x07);
			if((memory_read(address) & BIT_7)>0) apu.channel3.initier = 1;else apu.channel3.initier = 0;
			if((memory_read(address) & BIT_6)>0) apu.channel3.counter_consec = 1;else apu.channel3.counter_consec = 0;
		break;
		//SC4
		case NR41:
			apu.channel4.sound_length = (memory_read(address) & 0x3F);
		break;
		case NR42:
			apu.channel4.initial_env_volume = (memory_read(address) & 0xF0);
			apu.channel4.sweep_number = (memory_read(address) & 0x07);
			if((memory_read(address) & BIT_3)>0) apu.channel4.env_direction = 1;else apu.channel4.env_direction = 0;
		break;
		case NR43:
			apu.channel4.freq_shift = (memory_read(address) & 0xF0);
			apu.channel4.freq_division_ratio = (memory_read(address) & 0x07);
			if((memory_read(address) & BIT_3)>0) apu.channel4.step_counter = 1;else apu.channel4.step_counter = 0;
		break;
		case NR44:
			if((memory_read(address) & BIT_7)>0) apu.channel4.initier = 1;else apu.channel4.initier = 0;
			if((memory_read(address) & BIT_6)>0) apu.channel4.counter_consec = 1;else apu.channel4.counter_consec = 0;
		break;
		//SOUND CONTROLL
		case NR50:
			apu.sound_controller.so2_output_level = (memory_read(address) & 0x70);
			apu.sound_controller.so1_output_level = (memory_read(address) & 0x07);
			if((memory_read(address) & BIT_7)>0) apu.sound_controller.output_to_so2 = 1;else apu.sound_controller.output_to_so2 = 0;
			if((memory_read(address) & BIT_3)>0) apu.sound_controller.output_to_so1 = 1;else apu.sound_controller.output_to_so1 = 0;
		break;
		case NR51:
			apu.sound_controller.output_sound_so1[0] = (memory_read(address) & BIT_0);
			apu.sound_controller.output_sound_so1[1] = (memory_read(address) & BIT_1);
			apu.sound_controller.output_sound_so1[2] = (memory_read(address) & BIT_2);
			apu.sound_controller.output_sound_so1[3] = (memory_read(address) & BIT_3);
			apu.sound_controller.output_sound_so2[4] = (memory_read(address) & BIT_4);
			apu.sound_controller.output_sound_so2[5] = (memory_read(address) & BIT_5);
			apu.sound_controller.output_sound_so2[6] = (memory_read(address) & BIT_6);
			apu.sound_controller.output_sound_so2[7] = (memory_read(address) & BIT_7);

		break;
		case NR52:
			apu.sound_controller.sound_flags[0] = (memory_read(address) & BIT_0);
			apu.sound_controller.sound_flags[1] = (memory_read(address) & BIT_1);
			apu.sound_controller.sound_flags[2] = (memory_read(address) & BIT_2);
			apu.sound_controller.sound_flags[3] = (memory_read(address) & BIT_3);
			if((memory_read(address) & BIT_7)>0) apu.sound_controller.all_sounds_trigger = 1;else apu.sound_controller.all_sounds_trigger = 0;
		break;
		default:
		break;
	}
}

