#include <stdlib.h>
#include "util.h"
#include "player.h"
#include "equipment.h"
#include "battle.h"
#include "ability.h"

int get_random(int min, int max){
	if(max==0)
		return 0;
	if(max-min==0)
		return min;
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

int character_level(struct character *ch){
	int i,level;
	for(i=level=0;i<NUM_CLASS;i++)
		level+=ch->exp[i]/100;
	return level;
}

void level_up(struct character *ch){
	int level;
	int i;

	level=character_level(ch);

	for(i=0;i<NUM_STATS;i++)
		ch->raw[i]+=ch->raw[i]/(class_stats[(int)ch->primary].gainmod[i]+level);
}

void spend_jp_random(struct character *ch){
	int i;
	int num;
	int found=0;
	uint8_t bought[4]={0,0,0,0};

	while(ch->jp && !found){
		found=1;
		i=0;
		if(bought[i]<num_action[ch->primary] && claction[ch->primary][bought[i]].jp<ch->jp){
			ch->mastery[ch->primary]|=MASTERY_ACTION_BIT(bought[i]);
			ch->jp-=claction[ch->primary][bought[i]].jp;
			bought[i]++;
			found=0;
		}
		i++;
		if(bought[i]<num_reaction[ch->primary] && clreaction[ch->primary][bought[i]].jp<ch->jp){
			ch->mastery[ch->primary]|=MASTERY_REACTION_BIT(bought[i]);
			ch->jp-=clreaction[ch->primary][bought[i]].jp;
			bought[i]++;
			found=0;
		}
		i++;
		if(bought[i]<num_support[ch->primary] && clsupport[ch->primary][bought[i]].jp<ch->jp){
			ch->mastery[ch->primary]|=MASTERY_SUPPORT_BIT(bought[i]);
			ch->jp-=clsupport[ch->primary][bought[i]].jp;
			bought[i]++;
			found=0;
		}
		i++;
		if(bought[i]<num_movement[ch->primary] && clmovement[ch->primary][bought[i]].jp<ch->jp){
			ch->mastery[ch->primary]|=MASTERY_REACTION_BIT(bought[i]);
			ch->jp-=clmovement[ch->primary][bought[i]].jp;
			bought[i]++;
			found=0;
		}
	}
}

int job_level(struct character *ch){
	int jp=ch->jp;
	int mastery;
	int i;

	mastery=MASTERY_ACTION(ch->mastery[ch->primary]);
	for(i=0;i<num_action[ch->primary];i++)
		if(mastery&BIT(i))
			jp+=claction[ch->primary][i].jp;
	mastery=MASTERY_REACTION(ch->mastery[ch->primary]);
	for(i=0;i<num_reaction[ch->primary];i++)
		if(mastery&BIT(i))
			jp+=clreaction[ch->primary][i].jp;
	mastery=MASTERY_SUPPORT(ch->mastery[ch->primary]);
	for(i=0;i<num_support[ch->primary];i++)
		if(mastery&BIT(i))
			jp+=clsupport[ch->primary][i].jp;
	mastery=MASTERY_MOVEMENT(ch->mastery[ch->primary]);
	for(i=0;i<num_movement[ch->primary];i++)
		if(mastery&BIT(i))
			jp+=clmovement[ch->primary][i].jp;

	if(jp<100)
		return 0;
	if(jp<200)
		return 1;
	if(jp<350)
		return 2;	
	if(jp<550)
		return 3;
	if(jp<800)
		return 4;
	if(jp<1150)
		return 5;
	if(jp<1550)
		return 6;
	if(jp<2100)
		return 7;
	return 8;
}

void switch_jobs(struct character *ch, int index){
	// Should we check availability here or assume it's done front-side?
	ch->primary=index;
	ch->jp=0;

	if(job_level(ch)==0)
		ch->jp=100;
}

void jp_reward(struct character *ch){
	int jp;
	int jlvl=job_level(ch);
	int tlvl=0;

	tlvl=character_level(ch);
	
	jp=8+(jlvl*2)+(tlvl/4);

	if(ch->support==SFLAG_JPUP)
		jp*=2;

	ch->jp+=jp;
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

void add_item(uint16_t index){
	int i;
	if(EQ_INDEX_T(index)==0)
		return;
	for(i=0;i<NUM_ITEMS;i++){
		if(pdata.inventory[i].index==index){
			pdata.inventory[i].count++;
			return;
		}
	}
	for(i=0;i<NUM_ITEMS;i++){
		if(pdata.inventory[i].count<=0){
			pdata.inventory[i].count=1;
			pdata.inventory[i].index=index;
			break;
		}
	}
}

void remove_eq(struct character *ch, int loc){
	int o,t;
	o=ch->eq[loc]>>6;
	t=EQ_TYPE(ch->eq[loc]);
	
	add_item(EQ_INDEX(loc,t,o));
	ch->eq[loc]=0;
}

void wear_eq(struct character *ch, int offset){
	uint16_t index;

	pdata.inventory[offset].count--;

	index=pdata.inventory[offset].index;
	ch->eq[EQ_INDEX_L(index)]=(EQ_INDEX_O(index)<<6)|EQ_INDEX_T(index);
	
}

void set_level(struct character *ch, int lvl){
	while(lvl>0){
		ch->exp[ch->primary]+=100;
		level_up(ch);
		lvl--;
	}
}

void create_character(struct character *ch){
	int i;

	ch->gender=GENDER_MALE;
	ch->battleready=BATTLE_READY;
	ch->jp=100;
	for(i=0;i<NUM_CLASS;i++){
		ch->mastery[i]=0;
		ch->exp[i]=0;
	}
	//ch->primary=ch->secondary=CL_WIZARD;
	ch->primary=ch->secondary=CL_SQUIRE;
	ch->support=0;
	ch->reaction_class=ch->reaction_index=ch->reaction_trigger=0;
	ch->movement_class=ch->movement_index=0;
	ch->mastery[0]=~0; // Generic abilities
	//for(i=0;i<NUM_ACTION_PER_ABILITY;i++)
		//ch->mastery[ch->primary]|=BIT(i);
	for(i=0;i<NUM_EQ_SLOTS;i++)
		ch->eq[i]=0;
	ch->eq[EQ_WEAPON]=EQW_SWORD; // | 0<<6
	ch->faith=ch->brave=50;

	set_base_stats(ch);
}
