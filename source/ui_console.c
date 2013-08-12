#ifdef CONSOLE_MODE
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "ui_common.h"
#include "util.h"
#include "battle.h"
#include "map.h"
#include "ability.h"
#include "equipment.h"
#include "save.h"
#include "game.h"

const char terrain_char[]={
	' ',
	'n',
	'o',
	' ',
	'w',
	'r',
	'g',
	'b',
	'd',
	'p',
	's',
	'*',
	't',
};

void init_ui(){
}

/*
terrain  | height
   0 1 2 |    0 1 2
00 x x x | 00 x x x
01 x x x | 01 x x x
02 x x x | 02 x x x
*/
void print_map(struct battle_char **blist, int bi, int num){
	int i,j;
	int uid;

	fprintf(stderr,"num: %d\n",num);

	for(j=0;j<3;j++){
		printf("   ");
		for(i=0;i<MAP_WIDTH;i++){
			printf("%02d ",i);
		}
		if(j<2)printf(" | ");
	}
	printf("\n");

	for(j=0;j<MAP_HEIGHT;j++){
		printf("%02d ",j);
		for(i=0;i<MAP_WIDTH;i++){
			if((uid=unit_at(blist,num,i,j))>=0)
				printf("%d  ",uid);
			else
				printf("%c  ",terrain_char[get_map_terrain(i,j)]);
		}
		printf(" | ");

		printf("%02d ",j);
		for(i=0;i<MAP_WIDTH;i++){
			if(get_map_terrain(i,j)==MAP_T_NOTARGET)
				printf("   ");
			else
				printf("%d  ",get_map_height(i,j));
		}

		printf(" | ");

		printf("%02d ",j);
		for(i=0;i<MAP_WIDTH;i++){
			if(move_valid(i,j))
				printf("x  ");
			else
				printf("   ");
		}
		printf("\n");
	}
	printf("\n");
}

void print_info(struct battle_char *bc){
	int i;

	printf("UID: %d | DIR: %d | FOF: %d\n",bc->index,bc->dir,bc->fof);
	printf("HP: %d | MP: %d | CT: %d\n",bc->hp,bc->mp,bc->ct);
	printf("PA: %d | MA: %d | WP: %d\n",bc->pa,bc->ma,bc->wp);
	printf("JUMP: %d | MOVE: %d\n",bc->jump,bc->move);
	for(i=0;i<NUM_STATUS;i++)
		printf("%02d ",i);
	printf("\n");
	for(i=0;i<NUM_STATUS;i++)
		printf(" %d ",bc->status[i]);

	printf("\n\n");
}

void print_abilities(int group){
	int i;
	
	for(i=0;i<num_action[group];i++)
		printf("%d: %s\n",i,claction[group][i].name);
}

int run_ability(struct battle_char **blist, int bi, int num, int group, int x, int y, uint8_t *flags){
	struct battle_char **tl;
	int subcmd;
	int i;
	char buf[10];

	print_abilities(group);
	fgets(buf,10,stdin);

	sscanf(buf,"%d",&subcmd);

	if(!((*flags)&ACTED_FLAG) && check_action_range(blist[bi],x,y,group,subcmd)){
		if(claction[group][subcmd].ctr==0){
			tl=get_targets(blist,bi,num,x,y,claction[group][subcmd].ra.aoe,claction[group][subcmd].ra.aoe_vertical,AOE_DIR(claction[group][subcmd].ra.dir));
			for(i=0;i<num && tl[i];i++)
				fast_action(blist[bi],tl[i],group,subcmd);
			free(tl);
		}
		else
			slow_action(blist[bi],x,y,group,subcmd);
		*flags|=ACTED_FLAG;
		return 0;
	}
	else
		return 1;
}

void battle_orders(struct battle_char **blist, int bi, int num, uint8_t *flags){
	char buf[100];
	struct battle_char **tl;
	int cmd,x,y;
	int run;
	int uid;
	
	do{
		run=0;
		printf("%d | %s, %s, 3:map, 4:info, 5:skip, %s, %s\n:",bi,(*flags&ACTED_FLAG)==0?"1:attack":"",(*flags&MOVED_FLAG)==0?"2:move":"",(*flags&ACTED_FLAG)==0?"6:primary":"",(*flags&ACTED_FLAG)==0?"7:secondary":"");
		fgets(buf,100,stdin);

		sscanf(buf,"%d:%d:%d",&cmd,&x,&y);

		switch(cmd){
			case 1:
				if(weapon_can_hit(blist[bi],x,y) && !(*flags&ACTED_FLAG)){
					tl=get_targets(blist,bi,num,x,y,1,1,0);
					if(tl[0])
						fast_action(blist[bi],tl[0],0,0);
					free(tl);
					*flags|=ACTED_FLAG;
				}
				else
					run=1;
				break;
			case 2:
				if(!(*flags&MOVED_FLAG)){
					if(move(blist,bi,num,x,y)==MOVE_INVALID)
						run=1;
					else
						*flags|=MOVED_FLAG;
				}
				else
					run=1;
				break;
			case 3:
				print_map(blist,bi,num);
				run=1;
				break;
			case 4:
				uid=unit_at(blist,num,x,y);
				if(uid>=0)
					print_info(blist[uid]);
				run=1;
				break;
			case 5:
				// no action
				break;
			case 6:
				run=run_ability(blist,bi,num,blist[bi]->ch->primary,x,y,flags);
				break;
			case 7:
				run=run_ability(blist,bi,num,blist[bi]->ch->secondary,x,y,flags);
				break;
		}
	}while(run==1);
}

void edit_menu(struct character **clist, int num){
}

void area_menu(int *x, int *y){
	int i,j;
	char cmd;
	int numchars;
	char buf[100];
	int run;

	for(i=0;pdata.chars[i] && i<NUM_CHAR_SLOTS;i++);
	numchars=i;

	do{
		run=0;
		printf("[N]ext | [E]dit | [S]ave");
		fgets(buf,100,stdin);

		sscanf(buf,"%c",&cmd);

		switch(cmd){
			case 'n':
			case 'N':
				break;
			case 'e':
			case 'E':
				edit_menu(pdata.chars,numchars);
				run=1;
				break;
			case 's':
			case 'S':
				save();
				run=1;
				break;
			default:
				run=1;
				break;
		}
	}while(run);

	if(*x==MAP_HEIGHT-1 && *y==MAP_WIDTH-1)
		*x=*y=0;

	for(j=*y;j<MAP_HEIGHT;j++)
		for(i=*x+1;i<MAP_WIDTH;i++)
			if(get_area_map(i,j)&(AMAP_ENCOUNTER_BIT|AMAP_TREASURE_BIT)){
				*x=i;
				*y=j;
			}


}

void main_menu(){
	char cmd;
	char buf[100];
	int run;

	do{
		run=0;
		printf("[N]ew Game | [L]oad game");
		fgets(buf,100,stdin);

		sscanf(buf,"%c",&cmd);

		switch(cmd){
			case 'n':
			case 'N':
				create_game();
				break;
			case 'l':
			case 'L':
				load();
				break;
			default:
				run=1;
				break;
		}
	}while(run);
}


void print_message(char *msg){
	printf("%s\n",msg);
}

#endif
