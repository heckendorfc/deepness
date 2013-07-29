#include "battle.h"
#include "ability.h"
#include "player.h"
#include "equipment.h"

static void cure(struct battle_char *origin, struct battle_char **targets, int num){
}

static void cure2(struct battle_char *origin, struct battle_char **targets, int num){
}

static void cure3(struct battle_char *origin, struct battle_char **targets, int num){
}

static void cure4(struct battle_char *origin, struct battle_char **targets, int num){
}

static void raise(struct battle_char *origin, struct battle_char **targets, int num){
}

static void raise2(struct battle_char *origin, struct battle_char **targets, int num){
}

static void reraise(struct battle_char *origin, struct battle_char **targets, int num){
}

static void regen(struct battle_char *origin, struct battle_char **targets, int num){
}

static void protect(struct battle_char *origin, struct battle_char **targets, int num){
}

static void protect2(struct battle_char *origin, struct battle_char **targets, int num){
}

static void shell(struct battle_char *origin, struct battle_char **targets, int num){
}

static void shell2(struct battle_char *origin, struct battle_char **targets, int num){
}

static void wall(struct battle_char *origin, struct battle_char **targets, int num){
}

static void esuna(struct battle_char *origin, struct battle_char **targets, int num){
}

static void holy(struct battle_char *origin, struct battle_char **targets, int num){
}

static void fire(struct battle_char *origin, struct battle_char **targets, int num){
}

static void fire2(struct battle_char *origin, struct battle_char **targets, int num){
}

static void fire3(struct battle_char *origin, struct battle_char **targets, int num){
}

static void fire4(struct battle_char *origin, struct battle_char **targets, int num){
}

static void bolt(struct battle_char *origin, struct battle_char **targets, int num){
}

static void bolt2(struct battle_char *origin, struct battle_char **targets, int num){
}

static void bolt3(struct battle_char *origin, struct battle_char **targets, int num){
}

static void bolt4(struct battle_char *origin, struct battle_char **targets, int num){
}

static void ice(struct battle_char *origin, struct battle_char **targets, int num){
}

static void ice2(struct battle_char *origin, struct battle_char **targets, int num){
}

static void ice3(struct battle_char *origin, struct battle_char **targets, int num){
}

static void ice4(struct battle_char *origin, struct battle_char **targets, int num){
}

static void poison(struct battle_char *origin, struct battle_char **targets, int num){
}

static void frog(struct battle_char *origin, struct battle_char **targets, int num){
}

static void death(struct battle_char *origin, struct battle_char **targets, int num){
}

static void flare(struct battle_char *origin, struct battle_char **targets, int num){
}

static void haste(struct battle_char *origin, struct battle_char **targets, int num){
}

static void haste2(struct battle_char *origin, struct battle_char **targets, int num){
}

static void slow(struct battle_char *origin, struct battle_char **targets, int num){
}

static void slow2(struct battle_char *origin, struct battle_char **targets, int num){
}

static void stop(struct battle_char *origin, struct battle_char **targets, int num){
}

static void dont_move(struct battle_char *origin, struct battle_char **targets, int num){
}

static void magic_float(struct battle_char *origin, struct battle_char **targets, int num){
}

static void reflect(struct battle_char *origin, struct battle_char **targets, int num){
}

static void quick(struct battle_char *origin, struct battle_char **targets, int num){
}

static void demi(struct battle_char *origin, struct battle_char **targets, int num){
}

static void demi2(struct battle_char *origin, struct battle_char **targets, int num){
}

static void meteor(struct battle_char *origin, struct battle_char **targets, int num){
}

static void blind_yin(struct battle_char *origin, struct battle_char **targets, int num){
}

static void spell_absorb(struct battle_char *origin, struct battle_char **targets, int num){
}

static void life_drain(struct battle_char *origin, struct battle_char **targets, int num){
}

static void pray_faith(struct battle_char *origin, struct battle_char **targets, int num){
}

static void doubt_faith(struct battle_char *origin, struct battle_char **targets, int num){
}

static void zombie_yin(struct battle_char *origin, struct battle_char **targets, int num){
}

static void silence_song(struct battle_char *origin, struct battle_char **targets, int num){
}

static void blind_rage(struct battle_char *origin, struct battle_char **targets, int num){
}

static void foxbird(struct battle_char *origin, struct battle_char **targets, int num){
}

static void confusion_song(struct battle_char *origin, struct battle_char **targets, int num){
}

static void dispel_magic(struct battle_char *origin, struct battle_char **targets, int num){
}

static void paralyze(struct battle_char *origin, struct battle_char **targets, int num){
}

static void magic_sleep_yin(struct battle_char *origin, struct battle_char **targets, int num){
}

static void petrify(struct battle_char *origin, struct battle_char **targets, int num){
}

static void moogle(struct battle_char *origin, struct battle_char **targets, int num){
}

static void shiva(struct battle_char *origin, struct battle_char **targets, int num){
}

static void ramuh(struct battle_char *origin, struct battle_char **targets, int num){
}

static void ifrit(struct battle_char *origin, struct battle_char **targets, int num){
}

static void titan(struct battle_char *origin, struct battle_char **targets, int num){
}

static void golem(struct battle_char *origin, struct battle_char **targets, int num){
}

static void carbunkle(struct battle_char *origin, struct battle_char **targets, int num){
}

static void bahamut(struct battle_char *origin, struct battle_char **targets, int num){
}

static void odin(struct battle_char *origin, struct battle_char **targets, int num){
}

static void leviathan(struct battle_char *origin, struct battle_char **targets, int num){
}

static void salamander(struct battle_char *origin, struct battle_char **targets, int num){
}

static void silf(struct battle_char *origin, struct battle_char **targets, int num){
}

static void fairy(struct battle_char *origin, struct battle_char **targets, int num){
}

static void lich(struct battle_char *origin, struct battle_char **targets, int num){
}

static void cyclops(struct battle_char *origin, struct battle_char **targets, int num){
}

static void zodiac(struct battle_char *origin, struct battle_char **targets, int num){
}

static void asura_draw(struct battle_char *origin, struct battle_char **targets, int num){
}

static void koutetsu(struct battle_char *origin, struct battle_char **targets, int num){
}

static void bizen_boat(struct battle_char *origin, struct battle_char **targets, int num){
}

