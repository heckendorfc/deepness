#include <stdlib.h>
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
	{{0,{5},{0},1,0,0,0,NULL,NULL}}, // Hands
};


static void wear_ice_shield(struct battle_char *bc){
	bc->resist[ELEM_ICE]|=RESIST_ABSORB;
	bc->resist[ELEM_FIRE]|=RESIST_HALF;
	bc->resist[ELEM_LIGHTNING]|=RESIST_WEAK;
}

static void remove_ice_shield(struct battle_char *bc){
	bc->resist[ELEM_ICE]&=~RESIST_ABSORB;
	bc->resist[ELEM_FIRE]&=~RESIST_HALF;
	bc->resist[ELEM_LIGHTNING]&=~RESIST_WEAK;
}

static void wear_flame_shield(struct battle_char *bc){
	bc->resist[ELEM_FIRE]|=RESIST_ABSORB;
	bc->resist[ELEM_ICE]|=RESIST_HALF;
	bc->resist[ELEM_WATER]|=RESIST_WEAK;
}

static void remove_flame_shield(struct battle_char *bc){
	bc->resist[ELEM_FIRE]&=~RESIST_ABSORB;
	bc->resist[ELEM_ICE]&=~RESIST_HALF;
	bc->resist[ELEM_WATER]&=~RESIST_WEAK;
}

static void wear_aegis_shield(struct battle_char *bc){
	bc->ma+=1;
}

static void remove_aegis_shield(struct battle_char *bc){
	bc->ma-=1;
}

static void wear_kaiser_shield(struct battle_char *bc){
	bc->strengthen|=ELEM_FIRE|ELEM_LIGHTNING|ELEM_ICE;
}

static void remove_kaiser_shield(struct battle_char *bc){
	bc->strengthen&=~(ELEM_FIRE|ELEM_LIGHTNING|ELEM_ICE);
}

static void wear_venetian_shield(struct battle_char *bc){
	bc->resist[ELEM_FIRE]|=RESIST_HALF;
	bc->resist[ELEM_LIGHTNING]|=RESIST_HALF;
	bc->resist[ELEM_ICE]|=RESIST_HALF;
}

static void remove_venetian_shield(struct battle_char *bc){
	bc->resist[ELEM_FIRE]&=~RESIST_HALF;
	bc->resist[ELEM_LIGHTNING]&=~RESIST_HALF;
	bc->resist[ELEM_ICE]&=~RESIST_HALF;
}

const struct eq_item offhand[MAX_EQO_PER_TYPE]={
	{0,{0},{0},1,0,0,0,NULL,NULL}, // Naked
	{0,{10},{3},1,0,0,400,NULL,NULL}, // Escutcheon
	{0,{13},{3},3,0,0,700,NULL,NULL}, // Buckler
	{0,{16},{0},6,0,0,1200,NULL,NULL}, // Bronze Shield
	{0,{19},{0},9,0,0,1600,NULL,NULL}, // Round Shield
	{0,{22},{5},12,0,0,2500,NULL,NULL}, // Mythril Shield
	{0,{25},{0},15,0,0,3500,NULL,NULL}, // Gold Shield
	{0,{28},{0},22,0,0,6000,wear_ice_shield,remove_ice_shield}, // Ice Shield
	{0,{31},{0},22,0,0,6500,wear_flame_shield,remove_flame_shield}, // Flame Shield
	{0,{10},{50},22,0,0,10000,wear_aegis_shield,remove_aegis_shield}, // Aegis Shield
	{0,{34},{15},25,0,0,12000,NULL,NULL}, // Diamond Shield
	{0,{37},{10},29,0,0,16000,NULL,NULL}, // Platina Shield
	{0,{40},{15},36,0,0,21000,NULL,NULL}, // Crystal Shield
	{0,{43},{0},36,EQFLAG_NOFOE,0,10,NULL,NULL}, // Genji Shield
	{0,{46},{20},39,EQFLAG_NOFOE,0,10,wear_kaiser_shield,remove_kaiser_shield}, // Kaiser Plate
	{0,{50},{25},40,EQFLAG_NOFOE,0,10,wear_venetian_shield,remove_venetian_shield}, // Venetian Shield
	{0,{75},{50},50,EQFLAG_NOFOE,0,10,NULL,NULL}, // Escutcheon
};

