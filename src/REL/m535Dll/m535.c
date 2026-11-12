// m535Dll/REL/m535Dll/m535 - decompilation of the m535Dll/m535 unit.
//
// The overlay ships no symbol table for its internal helpers, so those keep
// objdiff's placeholder names fn_1_XXXX keyed to their .text offset.
// Written so far: fn_1_B31C (capsule object build), fn_1_9F2C (the flat
// full-screen pass), fn_1_A0 (free-camera debug update), ObjectSetup,
// fn_1_A3C4 (results row callback), fn_1_2F2C (the 54-capsule board layout),
// fn_1_88F0 / fn_1_8FD8 / fn_1_9830 (the fan-rim, indirect-texture and
// billboard draw callbacks), on top of the small helpers.  The remaining
// bodies (fn_1_69C, fn_1_39A8, fn_1_53F8) are still absent and therefore only
// declared - they must not be defined here or MWCC will inline them into
// their callers and break the match.

#include "types.h"

#include "game/audio.h"
#include "game/charman.h"
#include "game/gamemes.h"
#include "game/hu3d.h"
#include "game/init.h"
#include "game/object.h"
#include "game/flag.h"
#include "game/gamework.h"
#include "game/pad.h"
#include "game/wipe.h"

#include "dolphin/mtx.h"

typedef void (*VoidFunc)(void);

extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

typedef struct {
    f32 x; /* 0x00 */
    f32 z; /* 0x04 */
} M535RINGPT;

/* the two-row indirect-texture matrix fn_1_8FD8 hands to GXSetIndTexMtx */
typedef struct {
    f32 m[2][3];
} M535INDMTX;

/* the closed 14-point rim outline fn_1_8FD8 copies to the stack */
typedef struct {
    M535RINGPT v[14];
} M535RINGTBL;

typedef struct {
    Vec v[4];
} M535BASETBL;

typedef struct m535Esprite_s {
    s16 memberNo; /* 0x00 */
    s16 animNo; /* 0x02 */
} M535ESPRITE;

typedef struct m535Esanm_s {
    u32 dataNum; /* 0x00 */
    u16 useCnt; /* 0x04 */
    ANIMDATA *anim; /* 0x08 */
} M535ESPANIM;

SHARED_SYM extern M535ESPRITE esprite[];
SHARED_SYM extern M535ESPANIM espanim[];
s32 lbl_1_bss_81DC[13];
s32 lbl_1_bss_81D8;
u8 lbl_1_bss_8284[12 * 0x14];

static const M535INDMTX lbl_1_rodata_8A8 = {
    {
        {0.0f, 1.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
    },
};

/* the 8- and 4-float rate tables fn_1_69C block-copies onto its stack; kept
 * as structs so MWCC emits the same word-wise aggregate copy the target uses */
typedef struct {
    f32 v[8];
} M535F8TBL;

typedef struct {
    f32 v[4];
} M535F4TBL;

static const M535F8TBL lbl_1_rodata_40 = {
    {8.0f, 7.0f, 5.5f, 4.5f, 1.5f, 1.5f, 1.5f, 1.5f}
};

static const M535F4TBL lbl_1_rodata_60 = {
    {20.0f, 1.0f, 0.0f, 0.0f}
};

static const M535F8TBL lbl_1_rodata_70 = {
    {1.0f, 1.0f, 1.0f, 1.0f, 3.0f, 2.5f, 1.0f, 0.5f}
};

static const f32 lbl_1_rodata_140 = 600.0f;

static const f32 lbl_1_rodata_148 = 255.0f;

static const f32 lbl_1_rodata_14C = 30.0f;

static const f32 lbl_1_rodata_150 = 150.0f;
s32 lbl_1_bss_1C;
s32 lbl_1_bss_8254;

/* a float view of a work-record slot this function widens with the shared
 * int->double bias so the C matches the target's (f32) widening */
typedef union {
    f32 f;
    s32 i;
} M535F32;

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
void fn_1_69C(OMOBJ *obj);
f32 fn_1_2D68(f32 x, f32 a, f32 b, f32 w);
void fn_1_2F2C(s32 arg0, s32 arg1);
void fn_1_39A8(OMOBJ *obj);
void fn_1_53F8(OMOBJ *obj);
void fn_1_88F0(void);
Mtx lbl_1_bss_100;
Mtx44 lbl_1_bss_D0;
GXTexObj lbl_1_bss_90[2];
s32 lbl_1_bss_4;
s32 lbl_1_bss_14;
s32 lbl_1_bss_24;
f32 lbl_1_bss_28;
void fn_1_8FD8(HU3DMODEL *modelP, Mtx *mtx);
void fn_1_97AC(OMOBJ *obj, Mtx *mtx);
void fn_1_9830(HU3DMODEL *modelP, Mtx *mtx);
void fn_1_9F2C(void);
void fn_1_A3C4(OMOBJ *obj);
void fn_1_B31C(OMOBJ *obj);
void fn_1_B700(OMOBJ *obj);
void fn_1_BDC8(OMOBJ *obj);
void fn_1_BE20(OMOBJ *obj);

/* work record of a capsule object, zeroed by fn_1_B31C and 0x340 bytes long */
typedef struct m535Work_s {
    /* 0x000 */ s32 unk00;
    /* 0x004 */ s32 unk04;
    /* 0x008 */ s32 unk08;
    /* 0x00C */ s32 charNo;
    /* 0x010 */ s32 unk10;
    /* 0x014 */ s32 unk14;
    /* 0x018 */ s32 unk18;
    /* 0x01C */ s32 unk1C;
    /* 0x020 */ s32 unk20;
    /* 0x024 */ s32 unk24; /* per-frame update counter fn_1_69C increments */
    /* 0x028 */ f32 unk28; /* capsule spawn x-offset, set by fn_1_A3C4 */
    /* 0x02C */ f32 unk2C; /* capsule spawn z-offset, set by fn_1_A3C4 */
    /* 0x030 */ f32 unk30;
    /* 0x034 */ f32 unk34;
    /* 0x038 */ s32 unk38;
    /* 0x03C */ s32 unk3C;
    /* 0x040 */ s32 unk40;
    /* 0x044 */ f32 unk44;
    /* 0x048 */ f32 unk48;
    /* 0x04C */ s32 unk4C;
    /* 0x050 */ s32 unk50;
    /* 0x054 */ M535F32 unk54; /* scheduled move delay / target cell index */
    /* 0x058 */ s32 unk58;
    /* 0x05C */ s32 unk5C;
    /* 0x060 */ s32 unk60;
    /* 0x064 */ s32 unk64; /* stall state: 0 idle, 1 armed, 2 held */
    /* 0x068 */ s32 unk68;
    /* 0x06C */ f32 unk6C[30];
    /* 0x0E4 */ f32 unkE4[30];
    /* 0x15C */ f32 unk15C[30];
    /* 0x1D4 */ f32 unk1D4[30];
    /* 0x24C */ s32 unk24C[30];
    /* 0x2C4 */ s32 unk2C4[30];
    /* 0x33C */ s32 unk33C;
} M535WORK;static const M535RINGTBL lbl_1_rodata_8C0 = {
    {
        {0.0f, 0.0f},
        {20.0f, 0.0f},
        {17.32f, -10.0f},
        {9.99999f, -17.32f},
        {-0.000001f, -20.0f},
        {-10.0f, -17.32f},
        {-17.32f, -10.0f},
        {-19.99999f, 0.0f},
        {-17.32f, 10.0f},
        {-9.99999f, 17.32f},
        {0.000001f, 19.99999f},
        {10.0f, 17.32f},
        {17.32f, 10.0f},
        {20.0f, 0.0f},
    },
};

static const M535BASETBL lbl_1_rodata_944 = {
    {
        {0.0f, 1.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {0.0f, -1.0f, 0.0f},
        {-1.0f, 0.0f, 0.0f},
    },
};

static const Vec lbl_1_rodata_974 = {0.0f, 0.0f, 0.0f};

static const GXColor lbl_1_rodata_880 = {0, 0, 0, 0};

static const GXColor lbl_1_rodata_8A4 = {0xFF, 0xFF, 0xFF, 0xFF};

static const f32 lbl_1_rodata_770 = 0.015625f;

static const f64 lbl_1_rodata_888 = 128.0;

static const f64 lbl_1_rodata_890 = 127.0;

static const f32 lbl_1_rodata_89C = 480.0f;

static const f32 lbl_1_rodata_8A0 = 0.1f;

static const f32 lbl_1_rodata_144 = 60.0f;

static const f32 lbl_1_rodata_194 = 20.0f;

static const f32 lbl_1_rodata_930 = -0.4f;

static const f32 lbl_1_rodata_934 = -0.05f;

static const f32 lbl_1_rodata_938 = 0.0078125f;

static const f32 lbl_1_rodata_93C = 0.25f;

/* seat table fn_1_B31C rebuilds from GwPlayerConf each time it runs */
static const s32 lbl_1_rodata_B4C[4] = {0, 1, 2, 3};
/* spawn scale handed to Hu3DModelAmbSet */
static const f32 lbl_1_rodata_B5C = 1.1f;
/* motion ids passed to CharMotionCreate, in object order */
static const u32 lbl_1_data_34[4] = {0x008C0000, 0x008700BC, 0x008C0006, 0x008C0031};
/* spawn position / initial y-rotation, one four-float row per seat */
static const f32 lbl_1_data_74[4][4] = {
    {-175.0f, 0.0f, -142.5f, 45.0f},
    {175.0f, 0.0f, -142.5f, -42.5f},
    {-142.5f, 0.0f, 0.0f, 90.0f},
    {225.0f, 0.0f, 0.0f, -90.0f},
};
/* free-camera debug step, nudge size for CenterM / CZoomM */
static const f64 lbl_1_rodata_10 = 25.0;
/* horizontal orbit nudge multiplier */
static const f64 lbl_1_rodata_18 = 0.03125;
/* vertical orbit nudge divisor */
static const f64 lbl_1_rodata_20 = -32.0;
/* PI, and the degree divisor for the spherical-to-cartesian orbit */
static const f64 lbl_1_rodata_28 = 3.141592653589793;
static const f64 lbl_1_rodata_30 = 180.0;
/* far plane handed to C_MTXOrtho by fn_1_9F2C */
static const f32 lbl_1_rodata_884 = -1.0f;
s32 lbl_1_data_6C = -1;
s32 lbl_1_bss_81B8[54];
char lbl_1_data_B4[] = "m535_omote";
char lbl_1_data_BF[] = "m535_ura";
static const f32 lbl_1_rodata_190 = 4.0f;
static const f32 lbl_1_rodata_1E0 = 3.0f;
static const f32 lbl_1_rodata_6B4 = 25.0f;
/* pan constants: 64 + 32 * (x / 250) */
static const f32 lbl_1_rodata_750 = 64.0f;
static const f32 lbl_1_rodata_7E0 = 32.0f;
static const f32 lbl_1_rodata_B60 = 250.0f;
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
        res = (((x + lbl_1_rodata_178) * lbl_1_rodata_218) * x) / a / den;
    }
    else {
        if (x < (a + b)) {
            mid = ((((a + lbl_1_rodata_178) * lbl_1_rodata_218) * a) / a) / den + ((x - a) / den);
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

/*
 * Builds one of the four capsule objects: it works out which seat this object
 * owns from the owner index the caller stored in work[1], clears its work
 * record, pulls the character / pad / difficulty out of that seat's
 * GwPlayerConf row, creates the character model and its four motions, and then
 * parks the position on the object before handing it to the main callback.
 */
void fn_1_B31C(OMOBJ *arg0)
{
    M535WORK *r31 = (M535WORK *) arg0->data;
    s32 sp8[4];
    s32 r27;
    s32 i;
    s32 j;

    sp8[0] = lbl_1_rodata_B4C[0];
    sp8[1] = lbl_1_rodata_B4C[1];
    sp8[2] = lbl_1_rodata_B4C[2];
    sp8[3] = lbl_1_rodata_B4C[3];
    memset(r31, 0, 0x340);
    r27 = 0;
    for (j = 0; j < 4; j++) {
        for (i = 0; i < 4; i++) {
            if (j != GwPlayerConf[i].grp) {
                continue;
            }
            sp8[r27] = i;
            r27 += 1;
        }
    }
    r31->unk00 = arg0->work[0x4C / 4];
    r31->unk04 = sp8[r31->unk00];
    if (r31->unk04 == 4) {
        r31->unk04 = r31->unk00;
    }
    r31->charNo = GwPlayerConf[r31->unk04].charNo;
    r31->unk08 = (r31->charNo < 9) ? r31->charNo : 9;
    r31->unk10 = (GwPlayerConf[r31->unk04].type != 0) ? -1 : GwPlayerConf[r31->unk04].padNo;
    r31->unk14 = (r31->unk10 != -1) ? 0 : GwPlayerConf[r31->unk04].dif;
    r31->unk20 = 2;
    r31->unk50 = -1;
    r31->unk33C = 0;
    r31->unk3C = 0;
    for (i = 0; i < 30; i++) {
        r31->unk2C4[i] = -1;
    }
    arg0->mdlId[0] = CharModelCreate((s16) r31->charNo, 2);
    Hu3DModelLayerSet(arg0->mdlId[0], 1);
    Hu3DModelAttrReset(arg0->mdlId[0], 0x40000002);
    Hu3DModelAttrSet(arg0->mdlId[0], 0x40000001);
    Hu3DModelCameraSet(arg0->mdlId[0], 1);
    Hu3DModelShadowSet(arg0->mdlId[0]);
    CharEffectLayerSet(6);
    Hu3DModelAmbSet(arg0->mdlId[0], lbl_1_rodata_B5C, lbl_1_rodata_B5C, lbl_1_rodata_B5C);
    r31->unk18 = 0;
    r31->unk1C = -1;
    for (i = 0; i < 4; i++) {
        arg0->mtnId[i] = CharMotionCreate((s16) r31->charNo, lbl_1_data_34[i]);
    }
    Hu3DMotionSet(arg0->mdlId[0], arg0->mtnId[r31->unk18]);
    Hu3DModelAttrSet(arg0->mdlId[0], 0x40000001);
    arg0->trans.x = lbl_1_data_74[r31->unk00][0];
    arg0->trans.y = lbl_1_data_74[r31->unk00][1];
    arg0->trans.z = lbl_1_data_74[r31->unk00][2];
    arg0->rot.y = lbl_1_data_74[r31->unk00][3];
    arg0->rot.x = (arg0->rot.z = 0.0f);
    CharModelVoicePanSet((s16) r31->charNo, (s32) (lbl_1_rodata_750 + lbl_1_rodata_7E0 * (arg0->trans.x / lbl_1_rodata_B60)));
    arg0->objFunc = fn_1_69C;
}
/*
 * The flat full-screen pass: it sets an orthographic projection covering the
 * whole screen, lays out a single-colour, single-texture immediate-mode
 * vertex format, and draws the unit quad as two triangles directly into the
 * FIFO.  The colour is a fixed 227/224/220 with full alpha.
 */
void fn_1_9F2C(void)
{
    Mtx44 sp8;
    Mtx sp38;
    s32 r31 = 0xE3;
    s32 r30 = 0xE0;
    s32 r29 = 0xDC;

    C_MTXOrtho(sp8, lbl_1_rodata_13C, lbl_1_rodata_178, lbl_1_rodata_13C, lbl_1_rodata_178, lbl_1_rodata_13C, lbl_1_rodata_884);
    GXSetProjection(sp8, GX_ORTHOGRAPHIC);
    PSMTXIdentity(sp38);
    GXLoadPosMtxImm(sp38, 0);
    GXSetNumTevStages(1);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, 0, 0x7D);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXCOORD0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_CLAMP, GX_AF_SPEC);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGB8, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);

    *(f32 *) (uintptr_t) 0xCC008000 = lbl_1_rodata_13C;
    *(f32 *) (uintptr_t) 0xCC008000 = lbl_1_rodata_13C;
    *(f32 *) (uintptr_t) 0xCC008000 = lbl_1_rodata_13C;
    *(u8 *) (uintptr_t) 0xCC008000 = r31;
    *(u8 *) (uintptr_t) 0xCC008000 = r30;
    *(u8 *) (uintptr_t) 0xCC008000 = r29;
    *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
    *(f32 *) (uintptr_t) 0xCC008000 = lbl_1_rodata_13C;
    *(f32 *) (uintptr_t) 0xCC008000 = lbl_1_rodata_13C;
    *(f32 *) (uintptr_t) 0xCC008000 = lbl_1_rodata_178;
    *(f32 *) (uintptr_t) 0xCC008000 = lbl_1_rodata_13C;
    *(f32 *) (uintptr_t) 0xCC008000 = lbl_1_rodata_13C;
    *(u8 *) (uintptr_t) 0xCC008000 = r31;
    *(u8 *) (uintptr_t) 0xCC008000 = r30;
    *(u8 *) (uintptr_t) 0xCC008000 = r29;
    *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
    *(f32 *) (uintptr_t) 0xCC008000 = lbl_1_rodata_178;
    *(f32 *) (uintptr_t) 0xCC008000 = lbl_1_rodata_13C;
    *(f32 *) (uintptr_t) 0xCC008000 = lbl_1_rodata_178;
    *(f32 *) (uintptr_t) 0xCC008000 = lbl_1_rodata_178;
    *(f32 *) (uintptr_t) 0xCC008000 = lbl_1_rodata_13C;
    *(u8 *) (uintptr_t) 0xCC008000 = r31;
    *(u8 *) (uintptr_t) 0xCC008000 = r30;
    *(u8 *) (uintptr_t) 0xCC008000 = r29;
    *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
    *(f32 *) (uintptr_t) 0xCC008000 = lbl_1_rodata_178;
    *(f32 *) (uintptr_t) 0xCC008000 = lbl_1_rodata_178;
    *(f32 *) (uintptr_t) 0xCC008000 = lbl_1_rodata_13C;
    *(f32 *) (uintptr_t) 0xCC008000 = lbl_1_rodata_178;
    *(f32 *) (uintptr_t) 0xCC008000 = lbl_1_rodata_13C;
    *(u8 *) (uintptr_t) 0xCC008000 = r31;
    *(u8 *) (uintptr_t) 0xCC008000 = r30;
    *(u8 *) (uintptr_t) 0xCC008000 = r29;
    *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
    *(f32 *) (uintptr_t) 0xCC008000 = lbl_1_rodata_13C;
    *(f32 *) (uintptr_t) 0xCC008000 = lbl_1_rodata_178;
}
/* Overlay state shared across the m535 translation units; these slots are
 * owned elsewhere in the overlay and only referenced here. */
