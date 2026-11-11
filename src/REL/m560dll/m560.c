#include "game/audio.h"
#include <string.h>
#include "game/charman.h"
#include "game/data.h"
#include "game/flag.h"
#include "game/frand.h"
#include "game/gamework.h"
#include "game/hsfex.h"
#include "game/hu3d.h"
#include "game/printfunc.h"
#include "humath.h"
#include "game/memory.h"
#include "game/mg/seqman.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/process.h"
#include "game/sprite.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

/* Per-player work block (omObj data), 0x98 bytes */
typedef struct M560Player_s {
    /* 0x00 */ s16 motNo;
    /* 0x02 */ u8 pad02[0x12];
    /* 0x14 */ s32 playerNo;
    /* 0x18 */ s32 charNo;
    /* 0x1C */ s32 padNo;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 state;
    /* 0x28 */ s32 timer;
    /* 0x2C */ Vec pos;
    /* 0x38 */ Vec prevPos;
    /* 0x44 */ Vec rot;
    /* 0x50 */ u8 pad50[0x10];
    /* 0x60 */ u32 curItem;
    /* 0x64 */ s32 itemCnt;
    /* 0x68 */ s32 done;
    /* 0x6C */ s32 unk6C;
    /* 0x70 */ s16 mdl70;
    /* 0x72 */ s16 mdl72;
    /* 0x74 */ s16 mdl74;
    /* 0x76 */ s16 itemMdl[7];
    /* 0x84 */ s16 mdl84;
    /* 0x86 */ s16 mdl86;
    /* 0x88 */ s32 dif;
    /* 0x8C */ s32 reactRange;
    /* 0x90 */ s32 comTimer;
    /* 0x94 */ s32 comWait;
} M560Player;

/* Game state work block (omObj data), 0x3C bytes */
typedef struct M560Game_s {
    /* 0x00 */ s32 timer;
    /* 0x04 */ s16 mdl;
    /* 0x06 */ s16 pad06;
    /* 0x08 */ Vec pos;
    /* 0x14 */ Vec rot;
    /* 0x20 */ f32 unk20;
    /* 0x24 */ f32 unk24;
    /* 0x28 */ f32 speed;
    /* 0x2C */ f32 prevTime;
    /* 0x30 */ s32 motNo;
    /* 0x34 */ s32 state;
    /* 0x38 */ s32 particleMdl;
} M560Game;

typedef struct M560Node_s {
    u16 id;
    s16 val;
    struct M560Node_s *prev;
    struct M560Node_s *next;
} M560Node;

typedef struct M560NodeHead_s {
    M560Node head;
    s32 pad;
} M560NodeHead;

/* One particle of the effect system (stride 0x40) */
typedef struct M560Cell_s {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ Vec org;
    /* 0x10 */ f32 dist;
    /* 0x14 */ f32 ang;
    /* 0x18 */ f32 angOfs;
    /* 0x1C */ f32 speed;
    /* 0x20 */ f32 maxDist;
    /* 0x24 */ f32 targetY;
    /* 0x28 */ f32 rot;
    /* 0x2C */ f32 scale;
    /* 0x30 */ Vec pos;
    /* 0x3C */ s16 frame;
    /* 0x3E */ s16 idx;
} M560Cell;

typedef struct M560Uv_s {
    f32 v[8];
} M560Uv;

typedef struct M560Parm_s M560Parm;
typedef void (*M560PARMHOOK)(HU3DMODEL *model, M560Parm *parm, Mtx mtx);

/* hookData of the models made by fn_1_5A9C, 0x6C bytes on GC */
struct M560Parm_s {
    /* 0x00 */ M560PARMHOOK hook;
    /* 0x04 */ ANIMDATA *anim;
    /* 0x08 */ Vec *followPos;
    /* 0x0C */ f32 *followRot;
    /* 0x10 */ M560Cell *cells;
    /* 0x14 */ Vec *vtx;
    /* 0x18 */ M560Uv *uv;
    /* 0x1C */ f32 *uvTbl;
    /* 0x20 */ GXColor *color;
    /* 0x24 */ void *dl;
    /* 0x28 */ Vec corner[4];
    /* 0x58 */ s32 dlSize;
    /* 0x5C */ f32 uScale;
    /* 0x60 */ f32 vScale;
    /* 0x64 */ s16 cellCnt;
    /* 0x66 */ s16 activeCnt;
    /* 0x68 */ s16 frameCnt;
    /* 0x6A */ u8 blend;
    /* 0x6B */ u8 kill;
};

extern void HuSprTexLoad(ANIMDATA *anim, s16 bmpNo, s16 texMapId, s32 wrapS, s32 wrapT, s32 filter);

void ObjectSetup(void);
void fn_1_4B0(OMOBJ *obj);
void fn_1_9F8(OMOBJ *obj);
void fn_1_FE4(OMOBJ *obj);
s32 fn_1_12B4(M560Player *work);
u16 fn_1_143C(M560Player *work);
void fn_1_14E4(void);
void fn_1_15D8(M560Player *work, s32 arg1);
void fn_1_1F38(OMOBJ *obj, s32 motNo, f32 time, u32 attr);
void fn_1_1768(M560Player *work);
s32 fn_1_2054(void);
void fn_1_3468(OMOBJ *obj, s32 motNo, f32 time, u32 attr);
void fn_1_6264(s16 mdlId);
s32 fn_1_62A0(s16 mdlId);
void fn_1_635C(s16 mdlId, s16 count, Vec *pos, f32 angle, f32 maxDist, f32 targetY);
s16 fn_1_65BC(Vec *followPos, f32 *followRot, s16 cellCnt);
void fn_1_4C34(HU3DMODEL *model, M560Parm *parm, Mtx mtx);
s16 fn_1_5A9C(ANIMDATA *anim, Vec *followPos, f32 *followRot, s16 cellCnt, s16 cols, s16 rows, f32 range);
s32 fn_1_1FE4(void);
void fn_1_20F4(void);
void fn_1_24B8(OMOBJ *obj);
void fn_1_268C(OMOBJ *obj);
void fn_1_354C(OMOBJ *obj);
void fn_1_381C(void);
void fn_1_3830(void);
void fn_1_3860(void);
void fn_1_3890(u16 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4);
void fn_1_38EC(void);
s32 fn_1_3A58(Vec *pos);
s32 fn_1_3AD4(Vec *pos);
s32 fn_1_3B50(void);
void fn_1_1808(void);
void fn_1_18F8(void);
void fn_1_1B88(void);
void fn_1_1C64(void);
void fn_1_1D00(void);
void fn_1_1E30(void);

extern void fn_1_668C(void);
extern void fn_1_6770(s16 arg0, Vec *arg1);
extern void fn_1_7008(void);

ANIMDATA *lbl_1_bss_0;
f32 lbl_1_bss_4;
f32 lbl_1_bss_8;
s32 lbl_1_bss_C;
f32 lbl_1_bss_10;
f32 lbl_1_bss_14;
f32 lbl_1_bss_18[2];
s32 lbl_1_bss_20;
s32 lbl_1_bss_24;
s32 lbl_1_bss_28;
u32 lbl_1_bss_2C[8];
s16 lbl_1_bss_4C;
s16 lbl_1_bss_4E[0x157];
OMOBJ *lbl_1_bss_2FC;
OMOBJ **lbl_1_bss_300;
OMOBJ **lbl_1_bss_304;
struct {
    u32 unk0;
    s16 unk4;
    s16 pad6;
} lbl_1_bss_308[0x20];
/* 4-byte hole: the original lbl_1_bss_308 symbol spans 0x104 bytes */
s32 lbl_1_bss_408;
s32 lbl_1_bss_40C;
s32 lbl_1_bss_410[3];
Vec lbl_1_bss_41C[4];
Vec lbl_1_bss_44C[4];
Vec lbl_1_bss_47C[4];
Vec lbl_1_bss_4AC[4];
Vec lbl_1_bss_4DC;
Vec lbl_1_bss_4E8;
Vec lbl_1_bss_4F4;
Vec lbl_1_bss_500;
OMOBJMAN *lbl_1_bss_50C;
M560NodeHead lbl_1_bss_510;

Vec lbl_1_data_0 = { 0.0f, 10000.0f, 10000.0f };
Vec lbl_1_data_C = { 0.0f, 0.0f, 0.0f };
GXColor lbl_1_data_18 = { 0xFF, 0xFF, 0xFF, 0xFF };
s32 lbl_1_data_1C = -1;
s32 lbl_1_data_20[8] = {
    0x8C0000, 0x8C0019, 0x8700BB, 0x8C0086, 0x8C0001, 0x8C002C, 0x8C0091, 0x8C0020,
};
u32 lbl_1_data_40[9] = {
    0x730010, 0x730011, 0x730012, 0x730013, 0x730014, 0x730015, 0x730016, 0x730017, 0x730018,
};
u32 lbl_1_data_64[7] = { 0x730006, 0x730007, 0x730008, 0x730009, 0x73000A, 0x73000B, 0 };
OMCAMERAVIEW lbl_1_data_80 = { { 0.0f, 150.0f, -1544.0f }, { -10.1f, 0.0f, 0.0f }, 1070.0f };
OMCAMERAVIEW lbl_1_data_9C = { { 0.0f, 334.7f, 0.0f }, { -5.4f, 0.0f, 0.0f }, 1174.0f };
OMCAMERAVIEW lbl_1_data_B8 = { { 553.4f, 334.7f, -2.2f }, { -13.1f, 0.0f, 0.0f }, 2294.0f };
Vec lbl_1_data_D4[4] = {
    { -400.0f, 20.0f, 0.0f },
    { -150.0f, 20.0f, 0.0f },
    { 150.0f, 20.0f, 0.0f },
    { 400.0f, 20.0f, 0.0f },
};
Vec lbl_1_data_104[4] = {
    { 1000.0f, 40.0f, 230.0f },
    { 1150.0f, 40.0f, 230.0f },
    { 1300.0f, 40.0f, 230.0f },
    { 1450.0f, 40.0f, 230.0f },
};
Vec lbl_1_data_134 = { 0.0f, 0.0f, -1400.0f };
Vec lbl_1_data_140 = { 0.0f, 5000.0f, 1.0f };
Vec lbl_1_data_14C = { 0.0f, 1.0f, 0.0f };
Vec lbl_1_data_158 = { 0.0f, 0.0f, 0.0f };
s32 lbl_1_data_164[7] = { 0x40, 0x20, 0x800, 0x400, 0x100, 0x200, 0x10 };
MGSEQ_PARAM lbl_1_data_180 = {
    0,
    MGSEQ_TIMER_TOP,
    (MGSEQ_FUNC)fn_1_1808,
    (MGSEQ_FUNC)fn_1_18F8,
    NULL,
    (MGSEQ_FUNC)fn_1_1B88,
    (MGSEQ_FUNC)fn_1_1C64,
    (MGSEQ_FUNC)fn_1_1D00,
    (MGSEQ_FUNC)fn_1_1E30,
    NULL,
    NULL,
};
s32 lbl_1_data_1A8[4] = { 0x984, 0x985, 0x986, 0x987 };
s32 lbl_1_data_1B8[4] = { 0x988, 0x989, 0x98A, 0x98B };
s32 lbl_1_data_1C8[4] = { 0x98C, 0x98D, 0x98E, 0x98F };
s32 lbl_1_data_1D8[4] = { 0x990, 0x991, 0x992, 0x993 };

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