static void wear_reflect_mail(struct battle_char *bc){
	bc->status[STATUS_REFLECT]=UNTIMED_STATUS;
}

static void remove_reflect_mail(struct battle_char *bc){
	bc->status[STATUS_REFLECT]=0;
}

static void wear_judo_outfit(struct battle_char *bc){
	bc->pa+=1;
}

static void remove_judo_outfit(struct battle_char *bc){
	bc->pa-=1;
}

static void wear_power_sleeve(struct battle_char *bc){
	bc->pa+=2;
}

static void remove_power_sleeve(struct battle_char *bc){
	bc->pa-=2;
}

static void wear_earth_clothes(struct battle_char *bc){
	bc->strengthen|=ELEM_EARTH;
	bc->resist[ELEM_EARTH]|=RESIST_ABSORB;
}

static void remove_earth_clothes(struct battle_char *bc){
	bc->strengthen&=~ELEM_EARTH;
	bc->resist[ELEM_EARTH]&=~RESIST_ABSORB;
}

static void wear_secret_clothes(struct battle_char *bc){
	bc->speed+=2;
	bc->status[STATUS_TRANSPARENT]=UNTIMED_STATUS;
}

static void remove_secret_clothes(struct battle_char *bc){
	bc->speed-=2;
	bc->status[STATUS_TRANSPARENT]=0;
}

static void wear_black_costume(struct battle_char *bc){
	bc->status[STATUS_STOP]=IMMUNE_TO_STATUS;
}

static void remove_black_costume(struct battle_char *bc){
	bc->status[STATUS_STOP]=0;
}

static void wear_rubber_costume(struct battle_char *bc){
	bc->resist[ELEM_LIGHTNING]|=RESIST_HALF;
}

static void remove_rubber_costume(struct battle_char *bc){
	bc->resist[ELEM_LIGHTNING]&=~RESIST_HALF;
}

static void wear_wizard_robe(struct battle_char *bc){
	bc->ma+=2;
}

static void remove_wizard_robe(struct battle_char *bc){
	bc->ma-=2;
}

static void wear_chameleon_robe(struct battle_char *bc){
	bc->resist[ELEM_HOLY]|=RESIST_ABSORB;
}

static void remove_chameleon_robe(struct battle_char *bc){
	bc->resist[ELEM_HOLY]&=~RESIST_ABSORB;
}

static void wear_white_robe(struct battle_char *bc){
	bc->resist[ELEM_FIRE]|=RESIST_HALF;
	bc->resist[ELEM_LIGHTNING]|=RESIST_HALF;
	bc->resist[ELEM_ICE]|=RESIST_HALF;
}

static void remove_white_robe(struct battle_char *bc){
	bc->resist[ELEM_FIRE]&=~RESIST_HALF;
	bc->resist[ELEM_LIGHTNING]&=~RESIST_HALF;
	bc->resist[ELEM_ICE]&=~RESIST_HALF;
}

static void wear_black_robe(struct battle_char *bc){
	bc->strengthen|=ELEM_FIRE|ELEM_LIGHTNING|ELEM_ICE;
}

static void remove_black_robe(struct battle_char *bc){
	bc->strengthen&=~(ELEM_FIRE|ELEM_LIGHTNING|ELEM_ICE);
}

static void wear_robe_of_lords(struct battle_char *bc){
	bc->status[STATUS_PROTECT]=bc->status[STATUS_SHELL]=UNTIMED_STATUS;
	bc->pa+=2;
	bc->ma+=1;
}

static void remove_robe_of_lords(struct battle_char *bc){
	bc->status[STATUS_PROTECT]=bc->status[STATUS_SHELL]=0;
	bc->pa-=2;
	bc->ma-=1;
}