OMOBJMAN *lbl_1_bss_825C;
s32 lbl_1_bss_8250;
OMOBJ *lbl_1_bss_8264;
OMOBJ *lbl_1_bss_827C;
OMOBJ *lbl_1_bss_8280;
s32 lbl_1_data_5C = -1;
void *lbl_1_bss_81D0;
u32 lbl_1_bss_81D4;
void *lbl_1_bss_8210[13];
extern s32 lbl_1_bss_81DC[13];
extern s32 lbl_1_bss_81D8;
extern u8 lbl_1_bss_8284[12 * 0x14];
u8 lbl_1_bss_8374[0xBD0];

extern void OSReport(const char *msg, ...);
extern u32 GXGetTexBufferSize(u16 width, u16 height, u32 format, GXBool mipmap, u8 max_lod);
extern s32 rand8(void);
extern s16 espEntry(u32 dataNum, s16 prio, s16 bank);
extern void espDispOff(s16 espId);

/*
 * Overlay entry: builds the object manager, sets up the two gameplay cameras
 * and their orbit state, creates the global light and the multi-map shadow,
 * then spawns the results object, the four capsule objects, the out-view,
 * wipe-in and wipe-out objects and a texture buffer used by the wipe pass.
 */
void ObjectSetup(void)
{
    s32 i;
    s32 cam;
    s32 x;
    s32 y;
    s32 w;
    s32 h;
    OMOBJ *obj;

    OSReport("******* M535 ObjectSetup *********\n");
    lbl_1_bss_825C = omInitObjMan(600, 8192);
    omGameSysInit(lbl_1_bss_825C);
    Hu3DCameraCreate(3);
    for (i = 0; i < 2; i++) {
        cam = (i == 0) ? 1 : 2;
        w = 0x280;
        h = w * 3 / 4;
        x = (0x280 - w) / 2;
        y = (0x1E0 - h) / 2;
        Hu3DCameraPerspectiveSet(cam, 25.0f, 20.0f, 10000.0f, 1.2f);
        Hu3DCameraViewportSet(cam, x, y, w, h, 0.0f, 1.0f);
        Hu3DCameraScissorSet(cam, x, y, w, h);
        CZoomM[i] = 1100.0f;
        CenterM[i].x = 0.0f;
        CenterM[i].y = 115.0f;
        CenterM[i].z = 0.0f;
        CRotM[i].x = -11.5f;
        CRotM[i].y = 0.0f;
        CRotM[i].z = 0.0f;
    }
    Hu3DLighInit();
    {
        HuVecF pos = {0.0f, 5000.0f, 0.0f};
        HuVecF dir = {0.0f, -1.0f, -0.5f};
        GXColor color = {0xFF, 0xFF, 0xFF, 0xFF};

        lbl_1_bss_8250 = Hu3DGLightCreateV(&pos, &dir, &color);
    }
    Hu3DGLightStaticSet(lbl_1_bss_8250, 1);
    Hu3DGLightInfinitytSet(lbl_1_bss_8250);
    {
        HuVecF spos;
        HuVecF sup;
        HuVecF stgt;

        spos.x = 0.0f;
        spos.y = 12000.0f;
        spos.z = -500.0f;
        sup.x = 0.0f;
        sup.y = 0.0f;
        sup.z = -1.0f;
        stgt.x = 0.0f;
        stgt.y = 60.0f;
        stgt.z = 0.0f;
        Hu3DShadowMultiCreate(3.0f, 200.0f, 20000.0f, 3);
        Hu3DShadowMultiPosSet(&spos, &sup, &stgt, 1);
        spos.y = 4500.0f;
        spos.z = -500.0f;
        stgt.z = 0.0f;
        Hu3DShadowMultiPosSet(&spos, &sup, &stgt, 2);
        Hu3DShadowMultiTPLvlSet(0.3f, 1);
        Hu3DShadowMultiTPLvlSet(0.4f, 2);
    }
    lbl_1_bss_825A = -1;
    lbl_1_data_5C = -1;
    lbl_1_bss_8268 = omAddObjEx(lbl_1_bss_825C, 101, 75, 2, 1, fn_1_A3C4);
    for (i = 0; i < 4; i++) {
        obj = omAddObjEx(lbl_1_bss_825C, 100, 1, 4, 0, fn_1_B31C);
        lbl_1_bss_826C[i] = obj;
        obj->data = HuMemDirectMallocNum(0, 0x340, 0x10000000);
        obj->work[0] = i;
    }
    lbl_1_bss_827C = omAddObjEx(lbl_1_bss_825C, 0x7FDA, 0, 0, -1, omOutViewMulti);
    lbl_1_bss_827C->work[0] = 2;
    lbl_1_bss_8280 = omAddObjEx(lbl_1_bss_825C, 0x66, 0, 0, -1, fn_1_B700);
    lbl_1_bss_8264 = omAddObjEx(lbl_1_bss_825C, 0x67, 1, 0, -1, fn_1_BDC8);
    lbl_1_bss_81D4 = GXGetTexBufferSize(64, 64, 3, 0, 0);
    lbl_1_bss_81D0 = HuMemDirectMallocNum(2, lbl_1_bss_81D4, 0x10000000);
    memset(lbl_1_bss_81D0, 0, lbl_1_bss_81D4);
}

/*
 * Results-screen setup.  Builds the eight result-board models, then randomly
 * assigns the 54 scored capsules (9 lanes x 6 slots) into per-row bss records,
 * wires up the hook passes, loads the sprite/animation data and finally hands
 * the object over to fn_1_39A8.
 */
