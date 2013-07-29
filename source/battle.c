#include <stdlib.h>
#include "battle.h"
#include "ability.h"
#include "map.h"
#include "equipment.h"
#include "util.h"

#ifndef RELEASE_MODE
#include "ui_console.h"
#endif

int evaded(struct battle_char *target, int type, int dir, int base_hit){
	uint32_t tohit=base_hit;
	if(type==AFLAG_PHYSICAL){
		if(REACTION(target).flags&RFLAG_HITMOD && should_react(target))
			base_hit-=REACTION(target).rf(target,NULL);
		switch(dir){
			case ATTACK_DIR_FRONT:
				tohit*=(100-class_stats[(int)target->ch->primary].evade);
				tohit*=(100-misc_armor[EQ_TYPE(target->ch->eq[EQ_MISC])][target->ch->eq[EQ_MISC]>>6].p_evade);
				tohit*=(100-weapons[EQ_TYPE(target->ch->eq[EQ_WEAPON])][target->ch->eq[EQ_WEAPON]>>6].p_evade);
				if(EQ_TYPE(target->ch->eq[EQ_OFFHAND])==EQO_SHIELD)
					tohit*=(100-offhand[target->ch->eq[EQ_OFFHAND]>>6].p_evade);
				else
					tohit*=100;
				tohit/=100000000;
				break;
			case ATTACK_DIR_SIDE:
				tohit*=(100-misc_armor[EQ_TYPE(target->ch->eq[EQ_MISC])][target->ch->eq[EQ_MISC]>>6].p_evade);
				tohit*=(100-weapons[EQ_TYPE(target->ch->eq[EQ_WEAPON])][target->ch->eq[EQ_WEAPON]>>6].p_evade);
				if(EQ_TYPE(target->ch->eq[EQ_OFFHAND])==EQO_SHIELD)
					tohit*=(100-offhand[target->ch->eq[EQ_OFFHAND]>>6].p_evade);
				else
					tohit*=100;
				tohit/=1000000;
				break;
			case ATTACK_DIR_REAR:
				tohit*=(100-misc_armor[EQ_TYPE(target->ch->eq[EQ_MISC])][target->ch->eq[EQ_MISC]>>6].p_evade);
				tohit/=100;
				break;
		}
	}
	else{
		tohit*=(100-misc_armor[EQ_TYPE(target->ch->eq[EQ_MISC])][target->ch->eq[EQ_MISC]>>6].m_evade);
		if(EQ_TYPE(target->ch->eq[EQ_OFFHAND])==EQO_SHIELD)
			tohit*=(100-offhand[target->ch->eq[EQ_OFFHAND]>>6].m_evade);
		else
			tohit*=100;
		tohit/=10000;
	}

	if(get_random(0,100)<tohit)
		return 1;
	return 0;
}

int get_attack_dir(struct battle_char *attacker, struct battle_char *defender){
	switch(defender->dir){
		case DIR_NORTH:
			if(defender->y<attacker->y && (attacker->y-defender->y)>=abs(attacker->x-defender->x))
				return ATTACK_DIR_FRONT;
			if(abs(defender->x-attacker->x)>=abs(defender->y-attacker->y))
				return ATTACK_DIR_SIDE;
			return ATTACK_DIR_REAR;
		case DIR_EAST:
			if(defender->x<attacker->x && (attacker->x-defender->x)>=abs(attacker->y-defender->y))
				return ATTACK_DIR_FRONT;
			if(abs(defender->x-attacker->x)<=abs(defender->y-attacker->y))
				return ATTACK_DIR_SIDE;
			return ATTACK_DIR_REAR;
		case DIR_SOUTH:
			if(defender->y>attacker->y && (attacker->y-defender->y)>=abs(attacker->x-defender->x))
				return ATTACK_DIR_FRONT;
			if(abs(defender->x-attacker->x)>=abs(defender->y-attacker->y))
				return ATTACK_DIR_SIDE;
			return ATTACK_DIR_REAR;
		case DIR_WEST:
			if(defender->x>attacker->x && (attacker->x-defender->x)>=abs(attacker->y-defender->y))
				return ATTACK_DIR_FRONT;
			if(abs(defender->x-attacker->x)<=abs(defender->y-attacker->y))
				return ATTACK_DIR_SIDE;
			return ATTACK_DIR_REAR;
	}

	// This should not happen...
	return ATTACK_DIR_FRONT;
}

void deal_damage(struct battle_char *bc, uint16_t dmg){
	bc->hp-=dmg;
	if(bc->hp<0){
		bc->hp=0;
		bc->status[STATUS_DEAD]=4;
	}
	else if((bc->hp*100)/bc->hp_max<5)
		bc->status[STATUS_CRITICAL]=UNTIMED_STATUS;
}


void defend(struct battle_char *c){
}

void status_check(struct battle_char **blist, int num){
	int bi;
	int i;
	
	for(bi=0;bi<num;bi++){
		for(i=0;i<NUM_STATUS;i++){
			if(blist[bi]->status[i]>0 && i!=STATUS_DEAD && blist[bi]->status[i]!=UNTIMED_STATUS)
				blist[bi]->status[i]--;
		}
	}

}

void slow_action_charge(struct battle_char **blist, int num){
	int bi;
	
	for(bi=0;bi<num;bi++){
		if(blist[bi]->slow_act){
			blist[bi]->slow_act->ctr--;
		}
	}
}

