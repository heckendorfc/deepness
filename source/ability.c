#include <stdlib.h>
#include "battle.h"
#include "ability.h"
#include "player.h"
#include "equipment.h"
#include "util.h"
#include "map.h"

static int is_critical_hit(struct battle_char *attacker){
	if(get_random(0,20)==0)
		return 1;
	return 0;
}

/* Neutral success */
uint8_t mod1(struct battle_char *origin, struct battle_char *target, uint8_t var){
	uint8_t compat=sign_compat(origin->ch,target->ch);
	int16_t z=0;

	if(compat==SIGN_COMPAT_GOOD)
		z=origin->ma/4 + var/4;
	else if(compat==SIGN_COMPAT_BAD)
		z=-origin->ma/4 - var/4;
	else if(compat==SIGN_COMPAT_BEST)
		z=origin->ma/2 + var/2;
	else if(compat==SIGN_COMPAT_WORST)
		z=-origin->ma/2 - var/2;

	return origin->ma+var+z;
}

/* Physical damage */
uint16_t mod2(struct battle_char *attacker, struct battle_char *defender, uint16_t xa){
	uint8_t compat=sign_compat(attacker->ch,defender->ch);

	if(is_critical_hit(attacker))
		xa+=get_random(1,xa)-1;
// TODO: add Strengthen bonus here
	if(STATUS_SET(attacker,STATUS_ATTACKUP))
		xa=xa*4/3;
	if(attacker->ch->eq[EQ_WEAPON]==0 && attacker->ch->support==SFLAG_MARTIAL_ARTS)
		xa=xa*3/2;
	if(STATUS_SET(attacker,STATUS_BERSERK))
		xa=xa*3/2;
	if(STATUS_SET(defender,STATUS_DEFENSEUP))
		xa=xa*2/3;
	if(STATUS_SET(defender,STATUS_PROTECT))
		xa=xa*2/3;
	if(STATUS_SET(defender,STATUS_CHARGING))
		xa=xa*3/2;
	if(STATUS_SET(defender,STATUS_SLEEPING))
		xa=xa*3/2;
	if(STATUS_SET(defender,STATUS_POLYMORPH))
		xa=xa*3/2;
	
	if(compat==SIGN_COMPAT_GOOD)
		xa+=xa/4;
	else if(compat==SIGN_COMPAT_BAD)
		xa-=xa/4;
	else if(compat==SIGN_COMPAT_BEST)
		xa+=xa/2;
	else if(compat==SIGN_COMPAT_WORST)
		xa-=xa/2;

	return xa;
}

int16_t mod2_elem(struct battle_char *attacker, struct battle_char *defender, uint16_t dmg){
	uint8_t elem=claction[last_action.preresolve->jobindex][last_action.preresolve->findex].elem;

	if(elem==ELEM_WEAPON)
		elem=weapons[EQ_TYPE(attacker->ch->eq[EQ_WEAPON])][attacker->ch->eq[EQ_WEAPON]>>6].elem;

	if(defender->resist[elem]&RESIST_WEAK)
		dmg*=2;
	if(defender->resist[elem]&RESIST_HALF)
		dmg/=2;
	if(defender->resist[elem]&RESIST_ABSORB)
		dmg=-dmg;

	return dmg;
}

/* Physical success */
uint8_t mod3(struct battle_char *attacker, struct battle_char *defender, uint8_t var){
	uint8_t compat=sign_compat(attacker->ch,defender->ch);
	uint8_t xa=attacker->pa;

	if(STATUS_SET(attacker,STATUS_ATTACKUP))
		xa=xa*4/3;
	if(attacker->ch->eq[EQ_WEAPON]==0 && attacker->ch->support==SFLAG_MARTIAL_ARTS)
		xa=xa*3/2;
	if(STATUS_SET(defender,STATUS_DEFENSEUP))
		xa=xa*2/3;
	if(STATUS_SET(defender,STATUS_PROTECT))
		xa=xa*2/3;
	if(STATUS_SET(defender,STATUS_CHARGING))
		xa=xa*3/2;
	if(STATUS_SET(defender,STATUS_SLEEPING))
		xa=xa*3/2;
	if(STATUS_SET(defender,STATUS_POLYMORPH))
		xa=xa*3/2;

	if(compat==SIGN_COMPAT_GOOD)
		xa+=xa/4 + var/4;
	else if(compat==SIGN_COMPAT_BAD)
		xa+=-xa/4 - var/4;
	else if(compat==SIGN_COMPAT_BEST)
		xa+=xa/2 + var/2;
	else if(compat==SIGN_COMPAT_WORST)
		xa+=-xa/2 - var/2;

	return xa;
}

/* Steal success */
uint8_t mod4(struct battle_char *attacker, struct battle_char *defender, uint8_t var){
	uint8_t compat=sign_compat(attacker->ch,defender->ch);
	uint8_t xa=attacker->speed;

	if(STATUS_SET(attacker,STATUS_ATTACKUP))
		xa=xa*4/3;
	if(attacker->ch->eq[EQ_WEAPON]==0 && attacker->ch->support==SFLAG_MARTIAL_ARTS)
		xa=xa*3/2;
	if(STATUS_SET(defender,STATUS_DEFENSEUP))
		xa=xa*2/3;
	if(STATUS_SET(defender,STATUS_PROTECT))
		xa=xa*2/3;
	if(STATUS_SET(defender,STATUS_CHARGING))
		xa=xa*3/2;
	if(STATUS_SET(defender,STATUS_SLEEPING))
		xa=xa*3/2;
	if(STATUS_SET(defender,STATUS_POLYMORPH))
		xa=xa*3/2;

	if(compat==SIGN_COMPAT_GOOD)
		xa+=xa/4 + var/4;
	else if(compat==SIGN_COMPAT_BAD)
		xa+=-xa/4 - var/4;
	else if(compat==SIGN_COMPAT_BEST)
		xa+=xa/2 + var/2;
	else if(compat==SIGN_COMPAT_WORST)
		xa+=-xa/2 - var/2;

	return xa+var;
}

/* Magic damage */
int16_t mod5(struct battle_char *origin, struct battle_char *target, uint8_t var, uint8_t success){
	uint8_t compat=sign_compat(origin->ch,target->ch);
	uint8_t ma=origin->ma;
	uint8_t tfa=target->faith, cfa=origin->faith;
	int8_t n=1;
	int8_t d=1;

// TODO strengthen here
	if(STATUS_SET(origin,STATUS_ATTACKUP))
		ma=ma*4/3;
	if(target->fof!=origin->fof && STATUS_SET(target,STATUS_DEFENSEUP))
		ma=ma*2/3;
	if(target->fof!=origin->fof && STATUS_SET(target,STATUS_SHELL))
		ma=ma*2/3;
	
	if(compat==SIGN_COMPAT_GOOD)
		ma+=ma/4;
	else if(compat==SIGN_COMPAT_BAD)
		ma-=-ma/4;
	else if(compat==SIGN_COMPAT_BEST)
		ma+=ma/2;
	else if(compat==SIGN_COMPAT_WORST)
		ma-=-ma/2;

// TODO un-truth attacks here
	if(STATUS_SET(target,STATUS_FAITH))
		tfa=100;
	else if(STATUS_SET(target,STATUS_INNOCENT))
		tfa=0;

	if(STATUS_SET(origin,STATUS_FAITH))
		cfa=100;
	else if(STATUS_SET(origin,STATUS_INNOCENT))
		cfa=0;
	
// TODO weather here
	if(target->resist[claction[last_action.preresolve->jobindex][last_action.preresolve->findex].elem]&RESIST_WEAK)
		n*=2;
	if(target->resist[claction[last_action.preresolve->jobindex][last_action.preresolve->findex].elem]&RESIST_HALF)
		d*=2;
	if(target->resist[claction[last_action.preresolve->jobindex][last_action.preresolve->findex].elem]&RESIST_ABSORB)
		n=-n;

	return (cfa*tfa*var*ma*n)/(10000*d);
}

/* Magic success */
uint8_t mod6(struct battle_char *origin, struct battle_char *target, uint8_t var){
	uint16_t ma=origin->ma;
	uint8_t tfa=target->faith, cfa=origin->faith;
	uint8_t compat=sign_compat(origin->ch,target->ch);

	if(STATUS_SET(target,STATUS_FAITH))
		tfa=100;
	else if(STATUS_SET(target,STATUS_INNOCENT))
		tfa=0;

	if(STATUS_SET(origin,STATUS_FAITH))
		cfa=100;
	else if(STATUS_SET(origin,STATUS_INNOCENT))
		cfa=0;

// TODO strengthen here
	if(STATUS_SET(origin,STATUS_ATTACKUP))
		ma=ma*4/3;
	if(origin->fof!=target->fof && STATUS_SET(target,STATUS_DEFENSEUP))
		ma=ma*2/3;
	if(origin->fof!=target->fof && STATUS_SET(target,STATUS_SHELL))
		ma=ma*2/3;

	if(compat==SIGN_COMPAT_GOOD)
		ma+=ma/4 + var/4;
	else if(compat==SIGN_COMPAT_BAD)
		ma-=-ma/4 - var/4;
	else if(compat==SIGN_COMPAT_BEST)
		ma+=ma/2 + var/2;
	else if(compat==SIGN_COMPAT_WORST)
		ma-=-ma/2 - var/2;

	return cfa*tfa*(ma+var) / 10000;
}

static void cure(struct battle_char *origin, struct battle_char *target){
	int16_t var=mod5(origin,target,14,100);
	deal_damage(target,-var);
	last_action.damage=-var;
}

static void cure2(struct battle_char *origin, struct battle_char *target){
	int16_t var=mod5(origin,target,20,100);
	deal_damage(target,-var);
	last_action.damage=-var;
}

static void cure3(struct battle_char *origin, struct battle_char *target){
	int16_t var=mod5(origin,target,30,100);
	deal_damage(target,-var);
	last_action.damage=-var;
}

static void cure4(struct battle_char *origin, struct battle_char *target){
	int16_t var=mod5(origin,target,40,100);
	deal_damage(target,-var);
	last_action.damage=-var;
}

static void raise(struct battle_char *origin, struct battle_char *target){
	int8_t var=target->hp_max/2;
	if(get_random(0,100)>=mod6(origin,target,180))
		return;
	if(STATUS_SET(target,STATUS_DEAD) && !STATUS_SET(target,STATUS_UNDEAD)){
		remove_status(target,STATUS_DEAD);
		deal_damage(target,-var);
	}
	else if(STATUS_SET(target,STATUS_UNDEAD)){
		deal_damage(target,var);
	}
}

static void raise2(struct battle_char *origin, struct battle_char *target){
	int8_t var=target->hp_max/2;
	if(get_random(0,100)>=mod6(origin,target,160))
		return;
	if(STATUS_SET(target,STATUS_DEAD) && !STATUS_SET(target,STATUS_UNDEAD)){
		remove_status(target,STATUS_DEAD);
		deal_damage(target,-var);
	}
	else if(STATUS_SET(target,STATUS_UNDEAD)){
		deal_damage(target,var);
	}
}

static void reraise(struct battle_char *origin, struct battle_char *target){
	if(!STATUS_SET(target,STATUS_UNDEAD) && get_random(0,100)<mod6(origin,target,140))
		add_status(target,STATUS_RERAISE);
}

static void regen(struct battle_char *origin, struct battle_char *target){
	if(get_random(0,100)<mod6(origin,target,170))
		add_status(target,STATUS_REGEN);
}

static void protect(struct battle_char *origin, struct battle_char *target){
	if(get_random(0,100)<mod6(origin,target,200))
		add_status(target,STATUS_PROTECT);
}

static void protect2(struct battle_char *origin, struct battle_char *target){
	if(get_random(0,100)<mod6(origin,target,120))
		add_status(target,STATUS_PROTECT);
}

static void shell(struct battle_char *origin, struct battle_char *target){
	if(get_random(0,100)<mod6(origin,target,200))
		add_status(target,STATUS_SHELL);
}

static void shell2(struct battle_char *origin, struct battle_char *target){
	if(get_random(0,100)<mod6(origin,target,120))
		add_status(target,STATUS_SHELL);
}

static void wall(struct battle_char *origin, struct battle_char *target){
	if(get_random(0,100)<mod6(origin,target,140)){
		add_status(target,STATUS_SHELL);
		add_status(target,STATUS_PROTECT);
	}
}

static void esuna(struct battle_char *origin, struct battle_char *target){
	if(get_random(0,100)<mod6(origin,target,190)){
		remove_status(target,STATUS_PETRIFY);
		remove_status(target,STATUS_DARKNESS);
		remove_status(target,STATUS_CONFUSION);
		remove_status(target,STATUS_SILENCE);
		remove_status(target,STATUS_BERSERK);
		remove_status(target,STATUS_POLYMORPH);
		remove_status(target,STATUS_POISON);
		remove_status(target,STATUS_SLEEPING);
		remove_status(target,STATUS_NOMOVE);
		remove_status(target,STATUS_NOACT);
	}
}

static void holy(struct battle_char *origin, struct battle_char *target){
	uint16_t dmg = mod5(origin,target,50,100);
	deal_damage(target,dmg);
	last_action.damage=dmg;
}

static void fire(struct battle_char *origin, struct battle_char *target){
	uint16_t dmg = mod5(origin,target,14,100);
	deal_damage(target,dmg);
	last_action.damage=dmg;
}

static void fire2(struct battle_char *origin, struct battle_char *target){
	uint16_t dmg = mod5(origin,target,18,100);
	deal_damage(target,dmg);
	last_action.damage=dmg;
}

static void fire3(struct battle_char *origin, struct battle_char *target){
	uint16_t dmg = mod5(origin,target,24,100);
	deal_damage(target,dmg);
	last_action.damage=dmg;
}

static void fire4(struct battle_char *origin, struct battle_char *target){
	uint16_t dmg = mod5(origin,target,32,100);
	deal_damage(target,dmg);
	last_action.damage=dmg;
}

static void bolt(struct battle_char *origin, struct battle_char *target){
	uint16_t dmg = mod5(origin,target,14,100);
	deal_damage(target,dmg);
	last_action.damage=dmg;
}

static void bolt2(struct battle_char *origin, struct battle_char *target){
	uint16_t dmg = mod5(origin,target,18,100);
	deal_damage(target,dmg);
	last_action.damage=dmg;
}

static void bolt3(struct battle_char *origin, struct battle_char *target){
	uint16_t dmg = mod5(origin,target,24,100);
	deal_damage(target,dmg);
	last_action.damage=dmg;
}

static void bolt4(struct battle_char *origin, struct battle_char *target){
	uint16_t dmg = mod5(origin,target,32,100);
	deal_damage(target,dmg);
	last_action.damage=dmg;
}

