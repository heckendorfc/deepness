#ifndef ABILITY_H
#define ABILITY_H

#include <stdint.h>
#include "player.h"

#define MASTERY_ACTION(x) (x&0xFFFF)
#define MASTERY_REACTION(x) ((x>>16)&0x4)
#define MASTERY_SUPPORT(x) ((x>>20)&0x8)
#define MASTERY_MOVEMENT(x) ((x>>24)&0x2)

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

#define AFLAG_MOD_PA 0xFF
#define AFLAG_MOD_MA 0xFE
#define AFLAG_MOD_XA 0xFD

#define RFLAG_TRIGGER_DAMAGE 0
#define RFLAG_TRIGGER_COUNTER 1
#define RFLAG_TRIGGER_CRITICAL 2
#define RFLAG_TRIGGER_ALWAYS 3
#define RFLAG_TRIGGER_OTHER 4

#define RFLAG_BRAVE_PERCENT BIT(0)
#define RFLAG_HITMOD BIT(1)
#define RFLAG_EVADEMOD BIT(2)
#define RFLAG_WEAPONEVADE BIT(3)

#define SFLAG_EQUIP_AXE 1
#define SFLAG_DEFEND 2
#define SFLAG_JPUP 3
#define SFLAG_MONSTER_SKILL 4
#define SFLAG_EQUIP_ARMOR 5
#define SFLAG_EQUIP_SHIELD 6
#define SFLAG_EQUIP_SWORD 7
#define SFLAG_EQUIP_CROSSBOW 8
#define SFLAG_CONCENTRATE 9
#define SFLAG_MARTIAL_ARTS 10
#define SFLAG_ATTACKUP 11
#define SFLAG_EQUIP_SPEAR 12
#define SFLAG_MAGIC_DEFENDUP 13
#define SFLAG_MAGIC_ATTACKUP 14
#define SFLAG_THROW_ITEM 15
#define SFLAG_MAINTENANCE 16
#define SFLAG_EQUIP_CHANGE 17
#define SFLAG_SECRET_HUNT 18
#define SFLAG_DEFENSEUP 19
#define SFLAG_SHORTCHARGE 20
#define SFLAG_HALFMP 21
#define SFLAG_EQUIP_KNIFE 22
#define SFLAG_TWO_HANDS 23
#define SFLAG_TWO_SWORDS 24
#define SFLAG_EXPUP 25
#define SFLAG_EQUIP_GUN 26
#define SFLAG_MONSTER_TALK 27
#define SFLAG_TRAIN 28

#define ELEM_NEUTRAL 0
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
		//supportf sf;
		movementf mf;
	}f;
	uint16_t jp; // should we use char and divide jp by 10 for storage?
	uint16_t flags;
	uint8_t elem;
	uint8_t ctr;
	uint8_t mod; // Which formula to use
	uint8_t mod_v;
	uint8_t mp;
	struct range_aoe ra;
};

struct reaction_ability{
	reactionf rf;
	uint16_t jp;
	uint8_t trigger;
	uint8_t flags;
};

struct support_ability{
	uint16_t jp;
	uint16_t flags;
};

uint8_t mod1(struct battle_char *origin, struct battle_char *target, uint8_t var);
uint16_t mod2(struct battle_char *attacker, struct battle_char *defender, uint16_t xa);
uint8_t mod3(struct battle_char *origin, struct battle_char *target, uint8_t var);
uint8_t mod4(struct battle_char *origin, struct battle_char *target, uint8_t var);
int16_t mod5(struct battle_char *origin, struct battle_char *target, uint8_t var, uint8_t success);
uint8_t mod6(struct battle_char *origin, struct battle_char *target, uint8_t var);

extern const struct ability claction[NUM_CLASS][NUM_ACTION_PER_ABILITY];
extern const uint8_t num_support[];
extern const struct support_ability clsupport[NUM_CLASS][NUM_SUPPORT_PER_ABILITY];
extern const uint8_t num_reaction[];
extern const struct reaction_ability clreaction[NUM_CLASS][NUM_REACTION_PER_ABILITY];

#endif
