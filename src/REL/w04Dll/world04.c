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
static s16 PathCheck(s16 masuId, s16 depth);
static f32 MB4_PathTimeGet(Point3d *pPrev, Point3d *p1, Point3d *pNext, Point3d *unused, f32 tMax);
static f32 fn_1_DB68(f32 t, f32 pPrev, f32 p1, f32 pNext);
static f32 fn_1_DC9C(Point3d *pPrev, Point3d *p1, Point3d *pNext, f32 t);

static s16 mb4_PathValidF;
static s16 mb4_PathLen;
static s16 mb4_PathMasuNo;

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

/* NONMATCHING: register allocation differs from the original's manual
 * unroll of the first recursion levels (60% match); logic is complete and
 * faithful (depth-limited DFS toward the nearest star masu, used to drive
 * the "raise chance" star-distance display). */
static s16 PathCheck(s16 masuId, s16 depth) {
    MASU *masuP = MBMasuGet(MASU_LAYER_DEFAULT, masuId);
    s32 i;
    BOOL found;

    if (MBMasuTypeGet(MASU_LAYER_DEFAULT, masuId) == MASU_TYPE_STAR) {
        s16 dist = mb4_PathMasuNo - depth;
        if (dist < mb4_PathLen) {
            mb4_PathLen = dist;
        }
        mb4_PathValidF = 1;
        return 1;
    }
    if (MBMasuTypeGet(MASU_LAYER_DEFAULT, masuId) != MASU_TYPE_NONE) {
        depth -= 1;
    }
    found = FALSE;
    if (depth >= 0) {
        for (i = 0; i < masuP->linkNum; i++) {
            if ((MBMasuFlagGet(MASU_LAYER_DEFAULT, masuP->linkTbl[i]) & (MBBranchFlagGet() | 0x06000000)) == 0) {
                if (PathCheck((s16) masuP->linkTbl[i], depth) != 0) {
                    found = TRUE;
                }
            }
        }
    }
    return found ? 1 : 0;
}

/* Quadratic Bezier, applied per axis (x,y,z) over 3 flat f32 arrays. */
/* NONMATCHING: fewer nonvolatile FPRs live than the original's extra
 * register-to-register copies before the store (27% match); logic is
 * complete and faithful to the scalar MB4_BezierCalc(). */
void MB4_BezierCalcV(f32 *p0, f32 *p1, f32 *p2, f32 *out, f32 t) {
    s32 i;
    f32 p2v, p1v, p0v, omt, result;

    for (i = 0; i < 3; i++) {
        p2v = *p2;
        p2++;
        p1v = *p1;
        p1++;
        p0v = *p0;
        p0++;
        omt = (f32) (1.0 - (f64) t);
        result = (f32) ((f64) (p2v * (t * t)) + ((f64) (p0v * (omt * omt)) + (2.0 * (f64) (p1v * (omt * t)))));
        *out = result;
        out++;
    }
}

/* Quadratic Bezier: B(t) = p0*(1-t)^2 + 2*p1*(1-t)*t + p2*t^2 */
f32 MB4_BezierCalc(f32 t, f32 p0, f32 p1, f32 p2) {
    f32 omt = (f32) (1.0 - (f64) t);
    f32 result = (f32) ((f64) (p2 * (t * t)) + ((f64) (p0 * (omt * omt)) + (2.0 * (f64) (p1 * (omt * t)))));
    return result;
}

/* Hermite spline derivative (slope) at parameter t, blended over the two
 * endpoint positions (p0,p1) and two tangents (m0,m1): p0=farg1, p1=farg2,
 * m0=farg3, m1=farg4 in the original's calling convention. */
f32 MB4_HermiteSlopeCalc(f32 t, f32 p0, f32 p1, f32 m0, f32 m1) {
    f32 t2 = t * t;
    f32 a = (6.0f * t2) - (6.0f * t);
    f32 b = (-6.0f * t2) - (6.0f * t);
    f32 c = 1.0f + ((3.0f * t2) - (4.0f * t));
    f32 d = (3.0f * t2) - (2.0f * t);
    return ((a * p0) + (b * p1)) + (c * m0) + (d * m1);
}

