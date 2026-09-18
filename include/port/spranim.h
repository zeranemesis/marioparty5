#ifndef PORT_SPRANIM_H_
#define PORT_SPRANIM_H_

#include "game/animdata.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Converts an ANM file into an animation the host can use, or returns the one
 * already converted from that same file. */
ANIMDATA *PortSprAnimRead(void *file);
/* The animation converted from this file, NULL if there is none. */
ANIMDATA *PortSprAnimFind(const void *file);
/* Called before an animation is freed, so its file can be converted again. */
void PortSprAnimForget(const ANIMDATA *anim);

#ifdef __cplusplus
}
#endif

#endif