static void murasame(struct battle_char *origin, struct battle_char **targets, int num){
}

static void heavens_cloud(struct battle_char *origin, struct battle_char **targets, int num){
}

static void kiyomori(struct battle_char *origin, struct battle_char **targets, int num){
}

static void muramasa(struct battle_char *origin, struct battle_char **targets, int num){
}

static void kikuichimoji(struct battle_char *origin, struct battle_char **targets, int num){
}

static void masamune(struct battle_char *origin, struct battle_char **targets, int num){
}

static void chirijiraden(struct battle_char *origin, struct battle_char **targets, int num){
}

static void angel_song(struct battle_char *origin, struct battle_char **targets, int num){
}

static void life_song(struct battle_char *origin, struct battle_char **targets, int num){
}

static void cheer_song(struct battle_char *origin, struct battle_char **targets, int num){
}

static void battle_song(struct battle_char *origin, struct battle_char **targets, int num){
}

static void magic_song(struct battle_char *origin, struct battle_char **targets, int num){
}

static void nameless_song(struct battle_char *origin, struct battle_char **targets, int num){
}

static void last_song(struct battle_char *origin, struct battle_char **targets, int num){
}

static void witch_hunt(struct battle_char *origin, struct battle_char **targets, int num){
}

static void wiznaibus(struct battle_char *origin, struct battle_char **targets, int num){
}

static void slow_dance(struct battle_char *origin, struct battle_char **targets, int num){
}

static void polka_polka(struct battle_char *origin, struct battle_char **targets, int num){
}

static void disillusion(struct battle_char *origin, struct battle_char **targets, int num){
}

static void nameless_dance(struct battle_char *origin, struct battle_char **targets, int num){
}

static void last_dance(struct battle_char *origin, struct battle_char **targets, int num){
}

static void spin_fist(struct battle_char *origin, struct battle_char **targets, int num){
}

static void repeating_fist(struct battle_char *origin, struct battle_char **targets, int num){
}

static void wave_fist(struct battle_char *origin, struct battle_char **targets, int num){
}

static void earth_slash(struct battle_char *origin, struct battle_char **targets, int num){
}

static void secret_fist(struct battle_char *origin, struct battle_char **targets, int num){
}

static void stigma_magic(struct battle_char *origin, struct battle_char **targets, int num){
}

static void chakra(struct battle_char *origin, struct battle_char **targets, int num){
}

static void revive(struct battle_char *origin, struct battle_char **targets, int num){
}

static void gil_taking(struct battle_char *origin, struct battle_char **targets, int num){
}

static void steal_heart(struct battle_char *origin, struct battle_char **targets, int num){
}

static void steal_helmet(struct battle_char *origin, struct battle_char **targets, int num){
}

static void steal_armor(struct battle_char *origin, struct battle_char **targets, int num){
}

static void steal_shield(struct battle_char *origin, struct battle_char **targets, int num){
}

static void steal_weapon(struct battle_char *origin, struct battle_char **targets, int num){
}

static void steal_accessry(struct battle_char *origin, struct battle_char **targets, int num){
}

static void steal_exp(struct battle_char *origin, struct battle_char **targets, int num){
}

static void invitation(struct battle_char *origin, struct battle_char **targets, int num){
}

static void persuade(struct battle_char *origin, struct battle_char **targets, int num){
}

static void praise(struct battle_char *origin, struct battle_char **targets, int num){
}

static void threaten(struct battle_char *origin, struct battle_char **targets, int num){
}

static void preach(struct battle_char *origin, struct battle_char **targets, int num){
}

static void solution(struct battle_char *origin, struct battle_char **targets, int num){
}

static void death_sentence(struct battle_char *origin, struct battle_char **targets, int num){
}

static void negotiate(struct battle_char *origin, struct battle_char **targets, int num){
}

static void insult(struct battle_char *origin, struct battle_char **targets, int num){
}

static void mimic_daravon(struct battle_char *origin, struct battle_char **targets, int num){
}

static void pitfall(struct battle_char *origin, struct battle_char **targets, int num){
}

static void water_ball(struct battle_char *origin, struct battle_char **targets, int num){
}

static void hell_ivy(struct battle_char *origin, struct battle_char **targets, int num){
}

static void carve_model(struct battle_char *origin, struct battle_char **targets, int num){
}

static void local_quake(struct battle_char *origin, struct battle_char **targets, int num){
}

static void kamaitachi(struct battle_char *origin, struct battle_char **targets, int num){
}

static void demon_fire(struct battle_char *origin, struct battle_char **targets, int num){
}

static void quicksand(struct battle_char *origin, struct battle_char **targets, int num){
}

static void blizzard(struct battle_char *origin, struct battle_char **targets, int num){
}

static void gusty_wind(struct battle_char *origin, struct battle_char **targets, int num){
}

static void lava_ball(struct battle_char *origin, struct battle_char **targets, int num){
}

static void head_break(struct battle_char *origin, struct battle_char **targets, int num){
}

static void armor_break(struct battle_char *origin, struct battle_char **targets, int num){
}

static void shield_break(struct battle_char *origin, struct battle_char **targets, int num){
}

static void weapon_break(struct battle_char *origin, struct battle_char **targets, int num){
}

static void magic_break(struct battle_char *origin, struct battle_char **targets, int num){
}

static void speed_break(struct battle_char *origin, struct battle_char **targets, int num){
}

static void power_break(struct battle_char *origin, struct battle_char **targets, int num){
}

static void mind_break(struct battle_char *origin, struct battle_char **targets, int num){
}

static void accumulate(struct battle_char *origin, struct battle_char **targets, int num){
}

static void dash(struct battle_char *origin, struct battle_char **targets, int num){
}

static void throw_stone(struct battle_char *origin, struct battle_char **targets, int num){
}

static void heal(struct battle_char *origin, struct battle_char **targets, int num){
}

static void yell(struct battle_char *origin, struct battle_char **targets, int num){
}

static void cheer_up(struct battle_char *origin, struct battle_char **targets, int num){
}

static void wish(struct battle_char *origin, struct battle_char **targets, int num){
}

static void scream(struct battle_char *origin, struct battle_char **targets, int num){
}

static void ultima_guts(struct battle_char *origin, struct battle_char **targets, int num){
}

static void stasis_sword(struct battle_char *origin, struct battle_char **targets, int num){
}

