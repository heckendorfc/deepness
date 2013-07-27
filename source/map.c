#include <stdint.h>
#include "map.h"

uint8_t height[MAP_WIDTH*MAP_HEIGHT];
uint8_t terrain[MAP_WIDTH*MAP_HEIGHT];

uint8_t get_map_height(int x,int y){
	return height[MAT_INDEX(x,y,MAP_WIDTH)];
}