void ObjectSetup(void)
{
    OMOBJ *obj;
    s32 i;

    OSReport("******* M560 ObjectSetup *********\n");
    lbl_1_bss_50C = omInitObjMan(0xC8, 0x2000);
    omGameSysInit(lbl_1_bss_50C);
    Hu3DCameraCreate(1);
    Hu3DCameraPerspectiveSet(1, 45.0f, 20.0f, 50000.0f, 1.2f);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    omCameraViewSet(&lbl_1_data_80);
    lbl_1_bss_2FC = omAddObjEx(lbl_1_bss_50C, 0x7FDA, 0, 0, -1, omOutView);
    lbl_1_bss_2FC->work[0] = 1;
    lbl_1_bss_4C = Hu3DGLightCreate(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    Hu3DGLightPosAimSetV(lbl_1_bss_4C, &lbl_1_data_0, &lbl_1_data_C);
    Hu3DGLightStaticSet(lbl_1_bss_4C, 1);
    Hu3DGLightInfinitytSet(lbl_1_bss_4C);
    Hu3DGLightColorSet(lbl_1_bss_4C, lbl_1_data_18.r, lbl_1_data_18.g, lbl_1_data_18.b, lbl_1_data_18.a);
    Hu3DShadowCreate(45.0f, 2000.0f, 25000.0f);
    Hu3DShadowPosSet(&lbl_1_data_140, &lbl_1_data_14C, &lbl_1_data_158);
    Hu3DShadowTPLvlSet(0.3f);
    omMakeGroupEx(lbl_1_bss_50C, 0, 4);
    lbl_1_bss_304 = omGetGroupMemberListEx(lbl_1_bss_50C, 0);

    for (i = 0; i < 4; i++) {
        obj = omAddObjEx(lbl_1_bss_50C, 0x64, 1, 8, 0, fn_1_4B0);
        lbl_1_bss_304[i] = obj;
        obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M560Player), HU_MEMNUM_OVL);
        obj->work[0] = i;
    }

    omMakeGroupEx(lbl_1_bss_50C, 2, 1);
    lbl_1_bss_300 = omGetGroupMemberListEx(lbl_1_bss_50C, 2);
    obj = omAddObjEx(lbl_1_bss_50C, 0x66, 1, 9, 2, fn_1_24B8);
    *lbl_1_bss_300 = obj;
    obj->data = HuMemDirectMallocNum(HEAP_HEAP, 0x3C, HU_MEMNUM_OVL);
    lbl_1_bss_0 = HuSprAnimRead(HuDataReadNum(0x730019, HU_MEMNUM_OVL));
    HuSprAnimLock(lbl_1_bss_0);
    fn_1_14E4();
    lbl_1_bss_28 = 0;
    fn_1_381C();
    fn_1_668C();
    fn_1_7008();
    MgSeqCreatePrio(&lbl_1_data_180, 0x3E8);
}

void fn_1_4B0(OMOBJ *obj)
{
    s32 i;
    M560Player *work;

    work = obj->data;
    work->playerNo = obj->work[0];
    work->charNo = GwPlayerConf[work->playerNo].charNo;
    work->padNo = GwPlayerConf[work->playerNo].padNo;
    work->state = 0;
    work->timer = 0;
    obj->mdlId[0] = CharModelCreate(work->charNo, 2);

    for (i = 0; i < 8; i++) {
        obj->mtnId[i] = CharMotionCreate(work->charNo, lbl_1_data_20[i]);
        CharMotionSet(work->charNo, obj->mtnId[i]);
    }

    work->motNo = 1;
    Hu3DModelAttrSet(obj->mdlId[0], 1);
    Hu3DModelAttrSet(obj->mdlId[0], 0x40000001);
    Hu3DMotionSet(obj->mdlId[0], obj->mtnId[1]);
    Hu3DModelShadowSet(obj->mdlId[0]);
    work->curItem = -1;
    work->itemCnt = 0;
    work->curItem = lbl_1_bss_2C[0];
    work->unk20 = 0;
    work->done = 0;
    work->unk6C = 0;
    work->comTimer = 0;
    work->dif = GwPlayerConf[work->playerNo].dif;

    switch (work->dif) {
        case 0:
            work->reactRange = 5;
            work->comWait = frandmod(5) + 0x23;
            break;
        case 1:
            work->reactRange = 8;
            work->comWait = frandmod(5) + 0x23;
            break;
        case 2:
            work->reactRange = 0xF;
            work->comWait = frandmod(5) + 0x19;
            break;
        case 3:
            work->reactRange = 0x14;
            work->comWait = frandmod(5) + 0x14;
            break;
    }

    work->rot.x = work->rot.y = work->rot.z = 0.0f;
    work->pos.x = lbl_1_data_D4[work->playerNo].x;
    work->pos.y = lbl_1_data_D4[work->playerNo].y;
    work->pos.z = lbl_1_data_D4[work->playerNo].z;
    omSetTra(obj, work->pos.x, work->pos.y, work->pos.z);
    omSetRot(obj, work->rot.x, work->rot.y, work->rot.z);
    work->mdl70 = Hu3DModelCreateData(0x730003);
    work->mdl72 = Hu3DModelCreateData(0x730004);
    work->mdl74 = Hu3DModelCreateData(0x730005);
    Hu3DModelAttrSet(work->mdl70, 1);
    Hu3DModelAttrSet(work->mdl70, 0x40000002);
    Hu3DModelAttrSet(work->mdl72, 1);
    Hu3DModelAttrSet(work->mdl72, 0x40000002);
    Hu3DModelAttrSet(work->mdl74, 1);
    Hu3DModelAttrSet(work->mdl74, 0x40000002);
    work->mdl84 = Hu3DModelCreateData(0x73000C);
    work->mdl86 = Hu3DModelCreateData(0x73000D);
    Hu3DMotionTimeSet(work->mdl84, 0.0f);
    Hu3DMotionTimeSet(work->mdl86, 0.0f);
    Hu3DModelAttrSet(work->mdl84, 1);
    Hu3DModelAttrSet(work->mdl84, 0x40000002);
    Hu3DModelAttrSet(work->mdl86, 1);
    Hu3DModelAttrSet(work->mdl86, 0x40000002);
    Hu3DModelHookSet(work->mdl70, "itemhook2", work->mdl74);
    Hu3DModelPosSet(work->mdl70, lbl_1_data_D4[work->playerNo].x, 0.0f, lbl_1_data_D4[work->playerNo].z);
    Hu3DModelHookSet(work->mdl74, "itemhook_lamp", work->mdl72);

    for (i = 0; i < 7; i++) {
        work->itemMdl[i] = Hu3DModelCreateData(lbl_1_data_64[i]);
        Hu3DModelAttrSet(work->itemMdl[i], 1);
    }

    Hu3DModelAttrReset(work->mdl70, 1);
    Hu3DModelAttrReset(work->mdl72, 1);
    Hu3DModelAttrReset(work->mdl74, 1);
    Hu3DModelAttrReset(obj->mdlId[0], 1);
    CharMotionDataClose(work->charNo);
    obj->objFunc = fn_1_9F8;
}

