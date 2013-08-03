#ifdef NDS_MODE
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <nds.h>

#include "ui_common.h"
#include "util.h"
#include "battle.h"
#include "map.h"
#include "ability.h"

const char terrain_char[]={
	' ',
	'n',
	'o',
	' ',
	'w',
	'r',
	'g',
	'b',
	'd',
	'p',
	's',
	'*',
	't',
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

	fprintf(stderr,"num: %d\n",num);

	for(j=0;j<3;j++){
		printf("   ");
		for(i=0;i<MAP_WIDTH;i++){
			printf("%d ",i);
		}
		if(j<2)printf(" | ");
	}
	printf("\n");

	for(j=0;j<MAP_HEIGHT;j++){
		printf("%02d ",j);
		for(i=0;i<MAP_WIDTH;i++){
			if((uid=unit_at(blist,num,i,j))>=0)
				printf("%d ",uid);
			else
				printf("%c ",terrain_char[get_map_terrain(i,j)]);
		}
		printf(" | ");

		printf("%02d ",j);
		for(i=0;i<MAP_WIDTH;i++){
			if(get_map_terrain(i,j)==MAP_T_NOTARGET)
				printf("  ");
			else
				printf("%d ",get_map_height(i,j));
		}

		printf(" | ");

		printf("%02d ",j);
		for(i=0;i<MAP_WIDTH;i++){
			if(move_valid(i,j))
				printf("x ");
			else
				printf("  ");
		}
		printf("\n");
	}
	printf("\n");
}

void print_info(struct battle_char *bc){
	int i;

	printf("UID: %d | DIR: %d | FOF: %d\n",bc->index,bc->dir,bc->fof);
	printf("HP: %d | MP: %d | CT: %d\n",bc->hp,bc->mp,bc->ct);
	printf("PA: %d | MA: %d | WP: %d\n",bc->pa,bc->ma,bc->wp);
	printf("JUMP: %d | MOVE: %d\n",bc->jump,bc->move);
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
		printf("%d | %s, %s, 3:map, 4:info, 5:skip\n:",bi,(*flags&ACTED_FLAG)==0?"1:attack":"",(*flags&MOVED_FLAG)==0?"2:move":"");
		fgets(buf,100,stdin);

		sscanf(buf,"%d:%d:%d",&cmd,&x,&y);

		switch(cmd){
			case 1:
				if(!(*flags&ACTED_FLAG)){
					tl=get_targets(blist,num,x,y,1,1,0);
					fast_action(blist[bi],tl[0],0,0);
					free(tl);
					*flags|=ACTED_FLAG;
				}
				else
					run=1;
				break;
			case 2:
				if(!(*flags&MOVED_FLAG)){
					if(move(blist[bi],x,y)==MOVE_INVALID)
						run=1;
					else
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
#endif