static void ice(struct battle_char *origin, struct battle_char *target){
	uint16_t dmg = mod5(origin,target,14,100);
	deal_damage(target,dmg);
	last_action.damage=dmg;
}

static void ice2(struct battle_char *origin, struct battle_char *target){
	uint16_t dmg = mod5(origin,target,18,100);
	deal_damage(target,dmg);
	last_action.damage=dmg;
}

static void ice3(struct battle_char *origin, struct battle_char *target){
	uint16_t dmg = mod5(origin,target,24,100);
	deal_damage(target,dmg);
	last_action.damage=dmg;
}

static void ice4(struct battle_char *origin, struct battle_char *target){
	uint16_t dmg = mod5(origin,target,32,100);
	deal_damage(target,dmg);
	last_action.damage=dmg;
}

static void poison(struct battle_char *origin, struct battle_char *target){
	if(get_random(0,100)<mod6(origin,target,160))
		add_status(target,STATUS_POISON);
}

static void frog(struct battle_char *origin, struct battle_char *target){
	if(get_random(0,100)<mod6(origin,target,120))
		add_status(target,STATUS_POLYMORPH);
}

static void death(struct battle_char *origin, struct battle_char *target){
	if(get_random(0,100)<mod6(origin,target,100)){
		if(STATUS_SET(target,STATUS_UNDEAD)){
			deal_damage(target,-target->hp_max);
			last_action.damage=-target->hp_max;
		}
		else{
			deal_damage(target,target->hp_max);
			last_action.damage=target->hp_max;
		}
	}
}

static void flare(struct battle_char *origin, struct battle_char *target){
	uint16_t dmg = mod5(origin,target,46,100);
	deal_damage(target,dmg);
	last_action.damage=dmg;
}

static void haste(struct battle_char *origin, struct battle_char *target){
	if(get_random(0,100)<mod6(origin,target,180)){
		add_status(target,STATUS_HASTE);
	}
}

static void haste2(struct battle_char *origin, struct battle_char *target){
	if(get_random(0,100)<mod6(origin,target,240)){
		add_status(target,STATUS_HASTE);
	}
}

static void slow(struct battle_char *origin, struct battle_char *target){
	if(get_random(0,100)<mod6(origin,target,180)){
		add_status(target,STATUS_SLOW);
	}
}

static void slow2(struct battle_char *origin, struct battle_char *target){
	if(get_random(0,100)<mod6(origin,target,240)){
		add_status(target,STATUS_SLOW);
	}
}

static void stop(struct battle_char *origin, struct battle_char *target){
	if(get_random(0,100)<mod6(origin,target,110)){
		add_status(target,STATUS_STOP);
	}
}

static void dont_move(struct battle_char *origin, struct battle_char *target){
	if(get_random(0,100)<mod6(origin,target,190)){
		add_status(target,STATUS_NOMOVE);
	}
}

static void magic_float(struct battle_char *origin, struct battle_char *target){
	if(get_random(0,100)<mod6(origin,target,140)){
		add_status(target,STATUS_FLOAT);
	}
}

static void reflect(struct battle_char *origin, struct battle_char *target){
	if(get_random(0,100)<mod6(origin,target,180)){
		add_status(target,STATUS_REFLECT);
	}
}

static void quick(struct battle_char *origin, struct battle_char *target){
	if(get_random(0,100)<mod6(origin,target,140)){
		add_status(target,STATUS_QUICK);
	}
}

static void demi(struct battle_char *origin, struct battle_char *target){
	if(get_random(0,100)<mod6(origin,target,190)){
		last_action.damage=target->hp_max/4;
		deal_damage(target,target->hp_max/4);
	}
}

static void demi2(struct battle_char *origin, struct battle_char *target){
	if(get_random(0,100)<mod6(origin,target,120)){
		last_action.damage=target->hp_max/2;
		deal_damage(target,target->hp_max/2);
	}
}

static void meteor(struct battle_char *origin, struct battle_char *target){
	uint16_t dmg = mod5(origin,target,60,100);
	deal_damage(target,dmg);
	last_action.damage=dmg;
}

static void blind_yin(struct battle_char *origin, struct battle_char *target){
	if(get_random(0,100)<mod6(origin,target,200)){
		add_status(target,STATUS_DARKNESS);
	}
}

static void spell_absorb(struct battle_char *origin, struct battle_char *target){
	uint16_t drained;
	if(get_random(0,100)<mod6(origin,target,160)){
		drained=target->mp_max/3;
		if(target->mp<drained)
			drained=target->mp;
		target->mp-=drained;
		origin->mp+=drained;
		if(origin->mp>origin->mp_max)
			origin->mp=origin->mp_max;
	}
}

static void life_drain(struct battle_char *origin, struct battle_char *target){
	int16_t drained;
	if(get_random(0,100)<mod6(origin,target,160)){
		drained=target->hp_max/4;
		if(drained>target->hp)
			drained=target->hp;

		if(STATUS_SET(target,STATUS_UNDEAD)){
			deal_damage(target,-drained);
			deal_damage(origin,drained);
			last_action.damage=-drained;
		}
		else{
			deal_damage(target,drained);
			deal_damage(origin,-drained);
			last_action.damage=drained;
		}
	}
}

static void pray_faith(struct battle_char *origin, struct battle_char *target){
	if(get_random(0,100)<mod6(origin,target,150)){
		add_status(target,STATUS_FAITH);
	}
}

static void doubt_faith(struct battle_char *origin, struct battle_char *target){
	if(get_random(0,100)<mod6(origin,target,150)){
		add_status(target,STATUS_INNOCENT);
	}
}

static void zombie_yin(struct battle_char *origin, struct battle_char *target){
	if(get_random(0,100)<mod6(origin,target,100)){
		add_status(target,STATUS_UNDEAD);
	}
}

static void silence_song(struct battle_char *origin, struct battle_char *target){
	if(get_random(0,100)<mod6(origin,target,180)){
		add_status(target,STATUS_SILENCE);
	}
}

static void blind_rage(struct battle_char *origin, struct battle_char *target){
	if(get_random(0,100)<mod6(origin,target,120)){
		add_status(target,STATUS_BERSERK);
	}
}

static void foxbird(struct battle_char *origin, struct battle_char *target){
	if(get_random(0,100)<mod6(origin,target,140)){
		if(target->brave>30)
			target->brave-=30;
		else
			target->brave=0; // TODO: <5 br penalties?
	}
}

static void confusion_song(struct battle_char *origin, struct battle_char *target){
	if(get_random(0,100)<mod6(origin,target,130)){
		add_status(target,STATUS_CONFUSION);
	}
}

static void dispel_magic(struct battle_char *origin, struct battle_char *target){
	if(get_random(0,100)<mod6(origin,target,200)){
		remove_status(target,STATUS_PROTECT);
		remove_status(target,STATUS_SHELL);
		remove_status(target,STATUS_HASTE);
		remove_status(target,STATUS_FLOAT);
		remove_status(target,STATUS_REGEN);
		remove_status(target,STATUS_RERAISE);
		remove_status(target,STATUS_TRANSPARENT);
		remove_status(target,STATUS_FAITH);
		remove_status(target,STATUS_REFLECT);
	}
}

static void paralyze(struct battle_char *origin, struct battle_char *target){
	if(get_random(0,100)<mod6(origin,target,185)){
		add_status(target,STATUS_NOACT);
	}
}

static void magic_sleep_yin(struct battle_char *origin, struct battle_char *target){
	if(get_random(0,100)<mod6(origin,target,170)){
		add_status(target,STATUS_SLEEPING);
	}
}

static void petrify(struct battle_char *origin, struct battle_char *target){
	if(get_random(0,100)<mod6(origin,target,120)){
		add_status(target,STATUS_PETRIFY);
	}
}

static void moogle(struct battle_char *origin, struct battle_char *target){
	int16_t dmg = mod5(origin,target,12,100);
	if(!STATUS_SET(target,STATUS_UNDEAD))
		dmg=-dmg;
	deal_damage(target,dmg);
	last_action.damage=dmg;
}

static void shiva(struct battle_char *origin, struct battle_char *target){
	int16_t dmg = mod5(origin,target,24,100);
	deal_damage(target,dmg);
	last_action.damage=dmg;
}

static void ramuh(struct battle_char *origin, struct battle_char *target){
	int16_t dmg = mod5(origin,target,24,100);
	deal_damage(target,dmg);
	last_action.damage=dmg;
}

static void ifrit(struct battle_char *origin, struct battle_char *target){
	int16_t dmg = mod5(origin,target,24,100);
	deal_damage(target,dmg);
	last_action.damage=dmg;
}

static void titan(struct battle_char *origin, struct battle_char *target){
	int16_t dmg = mod5(origin,target,28,100);
	deal_damage(target,dmg);
	last_action.damage=dmg;
}

/*
static void golem(struct battle_char *origin, struct battle_char *target){
}
*/

static void carbunkle(struct battle_char *origin, struct battle_char *target){
	if(get_random(0,100)<mod6(origin,target,150))
		add_status(target,STATUS_REFLECT);
}

static void bahamut(struct battle_char *origin, struct battle_char *target){
	int16_t dmg = mod5(origin,target,46,100);
	deal_damage(target,dmg);
	last_action.damage=dmg;
}

static void odin(struct battle_char *origin, struct battle_char *target){
	int16_t dmg = mod5(origin,target,40,100);
	deal_damage(target,dmg);
	last_action.damage=dmg;
}

static void leviathan(struct battle_char *origin, struct battle_char *target){
	int16_t dmg = mod5(origin,target,38,100);
	deal_damage(target,dmg);
	last_action.damage=dmg;
}

static void salamander(struct battle_char *origin, struct battle_char *target){
	int16_t dmg = mod5(origin,target,38,100);
	deal_damage(target,dmg);
	last_action.damage=dmg;
}

static void silf(struct battle_char *origin, struct battle_char *target){
	if(get_random(0,100)<mod6(origin,target,150))
		add_status(target,STATUS_SILENCE);
}

static void fairy(struct battle_char *origin, struct battle_char *target){
	int16_t var=mod5(origin,target,24,100);
	if(!STATUS_SET(target,STATUS_UNDEAD))
		var=-var;

	deal_damage(target,var);
	last_action.damage=var;
}

static void lich(struct battle_char *origin, struct battle_char *target){
	if(get_random(0,100)<mod6(origin,target,160)){
		deal_damage(target,target->hp_max/2);
		last_action.damage=target->hp_max/2;
	}
}

static void cyclops(struct battle_char *origin, struct battle_char *target){
	int16_t dmg = mod5(origin,target,50,100);
	deal_damage(target,dmg);
	last_action.damage=dmg;
}

static void zodiac(struct battle_char *origin, struct battle_char *target){
	int16_t dmg = mod5(origin,target,96,100);
	deal_damage(target,dmg);
	last_action.damage=dmg;
}

static void asura_draw(struct battle_char *origin, struct battle_char *target){
}

static void koutetsu(struct battle_char *origin, struct battle_char *target){
}

static void bizen_boat(struct battle_char *origin, struct battle_char *target){
}

static void murasame(struct battle_char *origin, struct battle_char *target){
}

static void heavens_cloud(struct battle_char *origin, struct battle_char *target){
}

static void kiyomori(struct battle_char *origin, struct battle_char *target){
}

static void muramasa(struct battle_char *origin, struct battle_char *target){
}

static void kikuichimoji(struct battle_char *origin, struct battle_char *target){
}

static void masamune(struct battle_char *origin, struct battle_char *target){
}

static void chirijiraden(struct battle_char *origin, struct battle_char *target){
}

static void angel_song(struct battle_char *origin, struct battle_char *target){
	int16_t var=origin->ma+20;
	if(origin->fof==target->fof && !STATUS_SET(target,STATUS_SLEEP)){
		if(target->mp_max-target->mp<var)
			target->mp=target->mp_max;
		else
			target->mp+=var;
	}
}

static void life_song(struct battle_char *origin, struct battle_char *target){
	if(origin->fof==target->fof && !STATUS_SET(target,STATUS_SLEEP))
		deal_damage(target,-(origin->ma+10));
}

static void cheer_song(struct battle_char *origin, struct battle_char *target){
	if(origin->fof==target->fof && !STATUS_SET(target,STATUS_SLEEP) && get_random(0,2)==0)
		target->speed+=1;
}

static void battle_song(struct battle_char *origin, struct battle_char *target){
	if(origin->fof==target->fof && !STATUS_SET(target,STATUS_SLEEP) && get_random(0,2)==0)
		target->pa+=1;
}

static void magic_song(struct battle_char *origin, struct battle_char *target){
	if(origin->fof==target->fof && !STATUS_SET(target,STATUS_SLEEP) && get_random(0,2)==0)
		target->ma+=1;
}

static void nameless_song(struct battle_char *origin, struct battle_char *target){
	int r=get_random(0,5);
	if(origin->fof==target->fof && !STATUS_SET(target,STATUS_SLEEP) && get_random(0,2)==0){
		switch(r){
			case 0:
				add_status(target,STATUS_RERAISE);
				break;
			case 1:
				add_status(target,STATUS_REGEN);
				break;
			case 2:
				add_status(target,STATUS_PROTECT);
				break;
			case 3:
				add_status(target,STATUS_SHELL);
				break;
			case 4:
				add_status(target,STATUS_REFLECT);
				break;
		}
	}
}

static void last_song(struct battle_char *origin, struct battle_char *target){
	if(origin->fof==target->fof && !STATUS_SET(target,STATUS_SLEEP) && get_random(0,2)==0)
		add_status(target,STATUS_QUICK);
}

static void witch_hunt(struct battle_char *origin, struct battle_char *target){
	int16_t var=origin->pa+((origin->pa*origin->brave)/100);
	if(origin->fof!=target->fof && !STATUS_SET(target,STATUS_SLEEP)){
		if(target->mp<var)
			target->mp=0;
		else
			target->mp-=var;
	}

}

static void wiznaibus(struct battle_char *origin, struct battle_char *target){
	int16_t var=origin->pa+((origin->pa*origin->brave)/100);
	if(origin->fof!=target->fof && !STATUS_SET(target,STATUS_SLEEP)){
		deal_damage(target,var);
		last_action.damage=var;
	}
}

static void slow_dance(struct battle_char *origin, struct battle_char *target){
	if(origin->fof!=target->fof && !STATUS_SET(target,STATUS_SLEEP) && get_random(0,2)==0)
		if(target->speed>0)target->speed-=1;
}

static void polka_polka(struct battle_char *origin, struct battle_char *target){
	if(origin->fof!=target->fof && !STATUS_SET(target,STATUS_SLEEP) && get_random(0,2)==0)
		if(target->pa>0)target->pa-=1;
}

