#ifndef BATTLE_H
#define BATTLE_H

#include <stdint.h>

#include "player.h"
#include "ability.h"

/*
#define NUM_PREP_STATUS 3
#define NUM_SUPPORT_STATUS 7
#define NUM_NEGATIME_STATUS 18
#define NUM_DAMAGE_STATUS 2
#define NUM_MISC_STATUS 6
*/
#define NO_DAMAGE ((int16_t)0xFFFF)


#define UNTIMED_STATUS ((uint8_t)0xFF)
#define IMMUNE_TO_STATUS ((uint8_t)0xFE)

#define STATUS_SET(bc,s) ((bc->status[s]==IMMUNE_TO_STATUS || bc->status[s]==0)?0:1)

#define STATUS_NOMOVE 0
#define STATUS_NOACT 1
#define STATUS_DARKNESS 2
#define STATUS_UNDEAD 3
#define STATUS_SLEEPING 4
#define STATUS_POLYMORPH 5
#define STATUS_SLOW 6
#define STATUS_REGEN 7
#define STATUS_CONFUSION 8
#define STATUS_PETRIFY 9
#define STATUS_POISON 10
#define STATUS_SLEEP 11
#define STATUS_SILENCE 12
#define STATUS_DEATHSENTENCE 13
#define STATUS_INNOCENT 14
#define STATUS_FAITH 15
/* Weapons can't add status beyond here */
#define STATUS_STOP 16
#define STATUS_OIL 17
#define STATUS_ATTACKUP 18
#define STATUS_CHARM 19
#define STATUS_SHELL 20
#define STATUS_HASTE 21
#define STATUS_FLOAT 22
#define STATUS_PROTECT 23
#define STATUS_DEFENSEUP 24
#define STATUS_REFLECT 25
#define STATUS_RERAISE 26
#define STATUS_TRANSPARENT 27
#define STATUS_BERSERK 28
#define STATUS_QUICK 29
#define STATUS_CRITICAL 30
#define STATUS_DEAD 31
#define STATUS_PERFORMING 32
#define STATUS_CHARGING 33
#define STATUS_DEFENDING 34

#define MOVED_FLAG BIT(0)
#define ACTED_FLAG BIT(1)

#define ATTACK_DIR_FRONT 1
#define ATTACK_DIR_SIDE 2
#define ATTACK_DIR_REAR 3

#define DIR_NORTH 1
#define DIR_EAST 2
#define DIR_SOUTH 3
#define DIR_WEST 4

#define SIGN_ARIES 0
#define SIGN_TAURUS 1
#define SIGN_GEMINI 2
#define SIGN_CANCER 3
#define SIGN_LEO 4
#define SIGN_VIRGO 5
#define SIGN_LIBRA 6
#define SIGN_SCORPIO 7
#define SIGN_SAGITTARIUS 8
#define SIGN_CAPRICORN 9
#define SIGN_AQUARIUS 10
#define SIGN_PISCES 11
#define SIGN_SERPENTARIUS 12

#define SIGN_COMPAT_WORST 1
#define SIGN_COMPAT_BAD 2
#define SIGN_COMPAT_NEUTRAL 3
#define SIGN_COMPAT_GOOD 4
#define SIGN_COMPAT_BEST 5

#define RESIST_NEUTRAL BIT(0)
#define RESIST_WEAK BIT(1)
#define RESIST_HALF BIT(2)
#define RESIST_ABSORB BIT(3)

#define MOVE_INVALID 0
#define MOVE_SUCCESS 1
#define MOVE_FAILURE 2

void remove_status(struct battle_char *bc, int status);
void add_status(struct battle_char *bc, int status);
int sign_compat(struct character*,struct character*);
int move(struct battle_char *bc, int x, int y);
int evaded(struct battle_char *target, int type, int dir, int base_hit);
int get_attack_dir(struct battle_char *attacker, struct battle_char *defender);
void deal_damage(struct battle_char *bc, int16_t dmg);
void react(struct battle_char *attacker, struct battle_char **reacter, int num);
int should_react(struct battle_char *ch);
void fast_action(struct battle_char *source, struct battle_char *target, int jobindex, int findex);
void start_battle(struct character **friends, struct character *foes, int numfoe);

#endif
