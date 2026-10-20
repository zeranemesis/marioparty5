// m527Dll/m527 - decompilation of the m527Dll/m527 unit (53928 B, 16 fns).
// All 16 functions exist here.  fn_1_4F98 (the per-frame update) and fn_1_5B4
// (the per-player turn handler) were the last two, and both are at the target's
// row count but only partially matched.

#include "game/charman.h"
#include "game/gamemes.h"
#include "game/hsfformat.h"
#include "game/hu3d.h"
#include "game/object.h"
#include "game/wipe.h"

void HuAudAllStop(void);

typedef void (*VoidFunc)(void);

extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

/*
 * Module globals.  Every one of them is written by one of the unit's other
 * functions (ObjectSetup, fn_1_A0D0, ...), which are not part of this file yet,
 * so all of them are only referenced from here.
 *
 * lbl_1_bss_B8  the four player objects (0x10 = 4 pointers)
 * lbl_1_bss_8   set to 1 once the game has asked to leave, gates the wipe-out
 * lbl_1_bss_AC  game mode (0x3E8 wipe in, 0x3E9.. intro, 0x3EC start, ...)
 * lbl_1_bss_A6  live GameMes id, -1 when no message is up
 * lbl_1_data_164  overlay-history slot handed back on return
 */
extern OMOBJ *lbl_1_bss_B8[4];
extern s32 lbl_1_bss_8;
extern s32 lbl_1_bss_AC;
extern s16 lbl_1_bss_A6;
extern s32 lbl_1_data_164;
extern OMOBJMAN *lbl_1_bss_B4;
extern f32 lbl_1_bss_84[2];
extern f32 lbl_1_bss_7C[2];
extern f32 lbl_1_bss_0;
extern s32 lbl_1_bss_74[4];
extern s16 lbl_1_bss_78[2];
extern s32 lbl_1_bss_210[2];
extern u32 lbl_1_data_158[2];
extern u32 lbl_1_rodata_384[12];
extern u32 lbl_1_rodata_488[12];
extern u32 lbl_1_rodata_3BC[12];
extern u32 lbl_1_rodata_3EC[24];

extern s32 lbl_1_bss_8C[4];

extern u32 lbl_1_rodata_270[12];

extern u32 lbl_1_rodata_50[4];

extern s32 lbl_1_data_124[16];

extern u32 lbl_1_data_158[2];
extern u32 lbl_1_rodata_4B8[16];
extern u32 lbl_1_rodata_4F8[12];
extern f32 lbl_1_rodata_558;
extern s32 lbl_1_bss_D0;
SHARED_SYM extern u8 esprite[];
SHARED_SYM extern u8 espanim[];

/* Player work: OMOBJ::data of the four player objects, 0x6C bytes. */
typedef struct M527Player_s {
    /* 0x00 */ s32 no;
    /* 0x04 */ s32 playerNo;
    /* 0x08 */ s32 charNoClamp;
    /* 0x0C */ s32 charNo;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ u8 pad20[0x10];
    /* 0x30 */ s32 unk30;
    /* 0x34 */ u8 pad34[0x18];
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ u8 pad50[0x14];
    /* 0x64 */ s32 unk64;
    /* 0x68 */ s16 lightId;
    /* 0x6A */ u8 pad6A[2];
} M527Player; /* size = 0x6C */

void ObjectSetup(void);


#ifdef TARGET_PC
/*
 * The matching build resolves every lbl_1_* symbol from the original .plf,
 * which the native build does not have, so the module's own globals are
 * defined here instead.
 */
    OMOBJ *lbl_1_bss_B8[4];
    s32 lbl_1_bss_8;
    s32 lbl_1_bss_AC;
    s16 lbl_1_bss_A6;
    s32 lbl_1_data_164;
    OMOBJMAN *lbl_1_bss_B4;
    OMOBJMAN *lbl_1_bss_A8;
    f32 lbl_1_bss_84[2];
    f32 lbl_1_bss_7C[2];
    f32 lbl_1_bss_0;
    s32 lbl_1_bss_74[4];
    s16 lbl_1_bss_78[2];
    s32 lbl_1_bss_210[2];
    u32 lbl_1_data_158[2];
    u32 lbl_1_rodata_384[12];
    u32 lbl_1_rodata_488[12];
    u32 lbl_1_rodata_3BC[12];
    u32 lbl_1_rodata_3EC[24];
    s32 lbl_1_bss_8C[4];
    u32 lbl_1_rodata_270[12];
    u32 lbl_1_rodata_50[4];
    s32 lbl_1_data_124[16];
    u32 lbl_1_rodata_4B8[16];
    u32 lbl_1_rodata_4F8[12];
    f32 lbl_1_rodata_558;
    s32 lbl_1_bss_D0;
#endif

void fn_1_A0D0(OMOBJ *obj);
void fn_1_D1F0(OMOBJ *obj);
void fn_1_5B4(OMOBJ *obj);
void fn_1_4F98(OMOBJ *obj);
void fn_1_7E5C(HU3DMODEL *modelP, Mtx mtx);
void fn_1_94F4(HU3DMODEL *modelP, Mtx mtx);

s32 _prolog(void)
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

/* Material hook: the water-surface tev setup used by the shared models. */
void fn_1_9FE8(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    Hu3DMatLightSet(drawObj->model, 0, material->hiliteScale);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_BL_INVSRCALPHA);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_DIVIDE_2, GX_TRUE, GX_TEVPREV);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_ZERO);
    GXSetNumTevStages(1);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetZMode(GX_TRUE, GX_GREATER, GX_FALSE);
}

void fn_1_CB68(OMOBJ *obj)
{
    lbl_1_bss_AC = 0x3E8;
    WipeCreate(WIPE_MODE_IN, WIPE_TYPE_PREVTYPE, 60);
    obj->objFunc = fn_1_A0D0;
}

void fn_1_D198(OMOBJ *obj)
{
    if (lbl_1_bss_8 != 0) {
        WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_WHITEFADE, 60);
        obj->objFunc = fn_1_D1F0;
    }
}

