#include <stdlib.h>
#include <stdio.h>
#include "battle.h"
#include "ability.h"
#include "map.h"
#include "equipment.h"
#include "util.h"
#include "ai.h"

#include "ui_common.h"

int sign_compat(struct character *a, struct character *b){
	if(a->sign==SIGN_SERPENTARIUS)
		return SIGN_COMPAT_NEUTRAL;

	if((a->sign-b->sign)%4==0)
		return SIGN_COMPAT_GOOD;

	if((a->sign-b->sign)%6==0)
		return a->gender==b->gender?SIGN_COMPAT_WORST:SIGN_COMPAT_BEST;

	if((a->sign-b->sign)%3==0)
		return SIGN_COMPAT_BAD;

	return SIGN_COMPAT_NEUTRAL;
}

void remove_status(struct battle_char *bc, int status){
	if(bc->status[status]!=IMMUNE_TO_STATUS)
		bc->status[status]=0;
	else
		return;

	switch(status){
		case STATUS_CHARM:
			bc->fof=!bc->fof;
			break;
		case STATUS_OIL:
			bc->resist[ELEM_FIRE]&=~RESIST_WEAK;
			break;
		case STATUS_RERAISE:
			bc->status[status]=IMMUNE_TO_STATUS;
			break;
	}
}

void add_status(struct battle_char *bc, int status){
	if(bc->status[status]==IMMUNE_TO_STATUS)
		return;

	switch(status){
		case STATUS_CHARGING:
			bc->status[status]=UNTIMED_STATUS;
			break;
		case STATUS_DEFENDING:
			bc->status[status]=UNTIMED_STATUS;
			break;
		case STATUS_PERFORMING:
			bc->status[status]=UNTIMED_STATUS;
			break;
		case STATUS_FLOAT:
			bc->status[status]=UNTIMED_STATUS;
			break;
		case STATUS_HASTE:
			bc->status[status]=32;
			remove_status(bc,STATUS_SLOW);
			break;
		case STATUS_REGEN:
			bc->status[status]=36;
			remove_status(bc,STATUS_POISON);
			break;
		case STATUS_PROTECT:
			bc->status[status]=32;
			break;
		case STATUS_RERAISE:
			bc->status[status]=UNTIMED_STATUS;
			break;
		case STATUS_SHELL:
			bc->status[status]=32;
			break;
		case STATUS_TRANSPARENT:
			bc->status[status]=UNTIMED_STATUS;
			break;
		case STATUS_CHARM:
			bc->status[status]=32;
			bc->fof=!bc->fof;
			break;
		case STATUS_CONFUSION:
			bc->status[status]=UNTIMED_STATUS;
			break;
		case STATUS_DARKNESS:
			bc->status[status]=UNTIMED_STATUS;
			break;
		case STATUS_DEATHSENTENCE:
			bc->status[status]=120;
			break;
		case STATUS_NOACT:
			bc->status[status]=24;
			break;
		case STATUS_NOMOVE:
			bc->status[status]=24;
			break;
		case STATUS_POLYMORPH:
			bc->status[status]=UNTIMED_STATUS;
			break;
		case STATUS_OIL:
			if(!(bc->resist[ELEM_FIRE]&RESIST_WEAK)){
				bc->status[status]=UNTIMED_STATUS;
				bc->resist[ELEM_FIRE]|=RESIST_WEAK;
			}
			break;
		case STATUS_PETRIFY:
			bc->status[status]=UNTIMED_STATUS;
			break;
		case STATUS_POISON:
			bc->status[status]=36;
			break;
		case STATUS_SILENCE:
			bc->status[status]=UNTIMED_STATUS;
			break;
		case STATUS_SLEEP:
			bc->status[status]=60;
			break;
		case STATUS_SLOW:
			bc->status[status]=24;
			remove_status(bc,STATUS_HASTE);
			break;
		case STATUS_STOP:
			bc->status[status]=20;
			break;
		case STATUS_UNDEAD:
			bc->status[status]=UNTIMED_STATUS;
			break;
		case STATUS_CRITICAL:
			bc->status[status]=UNTIMED_STATUS;
			break;
		case STATUS_DEAD:
			bc->status[status]=4; // turns
			break;
		case STATUS_BERSERK:
			bc->status[status]=UNTIMED_STATUS;
			remove_status(bc,STATUS_CHARGING);
			remove_status(bc,STATUS_DEFENDING);
			remove_status(bc,STATUS_PERFORMING);
		case STATUS_FAITH:
			bc->status[status]=32;
			remove_status(bc,STATUS_INNOCENT);
			break;
		case STATUS_INNOCENT:
			bc->status[status]=32;
			remove_status(bc,STATUS_FAITH);
			break;
		case STATUS_REFLECT:
			bc->status[status]=32;
			break;
		case STATUS_QUICK:
			bc->status[status]=1;
			bc->ct=100;
			break;
	}
}

