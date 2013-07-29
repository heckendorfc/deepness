#ifndef MAP_H
#define MAP_H

#define MAP_WIDTH 10
#define MAP_HEIGHT 16

#define MAP_FRIEND_START 1
#define MAP_FOE_START 2

#define MAP_T_NORMAL (1<<2)
#define MAP_T_NOSTAND (2<<2)
#define MAP_T_NOTARGET (3<<2)
#define MAP_T_WATER (4<<2)
#define MAP_T_ROCK (5<<2)
#define MAP_T_SAND (6<<2)

#define MAP_INDEX(x,y) (x+(y*MAP_WIDTH))

uint8_t get_map_height(int x,int y);
uint8_t get_map_terrain(int x,int y);
uint8_t get_map_start(int x,int y);
void gen_random_map();

#endif
