#ifndef _GAME_FRAND_H
#define _GAME_FRAND_H

#include "dolphin/types.h"

u32 frand(void);
f32 frandf(void);
u32 frandmod(u32 arg0);

#ifdef TARGET_PC
/* frand() as the original code reads it: only its low 16 bits (clrlwi r,r,16), a cast the decompiled sources lost */
#define FRAND16() ((u16)frand())
#else
#define FRAND16() frand()
#endif

#endif