void fn_1_A3C4(OMOBJ *obj)
{
    s32 i;
    s32 mi;
    s32 row;
    s32 grp;
    s32 ln;
    s32 idx;
    s32 v0;
    s32 v4;
    s32 k;
    void *mdl;
    OMOBJ *cap;

    mi = 0;
    obj->mdlId[mi] = Hu3DModelCreate(HuDataSelHeapReadNum(0x5D0000, 0x10000000, 2));
    Hu3DModelLayerSet(obj->mdlId[mi], 3);
    Hu3DModelAttrSet(obj->mdlId[mi], 0x40000001);
    Hu3DModelCameraSet(obj->mdlId[mi], 1);

    mi = 1;
    obj->mdlId[mi] = Hu3DModelCreate(HuDataSelHeapReadNum(0x5D0001, 0x10000000, 2));
    Hu3DModelLayerSet(obj->mdlId[mi], 3);
    Hu3DModelAttrSet(obj->mdlId[mi], 0x40000001);
    Hu3DModelCameraSet(obj->mdlId[mi], 1);
    Hu3DModelShadowMapSet(obj->mdlId[mi]);

    mi = 2;
    obj->mdlId[mi] = Hu3DModelCreate(HuDataSelHeapReadNum(0x5D0002, 0x10000000, 2));
    Hu3DModelLayerSet(obj->mdlId[mi], 3);
    Hu3DModelAttrSet(obj->mdlId[mi], 0x40000001);
    Hu3DModelCameraSet(obj->mdlId[mi], 2);
    Hu3DModelShadowMapSet(obj->mdlId[mi]);

    mi = 3;
    obj->mdlId[mi] = Hu3DModelCreate(HuDataSelHeapReadNum(0x5D0003, 0x10000000, 2));
    Hu3DModelLayerSet(obj->mdlId[mi], 3);
    Hu3DModelPosSet(obj->mdlId[mi], 0.0f, 0.0f, -60.0f);
    Hu3DModelCameraSet(obj->mdlId[mi], 2);
    Hu3DModelShadowMapSet(obj->mdlId[mi]);

    mi = 4;
    obj->mdlId[mi] = Hu3DModelCreate(HuDataSelHeapReadNum(0x5D0004, 0x10000000, 2));
    Hu3DModelLayerSet(obj->mdlId[mi], 3);
    Hu3DModelPosSet(obj->mdlId[mi], 0.0f, 0.0f, -60.0f);
    Hu3DModelAttrSet(obj->mdlId[mi], 0x40000002);
    Hu3DModelCameraSet(obj->mdlId[mi], 2);
    Hu3DModelAttrSet(obj->mdlId[mi], 0x40000001);

    mi = 5;
    obj->mdlId[mi] = Hu3DModelCreate(HuDataSelHeapReadNum(0x5D0005, 0x10000000, 2));
    Hu3DModelLayerSet(obj->mdlId[mi], 3);
    Hu3DModelAttrSet(obj->mdlId[mi], 0x40000002);
    Hu3DModelPosSet(obj->mdlId[mi], 0.0f, 0.0f, -60.0f);
    Hu3DModelCameraSet(obj->mdlId[mi], 2);
    Hu3DModelShadowSet(obj->mdlId[mi]);

    mi = 6;
    obj->mdlId[mi] = Hu3DModelCreate(HuDataSelHeapReadNum(0x5D0006, 0x10000000, 2));
    Hu3DModelLayerSet(obj->mdlId[mi], 5);
    Hu3DModelAttrSet(obj->mdlId[mi], 0x40000001);
    Hu3DModelPosSet(obj->mdlId[mi], 0.0f, 69.0f, -60.0f);
    Hu3DModelCameraSet(obj->mdlId[mi], 2);

    mi = 7;
    obj->mdlId[mi] = Hu3DModelCreate(HuDataSelHeapReadNum(0x5D0007, 0x10000000, 2));
    Hu3DModelLayerSet(obj->mdlId[mi], 5);
    Hu3DModelAttrSet(obj->mdlId[mi], 0x40000002);
    Hu3DModelPosSet(obj->mdlId[mi], 0.0f, 0.0f, -60.0f);
    Hu3DModelCameraSet(obj->mdlId[mi], 1);

    memset(lbl_1_bss_8374, 0, 0xBD0);
    {
        s32 laneCount[9] = {0};
        s32 used[9][6] = {0};

        for (row = 0; row < 0x36; row++) {
            for (;;) {
                do {
                    grp = rand8();
                } while (grp >= 0xFC);
                if (laneCount[grp % 9] < 6) {
                    grp = grp % 9;
                    break;
                }
            }
            for (;;) {
                do {
                    ln = rand8();
                } while (ln >= 0xFC);
                if (used[grp][ln % 6] < 1) {
                    ln = ln % 6;
                    break;
                }
            }
            idx = grp * 6 + ln;
            laneCount[grp]++;
            used[grp][ln]++;
            if (idx < 0x15) {
                v0 = (idx / 7) * 0x19 - 0x64;
            }
            else if (idx < 0x21) {
                v0 = ((idx - 0x15) / 4) * 0x19 - 0x19;
            }
            else {
                v0 = ((idx - 0x21) / 7) * 0x19 + 0x32;
            }
            *(f32 *)((u8 *)lbl_1_bss_8374 + row * 0x38) = v0;
            if (idx < 0x15) {
                v4 = (idx % 7) * 0x19 - 0x4B;
            }
            else if (idx < 0x21) {
                v4 = ((idx - 0x15) / 4) * 0x19;
            }
            else {
                v4 = ((idx - 0x21) % 7) * 0x19 - 0x4B;
            }
            *(f32 *)((u8 *)lbl_1_bss_8374 + row * 0x38 + 4) = v4;
            *(f32 *)((u8 *)lbl_1_bss_8374 + row * 0x38 + 0x10) = 70.0f + 0.125f * (f32)row;
            *(s32 *)((u8 *)lbl_1_bss_8374 + row * 0x38 + 0x2C) = 0;
            *(s32 *)((u8 *)lbl_1_bss_8374 + row * 0x38 + 0x34) = -1;
            *(s32 *)((u8 *)lbl_1_bss_8374 + row * 0x38 + 0x30) = -1;
            mi = row + 0x15;
            if (row == 0) {
                obj->mdlId[mi] = Hu3DModelCreate(HuDataSelHeapReadNum(0x5D0008, 0x10000000, 2));
            }
            else {
                obj->mdlId[mi] = Hu3DModelLink(obj->mdlId[0x15]);
            }
            Hu3DModelAttrSet(obj->mdlId[mi], 0x40000002);
            Hu3DModelPosSet(obj->mdlId[mi], 0.0f, *(f32 *)((u8 *)lbl_1_bss_8374 + row * 0x38 + 0x10), -60.0f);
            Hu3DModelLayerSet(obj->mdlId[mi], 3);
            Hu3DModelCameraSet(obj->mdlId[mi], 2);
            Hu3DModelShadowSet(obj->mdlId[mi]);
            Hu3DModelShadowDispOff(obj->mdlId[mi]);
            Hu3DModelShadowMapSet(obj->mdlId[mi]);
            Hu3DModelAttrSet(obj->mdlId[mi], 1);
        }
    }
    mi = 9;
    obj->mdlId[mi] = Hu3DHookFuncCreate((HU3DMODELHOOK)fn_1_8FD8);
    Hu3DModelLayerSet(obj->mdlId[mi], 4);
    Hu3DModelPosSet(obj->mdlId[mi], 0.0f, 69.0f, -60.0f);
    Hu3DModelCameraSet(obj->mdlId[mi], 2);

    mi = 0xB;
    obj->mdlId[mi] = Hu3DHookFuncCreate((HU3DMODELHOOK)fn_1_9830);
    Hu3DModelLayerSet(obj->mdlId[mi], 6);
    Hu3DModelCameraSet(obj->mdlId[mi], 2);

    mi = 0xC;
    obj->mdlId[mi] = Hu3DHookFuncCreate((HU3DMODELHOOK)fn_1_9F2C);
    Hu3DModelLayerSet(obj->mdlId[mi], 2);
    Hu3DModelCameraSet(obj->mdlId[mi], 1);

    for (k = 0; k < 0xD; k++) {
        struct {
            s32 datanum[13];
            s32 bank[13];
        } tbl = {
            {0x5D0019, 0x5D001A, 0x5D001B, 0x5D001C, 0x5D001D, 0x5D001E, 0x5D001F, 0x5D0020, 0x5D0021, 0x5D0022, 0x5D0023, 0x5D0024, 0},
            {0x5D0025, 0x5D0026, 0x5D0027, 0x5D0028, 0x5D0029, 0x5D002A, 0x5D002B, 0x5D002C, 0x5D002D, 0x5D002E, 0x5D002F, 0x5D0030, 0x5D0031},
        };

        lbl_1_bss_8210[k] = HuDataSelHeapReadNum(tbl.datanum[k], 0x10000000, 2);
        lbl_1_bss_81DC[k] = espEntry(tbl.bank[k], 0, 0);
        espDispOff(lbl_1_bss_81DC[k]);
    }
    lbl_1_bss_81D8 = espEntry(33, 0, 0);
    espDispOff(lbl_1_bss_81D8);

    fn_1_2F2C(0, 0);
    for (k = 0; k < 0x36; k++) {
        Hu3DAnimCreate(lbl_1_bss_8210[rand8() % 12], lbl_1_bss_8268->mdlId[k + 0x15], "m535_ura");
    }
    {
        s32 check[4] = {0};

        for (k = 0; k < 4; k++) {
            struct {
                s32 datanum[13];
            } af4 = {
                {0x5D0009, 0x5D000A, 0x5D000B, 0x5D000C, 0x5D000D, 0x5D000E, 0x5D000F, 0x5D0010, 0x5D0011, 0x5D0012, 0x5D0013, 0x5D0014, 0x5D0015},
            };
            f32 tab[4][2] = {
                {-24.0f, -40.0f},
                {-24.0f, -27.0f},
                {10.0f, -27.0f},
                {25.0f, -40.0f},
            };
            s32 pick;

            cap = lbl_1_bss_826C[k];
            for (;;) {
                row = rand8() % 4;
                if (check[row] < 1) {
                    check[row] = 1;
                    break;
                }
            }
            pick = *(s32 *)((u8 *)cap->data + 0x0C);
            mi = k + 0x0D;
            obj->mdlId[mi] = Hu3DModelCreate(HuDataSelHeapReadNum(af4.datanum[pick], 0x10000000, 2));
            Hu3DModelLayerSet(obj->mdlId[mi], 3);
            *(f32 *)((u8 *)cap->data + 0x28) = tab[k][0];
            *(f32 *)((u8 *)cap->data + 0x2C) = tab[k][1];
            Hu3DModelPosSet(obj->mdlId[mi], *(f32 *)((u8 *)cap->data + 0x28), 61.0f, *(f32 *)((u8 *)cap->data + 0x2C));
            Hu3DModelCameraSet(obj->mdlId[mi], 2);
            Hu3DModelAttrSet(obj->mdlId[mi], 1);
            Hu3DModelShadowSet(obj->mdlId[mi]);

            mi = k + 0x11;
            obj->mdlId[mi] = Hu3DModelCreate(HuDataSelHeapReadNum(0x5D0016, 0x10000000, 2));
            Hu3DModelLayerSet(obj->mdlId[mi], 6);
            Hu3DModelAttrSet(obj->mdlId[mi], 1);
            Hu3DModelCameraSet(obj->mdlId[mi], 2);
        }
    }
    obj->mtnId[0] = Hu3DJointMotion(obj->mdlId[0x0D], HuDataSelHeapReadNum(0x5D0017, 0x10000000, 2));
    obj->mtnId[1] = Hu3DJointMotion(obj->mdlId[0x0D], HuDataSelHeapReadNum(0x5D0018, 0x10000000, 2));
    for (k = 0; k < 4; k++) {
        Hu3DMotionSet(obj->mdlId[k + 0x0D], obj->mtnId[1]);
    }
    for (k = 0; k < 0xC; k++) {
        *(s32 *)((u8 *)lbl_1_bss_8284 + k * 0x14 + 0x10) = -1;
    }
    obj->objFunc = (OMOBJFUNC)fn_1_39A8;
}

/*
 * Free-camera debug update.  Reads the pad and nudges, for each of the two
 * cameras, the pivot (CenterM), the zoom (CZoomM) and the orbit angles
 * (CRotM), then converts the spherical orbit back to a cartesian eye
 * position.  The pad bit extractions and the double-precision trigonometry
 * are what the target does; the constants are lbl_1_rodata_10 (25, the zoom
 * step), _18 (0.03125, the horizontal nudge), _20 (-32, the vertical nudge
 * divisor), _28/_30 (PI and 180).
 */
void fn_1_A0(void)
{
    f32 cx;
    f32 cy;
    f32 cz;
    f32 rx;
    f32 ry;
    f32 rz;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 sp18;
    f32 sp1C;
    f32 sp20;
    f32 sp24;
    f32 sp28;
    f32 sp2C;
    s32 i;

    for (i = 0; i < 2; i++) {
        cx = CenterM[i].x + (f32) (lbl_1_rodata_10 * (f64) ((s32) ((HuPadBtn[0] & 2) != 0) - (s32) ((HuPadBtn[0] & 1) != 0)));
        CenterM[i].x = cx;
        cy = CenterM[i].y + (f32) (lbl_1_rodata_10 * (f64) ((s32) ((HuPadBtn[0] & 8) != 0) - (s32) ((HuPadBtn[0] & 4) != 0)));
        CenterM[i].y = cy;
        cz = CZoomM[i] + (f32) (lbl_1_rodata_10 * (f64) ((s32) ((HuPadBtn[0] & 0x400) != 0) - (s32) ((HuPadBtn[0] & 0x800) != 0)));
        CZoomM[i] = cz;
        ry = CRotM[i].y + (f32) ((f64) HuPadSubStkX[0] * lbl_1_rodata_18);
        CRotM[i].y = ry;
        rx = CRotM[i].x + (f32) ((f64) HuPadSubStkY[0] / lbl_1_rodata_20);
        CRotM[i].x = rx;
        rz = CRotM[i].z;

        /* spherical orbit back to a cartesian eye position; the target keeps
         * these in stack slots rather than in a named struct */
        dx = CenterM[i].x + (cz * (f32) ((f64) sin(((lbl_1_rodata_28 * (f64) ry) / lbl_1_rodata_30)) * (f64) cos(((lbl_1_rodata_28 * (f64) rx) / lbl_1_rodata_30))));
        dz = CenterM[i].y + (cz * (f32) -((f64) sin(((lbl_1_rodata_28 * (f64) rx) / lbl_1_rodata_30))));
        dy = CenterM[i].z + (cz * (f32) ((f64) cos(((lbl_1_rodata_28 * (f64) rx) / lbl_1_rodata_30)) * (f64) cos(((lbl_1_rodata_28 * (f64) ry) / lbl_1_rodata_30))));
        sp24 = dx;
        sp28 = dz;
        sp2C = dy;
        sp18 = CenterM[i].x;
        sp1C = CenterM[i].y;
        sp20 = CenterM[i].z;
    }
}
/* the 54 capsule slots laid out back to back, 0x38 bytes each */
#define M535_SLOT_S32(i, off) (*(s32 *)((u8 *)lbl_1_bss_8374 + (s32)(i) * 0x38 + (off)))
/*
 * Widens a 0/1 flag the target keeps in an integer register into a double via
 * the shared unsigned bias, so the C reproduces the same widening the target
 * does before multiplying it into the float rates.
 */
#define M535WIDEN(v) ((f64) (u32) (v))
#define M535_SLOT_F32(i, off) (*(f32 *)((u8 *)lbl_1_bss_8374 + (s32)(i) * 0x38 + (off)))

/*
 * Lays out the 54 capsules of one board: picks a lane-set, shuffles the per
 * column type order with the "restart if the first column matches the
 * already-placed one" guard, and writes the resulting permutation into
 * lbl_1_bss_81B8.  arg0 is the lane-set index, arg1 the column spread.
 */
void fn_1_2F2C(s32 arg0, s32 arg1)
{
    /* per-lane-set remaining counts the target block-copies from rodata */
    static const s32 permSrc[3][3][12] = {
        {
            {3, 3, 3, 3, 3, 6, 6, 6, 6, 5, 5, 5},
            {3, 3, 3, 3, 3, 9, 8, 7, 5, 4, 4, 2},
            {3, 3, 3, 3, 3, 12, 7, 6, 5, 4, 4, 1},
        },
        {
            {2, 2, 2, 2, 2, 2, 2, 8, 8, 8, 8, 8},
            {2, 2, 2, 2, 2, 2, 2, 12, 11, 8, 5, 4},
            {2, 2, 2, 2, 2, 2, 2, 10, 9, 8, 7, 6},
        },
        {
            {1, 1, 1, 1, 1, 1, 1, 1, 12, 12, 11, 11},
            {1, 1, 1, 1, 1, 1, 1, 1, 20, 15, 8, 3},
            {1, 1, 1, 1, 1, 1, 1, 1, 15, 15, 10, 6},
        },
    };
    s32 grid[11][9];
    s32 i;
    s32 a;
    s32 g;
    s32 ln;
    s32 idx;
    s32 grade;
    s32 cnt;
    s32 pos;
    s32 xr;
    s32 zr;
    s32 xr2;
    s32 zr2;
    s32 side;
    s32 tmp;
    /* per-lane-set remaining counts the target block-copies from rodata */
    s32 perm[3][3][12] = {
        {
            {3, 3, 3, 3, 3, 6, 6, 6, 6, 5, 5, 5},
            {3, 3, 3, 3, 3, 9, 8, 7, 5, 4, 4, 2},
            {3, 3, 3, 3, 3, 12, 7, 6, 5, 4, 4, 1},
        },
        {
            {2, 2, 2, 2, 2, 2, 2, 8, 8, 8, 8, 8},
            {2, 2, 2, 2, 2, 2, 2, 12, 11, 8, 5, 4},
            {2, 2, 2, 2, 2, 2, 2, 10, 9, 8, 7, 6},
        },
        {
            {1, 1, 1, 1, 1, 1, 1, 1, 12, 12, 11, 11},
            {1, 1, 1, 1, 1, 1, 1, 1, 20, 15, 8, 3},
            {1, 1, 1, 1, 1, 1, 1, 1, 15, 15, 10, 6},
        },
    };
    s32 order[12] = {0};
    s32 laneUsed[12] = {0};
    s32 laneCnt[9] = {0};
    s32 used[9][6] = {0};

    pos = rand8() % 3;

    for (a = 0; a < 11; a++) {
        for (g = 0; g < 9; g++) {
            grid[a][g] = -1;
        }
    }

    pos = rand8() % 3;

    for (i = 0; i < 12; i++) {
        for (;;) {
            do {
                a = rand8();
            } while (a >= 0xFC);
            g = a % 12;
            if (laneUsed[g] != 0) {
                continue;
            }
            if (i == 0 && g == lbl_1_data_6C) {
                continue;
            }
            break;
        }
        order[i] = g;
        laneUsed[g] = 1;
    }
    lbl_1_data_6C = order[0];

    cnt = 0;
    for (i = 0; i < 0x36; i++) {
        for (;;) {
            do {
                a = rand8();
            } while (a >= 0xFC);
            g = a % 9;
            if (laneCnt[g] < 6) {
                break;
            }
        }
        for (;;) {
            do {
                a = rand8();
            } while (a >= 0xFC);
            ln = a % 6;
            if (used[g][ln] < 1) {
                break;
            }
        }
        idx = g * 6 + ln;
        laneCnt[g] += 1;
        used[g][ln] += 1;
        M535_SLOT_S32(idx, 0x28) = cnt;
        xr = (s32)(lbl_1_rodata_178 + (lbl_1_rodata_190 + (M535_SLOT_F32(idx, 0x00) / lbl_1_rodata_6B4)));
        zr = (s32)(lbl_1_rodata_178 + (lbl_1_rodata_1E0 + (M535_SLOT_F32(idx, 0x04) / lbl_1_rodata_6B4)));
        grid[xr][zr] = cnt;
        perm[arg0][pos][cnt] -= 1;
        if (cnt == 0) {
            lbl_1_bss_81B8[perm[arg0][pos][cnt]] = idx;
        }
        if (perm[arg0][pos][cnt] == 0) {
            cnt += 1;
        }
    }

    i = 0;
    while (i < 3 - arg0) {
        idx = lbl_1_bss_81B8[i];
        xr = (s32)(lbl_1_rodata_178 + (lbl_1_rodata_190 + (M535_SLOT_F32(idx, 0x00) / lbl_1_rodata_6B4)));
        zr = (s32)(lbl_1_rodata_178 + (lbl_1_rodata_1E0 + (M535_SLOT_F32(idx, 0x04) / lbl_1_rodata_6B4)));
        grade = M535_SLOT_S32(idx, 0x28);
        if (grid[xr - 1][zr] == grade || grid[xr + 1][zr] == grade
            || grid[xr][zr - 1] == grade || grid[xr][zr + 1] == grade) {
            for (;;) {
                a = rand8();
                if (a == idx || a >= 0xD8) {
                    continue;
                }
                a = a % 0x36;
                if (M535_SLOT_S32(a, 0x28) == grade) {
                    continue;
                }
                break;
            }
            xr2 = (s32)(lbl_1_rodata_178 + (lbl_1_rodata_190 + (M535_SLOT_F32(a, 0x00) / lbl_1_rodata_6B4)));
            zr2 = (s32)(lbl_1_rodata_178 + (lbl_1_rodata_1E0 + (M535_SLOT_F32(a, 0x04) / lbl_1_rodata_6B4)));
            tmp = M535_SLOT_S32(a, 0x28);
            M535_SLOT_S32(a, 0x28) = grade;
            M535_SLOT_S32(idx, 0x28) = tmp;
            grade = grid[xr2][zr2];
            grid[xr2][zr2] = grid[xr][zr];
            grid[xr][zr] = grade;
            lbl_1_bss_81B8[i] = a;
            i = 1;
        }
        else {
            i += 1;
        }
    }

    side = arg1 >= 0 ? arg1 : -arg1;
    for (i = 0; i < 0x36; i++) {
        grade = M535_SLOT_S32(i, 0x28);
        M535_SLOT_S32(i, 0x1C + arg0 * 4) = order[grade];
        a = M535_SLOT_S32(i, 0x30 + side * 4);
        if (a != -1) {
            Hu3DAnimKill((s16) a);
        }
        M535_SLOT_S32(i, 0x30 + side * 4) = (s32)(s16) Hu3DAnimCreate(
            lbl_1_bss_8210[order[grade]], lbl_1_bss_8268->mdlId[i + 0x15],
            side == 0 ? lbl_1_data_B4 : lbl_1_data_BF);
    }
}