int move(struct battle_char *bc, int x, int y){
	if(!(bc->x==x && bc->y==y) && move_valid(x,y)){
		bc->x=x;
		bc->y=y;
		return MOVE_SUCCESS;
	}

	return MOVE_INVALID;
}

int evaded(struct battle_char *target, int type, int dir, int base_hit){
	uint32_t tohit=base_hit;
	uint8_t evademod=1;
	char msg[20];

	if(REACTION(target).flags&RFLAG_EVADEMOD && should_react(target))
		evademod=REACTION(target).rf(target,NULL);
	if(type==AFLAG_PHYSICAL){
		if(REACTION(target).flags&RFLAG_HITMOD && should_react(target))
			tohit-=REACTION(target).rf(target,NULL);
		switch(dir){
			case ATTACK_DIR_FRONT:
				tohit*=(100-class_stats[(int)target->ch->primary].evade);
				tohit/=100;
				tohit*=(100-misc_armor[EQ_TYPE(target->ch->eq[EQ_MISC])][target->ch->eq[EQ_MISC]>>6].phys.evade*evademod);
				tohit/=100;
				if(REACTION(target).flags&RFLAG_WEAPONEVADE){
					tohit*=(100-weapons[EQ_TYPE(target->ch->eq[EQ_WEAPON])][target->ch->eq[EQ_WEAPON]>>6].phys.evade*evademod);
					tohit/=100;
				}
				if(EQ_TYPE(target->ch->eq[EQ_OFFHAND])==EQO_SHIELD){
					tohit*=(100-offhand[target->ch->eq[EQ_OFFHAND]>>6].phys.evade*evademod);
					tohit/=100;
				}
				break;
			case ATTACK_DIR_SIDE:
				tohit*=(100-misc_armor[EQ_TYPE(target->ch->eq[EQ_MISC])][target->ch->eq[EQ_MISC]>>6].phys.evade*evademod);
				tohit/=100;
				if(REACTION(target).flags&RFLAG_WEAPONEVADE){
					tohit*=(100-weapons[EQ_TYPE(target->ch->eq[EQ_WEAPON])][target->ch->eq[EQ_WEAPON]>>6].phys.evade*evademod);
					tohit/=100;
				}
				if(EQ_TYPE(target->ch->eq[EQ_OFFHAND])==EQO_SHIELD){
					tohit*=(100-offhand[target->ch->eq[EQ_OFFHAND]>>6].phys.evade*evademod);
					tohit/=100;
				}
				break;
			case ATTACK_DIR_REAR:
				tohit*=(100-misc_armor[EQ_TYPE(target->ch->eq[EQ_MISC])][target->ch->eq[EQ_MISC]>>6].phys.evade*evademod);
				tohit/=100;
				break;
		}
	}
	else{
		tohit*=(100-misc_armor[EQ_TYPE(target->ch->eq[EQ_MISC])][target->ch->eq[EQ_MISC]>>6].mag.evade*evademod);
		if(EQ_TYPE(target->ch->eq[EQ_OFFHAND])==EQO_SHIELD)
			tohit*=(100-offhand[target->ch->eq[EQ_OFFHAND]>>6].mag.evade*evademod);
		else
			tohit*=100;
		tohit/=10000;
	}

	if(get_random(0,100)<tohit){
		return 0;
	}

	sprintf(msg,"%d Evaded!",target->index);
	print_message(msg);
	return 1;
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

void restore_hp(struct battle_char *bc, int16_t var){
	char msg[40];

	if(!STATUS_SET(bc,STATUS_DEAD)){
		if(bc->hp+var>bc->hp_max)
			bc->hp=bc->hp_max;
		else
			bc->hp+=var;

		sprintf(msg,"%d restored %d hp",bc->index,var);
		print_message(msg);

		if((bc->hp*100)/bc->hp_max<5)
			remove_status(bc,STATUS_CRITICAL);
	}
}

void restore_mp(struct battle_char *bc, int16_t var){
	if(!STATUS_SET(bc,STATUS_DEAD)){
		if(bc->mp+var>bc->mp_max)
			bc->mp=bc->mp_max;
		else
			bc->mp+=var;
	}
}

void deal_damage(struct battle_char *bc, int16_t dmg){
	char msg[40];

	if(dmg>0 && bc->hp<dmg){
		bc->hp=0;
		add_status(bc,STATUS_DEAD);
	}
	else
		bc->hp-=dmg;

	if(bc->hp>bc->hp_max) // undead or absorbs
		bc->hp=bc->hp_max;
	else if((bc->hp*100)/bc->hp_max<5)
		add_status(bc,STATUS_CRITICAL);

	sprintf(msg,"%d took %d dmg",bc->index,dmg);
	print_message(msg);
}

int check_action_range(struct battle_char *bc, int x, int y, int jobindex, int findex){
	int dist=get_dist(bc,x,y,0,0,0);

	if(claction[jobindex][findex].ra.range==RANGE_WEAPON)
		return weapon_can_hit(bc,x,y);

	if(dist<=claction[jobindex][findex].ra.range)
		return 1;

	return 0;
}

void defend(struct battle_char *c){
}

void status_check(struct battle_char **blist, int num){
	int bi;
	int i;
	
	for(bi=0;bi<num;bi++){
		for(i=0;i<NUM_STATUS;i++){
			if(STATUS_SET(blist[bi],i) && i!=STATUS_DEAD && blist[bi]->status[i]!=UNTIMED_STATUS)
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
	if(ch->ch->reaction_class==0 || STATUS_SET(ch,STATUS_NOMOVE) || STATUS_SET(ch,STATUS_BERSERK) || STATUS_SET(ch,STATUS_SLEEPING) || STATUS_SET(ch,STATUS_CONFUSION) || STATUS_SET(ch,STATUS_STOP))
		return 0;

	if(REACTION(ch).flags&RFLAG_BRAVE_PERCENT && get_random(0,100)>ch->brave)
		return 0;

	if((REACTION(ch).trigger&RFLAG_TRIGGER_DAMAGE) && last_action.damage!=NO_DAMAGE)
		return 1;

	if((REACTION(ch).trigger&RFLAG_TRIGGER_COUNTER) && (claction[last_action.preresolve->jobindex][last_action.preresolve->findex].flags&AFLAG_COUNTER))
		return 1;
	
	if((REACTION(ch).trigger&RFLAG_TRIGGER_CRITICAL) && (STATUS_SET(ch,STATUS_CRITICAL)))
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

static uint8_t get_base_hit(struct battle_char *source, struct battle_char *target, int jobindex, int findex){
	uint8_t base_hit;
	uint8_t (*hitmod)(struct battle_char*,struct battle_char*,uint8_t);

	switch(claction[jobindex][findex].mod){
		case 1:
			hitmod=mod1;
			break;
		case 3:
			hitmod=mod3;
			break;
		case 4:
			hitmod=mod4;
			break;
		case 6:
			hitmod=mod6;
			break;
		default:
			hitmod=NULL;
			base_hit=100;
	}

	if(hitmod)
		base_hit=hitmod(source,target,claction[jobindex][findex].mod_v);

	return base_hit;
}

void fast_action(struct battle_char *source, struct battle_char *target, int jobindex, int findex){
	struct stored_action thisact;
	uint8_t type;
	const struct ability *a=&claction[jobindex][findex];
	char msg[30];

	if(claction[jobindex][findex].flags&AFLAG_PHYSICAL)
		type=AFLAG_PHYSICAL;
	else
		type=AFLAG_MAGIC;

	if(source->mp<a->mp || (source->ch->support==SFLAG_HALFMP && source->mp<a->mp/2))
		return;

	source->mp-=source->ch->support==SFLAG_HALFMP?source->mp<a->mp/2:a->mp;
	
	sprintf(msg,"%d used %s",source->index,a->name);
	print_message(msg);

	thisact.ctr=0;
	thisact.jobindex=jobindex;
	thisact.findex=findex;
	thisact.f=a->f.af;
	thisact.origin=source;
	thisact.target.x=target->x;
	thisact.target.y=target->y;
	thisact.target.width=a->ra.aoe;
	thisact.target.vertical=a->ra.aoe_vertical;
	thisact.target.dir=AOE_DIR(a->ra.dir);
	last_action.damage=NO_DAMAGE;

	if(!(claction[jobindex][findex].flags&AFLAG_EVADE && evaded(target,type,get_attack_dir(source,target),get_base_hit(source,target,jobindex,findex))))
		a->f.af(source,target);

	last_action.preresolve=&thisact;

	react(source,&target,1);

	last_action.mp_used=a->mp;
	last_action.item=0;

}

void slow_action(struct battle_char *source, int x, int y, int jobindex, int findex){
	struct stored_action *thisact;
	uint8_t type;
	const struct ability *a=&claction[jobindex][findex];

	if(source->mp<a->mp || (source->ch->support==SFLAG_HALFMP && source->mp<a->mp/2))
		return;

	source->mp-=source->ch->support==SFLAG_HALFMP?source->mp<a->mp/2:a->mp;

	source->slow_act=thisact=malloc(sizeof(*thisact));
	if(thisact==NULL)
		exit(1);
	
	if(a->flags&AFLAG_PHYSICAL)
		type=AFLAG_PHYSICAL;
	else
		type=AFLAG_MAGIC;

	thisact->ctr=a->ctr;
	thisact->jobindex=jobindex;
	thisact->findex=findex;
	thisact->f=a->f.af;
	thisact->origin=source;
	thisact->target.x=x;
	thisact->target.y=y;
	thisact->target.width=a->ra.aoe;
	thisact->target.vertical=a->ra.aoe_vertical;
	thisact->target.dir=AOE_DIR(a->ra.dir);
}

void slow_action_resolution(struct battle_char **blist, int num){
	int bi;
	struct stored_action *tmp;
	struct battle_char **targets;
	int num_t;
	int type;
	int dir;
	int base_hit;
	char msg[30];

	for(bi=0;bi<num;bi++){
		tmp=blist[bi]->slow_act;
		if(tmp && tmp->ctr==0){
			if(claction[tmp->jobindex][tmp->findex].flags&AFLAG_PHYSICAL)
				type=AFLAG_PHYSICAL;
			else
				type=AFLAG_MAGIC;

			sprintf(msg,"%d used %s",blist[bi]->index,claction[tmp->jobindex][tmp->findex].name);
			print_message(msg);

			//prereact(tmp->origin,tmp->target,tmp->num_target); // for hamedo
			targets=get_targets(blist,bi,num,tmp->target.x,tmp->target.y,tmp->target.width,tmp->target.vertical,tmp->target.dir);
			last_action.preresolve=tmp;
			for(num_t=0;targets[num_t];num_t++){
				last_action.damage=NO_DAMAGE;
				dir=get_attack_dir(blist[bi],targets[num_t]);
				base_hit=get_base_hit(blist[bi],targets[num_t],tmp->jobindex,tmp->findex);
				if(!(claction[tmp->jobindex][tmp->findex].flags&AFLAG_EVADE && evaded(targets[num_t],type,dir,base_hit)))
					tmp->f(tmp->origin,targets[num_t]);
				react(tmp->origin,targets,num_t);
			}
			free(targets);
			free(tmp);
			blist[bi]->slow_act=NULL;
		}
	}
}

void ct_charge(struct battle_char **blist, int num){
	int bi;
	
	for(bi=0;bi<num;bi++){
		if(!STATUS_SET(blist[bi],STATUS_STOP) && !STATUS_SET(blist[bi],STATUS_SLEEP) && !STATUS_SET(blist[bi],STATUS_PETRIFY))
			blist[bi]->ct+=blist[bi]->speed;
	}
}

void ct_resolution(struct battle_char **blist, int *num){
	int bi;
	uint8_t flags=0;
	
	for(bi=0;bi<*num;bi++){
		if(blist[bi]->ct>=100){
			if(STATUS_SET(blist[bi],STATUS_DEAD)){
				blist[bi]->status[STATUS_DEAD]--;
				if(blist[bi]->status[STATUS_DEAD]==0){
					*num-=1;
					blist[bi]=blist[*num];
					bi--;
				}
				continue;
			}
			set_map_moves(blist[bi]->x,blist[bi]->y,blist[bi]->move,blist[bi]->jump,clmovement[blist[bi]->ch->movement_class][blist[bi]->ch->movement_class].flags);
			ai_unit_init(blist[bi]);

			flags=0;
			if(STATUS_SET(blist[bi],STATUS_NOMOVE))
				flags|=MOVED_FLAG;
			if(STATUS_SET(blist[bi],STATUS_NOACT))
				flags|=ACTED_FLAG;

			// Select new actions
			if(flags==0){
				if(blist[bi]->fof==FOF_FRIEND)
					battle_orders(blist,bi,*num,&flags);
				else
					ai_battle_orders(blist,bi,*num,&flags);
			}

			// Select other action
			if(flags!=0 && flags!=(ACTED_FLAG|MOVED_FLAG)){
				if(blist[bi]->fof==FOF_FRIEND)
					battle_orders(blist,bi,*num,&flags);
				else
					ai_battle_orders(blist,bi,*num,&flags);
			}

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
	const struct eq_item *eqgroup;

	// HP,MP,MA,PA,SP
	set_battle_stats(bc);

	bc->faith=bc->ch->faith;
	bc->brave=bc->ch->brave;

	bc->slow_act=NULL;

	for(i=0;i<NUM_STATUS;i++)
		bc->status[i]=0;

	for(i=0;i<NUM_ELEM;i++)
		bc->resist[i]=0;

	bc->strengthen=0;
	bc->add_status=0;
	bc->ct=0;
	bc->move=class_stats[bc->ch->primary].move;
	bc->jump=class_stats[bc->ch->primary].jump;

	eqgroup=&weapons[EQ_TYPE(bc->ch->eq[EQ_WEAPON])][(bc->ch->eq[EQ_WEAPON]>>6)];
	bc->wp=eqgroup->wp;
	if(eqgroup->wear)eqgroup->wear(bc);
	
	if(EQ_TYPE(bc->ch->eq[EQ_OFFHAND])==EQO_SHIELD)
		eqgroup=&offhand[(bc->ch->eq[EQ_OFFHAND]>>6)];
	else
		eqgroup=&weapons[EQ_TYPE(bc->ch->eq[EQ_OFFHAND])][(bc->ch->eq[EQ_OFFHAND]>>6)];
	if(eqgroup->wear)eqgroup->wear(bc);
	
	eqgroup=&head_armor[EQ_TYPE(bc->ch->eq[EQ_HEAD])][(bc->ch->eq[EQ_HEAD]>>6)];
	if(eqgroup->wear)eqgroup->wear(bc);
	
	eqgroup=&body_armor[EQ_TYPE(bc->ch->eq[EQ_BODY])][(bc->ch->eq[EQ_BODY]>>6)];
	if(eqgroup->wear)eqgroup->wear(bc);
	
	eqgroup=&misc_armor[EQ_TYPE(bc->ch->eq[EQ_MISC])][(bc->ch->eq[EQ_MISC]>>6)];
	if(eqgroup->wear)eqgroup->wear(bc);
	
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

int battle_ended(struct battle_char **blist, int num){
	int numfriend;
	int numfoe;
	int i;

	for(numfriend=numfoe=i=0;i<num;i++){
		if(blist[i]->fof==FOF_FRIEND)
			numfriend++;
		else
			numfoe++;
	}
	
	if(numfriend==0)
		return FOF_FOE;
	if(numfoe==0)
		return FOF_FRIEND;

	return -1;
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

	ai_init(pblist+bi-numfoe,foe_placed);

	while(1){
		status_check(pblist,pi);
		slow_action_charge(pblist,pi);
		slow_action_resolution(pblist,pi);
		ct_charge(pblist,pi);
		ct_resolution(pblist,&pi);
		if(battle_ended(pblist,pi)>=0)
			break;
	}
}
