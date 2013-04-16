#include "sound.h"

static inline void update_channel1(int clocks);
static inline void update_channel2(int clocks);
static inline void update_channel3(int clocks);
static inline void update_channel4(int clocks);
static inline void channel_on(unsigned int channel);
static inline void channel_off(unsigned int channel);
static inline void clock_sample(sc3_t *sc3, int time_lap);
static inline void clock_sweep(sc1_t *sc1);
static inline void clock_lfsr(sc4_t *sc4, int time_lap);
static inline void clock_length(length_t *len, unsigned channel);
static inline void clock_envelope(envelope_t *env);
static inline void channel1_clock_square(sc1_t *sc1, int time_lap);
static inline void channel2_clock_square(sc2_t *sc2, int time_lap);
static inline void add_delta(int side, unsigned time_lap, short amplitude, short *last_delta);
static inline void sweep_freq();

apu_t apu;
int sound_cycles;
int sound_enabled = 1;
static blip_t* blip_left;
static blip_t* blip_right;
unsigned short *lfsr[2];
static unsigned lfsr_size[2];
static int sample_rate = 44100;
static int clock_rate = 4194304;
static short* wave_samples;
static SDL_mutex *mut_sound;

static const BYTE wave[16] = {
	0xAC,0xDD,0xDA,0x48,
	0x36,0x02,0xCF,0x16,
	0x2C,0x04,0xE5,0x2C,
	0xAC,0xDD,0xDA,0x48
};

static const BYTE square[4][8] = {
	{ 0, 0,-1, 0, 0, 0, 0, 0},
	{ 0,-1,-1, 0, 0, 0, 0, 0},
	{-1,-1,-1,-1, 0, 0, 0, 0},
	{-1, 0, 0,-1,-1,-1,-1,-1}
};

static const int freqtab[8] = {
	(1<<14)*2,
	(1<<14),
	(1<<14)/2,
	(1<<14)/3,
	(1<<14)/4,
	(1<<14)/5,
	(1<<14)/6,
	(1<<14)/7,
};

static void callback(void* data, Uint8 *stream, int len);
void sound_init(){
	BYTE r7;
	unsigned short r15;
	unsigned int i;
	lfsr_size[LFSR_7] = LFSR_7_S;
	lfsr_size[LFSR_15] = LFSR_15_S;
	lfsr[LFSR_7] = malloc(lfsr_size[LFSR_7]*sizeof(short));
	lfsr[LFSR_15] = malloc(lfsr_size[LFSR_15]*sizeof(short));

	//Initialisation des valeurs 7 bits LFSR
	r7 = 0xFF;
	for(i=0; i<lfsr_size[LFSR_7];i++){
		r7 >>=1;
		r7 |= (((r7 & 0x02) >> 1) ^ (r7 & 0x01)) << 7;
		if(r7 & 0x01)
			lfsr[LFSR_7][i] = HI / 15;
		else 
			lfsr[LFSR_7][i] = LO / 15;

	}

	//Initialisation des valeurs 15 bits LFSR
	r15 = 0xFFFF;
	for(i = 0;i<lfsr_size[LFSR_15];i++){
		r15 >>=1;
		r15 |= (((r15 & 0x0002) >> 1) ^ (r15 & 0x0001)) << 15;
		if(r15 & 0x0001)
			lfsr[LFSR_15][i] = HI / 15;
		else 
			lfsr[LFSR_15][i] = LO / 15;

	}


	wave_samples =(short*) malloc(32*sizeof(short));
	SDL_InitSubSystem(SDL_INIT_AUDIO); //AVOIR
	desired.freq = (sample_rate);
	desired.format = AUDIO_S16SYS;
	desired.channels = 2;
	desired.samples = 2048;
	desired.callback = callback;
	desired.userdata = NULL;
	if(SDL_OpenAudio(&desired, NULL) < 0){
		fprintf(stderr, "Erreur à l'ouverture du peripherique audio : %s\n", SDL_GetError()); // Écriture de l'erreur
		exit(EXIT_FAILURE); // On quitte le programme
	}
	blip_left = blip_new(sample_rate / 10);
	blip_set_rates(blip_left,clock_rate,sample_rate);

	blip_right = blip_new(sample_rate / 10);
	blip_set_rates(blip_right,clock_rate,sample_rate);
	mut_sound = SDL_CreateMutex();
	sound_enabled = 0;
	start_sound();


	/*for(int i=0;i<16;i++){
	//write_wave(0xFF30 + i, wave[i] );
	const short scale = ((32767 / 4)*2)/15;
	wave_samples[((0xFF30+i) - 0xFF30)*2] = ((wave[i] >> 4) - 7)*scale; 
	wave_samples[((0xFF30+i) - 0xFF30)*2 + 1] = ((wave[i] & 0x0F) - 7)*scale;
	internal_ram[(0xFF30+i)] = wave[i];
	}*/

	/*apu.channel1.sweep_period = 0x00;
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

	  for(int i=0;i<4;i++){
	  apu.sound_controller.output_sound_so1[i] = 0;
	  apu.sound_controller.output_sound_so2[i] = 0;
	  apu.sound_controller.sound_flags[i] = 0;
	  }*/

}

