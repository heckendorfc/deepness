#include <stdlib.h>
#include <stdint.h>
#include "map.h"
#include "ability.h"

static uint8_t height[MAP_WIDTH*MAP_HEIGHT];
static uint8_t terrain[MAP_WIDTH*MAP_HEIGHT];
static uint8_t validmove[MAP_WIDTH*MAP_HEIGHT];
static uint8_t areamap[MAP_WIDTH*MAP_HEIGHT];

const struct map_theme{
	uint8_t base;
	uint8_t options[3];
}themes[]={
	{MAP_T_SAND,{MAP_T_ROCK,MAP_T_ROCK,MAP_T_STONE}},
	{MAP_T_SWAMP,{MAP_T_WATER,MAP_T_WOOD,MAP_T_GRASS}},
	{MAP_T_ROCK,{MAP_T_STONE,MAP_T_GRASS,MAP_T_SNOW}},
	{MAP_T_STONE,{MAP_T_GRASS,MAP_T_WATER,MAP_T_GRASS}},
	{MAP_T_BRICK,{MAP_T_GRASS,MAP_T_WOOD,MAP_T_NORMAL}},
};

uint8_t get_map_height(int x,int y){
	return height[MAP_INDEX(x,y)];
}

uint8_t get_map_start(int x,int y){
	return terrain[MAP_INDEX(x,y)]&0x3;
}

uint8_t get_map_terrain(int x,int y){
	return terrain[MAP_INDEX(x,y)]>>2;
}

uint8_t get_area_map(int x, int y){
	return areamap[MAP_INDEX(x,y)];
}

void gen_random_map(){
	int i,index;
	int theme=rand()%NUM_MAP_THEME;
	int blobs=rand()%3+4;
	int blobi;
	int bwidth;
	int bheight;
	int offset;
	int start;

	for(i=0;i<MAP_WIDTH*MAP_HEIGHT;i++){
		//height[i]=rand()%5;
		height[i]=0;
		//terrain[i]=((rand()%12)+1)<<2;
		terrain[i]=(themes[theme].base)<<2;
	}

	while(blobs>0){
		offset=rand()%(MAP_HEIGHT*MAP_WIDTH);
		blobi=rand()%3;
		bheight=rand()%(MAP_HEIGHT*2/3)+2;
		bwidth=rand()%(MAP_WIDTH/3)+2;

		while(bheight>0){
			start=offset-((bheight/2)*MAP_WIDTH);
			if(start<0){
				bheight--;
				continue;
			}

			index=start-(bwidth/2);
			if(index<0){
				index=0;
			}
			for(i=0;i<bwidth && index<MAP_HEIGHT*MAP_WIDTH;i++){
				terrain[index]=themes[theme].options[blobi]<<2;
				index++;
			}

			if(rand()&1)
				bwidth+=rand()%3;
			else
				bwidth-=rand()%3;

			if(bwidth<0)break;

			bheight--;
		}

		blobs--;
	}

	for(blobs=rand()%7+1;blobs>0;blobs--)
		terrain[rand()%(MAP_HEIGHT*MAP_WIDTH)]=MAP_T_NOSTAND<<2;

	for(i=0;i<5;i++){
		terrain[i]=(themes[theme].base<<2)|MAP_FOE_START;
		terrain[(MAP_WIDTH*MAP_HEIGHT)-(i+1)]=(themes[theme].base<<2)|MAP_FRIEND_START;
	}
}

