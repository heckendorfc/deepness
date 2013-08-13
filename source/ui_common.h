#ifndef UI_COMMON_H
#define UI_COMMON_H

#include "player.h"

void init_ui();
void battle_orders(struct battle_char **blist, int bi, int num, uint8_t *flags);
void print_message(char *);
void edit_menu(struct character **clist, int num);
void area_menu(int *x, int *y);
void world_menu();
void main_menu();

#endif
