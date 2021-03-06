#ifndef PLAYER_H
#define PLAYER_H

#include <stdint.h>
#include "classes.h"

#define NUM_ITEMS 100

#define NUM_EQ_SLOTS 5
#define NUM_CHAR_SLOTS 10
#define NUM_ABILITY 5
#define NUM_STATUS 36
#define NUM_ELEM 9
#define NUM_ADD_STATUS 16

#define REACTION(x) (clreaction[x->ch->reaction_class][x->ch->reaction_index])

#define BIT(x) (1<<x)

#define GENDER_MALE 1
#define GENDER_FEMALE 2

#define FOF_FRIEND 0
#define FOF_FOE 1

#define BATTLE_NOTREADY 0
#define BATTLE_READY 1
#define BATTLE_UNAVAILABLE 2

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

	uint8_t move;
	uint8_t jump;
	
	uint8_t wp;

	uint8_t faith;
	uint8_t brave;

	uint8_t ct;

	struct stored_action *slow_act;

	uint8_t status[NUM_STATUS];
	uint8_t resist[NUM_ELEM];
	uint16_t strengthen;
	uint16_t add_status;

	uint8_t ai_role;
};

typedef void (*eqwearf)(struct battle_char*);
typedef void (*eqremovef)(struct battle_char*);

struct eq_item{
	char *name;
	uint8_t wp;
	union{
		uint8_t evade;
		uint8_t hp;
	}phys;
	union{
		uint8_t evade;
		uint8_t mp;
	}mag;
	uint8_t elvl;
	uint8_t	flags;
	uint8_t elem;
	uint16_t price;
	eqwearf wear;
	eqremovef remove;
};

typedef uint8_t reactret;
typedef uint8_t moveret;

typedef void (*actionf)(struct battle_char*,struct battle_char*);
typedef reactret (*reactionf)(struct battle_char*,struct battle_char*);
typedef void (*supportf)(struct battle_char*);
typedef moveret (*movementf)(struct battle_char*,int);
typedef int (*weapon_damagef)(const struct eq_item*,struct battle_char*,struct battle_char*);

struct character{
	char name[5];
	uint8_t gender;
	uint8_t battleready;
	uint8_t sign;
	uint16_t jp;
	uint32_t mastery[NUM_CLASS];
	uint16_t exp[NUM_CLASS];
	uint8_t primary; // Abilities
	uint8_t secondary; // Abilities
	uint16_t eq[NUM_EQ_SLOTS];
	uint8_t support;
	//reactionf reaction;
	uint8_t reaction_class;
	uint8_t reaction_index;
	uint8_t reaction_trigger;
	uint8_t movement_class;
	uint8_t movement_index;
	uint32_t raw[NUM_STATS];
	uint8_t faith;
	uint8_t brave;
};

struct move_action{
	struct battle_char *target; // Who gets moved
	uint8_t x;
	uint8_t y;
};
struct stored_action{
	uint8_t ctr;
	uint8_t jobindex;
	uint8_t findex;
	actionf f;
	struct battle_char *origin;
	struct aoe target;
	struct move_action move;
};

struct inv_item{
	uint16_t index;
	uint8_t count;
};

struct player{
	struct character *chars[NUM_CHAR_SLOTS];
	struct inv_item inventory[NUM_ITEMS];
	uint8_t d_level;
	uint8_t seed;
}pdata;

#endif