const struct eq_item body_armor[NUM_EQB_TYPES][MAX_EQB_PER_TYPE]={
	{{0,{0},{0},1,0,0,0,NULL,NULL}}, // Naked
	{
		{0,{10},{0},1,0,0,200,NULL,NULL},
		{0,{20},{0},4,0,0,600,NULL,NULL},
		{0,{30},{0},6,0,0,800,NULL,NULL},
		{0,{40},{0},8,0,0,1300,NULL,NULL},
		{0,{50},{0},11,0,0,2000,NULL,NULL},
		{0,{60},{0},14,0,0,3000,NULL,NULL},
		{0,{70},{0},17,0,0,3600,NULL,NULL},
		{0,{80},{0},22,0,0,6000,NULL,NULL},
		{0,{90},{0},26,0,0,9000,NULL,NULL},
		{0,{100},{0},30,0,0,13000,NULL,NULL},
		{0,{110},{0},37,0,0,19000,NULL,NULL},
		{0,{150},{0},37,EQFLAG_NOFOE,0,10,NULL,NULL},
		{0,{130},{0},39,EQFLAG_NOFOE,0,18000,wear_reflect_mail,remove_reflect_mail},
		{0,{200},{0},45,EQFLAG_NOFOE,0,10,NULL,NULL},
	}, // Armor
	{
		{0,{5},{0},1,0,0,150,NULL,NULL},
		{0,{10},{0},3,0,0,300,NULL,NULL},
		{0,{18},{0},5,0,0,500,NULL,NULL},
		{0,{24},{0},7,0,0,900,NULL,NULL},
		{0,{30},{0},10,0,0,1500,NULL,NULL},
		{0,{36},{0},12,0,0,1600,NULL,NULL},
		{0,{42},{15},14,0,0,1900,NULL,NULL},
		{0,{50},{0},17,0,0,2500,NULL,NULL},
		{0,{60},{0},21,0,0,4000,wear_judo_outfit,remove_judo_outfit},
		{0,{70},{0},25,0,0,7000,wear_power_sleeve,remove_power_sleeve},
		{0,{85},{10},29,0,0,10000,wear_earth_clothes,remove_earth_clothes},
		{0,{20},{0},35,EQFLAG_NOFOE,0,10,wear_secret_clothes,remove_secret_clothes},
		{0,{100},{0},37,0,0,12000,wear_black_costume,remove_black_costume},
		{0,{150},{50},50,EQFLAG_NOFOE,0,48000,wear_rubber_costume,remove_rubber_costume},
	}, // Clothes
	{
		{0,{10},{10},3,0,0,1200,NULL,NULL},
		{0,{20},{16},5,0,0,2400,NULL,NULL},
		{0,{30},{22},11,0,0,4000,wear_wizard_robe,remove_wizard_robe},
		{0,{40},{28},16,EQFLAG_NOFOE,0,5000,wear_chameleon_robe,remove_chameleon_robe},
		{0,{50},{34},26,0,0,9000,wear_white_robe,remove_white_robe},
		{0,{60},{30},31,0,0,13000,wear_black_robe,remove_black_robe},
		{0,{75},{50},38,0,0,30000,NULL,NULL},
		{0,{100},{80},45,EQFLAG_NOFOE,0,10,wear_robe_of_lords,remove_robe_of_lords},
	}, // Robes
};

static void wear_grand_helm(struct battle_char *bc){
	bc->status[STATUS_SLEEP]=bc->status[STATUS_DARKNESS]=IMMUNE_TO_STATUS;
}

static void remove_grand_helm(struct battle_char *bc){
	bc->status[STATUS_SLEEP]=bc->status[STATUS_DARKNESS]=0;
}

static void wear_headgear(struct battle_char *bc){
	bc->pa+=1;
}

static void remove_headgear(struct battle_char *bc){
	bc->pa-=1;
}

static void wear_triangle_hat(struct battle_char *bc){
	bc->ma+=1;
}

static void remove_triangle_hat(struct battle_char *bc){
	bc->ma-=1;
}

static void wear_green_beret(struct battle_char *bc){
	bc->speed+=2;
}

static void remove_green_beret(struct battle_char *bc){
	bc->speed-=2;
}

static void wear_twist_headband(struct battle_char *bc){
	bc->pa+=2;
}

static void remove_twist_headband(struct battle_char *bc){
	bc->pa-=2;
}

