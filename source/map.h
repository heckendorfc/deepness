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

#define NUM_MAP_THEME 5

#define MAP_THEME_DESERT 0
#define MAP_THEME_MARSH 1
#define MAP_THEME_MOUNTAIN 2
#define MAP_THEME_RUINS 3
#define MAP_THEME_TOWN 4

#define AMAP_EXPLORED_BIT BIT(0)
#define AMAP_ENCOUNTER_BIT BIT(1)
#define AMAP_TREASURE_BIT BIT(2)
#define AMAP_EXIT_BIT BIT(3)
#define AMAP_NORTH_BIT BIT(4)
#define AMAP_SOUTH_BIT BIT(5)
#define AMAP_EAST_BIT BIT(6)
#define AMAP_WEST_BIT BIT(7)

#define MAP_INDEX(x,y) (x+(y*MAP_WIDTH))

uint8_t get_map_height(int x,int y);
uint8_t get_map_terrain(int x,int y);
uint8_t get_area_map(int x, int y);
uint8_t get_map_start(int x,int y);
void gen_random_map(int theme);
void set_map_moves(int x, int y, int moves, int jump, int flags);
uint8_t move_valid(int x, int y);
void explore_areamap(int x, int y);
void gen_areamap();

#endif