static void split_punch(struct battle_char *origin, struct battle_char **targets, int num){
}

static void crush_punch(struct battle_char *origin, struct battle_char **targets, int num){
}

static void lightning_stab(struct battle_char *origin, struct battle_char **targets, int num){
}

static void holy_explosion(struct battle_char *origin, struct battle_char **targets, int num){
}

static void shellbust_stab(struct battle_char *origin, struct battle_char **targets, int num){
}

static void blastar_punch(struct battle_char *origin, struct battle_char **targets, int num){
}

static void hellcry_punch(struct battle_char *origin, struct battle_char **targets, int num){
}

static void icewolf_bite(struct battle_char *origin, struct battle_char **targets, int num){
}

static void dark_sword(struct battle_char *origin, struct battle_char **targets, int num){
}

static void night_sword(struct battle_char *origin, struct battle_char **targets, int num){
}

static void dark_holy(struct battle_char *origin, struct battle_char **targets, int num){
}

static void deathspell2(struct battle_char *origin, struct battle_char **targets, int num){
}

static void galaxy_stop(struct battle_char *origin, struct battle_char **targets, int num){
}

static void heaven_thunder(struct battle_char *origin, struct battle_char **targets, int num){
}

static void asura_truth(struct battle_char *origin, struct battle_char **targets, int num){
}

static void diamond_sword(struct battle_char *origin, struct battle_char **targets, int num){
}

static void hydragon_pit(struct battle_char *origin, struct battle_char **targets, int num){
}

static void space_storage(struct battle_char *origin, struct battle_char **targets, int num){
}

static void sky_demon(struct battle_char *origin, struct battle_char **targets, int num){
}

static void heaven_bltback(struct battle_char *origin, struct battle_char **targets, int num){
}

static void asura_back(struct battle_char *origin, struct battle_char **targets, int num){
}

static void dia_swd_back(struct battle_char *origin, struct battle_char **targets, int num){
}

static void dragon_pit_back(struct battle_char *origin, struct battle_char **targets, int num){
}

static void space_str_back(struct battle_char *origin, struct battle_char **targets, int num){
}

static void sky_demon_back(struct battle_char *origin, struct battle_char **targets, int num){
}

static void seal(struct battle_char *origin, struct battle_char **targets, int num){
}

static void shadow_stitch(struct battle_char *origin, struct battle_char **targets, int num){
}

static void stop_bracelet(struct battle_char *origin, struct battle_char **targets, int num){
}

static void shock(struct battle_char *origin, struct battle_char **targets, int num){
}

static void difference(struct battle_char *origin, struct battle_char **targets, int num){
}

static void chicken_race(struct battle_char *origin, struct battle_char **targets, int num){
}

static void hold_tight(struct battle_char *origin, struct battle_char **targets, int num){
}

static void darkness(struct battle_char *origin, struct battle_char **targets, int num){
}

static void lose_voice(struct battle_char *origin, struct battle_char **targets, int num){
}

static void loss(struct battle_char *origin, struct battle_char **targets, int num){
}

static void spell(struct battle_char *origin, struct battle_char **targets, int num){
}

static void nightmare(struct battle_char *origin, struct battle_char **targets, int num){
}

static void death_cold(struct battle_char *origin, struct battle_char **targets, int num){
}

static void magic_ruin(struct battle_char *origin, struct battle_char **targets, int num){
}

static void speed_ruin(struct battle_char *origin, struct battle_char **targets, int num){
}

static void power_ruin(struct battle_char *origin, struct battle_char **targets, int num){
}

static void mind_ruin(struct battle_char *origin, struct battle_char **targets, int num){
}

static void blood_suck(struct battle_char *origin, struct battle_char **targets, int num){
}

static void allure(struct battle_char *origin, struct battle_char **targets, int num){
}

static void bio(struct battle_char *origin, struct battle_char **targets, int num){
	// May add either dark, poison, oil
}

static void bio2(struct battle_char *origin, struct battle_char **targets, int num){
	// Adds either frog, slow, silence, petrify
}

static void bio3(struct battle_char *origin, struct battle_char **targets, int num){
	// May add either dead, undead, petrify
}

static void mbarrier(struct battle_char *origin, struct battle_char **targets, int num){
}

static void leg_aim(struct battle_char *origin, struct battle_char **targets, int num){
}

static void arm_aim(struct battle_char *origin, struct battle_char **targets, int num){
}

static void seal_evil(struct battle_char *origin, struct battle_char **targets, int num){
}

static void melt(struct battle_char *origin, struct battle_char **targets, int num){
}

static void tornado(struct battle_char *origin, struct battle_char **targets, int num){
}

static void quake(struct battle_char *origin, struct battle_char **targets, int num){
}

static void toad2(struct battle_char *origin, struct battle_char **targets, int num){
}

static void gravi2(struct battle_char *origin, struct battle_char **targets, int num){
}

static void flare2(struct battle_char *origin, struct battle_char **targets, int num){
}

static void blind2(struct battle_char *origin, struct battle_char **targets, int num){
}

static void small_bomb_grenade(struct battle_char *origin, struct battle_char **targets, int num){
}

static void small_bomb_explosive(struct battle_char *origin, struct battle_char **targets, int num){
}

static void confuse2(struct battle_char *origin, struct battle_char **targets, int num){
}

static void sleep2(struct battle_char *origin, struct battle_char **targets, int num){
}

static void ultima_ultimate(struct battle_char *origin, struct battle_char **targets, int num){
}

static void mute(struct battle_char *origin, struct battle_char **targets, int num){
}

static void despair2(struct battle_char *origin, struct battle_char **targets, int num){
}

static void return2(struct battle_char *origin, struct battle_char **targets, int num){
}

static void blind_sword(struct battle_char *origin, struct battle_char **targets, int num){
}

static void aspel(struct battle_char *origin, struct battle_char **targets, int num){
}

static void drain(struct battle_char *origin, struct battle_char **targets, int num){
}

static void faith(struct battle_char *origin, struct battle_char **targets, int num){
}

static void innocent(struct battle_char *origin, struct battle_char **targets, int num){
}

static void zombie_sword(struct battle_char *origin, struct battle_char **targets, int num){
}

static void silence(struct battle_char *origin, struct battle_char **targets, int num){
}

