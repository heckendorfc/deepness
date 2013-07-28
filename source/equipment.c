#include "battle.h"
#include "equipment.h"
#include "classes.h"
#include "util.h"
#include "player.h"
#include "ability.h"

/* Classes that can equip */
const int cleqw[] = {
	CLBIT(CL_SQUIRE) | CLBIT(CL_GEOMANCER),
	~0,
	CLBIT(CL_ARCHER),
	CLBIT(CL_DANCER),
	CLBIT(CL_ARCHER),
	CLBIT(CL_CALCULATOR) | CLBIT(CL_ORACLE),
	CLBIT(CL_NINJA) | CLBIT(CL_SQUIRE),
	CLBIT(CL_CHEMIST) | CLBIT(CL_MEDIATOR),
	CLBIT(CL_CHEMIST) | CLBIT(CL_MEDIATOR),
	CLBIT(CL_BARD),
	CLBIT(CL_SAMURAI),
	CLBIT(CL_KNIGHT),
	CLBIT(CL_CHEMIST) | CLBIT(CL_DANCER) | CLBIT(CL_MEDIATOR) | CLBIT(CL_NINJA) | CLBIT(CL_SQUIRE) | CLBIT(CL_THIEF),
	CLBIT(CL_NINJA),
	CLBIT(CL_ORACLE) | CLBIT(CL_SUMMONER) | CLBIT(CL_WIZARD),
	CLBIT(CL_LANCER),
	CLBIT(CL_PRIEST) | CLBIT(CL_SUMMONER) | CLBIT(CL_TIME_MAGE),
	CLBIT(CL_CALCULATOR) | CLBIT(CL_ORACLE),
	CLBIT(CL_GEOMANCER) | CLBIT(CL_KNIGHT) | CLBIT(CL_SQUIRE)
};

/* Armor */
const int cleqo[] = {
	CLBIT(CL_ARCHER) | CLBIT(CL_GEOMANCER) | CLBIT(CL_KNIGHT) | CLBIT(CL_LANCER)
};

const int cleqh[] = {
	CLBIT(CL_ARCHER) | CLBIT(CL_BARD) | CLBIT(CL_CALCULATOR) | CLBIT(CL_CHEMIST) | CLBIT(CL_DANCER) | CLBIT(CL_GEOMANCER) | 
		CLBIT(CL_MEDIATOR) | CLBIT(CL_NINJA) | CLBIT(CL_ORACLE) | CLBIT(CL_PRIEST) | CLBIT(CL_SQUIRE) | CLBIT(CL_THIEF) | 
		CLBIT(CL_TIME_MAGE) | CLBIT(CL_WIZARD),
	~0,
	CLBIT(CL_KNIGHT) | CLBIT(CL_LANCER) | CLBIT(CL_SAMURAI) | CLBIT(CL_KNIGHT),
};

const int cleqb[] = {
	CLBIT(CL_ARCHER) | CLBIT(CL_BARD) | CLBIT(CL_CALCULATOR) | CLBIT(CL_CHEMIST) | CLBIT(CL_DANCER) | CLBIT(CL_GEOMANCER) | 
		CLBIT(CL_MEDIATOR) | CLBIT(CL_MONK) | CLBIT(CL_NINJA) | CLBIT(CL_ORACLE) | CLBIT(CL_PRIEST) | CLBIT(CL_SQUIRE) | 
		CLBIT(CL_SUMMONER) | CLBIT(CL_THIEF) | CLBIT(CL_TIME_MAGE) | CLBIT(CL_WIZARD),
	CLBIT(CL_KNIGHT) | CLBIT(CL_LANCER) | CLBIT(CL_SAMURAI),
	CLBIT(CL_CALCULATOR) | CLBIT(CL_GEOMANCER) | CLBIT(CL_KNIGHT) | CLBIT(CL_LANCER) | CLBIT(CL_MEDIATOR) | CLBIT(CL_ORACLE) | 
		CLBIT(CL_PRIEST) | CLBIT(CL_SUMMONER) | CLBIT(CL_TIME_MAGE) | CLBIT(CL_WIZARD),
};