static void wear_holy_miter(struct battle_char *bc){
	bc->ma+=1;
}

static void remove_holy_miter(struct battle_char *bc){
	bc->ma-=1;
}

static void wear_gold_hairpin(struct battle_char *bc){
	bc->status[STATUS_SILENCE]=IMMUNE_TO_STATUS;
}

static void remove_gold_hairpin(struct battle_char *bc){
	bc->status[STATUS_SILENCE]=0;
}

static void wear_flash_hat(struct battle_char *bc){
	bc->ma+=1;
	bc->speed+=1;
}

static void remove_flash_hat(struct battle_char *bc){
	bc->ma-=1;
	bc->speed-=1;
}

static void wear_thief_hat(struct battle_char *bc){
	bc->speed+=2;
	bc->status[STATUS_NOMOVE]=bc->status[STATUS_NOACT]=IMMUNE_TO_STATUS;
}

static void remove_thief_hat(struct battle_char *bc){
	bc->speed-=2;
	bc->status[STATUS_NOMOVE]=bc->status[STATUS_NOACT]=0;
}

static void wear_cachusha(struct battle_char *bc){
	bc->status[STATUS_PETRIFY]=bc->status[STATUS_CONFUSION]=bc->status[STATUS_STOP]=bc->status[STATUS_BERSERK]=bc->status[STATUS_CHARM]=bc->status[STATUS_SLEEP]=IMMUNE_TO_STATUS;
}

static void remove_cachusha(struct battle_char *bc){
	bc->status[STATUS_PETRIFY]=bc->status[STATUS_CONFUSION]=bc->status[STATUS_STOP]=bc->status[STATUS_BERSERK]=bc->status[STATUS_CHARM]=bc->status[STATUS_SLEEP]=0;
}

static void wear_barette(struct battle_char *bc){
	bc->status[STATUS_UNDEAD]=bc->status[STATUS_DARKNESS]=bc->status[STATUS_SILENCE]=bc->status[STATUS_POLYMORPH]=bc->status[STATUS_POISON]=bc->status[STATUS_SLOW]=bc->status[STATUS_NOACT]=bc->status[STATUS_NOMOVE]=bc->status[STATUS_DEATHSENTENCE]=IMMUNE_TO_STATUS;
}

static void remove_barette(struct battle_char *bc){
	bc->status[STATUS_UNDEAD]=bc->status[STATUS_DARKNESS]=bc->status[STATUS_SILENCE]=bc->status[STATUS_POLYMORPH]=bc->status[STATUS_POISON]=bc->status[STATUS_SLOW]=bc->status[STATUS_NOACT]=bc->status[STATUS_NOMOVE]=bc->status[STATUS_DEATHSENTENCE]=0;
}

static void wear_ribbon(struct battle_char *bc){
	bc->status[STATUS_UNDEAD]=bc->status[STATUS_PETRIFY]=bc->status[STATUS_DARKNESS]=bc->status[STATUS_CONFUSION]=bc->status[STATUS_SILENCE]=bc->status[STATUS_BERSERK]=bc->status[STATUS_POLYMORPH]=bc->status[STATUS_POISON]=bc->status[STATUS_SLOW]=bc->status[STATUS_STOP]=bc->status[STATUS_CHARM]=bc->status[STATUS_SLEEP]=bc->status[STATUS_NOMOVE]=bc->status[STATUS_NOACT]=bc->status[STATUS_DEATHSENTENCE]=IMMUNE_TO_STATUS;
}

static void remove_ribbon(struct battle_char *bc){
	bc->status[STATUS_UNDEAD]=bc->status[STATUS_PETRIFY]=bc->status[STATUS_DARKNESS]=bc->status[STATUS_CONFUSION]=bc->status[STATUS_SILENCE]=bc->status[STATUS_BERSERK]=bc->status[STATUS_POLYMORPH]=bc->status[STATUS_POISON]=bc->status[STATUS_SLOW]=bc->status[STATUS_STOP]=bc->status[STATUS_CHARM]=bc->status[STATUS_SLEEP]=bc->status[STATUS_NOMOVE]=bc->status[STATUS_NOACT]=bc->status[STATUS_DEATHSENTENCE]=0;
}