void sound_fini(){
	if(sound_enabled == 1){
		stop_sound();
	}
	SDL_CloseAudio();
	free(lfsr[LFSR_7]);
	free(lfsr[LFSR_15]);
}

void stop_sound(){
	assert(sound_enabled == 1);
	SDL_PauseAudio(1);
	sound_enabled = 0;
}

void start_sound(){
	assert(sound_enabled == 0);
	SDL_PauseAudio(0);
	sound_enabled = 1;
}

void sound_reset(){
	memory_write(NR10,0x80);
	memory_write(NR11,0xBF);
	memory_write(NR12,0xF3);
	memory_write(NR13,0xFF);
	memory_write(NR14,0xBF);
	//memory_write(NR20,0xFF);
	memory_write(NR21,0x3F);
	memory_write(NR22,0x00);
	memory_write(NR23,0xFF);
	memory_write(NR24,0xBF);
	memory_write(NR30,0x7F);
	memory_write(NR31,0xFF);
	memory_write(NR32,0x9F);
	memory_write(NR33,0xFF);
	memory_write(NR34,0xBF);
	//memory_write(NR40,0xFF);
	memory_write(NR41,0xFF);
	memory_write(NR42,0x00);
	memory_write(NR43,0x00);
	memory_write(NR44,0xBF);
	memory_write(NR50,0x77);
	memory_write(NR51,0xF3);
	memory_write(NR52,0xF1);

	for(unsigned short i = 0xFF27;i<0xFF30;i++){
		memory_write(i,0xFF);
	}

	memset(&apu.channel1,0,sizeof(apu.channel1));
	memset(&apu.channel2,0,sizeof(apu.channel2));
	memset(&apu.channel3,0,sizeof(apu.channel3));
	memset(&apu.channel4,0,sizeof(apu.channel4));

	apu.channel1.period_counter = 1;
	apu.channel1.length.i = 16384;
	apu.channel1.envelope.i = 65536;
	apu.channel1.sweep.i = 32768;

	apu.channel2.period_counter = 1;
	apu.channel2.length.i = 16384;
	apu.channel2.envelope.i = 65536;

	apu.channel3.period_count = 1;
	apu.channel3.length.i = 16384;

	apu.channel4.period_count = 1;
	apu.channel4.length.i = 16384;
	apu.channel4.envelope.i = 65536;

	for(int i=0;i<16;i++){
		write_wave(0xFF30 + i, wave[i]);
	}
	if(!sound_enabled) start_sound();
	sound_cycles = 0;

}

