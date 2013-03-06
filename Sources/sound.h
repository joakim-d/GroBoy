#ifndef SOUND_H
#define SOUND_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "def.h"
#include "memory.h"

//Registres du Canal de son 1
#define NR10 0xff10 //Registre de balayage (R/W)
#define NR11 0xff11 //Longueur de son et duty d'une forme d'onde (R/W)

#define NR12 0xff12 //Enveloppe de volume (R/W)
#define NR13 0xff13 //Composantes basses de fréquence (W)
#define NR14 0xff14 //Composantes hautes de fréquence (R/W)

//Registres du Canal de son 2
#define NR21 0xff16 //Idem NR11 avec b6 NR24
#define NR22 0xff17 //Idem NR12
#define NR23 0xff18 //Idem NR13 
#define NR24 0xff19 //Idem NR14

//Registres du Canal de son 3
#define NR30 0xff1a //Son on-off (R/W)
#define NR31 0xff1b //Durée de son
#define NR32 0xff1c //Son sortie (R/W)
#define NR33 0xff1d //Donnée basse de la frequence (W)
#define NR34 0xff1e //Données hautes de la fréquence (R/W)
//Les adresses FF30 à FF3F contiennent 32 échantillons 4 bits
//lus dans le sens 4forts -> 4faibles

//Registres du Canal de son 4
#define NR41 0xff20 //Durée de son (R/W)
#define NR42 0xff21 //Enveloppe de volume (R/W)
#define NR43 0xff22 //Compteur polynomial (R/W)
#define NR44 0xff23 //Compteur/Consecutif; Initial (R/W)

//Registres de contrôle du son 
#define NR50 0xff24 //controle canal / on-off / volume (R/W)
#define NR51 0xff25 //sortie son vers terminal droite / gauche
#define NR52 0xff26 //son on-off

//Sound Channel 1 : Tone & Sweep
//Onde carrée
typedef struct{
	//NR10	
	BYTE sweep_period;
	BYTE sweep_shift;
	int sweep_regulation; // 0 : up, 1 : down
	//NR11	
	BYTE wave_duty;
	BYTE sound_length;
	//NR12	
	BYTE initial_volume;
	BYTE nb_sweep_env;
	int env_direction; // 0 : down, 1 : up
	//NR13	
	BYTE freq_low;
	//NR14	
	BYTE freq_high;
	int initier; // booleen
	int counter_consec; // booleen

} sc1_t;

//Sound Channel 2 : Idem que SC1 mais sans enveloppe / sweet
//Onde carrée
typedef struct{
	//NR21
	BYTE sound_length;
	BYTE wave_duty;
	//NR22
	BYTE initial_env_volume;
	int env_direction;
	BYTE nb_sweep_env;
	//NR23
	BYTE freq_low;
	//NR24
	BYTE freq_high;
	int initier; //booleen
	int counter_consec; //booleen

} sc2_t;

//Sound Channel 3 : Utilisé pour le son digital de sortie et tonalités normales à l'init de l'onde RAM
//Canal d'échantillons
typedef struct{
	//NR30
	int sound_trigger; //booleen (on/off)
	//NR31
	BYTE sound_length;
	//NR32
	BYTE output_level;
	//NR33
	BYTE freq_low;
	//NR34
	BYTE freq_high;
	int initier; //booleen
	int counter_consec; //booleen

	/*
	 * Les registres de FF30 à FF3F 
	 * stockent des formes d'ondes arbitraires
	 * déjà présentes dans la cartouche, 
	 * sous forme de 32 échantillons 4 bits
	 */

} sc3_t;

//Canal utilisé pour le bruit
typedef struct{
	//NR41
	BYTE sound_length;
	//NR42
	BYTE initial_env_volume;
	BYTE sweep_number;
	int env_direction; //0 = down, 1 = up
	//NR43
	BYTE freq_shift;
	BYTE freq_division_ratio;
	int step_counter; // 0 = 15b, 1 = 7b
	//NR44
	int initier; //booleen
	int counter_consec; //booleen

} sc4_t;

//Registres de controle du son
typedef struct {
	//NR50
	BYTE so2_output_level;
	BYTE so1_output_level;
	int output_to_so2; //booleen
	int output_to_so1; //booleen
	//NR51
	//FIXME
	/*int* output_sound_so1; //i[0..3], output sound i to so2 terminal
	int* output_sound_so2; //i[0..3], output sound i to so1 terminal*/
	int output_sound_so1[4];
	int output_sound_so2[4];
	//NR52
	int all_sounds_trigger; //booleen, 0 : stop all circuits 1 : on
	//FIXME
	//int* sound_flags;//i[0..3], flag for sci
	int sound_flags[4];
}sound_control_t;

//Audio processing unit
typedef struct{
	sc1_t channel1;//Canal de son 1
	sc2_t channel2;//Canal de son 2
	sc3_t channel3;//Canal de son 3
	sc4_t channel4;//Canal de son 4
	sound_control_t sound_controller;//Controleur de son

} apu_t;

void sound_init();
void sound_run(unsigned short address);
#endif