static void berserk(struct battle_char *origin, struct battle_char **targets, int num){
}

static void chicken(struct battle_char *origin, struct battle_char **targets, int num){
}

static void confuse(struct battle_char *origin, struct battle_char **targets, int num){
}

static void despair(struct battle_char *origin, struct battle_char **targets, int num){
}

static void magic_sleep_sword(struct battle_char *origin, struct battle_char **targets, int num){
}

// The magic sword ability: break. It does not break swords.
static void break_sword(struct battle_char *origin, struct battle_char **targets, int num){
}

static void ice_bracelet(struct battle_char *origin, struct battle_char **targets, int num){
}

static void fire_bracelet(struct battle_char *origin, struct battle_char **targets, int num){
}

static void thnder_brcelet(struct battle_char *origin, struct battle_char **targets, int num){
}

static void dragon_tame(struct battle_char *origin, struct battle_char **targets, int num){
}

static void dragon_care(struct battle_char *origin, struct battle_char **targets, int num){
}

static void dragon_powerup(struct battle_char *origin, struct battle_char **targets, int num){
}

static void dragon_levelup(struct battle_char *origin, struct battle_char **targets, int num){
}

static void holy_bracelet(struct battle_char *origin, struct battle_char **targets, int num){
}

static void braver(struct battle_char *origin, struct battle_char **targets, int num){
}

static void blade_beam(struct battle_char *origin, struct battle_char **targets, int num){
}

static void climhazzard(struct battle_char *origin, struct battle_char **targets, int num){
}

static void meteorain(struct battle_char *origin, struct battle_char **targets, int num){
}

static void finish_touch(struct battle_char *origin, struct battle_char **targets, int num){
}

static void omnislash(struct battle_char *origin, struct battle_char **targets, int num){
}

static void cherry_blossom(struct battle_char *origin, struct battle_char **targets, int num){
}

static void choco_attack(struct battle_char *origin, struct battle_char **targets, int num){
}

static void choco_ball(struct battle_char *origin, struct battle_char **targets, int num){
}

static void choco_meteor(struct battle_char *origin, struct battle_char **targets, int num){
}

static void choco_esuna(struct battle_char *origin, struct battle_char **targets, int num){
}

static void choco_cure(struct battle_char *origin, struct battle_char **targets, int num){
}

static void tackle(struct battle_char *origin, struct battle_char **targets, int num){
}

static void goblin_punch(struct battle_char *origin, struct battle_char **targets, int num){
}

static void turn_punch(struct battle_char *origin, struct battle_char **targets, int num){
}

static void eye_gouge(struct battle_char *origin, struct battle_char **targets, int num){
}

static void mutilate(struct battle_char *origin, struct battle_char **targets, int num){
}

static void small_bomb_bomb(struct battle_char *origin, struct battle_char **targets, int num){
}

static void flame_attack(struct battle_char *origin, struct battle_char **targets, int num){
}

static void spark(struct battle_char *origin, struct battle_char **targets, int num){
}

static void scratch(struct battle_char *origin, struct battle_char **targets, int num){
}

static void cat_kick(struct battle_char *origin, struct battle_char **targets, int num){
}

static void blaster(struct battle_char *origin, struct battle_char **targets, int num){
}

static void poison_nail(struct battle_char *origin, struct battle_char **targets, int num){
}

static void blood_suck_coeurl(struct battle_char *origin, struct battle_char **targets, int num){
}

static void black_ink(struct battle_char *origin, struct battle_char **targets, int num){
}

static void odd_soundwave(struct battle_char *origin, struct battle_char **targets, int num){
}

static void mind_blast(struct battle_char *origin, struct battle_char **targets, int num){
}

static void level_blast(struct battle_char *origin, struct battle_char **targets, int num){
}

static void knife_hand(struct battle_char *origin, struct battle_char **targets, int num){
}

static void thunder_soul(struct battle_char *origin, struct battle_char **targets, int num){
}

static void aqua_soul(struct battle_char *origin, struct battle_char **targets, int num){
}

static void ice_soul(struct battle_char *origin, struct battle_char **targets, int num){
}

static void wind_soul(struct battle_char *origin, struct battle_char **targets, int num){
}

static void throw_spirit(struct battle_char *origin, struct battle_char **targets, int num){
}

static void zombie_touch(struct battle_char *origin, struct battle_char **targets, int num){
}

static void sleep_touch(struct battle_char *origin, struct battle_char **targets, int num){
}

static void drain_touch(struct battle_char *origin, struct battle_char **targets, int num){
}

static void grease_touch(struct battle_char *origin, struct battle_char **targets, int num){
}

static void wing_attack(struct battle_char *origin, struct battle_char **targets, int num){
}

static void look_of_devil(struct battle_char *origin, struct battle_char **targets, int num){
}

static void look_of_fright(struct battle_char *origin, struct battle_char **targets, int num){
}

static void circle(struct battle_char *origin, struct battle_char **targets, int num){
}

static void death_sentence_ahriman(struct battle_char *origin, struct battle_char **targets, int num){
}

static void scratch_up(struct battle_char *origin, struct battle_char **targets, int num){
}

static void beak(struct battle_char *origin, struct battle_char **targets, int num){
}

static void shine_lover(struct battle_char *origin, struct battle_char **targets, int num){
}

static void feather_bomb(struct battle_char *origin, struct battle_char **targets, int num){
}

static void beaking(struct battle_char *origin, struct battle_char **targets, int num){
}

static void straight_dash(struct battle_char *origin, struct battle_char **targets, int num){
}

static void nose_bracelet(struct battle_char *origin, struct battle_char **targets, int num){
}

static void oink(struct battle_char *origin, struct battle_char **targets, int num){
}

static void please_eat(struct battle_char *origin, struct battle_char **targets, int num){
}

static void leaf_dance(struct battle_char *origin, struct battle_char **targets, int num){
}

static void protect_spirit(struct battle_char *origin, struct battle_char **targets, int num){
}

static void clam_spirit(struct battle_char *origin, struct battle_char **targets, int num){
}

static void spirit_of_life(struct battle_char *origin, struct battle_char **targets, int num){
}

static void magic_spirit(struct battle_char *origin, struct battle_char **targets, int num){
}

static void shake_off(struct battle_char *origin, struct battle_char **targets, int num){
}

static void wave_around(struct battle_char *origin, struct battle_char **targets, int num){
}

