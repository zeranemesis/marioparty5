#include "game/object.h"
#include "game/charman.h"
#include "game/animdata.h"
#include "game/esprite.h"
/* Without this the port link fails: M2C_ERROR is a real (if no-op) call and
 * has to be declared. Same trap m538Dll/m538.c documents at its line 14. It
 * must come after the Dolphin types, which m2c_macros.h itself uses. */
#include "m2c_macros.h"
#include "game/hu3d.h"
#include "game/mg/seqman.h"
#include "game/board/player.h"
#include "game/pad.h"
#include "game/printfunc.h"
#include "REL/m559dll.h"

typedef void (*VoidFunc)(void);
typedef void (*M559HOOKFUNC)(M559HOOKDATA *);
#define M559DATA(o) ((M559HOOK *) ((o)->data))
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];
extern void ObjectSetup(void);
void fn_1_32D0(OMOBJ *obj);

typedef struct M559ListHead_s {
    s32 field0;
    M559LISTNODE *prev;
    M559LISTNODE *next;
} M559LISTHEAD;

static M559VEC lbl_1_data_0 = { 0.0f, 10000.0f, 10000.0f };
static M559VEC lbl_1_data_C = { 0.0f, 0.0f, 0.0f };
static u8 lbl_1_data_18[4] = { 0xFF, 0xFF, 0xFF, 0xFF };
static s32 lbl_1_data_1C[8] = {
    0x8C0091, 0x8C0000, 0x8C0086, 0x8C0031,
    0x8C0001, 0x8C002C, 0x8C0017, 0x8C0019
};
static s32 lbl_1_data_3C[5] = { 0x72000B, 0x72000C, 0x72000D, 0x72000E, 0x72000F };
static OMCAMERAVIEW lbl_1_data_50 = { { 0.0f, 241.9f, 304.5f }, { -14.8f, 0.0f, 0.0f }, 1933.0f };
static M559VEC lbl_1_data_6C[4] = {
    { -1000.0f, 0.0f, 0.0f }, { -800.0f, 0.0f, 0.0f },
    { 800.0f, 0.0f, 0.0f }, { 1000.0f, 0.0f, 0.0f }
};
static M559VEC lbl_1_data_9C[4] = {
    { -300.0f, 0.0f, 0.0f }, { -100.36f, 0.0f, 0.0f },
    { 100.36f, 0.0f, 0.0f }, { 300.66f, 0.0f, 0.0f }
};
static M559VEC lbl_1_data_CC = { 400.0f, 110.0f, -950.0f };
static M559VEC lbl_1_data_D8 = { 200.0f, 110.0f, -950.0f };
static M559VEC lbl_1_data_E4 = { -100.0f, 8000.0f, 0.0f };
static M559VEC lbl_1_data_F0 = { 0.0f, 1.0f, 0.0f };
static M559VEC lbl_1_data_FC = { 0.0f, 0.0f, 0.0f };

static void fn_1_20D8(s16 mode, s16 frameNo);
static void fn_1_23C8(s16 mode, s16 frameNo);
static void fn_1_25F8(s16 mode, s16 frameNo);
static void fn_1_265C(s16 mode, s16 frameNo);
static void fn_1_2714(s16 mode, s16 frameNo);
static void fn_1_27A0(s16 mode, s16 frameNo);
static void fn_1_3068(OMOBJ *obj);
static void fn_1_3A1C(void);
static void fn_1_3DA8(void);
static void fn_1_40A4(void);
static void fn_1_413C(void);
static s32 fn_1_542C(void);
static void fn_1_54C4(void);
static void fn_1_5514(void);
static void fn_1_5564(s32 *flag);
static void fn_1_5578(void);
static void fn_1_55A8(void);
static void fn_1_29D4(void);
static void fn_1_2948(OMOBJ *obj, s16 motion, u32 attr, f32 endTime);
static void fn_1_3854(OMOBJ *obj, s16 motion, u32 attr, f32 endTime);
static void fn_1_5038(OMOBJ *obj);
static void fn_1_F48(OMOBJ *obj);
static void fn_1_1070(OMOBJ *obj);
static void fn_1_29D8(OMOBJ *obj);
static s16 fn_1_55D8(HuVecF *pos);

static MGSEQ_PARAM lbl_1_data_108 = {
    0,
    MGSEQ_TIMER_TOP,
    fn_1_20D8,
    fn_1_23C8,
    NULL,
    fn_1_25F8,
    fn_1_265C,
    fn_1_2714,
    fn_1_27A0,
    NULL,
    NULL
};

static char lbl_1_data_130[] = "******* M559 ObjectSetup *********\n";
static char lbl_1_data_170[] = "Center X:%.02f Y:%.02f Z:%.02f";
static char lbl_1_data_18F[] = "CRot X:%.02f Y:%.02f Z:%.02f";
static char lbl_1_data_1AC[] = "CZoom:%.02f";
static char lbl_1_data_1B8[] = "g000m0-itemhook_M";
static char lbl_1_data_1CA[] = "g000m0-ns_hed";

static ANIMDATA *lbl_1_bss_0;
static s32 lbl_1_bss_4;
static s32 lbl_1_bss_8;
static f32 lbl_1_bss_C;
static s16 lbl_1_bss_10;
static s16 lbl_1_bss_12;
static M559VEC lbl_1_bss_14;
static M559VEC lbl_1_bss_20;
static M559VEC lbl_1_bss_2C;
static s32 lbl_1_bss_38;
static s32 lbl_1_bss_3C;
static s32 lbl_1_bss_40;
static s32 lbl_1_bss_44;
static M559VEC lbl_1_bss_48[0x30];
static M559VEC lbl_1_bss_288[0x64];
static s32 lbl_1_bss_738;
static s32 lbl_1_bss_73C;
static s16 lbl_1_bss_740;
static s16 lbl_1_bss_742;
static s16 lbl_1_bss_744;
static s16 lbl_1_bss_746[7];
static OMOBJ *lbl_1_bss_754[0x30];
static OMOBJ *lbl_1_bss_814[0x65];
static OMOBJ **lbl_1_bss_9A8;
static OMOBJ **lbl_1_bss_9AC;
typedef struct M559Part_s {
    u32 unk0;
    s16 unk4;
    s16 pad6;
} M559PART; // size = 0x8
static M559PART lbl_1_bss_9B0[0x20];
static s32 lbl_1_bss_AB4;
static s32 lbl_1_bss_AB8;
static M559VEC lbl_1_bss_AC4[4];
static M559VEC lbl_1_bss_AF4[4];
static M559VEC lbl_1_bss_B24[4];
static M559VEC lbl_1_bss_B54[4];
static M559VEC lbl_1_bss_B84;
static M559VEC lbl_1_bss_B90;
static M559VEC lbl_1_bss_B9C;
static M559VEC lbl_1_bss_BA8;
static OMOBJMAN *lbl_1_bss_BB4;
static M559LISTHEAD lbl_1_bss_BB8;

static s32 fn_1_1210(M559VEC *target);
s32 fn_1_1894(OMOBJ *arg0);
s32 fn_1_19D8(OMOBJ *arg0);
static s32 fn_1_1394(M559VEC *pos, M559HOOK *hook, f32 range);
static s32 fn_1_1668(M559VEC *target, M559HOOK *hook, f32 range);
static void fn_1_1B1C(M559HOOK *hook, OMOBJ *obj);
static void fn_1_1F28(M559HOOK *hook);
static void fn_1_2CA8(void);
static s32 fn_1_3918(s32 no, f32 a, f32 b);
static void fn_1_41D4(f32 a, f32 b, f32 c);
static void fn_1_4364(OMOBJ *obj);
static void fn_1_4504(OMOBJ *obj);
static void fn_1_4848(OMOBJ *obj);
static void fn_1_4910(f32 a, f32 b, f32 c);
static void fn_1_4A9C(OMOBJ *obj);
static void fn_1_4C3C(OMOBJ *obj);
static void fn_1_4F70(OMOBJ *obj);
static f32 fn_1_5308(s32 a, s32 b, s32 c);
static s16 fn_1_5654(HuVecF *pos);
static void fn_1_56D0(void);
static void fn_1_56F4(u16 a, s16 b);
static s16 fn_1_577C(s16 no);
static void fn_1_5820(void);
static s16 fn_1_5878(u32 dataNum);
static void fn_1_598C(M559VEC *a, M559VEC *b, M559VEC *out, f32 t);
static void fn_1_5AE8(M559VEC *a, M559VEC *b, M559VEC *out);
static s32 fn_1_5B74(HuVecF *pos, f32 range);
static s32 fn_1_5EA8(HuVecF *pos, f32 range);
static f32 fn_1_628C(f32 range, f32 farg1, f32 farg2);
static void fn_1_6390(s32 no);
static void fn_1_63FC(s16 modelId, char *objName, M559VEC *out);
static s32 fn_1_6454(s32 a, s32 b);
static s32 fn_1_6490(M559VEC *a, M559VEC *b, f32 range, f32 farg1);
static s32 fn_1_64F0(HuVecF *a, s32 b, s32 c);
static s32 fn_1_65B4(HuVecF *a, s32 b, s32 c);
static void fn_1_6678(M559HOOKDATA *data);
static void fn_1_69D8(M559VERTS *a, M559VERTS *node);
static void fn_1_6A54(M559VERTS *a, M559VERTS *node, s16 no, f32 range);
static void fn_1_6AB0(M559VERTS *a, f32 *b, s16 c, f32 range, f32 e);
static void fn_1_6B54(HU3DMODEL *model, f32 (*mtx)[4]);
static void fn_1_729C(M559VEC *a, s16 b, s16 c, f32 d, f32 e);
static s16 fn_1_7368(ANIMDATA *anim, M559VEC *field8, f32 *fieldC, s16 partCount, s16 cols, s16 rows, f32 range);
static void *fn_1_79C0(s16 modelId, M559VEC *pos, M559CLR *clr, f32 unk2C, f32 unk28);
static void fn_1_7AC8(s16 modelId, M559PARTCALLBACK func);
static void fn_1_7B00(s16 modelId, s8 flag);
static void fn_1_7B30(s16 modelId);
static s32 fn_1_7B6C(s16 modelId);
static void fn_1_7C28(s16 modelId, s16 count, M559VEC *pos, f32 scale, f32 range, f32 time, s16 flag);
static s16 fn_1_7E88(s32 a, s32 b, s16 c);


s32 _prolog(void) {
    const VoidFunc *ctors = _ctors;
    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}
void _epilog(void) {
    const VoidFunc *dtors = _dtors;
    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}