void fn_1_D1F0(OMOBJ *obj)
{
    s32 i;
    M527Player *player;

    if (WipeCheckEnd() == 0) {
        for (i = 0; i < 4; i++) {
            player = lbl_1_bss_B8[i]->data;
            CharModelKill(player->charNo);
        }
        if (lbl_1_bss_A6 >= 0) {
            GameMesKill(lbl_1_bss_A6);
        }
        GameMesClose();
        HuAudAllStop();
        omOvlReturnEx(lbl_1_data_164, 1);
    }
}
#include "game/pad.h"
/* the two debug camera rigs live in the DOL; m527.c only manipulates them */
extern OMOBJMAN *lbl_1_bss_A8;

/*
 * Debug camera: nudges the two camera rigs from the pad. Each iteration takes
 * the trigger pair on the pivot height, the L/R shoulder pair on the zoom, and
 * the substick on the two angles, then converts the spherical result to a
 * cartesian eye position. lbl_1_rodata_10=20, _18=0.0078125, _20=25,
 * _28=0.03125, _30=-32, _38=PI, _40=180.
 */
void fn_1_A0(void)
{
    f32 sp0C;
    f32 sp14;
    f32 sp18;
    f32 sp1C;
    f32 sp20;
    f32 sp24;
    f32 sp28;
    s32 i;

    for (i = 0; i < 2; i++) {
        CenterM[i].y += (f32) (20.0 * (0.0078125 * (double) (HuPadTrigL[0] - HuPadTrigR[0])));
        CZoomM[i] += (f32) (25.0 * (double) ((s32) ((HuPadBtn[0] & 0x400) != 0) - (s32) ((HuPadBtn[0] & 0x800) != 0)));
        CRotM[i].y += (f32) (0.03125 * (double) HuPadSubStkX[0]);
        CRotM[i].x += (f32) ((double) HuPadSubStkY[0] / -32.0);
        sp18 = CRotM[i].z;
        sp24 = (f32) (CenterM[i].x + ((f64) CZoomM[i] * (sin((M_PI * CRotM[i].y) / 180.0) * cos((M_PI * CRotM[i].x) / 180.0))));
        sp28 = (f32) (CenterM[i].y + ((f64) CZoomM[i] * -sin((M_PI * CRotM[i].x) / 180.0)));
        sp0C = (f32) (sin((M_PI * CRotM[i].y) / 180.0) * sin((M_PI * CRotM[i].x) / 180.0));
        sp14 = (f32) (cos((M_PI * CRotM[i].y) / 180.0) * cos((M_PI * CRotM[i].x) / 180.0));
        sp1C = CenterM[i].y;
        sp20 = CenterM[i].z;
    }
}
/* module setup: object manager, the two debug cameras, their initial rigs, and
 * the single ambient light. The rodata values are the ones fnasm reports for
 * this function. */
void ObjectSetup(void)
{
    HuVecF sp3C;
    HuVecF sp30;
    GXColor sp8;
    HuVecF sp28;
    HuVecF sp1C;
    HuVecF sp10;
    s32 i;

    OSReport("******* M527 ObjectSetup *********\n");
    lbl_1_bss_A8 = omInitObjMan(0x258, 0x2000);
    omGameSysInit(lbl_1_bss_A8);
    Hu3DCameraCreate(3);
    Hu3DCameraPerspectiveSet(1, 40.0f, 20.0f, 10000.0f, 1.2f);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(1, 0, 0, 0x280, 0x1E0);
    Hu3DCameraPerspectiveSet(2, 40.0f, 20.0f, 10000.0f, 0.6f);
    Hu3DCameraViewportSet(2, 322.0f, 0.0f, 318.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(2, 0, 0, 0, 0x1E0);
    for (i = 0; i < 2; i++) {
        CZoomM[i] = 1775.0f;
        CenterM[i].x = 0.0f;
        CenterM[i].y = 340.0f;
        CenterM[i].z = 0.0f;
        CRotM[i].x = -30.0f;
        CRotM[i].y = 0.0f;
        CRotM[i].z = 0.0f;
    }
    Hu3DLighInit();
    sp3C.x = 0.0f;
    sp3C.y = 5000.0f;
    sp3C.z = 0.0f;
    sp30.x = 0.75f;
    sp30.y = -1.0f;
    sp30.z = -0.5f;
    sp8.r = 0;
    sp8.g = 0;
    sp8.b = 0;
    Hu3DGLightInfinitytSet((HU3DLIGHTID) 0);
    sp28.z = 345.0f;
    sp28.x = 10000.0f * (f32) sin(1.5707963267948966);
    sp28.y = 10000.0f * (f32) cos(1.5707963267948966);
    sp1C.x = 0.0f;
    sp1C.y = 1.0f;
    sp1C.z = 0.0f;
    sp10.x = 346.0f;
    sp10.y = 0.0f;
    sp10.z = 0.0f;
    Hu3DShadowCreate(12.0f, 7000.0f, 12000.0f);
    Hu3DCameraPosSet(3, sp28.x, sp28.y, sp28.z, sp1C.x, sp1C.y, sp1C.z, sp10.x, sp10.y, sp10.z);
}
/*
 * Create: groups the players by colour (GwPlayerConf[j].grp) into slot order,
 * fills the per-slot work record, then builds the character models. Slots whose
 * index is odd create their own model, the others link against the first
 * player's -- that is the split-screen pairing.
 */
void fn_1_C408(OMOBJ *arg0)
{
    s32 sp10[4];
    s32 i;
    s32 j;
    s32 n;
    M527Player *pl;

    sp10[0] = 0;
    sp10[1] = 1;
    sp10[2] = 2;
    sp10[3] = 3;
    pl = (M527Player *) arg0->data;
    memset(pl, 0, 0x6C);
    n = 0;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (i == GwPlayerConf[j].grp) {
                sp10[n] = j;
                n += 1;
            }
        }
    }
    pl->no = arg0->work[0x4C / 4];
    pl->playerNo = sp10[pl->no];
    if (pl->playerNo == 4) {
        pl->playerNo = pl->no;
    }
    pl->charNo = GwPlayerConf[pl->playerNo].charNo;
    pl->charNoClamp = pl->charNo >= 9 ? 9 : pl->charNo;
    pl->unk10 = GwPlayerConf[pl->playerNo].type != 0 ? -1 : GwPlayerConf[pl->playerNo].padNo;
    pl->unk14 = pl->unk10 != -1 ? 0 : GwPlayerConf[pl->playerNo].dif;
    pl->unk30 = 1;
    pl->unk4C = 0;
    if (pl->unk14 == 0) {
        pl->unk1C = (rand8() % 5) != 0;
    } else if (pl->unk14 == 1) {
        pl->unk1C = (rand8() % 3) != 0;
    } else {
        pl->unk1C = 0;
    }
    for (i = 0; i < 2; i++) {
        s16 id;

        if (pl->no == 1) {
            id = Hu3DModelCreate(HuDataSelHeapReadNum((i * 0x5500) + 0x5509, HU_MEMNUM_OVL, HEAP_MODEL));
            ((s16 *) arg0->mdlId)[i + 2] = id;
        } else {
            id = Hu3DModelLink(((s16 *) lbl_1_bss_B8[1]->mdlId)[i + 2]);
            ((s16 *) arg0->mdlId)[i + 2] = id;
        }
        Hu3DModelLayerSet(((s16 *) arg0->mdlId)[i + 2], 4);
    }
    for (i = 0; i < 9; i++) {
        s32 light = Hu3DLLightCreate(((s16 *) arg0->mdlId)[i + 2], 0.0f, 0.0f, 500.0f, 3.0f, -4.0f, -2.0f, 0xFF, 0xFF, 0xFF);

        Hu3DLLightInfinitytSet(((s16 *) arg0->mdlId)[i + 2], light);
    }
    arg0->objFunc = fn_1_5B4;
}
/*
 * GX hook: draws the two split-screen border quads. Each is a 4-vertex quad
 * scaled by an ease-in factor derived from the sequence frame, then offset by
 * the per-screen origin. rodata_384 is the 4x3 corner table, rodata_3B4 the
 * base width (133.33333), rodata_178/17C/180 the 50..70 frame window and its
 * 20-frame span, rodata_3B8 the border depth (1.5) and rodata_1A8 the UV
 * scale (0.5).
 */
