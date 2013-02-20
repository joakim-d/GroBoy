#ifndef SOUND_H
#define SOUND_H
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "memory.h"
#include "def.h"

//Sound Channel 1 : Tone & Sweep
//Onde carrée
typedef struct{


} sc1_t;

//Sound Channel 2 : Idem que SC1 mais sans enveloppe / sweet
//Onde carrée
typedef struct{


} sc2_t;

//Sound Channel 3 : Utilisé pour le son digital de sortie et tonalités normales à l'init de l'onde RAM
//Canal d'échantillons
typedef struct{

	

} sc3_t;

//Canal utilisé pour le bruit
typedef struct{
	

} sc4_t;

//Audio processing unit
typedef struct{
	
	//Registres du Canal de son 1
	BYTE NR21;
	BYTE NR22;
	BYTE NR23;
	BYTE NR24;

	//Registres du Canal de son 2
	

	//Registres du Canal de son 3
	BYTE NR30;//FF1A Son on-off (R/W)
	BYTE NR31;//FF1B Durée de son
	BYTE NR32;//FF1C Son sortie (R/W)
	BYTE NR33;//FF1D Donnée basse de la frequence (W)
	BYTE NR34;//FF1E Données hautes de la fréquence (R/W)
	//Les adresses FF30 à FF3F contiennent 32 échantillons 4 bits
	//lus dans le sens 4forts -> 4faibles

	//Registres du Canal de son 4
	BYTE NR41;//FF20 Durée de son (R/W)
	BYTE NR42;//FF21 Enveloppe de volume (R/W)
	BYTE NR43;//FF22 Compteur polynomial (R/W)
	BYTE NR44;//FF23 Compteur/Consecutif; Initial (R/W)
	
	//Registres de contrôle du son 
	BYTE NR50;//FF24 controle canal / on-off / volume (R/W)
	BYTE NR51;//FF25 sortie son vers terminal droite / gauche
	BYTE NR52;//FF26 son on-off

	sc1_t channel1;//Canal de son 1
	sc2_t channel2;//Canal de son 2
	sc3_t channel3;//Canal de son 3
	sc4_t channel4;//Canal de son 4

} apu_t;