static void disillusion(struct battle_char *origin, struct battle_char *target){
	if(origin->fof!=target->fof && !STATUS_SET(target,STATUS_SLEEP) && get_random(0,2)==0)
		if(target->ma>0)target->ma-=1;
}

static void nameless_dance(struct battle_char *origin, struct battle_char *target){
	int r=get_random(0,8);
	if(origin->fof==target->fof && !STATUS_SET(target,STATUS_SLEEP) && get_random(0,2)==0){
		switch(r){
			case 0:
				add_status(target,STATUS_DARKNESS);
				break;
			case 1:
				add_status(target,STATUS_CONFUSION);
				break;
			case 2:
				add_status(target,STATUS_SILENCE);
				break;
			case 3:
				add_status(target,STATUS_POLYMORPH);
				break;
			case 4:
				add_status(target,STATUS_POISON);
				break;
			case 5:
				add_status(target,STATUS_SLOW);
				break;
			case 6:
				add_status(target,STATUS_STOP);
				break;
			case 7:
				add_status(target,STATUS_SLEEP);
				break;
		}
	}
}

static void last_dance(struct battle_char *origin, struct battle_char *target){
	if(origin->fof==target->fof && !STATUS_SET(target,STATUS_SLEEP) && get_random(0,3)==0)
		target->ct=0;
}

static void spin_fist(struct battle_char *origin, struct battle_char *target){
	int16_t dmg = mod2(origin,target,origin->pa)*(origin->pa/2);
	if(origin==target)
		return;
	deal_damage(target,dmg);
	last_action.damage=dmg;
}

static void repeating_fist(struct battle_char *origin, struct battle_char *target){
	int16_t dmg = get_random(1,10)*(mod2(origin,target,origin->pa)*(origin->pa/2));
	if(origin==target)
		return;
	deal_damage(target,dmg);
	last_action.damage=dmg;
}

static void wave_fist(struct battle_char *origin, struct battle_char *target){
	int16_t dmg = mod2(origin,target,origin->pa)*(origin->pa+(origin->pa/2));
	if(origin==target)
		return;
	deal_damage(target,dmg);
	last_action.damage=dmg;
}

static void earth_slash(struct battle_char *origin, struct battle_char *target){
	int16_t dmg = mod2(origin,target,origin->pa)*(origin->pa/2);
	if(origin==target)
		return;
	deal_damage(target,dmg);
	last_action.damage=dmg;
}

static void secret_fist(struct battle_char *origin, struct battle_char *target){
	if(origin==target || get_random(0,100)>=mod3(origin,target,origin->ma)+50)
		return;
	add_status(target,STATUS_DEATHSENTENCE);
}

static void stigma_magic(struct battle_char *origin, struct battle_char *target){
	if(get_random(0,100)<mod3(origin,target,origin->ma)+50){
		remove_status(target,STATUS_PETRIFY);
		remove_status(target,STATUS_DARKNESS);
		remove_status(target,STATUS_CONFUSION);
		remove_status(target,STATUS_POLYMORPH);
		remove_status(target,STATUS_SILENCE);
		remove_status(target,STATUS_BERSERK);
		remove_status(target,STATUS_POISON);
		remove_status(target,STATUS_SLEEP);
		remove_status(target,STATUS_NOACT);
		remove_status(target,STATUS_NOMOVE);
	}
}

static void chakra(struct battle_char *origin, struct battle_char *target){
	int var = mod2(origin,target,origin->pa);

	restore_hp(target,var*5);
	restore_mp(target,var*5/2);
}

static void revive(struct battle_char *origin, struct battle_char *target){
	if(get_random(0,100)<mod3(origin,target,origin->pa)+70){
		remove_status(target,STATUS_DEAD);
		restore_hp(target,target->hp_max/5);
	}
}

static void gil_taking(struct battle_char *origin, struct battle_char *target){
}

static void steal_heart(struct battle_char *origin, struct battle_char *target){
}

static void steal_helmet(struct battle_char *origin, struct battle_char *target){
}

static void steal_armor(struct battle_char *origin, struct battle_char *target){
}

static void steal_shield(struct battle_char *origin, struct battle_char *target){
}

static void steal_weapon(struct battle_char *origin, struct battle_char *target){
}

static void steal_accessry(struct battle_char *origin, struct battle_char *target){
}

static void steal_exp(struct battle_char *origin, struct battle_char *target){
}

static void invitation(struct battle_char *origin, struct battle_char *target){
}

static void persuade(struct battle_char *origin, struct battle_char *target){
}

static void praise(struct battle_char *origin, struct battle_char *target){
}

static void threaten(struct battle_char *origin, struct battle_char *target){
}

static void preach(struct battle_char *origin, struct battle_char *target){
}

static void solution(struct battle_char *origin, struct battle_char *target){
}

static void death_sentence(struct battle_char *origin, struct battle_char *target){
}

static void negotiate(struct battle_char *origin, struct battle_char *target){
}

static void insult(struct battle_char *origin, struct battle_char *target){
}

static void mimic_daravon(struct battle_char *origin, struct battle_char *target){
}

static void pitfall(struct battle_char *origin, struct battle_char *target){
}

static void water_ball(struct battle_char *origin, struct battle_char *target){
}

static void hell_ivy(struct battle_char *origin, struct battle_char *target){
}

static void carve_model(struct battle_char *origin, struct battle_char *target){
}

static void local_quake(struct battle_char *origin, struct battle_char *target){
}

static void kamaitachi(struct battle_char *origin, struct battle_char *target){
}

static void demon_fire(struct battle_char *origin, struct battle_char *target){
}

static void quicksand(struct battle_char *origin, struct battle_char *target){
}

static void blizzard(struct battle_char *origin, struct battle_char *target){
}

static void gusty_wind(struct battle_char *origin, struct battle_char *target){
}

static void lava_ball(struct battle_char *origin, struct battle_char *target){
}

static void head_break(struct battle_char *origin, struct battle_char *target){
}

static void armor_break(struct battle_char *origin, struct battle_char *target){
}

static void shield_break(struct battle_char *origin, struct battle_char *target){
}

static void weapon_break(struct battle_char *origin, struct battle_char *target){
}

static void magic_break(struct battle_char *origin, struct battle_char *target){
}

static void speed_break(struct battle_char *origin, struct battle_char *target){
}

static void power_break(struct battle_char *origin, struct battle_char *target){
}

static void mind_break(struct battle_char *origin, struct battle_char *target){
}

static void accumulate(struct battle_char *origin, struct battle_char *target){
}

static void dash(struct battle_char *origin, struct battle_char *target){
}

static void throw_stone(struct battle_char *origin, struct battle_char *target){
}

static void heal(struct battle_char *origin, struct battle_char *target){
}

static void yell(struct battle_char *origin, struct battle_char *target){
}

static void cheer_up(struct battle_char *origin, struct battle_char *target){
}

static void wish(struct battle_char *origin, struct battle_char *target){
}

static void scream(struct battle_char *origin, struct battle_char *target){
}

static void ultima_guts(struct battle_char *origin, struct battle_char *target){
}

static void stasis_sword(struct battle_char *origin, struct battle_char *target){
}

static void split_punch(struct battle_char *origin, struct battle_char *target){
}

static void crush_punch(struct battle_char *origin, struct battle_char *target){
}

static void lightning_stab(struct battle_char *origin, struct battle_char *target){
}

static void holy_explosion(struct battle_char *origin, struct battle_char *target){
}

static void shellbust_stab(struct battle_char *origin, struct battle_char *target){
}

static void blastar_punch(struct battle_char *origin, struct battle_char *target){
}

static void hellcry_punch(struct battle_char *origin, struct battle_char *target){
}

static void icewolf_bite(struct battle_char *origin, struct battle_char *target){
}

static void dark_sword(struct battle_char *origin, struct battle_char *target){
}

static void night_sword(struct battle_char *origin, struct battle_char *target){
}

static void dark_holy(struct battle_char *origin, struct battle_char *target){
}

static void deathspell2(struct battle_char *origin, struct battle_char *target){
}

static void galaxy_stop(struct battle_char *origin, struct battle_char *target){
}

static void heaven_thunder(struct battle_char *origin, struct battle_char *target){
}

static void asura_truth(struct battle_char *origin, struct battle_char *target){
}

static void diamond_sword(struct battle_char *origin, struct battle_char *target){
}

static void hydragon_pit(struct battle_char *origin, struct battle_char *target){
}

static void space_storage(struct battle_char *origin, struct battle_char *target){
}

static void sky_demon(struct battle_char *origin, struct battle_char *target){
}

static void heaven_bltback(struct battle_char *origin, struct battle_char *target){
}

static void asura_back(struct battle_char *origin, struct battle_char *target){
}

static void dia_swd_back(struct battle_char *origin, struct battle_char *target){
}

static void dragon_pit_back(struct battle_char *origin, struct battle_char *target){
}

static void space_str_back(struct battle_char *origin, struct battle_char *target){
}

static void sky_demon_back(struct battle_char *origin, struct battle_char *target){
}

static void seal(struct battle_char *origin, struct battle_char *target){
}

static void shadow_stitch(struct battle_char *origin, struct battle_char *target){
}

static void stop_bracelet(struct battle_char *origin, struct battle_char *target){
}

static void shock(struct battle_char *origin, struct battle_char *target){
}

static void difference(struct battle_char *origin, struct battle_char *target){
}

static void chicken_race(struct battle_char *origin, struct battle_char *target){
}

static void hold_tight(struct battle_char *origin, struct battle_char *target){
}

static void darkness(struct battle_char *origin, struct battle_char *target){
}

static void lose_voice(struct battle_char *origin, struct battle_char *target){
}

static void loss(struct battle_char *origin, struct battle_char *target){
}

static void spell(struct battle_char *origin, struct battle_char *target){
}

static void nightmare(struct battle_char *origin, struct battle_char *target){
}

static void death_cold(struct battle_char *origin, struct battle_char *target){
}

static void magic_ruin(struct battle_char *origin, struct battle_char *target){
}

static void speed_ruin(struct battle_char *origin, struct battle_char *target){
}

static void power_ruin(struct battle_char *origin, struct battle_char *target){
}

static void mind_ruin(struct battle_char *origin, struct battle_char *target){
}

static void blood_suck(struct battle_char *origin, struct battle_char *target){
}

static void allure(struct battle_char *origin, struct battle_char *target){
}

static void bio(struct battle_char *origin, struct battle_char *target){
	// May add either dark, poison, oil
}

static void bio2(struct battle_char *origin, struct battle_char *target){
	// Adds either frog, slow, silence, petrify
}

static void bio3(struct battle_char *origin, struct battle_char *target){
	// May add either dead, undead, petrify
}

static void mbarrier(struct battle_char *origin, struct battle_char *target){
}

static void leg_aim(struct battle_char *origin, struct battle_char *target){
}

static void arm_aim(struct battle_char *origin, struct battle_char *target){
}

static void seal_evil(struct battle_char *origin, struct battle_char *target){
}

static void melt(struct battle_char *origin, struct battle_char *target){
}

static void tornado(struct battle_char *origin, struct battle_char *target){
}

static void quake(struct battle_char *origin, struct battle_char *target){
}

static void toad2(struct battle_char *origin, struct battle_char *target){
}

static void gravi2(struct battle_char *origin, struct battle_char *target){
}

static void flare2(struct battle_char *origin, struct battle_char *target){
}

static void blind2(struct battle_char *origin, struct battle_char *target){
}

static void small_bomb_grenade(struct battle_char *origin, struct battle_char *target){
}

static void small_bomb_explosive(struct battle_char *origin, struct battle_char *target){
}

static void confuse2(struct battle_char *origin, struct battle_char *target){
}

static void sleep2(struct battle_char *origin, struct battle_char *target){
}

static void ultima_ultimate(struct battle_char *origin, struct battle_char *target){
}

static void mute(struct battle_char *origin, struct battle_char *target){
}

static void despair2(struct battle_char *origin, struct battle_char *target){
}

static void return2(struct battle_char *origin, struct battle_char *target){
}

static void blind_sword(struct battle_char *origin, struct battle_char *target){
}

static void aspel(struct battle_char *origin, struct battle_char *target){
}

static void drain(struct battle_char *origin, struct battle_char *target){
}

static void faith(struct battle_char *origin, struct battle_char *target){
}

static void innocent(struct battle_char *origin, struct battle_char *target){
}

static void zombie_sword(struct battle_char *origin, struct battle_char *target){
}

static void silence(struct battle_char *origin, struct battle_char *target){
}

static void berserk(struct battle_char *origin, struct battle_char *target){
}

static void chicken(struct battle_char *origin, struct battle_char *target){
}

static void confuse(struct battle_char *origin, struct battle_char *target){
}

static void despair(struct battle_char *origin, struct battle_char *target){
}

static void magic_sleep_sword(struct battle_char *origin, struct battle_char *target){
}

// The magic sword ability: break. It does not break swords.
static void break_sword(struct battle_char *origin, struct battle_char *target){
}

static void ice_bracelet(struct battle_char *origin, struct battle_char *target){
}

static void fire_bracelet(struct battle_char *origin, struct battle_char *target){
}

static void thnder_brcelet(struct battle_char *origin, struct battle_char *target){
}

static void dragon_tame(struct battle_char *origin, struct battle_char *target){
}

static void dragon_care(struct battle_char *origin, struct battle_char *target){
}

static void dragon_powerup(struct battle_char *origin, struct battle_char *target){
}

static void dragon_levelup(struct battle_char *origin, struct battle_char *target){
}

static void holy_bracelet(struct battle_char *origin, struct battle_char *target){
}

static void braver(struct battle_char *origin, struct battle_char *target){
}

static void blade_beam(struct battle_char *origin, struct battle_char *target){
}

static void climhazzard(struct battle_char *origin, struct battle_char *target){
}

static void meteorain(struct battle_char *origin, struct battle_char *target){
}

static void finish_touch(struct battle_char *origin, struct battle_char *target){
}

static void omnislash(struct battle_char *origin, struct battle_char *target){
}

static void cherry_blossom(struct battle_char *origin, struct battle_char *target){
}

static void choco_attack(struct battle_char *origin, struct battle_char *target){
}

static void choco_ball(struct battle_char *origin, struct battle_char *target){
}

static void choco_meteor(struct battle_char *origin, struct battle_char *target){
}

static void choco_esuna(struct battle_char *origin, struct battle_char *target){
}

static void choco_cure(struct battle_char *origin, struct battle_char *target){
}

static void tackle(struct battle_char *origin, struct battle_char *target){
}

static void goblin_punch(struct battle_char *origin, struct battle_char *target){
}

static void turn_punch(struct battle_char *origin, struct battle_char *target){
}

