#include "save_manager.h"

void save_state(int num)
{	
	char name[0x10000];
	char path[0x1000C];
	strcpy(path, "saves/");
	get_gamename(name);
	strcat(path,name);
	switch(num){
		case 1: strcat(path,"1");break;
		case 2: strcat(path,"2");break;
		case 3: strcat(path,"3");break;
		case 4: strcat(path,"4");break;
		default: break;
	}
	strcat(path,".save");
	file = fopen(path,"w+");
	if(file == NULL)printf("Error when creating save file\n");
	else{
		save_cpu(file);
		save_gpu(file);
		save_interrupt(file);
		save_memory(file);
		save_timer(file);
		save_joypad(file);
		fclose(file);
	}
}

void restore(int num)
{	
	char name[0x10000];
	char path[0x1000C];
	strcpy(path,"saves/");
	get_gamename(name);
	strcat(path,name);
	switch(num){
		case 1: strcat(path,"1");break;
		case 2: strcat(path,"2");break;
		case 3: strcat(path,"3");break;
		case 4: strcat(path,"4");break;
		default: break;
	}
	strcat(path,".save");
	file = fopen(path,"r");
	if(file == NULL) printf("Error when opening save file\n");
	else{
		restore_cpu(file);
		restore_gpu(file);
		restore_interrupt(file);
		restore_memory(file);
		restore_timer(file);
		restore_joypad(file);
		fclose(file);
	}
}
