#include "game/board/main.h"
#include "game/board/effect.h"
#include "game/board/audio.h"
#include "game/board/scroll.h"
#include "game/board/model.h"
#include "game/board/masu.h"
#include "game/board/opening.h"
#include "game/board/capsule.h"
#include "game/board/branch.h"
#include "game/board/camera.h"
#include "game/board/player.h"
#include "game/board/window.h"
#include "game/board/coin.h"
#include "game/board/star.h"
#include "game/board/com.h"

#include "game/sprite.h"
#include "game/wipe.h"
#include "game/gamemes.h"
#include "game/pad.h"
#include "game/main.h"
#include "game/object.h"
#include "game/charman.h"
#include "game/hsfex.h"

#include <math.h>

/*
 * w04Dll - "Deep Bloober Sea" board.
 *
 * NOTE (port): mb4_Work is one flat scratch buffer (0x7F7C bytes) reused as
 * several different sub-structures, addressed with raw byte offsets in the
 * original code (typical of MWCC -O0 code that never got a single named
 * struct). Only the regions actually touched by the functions decompiled so
 * far are named below; the rest stays a raw byte array until reached.
 */

extern void ObjectSetup(void);

static void MB4_Create(void);
static void MB4_Kill(void);
static void MB4_CoinClose(void);
static void MB4_CoinEffClose(void);
static void MB4_BubbleEffClose(void);
static void MB4_SakanaClose(void);
static void MB4Ev_Kihon(s32 playerNo);
static void MB4Ev_Iruka(s32 playerNo);
static void MB4Ev_Blow(s32 playerNo);
static void MB4Ev_Kaisoo(s32 playerNo);

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

static OMOBJ *mb4_MainObj;

/* mb4_Work[0..8]: model ids, mb4_Work[6]/[7]: "Puku"(pufferfish) model ids
 * used by the fright/hatena walk-post reaction (see MB4Ev_MasuWalkPost).
 * mb4_Work[0x57..0x59]: generic 3-word state slot reused by several events
 * (state/substate/timer-ish triplet), only named per-caller as reached. */
static s32 mb4_Work[0x1FF7];

#define MB4W_S16(off) (*(s16 *) ((u8 *) mb4_Work + (off)))
#define MB4W_U16(off) (*(u16 *) ((u8 *) mb4_Work + (off)))
#define MB4W_S32(off) (*(s32 *) ((u8 *) mb4_Work + (off)))
#define MB4W_F32(off) (*(f32 *) ((u8 *) mb4_Work + (off)))

static Point3d mb4ev_Puku2EmiterPos = { -2800.0f, 300.0f, 1340.0f };

