#include <stdlib.h>
#include <stdint.h>
#include "ai.h"
#include "player.h"
#include "battle.h"
#include "map.h"
#include "util.h"
#include "equipment.h"

#define NUM_OPTIONS 10
#define NO_ACT_GROUP 0xFF
struct turn_opt{
	struct move{
		uint8_t x;
		uint8_t y;
	}move_opt;
	struct act{
		uint8_t x;
		uint8_t y;
		uint8_t group;
		uint8_t cmd;
	}act_opt;
	uint8_t score;
}turn_opts[NUM_OPTIONS];

const uint8_t class_role[]={
	AI_ROLE_MELEE,
	AI_ROLE_DAMAGE, // Archer
	AI_ROLE_HEAL|AI_ROLE_BUFF, // Bard
	0, // Calculator
	0, // Chemist
	AI_ROLE_DAMAGE|AI_ROLE_DEBUFF, // Dancer
	AI_ROLE_DAMAGE, // Geomancer
	AI_ROLE_MELEE|AI_ROLE_DEBUFF, // Knight
	AI_ROLE_MELEE|0, // Lancer
	0, // Mediator
	0, // Mime
	AI_ROLE_MELEE|AI_ROLE_DAMAGE|AI_ROLE_HEAL, // Monk
	AI_ROLE_MELEE|0, // Ninja
	AI_ROLE_DEBUFF, // Oracle
	AI_ROLE_HEAL|AI_ROLE_BUFF, // Priest
	AI_ROLE_MELEE|AI_ROLE_DAMAGE|AI_ROLE_DEBUFF, // Samurai
	AI_ROLE_MELEE|AI_ROLE_DAMAGE|AI_ROLE_BUFF, // Squire
	AI_ROLE_HEAL|AI_ROLE_DAMAGE|AI_ROLE_DEBUFF, // Summoner
	AI_ROLE_MELEE|0, // Thief
	AI_ROLE_DAMAGE|AI_ROLE_BUFF|AI_ROLE_DEBUFF, // Time Mage
	AI_ROLE_DAMAGE|AI_ROLE_DEBUFF, // Wizard
};

int life_percent(struct battle_char *t, int dmg){
	int ret;

	if(t->hp<1)
		return 0;

	ret=((abs(t->hp-dmg)*100)/t->hp);
	ret=100-ret;

	if(dmg<=0)
		ret=0;
	else if(ret<=0)
		ret=1;

	return ret;
}

int estimated_output(struct battle_char *s, struct battle_char *t, int mod, int var){
	uint16_t pri=s->ch->eq[EQ_WEAPON];
	switch(mod){
		case 0:
			break;
		case 1:
			break;
		case 2:
			if(var==AFLAG_MOD_PA)
				return life_percent(t,mod2(s,t,s->pa));
			else if(var==AFLAG_MOD_MA)
				return life_percent(t,mod2(s,t,s->ma));
			else if(var==AFLAG_MOD_XA)
				return life_percent(t,weapon_damage[EQ_TYPE(pri)](&weapons[EQ_TYPE(pri)][pri>>6],s,t));
			break;
		case 3:
			return life_percent(t,mod3(s,t,var));
			break;
		case 4:
			break;
		case 5:
			return life_percent(t,mod5(s,t,var,100));
			break;
		case 6:
			return life_percent(t,mod6(s,t,var));
			break;
	}
	return 50;
}


uint16_t sum_ability_flags(struct battle_char *bc){
	uint16_t ability_flags=0;
	int i,g;
	
	g=bc->ch->primary;
	for(i=0;i<num_action[g];i++)
		if(bc->ch->mastery[g]&BIT(i))
			ability_flags|=claction[g][i].flags;

	g=bc->ch->secondary;
	for(i=0;i<num_action[g];i++)
		if(bc->ch->mastery[g]&BIT(i))
			ability_flags|=claction[g][i].flags;
		
	return ability_flags;
}

uint8_t get_roles(uint8_t class_flags, uint16_t ability_flags){
	if(!(ability_flags&(AI_AFLAG_HEAL)))
		class_flags&=~AI_ROLE_HEAL;

	if(!(ability_flags&(AI_AFLAG_DAMAGE)))
		class_flags&=~AI_ROLE_DAMAGE;

	if(!(ability_flags&(AI_AFLAG_BUFF)))
		class_flags&=~AI_ROLE_BUFF;

	if(!(ability_flags&(AI_AFLAG_DEBUFF)))
		class_flags&=~AI_ROLE_DEBUFF;
	
	return class_flags;
}

