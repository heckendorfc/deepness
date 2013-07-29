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

struct battle_char** get_targets(struct battle_char **blist, int num, int x, int y, int w, int v, int d){
	struct battle_char **ret;
	int i;
	int bi=0;

	ret=malloc(sizeof(*ret)*num);

	for(i=0;i<num;i++){
		if(get_dist(blist[i],x,y,w,v,d)<w)
			ret[bi++]=blist[i];
	}

	if(bi<num)ret[bi]=NULL;

	return ret;
}

int unit_at(struct battle_char **blist, int num, int x, int y){
	int ret=-1;
	int i;

	for(i=0;i<num;i++)
		if(blist[i]->x==x && blist[i]->y==y)
			return i;

	return ret;
}

void set_base_stats(struct character *ch){
	if(ch->gender==GENDER_MALE){
		ch->raw[STAT_HP]=get_random(491520,524287);
		ch->raw[STAT_MP]=get_random(491520,524287);
		ch->raw[STAT_PA]=81920;
		ch->raw[STAT_PA]=65536;
	}
	else{
		ch->raw[STAT_HP]=get_random(458752,491519);
		ch->raw[STAT_MP]=get_random(458752,491519);
		ch->raw[STAT_PA]=65536;
		ch->raw[STAT_PA]=81920;
	}

	ch->raw[STAT_SP]=98304;
}

void level_up(struct character *ch){
	int level;
	int i;

	for(i=level=0;i<NUM_CLASS;i++)
		level+=ch->level[i];


	for(i=0;i<NUM_STATS;i++)
		ch->raw[i]+=ch->raw[i]/(class_stats[(int)ch->primary].gainmod[i]+level);
}

void set_battle_stats(struct battle_char *bc){
	bc->hp=(bc->ch->raw[STAT_HP]*class_stats[(int)bc->ch->primary].basemod[STAT_HP])/BASE_STAT_DENOMINATOR;
	if(bc->hp<1)bc->hp=1;

	bc->mp=(bc->ch->raw[STAT_MP]*class_stats[(int)bc->ch->primary].basemod[STAT_MP])/BASE_STAT_DENOMINATOR;
	if(bc->mp<1)bc->mp=1;

	bc->ma=(bc->ch->raw[STAT_MA]*class_stats[(int)bc->ch->primary].basemod[STAT_MA])/BASE_STAT_DENOMINATOR;
	if(bc->ma<1)bc->ma=1;

	bc->speed=(bc->ch->raw[STAT_SP]*class_stats[(int)bc->ch->primary].basemod[STAT_SP])/BASE_STAT_DENOMINATOR;
	if(bc->speed<1)bc->speed=1;

	bc->pa=(bc->ch->raw[STAT_PA]*class_stats[(int)bc->ch->primary].basemod[STAT_PA])/BASE_STAT_DENOMINATOR;
	if(bc->pa<1)bc->pa=1;
}

void create_character(struct character *ch){
	int i;

	ch->gender=GENDER_MALE;
	ch->battleready=1;
	ch->jp=0;
	for(i=0;i<NUM_CLASS;i++){
		ch->mastery[i]=0;
		ch->level[i]=0;
	}
	ch->primary=ch->secondary=CL_SQUIRE;
	for(i=0;i<NUM_EQ_SLOTS;i++)
		ch->eq[i]=0;
	ch->faith=ch->brave=50;

	set_base_stats(ch);
}