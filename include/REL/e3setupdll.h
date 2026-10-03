#ifndef E3SETUPDLL_H
#define E3SETUPDLL_H

#include "game/hu3d.h"
#include "game/sprite.h"

#ifdef __cplusplus
extern "C" {
#endif

/* stage.c */
void StageModelCreate(void *tbl);

/* e3setup.c */
void ObjectSetup(void *arg);

/* mgname.c */
void MgNameInit(void);
HUSPRGRPID MgNameCreate(void *arg);

/* toueidisp.c */
void ToueiDispInit(void);
void ToueiFBCopy(void *arg);
void ToueiZClear(void *arg);
void ToueiMatHook(void *arg);

#ifdef __cplusplus
}
#endif

#endif