uint16_t filter_ability_by_role(uint8_t role, uint16_t ability_flags, int group){
	int i;
	uint16_t filter=0;
	uint16_t ret=0;

	if(role&AI_ROLE_DAMAGE)
		filter|=AI_AFLAG_DAMAGE;
	if(role&AI_ROLE_HEAL)
		filter|=AI_AFLAG_HEAL;
	if(role&AI_ROLE_BUFF)
		filter|=AI_AFLAG_BUFF;
	if(role&AI_ROLE_DEBUFF)
		filter|=AI_AFLAG_DEBUFF;
	//if(role&AI_ROLE_MELEE)
		//filter|=AI_AFLAG_MELEE;
	
	for(i=0;i<num_action[group];i++)
		if(ability_flags&BIT(i) && claction[group][i].flags&filter)
			ret|=BIT(i);

	return ret;
}

int e_gcd(int a, int b){
	int x;
	if(b>a){
		x=a;
		a=b;
		b=x;
	}
	while((x=a%b)!=0){
		a=b;
		b=x;
	}
	return b;
}

void set_move(struct battle_char *bc, int opti, int x, int y, int dist, int range){
	int best=dist-range;
	int temp;
	int tx,ty;
	int rise=y-bc->y;
	int run=x-bc->x;
	int md;
	int ysign,xsign;
	int i;

	if(rise==run)
		rise=run=1;
	else if(rise>1 && run>1){
		temp=e_gcd(abs(rise),abs(run));
		rise/=temp;
		run/=temp;
	}

	temp=best/(rise+run);
	tx=bc->x+temp*run;
	ty=bc->y+temp*rise;
	md=temp*rise+temp*run;
	ysign=rise>=0?1:-1;
	xsign=run>=0?1:-1;

	if(abs(rise)>abs(run)){
		//y dom
		for(i=0;md<best && i<rise;i++){
			ty+=ysign;
			md++;
		}
		for(i=0;md<best && i<run;i++){
			tx+=xsign;
			md++;
		}
		do{
			if(move_valid(tx,ty)){
				turn_opts[opti].move_opt.x=tx;
				turn_opts[opti].move_opt.y=ty;
				turn_opts[opti].score=1;
				return;
			}
			tx+=xsign;
			ty-=ysign;
		}while(bc->move>=tx*xsign-bc->x + ty*ysign-bc->y);
	}
	else{
		//x dom
		for(i=0;md<best && i<run;i++){
			tx+=xsign;
			md++;
		}
		for(i=0;md<best && i<rise;i++){
			ty+=ysign;
			md++;
		}
		do{
			if(move_valid(tx,ty)){
				turn_opts[opti].move_opt.x=tx;
				turn_opts[opti].move_opt.y=ty;
				turn_opts[opti].score=1;
				return;
			}
			ty+=ysign;
			tx-=xsign;
		}while(bc->move>=tx*xsign-bc->x + ty*ysign-bc->y);
	}

}

int add_action(struct battle_char *bc, int x, int y, int dist, int group, int acti, int flagscan){
	int i;

	for(i=0;i<NUM_OPTIONS;i++){
		if(turn_opts[i].score==0){
			if(dist<=claction[group][acti].ra.range){
				turn_opts[i].move_opt.x=bc->x;
				turn_opts[i].move_opt.y=bc->y;
				turn_opts[i].score=1;
			}
			else
				set_move(bc,i,x,y,dist,claction[group][acti].ra.range);

			turn_opts[i].act_opt.x=x;
			turn_opts[i].act_opt.y=y;
			turn_opts[i].act_opt.group=group;
			turn_opts[i].act_opt.cmd=acti;
			return turn_opts[i].score>0?i:-1;
		}
	}
	return -1;
}

int score_action(struct battle_char *s, struct battle_char *t, int group, int cmd){
	int score;
	struct stored_action pr;

	pr.jobindex=group;
	pr.findex=cmd;
	last_action.preresolve=&pr;
	score=estimated_output(s,t,claction[group][cmd].mod,claction[group][cmd].mod_v);
	if(score==1 && claction[group][cmd].mp>0)
		score=100-((claction[group][cmd].mp*100)/s->mp);

	return score;
}

