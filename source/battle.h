#ifndef BATTLE_H
#define BATTLE_H

#include <stdint.h>

#include "player.h"

/*
#define NUM_PREP_STATUS 3
#define NUM_SUPPORT_STATUS 7
#define NUM_NEGATIME_STATUS 18
#define NUM_DAMAGE_STATUS 2
#define NUM_MISC_STATUS 6
*/
#define NO_DAMAGE ((int16_t)0xFFFF)

#define STATUS_CRITICAL 0
#define STATUS_NOMOVE 1
#define STATUS_NOACT 2
#define STATUS_ATTACKUP 3
#define STATUS_BERSERK 4
#define STATUS_DEFENSEUP 5
#define STATUS_PROTECT 6
#define STATUS_CHARGING 7
#define STATUS_SLEEPING 8
#define STATUS_POLYMORPH 9

#define MOVED_FLAG BIT(0)
#define ACTED_FLAG BIT(1)

void attack(struct battle_char *s, struct battle_char *d);

#endif
