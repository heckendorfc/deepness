#ifndef MAP_H
#define MAP_H

#define MAP_WIDTH 15
#define MAP_HEIGHT 11

#define MAP_FRIEND_START 1
#define MAP_FOE_START 2

#define MAP_T_NORMAL (1)
#define MAP_T_NOSTAND (2)
#define MAP_T_NOTARGET (3)
#define MAP_T_WATER (4)
#define MAP_T_ROCK (5)
#define MAP_T_GRASS (6)
#define MAP_T_BRICK (7)
#define MAP_T_WOOD (8)
#define MAP_T_SWAMP (9)
#define MAP_T_SAND (10)
#define MAP_T_SNOW (11)
#define MAP_T_STONE (12)

#define MAP_M_UNCHARTED 0
#define MAP_M_INVALID 1
#define MAP_M_VALID 2

#define MAP_INDEX(x,y) (x+(y*MAP_WIDTH))

uint8_t get_map_height(int x,int y);
uint8_t get_map_terrain(int x,int y);
uint8_t get_map_start(int x,int y);
void gen_random_map();
void set_map_moves(int x, int y, int moves, int jump, int flags);
uint8_t move_valid(int x, int y);

#endif