void write_sound(unsigned short addr, BYTE data){
	BYTE value = data;
	unsigned freq;
	//printf("VALUE : %x, ADDRESS : %x\n", value, address);
	update_sound();

	//Si le son n'est pas activé on ne peut écrire que dans NR52
	if(addr != NR52 && !apu.sound_controller.is_on){
		return;
	}

	switch(addr){
		case NR10:

			apu.channel1.sweep_regulation = (value & BIT_3)?1:0;
			apu.channel1.sweep.time = ((value & 0x70) >> 4); //Sweep period
			apu.channel1.sweep.shift_number = (value & 0x07); //Sweep shift
			apu.channel1.sweep.decrease_dir = apu.channel1.sweep_regulation; //Sweep direction
			internal_ram[addr] = value;	
			break;
		case NR11:

			apu.channel1.length.length = 64 - (value & 0x3F); //Sound length
			apu.channel1.duty.duty = ((value & 0xC0) >> 6); //Wave duty
			internal_ram[addr] = value;	
			break;
		case NR12:
			if((value & BIT_3)>0) apu.channel1.env_direction = 1;else apu.channel1.env_direction = 0;

			apu.channel1.envelope.length = (value & 0x07); //Number of sweep envelope
			apu.channel1.envelope.volume = ((value & 0xF0) >> 4); //Initial Volume
			apu.channel1.envelope.increase_dir = apu.channel1.env_direction; //Envelope direction
			internal_ram[addr] = value;
			break;
		case NR13:

			//Calcul de la fréquence et de la période, à partir des composantes basses
			freq = (apu.channel1.freq & 0x700) | value; //Partie gauche = composantes basses
			apu.channel1.freq = freq;
			apu.channel1.period = 2048 - freq;
			internal_ram[addr] = value;
			break;
		case NR14:
			if((value & BIT_7)>0) apu.channel1.initier = 1;else apu.channel1.initier = 0;
			if((value & BIT_6)>0) apu.channel1.counter_consec = 1;else apu.channel1.counter_consec = 0;

			//Calcul de la fréquence et de la période du premier canal, à partir des composantes hautes
			freq = (apu.channel1.freq & 0xFF) | ((value & 0x07) << 8); //Partie droite = composantes hautes
			apu.channel1.freq = freq;
			apu.channel1.period = 2048 - freq;

			apu.channel1.length.is_continue = apu.channel1.counter_consec;
			if(apu.channel1.initier){
				apu.channel1.envelope.volume = memory_read(NR12) >> 4;
				apu.channel1.length.is_on = 1;
				apu.channel1.envelope.length_count = apu.channel1.envelope.length;

				//Initialisation du sweep
				apu.channel1.sweep.hidden_freq = apu.channel1.freq;
				apu.channel1.sweep.time_count = apu.channel1.sweep.time;

				//Si la longueur n'est pas rechargé, on joue la longueur maximale
				if(apu.channel1.length.length == 0)
					apu.channel1.length.length = 63;
				channel_on(1);
				if(apu.channel1.sweep.time && apu.channel1.sweep.shift_number)
					sweep_freq();
			}
			internal_ram[addr] = value;
			break;
			//SC2
		case NR21:
			//apu.channel2.wave_duty = ((value & 0xC0) >> 6);
			//apu.channel2.sound_length = (value & 0x3F);

			apu.channel2.length.length = 64 - (value & 0x3F); //64 - sound_length
			apu.channel2.duty.duty = ((value & 0xC0) >> 6); //Wave duty
			internal_ram[addr] = value;
			break;
		case NR22:
			//apu.channel2.initial_env_volume = ((value & 0xF0) >> 4);
			//apu.channel2.nb_sweep_env = (value & 0x07);
			if((value & BIT_3)>0) apu.channel2.env_direction = 1;else apu.channel2.env_direction = 0;

			apu.channel2.envelope.length = (value & 0x07); //Nb sweep envelope
			apu.channel2.envelope.volume = ((value & 0xF0) >> 4); //Initial envelope volume
			apu.channel2.envelope.increase_dir = apu.channel2.env_direction;
			internal_ram[addr] = value;
			break;
		case NR23:
			//apu.channel2.freq_low = value;

			//Calcul de la fréquence du deuxième canal à partir des fréquences basses
			freq = (apu.channel2.freq & 0x700) | value; //Value = fréquences basses
			apu.channel2.freq = freq;
			apu.channel2.period = 2048 - freq;
			internal_ram[addr] = value;
			break;
		case NR24:
			//apu.channel2.freq_high = (value & 0x07);
			if((value & BIT_7)>0) apu.channel2.initier = 1;else apu.channel2.initier = 0;
			if((value & BIT_6)>0) apu.channel2.counter_consec = 1;else apu.channel2.counter_consec = 0;

			//Calcul de la fréquence du deuxième canal à partir des fréquences hautes
			freq = (apu.channel2.freq & 0xFF) | ((value & 0x07) << 8); //Partie droite = fréquences hautes
			apu.channel2.freq = freq;
			apu.channel2.period = 2048 - freq;

			apu.channel2.length.is_continue = apu.channel2.counter_consec;
			if(apu.channel2.initier){
				apu.channel2.envelope.volume = memory_read(NR22) >> 4;
				apu.channel2.length.is_on = 1;
				apu.channel2.envelope.length_count = apu.channel2.envelope.length;

				//Si longueur non rechargée, on joue la longueur max
				if(apu.channel2.length.length == 0)
					apu.channel2.length.length = 63;
				channel_on(2);
			}
			internal_ram[addr] = value;
			break;
			//SC3
		case NR30:
			if((value & BIT_7)>0) apu.channel3.sound_trigger = 1;else apu.channel3.sound_trigger = 0;

			apu.channel3.length.is_on = apu.channel3.sound_trigger;
			internal_ram[addr] = value;
			break;
		case NR31:
			//apu.channel3.sound_length = value;

			apu.channel3.length.length = 256 - value; //256 - Sound length
			internal_ram[addr] = value;
			break;
		case NR32:
			//apu.channel3.output_level = ((value & 0x60) >> 4);

			apu.channel3.volume = ((value & 0x60) >> 4); //Output level
			if(apu.channel3.volume == -1)
				apu.channel3.volume = 16;
			internal_ram[addr] = value;
			break;
		case NR33:
			//apu.channel3.freq_low = value;

			//Calcul de la fréquence du troisieme canal à partir des fréquences basses 
			freq = (apu.channel3.freq & 0x700) | value; //Partie droite = fréquences basses
			apu.channel3.freq = freq;
			apu.channel3.period = (2048 - freq) << 1;

			internal_ram[addr] = value;
			break;
		case NR34:
			//apu.channel3.freq_high = (value & 0x07);
			if((value & BIT_7)>0) apu.channel3.initier = 1;else apu.channel3.initier = 0;
			if((value & BIT_6)>0) apu.channel3.counter_consec = 1;else apu.channel3.counter_consec = 0;

			//Calcul de la fréquence du troisième canal à partir des fréquences hautes
			freq = (apu.channel3.freq & 0XFF) | ((value & 0X07) << 8); //Partie droite = fréquences hautes
			apu.channel3.freq = freq;
			apu.channel3.period = (2048 - freq) << 1;

			apu.channel3.length.is_continue = apu.channel3.counter_consec;
			if(apu.channel3.initier){
				apu.channel3.length.is_on = 1;

				//Si longueur non rechargée, on joue la longueur max
				if(apu.channel3.length.length == 0)
					apu.channel3.length.length = 255;
				channel_on(3);
			}
			internal_ram[addr] = value;
			break;
			//SC4
		case NR41:
			//apu.channel4.sound_length = (value & 0x3F);
			apu.channel4.length.length = 64 - (value & 0x3F); //64 - Sound length
			internal_ram[addr] = value;
			break;
		case NR42:
			//apu.channel4.initial_env_volume = ((value & 0xF0) >> 4);
			//apu.channel4.sweep_number = (value & 0x07);
			if((value & BIT_3)>0) apu.channel4.env_direction = 1;else apu.channel4.env_direction = 0;

			apu.channel4.envelope.volume = ((value & 0xF0) >> 4); //Initial envelope volume
			apu.channel4.envelope.length = (value & 0x07); //Sweep Number
			apu.channel4.envelope.increase_dir = apu.channel4.env_direction;
			internal_ram[addr] = value;
			break;
		case NR43:
			apu.channel4.freq_shift = ((value & 0xF0) >> 4);
			apu.channel4.freq_division_ratio = (value & 0x07);
			if((value & BIT_3)>0) apu.channel4.step_counter = 1;else apu.channel4.step_counter = 0;

			apu.channel4.lfsr.size = (value >> 3) & 0x01;
			apu.channel4.period = 4 * ((apu.channel4.freq_division_ratio + 1) << (((value >> 4 ) & 0x0F) + 1) );

			internal_ram[addr] = value;
			break;
		case NR44:
			if((value & BIT_7)>0) apu.channel4.initier = 1;else apu.channel4.initier = 0;
			if((value & BIT_6)>0) apu.channel4.counter_consec = 1;else apu.channel4.counter_consec = 0;

			apu.channel4.length.is_continue = apu.channel4.counter_consec;
			if(apu.channel4.initier){
				apu.channel4.envelope.volume = memory_read(NR42) >> 4;
				apu.channel4.length.is_on = 1;
				apu.channel4.envelope.length_count = apu.channel4.envelope.length;

				//Si longueur non rechargée, on joue la longueur max
				if(apu.channel4.length.length == 0)
					apu.channel4.length.length = 63;
				channel_on(4);
			}
			internal_ram[addr] = value;
			break;
			//SOUND CONTROLL
		case NR50:
			//apu.sound_controller.so2_output_level = ((value & 0x70) >> 4);
			//apu.sound_controller.so1_output_level = (value & 0x07);
			if((value & BIT_7)>0) apu.sound_controller.output_to_so2 = 1;else apu.sound_controller.output_to_so2 = 0;
			if((value & BIT_3)>0) apu.sound_controller.output_to_so1 = 1;else apu.sound_controller.output_to_so1 = 0;

			apu.sound_controller.left_level = ((value & 0x70) >> 4); //SO2 Output level
			apu.sound_controller.right_level = (value & 0x07); //SO1 Output level

			internal_ram[addr] = value;
			break;
		case NR51:
			/*apu.sound_controller.output_sound_so1[0] = (value & BIT_0); 
			apu.sound_controller.output_sound_so1[1] = (value & BIT_1);
			apu.sound_controller.output_sound_so1[2] = (value & BIT_2);
			apu.sound_controller.output_sound_so1[3] = (value & BIT_3);
			apu.sound_controller.output_sound_so2[0] = (value & BIT_4);
			apu.sound_controller.output_sound_so2[1] = (value & BIT_5);
			apu.sound_controller.output_sound_so2[2] = (value & BIT_6);
			apu.sound_controller.output_sound_so2[3] = (value & BIT_7);*/

			apu.channel1.is_right = (value & BIT_0); //Channel 1 to SO1
			apu.channel2.is_right = (value & BIT_1);
			apu.channel3.is_right = (value & BIT_2);
			apu.channel4.is_right = (value & BIT_3);
			apu.channel1.is_left = (value & BIT_4);
			apu.channel2.is_left = (value & BIT_5);
			apu.channel3.is_left = (value & BIT_6);
			apu.channel4.is_left = (value & BIT_7);

			internal_ram[addr] = value;
			break;
		case NR52:
			//apu.sound_controller.sound_flags[0] = (value & BIT_0);
			//apu.sound_controller.sound_flags[1] = (value & BIT_1);
			//apu.sound_controller.sound_flags[2] = (value & BIT_2);
			//apu.sound_controller.sound_flags[3] = (value & BIT_3);
			if((value & BIT_7)>0) apu.sound_controller.all_sounds_trigger = 1;else apu.sound_controller.all_sounds_trigger = 0;

			if(!apu.sound_controller.all_sounds_trigger){
				memory_write(NR10,0x80);
				memory_write(NR11,0xBF);
				memory_write(NR12,0xF3);
				memory_write(NR13,0xFF);
				memory_write(NR14,0xBF);
				//memory_write(NR20,0xFF);
				memory_write(NR21,0x3F);
				memory_write(NR22,0x00);
				memory_write(NR23,0xFF);
				memory_write(NR24,0xBF);
				memory_write(NR30,0x7F);
				memory_write(NR31,0xFF);
				memory_write(NR32,0x9F);
				memory_write(NR33,0xFF);
				memory_write(NR34,0xBF);
				//memory_write(NR40,0xFF);
				memory_write(NR41,0xFF);
				memory_write(NR42,0x00);
				memory_write(NR43,0x00);
				memory_write(NR44,0xBF);
				memory_write(NR50,0x77);
				memory_write(NR51,0xF3);
				memory_write(NR52,0xF1);

			}
			apu.sound_controller.is_on = apu.sound_controller.all_sounds_trigger;
			internal_ram[addr] = value;
			break;
		default:
			break;
	}
	//sound_out();
}