static void mimic_titan(struct battle_char *origin, struct battle_char **targets, int num){
}

static void gather_power(struct battle_char *origin, struct battle_char **targets, int num){
}

static void blow_fire(struct battle_char *origin, struct battle_char **targets, int num){
}

static void tentacle(struct battle_char *origin, struct battle_char **targets, int num){
}

static void lick(struct battle_char *origin, struct battle_char **targets, int num){
}

static void goo(struct battle_char *origin, struct battle_char **targets, int num){
}

static void bad_bracelet(struct battle_char *origin, struct battle_char **targets, int num){
}

static void moldball_virus(struct battle_char *origin, struct battle_char **targets, int num){
}

static void stab_up(struct battle_char *origin, struct battle_char **targets, int num){
}

static void sudden_cry(struct battle_char *origin, struct battle_char **targets, int num){
}

static void hurricane(struct battle_char *origin, struct battle_char **targets, int num){
}

static void ulmaguest(struct battle_char *origin, struct battle_char **targets, int num){
}

static void giga_flare(struct battle_char *origin, struct battle_char **targets, int num){
}

static void dash_dragon(struct battle_char *origin, struct battle_char **targets, int num){
}

static void tail_swing(struct battle_char *origin, struct battle_char **targets, int num){
}

static void ice_bracelet_dragon(struct battle_char *origin, struct battle_char **targets, int num){
}

static void fire_bracelet_dragon(struct battle_char *origin, struct battle_char **targets, int num){
}

static void thnder_brcelet_dragon(struct battle_char *origin, struct battle_char **targets, int num){
}

static void triple_attack(struct battle_char *origin, struct battle_char **targets, int num){
}

static void triple_bracelet(struct battle_char *origin, struct battle_char **targets, int num){
}

static void triple_thunder(struct battle_char *origin, struct battle_char **targets, int num){
}

static void triple_flame(struct battle_char *origin, struct battle_char **targets, int num){
}

static void dark_whisper(struct battle_char *origin, struct battle_char **targets, int num){
}

static void snake_carrier(struct battle_char *origin, struct battle_char **targets, int num){
}

static void poison_frog(struct battle_char *origin, struct battle_char **targets, int num){
}

static void midgar_swarm(struct battle_char *origin, struct battle_char **targets, int num){
}

static void lifebreak(struct battle_char *origin, struct battle_char **targets, int num){
}

static void nanoflare(struct battle_char *origin, struct battle_char **targets, int num){
}

static void grand_cross(struct battle_char *origin, struct battle_char **targets, int num){
}

static void destroy(struct battle_char *origin, struct battle_char **targets, int num){
}

static void compress(struct battle_char *origin, struct battle_char **targets, int num){
}

static void dispose(struct battle_char *origin, struct battle_char **targets, int num){
}

static void crush(struct battle_char *origin, struct battle_char **targets, int num){
}

static void energy(struct battle_char *origin, struct battle_char **targets, int num){
}

static void parasite(struct battle_char *origin, struct battle_char **targets, int num){
}

static void attack(struct battle_char *s, struct battle_char **d, int num){
	uint16_t pri=s->ch->eq[EQ_WEAPON];
	uint16_t sec=s->ch->eq[EQ_OFFHAND];
	uint16_t dmg;
	struct stored_action thisact;

	thisact.ctr=0;
	thisact.jobindex=CL_GENERIC;
	thisact.findex=0;
	thisact.f=attack;
	thisact.origin=s;
	thisact.target.x=d[0]->x;
	thisact.target.y=d[0]->y;
	thisact.target.width=1;
	thisact.target.vertical=3;
	thisact.target.dir=0;

	if(evaded(d[0],AFLAG_PHYSICAL,get_attack_dir(s,d[0]),100))
		return;

	dmg=weapon_damage[EQ_TYPE(pri)](&weapons[EQ_TYPE(pri)][pri>>6],s,d[0]);

	if(sec==0 && s->ch->support==SFLAG_TWO_HANDS)
		dmg<<=1;

	else if(sec>0 && EQ_TYPE(sec)!=EQO_SHIELD) // Implied SFLAG_TWO_SWORDS. check it?
		dmg+=weapon_damage[EQ_TYPE(sec)](&weapons[EQ_TYPE(sec)][sec>>6],s,d[0]);
	
	deal_damage(d[0],dmg);

	last_action.preresolve=&thisact;
	last_action.damage=dmg;
	last_action.mp_used=0;
	last_action.item=0;

	react(s,d,num);
}