static void eye_gouge(struct battle_char *origin, struct battle_char *target){
}

static void mutilate(struct battle_char *origin, struct battle_char *target){
}

static void small_bomb_bomb(struct battle_char *origin, struct battle_char *target){
}

static void flame_attack(struct battle_char *origin, struct battle_char *target){
}

static void spark(struct battle_char *origin, struct battle_char *target){
}

static void scratch(struct battle_char *origin, struct battle_char *target){
}

static void cat_kick(struct battle_char *origin, struct battle_char *target){
}

static void blaster(struct battle_char *origin, struct battle_char *target){
}

static void poison_nail(struct battle_char *origin, struct battle_char *target){
}

static void blood_suck_coeurl(struct battle_char *origin, struct battle_char *target){
}

static void black_ink(struct battle_char *origin, struct battle_char *target){
}

static void odd_soundwave(struct battle_char *origin, struct battle_char *target){
}

static void mind_blast(struct battle_char *origin, struct battle_char *target){
}

static void level_blast(struct battle_char *origin, struct battle_char *target){
}

static void knife_hand(struct battle_char *origin, struct battle_char *target){
}

static void thunder_soul(struct battle_char *origin, struct battle_char *target){
}

static void aqua_soul(struct battle_char *origin, struct battle_char *target){
}

static void ice_soul(struct battle_char *origin, struct battle_char *target){
}

static void wind_soul(struct battle_char *origin, struct battle_char *target){
}

static void throw_spirit(struct battle_char *origin, struct battle_char *target){
}

static void zombie_touch(struct battle_char *origin, struct battle_char *target){
}

static void sleep_touch(struct battle_char *origin, struct battle_char *target){
}

static void drain_touch(struct battle_char *origin, struct battle_char *target){
}

static void grease_touch(struct battle_char *origin, struct battle_char *target){
}

static void wing_attack(struct battle_char *origin, struct battle_char *target){
}

static void look_of_devil(struct battle_char *origin, struct battle_char *target){
}

static void look_of_fright(struct battle_char *origin, struct battle_char *target){
}

static void circle(struct battle_char *origin, struct battle_char *target){
}

static void death_sentence_ahriman(struct battle_char *origin, struct battle_char *target){
}

static void scratch_up(struct battle_char *origin, struct battle_char *target){
}

static void beak(struct battle_char *origin, struct battle_char *target){
}

static void shine_lover(struct battle_char *origin, struct battle_char *target){
}

static void feather_bomb(struct battle_char *origin, struct battle_char *target){
}

static void beaking(struct battle_char *origin, struct battle_char *target){
}

static void straight_dash(struct battle_char *origin, struct battle_char *target){
}

static void nose_bracelet(struct battle_char *origin, struct battle_char *target){
}

static void oink(struct battle_char *origin, struct battle_char *target){
}

static void please_eat(struct battle_char *origin, struct battle_char *target){
}

static void leaf_dance(struct battle_char *origin, struct battle_char *target){
}

static void protect_spirit(struct battle_char *origin, struct battle_char *target){
}

static void clam_spirit(struct battle_char *origin, struct battle_char *target){
}

static void spirit_of_life(struct battle_char *origin, struct battle_char *target){
}

static void magic_spirit(struct battle_char *origin, struct battle_char *target){
}

static void shake_off(struct battle_char *origin, struct battle_char *target){
}

static void wave_around(struct battle_char *origin, struct battle_char *target){
}

static void mimic_titan(struct battle_char *origin, struct battle_char *target){
}

static void gather_power(struct battle_char *origin, struct battle_char *target){
}

static void blow_fire(struct battle_char *origin, struct battle_char *target){
}

static void tentacle(struct battle_char *origin, struct battle_char *target){
}

static void lick(struct battle_char *origin, struct battle_char *target){
}

static void goo(struct battle_char *origin, struct battle_char *target){
}

static void bad_bracelet(struct battle_char *origin, struct battle_char *target){
}

static void moldball_virus(struct battle_char *origin, struct battle_char *target){
}

static void stab_up(struct battle_char *origin, struct battle_char *target){
}

static void sudden_cry(struct battle_char *origin, struct battle_char *target){
}

static void hurricane(struct battle_char *origin, struct battle_char *target){
}

static void ulmaguest(struct battle_char *origin, struct battle_char *target){
}

static void giga_flare(struct battle_char *origin, struct battle_char *target){
}

static void dash_dragon(struct battle_char *origin, struct battle_char *target){
}

static void tail_swing(struct battle_char *origin, struct battle_char *target){
}

static void ice_bracelet_dragon(struct battle_char *origin, struct battle_char *target){
}

static void fire_bracelet_dragon(struct battle_char *origin, struct battle_char *target){
}

static void thnder_brcelet_dragon(struct battle_char *origin, struct battle_char *target){
}

static void triple_attack(struct battle_char *origin, struct battle_char *target){
}

static void triple_bracelet(struct battle_char *origin, struct battle_char *target){
}

static void triple_thunder(struct battle_char *origin, struct battle_char *target){
}

static void triple_flame(struct battle_char *origin, struct battle_char *target){
}

static void dark_whisper(struct battle_char *origin, struct battle_char *target){
}

static void snake_carrier(struct battle_char *origin, struct battle_char *target){
}

static void poison_frog(struct battle_char *origin, struct battle_char *target){
}

static void midgar_swarm(struct battle_char *origin, struct battle_char *target){
}

static void lifebreak(struct battle_char *origin, struct battle_char *target){
}

static void nanoflare(struct battle_char *origin, struct battle_char *target){
}

static void grand_cross(struct battle_char *origin, struct battle_char *target){
}

static void destroy(struct battle_char *origin, struct battle_char *target){
}

static void compress(struct battle_char *origin, struct battle_char *target){
}

static void dispose(struct battle_char *origin, struct battle_char *target){
}

static void crush(struct battle_char *origin, struct battle_char *target){
}

static void energy(struct battle_char *origin, struct battle_char *target){
}

static void parasite(struct battle_char *origin, struct battle_char *target){
}

static int8_t elem_weapon_dmg(int16_t dmg, int elem, struct battle_char *d){
	if(d->resist[elem]&RESIST_ABSORB)
		return -dmg;
	else {
		if(d->resist[elem]&RESIST_WEAK)
			return dmg*2;
		if(d->resist[elem]&RESIST_HALF)
			return dmg/2;
	}
	return dmg;
}

static void attack(struct battle_char *s, struct battle_char *d){
	uint16_t pri=s->ch->eq[EQ_WEAPON];
	uint16_t sec=s->ch->eq[EQ_OFFHAND];
	int16_t pdmg=0;
	int16_t sdmg=0;
	int biti;

	pdmg=weapon_damage[EQ_TYPE(pri)](&weapons[EQ_TYPE(pri)][pri>>6],s,d);

	if(sec==0 && s->ch->support==SFLAG_TWO_HANDS)
		pdmg*=2;
	else if(sec>0 && EQ_TYPE(sec)!=EQO_SHIELD){ // Implied SFLAG_TWO_SWORDS. check it?
		//sdmg+=weapon_damage[EQ_TYPE(sec)](&weapons[EQ_TYPE(sec)][sec>>6],s,d);
		sdmg=elem_weapon_dmg(sdmg,weapons[EQ_TYPE(sec)][sec>>6].elem,d);
	}

	pdmg=elem_weapon_dmg(pdmg,weapons[EQ_TYPE(pri)][pri>>6].elem,d);
	
	deal_damage(d,pdmg+sdmg);

	if(s->add_status>0 && get_random(0,4)==0){
		for(biti=0;biti<NUM_ADD_STATUS;biti++)
			if(s->add_status&BIT(biti))
				add_status(d,biti);

	}

	last_action.damage=pdmg+sdmg;
}