void fn_1_7E5C(HU3DMODEL *modelP, Mtx mtx)
{
    f32 sp10[4][3];
    f32 ease;
    f32 w;
    f32 ox;
    f32 oz;
    f32 ux;
    f32 uz;
    s32 i;
    s32 j;
    s32 r28;

    r28 = modelP == (HU3DMODEL *) ((u8 *) &Hu3DData[*(s16 *) ((u8 *) lbl_1_bss_B4 + 0x40)]) ? 0 : 1;
    for (i = 0; i < 12; i++) {
        ((f32 *) sp10)[i] = ((f32 *) &lbl_1_rodata_384)[i];
    }
    GXLoadPosMtxImm(mtx, 0);
    GXSetNumTevStages(1);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, 0, 0x7D);
    GXSetZMode(0, GX_ALWAYS, 0);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_CLAMP, GX_AF_NONE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetZMode(1, GX_LEQUAL, 1);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZCompLoc(0);
    GXSetColorUpdate(0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_TEX_ST, GX_RGBA6, 0);
    GXSetVtxDesc(GX_VA_NBT, GX_INDEX8);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NBT, GX_TEX_ST, GX_RGBA6, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_RGBA4, 0);
    if (lbl_1_bss_AC == 0x3ED && r28 == 1 - lbl_1_data_158[0]) {
        GXSetColorUpdate(0);
        GXSetAlphaCompare(GX_ALWAYS, 0x80, GX_AOP_AND, GX_ALWAYS, 0x80);
        GXSetZMode(1, GX_LEQUAL, 1);
    }
    HuSprTexLoad(*(u32 *) (espanim + (*(u16 *) (esprite + (lbl_1_bss_210[0] * 4) + 2) * 0xC) + 8), 0, 0, 0, 0, 1);
    for (i = 0; i < 2; i++) {
        f32 t = lbl_1_bss_0;

        if (t < 50.0f) {
            ease = 0.0f;
        } else if (t > 70.0f) {
            ease = 1.0f;
        } else {
            ease = (t - 50.0f) / 20.0f;
        }
        w = 133.33333f * (1.0f - ((1.0f - ease) * (1.0f - ease)));
        ox = lbl_1_bss_84[i];
        oz = lbl_1_bss_7C[i];
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        for (j = 0; j < 4; j++) {
            *(f32 *) (uintptr_t) 0xCC008000 = ox + (w * sp10[j][0]);
            *(f32 *) (uintptr_t) 0xCC008000 = 1.5f;
            *(f32 *) (uintptr_t) 0xCC008000 = oz + (w * sp10[j][2]);
            *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
            *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
            *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
            *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
            *(f32 *) (uintptr_t) 0xCC008000 = (1.0f + sp10[j][0]) * 0.5f;
            *(f32 *) (uintptr_t) 0xCC008000 = (1.0f + sp10[j][2]) * 0.5f;
        }
    }
    GXPixModeSync();
    GXSetColorUpdate(1);
    GXSetZCompLoc(1);
}
/*
 * GX hook: the second split-screen border pass. Same corner table and quad
 * shape as fn_1_7E5C but a wider border (rodata_1DC = 116.666664), a deeper
 * one (rodata_200 = 27) and per-vertex colours computed from the overlay
 * state instead of a flat white. No texture is bound.
 */
