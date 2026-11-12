#ifndef _BOARD_ROULETTE_H
#define _BOARD_ROULETTE_H
#ifdef __cplusplus
extern "C" {
#endif

#include "game/board/main.h"

BOOL MBRouletteCreate(int playerNo, int maxPlayer);
BOOL MBRouletteKaneCreate(int playerNo);
BOOL MBRouletteCheck(void);
void MBRouletteWait(void);
int MBRouletteResultGet(void);
void MBRouletteMaxSpeedSet(float speed);
void MBRouletteComValueSet(s16 value);

#ifdef __cplusplus
}
#endif
#endif