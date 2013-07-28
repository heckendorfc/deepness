#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "ui_console.h"
#include "util.h"
#include "battle.h"
#include "map.h"

const char terrain_char[]={
	'n',
	'o',
	' ',
	'w',
	'r',
	's',
};
/*
terrain  | height
   0 1 2 |    0 1 2
00 x x x | 00 x x x
01 x x x | 01 x x x
02 x x x | 02 x x x
*/
void print_map(struct battle_char **blist, int bi, int num){
	int i,j;
	int uid;

	for(j=0;j<2;j++){
		printf("   ");
		for(i=0;i<MAP_WIDTH;i++){
			printf("%d ",i);
		}
		printf("| ");
	}

	for(j=0;j<MAP_HEIGHT;j++){
		printf("%02d ",i);
		for(i=0;i<MAP_WIDTH;i++){
			if((uid=unit_at(blist,num,i,j))>=0)
				printf("%d ",uid);
			else
				printf("%c ",terrain_char[get_map_terrain(i,j)]);
		}
		printf(" | ");

		printf("%02d ",i);
		for(i=0;i<MAP_WIDTH;i++){
			if(get_map_terrain(i,j)==MAP_T_NOTARGET)
				printf("  ");
			else
				printf("%d ",get_map_height(i,j));
		}
		printf("\n");
	}
	printf("\n");
}

void print_info(struct battle_char *bc){
	int i;

	printf("UID: %d | DIR: %d | FOF: %c\n",bc->index,bc->dir,bc->fof);
	printf("HP: %d | MP: %d | CT: %d",bc->hp,bc->mp,bc->ct);
	for(i=0;i<NUM_STATUS;i++)
		printf("%02d ",i);
	printf("\n");
	for(i=0;i<NUM_STATUS;i++)
		printf(" %d ",bc->status[i]);

	printf("\n\n");
}

void battle_orders(struct battle_char **blist, int bi, int num, uint8_t *flags){
	char buf[100];
	struct battle_char **tl;
	int cmd,x,y;
	int run;
	int uid;
	
	do{
		run=0;
		printf("1:attack, 2:move, 3:map, 4:info, 5:skip\n:");
		fgets(buf,100,stdin);

		sscanf(buf,"%d:%d:%d",&cmd,&x,&y);

		switch(buf[0]){
			case 1:
				if(!(*flags&ACTED_FLAG)){
					tl=get_targets(blist,num,x,y,1,1,0);
					attack(blist[bi],tl[0]);
					free(tl);
					*flags|=ACTED_FLAG;
				}
				else
					run=1;
				break;
			case 2:
				if(!(*flags&MOVED_FLAG)){
					*flags|=MOVED_FLAG;
				}
				else
					run=1;
				break;
			case 3:
				print_map(blist,bi,num);
				run=1;
				break;
			case 4:
				uid=unit_at(blist,num,x,y);
				if(uid>=0)
					print_info(blist[uid]);
				run=1;
				break;
			case 5:
				// no action
				break;
		}
	}while(run==1);
}