int should_react(struct battle_char *ch){
	if(ch->ch->reaction_class==0)
		return 0;

	if(REACTION(ch).flags&RFLAG_BRAVE_PERCENT && get_random(0,100)>ch->brave)
		return 0;

	if((REACTION(ch).trigger&RFLAG_TRIGGER_DAMAGE) && last_action.damage!=NO_DAMAGE)
		return 1;

	if((REACTION(ch).trigger&RFLAG_TRIGGER_COUNTER) && (claction[last_action.preresolve->jobindex][last_action.preresolve->findex].flags&AFLAG_COUNTER))
		return 1;
	
	if((REACTION(ch).trigger&RFLAG_TRIGGER_CRITICAL) && (ch->status[STATUS_CRITICAL]))
		return 1;
	
	if(REACTION(ch).trigger&RFLAG_TRIGGER_ALWAYS)
		return 1;

	return 0;
} 

void react(struct battle_char *attacker, struct battle_char **reacter, int num){
	int i;

	for(i=0;i<num;i++){
		if(should_react(reacter[i]))
			REACTION(reacter[i]).rf(reacter[i],attacker);
	}
}

void slow_action_resolution(struct battle_char **blist, int num){
	int i;
	int bi;
	struct stored_action *tmp;
	struct battle_char **targets;
	int num_t;
	int type;
	int dir;

	for(bi=0;bi<num;bi++){
		tmp=blist[bi]->slow_act;
		if(tmp && tmp->ctr==0){
			//prereact(tmp->origin,tmp->target,tmp->num_target); // for hamedo
			targets=get_targets(blist,num,tmp->target.x,tmp->target.y,tmp->target.width,tmp->target.vertical,tmp->target.dir);
			for(num_t=0;targets[num_t];num_t++);
			/*if(claction[tmp->jobindex][tmp->findex].flags&AFLAG_EVADE)
				for(i=0;i<num_t;i++)
			 		if(evaded(targets[i],))
			*/
			tmp->f(tmp->origin,targets,num_t);
			last_action.preresolve=tmp;
			react(tmp->origin,targets,num_t);
			free(targets);
		}
	}
}

void ct_charge(struct battle_char **blist, int num){
	int bi;
	
	for(bi=0;bi<num;bi++){
		blist[bi]->ct+=blist[bi]->speed;
	}
}

void ct_resolution(struct battle_char **blist, int *num){
	int bi;
	uint8_t flags=0;
	
	for(bi=0;bi<*num;bi++){
		if(blist[bi]->ct>=100){
			if(blist[bi]->hp==0 && blist[bi]->status[STATUS_DEAD]==0){
				*num-=1;
				blist[bi]=blist[*num];
				bi--;
				continue;
			}

			flags=0;
			if(blist[bi]->status[STATUS_NOMOVE])
				flags|=MOVED_FLAG;
			if(blist[bi]->status[STATUS_NOACT])
				flags|=ACTED_FLAG;

			// Select new actions
			if(flags==0)
				battle_orders(blist,bi,*num,&flags);

			// Select other action
			if(flags!=0 && flags!=(ACTED_FLAG|MOVED_FLAG))
				battle_orders(blist,bi,*num,&flags);

			// Update CT
			if(flags==0)
				blist[bi]->ct=blist[bi]->ct-60;
			else if(flags==(MOVED_FLAG|ACTED_FLAG))
				blist[bi]->ct=blist[bi]->ct-100;
			else
				blist[bi]->ct=blist[bi]->ct-80;

			if(blist[bi]->ct>60)
				blist[bi]->ct=60;
		}
	}
}

void init_battle_char(struct battle_char *bc){
	int i;

	set_battle_stats(bc);

	bc->faith=bc->ch->faith;
	bc->brave=bc->ch->brave;

	bc->slow_act=NULL;

	for(i=0;i<NUM_STATUS;i++)
		bc->status[i]=0;

	bc->wp=weapons[EQ_TYPE(bc->ch->eq[EQ_WEAPON])][(bc->ch->eq[EQ_WEAPON]>>6)].wp;

	bc->ct=0;
}

int place_units(struct battle_char *team, int num, int type){
	int i,j;
	int bi=0;

	for(j=0;j<MAP_HEIGHT;j++){
		for(i=0;i<MAP_WIDTH;i++){
			if(get_map_start(i,j)==type){
				team[bi].x=i;
				team[bi].y=j;
				bi++;
				if(bi==num)
					return num;
			}
		}
	}

	return bi;
}

void start_battle(struct character **friends, struct character *foes, int numfoe){
	struct battle_char *blist,**pblist;
	int bi;
	int pi;
	int i;
	int friend_placed,foe_placed;

	for(i=bi=0;i<NUM_CHAR_SLOTS;i++)
		if(friends[i] && friends[i]->battleready)
			bi++;

	blist=malloc(sizeof(*blist)*(bi+numfoe));
	pblist=malloc(sizeof(*pblist)*(bi+numfoe));

	for(i=bi=0;i<NUM_CHAR_SLOTS;i++)
		if(friends[i] && friends[i]->battleready){
			blist[bi].ch=friends[i];
			blist[bi].index=bi;
			blist[bi].fof=FOF_FRIEND;
			init_battle_char(blist+bi);
			bi++;
		}

	friend_placed=place_units(blist,bi,MAP_FRIEND_START);
	for(pi=0;pi<friend_placed;pi++)
		pblist[pi]=blist+pi;

	for(i=0;i<numfoe;i++){
		blist[bi].ch=foes+i;
		blist[bi].index=bi;
		blist[bi].fof=FOF_FOE;
		init_battle_char(blist+bi);
		bi++;
	}

	foe_placed=place_units(blist+(bi-numfoe),numfoe,MAP_FOE_START);
	for(i=0;i<foe_placed;pi++)
		pblist[pi]=blist+(bi-numfoe)+(i++);

	while(1){
		status_check(pblist,pi);
		slow_action_charge(pblist,pi);
		slow_action_resolution(pblist,pi);
		ct_charge(pblist,pi);
		ct_resolution(pblist,&pi);
	}
}