const struct eq_item head_armor[NUM_EQH_TYPES][MAX_EQH_PER_TYPE]={
	{{0,{0},{0},1,0,0,0,NULL,NULL}}, // Naked
	{
		{0,{10},{0},1,0,0,200,NULL,NULL},
		{0,{20},{0},4,0,0,500,NULL,NULL},
		{0,{30},{0},7,0,0,1000,NULL,NULL},
		{0,{40},{0},9,0,0,1500,NULL,NULL},
		{0,{50},{0},12,0,0,2100,NULL,NULL},
		{0,{60},{0},15,0,0,2800,NULL,NULL},
		{0,{70},{0},17,0,0,4000,NULL,NULL},
		{0,{80},{0},22,0,0,6000,NULL,NULL},
		{0,{90},{0},25,0,0,8000,NULL,NULL},
		{0,{100},{0},29,0,0,10000,NULL,NULL},
		{0,{120},{0},27,0,0,14000,NULL,NULL},
		{0,{130},{0},36,EQFLAG_NOFOE,0,10,NULL,NULL},
		{0,{150},{0},40,EQFLAG_NOFOE,0,10,wear_grand_helm,remove_grand_helm},
	}, // Helm
	{
		{0,{8},{0},1,0,0,150,NULL,NULL},
		{0,{16},{5},4,0,0,350,NULL,NULL},
		{0,{14},{8},7,0,0,800,NULL,NULL},
		{0,{32},{0},10,0,0,1200,wear_headgear,remove_headgear},
		{0,{40},{12},13,0,0,1800,wear_triangle_hat,remove_triangle_hat},
		{0,{48},{0},16,0,0,3000,wear_green_beret,remove_green_beret},
		{0,{56},{0},21,0,0,5000,wear_twist_headband,remove_twist_headband},
		{0,{64},{20},24,0,0,6000,wear_holy_miter,remove_holy_miter},
		{0,{72},{0},27,0,0,7000,NULL,NULL},
		{0,{80},{50},32,0,0,12000,wear_gold_hairpin,remove_gold_hairpin},
		{0,{88},{15},38,0,0,16000,wear_flash_hat,remove_flash_hat},
		{0,{100},{0},46,0,0,35000,wear_thief_hat,remove_thief_hat}
	}, // Hat
	{
		{0,{20},{0},50,0,0,20000,wear_cachusha,remove_cachusha},
		{0,{20},{0},51,0,0,20000,wear_barette,remove_barette},
		{0,{10},{0},52,0,0,60000,wear_ribbon,remove_ribbon}
	}, // Ribbon
};

static void wear_battle_boots(struct battle_char *bc){
	bc->move+=1;
}

static void remove_battle_boots(struct battle_char *bc){
	bc->move-=1;
}

static void wear_spike_boots(struct battle_char *bc){
	bc->jump+=1;
}

static void remove_spike_boots(struct battle_char *bc){
	bc->jump-=1;
}

static void wear_germinas_boots(struct battle_char *bc){
	bc->move+=1;
	bc->jump+=1;
}

static void remove_germinas_boots(struct battle_char *bc){
	bc->move-=1;
	bc->jump-=1;
}

static void wear_rubber_shoes(struct battle_char *bc){
	bc->status[STATUS_NOMOVE]=IMMUNE_TO_STATUS;
	bc->resist[ELEM_LIGHTNING]|=RESIST_HALF;
}

static void remove_rubber_shoes(struct battle_char *bc){
	bc->status[STATUS_NOMOVE]=0;
	bc->resist[ELEM_LIGHTNING]&=~RESIST_HALF;
}

static void wear_feather_boots(struct battle_char *bc){
	bc->status[STATUS_FLOAT]=UNTIMED_STATUS;
}

static void remove_feather_boots(struct battle_char *bc){
	bc->status[STATUS_FLOAT]=0;
}

static void wear_sprint_shoes(struct battle_char *bc){
	bc->speed+=1;
}

static void remove_sprint_shoes(struct battle_char *bc){
	bc->speed-=1;
}

static void wear_red_shoes(struct battle_char *bc){
	bc->move+=1;
	bc->ma+=1;
}

