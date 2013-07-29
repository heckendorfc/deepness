#ifndef PLAYER_H
#define PLAYER_H

#include <stdint.h>
#include "classes.h"

#define NUM_ITEMS 100

#define NUM_EQ_SLOTS 5
#define NUM_CHAR_SLOTS 10
#define NUM_ABILITY 5
#define NUM_STATUS 36

#define REACTION(x) (clreaction[x->ch->reaction_class][x->ch->reaction_index])

#define BIT(x) (1<<x)

#define GENDER_MALE 1
#define GENDER_FEMALE 2

#define FOF_FRIEND 1
#define FOF_FOE 2

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

	uint16_t hp_max;
	uint16_t mp_max;

	uint16_t hp;
	uint16_t mp;
	uint8_t pa;
	uint8_t ma;
	uint8_t speed;

	uint8_t wp;

	uint8_t faith;
	uint8_t brave;

	uint8_t ct;

	struct stored_action *slow_act;

	uint8_t status[NUM_STATUS];
};

struct eq_item{
	uint8_t wp;
	uint8_t p_evade;
	uint8_t m_evade;
	uint8_t elvl;
	uint8_t	flags;
	uint8_t elem;
	uint16_t add;
	uint16_t price;
};

typedef void (*actionf)(struct battle_char*,struct battle_char**,int);
typedef int (*reactionf)(struct battle_char*,struct battle_char*);
typedef void (*supportf)(struct battle_char*);
typedef void (*movementf)(struct battle_char*);
typedef int (*weapon_damagef)(const struct eq_item*,struct battle_char*,struct battle_char*);


struct character{
	uint8_t gender;
	uint8_t battleready;
	uint16_t jp;
	uint32_t mastery[NUM_CLASS];
	uint8_t level[NUM_CLASS];
	uint8_t primary; // Abilities
	uint8_t secondary; // Abilities
	uint16_t eq[NUM_EQ_SLOTS];
	uint8_t support;
	//reactionf reaction;
	uint8_t reaction_class;
	uint8_t reaction_index;
	uint8_t reaction_trigger;
	movementf movement;
	uint32_t raw[NUM_STATS];
	uint8_t faith;
	uint8_t brave;
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
