#include <stdio.h>
#include <stdlib.h>

#include "player.h"
#include "util.h"
#include "battle.h"

int main(void) {
	pdata.chars[0]=malloc(sizeof(**pdata.chars));
	create_character(pdata.chars[0]);
	while(1){
		//start_battle();
	}
	return 0;
}
