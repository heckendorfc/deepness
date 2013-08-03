#ifdef NDS_MODE
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <nds.h>

#include "ui_common.h"
#include "util.h"
#include "battle.h"
#include "map.h"
#include "ability.h"

#include "maptiles.h"
#include "heighttiles.h"
#include "arrowright.h"

#define MAP_MODE_HEIGHT 0
#define MAP_MODE_TERRAIN 1

#define CONTROL_MODE_ACTION 0
#define CONTROL_MODE_MAP 1

#define GROUP_SIZE 2

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

u8 *tileMemory;
u16 *mapMemory;
u8 mapmode;
u8 controlmode;
PrintConsole topScreen;
PrintConsole bottomScreen;


void set_tiles(int x, int y, int index){
	int i,j;
	int tileindex=(GROUP_SIZE*GROUP_SIZE*(index-1))+1;

	for(j=0;j<GROUP_SIZE;j++){
		for(i=0;i<GROUP_SIZE;i++){
			mapMemory[(GROUP_SIZE*y+j)*32+(GROUP_SIZE*x+i)]=
				index==0?
					0:
					tileindex+j*GROUP_SIZE+i;
					//((GROUP_SIZE*GROUP_SIZE*(index-1))+1)+(j*GROUP_SIZE)+i;
		}
	}
}

void display_terrain(){
	int i,j;

	for(i=0;i<maptilesPalLen/2;i++)
		BG_PALETTE[i]=maptilesPal[i];

	swiCopy(maptilesTiles,tileMemory,maptilesTilesLen);

	for(i=0;i<32*32;i++)
		mapMemory[i]=0;

	for(j=0;j<32/GROUP_SIZE;j++){
		for(i=0;i<32/GROUP_SIZE;i++){
			if(i<MAP_WIDTH && j<MAP_HEIGHT)
				set_tiles(i,j,get_map_terrain(i,j));
			else
				set_tiles(i,j,0);
		}
	}
}

void display_height(){
	int i,j;

	for(i=0;i<heighttilesPalLen/2;i++)
		BG_PALETTE[i]=heighttilesPal[i];

	swiCopy(heighttilesTiles,tileMemory,heighttilesTilesLen);
	

	for(j=0;j<32/GROUP_SIZE;j++){
		for(i=0;i<32/GROUP_SIZE;i++){
			if(i<MAP_WIDTH && j<MAP_HEIGHT && get_map_terrain(i,j)!=MAP_T_NOTARGET)
				set_tiles(i,j,get_map_height(i,j)+1);
			else
				set_tiles(i,j,0);
		}
	}
}

void init_ui(){
	videoSetMode(MODE_0_2D | DISPLAY_BG0_ACTIVE);
	videoSetModeSub(MODE_0_2D);
	vramSetBankA(VRAM_A_MAIN_BG_0x06000000);
	vramSetBankC(VRAM_C_SUB_BG);

	consoleInit(&bottomScreen, 3, BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);
	
	tileMemory=(u8*)BG_TILE_RAM(1);
	mapMemory=(u16*)BG_MAP_RAM(0);
		
	REG_BG0CNT = BG_32x32 | BG_COLOR_16 | BG_MAP_BASE(0) | BG_TILE_BASE(1);
	
	mapmode=MAP_MODE_TERRAIN;
	controlmode=CONTROL_MODE_ACTION;
}

/*
terrain  | height
   0 1 2 |    0 1 2
00 x x x | 00 x x x
01 x x x | 01 x x x
02 x x x | 02 x x x
*/
void print_map(struct battle_char **blist, int bi, int num){
	switch(mapmode){
		case MAP_MODE_TERRAIN:
			display_terrain();
			break;
		case MAP_MODE_HEIGHT:
			display_height();
			break;
	}
}

void print_info(struct battle_char *bc){
	int i;
	int printed=0;
	int printline=0;

	iprintf("\x1b[6;1HUID: %d | DIR: %d | FOF: %d",bc->index,bc->dir,bc->fof);
	iprintf("\x1b[7;1HHP: %d | MP: %d | CT: %d",bc->hp,bc->mp,bc->ct);
	iprintf("\x1b[8;1HPA: %d | MA: %d | WP: %d",bc->pa,bc->ma,bc->wp);
	iprintf("\x1b[9;1HJUMP: %d | MOVE: %d",bc->jump,bc->move);
	iprintf("\x1b[10;1H");
	for(i=0;i<NUM_STATUS;i++){
		if(STATUS_SET(bc,i)){
			iprintf("%02d ",i);
			printed++;
			if(printed%11==0){
				printline+=2;
				iprintf("\x1b[%d;1H",10+printline);
			}
				
		}
	}
	printed=printline=0;
	for(i=0;i<NUM_STATUS;i++){
		if(STATUS_SET(bc,i)){
			if(bc->status[i]==UNTIMED_STATUS)
				iprintf(" - ");
			else
				iprintf("%02d ",bc->status[i]);
			printed++;
			if(printed%11==0){
				printline+=2;
				iprintf("\x1b[%d;1H",10+printline);
			}
				
		}
	}
}

void handle_input(struct battle_char **blist, int bi, int num){
	int press = keysUp();

	if(press&(KEY_R|KEY_L)){
		mapmode=!mapmode;
		print_map(blist,bi,num);
	}
	if(press&(KEY_B)){
		controlmode=!controlmode;
	}

}

void battle_orders(struct battle_char **blist, int bi, int num, uint8_t *flags){
	char buf[100];
	struct battle_char **tl;
	int cmd,x,y;
	int run;
	int uid;

	swiWaitForVBlank();
	print_map(blist,bi,num);

	while(1){
		iprintf("\x1b[2J\x1b[1;1H%s\x1b[2;1H%s\x1b[3;1HSkip\x1b[4;1H",(*flags&ACTED_FLAG)==0?"Attack":"",(*flags&MOVED_FLAG)==0?"Move":"");
		print_info(blist[bi]);

		scanKeys();

		handle_input(blist,bi,num);

		swiWaitForVBlank();
	}
	
	do{
		run=0;
		iprintf("%d | %s, %s, 3:map, 4:info, 5:skip\n:",bi,(*flags&ACTED_FLAG)==0?"1:attack":"",(*flags&MOVED_FLAG)==0?"2:move":"");
		fgets(buf,100,stdin);

		sscanf(buf,"%d:%d:%d",&cmd,&x,&y);

		switch(cmd){
			case 1:
				if(!(*flags&ACTED_FLAG)){
					tl=get_targets(blist,num,x,y,1,1,0);
					fast_action(blist[bi],tl[0],0,0);
					free(tl);
					*flags|=ACTED_FLAG;
				}
				else
					run=1;
				break;
			case 2:
				if(!(*flags&MOVED_FLAG)){
					if(move(blist[bi],x,y)==MOVE_INVALID)
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
		}
	}while(run==1);
}
#endif
