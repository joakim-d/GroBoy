#include "sound.h"

apu_t apu;
int sound_cycles = 0;
static blip_t* blip;
static int sample_rate = 44100;
static int clock_rate = 4194304;
static void callback(void* data, Uint8 *stream, int len);
void sound_init(){

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

	/*apu.sound_controller.output_sound_so1 = (int *)malloc(4*sizeof(int));
	  apu.sound_controller.output_sound_so2 = (int *)malloc(4*sizeof(int));
	  apu.sound_controller.sound_flags = (int *)malloc(4*sizeof(int));*/
	for(int i=0;i<4;i++){
		apu.sound_controller.output_sound_so1[i] = 0;
		apu.sound_controller.output_sound_so2[i] = 0;
		apu.sound_controller.sound_flags[i] = 0;
	}
	desired.freq = (apu.channel1.freq_low);
	desired.format = AUDIO_S16SYS;
	desired.channels = 2;
	desired.samples = 2048;
	desired.callback = callback;
	desired.userdata = NULL;
	if(SDL_OpenAudio(&desired, NULL) < 0){
		fprintf(stderr, "Erreur à l'ouverture du peripherique audio : %s\n", SDL_GetError()); // Écriture de l'erreur
		exit(EXIT_FAILURE); // On quitte le programme
	}
	blip = blip_new(sample_rate / 10);
	blip_set_rates(blip,clock_rate,sample_rate);

	//SDL_PauseAudio(0);
}
void sound_out(){

	printf("channel 1 : %x %x %x %x %x %x %x %x %x %x %x %x \n",apu.channel1.sweep_period,
			apu.channel1.sweep_shift,
			apu.channel1.sweep_regulation, 
			apu.channel1.wave_duty,
			apu.channel1.sound_length,
			apu.channel1.initial_volume,
			apu.channel1.nb_sweep_env,
			apu.channel1.env_direction, 
			apu.channel1.freq_low,
			apu.channel1.freq_high,
			apu.channel1.initier,
			apu.channel1.counter_consec); 

	printf("channel 2 : %x %x %x %x %x %x %x %x %x \n",apu.channel2.sound_length,
			apu.channel2.wave_duty,
			apu.channel2.initial_env_volume,
			apu.channel2.env_direction, 
			apu.channel2.nb_sweep_env,
			apu.channel2.freq_low,
			apu.channel2.freq_high,
			apu.channel2.initier, 
			apu.channel2.counter_consec); 

	printf("channel 3 : %x %x %x %x %x %x %x \n",apu.channel3.sound_trigger,
			apu.channel3.sound_length,
			apu.channel3.output_level,
			apu.channel3.freq_low,
			apu.channel3.freq_high,
			apu.channel3.initier,
			apu.channel3.counter_consec); 

	printf("channel 4 : %x %x %x %x %x %x %x %x %x \n",apu.channel4.sound_length,
			apu.channel4.initial_env_volume,
			apu.channel4.sweep_number,
			apu.channel4.env_direction,
			apu.channel4.freq_shift,
			apu.channel4.freq_division_ratio,
			apu.channel4.step_counter, 
			apu.channel4.initier, 
			apu.channel4.counter_consec); 

	printf("sound controller : %x %x %x %x %x \n",apu.sound_controller.so2_output_level,
			apu.sound_controller.so1_output_level,
			apu.sound_controller.output_to_so2, 
			apu.sound_controller.output_to_so1, 
			apu.sound_controller.all_sounds_trigger); 

	for(int i=0;i<4;i++){
		printf("output so1 so2 flag : %d - %x %x %x\n",i,apu.sound_controller.output_sound_so1[i],
		apu.sound_controller.output_sound_so2[i],
		apu.sound_controller.sound_flags[i]);
	} 
}

