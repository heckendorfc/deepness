#include <stdlib.h>
#include "game.h"
#include "player.h"
#include "util.h"
#include "map.h"
#include "battle.h"
#include "ui_common.h"

static const char *names[]={
	"merb",
	"Bolo",
	"x4nt",
	"Bkt\0",
};

const int numchar=4;

void create_game(){
	int i,j;
	for(i=0;i<NUM_CHAR_SLOTS;i++)pdata.chars[i]=NULL;
	for(i=0;i<numchar;i++){
		pdata.chars[i]=malloc(sizeof(**pdata.chars));
		create_character(pdata.chars[i]);
		for(j=0;names[i][j];j++)
			pdata.chars[i]->name[j]=names[i][j];
		pdata.chars[i]->name[4]=0;
		if(i<2)
			pdata.chars[i]->battleready=BATTLE_READY;
		else
			pdata.chars[i]->battleready=BATTLE_UNAVAILABLE;
	}
}

static void encounter(){
	struct character *foe;
	int i,num=NUM_CHAR_SLOTS;

	for(i=0;i<NUM_CHAR_SLOTS;i++){
		if(pdata.chars[i]==NULL || pdata.chars[i]->battleready==BATTLE_UNAVAILABLE)
			num--;
	}

	gen_random_map();

	foe=malloc(num*sizeof(*foe));
	for(i=0;i<num;i++)
		create_character(foe+i);

	start_battle(pdata.chars,foe,num);

	free(foe);
}

void run_game(){
	int x,y;
	int flags;
	gen_areamap(&x,&y);
	while(1){
		area_menu(&x,&y);
		flags=get_area_map(x,y);
		if(flags&AMAP_ENCOUNTER_BIT)
			encounter();
		if(flags&AMAP_EXIT_BIT)
			break;

		explore_areamap(x,y);
	}
}
