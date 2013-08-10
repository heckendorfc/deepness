#ifndef AI_H
#define AI_H

#include <stdint.h>
#include "player.h"

#define AI_ROLE_DAMAGE BIT(0)
#define AI_ROLE_HEAL BIT(1)
#define AI_ROLE_BUFF BIT(2) 
#define AI_ROLE_DEBUFF BIT(3)
#define AI_ROLE_MELEE BIT(4)

void ai_battle_orders(struct battle_char **blist, int bi, int num, uint8_t *flags);
void ai_unit_init(struct battle_char *bc);
void ai_init(struct battle_char **blist, int num);

#endif
