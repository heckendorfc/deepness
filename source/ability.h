#ifndef ABILITY_H
#define ABILITY_H

#include <stdint.h>
#include "player.h"

#define BIT(x) (1<<x)

#define NUM_ACTION_PER_ABILITY 16
#define NUM_REACTION_PER_ABILITY 3
#define NUM_SUPPORT_PER_ABILITY 4
#define NUM_MOVEMENT_PER_ABILITY 2

#define AFLAG_COUNTER BIT(0)
#define AFLAG_COUNTER_MAGIC BIT(1)
#define AFLAG_COUNTER_FLOOD BIT(2)
#define AFLAG_EVADE BIT(3)
#define AFLAG_REFLECT BIT(4)
#define AFLAG_MATH BIT(5)
#define AFLAG_PHYSICAL BIT(6)
#define AFLAG_MAGIC BIT(7)

#define RFLAG_REACT_DAMAGE BIT(0)
#define RFLAG_REACT_COUNTER BIT(1)
#define RFLAG_REACT_CRITICAL BIT(2)

#define ELEM_FIRE 1
#define ELEM_ICE 2
#define ELEM_LIGHTNING 3
#define ELEM_WIND 4
#define ELEM_WATER 5
#define ELEM_EARTH 6
#define ELEM_HOLY 7
#define ELEM_DARK 8
#define ELEM_WEAPON 9

//#define RANGE_AUTO 0
#define RANGE_WEAPON 0xFE
#define RANGE_ALL 0xFF

#define RANGE_DIR(x) (x>>4)
#define AOE_DIR(x) (x&0x0F)

struct range_aoe{
	uint8_t range;
	uint8_t range_vertical;
	uint8_t aoe;
	uint8_t aoe_vertical;
	uint8_t dir; // packed: rrrraaaa
};

struct ability{
	union{
		actionf af;
		reactionf rf;
		supportf sf;
		movementf mf;
	}f;
	uint16_t jp; // should we use char and divide jp by 10 for storage?
	uint16_t flags;
	uint8_t elem;
	uint8_t ctr;
	uint8_t mod;
	uint8_t mp;
	struct range_aoe ra;
};

extern const struct ability claction[NUM_CLASS][NUM_ACTION_PER_ABILITY];

#endif
