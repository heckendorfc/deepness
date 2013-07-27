#ifndef BATTLE_H
#define BATTLE_H

#include <stdint.h>

/*
#define NUM_PREP_STATUS 3
#define NUM_SUPPORT_STATUS 7
#define NUM_NEGATIME_STATUS 18
#define NUM_DAMAGE_STATUS 2
#define NUM_MISC_STATUS 6
*/
#define NUM_STATUS 36
#define NO_DAMAGE ((int16_t)0xFFFF)

#define STATUS_CRITICAL 0
#define STATUS_NOMOVE 1
#define STATUS_NOACT 2

#define MOVED_FLAG BIT(0)
#define ACTED_FLAG BIT(1)

#endif