void fn_1_9F8(OMOBJ *obj)
{
    Vec pos2d;
    M560Player *work;
    M560Game *game;
    s32 ret;
    f32 pan;

    work = obj->data;
    game = (*lbl_1_bss_300)->data;
    work->prevPos = work->pos;

    switch (work->state) {
        case 1:
            if (game->state == 1 || game->state == 4) {
                ret = fn_1_12B4(work);
                if (ret == 1) {
                    Hu3DModelHookObjReset(work->mdl74, "itemhook_fx");
                    Hu3DModelAttrSet(work->mdl86, 0x40000002);
                    Hu3DModelAttrSet(work->mdl86, 1);
                    Hu3DMotionTimeSet(work->mdl86, 0.0f);
                    Hu3DMotionSpeedSet(work->mdl86, 0.0f);
                    Hu3DModelAttrReset(work->mdl84, 0x40000002);
                    Hu3DModelAttrReset(work->mdl84, 1);
                    Hu3DMotionTimeSet(work->mdl84, 0.0f);
                    Hu3DMotionSpeedSet(work->mdl84, 1.0f);
                    Hu3DMotionCalc(work->mdl84);
                    Hu3DModelHookSet(work->mdl74, "itemhook_fx", work->mdl84);
                    fn_1_15D8(work, 1);
                    HuAudFXPlay(lbl_1_data_1B8[work->playerNo]);
                } else if (ret == 2) {
                    omVibrate(work->playerNo, 0x14, 4, 2);
                    Hu3DModelHookObjReset(work->mdl74, "itemhook_fx");
                    Hu3DModelAttrSet(work->mdl84, 0x40000002);
                    Hu3DModelAttrSet(work->mdl84, 1);
                    Hu3DMotionTimeSet(work->mdl84, 0.0f);
                    Hu3DMotionSpeedSet(work->mdl84, 0.0f);
                    Hu3DModelAttrReset(work->mdl86, 1);
                    Hu3DModelAttrReset(work->mdl86, 0x40000002);
                    Hu3DMotionTimeSet(work->mdl86, 0.0f);
                    Hu3DMotionSpeedSet(work->mdl86, 1.0f);
                    Hu3DMotionCalc(work->mdl86);
                    Hu3DModelHookSet(work->mdl74, "itemhook_fx", work->mdl86);
                    fn_1_15D8(work, 0);
                    work->state = 2;
                    work->timer = 0;
                    HuAudFXPlay(lbl_1_data_1C8[work->playerNo]);
                }
            }
            break;
        case 2:
            if (work->timer > 0x1E) {
                work->timer = 0;
                work->state = 1;
            } else {
                work->timer++;
            }
            break;
        case 4:
            if (work->timer > 0xF) {
                work->timer = 0;
                work->state = 5;
            } else {
                work->timer++;
            }
            break;
        case 5:
            fn_1_1F38(obj, 5, 8.0f, 0x40000001);
            if (work->pos.z > 150.0f) {
                work->state = 6;
                work->rot.y = 90.0f;
            } else {
                work->pos.z += 5.0f;
                work->pos.y = 20.0f - 0.1875f * work->pos.z;
            }
            break;
        case 6:
            if (work->pos.x > 700.0f) {
                work->pos.x = lbl_1_data_104[lbl_1_bss_28].x;
                work->pos.y = lbl_1_data_104[lbl_1_bss_28].y;
                work->pos.z = lbl_1_data_104[lbl_1_bss_28].z;
                work->rot.y = 0.0f;
                lbl_1_bss_28++;
                work->state = 7;
                fn_1_1F38(obj, 0, 8.0f, 0x40000001);
            } else {
                work->pos.x += 10.0f;
            }
            break;
        case 8:
            if (work->timer < 0xF) {
                fn_1_FE4(obj);
            }
            fn_1_1F38(obj, 7, 8.0f, 0);
            if (work->timer > 0x2D) {
                work->state = 9;
                work->timer = 0;
            } else {
                work->timer++;
            }
            break;
        case 9:
            fn_1_1F38(obj, 0, 8.0f, 0x40000001);
            if (work->timer == 0) {
                Hu3DMotionOverlaySet(obj->mdlId[0], obj->mtnId[6]);
            }
            work->timer++;
            break;
    }

    omSetTra(obj, work->pos.x, work->pos.y, work->pos.z);
    omSetRot(obj, work->rot.x, work->rot.y, work->rot.z);

    if (work->state == 5 || work->state == 5) {
        fn_1_354C(obj);
    }

    Hu3D3Dto2D(&work->pos, 1, &pos2d);
    pan = pos2d.x / 576.0f;
    CharModelVoicePanSet(work->charNo, (s32)(32.0f + pan * 64.0f));
}

void fn_1_FE4(OMOBJ *obj)
{
    s32 i;
    s32 j;
    s32 flag;
    M560Player *work;
    HU3DMODEL *model;
    HSFDATA *hsf;
    HSFMATERIAL *mat;
    HSFATTRIBUTE *attr;
    char **eyeBmp;

    work = obj->data;
    model = &Hu3DData[obj->mdlId[0]];
    hsf = model->hsf;
    mat = hsf->material;

    if (work->charNo == 3) {
        for (i = 0; i < model->hsf->materialNum; i++, mat++) {
            if (i != 1 && i != 2) {
                mat->color[0] = (u8)(s32)((f32)mat->color[0] * 0.9f);
                mat->color[1] = (u8)(s32)((f32)mat->color[1] * 0.9f);
                mat->color[2] = (u8)(s32)((f32)mat->color[2] * 0.9f);
            }
        }
        return;
    }

    eyeBmp = CharModelEyeBmpGet(work->charNo, 2);

    for (i = 0; i < hsf->materialNum; i++, mat++) {
        flag = 1;

        for (j = 0; j < mat->attrNum; j++) {
            attr = &hsf->attribute[mat->attr[j]];
            if (strcmp(eyeBmp[0], attr->bitmap->name) == 0 || strcmp(eyeBmp[1], attr->bitmap->name) == 0) {
                flag = 0;
            }
        }

        if (flag != 0) {
            mat->color[0] = (u8)(s32)((f32)mat->color[0] * 0.9f);
            mat->color[1] = (u8)(s32)((f32)mat->color[1] * 0.9f);
            mat->color[2] = (u8)(s32)((f32)mat->color[2] * 0.9f);
        }
    }
}

s32 fn_1_12B4(M560Player *work)
{
    s32 ret;
    u16 target;
    u16 input;

    ret = 0;

    if (GwPlayerConf[work->playerNo].type != 0) {
        input = fn_1_143C(work);
    } else {
        input = HuPadBtnDown[work->padNo] & 0xF60;
    }

    target = lbl_1_data_164[lbl_1_bss_2C[work->itemCnt]];

    if (input != 0) {
        work->motNo = 2;
        Hu3DModelAttrReset(lbl_1_bss_304[work->playerNo]->mdlId[0], 0x40000002);
        CharMotionShiftSet(work->charNo, lbl_1_bss_304[work->playerNo]->mtnId[work->motNo], 0.0f, 4.0f, 0);
        ret = 2;
    }

    if ((input & target) != 0 && (input & ~target) == 0) {
        ret = 1;
    }

    return ret;
}

u16 fn_1_143C(M560Player *work)
{
    u16 ret;

    if (work->comTimer > work->comWait) {
        work->comTimer = 0;
        if (frandmod(work->reactRange) == 0) {
            ret = 0x10;
        } else {
            ret = lbl_1_data_164[lbl_1_bss_2C[work->itemCnt]];
        }
    } else {
        work->comTimer++;
        ret = 0;
    }

    return ret;
}

void fn_1_14E4(void)
{
    s32 i;
    s32 j;
    s32 cnt;

    for (i = 0; i < 8;) {
        lbl_1_bss_2C[i] = frandmod(6);

        if (i <= 0 || lbl_1_bss_2C[i] != lbl_1_bss_2C[i - 1]) {
            j = 0;
            cnt = 0;

            for (; j < i; j++) {
                if (lbl_1_bss_2C[i] == lbl_1_bss_2C[j]) {
                    cnt++;
                }
            }

            if (cnt < 2) {
                i++;
            }
        }
    }
}

void fn_1_15D8(M560Player *work, s32 arg1)
{
    if (arg1 != 0 && work->itemCnt < 8) {
        Hu3DModelAttrSet(work->itemMdl[work->curItem], 1);
        Hu3DModelHookObjReset(work->mdl74, "itemhook");
        work->itemCnt++;

        if (work->itemCnt == 8) {
            work->state = 4;
            work->timer = 0;
            Hu3DModelAttrReset(work->mdl70, 0x40000002);
            Hu3DMotionSpeedSet(work->mdl70, 2.0f);
            Hu3DMotionTimeSet(work->mdl72, 8.0f);
            work->done = 1;
            omVibrate(work->playerNo, 0xF, 4, 2);
            HuAudFXPlay(lbl_1_data_1D8[work->playerNo]);
            return;
        }

        work->curItem = lbl_1_bss_2C[work->itemCnt];
        Hu3DMotionTimeSet(work->mdl72, (f32)work->itemCnt);
        Hu3DModelHookSet(work->mdl74, "itemhook", work->itemMdl[work->curItem]);
        Hu3DModelAttrReset(work->itemMdl[work->curItem], 1);
    }
}

void fn_1_1768(M560Player *work)
{
    Hu3DModelAttrSet(work->itemMdl[work->curItem], 1);
    Hu3DModelHookObjReset(work->mdl74, "itemhook");
    Hu3DModelHookObjReset(work->mdl74, "itemhook_fx");
    Hu3DMotionTimeSet(work->mdl84, 0.0f);
    Hu3DMotionTimeSet(work->mdl86, 0.0f);
    Hu3DModelAttrSet(work->mdl84, 1);
    Hu3DModelAttrSet(work->mdl86, 1);
}

void fn_1_1808(void)
{
    lbl_1_bss_4E[0] = Hu3DModelCreateData(0x730000);
    lbl_1_bss_4E[1] = Hu3DModelCreateData(0x730001);
    lbl_1_bss_4E[2] = Hu3DModelCreateData(0x730002);
    lbl_1_bss_4E[3] = Hu3DModelCreateData(0x73000E);
    Hu3DModelShadowMapSet(lbl_1_bss_4E[0]);
    Hu3DModelAttrSet(lbl_1_bss_4E[3], 0x40000001);
    Hu3DModelAttrSet(lbl_1_bss_4E[2], 0x40000001);
    fn_1_3830();
    MgSeqModeNext();
}

void fn_1_18F8(void)
{
    s32 i;
    s16 pan;
    M560Game *game;
    M560Player *work;
    Vec pos = { 80.0f, 0.0f, -1150.0f };

    game = (*lbl_1_bss_300)->data;

    for (i = 0; i < 0x3C; i++) {
        HuPrcVSleep();
    }

    game->state = 2;
    HuAudFXPlay(0x2C9);

    for (i = 0; i < 0x32; i++) {
        HuPrcVSleep();
    }

    fn_1_6770(1, &pos);
    fn_1_3890(0x3C, 0, 25.0f, 0.85f, 90.0f);
    pan = fn_1_3A58(&game->pos);
    HuAudFXPlayVolPan(0x2C7, fn_1_3AD4(&game->pos), pan);

    for (i = 0; i < 4; i++) {
        work = lbl_1_bss_304[i]->data;
        omVibrate(work->playerNo, 0xF, 4, 2);
    }

    for (i = 0; i < 0x46; i++) {
        HuPrcVSleep();
    }

    omCameraViewMoveSimple(&lbl_1_data_9C, 0x78);

    for (i = 0; i < 0x6E; i++) {
        HuPrcVSleep();
    }

    for (i = 0; i < 4; i++) {
        work = lbl_1_bss_304[i]->data;
        Hu3DModelAttrReset(work->mdl74, 0x40000002);
        Hu3DMotionSpeedSet(work->mdl74, 3.0f);
        HuAudFXPlay(lbl_1_data_1A8[i]);
    }

    for (i = 0; i < 0xF; i++) {
        HuPrcVSleep();
    }

    for (i = 0; i < 4; i++) {
        work = lbl_1_bss_304[i]->data;
        fn_1_1F38(lbl_1_bss_304[work->playerNo], 2, 8.0f, 0x40000002);
    }

    for (i = 0; i < 0x2D; i++) {
        HuPrcVSleep();
    }

    MgSeqModeNext();
}

