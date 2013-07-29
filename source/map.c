#include <stdint.h>
#include "map.h"

uint8_t height[MAP_WIDTH*MAP_HEIGHT];
uint8_t terrain[MAP_WIDTH*MAP_HEIGHT];

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
		height[i]=0;
		terrain[i]=MAP_T_NORMAL;
	}

	for(i=0;i<5;i++){
		terrain[i]|=MAP_FOE_START;
		terrain[(MAP_WIDTH*MAP_HEIGHT)-(i+1)]|=MAP_FRIEND_START;
	}
}
