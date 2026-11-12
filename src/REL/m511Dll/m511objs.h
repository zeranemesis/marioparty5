#ifndef M511_OBJS_H
#define M511_OBJS_H

#include <dolphin.h>

#include "game/hu3d.h"

/* Objects owned by the boats; shared by boatsplash.cpp and player.cpp. */

/* Water spray around the boat. */
struct Mgr511Splash {
    s16 modelId;
    HU3DPARTICLE *particle;
    Vec pos;
    Vec rot;
    Vec pos2;
    u8 flag;
    f32 speed;
};

/* Expanding ring on the water. */
struct Mgr511SplashRing {
    s16 modelId;
    ANIMDATA *anim;
    Vec pos;
    u8 active;
    f32 timer;
    f32 scale;
};

/* Shadow under the boat (same layout as Mgr510Shadow in boatshadow.cpp). */
struct Mgr511Shadow {
    ANIMDATA *anim;
    s16 modelId;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
};

#endif
