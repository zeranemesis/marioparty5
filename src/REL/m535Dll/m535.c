// m535Dll/REL/m535Dll/m535 - decompilation of the m535Dll/m535 unit.
//
// The overlay ships no symbol table for its internal helpers, so those keep
// objdiff's placeholder names fn_1_XXXX keyed to their .text offset.
// Only the small helpers are recovered here; the large flow/camera bodies
// (fn_1_A0 .. fn_1_B31C, ObjectSetup) are still absent and therefore only
// declared - they must not be defined here or MWCC will inline them into
// their callers and break the match.

#include "types.h"

#include "game/audio.h"
#include "game/charman.h"
#include "game/gamemes.h"
#include "game/hu3d.h"
#include "game/object.h"
#include "game/wipe.h"

#include "dolphin/mtx.h"

typedef void (*VoidFunc)(void);

extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

/* ---- overlay state ---- */

/* start of the wipe-out sequence (fn_1_BDC8 only fires once this is set) */
s32 lbl_1_bss_C;
/* object list index the wipe-out waits on */
s32 lbl_1_bss_8260;
/* message still on screen when the overlay quits, -1 when none */
s16 lbl_1_bss_825A;
/* the single character model object drawn by fn_1_97AC */
OMOBJ *lbl_1_bss_8268;
/* the four capsule objects torn down by fn_1_BE20 */
OMOBJ *lbl_1_bss_826C[4];
/* overlay history slot returned to when quitting */
s32 lbl_1_data_64;

/* bone name passed to Hu3DModelObjDraw by fn_1_97AC */
char lbl_1_data_C1B4[] = "test4-omote";

/* rodata scalars referenced by the helpers below */
f32 lbl_1_rodata_178 = 1.0f;
f32 lbl_1_rodata_218 = 0.5f;
f32 lbl_1_rodata_13C = 0.0f;
f32 lbl_1_rodata_940 = 400.0f;

/* fn_1_97AC reads a single s16 out of the object's model record at +0x2A and
 * feeds it straight back to Hu3DModelObjDraw as the model id. The shared
 * HU3DMODEL layout does not name a field at that offset, so the read is done
 * through this byte-offset view of the record header. */
typedef struct m535ModelRec_s {
    u8 pad[0x2A];
    s16 mdlId; /* 0x2A */
} M535MODELREC;

/* obj->data of the four capsule objects; the character number the model was
 * built for sits at +0x0C. */
typedef struct m535CharWork_s {
    u8 pad[0x0C];
    s32 charNo; /* 0x0C */
} M535CHARWORK;

/* PSMTXConcat takes pointer-to-const-array parameters, which this compiler
 * will not derive implicitly from a plain Mtx array or Mtx * argument. */
typedef const f32 (*M535MTXCPTR)[4];

void ObjectSetup(void);
void fn_1_A0(void);
void fn_1_69C(void);
f32 fn_1_2D68(f32 x, f32 a, f32 b, f32 w);
void fn_1_2F2C(void);
void fn_1_39A8(void);
void fn_1_53F8(OMOBJ *obj);
void fn_1_88F0(void);
void fn_1_8FD8(void);
void fn_1_97AC(OMOBJ *obj, Mtx *mtx);
void fn_1_9830(void);
void fn_1_9F2C(void);
void fn_1_A3C4(OMOBJ *obj);
void fn_1_B31C(OMOBJ *obj);
void fn_1_B700(OMOBJ *obj);
void fn_1_BDC8(OMOBJ *obj);
void fn_1_BE20(OMOBJ *obj);

int _prolog(void)
{
    const VoidFunc *ctors = _ctors;

    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const VoidFunc *dtors = _dtors;

    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

/* Weight ramp used by the overlay's fade/camera helpers: rises quadratically
 * from 0 at x=0 to 1 across the first "a" units, then climbs linearly to the
 * point "a+b", then falls back out of the window of width "w". "den" is the
 * normalisation shared by all four segments. */
f32 fn_1_2D68(f32 x, f32 a, f32 b, f32 w)
{
    f32 den = w * lbl_1_rodata_218 + (a * lbl_1_rodata_218 + b);
    f32 res;
    f32 mid;
    f32 hi;

    if (x < a) {
        res = (((lbl_1_rodata_178 + x) * lbl_1_rodata_218) * x) / a / den;
    }
    else {
        if (x < (a + b)) {
            mid = ((((lbl_1_rodata_178 + a) * lbl_1_rodata_218) * a) / a) / den + ((x - a) / den);
        }
        else {
            if (x < ((w + (a + b)) - lbl_1_rodata_178)) {
                hi = lbl_1_rodata_178
                    - (((lbl_1_rodata_178 + (((w + (a + b)) - x) - lbl_1_rodata_178)) * lbl_1_rodata_218)
                        * ((((w + (a + b)) - lbl_1_rodata_178) - x) - lbl_1_rodata_178))
                        / (w - lbl_1_rodata_178) / den;
            }
            else {
                hi = lbl_1_rodata_178;
            }
            mid = hi;
        }
        res = mid;
    }
    return res;
}

/* Draws the overlay's "test4" model bone above the object's own draw pass:
 * the object's callback runs first, then the bone is drawn with a fixed
 * 0/400/0 offset matrix post-multiplied by the caller's matrix. */
void fn_1_97AC(OMOBJ *obj, Mtx *mtx)
{
    Mtx boneMtx;

    Hu3DModelObjDrawInit();
    PSMTXTrans(boneMtx, lbl_1_rodata_13C, lbl_1_rodata_940, lbl_1_rodata_13C);
    PSMTXConcat((M535MTXCPTR)mtx, (M535MTXCPTR)boneMtx, boneMtx);
    Hu3DModelObjDraw(((M535MODELREC *)lbl_1_bss_8268->mdlId)->mdlId, lbl_1_data_C1B4, boneMtx);
}

/* Arms the wipe-in over 60 frames and hands the object over to the main
 * "in-game" callback. */
void fn_1_B700(OMOBJ *obj)
{
    lbl_1_bss_8260 = 1000;
    WipeCreate(WIPE_MODE_IN, WIPE_TYPE_PREVTYPE, 60);
    obj->objFunc = fn_1_53F8;
}

/* Fade-out kickoff, but only once the overlay's exit flag has been set. */
void fn_1_BDC8(OMOBJ *obj)
{
    if (lbl_1_bss_C != 0) {
        WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_WHITEFADE, 60);
        obj->objFunc = fn_1_BE20;
    }
}

/* Waits for the fade-out to finish, tears down the four character models and
 * the results message, then hands control back to the calling overlay. */
void fn_1_BE20(OMOBJ *obj)
{
    s32 i;
    M535CHARWORK *work;

    if (WipeCheckEnd() == 0) {
        for (i = 0; i < 4; i++) {
            work = (M535CHARWORK *)lbl_1_bss_826C[i]->data;
            CharModelKill(work->charNo);
        }
        if (lbl_1_bss_825A >= 0) {
            GameMesKill(lbl_1_bss_825A);
        }
        GameMesClose();
        HuAudAllStop();
        omOvlReturnEx(lbl_1_data_64, 1);
    }
}
