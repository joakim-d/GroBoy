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

void sound_run(unsigned short address){
	BYTE value = memory_read(address);
	switch(address){
		case NR10:
			apu.channel1.sweep_period = ((value & 0x70) >> 4);
			apu.channel1.sweep_shift = (value & 0x07);
			if((value & BIT_3) > 0)apu.channel1.sweep_regulation = 1;else apu.channel1.sweep_regulation = 0;
		break;
		case NR11:
			apu.channel1.wave_duty = ((value & 0xC0) >> 6);
			apu.channel1.sound_length = (value & 0x3F);
		break;
		case NR12:
			apu.channel1.initial_volume = ((value & 0xF0) >> 4);
			apu.channel1.nb_sweep_env = (value & 0x07);
			if((value & BIT_3)>0) apu.channel1.env_direction = 1;else apu.channel1.env_direction = 0;
		break;
		case NR13:
		// REMPLIR NR13
		apu.channel1.freq_low = value;
		break;
		case NR14:
		//REMPLIR NR14
			apu.channel1.freq_high = (value & 0x07);
			if((value & BIT_7)>0) apu.channel1.initier = 1;else apu.channel1.initier = 0;
			if((value & BIT_6)>0) apu.channel1.counter_consec = 1;else apu.channel1.counter_consec = 0;
		break;
		//SC2
		case NR21:
			apu.channel2.wave_duty = ((value & 0xC0) >> 6);
			apu.channel2.sound_length = (value & 0x3F);
		break;
		case NR22:
			apu.channel2.initial_env_volume = ((value & 0xF0) >> 4);
			apu.channel2.nb_sweep_env = (value & 0x07);
			if((value & BIT_3)>0) apu.channel2.env_direction = 1;else apu.channel2.env_direction = 0;
		break;
		case NR23:
			apu.channel2.freq_low = value;
		break;
		case NR24:
			apu.channel2.freq_high = (value & 0x07);
			if((value & BIT_7)>0) apu.channel2.initier = 1;else apu.channel2.initier = 0;
			if((value & BIT_6)>0) apu.channel2.counter_consec = 1;else apu.channel2.counter_consec = 0;
		break;
		//SC3
		case NR30:
			if((value & BIT_7)>0) apu.channel3.sound_trigger = 1;else apu.channel3.sound_trigger = 0;
		break;
		case NR31:
			apu.channel3.sound_length = value;
		break;
		case NR32:
			apu.channel3.output_level = ((value & 0x60) >> 4);
		break;
		case NR33:
			apu.channel3.freq_low = value;
		break;
		case NR34:
			apu.channel3.freq_high = (value & 0x07);
			if((value & BIT_7)>0) apu.channel3.initier = 1;else apu.channel3.initier = 0;
			if((value & BIT_6)>0) apu.channel3.counter_consec = 1;else apu.channel3.counter_consec = 0;
		break;
		//SC4
		case NR41:
			apu.channel4.sound_length = (value & 0x3F);
		break;
		case NR42:
			apu.channel4.initial_env_volume = ((value & 0xF0) >> 4);
			apu.channel4.sweep_number = (value & 0x07);
			if((value & BIT_3)>0) apu.channel4.env_direction = 1;else apu.channel4.env_direction = 0;
		break;
		case NR43:
			apu.channel4.freq_shift = ((value & 0xF0) >> 4);
			apu.channel4.freq_division_ratio = (value & 0x07);
			if((value & BIT_3)>0) apu.channel4.step_counter = 1;else apu.channel4.step_counter = 0;
		break;
		case NR44:
			if((value & BIT_7)>0) apu.channel4.initier = 1;else apu.channel4.initier = 0;
			if((value & BIT_6)>0) apu.channel4.counter_consec = 1;else apu.channel4.counter_consec = 0;
		break;
		//SOUND CONTROLL
		case NR50:
			apu.sound_controller.so2_output_level = ((value & 0x70) >> 4);
			apu.sound_controller.so1_output_level = (value & 0x07);
			if((value & BIT_7)>0) apu.sound_controller.output_to_so2 = 1;else apu.sound_controller.output_to_so2 = 0;
			if((value & BIT_3)>0) apu.sound_controller.output_to_so1 = 1;else apu.sound_controller.output_to_so1 = 0;
		break;
		case NR51:
			apu.sound_controller.output_sound_so1[0] = (value & BIT_0);
			apu.sound_controller.output_sound_so1[1] = (value & BIT_1);
			apu.sound_controller.output_sound_so1[2] = (value & BIT_2);
			apu.sound_controller.output_sound_so1[3] = (value & BIT_3);
			apu.sound_controller.output_sound_so2[4] = (value & BIT_4);
			apu.sound_controller.output_sound_so2[5] = (value & BIT_5);
			apu.sound_controller.output_sound_so2[6] = (value & BIT_6);
			apu.sound_controller.output_sound_so2[7] = (value & BIT_7);

		break;
		case NR52:
			apu.sound_controller.sound_flags[0] = (value & BIT_0);
			apu.sound_controller.sound_flags[1] = (value & BIT_1);
			apu.sound_controller.sound_flags[2] = (value & BIT_2);
			apu.sound_controller.sound_flags[3] = (value & BIT_3);
			if((value & BIT_7)>0) apu.sound_controller.all_sounds_trigger = 1;else apu.sound_controller.all_sounds_trigger = 0;
		break;
		default:
		break;
	}
}

