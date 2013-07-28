#ifndef CLASSES_H
#define CLASSES_H

#include "player.h"

#define NUM_CLASS 20

#define CLBIT(x) (1<<(x-1))

#define MALE 1
#define FEMALE 2

#define CL_ARCHER 1
#define CL_BARD 2
#define CL_CALCULATOR 3
#define CL_CHEMIST 4
#define CL_DANCER 5
#define CL_GEOMANCER 6
#define CL_KNIGHT 7
#define CL_LANCER 8
#define CL_MEDIATOR 9
#define CL_MIME 10
#define CL_MONK 11
#define CL_NINJA 12
#define CL_ORACLE 13
#define CL_PRIEST 14
#define CL_SAMURAI 15
#define CL_SQUIRE 16
#define CL_SUMMONER 17
#define CL_THIEF 18
#define CL_TIME_MAGE 19
#define CL_WIZARD 20

#define STAT_HP 0
#define STAT_MP 1
#define STAT_SP 2
#define STAT_PA 3
#define STAT_MA 4

#define BASE_STAT_DENOMINATOR 1683400

struct class_stats{
	uint8_t move;
	uint8_t jump;
	uint8_t evade;

	uint8_t basemod[NUM_STATS];
	uint8_t gainmod[NUM_STATS];
};

#endif