/* NONMATCHING: written as a shared per-axis helper (the original inlines
 * this 3 times, once per x/y/z); logic and arithmetic ordering match the
 * decompiled body exactly, just factored out. Catmull-Rom-style tangents
 * (m0 from p0-pPrev, m1 from pNext-p1, both averaged with p1-p0) blended
 * with the standard cubic Hermite basis at t. */
static f32 MB4_HermiteCalcAxis(f32 pPrev, f32 p0, f32 p1, f32 pNext, f32 t) {
    f32 delta = p1 - p0;
    f32 m0, m1;
    f32 t2, t3, h1, h0, w0, w1;

    if (p0 == p1) {
        m0 = delta;
    } else {
        m0 = 0.5f * (delta + (p0 - pPrev));
    }
    if (p0 == pNext) {
        m1 = delta;
    } else {
        m1 = 0.5f * (delta + (pNext - p1));
    }
    t2 = t * t;
    t3 = t * t2;
    h1 = (f32) (((3.0 * (f64) t2) - (f64) t3) - (f64) t3);
    h0 = (f32) (1.0 - (f64) h1);
    w0 = t + ((t3 - t2) - t2);
    w1 = t3 - t2;
    return (w1 * m1) + (w0 * m0) + (h0 * p0) + (h1 * p1);
}

void MB4_HermiteCalcV(Point3d *pPrev, Point3d *p0, Point3d *p1, Point3d *pNext, Point3d *out, f32 t) {
    out->x = MB4_HermiteCalcAxis(pPrev->x, p0->x, p1->x, pNext->x, t);
    out->y = MB4_HermiteCalcAxis(pPrev->y, p0->y, p1->y, pNext->y, t);
    out->z = MB4_HermiteCalcAxis(pPrev->z, p0->z, p1->z, pNext->z, t);
}

/* Cubic Hermite basis weights for parameter t, one output per blend term. */
void MB4_HermiteCalc(f32 *w0, f32 *w1, f32 *w2, f32 *w3, f32 t) {
    f32 t2 = t * t;
    f32 t3 = t * t2;
    f32 h1 = (f32) (((3.0 * (f64) t2) - (f64) t3) - (f64) t3);
    *w0 = (f32) (1.0 - (f64) h1);
    *w1 = h1;
    *w2 = t + ((t3 - t2) - t2);
    *w3 = t3 - t2;
}

/* NONMATCHING: argument-reload ordering around the trailing 1.0f literal
 * differs (37% match); this is a straight forwarding call. */
/* NONMATCHING: written as a real loop calling the fn_1_DC9C helper 16
 * times instead of the original's full 16-way manual unroll; the algorithm
 * is exactly Simpson's rule (confirmed from the decompiled body: h =
 * tMax/16, endpoints weight 1, odd samples weight 4, even samples weight
 * 2, each sample being the curve-speed magnitude fn_1_DC9C(pPrev,p1,pNext,t)
 * - same formula reused for every sample in the original too) integrating
 * curve speed over [0, tMax] to get an arc-length-parameterized time; the
 * unused 4th Point3d argument matches the original's dead r6 parameter. */
static f32 MB4_PathTimeGet(Point3d *pPrev, Point3d *p1, Point3d *pNext, Point3d *unused, f32 tMax) {
    f32 h = tMax / 16.0f;
    f32 sum = fn_1_DC9C(pPrev, p1, pNext, 0.0f) + fn_1_DC9C(pPrev, p1, pNext, tMax);
    s32 i;

    for (i = 1; i < 16; i += 2) {
        sum += 4.0f * fn_1_DC9C(pPrev, p1, pNext, (f32) i * h);
    }
    for (i = 2; i < 16; i += 2) {
        sum += 2.0f * fn_1_DC9C(pPrev, p1, pNext, (f32) i * h);
    }
    return (h / 3.0f) * sum;
}

