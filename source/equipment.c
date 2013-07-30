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

static int hands_dmg(const struct eq_item *item, struct battle_char *attacker, struct battle_char *defender){
	return attacker->pa*mod2(attacker,defender,attacker->pa*attacker->brave/100);
}

static int axe_dmg(const struct eq_item *item, struct battle_char *attacker, struct battle_char *defender){
	return item->wp*mod2(attacker,defender,get_random(1,attacker->pa));
}

static int bag_dmg(const struct eq_item *item, struct battle_char *attacker, struct battle_char *defender){
	return item->wp*mod2(attacker,defender,get_random(1,attacker->pa));
}

static int bow_dmg(const struct eq_item *item, struct battle_char *attacker, struct battle_char *defender){
	return item->wp*mod2(attacker,defender,(attacker->pa+attacker->speed)/2);
}

static int cloth_dmg(const struct eq_item *item, struct battle_char *attacker, struct battle_char *defender){
	return mod2(attacker,defender,(attacker->pa+attacker->ma)/2)*item->wp;
}

static int crossbow_dmg(const struct eq_item *item, struct battle_char *attacker, struct battle_char *defender){
	return item->wp*mod2(attacker,defender,attacker->pa);
}

static int dictionary_dmg(const struct eq_item *item, struct battle_char *attacker, struct battle_char *defender){
	return mod2(attacker,defender,(attacker->pa+attacker->ma)/2)*item->wp;
}

static int flail_dmg(const struct eq_item *item, struct battle_char *attacker, struct battle_char *defender){
	return item->wp*mod2(attacker,defender,get_random(1,attacker->pa));
}

static int gun_dmg(const struct eq_item *item, struct battle_char *attacker, struct battle_char *defender){
	return item->wp*mod2(attacker,defender,item->wp);
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
	return mod2(attacker,defender,(attacker->pa+attacker->ma)/2)*item->wp;
}

static int katana_dmg(const struct eq_item *item, struct battle_char *attacker, struct battle_char *defender){
	return mod2(attacker,defender,(attacker->brave/100)*attacker->pa)*item->wp;
}

static int knightsword_dmg(const struct eq_item *item, struct battle_char *attacker, struct battle_char *defender){
	return mod2(attacker,defender,(attacker->brave/100)*attacker->pa)*item->wp;
}

static int knife_dmg(const struct eq_item *item, struct battle_char *attacker, struct battle_char *defender){
	return mod2(attacker,defender,(attacker->pa+attacker->speed)/2)*item->wp;
}

static int ninjasword_dmg(const struct eq_item *item, struct battle_char *attacker, struct battle_char *defender){
	return mod2(attacker,defender,(attacker->pa+attacker->speed)/2)*item->wp;
}

static int rod_dmg(const struct eq_item *item, struct battle_char *attacker, struct battle_char *defender){
	return mod2(attacker,defender,attacker->pa)*item->wp;
}

static int spear_dmg(const struct eq_item *item, struct battle_char *attacker, struct battle_char *defender){
	return mod2(attacker,defender,attacker->pa)*item->wp;
}

static int staff_dmg(const struct eq_item *item, struct battle_char *attacker, struct battle_char *defender){
	return mod2(attacker,defender,attacker->ma)*item->wp;
}

static int stick_dmg(const struct eq_item *item, struct battle_char *attacker, struct battle_char *defender){
	return mod2(attacker,defender,attacker->ma)*item->wp;
}

static int sword_dmg(const struct eq_item *item, struct battle_char *attacker, struct battle_char *defender){
	return mod2(attacker,defender,attacker->pa)*item->wp;
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
	{{0,5,0,1,0,0,0,0}}, // Hands
};
const struct eq_item offhand[MAX_EQO_PER_TYPE]={
	{0,0,0,1,0,0,0,0}, // Naked
};
const struct eq_item body_armor[NUM_EQB_TYPES][MAX_EQB_PER_TYPE]={
	{{0,0,0,1,0,0,0,0}}, // Naked
};
const struct eq_item head_armor[NUM_EQH_TYPES][MAX_EQH_PER_TYPE]={
	{{0,0,0,1,0,0,0,0}}, // Naked
};
const struct eq_item misc_armor[NUM_EQM_TYPES][MAX_EQM_PER_TYPE]={
	{{0,0,0,1,0,0,0,0}}, // Naked
};