static int frame_sequencer(unsigned period_counter, unsigned length, unsigned envelope, unsigned sweep, int *clocks){
	int soonest;
	if( period_counter < length){
		if (period_counter < envelope){
			if (period_counter < sweep){
				soonest = PERIOD;
				*clocks = period_counter;
			} else {
				soonest = SWEEP;
				*clocks = sweep;
			}
		} else {
			if (envelope < sweep){
				soonest = ENVELOPE;
				*clocks = envelope;
			} else {
				soonest = SWEEP;
				*clocks = sweep;
			}
		}
	} else {
		if (length < envelope){
			if (length < sweep){
				soonest = LENGTH;
				*clocks = length;
			} else {
				soonest = SWEEP;
				*clocks = sweep;
			}	
		} else {
			if(envelope < sweep){
				soonest = ENVELOPE;
				*clocks = envelope;
			} else {
				soonest = SWEEP;
				*clocks = sweep;
			}
		}
	}
	return soonest;
}

/* Modifie la forme d'onde en RAM quand celle de la gb est modifiée, premier nibble joué en premier */
void write_wave(unsigned short addr, BYTE value){
	const short scale = ((HI*2)/15);
	wave_samples[(addr - 0xFF30) * 2] = ((value >> 4) - 7) * scale;
	wave_samples[(addr - 0xFF30) * 2 + 1] = ((value & 0x0F) - 7) * scale;
	internal_ram[addr] = value;
}