// Sorry this is so ugly... it's perl's fault :P
const struct ability claction[NUM_CLASS][NUM_ACTION_PER_ABILITY]={
{{.f.af=attack,0,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_EVADE|0,0,0,2,AFLAG_MOD_XA,0,{RANGE_WEAPON,0,1,0,0}}}, // Generic
{},//CHARGE (Archer)
{{.f.af=angel_song,100,0,0,6,0,0,0,{0,0,0,0,0}},{.f.af=life_song,100,0,0,6,0,0,0,{0,0,0,0,0}},{.f.af=cheer_song,100,0,0,8,0,0,0,{0,0,0,0,0}},{.f.af=battle_song,100,0,0,8,0,0,0,{0,0,0,0,0}},{.f.af=magic_song,100,0,0,10,0,0,0,{0,0,0,0,0}},{.f.af=nameless_song,100,0,0,10,0,0,0,{0,0,0,0,0}},{.f.af=last_song,100,0,0,20,0,0,0,{0,0,0,0,0}}}, //SING (Bard)
{},//MATH SKILL (Calculator)
{},//ITEM (Chemist)
{{.f.af=witch_hunt,100,0,0,6,0,0,0,{0,0,0,0,0}},{.f.af=wiznaibus,100,0,0,6,0,0,0,{0,0,0,0,0}},{.f.af=slow_dance,100,0,0,8,0,0,0,{0,0,0,0,0}},{.f.af=polka_polka,100,0,0,8,0,0,0,{0,0,0,0,0}},{.f.af=disillusion,100,0,0,8,0,0,0,{0,0,0,0,0}},{.f.af=nameless_dance,100,0,0,10,0,0,0,{0,0,0,0,0}},{.f.af=last_dance,100,0,0,20,0,0,0,{0,0,0,0,0}}}, //DANCE  (Dancer)
{{.f.af=pitfall,150,AFLAG_MAGIC|AFLAG_COUNTER_FLOOD|0,0,0,5,AFLAG_MOD_MA,0,{5,0,2,0,0}},{.f.af=water_ball,150,AFLAG_MAGIC|AFLAG_COUNTER_FLOOD|0,ELEM_WATER,0,5,AFLAG_MOD_MA,0,{5,0,2,0,0}},{.f.af=hell_ivy,150,AFLAG_MAGIC|AFLAG_COUNTER_FLOOD|0,0,0,5,AFLAG_MOD_MA,0,{5,0,2,0,0}},{.f.af=carve_model,150,AFLAG_MAGIC|AFLAG_COUNTER_FLOOD|0,0,0,5,AFLAG_MOD_MA,0,{5,0,2,0,0}},{.f.af=local_quake,150,AFLAG_MAGIC|AFLAG_COUNTER_FLOOD|0,ELEM_EARTH,0,5,AFLAG_MOD_MA,0,{5,0,2,0,0}},{.f.af=kamaitachi,150,AFLAG_MAGIC|AFLAG_COUNTER_FLOOD|0,ELEM_WIND,0,5,AFLAG_MOD_MA,0,{5,0,2,0,0}},{.f.af=demon_fire,150,AFLAG_MAGIC|AFLAG_COUNTER_FLOOD|0,ELEM_FIRE,0,5,AFLAG_MOD_MA,0,{5,0,2,0,0}},{.f.af=quicksand,150,AFLAG_MAGIC|AFLAG_COUNTER_FLOOD|0,ELEM_WATER,0,5,AFLAG_MOD_MA,0,{5,0,2,0,0}},{.f.af=blizzard,150,AFLAG_MAGIC|AFLAG_COUNTER_FLOOD|0,ELEM_ICE,0,5,AFLAG_MOD_MA,0,{5,0,2,0,0}},{.f.af=gusty_wind,150,AFLAG_MAGIC|AFLAG_COUNTER_FLOOD|0,ELEM_WATER,0,5,AFLAG_MOD_MA,0,{5,0,2,0,0}},{.f.af=lava_ball,150,AFLAG_MAGIC|AFLAG_COUNTER_FLOOD|0,ELEM_FIRE,0,5,AFLAG_MOD_MA,0,{5,0,2,0,0}}}, //ELEMENTAL (Geomancer)
{{.f.af=head_break,300,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_EVADE|0,ELEM_WEAPON,0,3,45,0,{RANGE_WEAPON,0,1,0,0}},{.f.af=armor_break,400,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_EVADE|0,ELEM_WEAPON,0,3,40,0,{RANGE_WEAPON,0,1,0,0}},{.f.af=shield_break,300,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_EVADE|0,ELEM_WEAPON,0,3,55,0,{RANGE_WEAPON,0,1,0,0}},{.f.af=weapon_break,400,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_EVADE|0,ELEM_WEAPON,0,3,30,0,{RANGE_WEAPON,0,1,0,0}},{.f.af=magic_break,250,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_EVADE|0,ELEM_WEAPON,0,3,50,0,{RANGE_WEAPON,0,1,0,0}},{.f.af=speed_break,250,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_EVADE|0,ELEM_WEAPON,0,3,50,0,{RANGE_WEAPON,0,1,0,0}},{.f.af=power_break,250,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_EVADE|0,ELEM_WEAPON,0,3,50,0,{RANGE_WEAPON,0,1,0,0}},{.f.af=mind_break,250,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_EVADE|0,ELEM_WEAPON,0,3,50,0,{RANGE_WEAPON,0,1,0,0}}}, //BATTLE SKILL (Knight)
{}, //JUMP (Lancer)
{{.f.af=invitation,100,0,0,0,1,20,0,{3,3,1,0,0}},{.f.af=persuade,100,0,0,0,1,30,0,{3,3,1,0,0}},{.f.af=praise,200,0,0,0,1,50,0,{3,3,1,0,0}},{.f.af=threaten,200,0,0,0,1,90,0,{3,3,1,0,0}},{.f.af=preach,200,0,0,0,1,50,0,{3,3,1,0,0}},{.f.af=solution,200,0,0,0,1,90,0,{3,3,1,0,0}},{.f.af=death_sentence,500,0,0,0,1,30,0,{3,3,1,0,0}},{.f.af=negotiate,100,0,0,0,1,90,0,{3,3,1,0,0}},{.f.af=insult,300,0,0,0,1,40,0,{3,3,1,0,0}},{.f.af=mimic_daravon,300,0,0,0,1,40,0,{3,3,2,3,0}}}, //TALK SKILL (Mediator)
{}, // MIMICRY (Mime)
{{.f.af=spin_fist,150,AFLAG_PHYSICAL|AFLAG_EVADE|0,0,0,2,AFLAG_MOD_XA,0,{0,0,2,0,0}},{.f.af=repeating_fist,300,AFLAG_PHYSICAL|AFLAG_EVADE|0,0,0,2,AFLAG_MOD_XA,0,{1,1,1,0,0}},{.f.af=wave_fist,300,AFLAG_PHYSICAL|AFLAG_EVADE|0,0,0,2,AFLAG_MOD_XA,0,{3,3,1,0,0}},{.f.af=earth_slash,600,AFLAG_PHYSICAL|0,ELEM_EARTH,0,2,AFLAG_MOD_XA,0,{4,0,8,2,0}},{.f.af=secret_fist,300,AFLAG_PHYSICAL|0,0,0,3,50,0,{1,0,1,0,0}},{.f.af=stigma_magic,200,AFLAG_PHYSICAL|0,0,0,3,120,0,{0,0,2,0,0}},{.f.af=chakra,350,AFLAG_PHYSICAL|0,0,0,2,AFLAG_MOD_XA,0,{0,0,2,0,0}},{.f.af=revive,500,AFLAG_PHYSICAL|0,0,0,3,70,0,{1,0,1,0,0}}}, //PUNCH ART (Monk)
{}, //THROW (Ninja)
{{.f.af=blind_yin,100,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,0,2,6,200,4,{4,0,2,1,0}},{.f.af=spell_absorb,200,AFLAG_MAGIC|AFLAG_COUNTER_MAGIC|0,0,2,6,160,2,{4,0,1,0,0}},{.f.af=life_drain,350,AFLAG_MAGIC|AFLAG_COUNTER_MAGIC|0,0,2,6,160,16,{4,0,1,0,0}},{.f.af=pray_faith,400,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_MATH|0,0,4,6,150,6,{4,0,1,0,0}},{.f.af=doubt_faith,400,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_MATH|0,0,4,6,150,6,{4,0,1,0,0}},{.f.af=zombie_yin,300,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,0,5,6,100,20,{4,0,1,0,0}},{.f.af=silence_song,170,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,0,3,6,180,16,{4,0,2,1,0}},{.f.af=blind_rage,400,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,0,5,6,120,16,{4,0,1,0,0}},{.f.af=foxbird,200,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,0,4,6,140,20,{4,0,1,0,0}},{.f.af=confusion_song,400,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,0,5,6,130,20,{4,0,1,0,0}},{.f.af=dispel_magic,700,AFLAG_MAGIC|AFLAG_COUNTER_MAGIC|AFLAG_MATH|0,0,3,6,200,34,{4,0,1,0,0}},{.f.af=paralyze,100,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,0,5,6,185,10,{4,0,2,0,0}},{.f.af=magic_sleep_yin,350,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,0,6,170,6,24,{4,0,2,1,0}},{.f.af=petrify,580,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,0,9,6,120,16,{4,0,1,0,0}}}, //YIN-YANG MAGIC (Oracle)
{{.f.af=cure,50,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_MATH|0,0,4,5,AFLAG_MOD_MA,6,{4,0,2,1,0}},{.f.af=cure2,180,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_MATH|0,0,5,AFLAG_MOD_MA,5,10,{4,0,2,1,0}},{.f.af=cure3,400,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_MATH|0,0,7,5,AFLAG_MOD_MA,16,{4,0,2,2,0}},{.f.af=cure4,700,AFLAG_MAGIC|0,0,10,5,AFLAG_MOD_MA,20,{4,0,2,3,0}},{.f.af=raise,180,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_MATH|0,0,4,6,180,10,{4,0,1,0,0}},{.f.af=raise2,500,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_MATH|0,0,10,6,160,20,{4,0,1,0,0}},{.f.af=reraise,800,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_MATH|0,0,7,6,140,16,{4,0,1,0,0}},{.f.af=regen,300,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_MATH|0,0,4,6,170,8,{3,0,2,0,0}},{.f.af=protect,70,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_MATH|0,0,4,6,200,6,{3,0,2,0,0}},{.f.af=protect2,500,AFLAG_MAGIC|0,0,7,6,120,24,{3,0,2,3,0}},{.f.af=shell,70,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_MATH|0,0,4,6,200,6,{3,0,2,0,0}},{.f.af=shell2,500,AFLAG_MAGIC|0,0,7,6,120,20,{3,0,2,3,0}},{.f.af=wall,380,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_MATH|0,0,4,6,140,24,{3,0,1,0,0}},{.f.af=esuna,280,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_MATH|0,0,3,6,190,18,{3,0,2,2,0}},{.f.af=holy,600,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|0,ELEM_HOLY,6,5,AFLAG_MOD_MA,56,{5,0,1,0,0}}}, //WHITE MAGIC (Priest)
{{.f.af=asura_draw,100,AFLAG_MAGIC|0,0,0,5,AFLAG_MOD_MA,0,{0,0,3,3,0}},{.f.af=koutetsu,180,AFLAG_MAGIC|0,0,0,5,AFLAG_MOD_MA,0,{0,0,3,3,0}},{.f.af=bizen_boat,260,AFLAG_MAGIC|0,0,0,5,AFLAG_MOD_MA,0,{0,0,3,3,0}},{.f.af=murasame,340,0,0,0,0,0,0,{0,0,3,3,0}},{.f.af=heavens_cloud,420,AFLAG_MAGIC|0,0,0,5,AFLAG_MOD_MA,0,{0,0,3,3,0}},{.f.af=kiyomori,500,0,0,0,0,0,0,{0,0,3,3,0}},{.f.af=muramasa,580,AFLAG_MAGIC|0,0,0,5,AFLAG_MOD_MA,0,{0,0,3,3,0}},{.f.af=kikuichimoji,660,AFLAG_MAGIC|0,0,0,5,AFLAG_MOD_MA,0,{4,0,8,3,0}},{.f.af=masamune,740,0,0,0,0,0,0,{0,0,3,3,0}},{.f.af=chirijiraden,820,AFLAG_MAGIC|0,0,0,5,AFLAG_MOD_MA,0,{0,0,3,3,0}}}, //DRAW OUT (Samurai)
{{.f.af=accumulate,300,0,0,0,0,0,0,{0,0,1,0,0}},{.f.af=dash,75,AFLAG_PHYSICAL|0,0,0,2,AFLAG_MOD_XA,0,{1,1,1,0,0}},{.f.af=throw_stone,90,AFLAG_PHYSICAL|AFLAG_EVADE|0,0,0,2,AFLAG_MOD_XA,0,{4,0,1,0,0}},{.f.af=heal,150,0,0,0,0,0,0,{1,2,1,0,0}}}, //BASIC SKILL (Squire)
{{.f.af=moogle,110,AFLAG_MAGIC|0,0,2,5,AFLAG_MOD_MA,8,{4,0,3,2,0}},{.f.af=shiva,200,AFLAG_MAGIC|0,ELEM_ICE,4,5,AFLAG_MOD_MA,24,{4,0,3,2,0}},{.f.af=ramuh,200,AFLAG_MAGIC|0,ELEM_LIGHTNING,4,5,AFLAG_MOD_MA,24,{4,0,3,2,0}},{.f.af=ifrit,200,AFLAG_MAGIC|0,ELEM_FIRE,4,5,AFLAG_MOD_MA,24,{4,0,3,2,0}},{.f.af=titan,220,AFLAG_MAGIC|0,ELEM_EARTH,5,5,AFLAG_MOD_MA,30,{4,0,3,2,0}},{.f.af=golem,500,AFLAG_MAGIC|0,0,3,8,200,40,{0,0,1,0,0}},{.f.af=carbunkle,350,AFLAG_MAGIC|0,0,4,6,150,30,{4,0,3,2,0}},{.f.af=bahamut,1200,AFLAG_MAGIC|0,0,10,5,AFLAG_MOD_MA,60,{4,0,4,3,0}},{.f.af=odin,900,AFLAG_MAGIC|0,0,9,5,AFLAG_MOD_MA,50,{4,0,4,3,0}},{.f.af=leviathan,850,AFLAG_MAGIC|0,ELEM_WATER,9,5,AFLAG_MOD_MA,48,{4,0,4,3,0}},{.f.af=salamander,820,AFLAG_MAGIC|0,ELEM_FIRE,9,5,AFLAG_MOD_MA,48,{4,0,3,2,0}},{.f.af=silf,400,AFLAG_MAGIC|0,0,5,6,150,26,{4,0,3,2,0}},{.f.af=fairy,400,AFLAG_MAGIC|0,0,4,5,AFLAG_MOD_MA,28,{4,0,3,2,0}},{.f.af=lich,600,AFLAG_MAGIC|0,ELEM_DARK,9,6,160,40,{4,0,3,2,0}},{.f.af=cyclops,1000,AFLAG_MAGIC|0,0,9,5,AFLAG_MOD_MA,62,{4,0,3,2,0}},{.f.af=zodiac,1000,AFLAG_MAGIC|0,0,10,5,AFLAG_MOD_MA,99,{4,0,4,3,0}}}, //SUMMON MAGIC (Summoner)
{{.f.af=gil_taking,10,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_EVADE|0,0,0,4,200,0,{1,1,1,0,0}},{.f.af=steal_heart,150,0,0,0,1,50,0,{3,0,1,0,0}},{.f.af=steal_helmet,350,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_EVADE|0,0,0,4,50,0,{1,1,1,0,0}},{.f.af=steal_armor,450,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_EVADE|0,0,0,4,35,0,{1,1,1,0,0}},{.f.af=steal_shield,350,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_EVADE|0,0,0,4,35,0,{1,1,1,0,0}},{.f.af=steal_weapon,600,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_EVADE|0,0,0,4,30,0,{1,1,1,0,0}},{.f.af=steal_accessry,500,AFLAG_PHYSICAL|AFLAG_COUNTER|AFLAG_EVADE|0,0,0,4,40,0,{1,1,1,0,0}},{.f.af=steal_exp,250,AFLAG_PHYSICAL|0,0,0,4,70,0,{1,1,1,0,0}}}, //STEAL (Thief)
{{.f.af=haste,100,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_MATH|0,0,2,6,180,8,{3,0,2,0,0}},{.f.af=haste2,550,AFLAG_MAGIC|0,0,7,6,240,30,{3,0,2,3,0}},{.f.af=slow,80,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,0,2,6,180,8,{3,0,2,0,0}},{.f.af=slow2,520,AFLAG_MAGIC|AFLAG_COUNTER_MAGIC|AFLAG_EVADE|0,0,7,6,240,30,{3,0,2,3,0}},{.f.af=stop,330,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,0,7,6,110,14,{3,0,2,0,0}},{.f.af=dont_move,100,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,0,3,6,190,10,{3,0,2,1,0}},{.f.af=magic_float,200,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_MATH|0,0,2,6,140,8,{4,0,2,1,0}},{.f.af=reflect,330,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_MATH|0,0,2,6,180,12,{4,0,1,0,0}},{.f.af=quick,800,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|0,0,4,6,140,24,{4,0,1,0,0}},{.f.af=demi,250,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,0,6,6,190,24,{4,0,2,1,0}},{.f.af=demi2,550,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,0,9,6,120,50,{4,0,2,3,0}},{.f.af=meteor,1500,AFLAG_MAGIC|AFLAG_COUNTER_MAGIC|0,0,13,5,AFLAG_MOD_MA,70,{4,0,4,3,0}}}, //TIME MAGIC (Time Mage)
{{.f.af=fire,50,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,ELEM_FIRE,4,5,AFLAG_MOD_MA,6,{4,0,2,1,0}},{.f.af=fire2,200,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,ELEM_FIRE,5,5,AFLAG_MOD_MA,12,{4,0,2,2,0}},{.f.af=fire3,480,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,ELEM_FIRE,7,5,AFLAG_MOD_MA,24,{4,0,2,3,0}},{.f.af=fire4,850,AFLAG_MAGIC|AFLAG_EVADE|0,ELEM_FIRE,10,5,AFLAG_MOD_MA,48,{4,0,3,3,0}},{.f.af=bolt,50,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,ELEM_LIGHTNING,4,5,AFLAG_MOD_MA,6,{4,0,2,1,0}},{.f.af=bolt2,200,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,ELEM_LIGHTNING,5,5,AFLAG_MOD_MA,10,{4,0,2,2,0}},{.f.af=bolt3,480,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,ELEM_LIGHTNING,7,5,AFLAG_MOD_MA,24,{4,0,2,3,0}},{.f.af=bolt4,850,AFLAG_MAGIC|AFLAG_EVADE|0,ELEM_LIGHTNING,10,5,AFLAG_MOD_MA,48,{4,0,3,3,0}},{.f.af=ice,50,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,ELEM_ICE,4,5,AFLAG_MOD_MA,6,{4,0,2,1,0}},{.f.af=ice2,200,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,ELEM_ICE,5,5,AFLAG_MOD_MA,12,{4,0,2,2,0}},{.f.af=ice3,480,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,ELEM_ICE,7,5,AFLAG_MOD_MA,24,{4,0,2,3,0}},{.f.af=ice4,850,AFLAG_MAGIC|AFLAG_EVADE|0,ELEM_ICE,10,5,AFLAG_MOD_MA,48,{4,0,3,3,0}},{.f.af=poison,150,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,0,3,6,160,6,{3,0,2,0,0}},{.f.af=frog,500,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,0,5,6,120,12,{3,0,1,0,0}},{.f.af=death,600,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,0,10,6,100,24,{3,0,1,0,0}},{.f.af=flare,900,AFLAG_MAGIC|AFLAG_REFLECT|AFLAG_COUNTER_MAGIC|AFLAG_MATH|AFLAG_EVADE|0,0,7,5,AFLAG_MOD_MA,60,{5,0,1,0,0}}}, //BLACK MAGIC (Wizard)
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

static int speed_save(struct battle_char *reacter, struct battle_char *attacker){
}

/*
const uint8_t num_reaction[]={};
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
};
*/