int scan_action(struct battle_char **blist, int bc, int bi, int x, int y, int dist, int group, int flagscan){
	int i;
	uint16_t avail=0;
	uint16_t max=0;
	uint16_t avail_flag=0;
	uint8_t role=0;
	int temp,max_score=0,max_i=0;

	// Select actions that won't automatically fail
	for(i=0;i<num_action[group];i++)
		if(claction[group][i].flags&flagscan &&
		  blist[bc]->ch->mastery[group]&BIT(i) &&
		  dist<=blist[bc]->move+claction[group][i].ra.range && 
		  (blist[bc]->mp>=claction[group][i].mp || (blist[bc]->ch->support==SFLAG_HALFMP && blist[bc]->mp>=claction[group][i].mp/2))){
			avail|=BIT(i);
			avail_flag|=claction[group][i].flags;
		}

	role=get_roles(blist[bc]->ai_role,avail_flag);

	if(role==0)
		return -1;

	// Select one of the available roles
	if(role&AI_ROLE_HEAL)
		role=AI_ROLE_HEAL;
	else if(role&AI_ROLE_BUFF)
		role=AI_ROLE_BUFF;

	if(role&AI_ROLE_DEBUFF && role!=AI_ROLE_DEBUFF && !STATUS_SET(blist[bi],STATUS_CRITICAL) && get_random(0,4)==0)
		role=AI_ROLE_DEBUFF;
	else if(role&AI_ROLE_DAMAGE)
		role=AI_ROLE_DAMAGE;
	//else
		//role=AI_ROLE_MELEE;

	// Select actions in that role
	avail=filter_ability_by_role(role,avail,group);

	if(avail==0)
		return -1;

	// Find the best action
	max=max_i=0;
	for(i=0;i<num_action[group];i++){
		if(avail&BIT(i)){
			temp=score_action(blist[bc],blist[bi],group,i);
			if(temp>max_score){
				max_score=temp;
				max_i=1;
				max=BIT(i);
			}
			else if(temp==max_score){
				max|=BIT(i);
				max_i++;
			}
		}
	}

	if(max_i>1){
		temp=get_random(1,max_i+1);
		for(i=0;i<num_action[group];i++){
			if(max&BIT(i))
				temp--;
			if(temp==0)
				return i;
		}
	}
	else{
		for(i=0;i<num_action[group];i++)
			if(max&BIT(i))
				return i;
	}

	return -1;
}

void select_action_type(struct battle_char **blist, int bi, int num, uint16_t ability_flags, uint8_t flags){
	int i;
	int dist;
	int opi=-1;
	int flagscan=0;
	int act;
	int score;

	for(i=0;i<num;i++){
		opi=-1;
		flagscan=0;
		dist=get_dist(blist[bi],blist[i]->x,blist[i]->y,0,0,0);

		if(blist[bi]->fof==blist[i]->fof){
			if(blist[bi]->ai_role&AI_ROLE_HEAL){
				if(STATUS_SET(blist[i],STATUS_DEAD) && ability_flags&AFLAG_RESTORE_LIFE){
					flagscan=AFLAG_RESTORE_LIFE;
					score=10;
				}
				else if(blist[i]->hp<blist[i]->hp_max/2 && ability_flags&AFLAG_RESTORE_HP){
					flagscan=AFLAG_RESTORE_HP;
					score=5;
				}
				else if(blist[i]->mp<blist[i]->mp_max/4 && ability_flags&AFLAG_RESTORE_MP){
					flagscan=AFLAG_RESTORE_MP;
					score=2;
				}
			}		
			else if(blist[bi]->ai_role&AI_ROLE_BUFF){
				flagscan=AFLAG_BUFF;
				score=3;
			}
		}
		else{
			if(blist[bi]->ai_role&AI_ROLE_DAMAGE){
				flagscan|=AFLAG_DAMAGE;
			}
			if(blist[bi]->ai_role&AI_ROLE_DEBUFF){
				flagscan|=AFLAG_DEBUFF;
			}
			if(STATUS_SET(blist[i],STATUS_DEAD))
				flagscan=0;

			score=4;
		}

		if(flagscan>0){
			act=scan_action(blist,bi,i,blist[i]->x,blist[i]->y,dist,blist[bi]->ch->primary,flagscan);
			if(act>=0)
				opi=add_action(blist[bi],blist[i]->x,blist[i]->y,dist,blist[bi]->ch->primary,act,flagscan);
			else{
				scan_action(blist,bi,i,blist[i]->x,blist[i]->y,dist,blist[bi]->ch->secondary,flagscan);
				if(act>=0)
					opi=add_action(blist[bi],blist[i]->x,blist[i]->y,dist,blist[bi]->ch->secondary,act,flagscan);
			}
		}

		if(opi>=0)
			turn_opts[opi].score=score;
	}
}