static inline void channel_on(unsigned int channel){
	internal_ram[NR52] = memory_read(NR52) | (0x01 << (channel - 1)); 
}

static inline void channel_off(unsigned int channel){
	internal_ram[NR52] = memory_read(NR52) & ~(0x01 << (channel - 1)); 
}

static inline void update_channel1(int clocks){
	int cycle_tmp, soonest;
	unsigned time_lap = 0;
	if(!apu.channel1.length.is_on)
		return;
	while(1){
		soonest = frame_sequencer(apu.channel1.period_counter, apu.channel1.length.i, apu.channel1.envelope.i, apu.channel1.sweep.i, &cycle_tmp);
		if(cycle_tmp > clocks){
			apu.channel1.period_counter -= clocks;
			apu.channel1.length.i -= clocks;
			apu.channel1.envelope.i -= clocks;
			apu.channel1.sweep.i -= clocks;
			return;
		}
		clocks -= cycle_tmp;
		time_lap += cycle_tmp;
		apu.channel1.period_counter -= cycle_tmp;
		apu.channel1.length.i -= cycle_tmp;
		apu.channel1.envelope.i -= cycle_tmp;
		apu.channel1.sweep.i -= cycle_tmp;
		
		switch(soonest){
			case PERIOD:
				apu.channel1.period_counter = apu.channel1.period + 1;
				channel1_clock_square(&apu.channel1,time_lap);
				break;
			case LENGTH:
				apu.channel1.length.i = 16384;
				clock_length(&apu.channel1.length, 1);
				break;
			case ENVELOPE:
				apu.channel1.envelope.i = 65536;
				clock_envelope(&apu.channel1.envelope);
				break;
			case SWEEP:
				apu.channel1.sweep.i = 32768;
				clock_sweep(&apu.channel1);
				break;
		}
	}
}	