void fn_1_88F0(void)
{
    GXColor spC;
    s32 cA;
    s32 cB;
    s32 i;

    spC = lbl_1_rodata_880;
    C_MTXOrtho(lbl_1_bss_D0, lbl_1_rodata_13C, lbl_1_rodata_178, lbl_1_rodata_13C, lbl_1_rodata_178, lbl_1_rodata_13C, lbl_1_rodata_884);
    GXSetProjection(lbl_1_bss_D0, GX_ORTHOGRAPHIC);
    PSMTXIdentity(lbl_1_bss_100);
    GXLoadPosMtxImm(lbl_1_bss_100, 0);
    GXSetNumTevStages(1);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, 0, 0x7D);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXCOORD0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_CLAMP, GX_AF_SPEC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_SET);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 0x82);
    for (i = 0; i < 0x40; i++) {
        cA = (s32) (lbl_1_rodata_890 * cos(lbl_1_rodata_28 * (360.0f * ((f32) i * lbl_1_rodata_770)) / lbl_1_rodata_30) + lbl_1_rodata_888);
        cB = (s32) (lbl_1_rodata_890 * cos(lbl_1_rodata_28 * (2.0f * (360.0f * ((f32) i * lbl_1_rodata_770))) / lbl_1_rodata_30) + lbl_1_rodata_888);
        *(f32 *) (uintptr_t) 0xCC008000 = lbl_1_rodata_13C;
        *(f32 *) (uintptr_t) 0xCC008000 = (f32) i / lbl_1_rodata_89C;
        *(f32 *) (uintptr_t) 0xCC008000 = lbl_1_rodata_13C;
        *(u8 *) (uintptr_t) 0xCC008000 = 0;
        *(u8 *) (uintptr_t) 0xCC008000 = (u8) cB;
        *(u8 *) (uintptr_t) 0xCC008000 = (u8) cA;
        *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
        *(f32 *) (uintptr_t) 0xCC008000 = lbl_1_rodata_8A0;
        *(f32 *) (uintptr_t) 0xCC008000 = (f32) i / lbl_1_rodata_89C;
        *(f32 *) (uintptr_t) 0xCC008000 = lbl_1_rodata_13C;
        *(u8 *) (uintptr_t) 0xCC008000 = 0;
        *(u8 *) (uintptr_t) 0xCC008000 = (u8) cB;
        *(u8 *) (uintptr_t) 0xCC008000 = (u8) cA;
        *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
    }
    /* the strip needs 130 vertices and the loop only produced 128, so the
     * closing row repeats the last colour pair at index 64 */
    *(f32 *) (uintptr_t) 0xCC008000 = lbl_1_rodata_13C;
    *(f32 *) (uintptr_t) 0xCC008000 = (f32) i / lbl_1_rodata_89C;
    *(f32 *) (uintptr_t) 0xCC008000 = lbl_1_rodata_13C;
    *(u8 *) (uintptr_t) 0xCC008000 = 0;
    *(u8 *) (uintptr_t) 0xCC008000 = (u8) cB;
    *(u8 *) (uintptr_t) 0xCC008000 = (u8) cA;
    *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
    *(f32 *) (uintptr_t) 0xCC008000 = lbl_1_rodata_8A0;
    *(f32 *) (uintptr_t) 0xCC008000 = (f32) i / lbl_1_rodata_89C;
    *(f32 *) (uintptr_t) 0xCC008000 = lbl_1_rodata_13C;
    *(u8 *) (uintptr_t) 0xCC008000 = 0;
    *(u8 *) (uintptr_t) 0xCC008000 = (u8) cB;
    *(u8 *) (uintptr_t) 0xCC008000 = (u8) cA;
    *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
    GXSetCopyFilter(GX_FALSE, RenderMode->sample_pattern, GX_FALSE, RenderMode->vfilter);
    GXSetTexCopySrc(0, 0, 0x40, 0x40);
    GXSetTexCopyDst(0x40, 0x40, GX_CTF_GB8, GX_FALSE);
    GXSetCopyClear(spC, 0xFFFF);
    GXCopyTex(lbl_1_bss_81D0, GX_TRUE);
    DCFlushRange(lbl_1_bss_81D0, lbl_1_bss_81D4);
    GXPixModeSync();
    GXSetCopyFilter(RenderMode->aa, RenderMode->sample_pattern, GX_TRUE, RenderMode->vfilter);
}

void fn_1_8FD8(HU3DMODEL *modelP, Mtx *mtx)
{
    f32 sp24 = lbl_1_rodata_178;
    f32 sp20 = lbl_1_rodata_178;
    f32 sp1C = lbl_1_rodata_178;
    f32 sp18 = lbl_1_rodata_178;
    f32 fps = lbl_1_rodata_144;
    /* the target normalises the clock onto one second and then never reads
     * the result back */
    f32 phase = (f32) (lbl_1_bss_4 % (s32) fps) / fps;
    GXColor sp10 = lbl_1_rodata_8A4;
    GXColor spC;
    M535INDMTX sp28 = lbl_1_rodata_8A8;
    M535RINGTBL ring = lbl_1_rodata_8C0;
    f32 zScale = lbl_1_rodata_930;
    f32 zBias = lbl_1_rodata_934;
    s32 i;
    s32 j;
    s32 espId;

    GXLoadPosMtxImm(*mtx, GX_PNMTX0);
    GXSetNumTevStages(1);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, 0, 0x7D);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXCOORD0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_CLAMP, GX_AF_SPEC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_HALF, GX_CC_A0, GX_CC_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_SET);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    sp10.a = (u8) lbl_1_bss_14;
    spC = sp10;
    GXSetTevColor(GX_TEVREG1, spC);
    if (lbl_1_bss_8260 >= 0x3EE) {
        espId = lbl_1_bss_24;
    }
    else {
        espId = 0xC;
    }
    HuSprTexLoad(espanim[esprite[lbl_1_bss_81DC[espId]].animNo].anim, 0, GX_TEXMAP0,
                 GX_CLAMP, GX_CLAMP, GX_LINEAR);
    GXSetVtxDesc(GX_VA_TEX1, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_TEX_ST, GX_F32, 0);
    GXInitTexObj(&lbl_1_bss_90[1], lbl_1_bss_81D0, 0x40, 0x40, 3, GX_REPEAT, GX_REPEAT, GX_TRUE);
    GXLoadTexObj(&lbl_1_bss_90[1], GX_TEXMAP1);
    GXInitTexObjLOD(&lbl_1_bss_90[1], GX_LINEAR, GX_LINEAR, lbl_1_rodata_13C, lbl_1_rodata_13C,
                    lbl_1_rodata_13C, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, 0x3C, 0, 0x7D);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR0A0);
    GXSetNumIndStages(1);
    GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD1, GX_TEXMAP1);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_1, GX_ITS_1);
    GXSetIndTexMtx(GX_ITM_1, sp28.m, 0);
    GXSetTevIndWarp(GX_TEVSTAGE0, GX_INDTEXSTAGE0, GX_TRUE, GX_FALSE, GX_ITM_1);
    GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 0xE);
    for (i = 0; i < 0xD; i++) {
        f32 px;
        f32 pz;
        f32 tA;
        f32 tB;

        j = (i == 0) ? 0 : 0xD - (i - 1);
        pz = ring.v[j].z;
        px = ring.v[j].x;
        *(f32 *) (uintptr_t) 0xCC008000 = px;
        *(f32 *) (uintptr_t) 0xCC008000 = lbl_1_rodata_13C;
        *(f32 *) (uintptr_t) 0xCC008000 = pz;
        tA = lbl_1_rodata_218 + (ring.v[j].z / lbl_1_rodata_194) * lbl_1_rodata_218
            + zScale * (lbl_1_bss_28 * lbl_1_rodata_938 + zBias);
        tB = lbl_1_rodata_218 + (ring.v[j].x / lbl_1_rodata_194) * lbl_1_rodata_218
            + lbl_1_rodata_93C * zScale * (lbl_1_bss_28 * lbl_1_rodata_938 + zBias);
        *(f32 *) (uintptr_t) 0xCC008000 = tB;
        *(f32 *) (uintptr_t) 0xCC008000 = tA;
        /* the second texture set carries the same displacements a quarter of
         * the way back so the two lookups straddle the parallax offset */
        tA = lbl_1_rodata_218 + (ring.v[j].z / lbl_1_rodata_194) * lbl_1_rodata_218
            + zScale * (lbl_1_bss_28 * lbl_1_rodata_938 + zBias);
        tB = lbl_1_rodata_218 + (ring.v[j].x / lbl_1_rodata_194) * lbl_1_rodata_218
            + lbl_1_rodata_93C * zScale * (lbl_1_bss_28 * lbl_1_rodata_938 + zBias);
        *(f32 *) (uintptr_t) 0xCC008000 = tB;
        *(f32 *) (uintptr_t) 0xCC008000 = tA;
    }
    GXSetNumIndStages(0);
    GXSetTevDirect(GX_TEVSTAGE0);
}

void fn_1_9830(HU3DMODEL *modelP, Mtx *mtx)
{
    Mtx spA8;
    Mtx sp78;
    M535BASETBL sp48 = lbl_1_rodata_944;
    M535BASETBL sp18;
    HU3DCAMERA *cam;
    s32 camNo;
    s32 i;
    s32 j;
    f32 sc;
    u8 alpha;
    f32 ax0;
    f32 ay0;
    f32 az0;
    f32 ax1;
    f32 ay1;
    f32 az1;
    f32 ax2;
    f32 ay2;
    f32 az2;
    f32 ax3;
    f32 ay3;
    f32 az3;

    for (camNo = 0; camNo < 0x10; camNo++) {
        if ((modelP->cameraBit & (u16) (1 << camNo)) != 0) {
            break;
        }
    }
    cam = &Hu3DCamera[camNo];
    C_MTXLookAt(spA8, &cam->pos, &cam->up, &cam->target);
    PSMTXTranspose(spA8, sp78);
    PSMTXMultVecArray(sp78, sp48.v, sp18.v, 4);
    GXLoadPosMtxImm(*mtx, GX_PNMTX0);
    GXSetNumTevStages(1);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, 0, 0x7D);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXCOORD0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_CLAMP, GX_AF_SPEC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASA, GX_CC_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_SET);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    HuSprTexLoad(espanim[esprite[lbl_1_bss_81D8].animNo].anim, 0, GX_TEXMAP0,
                 GX_CLAMP, GX_CLAMP, GX_LINEAR);
    for (i = 0; i < 4; i++) {
        M535WORK *work = (M535WORK *) lbl_1_bss_826C[i]->data;

        for (j = 0; j < 30; j++) {
            Vec pos = lbl_1_rodata_974;

            if (work->unk2C4[j] < 0) {
                continue;
            }
            pos.x = work->unk6C[j];
            pos.y = work->unkE4[j];
            pos.z = work->unk15C[j];
            alpha = (u8) work->unk24C[j];
            sc = work->unk1D4[j];
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            az0 = pos.z + sp18.v[0].z * sc;
            ay0 = pos.y + sp18.v[0].y * sc;
            ax0 = pos.x + sp18.v[0].x * sc;
            *(f32 *) (uintptr_t) 0xCC008000 = ax0;
            *(f32 *) (uintptr_t) 0xCC008000 = ay0;
            *(f32 *) (uintptr_t) 0xCC008000 = az0;
            *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
            *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
            *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
            *(u8 *) (uintptr_t) 0xCC008000 = alpha;
            *(f32 *) (uintptr_t) 0xCC008000 = lbl_1_rodata_13C;
            *(f32 *) (uintptr_t) 0xCC008000 = lbl_1_rodata_13C;
            az0 = pos.z + sp18.v[1].z * sc;
            ay0 = pos.y + sp18.v[1].y * sc;
            ax0 = pos.x + sp18.v[1].x * sc;
            *(f32 *) (uintptr_t) 0xCC008000 = ax0;
            *(f32 *) (uintptr_t) 0xCC008000 = ay0;
            *(f32 *) (uintptr_t) 0xCC008000 = az0;
            *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
            *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
            *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
            *(u8 *) (uintptr_t) 0xCC008000 = alpha;
            *(f32 *) (uintptr_t) 0xCC008000 = lbl_1_rodata_178;
            *(f32 *) (uintptr_t) 0xCC008000 = lbl_1_rodata_13C;
            az0 = pos.z + sp18.v[2].z * sc;
            ay0 = pos.y + sp18.v[2].y * sc;
            ax0 = pos.x + sp18.v[2].x * sc;
            *(f32 *) (uintptr_t) 0xCC008000 = ax0;
            *(f32 *) (uintptr_t) 0xCC008000 = ay0;
            *(f32 *) (uintptr_t) 0xCC008000 = az0;
            *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
            *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
            *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
            *(u8 *) (uintptr_t) 0xCC008000 = alpha;
            *(f32 *) (uintptr_t) 0xCC008000 = lbl_1_rodata_178;
            *(f32 *) (uintptr_t) 0xCC008000 = lbl_1_rodata_178;
            az0 = pos.z + sp18.v[3].z * sc;
            ay0 = pos.y + sp18.v[3].y * sc;
            ax0 = pos.x + sp18.v[3].x * sc;
            *(f32 *) (uintptr_t) 0xCC008000 = ax0;
            *(f32 *) (uintptr_t) 0xCC008000 = ay0;
            *(f32 *) (uintptr_t) 0xCC008000 = az0;
            *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
            *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
            *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
            *(u8 *) (uintptr_t) 0xCC008000 = alpha;
            *(f32 *) (uintptr_t) 0xCC008000 = lbl_1_rodata_13C;
            *(f32 *) (uintptr_t) 0xCC008000 = lbl_1_rodata_178;
            GXPixModeSync();
        }
    }
}


