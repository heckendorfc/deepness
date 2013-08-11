#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "player.h"
#include "util.h"
#include "battle.h"
#include "map.h"

#include "ui_common.h"

static const char *names[]={
	"merb",
	"Bolo",
	"x4nt",
	"Bkt\0",
};

int main(void) {
	const int numchar=2;
	int i,j;
	struct character foe[numchar];
	srand(time(NULL));
	for(i=0;i<NUM_CHAR_SLOTS;i++)pdata.chars[i]=NULL;
	for(i=0;i<numchar;i++){
		pdata.chars[i]=malloc(sizeof(**pdata.chars));
		create_character(pdata.chars[i]);
		for(j=0;names[i][j];j++)
			pdata.chars[i]->name[j]=names[i][j];
		pdata.chars[i]->name[4]=0;

		create_character(foe+i);
	}
	gen_random_map();

	init_ui();

	edit_menu(pdata.chars,numchar);

	while(1){
		start_battle(pdata.chars,foe,2);
	}
	return 0;
}