void fn_1_94F4(HU3DMODEL *modelP, Mtx mtx)
{
    f32 sp10[4][3];
    f32 ease;
    f32 w;
    f32 ox;
    f32 oz;
    u8 cr;
    u8 cg;
    u8 cb;
    u8 ca;
    s32 i;
    s32 j;
    s32 r30;

    r30 = modelP == (HU3DMODEL *) ((u8 *) &Hu3DData[*(s16 *) ((u8 *) lbl_1_bss_B4 + 0x40)]) ? 0 : 1;
    for (i = 0; i < 12; i++) {
        ((f32 *) sp10)[i] = ((f32 *) &lbl_1_rodata_488)[i];
    }
    GXLoadPosMtxImm(mtx, 0);
    GXSetNumTevStages(1);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, 0, 0x7D);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_CLAMP, GX_AF_NONE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetZMode(1, GX_LEQUAL, 1);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZCompLoc(0);
    GXSetColorUpdate(0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_TEX_ST, GX_RGBA6, 0);
    GXSetVtxDesc(GX_VA_NBT, GX_INDEX8);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NBT, GX_TEX_ST, GX_RGBA6, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_RGBA4, 0);
    ca = (u8) (17.0f * 15.0f);
    for (i = 0; i < 2; i++) {
        f32 t = lbl_1_bss_0;

        if (t < 50.0f) {
            ease = 0.0f;
        } else if (t > 70.0f) {
            ease = 1.0f;
        } else {
            ease = (t - 50.0f) / 20.0f;
        }
        w = 116.666664f * (1.0f - ((1.0f - ease) * (1.0f - ease)));
        ox = lbl_1_bss_84[i];
        oz = lbl_1_bss_7C[i];
        cr = (u8) (255.0f * ease);
        cg = (u8) (255.0f * ease);
        cb = (u8) (255.0f * ease);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        for (j = 0; j < 4; j++) {
            *(f32 *) (uintptr_t) 0xCC008000 = ox + (w * sp10[j][0]);
            *(f32 *) (uintptr_t) 0xCC008000 = 0.0f;
            *(f32 *) (uintptr_t) 0xCC008000 = oz + (w * sp10[j][2]);
            *(u8 *) (uintptr_t) 0xCC008000 = cr;
            *(u8 *) (uintptr_t) 0xCC008000 = cg;
            *(u8 *) (uintptr_t) 0xCC008000 = cb;
            *(u8 *) (uintptr_t) 0xCC008000 = ca;
            *(f32 *) (uintptr_t) 0xCC008000 = 0.0f;
            *(f32 *) (uintptr_t) 0xCC008000 = 0.0f;
            *(f32 *) (uintptr_t) 0xCC008000 = 27.0f;
            *(f32 *) (uintptr_t) 0xCC008000 = 0.0f;
            *(f32 *) (uintptr_t) 0xCC008000 = oz + (w * sp10[j][2]);
            *(u8 *) (uintptr_t) 0xCC008000 = cr;
            *(u8 *) (uintptr_t) 0xCC008000 = cg;
            *(u8 *) (uintptr_t) 0xCC008000 = cb;
            *(u8 *) (uintptr_t) 0xCC008000 = ca;
        }
    }
    GXPixModeSync();
    GXSetColorUpdate(1);
    GXSetZCompLoc(1);
}
/*
 * Create: builds the whole overlay set -- the two border passes (fn_1_7E5C,
 * fn_1_94F4), the four character-side models with their lights, the score
 * window and the overlay sprite. Each model is created, layered, camera-bound
 * and given a light; the ones the matching build keeps are hooked rather than
 * created. Ends by handing over to fn_1_4F98.
 */
