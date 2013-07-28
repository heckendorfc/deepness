#ifndef PLAYER_H
#define PLAYER_H

#include "battle.h"
#include "classes.h"

#define NUM_ITEMS 100

#define NUM_EQ_SLOTS 5
#define NUM_CHAR_SLOTS 10
#define NUM_ABILITY 5

struct character;

struct aoe{
	uint8_t x;
	uint8_t y;
	uint8_t width;
	uint8_t vertical;
	uint8_t dir; //directionality;
};

struct stored_action;

struct action_result{
	struct stored_action *preresolve;
	int16_t damage;
	uint8_t mp_used;
	int item;
}last_action;

struct battle_char{
	struct character *ch;
	uint8_t fof; // Friend or Foe

	uint8_t index;

	uint8_t x;
	uint8_t y;
	uint8_t dir;

	uint16_t hp;
	uint16_t mp;

	uint8_t pa;
	uint8_t ma;
	uint8_t speed;
	uint8_t wp;

	uint16_t faith;
	uint16_t brave;

	uint8_t ct;

	struct stored_action *slow_act;

	uint8_t status[NUM_STATUS];
};

struct eq_item{
	uint8_t wp;
	uint8_t evade;
	uint8_t elvl;
	uint8_t	flags;
	uint8_t elem;
	uint16_t add;
	uint16_t price;
};

typedef void (*actionf)(struct battle_char*,struct battle_char**,int);
typedef void (*reactionf)(struct battle_char*,struct battle_char*);
typedef void (*supportf)(struct battle_char*);
typedef void (*movementf)(struct battle_char*);
typedef int (*weapon_damagef)(const struct eq_item*,struct battle_char*,struct battle_char*);


struct character{
	char gender;
	char battleready;
	int jp[NUM_CLASS];
	char primary;
	char secondary;
	int eq[NUM_EQ_SLOTS];
	uint8_t support;
	reactionf reaction;
	movementf movement;
	uint8_t reaction_trigger;
};

struct stored_action{
	uint8_t ctr;
	uint8_t jobindex;
	uint8_t findex;
	actionf f;
	struct battle_char *origin;
	struct aoe target;
	//struct battle_char **target;
	//uint8_t num_target;
};

struct player{
	struct character *chars[NUM_CHAR_SLOTS];
	char inventory[NUM_ITEMS];
}pdata;

#endif
