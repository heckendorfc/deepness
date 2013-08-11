#include <stdlib.h>
#include <stdio.h>
#include <nds.h>
#include <fat.h>
#include "save.h"
#include "player.h"

void save_init(){
	fatInitDefault();
}

void save(){
	int i;
	uint8_t num_chars=0;
	FILE *fd;

	fd=fopen("fat:/savefile.sav","wb");

	for(i=0;i<NUM_CHAR_SLOTS;i++){
		if(pdata.chars[i])
			num_chars++;
	}

	fwrite(&num_chars,1,sizeof(num_chars),fd);
	for(i=0;i<num_chars;i++)
		fwrite(pdata.chars[i],1,sizeof(**pdata.chars),fd);

	fclose(fd);
}

void load(){
	int i;
	uint8_t num_chars;
	FILE *fd;

	fd=fopen("fat:/savefile.sav","rb");

	fread(&num_chars,1,sizeof(num_chars),fd);
	for(i=0;i<num_chars;i++){
		pdata.chars[i]=malloc(sizeof(**pdata.chars));
		fread(pdata.chars[i],1,sizeof(**pdata.chars),fd);
	}

	fclose(fd);
}