void fn_1_B8C4(OMOBJ *arg0)
{
    s32 i;
    s32 light;
    s16 mdl[8];

    for (i = 0; i < 2; i++) {
        mdl[i] = Hu3DModelCreate(HuDataSelHeapReadNum(i + 0x55, HU_MEMNUM_OVL, HEAP_MODEL));
        ((s16 *) arg0->mdlId)[i] = mdl[i];
        Hu3DModelLayerSet(((s16 *) arg0->mdlId)[i], 4);
        Hu3DModelCameraSet(((s16 *) arg0->mdlId)[i], 3);
        Hu3DModelAttrSet(((s16 *) arg0->mdlId)[i], 0x40000001);
    }
    light = Hu3DLLightCreate(((s16 *) arg0->mdlId)[0], 0.0f, 0.0f, 500.0f, 4.0f, -4.0f, -1.0f, 0, 0, 0);
    lbl_1_bss_74[0] = light;
    Hu3DLLightInfinitytSet(((s16 *) arg0->mdlId)[0], lbl_1_bss_74[0]);
    {
        s16 id = Hu3DModelCreate(HuDataSelHeapReadNum(0x57, HU_MEMNUM_OVL, HEAP_MODEL));

        ((s16 *) arg0->mdlId)[2] = id;
        Hu3DModelLayerSet(id, 4);
        Hu3DModelCameraSet(id, 3);
        Hu3DModelAttrSet(id, 0x40000001);
        Hu3DModelShadowMapSet(id);
        light = Hu3DLLightCreate(id, 0.0f, 0.0f, 500.0f, 4.0f, -4.0f, -1.0f, 0, 0, 0);
        lbl_1_bss_74[1] = light;
        Hu3DLLightInfinitytSet(id, lbl_1_bss_74[1]);
    }
    {
        s16 id = Hu3DModelCreate(HuDataSelHeapReadNum(0x58, HU_MEMNUM_OVL, HEAP_MODEL));

        ((s16 *) arg0->mdlId)[3] = id;
        Hu3DModelLayerSet(id, 4);
        Hu3DModelCameraSet(id, 3);
        Hu3DModelAttrSet(id, 0x40000001);
        light = Hu3DLLightCreate(id, 0.0f, 0.0f, 500.0f, 4.0f, -4.0f, -1.0f, 0, 0, 0);
        lbl_1_bss_74[2] = light;
        Hu3DLLightInfinitytSet(id, lbl_1_bss_74[2]);
    }
    Hu3DHookFuncCreate((HU3DMODELHOOK) fn_1_7E5C);
    Hu3DModelLayerSet(Hu3DHookFuncCreate((HU3DMODELHOOK) fn_1_7E5C), 4);
    Hu3DModelCameraSet(Hu3DHookFuncCreate((HU3DMODELHOOK) fn_1_7E5C), 3);
    Hu3DHookFuncCreate((HU3DMODELHOOK) fn_1_94F4);
    Hu3DModelLayerSet(Hu3DHookFuncCreate((HU3DMODELHOOK) fn_1_94F4), 4);
    Hu3DModelCameraSet(Hu3DHookFuncCreate((HU3DMODELHOOK) fn_1_94F4), 3);
    {
        s16 id = Hu3DModelCreate(HuDataSelHeapReadNum(0x59, HU_MEMNUM_OVL, HEAP_MODEL));
        s16 link = Hu3DModelLink(id);

        ((s16 *) arg0->mdlId)[4] = link;
        Hu3DModelLayerSet(link, 4);
        Hu3DModelCameraSet(link, 3);
        Hu3DModelAttrSet(link, 0x40000001);
        light = Hu3DLLightCreate(link, 0.0f, 0.0f, 500.0f, 4.0f, -4.0f, -1.0f, 0, 0, 0);
        lbl_1_bss_74[3] = light;
        Hu3DLLightInfinitytSet(link, lbl_1_bss_74[3]);
        Hu3DModelMatHookSet(link, (HU3DMATHOOK) fn_1_7E5C);
        Hu3DModelAmbSet(link, 1.0f, 0.5f, 0.5f);
    }
    Hu3DHookFuncCreate((HU3DMODELHOOK) fn_1_94F4);
    Hu3DModelLayerSet(Hu3DHookFuncCreate((HU3DMODELHOOK) fn_1_94F4), 4);
    Hu3DModelAttrSet(Hu3DHookFuncCreate((HU3DMODELHOOK) fn_1_94F4), 0x40000001);
    {
        s16 id = Hu3DModelCreate(HuDataSelHeapReadNum(0x5A, HU_MEMNUM_OVL, HEAP_MODEL));
        s32 mtn0 = Hu3DJointMotion(id, HuDataSelHeapReadNum(0x5B, HU_MEMNUM_OVL, HEAP_MODEL));
        s32 mtn1 = Hu3DJointMotion(id, HuDataSelHeapReadNum(0x5C, HU_MEMNUM_OVL, HEAP_MODEL));

        Hu3DMotionSet(id, mtn0);
        Hu3DModelLayerSet(id, 4);
        Hu3DModelCameraSet(id, 3);
        Hu3DModelAttrSet(id, 0x40000001);
    }
    lbl_1_bss_78[1] = MgScoreWinCreate(0x52, 0x1EE);
    MgScoreWinColorSet(lbl_1_bss_78[1], 0, 0x32, 0xFA);
    MgScoreWinPosSet(lbl_1_bss_78[1], 82.0f, 0.0f);
    MgScoreWinDispSet(lbl_1_bss_78[1], 0);
    lbl_1_bss_210[0] = (s32) (s16) espEntry(0x55006, 0, 0);
    espDispOff((s16) lbl_1_bss_210[0]);
    arg0->objFunc = fn_1_4F98;
}
/*
 * GX hook: the score panel. It orients the panel toward the average of the two
 * border origins (atan2 of the delta, wrapped into 0..360), fades it in over
 * the same 50..70 frame window as the borders, then emits one quad per
 * rodata_3EC corner. rodata_3BC is the same corner table the borders use and
 * rodata_3EC is the panel's own, larger corner set.
 */
void fn_1_87F4(HU3DMODEL *modelP, Mtx mtx)
{
    f32 sp90[12];
    f32 sp40[8][3];
    f32 ang;
    f32 ease;
    f32 w;
    f32 ox;
    f32 oz;
    s32 i;
    s32 j;
    s32 r30;

    r30 = modelP == (HU3DMODEL *) ((u8 *) &Hu3DData[*(s16 *) ((u8 *) lbl_1_bss_B4 + 0x1C)]) ? 0 : 1;
    for (i = 0; i < 12; i++) {
        ((f32 *) sp90)[i] = ((f32 *) &lbl_1_rodata_3BC)[i];
    }
    for (i = 0; i < 24; i++) {
        ((f32 *) sp40)[i] = ((f32 *) &lbl_1_rodata_3EC)[i];
    }
    ang = (f32) ((180.0 * atan2(lbl_1_bss_84[1 - r30] - lbl_1_bss_84[r30], lbl_1_bss_7C[1 - r30] - lbl_1_bss_7C[r30])) / M_PI - 90.0);
    if (ang < 0.0f) {
        ang += 360.0f;
    } else if (ang >= 360.0f) {
        ang -= 360.0f;
    }
    GXLoadPosMtxImm(mtx, 0);
    GXSetNumTevStages(1);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, 0, 0x7D);
    GXSetZMode(0, GX_ALWAYS, 0);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_CLAMP, GX_AF_NONE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetZMode(1, GX_LEQUAL, 1);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetZCompLoc(0);
    GXSetColorUpdate(0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_TEX_ST, GX_RGBA6, 0);
    GXSetVtxDesc(GX_VA_NBT, GX_INDEX8);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NBT, GX_TEX_ST, GX_RGBA6, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_RGBA4, 0);
    if (lbl_1_bss_AC == 0x3ED) {
        GXSetAlphaCompare(GX_ALWAYS, 0x80, GX_AOP_AND, GX_ALWAYS, 0x80);
    }
    if (lbl_1_bss_0 > 70.0f) {
        ease = 1.0f;
    } else {
        ease = (lbl_1_bss_0 - 1.0f - 50.0f) / 20.0f;
    }
    w = 133.33333f * (1.0f - ((1.0f - ease) * (1.0f - ease)));
    ox = lbl_1_bss_84[r30];
    oz = lbl_1_bss_7C[r30];
    GXBegin(GX_QUADS, GX_VTXFMT0, 8);
    for (j = 0; j < 8; j++) {
        *(f32 *) (uintptr_t) 0xCC008000 = ox + (w * sp40[j][0]);
        *(f32 *) (uintptr_t) 0xCC008000 = 0.0f;
        *(f32 *) (uintptr_t) 0xCC008000 = oz + (w * sp40[j][2]);
        *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
        *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
        *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
        *(u8 *) (uintptr_t) 0xCC008000 = 0xFF;
        *(f32 *) (uintptr_t) 0xCC008000 = sp40[j][0];
        *(f32 *) (uintptr_t) 0xCC008000 = sp40[j][2];
    }
    GXPixModeSync();
    GXSetColorUpdate(1);
    GXSetZCompLoc(1);
}
/*
 * The story-mode state machine. Dispatches on lbl_1_bss_AC - 0x3E8 through a
 * 7-entry table (jumptable_1_data_26C). Collects the four player work records
 * first, then runs whichever state is current. rodata_4B8 is the 16-entry
 * GameMes id table the message states walk.
 */