const uint8_t num_action[]={1,0,7,0,0,7,11,8,0,10,0,8,0,14,15,10,4,15,8,12,16};
// Sorry this is so ugly... it's perl's fault :P
const struct ability claction[NUM_CLASS][NUM_ACTION_PER_ABILITY]={
{{"Attack",.f.af=attack,0,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_EVADE|0,0,0,2,AFLAG_MOD_XA,0,{RANGE_WEAPON,0,1,0,0}}}, // Generic
{},//CHARGE (Archer)
{{"Angel Song",.f.af=angel_song,100,0,0,6,0,0,0,{0,0,0,0,0}},{"Life Song",.f.af=life_song,100,0,0,6,0,0,0,{0,0,0,0,0}},{"Cheer Song",.f.af=cheer_song,100,0,0,8,0,0,0,{0,0,0,0,0}},{"Battle Song",.f.af=battle_song,100,0,0,8,0,0,0,{0,0,0,0,0}},{"Magic Song",.f.af=magic_song,100,0,0,10,0,0,0,{0,0,0,0,0}},{"Nameless Song",.f.af=nameless_song,100,0,0,10,0,0,0,{0,0,0,0,0}},{"Last Song",.f.af=last_song,100,0,0,20,0,0,0,{0,0,0,0,0}}}, //SING (Bard)
{},//MATH SKILL (Calculator)
{},//ITEM (Chemist)
{{"Witch Hunt",.f.af=witch_hunt,100,0,0,6,0,0,0,{0,0,0,0,0}},{"Wiznaibus",.f.af=wiznaibus,100,0,0,6,0,0,0,{0,0,0,0,0}},{"Slow Dance",.f.af=slow_dance,100,0,0,8,0,0,0,{0,0,0,0,0}},{"Polka Polka",.f.af=polka_polka,100,0,0,8,0,0,0,{0,0,0,0,0}},{"Dillusion",.f.af=disillusion,100,0,0,8,0,0,0,{0,0,0,0,0}},{"Nameless Dance",.f.af=nameless_dance,100,0,0,10,0,0,0,{0,0,0,0,0}},{"Last Dance",.f.af=last_dance,100,0,0,20,0,0,0,{0,0,0,0,0}}}, //DANCE  (Dancer)
{{"Pitfall",.f.af=pitfall,150,AFLAG_MAGIC|AFLAG_COUNTER_FLOOD|0,0,0,5,AFLAG_MOD_MA,0,{5,0,2,0,0}},{"Water Ball",.f.af=water_ball,150,AFLAG_MAGIC|AFLAG_COUNTER_FLOOD|0,ELEM_WATER,0,5,AFLAG_MOD_MA,0,{5,0,2,0,0}},{"Hell Ivy",.f.af=hell_ivy,150,AFLAG_MAGIC|AFLAG_COUNTER_FLOOD|0,0,0,5,AFLAG_MOD_MA,0,{5,0,2,0,0}},{"Carve Model",.f.af=carve_model,150,AFLAG_MAGIC|AFLAG_COUNTER_FLOOD|0,0,0,5,AFLAG_MOD_MA,0,{5,0,2,0,0}},{"Local Quake",.f.af=local_quake,150,AFLAG_MAGIC|AFLAG_COUNTER_FLOOD|0,ELEM_EARTH,0,5,AFLAG_MOD_MA,0,{5,0,2,0,0}},{"Kamaitachi",.f.af=kamaitachi,150,AFLAG_MAGIC|AFLAG_COUNTER_FLOOD|0,ELEM_WIND,0,5,AFLAG_MOD_MA,0,{5,0,2,0,0}},{"Demon Fire",.f.af=demon_fire,150,AFLAG_MAGIC|AFLAG_COUNTER_FLOOD|0,ELEM_FIRE,0,5,AFLAG_MOD_MA,0,{5,0,2,0,0}},{"Quicksand",.f.af=quicksand,150,AFLAG_MAGIC|AFLAG_COUNTER_FLOOD|0,ELEM_WATER,0,5,AFLAG_MOD_MA,0,{5,0,2,0,0}},{"Blizzard",.f.af=blizzard,150,AFLAG_MAGIC|AFLAG_COUNTER_FLOOD|0,ELEM_ICE,0,5,AFLAG_MOD_MA,0,{5,0,2,0,0}},{"Gusty Wind",.f.af=gusty_wind,150,AFLAG_MAGIC|AFLAG_COUNTER_FLOOD|0,ELEM_WATER,0,5,AFLAG_MOD_MA,0,{5,0,2,0,0}},{"Lava Ball",.f.af=lava_ball,150,AFLAG_MAGIC|AFLAG_COUNTER_FLOOD|0,ELEM_FIRE,0,5,AFLAG_MOD_MA,0,{5,0,2,0,0}}}, //ELEMENTAL (Geomancer)
{{"Head break",.f.af=head_break,300,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_EVADE|0,ELEM_WEAPON,0,3,45,0,{RANGE_WEAPON,0,1,0,0}},{"Armor Break",.f.af=armor_break,400,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_EVADE|0,ELEM_WEAPON,0,3,40,0,{RANGE_WEAPON,0,1,0,0}},{"Shield Break",.f.af=shield_break,300,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_EVADE|0,ELEM_WEAPON,0,3,55,0,{RANGE_WEAPON,0,1,0,0}},{"Weapon Break",.f.af=weapon_break,400,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_EVADE|0,ELEM_WEAPON,0,3,30,0,{RANGE_WEAPON,0,1,0,0}},{"Magic Break",.f.af=magic_break,250,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_EVADE|0,ELEM_WEAPON,0,3,50,0,{RANGE_WEAPON,0,1,0,0}},{"Speed Break",.f.af=speed_break,250,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_EVADE|0,ELEM_WEAPON,0,3,50,0,{RANGE_WEAPON,0,1,0,0}},{"Power Break",.f.af=power_break,250,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_EVADE|0,ELEM_WEAPON,0,3,50,0,{RANGE_WEAPON,0,1,0,0}},{"Mind Break",.f.af=mind_break,250,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_EVADE|0,ELEM_WEAPON,0,3,50,0,{RANGE_WEAPON,0,1,0,0}}}, //BATTLE SKILL (Knight)
{}, //JUMP (Lancer)
{{"Invitation",.f.af=invitation,100,0,0,0,1,20,0,{3,3,1,0,0}},{"Persuade",.f.af=persuade,100,0,0,0,1,30,0,{3,3,1,0,0}},{"Praise",.f.af=praise,200,0,0,0,1,50,0,{3,3,1,0,0}},{"Threaten",.f.af=threaten,200,0,0,0,1,90,0,{3,3,1,0,0}},{"Preach",.f.af=preach,200,0,0,0,1,50,0,{3,3,1,0,0}},{"Solution",.f.af=solution,200,0,0,0,1,90,0,{3,3,1,0,0}},{"Death Sentence",.f.af=death_sentence,500,0,0,0,1,30,0,{3,3,1,0,0}},{"Negotiate",.f.af=negotiate,100,0,0,0,1,90,0,{3,3,1,0,0}},{"Insult",.f.af=insult,300,0,0,0,1,40,0,{3,3,1,0,0}},{"Mimic Daravon",.f.af=mimic_daravon,300,0,0,0,1,40,0,{3,3,2,3,0}}}, //TALK SKILL (Mediator)
{}, // MIMICRY (Mime)
{{"Spin Fist",.f.af=spin_fist,150,AFLAG_PHYSICAL|AFLAG_EVADE|0,0,0,2,AFLAG_MOD_XA,0,{0,0,2,0,0}},{"Repeating Fist",.f.af=repeating_fist,300,AFLAG_PHYSICAL|AFLAG_EVADE|0,0,0,2,AFLAG_MOD_XA,0,{1,1,1,0,0}},{"Wave Fist",.f.af=wave_fist,300,AFLAG_PHYSICAL|AFLAG_EVADE|0,0,0,2,AFLAG_MOD_XA,0,{3,3,1,0,0}},{"Earth Slash",.f.af=earth_slash,600,AFLAG_PHYSICAL|0,ELEM_EARTH,0,2,AFLAG_MOD_XA,0,{1,0,8,2,BIT(ATTACK_DIR_FRONT)}},{"Secret Fist",.f.af=secret_fist,300,AFLAG_PHYSICAL|0,0,0,3,50,0,{1,0,1,0,0}},{"Stigma Magic",.f.af=stigma_magic,200,AFLAG_PHYSICAL|0,0,0,3,120,0,{0,0,2,0,0}},{"Chakra",.f.af=chakra,350,AFLAG_PHYSICAL|0,0,0,2,AFLAG_MOD_XA,0,{0,0,2,0,0}},{.f.af=revive,500,AFLAG_PHYSICAL|0,0,0,3,70,0,{1,0,1,0,0}}}, //PUNCH ART (Monk)
{}, //THROW (Ninja)
{{"Blind Yin",.f.af=blind_yin,100,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,0,2,6,200,4,{4,0,2,1,0}},{"Spell Absorb",.f.af=spell_absorb,200,AFLAG_MAGIC|AFLAG_COUNTER_MAGIC|0,0,2,6,160,2,{4,0,1,0,0}},{"Life Drain",.f.af=life_drain,350,AFLAG_MAGIC|AFLAG_COUNTER_MAGIC|0,0,2,6,160,16,{4,0,1,0,0}},{"Pray Faith",.f.af=pray_faith,400,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_MATH|0,0,4,6,150,6,{4,0,1,0,0}},{"Doubt Faith",.f.af=doubt_faith,400,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_MATH|0,0,4,6,150,6,{4,0,1,0,0}},{"Zombie",.f.af=zombie_yin,300,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,0,5,6,100,20,{4,0,1,0,0}},{"Silence Song",.f.af=silence_song,170,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,0,3,6,180,16,{4,0,2,1,0}},{"Blind Rage",.f.af=blind_rage,400,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,0,5,6,120,16,{4,0,1,0,0}},{"Foxbird",.f.af=foxbird,200,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,0,4,6,140,20,{4,0,1,0,0}},{"Confusion Song",.f.af=confusion_song,400,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,0,5,6,130,20,{4,0,1,0,0}},{"Dispel Magic",.f.af=dispel_magic,700,AFLAG_MAGIC|AFLAG_COUNTER_MAGIC|AFLAG_MATH|0,0,3,6,200,34,{4,0,1,0,0}},{"Paralyze",.f.af=paralyze,100,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,0,5,6,185,10,{4,0,2,0,0}},{"Magic Sleep",.f.af=magic_sleep_yin,350,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,0,6,170,6,24,{4,0,2,1,0}},{"Petrify",.f.af=petrify,580,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,0,9,6,120,16,{4,0,1,0,0}}}, //YIN-YANG MAGIC (Oracle)
{{"Cure",.f.af=cure,50,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_MATH|0,0,4,5,AFLAG_MOD_MA,6,{4,0,2,1,0}},{"Cure2",.f.af=cure2,180,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_MATH|0,0,5,AFLAG_MOD_MA,5,10,{4,0,2,1,0}},{"Cure 3",.f.af=cure3,400,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_MATH|0,0,7,5,AFLAG_MOD_MA,16,{4,0,2,2,0}},{"Cure 4",.f.af=cure4,700,AFLAG_MAGIC|0,0,10,5,AFLAG_MOD_MA,20,{4,0,2,3,0}},{"Raise",.f.af=raise,180,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_MATH|0,0,4,6,180,10,{4,0,1,0,0}},{"Raise 2",.f.af=raise2,500,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_MATH|0,0,10,6,160,20,{4,0,1,0,0}},{"Reraise",.f.af=reraise,800,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_MATH|0,0,7,6,140,16,{4,0,1,0,0}},{"Regen",.f.af=regen,300,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_MATH|0,0,4,6,170,8,{3,0,2,0,0}},{"Protect",.f.af=protect,70,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_MATH|0,0,4,6,200,6,{3,0,2,0,0}},{"Protect 2",.f.af=protect2,500,AFLAG_MAGIC|0,0,7,6,120,24,{3,0,2,3,0}},{"Shell",.f.af=shell,70,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_MATH|0,0,4,6,200,6,{3,0,2,0,0}},{"Shell 2",.f.af=shell2,500,AFLAG_MAGIC|0,0,7,6,120,20,{3,0,2,3,0}},{"Wall",.f.af=wall,380,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_MATH|0,0,4,6,140,24,{3,0,1,0,0}},{"Esuna",.f.af=esuna,280,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_MATH|0,0,3,6,190,18,{3,0,2,2,0}},{"Holy",.f.af=holy,600,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|0,ELEM_HOLY,6,5,AFLAG_MOD_MA,56,{5,0,1,0,0}}}, //WHITE MAGIC (Priest)
{{"Asura",.f.af=asura_draw,100,AFLAG_MAGIC|0,0,0,5,AFLAG_MOD_MA,0,{0,0,3,3,0}},{"Koutetsu",.f.af=koutetsu,180,AFLAG_MAGIC|0,0,0,5,AFLAG_MOD_MA,0,{0,0,3,3,0}},{"Bizen Boat",.f.af=bizen_boat,260,AFLAG_MAGIC|0,0,0,5,AFLAG_MOD_MA,0,{0,0,3,3,0}},{"Murasame",.f.af=murasame,340,0,0,0,0,0,0,{0,0,3,3,0}},{"Heaven's Cloud",.f.af=heavens_cloud,420,AFLAG_MAGIC|0,0,0,5,AFLAG_MOD_MA,0,{0,0,3,3,0}},{"kiyomori",.f.af=kiyomori,500,0,0,0,0,0,0,{0,0,3,3,0}},{"Muramasa",.f.af=muramasa,580,AFLAG_MAGIC|0,0,0,5,AFLAG_MOD_MA,0,{0,0,3,3,0}},{"Kikuichimoji",.f.af=kikuichimoji,660,AFLAG_MAGIC|0,0,0,5,AFLAG_MOD_MA,0,{4,0,8,3,0}},{"Masamune",.f.af=masamune,740,0,0,0,0,0,0,{0,0,3,3,0}},{"Chirijiraden",.f.af=chirijiraden,820,AFLAG_MAGIC|0,0,0,5,AFLAG_MOD_MA,0,{0,0,3,3,0}}}, //DRAW OUT (Samurai)
{{"Accumulate",.f.af=accumulate,300,0,0,0,0,0,0,{0,0,1,0,0}},{"Dash",.f.af=dash,75,AFLAG_PHYSICAL|0,0,0,2,AFLAG_MOD_XA,0,{1,1,1,0,0}},{"Throw Stone",.f.af=throw_stone,90,AFLAG_PHYSICAL|AFLAG_EVADE|0,0,0,2,AFLAG_MOD_XA,0,{4,0,1,0,0}},{"Heal",.f.af=heal,150,0,0,0,0,0,0,{1,2,1,0,0}}}, //BASIC SKILL (Squire)
{{"Moogle",.f.af=moogle,110,AFLAG_MAGIC|0,0,2,5,AFLAG_MOD_MA,8,{4,0,3,2,0}},{"Shiva",.f.af=shiva,200,AFLAG_MAGIC|0,ELEM_ICE,4,5,AFLAG_MOD_MA,24,{4,0,3,2,0}},{"Ramuh",.f.af=ramuh,200,AFLAG_MAGIC|0,ELEM_LIGHTNING,4,5,AFLAG_MOD_MA,24,{4,0,3,2,0}},{"Ifrit",.f.af=ifrit,200,AFLAG_MAGIC|0,ELEM_FIRE,4,5,AFLAG_MOD_MA,24,{4,0,3,2,0}},{"Titan",.f.af=titan,220,AFLAG_MAGIC|0,ELEM_EARTH,5,5,AFLAG_MOD_MA,30,{4,0,3,2,0}},/*{.f.af=golem,500,AFLAG_MAGIC|0,0,3,8,200,40,{0,0,1,0,0}},*/{"Carbunkle",.f.af=carbunkle,350,AFLAG_MAGIC|0,0,4,6,150,30,{4,0,3,2,0}},{"Bahamut",.f.af=bahamut,1200,AFLAG_MAGIC|0,0,10,5,AFLAG_MOD_MA,60,{4,0,4,3,0}},{"Odin",.f.af=odin,900,AFLAG_MAGIC|0,0,9,5,AFLAG_MOD_MA,50,{4,0,4,3,0}},{"Leviathan",.f.af=leviathan,850,AFLAG_MAGIC|0,ELEM_WATER,9,5,AFLAG_MOD_MA,48,{4,0,4,3,0}},{"Salamander",.f.af=salamander,820,AFLAG_MAGIC|0,ELEM_FIRE,9,5,AFLAG_MOD_MA,48,{4,0,3,2,0}},{"Silf",.f.af=silf,400,AFLAG_MAGIC|0,0,5,6,150,26,{4,0,3,2,0}},{"Fairy",.f.af=fairy,400,AFLAG_MAGIC|0,0,4,5,AFLAG_MOD_MA,28,{4,0,3,2,0}},{"Lich",.f.af=lich,600,AFLAG_MAGIC|0,ELEM_DARK,9,6,160,40,{4,0,3,2,0}},{"Cyclops",.f.af=cyclops,1000,AFLAG_MAGIC|0,0,9,5,AFLAG_MOD_MA,62,{4,0,3,2,0}},{"Zodiac",.f.af=zodiac,1000,AFLAG_MAGIC|0,0,10,5,AFLAG_MOD_MA,99,{4,0,4,3,0}}}, //SUMMON MAGIC (Summoner)
{{"Gil Taking",.f.af=gil_taking,10,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_EVADE|0,0,0,4,200,0,{1,1,1,0,0}},{"Steal Heart",.f.af=steal_heart,150,0,0,0,1,50,0,{3,0,1,0,0}},{"Seal Helmet",.f.af=steal_helmet,350,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_EVADE|0,0,0,4,50,0,{1,1,1,0,0}},{"Steal Armor",.f.af=steal_armor,450,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_EVADE|0,0,0,4,35,0,{1,1,1,0,0}},{"Steal Shield",.f.af=steal_shield,350,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_EVADE|0,0,0,4,35,0,{1,1,1,0,0}},{"Steal Weapon",.f.af=steal_weapon,600,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_EVADE|0,0,0,4,30,0,{1,1,1,0,0}},{"Steal Accessory",.f.af=steal_accessry,500,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_EVADE|0,0,0,4,40,0,{1,1,1,0,0}},{"Steal Exp",.f.af=steal_exp,250,AFLAG_PHYSICAL|0,0,0,4,70,0,{1,1,1,0,0}}}, //STEAL (Thief)
{{"Haste",.f.af=haste,100,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_MATH|0,0,2,6,180,8,{3,0,2,0,0}},{"Haste 2",.f.af=haste2,550,AFLAG_MAGIC|0,0,7,6,240,30,{3,0,2,3,0}},{"Slow",.f.af=slow,80,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,0,2,6,180,8,{3,0,2,0,0}},{"Slow 2",.f.af=slow2,520,AFLAG_MAGIC|AFLAG_COUNTER_MAGIC|AFLAG_EVADE|0,0,7,6,240,30,{3,0,2,3,0}},{"Stop",.f.af=stop,330,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,0,7,6,110,14,{3,0,2,0,0}},{"Don't Move",.f.af=dont_move,100,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,0,3,6,190,10,{3,0,2,1,0}},{"Float",.f.af=magic_float,200,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_MATH|0,0,2,6,140,8,{4,0,2,1,0}},{"Reflect",.f.af=reflect,330,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_MATH|0,0,2,6,180,12,{4,0,1,0,0}},{"Quick",.f.af=quick,800,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|0,0,4,6,140,24,{4,0,1,0,0}},{"Demi",.f.af=demi,250,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,0,6,6,190,24,{4,0,2,1,0}},{"Demi 2",.f.af=demi2,550,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,0,9,6,120,50,{4,0,2,3,0}},{"Meteor",.f.af=meteor,1500,AFLAG_MAGIC|AFLAG_COUNTER_MAGIC|0,0,13,5,AFLAG_MOD_MA,70,{4,0,4,3,0}}}, //TIME MAGIC (Time Mage)
{{"Fire",.f.af=fire,50,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,ELEM_FIRE,4,5,AFLAG_MOD_MA,6,{4,0,2,1,0}},{"Fire 2",.f.af=fire2,200,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,ELEM_FIRE,5,5,AFLAG_MOD_MA,12,{4,0,2,2,0}},{"Fire 3",.f.af=fire3,480,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,ELEM_FIRE,7,5,AFLAG_MOD_MA,24,{4,0,2,3,0}},{"Fire 4",.f.af=fire4,850,AFLAG_MAGIC|AFLAG_EVADE|0,ELEM_FIRE,10,5,AFLAG_MOD_MA,48,{4,0,3,3,0}},{"Bolt",.f.af=bolt,50,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,ELEM_LIGHTNING,4,5,AFLAG_MOD_MA,6,{4,0,2,1,0}},{"Bolt 2",.f.af=bolt2,200,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,ELEM_LIGHTNING,5,5,AFLAG_MOD_MA,10,{4,0,2,2,0}},{"Bolt 3",.f.af=bolt3,480,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,ELEM_LIGHTNING,7,5,AFLAG_MOD_MA,24,{4,0,2,3,0}},{"Bolt 4",.f.af=bolt4,850,AFLAG_MAGIC|AFLAG_EVADE|0,ELEM_LIGHTNING,10,5,AFLAG_MOD_MA,48,{4,0,3,3,0}},{"Ice",.f.af=ice,50,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,ELEM_ICE,4,5,AFLAG_MOD_MA,6,{4,0,2,1,0}},{"Ice 2",.f.af=ice2,200,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,ELEM_ICE,5,5,AFLAG_MOD_MA,12,{4,0,2,2,0}},{"Ice 3",.f.af=ice3,480,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,ELEM_ICE,7,5,AFLAG_MOD_MA,24,{4,0,2,3,0}},{"Ice 4",.f.af=ice4,850,AFLAG_MAGIC|AFLAG_EVADE|0,ELEM_ICE,10,5,AFLAG_MOD_MA,48,{4,0,3,3,0}},{"Poison",.f.af=poison,150,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,0,3,6,160,6,{3,0,2,0,0}},{"Frog",.f.af=frog,500,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,0,5,6,120,12,{3,0,1,0,0}},{"Death",.f.af=death,600,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,0,10,6,100,24,{3,0,1,0,0}},{"Flare",.f.af=flare,900,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,0,7,5,AFLAG_MOD_MA,60,{5,0,1,0,0}}}, //BLACK MAGIC (Wizard)
};
/*
//NON-JOB ABILITIES: 
{{.f.af=yell,200,0,0,0,0,0,{3,0,1,0,0}},{.f.af=cheer_up,200,0,0,0,0,0,{3,0,1,0,0}},{.f.af=wish,0,0,0,0,0,0,{1,3,1,0,0}},{.f.af=scream,500,0,0,0,0,0,{0,0,1,0,0}},{.f.af=ultima_guts,500,AFLAG_MAGIC|0,0,5,5,10,{4,0,2,1,0}}}, //GUTS 
{{.f.af=stasis_sword,100,AFLAG_PHYSICAL|0,0,0,5,0,{2,0,2,0,0}},{.f.af=split_punch,400,AFLAG_PHYSICAL|0,0,0,5,0,{3,2,1,0,0}},{.f.af=crush_punch,500,AFLAG_PHYSICAL|0,0,0,5,0,{3,1,1,0,0}},{.f.af=lightning_stab,700,AFLAG_PHYSICAL|0,0,0,5,0,{3,0,2,1,0}},{.f.af=holy_explosion,800,AFLAG_PHYSICAL|0,0,0,5,0,{4,0,5,2,0}}}, //HOLY SWORD 
{{.f.af=shellbust_stab,200,AFLAG_PHYSICAL|0,0,0,5,0,{3,0,1,0,0}},{.f.af=blastar_punch,400,AFLAG_PHYSICAL|0,0,0,5,0,{3,0,1,0,0}},{.f.af=hellcry_punch,500,AFLAG_PHYSICAL|0,0,0,5,0,{3,0,1,0,0}},{.f.af=icewolf_bite,800,AFLAG_PHYSICAL|0,0,0,5,0,{3,0,1,0,0}}}, //MIGHTY SWORD 
{{.f.af=dark_sword,500,AFLAG_PHYSICAL|0,0,0,5,0,{3,0,1,0,0}},{.f.af=night_sword,250,AFLAG_PHYSICAL|0,0,0,5,0,{3,0,1,0,0}}}, //DARK SWORD 
{{.f.af=dark_holy,0,AFLAG_MAGIC|AFLAG_COUNTER_MAGIC|0,ELEM_DARK,7,5,40,{4,0,1,0,0}}}, //DARK MAGIC 
{{.f.af=deathspell2,0,AFLAG_MAGIC|0,0,4,6,20,{4,0,1,0,0}}}, //HOLY MAGIC 
{{.f.af=galaxy_stop,0,0,0,5,0,0,{4,0,1,0,0}}}, //STARRY HEAVEN 
{{.f.af=heaven_thunder,0,AFLAG_MAGIC|0,ELEM_LIGHTNING,3,5,0,{4,0,2,3,0}},{.f.af=asura_truth,200,AFLAG_MAGIC|0,ELEM_FIRE,4,5,0,{4,0,2,3,0}},{.f.af=diamond_sword,300,AFLAG_MAGIC|0,ELEM_WIND,5,5,0,{4,0,2,3,0}},{.f.af=hydragon_pit,400,AFLAG_MAGIC|0,ELEM_WATER,6,5,0,{4,0,2,3,0}},{.f.af=space_storage,500,AFLAG_MAGIC|0,0,5,5,0,{4,0,2,3,0}},{.f.af=sky_demon,600,AFLAG_MAGIC|0,ELEM_EARTH,7,5,0,{4,0,2,3,0}}}, //TRUTH 
{{.f.af=heaven_bltback,0,AFLAG_MAGIC|0,ELEM_LIGHTNING,3,5,0,{4,0,2,3,0}},{.f.af=asura_back,200,AFLAG_MAGIC|0,ELEM_FIRE,4,5,0,{4,0,2,3,0}},{.f.af=dia_swd_back,300,AFLAG_MAGIC|0,ELEM_WIND,5,5,0,{4,0,2,3,0}},{.f.af=dragon_pit_back,400,AFLAG_MAGIC|0,ELEM_WATER,6,5,0,{4,0,2,3,0}}}, //UN-TRUTH 
{{.f.af=space_str_back,500,AFLAG_MAGIC|0,0,5,5,0,{4,0,2,3,0}}}, //TRUTH 
{{.f.af=sky_demon_back,600,AFLAG_MAGIC|0,ELEM_EARTH,7,5,0,{4,0,2,3,0}}}, //UN-TRUTH 
{{.f.af=seal,0,0,0,0,0,0,{6,0,1,0,0}},{.f.af=shadow_stitch,0,0,0,0,0,0,{4,0,1,0,0}},{.f.af=stop_bracelet,0,0,0,0,0,0,{1,0,1,0,0}}}, //USE HAND 
{{.f.af=shock,0,0,0,0,0,0,{5,3,1,0,0}},{.f.af=difference,0,0,0,0,0,0,{5,3,1,0,0}}}, //BYBLOS 
{{.f.af=seal,0,0,0,0,0,0,{5,0,1,0,0}},{.f.af=chicken_race,0,0,0,0,0,0,{5,0,1,0,0}},{.f.af=hold_tight,0,0,0,0,0,0,{5,0,1,0,0}},{.f.af=darkness,0,0,0,0,0,0,{5,0,1,0,0}},{.f.af=lose_voice,0,0,0,0,0,0,{5,0,1,0,0}},{.f.af=loss,0,0,0,0,0,0,{5,0,2,0,0}},{.f.af=spell,0,0,0,0,0,0,{5,0,2,0,0}},{.f.af=nightmare,0,0,0,0,0,0,{5,0,2,0,0}},{.f.af=death_cold,0,0,0,0,0,0,{5,0,2,0,0}}}, //FEAR 
{{.f.af=magic_ruin,400,AFLAG_MAGIC|0,0,3,6,20,{4,0,2,0,0}},{.f.af=speed_ruin,400,AFLAG_MAGIC|0,0,3,6,20,{4,0,2,0,0}},{.f.af=power_ruin,400,AFLAG_MAGIC|0,0,3,6,20,{4,0,2,0,0}},{.f.af=mind_ruin,400,AFLAG_MAGIC|0,0,3,6,20,{4,0,2,0,0}}}, //DESTROY SWORD 
{{.f.af=blood_suck,0,0,0,0,0,20,{1,0,1,0,0}}}, //BLOOD SUCK 
{{.f.af=allure,0,0,0,0,1,20,{3,0,1,0,0}}}, //USE HAND 
{{.f.af=bio,0,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_EVADE|0,0,3,5,8,{4,0,2,2,0}},{.f.af=bio,0,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_EVADE|0,0,3,5,8,{4,0,2,2,0}},{.f.af=bio,0,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_EVADE|0,0,3,5,8,{4,0,2,2,0}},{.f.af=bio2,0,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_EVADE|0,0,5,6,16,{4,0,2,3,0}},{.f.af=bio2,0,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_EVADE|0,0,5,6,16,{4,0,2,3,0}},{.f.af=bio2,0,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_EVADE|0,0,5,6,16,{4,0,2,3,0}},{.f.af=bio2,0,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_EVADE|0,0,5,6,16,{4,0,2,3,0}},{.f.af=bio3,0,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_EVADE|0,0,6,5,24,{4,0,3,3,0}},{.f.af=bio3,0,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_EVADE|0,0,6,5,24,{4,0,3,3,0}},{.f.af=bio3,0,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_EVADE|0,0,6,5,24,{4,0,3,3,0}}}, //BIO 
{{.f.af=mbarrier,0,AFLAG_MAGIC|0,0,4,6,30,{5,0,1,0,0}}}, //HOLY MAGIC 
{{.f.af=leg_aim,200,AFLAG_PHYSICAL|0,0,0,3,0,{RANGE_WEAPON,0,1,0,0}},{.f.af=arm_aim,300,AFLAG_PHYSICAL|0,0,0,3,0,{RANGE_WEAPON,0,1,0,0}},{.f.af=seal_evil,200,AFLAG_PHYSICAL|0,0,0,3,0,{RANGE_WEAPON,0,1,0,0}}}, //SNIPE 
{{.f.af=melt,9999,AFLAG_MAGIC|AFLAG_COUNTER_MAGIC|0,ELEM_FIRE,7,5,70,{4,0,4,3,0}},{.f.af=tornado,9999,AFLAG_MAGIC|AFLAG_COUNTER_MAGIC|0,ELEM_WIND,7,5,70,{4,0,4,3,0}},{.f.af=quake,9999,AFLAG_MAGIC|AFLAG_COUNTER_MAGIC|0,ELEM_EARTH,7,5,70,{4,0,4,3,0}}}, //DIMENSION MAGIC 
{{.f.af=toad2,0,AFLAG_MAGIC|0,0,4,6,35,{4,0,2,3,0}},{.f.af=gravi2,0,AFLAG_MAGIC|0,0,4,6,35,{4,0,2,3,0}},{.f.af=flare2,0,AFLAG_MAGIC|0,0,4,6,35,{4,0,2,3,0}},{.f.af=blind2,0,AFLAG_MAGIC|0,0,4,6,35,{4,0,2,3,0}}}, //JA MAGIC 
{{.f.af=small_bomb_grenade,0,AFLAG_PHYSICAL|AFLAG_EVADE|0,0,0,2,0,{1,0,1,0,0}},{.f.af=small_bomb_explosive,0,AFLAG_PHYSICAL|AFLAG_EVADE|0,0,0,2,0,{1,0,1,0,0}}}, //BOMB 
{{.f.af=confuse2,0,AFLAG_MAGIC|0,0,4,6,35,{4,0,2,3,0}},{.f.af=sleep2,0,AFLAG_MAGIC|0,0,4,6,35,{4,0,2,3,0}}}, //JA MAGIC 
{{.f.af=ultima_ultimate,0,AFLAG_MAGIC|0,0,7,5,40,{4,0,3,3,0}},0,AFLAG_MAGIC|0,0,15,5,50,{5,0,4,3,0}}}, //ULTIMATE MAGIC 
{{.f.af=mute,0,AFLAG_MAGIC|0,0,7,6,0,{4,0,3,3,0}},{.f.af=despair2,0,AFLAG_MAGIC|0,0,7,6,0,{4,0,3,3,0}},{.f.af=return2,0,AFLAG_MAGIC|0,0,7,6,0,{4,0,3,3,0}}}, //SATURATION 
{{.f.af=blind_sword,50,AFLAG_MAGIC|AFLAG_EVADE|0,0,0,6,6,{4,0,1,0,0}},{.f.af=aspel,0,AFLAG_MAGIC|AFLAG_EVADE|0,0,0,6,2,{4,0,1,0,0}},{.f.af=drain,180,AFLAG_MAGIC|AFLAG_EVADE|0,0,0,6,12,{4,0,1,0,0}},{.f.af=faith,200,AFLAG_MAGIC|AFLAG_EVADE|0,0,0,6,10,{4,0,1,0,0}},{.f.af=innocent,200,AFLAG_MAGIC|AFLAG_EVADE|0,0,0,6,10,{4,0,1,0,0}},{.f.af=zombie_sword,150,AFLAG_MAGIC|AFLAG_EVADE|0,0,0,6,14,{4,0,1,0,0}},{.f.af=silence,90,AFLAG_MAGIC|AFLAG_EVADE|0,0,0,6,16,{4,0,1,0,0}},{.f.af=berserk,200,AFLAG_MAGIC|AFLAG_EVADE|0,0,0,6,16,{4,0,1,0,0}},{.f.af=chicken,500,AFLAG_MAGIC|AFLAG_EVADE|0,0,0,6,12,{4,0,1,0,0}},{.f.af=confuse,200,AFLAG_MAGIC|AFLAG_EVADE|0,0,0,6,14,{4,0,1,0,0}},{.f.af=despair,300,AFLAG_MAGIC|AFLAG_EVADE|0,0,0,6,20,{4,0,1,0,0}},50,AFLAG_MAGIC|AFLAG_EVADE|0,0,0,6,14,{4,0,1,0,0}},{.f.af=magic_sleep_sword,170,AFLAG_MAGIC|AFLAG_EVADE|0,0,0,6,20,{4,0,1,0,0}},{.f.af=break_sword,300,AFLAG_MAGIC|AFLAG_EVADE|0,0,0,6,24,{4,0,1,0,0}},{.f.af=shock,600,0,0,0,0,20,{8,0,1,0,0}}}, //MAGIC SWORD 
{{.f.af=ice_bracelet,0,AFLAG_MAGIC|AFLAG_EVADE|0,ELEM_ICE,0,5,0,{2,2,1,0,0}},{.f.af=fire_bracelet,0,AFLAG_MAGIC|AFLAG_EVADE|0,ELEM_FIRE,0,5,0,{2,2,1,0,0}},{.f.af=thnder_brcelet,0,AFLAG_MAGIC|AFLAG_EVADE|0,ELEM_LIGHTNING,0,5,0,{2,2,1,0,0}},{.f.af=dragon_tame,300,0,0,0,0,0,{2,2,1,0,0}},{.f.af=dragon_care,300,0,0,0,0,0,{2,2,1,0,0}},{.f.af=dragon_powerup,400,0,0,0,0,0,{2,2,1,0,0}},{.f.af=dragon_levelup,400,0,0,0,0,0,{2,2,1,0,0}},{.f.af=holy_bracelet,900,AFLAG_MAGIC|0,ELEM_HOLY,0,5,0,{4,0,3,3,0}}}, //DRAGON 
{{.f.af=braver,150,AFLAG_MAGIC|0,0,3,5,0,{2,0,1,0,0}},200,AFLAG_MAGIC|0,0,4,5,0,{2,0,2,0,0}},{.f.af=blade_beam,250,AFLAG_MAGIC|0,0,5,5,0,{2,0,1,0,0}},{.f.af=climhazzard,450,AFLAG_MAGIC|0,0,7,5,0,{2,0,1,0,0}},{.f.af=meteorain,560,AFLAG_MAGIC|0,0,10,5,0,{3,0,3,0,0}},{.f.af=finish_touch,670,AFLAG_MAGIC|0,0,5,5,0,{3,0,2,0,0}},{.f.af=omnislash,900,AFLAG_MAGIC|0,0,15,5,0,{3,0,3,0,0}},{.f.af=cherry_blossom,1200,AFLAG_MAGIC|0,0,20,5,0,{3,0,3,0,0}}}, //LIMIT 
{{.f.af=choco_attack,0,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_COUNTER_FLOOD|AFLAG_EVADE|0,0,0,2,0,{1,2,1,0,0}},{.f.af=choco_ball,0,AFLAG_PHYSICAL|AFLAG_EVADE|0,0,0,2,0,{4,0,1,0,0}},{.f.af=choco_meteor,0,AFLAG_MAGIC|0,0,0,5,0,{5,0,1,0,0}},{.f.af=choco_esuna,0,AFLAG_MAGIC|0,0,0,6,0,{0,0,2,2,0}},{.f.af=choco_cure,0,AFLAG_MAGIC|0,0,0,5,0,{0,0,2,2,0}}}, //(CHOCOBO)
{{.f.af=tackle,0,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_COUNTER_FLOOD|AFLAG_EVADE|0,0,0,2,0,{1,2,1,0,0}},{.f.af=goblin_punch,0,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_COUNTER_FLOOD|AFLAG_EVADE|0,0,0,3,0,{1,1,1,0,0}},{.f.af=turn_punch,0,AFLAG_PHYSICAL|AFLAG_EVADE|0,0,0,2,0,{0,0,2,1,0}},{.f.af=eye_gouge,0,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_COUNTER_FLOOD|AFLAG_EVADE|0,0,0,3,0,{1,2,1,0,0}},{.f.af=mutilate,0,AFLAG_PHYSICAL|AFLAG_EVADE|0,0,0,3,0,{1,0,1,0,0}},0,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_COUNTER_FLOOD|AFLAG_EVADE|0,0,0,2,0,{1,2,1,0,0}}}, //(GOBLIN)
{{.f.af=small_bomb_bomb,0,AFLAG_MAGIC|AFLAG_EVADE|0,0,0,5,0,{1,0,1,0,0}},0,0,0,0,0,0,{0,0,3,3,0}},{.f.af=flame_attack,0,AFLAG_MAGIC|AFLAG_EVADE|0,ELEM_FIRE,0,5,0,{3,0,1,0,0}},{.f.af=spark,0,AFLAG_MAGIC|0,ELEM_FIRE,0,5,0,{0,0,3,1,0}}}, //(BOMB)
{{.f.af=scratch,0,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_COUNTER_FLOOD|AFLAG_EVADE|0,0,0,2,0,{1,3,1,0,0}},{.f.af=cat_kick,0,AFLAG_PHYSICAL|AFLAG_EVADE|0,0,0,2,0,{1,2,1,0,0}},{.f.af=blaster,0,AFLAG_MAGIC|AFLAG_EVADE|0,0,0,6,0,{3,0,1,0,0}},{.f.af=poison_nail,0,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_COUNTER_FLOOD|AFLAG_EVADE|0,0,0,3,0,{1,2,1,0,0}},{.f.af=blood_suck_coeurl,0,0,0,0,0,0,{1,0,1,0,0}}}, //(COEURL)
{{.f.af=tentacle,0,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_COUNTER_FLOOD|AFLAG_EVADE|0,0,0,2,0,{1,2,1,0,0}},{.f.af=black_ink,0,AFLAG_PHYSICAL|AFLAG_EVADE|0,0,0,3,0,{2,0,1,0,0}},{.f.af=odd_soundwave,0,0,0,0,0,0,{0,0,3,1,0}},{.f.af=mind_blast,0,AFLAG_MAGIC|0,0,0,6,0,{3,0,2,1,0}},{.f.af=level_blast,0,AFLAG_MAGIC|0,0,0,6,0,{4,0,1,0,0}}}, //(MINDFLAYER)
{{.f.af=knife_hand,0,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_COUNTER_FLOOD|AFLAG_EVADE|0,0,0,2,0,{1,2,1,0,0}},{.f.af=thunder_soul,0,AFLAG_MAGIC|AFLAG_EVADE|0,ELEM_LIGHTNING,0,5,0,{3,0,1,0,0}},{.f.af=aqua_soul,0,AFLAG_MAGIC|AFLAG_EVADE|0,ELEM_WATER,0,5,0,{3,0,1,0,0}},{.f.af=ice_soul,0,AFLAG_MAGIC|AFLAG_EVADE|0,ELEM_ICE,0,5,0,{3,0,1,0,0}},{.f.af=wind_soul,0,AFLAG_MAGIC|AFLAG_EVADE|0,ELEM_WIND,0,5,0,{3,0,1,0,0}}}, //(SKELETON)
{{.f.af=throw_spirit,0,AFLAG_PHYSICAL|AFLAG_COUNTER_FLOOD|AFLAG_EVADE|0,0,0,2,0,{3,0,1,0,0}},{.f.af=zombie_touch,0,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_EVADE|0,0,0,3,0,{1,2,1,0,0}},{.f.af=sleep_touch,0,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_EVADE|0,0,0,3,0,{1,2,1,0,0}},{.f.af=drain_touch,0,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_EVADE|0,0,0,3,0,{1,2,1,0,0}},{.f.af=grease_touch,0,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_EVADE|0,0,0,3,0,{1,2,1,0,0}}}, //(GHOST)
{{.f.af=wing_attack,0,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_COUNTER_FLOOD|AFLAG_EVADE|0,0,0,2,0,{1,2,1,0,0}},{.f.af=look_of_devil,0,AFLAG_PHYSICAL|AFLAG_EVADE|0,0,0,3,0,{3,0,1,0,0}},{.f.af=look_of_fright,0,AFLAG_MAGIC|AFLAG_EVADE|0,0,0,6,0,{3,0,1,0,0}},{.f.af=circle,0,AFLAG_MAGIC|0,0,0,6,0,{4,0,1,0,0}},{.f.af=death_sentence_ahriman,0,AFLAG_MAGIC|0,0,0,6,0,{3,0,1,0,0}}}, //(AHRIMAN)
{{.f.af=scratch_up,0,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_COUNTER_FLOOD|AFLAG_EVADE|0,0,0,2,0,{1,2,1,0,0}},{.f.af=beak,0,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_EVADE|0,0,0,3,0,{1,0,1,0,0}},{.f.af=shine_lover,0,AFLAG_PHYSICAL|AFLAG_COUNTER|0,0,0,3,0,{1,1,1,0,0}},{.f.af=feather_bomb,0,AFLAG_MAGIC|0,0,0,5,0,{3,0,1,0,0}},{.f.af=beaking,0,AFLAG_MAGIC|AFLAG_COUNTER|AFLAG_EVADE|0,0,0,6,0,{1,1,1,0,0}}}, //(COCKATRICE)
{{.f.af=straight_dash,0,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_COUNTER_FLOOD|AFLAG_EVADE|0,0,0,2,0,{1,0,1,0,0}},{.f.af=nose_bracelet,0,0,0,0,1,0,{1,1,1,0,0}},{.f.af=oink,0,AFLAG_PHYSICAL|0,0,0,3,0,{1,1,1,0,0}},0,0,0,0,0,0,{1,1,1,0,0}},{.f.af=please_eat,0,0,0,0,0,0,{1,1,1,0,0}}}, //(URIBO)
{{.f.af=leaf_dance,0,AFLAG_MAGIC|AFLAG_COUNTER_FLOOD|0,0,0,5,0,{0,0,2,0,0}},{.f.af=protect_spirit,0,AFLAG_MAGIC|0,0,0,6,0,{0,0,2,0,0}},{.f.af=clam_spirit,0,AFLAG_MAGIC|0,0,0,6,0,{0,0,2,0,0}},{.f.af=spirit_of_life,0,AFLAG_MAGIC|0,0,0,5,0,{0,0,2,0,0}},{.f.af=magic_spirit,0,AFLAG_MAGIC|0,0,0,5,0,{0,0,2,0,0}}}, //(DRYAD)
{{.f.af=shake_off,0,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_COUNTER_FLOOD|AFLAG_EVADE|0,0,0,2,0,{1,2,1,0,0}},{.f.af=wave_around,0,AFLAG_PHYSICAL|AFLAG_COUNTER|0,0,0,2,0,{0,0,2,1,0}},{.f.af=mimic_titan,0,AFLAG_MAGIC|0,ELEM_EARTH,0,5,0,{0,0,3,1,0}},{.f.af=gather_power,0,0,0,0,0,0,{0,0,1,0,0}},{.f.af=blow_fire,0,AFLAG_MAGIC|AFLAG_EVADE|0,ELEM_FIRE,0,5,0,{2,2,1,0,0}}}, //(MINOTAUR)
{{.f.af=tentacle,0,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_COUNTER_FLOOD|AFLAG_EVADE|0,0,0,2,0,{1,1,1,0,0}},{.f.af=lick,0,0,0,0,0,0,{1,0,1,0,0}},{.f.af=goo,0,AFLAG_COUNTER|0,0,0,0,0,{1,1,1,0,0}},{.f.af=bad_bracelet,0,0,0,0,0,0,{0,0,3,0,0}},{.f.af=moldball_virus,0,0,0,0,0,0,{1,0,1,0,0}}}, //(MORBOL)
{{.f.af=stab_up,0,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_COUNTER_FLOOD|AFLAG_EVADE|0,0,0,2,0,{1,2,1,0,0}},{.f.af=sudden_cry,0,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_EVADE|0,0,0,2,0,{1,0,1,0,0}},{.f.af=hurricane,0,AFLAG_MAGIC|0,ELEM_WIND,0,6,0,{4,0,3,2,0}},{.f.af=ulmaguest,0,0,0,0,0,0,{4,0,3,1,0}},{.f.af=giga_flare,0,AFLAG_MAGIC|0,0,0,5,0,{4,0,3,0,0}}}, //(BEHEMOTH)
{{.f.af=dash_dragon,0,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_COUNTER_FLOOD|AFLAG_EVADE|0,0,0,2,0,{1,2,1,0,0}},{.f.af=tail_swing,0,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_EVADE|0,0,0,2,0,{1,2,1,0,0}},{.f.af=ice_bracelet_dragon,0,AFLAG_MAGIC|AFLAG_EVADE|0,ELEM_ICE,0,5,0,{2,2,1,0,0}},{.f.af=fire_bracelet_dragon,0,AFLAG_MAGIC|AFLAG_EVADE|0,ELEM_FIRE,0,5,0,{2,2,1,0,0}},{.f.af=thnder_brcelet_dragon,0,AFLAG_MAGIC|AFLAG_EVADE|0,ELEM_LIGHTNING,0,5,0,{2,2,1,0,0}}}, //(DRAGON)
{{.f.af=triple_attack,0,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_COUNTER_FLOOD|AFLAG_EVADE|0,0,0,2,0,{1,1,1,0,0}},{.f.af=triple_bracelet,0,AFLAG_MAGIC|AFLAG_EVADE|0,0,0,0,0,{2,2,2,0,0}},{.f.af=triple_thunder,0,AFLAG_MAGIC|0,ELEM_LIGHTNING,0,5,0,{4,0,2,0,0}},{.f.af=triple_flame,0,AFLAG_MAGIC|0,ELEM_FIRE,0,5,0,{4,0,2,0,0}},{.f.af=dark_whisper,0,AFLAG_MAGIC|0,ELEM_DARK,0,5,0,{4,0,2,0,0}}}, //(HYDRA)
{{.f.af=snake_carrier,0,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_EVADE|0,0,0,2,0,{1,2,1,0,0}},{.f.af=poison_frog,0,0,0,0,0,0,{4,0,3,1,0}},{.f.af=midgar_swarm,0,AFLAG_MAGIC|0,0,10,5,0,{4,0,3,2,0}}}, //DARK CLOUD 
{{.f.af=lifebreak,0,0,0,2,0,0,{4,0,1,0,0}}}, //DARK MAGIC 
{{.f.af=nanoflare,0,AFLAG_MAGIC|0,0,5,5,0,{4,0,3,1,0}}}, //NIGHT MAGIC 
{{.f.af=grand_cross,0,0,0,5,0,0,{4,0,3,2,0}}}, //COMPLETE MAGIC 
{{.f.af=destroy,0,0,0,0,7,0,{1,2,1,0,0}},{.f.af=compress,0,0,0,0,7,0,{1,2,1,0,0}},{.f.af=dispose,0,0,0,0,7,0,{8,0,1,0,0}},{.f.af=crush,0,0,0,0,7,0,{1,0,1,0,0}}}, //WORK 
{{.f.af=energy,0,0,0,0,0,0,{4,0,1,0,0}},{.f.af=parasite,0,0,0,0,0,0,{4,0,1,0,0}}}, // BYBLOS 
*/

