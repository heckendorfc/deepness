#include <stdlib.h>
#include "battle.h"
#include "ability.h"
#include "map.h"
#include "equipment.h"

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

void attack(struct battle_char *s, struct battle_char *d){
	int pri=s->ch->eq[EQ_WEAPON];
	int sec=s->ch->eq[EQ_OFFHAND];

	if(EQ_TYPE(pri)<EQO_SHIELD)
		weapon_damage[pri](&weapons[EQ_TYPE(pri)][pri>>6],s,d);


}

void defend(struct battle_char *c){
}

void status_check(struct battle_char *blist, int num){
	int bi;
	int i;
	
	for(bi=0;bi<num;bi++){
		for(i=0;i<NUM_STATUS;i++){
			if(blist[bi].status[i]>0)
				blist[bi].status[i]--;
		}
	}
}

void slow_action_charge(struct battle_char *blist, int num){
	int bi;
	
	for(bi=0;bi<num;bi++){
		if(blist[bi].slow_act){
			blist[bi].slow_act->ctr--;
		}
	}
}

int should_react(struct battle_char *ch){
	if(ch->ch->reaction==NULL)
		return 0;

	if((ch->ch->reaction_trigger&RFLAG_REACT_DAMAGE) && last_action.damage!=NO_DAMAGE)
		return 1;

	if((ch->ch->reaction_trigger&RFLAG_REACT_COUNTER) && (claction[last_action.preresolve->jobindex][last_action.preresolve->findex].flags&AFLAG_COUNTER))
		return 1;
	
	if((ch->ch->reaction_trigger&RFLAG_REACT_CRITICAL) && (ch->status[STATUS_CRITICAL]))
		return 1;
	
	return 0;
} 

void react(struct battle_char *attacker, struct battle_char **reacter, int num){
	int i;

	for(i=0;i<num;i++){
		if(should_react(reacter[i]))
			reacter[i]->ch->reaction(reacter[i],attacker);
	}
}

void slow_action_resolution(struct battle_char *blist, int num){
	int bi;
	struct stored_action *tmp;
	struct battle_char **targets;
	int num_t;

	for(bi=0;bi<num;bi++){
		tmp=blist[bi].slow_act;
		if(tmp && tmp->ctr==0){
			//prereact(tmp->origin,tmp->target,tmp->num_target); // for hamedo
			targets=get_targets(blist,num,tmp->target.x,tmp->target.y,tmp->target.width,tmp->target.vertical,tmp->target.dir);
			for(num_t=0;targets[num_t];num_t++);
			tmp->f(tmp->origin,targets,num_t);
			last_action.preresolve=tmp;
			react(tmp->origin,targets,num_t);
			free(targets);
		}
	}
}

void ct_charge(struct battle_char *blist, int num){
	int bi;
	
	for(bi=0;bi<num;bi++){
		blist[bi].ct+=blist[bi].speed;
	}
}

void ct_resolution(struct battle_char *blist, int num){
	int bi;
	uint8_t flags=0;
	
	for(bi=0;bi<num;bi++){
		if(blist[bi].ct>=100){
			flags=0;
			if(blist[bi].status[STATUS_NOMOVE])
				flags|=MOVED_FLAG;
			if(blist[bi].status[STATUS_NOACT])
				flags|=ACTED_FLAG;

			// Select new actions
			if(flags==0)
				battle_orders(blist,bi,&flags);

			// Move
			if(flags!=(ACTED_FLAG|MOVED_FLAG))
				battle_orders(blist,bi,&flags);

			// Update CT
			if(flags==0)
				blist[bi].ct=blist[bi].ct-60;
			else if(flags==(MOVED_FLAG|ACTED_FLAG))
				blist[bi].ct=blist[bi].ct-100;
			else
				blist[bi].ct=blist[bi].ct-80;

			if(blist[bi].ct>60)
				blist[bi].ct=60;
		}
	}
}

void start_battle(struct character **friends, struct character *foes, int numfoe){
	struct battle_char *blist;
	int bi;
	int i;

	for(i=bi=0;i<NUM_CHAR_SLOTS;i++)
		if(friends[i] && friends[i]->battleready)
			bi++;

	blist=malloc(sizeof(*blist)*(bi+numfoe));

	for(i=bi=0;i<NUM_CHAR_SLOTS;i++)
		if(friends[i] && friends[i]->battleready){
			blist[bi].ch=friends[i];
			init_battle_char(blist[bi]);
			bi++;
		}

	for(i=0;i<numfoe;i++){
		blist[bi].ch=foes+i;
		init_battle_char(blist[bi]);
		bi++;
	}

	while(1){
		status_check(blist,bi);
		slow_action_charge(blist,bi);
		slow_action_resolution(blist,bi);
		ct_charge(blist,bi);
		ct_resolution(blist,bi);
	}
}