void write_sound(unsigned short addr, BYTE data){
	blip = blip_new(sample_rate / 10);
	blip_set_rates(blip,clock_rate,sample_rate);
	BYTE value = data;
	//printf("VALUE : %x, ADDRESS : %x\n", value, address);
	SDL_PauseAudio(0);
	update_sound();
	switch(addr){
		case NR10:
			apu.channel1.sweep_period = ((value & 0x70) >> 4);
			apu.channel1.sweep_shift = (value & 0x07);
			if((value & BIT_3) > 0)apu.channel1.sweep_regulation = 1;else apu.channel1.sweep_regulation = 0;
			internal_ram[addr] = value | 0x80;	
			break;
		case NR11:
			apu.channel1.wave_duty = ((value & 0xC0) >> 6);
			apu.channel1.sound_length = (value & 0x3F);
			internal_ram[addr] = value | 0x3f;	
			break;
		case NR12:
			apu.channel1.initial_volume = ((value & 0xF0) >> 4);
			apu.channel1.nb_sweep_env = (value & 0x07);
			if((value & BIT_3)>0) apu.channel1.env_direction = 1;else apu.channel1.env_direction = 0;
			internal_ram[addr] = value;
			break;
		case NR13:
			// REMPLIR NR13
			apu.channel1.freq_low = value;
			internal_ram[addr] = 0xff;
			break;
		case NR14:
			//REMPLIR NR14
			apu.channel1.freq_high = (value & 0x07);
			if((value & BIT_7)>0) apu.channel1.initier = 1;else apu.channel1.initier = 0;
			if((value & BIT_6)>0) apu.channel1.counter_consec = 1;else apu.channel1.counter_consec = 0;
			internal_ram[addr] = value | 0xbf;
			break;
			//SC2
		case NR21:
			apu.channel2.wave_duty = ((value & 0xC0) >> 6);
			apu.channel2.sound_length = (value & 0x3F);
			internal_ram[addr] = value | 0x3f;
			break;
		case NR22:
			apu.channel2.initial_env_volume = ((value & 0xF0) >> 4);
			apu.channel2.nb_sweep_env = (value & 0x07);
			if((value & BIT_3)>0) apu.channel2.env_direction = 1;else apu.channel2.env_direction = 0;
			internal_ram[addr] = value;
			break;
		case NR23:
			apu.channel2.freq_low = value;
			internal_ram[addr] = 0xff;
			break;
		case NR24:
			apu.channel2.freq_high = (value & 0x07);
			if((value & BIT_7)>0) apu.channel2.initier = 1;else apu.channel2.initier = 0;
			if((value & BIT_6)>0) apu.channel2.counter_consec = 1;else apu.channel2.counter_consec = 0;
			internal_ram[addr] = value | 0xbf;
			break;
			//SC3
		case NR30:
			if((value & BIT_7)>0) apu.channel3.sound_trigger = 1;else apu.channel3.sound_trigger = 0;
			internal_ram[addr] = value | 0x7f;
			break;
		case NR31:
			apu.channel3.sound_length = value;
			internal_ram[addr] = 0xff;
			break;
		case NR32:
			apu.channel3.output_level = ((value & 0x60) >> 4);
			internal_ram[addr] = value | 0x9f;
			break;
		case NR33:
			apu.channel3.freq_low = value;
			internal_ram[addr] = 0xff;
			break;
		case NR34:
			apu.channel3.freq_high = (value & 0x07);
			if((value & BIT_7)>0) apu.channel3.initier = 1;else apu.channel3.initier = 0;
			if((value & BIT_6)>0) apu.channel3.counter_consec = 1;else apu.channel3.counter_consec = 0;
			internal_ram[addr] = value | 0xbf;
			break;
			//SC4
		case NR41:
			apu.channel4.sound_length = (value & 0x3F);
			internal_ram[addr] = 0xff;
			break;
		case NR42:
			apu.channel4.initial_env_volume = ((value & 0xF0) >> 4);
			apu.channel4.sweep_number = (value & 0x07);
			if((value & BIT_3)>0) apu.channel4.env_direction = 1;else apu.channel4.env_direction = 0;
			internal_ram[addr] = value;
			break;
		case NR43:
			apu.channel4.freq_shift = ((value & 0xF0) >> 4);
			apu.channel4.freq_division_ratio = (value & 0x07);
			if((value & BIT_3)>0) apu.channel4.step_counter = 1;else apu.channel4.step_counter = 0;
			internal_ram[addr] = value;
			break;
		case NR44:
			if((value & BIT_7)>0) apu.channel4.initier = 1;else apu.channel4.initier = 0;
			if((value & BIT_6)>0) apu.channel4.counter_consec = 1;else apu.channel4.counter_consec = 0;
			internal_ram[addr] = value | 0xbf;
			break;
			//SOUND CONTROLL
		case NR50:
			apu.sound_controller.so2_output_level = ((value & 0x70) >> 4);
			apu.sound_controller.so1_output_level = (value & 0x07);
			if((value & BIT_7)>0) apu.sound_controller.output_to_so2 = 1;else apu.sound_controller.output_to_so2 = 0;
			if((value & BIT_3)>0) apu.sound_controller.output_to_so1 = 1;else apu.sound_controller.output_to_so1 = 0;
			internal_ram[addr] = value;
			break;
		case NR51:
			apu.sound_controller.output_sound_so1[0] = (value & BIT_0); 
			apu.sound_controller.output_sound_so1[1] = (value & BIT_1);
			apu.sound_controller.output_sound_so1[2] = (value & BIT_2);
			apu.sound_controller.output_sound_so1[3] = (value & BIT_3);
			apu.sound_controller.output_sound_so2[0] = (value & BIT_4);
			apu.sound_controller.output_sound_so2[1] = (value & BIT_5);
			apu.sound_controller.output_sound_so2[2] = (value & BIT_6);
			apu.sound_controller.output_sound_so2[3] = (value & BIT_7);
			internal_ram[addr] = value;

			break;
		case NR52:
			apu.sound_controller.sound_flags[0] = (value & BIT_0);
			apu.sound_controller.sound_flags[1] = (value & BIT_1);
			apu.sound_controller.sound_flags[2] = (value & BIT_2);
			apu.sound_controller.sound_flags[3] = (value & BIT_3);
			if((value & BIT_7)>0) apu.sound_controller.all_sounds_trigger = 1;else apu.sound_controller.all_sounds_trigger = 0;
			internal_ram[addr] = (value & 0x80) | 0x70;
			break;
		default:
			break;
	}
	sound_out();
	SDL_PauseAudio(1);
}


