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
	pdata.d_level=1;
	for(i=0;i<NUM_CHAR_SLOTS;i++)pdata.chars[i]=NULL;
	for(i=0;i<numchar;i++){
		pdata.chars[i]=malloc(sizeof(**pdata.chars));
		create_character(pdata.chars[i]);
		for(j=0;names[i][j];j++)
			pdata.chars[i]->name[j]=names[i][j];
		pdata.chars[i]->name[4]=0;
		if(i<2){
			pdata.chars[i]->battleready=BATTLE_READY;
			set_level(pdata.chars[i],1);
		}
		else
			pdata.chars[i]->battleready=BATTLE_UNAVAILABLE;
	}
	for(i=0;i<NUM_ITEMS;i++){
		pdata.inventory[i].count=0;
		pdata.inventory[i].index=EQ_INDEX(9,0,0);
	}
}

static int encounter(int theme){
	struct character *foe;
	int ret,i,num=NUM_CHAR_SLOTS;
	int plvl=0;

	for(i=0;i<NUM_CHAR_SLOTS;i++){
		if(pdata.chars[i]==NULL || pdata.chars[i]->battleready==BATTLE_UNAVAILABLE)
			num--;
		else if(plvl<(ret=character_level(pdata.chars[i])))
			plvl=ret;
	}

	gen_random_map(theme);

	foe=malloc(num*sizeof(*foe));
	for(i=0;i<num;i++){
		create_character(foe+i);
		set_level(foe+i,get_random(plvl-(plvl/8),plvl));
		foe[i].jp=pdata.d_level*100;
		spend_jp_random(foe+i);
	}

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
	char buf[40];
	int x,y;
	int flags;
	int result=1;
	int index;
	int theme;
	while(1){ // World map
		gen_areamap(&x,&y);
		theme=get_random(0,NUM_MAP_THEME);
		while(1){ // Area map
			area_menu(&x,&y);
			flags=get_area_map(x,y);
			if(flags&AMAP_ENCOUNTER_BIT){
				result=encounter(theme);
				if(!result){
					destroy_game();
					return;
				}
			}

			if(flags&AMAP_TREASURE_BIT){
				index=spawn_item_by_price(50,500);
				add_item(index);
				sprintf(buf,"Obtained %s!",eq_name(index));
				print_message(buf);
			}

			if(flags&AMAP_EXIT_BIT)
				break;

			explore_areamap(x,y);
		}
		world_menu();
	}
}
