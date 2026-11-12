#ifndef _BOARD_MGCALL_H
#define _BOARD_MGCALL_H
#ifdef __cplusplus
extern "C" {
#endif

#include "game/board/main.h"

void MBCircuitExec(BOOL turnIntrF);
void MBCircuitReset(void);
void MBCircuitMgEndExec(int playerNo);
BOOL MBCircuitCarMoveExec(int playerNo);
int MBCircuitKettouSaiExec(int playerNo);
void MBCircuitCarInit(void);
int MBCircuitGoalDistGet(void);
BOOL MBCircuitGoalCheck(int playerNo);

#ifdef __cplusplus
}
#endif
#endif