void ObjectSetup(void) {
    OMOBJ *temp_r3;
    OMOBJ *temp_r3_2;
    s32 var_r31;
    s32 var_r31_2;
    s32 var_r31_3;

    OSReport("******* M559 ObjectSetup *********\n");
    lbl_1_bss_BB4 = omInitObjMan(0xC8, 0x2000);
    omGameSysInit(lbl_1_bss_BB4);
    lbl_1_bss_12 = Hu3DMotionCreate(HuDataSelHeapReadNum(0x720010, 0x10000000, HEAP_MODEL));
    lbl_1_bss_10 = Hu3DModelCameraCreate(lbl_1_bss_12, 1U);
    HuPrcVSleep(&lbl_1_bss_10);
    lbl_1_bss_C = Hu3DMotionMotionMaxTimeGet(lbl_1_bss_12);
    Hu3DCameraPosGet(1, (Vec *) &lbl_1_bss_2C, (Vec *) &lbl_1_bss_20, (Vec *) &lbl_1_bss_14);
    HuPrcVSleep();
    Hu3DModelKill(lbl_1_bss_10);
    HuPrcVSleep();
    Hu3DCameraCreate(1);
    Hu3DCameraPerspectiveSet(1, 30.0f, 20.0f, 15000.0f, 1.2f);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraPosSetV(1, (Vec *) &lbl_1_bss_2C, (Vec *) &lbl_1_bss_20, (Vec *) &lbl_1_bss_14);
    lbl_1_bss_744 = Hu3DGLightCreate(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0U, 0U, 0U);
    Hu3DGLightPosAimSetV(lbl_1_bss_744, (HuVecF *) &lbl_1_data_0, (HuVecF *) &lbl_1_data_C);
    Hu3DGLightStaticSet(lbl_1_bss_744, 1);
    Hu3DGLightInfinitytSet(lbl_1_bss_744);
    Hu3DGLightColorSet(lbl_1_bss_744, lbl_1_data_18[0], lbl_1_data_18[1], lbl_1_data_18[2], lbl_1_data_18[3]);
    Hu3DShadowCreate(10.0f, 10.0f, 10000.0f);
    Hu3DShadowColSet(0x10U, 0x10U, 0x10U);
    Hu3DShadowPosSet((HuVecF *) &lbl_1_data_E4, (HuVecF *) &lbl_1_data_F0, (HuVecF *) &lbl_1_data_FC);
    Hu3DShadowTPLvlSet(0.3f);
    omMakeGroupEx(lbl_1_bss_BB4, 0U, 4U);
    lbl_1_bss_9AC = omGetGroupMemberListEx(lbl_1_bss_BB4, 0);
    var_r31 = 0;
loop_2:
    if (var_r31 < 4) {
        temp_r3 = omAddObjEx(lbl_1_bss_BB4, 0x64, 1U, 8U, 0, fn_1_694);
        lbl_1_bss_9AC[var_r31] = temp_r3;
        temp_r3->data = HuMemDirectMallocNum(HEAP_HEAP, 0x8C, 0x10000000U);
        temp_r3->work[0] = (u32) var_r31;
        var_r31 += 1;
        goto loop_2;
    }
    CharEffectLayerSet(4);
    omMakeGroupEx(lbl_1_bss_BB4, 3U, 1U);
    lbl_1_bss_9A8 = omGetGroupMemberListEx(lbl_1_bss_BB4, 3);
    temp_r3_2 = omAddObjEx(lbl_1_bss_BB4, 0x67, 1U, 5U, 3, fn_1_3068);
    *lbl_1_bss_9A8 = temp_r3_2;
    temp_r3_2->data = HuMemDirectMallocNum(HEAP_HEAP, 0x2C, 0x10000000U);
    lbl_1_bss_0 = HuSprAnimRead(HuDataReadNum(0x720011, 0x10000000));
    HuSprAnimLock(lbl_1_bss_0);
    lbl_1_bss_742 = Hu3DModelCreate(HuDataSelHeapReadNum(0x720006, 0x10000000, HEAP_MODEL));
    lbl_1_bss_740 = Hu3DModelCreate(HuDataSelHeapReadNum(0x720007, 0x10000000, HEAP_MODEL));
    Hu3DModelAttrSet(lbl_1_bss_742, 1U);
    Hu3DModelAttrSet(lbl_1_bss_742, 0x40000001U);
    Hu3DModelAttrSet(lbl_1_bss_740, 1U);
    lbl_1_bss_73C = 0;
    lbl_1_bss_738 = 0;
    var_r31_2 = 0;
loop_5:
    if (var_r31_2 < 0x64) {
        lbl_1_bss_814[var_r31_2] = NULL;
        var_r31_2 += 1;
        goto loop_5;
    }
    var_r31_3 = 0;
loop_8:
    if (var_r31_3 < 0x30) {
        lbl_1_bss_754[var_r31_3] = NULL;
        var_r31_3 += 1;
        goto loop_8;
    }
    omMakeGroupEx(lbl_1_bss_BB4, 1U, 0x64U);
    omGetGroupMemberListEx(lbl_1_bss_BB4, 1);
    omMakeGroupEx(lbl_1_bss_BB4, 2U, 0x30U);
    omGetGroupMemberListEx(lbl_1_bss_BB4, 2);
    fn_1_3A1C();
    fn_1_3DA8();
    lbl_1_bss_3C = 0;
    lbl_1_bss_38 = 0;
    fn_1_5564(&lbl_1_bss_38);
    MgSeqCreatePrio(&lbl_1_data_108, 0x3E8);
}
void fn_1_694(OMOBJ *obj) {
    s32 temp_r0;
    s32 var_r29;
    M559HOOK *temp_r31;

    temp_r31 = obj->data;
    temp_r31->playerNo = (u32) obj->work[0];
    temp_r31->charNo = (s32) GwPlayerConf[temp_r31->playerNo].charNo;
    temp_r31->padNo = (s32) GwPlayerConf[temp_r31->playerNo].padNo;
    temp_r31->stickY = 0.0f;
    temp_r31->stickX = 0.0f;
    temp_r31->mode = 0;
    temp_r31->timer = 0;
    temp_r31->pad68 = 0;
    *obj->mdlId = CharModelCreate((s16) temp_r31->charNo, 2);
    var_r29 = 0;
loop_2:
    if (var_r29 < 8) {
        obj->mtnId[var_r29] = CharMotionCreate((s16) temp_r31->charNo, lbl_1_data_1C[var_r29]);
        CharMotionSet((s16) temp_r31->charNo, obj->mtnId[var_r29]);
        var_r29 += 1;
        goto loop_2;
    }
    temp_r31->state = 1;
    Hu3DModelAttrSet(*obj->mdlId, 1U);
    Hu3DModelAttrSet(*obj->mdlId, 0x40000001U);
    Hu3DMotionSet(*obj->mdlId, obj->mtnId[1]);
    Hu3DModelShadowSet(*obj->mdlId);
    Hu3DModelLayerSet(*obj->mdlId, 4);
    CharMotionUpdateSet((s16) temp_r31->charNo, 0x8C0017U, 0);
    temp_r31->dif = (s32) GwPlayerConf[temp_r31->playerNo].dif;
    temp_r31->timer3 = 0;
    temp_r31->subMode = 0;
    temp_r0 = temp_r31->dif;
    switch (temp_r0) {                              /* irregular */
    case 0:
        temp_r31->waitTime = (s32) (frandmod(5U) + 0x14);
        break;
    case 1:
        temp_r31->waitTime = (s32) (frandmod(5U) + 0xF);
        break;
    case 2:
        temp_r31->waitTime = (s32) (frandmod(4U) + 6);
        break;
    case 3:
        temp_r31->waitTime = (s32) (frandmod(4U) + 1);
        break;
    }
    temp_r31->rot.z = 0.0f;
    temp_r31->rot.x = 0.0f;
    if ((s32) temp_r31->playerNo < 2) {
        temp_r31->rot.y = 90.0f;
    } else {
        temp_r31->rot.y = -90.0f;
    }
    temp_r31->target.x = 0.0f;
    temp_r31->target.y = 0.0f;
    temp_r31->target.z = 0.0f;
    temp_r31->pos.x = (f32) lbl_1_data_6C[temp_r31->playerNo].x;
    temp_r31->pos.y = (f32) lbl_1_data_6C[temp_r31->playerNo].y;
    temp_r31->pos.z = (f32) lbl_1_data_6C[temp_r31->playerNo].z;
    omSetTra(obj, temp_r31->pos.x, temp_r31->pos.y, temp_r31->pos.z);
    omSetRot(obj, temp_r31->rot.x, temp_r31->rot.y, temp_r31->rot.z);
    temp_r31->hookModel = Hu3DModelCreate(HuDataSelHeapReadNum(0x720008, 0x10000000, HEAP_MODEL));
    Hu3DModelLayerSet(temp_r31->hookModel, 4);
    Hu3DModelAttrSet(temp_r31->hookModel, 0x40000002U);
    Hu3DModelAttrSet(temp_r31->hookModel, 1U);
    Hu3DModelPosSet(temp_r31->hookModel, temp_r31->pos.x, temp_r31->pos.y, temp_r31->pos.z);
    Hu3DModelAttrReset(*obj->mdlId, 1U);
    CharMotionDataClose((s16) temp_r31->charNo);
    obj->objFunc = fn_1_A5C;
}
static void fn_1_A5C(OMOBJ *obj) {
    Point3d sp8;
    f32 temp_r3;

    u32 temp_r0;

    M559HOOK *temp_r31;

    temp_r31 = obj->data;
    Hu3D3Dto2D(temp_r31 + 0x28, 1, &sp8);
    CharModelVoicePanSet((s16) temp_r31->charNo, (s16) (32.0f + ((sp8.x / 576.0f) * 64.0f)));


    if (MgSeqModeGet() == MGSEQ_MODE_MAIN) {
        if ((s16) GwPlayerConf[temp_r31->playerNo].type != 0) {
            if (((s32) temp_r31->dif == 0) || ((s32) temp_r31->dif == 1)) {
        fn_1_F48(obj);
            } else {
        fn_1_1070(obj);
            }
        } else {
            temp_r31->stickX = (f32) (s8) (u8) HuPadStkX[temp_r31->padNo];

            temp_r31->stickY = (f32) (s8) (u8) HuPadStkY[temp_r31->padNo];
        }
    }
    temp_r0 = temp_r31->mode;
    switch (temp_r0) {
    case 0:
        fn_1_2948(obj, 1, 0x40000001U, 8.0f);
        break;
    case 1:
        fn_1_2948(obj, 5, 0x40000001U, 8.0f);
        if ((f32) temp_r31->timer > 90.0f) {
            temp_r31->timer = 0;
            temp_r31->rot.y = 0.0f;
            temp_r31->mode = 2U;
            temp_r31->pos.x = (f32) lbl_1_data_9C[temp_r31->playerNo].x;
            temp_r31->pos.y = (f32) lbl_1_data_9C[temp_r31->playerNo].y;
            temp_r31->pos.z = (f32) lbl_1_data_9C[temp_r31->playerNo].z;
            fn_1_2948(obj, 7, 0x40000001U, 8.0f);
        } else {
            if ((s32) temp_r31->playerNo < 2) {
                temp_r31->pos.x = (f32) (temp_r31->pos.x + 7.7777777f);
            } else {
                temp_r31->pos.x = (f32) (temp_r31->pos.x - 7.7777777f);
            }
            temp_r31->timer = (s32) (temp_r31->timer + 1);
        }
        break;
    case 2:
        temp_r3 = temp_r31->pos.x;
        temp_r31->prevPos.x = temp_r3;
        temp_r31->prevPos.y = (f32) temp_r31->pos.y;
        temp_r31->prevPos.z = (f32) temp_r31->pos.z;
        if (MgSeqModeGet() == 5) {
            fn_1_1B1C(temp_r31, obj);
        }
        break;
    case 3:
        fn_1_2948(obj, 1, 0U, 8.0f);
        if ((f32) temp_r31->timer > 40.0f) {
            temp_r31->timer = 0;
            temp_r31->mode = 4U;
        } else {
            temp_r31->timer = (s32) (temp_r31->timer + 1);
            if ((s32) temp_r31->timer < 0xF) {
                fn_1_5038(obj);
            }
        }
        break;
    case 4:
        fn_1_2948(obj, 6, 0x40000001U, 8.0f);
        break;
    case 5:
        fn_1_2948(obj, 4, 0x40000001U, 8.0f);
        if ((f32) temp_r31->timer > 30.0f) {
            temp_r31->timer = 0;
            temp_r31->mode = 6U;
            fn_1_2948(obj, 1, 0x40000001U, 8.0f);
        } else {
            temp_r31->rot.y = (f32) (temp_r31->rot.y + fn_1_5308((s32) temp_r31->field5C, 0, 0x1E));
            temp_r31->timer = (s32) (temp_r31->timer + 1);
        }
        break;
    }
    omSetTra(obj, temp_r31->pos.x, temp_r31->pos.y, temp_r31->pos.z);
    omSetRot(obj, temp_r31->rot.x, temp_r31->rot.y, temp_r31->rot.z);
    Hu3DModelPosSet(temp_r31->hookModel, temp_r31->pos.x, temp_r31->pos.y, temp_r31->pos.z);
}
void fn_1_F48(OMOBJ *arg0) {
    s32 temp_r0;
    M559HOOK *temp_r31;

    temp_r31 = arg0->data;
    temp_r31->stickY = 0.0f;
    temp_r31->stickX = 0.0f;
    temp_r0 = temp_r31->subMode;
    switch (temp_r0) {                              /* irregular */
    case 0:
        if (fn_1_1210(&temp_r31->target) != 0) {
            temp_r31->subMode = 1;
            return;
        }
        return;
    case 1:
        temp_r31->timer2 = (s32) (temp_r31->timer2 + 1);
        if ((s32) temp_r31->timer2 > 0x78) {
            temp_r31->timer2 = 0;
            temp_r31->subMode = 0;
            return;
        }
        if (fn_1_1894(arg0) != 0) {
            temp_r31->subMode = 2;
            temp_r31->stickY = 0.0f;
            temp_r31->stickX = 0.0f;
            return;
        }
        break;
    case 2:
        temp_r31->stickY = 0.0f;
        temp_r31->stickX = 0.0f;
        temp_r31->timer3 = (s32) (temp_r31->timer3 + 1);
        if ((s32) temp_r31->timer3 > (s32) temp_r31->waitTime) {
            temp_r31->subMode = 0;
            temp_r31->timer3 = 0;
        }
        break;
    }
}
void fn_1_1070(OMOBJ *arg0) {
    f32 var_f31;
    M559HOOK *temp_r31;

    temp_r31 = arg0->data;
    if ((s32) temp_r31->pad68 != 0) {
        temp_r31->stickX = 0.0f;
        temp_r31->stickY = 0.0f;
        return;
    }
    if ((s32) temp_r31->dif == 2) {
        var_f31 = 200.0f;
    } else {
        var_f31 = 150.0f;
    }
    if (MgSeqFrameNoGet() == 0x23) {
        if (fn_1_1394(&temp_r31->pos, temp_r31, var_f31) != 0) {
            temp_r31->subMode = 1;
        }
    } else if ((MgSeqFrameNoGet() == 0xE6) && (fn_1_1668(&temp_r31->target, temp_r31, var_f31) != 0)) {
        temp_r31->subMode = 1;
    }
    if (((temp_r31->target.x != 0.0f) || (temp_r31->target.z != 0.0f)) && (fn_1_19D8(arg0) != 0)) {
        temp_r31->stickX = 0.0f;
        temp_r31->stickY = 0.0f;
        temp_r31->pos.x = (f32) temp_r31->target.x;
        temp_r31->pos.y = (f32) temp_r31->target.y;
        temp_r31->pos.z = (f32) temp_r31->target.z;
        temp_r31->target.x = 0.0f;
        temp_r31->target.y = 0.0f;
        temp_r31->target.z = 0.0f;
    }
}
static s32 fn_1_1210(M559VEC *target) {
    f32 sp1C;
    f32 sp18;
    f32 sp14;
    f32 sp10;
    f32 spC;
    f32 sp8;
    f32 temp_f0;
    f32 temp_f0_2;
    s32 var_r30;

    sp8 = 0.0f;
    spC = 0.0f;
    sp10 = 0.0f;
    do {
        temp_f0 = (f32) frandmod(0x514U) - 635.0f;
        sp14 = temp_f0;
        sp18 = 0.0f;
        temp_f0_2 = (f32) frandmod(0x514U) - 635.0f;
        sp1C = temp_f0_2;
        if ((((sp8 - temp_f0) * (sp8 - temp_f0)) + ((sp10 - temp_f0_2) * (sp10 - temp_f0_2))) < 342225.0f) {
            var_r30 = 1;
        } else {
            var_r30 = 0;
        }
    } while (var_r30 == 0);
    target->x = sp14;
    target->y = 0.0f;
    target->z = sp1C;
}
static s32 fn_1_1394(M559VEC *pos, M559HOOK *hook, f32 range) {
    f32 sp20;
    f32 sp1C;
    f32 sp18;
    f32 sp14;
    f32 sp10;
    f32 spC;
    f32 sp8;
    f32 temp_f0;
    f32 temp_f0_2;
    s32 var_r26;
    s32 var_r27;
    s32 var_r28;
    s32 var_r30;
    M559HOOK *temp_r31;

    sp8 = pos->x;
    spC = 0.0f;
    sp10 = 0.0f;
    sp14 = 0.0f;
    do {
loop_1:
        temp_f0 = (f32) frandmod(0x514U) - 635.0f;
        sp18 = temp_f0;
        sp1C = 0.0f;
        temp_f0_2 = (f32) frandmod(0x514U) - 635.0f;
        sp20 = temp_f0_2;
        if ((((spC - temp_f0) * (spC - temp_f0)) + ((sp14 - temp_f0_2) * (sp14 - temp_f0_2))) < 342225.0f) {
            var_r28 = 1;
        } else {
            var_r28 = 0;
        }
        if (var_r28 == 0) {
            goto loop_1;
        }
        if ((((sp18 -  sp8) * (sp18 -  sp8)) + ((sp20 - sp10) * (sp20 - sp10))) < 40000.0f) {
            var_r27 = 1;
        } else {
            var_r27 = 0;
        }
        if (var_r27 == 0) {
            goto loop_1;
        }
        var_r30 = 0;
loop_16:
        if (var_r30 < 0x64) {
            if ((OMOBJ *) lbl_1_bss_814[var_r30] != NULL) {
                temp_r31 = lbl_1_bss_814[var_r30]->data;
                if ((((temp_r31->unk4 - sp18) * (temp_r31->unk4 - sp18)) + ((temp_r31->unkC - sp20) * (temp_r31->unkC - sp20))) < 5625.0f) {
                    var_r26 = 1;
                } else {
                    var_r26 = 0;
                }
                if (var_r26 == 0) {
                    goto block_15;
                }
            } else {
block_15:
                var_r30 += 1;
                goto loop_16;
            }
        }
    } while (var_r30 != 0x64);
    if ((sp18 == 0.0f) && (sp20 == 0.0f)) {
        sp18 = 0.1f;
    }
    hook->unk0 = sp18;
    hook->unk4 = 0.0f;
    hook->unk8 = sp20;
}
static s32 fn_1_1668(M559VEC *target, M559HOOK *hook, f32 range) {
    f32 sp1C;
    f32 sp18;
    f32 sp14;
    f32 sp10;
f32 spC;
    f32 sp8;
    f32 temp_f0;
    f32 temp_f0_2;
    s32 var_r27;
    s32 var_r28;
    s32 var_r30;
    M559HOOK *temp_r31;

    sp8 = 0.0f;
    spC = 0.0f;
    sp10 = 0.0f;
    do {
loop_1:
        temp_f0 = (f32) frandmod(0x514U) - 635.0f;
        sp14 = temp_f0;
        sp18 = 0.0f;
        temp_f0_2 = (f32) frandmod(0x514U) - 635.0f;
        sp1C = temp_f0_2;
        if ((((sp8 - temp_f0) * (sp8 - temp_f0)) + ((sp10 - temp_f0_2) * (sp10 - temp_f0_2))) < 342225.0f) {
            var_r28 = 1;
        } else {
            var_r28 = 0;
        }
        if (var_r28 == 0) {
            goto loop_1;
        }
        var_r30 = 0;
loop_12:
        if (var_r30 < 0x30) {
            if ((OMOBJ *) lbl_1_bss_754[var_r30] != NULL) {
                temp_r31 = lbl_1_bss_754[var_r30]->data;
                if ((((temp_r31->unk4 - sp14) * (temp_r31->unk4 - sp14)) + ((temp_r31->unkC - sp1C) * (temp_r31->unkC - sp1C))) < 5625.0f) {
                    var_r27 = 1;
                } else {
                    var_r27 = 0;
                }
                if (var_r27 == 0) {
                    goto block_11;
                }
            } else {
block_11:
                var_r30 += 1;
                goto loop_12;
            }
        }
    } while (var_r30 != 0x30);
    hook->unk0 = sp14;
    hook->unk4 = 0.0f;
    hook->unk8 = sp1C;
}
s32 fn_1_1894(OMOBJ *arg0) {
    Point3d sp14;
    f32 spC;
    f32 sp8;
    f32 temp_f0;
    M559HOOK *temp_r31;

    temp_r31 = arg0->data;
    PSVECSubtract((Vec *) &temp_r31->target, (Vec *) &temp_r31->pos, &sp14);
    sp14.y = 0.0f;
    if (PSVECSquareMag(&sp14) < 25.001f) {
        return 1;
    }
    PSVECNormalize(&sp14, &sp14);
    temp_f0 = sp14.x;
    sp8 = temp_f0;
    spC = sp14.z;
    temp_r31->stickX = (f32) (s8) (56.0f * temp_f0);
    temp_r31->stickY = (f32) (s8) (56.0f * -sp14.z);
    return 0;
}
s32 fn_1_19D8(OMOBJ *arg0) {
    Point3d sp14;
    f32 spC;
    f32 sp8;
    f32 temp_f0;
    M559HOOK *temp_r31;

    temp_r31 = arg0->data;
    PSVECSubtract((Vec *) &temp_r31->target, (Vec *) &temp_r31->pos, &sp14);
    sp14.y = 0.0f;
    if (PSVECSquareMag(&sp14) < 25.001f) {
        return 1;
    }
    PSVECNormalize(&sp14, &sp14);
    temp_f0 = sp14.x;
    sp8 = temp_f0;
    spC = sp14.z;
    temp_r31->stickX = (f32) (s8) (56.0f * temp_f0);
    temp_r31->stickY = (f32) (s8) (56.0f * -sp14.z);
    return 0;
}
static void fn_1_1B1C(M559HOOK *hook, OMOBJ *obj) {
    f32 sp8;
    f32 temp_f26;
    f32 temp_f30;
    f32 var_f27;
    f32 var_f28;
    f32 var_f29;
    f64 temp_f31;
    f64 temp_f31_2;
    f64 temp_f31_3;
    s16 var_r30;

    hook->unk50 = 0.0f;
    hook->unk54 = 0.0f;
    hook->unk58 = 0.0f;
    hook->unk50 = (f32) (hook->unk60 * 0.25f);
    hook->unk58 = (f32) (-hook->unk64 * 0.25f);
    var_r30 = 1;
    temp_f30 = (hook->unk50 * hook->unk50) + (hook->unk58 * hook->unk58);
    if (temp_f30 > 0.0f) {
        temp_f31 = __frsqrte(temp_f30);
        temp_f31_2 = 0.5 * temp_f31 * (3.0 - ((f64) temp_f30 * (temp_f31 * temp_f31)));
        temp_f31_3 = 0.5 * temp_f31_2 * (3.0 - ((f64) temp_f30 * (temp_f31_2 * temp_f31_2)));
        sp8 = (f32) ((f64) temp_f30 * (0.5 * temp_f31_3 * (3.0 - ((f64) temp_f30 * (temp_f31_3 * temp_f31_3)))));
        var_f27 = sp8;
    } else {
        var_f27 = temp_f30;
    }
    hook->unk4C = var_f27;
    if (hook->unk4C > 0.0f) {
                M2C_ERROR(/* unknown instruction: cror eq, lt, eq */);
        if (hook->unk4C == 10.0f) {
            hook->unk50 = (f32) (hook->unk50 / hook->unk4C);
            hook->unk58 = (f32) (hook->unk58 / hook->unk4C);
            hook->unk4C = 10.0f;
            hook->unk50 = (f32) (10.0f * hook->unk50);
            hook->unk58 = (f32) (10.0f * hook->unk58);
        }
        temp_f26 = hook->unk44;
        var_f29 = (f32) fmod((f64) ((f32) (180.0 * (atan2((f64) hook->unk50, (f64) hook->unk58) / 3.141592653589793)) - temp_f26), 360.0);
        if (var_f29 < 0.0f) {
            var_f29 += 360.0f;
        }
        if (var_f29 > 180.0f) {
            var_f29 -= 360.0f;
        }
        var_f28 = (f32) fmod((f64) (temp_f26 + (var_f29 * 0.4f)), 360.0);
        if (var_f28 < 0.0f) {
            var_f28 += 360.0f;
        }
        hook->unk44 = var_f28;
                M2C_ERROR(/* unknown instruction: cror eq, lt, eq */);
        if (hook->unk4C == 7.0f) {
            var_r30 = 5;
        } else {
            var_r30 = 4;
        }
    }
    PSVECAdd((Vec *) &hook->pos, (Vec *) &hook->offset50, (Vec *) &hook->pos);
    fn_1_1F28(hook);
    fn_1_29D8(obj);
    if ((s16) hook->unk0 != var_r30) {
        hook->unk0 = var_r30;
        Hu3DMotionShiftSet(*obj->mdlId, obj->mtnId[hook->unk0], 0.0f, 8.0f, 0x40000001U);
    }
}
static void fn_1_1F28(M559HOOK *hook) {
    f32 sp8;
    f32 temp_f30;
    f32 temp_f31;
    f32 var_f27;
    f64 temp_f29;
    f64 temp_f29_2;
    f64 temp_f29_3;

    temp_f31 = hook->unk28;
    temp_f30 = hook->unk30;
    if (((temp_f31 * temp_f31) + (temp_f30 * temp_f30)) > 0.0f) {
        temp_f29 = __frsqrte((temp_f31 * temp_f31) + (temp_f30 * temp_f30));
        temp_f29_2 = 0.5 * temp_f29 * (3.0 - ((f64) ((temp_f31 * temp_f31) + (temp_f30 * temp_f30)) * (temp_f29 * temp_f29)));
        temp_f29_3 = 0.5 * temp_f29_2 * (3.0 - ((f64) ((temp_f31 * temp_f31) + (temp_f30 * temp_f30)) * (temp_f29_2 * temp_f29_2)));
        sp8 = (f32) ((f64) ((temp_f31 * temp_f31) + (temp_f30 * temp_f30)) * (0.5 * temp_f29_3 * (3.0 - ((f64) ((temp_f31 * temp_f31) + (temp_f30 * temp_f30)) * (temp_f29_3 * temp_f29_3)))));
        var_f27 = sp8;
    } else {
        var_f27 = (temp_f31 * temp_f31) + (temp_f30 * temp_f30);
    }
    if (var_f27 > 585.0f) {
        hook->unk28 = (f32) (585.0f * (temp_f31 / var_f27));
        hook->unk30 = (f32) (585.0f * (temp_f30 / var_f27));
    }
}
static void fn_1_20D8(s16 mode, s16 frameNo) {
    lbl_1_bss_746[0] = Hu3DModelCreate(HuDataSelHeapReadNum(0x720000, 0x10000000, HEAP_MODEL));
    lbl_1_bss_746[1] = Hu3DModelCreate(HuDataSelHeapReadNum(0x720001, 0x10000000, HEAP_MODEL));
    lbl_1_bss_746[2] = Hu3DModelCreate(HuDataSelHeapReadNum(0x720002, 0x10000000, HEAP_MODEL));
    lbl_1_bss_746[3] = Hu3DModelCreate(HuDataSelHeapReadNum(0x720003, 0x10000000, HEAP_MODEL));
    lbl_1_bss_746[4] = Hu3DModelCreate(HuDataSelHeapReadNum(0x720004, 0x10000000, HEAP_MODEL));
    lbl_1_bss_746[5] = Hu3DModelCreate(HuDataSelHeapReadNum(0x720005, 0x10000000, HEAP_MODEL));
    lbl_1_bss_746[6] = Hu3DModelCreate(HuDataSelHeapReadNum(0x720009, 0x10000000, HEAP_MODEL));
    Hu3DModelAttrSet(lbl_1_bss_746[4], 0x40000002U);
    Hu3DModelAttrSet(lbl_1_bss_746[5], 0x40000002U);
    Hu3DModelAttrSet(lbl_1_bss_746[6], 0x40000002U);
    Hu3DModelShadowMapSet(lbl_1_bss_746[0]);
    Hu3DModelLayerSet(lbl_1_bss_746[0], 1);
    Hu3DModelLayerSet(lbl_1_bss_746[1], 1);
    Hu3DModelLayerSet(lbl_1_bss_746[2], 1);
    Hu3DModelLayerSet(lbl_1_bss_746[3], 1);
    Hu3DModelLayerSet(lbl_1_bss_746[4], 1);
    Hu3DModelLayerSet(lbl_1_bss_746[5], 1);
    Hu3DModelLayerSet(lbl_1_bss_746[6], 1);
    Hu3DModelAttrSet(lbl_1_bss_746[2], 0x40000001U);
    Hu3DModelAttrSet(lbl_1_bss_746[6], 0x40000002U);
    Hu3DModelAttrSet(lbl_1_bss_746[6], 1U);
    Hu3DCameraPosSetV(1, (Vec *) &lbl_1_bss_2C, (Vec *) &lbl_1_bss_20, (Vec *) &lbl_1_bss_14);
    HuPrcVSleep();
    lbl_1_bss_10 = Hu3DModelCameraCreate(lbl_1_bss_12, 1U);
    Hu3DMotionSpeedSet(lbl_1_bss_10, 1.0f);
    Hu3DCameraMotionStart(lbl_1_bss_10, 1U);
    Hu3DModelAttrReset(lbl_1_bss_746[4], 0x40000002U);
    Hu3DModelAttrReset(lbl_1_bss_746[5], 0x40000002U);
    fn_1_40A4();
    fn_1_413C();
    fn_1_5578();
    MgSeqModeNext();
}
static void fn_1_23C8(s16 mode, s16 frameNo) {
    s32 var_r30;
    s32 var_r31;
    s32 var_r31_2;
    s32 var_r31_3;
    M559HOOK *temp_r28;

    temp_r28 = (*lbl_1_bss_9A8)->data;
    var_r30 = 0;
loop_24:
    if ((f32) var_r30 < lbl_1_bss_C) {
        if (var_r30 == 0x14) {
            var_r31 = 0;
loop_4:
            if (var_r31 < 4) {
                M559DATA(lbl_1_bss_9AC[var_r31])->unk20 = 1;
                var_r31 += 1;
                goto loop_4;
            }
        }
        if (var_r30 == 0x50) {
            temp_r28->mode = 1;
        }
        if (var_r30 == 0x8C) {
            lbl_1_bss_4 = HuAudFXPlayPan(0x97C, 0x37);
        }
        if (var_r30 == 0x118) {
            HuAudFXStop(lbl_1_bss_4);
        }
        if (var_r30 == 0x131) {
            HuAudFXPlay(0x97D);
            HuAudFXPlay(0x97E);
            var_r31_2 = 0;
loop_14:
            if (var_r31_2 < 4) {
                omVibrate((s16) M559DATA(lbl_1_bss_9AC[var_r31_2])->unk14, 0x1E, 0xC, 0);
                var_r31_2 += 1;
                goto loop_14;
            }
        }
        if (var_r30 == 0x1BD) {
            HuAudFXPlay(0x97F);
            var_r31_3 = 0;
loop_18:
            if (var_r31_3 < 4) {
                omVibrate((s16) M559DATA(lbl_1_bss_9AC[var_r31_3])->unk14, 0x1E, 0xC, 0);
                var_r31_3 += 1;
                goto loop_18;
            }
        }
        if (var_r30 == 0x190) {
            temp_r28->mode = 3;
        }
        if (var_r30 == 0x154) {
            Hu3DMotionTimeSet(lbl_1_bss_746[6], 0.0f);
            Hu3DModelAttrReset(lbl_1_bss_746[6], 0x40000002U);
            Hu3DModelAttrReset(lbl_1_bss_746[6], 1U);
        }
        HuPrcVSleep();
        var_r30 += 1;
        goto loop_24;
    }
    MgSeqModeNext();
}
static void fn_1_25F8(s16 mode, s16 frameNo) {
    fn_1_29D4();
    if (MgSeqFrameNoGet() == 5) {
        fn_1_54C4();
        lbl_1_bss_40 = HuAudFXPlay(0x981);
    }
    if (MgSeqFrameNoGet() == 0xC8) {
        fn_1_5514();
    }
    if (MgSeqFrameNoGet() == 0x14A) {
        MgSeqModeNext();
    }
}
static void fn_1_265C(s16 mode, s16 frameNo) {
    s32 var_r31;

    if (MgSeqFrameNoGet() == 0) {
        fn_1_55A8();
        HuAudFXFadeOut(lbl_1_bss_40, 0x64);
        var_r31 = 0;
loop_5:
        if (var_r31 < 4) {
            if ((s32) M559DATA(lbl_1_bss_9AC[var_r31])->unk68 == 0) {
                fn_1_2948(lbl_1_bss_9AC[var_r31], 1, 0x40000001U, 8.0f);
            }
            var_r31 += 1;
            goto loop_5;
        }
    }
}
static void fn_1_2714(s16 mode, s16 frameNo) {
    s32 var_r30;
    M559HOOK *temp_r31;

    if (MgSeqFrameNoGet() == 0) {
        var_r30 = 0;
loop_3:
        if (var_r30 < 4) {
            temp_r31 = lbl_1_bss_9AC[var_r30]->data;
            temp_r31->mode = 5;
            temp_r31->field5C = (f32) temp_r31->rot.y;
            temp_r31->timer = 0;
            var_r30 += 1;
            goto loop_3;
        }
    }
    if (MgSeqFrameNoGet() > 0x3CU) {
        MgSeqModeNext();
    }
}
static void fn_1_27A0(s16 mode, s16 frameNo) {
    M559HOOK *sp8;
    OMOBJ *temp_r3;
    s32 temp_r29;
    s32 var_r30;
    M559HOOK *temp_r31;

    temp_r3 = *lbl_1_bss_9A8;
    sp8 = temp_r3->data;
    if (MgSeqFrameNoGet(temp_r3) == 0) {
        var_r30 = 0;
loop_7:
        if (var_r30 < 4) {
            temp_r31 = lbl_1_bss_9AC[var_r30]->data;
            if ((s32) temp_r31->unk68 == 0) {
                fn_1_2948(lbl_1_bss_9AC[temp_r31->unk14], 2, 0U, 8.0f);
                temp_r29 = temp_r31->unk14;
                if (_CheckFlag(0x1000FU) == 0) {
                    GwPlayer[temp_r29].mgCoinBonus = 0xA;
                }
            } else {
                Hu3DMotionOverlaySet(*lbl_1_bss_9AC[temp_r31->unk14]->mdlId, *lbl_1_bss_9AC[temp_r31->unk14]->mtnId);
            }
            var_r30 += 1;
            goto loop_7;
        }
        if (fn_1_542C() != 0) {
            fn_1_3854(*lbl_1_bss_9A8, 4, 0U, 8.0f);
            HuAudJinglePlay(0x7B);
            return;
        }
        fn_1_3854(*lbl_1_bss_9A8, 3, 0U, 8.0f);
        HuAudJinglePlay(0x7A);
    }
}
static void fn_1_2948(OMOBJ *obj, s16 motion, u32 attr, f32 endTime) {
    u32 spC;
    f32 sp8;
    M559HOOK *temp_r31;

    sp8 = endTime;
    spC = attr;
    temp_r31 = obj->data;
    if ((s16) temp_r31->state != motion) {
        temp_r31->state = motion;
        CharMotionShiftSet((s16) temp_r31->charNo, *(obj->mtnId + (temp_r31->state * 2)), 0.0f, sp8, spC);
    }
}
static void fn_1_29D4(void) {

}
static void fn_1_29D8(OMOBJ *obj) {
    Point3d sp2C;
    Point3d sp20;
    f32 sp1C;
    f32 sp18;
    f32 sp14;
    f32 sp10;
    f32 spC;
    f32 sp8;
    f32 temp_f1;
    f32 temp_f1_2;
    f32 temp_f28;
    f32 temp_f30;
    f32 temp_f30_2;
    f32 temp_f31;
    s32 var_r29;
    M559HOOK *temp_r30;
    M559HOOK *temp_r31;

    temp_r31 = obj->data;
    PSVECSubtract((Vec *) &temp_r31->pos, (Vec *) &temp_r31->prevPos, &sp2C);
    temp_f1 = PSVECMag(&sp2C);
    if (temp_f1 != 0.0f) {
        var_r29 = 0;
loop_8:
        if (var_r29 < 4) {
            if (var_r29 != (s32) temp_r31->playerNo) {
                temp_r30 = lbl_1_bss_9AC[var_r29]->data;
                sp14 = temp_r31->pos.x;
                sp18 = temp_r31->pos.y;
                sp1C = temp_r31->pos.z;
                sp18 = 0.0f;
                sp8 = temp_r30->unk8;
                spC = temp_r30->unkC;
                sp10 = temp_r30->unk30;
                spC = 0.0f;
                PSVECSubtract((Point3d *) &sp14, (Point3d *) &sp8, &sp2C);
                temp_f1_2 = PSVECMag(&sp2C);
                M2C_ERROR(/* unknown instruction: cror eq, lt, eq */);
                if (temp_f1_2 != 99.5f) {
                    PSVECSubtract((Vec *) &temp_r30->pos, (Vec *) &temp_r30->prevPos, &sp2C);
                    temp_f28 = PSVECMag(&sp2C);
                    PSVECSubtract((Point3d *) &sp14, (Point3d *) &sp8, &sp20);
                    PSVECNormalize(&sp20, &sp20);
                    temp_f31 = 100.0f - temp_f1_2;
                    temp_f30 = ((0.6f * temp_f28) + (0.4f * temp_f1)) / (temp_f1 + temp_f28);
                    temp_r31->pos.x = (f32) (temp_r31->pos.x + (temp_f30 * (temp_f31 *  sp20.x)));
                    temp_r31->pos.y = (f32) (temp_r31->pos.y + (temp_f30 * (temp_f31 * sp20.y)));
                    temp_r31->pos.z = (f32) (temp_r31->pos.z + (temp_f30 * (temp_f31 * sp20.z)));
                    temp_f30_2 = ((0.6f * temp_f1) + (0.4f * temp_f1)) / (temp_f1 + temp_f28);
                    if ((s32) temp_r30->unk68 == 0) {
                        temp_r30->unk8 = (f32) (temp_r30->unk8 + (temp_f30_2 * (temp_f31 * - sp20.x)));
                        temp_r30->unkC = (f32) (temp_r30->unkC + (temp_f30_2 * (temp_f31 * -sp20.y)));
                        temp_r30->unk30 = (f32) (temp_r30->unk30 + (temp_f30_2 * (temp_f31 * -sp20.z)));
                    }
                    fn_1_29D8(lbl_1_bss_9AC[var_r29]);
                }
            }
            var_r29 += 1;
            goto loop_8;
        }
    }
}
static void fn_1_2CA8(void) {
    s8 spB;
    s8 spA;
    s8 sp9;
    s8 sp8;

    sp8 = 0;
    sp9 = 0;
    spA = 0x90;
    spB = 0xC0;
    CZoom += (f32) (*HuPadTrigL / 5);
    CZoom -= (f32) (*HuPadTrigR / 5);
    Center.x += 10.0f * ((f32) (s8) (u8) HuPadSubStkX[0] / 59.0f);
    Center.z -= 10.0f * ((f32) (s8) (u8) HuPadSubStkY[0] / 59.0f);
    CRot.x += 5.0f * ((f32) (s8) (u8) HuPadSubStkY[1] / 59.0f);
    CRot.y -= 5.0f * ((f32) (s8) (u8) HuPadSubStkX[1] / 59.0f);
    printWin(0x10, 0x28, 0x180, 0x24, (GXColor *) &sp8);
    fontcolor = 0xE;
    print8(0x10, 0x28, 1.5f, "Center X:%.02f Y:%.02f Z:%.02f", &Center, Center.x, Center.y, Center.z);
    print8(0x10, 0x34, 1.5f, "CRot X:%.02f Y:%.02f Z:%.02f", &CRot, CRot.x, CRot.y, CRot.z);
    print8(0x10, 0x40, 1.5f, "CZoom:%.02f", &CZoom, CZoom);
    if ((s32) (*HuPadBtnDown & 0x400) != 0) {
        omCameraViewSet(&lbl_1_data_50);
    }
}
static void fn_1_3068(OMOBJ *obj) {
    s16 temp_r3;
    s32 var_r29;
    M559NOD *temp_r31;

    temp_r31 = obj->data;
    *obj->mdlId = Hu3DModelCreate(HuDataSelHeapReadNum(0x72000A, 0x10000000, HEAP_MODEL));
    var_r29 = 0;
loop_2:
    if (var_r29 < 5) {
        obj->mtnId[var_r29] = Hu3DJointMotion(*obj->mdlId, HuDataSelHeapReadNum(lbl_1_data_3C[var_r29], 0x10000000, HEAP_MODEL));
        var_r29 += 1;
        goto loop_2;
    }
    temp_r31->count = 5;
    fn_1_3854(obj, 0, 0x40000001U, 0.0f);
    temp_r31->state = (s16) *obj->mdlId;
    temp_r31->pos.x = (f32) lbl_1_data_CC.x;
    temp_r31->pos.y = (f32) lbl_1_data_CC.y;
    temp_r31->pos.z = (f32) lbl_1_data_CC.z;
    temp_r31->field10 = 0.0f;
    temp_r31->rotY = -90.0f;
    temp_r31->field18 = 0.0f;
    temp_r31->mode = 0;
    temp_r31->timer = 0;
    temp_r31->arg = -1;
    Hu3DModelShadowSet(*obj->mdlId);
    Hu3DModelLayerSet(*obj->mdlId, 4);
    temp_r3 = fn_1_7368(lbl_1_bss_0, 0, 0, 0x200, 0x40, 0x40, 40.0f);
    *((M559HOOKFUNC *) Hu3DData[temp_r3].hookData) = fn_1_6678;
    ((M559HOOKDATA *) Hu3DData[temp_r3].hookData)->unk6A = 0;
    Hu3DModelLayerSet(temp_r3, 4);
    temp_r31->arg = (s32) temp_r3;
    omSetTra(obj, temp_r31->pos.x, temp_r31->pos.y, temp_r31->pos.z);
    omSetRot(obj, temp_r31->field10, temp_r31->rotY, temp_r31->field18);
    omSetSca(obj, 1.5f, 1.5f, 1.5f);
    obj->objFunc = fn_1_32D0;
}
void fn_1_32D0(OMOBJ *obj) {
    f32 sp30;
    f32 sp2C;
    f32 sp28;
    f32 sp24;
    f32 sp20;
    f32 sp1C;
    Point3d sp10;
    u8 spF;
    u8 spE;
    u8 spD;
    u8 spC;
    s16 spA;
    s16 sp8;
    Mtx mtxHook;
    Mtx mtxHead;
    HU3DMODEL *temp_r24;
    f32 temp_f31;
    s16 var_r22;
    s16 var_r25;
    s32 temp_r0;
    s32 temp_r0_2;
    s32 var_r20;
    s32 var_r23;
    M559HOOKDATA *temp_r26;
    M559HOOKDATA *temp_r29;
    M559HOOK *temp_r31;
    M559CLR *temp_r3;
    M559ANIMPART *var_r21;
    M559ANIMPART *var_r28;

    var_r23 = 0;
    temp_r31 = obj->data;
    temp_r0 = temp_r31->mode;
    switch (temp_r0) {                              /* irregular */
    case 2:
    case 0:
        break;
    case 1:
        fn_1_3854(obj, 2, 0U, 8.0f);
        if ((s32) temp_r31->timer > 0x64) {
            temp_r31->timer = 0;
            fn_1_3854(obj, 0, 0x40000001U, 8.0f);
            temp_r31->mode = 0;
        } else {
            if (((s32) temp_r31->timer >= 0x1E) && ((s32) temp_r31->timer < 0x5A)) {
                Hu3DModelAttrSet(*obj->mdlId, 0x40000002U);
                var_r23 = 1;
                if ((s32) temp_r31->timer == 0x1E) {
                    lbl_1_bss_8 = HuAudFXPlayPan(0x97B, fn_1_55D8((HuVecF *) &temp_r31->offset));
                }
            } else if ((s32) temp_r31->timer == 0x5A) {
                Hu3DModelAttrReset(*obj->mdlId, 0x40000002U);
                var_r23 = 0;
                HuAudFXStop(lbl_1_bss_8);
            }
            temp_r31->timer = (s32) (temp_r31->timer + 1);
        }
        break;
    case 3:
        temp_r31->offset.x = (f32) lbl_1_data_D8.x;
        temp_r31->offset.y = (f32) lbl_1_data_D8.y;
        temp_r31->offset.z = (f32) lbl_1_data_D8.z;
        temp_r31->playerNo = 0;
        temp_r31->mode = 0;
        break;
    }
    if ((s32) temp_r31->modelId >= 0) {
        if (var_r23 != 0) {
            Hu3DModelObjMtxGet(*obj->mdlId, "g000m0-itemhook_M", mtxHook);
            sp28 = mtxHook[2][0];
            sp2C = mtxHook[3][0];
            sp30 = mtxHook[3][1];
            spA = *obj->mdlId;
            Hu3DModelObjMtxGet(spA, "g000m0-ns_hed", mtxHead);
            sp1C = mtxHead[2][0];
            sp20 = mtxHead[3][0];
            sp24 = mtxHead[3][1];
            sp20 += 100.0f;
            PSVECSubtract((Vec *) &sp28, (Vec *) &sp1C, &sp10);
            temp_f31 = (f32) (180.0 * (atan2((f64) sp10.x, (f64) sp10.z) / 3.141592653589793));
            sp8 = temp_r31->modelId;
            spC = lbl_1_data_18[0];
            spD = lbl_1_data_18[1];
            spE = lbl_1_data_18[2];
            spF = lbl_1_data_18[3];
            var_r22 = 0;
loop_31:
            if (var_r22 < 1) {
                temp_r26 = Hu3DData[sp8].hookData;
                if ((u8) temp_r26->unk6B != 0) {
                    var_r21 = NULL;
                } else {
                    var_r25 = 0;
                    var_r28 = temp_r26->unk10;
loop_25:
                    if (var_r25 < (s16) temp_r26->unk64) {
                        if ((s16) var_r28->unk3E >= 0) {
                            var_r25 += 1;
                            var_r28 += 0x40;
                            goto loop_25;
                        }
                    }
                    if (var_r25 == (s16) temp_r26->unk64) {
                        var_r21 = NULL;
                    } else {
                        temp_r3 = temp_r26->unk20 + (var_r25 * 4);
                        temp_r3->unk0 = spC;
                        temp_r3->unk1 = spD;
                        temp_r3->unk2 = spE;
                        temp_r3->unk3 = spF;
                        var_r28->unk30 = sp28;
                        var_r28->unk34 = sp2C;
                        var_r28->unk38 = sp30;
                        var_r28->unk2C = 0.0f;
                        var_r28->unk28 = 0.0f;
                        var_r28->unk3C = 0;
                        var_r28->unk0 = 0;
                        var_r28->unk3E = var_r25;
                        var_r21 = var_r28;
                    }
                }
                if (var_r21 != NULL) {
                    var_r21->unk4 = sp28;
                    var_r21->unk8 = sp2C;
                    var_r21->unkC = sp30;
                    var_r21->unk10 = 0.0f;
                    var_r21->unk14 = temp_f31;
                    var_r21->unk1C = 10.0f;
                    var_r21->unk20 = 250.0f;
                    var_r21->unk24 = 0.0f;
                    var_r21->unk18 = (f32) ((20.0f * frandf()) - 10.0f);
                    var_r21->unk1C = (f32) (var_r21->unk1C * (0.8f + (0.4f * frandf())));
                    var_r21->unk20 = (f32) (var_r21->unk20 * (0.9f + (0.3f * frandf())));
                    var_r22 += 1;
                    goto loop_31;
                }
            }
        }
        temp_r0_2 = temp_r31->modelId;
        temp_r24 = &Hu3DData[temp_r31->modelId];
        temp_r29 = temp_r24->hookData;
        if (((u8) temp_r29->unk6B != 0) && ((s16) temp_r29->arg66 == 0)) {
            HuMemDirectFree(temp_r29->unk10);
            HuMemDirectFree(temp_r29->unk14);
            HuMemDirectFree(temp_r29->unk10);
            HuMemDirectFree(temp_r29->unk18);
            HuMemDirectFree(temp_r29->unk1C);
            HuMemDirectFree(temp_r29->unk14);
            HuSprAnimKill(temp_r29->unk4);
            HuMemDirectFree(temp_r24->hookData);
            Hu3DModelKill((s16) temp_r0_2);
            var_r20 = 1;
        } else {
            var_r20 = 0;
        }
        if (var_r20 != 0) {
            temp_r31->modelId = -1;
        }
    }
    omSetTra(obj, temp_r31->offset.x, temp_r31->offset.y, temp_r31->offset.z);
    omSetRot(obj, temp_r31->field10, temp_r31->playerNo, temp_r31->charNo);
}
static void fn_1_3854(OMOBJ *obj, s16 motion, u32 attr, f32 endTime) {
    u32 spC;
    f32 sp8;
    M559NOD *temp_r31;

    sp8 = endTime;
    spC = attr;
    temp_r31 = obj->data;
    if ((s32) temp_r31->count != motion) {
        temp_r31->count = motion;
        Hu3DMotionShiftSet(*obj->mdlId, *(obj->mtnId + (motion * 2)), 0.0f, sp8, spC);
        if ((s32) temp_r31->count == 3) {
            HuAudFXPlayPan(0x2C9, fn_1_55D8((HuVecF *) &temp_r31->pos));
        }
        if ((s32) temp_r31->count == 4) {
            HuAudFXPlayPan(0x2CA, fn_1_55D8((HuVecF *) &temp_r31->pos));
        }
    }
}
static s32 fn_1_3918(s32 no, f32 a, f32 b) {
    s32 var_r3;

    var_r3 = no;
    switch (var_r3) {                               /* irregular */
    case 0:
        if ((a < 0.0f) && (b < 0.0f)) {
            return 1;
        }
        return 0;
    case 1:
        if ((a > 0.0f) && (b < 0.0f)) {
            return 1;
        }
        return 0;
    case 2:
        if ((a < 0.0f) && (b > 0.0f)) {
            return 1;
        }
        return 0;
    case 3:
        if ((a > 0.0f) && (b > 0.0f)) {
            return 1;
        }
        var_r3 = 0;
    default:
        return var_r3;
    }
}
static void fn_1_3A1C(void) {
    f32 sp1C;
    f32 sp18;
    f32 sp14;
    f32 sp10;
    f32 spC;
    f32 sp8;
    f32 temp_f0;
    f32 temp_f0_2;
    s32 var_r27;
    s32 var_r28;
    s32 var_r29;
    s32 var_r30;
    s32 var_r31;
    s32 var_r31_2;

    var_r29 = 0;
    sp8 = 0.0f;
    spC = 0.0f;
    sp10 = 0.0f;
    var_r30 = 0;
    var_r28 = 0;
loop_30:
    if (var_r28 < 4) {
        var_r31 = 0;
loop_28:
        if (var_r31 < 0x18) {
            do {
loop_2:
                temp_f0 = (f32) frandmod(0x514U) - 635.0f;
                sp14 = temp_f0;
                sp18 = 800.0f;
                temp_f0_2 = (f32) frandmod(0x514U) - 635.0f;
                sp1C = temp_f0_2;
                if ((((sp8 - temp_f0) * (sp8 - temp_f0)) + ((sp10 - temp_f0_2) * (sp10 - temp_f0_2))) < 403225.0f) {
                    var_r27 = 1;
                } else {
                    var_r27 = 0;
                }
                if (var_r27 == 0) {
                    goto loop_2;
                }
                switch (var_r28) {                  /* irregular */
                case 0:
                    if ((sp14 < 0.0f) && (sp1C < 0.0f)) {
                        var_r29 = 1;
                    } else {
                        var_r29 = 0;
                    }
                    break;
                case 1:
                    if ((sp14 > 0.0f) && (sp1C < 0.0f)) {
                        var_r29 = 1;
                    } else {
                        var_r29 = 0;
                    }
                    break;
                case 2:
                    if ((sp14 < 0.0f) && (sp1C > 0.0f)) {
                        var_r29 = 1;
                    } else {
                        var_r29 = 0;
                    }
                    break;
                case 3:
                    if ((sp14 > 0.0f) && (sp1C > 0.0f)) {
                        var_r29 = 1;
                    } else {
                        var_r29 = 0;
                    }
                    break;
                }
            } while (var_r29 == 0);
            lbl_1_bss_288[var_r30].x = sp14;
            lbl_1_bss_288[var_r30].y = 800.0f;
            lbl_1_bss_288[var_r30].z = sp1C;
            var_r30 += 1;
            var_r31 += 1;
            goto loop_28;
        }
        var_r28 += 1;
        goto loop_30;
    }
    var_r31_2 = 0;
loop_33:
    if (var_r31_2 < 4) {
        lbl_1_bss_288[var_r30].x = lbl_1_data_9C[var_r31_2].x;
        lbl_1_bss_288[var_r30].y = lbl_1_data_9C[var_r31_2].y;
        lbl_1_bss_288[var_r30].z = lbl_1_data_9C[var_r31_2].z;
        var_r30 += 1;
        var_r31_2 += 1;
        goto loop_33;
    }
}
static void fn_1_3DA8(void) {
    f32 sp1C;
    f32 sp18;
    f32 sp14;
    f32 sp10;
    f32 spC;
    f32 sp8;
    f32 temp_f0;
    f32 temp_f0_2;
    s32 var_r27;
    s32 var_r28;
    s32 var_r29;
    s32 var_r30;
    s32 var_r31;

    var_r31 = 0;
    sp8 = 0.0f;
    spC = 0.0f;
    sp10 = 0.0f;
    var_r29 = 0;
    var_r30 = 0;
loop_30:
    if (var_r30 < 4) {
        var_r28 = 0;
loop_28:
        if (var_r28 < 0xC) {
            do {
loop_2:
                temp_f0 = (f32) frandmod(0x514U) - 635.0f;
                sp14 = temp_f0;
                sp18 = 800.0f;
                temp_f0_2 = (f32) frandmod(0x514U) - 635.0f;
                sp1C = temp_f0_2;
                if ((((sp8 - temp_f0) * (sp8 - temp_f0)) + ((sp10 - temp_f0_2) * (sp10 - temp_f0_2))) < 403225.0f) {
                    var_r27 = 1;
                } else {
                    var_r27 = 0;
                }
                if (var_r27 == 0) {
                    goto loop_2;
                }
                switch (var_r30) {                  /* irregular */
                case 0:
                    if ((sp14 < 0.0f) && (sp1C < 0.0f)) {
                        var_r31 = 1;
                    } else {
                        var_r31 = 0;
                    }
                    break;
                case 1:
                    if ((sp14 > 0.0f) && (sp1C < 0.0f)) {
                        var_r31 = 1;
                    } else {
                        var_r31 = 0;
                    }
                    break;
                case 2:
                    if ((sp14 < 0.0f) && (sp1C > 0.0f)) {
                        var_r31 = 1;
                    } else {
                        var_r31 = 0;
                    }
                    break;
                case 3:
                    if ((sp14 > 0.0f) && (sp1C > 0.0f)) {
                        var_r31 = 1;
                    } else {
                        var_r31 = 0;
                    }
                    break;
                }
            } while (var_r31 == 0);
            lbl_1_bss_48[var_r29].x = sp14;
            lbl_1_bss_48[var_r29].y = 800.0f;
            lbl_1_bss_48[var_r29].z = sp1C;
            var_r29 += 1;
            var_r28 += 1;
            goto loop_28;
        }
        var_r30 += 1;
        goto loop_30;
    }
}
static void fn_1_40A4(void) {
    s32 var_r31;

    var_r31 = 0;
loop_2:
    if (var_r31 < 0x64) {
        fn_1_41D4(lbl_1_bss_288[var_r31].x, 800.0f, lbl_1_bss_288[var_r31].z);
        var_r31 += 1;
        goto loop_2;
    }
}
static void fn_1_413C(void) {
    s32 var_r31;

    var_r31 = 0;
loop_2:
    if (var_r31 < 0x30) {
        fn_1_4910(lbl_1_bss_48[var_r31].x, 800.0f, lbl_1_bss_48[var_r31].z);
        var_r31 += 1;
        goto loop_2;
    }
}
static void fn_1_41D4(f32 a, f32 b, f32 c) {
    f32 sp10;
    f32 spC;
    f32 sp8;
    OMOBJ *temp_r30;
    M559HEAD *temp_r31;

    sp8 = a;
    spC = c;
    sp10 = c;
    if ((s32) lbl_1_bss_73C <= 0x63) {
        temp_r30 = omAddObjEx(lbl_1_bss_BB4, 0x65, 1U, 0U, 1, fn_1_4364);
        temp_r30->data = HuMemDirectMallocNum(HEAP_HEAP, 0x58, 0x10000000U);
        temp_r31 = temp_r30->data;
        temp_r31->unk4 = sp8;
        temp_r31->unk10 = sp8;
        temp_r31->unk8 = spC;
        temp_r31->unk14 = spC;
        temp_r31->unkC = sp10;
        temp_r31->unk18 = sp10;
        temp_r31->field1C.i = 0.0f;
        temp_r31->field20.i = 0.0f;
        temp_r31->field24.i = 0.0f;
        temp_r31->field30.i = 1.0f;
        temp_r31->field2C.i = 1.0f;
        temp_r31->field28.i = 1.0f;
        temp_r31->field40.i = 0;
        temp_r31->field48.i = (s32) (frandmod(0x2DU) + 0xF);
        temp_r31->field44.i = 0;
        temp_r31->field4C.i = (s32) (frandmod(0x14U) + 0x50);
        temp_r31->field38.i = (f32) (temp_r31->unk8 / (f32) temp_r31->field4C.i);
        lbl_1_bss_73C += 1;
    }
}
static void fn_1_4364(OMOBJ *obj) {
    s32 var_r30;
    M559HEAD *temp_r31;

    temp_r31 = obj->data;
    temp_r31->state = Hu3DModelLink(lbl_1_bss_742);
    temp_r31->modelId = Hu3DModelLink(lbl_1_bss_740);
    temp_r31->field50.f = 0.0f;
    Hu3DModelAttrSet(temp_r31->state, 0x40000001U);
    Hu3DModelLayerSet(temp_r31->state, 5);
    Hu3DModelLayerSet(temp_r31->modelId, 2);
    Hu3DModelPosSet(temp_r31->state, temp_r31->pos.x, temp_r31->pos.y, temp_r31->pos.z);
    Hu3DModelPosSet(temp_r31->modelId, temp_r31->pos.x, 0.2f, temp_r31->pos.z);
    Hu3DModelScaleSet(temp_r31->modelId, 0.0f, 0.0f, 0.0f);
    Hu3DMotionTimeSet(temp_r31->state, (f32) frandmod((u32) Hu3DMotionMaxTimeGet(temp_r31->state)));
    var_r30 = 0;
loop_4:
    if (var_r30 < 0x64) {
        if ((OMOBJ *) lbl_1_bss_814[var_r30] == NULL) {
            lbl_1_bss_814[var_r30] = obj;
        } else {
            var_r30 += 1;
            goto loop_4;
        }
    }
    Hu3DModelAttrSet(temp_r31->state, 1U);
    Hu3DModelAttrSet(temp_r31->modelId, 1U);
    obj->objFunc = fn_1_4504;
}
static void fn_1_4504(OMOBJ *obj) {
    s16 temp_r26;
    s32 temp_r0;
    s32 var_r28;
    s32 var_r29;
    M559HOOK *temp_r30;
    M559HEAD *temp_r31;

    temp_r31 = obj->data;
    temp_r0 = temp_r31->field40.i;
    switch (temp_r0) {                              /* irregular */
    case 0:
        break;
    case 1:
        Hu3DModelAttrReset(temp_r31->state, 1U);
        Hu3DModelAttrReset(temp_r31->modelId, 1U);
        temp_r31->field44.i = (s32) (temp_r31->field44.i + 1);
        if ((s32) temp_r31->field44.i > (s32) temp_r31->field48.i) {
            temp_r31->field44.i = 0;
            temp_r31->field40.i = 2;
        }
        break;
    case 2:
        temp_r31->pos.y = (f32) (temp_r31->pos.y - temp_r31->field38.f);
        if (temp_r31->pos.y < 0.0f) {
            temp_r31->pos.y = 0.0f;
            temp_r31->field40.i = 3;
            temp_r31->field44.i = 0;
            if ((s32) lbl_1_bss_3C == 0) {
                lbl_1_bss_3C = 1;
                HuAudFXPlay(0x982);
            }
        } else {
            temp_r31->field44.i = (s32) (temp_r31->field44.i + 1);
        }
        break;
    case 3:
        temp_r31->field44.i = (s32) (temp_r31->field44.i + 1);
        if ((s32) temp_r31->field44.i > 0x3C) {
            temp_r31->field44.i = 0;
            temp_r31->field40.i = 4;
        }
        if ((s32) (temp_r31->field44.i & 1) != 0) {
            Hu3DModelAttrSet(temp_r31->state, 1U);
            Hu3DModelAttrSet(temp_r31->modelId, 1U);
        } else {
            Hu3DModelAttrReset(temp_r31->state, 1U);
            Hu3DModelAttrReset(temp_r31->modelId, 1U);
        }
        break;
    case 4:
        fn_1_4848(obj);
        break;
    }
    if (temp_r31->pos.y < 150.0f) {
        var_r29 = 0;
loop_29:
        if (var_r29 < 4) {
            temp_r30 = lbl_1_bss_9AC[var_r29]->data;
            if ((((temp_r30->unk8 - temp_r31->pos.x) * (temp_r30->unk8 - temp_r31->pos.x)) + ((temp_r30->unk30 - temp_r31->pos.z) * (temp_r30->unk30 - temp_r31->pos.z))) < 3025.0f) {
                var_r28 = 1;
            } else {
                var_r28 = 0;
            }
            if ((var_r28 != 0) && ((s32) temp_r30->unk68 == 0)) {
                temp_r30->unk20 = 3;
                temp_r30->unk68 = 1;
                Hu3DModelAttrReset(temp_r30->unk12, 1U);
                Hu3DModelAttrReset(temp_r30->unk12, 0x40000002U);
                omVibrate((s16) temp_r30->unk14, 0x1E, 0xC, 0);
                temp_r26 = fn_1_55D8((HuVecF *) &temp_r30->pos);
                HuAudFXPlayVolPan(0x983, fn_1_5654((HuVecF *) &temp_r30->pos), temp_r26);
            }
            var_r29 += 1;
            goto loop_29;
        }
    }
    Hu3DModelPosSet(temp_r31->state, temp_r31->pos.x, temp_r31->pos.y, temp_r31->pos.z);
    temp_r31->field50.f = (f32) (0.3f + (1.0f - (temp_r31->pos.y / temp_r31->start.y)));
    Hu3DModelScaleSet(temp_r31->modelId, temp_r31->field50.f, 1.0f, temp_r31->field50.f);
    Hu3DModelPosSet(temp_r31->modelId, temp_r31->pos.x, 0.2f, temp_r31->pos.z);
}
static void fn_1_4848(OMOBJ *obj) {
    s32 var_r31;
    M559HOOK *temp_r30;

    temp_r30 = obj->data;
    Hu3DModelKill(temp_r30->state);
    Hu3DModelKill(temp_r30->pad2);
    var_r31 = 0;
loop_4:
    if (var_r31 < 0x64) {
        if (obj == (OMOBJ *) lbl_1_bss_814[var_r31]) {
            lbl_1_bss_814[var_r31] = NULL;
        } else {
            var_r31 += 1;
            goto loop_4;
        }
    }
    lbl_1_bss_73C -= 1;
    omDelObjEx(lbl_1_bss_BB4, obj);
}
static void fn_1_4910(f32 a, f32 b, f32 c) {
    f32 sp10;
    f32 spC;
    f32 sp8;
    OMOBJ *temp_r30;
    M559HEAD *temp_r31;

    sp8 = a;
    spC = c;
    sp10 = c;
    if ((s32) lbl_1_bss_738 <= 0x2F) {
        temp_r30 = omAddObjEx(lbl_1_bss_BB4, 0x66, 1U, 0U, 2, fn_1_4A9C);
        temp_r30->data = HuMemDirectMallocNum(HEAP_HEAP, 0x58, 0x10000000U);
        temp_r31 = temp_r30->data;
        temp_r31->pos.x = sp8;
        temp_r31->start.x = sp8;
        temp_r31->pos.y = spC;
        temp_r31->start.y = spC;
        temp_r31->pos.z = sp10;
        temp_r31->unk18 = sp10;
        temp_r31->field1C.i = 0.0f;
        temp_r31->field20.f = 0.0f;
        temp_r31->field24.i = 0.0f;
        temp_r31->field30.f = 1.0f;
        temp_r31->field2C.i = 1.0f;
        temp_r31->field28.f = 1.0f;
        temp_r31->field40.i = 0;
        temp_r31->field48.i = frandmod(0x2DU);
        temp_r31->field44.i = 0;
        temp_r31->field4C.i = (s32) (frandmod(0x14U) + 0x3C);
        temp_r31->field38.f = (f32) (temp_r31->pos.y / (f32) temp_r31->field4C.i);
        lbl_1_bss_738 += 1;
    }
}
static void fn_1_4A9C(OMOBJ *obj) {
    s32 var_r30;
    M559HEAD *temp_r31;

    temp_r31 = obj->data;
    temp_r31->state = Hu3DModelLink(lbl_1_bss_742);
    temp_r31->modelId = Hu3DModelLink(lbl_1_bss_740);
    temp_r31->field50.f = 0.0f;
    Hu3DModelAttrSet(temp_r31->state, 0x40000001U);
    Hu3DModelLayerSet(temp_r31->state, 5);
    Hu3DModelLayerSet(temp_r31->modelId, 2);
    Hu3DModelPosSet(temp_r31->state, temp_r31->pos.x, temp_r31->pos.y, temp_r31->pos.z);
    Hu3DModelPosSet(temp_r31->modelId, temp_r31->pos.x, 0.2f, temp_r31->pos.z);
    Hu3DModelScaleSet(temp_r31->modelId, 0.0f, 0.0f, 0.0f);
    Hu3DMotionTimeSet(temp_r31->state, (f32) frandmod((u32) Hu3DMotionMaxTimeGet(temp_r31->state)));
    var_r30 = 0;
loop_4:
    if (var_r30 < 0x30) {
        if ((OMOBJ *) lbl_1_bss_754[var_r30] == NULL) {
            lbl_1_bss_754[var_r30] = obj;
        } else {
            var_r30 += 1;
            goto loop_4;
        }
    }
    Hu3DModelAttrSet(temp_r31->state, 1U);
    Hu3DModelAttrSet(temp_r31->modelId, 1U);
    obj->objFunc = fn_1_4C3C;
}
static void fn_1_4C3C(OMOBJ *obj) {
    s16 temp_r26;
    s32 temp_r0;
    s32 var_r28;
    s32 var_r29;
    M559HOOK *temp_r30;
    M559HEAD *temp_r31;

    temp_r31 = obj->data;
    temp_r0 = temp_r31->field40.i;
    switch (temp_r0) {                              /* irregular */
    case 0:
        break;
    case 1:
        Hu3DModelAttrReset(temp_r31->state, 1U);
        Hu3DModelAttrReset(temp_r31->modelId, 1U);
        temp_r31->field44.i = (s32) (temp_r31->field44.i + 1);
        if ((s32) temp_r31->field44.i > (s32) temp_r31->field48.i) {
            temp_r31->field44.i = 0;
            temp_r31->field40.i = 2;
        }
        break;
    case 2:
        temp_r31->pos.y = (f32) (temp_r31->pos.y - temp_r31->field38.f);
        if (temp_r31->pos.y < 0.0f) {
            temp_r31->pos.y = 0.0f;
            temp_r31->field40.i = 3;
            temp_r31->field44.i = 0;
            if ((s32) lbl_1_bss_38 == 0) {
                lbl_1_bss_38 = 1;
                HuAudFXPlay(0x982);
            }
        } else {
            temp_r31->field44.i = (s32) (temp_r31->field44.i + 1);
        }
        break;
    case 3:
        temp_r31->field44.i = (s32) (temp_r31->field44.i + 1);
        if ((s32) temp_r31->field44.i > 0x3C) {
            temp_r31->field44.i = 0;
            temp_r31->field40.i = 4;
        }
        if ((s32) (temp_r31->field44.i & 1) != 0) {
            Hu3DModelAttrSet(temp_r31->state, 1U);
            Hu3DModelAttrSet(temp_r31->modelId, 1U);
        } else {
            Hu3DModelAttrReset(temp_r31->state, 1U);
            Hu3DModelAttrReset(temp_r31->modelId, 1U);
        }
        break;
    case 4:
        fn_1_4F70(obj);
        break;
    }
    if (temp_r31->pos.y < 150.0f) {
        var_r29 = 0;
loop_29:
        if (var_r29 < 4) {
            temp_r30 = lbl_1_bss_9AC[var_r29]->data;
            if ((((temp_r30->unk8 - temp_r31->pos.x) * (temp_r30->unk8 - temp_r31->pos.x)) + ((temp_r30->unk30 - temp_r31->pos.z) * (temp_r30->unk30 - temp_r31->pos.z))) < 3025.0f) {
                var_r28 = 1;
            } else {
                var_r28 = 0;
            }
            if ((var_r28 != 0) && ((s32) temp_r30->unk68 == 0)) {
                temp_r30->unk20 = 3;
                temp_r30->unk68 = 1;
                Hu3DModelAttrReset(temp_r30->unk12, 1U);
                Hu3DModelAttrReset(temp_r30->unk12, 0x40000002U);
                omVibrate((s16) temp_r30->unk14, 0x1E, 0xC, 0);
                temp_r26 = fn_1_55D8((HuVecF *) &temp_r30->pos);
                HuAudFXPlayVolPan(0x983, fn_1_5654((HuVecF *) &temp_r30->pos), temp_r26);
            }
            var_r29 += 1;
            goto loop_29;
        }
    }
    Hu3DModelPosSet(temp_r31->state, temp_r31->pos.x, temp_r31->pos.y, temp_r31->pos.z);
    temp_r31->field50.f = (f32) (1.3f - (temp_r31->pos.y / temp_r31->start.y));
    Hu3DModelScaleSet(temp_r31->modelId, temp_r31->field50.f, 1.0f, temp_r31->field50.f);
    Hu3DModelPosSet(temp_r31->modelId, temp_r31->pos.x, 0.2f, temp_r31->pos.z);
}
static void fn_1_4F70(OMOBJ *obj) {
    s32 var_r31;
    M559HOOK *temp_r30;

    temp_r30 = obj->data;
    Hu3DModelKill(temp_r30->state);
    Hu3DModelKill(temp_r30->pad2);
    var_r31 = 0;
loop_4:
    if (var_r31 < 0x30) {
        if (obj == (OMOBJ *) lbl_1_bss_754[var_r31]) {
            lbl_1_bss_754[var_r31] = NULL;
        } else {
            var_r31 += 1;
            goto loop_4;
        }
    }
    lbl_1_bss_738 -= 1;
    omDelObjEx(lbl_1_bss_BB4, obj);
}
static void fn_1_5038(OMOBJ *obj) {
    HSFATTRIBUTE *temp_r24;
    HSFDATA *temp_r29;
    HSFMATERIAL *var_r31;
    HU3DMODEL *temp_r25;
    s32 var_r23;
    s32 var_r30;
    s32 var_r30_2;
    char **temp_r27;
    u32 var_r26;
    M559HOOK *temp_r28;

    temp_r28 = obj->data;
    temp_r25 = &Hu3DData[*obj->mdlId];
    temp_r29 = temp_r25->hsf;
    var_r31 = temp_r29->material;
    if ((s32) temp_r28->charNo == 3) {
        var_r30 = 0;
loop_6:
        if (var_r30 < (s16) temp_r25->hsf->materialNum) {
            if ((var_r30 != 1) && (var_r30 != 2)) {
                var_r31->color[0] = (u8) (s32) ((f32) var_r31->color[0] * 0.9f);
                var_r31->color[1] = (u8) (s32) ((f32) var_r31->color[1] * 0.9f);
                var_r31->color[2] = (u8) (s32) ((f32) var_r31->color[2] * 0.9f);
            }
            var_r30 += 1;
            var_r31 += 0x3C;
            goto loop_6;
        }
        return;
    }
    temp_r27 = CharModelEyeBmpGet((s16) temp_r28->charNo, 2);
    var_r30_2 = 0;
loop_18:
    if (var_r30_2 < (s16) temp_r29->materialNum) {
        var_r23 = 1;
        var_r26 = 0;
loop_14:
        if (var_r26 < (u32) var_r31->attrNum) {
            temp_r24 = &temp_r29->attribute[var_r31->attr[var_r26]];
            if ((strcmp(temp_r27[0], temp_r24->bitmap->name) == 0) || (strcmp(temp_r27[1], temp_r24->bitmap->name) == 0)) {
                var_r23 = 0;
            }
            var_r26 += 1;
            goto loop_14;
        }
        if (var_r23 != 0) {
            var_r31->color[0] = (u8) (s32) ((f32) var_r31->color[0] * 0.9f);
            var_r31->color[1] = (u8) (s32) ((f32) var_r31->color[1] * 0.9f);
            var_r31->color[2] = (u8) (s32) ((f32) var_r31->color[2] * 0.9f);
        }
        var_r30_2 += 1;
        var_r31 += 0x3C;
        goto loop_18;
    }
}
static f32 fn_1_5308(s32 a, s32 b, s32 c) {
    s32 sp8;
    f32 var_f31;

    sp8 = c;
    if (a == b) {
        return 0.0f;
    }
    var_f31 = (f32) fmod((f64) (b - a), 360.0);
    if (var_f31 < 0.0f) {
        var_f31 += 360.0f;
    }
    if (var_f31 > 180.0f) {
        var_f31 -= 360.0f;
    }
    return var_f31 / (f32) sp8;
}
static s32 fn_1_542C(void) {
    s32 var_r31;
    M559HOOK *sp8[4];

    var_r31 = 0;
loop_2:
    if (var_r31 < 4) {
        sp8[var_r31] = lbl_1_bss_9AC[var_r31]->data;
        var_r31 += 1;
        goto loop_2;
    }
    if (((s32) sp8[0]->unk68 == 0) && ((s32) sp8[1]->unk68 == 0) && ((s32) sp8[2]->unk68 == 0) && ((s32) sp8[3]->unk68 == 0)) {
        return 1;
    }
    return 0;
}
static void fn_1_54C4(void) {
    s32 var_r31;

    var_r31 = 0;
loop_2:
    if (var_r31 < 0x64) {
        M559DATA(lbl_1_bss_814[var_r31])->unk40 = 1;
        var_r31 += 1;
        goto loop_2;
    }
}
static void fn_1_5514(void) {
    s32 var_r31;

    var_r31 = 0;
loop_2:
    if (var_r31 < 0x30) {
        M559DATA(lbl_1_bss_754[var_r31])->unk40 = 1;
        var_r31 += 1;
        goto loop_2;
    }
}
static void fn_1_5564(s32 *flag) {
    lbl_1_bss_44 = -1;
}
static void fn_1_5578(void) {
    lbl_1_bss_44 = HuAudSStreamPlay(0x3F);
}
static void fn_1_55A8(void) {
    HuAudSStreamFadeOut(lbl_1_bss_44, 0x64);
}
static s16 fn_1_55D8(HuVecF *pos) {
    HuVecF spC;
    HuVecF *sp8;

    sp8 = pos;
    Hu3D3Dto2D(pos, 1, &spC);
    return (s16) (32.0f + (64.0f * ( spC.x / 576.0f)));
}
static s16 fn_1_5654(HuVecF *pos) {
    HuVecF spC;
    HuVecF *sp8;

    sp8 = pos;
    Hu3D3Dto2D(pos, 1, &spC);
    return (s16) (64.0f + (63.0f * (spC.y / 480.0f)));
}
static void fn_1_56D0(void) {
    lbl_1_bss_BB8.prev = 0;
    lbl_1_bss_BB8.next = 0;
}
static void fn_1_56F4(u16 a, s16 b) {
    s16 spA;
    u16 sp8;
    M559LISTNODE *temp_r29;
    M559LISTNODE *temp_r3;

    sp8 = a;
    spA = b;
    temp_r3 = HuMemDirectMallocNum(HEAP_HEAP, 0xC, 0x10000000U);
    temp_r3->id = sp8;
    temp_r3->value = spA;
    temp_r29 = lbl_1_bss_BB8.next;
    lbl_1_bss_BB8.next = temp_r3;
    if (temp_r29 != NULL) {
        temp_r29->prev = temp_r3;
    }
    temp_r3->prev = (M559LISTNODE *) &lbl_1_bss_BB8;
    temp_r3->next = temp_r29;
}
static s16 fn_1_577C(s16 no) {
    s16 sp8;
    s16 temp_r28;
    M559LISTNODE *temp_r29;
    M559LISTNODE *temp_r30;
    M559LISTNODE *var_r31;

    sp8 = no;
    var_r31 = lbl_1_bss_BB8.next;
loop_6:
    if (var_r31 == NULL) {
        return -1;
    }
    if (sp8 == (s32) var_r31->id) {
        temp_r28 = var_r31->value;
        temp_r29 = var_r31->prev;
        temp_r30 = var_r31->next;
        temp_r29->next = temp_r30;
        if (temp_r30 != NULL) {
            temp_r30->prev = temp_r29;
        }
        HuMemDirectFree(var_r31);
        return temp_r28;
    }
    var_r31 = var_r31->next;
    goto loop_6;
}
static void fn_1_5820(void) {
    s32 var_r31;

    var_r31 = 0;
    while (var_r31 < 0x20) {
        lbl_1_bss_9B0[var_r31].unk0 = 0U;
        lbl_1_bss_9B0[var_r31].unk4 = -1;
        var_r31 += 1;
    }
}
static s16 fn_1_5878(u32 dataNum) {
    s32 var_r31;
    s32 var_r31_2;

    var_r31 = 0;
loop_4:
    if (var_r31 >= 0x20) {
        var_r31_2 = 0;
loop_9:
        if (var_r31_2 >= 0x20) {
            return Hu3DModelCreate(HuDataSelHeapReadNum((s32) dataNum, 0x10000000, HEAP_MODEL));
        }
        if ((u32) lbl_1_bss_9B0[var_r31_2].unk0 == 0U) {
            lbl_1_bss_9B0[var_r31_2].unk0 = dataNum;
            lbl_1_bss_9B0[var_r31_2].unk4 = Hu3DModelCreate(HuDataSelHeapReadNum((s32) dataNum, 0x10000000, HEAP_MODEL));
            return lbl_1_bss_9B0[var_r31_2].unk4;
        }
        var_r31_2 += 1;
        goto loop_9;
    }
    if (dataNum == (u32) lbl_1_bss_9B0[var_r31].unk0) {
        return Hu3DModelLink(lbl_1_bss_9B0[var_r31].unk4);
    }
    var_r31 += 1;
    goto loop_4;
}
static void fn_1_598C(M559VEC *a, M559VEC *b, M559VEC *out, f32 t) {
    f32 temp_f31;
    f32 var_f30;
    f64 temp_f29;

    var_f30 = t;
    if (var_f30 > 1.0f) {
        var_f30 = 1.0f;
    }
    temp_f29 = sin((3.141592653589793 * (f64) (90.0f * var_f30)) / 180.0);
    temp_f31 = (f32) (sin((3.141592653589793 * (f64) (90.0f * var_f30)) / 180.0) * temp_f29);
    out->x = (f32) (a->x + (temp_f31 * (b->x - a->x)));
    out->y = (f32) (a->y + (temp_f31 * (b->y - a->y)));
    out->z = (f32) (a->z + (temp_f31 * (b->z - a->z)));
}
static void fn_1_5AE8(M559VEC *a, M559VEC *b, M559VEC *out) {
    M559VEC *sp8;

    sp8 = b;
    Hu3DCameraPosSetV(1, (Vec *) a, (Vec *) sp8, (Vec *) out);
    M2C_STRUCT_COPY(&lbl_1_bss_BA8, a, 0xC);
    M2C_STRUCT_COPY(&lbl_1_bss_B90, out, 0xC);
}
static s32 fn_1_5B74(HuVecF *pos, f32 range) {
    f32 sp20;
    f32 sp1C;
    f32 sp18;
    f32 sp14;
    f32 sp10;
    f32 spC;
    HuVecF *sp8;
    f32 temp_f28;
    f32 temp_f29;
    f32 var_f30;
    f32 var_f31;
    f64 temp_f26;
    f64 temp_f26_2;

    sp8 = pos;
    var_f31 = range;
    if (var_f31 > 1.0f) {
        var_f31 = 1.0f;
    }
    temp_f26 = sin((3.141592653589793 * (f64) (90.0f * var_f31)) / 180.0);
    temp_f29 = (f32) (sin((3.141592653589793 * (f64) (90.0f * var_f31)) / 180.0) * temp_f26);
    sp18 = lbl_1_bss_BA8.x + (temp_f29 * (lbl_1_bss_B9C.x - lbl_1_bss_BA8.x));
    sp1C = lbl_1_bss_BA8.y + (temp_f29 * (lbl_1_bss_B9C.y - lbl_1_bss_BA8.y));
    sp20 = lbl_1_bss_BA8.z + (temp_f29 * (lbl_1_bss_B9C.z - lbl_1_bss_BA8.z));
    var_f30 = range;
    if (var_f30 > 1.0f) {
        var_f30 = 1.0f;
    }
    temp_f26_2 = sin((3.141592653589793 * (f64) (90.0f * var_f30)) / 180.0);
    temp_f28 = (f32) (sin((3.141592653589793 * (f64) (90.0f * var_f30)) / 180.0) * temp_f26_2);
    spC = lbl_1_bss_B90.x + (temp_f28 * (lbl_1_bss_B84.x - lbl_1_bss_B90.x));
    sp10 = lbl_1_bss_B90.y + (temp_f28 * (lbl_1_bss_B84.y - lbl_1_bss_B90.y));
    sp14 = lbl_1_bss_B90.z + (temp_f28 * (lbl_1_bss_B84.z - lbl_1_bss_B90.z));
    Hu3DCameraPosSetV(1, (Vec *) &sp18, sp8, (Vec *) &spC);
                M2C_ERROR(/* unknown instruction: cror eq, lt, eq */);
    if (range == 1.0f) {
        return 1;
    }
    lbl_1_bss_AB4 = 1;
    return 0;
}
static s32 fn_1_5EA8(HuVecF *pos, f32 range) {
    f32 sp20;
    f32 sp1C;
    f32 sp18;
    f32 sp14;
    f32 sp10;
    f32 spC;
    HuVecF *sp8;
    f32 temp_f28;
    f32 temp_f29;
    f32 var_f30;
    f32 var_f31;
    f64 temp_f26;
    f64 temp_f26_2;
    s32 var_r31;

    sp8 = pos;
    var_r31 = 0;
loop_6:
    if (var_r31 < 4) {
        var_f31 = range;
        if (var_f31 > 1.0f) {
            var_f31 = 1.0f;
        }
        temp_f26 = sin((3.141592653589793 * (f64) (90.0f * var_f31)) / 180.0);
        temp_f29 = (f32) (sin((3.141592653589793 * (f64) (90.0f * var_f31)) / 180.0) * temp_f26);
        sp18 = lbl_1_bss_B54[var_r31].x + (temp_f29 * (lbl_1_bss_B24[var_r31].x - lbl_1_bss_B54[var_r31].x));
        sp1C = lbl_1_bss_B54[var_r31].y + (temp_f29 * (lbl_1_bss_B24[var_r31].y - lbl_1_bss_B54[var_r31].y));
        sp20 = lbl_1_bss_B54[var_r31].z + (temp_f29 * (lbl_1_bss_B24[var_r31].z - lbl_1_bss_B54[var_r31].z));
        var_f30 = range;
        if (var_f30 > 1.0f) {
            var_f30 = 1.0f;
        }
        temp_f26_2 = sin((3.141592653589793 * (f64) (90.0f * var_f30)) / 180.0);
        temp_f28 = (f32) (sin((3.141592653589793 * (f64) (90.0f * var_f30)) / 180.0) * temp_f26_2);
        spC = lbl_1_bss_AF4[var_r31].x + (temp_f28 * (lbl_1_bss_AC4[var_r31].x - lbl_1_bss_AF4[var_r31].x));
        sp10 = lbl_1_bss_AF4[var_r31].y + (temp_f28 * (lbl_1_bss_AC4[var_r31].y - lbl_1_bss_AF4[var_r31].y));
        sp14 = lbl_1_bss_AF4[var_r31].z + (temp_f28 * (lbl_1_bss_AC4[var_r31].z - lbl_1_bss_AF4[var_r31].z));
        Hu3DCameraPosSetV(1 << var_r31, (Vec *) &sp18, (Vec *) sp8, (Vec *) &spC);
        var_r31 += 1;
        goto loop_6;
    }
                M2C_ERROR(/* unknown instruction: cror eq, lt, eq */);
    if (range == 1.0f) {
        return 1;
    }
    lbl_1_bss_AB4 = 1;
    return 0;
}
static f32 fn_1_628C(f32 range, f32 farg1, f32 farg2) {
    f32 spC;
    f32 sp8;
    f32 var_f30;
    f32 var_f31;

    sp8 = farg1;
    spC = farg2;
    var_f31 = (f32) fmod((f64) (sp8 - range), 360.0);
    if (var_f31 < 0.0f) {
        var_f31 += 360.0f;
    }
    if (var_f31 > 180.0f) {
        var_f31 -= 360.0f;
    }
    var_f30 = (f32) fmod((f64) (range + (var_f31 * spC)), 360.0);
    if (var_f30 < 0.0f) {
        var_f30 += 360.0f;
    }
    return var_f30;
}
static void fn_1_6390(s32 no) {
    s32 var_r31;

    var_r31 = no;
    if (lbl_1_bss_AB8 >= 0) {
        if (var_r31 < 0) {
            var_r31 = 0x3E8;
        }
        HuAudSeqFadeOut(lbl_1_bss_AB8, var_r31);
    }
    lbl_1_bss_AB8 = -1;
}
static void fn_1_63FC(s16 modelId, char *objName, M559VEC *out) {
    Mtx mtx;

    Hu3DModelObjMtxGet(modelId, objName, mtx);
    out->x = mtx[3][0];
    out->y = mtx[3][1];
    out->z = mtx[3][2];
}
static s32 fn_1_6454(s32 a, s32 b) {
    s32 sp8;

    sp8 = b;
    return a + frandmod(sp8 - a);
}
static s32 fn_1_6490(M559VEC *a, M559VEC *b, f32 range, f32 farg1) {
    if ((((b->x - a->x) * (b->x - a->x)) + ((b->z - a->z) * (b->z - a->z))) < ((range + farg1) * (range + farg1))) {
        return 1;
    }
    return 0;
}
static s32 fn_1_64F0(HuVecF *a, s32 b, s32 c) {
    Point3d sp10;
    s32 spC;
    HuVecF *sp8;

    sp8 = a;
    spC = c;
    Hu3D3Dto2D((HuVecF *) a, 1, (HuVecF *) &sp10);
    return (s32) ((f32) b + ((sp10.x / 576.0f) * (f32) (spC - b)));
}
static s32 fn_1_65B4(HuVecF *a, s32 b, s32 c) {
    Point3d sp10;
    s32 spC;
    HuVecF *sp8;

    sp8 = a;
    spC = c;
    Hu3D3Dto2D((HuVecF *) a, 1, (HuVecF *) &sp10);
    return (s32) ((f32) b + ((sp10.y / 480.0f) * (f32) (spC - b)));
}
static void fn_1_6678(M559HOOKDATA *data) {
    f32 temp_f30;
    f32 temp_f31;
    f32 var_f29;
    f32 var_f31;
    s16 var_r28;
    M559VEC *temp_r4;
    M559CLR *var_r29;
    M559ANIMPART *var_r31;

    var_r31 = data->unk10;
    var_r29 = (M559CLR *) data->unk20;
    data->arg66 = 0;
    var_r28 = 0;
loop_17:
    if (var_r28 < data->arg64) {
        if ((s16) var_r31->unk3E >= 0) {
            var_r31->unk10 = (f32) (var_r31->unk10 + var_r31->unk1C);
            if (var_r31->unk20 < var_r31->unk10) {
                var_r31->unk3E = -1;
            } else {
                data->arg66 = (s16) (data->arg66 + 1);
                if (data->field8 != NULL) {
                    temp_r4 = (M559VEC *) data->field8;
                    var_r31->unk4 = temp_r4->x;
                    var_r31->unk8 = temp_r4->y;
                    var_r31->unkC = temp_r4->z;
                }
                if (data->fieldC != NULL) {
                    var_r31->unk14 = *data->fieldC;
                }
                var_r31->unk30 = (f32) ((f64) var_r31->unk4 + ((f64) var_r31->unk10 * sin((3.141592653589793 * (f64) (var_r31->unk14 + var_r31->unk18)) / 180.0)));
                var_r31->unk34 = (f32) (var_r31->unk34 + (0.03f * (var_r31->unk24 - var_r31->unk34)));
                var_r31->unk38 = (f32) ((f64) var_r31->unkC + ((f64) var_r31->unk10 * cos((3.141592653589793 * (f64) (var_r31->unk14 + var_r31->unk18)) / 180.0)));
                temp_f31 = var_r31->unk10 / var_r31->unk20;
                temp_f30 = (f32) (128.0 * cos((3.141592653589793 * (f64) (90.0f * temp_f31)) / 180.0));
                if (temp_f30 > 255.0f) {
                    var_f29 = 255.0f;
                } else {
                    var_f29 = temp_f30;
                }
                var_r31->unk3C = (s16) ((f32) data->arg68 * temp_f31);
                if (data->arg68 <= (s16) var_r31->unk3C) {
                    var_r31->unk3C = (s16) (data->arg68 - 1);
                    var_r31->unk3C = (s16) (data->arg68 - 1);
                }
                var_f31 = temp_f31 * 2.0f;
                if (var_f31 > 1.0f) {
                    var_f31 = 1.0f;
                }
                var_r31->unk2C = (f32) (1.5 * sin((3.141592653589793 * (f64) (5.0f + (100.0f * var_f31))) / 180.0));
            }
        }
        var_r28 += 1;
        var_r31 += 0x40;
        var_r29 += 4;
        goto loop_17;
    }
}
static void fn_1_69D8(M559VERTS *a, M559VERTS *node) {
    node->unk0 = (f32) a->unk0;
    node->unk10 = (f32) a->unk4;
    node->unk20 = (f32) a->unk8;
    node->unk4 = (f32) a->unk10;
    node->unk14 = (f32) a->unk14;
    node->unk24 = (f32) a->unk18;
    node->unk8 = (f32) a->unk20;
    node->unk18 = (f32) a->unk24;
    node->unk28 = (f32) a->unk28;
    node->unkC = 0.0f;
    node->unk1C = 0.0f;
    node->unk2C = 0.0f;
}
static void fn_1_6A54(M559VERTS *a, M559VERTS *node, s16 no, f32 range) {
    s16 var_r31;
    M559VERTS *var_r3;
    M559VERTS *var_r4;

    var_r3 = (M559VERTS *) a;
    var_r4 = node;
    var_r31 = 0;
loop_2:
    if (var_r31 < no) {
        var_r4->unk0 = (f32) (var_r3->unk0 * range);
        var_r4->unk4 = (f32) (var_r3->unk4 * range);
        var_r4->unk8 = (f32) (var_r3->unk8 * range);
        var_r31 += 1;
        var_r3 += 0xC;
        var_r4 += 0xC;
        goto loop_2;
    }
}
static void fn_1_6AB0(M559VERTS *a, f32 *b, s16 c, f32 range, f32 e) {
    a->unk0 = (f32) *(b + (c * 8));
    a->unk4 = (f32) *(b + ((c * 8) + 4));
    a->unk8 = (f32) (range + *(b + (c * 8)));
    a->unkC = (f32) *(b + ((c * 8) + 4));
    a->unk10 = (f32) (range + *(b + (c * 8)));
    a->unk14 = (f32) (e + *(b + ((c * 8) + 4)));
    a->unk18 = (f32) *(b + (c * 8));
    a->unk1C = (f32) (e + *(b + ((c * 8) + 4)));
}
void fn_1_6B54(HU3DMODEL *model, f32 (*mtx)[4]) {
    f32 sp130[12];
    f32 sp15C;
    f32 sp158;
    f32 sp154;
    f32 sp150;
    f32 sp14C;
    f32 sp148;
    f32 sp144;
    f32 sp140;
    f32 sp13C;
    f32 sp138;
    f32 sp134;
    Point3d sp70;
    Point3d sp64;
    Point3d sp58;
    Point3d sp4C;
    Point3d sp40;
    Point3d sp34;
    Point3d sp28;
    Point3d sp1C;
    Point3d sp10;
    M559PARTCALLBACK spC;
    s16 sp8;
    Point3d *temp_r30_4;
    Point3d *temp_r30_5;
    Point3d *temp_r30_6;
    Point3d *temp_r30_7;
    Point3d *temp_r30_8;
    Point3d *temp_r30_9;
    Point3d *var_r22;
    Point3d *var_r24;
    Point3d *var_r25;
    Point3d *var_r30;
    f32 temp_f28;
    f32 temp_f29;
    f32 temp_f30;
    f32 temp_f31;
    s16 temp_r0;
    s16 temp_r21;
    s16 var_r17;
    s16 var_r18;
    s16 var_r19;
    f32 *temp_r26;
    u8 temp_r0_2;
    Point3d *temp_r30;
    Point3d *temp_r30_2;
    Point3d *temp_r30_3;
    M559HOOKDATA *temp_r31;
    Point3d *var_r23;
    M559TEXATTR *var_r27;
    M559ANIMPART *var_r29;
    f32 sp100[4][3];
    f32 spD0[4][4];
    f32 spA0[4][4];

    temp_r31 = (M559HOOKDATA *) model->hookData;
    if (((u8) temp_r31->flag6B == 0) || ((s16) temp_r31->unk66 != 0)) {
        GXLoadPosMtxImm(mtx, 0U);
        GXSetNumTevStages(1U);
        GXSetNumTexGens(1U);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3CU, 0U, 0x7DU);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        temp_r0 = temp_r31->anim->bmp->dataFmt & 0xF;
        sp8 = temp_r0;
        if ((temp_r0 == 8) || (sp8 == 7)) {
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXA, GX_CC_RASC, GX_CC_ZERO);
        } else {
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_ZERO);
        }
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1U, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1U, GX_TEVPREV);
        GXSetNumChans(1U);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_CLAMP, GX_AF_NONE);
        if ((model->attr & 2) != 0) {
            GXSetZMode(1U, GX_LEQUAL, 1U);
        } else {
            GXSetZMode(1U, GX_LEQUAL, 0U);
        }
        HuSprTexLoad(temp_r31->anim, 0, 0, 1, 1, 1);
        GXSetAlphaCompare(GX_GEQUAL, 1U, GX_AOP_AND, GX_GEQUAL, 1U);
        GXSetZCompLoc(0U);
        temp_r0_2 = temp_r31->unk6A;
        switch (temp_r0_2) {                  /* irregular */
        case 0:
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
            break;
        case 1:
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_NOOP);
            break;
        case 2:
            GXSetBlendMode(GX_BM_BLEND, GX_BL_ZERO, GX_BL_INVDSTCLR, GX_LO_NOOP);
            break;
        }
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_TEX_ST, GX_RGBA6, 0U);
        GXSETARRAY(GX_VA_POS, temp_r31->unk14, (u32) (temp_r31->arg64 * 4), 0xC, TRUE);
        GXSetVtxDesc(GX_VA_CLR0, GX_INDEX16);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_TEX_ST, GX_RGBA8, 0U);
        GXSETARRAY(GX_VA_CLR0, temp_r31->unk20, (u32) (temp_r31->arg64 * 4), 4, TRUE);
        GXSetVtxDesc(GX_VA_TEX0, GX_INDEX16);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_RGBA6, 0U);
        GXSETARRAY(GX_VA_TEX0, temp_r31->unk18, (u32) (temp_r31->arg64 * 4), 8, TRUE);
        sp130[0] = mtx[0][0];
        sp130[3] = mtx[0][1];
        sp130[6] = mtx[0][2];
        sp130[1] = mtx[1][0];
        sp130[4] = mtx[1][1];
        sp130[7] = mtx[1][2];
        sp130[2] = mtx[2][0];
        sp130[5] = mtx[2][1];
        sp130[8] = mtx[2][2];
        sp130[9] = 0.0f;
        sp130[10] = 0.0f;
        sp130[11] = 0.0f;
        PSMTXReorder((f32 (*)[4]) sp130, (f32 (*)[3]) sp100);
        if ((omPauseChk() == 0) && (temp_r31->unk0 != NULL)) {
            spC = temp_r31->unk0;
            spC(model, temp_r31, mtx);
        }
        var_r29 = temp_r31->unk10;
        var_r30 = (Point3d *) temp_r31->unk14;
        var_r27 = (M559TEXATTR *) temp_r31->unk18;
        PSMTXROMultVecArray(sp100, (Vec *) temp_r31->mtx28, (Vec *) &sp70, 4U);
        var_r19 = 0;