void fn_1_A0D0(OMOBJ *arg0)
{
    M527Player *sp24[4];
    s32 sp94[12];
    s32 sp64[12];
    s32 i;
    s32 j;
    s32 frame;
    s32 idx;

    for (i = 0; i < 4; i++) {
        sp24[i] = (M527Player *) lbl_1_bss_B8[i]->data;
    }
    for (i = 0; i < 16; i++) {
        ((s32 *) sp94)[i] = ((s32 *) &lbl_1_rodata_4B8)[i];
    }
    for (i = 0; i < 4; i++) {
        if (sp24[i]->unk10 > 3) {
            continue;
        }
        if (sp24[i]->unk10 > 0) {
            HuAudFXPlay(sp94[sp24[i]->unk10]);
        }
    }
    if (WipeCheckEnd() == 0) {
        lbl_1_bss_AC += 1;
        lbl_1_bss_0 = 0.0f;
        return;
    }
    if (lbl_1_bss_0 == 1.0f && lbl_1_bss_D0 < 3) {
        ((f32 *) &lbl_1_bss_D0)[lbl_1_bss_D0 + 1] = 0.0f;
        lbl_1_bss_D0 += 1;
    }
    lbl_1_bss_0 += 1.0f;
    frame = (s32) lbl_1_bss_0;
    if (frame % 10 != 1) {
        return;
    }
    idx = ((s32) (lbl_1_bss_0 - 1.0f)) / 10;
    if (idx > 5) {
        j = 10 - idx;
    } else {
        j = idx;
    }
    if (j < 0) {
        j = 0;
    }
    for (i = 0; i < 4; i++) {
        if ((s32) sp24[i]->unk10 == -1) {
            continue;
        }
        {
            s32 *vib = (s32 *) &sp64[0];

            for (j = 0; j < 12; j++) {
                vib[j] = ((s32 *) &lbl_1_rodata_4F8)[j];
            }
            omVibrate((s16) ((s32 *) sp24[i])[4 / 4], 0xA, (s16) ((s32 *) &sp64)[j * 2], (s16) ((s32 *) &sp64)[(j * 2) + 1]);
        }
    }
    if (lbl_1_bss_0 >= *(f32 *) &lbl_1_rodata_558) {
        lbl_1_bss_AC += 1;
    }
}

/* Turn work: OMOBJ::data of each per-player turn handler. */
typedef struct M527Work_s {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ s32 unk08;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ u8 pad10[0x08];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 pad1C[0x10];
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ u8 pad38[0x08];
    /* 0x40 */ f32 unk40;
    /* 0x44 */ u8 pad44[0x04];
    /* 0x48 */ f32 unk48;
} M527Work;

/*
 * The per-frame update.  Every frame it refreshes the four player work
 * pointers, then runs the camera / model block for whichever game mode
 * lbl_1_bss_AC is at (0x3E9 intro sweep, 0x3EA split-screen, 0x3EB model
 * swap, 0x3ED .. 0x3EE board play, 0x3EC light tuning).  The modes are
 * tested one after another rather than switched, so each block only runs on
 * its own frame.
 */

/*
 * The per-frame update.  Each frame it refreshes the four player work
 * pointers, then runs the block for whichever game mode lbl_1_bss_AC is at.
 * The modes are tested one after another instead of being switched, so only
 * the block matching the current mode runs:
 *   0x3E9  intro sweep - lifts the four flood lights out of the floor
 *   0x3EA  split screen - drives both camera viewports and scissors
 *   0x3EB  model swap   - plays the board motion once
 *   0x3ED  board play   - per-player model and camera placement
 *   0x3EE  .. 0x3EC     light tuning and the closing wipe
 */
