#ifndef UTIL_H
#define UTIL_H

#include "player.h"

int get_random(int min, int max);
int get_dist(struct battle_char *bc, int x, int y, int w, int v, int d);
struct battle_char** get_targets(struct battle_char **blist, int sourcei, int num, int x, int y, int w, int v, int d);
int unit_at(struct battle_char **blist, int num, int x, int y);
void set_base_stats(struct character *ch);
int character_level(struct character *ch);
void level_up(struct character *ch);
void spend_jp_random(struct character *ch);
int job_level(struct character *ch);
void jp_reward(struct character *ch);
void switch_jobs(struct character *ch, int index);
void set_battle_stats(struct battle_char *bc);
void set_level(struct character *ch, int lvl);
void create_character(struct character *ch);
void add_item(uint16_t index);
void remove_eq(struct character *ch, int loc);
void wear_eq(struct character *ch, int offset);

#endif
