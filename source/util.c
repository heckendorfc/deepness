#include <stdlib.h>
#include "util.h"
#include "player.h"
#include "equipment.h"
#include "battle.h"

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

static int get_directionality(struct battle_char *bc, int x, int y, int d){
	int front;
	int side;
	int ret=0;

	if(abs(bc->x-x)<abs(bc->y-y)){
		side=BIT(DIR_EAST)|BIT(DIR_WEST);
		if(bc->y<y)
			front=BIT(DIR_NORTH);
		else
			front=BIT(DIR_SOUTH);
	}
	else{
		side=BIT(DIR_NORTH)|BIT(DIR_SOUTH);
		if(bc->x<x)
			front=BIT(DIR_EAST);
		else
			front=BIT(DIR_WEST);
	}

	if(d&BIT(ATTACK_DIR_FRONT))
		ret|=front;
	if(d&BIT(ATTACK_DIR_SIDE))
		ret|=side;

	return ret;
}

struct battle_char** get_targets(struct battle_char **blist, int sourcei, int num, int x, int y, int w, int v, int d){
	struct battle_char **ret;
	int i;
	int bi=0;
	int dir;

	ret=malloc(sizeof(*ret)*num);

	if(d==0){
		for(i=0;i<num;i++){
			if(get_dist(blist[i],x,y,w,v,d)<w)
				ret[bi++]=blist[i];
		}
	}
	else if(d==(BIT(ATTACK_DIR_FRONT)|BIT(ATTACK_DIR_SIDE)|BIT(ATTACK_DIR_REAR))){
		for(i=0;i<num;i++)
			if((blist[bi]->x==x || blist[bi]->y==y) && get_dist(blist[i],x,y,w,v,d)<w)
				ret[bi++]=blist[i];
	}
	else{
		dir=get_directionality(blist[sourcei],x,y,d);
		if(dir&DIR_NORTH)
			for(i=0;i<num;i++)
				if(blist[bi]->y>=y && get_dist(blist[i],x,y,w,v,d)<w)
					ret[bi++]=blist[i];
		if(dir&DIR_EAST)
			for(i=0;i<num;i++)
				if(blist[bi]->x>=x && get_dist(blist[i],x,y,w,v,d)<w)
					ret[bi++]=blist[i];
		if(dir&DIR_SOUTH)
			for(i=0;i<num;i++)
				if(blist[bi]->y<=y && get_dist(blist[i],x,y,w,v,d)<w)
					ret[bi++]=blist[i];
		if(dir&DIR_WEST)
			for(i=0;i<num;i++)
				if(blist[bi]->x<=x && get_dist(blist[i],x,y,w,v,d)<w)
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
		ch->raw[STAT_MA]=65536;
	}
	else{
		ch->raw[STAT_HP]=get_random(458752,491519);
		ch->raw[STAT_MP]=get_random(458752,491519);
		ch->raw[STAT_PA]=65536;
		ch->raw[STAT_MA]=81920;
	}

	ch->raw[STAT_SP]=98304;
}

void level_up(struct character *ch){
	int level;
	int i;

	for(i=level=0;i<NUM_CLASS;i++)
		level+=ch->exp[i]/100;


	for(i=0;i<NUM_STATS;i++)
		ch->raw[i]+=ch->raw[i]/(class_stats[(int)ch->primary].gainmod[i]+level);
}

void set_battle_stats(struct battle_char *bc){
	bc->hp=(bc->ch->raw[STAT_HP]*class_stats[(int)bc->ch->primary].basemod[STAT_HP])/BASE_STAT_DENOMINATOR;
	if(bc->hp<1)bc->hp=1;
	bc->hp_max=bc->hp;

	bc->mp=(bc->ch->raw[STAT_MP]*class_stats[(int)bc->ch->primary].basemod[STAT_MP])/BASE_STAT_DENOMINATOR;
	if(bc->mp<1)bc->mp=1;
	bc->mp_max=bc->mp;

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
	ch->battleready=BATTLE_READY;
	ch->jp=0;
	for(i=0;i<NUM_CLASS;i++){
		ch->mastery[i]=0;
		ch->exp[i]=0;
	}
	//ch->primary=ch->secondary=CL_WIZARD;
	ch->primary=ch->secondary=CL_SQUIRE;
	ch->mastery[0]=~0; // Generic abilities
	//for(i=0;i<NUM_ACTION_PER_ABILITY;i++)
		//ch->mastery[ch->primary]|=BIT(i);
	for(i=0;i<NUM_EQ_SLOTS;i++)
		ch->eq[i]=0;
	ch->eq[EQ_WEAPON]=EQW_SWORD; // | 0<<6
	ch->faith=ch->brave=50;

	set_base_stats(ch);
}