const uint8_t num_support[]={0,2,0,1,3,0,1,3,1,3,0,1,1,1,1,2,4,1,1,1,1};
const struct support_ability clsupport[NUM_CLASS][NUM_SUPPORT_PER_ABILITY]={
	{}, // Generic
	{{SFLAG_EQUIP_CROSSBOW,350},{SFLAG_CONCENTRATE,400}}, // Archer
	{}, // Bard
	{{SFLAG_EXPUP,350}}, // Calculator
	{{SFLAG_THROW_ITEM,350},{SFLAG_MAINTENANCE,250},{SFLAG_EQUIP_CHANGE,0}}, // Chemist
	{}, // Dancer
	{{SFLAG_ATTACKUP,400}}, // Geomancer
	{{SFLAG_EQUIP_ARMOR,500},{SFLAG_EQUIP_SHIELD,250},{SFLAG_EQUIP_SWORD,400}}, // Knight
	{{SFLAG_EQUIP_SPEAR,400}}, // Lancer
	{{SFLAG_EQUIP_GUN,750},{SFLAG_TRAIN,450},{SFLAG_MONSTER_TALK,100}}, // Mediator
	{}, // Mime
	{{SFLAG_MARTIAL_ARTS,200}}, // Monk
	{{SFLAG_TWO_SWORDS,900}}, // Ninja
	{{SFLAG_DEFENSEUP,400}}, // Oracle
	{{SFLAG_MAGIC_DEFENDUP,400}}, // Priest
	{{SFLAG_EQUIP_KNIFE,400},{SFLAG_TWO_HANDS,900}}, // Samurai
	{{SFLAG_EQUIP_AXE,170},{SFLAG_MONSTER_SKILL,200},{SFLAG_DEFEND,50},{SFLAG_JPUP,200}}, // Squire
	{{SFLAG_HALFMP,900}}, // Summoner
	{{SFLAG_SECRET_HUNT,200}}, // Thief
	{{SFLAG_SHORTCHARGE,800}}, // Time Mage
	{{SFLAG_MAGIC_ATTACKUP,400}}, // Wizard
};

