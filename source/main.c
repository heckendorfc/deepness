#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "player.h"
#include "util.h"
#include "battle.h"
#include "map.h"
#include "game.h"

#include "ui_common.h"

int main(void) {
	srand(time(NULL));

	init_ui();

	while(1){
		main_menu();
		run_game();
	}

	return 0;
}
