#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include "player.h"

#define EQ_TYPE(x) (x&0x2F)

#define EQ_WEAPON 0
#define EQ_OFFHAND 1
#define EQ_HEAD 2
#define EQ_BODY 3
#define EQ_MISC 4

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

/* Offhand */
#define EQO_SHIELD 20

/* Head */
#define EQH_HAT 1
#define EQH_RIBBON 2
#define EQH_HELMET 3

/* Body */
#define EQB_CLOTHES 1
#define EQB_ARMOR 2
#define EQB_ROBE 3

/* Misc */
#define EQM_MANTLE 1
#define EQM_ARMWEAR 2
#define EQM_FOOT 3

#define EQWFLAG_TWO_SWORDS BIT(0)
#define EQWFLAG_TWO_HAND BIT(1)
#define EQWFLAG_TWO_HAND_ONLY BIT(2)


extern const weapon_damagef weapon_damage[];
extern const struct eq_item weapons[NUM_EQW_TYPES][MAX_EQW_PER_TYPE];

#endif