static reactret speed_save(struct battle_char *reacter, struct battle_char *attacker){
	reacter->speed+=1;
	return 0;
}

static reactret arrow_guard(struct battle_char *reacter, struct battle_char *attacker){
	return reacter->brave;
}

static reactret ma_save(struct battle_char *reacter, struct battle_char *attacker){
	reacter->ma+=1;
	return 0;
}

static reactret face_up(struct battle_char *reacter, struct battle_char *attacker){
	if(claction[last_action.preresolve->jobindex][last_action.preresolve->jobindex].mod==5 || claction[last_action.preresolve->jobindex][last_action.preresolve->jobindex].mod==6)
		reacter->faith+=3;
	return 0;
}

static reactret distribute(struct battle_char *reacter, struct battle_char *attacker){
	if(last_action.damage<0 || STATUS_SET(reacter,STATUS_UNDEAD))
		deal_damage(attacker,last_action.damage/2);
	return 0;
}

static reactret damage_split(struct battle_char *reacter, struct battle_char *attacker){
	if(last_action.damage>0 || STATUS_SET(reacter,STATUS_UNDEAD)){
		deal_damage(attacker,last_action.damage/2);
		deal_damage(reacter,-last_action.damage/2);
	}
	return 0;
}

static reactret auto_potion(struct battle_char *reacter, struct battle_char *attacker){
	if(last_action.damage>0 || STATUS_SET(reacter,STATUS_UNDEAD)){
		//TODO use_item
	}
	return 0;
}