extern f32 lbl_1_bss_0;

typedef struct {
    f32 v[20];
} M535F20TBL;

/* per-seat landing offsets blended in at the end of fn_1_69C: x, then z */
static const M535F20TBL lbl_1_rodata_9C = {
    {10.0f, 10.0f, 15.0f, 0.0f, 0.0f, 15.0f, 0.0f, 8.0f, 0.0f, 0.0f,
     0.0f, 0.0f, 5.0f, -3.0f, 0.0f, 5.0f, 0.0f, 22.0f, 0.0f, 9.0f}
};

static const M535F20TBL lbl_1_rodata_EC = {
    {0.0f, 0.0f, 0.0f, 0.0f, 15.0f, 0.0f, 15.0f, 10.0f, 0.0f, 10.0f,
     0.0f, 0.0f, 5.0f, -3.0f, 0.0f, 5.0f, 0.0f, -8.0f, 0.0f, 9.0f}
};

void fn_1_69C(OMOBJ *obj)
{
    M535WORK *work = (M535WORK *) obj->data;
    M535WORK *capWork[4];
    s16 sp8 = -1; /* scratch, carries -1 into the per-frame body */
    f32 f30 = lbl_1_rodata_13C; /* this player's stick X */
    f32 f31 = lbl_1_rodata_13C; /* this player's stick Y */
    f32 spB0 = lbl_1_rodata_13C; /* this player's A-press weight */
    f32 spAC = work->unk28;
    f32 spA8 = work->unk2C;
    s32 r26;
    s32 r27;
    s32 r24;
    s32 r25;
    s32 i;
    s32 j;
    s32 moved;
    s32 row;
    f32 dist;
    f32 dx;
    f32 dz;
    f32 cx;
    f32 cz;
    f32 rad;
    f32 blend;
    f32 tmpA;
    f32 tmpB;
    f32 tmpC;
    Vec wob;
    Mtx rotMtx;
    HU3DMODEL *model;

    (void) sp8;
    for (i = 0; i < 4; i++) {
        capWork[i] = (M535WORK *) lbl_1_bss_826C[i]->data;
    }
    work->unk24 = work->unk24 + 1;
    if (work->unk20 == 2) {
        work->unk38 = -1;
        if (lbl_1_bss_8260 == 0x3ef) {
            if (work->unk10 != -1) {
                /* human capsule: read the analog stick and the A button */
                f30 = (f32) (s8) HuPadStkX[work->unk10];
                f31 = (f32) (s8) HuPadStkY[work->unk10];
                spB0 = (f32) ((HuPadBtnDown[work->unk10] & 0x100) != 0);
            }
            else {
                /* CPU capsule: tally the human players still in play */
                r27 = 0;
                r26 = 0;
                for (i = 0; i < 4; i++) {
                    if (capWork[i]->unk10 != -1 && capWork[i]->unk20 != 4) {
                        r26 += 1;
                    }
                    if (capWork[i]->unk10 != -1) {
                        r27 += 1;
                    }
                }
                r24 = 1;
                if (r27 != 0 && r26 <= 0) {
                    r24 = 0;
                }
                if (lbl_1_bss_8260 == 0x3ef && lbl_1_bss_8254 == 0x258) {
                    M535F8TBL rateA = lbl_1_rodata_40;
                    M535F4TBL rateB = lbl_1_rodata_60;
                    f32 delay;

                    delay = 60.0f * (rateA.v[work->unk14]
                                     + ((rateA.v[work->unk14 + 4] * (f32) rand8()) / 255.0f));
                    work->unk54.i = (s32) (600.0f - ((f32) (u32) (r24 > 0) * delay));
                    work->unk5C = lbl_1_bss_81B8[rand8() % (3 - lbl_1_bss_1C)];
                    if (lbl_1_bss_8254 >= work->unk54.i) {
                        work->unk64 = 1;
                        work->unk68 = (s32) (30.0f + (150.0f * ((f32) rand8() / 255.0f)));
                    }
                    else {
                        work->unk64 = 0;
                    }
                    if (work->unk64 != 0) {
                        do {
                            work->unk5C = rand8() % 54;
                        } while (M535_SLOT_S32(work->unk5C, 0x28) == 0);
                    }
                    work->unk58 = 0;
                }
                if (lbl_1_bss_8254 < work->unk54.i && work->unk64 != 0) {
                    work->unk64 = 2;
                }
                if ((capWork[(work->unk00 + 1) % 4]->unk20 == 5
                     && (work->unk5C == capWork[(work->unk00 + 1) % 4]->unk38))
                    || (capWork[(work->unk00 + 2) % 4]->unk20 == 5
                        && (work->unk5C == capWork[(work->unk00 + 2) % 4]->unk38))
                    || (capWork[(work->unk00 + 3) % 4]->unk20 == 5
                        && (work->unk5C == capWork[(work->unk00 + 3) % 4]->unk38))
                    || work->unk64 == 2) {
                    M535F8TBL rateC = lbl_1_rodata_70;

                    if (lbl_1_bss_8254 < work->unk54.i) {
                        if (work->unk64 != 2) {
                            work->unk54.i = (s32) ((f32) lbl_1_bss_8254
                                - ((f32) (u32) (r24 > 0) * (60.0f * (rateC.v[work->unk14]
                                    + ((rateC.v[work->unk14 + 4] * (f32) rand8()) / 255.0f)))));
                        }
                        for (j = 0; j < 0x20; j++) {
                            work->unk5C = lbl_1_bss_81B8[rand8() % (3 - lbl_1_bss_1C)];
                            if (!((capWork[(work->unk00 + 1) % 4]->unk20 == 5
                                   && (work->unk5C == capWork[(work->unk00 + 1) % 4]->unk38))
                                  || (capWork[(work->unk00 + 2) % 4]->unk20 == 5
                                      && (work->unk5C == capWork[(work->unk00 + 2) % 4]->unk38))
                                  || (capWork[(work->unk00 + 3) % 4]->unk20 == 5
                                      && (work->unk5C == capWork[(work->unk00 + 3) % 4]->unk38)))) {
                                break;
                            }
                        }
                        if (j == 0x20) {
                            work->unk54.i = -1;
                        }
                        work->unk64 = 0;
                    }
                    else {
                        work->unk64 = 1;
                        work->unk68 = (s32) (30.0f + (30.0f * ((f32) rand8() / 255.0f)));
                        if (work->unk64 != 0) {
                            do {
                                work->unk5C = rand8() % 54;
                            } while (M535_SLOT_S32(work->unk5C, 0x28) == 0);
                        }
                    }
                    /* a cell at the edge that this capsule cannot reach straight on
                     * restarts the approach at state 0, otherwise it goes direct */
                    if (fabs((f64) M535_SLOT_F32(work->unk5C, 0x00)) >= 37.5
                        && (fabs((f64) work->unk28) < 37.5
                            || (work->unk28 * M535_SLOT_F32(work->unk5C, 0x00)) <= 0.0f)) {
                        work->unk58 = 0;
                    }
                    else {
                        work->unk58 = 3;
                    }
                }
                work->unk68 = work->unk68 - (work->unk68 > 0);
                if ((work->unk64 == 1 && work->unk68 == 0) || lbl_1_bss_8254 < work->unk54.i) {
                    switch (work->unk58) {
                    case 0:
                        work->unk58 = work->unk58 + 1;
                        /* fall through */
                    case 1:
                        if (work->unk2C < -10.0f) {
                            f31 = -70.0f;
                            break;
                        }
                        work->unk58 = work->unk58 + 1;
                        /* fall through */
                    case 2:
                        if (work->unk2C < -10.0f) {
                            f31 = -70.0f;
                        }
                        if (fabs((f64) M535_SLOT_F32(work->unk5C, 0x00)) >= 37.5
                            && (fabs((f64) work->unk28) < 37.5
                                || (work->unk28 * M535_SLOT_F32(work->unk5C, 0x00)) <= 0.0f)) {
                            if (M535_SLOT_F32(work->unk5C, 0x00) < 0.0f) {
                                r25 = -0x46;
                            }
                            else {
                                r25 = 0x46;
                            }
                            f30 = (f32) r25;
                            break;
                        }
                        work->unk58 = work->unk58 + 1;
                        /* fall through */
                    case 3:
                        f30 = M535_SLOT_F32(work->unk5C, 0x00) - work->unk28;
                        f31 = -(M535_SLOT_F32(work->unk5C, 0x04) - work->unk2C);
                        dist = sqrtf((f30 * f30) + (f31 * f31));
                        if (dist < (f32) (work->unk14 + 5)) {
                            if (work->unk64 != 0) {
                                work->unk64 = work->unk64 + (work->unk64 == 1);
                                if (work->unk14 < 1) {
                                    spB0 = (f32) ((rand8() % 4) == 0);
                                }
                            }
                            else {
                                spB0 = 1.0f;
                            }
                        }
                        break;
                    }
                }
                /* wobble the steering by a small random yaw */
                wob.x = 0.0f;
                wob.y = 0.0f;
                wob.z = 0.0f;
                wob.x = f30;
                wob.z = -f31;
                PSMTXRotRad(rotMtx, 'y', (f32) (0.017453292f * (45.0 * sin((3.141592653589793
                    * (f64) (((work->unk60 * 3) % 360) + (((rand8() % 2) * 2) - 1))) / 180.0))));
                PSMTXMultVec(rotMtx, &wob, &wob);
                f30 = wob.x * 4.0f;
                f31 = -wob.z * 4.0f;
            }
        }
        f30 /= 20.0f;
        f31 /= 20.0f;
        dist = sqrtf((f30 * f30) + (f31 * f31));
        tmpA = dist;
        if (tmpA > 1.0f) {
            f30 *= 1.0f / tmpA;
            f31 *= 1.0f / tmpA;
        }
        work->unk28 = (f32) ((f64) work->unk28 + (1.25 * (f64) f30));
        work->unk2C = (f32) ((f64) work->unk2C - (1.25 * (f64) f31));
    }
    else if (work->unk20 == 5) {
        dx = work->unk28 - M535_SLOT_F32(work->unk38, 0x00);
        dz = work->unk2C - M535_SLOT_F32(work->unk38, 0x04);
        dist = sqrtf((dx * dx) + (dz * dz));
        tmpA = dist;
        if (dist > 0.0f) {
            tmpB = (tmpA < 1.0f) ? 0.0f : tmpA - 1.0f;
            tmpC = (tmpA < 1.0f) ? 0.0f : tmpA - 1.0f;
            work->unk28 = ((dx / tmpA) * tmpB) + M535_SLOT_F32(work->unk38, 0x00);
            work->unk2C = ((dz / tmpA) * tmpC) + M535_SLOT_F32(work->unk38, 0x04);
        }
        work->unk30 = work->unk30 - 0.2f;
        work->unk30 = (work->unk30 < 0.0f) ? 0.0f : work->unk30;
        Hu3DModelPosSet(lbl_1_bss_8268->mdlId[work->unk00 + 0xD], work->unk28, 61.0f + work->unk30, work->unk2C);
    }
    else if (work->unk20 == 3) {
        work->unk38 = -1;
        if (work->unk24 >= 0x2D) {
            work->unk20 = 2;
            work->unk24 = 0;
        }
    }
    if (work->unk20 == 2 || work->unk20 == 3) {
        /* push apart from the other capsules */
        r25 = 0;
        for (i = 0; i < 4; i++) {
            if (work->unk00 != i && capWork[i]->unk20 != 4) {
                dx = capWork[i]->unk28 - work->unk28;
                dz = capWork[i]->unk2C - work->unk2C;
                dist = sqrtf((dx * dx) + (dz * dz));
                tmpA = dist;
                if (tmpA < 15.0f) {
                    work->unk28 = capWork[i]->unk28 - (15.0f * (dx / tmpA));
                    work->unk2C = capWork[i]->unk2C - (15.0f * (dz / tmpA));
                    r25 += 1;
                }
            }
        }
        if (r25 != 0) {
            row = work->unk60 + 1;
        }
        else {
            row = 0;
        }
        work->unk60 = row;
        work->unk28 = (work->unk28 < -105.0f) ? -105.0f : ((work->unk28 > 105.0f) ? 105.0f : work->unk28);
        work->unk2C = (work->unk2C < -80.0f) ? -80.0f : ((work->unk2C > 80.0f) ? 80.0f : work->unk2C);
        /* keep out of the three obstacles on the board */
        for (j = 0; j < 3; j++) {
            if (j == 0) {
                cx = 0.0f;
                cz = -60.0f;
                rad = 22.5f;
            }
            else {
                cx = work->unk28;
                cz = work->unk2C;
                if (j == 1) {
                    row = 0xA;
                }
                else {
                    row = 2;
                }
                rad = (f32) row;
                if (j == 1) {
                    cx = (cx < -20.0f) ? -20.0f : ((cx > 20.0f) ? 20.0f : cx);
                    cz = (cz < -100.0f) ? -100.0f : ((cz > -57.0f) ? -57.0f : cz);
                }
                else {
                    cx = (cx < -37.0f) ? -37.0f : ((cx > 37.0f) ? 37.0f : cx);
                    cz = (cz < -100.0f) ? -100.0f : ((cz > -63.0f) ? -63.0f : cz);
                }
            }
            dx = cx - work->unk28;
            dz = cz - work->unk2C;
            dist = sqrtf((dx * dx) + (dz * dz));
            tmpA = dist;
            if (tmpA < (7.5f + rad) && tmpA != 0.0f) {
                work->unk28 = cx - ((7.5f + rad) * (dx / tmpA));
                work->unk2C = cz - ((7.5f + rad) * (dz / tmpA));
            }
        }
        if (lbl_1_bss_8260 >= 0x3EE || (lbl_1_bss_8260 == 0x3ED && lbl_1_bss_0 >= 60.0f)) {
            work->unk30 = work->unk30 + 0.2f;
        }
        work->unk30 = (work->unk30 > 3.0f) ? 3.0f : work->unk30;
        Hu3DModelPosSet(lbl_1_bss_8268->mdlId[work->unk00 + 0xD], work->unk28, 61.0f + work->unk30, work->unk2C);
        if (work->unk20 == 2 && spB0 != 0.0f) {
            /* find the capsule slot this capsule is standing over */
            for (j = 0; j < 0x36; j++) {
                if ((M535_SLOT_F32(j, 0x00) - 12.5f) <= work->unk28
                    && work->unk28 <= (12.5f + M535_SLOT_F32(j, 0x00))
                    && (M535_SLOT_F32(j, 0x04) - 12.5f) <= work->unk2C
                    && work->unk2C <= (12.5f + M535_SLOT_F32(j, 0x04))) {
                    break;
                }
            }
            work->unk38 = j;
            work->unk50 = 0;
        }
    }
    if (work->unk50 >= 0) {
        if (work->unk50 == 0) {
            Hu3DModelAttrReset(lbl_1_bss_8268->mdlId[work->unk00 + 0x11], 1);
            Hu3DMotionTimeSet(lbl_1_bss_8268->mdlId[work->unk00 + 0x11], 0.0f);
        }
        Hu3DModelPosSet(lbl_1_bss_8268->mdlId[work->unk00 + 0x11], work->unk28, 61.0f, work->unk2C);
        if (work->unk50 >= 0x1E) {
            work->unk50 = -1;
            Hu3DModelAttrSet(lbl_1_bss_8268->mdlId[work->unk00 + 0x11], 1);
        }
        else {
            work->unk50 = work->unk50 + 1;
        }
    }
    moved = (work->unk20 == 2 && f30 != 0.0f && f31 != 0.0f && spAC != work->unk28 && spA8 != work->unk2C);
    work->unk33C = work->unk33C - moved;
    if (work->unk33C <= 0) {
        /* spawn a dust puff in the first free slot */
        for (j = 0; j < 0x1E; j++) {
            if (work->unk2C4[j] < 0) {
                work->unk2C4[j] = 0;
                work->unk6C[j] = (work->unk28 + (6.0f * ((f32) rand8() / 255.0f))) - 3.0f;
                work->unkE4[j] = 5.0f + (61.0f + (2.0f * ((f32) rand8() / 255.0f)));
                work->unk15C[j] = (work->unk2C + (6.0f * ((f32) rand8() / 255.0f))) - 3.0f;
                work->unk24C[j] = 0xFF;
                break;
            }
        }
        work->unk33C = (rand8() % 5) + 2;
    }
    for (j = 0; j < 0x1E; j++) {
        if (work->unk2C4[j] >= 0) {
            if (work->unk2C4[j] >= 0x28) {
                work->unk2C4[j] = -1;
            }
            else {
                work->unk2C4[j] = work->unk2C4[j] + 1;
            }
            work->unkE4[j] = work->unkE4[j] - 0.125f;
            if (work->unkE4[j] < 3.0f) {
                tmpA = 192.0f * (work->unkE4[j] / 3.0f);
            }
            else {
                tmpA = 192.0f;
            }
            work->unk24C[j] = (s32) tmpA;
            if (work->unk2C4[j] < 0x14) {
                tmpB = 1.0f;
            }
            else {
                tmpB = (f32) (0x28 - work->unk2C4[j]) / 20.0f;
            }
            work->unk1D4[j] = (f32) ((f64) tmpB * (3.5 * (0.25 + (0.75 * (f64) ((f32) rand8() / 255.0f)))));
        }
    }
    {
        M535F20TBL offX = lbl_1_rodata_9C;
        M535F20TBL offZ = lbl_1_rodata_EC;
        s32 side;
        s32 sgn;

        model = &Hu3DData[obj->mdlId[0]];
        if (model->motIdShift != -1) {
            blend = model->motOvlWork.start / model->motOvlWork.end;
            if (model->motId == obj->mtnId[1]) {
                blend = 1.0f - blend;
            }
            if (model->motId != obj->mtnId[1] && model->motIdShift != obj->mtnId[1]) {
                blend = 0.0f;
            }
        }
        else {
            s32 same;

            if (model->motId == obj->mtnId[1]) {
                same = 1;
            }
            else {
                same = 0;
            }
            blend = (f32) same;
        }
        if ((work->unk00 / 2) == 1) {
            side = 1;
        }
        else {
            side = 0;
        }
        if ((work->unk00 % 2) == 1) {
            sgn = -1;
        }
        else {
            sgn = 1;
        }
        obj->trans.x = lbl_1_data_74[work->unk00][0]
            + ((f32) sgn * (blend * offX.v[side * 10 + work->unk08]));
        if ((work->unk00 / 2) == 1) {
            side = 1;
        }
        else {
            side = 0;
        }
        obj->trans.z = lbl_1_data_74[work->unk00][2] + (blend * offZ.v[side * 10 + work->unk08]);
    }
}

