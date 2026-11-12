#ifndef _BOARD_OPENING_H
#define _BOARD_OPENING_H
#ifdef __cplusplus
extern "C" {
#endif

#include "game/board/main.h"

void MBOpeningProcExec(void);
void MBOpeningExec(void);

void MBOpeningViewSet(HuVecF *rot, HuVecF *pos, float zoom);
float MBOpeningViewZoomGet(void);
void MBOpeningViewRotGet(HuVecF *rot);
void MBOpeningViewFocusGet(HuVecF *focus);

#ifdef __cplusplus
}
#endif
#endif