int ai_move(struct battle_char **blist, int bi, int num){
	int i;
	int max_i=0;

	for(i=1;i<NUM_OPTIONS;i++){
		if(turn_opts[i].score>turn_opts[max_i].score)
			max_i=i;
	}
	if(turn_opts[max_i].score>0){
		move(blist[bi],turn_opts[max_i].move_opt.x,turn_opts[max_i].move_opt.y);
		return 1;
	}

	if(class_role[blist[bi]->ch->primary]&(AI_ROLE_HEAL|AI_ROLE_BUFF)){
		for(i=0;i<num;i++){
			if(blist[i]->fof==blist[bi]->fof && class_role[blist[i]->ch->primary]&(AI_ROLE_MELEE))
				break;
		}
	}
	else{
		for(i=0;i<num;i++){
			if(blist[i]->fof!=blist[bi]->fof)
				break;
		}
	}

	set_move(blist[bi],0,blist[i]->x,blist[i]->y,blist[bi]->move,0);
	turn_opts[0].act_opt.group=NO_ACT_GROUP;
	if(turn_opts[0].score){
		move(blist[bi],turn_opts[0].move_opt.x,turn_opts[0].move_opt.y);
		return 1;
	}

	return 0;
}

int ai_act(struct battle_char **blist, int bi, int num){
	int i;
	int max_i=0;
	struct battle_char **tl;
	int g,cmd;
	for(i=1;i<NUM_OPTIONS;i++){
		if(turn_opts[i].score>turn_opts[max_i].score)
			max_i=i;
	}
	if(turn_opts[max_i].score>0 && turn_opts[max_i].act_opt.group!=NO_ACT_GROUP){
		g=turn_opts[max_i].act_opt.group;
		cmd=turn_opts[max_i].act_opt.cmd;
		if(claction[g][cmd].ctr==0){
			tl=get_targets(blist,bi,num,turn_opts[max_i].act_opt.x,turn_opts[max_i].act_opt.y,claction[g][cmd].ra.aoe,claction[g][cmd].ra.aoe_vertical,AOE_DIR(claction[g][cmd].ra.dir));
			for(i=0;i<num && tl[i];i++)
				fast_action(blist[bi],tl[i],g,cmd);
			free(tl);
		}
		else{
			slow_action(blist[bi],turn_opts[max_i].act_opt.x,turn_opts[max_i].act_opt.y,g,cmd);
		}
		return 1;
	}
	return 0;
}

void ai_battle_orders(struct battle_char **blist, int bi, int num, uint8_t *flags){
	uint16_t ability_flags=0;
	int i;

	for(i=0;i<NUM_OPTIONS && turn_opts[i].score==0;i++);	
	if(i==NUM_OPTIONS){
		ability_flags=sum_ability_flags(blist[bi]);
		select_action_type(blist,bi,num,ability_flags,*flags);
	}

	if(!(*flags&MOVED_FLAG)){
		if(ai_move(blist,bi,num))
			*flags|=MOVED_FLAG;
		return;
	}
	if(!(*flags&ACTED_FLAG)){
		if(ai_act(blist,bi,num))
			*flags|=ACTED_FLAG;
		return;
	}
}

void ai_unit_init(struct battle_char *bc){
	int i;

	for(i=0;i<NUM_OPTIONS;i++)
		turn_opts[i].score=0;
}

void ai_init(struct battle_char **blist, int num){
	int bi;
	uint16_t ability_flags=0;
	uint8_t class_flags=0;

	for(bi=0;bi<num;bi++){
		class_flags=class_role[blist[bi]->ch->primary]|class_role[blist[bi]->ch->secondary];
		ability_flags=sum_ability_flags(blist[bi]);

		blist[bi]->ai_role=get_roles(class_flags,ability_flags);
	}
}

