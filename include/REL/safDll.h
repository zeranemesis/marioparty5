#ifndef _SAFDLL_H
#define _SAFDLL_H

#include "game/object.h"
#include "game/hu3d.h"
#include "game/sprite.h"
#include "game/wipe.h"

/* saf.c */
void SafObjectSetup(void);
void SafMain(void);
void SafCameraUpdate(OMOBJ *obj);

/* safanim.c - unused/debug camera rehearsal for the ball reflection test */
void SafAnimMain(void);
void SafAnimExit(void);
void SafAnimCameraUpdate(void);

/* safmotion.c - unused/debug camera rehearsal for the motion test */
void SafMotionObjectSetup(void);
void SafMotionMain(void);
void SafMotionCameraUpdate(OMOBJ *obj);

#endif