static void remove_red_shoes(struct battle_char *bc){
	bc->move-=1;
	bc->ma-=1;
}

static void wear_power_wrist(struct battle_char *bc){
	bc->pa+=1;
}

static void remove_power_wrist(struct battle_char *bc){
	bc->pa-=1;
}

static void wear_genji_gauntlet(struct battle_char *bc){
	bc->pa+=2;
	bc->ma+=2;
}

static void remove_genji_gauntlet(struct battle_char *bc){
	bc->pa-=2;
	bc->ma-=2;
}

static void wear_magic_gauntlet(struct battle_char *bc){
	bc->ma+=2;
}

static void remove_magic_gauntlet(struct battle_char *bc){
	bc->ma-=2;
}

static void wear_bracer(struct battle_char *bc){
	bc->pa+=3;
}

static void remove_bracer(struct battle_char *bc){
	bc->pa-=3;
}

static void wear_reflect_ring(struct battle_char *bc){
	bc->status[STATUS_REFLECT]=UNTIMED_STATUS;
}

static void remove_reflect_ring(struct battle_char *bc){
	bc->status[STATUS_REFLECT]=0;
}

static void wear_defense_ring(struct battle_char *bc){
	bc->status[STATUS_SLEEP]=bc->status[STATUS_DEATHSENTENCE]=IMMUNE_TO_STATUS;
}

static void remove_defense_ring(struct battle_char *bc){
	bc->status[STATUS_SLEEP]=bc->status[STATUS_DEATHSENTENCE]=0;
}

static void wear_magic_ring(struct battle_char *bc){
	bc->status[STATUS_SILENCE]=bc->status[STATUS_BERSERK]=IMMUNE_TO_STATUS;
}

static void remove_magic_ring(struct battle_char *bc){
	bc->status[STATUS_SILENCE]=bc->status[STATUS_BERSERK]=0;
}

static void wear_cursed_ring(struct battle_char *bc){
	bc->pa+=1;
	bc->ma+=1;
	bc->speed+=1;
	bc->status[STATUS_UNDEAD]=UNTIMED_STATUS;
}

static void remove_cursed_ring(struct battle_char *bc){
	bc->pa-=1;
	bc->ma-=1;
	bc->speed-=1;
	bc->status[STATUS_UNDEAD]=0;
}

static void wear_angel_ring(struct battle_char *bc){
	bc->status[STATUS_RERAISE]=UNTIMED_STATUS;
}

static void remove_angel_ring(struct battle_char *bc){
	bc->status[STATUS_RERAISE]=0;
}

static void wear_diamond_armlet(struct battle_char *bc){
	bc->pa+=1;
	bc->ma+=1;
	bc->status[STATUS_SLOW]=IMMUNE_TO_STATUS;
}

static void remove_diamond_armlet(struct battle_char *bc){
	bc->pa-=1;
	bc->ma-=1;
	bc->status[STATUS_SLOW]=0;
	
}

static void wear_jade_armlet(struct battle_char *bc){
	bc->status[STATUS_PETRIFY]=bc->status[STATUS_STOP]=IMMUNE_TO_STATUS;
}

static void remove_jade_armlet(struct battle_char *bc){
	bc->status[STATUS_PETRIFY]=bc->status[STATUS_STOP]=0;
}

static void wear_108_gems(struct battle_char *bc){
	bc->status[STATUS_UNDEAD]=bc->status[STATUS_POLYMORPH]=bc->status[STATUS_POISON]=IMMUNE_TO_STATUS;
	bc->strengthen=~0;
}

static void remove_108_gems(struct battle_char *bc){
	bc->status[STATUS_UNDEAD]=bc->status[STATUS_POLYMORPH]=bc->status[STATUS_POISON]=0;
	bc->strengthen=0;
}

static void wear_nkai_armlet(struct battle_char *bc){
	bc->status[STATUS_CONFUSION]=bc->status[STATUS_CHARM]=IMMUNE_TO_STATUS;
	bc->resist[ELEM_DARK]|=RESIST_HALF;
}