static inline void update_channel2(int clocks){
	int cycle_tmp, soonest;
	unsigned time_lap = 0;
	if(!apu.channel2.length.is_on)
		return;
	
	while(1){
		soonest = frame_sequencer(apu.channel2.period_counter, apu.channel2.length.i, apu.channel2.envelope.i, -1, &cycle_tmp);
		if(cycle_tmp > clocks){
			apu.channel2.period_counter -= clocks;
			apu.channel2.length.i -= clocks;
			apu.channel2.envelope.i -= clocks;
			return;
		}
		clocks -= cycle_tmp;
		time_lap += cycle_tmp;
		apu.channel2.period_counter -= cycle_tmp;
		apu.channel2.length.i -= cycle_tmp;
		apu.channel2.envelope.i -= cycle_tmp;
		switch(soonest){
			case PERIOD:
				apu.channel2.period_counter = apu.channel2.period + 1;
				channel2_clock_square(&apu.channel2, time_lap);
				break;
			case LENGTH:
				apu.channel2.length.i = 16384;
				clock_length(&apu.channel2.length, 2);
				break;
			case ENVELOPE:
				apu.channel2.envelope.i = 65536;
				clock_envelope(&apu.channel2.envelope);
				break;
		}
	}

}

int get_DAC_output(int volume){
	static double analog[] = {-1, -0.8667, -0.7334, -0.6, -0.4668, -0.3335, -0.2, -0.067, 0.0664, 0.2, 0.333, 0.4668, 0.6, 0.7334, 0.8667, 1};
	return (int)(8000 * analog[volume]);
}

static inline void update_channel3(int clocks){
	int cycle_tmp, soonest;
	unsigned time_lap = 0;
	if(!apu.channel3.length.is_on)
		return;

	while(1){
		soonest = frame_sequencer(apu.channel3.period_count, apu.channel3.length.i, -1,-1,&cycle_tmp);
		if(cycle_tmp > clocks){
			apu.channel3.period_count -= clocks;
			apu.channel3.length.i -= clocks;
			return;
		}
		clocks -= cycle_tmp;
		time_lap += cycle_tmp;
		apu.channel3.period_count -= cycle_tmp;
		apu.channel3.length.i -= cycle_tmp;
		switch(soonest){
			case PERIOD:
				apu.channel3.period_count = apu.channel3.period + 1;
				clock_sample(&apu.channel3, time_lap);
				break;
			case LENGTH:
				apu.channel3.length.i = 16384;
				clock_length(&apu.channel3.length,3);
				break;
		}
	}
}

