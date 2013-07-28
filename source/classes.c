#include "player.h"
#include "classes.h"
#include "util.h"

const char class_unlocks[NUM_CLASS][NUM_CLASS+1]={
	{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0}, /* Archer */
	{1,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,4,0,0,0}, /* Bard */
	{3,0,0,0,0,0,0,0,0,0,0,0,0,0,3,4,0,0,0,3,4}, /* Calculator */
	{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, /* Chemist */
	{3,0,0,0,0,4,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0}, /* Dancer */
	{3,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,0,0}, /* Geomancer */
	{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0}, /* Knight */
	{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0}, /* Lancer */
	{3,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0}, /* Mediator */
	{3,0,0,0,8,0,4,0,4,4,0,0,0,0,0,0,8,4,0,0,0}, /* Mime */
	{3,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0}, /* Monk */
	{3,3,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,4,0,0}, /* Ninja */
	{3,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0}, /* Oracle */
	{3,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, /* Priest */
	{3,0,0,0,0,0,0,3,2,0,0,4,0,0,0,0,0,0,0,0,0}, /* Samurai */
	{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, /* Squire */
	{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0}, /* Summoner */
	{3,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, /* Thief */
	{3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2}, /* Time Mage */
	{3,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} /* Wizard */
};

/*
// hp/mp gain formula: r(10:17)/c[hm]p
// Speed point gained every speed levels
const struct class_stats[NUM_CLASS]={
	{3,3,10,4,12,18}, // Archer
	{3,3,5,10,14,18}, // Bard
	{3,3,5,8,8,30}, // Calculator
	{3,3,5,5,6,18}, // Chemist
	{3,4,5,8,15,18}, // Dancer
	{4,3,10,4,8,18}, // Geomancer
	{3,3,10,3,8,18}, // Knight
	{3,4,15,3,12,18}, // Lancer
	{3,4,5,5,12,18}, // Mediator
	{4,4,5,2,13,13}, // Mime
	{3,4,20,3,11,15}, // Monk
	{4,5,30,6,13,10}, // Ninja
	{3,3,5,7,8,18}, // Oracle
	{3,3,5,6,5,15}, // Priest
	{3,4,20,5,12,18}, // Samurai
	{4,3,5,5,8,18}, // Squire
	{3,3,5,7,5,23}, // Summoner
	{4,4,25,5,14,14}, // Thief
	{3,3,5,7,6,18}, // Time Mage
	{3,3,5,6,5,18}, // Wizard
};
*/

// hp/mp gain formula: r(10:17)/c[hm]p
// Speed point gained every speed levels
const struct class_stat class_stats[]={
	{3,3,10,{100,75,100,110,80},{11,16,100,45,50}}, // Archer
	{3,3,5,{55,50,100,30,115},{20,20,100,80,50}}, // Bard
	{3,3,5,{65,80,50,50,70},{14,10,100,70,50}}, // Calculator
	{3,3,5,{80,75,100,75,80},{12,16,100,75,50}}, // Chemist
	{3,4,5,{60,50,100,110,95},{20,20,100,50,50}}, // Dancer
	{4,3,10,{110,95,100,110,105},{10,11,100,45,50}}, // Geomancer
	{3,3,10,{120,80,100,120,80},{10,15,100,40,50}}, // Knight
	{3,4,15,{120,50,100,120,50},{10,15,100,40,50}}, // Lancer
	{3,4,5,{80,70,100,75,75},{11,18,100,55,50}}, // Mediator
	{4,4,5,{140,50,120,120,115},{6,30,100,35,40}}, // Mime
	{3,4,20,{135,80,110,129,80},{9,13,100,48,50}}, // Monk
	{4,5,30,{70,50,120,120,75},{12,13,80,43,50}}, // Ninja
	{3,3,5,{75,110,100,50,120},{12,10,100,60,50}}, // Oracle
	{3,3,5,{80,120,90,90.110},{10,10,100,50,50}}, // Priest
	{3,4,20,{75,75,100,128,90},{12,14,100,45,50}}, // Samurai
	{4,3,5,{100,75,100,90,80},{11,15,100,60,50}}, // Squire
	{3,3,5,{70,125,90,50,125},{13,8,100,70,50}}, // Summoner
	{4,4,25,{90,50,110,100,60},{11,16,90,50,50}}, // Thief
	{3,3,5,{75,120,100,50,130},{12,10,100,65,50}}, // Time Mage
	{3,3,5,{75,120,100,60,150},{12,9,100,60,50}}, // Wizard
};


void set_base_stats(struct character *ch){
	if(ch->gender==GENDER_MALE){
		ch->raw[STAT_HP]=get_random(491520,524287);
		ch->raw[STAT_MP]=get_random(491520,524287);
		ch->raw[STAT_PA]=81920;
		ch->raw[STAT_PA]=65536;
	}
	else{
		ch->raw[STAT_HP]=get_random(458752,491519);
		ch->raw[STAT_MP]=get_random(458752,491519);
		ch->raw[STAT_PA]=65536;
		ch->raw[STAT_PA]=81920;
	}

	ch->raw[STAT_SP]=98304;
}

void level_up(struct character *ch){
	int level;
	int i;

	for(i=level=0;i<NUM_CLASS;i++)
		level+=ch->level[i];


	for(i=0;i<NUM_STATS;i++)
		ch->raw[i]+=ch->raw[i]/(class_stats[(int)ch->primary].gainmod[i]+level);
}

void set_battle_stats(struct battle_char *bc){
	bc->hp=(bc->ch->raw[STAT_HP]*class_stats[(int)bc->ch->primary].basemod[STAT_HP])/BASE_STAT_DENOMINATOR;
	if(bc->hp<1)bc->hp=1;

	bc->mp=(bc->ch->raw[STAT_MP]*class_stats[(int)bc->ch->primary].basemod[STAT_MP])/BASE_STAT_DENOMINATOR;
	if(bc->mp<1)bc->mp=1;

	bc->pa=(bc->ch->raw[STAT_PA]*class_stats[(int)bc->ch->primary].basemod[STAT_PA])/BASE_STAT_DENOMINATOR;
	if(bc->pa<1)bc->pa=1;

	bc->ma=(bc->ch->raw[STAT_MA]*class_stats[(int)bc->ch->primary].basemod[STAT_MA])/BASE_STAT_DENOMINATOR;
	if(bc->ma<1)bc->ma=1;

	bc->speed=(bc->ch->raw[STAT_SP]*class_stats[(int)bc->ch->primary].basemod[STAT_SP])/BASE_STAT_DENOMINATOR;
	if(bc->speed<1)bc->speed=1;
}