static void recursive_move_chart(int x, int y, int prevheight, int moves, int jump, int flags){
	uint8_t t;
	uint8_t movemod=1;
	int8_t thisheight;


	thisheight=get_map_height(x,y);
	t=get_map_terrain(x,y);

	if(validmove[MAP_INDEX(x,y)]==MAP_M_VALID)
		goto nextsquare;

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
	if(y<MAP_HEIGHT-1)
		recursive_move_chart(x,y+1,thisheight,moves-movemod,jump,flags);
	if(x<MAP_WIDTH-1)
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

uint8_t get_map_random(int min, int max){
	return (rand()%(max-min))+min;
}

void recursive_maze(uint8_t ax, uint8_t ay, uint8_t bx, uint8_t by){
	uint8_t i;
	uint8_t rx,ry,rsafe,rh;

	if(ax>=bx || ay>=by)
		return;
	
	rx=get_map_random(ax,bx);
	for(i=ay;i<=by;i++){
		areamap[MAP_INDEX(rx,i)]&=~AMAP_EAST_BIT;
		areamap[MAP_INDEX((rx+1),i)]&=~AMAP_WEST_BIT;
	}

	ry=get_map_random(ay,by);
	for(i=ax;i<=bx;i++){
		areamap[MAP_INDEX(i,ry)]&=~AMAP_SOUTH_BIT;
		areamap[MAP_INDEX(i,(ry+1))]&=~AMAP_NORTH_BIT;
	}
	

	rsafe=get_map_random(0,4);

	if(rsafe!=0){
		rh=get_map_random(ay,ry+1);
		areamap[MAP_INDEX(rx,rh)]|=AMAP_EAST_BIT;
		areamap[MAP_INDEX((rx+1),rh)]|=AMAP_WEST_BIT;
	}
	if(rsafe!=1){
		rh=get_map_random(rx+1,bx);
		areamap[MAP_INDEX(rh,ry)]|=AMAP_SOUTH_BIT;
		areamap[MAP_INDEX(rh,(ry+1))]|=AMAP_NORTH_BIT;
	}
	if(rsafe!=2){
		rh=get_map_random(ry+1,by);
		areamap[MAP_INDEX(rx,rh)]|=AMAP_EAST_BIT;
		areamap[MAP_INDEX((rx+1),rh)]|=AMAP_WEST_BIT;
	}
	if(rsafe!=3){
		rh=get_map_random(ax,rx+1);
		areamap[MAP_INDEX(rh,ry)]|=AMAP_SOUTH_BIT;
		areamap[MAP_INDEX(rh,(ry+1))]|=AMAP_NORTH_BIT;
	}

	recursive_maze(ax,ay,rx,ry);
	recursive_maze(rx+1,ay,bx,ry);
	recursive_maze(rx+1,ry+1,bx,by);
	recursive_maze(ax,ry+1,rx,by);
}

void explore_areamap(int x, int y){
	areamap[MAP_INDEX(x,y)]&=~(AMAP_ENCOUNTER_BIT|AMAP_TREASURE_BIT);
	areamap[MAP_INDEX(x,y)]|=AMAP_EXPLORED_BIT;
	if((areamap[MAP_INDEX(x,y)]&AMAP_NORTH_BIT) && y>0)
		areamap[MAP_INDEX(x,(y-1))]|=AMAP_EXPLORED_BIT;
	if((areamap[MAP_INDEX(x,y)]&AMAP_SOUTH_BIT) && y<MAP_HEIGHT-1)
		areamap[MAP_INDEX(x,(y+1))]|=AMAP_EXPLORED_BIT;
	if((areamap[MAP_INDEX(x,y)]&AMAP_EAST_BIT) && x<MAP_WIDTH-1)
		areamap[MAP_INDEX((x+1),y)]|=AMAP_EXPLORED_BIT;
	if((areamap[MAP_INDEX(x,y)]&AMAP_WEST_BIT) && x>0)
		areamap[MAP_INDEX((x-1),y)]|=AMAP_EXPLORED_BIT;
}

void gen_areamap(int *x, int *y){
	int i;
	int numencounter=15;
	int numtreasure=3;
	int coord;
	const uint8_t default_flags=AMAP_NORTH_BIT|AMAP_EAST_BIT|AMAP_SOUTH_BIT|AMAP_WEST_BIT;

	for(i=0;i<MAP_WIDTH*MAP_HEIGHT;i++)
		areamap[i]=default_flags;

	for(i=0;i<MAP_WIDTH;i++){
		areamap[MAP_INDEX(i,0)]&=~AMAP_NORTH_BIT;
		areamap[MAP_INDEX(i,(MAP_HEIGHT-1))]&=~AMAP_SOUTH_BIT;
	}
	for(i=0;i<=MAP_HEIGHT;i++){
		areamap[MAP_INDEX(0,i)]&=~AMAP_WEST_BIT;
		areamap[MAP_INDEX((MAP_WIDTH-1),i)]&=~AMAP_EAST_BIT;
	}

	recursive_maze(0,0,MAP_WIDTH-1,MAP_HEIGHT-1);

	while(numencounter){
		coord=get_map_random(0,MAP_WIDTH*MAP_HEIGHT);
		if(!(areamap[coord]&AMAP_ENCOUNTER_BIT)){
			areamap[coord]|=AMAP_ENCOUNTER_BIT;
			numencounter--;
		}
	}
	while(numtreasure){
		coord=get_map_random(0,MAP_WIDTH*MAP_HEIGHT);
		if(!(areamap[coord]&AMAP_TREASURE_BIT)){
			areamap[coord]|=AMAP_TREASURE_BIT;
			numtreasure--;
		}
	}

	// TODO: Add treasure.

	areamap[MAP_WIDTH*MAP_HEIGHT-1]|=AMAP_EXIT_BIT;
	areamap[0]|=AMAP_EXIT_BIT;
	explore_areamap(MAP_WIDTH-1,MAP_HEIGHT-1);

	*x=MAP_WIDTH-1;
	*y=MAP_HEIGHT-1;
}
