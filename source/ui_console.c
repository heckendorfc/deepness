#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "ui_console.h"

void battle_orders(struct battle_char blist, int bi, int num, uint8_t *flags){
	char buf[100];
	struct battle_char **tl;
	int cmd,x,y;
	
	printf(":");
	fgets(buf,100,stdin);

	sscanf(buf,"%d:%d:%d",&cmd,&x,&y);

	switch(buf[0]){
		case 1:
			tl=get_targets(blist,num,x,y,1,1);
			attack(blist+bi,tl[0]);
			free(tl);
			break;
	}

}