void fn_1_1B88(void)
{
    M560Player *work;
    M560Game *game;
    s32 i;

    game = (*lbl_1_bss_300)->data;

    if (MgSeqFrameNoGet() == 0) {
        game->state = 1;

        for (i = 0; i < 4; i++) {
            work = lbl_1_bss_304[i]->data;
            work->state = 1;
            Hu3DModelHookSet(work->mdl74, "itemhook", work->itemMdl[work->curItem]);
            Hu3DModelAttrReset(work->itemMdl[work->curItem], 1);
        }
    }

    if (game->state == 6) {
        MgSeqModeNext();
    }

    fn_1_20F4();
}

void fn_1_1C64(void)
{
    s32 i;
    M560Player *work;

    if (MgSeqFrameNoGet() == 0) {
        fn_1_3860();

        for (i = 0; i < 4; i++) {
            work = lbl_1_bss_304[i]->data;
            fn_1_1F38(lbl_1_bss_304[work->playerNo], 0, 8.0f, 0x40000001);
        }
    }
}

void fn_1_1D00(void)
{
    s32 i;
    M560Player *work;
    f32 fov = 45.0f;

    for (i = 0; i < 0x1E; i++) {
        HuPrcVSleep();
    }

    if (fn_1_1FE4() != 0) {
        omCameraViewMoveSimple(&lbl_1_data_B8, 0x5A);

        for (i = 0; i < 4; i++) {
            work = lbl_1_bss_304[i]->data;
            if (work->done == 0) {
                Hu3DModelAttrSet(work->mdl74, 0x40000004);
                Hu3DMotionSpeedSet(work->mdl74, 1.0f);
            }
        }

        for (i = 0; i < 0x5A; i++) {
            Hu3DCameraPerspectiveSet(1, fov - 0.2777778f, 20.0f, 50000.0f, 1.2f);
            HuPrcVSleep();
        }
    }

    MgSeqModeNext();
}

void fn_1_1E30(void)
{
    M560Player *work;
    s32 i;
    M560Game *game;
    s32 playerNo;

    game = (*lbl_1_bss_300)->data;

    if (MgSeqFrameNoGet() == 0) {
        if (fn_1_1FE4() != 0) {
            for (i = 0; i < 4; i++) {
                work = lbl_1_bss_304[i]->data;
                if (work->done != 0) {
                    fn_1_1F38(lbl_1_bss_304[work->playerNo], 3, 8.0f, 0);
                    playerNo = work->playerNo;
                    GWMgCoinBonusSet(playerNo, 10);
                }
            }
        }

        game->state = 7;
    }
}

void fn_1_1F38(OMOBJ *obj, s32 motNo, f32 time, u32 attr)
{
    M560Player *work = obj->data;

    if (work->motNo != motNo) {
        work->motNo = motNo;
        CharMotionShiftSet(work->charNo, obj->mtnId[work->motNo], 0.0f, time, attr);
        if (motNo == 7) {
            omVibrate(work->playerNo, 0x1E, 0xC, 0);
        }
    }
}

s32 fn_1_1FE4(void)
{
    s32 ret;
    s32 i;
    M560Player *work;

    ret = 0;

    for (i = 0; i < 4; i++) {
        work = lbl_1_bss_304[i]->data;
        if (work->done != 0) {
            ret = 1;
            return ret;
        }
    }

    return ret;
}

s32 fn_1_2054(void)
{
    s32 ret;
    s32 i;
    M560Player *players[4];

    ret = 0;

    for (i = 0; i < 4; i++) {
        players[i] = lbl_1_bss_304[i]->data;
    }

    if (players[0]->done != 0 && players[1]->done != 0 && players[2]->done != 0 && players[3]->done != 0) {
        return 1;
    }

    return 0;
}

void fn_1_20F4(void)
{
}

void fn_1_20F8(void)
{
    GXColor color;

    color.r = 0;
    color.g = 0;
    color.b = 0x90;
    color.a = 0xC0;
    CZoom += (f32)(HuPadTrigL[0] / 5);
    CZoom -= (f32)(HuPadTrigR[0] / 5);
    Center.x += 10.0f * ((f32)HuPadSubStkX[0] / 59.0f);
    Center.z = Center.z - (10.0f * ((f32)HuPadSubStkY[0] / 59.0f));
    CRot.x += 5.0f * ((f32)HuPadSubStkY[1] / 59.0f);
    CRot.y = CRot.y - (5.0f * ((f32)HuPadSubStkX[1] / 59.0f));
    printWin(0x10, 0x28, 0x180, 0x24, &color);
    fontcolor = 0xE;
    print8(0x10, 0x28, 1.5f, "Center X:%.02f Y:%.02f Z:%.02f", Center.x, Center.y, Center.z);
    print8(0x10, 0x34, 1.5f, "CRot X:%.02f Y:%.02f Z:%.02f", CRot.x, CRot.y, CRot.z);
    print8(0x10, 0x40, 1.5f, "CZoom:%.02f", CZoom);

    if ((HuPadBtnDown[0] & 0x400) != 0) {
        omCameraViewSet(&lbl_1_data_9C);
    }
}

void fn_1_24B8(OMOBJ *obj)
{
    s32 i;
    M560Game *game;

    game = obj->data;
    obj->mdlId[0] = Hu3DModelCreateData(0x73000F);

    for (i = 0; i < 9; i++) {
        obj->mtnId[i] = Hu3DJointMotion(obj->mdlId[0], HuDataSelHeapReadNum(lbl_1_data_40[i], HU_MEMNUM_OVL, HEAP_MODEL));
    }

    game->motNo = 9;
    fn_1_3468(obj, 2, 0.0f, 0x40000001);
    Hu3DModelShadowSet(obj->mdlId[0]);
    game->mdl = obj->mdlId[0];
    game->timer = 0;
    game->particleMdl = -1;
    game->pos.x = lbl_1_data_134.x;
    game->pos.y = lbl_1_data_134.y;
    game->pos.z = lbl_1_data_134.z;
    game->state = 0;
    game->prevTime = game->speed = game->unk24 = game->unk20 = game->rot.z = game->rot.y = game->rot.x = 0.0f;
    omSetRot(obj, game->rot.x, game->rot.y, game->rot.z);
    omSetSca(obj, 1.5f, 1.5f, 1.5f);
    omSetTra(obj, game->pos.x, game->pos.y, game->pos.z);
    obj->objFunc = fn_1_268C;
}

