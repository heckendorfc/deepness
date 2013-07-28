#include <stdlib.h>
#include "util.h"
#include "player.h"

int get_random(int min, int max){
	return (rand()%(max-min))+min;
}

int get_dist(struct battle_char *bc, int x, int y, int w, int v, int d){
	int xd,yd;

	xd=bc->x-x;
	if(xd<0)xd=-xd;

	yd=bc->y-y;
	if(yd<0)yd=-yd;

	return xd+yd;
}

struct battle_char** get_targets(struct battle_char *blist, int num, int x, int y, int w, int v, int d){
	struct battle_char **ret;
	int i;
	int bi=0;

	ret=malloc(sizeof(*ret)*num);

	for(i=0;i<num;i++){
		if(get_dist(blist+i,x,y,w,v,d)<w)
			ret[bi++]=blist+i;
	}

	if(bi<num)ret[bi]=NULL;

	return ret;
}

int unit_at(struct battle_char *blist, int num, int x, int y){
	int ret=-1;
	int i;

	for(i=0;i<num;i++)
		if(blist[i].x==x && blist[i].y==y)
			return i;

	return ret;
}
