#ifdef NDS_MODE
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <nds.h>

#include "ui_common.h"
#include "util.h"
#include "battle.h"
#include "map.h"
#include "ability.h"
#include "equipment.h"
#include "save.h"
#include "game.h"

#include "maptiles.h"
#include "heighttiles.h"
#include "sprites.h"
#include "mazetiles.h"

#define MAP_MODE_HEIGHT 0
#define MAP_MODE_TERRAIN 1

#define CONTROL_MODE_ACTION 0
#define CONTROL_MODE_MAP 1
#define CONTROL_MODE_MSG 2

#define GROUP_SIZE 2
#define NUM_SPRITES 6

#define MENU_MAIN 0
#define MENU_JOB 1
#define MENU_SJOB 2
#define MENU_EQ 3
#define MENU_JP 4
#define MENU_JP_ACTION 5
#define MENU_JP_REACTION 6
#define MENU_JP_SUPPORT 7
#define MENU_JP_MOVEMENT 8
#define MENU_EQ_WEAPON 9
#define MENU_EQ_OFFHAND 10
#define MENU_EQ_HEAD 11
#define MENU_EQ_BODY 12
#define MENU_EQ_MISC 13

#define MENU_ITEMS_PER_PAGE 5

#define NUM_MSG 10
#define MAX_MSG_LEN 30

u16 *tileMemory;
u16 *mapMemory;
u8 mapmode;
u8 controlmode;
PrintConsole topScreen;
PrintConsole bottomScreen;
u16 *spider_gfx;
u16 *cursor_gfx;
u16 *hilight_gfx;
u16 *acursor_gfx;
u16 *aunknown_gfx;
u16 *aexit_gfx;
char *last_msg[NUM_MSG];
uint8_t msg_i;

