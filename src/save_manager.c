#include "save_manager.h"

void save_state()
{	
	char name[0x10000];
	char path[0x1000C];
	strcpy(path, "saves/");
	strcpy(name, getName());
	strcat(path,name);
	strcat(path,".save");
	fichier = fopen(path,"w+");
	if(fichier == NULL)printf("Error when creating save file\n");
	else{
		save_cpu(fichier);
		save_gpu(fichier);
		save_interrupt(fichier);
		save_memory(fichier);
		fclose(fichier);
	}
	//restore();
}

void restore()
{	
	char name[0x10000];
	char path[0x1000C];
	strcpy(path,"saves/");
	strcpy(name, getName());
	strcat(path,name);
	strcat(path,".save");
	fichier = fopen(path,"r");
	if(fichier == NULL) printf("Error when opening save file\n");
	else{
		restore_cpu(fichier);
		restore_gpu(fichier);
		restore_interrupt(fichier);
		restore_memory(fichier);
		fclose(fichier);
	}
}
