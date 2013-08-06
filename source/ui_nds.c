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
#include "equipment.h"

#include "maptiles.h"
#include "heighttiles.h"
#include "sprites.h"

#define MAP_MODE_HEIGHT 0
#define MAP_MODE_TERRAIN 1

#define CONTROL_MODE_ACTION 0
#define CONTROL_MODE_MAP 1

#define GROUP_SIZE 2
#define NUM_SPRITES 3

#define MENU_ITEMS_PER_PAGE 5

u8 *tileMemory;
u16 *mapMemory;
u8 mapmode;
u8 controlmode;
PrintConsole topScreen;
PrintConsole bottomScreen;
u16 *spider_gfx;
u16 *cursor_gfx;
u16 *hilight_gfx;
char last_msg[20];

const char *terrain_name[]={
	"Normal",
	"",
	"",
	"Water",
	"Rock",
	"Grass",
	"Brick",
	"Wood",
	"Swamp",
	"Sand",
	"Snow",
	"Stone",
};


void set_tiles(int x, int y, int index){
	int i,j;
	int tileindex=(GROUP_SIZE*GROUP_SIZE*(index-1))+1;

	for(j=0;j<GROUP_SIZE;j++){
		for(i=0;i<GROUP_SIZE;i++){
			mapMemory[(GROUP_SIZE*y+j)*32+(GROUP_SIZE*x+i)]=
				index==0?
					0:
					tileindex+j*GROUP_SIZE+i;
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
	int i;
	videoSetMode(MODE_0_2D | DISPLAY_BG0_ACTIVE);
	videoSetModeSub(MODE_0_2D);
	vramSetBankA(VRAM_A_MAIN_BG_0x06000000);
	vramSetBankC(VRAM_C_SUB_BG);
	vramSetBankE(VRAM_E_MAIN_SPRITE);

	oamInit(&oamMain,SpriteMapping_1D_128,false);
	spider_gfx=oamAllocateGfx(&oamMain,SpriteSize_16x16,SpriteColorFormat_16Color);
	cursor_gfx=oamAllocateGfx(&oamMain,SpriteSize_16x16,SpriteColorFormat_16Color);
	hilight_gfx=oamAllocateGfx(&oamMain,SpriteSize_16x16,SpriteColorFormat_16Color);

	consoleInit(&bottomScreen, 3, BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);
	
	tileMemory=(u8*)BG_TILE_RAM(1);
	mapMemory=(u16*)BG_MAP_RAM(0);
		
	REG_BG0CNT = BG_32x32 | BG_COLOR_16 | BG_MAP_BASE(0) | BG_TILE_BASE(1) | BG_PRIORITY(3);

	dmaCopy(&spritesTiles[spritesTilesLen/4/NUM_SPRITES*0],spider_gfx,spritesTilesLen/NUM_SPRITES);
	dmaCopy(&spritesTiles[spritesTilesLen/4/NUM_SPRITES*1],cursor_gfx,spritesTilesLen/NUM_SPRITES);
	dmaCopy(&spritesTiles[spritesTilesLen/4/NUM_SPRITES*2],hilight_gfx,spritesTilesLen/NUM_SPRITES);
	dmaCopy(spritesPal,SPRITE_PALETTE,spritesPalLen);

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
	iprintf("\x1b[11;1H");
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
}

void update_sprite_position(struct battle_char *bc){
	oamSet(&oamMain,bc->index,16*bc->x,16*bc->y,0,0,SpriteSize_16x16,SpriteColorFormat_16Color,spider_gfx,-1,false,false,false,false,false);
	oamUpdate(&oamMain);
}

void update_cursor(int num, int x, int y){
	oamSet(&oamMain,num,16*x,16*y,1,0,SpriteSize_16x16,SpriteColorFormat_16Color,cursor_gfx,-1,false,false,false,false,false);
	oamUpdate(&oamMain);
}

void hide_cursor(int num){
	oamClear(&oamMain,num,1);
	oamUpdate(&oamMain);
}

int show_moves(int num){
	int i,j,index=0;
		
	for(j=0;j<MAP_HEIGHT;j++){
		for(i=0;i<MAP_WIDTH;i++){
			if(move_valid(i,j)){
				index++;
				oamSet(&oamMain,num+index,16*i,16*j,1,0,SpriteSize_16x16,SpriteColorFormat_16Color,hilight_gfx,-1,false,false,false,false,false);
			}
		}
	}

	oamUpdate(&oamMain);

	return index;
}

void hide_moves(int num, int moves){
	oamClear(&oamMain,num+1,moves);
	oamUpdate(&oamMain);
}

void battle_orders(struct battle_char **blist, int bi, int num, uint8_t *flags){
	struct battle_char **tl;
	int cmd,x,y;
	int i;
	int run=1;
	int uid;
	int press;
	int cursorx=1;
	int cursory=0;
	int moves=0;
	int cmdgroup;
	int subcmd;
	int offset;

	controlmode=CONTROL_MODE_ACTION;
	while(1){
		cursory++;
		if(cursory>3)cursory=1;
		if(!((cursory==1 || cursory==2 || cursory==3) && *flags&ACTED_FLAG) && !(cursory==2 && *flags&MOVED_FLAG))
			break;
	}

	for(i=0;i<num;i++){
		oamSet(&oamMain,blist[i]->index,16*blist[i]->x,16*blist[i]->y,0,0,SpriteSize_16x16,SpriteColorFormat_16Color,spider_gfx,-1,false,false,false,false,false);
	}
	update_cursor(num,blist[bi]->x,blist[bi]->y);

	swiWaitForVBlank();

	print_map(blist,bi,num);

	while(run){
		if(controlmode==CONTROL_MODE_ACTION){
			iprintf("\x1b[2J");
			if(cursorx==1)
				iprintf("\x1b[1;1H%c%s\x1b[2;1H%c%s\x1b[3;1H%c%s\x1b[4;1H%c%s\x1b[5;1H%cSkip",
					(cursory==1)?'*':' ',
					(*flags&ACTED_FLAG)==0?"Attack":"",
					(cursory==2)?'*':' ',
					(*flags&ACTED_FLAG)==0?"Primary ->":"",
					(cursory==3)?'*':' ',
					(*flags&ACTED_FLAG)==0?"Secondary ->":"",
					(cursory==4)?'*':' ',
					(*flags&MOVED_FLAG)==0?"Move":"",
					(cursory==5)?'*':' ');
				print_info(blist[bi]);
			else{
				for(i=offset;i-offset<MENU_ITEMS_PER_PAGE && i<num_ability[cmdgroup];i++)
					iprintf("\x1b[%d;1H%s",i-offset+1,clability[cmdgroup][i].name); // TODO: Skip unavailable ones
			}
		}
		else{
			iprintf("\x1b[2J\x1b[1;1HHeight: %d\x1b[2;1HTerrain: %s",
				get_map_height(cursorx,cursory),
				terrain_name[get_map_terrain(cursorx,cursory)]);

			if((i=unit_at(blist,num,cursorx,cursory))>=0)
				print_info(blist[i]);
		}

		scanKeys();
		press=keysDown();

		if(press&(KEY_R|KEY_L)){
			mapmode=!mapmode;
			print_map(blist,bi,num);
		}
		if(press&(KEY_B)){
			controlmode=!controlmode;
			if(controlmode==CONTROL_MODE_ACTION){
				cursorx=1;
				cursory=1;
				//hide_cursor(num);
				update_cursor(num,blist[bi]->x,blist[bi]->y);
			}
			else{
				cursorx=blist[bi]->x;
				cursory=blist[bi]->y;
				update_cursor(num,cursorx,cursory);
			}
		}
		if(press&KEY_UP){
			if(controlmode==CONTROL_MODE_ACTION){
				while(1){
					cursory--;
					if(cursory<1)cursory=5;
					if(!((cursory==1 || cursory==2 || cursory==3) && *flags&ACTED_FLAG) && !(cursory==2 && *flags&MOVED_FLAG))
						break;
				}
			}
			else{
				if(cursory>0)cursory--;
				update_cursor(num,cursorx,cursory);
			}
		}
		if(press&KEY_DOWN){
			if(controlmode==CONTROL_MODE_ACTION){
				while(1){
					cursory++;
					if(cursory>5)cursory=1;
					if(!((cursory==1 || cursory==2 || cursory==3) && *flags&ACTED_FLAG) && !(cursory==2 && *flags&MOVED_FLAG))
						break;
				}
			}
			else{
				if(cursory<MAP_HEIGHT)cursory++;
				update_cursor(num,cursorx,cursory);
			}
		}
		if(press&KEY_LEFT){
			if(controlmode==CONTROL_MODE_ACTION){
				if(cursorx>0)cursorx--;
			}
			else{
				if(cursorx>0)cursorx--;
				update_cursor(num,cursorx,cursory);
			}
		}
		if(press&KEY_RIGHT){
			if(controlmode==CONTROL_MODE_ACTION){
				if(cursorx<2){
					cursorx++;
					cmd=cursory;
					offset=0;
					if(cursory==2)
						cmdgroup=blist[bi]->primary;
					else if(cursory==3)
						cmdgroup=blist[bi]->secondary;
				}
			}
			else{
				if(cursorx<MAP_WIDTH)cursorx++;
				update_cursor(num,cursorx,cursory);
			}
		}
		if(press&KEY_Y){
			if(controlmode==CONTROL_MODE_ACTION){
				if(cursory==5)
					break;
				if(cursorx==1)
					cmd=cursory;
				else
					subcmd=cursory;
				controlmode=!controlmode;
				cursorx=blist[bi]->x;
				cursory=blist[bi]->y;
				update_cursor(num,cursorx,cursory);
				if(cmd==2)
					moves=show_moves(num);
			}
			else{
				switch(cmd){
					case 1:
						if(weapon_can_hit(blist[bi],cursorx,cursory) && !(*flags&ACTED_FLAG)){
							tl=get_targets(blist,num,cursorx,cursory,1,1,0);
							if(tl[0])
								fast_action(blist[bi],tl[0],0,0);
							free(tl);
							*flags|=ACTED_FLAG;
							run=0;
						}
						else
							run=1;
						break;
					case 2:
					case 3:
						if(!(*flags&ACTED_FLAG)){
							if(claction[cmdgroup][subcmd].ctr==0){
								tl=get_targets(blist,num,cursorx,cursory,clability[cmdgroup][subcmd].ra.aoe,clability[cmdgroup][subcmd].ra.aoe_vertical,AOE_DIR(clability[cmdgroup][subcmd].ra.dir));
								for(i=0;i<num && tl[i];i++)
									fast_action(blist[bi],tl[i],subgroup,subcmd);
								free(tl);
							}
							else
								slow_action(blist[bi],cursorx,cursory,subgroup,subcmd);
							*flags|=ACTED_FLAG;
							run=0;
						}
						else
							run=1;
						break;
					case 4:
						if(!(*flags&MOVED_FLAG)){
							run=0;
							if(move(blist[bi],cursorx,cursory)==MOVE_INVALID){
								controlmode=CONTROL_MODE_ACTION;
								update_cursor(num,blist[bi]->x,blist[bi]->y);
								cursory=2;
								run=1;
							}
							else
								*flags|=MOVED_FLAG;
						}
						else{
							run=1;
						}
						hide_moves(num,moves);
						break;
					case 5:
						// no action
						run=0;
						break;
				}
			}
		}
		
		swiWaitForVBlank();
	}
}

void print_message(char *msg){
	//  TODO: Should we sleep here? Keep a message log?
	iprintf("\x1b[0;0H%s",msg);
	swiWaitForVBlank();
}

#endif