static reactret a_save(struct battle_char *reacter, struct battle_char *attacker){
	reacter->pa+=1;
	return 0;
}

static reactret brave_up(struct battle_char *reacter, struct battle_char *attacker){
	reacter->brave+=3;
	return 0;
}

static reactret counter_flood(struct battle_char *reacter, struct battle_char *attacker){
	uint8_t terrain;
	uint8_t findex=0;

	if(!claction[last_action.preresolve->jobindex][last_action.preresolve->findex].flags&AFLAG_COUNTER_FLOOD)
		return 0;

	terrain=get_map_terrain(reacter->x,reacter->y);
	switch(terrain){
		case MAP_T_NORMAL:
			findex=0;
			break;
		case MAP_T_WATER:
			findex=1;
			break;
		case MAP_T_STONE:
			findex=2;
			break;
		case MAP_T_GRASS:
			findex=3;
			break;
		case MAP_T_ROCK:
			findex=4;
			break;
		case MAP_T_WOOD:
			findex=6;
			break;
		case MAP_T_SWAMP:
			findex=7;
			break;
		case MAP_T_SAND:
			findex=8;
			break;
		case MAP_T_SNOW:
			findex=9;
			break;
	}

	// TODO cast the spell

	return 0;
}

static reactret weapon_guard(struct battle_char *reacter, struct battle_char *attacker){
	return 0;
}

static reactret dragon_spirit(struct battle_char *reacter, struct battle_char *attacker){
	if(reacter->status[STATUS_RERAISE]!=IMMUNE_TO_STATUS)
		reacter->status[STATUS_RERAISE]=UNTIMED_STATUS;
	return 0;
}

static reactret finger_guard(struct battle_char *reacter, struct battle_char *attacker){
	if(last_action.preresolve->jobindex==CL_MEDIATOR)
		return reacter->brave;
	return 0;
}

static reactret hp_restore(struct battle_char *reacter, struct battle_char *attacker){
	reacter->hp=reacter->hp_max;
	return 0;
}

static reactret counter(struct battle_char *reacter, struct battle_char *attacker){
	attack(reacter,attacker);
	return 0;
}

static reactret abandon(struct battle_char *reacter, struct battle_char *attacker){
	return 2;
}

static reactret sunken_state(struct battle_char *reacter, struct battle_char *attacker){
	reacter->status[STATUS_TRANSPARENT]=UNTIMED_STATUS;
	return 0;
}

static reactret absorb_used_mp(struct battle_char *reacter, struct battle_char *attacker){
	reacter->mp+=last_action.mp_used;
	if(reacter->mp>reacter->mp_max)
		reacter->mp=reacter->mp_max;
	return 0;
}

static reactret regenerator(struct battle_char *reacter, struct battle_char *attacker){
	add_status(reacter,STATUS_REGEN);
	return 0;
}

static reactret blade_grasp(struct battle_char *reacter, struct battle_char *attacker){
	return reacter->brave;
}

static reactret meatbone_slash(struct battle_char *reacter, struct battle_char *attacker){
	deal_damage(attacker,reacter->hp);
	return 0;
}

static reactret counter_tackle(struct battle_char *reacter, struct battle_char *attacker){
	dash(reacter,attacker);
	return 0;
}

static reactret mp_restore(struct battle_char *reacter, struct battle_char *attacker){
	reacter->mp=reacter->mp_max;
	return 0;
}

static reactret caution(struct battle_char *reacter, struct battle_char *attacker){
	add_status(reacter,STATUS_DEFENDING);
	return 0;
}

static reactret critical_quick(struct battle_char *reacter, struct battle_char *attacker){
	add_status(reacter,STATUS_QUICK);
	return 0;
}

static reactret mp_switch(struct battle_char *reacter, struct battle_char *attacker){
	if(reacter->mp>0){
		if(reacter->mp>last_action.damage){
			reacter->mp-=last_action.damage;
			restore_hp(reacter,last_action.damage);
		}
		else{
			restore_hp(reacter,reacter->mp);
			reacter->mp=0;
		}
	}
	return 0;
}

static reactret counter_magic(struct battle_char *reacter, struct battle_char *attacker){
	const struct ability *act=&claction[last_action.preresolve->jobindex][last_action.preresolve->findex];
	if(act->flags&AFLAG_COUNTER_MAGIC && reacter->mp>act->mp){
		reacter->mp-=act->mp;
		act->f.af(reacter,attacker);
	}
	return 0;
}

const uint8_t num_reaction[]={0,2,2,2,1,2,1,1,1,1,0,2,2,1,1,2,1,1,1,2,1};
const struct reaction_ability clreaction[NUM_CLASS][NUM_REACTION_PER_ABILITY]={
	{}, // Generic
	{
		{speed_save,800,RFLAG_TRIGGER_DAMAGE,RFLAG_BRAVE_PERCENT},
		{arrow_guard,450,RFLAG_TRIGGER_ALWAYS,RFLAG_BRAVE_PERCENT|RFLAG_HITMOD},
	}, // Archer
	{
		{ma_save,450,RFLAG_TRIGGER_DAMAGE,RFLAG_BRAVE_PERCENT},
		{face_up,500,RFLAG_TRIGGER_OTHER,RFLAG_BRAVE_PERCENT},
	}, // Bard
	{
		{distribute,200,RFLAG_TRIGGER_OTHER,RFLAG_BRAVE_PERCENT},
		{damage_split,300,RFLAG_TRIGGER_DAMAGE,RFLAG_BRAVE_PERCENT},
	}, // Calculator
	{
		{auto_potion,400,RFLAG_TRIGGER_DAMAGE,RFLAG_BRAVE_PERCENT},
	}, // Chemist
	{
		{a_save,550,RFLAG_TRIGGER_DAMAGE,RFLAG_BRAVE_PERCENT},
		{brave_up,500,RFLAG_TRIGGER_COUNTER,RFLAG_BRAVE_PERCENT},
	}, // Dancer
	{
		{counter_flood,300,RFLAG_TRIGGER_COUNTER,RFLAG_BRAVE_PERCENT},
	}, // Geomancer
	{
		{weapon_guard,200,RFLAG_TRIGGER_ALWAYS,RFLAG_WEAPONEVADE|RFLAG_BRAVE_PERCENT},
	}, // Knight
	{
		{dragon_spirit,200,RFLAG_TRIGGER_COUNTER,RFLAG_BRAVE_PERCENT},
	}, // Lancer
	{
		{finger_guard,300,RFLAG_TRIGGER_ALWAYS,RFLAG_HITMOD},
	}, // Mediator
	{}, // Mime
	{
		{hp_restore,500,RFLAG_TRIGGER_CRITICAL,RFLAG_BRAVE_PERCENT},
		{counter,300,RFLAG_TRIGGER_COUNTER,RFLAG_BRAVE_PERCENT},
		//{hamedo,1200,RFLAG_TRIGGER_PRECOUNTER,RFLAG_BRAVE_PERCENT},
	}, // Monk
	{
		{abandon,400,RFLAG_TRIGGER_ALWAYS,RFLAG_EVADEMOD},
		{sunken_state,900,RFLAG_TRIGGER_COUNTER,RFLAG_BRAVE_PERCENT},
	}, // Ninja
	{
		{absorb_used_mp,250,RFLAG_TRIGGER_ALWAYS,RFLAG_BRAVE_PERCENT},
	}, // Oracle
	{
		{regenerator,400,RFLAG_TRIGGER_DAMAGE,RFLAG_BRAVE_PERCENT},
	}, // Priest
	{
		{blade_grasp,700,RFLAG_TRIGGER_COUNTER,RFLAG_HITMOD|RFLAG_BRAVE_PERCENT},
		{meatbone_slash,200,RFLAG_TRIGGER_CRITICAL,RFLAG_BRAVE_PERCENT},
	}, // Priest
	{
		{counter_tackle,180,RFLAG_TRIGGER_COUNTER,RFLAG_BRAVE_PERCENT},
	}, // Squire
	{
		{mp_restore,400,RFLAG_TRIGGER_CRITICAL,RFLAG_BRAVE_PERCENT},
	}, // Summoner
	{
		{caution,200,RFLAG_TRIGGER_DAMAGE,RFLAG_BRAVE_PERCENT},
		//{gilgame_heart,200,RFLAG_TRIGGER_DAMAGE,RFLAG_BRAVE_PERCENT},
		//{catch,200,RFLAG_TRIGGER_DAMAGE,RFLAG_BRAVE_PERCENT},
	}, // Thief
	{
		{critical_quick,700,RFLAG_TRIGGER_CRITICAL,RFLAG_BRAVE_PERCENT},
		{mp_switch,400,RFLAG_TRIGGER_DAMAGE,RFLAG_BRAVE_PERCENT},
	}, // Time Mage
	{
		{counter_magic,800,RFLAG_TRIGGER_COUNTER,RFLAG_BRAVE_PERCENT},
	}, // Time Mage
};

static moveret mjump1(struct battle_char *bc, int dist){
	bc->jump+=1;
	return 1;
}

static moveret mjump2(struct battle_char *bc, int dist){
	bc->jump+=2;
	return 1;
}

static moveret mjump3(struct battle_char *bc, int dist){
	bc->jump+=3;
	return 1;
}

static moveret mmove1(struct battle_char *bc, int dist){
	bc->move+=1;
	return 1;
}

static moveret mmove2(struct battle_char *bc, int dist){
	bc->move+=2;
	return 1;
}

static moveret mmove3(struct battle_char *bc, int dist){
	bc->move+=3;
	return 1;
}

static moveret mget_exp(struct battle_char *bc, int dist){
	bc->ch->exp[bc->ch->primary]+=2; // What's a goot number?
	return 1;
}

static moveret mget_jp(struct battle_char *bc, int dist){
	bc->ch->jp+=2; // What's a goot number?
	return 1;
}

static moveret mfind_item(struct battle_char *bc, int dist){
// TODO: find items
	return 1;
}

static moveret mhp_up(struct battle_char *bc, int dist){
	if(dist>=0)
		restore_hp(bc,bc->hp_max/10);
	return 1;
}

static moveret mmp_up(struct battle_char *bc, int dist){
	if(dist>=0)
		restore_mp(bc,bc->mp_max/10);
	return 1;
}

static moveret mteleport(struct battle_char *bc, int dist){
	uint8_t success=100;

	if(dist>bc->move+9)
		return 0;
	
	if(get_random(0,100)<success-((dist-bc->move)*10))
		return 1;

	return 0;
}

const uint8_t num_move[]={0,1,2,2,1,2,2,10,1,0,0,1,1,1,0,1,1,0,2,2,0};
const struct movement_ability clmovement[NUM_CLASS][NUM_MOVEMENT_PER_ABILITY]={
	{}, // Generic
	{
		{mjump1,200,MFLAG_TRIGGER_START,0},
	}, // Archer
	{
		{NULL,1200,MFLAG_TRIGGER_SPECIAL,MFLAG_FLY},
		{mmove3,1000,MFLAG_TRIGGER_START,0},
	}, // Bard
	{
		{mget_exp,400,MFLAG_TRIGGER_MOVE,0},
		{mget_jp,400,MFLAG_TRIGGER_MOVE,0},
	}, // Calculator
	{
		{mfind_item,100,MFLAG_TRIGGER_MOVE,0},
	}, // Chemist
	{
		{NULL,1200,MFLAG_TRIGGER_SPECIAL,MFLAG_FLY},
		{mjump3,1000,MFLAG_TRIGGER_START,0},
	}, // Dancer
	{
		{NULL,220,MFLAG_TRIGGER_SPECIAL,MFLAG_WATERWALK},
		{NULL,150,MFLAG_TRIGGER_SPECIAL,MFLAG_LAVAWALK},
	}, // Geomancer
	{
	}, // Knight
	{
		{NULL,700,MFLAG_TRIGGER_SPECIAL,MFLAG_IGNOREHEIGHT},
	}, // Lancer
	{
	}, // Mediator
	{}, // Mime
	{
		{mhp_up,300,MFLAG_TRIGGER_MOVE,0},
	}, // Monk
	{
		{NULL,220,MFLAG_TRIGGER_SPECIAL,MFLAG_WATERWALK},
	}, // Ninja
	{
		{mmp_up,300,MFLAG_TRIGGER_MOVE,0},
	}, // Oracle
	{
	}, // Priest
	{
		{NULL,300,MFLAG_TRIGGER_SPECIAL,MFLAG_WATERWALK},
	}, // Samurai
	{
		{mmove1,200,MFLAG_TRIGGER_START,0},
	}, // Squire
	{
	}, // Summoner
	{
		{mjump2,480,MFLAG_TRIGGER_START,0},
		{mmove2,520,MFLAG_TRIGGER_START,0},
	}, // Thief
	{
		{NULL,540,MFLAG_TRIGGER_SPECIAL,MFLAG_FLOAT},
		{mteleport,600,MFLAG_TRIGGER_MOVE,MFLAG_FLY},
	}, // Time Mage
	{
	}, // Wizard
};