void fn_1_268C(OMOBJ *obj)
{
    Vec posL = { -80.0f, 0.0f, 70.0f };
    Vec posR = { 80.0f, 0.0f, 70.0f };
    Mtx rotMtx;
    Mtx mtxA;
    Mtx mtxB;
    Vec out;
    Vec headPos;
    Vec handPos;
    Vec diff;
    Vec pan;
    GXColor color;
    M560Player *players[4];
    s32 stepFlag;
    s32 i;
    s32 allDone;
    s32 freed;
    s32 n;
    s16 k;
    s16 mdlId;
    f32 time;
    f32 ang;
    s16 panVal;
    M560Game *game;
    M560Player *work;
    HU3DMODEL *hModel;
    M560Parm *parm;
    M560Cell *cellP;
    M560Cell *cellQ;

    stepFlag = 0;
    game = obj->data;

    switch (game->state) {
        case 2:
            fn_1_3468(*lbl_1_bss_300, 3, 8.0f, 0);
            if (game->timer > 0x78) {
                game->timer = 0;
                game->state = 3;
                fn_1_3468(obj, 0, 8.0f, 0x40000001);
            } else {
                game->timer++;
            }
            break;
        case 3:
            if (game->timer > 8) {
                game->timer = 0;
                game->state = 0;
            } else {
                if (game->timer >= 0 && game->timer < 8) {
                    game->pos.z += 16.25f;
                }
                game->timer++;
            }
            break;
        case 4:
            fn_1_3468(obj, 0, 8.0f, 0x40000001);
            if (game->timer > 0x3C) {
                game->timer = 0;
                game->state = 1;
            } else {
                if (game->timer == 0) {
                    game->speed = (f32)fabs(game->pos.z - -600.0f) / 342.0f;
                }
                game->timer++;
            }
            break;
        case 1:
            if (game->particleMdl >= 0) {
                mdlId = game->particleMdl;
                hModel = &Hu3DData[mdlId];
                parm = hModel->hookData;
                parm->kill = 1;
            }
            Hu3DMotionSpeedSet(obj->mdlId[0], 0.8f);
            fn_1_3468(obj, 8, 8.0f, 0x40000001);
            time = Hu3DMotionTimeGet(obj->mdlId[0]);
            if ((f32)game->timer > 342.0f) {
                game->timer = 0;
                game->state = 5;
                game->pos.z = -600.0f;
                fn_1_3468(obj, 4, 8.0f, 0);

                for (i = 0; i < 4; i++) {
                    work = lbl_1_bss_304[i]->data;
                    Hu3DModelAttrSet(work->itemMdl[work->curItem], 1);
                    Hu3DModelHookObjReset(work->mdl74, "itemhook");
                    Hu3DModelHookObjReset(work->mdl74, "itemhook_fx");
                    Hu3DMotionTimeSet(work->mdl84, 0.0f);
                    Hu3DMotionTimeSet(work->mdl86, 0.0f);
                    Hu3DModelAttrSet(work->mdl84, 1);
                    Hu3DModelAttrSet(work->mdl86, 1);
                }
            } else {
                if (game->timer == 0) {
                    game->speed = (f32)fabs(game->pos.z - -600.0f) / 342.0f;
                }
                game->pos.z = game->pos.z + game->speed;
                if (game->prevTime < 10.0f && time >= 10.0f) {
                    if (game->motNo == 8) {
                        fn_1_3890(0x28, 1, 10.0f, 0.95f, 90.0f);
                        panVal = fn_1_3A58(&game->pos);
                        HuAudFXPlayVolPan(0x2C7, fn_1_3B50(), panVal);
                    }
                    PSMTXRotRad(rotMtx, 'Y', 0.017453292f * game->rot.y);
                    PSMTXMultVec(rotMtx, &posL, &out);
                    PSVECAdd(&out, &game->pos, &out);
                    fn_1_6770(1, &out);
                } else if (game->prevTime < 60.0f) {
                    if (time >= 60.0f) {
                        if (game->motNo == 8) {
                            fn_1_3890(0x28, 1, 10.0f, 0.95f, 90.0f);
                            panVal = fn_1_3A58(&game->pos);
                            HuAudFXPlayVolPan(0x2C7, fn_1_3B50(), panVal);
                        }
                        PSMTXRotRad(rotMtx, 'Y', 0.017453292f * game->rot.y);
                        PSMTXMultVec(rotMtx, &posR, &out);
                        PSVECAdd(&out, &game->pos, &out);
                        fn_1_6770(1, &out);
                    }
                }
                game->timer++;
            }
            game->prevTime = time;
            break;
        case 5:
            if (game->timer == 0) {
                mdlId = fn_1_5A9C(lbl_1_bss_0, NULL, NULL, 0x200, 0x40, 0x40, 40.0f);
                hModel = &Hu3DData[mdlId];
                parm = hModel->hookData;
                parm->hook = fn_1_4C34;
                hModel = &Hu3DData[mdlId];
                parm = hModel->hookData;
                parm->blend = 0;
                Hu3DModelLayerSet(mdlId, 5);
                game->particleMdl = mdlId;
            }
            Hu3DMotionSpeedSet(obj->mdlId[0], 1.25f);
            if (game->timer > 0x2D) {
                stepFlag = 1;
                if (lbl_1_data_1C < 0) {
                    lbl_1_data_1C = HuAudFXPlayPan(0x97B, fn_1_3A58(&game->pos));
                }
            } else {
                stepFlag = 0;
            }
            game->timer++;
            if (game->timer > 0xAA) {
                game->state = 6;
                game->timer = 0;
                Hu3DMotionSpeedSet(obj->mdlId[0], 1.0f);
                if (lbl_1_data_1C > 0) {
                    HuAudFXStop(lbl_1_data_1C);
                    lbl_1_data_1C = -1;
                }
            }
            break;
        case 6:
            fn_1_3468(obj, 0, 8.0f, 0x40000001);
            Hu3DMotionSpeedSet(obj->mdlId[0], 1.0f);
            game->timer++;
            if (game->timer > 0x3C) {
                game->state = 0;
                game->timer = 0;
            }
            break;
        case 7:
            for (i = 0; i < 4; i++) {
                players[i] = lbl_1_bss_304[i]->data;
            }
            if (players[0]->done != 0 && players[1]->done != 0 && players[2]->done != 0 && players[3]->done != 0) {
                allDone = 1;
            } else {
                allDone = 0;
            }
            if (allDone != 0) {
                fn_1_3468(obj, 6, 8.0f, 0);
            } else {
                fn_1_3468(obj, 5, 8.0f, 0);
            }
            break;
    }

    if (game->particleMdl >= 0) {
        if (stepFlag != 0) {
            Hu3DModelObjMtxGet(obj->mdlId[0], "g000m0-itemhook_M", mtxA);
            handPos.x = mtxA[0][3];
            handPos.y = mtxA[1][3];
            handPos.z = mtxA[2][3];
            Hu3DModelObjMtxGet(obj->mdlId[0], "g000m0-ns_hed", mtxB);
            headPos.x = mtxB[0][3];
            headPos.y = mtxB[1][3];
            headPos.z = mtxB[2][3];
            PSVECSubtract(&handPos, &headPos, &diff);
            ang = (f32)(180.0 * (atan2(diff.x, diff.z) / 3.141592653589793));
            mdlId = game->particleMdl;
            color.r = 0xFF;
            color.g = 0xFF;
            color.b = 0xFF;
            color.a = 0xFF;

            for (n = 0; n < 1; n++) {
                hModel = &Hu3DData[mdlId];
                parm = hModel->hookData;
                if (parm->kill != 0) {
                    cellP = NULL;
                } else {
                    k = 0;
                    cellQ = parm->cells;

                    for (; k < parm->cellCnt; k++, cellQ++) {
                        if (cellQ->idx < 0) {
                            break;
                        }
                    }

                    if (k == parm->cellCnt) {
                        cellP = NULL;
                    } else {
                        parm->color[k].r = color.r;
                        parm->color[k].g = color.g;
                        parm->color[k].b = color.b;
                        parm->color[k].a = color.a;
                        cellQ->pos.x = handPos.x;
                        cellQ->pos.y = handPos.y;
                        cellQ->pos.z = handPos.z;
                        cellQ->scale = 0.0f;
                        cellQ->rot = 0.0f;
                        cellQ->frame = 0;
                        cellQ->unk0 = 0;
                        cellQ->idx = k;
                        cellP = cellQ;
                    }
                }
                if (cellP == NULL) {
                    break;
                }
                cellP->org.x = handPos.x;
                cellP->org.y = handPos.y;
                cellP->org.z = handPos.z;
                cellP->dist = 0.0f;
                cellP->ang = ang;
                cellP->speed = 10.0f;
                cellP->maxDist = 750.0f;
                cellP->targetY = 50.0f;
                cellP->angOfs = (20.0f * frandf()) - 10.0f;
                cellP->speed = cellP->speed * (0.8f + (0.4f * frandf()));
                cellP->maxDist = cellP->maxDist * (0.9f + (0.3f * frandf()));
            }

            if (lbl_1_data_1C > 0) {
                pan.x = (f32)(750.0 * sin((3.141592653589793 * (f64)ang) / 180.0));
                pan.y = 0.0f;
                pan.z = (f32)(750.0 * cos((3.141592653589793 * (f64)ang) / 180.0));
                HuAudFXPanning(lbl_1_data_1C, fn_1_3A58(&pan));
            }
        }

        mdlId = game->particleMdl;
        hModel = &Hu3DData[mdlId];
        parm = hModel->hookData;
        if (parm->kill != 0 && parm->activeCnt == 0) {
            HuMemDirectFree(parm->cells);
            HuMemDirectFree(parm->vtx);
            HuMemDirectFree(parm->color);
            HuMemDirectFree(parm->uv);
            HuMemDirectFree(parm->uvTbl);
            HuMemDirectFree(parm->dl);
            HuSprAnimKill(parm->anim);
            HuMemDirectFree(hModel->hookData);
            Hu3DModelKill(mdlId);
            freed = 1;
        } else {
            freed = 0;
        }
        if (freed != 0) {
            game->particleMdl = -1;
        }
    }

    fn_1_38EC();
    omSetTra(obj, game->pos.x, game->pos.y, game->pos.z);
    omSetRot(obj, game->rot.x, game->rot.y, game->rot.z);
    MgSeqFrameNoGet();
}

void fn_1_3468(OMOBJ *obj, s32 motNo, f32 time, u32 attr)
{
    M560Game *game = obj->data;

    if (game->motNo != motNo) {
        game->motNo = motNo;
        Hu3DMotionShiftSet(obj->mdlId[0], obj->mtnId[motNo], 0.0f, time, attr);
        game->prevTime = Hu3DMotionTimeGet(obj->mdlId[0]);
        if (game->motNo == 5) {
            HuAudFXPlayPan(0x2C9, fn_1_3A58(&game->pos));
            HuAudJinglePlay(0x7A);
        }
        if (game->motNo == 6) {
            HuAudFXPlayPan(0x2CA, fn_1_3A58(&game->pos));
            HuAudJinglePlay(0x7B);
        }
    }
}

void fn_1_354C(OMOBJ *obj)
{
    Vec delta;
    Vec dir;
    Vec posA;
    Vec posB;
    f32 magA;
    f32 magB;
    f32 dist;
    f32 push;
    f32 ratio;
    s32 i;
    M560Player *work;
    M560Player *other;

    work = obj->data;
    PSVECSubtract(&work->pos, &work->prevPos, &delta);
    magA = PSVECMag(&delta);

    if (magA != 0.0f) {
        for (i = 0; i < 4; i++) {
            if (i != work->playerNo) {
                other = lbl_1_bss_304[i]->data;
                posA.x = work->pos.x;
                posA.y = work->pos.y;
                posA.z = work->pos.z;
                posA.y = 0.0f;
                posB.x = other->pos.x;
                posB.y = other->pos.y;
                posB.z = other->pos.z;
                posB.y = 0.0f;
                PSVECSubtract(&posA, &posB, &delta);
                dist = PSVECMag(&delta);
                if (dist < 159.2f) {
                    PSVECSubtract(&other->pos, &other->prevPos, &delta);
                    magB = PSVECMag(&delta);
                    PSVECSubtract(&posA, &posB, &dir);
                    PSVECNormalize(&dir, &dir);
                    push = 160.0f - dist;
                    ratio = ((0.6f * magB) + (0.4f * magA)) / (magA + magB);
                    work->pos.x = work->pos.x + (ratio * (push * dir.x));
                    work->pos.y = work->pos.y + (ratio * (push * dir.y));
                    work->pos.z = work->pos.z + (ratio * (push * dir.z));
                    if (work->state == 6) {
                        ratio = ((0.6f * magA) + (0.4f * magA)) / (magA + magB);
                        other->pos.x = other->pos.x + (ratio * (push * -dir.x));
                        other->pos.y = other->pos.y + (ratio * (push * -dir.y));
                        other->pos.z = other->pos.z + (ratio * (push * -dir.z));
                    }
                    fn_1_354C(lbl_1_bss_304[i]);
                }
            }
        }
    }
}

void fn_1_381C(void)
{
    lbl_1_bss_24 = -1;
}

void fn_1_3830(void)
{
    lbl_1_bss_24 = HuAudSStreamPlay(0x3F);
}

void fn_1_3860(void)
{
    HuAudSStreamFadeOut(lbl_1_bss_24, 0x64);
}