void update_sound(){
	//int delta = ; //phase * volume - amplitude
	//blip_add_delta(blip, apu.channel1.sound_length, apu.channel1.initial_volume);
	double period = (clock_rate / ((44100) / 2 +0.5));
	double ampl = apu.channel1.freq_high - apu.channel1.freq_low;
	double length = apu.channel1.sound_length;
	double volume = apu.channel1.initial_volume * 65536 / 2 + 0.5;
	int phase = apu.channel1.nb_sweep_env;
	for( ; length < sound_cycles; length += period){
		int delta = phase * volume - ampl;
		ampl += delta;
		blip_add_delta(blip,length,delta);
		phase = -1 * phase;
		printf("PERIOD : %f \n", period);
	}
	length -= sound_cycles;

	/*ampl = apu.channel2.freq_high - apu.channel2.freq_low;
	length = apu.channel2.sound_length;
	volume = apu.channel2.initial_env_volume * 65536 / 2 + 0.5;
	phase = apu.channel2.nb_sweep_env;
	for( ; length < sound_cycles; length += apu.channel1.sweep_period){
		int delta = phase * volume - ampl;
		ampl += delta;
		blip_add_delta(blip,length,delta);
		phase = -1 * phase;
	}
	length -= sound_cycles;*/



}

static void callback(void* data, Uint8 *stream, int len){
	Sint16 *buffer = (Sint16 *)stream;
	sound_cycles = blip_clocks_needed(blip, len/4);
	update_sound();
	blip_end_frame(blip, sound_cycles);
	blip_read_samples(blip, buffer, len, 1);
}