int _prolog(void) {
    const VoidFunc* ctors = _ctors;
    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void) {
    const VoidFunc* dtors = _dtors;
    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

void ObjectSetup(void) {
    GWPartyFSet(TRUE);
    MBObjectSetup(3, MB4_Create, MB4_Kill);
}

/* ----------------------------- leaf helpers ----------------------------- */

void MB4_Kill(void) {
    if (mb4_MainObj != NULL) {
        mb4_MainObj = NULL;
    }
    MB4_CoinClose();
    MB4_CoinEffClose();
    MB4_BubbleEffClose();
    MB4_SakanaClose();
}

/* NONMATCHING: MWCC keeps the literal args to the nested Hu3DGLightCreate()
 * call in nonvolatile FPRs (f26-f31) here instead of the original's stack
 * temps; the exact original source form of this call wasn't recovered. */
void MB4_LightSet(void) {
    Hu3DShadowCreate(45.0f, 1000.0f, 250000.0f);
    Hu3DShadowTPLvlSet(0.3f);
    Hu3DBGColorSet(0xFF, 0xFF, 0xFF);
    Hu3DGLightInfinitytSet(Hu3DGLightCreate(-3700.0f, 12000.0f, 22000.0f, 3700.0f, -12000.0f, -22000.0f, 0xFF, 0xFF, 0xFF));
}

void MB4_LightReset(void) {
}

f32 MB4_AngleDiff(f32 a, f32 b) {
    f32 diff;

    if (a >= 360.0f) {
        a -= 360.0f;
    } else if (a < 0.0f) {
        a += 360.0f;
    }
    if (b >= 360.0f) {
        b -= 360.0f;
    } else if (b < 0.0f) {
        b += 360.0f;
    }
    diff = a - b;
    if (diff <= -180.0f) {
        diff += 360.0f;
    } else if (diff >= 180.0f) {
        diff -= 360.0f;
    }
    return diff;
}

/* masu->flag bits used by this board's event dispatch; not part of the
 * shared MASU_FLAG_* set in game/board/masu.h (those start at bit 16). */
#define MB4_MASU_FLAG_HATENA 0x1
#define MB4_MASU_FLAG_IRUKA 0x10
#define MB4_MASU_FLAG_BLOW 0x40
#define MB4_MASU_FLAG_KAISOO 0x100

void MB4Ev_MasuHatena(s32 playerNo) {
    s16 masuId = GwPlayer[playerNo].masuId;
    MASU *masuP = MBMasuGet(MASU_LAYER_DEFAULT, masuId);
    if (masuP->flag & MB4_MASU_FLAG_HATENA) {
        MB4Ev_Kihon(playerNo);
    } else if (masuP->flag & MB4_MASU_FLAG_IRUKA) {
        MB4Ev_Iruka(playerNo);
    } else if (masuP->flag & MB4_MASU_FLAG_BLOW) {
        MB4Ev_Blow(playerNo);
    } else if (masuP->flag & MB4_MASU_FLAG_KAISOO) {
        MB4Ev_Kaisoo(playerNo);
    }
}

s32 MB4_MasuStub(void) {
    return 0;
}

s32 MB4Ev_MasuWalkPre(s32 playerNo) {
    return 0;
}

s32 MB4Ev_MasuWalkPost(s32 playerNo) {
    s16 masuId = GwPlayer[playerNo].masuId;
    MASU *masuP = MBMasuGet(MASU_LAYER_DEFAULT, masuId);
    if (masuP->flag & 0x200) {
        s32 *stateP;
        MBModelAttrReset((s16) MB4W_S32(0x18), 0x40000002U);
        MBMotionTimeSet((s16) MB4W_S32(0x18), 0.0f);
        MBMotionSpeedSet((s16) MB4W_S32(0x18), 1.0f);
        MBModelAttrReset((s16) MB4W_S32(0x1C), 0x40000002U);
        MBMotionTimeSet((s16) MB4W_S32(0x1C), 0.0f);
        MBMotionSpeedSet((s16) MB4W_S32(0x1C), 1.0f);
        MBModelDispSet((s16) MB4W_S32(0x1C), TRUE);
        HuAudFXEmiterPlay(0x5EF, &mb4ev_Puku2EmiterPos);
        stateP = &MB4W_S32(0x15C);
        stateP[0] = 3;
        stateP[1] = 0x12;
        stateP[2] = -1;
    }
    return 0;
}

/* NONMATCHING: register allocation keeps 3 more nonvolatile FPRs live than
 * the original (f23-f25 vs none) for the diff/absDiff temps; logic matches. */
f32 MB4_AngleLerp(f32 a, f32 b, f32 step) {
    f32 diff;
    f32 absDiff;
    f32 result;

    if (a >= 360.0) {
        a = (f32) ((f64) a - 360.0);
    } else if (a < 0.0) {
        a = (f32) ((f64) a + 360.0);
    }
    if (b >= 360.0) {
        b = (f32) ((f64) b - 360.0);
    } else if (b < 0.0) {
        b = (f32) ((f64) b + 360.0);
    }
    diff = (f32) (360.0 + (f64) (a - b));
    absDiff = (f32) fabs(diff);
    if (absDiff >= 360.0) {
        diff = (f32) fmod((f64) diff, 360.0);
    }
    if (diff < 180.0f) {
        result = (diff <= step) ? diff : step;
    } else {
        result = ((f32) (360.0 - (f64) diff) <= step) ? (f32) -(360.0 - (f64) diff) : -step;
    }
    result = result + b;
    if (result >= 360.0) {
        result = (f32) ((f64) result - 360.0);
    } else if (result < 0.0) {
        result = (f32) ((f64) result + 360.0);
    }
    return result;
}

void MB4_MapViewHook(s32 enterF) {
    if (enterF) {
        MBModelDispSet((s16) MB4W_S32(0x58), FALSE);
    } else {
        MBModelDispSet((s16) MB4W_S32(0x58), TRUE);
    }
}

s32 MB4_MasuPathCheck(s16 id, u32 flag, s16 *linkTbl, s32 endF) {
    return -1;
}

void MB4_ModelStub(HU3DMODEL *a, HU3DMODEL *b, HU3DMODEL *c, HU3DMODEL *d) {
}