void fn_1_3890(u16 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4)
{
    lbl_1_bss_20 = arg0;
    lbl_1_bss_18[0] = arg2;
    lbl_1_bss_14 = arg3;
    lbl_1_bss_8 = arg4;
    lbl_1_bss_10 = 90.0f;
    lbl_1_bss_C = arg1;
}

void fn_1_38EC(void)
{
    lbl_1_bss_4 = 0.0f;

    if (lbl_1_bss_20 > 0) {
        lbl_1_bss_4 = (f32)((f64)lbl_1_bss_18[0] * sin((3.141592653589793 * (f64)lbl_1_bss_10) / 180.0));
        lbl_1_bss_10 += lbl_1_bss_8;
        lbl_1_bss_18[0] *= lbl_1_bss_14;
        lbl_1_bss_20--;
        if (lbl_1_bss_C == 0) {
            Center.y = lbl_1_data_80.center.y + lbl_1_bss_4;
            return;
        }
        Center.y = lbl_1_data_9C.center.y + lbl_1_bss_4;
    }
}

s32 fn_1_3A58(Vec *pos)
{
    Vec out;
    s32 ret;

    Hu3D3Dto2D(pos, 1, &out);
    ret = (s32)(32.0f + (64.0f * (out.x / 576.0f)));
    return ret;
}

s32 fn_1_3AD4(Vec *pos)
{
    Vec out;
    s32 ret;

    Hu3D3Dto2D(pos, 1, &out);
    ret = (s32)(64.0f + (63.0f * (out.y / 480.0f)));
    return ret;
}

s32 fn_1_3B50(void)
{
    M560Game *game;
    s32 ret;
    s32 total;

    game = (*lbl_1_bss_300)->data;
    total = (s32)fabs(lbl_1_data_134.z - -600.0f);
    ret = (s32)(88.0 + (39.0 * (fabs(lbl_1_data_134.z - game->pos.z) / (f64)total)));
    return ret;
}

void fn_1_3C8C(void)
{
    lbl_1_bss_510.head.prev = NULL;
    lbl_1_bss_510.head.next = NULL;
}

void fn_1_3CB0(u16 id, s16 val)
{
    M560Node *node;
    M560Node *oldNext;

    node = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M560Node), HU_MEMNUM_OVL);
    node->id = id;
    node->val = val;
    oldNext = lbl_1_bss_510.head.next;
    lbl_1_bss_510.head.next = node;
    if (oldNext != NULL) {
        oldNext->prev = node;
    }
    node->prev = &lbl_1_bss_510.head;
    node->next = oldNext;
}

s16 fn_1_3D38(s16 id)
{
    M560Node *node;
    M560Node *prev;
    M560Node *next;
    s16 val;

    node = lbl_1_bss_510.head.next;

    while (node != NULL) {
        if (id == node->id) {
            val = node->val;
            prev = node->prev;
            next = node->next;
            prev->next = next;
            if (next != NULL) {
                next->prev = prev;
            }
            HuMemDirectFree(node);
            return val;
        }
        node = node->next;
    }

    return -1;
}

void fn_1_3DDC(void)
{
    s32 i;

    for (i = 0; i < 0x20; i++) {
        lbl_1_bss_308[i].unk0 = 0;
        lbl_1_bss_308[i].unk4 = -1;
    }
}

s16 fn_1_3E34(u32 dataNum)
{
    s32 i;
    s32 j;

    for (i = 0; i < 0x20; i++) {
        if (dataNum == lbl_1_bss_308[i].unk0) {
            return Hu3DModelLink(lbl_1_bss_308[i].unk4);
        }
    }

    for (j = 0; j < 0x20; j++) {
        if (lbl_1_bss_308[j].unk0 == 0) {
            lbl_1_bss_308[j].unk0 = dataNum;
            lbl_1_bss_308[j].unk4 = Hu3DModelCreateData(dataNum);
            return lbl_1_bss_308[j].unk4;
        }
    }

    return Hu3DModelCreateData(dataNum);
}

void fn_1_3F48(Vec *a, Vec *b, Vec *out, f32 t)
{
    f32 k;

    if (1.0f < t) {
        t = 1.0f;
    }
    k = (f32)(sin((3.141592653589793 * (f64)(90.0f * t)) / 180.0) * sin((3.141592653589793 * (f64)(90.0f * t)) / 180.0));
    out->x = a->x + (k * (b->x - a->x));
    out->y = a->y + (k * (b->y - a->y));
    out->z = a->z + (k * (b->z - a->z));
}

void fn_1_40A4(Vec *pos, Vec *up, Vec *target)
{
    Hu3DCameraPosSetV(1, pos, up, target);
    lbl_1_bss_500 = *pos;
    lbl_1_bss_4E8 = *target;
}

s32 fn_1_4130(Vec *up, f32 t)
{
    Vec target;
    Vec pos;
    f32 k;
    f32 k2;
    f32 tc;
    f32 tc2;

    tc = t;
    if (1.0f < tc) {
        tc = 1.0f;
    }
    k = (f32)(sin((3.141592653589793 * (f64)(90.0f * tc)) / 180.0) * sin((3.141592653589793 * (f64)(90.0f * tc)) / 180.0));
    pos.x = lbl_1_bss_500.x + (k * (lbl_1_bss_4F4.x - lbl_1_bss_500.x));
    pos.y = lbl_1_bss_500.y + (k * (lbl_1_bss_4F4.y - lbl_1_bss_500.y));
    pos.z = lbl_1_bss_500.z + (k * (lbl_1_bss_4F4.z - lbl_1_bss_500.z));
    tc2 = t;
    if (1.0f < tc2) {
        tc2 = 1.0f;
    }
    k2 = (f32)(sin((3.141592653589793 * (f64)(90.0f * tc2)) / 180.0) * sin((3.141592653589793 * (f64)(90.0f * tc2)) / 180.0));
    target.x = lbl_1_bss_4E8.x + (k2 * (lbl_1_bss_4DC.x - lbl_1_bss_4E8.x));
    target.y = lbl_1_bss_4E8.y + (k2 * (lbl_1_bss_4DC.y - lbl_1_bss_4E8.y));
    target.z = lbl_1_bss_4E8.z + (k2 * (lbl_1_bss_4DC.z - lbl_1_bss_4E8.z));
    Hu3DCameraPosSetV(1, &pos, up, &target);
    if (t >= 1.0f) {
        return 1;
    }
    lbl_1_bss_40C = 1;
    return 0;
}

s32 fn_1_4464(Vec *up, f32 t)
{
    Vec target;
    Vec pos;
    f32 k;
    f32 k2;
    f32 tc;
    f32 tc2;
    s32 i;

    for (i = 0; i < 4; i++) {
        tc = t;
        if (1.0f < tc) {
            tc = 1.0f;
        }
        k = (f32)(sin((3.141592653589793 * (f64)(90.0f * tc)) / 180.0) * sin((3.141592653589793 * (f64)(90.0f * tc)) / 180.0));
        pos.x = lbl_1_bss_4AC[i].x + (k * (lbl_1_bss_47C[i].x - lbl_1_bss_4AC[i].x));
        pos.y = lbl_1_bss_4AC[i].y + (k * (lbl_1_bss_47C[i].y - lbl_1_bss_4AC[i].y));
        pos.z = lbl_1_bss_4AC[i].z + (k * (lbl_1_bss_47C[i].z - lbl_1_bss_4AC[i].z));
        tc2 = t;
        if (1.0f < tc2) {
            tc2 = 1.0f;
        }
        k2 = (f32)(sin((3.141592653589793 * (f64)(90.0f * tc2)) / 180.0) * sin((3.141592653589793 * (f64)(90.0f * tc2)) / 180.0));
        target.x = lbl_1_bss_44C[i].x + (k2 * (lbl_1_bss_41C[i].x - lbl_1_bss_44C[i].x));
        target.y = lbl_1_bss_44C[i].y + (k2 * (lbl_1_bss_41C[i].y - lbl_1_bss_44C[i].y));
        target.z = lbl_1_bss_44C[i].z + (k2 * (lbl_1_bss_41C[i].z - lbl_1_bss_44C[i].z));
        Hu3DCameraPosSetV(1 << i, &pos, up, &target);
    }

    if (t >= 1.0f) {
        return 1;
    }
    lbl_1_bss_40C = 1;
    return 0;
}

f32 fn_1_4848(f32 a, f32 b, f32 t)
{
    f32 diff;
    f32 res;

    diff = (f32)fmod(b - a, 360.0);
    if (0.0f > diff) {
        diff += 360.0f;
    }
    if (180.0f < diff) {
        diff -= 360.0f;
    }
    res = (f32)fmod(a + (diff * t), 360.0);
    if (0.0f > res) {
        res += 360.0f;
    }
    return res;
}

void fn_1_494C(s32 speed)
{
    if (lbl_1_bss_410[0] >= 0) {
        if (speed < 0) {
            speed = 0x3E8;
        }
        HuAudSeqFadeOut(lbl_1_bss_410[0], speed);
    }
    lbl_1_bss_410[0] = -1;
}

void fn_1_49B8(s16 mdlId, char *name, Vec *out)
{
    Mtx mtx;

    Hu3DModelObjMtxGet(mdlId, name, mtx);
    out->x = mtx[0][3];
    out->y = mtx[1][3];
    out->z = mtx[2][3];
}

s32 fn_1_4A10(s32 lo, s32 hi)
{
    return lo + frandmod(hi - lo);
}

s32 fn_1_4A4C(Vec *a, Vec *b, f32 r1, f32 r2)
{
    if ((((b->x - a->x) * (b->x - a->x)) + ((b->z - a->z) * (b->z - a->z))) < ((r1 + r2) * (r1 + r2))) {
        return 1;
    }
    return 0;
}

s32 fn_1_4AAC(Vec *pos, s32 lo, s32 hi)
{
    Vec out;
    s32 ret;

    Hu3D3Dto2D(pos, 1, &out);
    ret = (s32)((f32)lo + ((out.x / 576.0f) * (f32)(hi - lo)));
    return ret;
}

s32 fn_1_4B70(Vec *pos, s32 lo, s32 hi)
{
    Vec out;
    s32 ret;

    Hu3D3Dto2D(pos, 1, &out);
    ret = (s32)((f32)lo + ((out.y / 480.0f) * (f32)(hi - lo)));
    return ret;
}

