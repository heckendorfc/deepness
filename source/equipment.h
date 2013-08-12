#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include "player.h"

#define EQ_TYPE(x) (x&0x3F)

#define WEAP_RANGE(x) (x&0x3F)
#define WRANGE_HEIGHT 0x80
#define WRANGE_ALL 0x40

// oooooootttttllll
#define EQ_INDEX(l,t,o) ((o<<9)|(t<<4)|(l&0x4))

#define EQ_WEAPON 0
#define EQ_OFFHAND 1
#define EQ_HEAD 2
#define EQ_BODY 3
#define EQ_MISC 4
#define EQ_ITEM 5

#define NUM_EQW_TYPES 20
#define MAX_EQW_PER_TYPE 16
/* Weapons */
#define EQW_HANDS 0
#define EQW_AXE 1
#define EQW_BAG 2
#define EQW_BOW 3
#define EQW_CLOTH 4
#define EQW_CROSSBOW 5
#define EQW_DICTIONARY 6
#define EQW_FLAIL 7
#define EQW_GUN 8
#define EQW_SPELLGUN 9
#define EQW_HARP 10
#define EQW_KATANA 11
#define EQW_KNIGHTSWORD 12
#define EQW_KNIFE 13
#define EQW_NINJASWORD 14
#define EQW_ROD 15
#define EQW_SPEAR 16
#define EQW_STAFF 17
#define EQW_STICK 18
#define EQW_SWORD 19

#define NUM_EQO_TYPES 1
#define MAX_EQO_PER_TYPE 17
/* Offhand */
#define EQO_SHIELD 20

#define NUM_EQH_TYPES 4
#define MAX_EQH_PER_TYPE 13
/* Head */
#define EQH_HAT 1
#define EQH_RIBBON 2
#define EQH_HELMET 3


#define NUM_EQB_TYPES 4
#define MAX_EQB_PER_TYPE 14
/* Body */
#define EQB_CLOTHES 1
#define EQB_ARMOR 2
#define EQB_ROBE 3

#define NUM_EQM_TYPES 4
#define MAX_EQM_PER_TYPE 14
/* Misc */
#define EQM_MANTLE 1
#define EQM_ARMWEAR 2
#define EQM_FOOT 3

#define EQWFLAG_TWO_SWORDS BIT(0)
#define EQWFLAG_TWO_HAND BIT(1)
#define EQWFLAG_TWO_HAND_ONLY BIT(2)

#define EQFLAG_NOFOE BIT(7)

int actual_weapon_range(struct battle_char *bc, int x, int y);
int weapon_can_hit(struct battle_char *bc, int x, int y);

extern const weapon_damagef weapon_damage[];
extern const uint8_t weapon_range[];
extern const struct eq_item weapons[NUM_EQW_TYPES][MAX_EQW_PER_TYPE];
extern const struct eq_item offhand[MAX_EQO_PER_TYPE];
extern const struct eq_item body_armor[NUM_EQB_TYPES][MAX_EQB_PER_TYPE];
extern const struct eq_item head_armor[NUM_EQH_TYPES][MAX_EQH_PER_TYPE];
extern const struct eq_item misc_armor[NUM_EQM_TYPES][MAX_EQM_PER_TYPE];

#endif