void fn_1_4F98(OMOBJ *arg0)
{
    M527Work *sp140[4];
    s32 i;

    for (i = 0; i < 4; i++) {
        sp140[i] = (M527Work *) lbl_1_bss_B8[i]->data;
    }

    if (lbl_1_bss_AC == 0x3E9) {
        f64 t;
        f32 f26;
        f32 f16;
        f32 f17;
        f32 f15;
        f32 f2;
        s32 i2;
        s32 sp130[4];
        f64 sp108;
        f64 sp100;

        f26 = lbl_1_bss_0 / 115.0f;
        if (f26 > 1.0f) {
            t = 1.0;
        } else if (f26 >= 0.75) {
            t = 0.66 + (f26 - 0.75) / 0.25 * 0.66;
        } else {
            t = 0.66 * (f26 / 0.75);
        }
        sp100 = t;
        sp108 = sp100;
        f26 = (f32) sp108;
        if (lbl_1_bss_0 <= 5.0f) {
            Hu3DMotionTimeSet(arg0->mdlId[5], 0.0f);
        }
        Hu3DModelPosSet(arg0->mdlId[5], 0.0f, 0.0f, 0.0f);
        Hu3DModelScaleSet(arg0->mdlId[5], 1.0f, 1.0f, 1.0f);
        Hu3DModelRotSet(arg0->mdlId[5], 0.0f, 0.0f, 0.0f);
        Hu3DModelAttrReset(arg0->mdlId[5], 1);

        sp130[0] = 1;
        sp130[1] = 4;
        sp130[2] = 2;
        sp130[3] = 3;
        for (i2 = 0; i2 < 4; i2++) {
            f64 x = 3.141592653589793 * (148.5 * f26) / 180.0;

            f16 = (f32) cos(x);
            f17 = (f32) -sin(x);
            f15 = (f32) (-500.0 * cos(x));
            f2 = (f32) (-500.0 * -sin(x));
            Hu3DLLightPosSet(((s16 *) sp140[i2])[sp130[i2]], (s16) lbl_1_bss_8C[i2], 0.0f, f2, f15, 0.0f, f17, f16);
        }
    } else {
        Hu3DModelAttrSet(arg0->mdlId[0], 1);
    }

    if (lbl_1_bss_AC == 0x3EA) {
        f32 spFC;
        f32 spB8;
        f32 spF8;
        f32 spF4;
        f32 spF0;
        f32 spB4;
        f32 spB0;
        f32 spAC;
        f32 spA8;
        f32 spA4;
        f32 f15;
        f32 f22;
        f32 f25;
        f32 f24;
        f32 f30;
        f32 f20;

        spB8 = lbl_1_bss_0 > 60.0f ? 1.0f : lbl_1_bss_0 / 60.0f;
        spFC = spB8;
        f15 = (f32) sin(2.0943951023931953);
        spF8 = (f32) (sin(3.141592653589793 * (120.0f * spFC) / 180.0) / f15);
        spF4 = 320.0f - 160.0f * spFC;
        spF0 = 320.0f + 160.0f * spFC;
        f22 = 320.0f - 160.0f * spFC;
        f25 = spF4 - f22;
        f24 = spF4 + f22;
        f30 = spF0 - f22;
        f20 = spF0 + f22;

        Hu3DCameraPerspectiveSet(1, 40.0f, 20.0f, 10000.0f, 576.0f * ((2.0f * f22) / 640.0f) / 480.0f);
        Hu3DCameraViewportSet(1, f25, 0.0f, f24 - f25, 480.0f, 0.0f, 1.0f);
        Hu3DCameraPerspectiveSet(2, 40.0f, 20.0f, 10000.0f, 576.0f * ((2.0f * f22) / 640.0f) / 480.0f);
        Hu3DCameraViewportSet(2, f30, 0.0f, f20 - f30, 480.0f, 0.0f, 1.0f);

        spB4 = f24 > f30 ? f24 : f30;
        spB4 = f25 < 0.0f ? 0.0f : f25;
        spB0 = f30 < 0.0f ? 0.0f : f30;
        spAC = f24 > 640.0f ? 640.0f : f24;
        spA4 = f20 > 640.0f ? 640.0f : f20;

        Hu3DCameraScissorSet(1, 0, (unsigned) (spAC - spB0 - 2.0f), 1, 0x1E0);
        Hu3DCameraScissorSet(2, 0, (unsigned) (spA4 - spB0 - 2.0f), 1, 0x1E0);
        (void) spB4;
        (void) spB8;
        (void) spF8;
        (void) spF0;
        (void) f20;
    }

    if (lbl_1_bss_AC == 0x3EB) {
        if (10.0f == lbl_1_bss_0) {
            Hu3DMotionSet(arg0->mdlId[5], arg0->mtnId[0]);
            Hu3DModelAttrSet(arg0->mdlId[5], 0x4001);
        }
    }

    if (lbl_1_bss_AC == 0x3ED && lbl_1_data_158[0] != 4) {
        s32 sp14C[12];
        s32 j;

        for (j = 0; j < 12; j++) {
            sp14C[j] = ((s32 *) &lbl_1_rodata_270)[j];
        }
        for (i = 0; i < 4; i++) {
            if (((s32 *) sp140[i])[9] == 0) {
                continue;
            }
            Hu3DModelPosSet(((s16 *) sp140[i])[9], ((f32 *) sp14C)[0], ((f32 *) sp14C)[1], ((f32 *) sp14C)[2]);
            Hu3DModelScaleSet(((s16 *) sp140[i])[9], ((f32 *) sp14C)[3], ((f32 *) sp14C)[4], ((f32 *) sp14C)[5]);
            Hu3DModelAttrReset(((s16 *) sp140[i])[9], 1);
            Hu3DModelCameraSet(((s16 *) sp140[i])[9], 0x80000001);
            Hu3DModelCameraSet(((s16 *) sp140[i])[9], 0x80000002);
            Hu3DModelAttrReset(((s16 *) sp140[i])[9], 1);
            Hu3DCameraPerspectiveSet(1, 40.0f, 20.0f, 10000.0f, 576.0f * ((2.0f * 320.0f) / 640.0f) / 480.0f);
            Hu3DCameraViewportSet(1, 0.0f, 0.0f, 480.0f, 480.0f, 0.0f, 1.0f);
            Hu3DCameraPerspectiveSet(2, 40.0f, 20.0f, 10000.0f, 576.0f * ((2.0f * 320.0f) / 640.0f) / 480.0f);
            Hu3DCameraViewportSet(2, 0.0f, 0.0f, 480.0f, 480.0f, 0.0f, 1.0f);
            Hu3DCameraScissorSet(1, 0, (unsigned) (480.0f - 2.0f), 1, 0x1E0);
            Hu3DCameraScissorSet(2, 0, (unsigned) (480.0f - 2.0f), 1, 0x1E0);
        }
    }

    if (lbl_1_bss_AC == 0x3EE) {
        s32 i3;

        for (i3 = 0; i3 < 2; i3++) {
            Hu3DMotionTimeSet(arg0->mdlId[i3], lbl_1_bss_84[i3]);
            Hu3DMotionTimeSet(arg0->mdlId[i3], lbl_1_bss_84[i3]);
            Hu3DModelPosSet(arg0->mdlId[i3], lbl_1_bss_7C[0], lbl_1_bss_7C[1], -500.0f);
            Hu3DModelScaleSet(arg0->mdlId[i3], 1.25f, 1.25f, 1.25f);
            Hu3DModelRotSet(arg0->mdlId[i3], lbl_1_bss_78[0], lbl_1_bss_78[1], 0.0f);
            Hu3DModelAttrReset(arg0->mdlId[i3], 1);
            Hu3DModelAttrSet(arg0->mdlId[i3], 0x80000001);
            Hu3DModelPosSet(arg0->mtnId[0], -270.0f, 270.0f, 70.0f);
            Hu3DModelRotSet(arg0->mtnId[0], 0.0f, 0.0f, 0.0f);
            Hu3DModelScaleSet(arg0->mtnId[0], 1.0f, 1.0f, 1.0f);
            Hu3DModelAttrReset(arg0->mtnId[0], 1);
            Hu3DModelAttrSet(arg0->mtnId[0], 0x80000001);
        }
    }

    if (lbl_1_bss_AC == 0x3EC || lbl_1_bss_AC == 0x3ED) {
        Hu3DLLightColorSet(0, 32, 255, 200, 0, 0);
    }
}
/*
 * The per-player turn handler.  One copy runs for each of the four players;
 * arg0's unk_4C picks the slot, whose index in lbl_1_rodata_50 selects the
 * owning player object.  The body is the player's own state machine
 * (data->unk30, 0..5), and while it is in state 1 it further branches on the
 * board mode lbl_1_bss_AC.
 */
