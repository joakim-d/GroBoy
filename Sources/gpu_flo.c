//recupere la tuile 
//type 1 -> sprite
//type 2 -> background
//type 3 -> window
int get_tyle(unsigned short num, BYTE *tyle[16][8], int type){
	int size = 8;// 8x8 ou 8x19
	int lig = 0;
	BYTE bg_tile_table_addr; //table du background
	BYTE wd_tile_table_addr; //table pour la fenetre
	BYTE tile_table_addr; //table des tuiles
	BYTE byte1,byte2,pos; 

	if(lcd_cont & 0x08) bg_tile_table_addr = 0x9C00;
	else bg_tile_table_addr = 0x9800;
	
	if(lcd_cond & 0x10) tile_table_addr = 0x8000;
	else tile_table_addr = 0x8800;

	if(lcd_cond & 0x40) wd_tile_table_addr = 0x9C00;
	else wd_tile_table_addre = 0x9800;
			 

	switch(type)
	{
		case 1:
			if(lcd_cont & 0x04) size =16;
			pos  = read_memory(tile_table_addr + num);
			break;

		case 2:
			pos  = read_memory(bg_tile_table_addr + num);
			break;
	
		case 3: 
			pos  = read_memory(wd_tile_table_addr + num);
                        break;
	}


	for(int pt=pos; pt=pos+15; pt+=2;
	{
		byte1 = read_memory(pt);
		byte2 = read_memory(pt+1);
		for(int i=0; i<8; i++)
		{
			if(byte1 & (1<<(7-i))) tyle[lig][i] = 1;
			else tyle[lig][i]=0;
			if(byte2 & (1<<(7-i))) tyle[lig][i] |= 2;
		}
		lig++;
	}

	return size;

}