static const char *terrain_name[]={
	"",
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

struct edit_data{
	struct character *ch;
	int group;
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

void set_area_tiles(int x, int y, int flags){
	int i,j;

	if(flags==0){
		for(j=0;j<GROUP_SIZE;j++)
			for(i=0;i<GROUP_SIZE;i++)
				mapMemory[(GROUP_SIZE*y+j)*32+(GROUP_SIZE*x+i)]=0;
		return;
	}

	if(!(flags&AMAP_EXPLORED_BIT)){
		for(j=0;j<GROUP_SIZE;j++)
			for(i=0;i<GROUP_SIZE;i++)
				mapMemory[(GROUP_SIZE*y+j)*32+(GROUP_SIZE*x+i)]=5;
		return;
	}

	if(flags&AMAP_NORTH_BIT){
		if(flags&AMAP_WEST_BIT)
			mapMemory[(GROUP_SIZE*y+0)*32+(GROUP_SIZE*x+0)]=1;
		else
			mapMemory[(GROUP_SIZE*y+0)*32+(GROUP_SIZE*x+0)]=4|TILE_FLIP_H|TILE_FLIP_V;

		if(flags&AMAP_EAST_BIT)
			mapMemory[(GROUP_SIZE*y+0)*32+(GROUP_SIZE*x+1)]=1|TILE_FLIP_H;
		else
			mapMemory[(GROUP_SIZE*y+0)*32+(GROUP_SIZE*x+1)]=4|TILE_FLIP_V;
	}
	else{
		if(flags&AMAP_WEST_BIT)
			mapMemory[(GROUP_SIZE*y+0)*32+(GROUP_SIZE*x+0)]=3|TILE_FLIP_V;
		else
			mapMemory[(GROUP_SIZE*y+0)*32+(GROUP_SIZE*x+0)]=2|TILE_FLIP_H;
		if(flags&AMAP_EAST_BIT)
			mapMemory[(GROUP_SIZE*y+0)*32+(GROUP_SIZE*x+1)]=3|TILE_FLIP_V|TILE_FLIP_H;
		else
			mapMemory[(GROUP_SIZE*y+0)*32+(GROUP_SIZE*x+1)]=2;
	}
	if(flags&AMAP_SOUTH_BIT){
		if(flags&AMAP_WEST_BIT)
			mapMemory[(GROUP_SIZE*y+1)*32+(GROUP_SIZE*x+0)]=1|TILE_FLIP_V;
		else
			mapMemory[(GROUP_SIZE*y+1)*32+(GROUP_SIZE*x+0)]=4|TILE_FLIP_H;
		if(flags&AMAP_EAST_BIT)
			mapMemory[(GROUP_SIZE*y+1)*32+(GROUP_SIZE*x+1)]=1|TILE_FLIP_V|TILE_FLIP_H;
		else
			mapMemory[(GROUP_SIZE*y+1)*32+(GROUP_SIZE*x+1)]=4;
	}
	else{
		if(flags&AMAP_WEST_BIT)
			mapMemory[(GROUP_SIZE*y+1)*32+(GROUP_SIZE*x+0)]=3;
		else
			mapMemory[(GROUP_SIZE*y+1)*32+(GROUP_SIZE*x+0)]=2|TILE_FLIP_H|TILE_FLIP_V;
		if(flags&AMAP_EAST_BIT)
			mapMemory[(GROUP_SIZE*y+1)*32+(GROUP_SIZE*x+1)]=3|TILE_FLIP_H;
		else
			mapMemory[(GROUP_SIZE*y+1)*32+(GROUP_SIZE*x+1)]=2|TILE_FLIP_V;
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

void display_area(){
	int i,j;

	for(i=0;i<mazetilesPalLen/2;i++)
		BG_PALETTE[i]=mazetilesPal[i];

	swiCopy(mazetilesTiles,tileMemory,mazetilesTilesLen);

	for(j=0;j<32/GROUP_SIZE;j++){
		for(i=0;i<32/GROUP_SIZE;i++){
			if(i<MAP_WIDTH && j<MAP_HEIGHT)
				set_area_tiles(i,j,get_area_map(i,j));
			else
				set_area_tiles(i,j,0);
		}
	}
}

void init_ui(){
	int i;
	videoSetMode(MODE_0_2D | DISPLAY_BG0_ACTIVE);
	videoSetModeSub(MODE_0_2D);
	vramSetBankA(VRAM_A_MAIN_BG_0x06000000);
	vramSetBankC(VRAM_C_SUB_BG_0x06200000);
	vramSetBankE(VRAM_E_MAIN_SPRITE);

	oamInit(&oamMain,SpriteMapping_1D_128,false);
	spider_gfx=oamAllocateGfx(&oamMain,SpriteSize_16x16,SpriteColorFormat_16Color);
	cursor_gfx=oamAllocateGfx(&oamMain,SpriteSize_16x16,SpriteColorFormat_16Color);
	hilight_gfx=oamAllocateGfx(&oamMain,SpriteSize_16x16,SpriteColorFormat_16Color);
	acursor_gfx=oamAllocateGfx(&oamMain,SpriteSize_16x16,SpriteColorFormat_16Color);
	aunknown_gfx=oamAllocateGfx(&oamMain,SpriteSize_16x16,SpriteColorFormat_16Color);
	aexit_gfx=oamAllocateGfx(&oamMain,SpriteSize_16x16,SpriteColorFormat_16Color);

	consoleInit(&bottomScreen, 3, BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);

	i=bgInit(0,BgType_Text4bpp,BgSize_T_256x256,0,1);
	mapMemory=bgGetMapPtr(i);
	tileMemory=bgGetGfxPtr(i);
	bgSetPriority(i,3);
/*
	tileMemory=(u16*)BG_TILE_RAM(1);
	mapMemory=(u16*)BG_MAP_RAM(0);
	REG_BG0CNT = BG_32x32 | BG_COLOR_16 | BG_MAP_BASE(0) | BG_TILE_BASE(1) | BG_PRIORITY(3);
*/
	dmaCopy(&spritesTiles[spritesTilesLen/4/NUM_SPRITES*0],spider_gfx,spritesTilesLen/NUM_SPRITES);
	dmaCopy(&spritesTiles[spritesTilesLen/4/NUM_SPRITES*1],cursor_gfx,spritesTilesLen/NUM_SPRITES);
	dmaCopy(&spritesTiles[spritesTilesLen/4/NUM_SPRITES*2],hilight_gfx,spritesTilesLen/NUM_SPRITES);
	dmaCopy(&spritesTiles[spritesTilesLen/4/NUM_SPRITES*3],acursor_gfx,spritesTilesLen/NUM_SPRITES);
	dmaCopy(&spritesTiles[spritesTilesLen/4/NUM_SPRITES*4],aunknown_gfx,spritesTilesLen/NUM_SPRITES);
	dmaCopy(&spritesTiles[spritesTilesLen/4/NUM_SPRITES*5],aexit_gfx,spritesTilesLen/NUM_SPRITES);
	dmaCopy(spritesPal,SPRITE_PALETTE,spritesPalLen);

	mapmode=MAP_MODE_TERRAIN;
	controlmode=CONTROL_MODE_ACTION;
	
	for(i=0;i<NUM_MSG;i++){
		last_msg[i]=malloc(MAX_MSG_LEN);
		if(last_msg[i]==NULL)exit(1);
		*last_msg[i]=0;
	}
	msg_i=0;

	save_init();
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

	if(bc->fof==FOF_FRIEND)
		iprintf("\x1b[20;1HName: %s",bc->ch->name);
	else
		iprintf("\x1b[20;1HName: %02d%d",bc->ch->primary,bc->index);
	
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

int show_action_range(struct battle_char *bc, int num, int group, int subcmd){
	int i,j,index=0;
	int width=claction[group][subcmd].ra.range;

	if(width==RANGE_WEAPON){
		// north
		for(i=bc->y-1;i>=0 && weapon_can_hit(bc,bc->x,i);i--);
		i++;
		oamSet(&oamMain,num+(++index),16*bc->x,16*i,1,0,SpriteSize_16x16,SpriteColorFormat_16Color,hilight_gfx,-1,false,false,false,false,false);
		// east
		for(i=bc->x+1;i<MAP_WIDTH && weapon_can_hit(bc,i,bc->y);i++);
		i--;
		oamSet(&oamMain,num+(++index),16*i,16*bc->y,1,0,SpriteSize_16x16,SpriteColorFormat_16Color,hilight_gfx,-1,false,false,false,false,false);
		// south
		for(i=bc->y+1;i<MAP_HEIGHT && weapon_can_hit(bc,bc->x,i);i++);
		i--;
		oamSet(&oamMain,num+(++index),16*bc->x,16*i,1,0,SpriteSize_16x16,SpriteColorFormat_16Color,hilight_gfx,-1,false,false,false,false,false);
		// west
		for(i=bc->x-1;i>=0 && weapon_can_hit(bc,i,bc->y);i--);
		i++;
		oamSet(&oamMain,num+(++index),16*i,16*bc->y,1,0,SpriteSize_16x16,SpriteColorFormat_16Color,hilight_gfx,-1,false,false,false,false,false);
	}
	else{
		if(bc->y>=width)
			oamSet(&oamMain,num+(++index),16*bc->x,16*(bc->y-width),1,0,SpriteSize_16x16,SpriteColorFormat_16Color,hilight_gfx,-1,false,false,false,false,false);
		if(bc->x>=width)
			oamSet(&oamMain,num+(++index),16*(bc->x-width),16*bc->y,1,0,SpriteSize_16x16,SpriteColorFormat_16Color,hilight_gfx,-1,false,false,false,false,false);
		if(bc->y+width<MAP_HEIGHT)
			oamSet(&oamMain,num+(++index),16*bc->x,16*(bc->y+width),1,0,SpriteSize_16x16,SpriteColorFormat_16Color,hilight_gfx,-1,false,false,false,false,false);
		if(bc->x+width<MAP_WIDTH)
			oamSet(&oamMain,num+(++index),16*(bc->x+width),16*bc->y,1,0,SpriteSize_16x16,SpriteColorFormat_16Color,hilight_gfx,-1,false,false,false,false,false);
	}

	oamUpdate(&oamMain);

	return index;
}

void hide_moves(int num, int moves){
	oamClear(&oamMain,num+1,moves);
	oamUpdate(&oamMain);
}

struct action_data{
	struct battle_char *bc;
	uint8_t group;
};

static int next_action(void *arg, int i){
	struct action_data *ad=(struct action_data*)arg;
	do{
		if(i==NUM_ACTION_PER_ABILITY-1)
			i=0;
		else
			i++;
	}while((ad->bc->ch->mastery[ad->group]&BIT(i))==0);
	return i;
}

static int prev_action(void *arg, int i){
	struct action_data *ad=(struct action_data*)arg;
	do{
		if(i==0)
			i=NUM_ACTION_PER_ABILITY-1;
		else
			i--;
	}while((ad->bc->ch->mastery[ad->group]&BIT(i))==0);
	return i;
}

static char* action_name_cb(int *avail, int *side, int i, void *arg){
	struct action_data *ad=(struct action_data*)arg;
	
	*side=claction[ad->group][i].mp;
	
	*avail=ad->bc->ch->mastery[ad->group]&BIT(i);

	return claction[ad->group][i].name;
}

static void scroll_menu(int offset, int num, char*(*get_menu_item_name)(int*,int*,int,void*),void *arg){
	int i;
	int start=0,end,line=1;
	int avail;
	int side;
	char *s;

	if(offset+1<(MENU_ITEMS_PER_PAGE+1)/2){
		start=0;
		end=((MENU_ITEMS_PER_PAGE+1)/2)+offset;
		if(end>num)end=num;
		line=((MENU_ITEMS_PER_PAGE+1)/2)-offset;
	}
	else if(num-offset<(MENU_ITEMS_PER_PAGE+1)/2){
		start=offset+1-((MENU_ITEMS_PER_PAGE+1)/2);
		end=num-start;
		line=1;
	}
	else{
		line=1;
		start=offset+1-((MENU_ITEMS_PER_PAGE+1)/2);
		end=MENU_ITEMS_PER_PAGE;
	}

	for(i=start;i-start<end;i++){
		s=get_menu_item_name(&avail,&side,i,arg);
		if(avail)
			iprintf("\x1b[%d;1H%c%s",line,(line==(MENU_ITEMS_PER_PAGE+1)/2)?'*':' ',s);
		else
			iprintf("\x1b[%d;1H [%s]",line,s);
		if(side)
			iprintf("  %d",side);
		
		line++;
	}
}

void print_message_list(){
	int i;
	int temp=msg_i;

	for(i=0;i<NUM_MSG;i++){
		if(msg_i==NUM_MSG-1)
			msg_i=0;
		else
			msg_i++;
		iprintf("\x1b[%d;0H%s",i,last_msg[msg_i]);
	}

	msg_i=temp;
}

void battle_orders(struct battle_char **blist, int bi, int num, uint8_t *flags){
	struct action_data ad;
	struct battle_char **tl;
	int cmd=5;
	int i;
	int run=1;
	int press;
	int cursorx=1;
	int cursory=1;
	int moves=0;
	int cmdgroup=0;
	int subcmd=0;
	int offset=0;

	ad.bc=blist[bi];
	ad.group=0;

	controlmode=CONTROL_MODE_ACTION;
	while(1){
		cursory++;
		if(cursory>5)cursory=1;
		if(!((cursory==1 || cursory==2 || cursory==3) && *flags&ACTED_FLAG) && !(cursory==4 && *flags&MOVED_FLAG))
			break;
	}

	for(i=0;i<num;i++){
		oamSet(&oamMain,blist[i]->index,16*blist[i]->x,16*blist[i]->y,0,0,SpriteSize_16x16,SpriteColorFormat_16Color,spider_gfx,-1,false,false,false,false,false);
	}
	update_cursor(num,blist[bi]->x,blist[bi]->y);

	swiWaitForVBlank();

	print_map(blist,bi,num);

	scanKeys();
	press=keysDown();

	swiWaitForVBlank();

	while(run){
		iprintf("\x1b[2J");
		if(controlmode==CONTROL_MODE_MSG){
			print_message_list();
		}
		else if(controlmode==CONTROL_MODE_ACTION){
			if(cursorx==1){
				if(last_msg[msg_i][0])
					iprintf("\x1b[0;0HLast: %s",last_msg[msg_i]);
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
			}
			else{
				scroll_menu(offset,num_action[cmdgroup],action_name_cb,&ad);
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
				if(cursorx==1){
					while(1){
						cursory--;
						if(cursory<1)cursory=5;
						if(!((cursory==1 || cursory==2 || cursory==3) && *flags&ACTED_FLAG) && !(cursory==4 && *flags&MOVED_FLAG))
							break;
					}
				}
				else
					if(offset>0)offset=prev_action(&ad,offset);
			}
			else{
				if(cursory>0)cursory--;
				update_cursor(num,cursorx,cursory);
			}
		}
		if(press&KEY_DOWN){
			if(controlmode==CONTROL_MODE_ACTION){
				if(cursorx==1){
					while(1){
						cursory++;
						if(cursory>5)cursory=1;
						if(!((cursory==1 || cursory==2 || cursory==3) && *flags&ACTED_FLAG) && !(cursory==4 && *flags&MOVED_FLAG))
							break;
					}
				}
				else
					if(offset<num_action[cmdgroup]-1)offset=next_action(&ad,offset);
			}
			else{
				if(cursory<MAP_HEIGHT)cursory++;
				update_cursor(num,cursorx,cursory);
			}
		}
		if(press&KEY_LEFT){
			if(controlmode==CONTROL_MODE_ACTION){
				if(cursorx>1)cursorx--;
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
					if(cursory==2){
						ad.group=cmdgroup=blist[bi]->ch->primary;
					}
					else if(cursory==3){
						ad.group=cmdgroup=blist[bi]->ch->secondary;
					}
				}
			}
			else{
				if(cursorx<MAP_WIDTH)cursorx++;
				update_cursor(num,cursorx,cursory);
			}
		}
		if(press&KEY_X){
			if(controlmode!=CONTROL_MODE_MSG){
				controlmode=CONTROL_MODE_MSG;
			}
			else{
				controlmode=CONTROL_MODE_ACTION;
				cursorx=1;
				cursory=1;
				update_cursor(num,blist[bi]->x,blist[bi]->y);
			}

		}
		if(press&KEY_Y){
			if(controlmode==CONTROL_MODE_ACTION){
				if(cursory==5)
					break;
				if(cursorx==1){
					cmd=cursory;
					cmdgroup=subcmd=0;
				}
				else
					subcmd=offset;
				controlmode=!controlmode;
				cursorx=blist[bi]->x;
				cursory=blist[bi]->y;
				update_cursor(num,cursorx,cursory);
				if(cmd==4)
					moves=show_moves(num);
				else if(cmd<4)
					moves=show_action_range(blist[bi],num,cmdgroup,subcmd);
			}
			else{
				switch(cmd){
					case 1:
						if(weapon_can_hit(blist[bi],cursorx,cursory) && !((*flags)&ACTED_FLAG)){
							tl=get_targets(blist,bi,num,cursorx,cursory,1,1,0);
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
						if(!((*flags)&ACTED_FLAG) && check_action_range(blist[bi],cursorx,cursory,cmdgroup,subcmd)){
							if(claction[cmdgroup][subcmd].ctr==0){
								tl=get_targets(blist,bi,num,cursorx,cursory,claction[cmdgroup][subcmd].ra.aoe,claction[cmdgroup][subcmd].ra.aoe_vertical,AOE_DIR(claction[cmdgroup][subcmd].ra.dir));
								for(i=0;i<num && tl[i];i++)
									fast_action(blist[bi],tl[i],cmdgroup,subcmd);
								free(tl);
							}
							else{
								slow_action(blist[bi],cursorx,cursory,cmdgroup,subcmd);
							}
							*flags|=ACTED_FLAG;
							run=0;
						}
						else
							run=1;
						break;
					case 4:
						if(!(*flags&MOVED_FLAG)){
							run=0;
							if(move(blist,bi,num,cursorx,cursory)==MOVE_INVALID){
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
	hide_moves(num,moves);
	hide_cursor(num);
}

void print_message(char *msg){
	//  TODO: Should we sleep here? Keep a message log?
	msg_i++;
	if(msg_i>=NUM_MSG)msg_i=0;
	strncpy(last_msg[msg_i],msg,MAX_MSG_LEN);
	//snprintf(msg[msg_i],MAX_MSG_LEN,"%s",msg);
	//swiWaitForVBlank();
}

int print_edit_main_menu(int line,int offset){
	int i=line;

	iprintf("\x1b[%d;1H%cChange Job (will reset JP)",i,(line+offset)==i?'*':' '); i++;
	iprintf("\x1b[%d;1H%cSet Secondary",i,(line+offset)==i?'*':' '); i++;
	iprintf("\x1b[%d;1H%cChange Equipment",i,(line+offset)==i?'*':' '); i++;
	iprintf("\x1b[%d;1H%cSpend JP",i,(line+offset)==i?'*':' '); i++;
	iprintf("\x1b[%d;1H%cDone",i,(line+offset)==i?'*':' '); i++;

	return i;
}

int print_edit_jp_menu(int line,int offset){
	int i=line;

	iprintf("\x1b[%d;1H%cAction",i,(line+offset)==i?'*':' '); i++;
	iprintf("\x1b[%d;1H%cReaction",i,(line+offset)==i?'*':' '); i++;
	iprintf("\x1b[%d;1H%cSupport",i,(line+offset)==i?'*':' '); i++;
	iprintf("\x1b[%d;1H%cMovement",i,(line+offset)==i?'*':' '); i++;

	return i;
}

int print_edit_eq_menu(int line,int offset){
	int i=line;

	iprintf("\x1b[%d;1H%cWeapon",i,(line+offset)==i?'*':' '); i++;
	iprintf("\x1b[%d;1H%cOffhand",i,(line+offset)==i?'*':' '); i++;
	iprintf("\x1b[%d;1H%cHead",i,(line+offset)==i?'*':' '); i++;
	iprintf("\x1b[%d;1H%cBody",i,(line+offset)==i?'*':' '); i++;
	iprintf("\x1b[%d;1H%cMisc",i,(line+offset)==i?'*':' '); i++;

	return i;
}

int print_char_info(struct character *ch, int line){
	int i=line;
	int level;
	int j;

	for(j=level=0;j<NUM_CLASS;j++)
		level+=ch->exp[j]/100;

	iprintf("\x1b[%d;0HName: %s (lvl %d)",i,ch->name,level); i++;
	iprintf("\x1b[%d;0HJob: %s (jlvl %d)",i,class_name[ch->primary],ch->exp[ch->primary]/100); i++;
	iprintf("\x1b[%d;0HSecondary: %s",i,class_name[ch->secondary]); i++;
	iprintf("\x1b[%d;0HJP: %d",i,ch->jp); i++;
	//iprintf("\x1b[%d;0H",i); i++;

	return i;
}


static char* class_name_cb(int *avail, int *side, int i, void *arg){
	struct edit_data *ed=(struct edit_data*)arg;
	int j;

	*avail=1;
	for(j=0;j<NUM_CLASS;j++){
		if(ed->ch->exp[j]/100<class_unlocks[i][j+1]){
			*avail=0;
			break;
		}
	}

	*side=0;

	return class_name[i];
}

static char* ability_action_cb(int *avail, int *side, int i, void *arg){
	struct edit_data *ed=(struct edit_data*)arg;

	*side=claction[ed->group][i].jp;

	*avail=(!(MASTERY_ACTION(ed->ch->mastery[ed->group])&BIT(i)) && ed->ch->jp>=*side);

	return claction[ed->group][i].name;
}

static char* ability_reaction_cb(int *avail, int *side, int i, void *arg){
	struct edit_data *ed=(struct edit_data*)arg;

	*side=clreaction[ed->group][i].jp;

	*avail=(!(MASTERY_REACTION(ed->ch->mastery[ed->group])&BIT(i)) && ed->ch->jp>=*side);

	return clreaction[ed->group][i].name;
}

static char* ability_support_cb(int *avail, int *side, int i, void *arg){
	struct edit_data *ed=(struct edit_data*)arg;

	*side=clsupport[ed->group][i].jp;

	*avail=(!(MASTERY_SUPPORT(ed->ch->mastery[ed->group])&BIT(i)) && ed->ch->jp>=*side);

	return clsupport[ed->group][i].name;
}

static char* ability_movement_cb(int *avail, int *side, int i, void *arg){
	struct edit_data *ed=(struct edit_data*)arg;

	*side=clmovement[ed->group][i].jp;

	*avail=(!(MASTERY_MOVEMENT(ed->ch->mastery[ed->group])&BIT(i)) && ed->ch->jp>=*side);

	return clmovement[ed->group][i].name;
}

int get_wearable_inv_index(struct edit_data *ed, int i){
	int reali;
	for(reali=0;reali<NUM_ITEMS;reali++){
		if(EQ_INDEX_L(pdata.inventory[reali].index)==ed->group && char_can_wear(ed->ch,ed->group)){
			i--;
			if(i<0)
				return reali;
		}
	}
	return -1;
}

static char* eq_item_cb(int *avail, int *side, int i, void *arg){
	struct edit_data *ed=(struct edit_data*)arg;
	int reali;

	*avail=1;
	*side=0;
	
	reali=get_wearable_inv_index(ed,i);
	if(reali<0)
		return weapons[0][0].name;

	*side=pdata.inventory[reali].count;

	return eq_name(pdata.inventory[reali].index);
}

int count_inventory_location(struct character *ch, int location){
	int i;
	int ret=0;

	for(i=0;i<NUM_ITEMS;i++){
		if(EQ_INDEX_L(pdata.inventory[i].index)==location && char_can_wear(ch,location))
			ret++;
	}

	return ret;
}


void edit_menu(struct character **clist, int num){
	struct edit_data ed;
	int i;
	int menu=MENU_MAIN;
	int menu_items;
	int offset=0;
	int run=1;
	int ci=0;
	int line=0;
	int press;
	int avail,side;
	int numeq=0;

	swiWaitForVBlank();

	while(clist[ci]->battleready==BATTLE_UNAVAILABLE){
		ci++;
		if(ci>=num)ci=0;
	}

	while(run){
		ed.ch=clist[ci];
		iprintf("\x1b[2J");
		if(menu==MENU_MAIN){
			menu_items=5;
			line=print_edit_main_menu(0,offset);
			line+=2;
			print_char_info(clist[ci],line);
		}
		else if(menu==MENU_JOB || menu==MENU_SJOB){
			ed.group=offset;
			line=MENU_ITEMS_PER_PAGE+2;
			menu_items=NUM_CLASS-1;
			scroll_menu(offset,NUM_CLASS-1,class_name_cb,&ed);
			print_char_info(clist[ci],line);
		}
		else if(menu==MENU_EQ){
			menu_items=5;
			line=print_edit_eq_menu(0,offset);
			line+=2;
			print_char_info(clist[ci],line);
		}
		else if(menu==MENU_EQ_WEAPON){
			ed.group=EQ_WEAPON;
			line=MENU_ITEMS_PER_PAGE+2;
			menu_items=numeq+1;
			scroll_menu(offset,menu_items,eq_item_cb,&ed);
			print_char_info(clist[ci],line);
		}
		else if(menu==MENU_EQ_OFFHAND){
			ed.group=EQ_OFFHAND;
			line=MENU_ITEMS_PER_PAGE+2;
			menu_items=numeq+1;
			scroll_menu(offset,menu_items,eq_item_cb,&ed);
			print_char_info(clist[ci],line);
		}
		else if(menu==MENU_EQ_HEAD){
			ed.group=EQ_HEAD;
			line=MENU_ITEMS_PER_PAGE+2;
			menu_items=numeq+1;
			scroll_menu(offset,menu_items,eq_item_cb,&ed);
			print_char_info(clist[ci],line);
		}
		else if(menu==MENU_EQ_BODY){
			ed.group=EQ_BODY;
			line=MENU_ITEMS_PER_PAGE+2;
			menu_items=numeq+1;
			scroll_menu(offset,menu_items,eq_item_cb,&ed);
			print_char_info(clist[ci],line);
		}
		else if(menu==MENU_EQ_MISC){
			ed.group=EQ_MISC;
			line=MENU_ITEMS_PER_PAGE+2;
			menu_items=numeq+1;
			scroll_menu(offset,menu_items,eq_item_cb,&ed);
			print_char_info(clist[ci],line);
		}
		else if(menu==MENU_JP){
			line=print_edit_jp_menu(0,offset);
			line+=2;
			print_char_info(clist[ci],line);
			menu_items=4;
		}
		else if(menu==MENU_JP_ACTION){
			ed.group=clist[ci]->primary;
			line=MENU_ITEMS_PER_PAGE+2;
			menu_items=num_action[clist[ci]->primary];
			scroll_menu(offset,menu_items,ability_action_cb,&ed);
			print_char_info(clist[ci],line);
		}
		else if(menu==MENU_JP_REACTION){
			ed.group=clist[ci]->primary;
			line=MENU_ITEMS_PER_PAGE+2;
			menu_items=num_reaction[clist[ci]->primary];
			scroll_menu(offset,menu_items,ability_reaction_cb,&ed);
			print_char_info(clist[ci],line);
		}
		else if(menu==MENU_JP_SUPPORT){
			ed.group=clist[ci]->primary;
			line=MENU_ITEMS_PER_PAGE+2;
			menu_items=num_support[clist[ci]->primary];
			scroll_menu(offset,menu_items,ability_support_cb,&ed);
			print_char_info(clist[ci],line);
		}
		else if(menu==MENU_JP_MOVEMENT){
			ed.group=clist[ci]->primary;
			line=MENU_ITEMS_PER_PAGE+2;
			menu_items=num_movement[clist[ci]->primary];
			scroll_menu(offset,menu_items,ability_movement_cb,&ed);
			print_char_info(clist[ci],line);
		}

		scanKeys();
		press=keysDown();

		if(press&KEY_R){
			do{
				ci++;
				if(ci>=num)ci=0;
			}while(clist[ci]->battleready==BATTLE_UNAVAILABLE);
		}
		if(press&KEY_L){
			do{
				ci--;
				if(ci<0)ci=num-1;
			}while(clist[ci]->battleready==BATTLE_UNAVAILABLE);
		}
		if(press&KEY_UP){
			offset--;
			if(offset<0)offset=menu_items-1;
		}
		if(press&KEY_DOWN){
			offset++;
			if(offset>=menu_items)offset=0;
		}
		if(press&KEY_Y){
			if(menu==MENU_MAIN){
				switch(offset){
					case 0:
						menu=MENU_JOB;
						break;
					case 1:
						menu=MENU_SJOB;
						break;
					case 2:
						menu=MENU_EQ;
						break;
					case 3:
						menu=MENU_JP;
						break;
					case 4:
						run=0;
						break;
				}
				offset=0;
			}
			else if(menu==MENU_JOB){
				class_name_cb(&avail,&side,offset,&ed);
				if(avail){
					switch_jobs(clist[ci],offset);
					offset=0;
					menu=MENU_MAIN;
				}
			}
			else if(menu==MENU_SJOB){
				class_name_cb(&avail,&side,offset,&ed);
				if(avail){
					clist[ci]->secondary=offset;
					offset=0;
					menu=MENU_MAIN;
				}
			}
			else if(menu==MENU_JP){
				switch(offset){
					case 0:
						if(num_action[clist[ci]->primary])
							menu=MENU_JP_ACTION;
						break;
					case 1:
						if(num_reaction[clist[ci]->primary])
							menu=MENU_JP_REACTION;
						break;
					case 2:
						if(num_support[clist[ci]->primary])
							menu=MENU_JP_SUPPORT;
						break;
					case 3:
						if(num_movement[clist[ci]->primary])
							menu=MENU_JP_MOVEMENT;
						break;
				}
				offset=0;
			}
			else if(menu==MENU_EQ){
				switch(offset){
					case 0:
						if((numeq=count_inventory_location(clist[ci],EQ_WEAPON)))
							menu=MENU_EQ_WEAPON;
						break;
					case 1:
						if((numeq=count_inventory_location(clist[ci],EQ_OFFHAND)))
							menu=MENU_EQ_OFFHAND;
						break;
					case 2:
						if((numeq=count_inventory_location(clist[ci],EQ_HEAD)))
							menu=MENU_EQ_HEAD;
						break;
					case 3:
						if((numeq=count_inventory_location(clist[ci],EQ_BODY)))
							menu=MENU_EQ_BODY;
						break;
					case 4:
						if((numeq=count_inventory_location(clist[ci],EQ_MISC)))
							menu=MENU_EQ_MISC;
						break;
				}
				offset=0;
			}
			else if(menu==MENU_EQ_WEAPON){
				offset=get_wearable_inv_index(&ed,offset);
				remove_eq(clist[ci],EQ_WEAPON);
				if(offset>=0)
					wear_eq(clist[ci],offset);
				menu=MENU_EQ;
			}	
			else if(menu==MENU_EQ_OFFHAND){
				offset=get_wearable_inv_index(&ed,offset);
				remove_eq(clist[ci],EQ_OFFHAND);
				if(offset>=0)
					wear_eq(clist[ci],offset);
				menu=MENU_EQ;
			}	
			else if(menu==MENU_EQ_HEAD){
				offset=get_wearable_inv_index(&ed,offset);
				remove_eq(clist[ci],EQ_HEAD);
				if(offset>=0)
					wear_eq(clist[ci],offset);
				menu=MENU_EQ;
			}	
			else if(menu==MENU_EQ_BODY){
				offset=get_wearable_inv_index(&ed,offset);
				remove_eq(clist[ci],EQ_BODY);
				if(offset>=0)
					wear_eq(clist[ci],offset);
				menu=MENU_EQ;
			}	
			else if(menu==MENU_EQ_MISC){
				offset=get_wearable_inv_index(&ed,offset);
				remove_eq(clist[ci],EQ_MISC);
				if(offset>=0)
					wear_eq(clist[ci],offset);
				menu=MENU_EQ;
			}	
			else if(menu==MENU_JP_ACTION){
				if(!(MASTERY_ACTION(ed.ch->mastery[ed.ch->primary])&BIT(offset)) && ed.ch->jp>=claction[ed.ch->primary][offset].jp){
					ed.ch->jp-=claction[ed.ch->primary][offset].jp;
					ed.ch->mastery[ed.ch->primary]|=MASTERY_ACTION_BIT(offset);
				}
			}
			else if(menu==MENU_JP_REACTION){
				if(!(MASTERY_REACTION(ed.ch->mastery[ed.ch->primary])&BIT(offset)) && ed.ch->jp>=clreaction[ed.ch->primary][offset].jp){
					ed.ch->jp-=clreaction[ed.ch->primary][offset].jp;
					ed.ch->mastery[ed.ch->primary]|=MASTERY_REACTION_BIT(offset);
				}
			}
			else if(menu==MENU_JP_SUPPORT){
				if(!(MASTERY_SUPPORT(ed.ch->mastery[ed.ch->primary])&BIT(offset)) && ed.ch->jp>=clsupport[ed.ch->primary][offset].jp){
					ed.ch->jp-=clsupport[ed.ch->primary][offset].jp;
					ed.ch->mastery[ed.ch->primary]|=MASTERY_SUPPORT_BIT(offset);
				}
			}
			else if(menu==MENU_JP_MOVEMENT){
				if(!(MASTERY_MOVEMENT(ed.ch->mastery[ed.ch->primary])&BIT(offset)) && ed.ch->jp>=clmovement[ed.ch->primary][offset].jp){
					ed.ch->jp-=clmovement[ed.ch->primary][offset].jp;
					ed.ch->mastery[ed.ch->primary]|=MASTERY_MOVEMENT_BIT(offset);
				}
			}
		}
		if(press&KEY_B){
			menu=MENU_MAIN;
			offset=0;
		}
		
		
		swiWaitForVBlank();
	}
}

void update_area_cursor(int x, int y){
	oamSet(&oamMain,0,16*x,16*y,0,0,SpriteSize_16x16,SpriteColorFormat_16Color,acursor_gfx,-1,false,false,false,false,false);
	oamUpdate(&oamMain);
}

int update_area_unknown(int num){
	int flags;
	int i,j;
	for(j=0;j<MAP_HEIGHT;j++)
		for(i=0;i<MAP_WIDTH;i++){
			flags=get_area_map(i,j);
			if((flags&AMAP_EXPLORED_BIT) && (flags&(AMAP_ENCOUNTER_BIT|AMAP_TREASURE_BIT))){
				oamSet(&oamMain,num++,16*i,16*j,1,0,SpriteSize_16x16,SpriteColorFormat_16Color,aunknown_gfx,-1,false,false,false,false,false);
			}
		}
	oamUpdate(&oamMain);
	return num;
}

int update_area_exits(int num){
	int flags;
	int i,j;
	for(j=0;j<MAP_HEIGHT;j++)
		for(i=0;i<MAP_WIDTH;i++){
			flags=get_area_map(i,j);
			if((flags&AMAP_EXPLORED_BIT) && (flags&(AMAP_EXIT_BIT))){
				oamSet(&oamMain,num++,16*i,16*j,1,0,SpriteSize_16x16,SpriteColorFormat_16Color,aexit_gfx,-1,false,false,false,false,false);
			}
		}
	oamUpdate(&oamMain);
	return num;
}

void area_menu(int *x, int *y){
	int run=1;
	int press;
	int num;
	int numsprites=1;
	int saved=0;
	
	scanKeys();
	press=keysDown();
	display_area();
	update_area_cursor(*x,*y);
	numsprites=update_area_unknown(numsprites);
	numsprites=update_area_exits(numsprites);

	while(run){
		iprintf("\x1b[2J\x1b[1;1HArea Map");
		iprintf("\x1b[3;1HY: Edit units");
		if(saved)
			iprintf("\x1b[5;1H   Saved!");
		else
			iprintf("\x1b[4;1HX: Save");
		
		if(last_msg[msg_i][0])
			iprintf("\x1b[10;0HLast: %s",last_msg[msg_i]);

		scanKeys();
		press=keysDown();

		if(press&KEY_UP){
			if(get_area_map(*x,*y)&AMAP_NORTH_BIT){
				(*y)-=1;
				run=0;
			}
		}
		if(press&KEY_DOWN){
			if(get_area_map(*x,*y)&AMAP_SOUTH_BIT){
				(*y)+=1;
				run=0;
			}
		}
		if(press&KEY_LEFT){
			if(get_area_map(*x,*y)&AMAP_WEST_BIT){
				(*x)-=1;
				run=0;
			}
		}
		if(press&KEY_RIGHT){
			if(get_area_map(*x,*y)&AMAP_EAST_BIT){
				(*x)+=1;;
				run=0;
			}
		}
		if(press&KEY_Y){
			for(num=0;pdata.chars[num];num++);
			edit_menu(pdata.chars,num);
			saved=0;
		}
		if(press&KEY_X){
			save();
			saved=1;
		}

		swiWaitForVBlank();
	}

	oamClear(&oamMain,0,numsprites+1);
	oamUpdate(&oamMain);
}

int print_main_menu(int line, int offset){
	int i=line;

	iprintf("\x1b[%d;1H%cNew Game",i,(line+offset)==i?'*':' '); i++;
	iprintf("\x1b[%d;1H%cLoad Game",i,(line+offset)==i?'*':' '); i++;
	// TODO: Builder/skirmish mode

	return i;
}

void main_menu(){
	int menu_items=2;
	int offset=0;
	int run=1;
	int press;

	swiWaitForVBlank();

	while(run){
		iprintf("\x1b[2J");
			
		iprintf("\x1b[0;0H~DEEPNESS~");

		print_main_menu(5,offset);

		scanKeys();
		press=keysDown();

		if(press&KEY_UP){
			offset--;
			if(offset<0)offset=menu_items-1;
		}
		if(press&KEY_DOWN){
			offset++;
			if(offset>=menu_items)offset=0;
		}
		if(press&KEY_Y){
			switch(offset){
				case 0:
					create_game();
					break;
				case 1:
					load();
					break;
			}
			offset=0;
			run=0;
		}
		
		swiWaitForVBlank();
	}
}

#endif