static void remove_nkai_armlet(struct battle_char *bc){
	bc->status[STATUS_CONFUSION]=bc->status[STATUS_CHARM]=0;
	bc->resist[ELEM_DARK]&=~RESIST_HALF;
}

static void wear_defense_armlet(struct battle_char *bc){
	bc->status[STATUS_NOACT]=bc->status[STATUS_NOMOVE]=IMMUNE_TO_STATUS;
}

static void remove_defense_armlet(struct battle_char *bc){
	bc->status[STATUS_NOACT]=bc->status[STATUS_NOMOVE]=0;
}

static void wear_wizard_mantle(struct battle_char *bc){
	bc->ma+=1;
}

static void remove_wizard_mantle(struct battle_char *bc){
	bc->ma-=1;
}

static void wear_vanish_mantle(struct battle_char *bc){
	bc->status[STATUS_TRANSPARENT]=UNTIMED_STATUS;
}

static void remove_vanish_mantle(struct battle_char *bc){
	bc->status[STATUS_TRANSPARENT]=0;
}

const struct eq_item misc_armor[NUM_EQM_TYPES][MAX_EQM_PER_TYPE]={
	{{0,{0},{0},1,0,0,0,NULL,NULL}}, // Naked
	{
		{0,{10},{10},10,0,0,300,NULL,NULL},
		{0,{15},{15},20,0,0,800,NULL,NULL},
		{0,{18},{18},30,0,0,2000,wear_wizard_mantle,remove_wizard_mantle},
		{0,{25},{25},40,0,0,8000,NULL,NULL},
		{0,{28},{28},49,0,0,15000,NULL,NULL},
		{0,{40},{30},50,0,0,20000,NULL,NULL},
		{0,{35},{0},5,EQFLAG_NOFOE,0,10,wear_vanish_mantle,remove_vanish_mantle},
	}, // Mantle
	{
		{0,{0},{0},20,0,0,5000,wear_power_wrist,remove_power_wrist},
		{0,{0},{0},50,EQFLAG_NOFOE,0,10,wear_genji_gauntlet,remove_genji_gauntlet},
		{0,{0},{0},35,0,0,20000,wear_magic_gauntlet,remove_magic_gauntlet},
		{0,{0},{0},60,0,0,50000,wear_bracer,remove_bracer},

		{0,{0},{0},50,EQFLAG_NOFOE,0,10000,wear_reflect_ring,remove_reflect_ring},
		{0,{0},{0},25,0,0,5000,wear_defense_ring,remove_defense_ring},
		{0,{0},{0},25,0,0,10000,wear_magic_ring,remove_magic_ring},
		{0,{0},{0},5,EQFLAG_NOFOE,0,10,wear_cursed_ring,remove_cursed_ring},
		{0,{0},{0},25,EQFLAG_NOFOE,0,20000,wear_angel_ring,remove_angel_ring},

		{0,{0},{0},26,0,0,5000,wear_diamond_armlet,remove_diamond_armlet},
		{0,{0},{0},25,0,0,10000,wear_jade_armlet,remove_jade_armlet},
		{0,{0},{0},25,0,0,15000,wear_108_gems,remove_108_gems},
		{0,{0},{0},25,0,0,10000,wear_nkai_armlet,remove_nkai_armlet},
		{0,{0},{0},25,0,0,7000,wear_defense_armlet,remove_defense_armlet},
	}, // Armwear
	{
		{0,{0},{0},10,0,0,1000,wear_battle_boots,remove_battle_boots},
		{0,{0},{0},20,0,0,1200,wear_spike_boots,remove_spike_boots},
		{0,{0},{0},30,0,0,5000,wear_germinas_boots,remove_germinas_boots},
		{0,{0},{0},5,EQFLAG_NOFOE,0,1500,wear_rubber_shoes,remove_rubber_shoes},
		{0,{0},{0},50,EQFLAG_NOFOE,0,2500,wear_feather_boots,remove_feather_boots},
		{0,{0},{0},30,EQFLAG_NOFOE,0,7000,wear_sprint_shoes,remove_sprint_shoes},
		{0,{0},{0},40,EQFLAG_NOFOE,0,10000,wear_red_shoes,remove_red_shoes},
	}, // Foot
};
