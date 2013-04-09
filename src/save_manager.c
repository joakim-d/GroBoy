#include "save_manager.h"

void save_state()
{
	fichier = fopen(save_path,"w+");
	if(fichier == NULL)printf("impossible d'ouvrir le fichier");
	save_cpu();
	save_gpu();
	fclose(fichier);
}

void restore()
{
	fichier = fopen(save_path,"r");
	if(fichier == NULL)printf("impossible d'ouvrir le fichier");
	int buffer;
	for(int i=0; i<4; i++)
	{
		fread(buffer,4,1,fichier);
		printf("%d\n",buffer);
	}	
}
