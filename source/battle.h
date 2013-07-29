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

#define UNTIMED_STATUS ((uint8_t)0xFF)

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
#define STATUS_DEAD 10

#define MOVED_FLAG BIT(0)
#define ACTED_FLAG BIT(1)

#define ATTACK_DIR_FRONT 1
#define ATTACK_DIR_SIDE 2
#define ATTACK_DIR_REAR 3

#define DIR_NORTH 1
#define DIR_EAST 2
#define DIR_SOUTH 3
#define DIR_WEST 4

int evaded(struct battle_char *target, int type, int dir, int base_hit);
int get_attack_dir(struct battle_char *attacker, struct battle_char *defender);
void deal_damage(struct battle_char *bc, uint16_t dmg);
void react(struct battle_char *attacker, struct battle_char **reacter, int num);
int should_react(struct battle_char *ch);
void start_battle(struct character **friends, struct character *foes, int numfoe);

#endif
