#include "game/audio.h"
#include "game/data.h"
#include "game/flag.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/printfunc.h"
#include "game/process.h"
#include "game/window.h"
#include "game/wipe.h"

#include "REL/mdstoryDll.h"

/*************************************************************
 * mdstory.c
 *
 * "mdstory" overlay.  The overlay shipped no symbol table for
 * its process entry points, so those carry objdiff's placeholder
 * names (fn_1_XXXX) keyed to their .text offset; the comments
 * below give the recovered meaning.
 *************************************************************/

extern const MD_FUNC _ctors[];
extern const MD_FUNC _dtors[];

extern void ObjectSetup(void);

/* Dot/glow effect work: three particle-manager ids held in
 * 4-byte slots (the ids themselves are s16, sign-extended on
 * store), so the 0xC-byte allocation has no padding. */
typedef struct MdStoryEffect_s {
    int parManId[3];
} MDSTORY_EFFECT;

/* 0x0 - module init: run the static constructors, then register
 * the overlay's objects */
int _prolog(void)
{
    const MD_FUNC *ctors = _ctors;
    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

/* 0x54 - module teardown: run the static destructors */
void _epilog(void)
{
    const MD_FUNC *dtors = _dtors;
    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

/* 0x1E3C - RingUpdate: per-frame ring animation driver */
void fn_1_1E3C(void)
{
    while (1) {
        MdRingMain();
        HuPrcVSleep();
    }
}

/* 0x1E54 - MainWinUpdate: per-frame main message window driver */
void fn_1_1E54(void)
{
    MDMESS *mess;

    mess = sys->mess;
    while (1) {
        mess = sys->mess;
        MdMessMain(mess, 0);
        HuPrcVSleep();
    }
}

/* 0x1E98 - SubWinUpdate: per-frame help/team text window driver */
void fn_1_1E98(void)
{
    MDMESS *mess;

    mess = sys->mess;
    while (1) {
        mess = sys->mess;
        MdMessMain(mess, 1);
        HuPrcVSleep();
    }
}

/* 0xAA64 - release the three particle managers of a "dot" effect */
static void ParticleDotKill(MDSTORY_EFFECT *effectP)
{
    Hu3DParManKill(effectP->parManId[0]);
    Hu3DParManKill(effectP->parManId[1]);
    Hu3DParManKill(effectP->parManId[2]);
}

/* 0xAB8C - reposition the single particle manager of a "glow" effect
 * and restart its two-frame lifetime */
static void ParticleGlowPosSet(MDSTORY_EFFECT *effectP, HuVecF *pos)
{
    Hu3DParManPosSet(effectP->parManId[0], pos->x, pos->y, pos->z);
    Hu3DParManTimeLimitSet(effectP->parManId[0], 2);
    Hu3DParManAttrReset(effectP->parManId[0], HU3D_PARMAN_ATTR_TIMEUP);
}

/* 0xABF8 - release the single particle manager of a "glow" effect */
static void ParticleGlowKill(MDSTORY_EFFECT *effectP)
{
    Hu3DParManKill(effectP->parManId[0]);
}