/* Misc */
const int cleqm []={
	CLBIT(CL_ARCHER) | CLBIT(CL_BARD) | CLBIT(CL_CALCULATOR) | CLBIT(CL_CHEMIST) | CLBIT(CL_DANCER) | CLBIT(CL_GEOMANCER) | 
		CLBIT(CL_KNIGHT) | CLBIT(CL_LANCER) | CLBIT(CL_MEDIATOR) | CLBIT(CL_MONK) | CLBIT(CL_NINJA) | CLBIT(CL_ORACLE) | 
		CLBIT(CL_PRIEST) | CLBIT(CL_SAMURAI) | CLBIT(CL_SQUIRE) | CLBIT(CL_SUMMONER) | CLBIT(CL_THIEF) | CLBIT(CL_TIME_MAGE) |
		CLBIT(CL_WIZARD),
	CLBIT(CL_ARCHER) | CLBIT(CL_BARD) | CLBIT(CL_CALCULATOR) | CLBIT(CL_CHEMIST) | CLBIT(CL_DANCER) | CLBIT(CL_GEOMANCER) | 
		CLBIT(CL_KNIGHT) | CLBIT(CL_LANCER) | CLBIT(CL_MEDIATOR) | CLBIT(CL_MONK) | CLBIT(CL_NINJA) | CLBIT(CL_ORACLE) | 
		CLBIT(CL_PRIEST) | CLBIT(CL_SAMURAI) | CLBIT(CL_SQUIRE) | CLBIT(CL_SUMMONER) | CLBIT(CL_THIEF) | CLBIT(CL_TIME_MAGE) |
		CLBIT(CL_WIZARD),
	CLBIT(CL_ARCHER) | CLBIT(CL_BARD) | CLBIT(CL_CALCULATOR) | CLBIT(CL_CHEMIST) | CLBIT(CL_DANCER) | CLBIT(CL_GEOMANCER) | 
		CLBIT(CL_KNIGHT) | CLBIT(CL_LANCER) | CLBIT(CL_MEDIATOR) | CLBIT(CL_MONK) | CLBIT(CL_NINJA) | CLBIT(CL_ORACLE) | 
		CLBIT(CL_PRIEST) | CLBIT(CL_SAMURAI) | CLBIT(CL_SQUIRE) | CLBIT(CL_SUMMONER) | CLBIT(CL_THIEF) | CLBIT(CL_TIME_MAGE) |
		CLBIT(CL_WIZARD),
};

static int is_critical_hit(struct battle_char *attacker){
	if(get_random(0,20)==0)
		return 1;
	return 0;
}

static int get_dmg_mod(int xa, struct battle_char *attacker, struct battle_char *defender){
	if(is_critical_hit(attacker))
		xa+=get_random(1,xa)-1;
// TODO: add Strengthen bonus here
	if(attacker->status[STATUS_ATTACKUP])
		xa=xa*4/3;
	if(attacker->ch->eq[EQ_WEAPON]==0 && attacker->ch->support==SFLAG_MARTIAL_ARTS)
		xa=xa*3/2;
	if(attacker->status[STATUS_BERSERK])
		xa=xa*3/2;
	if(defender->status[STATUS_DEFENSEUP])
		xa=xa*2/3;
	if(defender->status[STATUS_PROTECT])
		xa=xa*2/3;
	if(defender->status[STATUS_CHARGING])
		xa=xa*3/2;
	if(defender->status[STATUS_SLEEPING])
		xa=xa*3/2;
	if(defender->status[STATUS_POLYMORPH])
		xa=xa*3/2;
// TODO: add zodiac modifiers here

	return xa;
}

static int hands_dmg(const struct eq_item *item, struct battle_char *attacker, struct battle_char *defender){
	return attacker->pa*get_dmg_mod(attacker->pa*attacker->brave/100,attacker,defender);
}

static int axe_dmg(const struct eq_item *item, struct battle_char *attacker, struct battle_char *defender){
	return item->wp*get_dmg_mod(get_random(1,attacker->pa),attacker,defender);
}

static int bag_dmg(const struct eq_item *item, struct battle_char *attacker, struct battle_char *defender){
	return item->wp*get_dmg_mod(get_random(1,attacker->pa),attacker,defender);
}