loop_36:
        if (var_r19 < temp_r31->unk64) {
            if (var_r29->unk3E < 0) {
                var_r30->z = 0.0f;
                var_r30->y = 0.0f;
                var_r30->x = 0.0f;
                temp_r30 = var_r30 + 1;
                temp_r30->z = 0.0f;
                temp_r30->y = 0.0f;
                temp_r30->x = 0.0f;
                temp_r30_2 = temp_r30 + 1;
                temp_r30_2->z = 0.0f;
                temp_r30_2->y = 0.0f;
                temp_r30_2->x = 0.0f;
                temp_r30_3 = temp_r30_2 + 1;
                temp_r30_3->z = 0.0f;
                temp_r30_3->y = 0.0f;
                temp_r30_3->x = 0.0f;
                var_r30 = temp_r30_3 + 1;
            } else {
                if (var_r29->unk28 == 0.0f) {
                    temp_f31 = var_r29->unk2C;
                    var_r24 = &sp40;
                    var_r25 = &sp70;
                    var_r18 = 0;
loop_35:
                    if (var_r18 < 4) {
                        var_r24->x = var_r25->x * temp_f31;
                        var_r24->y = var_r25->y * temp_f31;
                        var_r24->z = var_r25->z * temp_f31;
                        var_r18 += 1;
                        var_r25 += 1;
                        var_r24 += 1;
                        goto loop_35;
                    }
                    temp_r30_4 = var_r30 + 1;
                    PSVECAdd((Vec *) &sp40, (Vec *) &var_r29->unk30, (Vec *) var_r30);
                    temp_r30_5 = temp_r30_4 + 1;
                    PSVECAdd((Vec *) &sp4C, (Vec *) &var_r29->unk30, (Vec *) temp_r30_4);
                    temp_r30_6 = temp_r30_5 + 1;
                    PSVECAdd((Vec *) &sp58, (Vec *) &var_r29->unk30, (Vec *) temp_r30_5);
                    var_r30 = temp_r30_6 + 1;
                    PSVECAdd((Vec *) &sp64, (Vec *) &var_r29->unk30, (Vec *) temp_r30_6);
                } else {
                    temp_f30 = var_r29->unk2C;
                    var_r22 = &sp40;
                    var_r23 = (Point3d *) temp_r31->mtx28;
                    var_r17 = 0;
loop_34:
                    if (var_r17 < 4) {
                        var_r22->x = var_r23->x * temp_f30;
                        var_r22->y = var_r23->y * temp_f30;
                        var_r22->z = var_r23->z * temp_f30;
                        var_r17 += 1;
                        var_r23 += 1;
                        var_r22 += 1;
                        goto loop_34;
                    }
                    PSMTXRotRad((f32 (*)[4]) spD0, 0x5A, 0.017453292f * var_r29->unk28);
                    PSMTXConcat((f32 (*)[4]) sp130, (f32 (*)[4]) spD0, (f32 (*)[4]) spA0);
                    PSMTXMultVecArray((f32 (*)[4]) spA0, (Vec *) &sp40, (Vec *) &sp10, 4U);
                    temp_r30_7 = var_r30 + 1;
                    PSVECAdd((Vec *) &sp10, (Vec *) &var_r29->unk30, (Vec *) var_r30);
                    temp_r30_8 = temp_r30_7 + 1;
                    PSVECAdd((Vec *) &sp1C, (Vec *) &var_r29->unk30, (Vec *) temp_r30_7);
                    temp_r30_9 = temp_r30_8 + 1;
                    PSVECAdd((Vec *) &sp28, (Vec *) &var_r29->unk30, (Vec *) temp_r30_8);
                    var_r30 = temp_r30_9 + 1;
                    PSVECAdd((Vec *) &sp34, (Vec *) &var_r29->unk30, (Vec *) temp_r30_9);
                }
                if (var_r29->unk3C != -1) {
                    temp_f28 = temp_r31->unk60;
                    temp_f29 = temp_r31->unk5C;
                    temp_r21 = var_r29->unk3C;
                    temp_r26 = temp_r31->unk1C;
                    var_r27->unk0 = temp_r26[temp_r21 * 2];
                    var_r27->unk4 = temp_r26[(temp_r21 * 2) + 1];
                    var_r27->unk8 = temp_f29 + temp_r26[temp_r21 * 2];
                    var_r27->unkC = temp_r26[(temp_r21 * 2) + 1];
                    var_r27->unk10 = temp_f29 + temp_r26[temp_r21 * 2];
                    var_r27->unk14 = temp_f28 + temp_r26[(temp_r21 * 2) + 1];
                    var_r27->unk18 = temp_r26[temp_r21 * 2];
                    var_r27->unk1C = temp_f28 + temp_r26[(temp_r21 * 2) + 1];
                }
            }
            var_r19 += 1;
            var_r29 += 1;
            var_r27 += 1;
            goto loop_36;
        }
        DCFlushRangeNoSync(temp_r31->unk14, temp_r31->unk64 * 0xC * 4);
        DCFlushRangeNoSync(temp_r31->unk18, temp_r31->unk64 * 8 * 4);
        GXCallDisplayList(temp_r31->unk24, temp_r31->unk58);
    }
}
static void fn_1_729C(M559VEC *a, s16 b, s16 c, f32 d, f32 e) {
    s16 var_r31;
    M559VERTS *var_r3;

    var_r3 = (M559VERTS *) a;
    var_r31 = 0;
loop_2:
    if (var_r31 < b) {
        var_r3->unk0 = (f32) ((f32) (s16) (var_r31 % c) * d);
        var_r3->unk4 = (f32) ((f32) (s16) (var_r31 / c) * e);
        var_r31 += 1;
        var_r3 += 8;
        goto loop_2;
    }
}
static s16 fn_1_7368(ANIMDATA *anim, M559VEC *field8, f32 *fieldC, s16 partCount, s16 cols, s16 rows, f32 range) {
    void *sp1C;
    u8 sp18;
    u8 sp16;
    u8 sp14;
    s16 sp12;
    s16 sp10;
    f32 *spC;
    M559VEC *sp8;
    HU3DMODEL *temp_r28;
    f32 temp_f0;
    s16 temp_r0_2;
    s16 temp_r3;
    s16 var_r17;
    s16 var_r20;
    s16 var_r21;
    s16 var_r30;
    s16 var_r30_2;
    s16 var_r30_3;
    s16 var_r30_4;
    s16 var_r30_5;
    s32 temp_r0;
    s32 temp_r0_3;
    M559TEXATTR *temp_r29;
    M559TEXATTR *temp_r29_2;
    M559TEXATTR *temp_r29_3;
    M559HOOKDATA *temp_r31;
    void *temp_r3_2;
    f32 *var_r23;
    M559ANIMPART *var_r24;
    Point3d *var_r25;
    M559CLR *var_r26;
    M559TEXATTR *var_r29;

    sp8 = field8;
    spC = fieldC;
    sp10 = cols;
    sp12 = rows;
    temp_r3 = Hu3DHookFuncCreate((HU3DMODELHOOK) fn_1_6B54);
    temp_r28 = &Hu3DData[temp_r3];
    temp_r28->hookData = HuMemDirectMallocNum(HEAP_MODEL, 0x6C, temp_r28->mallocNo);
    temp_r31 = (M559HOOKDATA *) temp_r28->hookData;
    anim->useNum += 1;
    temp_r31->anim = anim;
    temp_r31->field8 = sp8;
    temp_r31->fieldC = spC;
    temp_r31->arg64 = partCount;
    temp_r31->flag6A = 0;
    temp_r31->state = 0;
    temp_r31->arg66 = partCount;
    temp_r31->flag6B = 0;
    temp_r31->partIdx = (M559ANIMPART *) HuMemDirectMallocNum(HEAP_MODEL, partCount << 6, temp_r28->mallocNo);
    var_r24 = temp_r31->partIdx;
    var_r30 = 0;
loop_2:
    if (var_r30 < partCount) {
        var_r24->unk3E = -1;
        var_r24->unk3C = -1;
        var_r30 += 1;
        var_r24 += 1;
        goto loop_2;
    }
    temp_r31->partPos = (f32 *) HuMemDirectMallocNum(HEAP_MODEL, partCount * 0xC * 4, temp_r28->mallocNo);
    var_r25 = (Point3d *) temp_r31->partPos;
    var_r30_2 = 0;
loop_5:
    if (var_r30_2 < (s32) (partCount * 4)) {
        var_r25->x = 0.0f;
        var_r25->y = 0.0f;
        var_r25->z = 0.0f;
        var_r30_2 += 1;
        var_r25 += 1;
        goto loop_5;
    }
    temp_r31->unk20 = (M559CLR *) HuMemDirectMallocNum(HEAP_MODEL, partCount * 4, temp_r28->mallocNo);
    var_r26 = temp_r31->unk20;
    var_r30_3 = 0;
loop_8:
    if (var_r30_3 < partCount) {
        var_r26->unk0 = 0xFF;
        var_r26->unk1 = 0xFF;
        var_r26->unk2 = 0xFF;
        var_r26->unk3 = 0xFF;
        var_r30_3 += 1;
        var_r26 += 1;
        goto loop_8;
    }
    temp_r31->partAttr = (u32 *) HuMemDirectMallocNum(HEAP_MODEL, partCount * 8 * 4, temp_r28->mallocNo);
    var_r29 = (M559TEXATTR *) temp_r31->partAttr;
    var_r30_4 = 0;
loop_11:
    if (var_r30_4 < partCount) {
        var_r29->unk0 = 0.0f;
        var_r29->unk4 = 0.0f;
        temp_r29 = var_r29 + 1;
        var_r29->unk8 = 1.0f;
        temp_r29->unk4 = 0.0f;
        temp_r29_2 = temp_r29 + 1;
        temp_r29->unk8 = 1.0f;
        temp_r29_2->unk4 = 1.0f;
        temp_r29_3 = temp_r29_2 + 1;
        temp_r29_2->unk8 = 0.0f;
        temp_r29_3->unk4 = 1.0f;
        var_r29 = temp_r29_3 + 1;
        var_r30_4 += 1;
        goto loop_11;
    }
    if ((sp10 != 0) || (sp12 != 0)) {
        var_r21 = (s16) anim->bmp->sizeX / sp10;
        var_r17 = (s16) anim->bmp->sizeY / sp12;
        temp_r0 = var_r17;
        temp_r31->unk68 = (s16) (var_r21 * (s16) temp_r0);
        temp_r31->field5C = (f32) (1.0f / (f32) var_r21);
        temp_r31->field60 = 1.0f / (f32) var_r17;
    } else {
        var_r21 = 1;
        temp_r31->unk68 = 1;
        temp_r31->field5C = 1.0f;
        temp_r31->field60 = 1.0f;
    }
    temp_r31->unk1C = (f32 *) HuMemDirectMallocNum(HEAP_MODEL, temp_r31->unk68 * 8, temp_r28->mallocNo);
    sp14 = temp_r31->unk68;
    var_r23 = temp_r31->unk1C;
    var_r20 = 0;
loop_18:
    if (var_r20 < sp14) {
        temp_r0_2 = var_r20 % var_r21;
        sp16 = temp_r0_2;
        temp_r0_3 = var_r20 / var_r21;
        sp18 = (u8) temp_r0_3;
        var_r23[0] = (f32) ((f32) temp_r0_2 * temp_r31->field5C);
        var_r23[1] = (f32) ((f32) temp_r0_3 * temp_r31->field60);
        var_r20 += 1;
        var_r23 += 2;
        goto loop_18;
    }
    temp_f0 = -range;
    temp_r31->field50 = temp_f0;
    temp_r31->field44 = temp_f0;
    temp_r31->field4C = temp_f0;
    temp_r31->field28 = temp_f0;
    temp_r31->field38 = range;
    temp_r31->field2C = range;
    temp_r31->field40 = range;
    temp_r31->field34 = range;
    temp_r31->field54 = 0.0f;
    temp_r31->field48 = 0.0f;
    temp_r31->field3C = 0.0f;
    temp_r31->field30 = 0.0f;
    temp_r3_2 = HuMemDirectMallocNum(HEAP_MODEL, 0x20000, temp_r28->mallocNo);
    sp1C = temp_r3_2;
    GXBeginDisplayList(sp1C, 0x20000U);
    GXBegin(GX_QUADS, GX_VTXFMT0, (u16) (partCount * 4));
    var_r30_5 = 0;
loop_21:
    if (var_r30_5 < partCount) {
        GXPosition1x16((u16) (var_r30_5 * 4));
        GXPosition1x16((u16) var_r30_5);
        GXPosition1x16((u16) (var_r30_5 * 4));
        GXPosition1x16((u16) ((var_r30_5 * 4) + 1));
        GXPosition1x16((u16) var_r30_5);
        GXPosition1x16((u16) ((var_r30_5 * 4) + 1));
        GXPosition1x16((u16) ((var_r30_5 * 4) + 2));
        GXPosition1x16((u16) var_r30_5);
        GXPosition1x16((u16) ((var_r30_5 * 4) + 2));
        GXPosition1x16((u16) ((var_r30_5 * 4) + 3));
        GXPosition1x16((u16) var_r30_5);
        GXPosition1x16((u16) ((var_r30_5 * 4) + 3));
        var_r30_5 += 1;
        goto loop_21;
    }
    temp_r31->unk58 = GXEndDisplayList();
    DCFlushRangeNoSync(temp_r3_2, temp_r31->unk58);
    temp_r31->unk24 = HuMemDirectMallocNum(HEAP_MODEL, (s32) temp_r31->unk58, temp_r28->mallocNo);
    memcpy(temp_r31->unk24, temp_r3_2, temp_r31->unk58);
    DCFlushRangeNoSync(temp_r31->unk24, temp_r31->unk58);
    HuMemDirectFree(temp_r3_2);
    return temp_r3;
}
static void *fn_1_79C0(s16 modelId, M559VEC *pos, M559CLR *clr, f32 unk2C, f32 unk28) {
    s16 var_r29;
    M559HOOKDATA *temp_r30;
    M559CLR *temp_r6;
    M559ANIMPART *var_r31;

    temp_r30 = (M559HOOKDATA *) Hu3DData[modelId].hookData;
    if (temp_r30->flag6B != 0) {
        return NULL;
    }
    var_r29 = 0;
    var_r31 = temp_r30->partIdx;
loop_5:
    if (var_r29 < temp_r30->arg64) {
        if (var_r31->unk3E >= 0) {
            var_r29 += 1;
            var_r31 += 1;
            goto loop_5;
        }
    }
    if (var_r29 == temp_r30->arg64) {
        return NULL;
    }
    temp_r6 = temp_r30->unk20 + var_r29;
    temp_r6->unk0 = clr->unk0;
    temp_r6->unk1 = clr->unk1;
    temp_r6->unk2 = clr->unk2;
    temp_r6->unk3 = clr->unk3;
    var_r31->unk30 = pos->x;
    var_r31->unk34 = pos->y;
    var_r31->unk38 = pos->z;
    var_r31->unk2C = unk2C;
    var_r31->unk28 = unk28;
    var_r31->unk3C = 0;
    var_r31->unk0 = 0;
    var_r31->unk3E = var_r29;
    return var_r31;
}
static void fn_1_7AC8(s16 modelId, M559PARTCALLBACK func) {
    ((M559HOOKDATA *) Hu3DData[modelId].hookData)->unk0 = func;
}
static void fn_1_7B00(s16 modelId, s8 flag) {
    ((M559HOOKDATA *) Hu3DData[modelId].hookData)->unk6A = flag;
}
static void fn_1_7B30(s16 modelId) {
    ((M559HOOKDATA *) Hu3DData[modelId].hookData)->unk6B = 1;
}
static s32 fn_1_7B6C(s16 modelId) {
    HU3DMODEL *temp_r30;
    M559HOOKDATA *temp_r31;

    temp_r30 = &Hu3DData[modelId];
    temp_r31 = (M559HOOKDATA *) temp_r30->hookData;
    if ((temp_r31->flag6B != 0) && (temp_r31->arg66 == 0)) {
        HuMemDirectFree(temp_r31->partIdx);
        HuMemDirectFree(temp_r31->partPos);
        HuMemDirectFree(temp_r31->unk20);
        HuMemDirectFree(temp_r31->partAttr);
        HuMemDirectFree(temp_r31->unk1C);
        HuMemDirectFree(temp_r31->unk24);
        HuSprAnimKill(temp_r31->anim);
        HuMemDirectFree(temp_r30->hookData);
        Hu3DModelKill(modelId);
        return 1;
    }
    return 0;
}
static void fn_1_7C28(s16 modelId, s16 count, M559VEC *pos, f32 scale, f32 range, f32 time, s16 flag) {
    u8 sp1B;
    u8 sp1A;
    u8 sp19;
    u8 sp18;
    f32 sp14;
    f32 sp10;
    f32 spC;
    s16 spA;
    s16 sp8;
    s16 var_r27;
    s16 var_r28;
    M559HOOKDATA *temp_r29;
    M559CLR *temp_r3;
    M559ANIMPART *var_r26;
    M559ANIMPART *var_r30;

    sp8 = modelId;
    spA = count;
    spC = scale;
    sp10 = range;
    sp14 = time;
    sp18 = lbl_1_data_18[0];
    sp19 = lbl_1_data_18[1];
    sp1A = lbl_1_data_18[2];
    sp1B = lbl_1_data_18[3];
    var_r27 = 0;
loop_12:
    if (var_r27 < spA) {
        temp_r29 = (M559HOOKDATA *) Hu3DData[sp8].hookData;
        if (temp_r29->flag6B != 0) {
            var_r26 = NULL;
        } else {
            var_r28 = 0;
            var_r30 = temp_r29->unk10;
loop_6:
            if (var_r28 < temp_r29->unk64) {
                if (var_r30->unk3E >= 0) {
                    var_r28 += 1;
                    var_r30 += 1;
                    goto loop_6;
                }
            }
            if (var_r28 == temp_r29->unk64) {
                var_r26 = NULL;
            } else {
                temp_r3 = temp_r29->unk20 + var_r28;
                temp_r3->unk0 = sp18;
                temp_r3->unk1 = sp19;
                temp_r3->unk2 = sp1A;
                temp_r3->unk3 = sp1B;
                var_r30->unk30 = pos->x;
                var_r30->unk34 = pos->y;
                var_r30->unk38 = pos->z;
                var_r30->unk2C = 0.0f;
                var_r30->unk28 = 0.0f;
                var_r30->unk3C = 0;
                var_r30->unk0 = 0;
                var_r30->unk3E = var_r28;
                var_r26 = var_r30;
            }
        }
        if (var_r26 != NULL) {
            var_r26->unk4 = pos->x;
            var_r26->unk8 = pos->y;
            var_r26->unkC = pos->z;
            var_r26->unk10 = 0.0f;
            var_r26->unk14 = spC;
            var_r26->unk1C = 10.0f;
            var_r26->unk20 = sp10;
            var_r26->unk24 = sp14;
            var_r26->unk18 = (20.0f * frandf()) - 10.0f;
            var_r26->unk1C *= 0.8f + (0.4f * frandf());
            var_r26->unk20 *= 0.9f + (0.3f * frandf());
            var_r27 += 1;
            goto loop_12;
        }
    }
}
static s16 fn_1_7E88(s32 a, s32 b, s16 c) {
    s16 sp10;
    f32 *spC;
    M559VEC *sp8;
    s16 temp_r3;

    sp8 = (M559VEC *) (uintptr_t) a;
    spC = (f32 *) (uintptr_t) b;
    sp10 = c;
    temp_r3 = fn_1_7368(lbl_1_bss_0, sp8, spC, sp10, 0x40, 0x40, 40.0f);
    ((M559HOOKDATA *) Hu3DData[temp_r3].hookData)->unk0 = (M559PARTCALLBACK) fn_1_6678;
    ((M559HOOKDATA *) Hu3DData[temp_r3].hookData)->unk6A = 0;
    Hu3DModelLayerSet(temp_r3, 4);
    return temp_r3;
}