/* ---- ripple surface / camera director (fn_1_39A8) ---- */
s32 lbl_1_data_70 = -1;
s32 lbl_1_data_68 = -1;
s32 lbl_1_data_C0C8 = -1;
s32 lbl_1_data_C8[3][64][64];
s32 lbl_1_data_C0CC[6][6];
s32 lbl_1_bss_10;
s32 lbl_1_bss_18;
f32 lbl_1_bss_81C4;
f32 lbl_1_bss_81C8;
f32 lbl_1_bss_81CC;
s32 lbl_1_bss_170[0x1000];
s32 lbl_1_bss_4170[0x1000];

/* sound-effect bookkeeping block */
#define M535_SE_S32(off) (*(s32 *) ((u8 *) lbl_1_bss_8284 + (off)))
#define M535_SE_F32(off) (*(f32 *) ((u8 *) lbl_1_bss_8284 + (off)))
/* clamp a ripple height into 1..0xFFFF */
#define M535_RIPCLAMP(v) (((v) < 1) ? 1 : (((v) > 0xFFFF) ? 0xFFFF : (v)))

extern f32 Hu3DMotionMaxTimeGet(HU3DMODELID id);

void fn_1_39A8(OMOBJ *obj)
{
    M535WORK *capWork[4];
    u32 sum;
    s32 total;
    s32 i;
    s32 x;
    s32 y;
    s32 k;
    s32 prev;
    s32 prev2;
    s32 cx;
    s32 cy;
    s32 v;
    s32 limit;
    s32 flag;
    s32 pick;
    s32 pickB;
    s32 idx;
    s32 j;
    f32 maxTime;
    f32 fx;
    f32 fy;
    f32 power;
    f32 span;
    f32 dist;
    f32 weight;
    f32 inv;
    f64 gain;
    f64 gainB;
    f32 sizeTbl[3];
    f32 zoomTbl[3];
    f32 centerTbl[3];
    f32 rotTbl[3];
    s32 pairTbl[3][2];
    s32 timeTbl[3][3];
    f32 dx;
    f32 dy;
    f32 a;
    f32 b;
    f32 c;

    for (i = 0; i < 4; i++) {
        capWork[i] = (M535WORK *) lbl_1_bss_826C[i]->data;
    }
    maxTime = Hu3DMotionMaxTimeGet(lbl_1_bss_8268->mdlId[7]);
    if (Hu3DMotionTimeGet(lbl_1_bss_8268->mdlId[7]) >= maxTime) {
        Hu3DModelAttrSet(lbl_1_bss_8268->mdlId[7], 1);
        Hu3DModelAttrSet(lbl_1_bss_8268->mdlId[4], 0x40000002);
        Hu3DMotionTimeSet(lbl_1_bss_8268->mdlId[7], 0.0f);
    }
    if (lbl_1_data_70 == 1 || lbl_1_data_70 == -1) {
        sum = 0;
        if (lbl_1_data_70 == -1) {
            lbl_1_data_70 = 0;
        }
        if (lbl_1_data_C0C8 == -1) {
            /* first run: clear the borders, build the bump kernel and the
             * texel -> swizzled-cell lookup tables */
            for (k = 0; k < 3; k++) {
                for (y = 0; y < 64; y++) {
                    for (x = 0; x < 64; x++) {
                        v = (x == 0 || y == 0 || x == 63 || y == 63) ? 0 : 1;
                        lbl_1_data_C8[k][y][x] = v;
                    }
                }
            }
            for (y = 0; y < 6; y++) {
                for (x = 0; x < 6; x++) {
                    dx = (f32) x - 2.5f;
                    dy = (f32) y - 2.5f;
                    dist = sqrtf(dx * dx + dy * dy);
                    if (dist < 2.5f) {
                        gain = 256.0 * (64.0 * ((1.0 + cos(3.141592653589793 * (f64) (180.0f * (dist / 2.5f)) / 180.0)) * 0.5));
                    }
                    else {
                        gain = 0.0;
                    }
                    lbl_1_data_C0CC[y][x] = (s32) gain;
                }
            }
            for (i = 0; i < 0x1000; i++) {
                lbl_1_bss_4170[i] = (i % 4) + (((i / 16) * 4) % 64);
                lbl_1_bss_170[i] = ((i / 4) % 4) + (((i / 16) * 4) / 64) * 4;
            }
        }
        lbl_1_data_C0C8 += 1;
        lbl_1_data_C0C8 %= 3;
        prev = (lbl_1_data_C0C8 + 2) % 3;
        prev2 = (lbl_1_data_C0C8 + 1) % 3;
        for (y = 1; y < 63; y++) {
            for (x = 1; x < 63; x++) {
                if (lbl_1_data_C8[lbl_1_data_C0C8][y][x] > 0) {
                    v = ((lbl_1_data_C8[prev][y - 1][x]
                          + (lbl_1_data_C8[prev][y][x - 1] + lbl_1_data_C8[prev][y][x + 1]
                             + lbl_1_data_C8[prev][y + 1][x])) / 2)
                        - lbl_1_data_C8[prev2][y][x];
                    lbl_1_data_C8[lbl_1_data_C0C8][y][x] = M535_RIPCLAMP(v);
                }
                sum += lbl_1_data_C8[lbl_1_data_C0C8][y][x] >> 8;
            }
        }
        /* drops: the scripted one (k == -1) plus random ones during play */
        for (k = -lbl_1_bss_18;; k++) {
            flag = 0;
            if (lbl_1_bss_8260 == 0x3EF && lbl_1_bss_8254 == 0x258) {
                flag = 1;
            }
            limit = flag ? 15 : ((lbl_1_bss_8260 == 0x3EF) ? 1 : 0);
            if (!(k < limit)) {
                break;
            }
            fx = (k == -1) ? 64.0f * lbl_1_bss_81CC : (f32) (rand8() % 64);
            cx = (s32) fx;
            fy = (k == -1) ? 64.0f * lbl_1_bss_81C8 : (f32) (rand8() % 64);
            cy = (s32) fy;
            sizeTbl[0] = 4.0f;
            sizeTbl[1] = 5.0f;
            sizeTbl[2] = 6.0f;
            span = (600.0f * sizeTbl[lbl_1_bss_1C]) / 10.0f;
            if (k == -1) {
                power = lbl_1_bss_81C4;
            }
            else {
                if ((f32) lbl_1_bss_8254 > (600.0f - span)) {
                    a = (3.0f * ((f32) lbl_1_bss_8254 - (600.0f - span))) / span;
                }
                else {
                    a = 0.0f;
                }
                power = a;
            }
            if (power > 2.5) {
                gain = 3.0;
            }
            else {
                if (power > 1.75) {
                    gainB = 1.0 + (2.0 * (((f64) power - 1.75) / 0.75));
                }
                else {
                    gainB = (f64) power / 1.75;
                }
                gain = gainB;
            }
            for (y = 0; y < 6; y++) {
                for (x = 0; x < 6; x++) {
                    i = x + (cx - 3);
                    j = y + (cy - 3);
                    if (i > 0 && i < 63 && j > 0 && j < 63 && lbl_1_data_C8[lbl_1_data_C0C8][j][i] > 0) {
                        v = (s32) ((f32) lbl_1_data_C8[lbl_1_data_C0C8][j][i]
                                   + ((f32) gain * (f32) lbl_1_data_C0CC[y][x]));
                        lbl_1_data_C8[lbl_1_data_C0C8][j][i] = M535_RIPCLAMP(v);
                    }
                }
            }
        }
        if (lbl_1_bss_8260 >= 0x3EF) {
            lbl_1_bss_14 = (lbl_1_bss_14 < 4) ? 0 : lbl_1_bss_14 - 4;
        }
        total = sum;
        lbl_1_bss_28 = (f32) sum * 0.015625f * 0.015625f;
        sum = sum / 3 / 64 / 64 - 3;
        for (i = 0; i < 0x1000; i++) {
            ((u8 *) lbl_1_bss_81D0)[i * 2] =
                0x80 - (lbl_1_data_C8[lbl_1_data_C0C8][lbl_1_bss_170[i]][lbl_1_bss_4170[i]] >> 8 >> 2);
            ((u8 *) lbl_1_bss_81D0)[i * 2 + 1] =
                0x80 - ((u32) (lbl_1_data_C8[lbl_1_data_C0C8][lbl_1_bss_170[i]][lbl_1_bss_4170[i]] >> 8 >> 2) >> 2);
        }
    }
    if (M535_SE_S32(0xB0) != -1) {
        limit = (lbl_1_bss_8260 == 0x3EF) ? 0x12C0 : 0x1770;
        if (total < limit || lbl_1_data_70 != 1) {
            if (M535_SE_S32(0xB0) != -1) {
                HuAudFXStop(M535_SE_S32(0xB0));
                M535_SE_S32(0xB0) = -1;
            }
            if (M535_SE_S32(0xB4) < 3) {
                M535_SE_F32(0xB8 + M535_SE_S32(0xB4) * 4) = 0.0f;
                M535_SE_S32(0xB4) += 1;
            }
        }
    }
    if (lbl_1_data_70 != 0) {
        Hu3DModelAttrSet(lbl_1_bss_826C[2]->mdlId[0], 1);
        Hu3DModelAttrSet(lbl_1_bss_826C[3]->mdlId[0], 1);
    }
    else {
        Hu3DModelAttrReset(lbl_1_bss_826C[2]->mdlId[0], 1);
        Hu3DModelAttrReset(lbl_1_bss_826C[3]->mdlId[0], 1);
    }
    pick = rand8() % 2;
    pickB = rand8() % 2;
    for (i = 0; i < 4; i++) {
        idx = pickB + (pick * 2);
        pick = ((i % 2) == 1) ? 1 - pick : pick;
        pickB = 1 - pickB;
        if (capWork[idx]->unk38 != -1 && capWork[idx]->unk20 == 2) {
            for (j = 1; j < 4; j++) {
                if (capWork[idx]->unk38 == capWork[(idx + j) % 4]->unk38
                    && (capWork[(idx + j) % 4]->unk20 == 2) == 0) {
                    break;
                }
            }
            Hu3DModelAttrReset(obj->mdlId[idx + 0xD], 0x40000002);
            if (j < 4 || capWork[idx]->unk38 == 0x36 || M535_SLOT_S32(capWork[idx]->unk38, 0x28) != 0) {
                capWork[idx]->unk20 = 3;
                Hu3DMotionSet(obj->mdlId[idx + 0xD], obj->mtnId[1]);
                omVibrate((s16) capWork[idx]->unk04, 0xC, 4, 2);
                if (M535_SE_S32(0x64) < 3) {
                    M535_SE_F32(0x68 + M535_SE_S32(0x64) * 4) = capWork[idx]->unk28 / 110.0f;
                    M535_SE_S32(0x64) += 1;
                }
            }
            else {
                capWork[idx]->unk20 = 5;
                Hu3DMotionSet(obj->mdlId[idx + 0xD], obj->mtnId[0]);
                omVibrate((s16) capWork[idx]->unk04, 0xC, 6, 6);
                if (M535_SE_S32(0x50) < 3) {
                    M535_SE_F32(0x54 + M535_SE_S32(0x50) * 4) = capWork[idx]->unk28 / 110.0f;
                    M535_SE_S32(0x50) += 1;
                }
            }
            capWork[idx]->unk24 = 0;
        }
    }
    lbl_1_bss_18 = 0;
    if (lbl_1_data_68 != -1) {
        lbl_1_bss_10 += 1;
        zoomTbl[0] = 500.0f;
        zoomTbl[1] = 1100.0f;
        zoomTbl[2] = 210.0f;
        centerTbl[0] = 70.0f;
        centerTbl[1] = 115.0f;
        centerTbl[2] = 140.0f;
        rotTbl[0] = -66.0f;
        rotTbl[1] = -11.5f;
        rotTbl[2] = -50.0f;
        pairTbl[0][0] = 1;
        pairTbl[0][1] = 2;
        pairTbl[1][0] = 2;
        pairTbl[1][1] = 0;
        pairTbl[2][0] = 0;
        pairTbl[2][1] = 1;
        timeTbl[0][0] = 35;
        timeTbl[0][1] = 35;
        timeTbl[0][2] = 50;
        timeTbl[1][0] = 15;
        timeTbl[1][1] = 2;
        timeTbl[1][2] = 40;
        timeTbl[2][0] = 10;
        timeTbl[2][1] = 30;
        timeTbl[2][2] = 50;
        c = (f32) timeTbl[lbl_1_data_68][2];
        b = (f32) timeTbl[lbl_1_data_68][1];
        a = (f32) timeTbl[lbl_1_data_68][0];
        weight = fn_1_2D68((f32) lbl_1_bss_10, a, b, c);
        inv = 1.0f - weight;
        for (i = 0; i < 2; i++) {
            CZoomM[i] = inv * zoomTbl[pairTbl[lbl_1_data_68][0]] + weight * zoomTbl[pairTbl[lbl_1_data_68][1]];
            CenterM[i].y = inv * centerTbl[pairTbl[lbl_1_data_68][0]] + weight * centerTbl[pairTbl[lbl_1_data_68][1]];
            CRotM[i].x = inv * rotTbl[pairTbl[lbl_1_data_68][0]] + weight * rotTbl[pairTbl[lbl_1_data_68][1]];
        }
        if (lbl_1_bss_10 >= timeTbl[lbl_1_data_68][0] + (timeTbl[lbl_1_data_68][1] + timeTbl[lbl_1_data_68][2])) {
            lbl_1_data_68 = -1;
            lbl_1_bss_10 = 0;
        }
    }
    DCFlushRange(lbl_1_bss_81D0, lbl_1_bss_81D4);
}