static inline void update_channel4(int clocks){
	int cycle_tmp, soonest;
	unsigned time_lap = 0;
	if(!apu.channel4.length.is_on)
		return;
	while(1){
		soonest = frame_sequencer(apu.channel4.period_count, apu.channel4.length.i, apu.channel4.envelope.i, -1, &cycle_tmp);
		if(cycle_tmp > clocks){
			apu.channel4.period_count -= clocks;
			apu.channel4.length.i -= clocks;
			apu.channel4.envelope.i -= clocks;
			return;
		}
		clocks -= cycle_tmp;
		time_lap += cycle_tmp;
		apu.channel4.period_count -= cycle_tmp;
		apu.channel4.length.i -= cycle_tmp;
		apu.channel4.envelope.i -= cycle_tmp;
		switch(soonest){
			case PERIOD:
				apu.channel4.period_count = apu.channel4.period + 1;
				clock_lfsr(&apu.channel4, time_lap);
				break;
			case LENGTH:
				apu.channel4.length.i = 16384;
				clock_length(&apu.channel4.length, 4);
				break;
			case ENVELOPE:
				apu.channel4.envelope.i = 65536;
				clock_envelope(&apu.channel4.envelope);
				break;
		}
	}
}		

void update_sound(){
	if(sound_cycles == 0)
		return ;

	SDL_LockMutex(mut_sound);
	update_channel1(sound_cycles);
	update_channel2(sound_cycles);
	update_channel3(sound_cycles);
	update_channel4(sound_cycles);
	SDL_UnlockMutex(mut_sound);
}

static inline void channel1_clock_square(sc1_t *sc1, int time_lap){
	const int duty_hi[4] = {16,16,8,24};
	const int duty_lo[4] = {20,24,24,16};

	if( (sc1->period != 0) && (sc1->period != 2048) ){
		sc1->duty.i = (sc1->duty.i + 1) & 0x1F;
		if(sc1->duty.i == duty_hi[sc1->duty.duty]){
			if(sc1->is_right)
				add_delta(RIGHT, time_lap, (HI/15)* sc1->envelope.volume, &sc1->last_delta_right);
			if(sc1->is_left)
				add_delta(LEFT, time_lap, (HI/15)* sc1->envelope.volume, &sc1->last_delta_left);

		} else if (sc1->duty.i == duty_lo[sc1->duty.duty]){
			if(sc1->is_right)
				add_delta(RIGHT, time_lap, (LO/15)* sc1->envelope.volume, &sc1->last_delta_right);
			if(sc1->is_left)
				add_delta(LEFT, time_lap, (LO/15)* sc1->envelope.volume, &sc1->last_delta_left);
		}
	}
}

static inline void channel2_clock_square(sc2_t *sc2, int time_lap){
	const int duty_hi[4] = {16,16,8,24};
	const int duty_lo[4] = {20,24,24,16};

	if( (sc2->period != 0) && (sc2->period != 2048) ){
		sc2->duty.i = (sc2->duty.i + 1) & 0x1F;
		if(sc2->duty.i == duty_hi[sc2->duty.duty]){
			if(sc2->is_right)
				add_delta(RIGHT, time_lap, (HI/15)* sc2->envelope.volume, &sc2->last_delta_right);
			if(sc2->is_left)
				add_delta(LEFT, time_lap, (HI/15)* sc2->envelope.volume, &sc2->last_delta_left);

		} else if (sc2->duty.i == duty_lo[sc2->duty.duty]){
			if(sc2->is_right)
				add_delta(RIGHT, time_lap, (LO/15)* sc2->envelope.volume, &sc2->last_delta_right);
			if(sc2->is_left)
				add_delta(LEFT, time_lap, (LO/15)* sc2->envelope.volume, &sc2->last_delta_left);
		}
	}
}

static inline void clock_sample(sc3_t *sc3, int time_lap){
	if((sc3-> period != 0) && (sc3->period != 2048)){
		sc3->wave.i = (sc3->wave.i + 1) & 0x1F;
		if(sc3->is_left)
			add_delta(LEFT, time_lap, wave_samples[sc3->wave.i] >> sc3->volume, &sc3->last_delta_left);
		if(sc3->is_right)
			add_delta(RIGHT, time_lap, wave_samples[sc3->wave.i] >> sc3->volume, &sc3->last_delta_right);
	}
}

static inline void clock_lfsr(sc4_t *sc4, int time_lap){
	if(sc4->period != 0){
		sc4->lfsr.i = ( (sc4->lfsr.i + 1) & (lfsr_size[sc4->lfsr.size] - 1) );
		if(sc4->is_left)
			add_delta(LEFT, time_lap, lfsr[sc4->lfsr.size][sc4->lfsr.i] * sc4->envelope.volume, &sc4->last_delta_left);
		if(sc4->is_right)
			add_delta(RIGHT, time_lap, lfsr[sc4->lfsr.size][sc4->lfsr.i] * sc4->envelope.volume, &sc4->last_delta_right);
	}
}

static inline void clock_length(length_t *len, unsigned channel){
	if(!len->is_continue){
		--len->length;
		if(len->length == 0){
			channel_off(channel);
			len->is_on = 0;
		}
	}
}

