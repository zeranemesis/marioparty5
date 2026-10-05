#ifndef _M543DLL_H
#define _M543DLL_H

#include "game/object.h"
#include "game/audio.h"
#include "game/mg/timer.h"
#include "game/mg/seqman.h"

#include "datanum/m543.h"

void fn_1_3FC(void);
void fn_1_3CCC(void);
BOOL fn_1_1B2C(MGTIMER* timer);
void fn_1_2F4(void);
void fn_1_320(void);
void fn_1_34C(void);

extern void ObjectSetup(void);

#endif