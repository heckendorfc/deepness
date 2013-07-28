#ifndef UTIL_H
#define UTIL_H

#include "player.h"

int get_random(int min, int max);
int get_dist(struct battle_char *bc, int x, int y, int w, int v, int d);
struct battle_char** get_targets(struct battle_char *blist, int num, int x, int y, int w, int v, int d);
int unit_at(struct battle_char *blist, int num, int x, int y);

#endif