void fn_1_4C34(HU3DMODEL *model, M560Parm *parm, Mtx mtx)
{
    Vec posA[4];
    Vec posB[4];
    M560Cell *cell;
    GXColor *color;
    M560Player *work;
    f32 t;
    f32 alpha;
    f32 alphaClamp;
    f32 tScale;
    f32 scaleBase;
    s16 i;
    s16 j;
    s32 hit;

    cell = parm->cells;
    color = parm->color;
    parm->activeCnt = 0;

    for (i = 0; i < parm->cellCnt; i++, cell++, color++) {
        if (cell->idx < 0) {
            continue;
        }

        cell->dist = cell->dist + cell->speed;
        if (cell->maxDist < cell->dist) {
            cell->idx = -1;
            continue;
        }

        parm->activeCnt++;
        if (parm->followPos != NULL) {
            cell->org.x = parm->followPos->x;
            cell->org.y = parm->followPos->y;
            cell->org.z = parm->followPos->z;
        }
        if (parm->followRot != NULL) {
            cell->ang = *parm->followRot;
        }
        cell->pos.x = (f32)((f64)cell->org.x + ((f64)cell->dist * sin((3.141592653589793 * (f64)(cell->ang + cell->angOfs)) / 180.0)));
        cell->pos.y = cell->pos.y + (0.03f * (cell->targetY - cell->pos.y));
        cell->pos.z = (f32)((f64)cell->org.z + ((f64)cell->dist * cos((3.141592653589793 * (f64)(cell->ang + cell->angOfs)) / 180.0)));
        t = cell->dist / cell->maxDist;
        alpha = (f32)(128.0 * cos((3.141592653589793 * (f64)(90.0f * t)) / 180.0));
        if (alpha > 255.0f) {
            alphaClamp = 255.0f;
        } else {
            alphaClamp = alpha;
        }
        color->a = (u8)alphaClamp;
        cell->frame = (s16)((f32)parm->frameCnt * t);
        if (parm->frameCnt <= cell->frame) {
            cell->frame = parm->frameCnt - 1;
        }
        tScale = t * 2.0f;
        if (tScale > 1.0f) {
            tScale = 1.0f;
        }
        cell->scale = (f32)(1.5 + (3.0 * sin((3.141592653589793 * (f64)(100.0f * tScale)) / 180.0)));
        scaleBase = 40.0f * cell->scale;

        for (j = 0; j < 4; j++) {
            work = lbl_1_bss_304[j]->data;
            posA[j].x = work->pos.x;
            posA[j].y = work->pos.y;
            posA[j].z = work->pos.z;
            posA[j].y = posA[j].y + scaleBase;
            posB[j].x = work->pos.x;
            posB[j].y = work->pos.y;
            posB[j].z = work->pos.z;
            posB[j].y = posB[j].y + (170.0f - scaleBase);
            if (cell->frame < 0xC) {
                if ((((cell->pos.x - work->pos.x) * (cell->pos.x - work->pos.x)) + ((cell->pos.z - work->pos.z) * (cell->pos.z - work->pos.z))) < 8100.0f) {
                    hit = 1;
                } else {
                    hit = 0;
                }
                if (hit != 0 && work->unk6C == 0 && work->done == 0) {
                    work->state = 8;
                    work->unk6C = 1;
                }
            }
        }
    }
}

void fn_1_510C(Mtx src, Mtx dst)
{
    dst[0][0] = src[0][0];
    dst[1][0] = src[0][1];
    dst[2][0] = src[0][2];
    dst[0][1] = src[1][0];
    dst[1][1] = src[1][1];
    dst[2][1] = src[1][2];
    dst[0][2] = src[2][0];
    dst[1][2] = src[2][1];
    dst[2][2] = src[2][2];
    dst[0][3] = 0.0f;
    dst[1][3] = 0.0f;
    dst[2][3] = 0.0f;
}

void fn_1_5188(Vec *src, Vec *dst, s16 count, f32 scale)
{
    s16 i;

    for (i = 0; i < count; i++, src++, dst++) {
        dst->x = src->x * scale;
        dst->y = src->y * scale;
        dst->z = src->z * scale;
    }
}

void fn_1_51E4(f32 *uv, f32 *tbl, s16 idx, f32 du, f32 dv)
{
    uv[0] = tbl[idx * 2];
    uv[1] = tbl[idx * 2 + 1];
    uv[2] = du + tbl[idx * 2];
    uv[3] = tbl[idx * 2 + 1];
    uv[4] = du + tbl[idx * 2];
    uv[5] = dv + tbl[idx * 2 + 1];
    uv[6] = tbl[idx * 2];
    uv[7] = dv + tbl[idx * 2 + 1];
}

void fn_1_5288(HU3DMODEL *model, Mtx *mtxP)
{
    f32(*mtx)[4];
    f32 tmp[12];
    f32 ro[4][3];
    f32 rotMtx[3][4];
    f32 concatMtx[3][4];
    Vec rotCorner[4];
    Vec scaled[4];
    Vec final[4];
    M560Parm *parm;
    M560Cell *cell;
    Vec *vtx;
    M560Uv *uv;
    Vec *srcCorner;
    Vec *dstCorner;
    f32 scale;
    f32 du;
    f32 dv;
    s16 fmt;
    s16 i;
    s16 j;
    s16 k;
    s16 frame;
    f32 *uvTbl;

    mtx = *mtxP;
    parm = model->hookData;

    if (parm->kill == 0 || parm->activeCnt != 0) {
        GXLoadPosMtxImm(mtx, GX_PNMTX0);
        GXSetNumTevStages(1);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, 0, 0x7D);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        fmt = parm->anim->bmp->dataFmt & 0xF;
        if (fmt == 8 || fmt == 7) {
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXA, GX_CC_RASC, GX_CC_ZERO);
        } else {
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_ZERO);
        }
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_CLAMP, GX_AF_NONE);
        if ((model->attr & 2) != 0) {
            GXSetZMode(1, GX_LEQUAL, 1);
        } else {
            GXSetZMode(1, GX_LEQUAL, 0);
        }
        HuSprTexLoad(parm->anim, 0, 0, 1, 1, 1);
        GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
        GXSetZCompLoc(0);
        switch (parm->blend) {
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
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_TEX_ST, GX_RGBA6, 0);
        GXSETARRAY(GX_VA_POS, parm->vtx, parm->cellCnt * 4 * sizeof(Vec), sizeof(Vec), TRUE);
        GXSetVtxDesc(GX_VA_CLR0, GX_INDEX16);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_TEX_ST, GX_RGBA8, 0);
        GXSETARRAY(GX_VA_CLR0, parm->color, parm->cellCnt * 4, sizeof(GXColor), TRUE);
        GXSetVtxDesc(GX_VA_TEX0, GX_INDEX16);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_RGBA6, 0);
        GXSETARRAY(GX_VA_TEX0, parm->uv, parm->cellCnt * 4 * 8, 8, TRUE);
        tmp[0] = mtx[0][0];
        tmp[3] = mtx[0][1];
        tmp[6] = mtx[0][2];
        tmp[1] = mtx[1][0];
        tmp[4] = mtx[1][1];
        tmp[7] = mtx[1][2];
        tmp[2] = mtx[2][0];
        tmp[5] = mtx[2][1];
        tmp[8] = mtx[2][2];
        tmp[9] = 0.0f;
        tmp[10] = 0.0f;
        tmp[11] = 0.0f;
        PSMTXReorder((f32(*)[4])tmp, ro);

        if (omPauseChk() == 0 && parm->hook != NULL) {
            parm->hook(model, parm, mtx);
        }

        cell = parm->cells;
        vtx = parm->vtx;
        uv = parm->uv;
        PSMTXROMultVecArray(ro, parm->corner, rotCorner, 4);

        for (i = 0; i < parm->cellCnt; i++, cell++, uv++) {
            if (cell->idx < 0) {
                vtx[0].x = vtx[0].y = vtx[0].z = 0.0f;
                vtx[1].x = vtx[1].y = vtx[1].z = 0.0f;
                vtx[2].x = vtx[2].y = vtx[2].z = 0.0f;
                vtx[3].x = vtx[3].y = vtx[3].z = 0.0f;
                vtx += 4;
            } else {
                if (cell->rot == 0.0f) {
                    scale = cell->scale;
                    dstCorner = scaled;
                    srcCorner = rotCorner;
                    for (j = 0; j < 4; j++, srcCorner++, dstCorner++) {
                        dstCorner->x = srcCorner->x * scale;
                        dstCorner->y = srcCorner->y * scale;
                        dstCorner->z = srcCorner->z * scale;
                    }
                    PSVECAdd(&scaled[0], &cell->pos, &vtx[0]);
                    PSVECAdd(&scaled[1], &cell->pos, &vtx[1]);
                    PSVECAdd(&scaled[2], &cell->pos, &vtx[2]);
                    PSVECAdd(&scaled[3], &cell->pos, &vtx[3]);
                    vtx += 4;
                } else {
                    scale = cell->scale;
                    dstCorner = scaled;
                    srcCorner = parm->corner;
                    for (k = 0; k < 4; k++, srcCorner++, dstCorner++) {
                        dstCorner->x = srcCorner->x * scale;
                        dstCorner->y = srcCorner->y * scale;
                        dstCorner->z = srcCorner->z * scale;
                    }
                    PSMTXRotRad(rotMtx, 'Z', 0.017453292f * cell->rot);
                    PSMTXConcat((f32(*)[4])tmp, rotMtx, concatMtx);
                    PSMTXMultVecArray(concatMtx, scaled, final, 4);
                    PSVECAdd(&final[0], &cell->pos, &vtx[0]);
                    PSVECAdd(&final[1], &cell->pos, &vtx[1]);
                    PSVECAdd(&final[2], &cell->pos, &vtx[2]);
                    PSVECAdd(&final[3], &cell->pos, &vtx[3]);
                    vtx += 4;
                }
                if (cell->frame != -1) {
                    dv = parm->vScale;
                    du = parm->uScale;
                    frame = cell->frame;
                    uvTbl = parm->uvTbl;
                    uv->v[0] = uvTbl[frame * 2];
                    uv->v[1] = uvTbl[frame * 2 + 1];
                    uv->v[2] = du + uvTbl[frame * 2];
                    uv->v[3] = uvTbl[frame * 2 + 1];
                    uv->v[4] = du + uvTbl[frame * 2];
                    uv->v[5] = dv + uvTbl[frame * 2 + 1];
                    uv->v[6] = uvTbl[frame * 2];
                    uv->v[7] = dv + uvTbl[frame * 2 + 1];
                }
            }
        }

        DCFlushRangeNoSync(parm->vtx, parm->cellCnt * 0xC * 4);
        DCFlushRangeNoSync(parm->uv, parm->cellCnt * 8 * 4);
        GXCallDisplayList(parm->dl, parm->dlSize);
    }
}

