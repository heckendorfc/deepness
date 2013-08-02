#include <stdlib.h>
#include <stdint.h>
#include "map.h"
#include "ability.h"

uint8_t height[MAP_WIDTH*MAP_HEIGHT];
uint8_t terrain[MAP_WIDTH*MAP_HEIGHT];
uint8_t validmove[MAP_WIDTH*MAP_HEIGHT];

uint8_t get_map_height(int x,int y){
	return height[MAP_INDEX(x,y)];
}

uint8_t get_map_start(int x,int y){
	return terrain[MAP_INDEX(x,y)]&0x3;
}

uint8_t get_map_terrain(int x,int y){
	return terrain[MAP_INDEX(x,y)]>>2;
}

void gen_random_map(){
	int i;

	for(i=0;i<MAP_WIDTH*MAP_HEIGHT;i++){
		height[i]=rand()%5;
		terrain[i]=((rand()%12)+1)<<2;
	}

	for(i=0;i<5;i++){
		terrain[i]=(MAP_T_NORMAL<<2)|MAP_FOE_START;
		terrain[(MAP_WIDTH*MAP_HEIGHT)-(i+1)]=(MAP_T_NORMAL<<2)|MAP_FRIEND_START;
	}
}

static void recursive_move_chart(int x, int y, int prevheight, int moves, int jump, int flags){
	uint8_t t;
	uint8_t movemod=1;
	int8_t thisheight;


	thisheight=get_map_height(x,y);

	if(validmove[MAP_INDEX(x,y)]!=MAP_M_UNCHARTED)
		goto nextsquare;

	t=get_map_terrain(x,y);

	if(t==MAP_T_NOSTAND || t==MAP_T_NOTARGET){
		validmove[MAP_INDEX(x,y)]=MAP_M_INVALID;
		if(!(flags==MFLAG_FLOAT || flags==MFLAG_FLY))
			return;
	}
	else{
		if(abs(prevheight-thisheight)>jump && !(flags==MFLAG_IGNOREHEIGHT || flags==MFLAG_FLY)){
			validmove[MAP_INDEX(x,y)]=MAP_M_INVALID;
			return;
		}
		validmove[MAP_INDEX(x,y)]=MAP_M_VALID;
	}

nextsquare:	
	if(t==MAP_T_WATER)
		movemod=2;

	if(movemod>moves)
		return;

	if(x>0)
		recursive_move_chart(x-1,y,thisheight,moves-movemod,jump,flags);
	if(y>0)
		recursive_move_chart(x,y-1,thisheight,moves-movemod,jump,flags);
	if(y<MAP_HEIGHT)
		recursive_move_chart(x,y+1,thisheight,moves-movemod,jump,flags);
	if(x<MAP_WIDTH)
		recursive_move_chart(x+1,y,thisheight,moves-movemod,jump,flags);
}

void set_map_moves(int x, int y, int moves, int jump, int flags){
	int i;
	int jumpmod=0;

	if(flags==MFLAG_FLOAT)
		jumpmod=2;

	for(i=0;i<MAP_WIDTH*MAP_HEIGHT;i++)
		validmove[i]=MAP_M_UNCHARTED;

	recursive_move_chart(x,y,get_map_height(x,y),moves,jump+jumpmod,flags);
}

uint8_t move_valid(int x, int y){
	if(validmove[MAP_INDEX(x,y)]==MAP_M_VALID)
		return 1;
	return 0;
}