/* ---- main sequencer (fn_1_53F8) ---- */
f32 lbl_1_bss_0;
s32 lbl_1_bss_20;
s16 lbl_1_bss_8258;
s32 lbl_1_data_60 = -1;
u8 lbl_1_data_44[4] = {0, 1, 1, 1};
u8 lbl_1_data_48[4] = {0x14, 0x26, 8, 8};
s32 lbl_1_data_4C[4];
s32 lbl_1_data_C15C[4];
char lbl_1_data_C16C[] = "*** main mode error(%d)!\n";

#define M535_MDL(n) (lbl_1_bss_8268->mdlId[(n)])

void fn_1_53F8(OMOBJ *obj)
{
    M535WORK *capWork[4];
    s32 i;
    s32 j;
    s32 k;
    s32 cnt;
    s32 rank;
    s32 idx;
    s32 maxT;
    s32 curT;
    s32 mt;
    s32 flag;
    u32 attr;
    f32 t;
    f32 u;
    f32 scl;
    f32 y;
    f32 vy;
    f32 dist;
    f32 rem;
    f32 nx;
    f32 nz;
    f32 cur;
    f32 h;
    f32 p;
    f32 q;
    f64 s1;
    f64 s2;
    f32 d2;
    s32 rankTbl[4];
    f32 landTbl[8];

    lbl_1_bss_4 += 1;
    for (i = 0; i < 4; i++) {
        capWork[i] = (M535WORK *) lbl_1_bss_826C[i]->data;
    }
    for (i = 0; i < 12; i++) {
        s32 seIds[12] = {0x7C3, 0x7C4, 0x7C5, 0x7C6, 0x7C7, 0x7C8, 0x7C9, 0x7CA, 0x7CB, 0x7CC, 0x7CD, 0x7CE};

        if (M535_SE_S32(i * 0x14) > 3) {
            M535_SE_S32(i * 0x14) = 3;
        }
        if (M535_SE_S32(i * 0x14) > 0 && seIds[i] != -1) {
            s32 h2 = HuAudFXPlay(seIds[i]);

            M535_SE_S32(i * 0x14 + 0x10) = h2;
            HuAudFXPanning(h2, (s16) (0.5 + (f64) (64.0f + (32.0f * M535_SE_F32(i * 0x14 + (M535_SE_S32(i * 0x14) - 1) * 4 + 4)))));
            M535_SE_S32(i * 0x14) -= 1;
        }
    }
    if (omSysExitReq != 0 && WipeCheckEnd() == 0 && lbl_1_bss_C == 0) {
        HuAudFadeOut(1000);
        lbl_1_data_64 = 1;
        lbl_1_bss_C = 1;
    }
    switch (lbl_1_bss_8260) {
    case 0x3E8:
        lbl_1_bss_0 += 1.0f;
        if (WipeCheckEnd() == 0) {
            lbl_1_bss_8260 += 1;
            lbl_1_bss_0 = 0.0f;
            return;
        }
        return;
    case 0x3E9:
        lbl_1_bss_0 += 1.0f;
        if (lbl_1_bss_0 == 20.0f) {
            for (i = 0; i < 4; i++) {
                if (lbl_1_data_44[1] != 0) {
                    attr = 0;
                }
                else {
                    attr = 0x40000001;
                }
                Hu3DMotionShiftSet(lbl_1_bss_826C[i]->mdlId[0], lbl_1_bss_826C[i]->mtnId[1],
                                   (f32) lbl_1_data_4C[1], (f32) lbl_1_data_48[1], attr);
            }
            lbl_1_data_68 = 0;
        }
        if (lbl_1_bss_0 >= 70.0f) {
            lbl_1_bss_8260 += 1;
            lbl_1_bss_0 = 0.0f;
            return;
        }
        break;
    case 0x3EA:
        lbl_1_bss_0 += 1.0f;
        if (lbl_1_bss_0 == 1.0f) {
            Hu3DModelAttrReset(M535_MDL(4), 1);
            Hu3DModelAttrReset(M535_MDL(4), 0x40000002);
            Hu3DModelAttrReset(M535_MDL(5), 0x40000002);
            if (M535_SE_S32(0) < 3) {
                M535_SE_F32(4 + M535_SE_S32(0) * 4) = 0.0f;
                M535_SE_S32(0) += 1;
            }
            if (M535_SE_S32(0xC8) < 3) {
                M535_SE_F32(0xCC + M535_SE_S32(0xC8) * 4) = 0.0f;
                M535_SE_S32(0xC8) += 1;
            }
            for (i = 0; i < 0x36; i++) {
                M535_SLOT_F32(i, 0x10) = 70.0f;
            }
        }
        if (lbl_1_bss_0 >= 106.0f) {
            lbl_1_bss_8260 += 1;
            lbl_1_bss_0 = 0.0f;
            lbl_1_data_70 = 1;
            return;
        }
        break;
    case 0x3EB:
        if (lbl_1_bss_0 == 15.0f) {
            lbl_1_data_68 = 1;
            for (i = 0; i < 4; i++) {
                if (lbl_1_data_44[0] != 0) {
                    attr = 0;
                }
                else {
                    attr = 0x40000001;
                }
                Hu3DMotionShiftSet(lbl_1_bss_826C[i]->mdlId[0], lbl_1_bss_826C[i]->mtnId[0],
                                   (f32) lbl_1_data_4C[0], (f32) lbl_1_data_48[0], attr);
            }
        }
        cnt = 0;
        if ((s32) lbl_1_bss_0 < 0x36) {
            k = 0x35 - (s32) lbl_1_bss_0;
            if (k == 0x35 && M535_SE_S32(0x14) < 3) {
                M535_SE_F32(0x18 + M535_SE_S32(0x14) * 4) = 0.0f;
                M535_SE_S32(0x14) += 1;
            }
            Hu3DModelAttrReset(M535_MDL(k + 0x15), 1);
            Hu3DModelShadowMapObjReset(M535_MDL(k + 0x15));
            Hu3DModelShadowDispOn(M535_MDL(k + 0x15));
            M535_SLOT_S32(k, 0x2C) = 1;
            y = M535_SLOT_F32(k, 0x10);
            vy = 10.0f;
            M535_SLOT_F32(k, 0x14) = 10.0f;
            M535_SLOT_S32(k, 0x18) = 0;
            while (y > 61.0f || vy > 0.0f) {
                y += vy;
                vy -= 0.5f;
                M535_SLOT_S32(k, 0x18) += 1;
            }
            lbl_1_bss_18 = 1;
            lbl_1_bss_81C4 = 0.6f;
            lbl_1_bss_81CC = (f32) (0.5 + (0.125 * (f64) ((2.0f * ((f32) rand8() / 255.0f)) - 1.0f)));
            lbl_1_bss_81C8 = (f32) (0.5 + (0.125 * (f64) ((2.0f * ((f32) rand8() / 255.0f)) - 1.0f)));
            M535_SLOT_F32(k, 8) = (f32) (18.0 * (((f64) lbl_1_bss_81CC - 0.5) / 0.5));
            M535_SLOT_F32(k, 0xC) = (f32) (-60.0 + (18.0 * (((f64) lbl_1_bss_81C8 - 0.5) / 0.5)));
        }
        lbl_1_bss_0 += 1.0f;
        for (k = 0; k < 0x36; k++) {
            if (M535_SLOT_S32(k, 0x2C) > 0) {
                t = (f32) M535_SLOT_S32(k, 0x2C) / (f32) M535_SLOT_S32(k, 0x18);
                u = 1.0f - t;
                if (t <= 1.0f) {
                    M535_SLOT_F32(k, 0x10) += M535_SLOT_F32(k, 0x14);
                    if (t == 1.0f) {
                        M535_SLOT_F32(k, 0x10) = 61.0f;
                        Hu3DModelShadowDispOff(M535_MDL(k + 0x15));
                        Hu3DModelShadowMapSet(M535_MDL(k + 0x15));
                        if (k == 0x35 && M535_SE_S32(0x28) < 3) {
                            M535_SE_F32(0x2C + M535_SE_S32(0x28) * 4) = 0.0f;
                            M535_SE_S32(0x28) += 1;
                        }
                        if (k == 0 && M535_SE_S32(0xD8) != -1) {
                            HuAudFXStop(M535_SE_S32(0xD8));
                            M535_SE_S32(0xD8) = -1;
                        }
                    }
                    M535_SLOT_F32(k, 0x14) -= 0.5f;
                    Hu3DModelPosSet(M535_MDL(k + 0x15), (t * M535_SLOT_F32(k, 0)) + (u * M535_SLOT_F32(k, 8)),
                                    M535_SLOT_F32(k, 0x10),
                                    (t * M535_SLOT_F32(k, 4)) + (u * M535_SLOT_F32(k, 0xC)));
                    scl = (f32) (0.25 + (0.75 * (f64) t));
                    Hu3DModelScaleSet(M535_MDL(k + 0x15), scl, scl, scl);
                    M535_SLOT_S32(k, 0x2C) += 1;
                    cnt += 1;
                }
            }
        }
        if (cnt == 0) {
            lbl_1_bss_8260 += 1;
            lbl_1_bss_0 = 0.0f;
            for (i = 0; i < 4; i++) {
                capWork[i]->unk20 = 2;
            }
            return;
        }
        break;
    case 0x3EC:
        cnt = 0;
        if (((s32) lbl_1_bss_0 % 10) == 2 && ((s32) lbl_1_bss_0 / 10) < 4) {
            do {
                k = rand8() % 4;
            } while (lbl_1_data_C15C[k] == 1);
            lbl_1_data_C15C[k] = 1;
            capWork[k]->unk40 = 1;
            y = capWork[k]->unk44;
            vy = 14.4f;
            capWork[k]->unk48 = 14.4f;
            capWork[k]->unk4C = 0;
            while (y > 61.0f || vy >= 0.0f) {
                y += vy;
                vy -= 0.9f;
                capWork[k]->unk4C += 1;
            }
            Hu3DModelAttrReset(M535_MDL(k + 0xD), 1);
            lbl_1_bss_18 = 1;
            lbl_1_bss_81C4 = 1.75f;
            lbl_1_bss_81CC = 0.5f;
            lbl_1_bss_81C8 = 0.5f;
        }
        lbl_1_bss_0 += 1.0f;
        for (i = 0; i < 4; i++) {
            if (capWork[i]->unk40 > 0) {
                t = (f32) capWork[i]->unk40 / (f32) capWork[i]->unk4C;
                u = 1.0f - t;
                if (t <= 1.0f) {
                    if (t < 0.45f) {
                        scl = t / 0.45f;
                    }
                    else {
                        scl = 1.0f;
                    }
                    capWork[i]->unk44 += capWork[i]->unk48;
                    if (capWork[i]->unk40 == 3
                        && (lbl_1_data_C15C[0] + (lbl_1_data_C15C[1] + lbl_1_data_C15C[2] + lbl_1_data_C15C[3])) == 1
                        && M535_SE_S32(0x3C) < 3) {
                        M535_SE_F32(0x40 + M535_SE_S32(0x3C) * 4) = 0.0f;
                        M535_SE_S32(0x3C) += 1;
                    }
                    if (capWork[i]->unk40 == capWork[i]->unk4C - 1 && M535_SE_S32(0xDC) < 3) {
                        M535_SE_F32(0xE0 + M535_SE_S32(0xDC) * 4) = 0.0f;
                        M535_SE_S32(0xDC) += 1;
                    }
                    if (t == 1.0f) {
                        capWork[i]->unk44 = 61.0f;
                        omVibrate((s16) capWork[i]->unk04, 0xC, 6, 6);
                    }
                    capWork[i]->unk48 -= 0.9f;
                    nz = (f32) ((f64) ((-68.0f * u) + (t * capWork[i]->unk2C))
                                - ((f64) scl * (10.0 * sin((3.141592653589793 * (f64) (-600.0f * u)) / 180.0))));
                    Hu3DModelPosSet(M535_MDL(i + 0xD), t * capWork[i]->unk28,
                                    (f32) ((f64) (10.0f * scl)
                                           + ((f64) capWork[i]->unk44
                                              - ((f64) scl * (10.0 * cos((3.141592653589793 * (f64) (-600.0f * u)) / 180.0))))),
                                    nz);
                    Hu3DModelRotSet(M535_MDL(i + 0xD), -600.0f * u, 0.0f, 0.0f);
                    Hu3DModelScaleSet(M535_MDL(i + 0xD), scl, scl, scl);
                    capWork[i]->unk40 += 1;
                    cnt += 1;
                }
            }
        }
        if (cnt == 0 && ((s32) lbl_1_bss_0 / 10) >= 4) {
            lbl_1_bss_8260 += 1;
            lbl_1_bss_0 = 0.0f;
            return;
        }
        break;
    case 0x3ED:
        lbl_1_bss_0 += 1.0f;
        if (lbl_1_bss_0 == 5.0f && lbl_1_bss_825A < 0) {
            lbl_1_bss_825A = GameMesCreate(2, 0);
        }
        if (lbl_1_bss_0 >= 5.0f && lbl_1_data_60 == -1) {
            lbl_1_data_60 = HuAudSStreamPlay(0x34);
        }
        if (lbl_1_bss_0 >= 30.0f) {
            if (lbl_1_bss_0 <= 90.0f) {
                lbl_1_bss_14 = (s32) (255.0f * ((lbl_1_bss_0 - 30.0f) / 60.0f));
            }
        }
        if (lbl_1_bss_0 >= 5.0f && GameMesStatGet(lbl_1_bss_825A) == 0 && lbl_1_bss_825A >= 0) {
            lbl_1_bss_8260 += 1;
            lbl_1_bss_8254 = 0;
            lbl_1_bss_825A = -1;
            lbl_1_bss_0 = 0.0f;
            Hu3DModelAttrSet(M535_MDL(4), 1);
            return;
        }
        break;
    case 0x3EE:
        lbl_1_bss_0 += 1.0f;
        lbl_1_bss_8260 += 1;
        lbl_1_bss_8254 = 0x258;
        lbl_1_bss_0 = 0.0f;
        lbl_1_bss_8258 = GameMesCreate(1, lbl_1_bss_8254 / 60, -1, -1);
        GameMesPosSet(lbl_1_bss_8258, 288.0f, 64.0f);
        lbl_1_bss_24 = lbl_1_data_6C;
        Hu3DModelAttrReset(M535_MDL(7), 0x40000002);
        Hu3DModelAttrReset(M535_MDL(7), 1);
        Hu3DMotionTimeSet(M535_MDL(7), 0.0f);
        if (M535_SE_S32(0xA0) < 3) {
            M535_SE_F32(0xA4 + M535_SE_S32(0xA0) * 4) = 0.0f;
            M535_SE_S32(0xA0) += 1;
            return;
        }
        break;
    case 0x3EF:
        lbl_1_bss_0 += 1.0f;
        if (lbl_1_bss_8258 != -1) {
            GameMesDispSet(lbl_1_bss_8258, 1, (s16) ((lbl_1_bss_8254 + 0x3A) / 60));
        }
        lbl_1_bss_8254 -= lbl_1_bss_8254 > 0;
        cnt = (capWork[0]->unk20 == 5)
            + ((capWork[1]->unk20 == 5) + (capWork[2]->unk20 == 5) + (capWork[3]->unk20 == 5));
        if (lbl_1_bss_8254 <= 0 || cnt >= (3 - lbl_1_bss_1C)) {
            lbl_1_bss_8260 = 0x3F0;
            lbl_1_bss_0 = 0.0f;
            return;
        }
        break;
    case 0x3F0:
        lbl_1_bss_0 += 1.0f;
        if (lbl_1_bss_0 == 1.0f) {
            GameMesDispSet(lbl_1_bss_8258, 2, -1);
            for (i = 0; i < 4; i++) {
                if (capWork[i]->unk20 != 5 && capWork[i]->unk3C == 0) {
                    capWork[i]->unk3C = 3 - lbl_1_bss_1C;
                }
            }
            cnt = (capWork[0]->unk20 == 5)
                + ((capWork[1]->unk20 == 5) + (capWork[2]->unk20 == 5) + (capWork[3]->unk20 == 5));
            if (cnt <= 1) {
                lbl_1_bss_825A = GameMesCreate(2, 1);
                if (lbl_1_data_60 != -1) {
                    HuAudSStreamFadeOut(lbl_1_data_60, 0x64);
                }
                lbl_1_data_5C = 4;
                for (i = 0; i < 4; i++) {
                    if (capWork[i]->unk20 == 5) {
                        lbl_1_data_5C = i;
                        break;
                    }
                }
            }
            else {
                lbl_1_bss_1C = 4 - cnt;
                lbl_1_bss_20 += 1;
            }
        }
        if (lbl_1_bss_0 >= 30.0f) {
            if (lbl_1_bss_0 <= 150.0f) {
                cnt = 0;
                for (i = 0; i < 4; i++) {
                    if (capWork[i]->unk20 != 5 && capWork[i]->unk20 != 4) {
                        d2 = (capWork[i]->unk28 * capWork[i]->unk28)
                            + ((-60.0f - capWork[i]->unk2C) * (-60.0f - capWork[i]->unk2C));
                        dist = sqrtf(d2);
                        nx = capWork[i]->unk28 / dist;
                        nz = (-60.0f - capWork[i]->unk2C) / dist;
                        rem = dist - (300.0f * ((lbl_1_bss_0 - 30.0f) / 120.0f));
                        if (lbl_1_bss_0 == 30.0f) {
                            omVibrate((s16) capWork[i]->unk04, 0x14, 1, 0);
                            if (M535_SE_S32(0x78) < 3) {
                                M535_SE_F32(0x7C + M535_SE_S32(0x78) * 4) = capWork[i]->unk28 / 110.0f;
                                M535_SE_S32(0x78) += 1;
                            }
                        }
                        if (rem >= 0.0f) {
                            cur = (rem < 0.0f) ? 0.0f : rem;
                            p = 1.0f - (cur / dist);
                            q = 1.0f - p;
                            if (p < 0.5f) {
                                s1 = sin((3.141592653589793 * (90.0 * ((f64) p / 0.5))) / 180.0);
                            }
                            else {
                                s1 = 1.0;
                            }
                            Hu3DModelPosSet(M535_MDL(i + 0xD), q * capWork[i]->unk28, (f32) (61.0 + (30.0 * s1)),
                                            (-60.0f * p) + (q * capWork[i]->unk2C));
                            if (p < 0.5f) {
                                s2 = sin((3.141592653589793 * (90.0 * ((f64) p / 0.5))) / 180.0);
                            }
                            else {
                                s2 = 1.0;
                            }
                            Hu3DModelRotSet(M535_MDL(i + 0xD), (f32) (10.0 + (10.0 * s2)),
                                            (f32) -(180.0 * (atan2((f64) nx, (f64) nz) / 3.141592653589793)), 0.0f);
                            Hu3DModelScaleSet(M535_MDL(i + 0xD), 1.0f, 1.0f, 1.0f);
                            capWork[i]->unk34 = 0.0f;
                        }
                        else {
                            capWork[i]->unk34 += 1.0f;
                            if (capWork[i]->unk34 == 20.0f) {
                                lbl_1_bss_18 = 1;
                                lbl_1_bss_81C8 = 0.5f;
                                lbl_1_bss_81CC = 0.5f;
                                lbl_1_bss_81C4 = 1.5f;
                            }
                            if (capWork[i]->unk34 < 25.0f) {
                                p = capWork[i]->unk34 / 25.0f;
                                q = 1.0f - p;
                                Hu3DModelPosSet(M535_MDL(i + 0xD), 0.0f, 91.0f + (-30.0f * p), -60.0f);
                                Hu3DModelScaleSet(M535_MDL(i + 0xD), q, q, q);
                                Hu3DModelRotSet(M535_MDL(i + 0xD), 20.0f,
                                                (f32) ((f64) (380.0f * p)
                                                       + -(180.0 * (atan2((f64) nx, (f64) nz) / 3.141592653589793))),
                                                0.0f);
                            }
                            else {
                                Hu3DModelAttrSet(M535_MDL(i + 0xD), 1);
                                cnt += 1;
                            }
                        }
                    }
                    else {
                        cnt += 1;
                    }
                }
                if (cnt >= 4) {
                    lbl_1_bss_0 = 150.0f;
                }
            }
        }
        if (lbl_1_data_5C == -1 || GameMesStatGet(lbl_1_bss_825A) == 0) {
            flag = (lbl_1_data_5C == -1) ? 0xF : 0;
            if (lbl_1_bss_0 > (f32) (flag + 0x96)) {
                if (lbl_1_data_5C == -1) {
                    lbl_1_bss_8260 += 1;
                    lbl_1_bss_0 = 0.0f;
                }
                else {
                    lbl_1_bss_8260 = 0x3F2;
                    lbl_1_bss_0 = 0.0f;
                    lbl_1_data_68 = 2;
                    rank = 0;
                    rankTbl[0] = 0;
                    rankTbl[1] = 0;
                    rankTbl[2] = 0;
                    rankTbl[3] = 0;
                    for (i = 0; i < 4; i++) {
                        flag = 0;
                        for (j = 0; j < 4; j++) {
                            if (i == capWork[j]->unk3C) {
                                rankTbl[j] = rank;
                                flag = 1;
                            }
                        }
                        rank += flag;
                    }
                    for (i = 0; i < 4; i++) {
                        if (rankTbl[0] == rankTbl[1] && rankTbl[0] == rankTbl[2] && rankTbl[0] == rankTbl[3]) {
                            GWMgCoinBonusSet(capWork[i]->unk04, 3);
                        }
                        else {
                            GWMgCoinBonusSet(capWork[i]->unk04, (s16) rankTbl[i]);
                        }
                    }
                }
            }
        }
        if (lbl_1_data_5C == -1 && lbl_1_bss_0 > 150.0f) {
            if (lbl_1_bss_0 <= 165.0f) {
                for (i = 0; i < 4; i++) {
                    if (capWork[i]->unk20 == 5) {
                        Hu3DModelAttrSet(M535_MDL(i + 0xD), 0x40000002);
                        Hu3DMotionTimeSet(M535_MDL(i + 0xD), 15.0f - ((lbl_1_bss_0 - 30.0f) - 120.0f));
                    }
                }
                return;
            }
        }
        break;
    case 0x3F1: {
        f32 landPos[8] = {-25.0f, -38.0f, -10.0f, -27.0f, 10.0f, -27.0f, 25.0f, -38.0f};

        lbl_1_bss_0 += 1.0f;
        if (lbl_1_bss_0 == 1.0f) {
            fn_1_2F2C(lbl_1_bss_1C, lbl_1_bss_20);
            if (M535_SE_S32(0x8C) < 3) {
                M535_SE_F32(0x90 + M535_SE_S32(0x8C) * 4) = 0.0f;
                M535_SE_S32(0x8C) += 1;
            }
        }
        maxT = (s32) Hu3DMotionMaxTimeGet(M535_MDL(i + 0x15));
        flag = 0;
        if (lbl_1_bss_0 >= 0.0f) {
            if (lbl_1_bss_0 <= (f32) maxT) {
                flag = 1;
            }
        }
        if (flag != 0) {
            cur = lbl_1_bss_0;
        }
        else {
            cur = (f32) maxT;
        }
        curT = (s32) cur;
        for (j = 0; j < 4; j++) {
            if (capWork[j]->unk20 == 5) {
                t = (f32) curT / (f32) maxT;
                u = 1.0f - t;
                h = 1.0f - ((((t - 0.5f) * (t - 0.5f)) / 0.5f) / 0.5f);
                Hu3DModelPosSet(M535_MDL(j + 0xD), (u * capWork[j]->unk28) + (t * landPos[j * 2]), 61.0f + (70.0f * h),
                                (u * capWork[j]->unk2C) + (t * landPos[j * 2 + 1]));
            }
        }
        lbl_1_bss_14 = (s32) ((255.0f * lbl_1_bss_0) / (f32) maxT);
        lbl_1_bss_14 = (lbl_1_bss_14 < 0) ? 0 : ((lbl_1_bss_14 > 0xFF) ? 0xFF : lbl_1_bss_14);
        if ((lbl_1_bss_20 % 2) == 0) {
            mt = maxT - curT;
        }
        else {
            mt = curT;
        }
        for (k = 0; k < 0x36; k++) {
            Hu3DMotionTimeSet(M535_MDL(k + 0x15), (f32) mt);
            if (lbl_1_bss_0 == 1.0f) {
                Hu3DModelShadowMapObjReset(M535_MDL(k + 0x15));
                Hu3DModelShadowDispOn(M535_MDL(k + 0x15));
            }
            else if (lbl_1_bss_0 == (f32) (maxT - 1)) {
                Hu3DModelShadowDispOff(M535_MDL(k + 0x15));
                Hu3DModelShadowMapSet(M535_MDL(k + 0x15));
            }
        }
        if (lbl_1_bss_0 == (f32) (maxT + 0x1E)) {
            lbl_1_bss_8260 = 0x3EE;
            lbl_1_bss_8254 = 0;
            lbl_1_bss_825A = -1;
            lbl_1_bss_0 = 0.0f;
            for (i = 0; i < 4; i++) {
                if (capWork[i]->unk20 == 5) {
                    capWork[i]->unk20 = 2;
                    capWork[i]->unk28 = landPos[i * 2];
                    capWork[i]->unk2C = landPos[i * 2 + 1];
                }
                else {
                    capWork[i]->unk20 = 4;
                }
                capWork[i]->unk24 = 0;
            }
            return;
        }
        break;
    }
    case 0x3F2:
        lbl_1_bss_0 += 1.0f;
        if (lbl_1_bss_0 == 5.0f) {
            lbl_1_data_70 = 0;
        }
        if (lbl_1_bss_0 == 76.0f) {
            for (i = 0; i < 4; i++) {
                idx = (lbl_1_data_5C == i) ? 2 : 3;
                if (lbl_1_data_44[idx] != 0) {
                    attr = 0;
                }
                else {
                    attr = 0x40000001;
                }
                Hu3DMotionShiftSet(lbl_1_bss_826C[i]->mdlId[0], lbl_1_bss_826C[i]->mtnId[idx],
                                   (f32) lbl_1_data_4C[idx], (f32) lbl_1_data_48[idx], attr);
                if (lbl_1_data_5C == 4) {
                    CharFXPlay((s16) capWork[i]->charNo, 0x200);
                }
            }
            if (lbl_1_data_5C != 4) {
                HuAudSStreamPlay(0x79);
            }
            else {
                HuAudSStreamPlay(0x7B);
            }
        }
        if (lbl_1_bss_0 >= 76.0f) {
            for (i = 0; i < 4; i++) {
                if (lbl_1_data_5C == i || capWork[i]->unk08 != 6 || capWork[i]->unk00 < 2) {
                    lbl_1_bss_826C[i]->rot.y *= 0.8f;
                }
            }
        }
        for (i = 0; i < 4; i++) {
            if (lbl_1_data_5C == 4 || lbl_1_data_5C == i / 2) {
                if (capWork[i]->unk20 == 7) {
                    k = 9;
                }
                else {
                    k = capWork[i]->unk20;
                }
                capWork[i]->unk20 = k;
            }
        }
        if (lbl_1_bss_0 > 286.0f && lbl_1_bss_C == 0) {
            lbl_1_bss_825A = -1;
            lbl_1_bss_C = 1;
            return;
        }
        break;
    default:
        OSReport(lbl_1_data_C16C, lbl_1_bss_8260);
        break;
    }
}
