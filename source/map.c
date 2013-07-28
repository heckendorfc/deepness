#include <stdint.h>
#include "map.h"

uint8_t height[MAP_WIDTH*MAP_HEIGHT];
uint8_t terrain[MAP_WIDTH*MAP_HEIGHT];

uint8_t get_map_height(int x,int y){
	return height[MAP_INDEX(x,y)];
}

uint8_t get_map_terrain(int x,int y){
	return terrain[MAP_INDEX(x,y)];
}
