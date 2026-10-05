// m527Dll/m527 - decompilation of the m527Dll/m527 unit (53928 B, 16 fns).
// _prolog, _epilog, fn_1_9FE8, fn_1_CB68, fn_1_D198 and fn_1_D1F0 match; the rest are absent.

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
void fn_1_A0D0(OMOBJ *obj);
void fn_1_D1F0(OMOBJ *obj);

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