static inline void clock_envelope(envelope_t *env){
	if(env->length != 0){
		--env->length_count;
		if(env->length_count == 0){
			env->length_count = env->length;
			if(env->increase_dir){
				if(env->volume != 15)
					++env->volume;
			} else {
				if(env->volume != 0)
					--env->volume;
				else 
					env->zombie_mode = 1;
			}	

		}
	}
}

static inline void clock_sweep(sc1_t *sc1){
	if(sc1->sweep.time != 0){
		if(sc1->sweep.time_count == 0){
			sc1->sweep.time_count = sc1->sweep.time;
			if(sc1->sweep.time == 0){
				sc1->length.is_on = 0;
			} else {
				sweep_freq();
			}
		} else {
			--sc1->sweep.time_count;
		}
	}
}

static inline void sweep_freq(){
	apu.channel1.freq = apu.channel1.sweep.hidden_freq;
	if(apu.channel1.freq == 0){
		apu.channel1.period = 0;
	} else {
		apu.channel1.period = 2048 - apu.channel1.freq;
	}

	if(!apu.channel1.sweep.decrease_dir){
		apu.channel1.sweep.hidden_freq += (apu.channel1.sweep.hidden_freq >> apu.channel1.sweep.shift_number);
		if(apu.channel1.sweep.hidden_freq > 2047){
			apu.channel1.length.is_on = 0;
			apu.channel1.sweep.hidden_freq = 2048;
		}
	} else {
		apu.channel1.sweep.hidden_freq -= (apu.channel1.sweep.hidden_freq >> apu.channel1.sweep.shift_number);
		if(apu.channel1.sweep.hidden_freq > 2047){
			apu.channel1.length.is_on = 0;
			apu.channel1.sweep.hidden_freq = 0;
		}
		
	}
}

static inline void add_delta(int side, unsigned time_lap, short amplitude, short *last_delta){
	blip_t *blip;
	if(side == LEFT){
		blip = blip_left;
		amplitude = (amplitude / 7) * apu.sound_controller.left_level;
	} else {
		blip = blip_right;
		amplitude = (amplitude / 7) * apu.sound_controller.right_level;
	}
	blip_add_delta(blip, time_lap, amplitude - *last_delta);
	*last_delta = amplitude;
}

void sc1_freq(){
	int d= 2048 - ( (apu.channel1.freq_high << 8) + apu.channel1.freq_low );
	apu.channel1.freq = (sample_rate > (d << 4))?0:(sample_rate << 17 / d);
}

void sc2_freq(){
	int d= 2048 - ( (apu.channel2.freq_high << 8) + apu.channel2.freq_low );
	apu.channel2.freq = (sample_rate > (d << 4))?0:(sample_rate << 17 / d);

}

void sc3_freq(){
	int d= 2048 - ( (apu.channel3.freq_high << 8) + apu.channel3.freq_low );
	apu.channel3.freq = (sample_rate > (d << 4))?0:(sample_rate << 17 / d);

}

void sc4_freq(){
	apu.channel4.freq = (freqtab[apu.channel4.freq_division_ratio] >> (apu.channel4.freq_shift) * sample_rate);
	if(apu.channel4.freq >> 18)apu.channel4.freq = 1 << 18;

}

static void callback(void* data, Uint8 *stream, int len){
	Sint16 *buffer = (Sint16 *)stream;
	update_sound();

	/*double step = 1/(double)clock_rate;
	  double x=0;
	  int nbytes=2;
	  sc1_freq();
	//len = len/256;
	for(int i=0;i<len;i++){
	double s=sin(2*M_PI*x*apu.channel1.freq);
	int maxi = (1<<(nbytes*8-1))-1;
	int n = (int)(maxi*s);

	blip_add_delta(blip,apu.channel1.sound_length,n);
	//SDL_MixAudio((Uint8 *)buffer, n, len, SDL_MIX_MAXVOLUME);
	for(int j=0;j<nbytes;j++){
	buffer[i] = (BYTE)((n>>(j*8))&0xFF);
	}
	x+=step;

	}*/
	blip_end_frame(blip_left, sound_cycles);
	blip_end_frame(blip_right, sound_cycles);

	sound_cycles = 0;
	
	SDL_LockMutex(mut_sound);
	blip_read_samples(blip_left, buffer, len/4, 1);
	blip_read_samples(blip_right, buffer+1, len/4, 1);
	SDL_UnlockMutex(mut_sound);
}

void sound_out(){
	if(DEBUG_SND){
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
}
