#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "player.h"
#include "util.h"
#include "battle.h"
#include "map.h"

int main(void) {
	int i;
	struct character foe;
	srand(time(NULL));
	for(i=0;i<NUM_CHAR_SLOTS;i++)pdata.chars[i]=NULL;
	pdata.chars[0]=malloc(sizeof(**pdata.chars));
	create_character(pdata.chars[0]);
	create_character(&foe);
	gen_random_map();
	while(1){
		start_battle(pdata.chars,&foe,1);
	}
	return 0;
}
