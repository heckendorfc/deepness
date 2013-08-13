#include <stdlib.h>
#include <stdio.h>

#ifdef NDS_MODE
#include <nds.h>
#include <fat.h>
#endif

#include "save.h"
#include "player.h"

void save_init(){
#ifdef NDS_MODE
	fatInitDefault();
#endif
}

void save(){
	int i;
	uint8_t num_chars=0;
	FILE *fd;

#ifdef NDS_MODE
	fd=fopen("fat:/savefile.sav","wb");
#else
	fd=fopen("deepness.sav","wb");
#endif

	for(i=0;i<NUM_CHAR_SLOTS;i++){
		if(pdata.chars[i])
			num_chars++;
	}

	fwrite(&num_chars,1,sizeof(num_chars),fd);
	for(i=0;i<num_chars;i++)
		fwrite(pdata.chars[i],1,sizeof(**pdata.chars),fd);
	fwrite(pdata.inventory,NUM_ITEMS,sizeof(*pdata.inventory),fd);
	fwrite(&pdata.d_level,1,sizeof(pdata.d_level),fd);
	fwrite(&pdata.seed,1,sizeof(pdata.seed),fd);

	fclose(fd);
}

void load(){
	int i;
	uint8_t num_chars;
	FILE *fd;

#ifdef NDS_MODE
	fd=fopen("fat:/savefile.sav","rb");
#else
	fd=fopen("deepness.sav","rb");
#endif

	fread(&num_chars,1,sizeof(num_chars),fd);
	for(i=0;i<num_chars;i++){
		pdata.chars[i]=malloc(sizeof(**pdata.chars));
		fread(pdata.chars[i],1,sizeof(**pdata.chars),fd);
	}
	fread(pdata.inventory,NUM_ITEMS,sizeof(*pdata.inventory),fd);
	fread(&pdata.d_level,1,sizeof(pdata.d_level),fd);
	fread(&pdata.seed,1,sizeof(pdata.seed),fd);

	fclose(fd);
}
