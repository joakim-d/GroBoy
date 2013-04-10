#include "save_manager.h"

void save_state()
{	
	char nom[120];
	char path[120];
	strcpy(path, "saves/");
	getName(nom);
	strcat(path,nom);
	strcat(path,".save");
	fichier = fopen(path,"w+");
	if(fichier == NULL)printf("impossible d'ouvrir le fichier");
	save_cpu(fichier);
	save_gpu(fichier);
	save_interrupt(fichier);
	save_memory(fichier);
	fclose(fichier);
	//restore();
}

void restore()
{	
	char nom[120];
	char path[120];
	strcpy(path,"saves/");
	getName(nom);
        strcat(path,nom);
        strcat(path,".save");
	fichier = fopen(path,"r");
	if(fichier == NULL)printf("impossible d'ouvrir le fichier");
	restore_cpu(fichier);
	restore_gpu(fichier);
	restore_interrupt(fichier);
	restore_memory(fichier);
	fclose(fichier);
}