void fn_1_5B4(OMOBJ *arg0)
{
    s32 sp188[4];
    M527Work *sp13C;
    s32 sp138;
    s32 r27;
    f32 spFC;
    f32 f29;
    f32 f30;
    f32 f31;

    sp188[0] = ((s32 *) &lbl_1_rodata_50)[0];
    sp188[1] = ((s32 *) &lbl_1_rodata_50)[1];
    sp188[2] = ((s32 *) &lbl_1_rodata_50)[2];
    sp188[3] = ((s32 *) &lbl_1_rodata_50)[3];
    lbl_1_bss_B8[sp188[arg0->work[0x4C / 4]]];
    sp13C = (M527Work *) arg0->data;
    lbl_1_bss_B8[sp188[arg0->work[0x4C / 4]]]->data;
    f29 = 0.0f;
    f30 = 0.0f;
    f31 = 0.0f;
    sp138 = 0.0f;
    r27 = -1;
    lbl_1_data_124[9] = abs(sp13C->unk00) * 0x3C;
    if (sp13C->unk18 != 3) {
        sp13C->unk48 = 0.0f;
        sp13C->unk40 = 0.0f;
    }
    sp13C->unk34 += 1;

    switch (sp13C->unk30) {
    case 1:
        if (lbl_1_bss_AC == 0x3E9) {
            r27 = 0;
            if (lbl_1_bss_0 >= 40.0f) {
                return;
            }
            if (lbl_1_bss_0 < 55.0f && sp13C->unk00 < 2) {
                r27 = 8;
            }
        } else if (lbl_1_bss_AC == 0x3EA) {
            r27 = 8;
        } else if (lbl_1_bss_AC == 0x3EB) {
            /* board fade in: spin the place marker around its axis */
            {
                Vec sp108 = {0.0f, 0.0f, 0.0f};
                Vec spF0;
                Mtx spC4;

                sp108.x = 0.0f;
                sp108.y = 0.0f;
                sp108.z = 0.0f;
                PSMTXRotRad(spC4, 'z', 0.0);
                PSMTXMultVec(spC4, &sp108, &spF0);
            }
        }
        break;

    case 2:
        {
            s32 sp48;

            sp48 = rand8();
            if (sp13C->unk0C >= 4) {
                Hu3DModelAttrSet(sp13C->unk2C, 1);
                CharFXPlay(0, 0, 0);
            } else {
                Hu3DModelAttrSet(sp13C->unk2C, 1);
                Hu3DMotionSpeedSet(0, 1.0f);
                Hu3DModelLayerSet(0, 1);
                Hu3DLLightPosSet(0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
                Hu3DLLightColorSet(0, 0, 0, 0, 0, 0);
                Hu3DModelAttrSet(0, 1);
                Hu3DModelAttrReset(0, 1);
                Hu3DLLightColorSet(0, 0, 0, 0, 0, 0);
                if (CharMotionEndCheck(0) || CharMotionEndCheck(0)) {
                    r27 = 8;
                }
            }
        }
        break;

    case 3:
    case 0:
        break;

    case 4:
        {
            f32 sp78;
            f32 sp68;
            f32 sp64;

            sp78 = (f32) atan2(0.0, 1.0);
            sp68 = (f32) atan2(0.0, 1.0);
            if (CharMotionEndCheck(0)) {
                omVibrate(0, 10, 12.0f, 1);
            }
        }
        break;

    case 5:
        espDispOn(0);
        espDispOff(0);
        espTPLvlSet(0, 0);
        espScaleSet(0, 0.0f, 0.0f);
        Hu3DModelAttrSet(sp13C->unk2C, 1);
        Hu3DModelAttrReset(sp13C->unk2C, 1);
        Hu3DModelPosSet(sp13C->unk2C, 0.0f, 0.0f, 0.0f);
        break;
    }

    if (lbl_1_bss_AC == 0x3ED) {
        Hu3DModelAttrReset(sp13C->unk2C, 1);
        Hu3DModelPosSet(sp13C->unk2C, 0.0f, 0.0f, 0.0f);
    }

    if (lbl_1_bss_AC == 0x3EA || lbl_1_bss_AC == 0x3EE) {
        CharModelVoicePanSet(0, 0);
        CharModelVoiceVolSet(0, 0);
    }
}
