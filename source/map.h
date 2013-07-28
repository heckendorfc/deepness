#ifndef MAP_H
#define MAP_H

#define MAP_WIDTH 10
#define MAP_HEIGHT 16

#define MAP_T_NORMAL 0
#define MAP_T_NOSTAND 1
#define MAP_T_NOTARGET 2
#define MAP_T_WATER 3
#define MAP_T_ROCK 4
#define MAP_T_SAND 5

#define MAP_INDEX(x,y) (x+(y*MAP_WIDTH))

uint8_t get_map_height(int x,int y);
uint8_t get_map_terrain(int x,int y);

#endif