static int bow_dmg(const struct eq_item *item, struct battle_char *attacker, struct battle_char *defender){
	return item->wp*get_dmg_mod((attacker->pa+attacker->speed)/2,attacker,defender);
}

static int cloth_dmg(const struct eq_item *item, struct battle_char *attacker, struct battle_char *defender){
	return get_dmg_mod((attacker->pa+attacker->ma)/2,attacker,defender)*item->wp;
}

static int crossbow_dmg(const struct eq_item *item, struct battle_char *attacker, struct battle_char *defender){
	return item->wp*get_dmg_mod(attacker->pa,attacker,defender);
}

static int dictionary_dmg(const struct eq_item *item, struct battle_char *attacker, struct battle_char *defender){
	return get_dmg_mod((attacker->pa+attacker->ma)/2,attacker,defender)*item->wp;
}

static int flail_dmg(const struct eq_item *item, struct battle_char *attacker, struct battle_char *defender){
	return item->wp*get_dmg_mod(get_random(1,attacker->pa),attacker,defender);
}

static int gun_dmg(const struct eq_item *item, struct battle_char *attacker, struct battle_char *defender){
	return item->wp*get_dmg_mod(item->wp,attacker,defender);
}

static int spellgun_dmg(const struct eq_item *item, struct battle_char *attacker, struct battle_char *defender){
	int r=get_random(0,10);
	int mod;

	if(r<6)
		mod=14;
	else if(r<9)
		mod=18;
	else
		mod=24;

	return attacker->faith*defender->faith*item->wp*mod/10000;
}

static int harp_dmg(const struct eq_item *item, struct battle_char *attacker, struct battle_char *defender){
	return get_dmg_mod((attacker->pa+attacker->ma)/2,attacker,defender)*item->wp;
}

static int katana_dmg(const struct eq_item *item, struct battle_char *attacker, struct battle_char *defender){
	return get_dmg_mod((attacker->brave/100)*attacker->pa,attacker,defender)*item->wp;
}

static int knightsword_dmg(const struct eq_item *item, struct battle_char *attacker, struct battle_char *defender){
	return get_dmg_mod((attacker->brave/100)*attacker->pa,attacker,defender)*item->wp;
}

static int knife_dmg(const struct eq_item *item, struct battle_char *attacker, struct battle_char *defender){
	return get_dmg_mod((attacker->pa+attacker->speed)/2,attacker,defender)*item->wp;
}

static int ninjasword_dmg(const struct eq_item *item, struct battle_char *attacker, struct battle_char *defender){
	return get_dmg_mod((attacker->pa+attacker->speed)/2,attacker,defender)*item->wp;
}

static int rod_dmg(const struct eq_item *item, struct battle_char *attacker, struct battle_char *defender){
	return get_dmg_mod(attacker->pa,attacker,defender)*item->wp;
}

static int spear_dmg(const struct eq_item *item, struct battle_char *attacker, struct battle_char *defender){
	return get_dmg_mod(attacker->pa,attacker,defender)*item->wp;
}

static int staff_dmg(const struct eq_item *item, struct battle_char *attacker, struct battle_char *defender){
	return get_dmg_mod(attacker->ma,attacker,defender)*item->wp;
}

static int stick_dmg(const struct eq_item *item, struct battle_char *attacker, struct battle_char *defender){
	return get_dmg_mod(attacker->ma,attacker,defender)*item->wp;
}

static int sword_dmg(const struct eq_item *item, struct battle_char *attacker, struct battle_char *defender){
	return get_dmg_mod(attacker->pa,attacker,defender)*item->wp;
}

const weapon_damagef weapon_damage[]={
	hands_dmg,
	axe_dmg,
	bag_dmg,
	bow_dmg,
	cloth_dmg,
	crossbow_dmg,
	dictionary_dmg,
	flail_dmg,
	gun_dmg,
	spellgun_dmg,
	harp_dmg,
	katana_dmg,
	knightsword_dmg,
	knife_dmg,
	ninjasword_dmg,
	rod_dmg,
	spear_dmg,
	staff_dmg,
	stick_dmg,
	sword_dmg,
};

const struct eq_item weapons[NUM_EQW_TYPES][MAX_EQW_PER_TYPE]={
	{{0,5,1,0,0,0,0}}, // Hands
};