void fn_1_59D0(f32 *uv, s16 count, s16 cols, f32 du, f32 dv)
{
    s16 i;

    for (i = 0; i < count; i++, uv += 2) {
        uv[0] = (f32)(s16)(i % cols) * du;
        uv[1] = (f32)(s16)(i / cols) * dv;
    }
}

s16 fn_1_5A9C(ANIMDATA *anim, Vec *followPos, f32 *followRot, s16 cellCnt, s16 cols, s16 rows, f32 range)
{
    s16 mdl;
    s16 i;
    s16 colsN;
    s16 rowsN;
    s16 frames;
    HU3DMODEL *model;
    M560Parm *parm;
    M560Cell *cell;
    Vec *vtx;
    GXColor *color;
    M560Uv *uv;
    f32 *uvTbl;
    void *dlTmp;

    mdl = Hu3DHookFuncCreate(fn_1_5288);
    model = &Hu3DData[mdl];
    model->hookData = HuMemDirectMallocNum(HEAP_MODEL, sizeof(M560Parm), model->mallocNo);
    parm = model->hookData;
    anim->useNum++;
    parm->anim = anim;
    parm->followPos = followPos;
    parm->followRot = followRot;
    parm->cellCnt = cellCnt;
    parm->blend = 0;
    parm->hook = NULL;
    parm->activeCnt = cellCnt;
    parm->kill = 0;
    parm->cells = HuMemDirectMallocNum(HEAP_MODEL, cellCnt * sizeof(M560Cell), model->mallocNo);
    cell = parm->cells;

    for (i = 0; i < cellCnt; i++, cell++) {
        cell->idx = -1;
        cell->frame = -1;
    }

    parm->vtx = HuMemDirectMallocNum(HEAP_MODEL, cellCnt * 4 * sizeof(Vec), model->mallocNo);
    vtx = parm->vtx;

    for (i = 0; i < (s32)(cellCnt * 4); i++, vtx++) {
        vtx->x = 0.0f;
        vtx->y = 0.0f;
        vtx->z = 0.0f;
    }

    parm->color = HuMemDirectMallocNum(HEAP_MODEL, cellCnt * sizeof(GXColor), model->mallocNo);
    color = parm->color;

    for (i = 0; i < cellCnt; i++, color++) {
        color->r = 0xFF;
        color->g = 0xFF;
        color->b = 0xFF;
        color->a = 0xFF;
    }

    parm->uv = HuMemDirectMallocNum(HEAP_MODEL, cellCnt * sizeof(M560Uv), model->mallocNo);
    uv = parm->uv;

    for (i = 0; i < cellCnt; i++, uv++) {
        uv->v[0] = 0.0f;
        uv->v[1] = 0.0f;
        uv->v[2] = 1.0f;
        uv->v[3] = 0.0f;
        uv->v[4] = 1.0f;
        uv->v[5] = 1.0f;
        uv->v[6] = 0.0f;
        uv->v[7] = 1.0f;
    }

    if (cols != 0 || rows != 0) {
        colsN = anim->bmp->sizeX / cols;
        rowsN = anim->bmp->sizeY / rows;
        parm->frameCnt = colsN * rowsN;
        parm->uScale = 1.0f / (f32)colsN;
        parm->vScale = 1.0f / (f32)rowsN;
    } else {
        colsN = 1;
        parm->frameCnt = 1;
        parm->uScale = 1.0f;
        parm->vScale = 1.0f;
    }

    parm->uvTbl = HuMemDirectMallocNum(HEAP_MODEL, parm->frameCnt * 8, model->mallocNo);
    frames = parm->frameCnt;
    uvTbl = parm->uvTbl;

    for (i = 0; i < frames; i++, uvTbl += 2) {
        uvTbl[0] = (f32)(s16)(i % colsN) * parm->uScale;
        uvTbl[1] = (f32)(s16)(i / colsN) * parm->vScale;
    }

    parm->corner[0].x = -range;
    parm->corner[2].y = -range;
    parm->corner[3].x = -range;
    parm->corner[3].y = -range;
    parm->corner[0].y = range;
    parm->corner[1].x = range;
    parm->corner[1].y = range;
    parm->corner[2].x = range;
    parm->corner[3].z = 0.0f;
    parm->corner[2].z = 0.0f;
    parm->corner[1].z = 0.0f;
    parm->corner[0].z = 0.0f;

    dlTmp = HuMemDirectMallocNum(HEAP_MODEL, 0x20000, model->mallocNo);
    GXBeginDisplayList(dlTmp, 0x20000);
    GXBegin(GX_QUADS, GX_VTXFMT0, (u16)(cellCnt * 4));

    for (i = 0; i < cellCnt; i++) {
        GXPosition1x16((u16)(i * 4));
        GXPosition1x16((u16)i);
        GXPosition1x16((u16)(i * 4));
        GXPosition1x16((u16)((i * 4) + 1));
        GXPosition1x16((u16)i);
        GXPosition1x16((u16)((i * 4) + 1));
        GXPosition1x16((u16)((i * 4) + 2));
        GXPosition1x16((u16)i);
        GXPosition1x16((u16)((i * 4) + 2));
        GXPosition1x16((u16)((i * 4) + 3));
        GXPosition1x16((u16)i);
        GXPosition1x16((u16)((i * 4) + 3));
    }

    parm->dlSize = GXEndDisplayList();
    DCFlushRangeNoSync(dlTmp, parm->dlSize);
    parm->dl = HuMemDirectMallocNum(HEAP_MODEL, parm->dlSize, model->mallocNo);
    memcpy(parm->dl, dlTmp, parm->dlSize);
    DCFlushRangeNoSync(parm->dl, parm->dlSize);
    HuMemDirectFree(dlTmp);
    return mdl;
}

M560Cell *fn_1_60F4(s16 mdlId, Vec *pos, GXColor *color, f32 scale, f32 rot)
{
    M560Cell *cell;
    M560Parm *parm;
    s16 i;
    HU3DMODEL *model;
    GXColor *slot;

    model = &Hu3DData[mdlId];
    parm = model->hookData;

    if (parm->kill != 0) {
        return NULL;
    }

    i = 0;
    cell = parm->cells;

    for (; i < parm->cellCnt; i++, cell++) {
        if (cell->idx < 0) {
            break;
        }
    }

    if (i == parm->cellCnt) {
        return NULL;
    }

    slot = &parm->color[i];
    slot->r = color->r;
    slot->g = color->g;
    slot->b = color->b;
    slot->a = color->a;
    cell->pos = *pos;
    cell->scale = scale;
    cell->rot = rot;
    cell->frame = 0;
    cell->unk0 = 0;
    cell->idx = i;
    return cell;
}

void fn_1_61FC(s16 mdlId, M560PARMHOOK hook)
{
    HU3DMODEL *model = &Hu3DData[mdlId];
    M560Parm *parm = model->hookData;

    parm->hook = hook;
}

void fn_1_6234(s16 modelId, u8 value)
{
    HU3DMODEL *model = &Hu3DData[modelId];

    ((M560Parm *)model->hookData)->blend = value;
}

void fn_1_6264(s16 mdlId)
{
    HU3DMODEL *model = &Hu3DData[mdlId];
    M560Parm *parm = model->hookData;

    parm->kill = 1;
}

s32 fn_1_62A0(s16 mdlId)
{
    M560Parm *parm;
    HU3DMODEL *model;

    model = &Hu3DData[mdlId];
    parm = model->hookData;

    if (parm->kill != 0 && parm->activeCnt == 0) {
        HuMemDirectFree(parm->cells);
        HuMemDirectFree(parm->vtx);
        HuMemDirectFree(parm->color);
        HuMemDirectFree(parm->uv);
        HuMemDirectFree(parm->uvTbl);
        HuMemDirectFree(parm->dl);
        HuSprAnimKill(parm->anim);
        HuMemDirectFree(model->hookData);
        Hu3DModelKill(mdlId);
        return 1;
    }

    return 0;
}

void fn_1_635C(s16 mdlId, s16 count, Vec *pos, f32 angle, f32 maxDist, f32 targetY)
{
    GXColor color = { 0xFF, 0xFF, 0xFF, 0xFF };
    s16 i;
    M560Cell *cell;

    for (i = 0; i < count; i++) {
        cell = fn_1_60F4(mdlId, pos, &color, 0.0f, 0.0f);
        if (cell == NULL) {
            break;
        }
        cell->org.x = pos->x;
        cell->org.y = pos->y;
        cell->org.z = pos->z;
        cell->dist = 0.0f;
        cell->ang = angle;
        cell->speed = 10.0f;
        cell->maxDist = maxDist;
        cell->targetY = targetY;
        cell->angOfs = (20.0f * frandf()) - 10.0f;
        cell->speed = cell->speed * (0.8f + (0.4f * frandf()));
        cell->maxDist = cell->maxDist * (0.9f + (0.3f * frandf()));
    }
}

s16 fn_1_65BC(Vec *followPos, f32 *followRot, s16 cellCnt)
{
    s16 mdl;

    mdl = fn_1_5A9C(lbl_1_bss_0, followPos, followRot, cellCnt, 0x40, 0x40, 40.0f);
    ((M560Parm *)Hu3DData[mdl].hookData)->hook = fn_1_4C34;
    ((M560Parm *)Hu3DData[mdl].hookData)->blend = 0;
    Hu3DModelLayerSet(mdl, 5);
    return mdl;
}
