#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "player.h"
#include "util.h"
#include "map.h"
#include "battle.h"
#include "ui_common.h"
#include "equipment.h"

static const char *names[]={
	"merb",
	"Bolo",
	"x4nt",
	"Bkt\0",
	"tofu",
};

const int numchar=4;

static void destroy_game(){
	int i;
	for(i=0;i<NUM_CHAR_SLOTS;i++){
		if(pdata.chars[i]){
			free(pdata.chars[i]);
			pdata.chars[i]=NULL;
		}
	}
}

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

static int encounter(){
	struct character *foe;
	int ret,i,num=NUM_CHAR_SLOTS;

	for(i=0;i<NUM_CHAR_SLOTS;i++){
		if(pdata.chars[i]==NULL || pdata.chars[i]->battleready==BATTLE_UNAVAILABLE)
			num--;
	}

	gen_random_map();

	foe=malloc(num*sizeof(*foe));
	for(i=0;i<num;i++)
		create_character(foe+i);

	ret=start_battle(pdata.chars,foe,num);

	if(ret){
		for(i=0;i<NUM_CHAR_SLOTS;i++)
			if(pdata.chars[i]->battleready==BATTLE_READY)
				jp_reward(pdata.chars[i]);
	}

	free(foe);

	return ret;
}

void run_game(){
	int i;
	char buf[40];
	int x,y;
	int flags;
	int result=1;
	int index;
	gen_areamap(&x,&y);
	while(result){
		area_menu(&x,&y);
		flags=get_area_map(x,y);
		if(flags&AMAP_ENCOUNTER_BIT){
			result=encounter();
			if(!result){
				destroy_game();
				return;
			}
		}

		if(flags&AMAP_TREASURE_BIT){
			index=spawn_item_by_price(50,500);
			sprintf(buf,"Obtained %s!",eq_name(index));
			print_message(buf);
			for(i=0;i<NUM_ITEMS;i++){
				if(pdata.inventory[i].count<=0){
					pdata.inventory[i].count=1;
					pdata.inventory[i].index=index;
					break;
				}
			}
		}

		if(flags&AMAP_EXIT_BIT)
			break;

		explore_areamap(x,y);
	}
}
