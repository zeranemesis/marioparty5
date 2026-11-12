#ifndef _BOARD_BRANCH_H
#define _BOARD_BRANCH_H
#ifdef __cplusplus
extern "C" {
#endif

#include "game/board/main.h"
#include "game/board/masu.h"

void MBBranchInit(void);
BOOL MBBranchExec(int playerNo, s16 *masuId);
BOOL MBBranchExecDebug(int playerNo, s16 *masuId);
void MBBranchFlagSet(u32 flag);
void MBBranchFlagReset(u32 flag);
void MBBranchFlagInit(void);
u32 MBBranchFlagGet(void);

#ifdef __cplusplus
}
#endif
#endif