f32 MB4_PathNextTimeGet(Point3d *p0, Point3d *p1, Point3d *p2, Point3d *p3) {
    return MB4_PathTimeGet(p0, p1, p2, p3, 1.0f);
}

/* Derivative of the Catmull-Rom/Hermite-tangent curve segment [p1,pNext] at
 * parameter t, tangent at p1 computed from the symmetric neighbor formula
 * m = 0.5*(pNext-pPrev) (no separate tangent at pNext - only used where the
 * far end is a fixed direction, see callers). */
static f32 fn_1_DB68(f32 t, f32 pPrev, f32 p1, f32 pNext) {
    f32 m0 = 0.5f * ((pNext + (p1 - pPrev)) - p1);
    f32 m1 = 0.5f * ((pNext + (p1 - pPrev)) - p1);
    return m0 + ((t * (3.0f * (m1 + (m0 + ((2.0f * p1) - (2.0f * pNext)))) * t)) + (2.0f * ((((-3.0f * p1) + (3.0f * pNext)) - (2.0f * m0)) - m1) * t));
}

/* PowerPC fast reciprocal-sqrt estimate refined by 3 Newton-Raphson
 * iterations, matching the __frsqrte-based pattern used elsewhere in the
 * decomp (see w07Dll's MB7_FastSqrt for the exact reference). */
static f32 MB4_FastSqrt(f32 x) {
#ifdef TARGET_PC
    f64 guess = 1.0 / sqrt((f64) x);
#else
    f64 guess = __frsqrte((f64) x);
#endif
    guess = 0.5 * guess * (3.0 - ((f64) x * guess * guess));
    guess = 0.5 * guess * (3.0 - ((f64) x * guess * guess));
    guess = 0.5 * guess * (3.0 - ((f64) x * guess * guess));
    return (f32) ((f64) x * guess);
}

/* NONMATCHING: written using the fn_1_DB68 helper 3 times (once per axis)
 * plus MB4_FastSqrt, matching the decompiled body's arithmetic exactly;
 * the original inlines all of this instead. Speed (magnitude of the curve
 * derivative) of the [p1,pNext] segment at parameter t, used to normalize
 * animation playback rate against travel speed along the path. */
static f32 fn_1_DC9C(Point3d *pPrev, Point3d *p1, Point3d *pNext, f32 t) {
    f32 dx = fn_1_DB68(t, pPrev->x, p1->x, pNext->x);
    f32 dy = fn_1_DB68(t, pPrev->y, p1->y, pNext->y);
    f32 dz = fn_1_DB68(t, pPrev->z, p1->z, pNext->z);
    f32 magSq = (dz * dz) + ((dx * dx) + (dy * dy));
    if (magSq > 0.0f) {
        return MB4_FastSqrt(magSq);
    }
    return magSq;
}

s16 MB4_PathLenGet(s16 masuId, s16 depthLimit) {
    mb4_PathMasuNo = depthLimit;
    mb4_PathLen = depthLimit;
    mb4_PathValidF = 0;
    PathCheck(masuId, depthLimit);
    if (mb4_PathValidF) {
        return mb4_PathLen;
    }
    return -1;
}

void MB4_ModelStub(HU3DMODEL *a, HU3DMODEL *b, HU3DMODEL *c, HU3DMODEL *d) {
}

/* NONMATCHING: register allocation for the Hu3DData index differs (48%). */
void MB4_ModelMtxSet(s16 modelId, Mtx mtx) {
    s16 hu3dModelId = (s16) MBModelIdGet(modelId);
    PSMTXCopy(mtx, Hu3DData[hu3dModelId].mtx);
}

/* NONMATCHING: register allocation for the Hu3DData index differs (46%). */
void MB4_PlayerMtxSet(s16 playerNo, Mtx mtx) {
    s16 hu3dModelId = (s16) MBModelIdGet(MBPlayerModelGet(playerNo));
    PSMTXCopy(mtx, Hu3DData[hu3dModelId].mtx);
}
