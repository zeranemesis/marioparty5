#include "game/object.h"
#include "game/charman.h"
#include "game/animdata.h"
#include "game/audio.h"
#include "game/data.h"
#include "game/flag.h"
#include "game/frand.h"
#include "game/gamework.h"
#include "game/gxarray.h"
#include "game/hsfex.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/pad.h"
#include "game/printfunc.h"
#include "game/process.h"
#include "game/sprite.h"
#include "game/mg/seqman.h"
#include "game/mg/timer.h"
#include "REL/m561dll.h"
#include <math.h>

typedef void (*VoidFunc)(void);

typedef struct M561Node_s {
    s16 id;
    s16 val;
    struct M561Node_s *prev;
    struct M561Node_s *next;
} M561Node;
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];
extern void ObjectSetup(void);

/* One waypoint of a player's run-in path. */
typedef struct M561Path_s {
    s32 frame;
    f32 x, y, z;
    f32 rot;
} M561Path;

/* Per-player work block (omObj->data, 0xAC bytes). */
typedef struct M561Player_s {
    /* 0x00 */ s16 motion;
    /* 0x02 */ s16 pad02;
    /* 0x04 */ u8 pad04[0x10];
    /* 0x14 */ s32 playerNo;
    /* 0x18 */ s32 charNo;
    /* 0x1C */ s32 padNo;
    /* 0x20 */ s32 input;
    /* 0x24 */ s32 state;
    /* 0x28 */ s32 timer;
    /* 0x2C */ Vec pos;
    /* 0x38 */ f32 scale;
    /* 0x3C */ Vec *target;
    /* 0x40 */ Vec prevPos;
    /* 0x4C */ Vec rot;
    /* 0x58 */ f32 baseY;
    /* 0x5C */ f32 angle;
    /* 0x60 */ f32 dist;
    /* 0x64 */ Vec dir;
    /* 0x70 */ Vec off;
    /* 0x7C */ s32 targetIdx;
    /* 0x80 */ s32 locked;
    /* 0x84 */ s32 done;
    /* 0x88 */ s32 level;
    /* 0x8C */ s32 pad8C;
    /* 0x90 */ s32 aiDelay;
    /* 0x94 */ s32 aiTimer;
    /* 0x98 */ s32 pathIdx;
    /* 0x9C */ f32 pathT;
    /* 0xA0 */ s32 waitCnt;
    /* 0xA4 */ s32 waitMax;
    /* 0xA8 */ f32 rotStep;
} M561Player;

/* Game-manager work block (0x60 bytes). */
typedef struct M561Game_s {
    /* 0x00 */ s16 modelId;
    /* 0x02 */ s16 pad02;
    /* 0x04 */ Vec pos;
    /* 0x10 */ Vec vel;
    /* 0x1C */ Vec rot;
    /* 0x28 */ f32 rotStep;
    /* 0x2C */ s32 motion;
    /* 0x30 */ u32 state;
    /* 0x34 */ s32 timer;
    /* 0x38 */ s32 pathIdx;
    /* 0x3C */ f32 pathT;
    /* 0x40 */ f32 prevTime;
    /* 0x44 */ s32 effectA;
    /* 0x48 */ s32 effectB;
    /* 0x4C */ s32 cur;
    /* 0x50 */ s32 prev;
    /* 0x54 */ s32 stage;
    /* 0x58 */ s32 flagA;
    /* 0x5C */ s32 flagB;
} M561Game;

#define PLAYER(o) ((M561Player *)(o)->data)
#define GAME(o) ((M561Game *)(o)->data)

/* ---- .data ---- */
Vec lbl_1_data_0 = { 0.0f, 10000.0f, 10000.0f };
Vec lbl_1_data_C = { 0.0f, 0.0f, 0.0f };
u8 lbl_1_data_18[4] = { 0xFF, 0xFF, 0xFF, 0xFF };
s32 lbl_1_data_1C = -1;
s32 lbl_1_data_20 = -1;
s32 lbl_1_data_24[9] = { 0x8C0000, 0x8C0019, 0x8C0086, 0x8C0031, 0x8C0002, 0x8C002C, 0x8C0003, 0x8C0004, 0x8C0015 };
s32 lbl_1_data_48[9] = { 0x740001, 0x740002, 0x740003, 0x740004, 0x740005, 0x740006, 0x740007, 0x740008, 0x740009 };
M561Path lbl_1_data_6C[4][7] = {
    {
        { 70, -80.0f, 0.0f, -2420.0f, 0.0f },
        { 230, -60.0f, 0.0f, -1345.0f, 1.0f },
        { 270, -58.0f, 0.0f, -1077.0f, 3.79999995f },
        { 285, -68.0f, 0.0f, -978.0f, -14.8000002f },
        { 300, -150.0f, 0.0f, -920.0f, -50.0f },
        { 315, -239.0f, 0.0f, -875.0f, -32.2000008f },
        { 330, -300.0f, 0.0f, -800.0f, 0.0f },
    },
    {
        { 70, -80.0f, 0.0f, -2630.0f, 0.0f },
        { 230, -56.0f, 0.0f, -1622.0f, 4.30000019f },
        { 280, -12.0f, 0.0f, -1310.0f, 10.6999998f },
        { 310, 12.0f, 0.0f, -1123.0f, 0.0f },
        { 325, 0.0f, 0.0f, -1030.0f, -7.4000001f },
        { 345, -52.0f, 0.0f, -915.0f, -1.20000005f },
        { 365, -100.0f, 0.0f, -830.0f, 0.0f },
    },
    {
        { 70, 80.0f, 0.0f, -2530.0f, 0.0f },
        { 210, 73.0f, 0.0f, -1626.0f, -5.0f },
        { 260, 46.0f, 0.0f, -1304.0f, -6.5f },
        { 295, 26.0f, 0.0f, -1079.0f, 0.0f },
        { 310, 44.0f, 0.0f, -985.0f, 13.1999998f },
        { 325, 77.0f, 0.0f, -993.0f, 10.5f },
        { 340, 100.0f, 0.0f, -830.0f, 0.0f },
    },
    {
        { 70, 80.0f, 0.0f, -2330.0f, 0.0f },
        { 240, 47.0f, 0.0f, -1212.0f, -5.0f },
        { 260, 58.0f, 0.0f, -1081.0f, -2.4000001f },
        { 275, 88.0f, 0.0f, -989.0f, 31.0f },
        { 290, 161.0f, 0.0f, -924.0f, 56.5f },
        { 305, 248.0f, 0.0f, -878.0f, 36.5f },
        { 320, 300.0f, 0.0f, -800.0f, 0.0f },
    },
};
struct { s32 frame; f32 x, y, z; s32 pad; } lbl_1_data_29C[2] = { { 80, 0.0f, 0.0f, -3000.0f, 0 }, { 370, 0.0f, 0.0f, -1800.0f, 0 } };
f32 lbl_1_data_2C4[3] = { -45.0f, 0.0f, 45.0f };
OMCAMERAVIEW lbl_1_data_2D0 = { { 0.0f, 241.9f, -130.0f }, { -14.8f, 0.0f, 0.0f }, 2481.0f };
Vec lbl_1_data_2EC[8] = {
    { -80.0f, 0.0f, -2420.0f },
    { -80.0f, 0.0f, -2630.0f },
    { 80.0f, 0.0f, -2530.0f },
    { 80.0f, 0.0f, -2330.0f },
    { -300.0f, 0.0f, -800.0f },
    { -100.0f, 0.0f, -800.0f },
    { 100.0f, 0.0f, -800.0f },
    { 300.0f, 0.0f, -800.0f },
};
Vec lbl_1_data_34C = { 0.0f, 0.0f, -3000.0f };
Vec lbl_1_data_358 = { 0.0f, 0.0f, -1440.0f };
Vec lbl_1_data_364 = { 1.0f, 10000.0f, 0.0f };
Vec lbl_1_data_370 = { 0.0f, 1.0f, 0.0f };
Vec lbl_1_data_37C = { 0.0f, 0.0f, 0.0f };
Vec lbl_1_data_388[3] = { { -450.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 450.0f, 0.0f, 0.0f } };
f32 lbl_1_data_3AC[4][12] = {
    { 0.0f, 0.0f, 300.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { -75.0f, 0.0f, 300.0f, 75.0f, 0.0f, 300.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
    { -175.0f, 0.0f, 300.0f, 0.0f, 0.0f, 300.0f, 175.0f, 0.0f, 300.0f, 0.0f, 0.0f, 0.0f },
    { -250.0f, 0.0f, 300.0f, -75.0f, 0.0f, 300.0f, 75.0f, 0.0f, 300.0f, 250.0f, 0.0f, 300.0f },
};
s32 lbl_1_data_46C[3] = { 0x200, 0x100, 0x400 };

void fn_1_1E28(void);
void fn_1_23BC(void);
void fn_1_2514(void);
void fn_1_2860(void);
void fn_1_289C(void);
void fn_1_298C(void);

MGSEQ_PARAM lbl_1_data_478 = {
    0,
    0,
    (MGSEQ_FUNC)fn_1_1E28,
    (MGSEQ_FUNC)fn_1_23BC,
    NULL,
    (MGSEQ_FUNC)fn_1_2514,
    (MGSEQ_FUNC)fn_1_2860,
    (MGSEQ_FUNC)fn_1_289C,
    (MGSEQ_FUNC)fn_1_298C,
    NULL,
    NULL,
};

/* ---- .bss ---- */
s32 lbl_1_bss_0;
s32 lbl_1_bss_4[3];
s32 lbl_1_bss_10[3];
void *lbl_1_bss_1C;
u16 lbl_1_bss_20;
f32 lbl_1_bss_24;
f32 lbl_1_bss_28;
f32 lbl_1_bss_2C;
f32 lbl_1_bss_30;
f32 lbl_1_bss_34;
f32 lbl_1_bss_38;
s32 lbl_1_bss_3C;
Vec lbl_1_bss_40;
Vec lbl_1_bss_4C;
Vec lbl_1_bss_58;
Vec lbl_1_bss_64;
Vec lbl_1_bss_70;
Vec lbl_1_bss_7C;
s32 lbl_1_bss_88;
f32 lbl_1_bss_8C;
s16 lbl_1_bss_90;
s16 lbl_1_bss_92;
s32 lbl_1_bss_94;
MGTIMER *lbl_1_bss_98;
s16 lbl_1_bss_A4;
s16 lbl_1_bss_A6[0xF];
OMOBJ **lbl_1_bss_C4;
OMOBJ **lbl_1_bss_C8;
struct { u32 key; s16 model; } lbl_1_bss_CC[0x20];
s32 lbl_1_bss_1D0;
s32 lbl_1_bss_1D4;
Vec lbl_1_bss_1E0[4];
Vec lbl_1_bss_210[4];
Vec lbl_1_bss_240[4];
Vec lbl_1_bss_270[4];
Vec lbl_1_bss_2A0;
Vec lbl_1_bss_2AC;
Vec lbl_1_bss_2B8;
Vec lbl_1_bss_2C4;
OMOBJMAN *lbl_1_bss_2D0;
M561Node lbl_1_bss_2D4;

void fn_1_58C(OMOBJ *obj);
void fn_1_8C8(OMOBJ *obj);
void fn_1_35D4(OMOBJ *obj);
void fn_1_9464(void);
void fn_1_8AE8(void);
void fn_1_5D58(void);
void fn_1_5D6C(void);
void fn_1_5D9C(void);
s32 fn_1_1DB4(M561Player *p);
void fn_1_2B50(OMOBJ *obj, s32 motion, f32 speed, u32 attr);
void fn_1_2BDC(OMOBJ *obj, s32 motion);
void fn_1_2C4C(OMOBJ *obj);
void fn_1_2F2C(OMOBJ *obj);
void fn_1_3210(void);
void fn_1_38EC(OMOBJ *obj);
void fn_1_5394(OMOBJ *obj, s32 motion, f32 speed, u32 attr);
u32 fn_1_5428(void);
f32 fn_1_54BC(s32 from, s32 to, s32 frames);
void fn_1_55E0(s32 target);
void fn_1_5684(void);
f32 fn_1_5918(f32 x0, f32 z0, f32 x1, f32 z1);
s32 fn_1_5A88(void);
s32 fn_1_5B20(void);
s32 fn_1_5BB8(M561Player *p);
f32 fn_1_5C48(s32 frames, f32 from, f32 to);
void fn_1_5DCC(u16 frames, u16 mode, f32 amp, f32 decay, f32 speed);
void fn_1_5E28(void);
s16 fn_1_6078(Vec *pos);
s16 fn_1_60F4(Vec *pos);
s16 fn_1_6170(void);
void fn_1_61DC(s16 modelId, s16 motId, int lagF);
void fn_1_71B8(HU3DMODEL *modelP, HU3DPARTICLE *particleP, f32 mtx[3][4]);
s16 fn_1_8A18(Vec *follow, f32 *followRot, s16 count);
void fn_1_87B8(s16 id, s16 count, Vec *pos, f32 yaw, f32 range, f32 targetY);
s32 fn_1_86FC(s16 id);
void HuSprTexLoad(ANIMDATA *anim, s16 bmpNo, s16 texMapId, GXTexWrapMode wrapS, GXTexWrapMode wrapT, GXTexFilter filter);
void fn_1_8BCC(s16 arg0, Vec *arg1);
void fn_1_5D58(void);

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

void ObjectSetup(void)
{
    OMOBJ *obj;
    s32 i;

    OSReport("******* M561 ObjectSetup *********\n");
    lbl_1_bss_2D0 = omInitObjMan(0xC8, 0x2000);
    omGameSysInit(lbl_1_bss_2D0);
    lbl_1_bss_92 = Hu3DMotionCreate(HuDataSelHeapReadNum(0x740018, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_90 = Hu3DModelCameraCreate(lbl_1_bss_92, 1);
    HuPrcVSleep();
    lbl_1_bss_8C = Hu3DMotionMotionMaxTimeGet(lbl_1_bss_92);
    Hu3DCameraPosGet(1, &lbl_1_bss_7C, &lbl_1_bss_70, &lbl_1_bss_64);
    HuPrcVSleep();
    Hu3DModelKill(lbl_1_bss_90);
    HuPrcVSleep();
    Hu3DCameraCreate(1);
    Hu3DCameraPerspectiveSet(1, 30.0f, 20.0f, 15000.0f, 1.2f);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraPosSetV(1, &lbl_1_bss_7C, &lbl_1_bss_70, &lbl_1_bss_64);
    lbl_1_bss_3C = 0;
    lbl_1_bss_38 = 0.0f;
    lbl_1_bss_A4 = Hu3DGLightCreate(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    Hu3DGLightPosAimSetV(lbl_1_bss_A4, &lbl_1_data_0, &lbl_1_data_C);
    Hu3DGLightStaticSet(lbl_1_bss_A4, 1);
    Hu3DGLightInfinitytSet(lbl_1_bss_A4);
    Hu3DGLightColorSet(lbl_1_bss_A4, lbl_1_data_18[0], lbl_1_data_18[1], lbl_1_data_18[2], lbl_1_data_18[3]);
    Hu3DShadowCreate(30.0f, 10.0f, 10000.0f);
    Hu3DShadowPosSet(&lbl_1_data_364, &lbl_1_data_370, &lbl_1_data_37C);
    Hu3DShadowTPLvlSet(0.5f);
    omMakeGroupEx(lbl_1_bss_2D0, 0, 4);
    lbl_1_bss_C8 = omGetGroupMemberListEx(lbl_1_bss_2D0, 0);
    for (i = 0; i < 4; i++) {
        obj = omAddObjEx(lbl_1_bss_2D0, 0x64, 1, 9, 0, fn_1_58C);
        lbl_1_bss_C8[i] = obj;
        obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M561Player), HU_MEMNUM_OVL);
        obj->work[0] = i;
    }
    omMakeGroupEx(lbl_1_bss_2D0, 1, 1);
    lbl_1_bss_C4 = omGetGroupMemberListEx(lbl_1_bss_2D0, 1);
    obj = omAddObjEx(lbl_1_bss_2D0, 0x65, 1, 9, 1, fn_1_35D4);
    *lbl_1_bss_C4 = obj;
    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M561Game), HU_MEMNUM_OVL);
    lbl_1_bss_10[0] = lbl_1_bss_10[1] = lbl_1_bss_10[2] = 0;
    lbl_1_bss_4[0] = lbl_1_bss_4[1] = lbl_1_bss_4[2] = 0;
    lbl_1_bss_1C = HuSprAnimRead(HuDataReadNum(0x740019, HU_MEMNUM_OVL));
    HuSprAnimLock(lbl_1_bss_1C);
    lbl_1_bss_0 = -1;
    fn_1_5D58();
    fn_1_8AE8();
    fn_1_9464();
    MgSeqCreatePrio(&lbl_1_data_478, 0x3E8);
}

void fn_1_58C(OMOBJ *obj)
{
    M561Player *p = PLAYER(obj);
    HU3DMODELID *mdl;
    s32 i;

    p->playerNo = obj->work[0];
    p->charNo = GwPlayerConf[p->playerNo].charNo;
    p->padNo = GwPlayerConf[p->playerNo].padNo;
    p->state = 1;
    p->timer = 0;
    p->targetIdx = 0;
    p->angle = 0.0f;
    p->aiTimer = 0;
    p->locked = 0;
    p->done = 0;
    p->baseY = 0.0f;
    p->pathIdx = 0;
    p->pathT = 0.0f;
    p->rotStep = 0.0f;
    mdl = obj->mdlId;
    *mdl = CharModelCreate(p->charNo, 2);
    CharEffectLayerSet(6);
    for (i = 0; i < 9; i++) {
        obj->mtnId[i] = CharMotionCreate(p->charNo, lbl_1_data_24[i]);
        CharMotionSet(p->charNo, obj->mtnId[i]);
    }
    p->motion = 5;
    Hu3DModelAttrSet(*obj->mdlId, 1);
    Hu3DModelAttrSet(*obj->mdlId, 0x40000001);
    Hu3DMotionSet(*obj->mdlId, obj->mtnId[5]);
    Hu3DModelShadowSet(*obj->mdlId);
    p->level = GwPlayerConf[p->playerNo].dif;
    switch (p->level) {
        case 0:
            p->aiDelay = frandmod(0x5A) + 0x5A;
            break;
        case 1:
            p->aiDelay = frandmod(0x3C) + 0x3C;
            break;
        case 2:
            p->aiDelay = frandmod(0x2D) + 0x2D;
            break;
        case 3:
            p->aiDelay = frandmod(0x1E) + 0x1E;
            break;
    }
    p->rot.z = 0.0f;
    p->rot.y = 0.0f;
    p->rot.x = 0.0f;
    p->pos.x = lbl_1_data_2EC[p->playerNo].x;
    p->pos.y = lbl_1_data_2EC[p->playerNo].y;
    p->pos.z = lbl_1_data_2EC[p->playerNo].z;
    p->prevPos.x = p->pos.x;
    p->prevPos.y = p->pos.y;
    p->prevPos.z = p->pos.z;
    p->scale = 1.0f;
    omSetSca(obj, p->scale, p->scale, p->scale);
    omSetTra(obj, p->pos.x, p->pos.y, p->pos.z);
    omSetRot(obj, p->rot.x, p->rot.y, p->rot.z);
    Hu3DModelAttrReset(*obj->mdlId, 1);
    CharMotionDataClose(p->charNo);
    obj->objFunc = fn_1_8C8;
}

// Remaining diff: original frame is 0x30 larger (separate int-to-float temporaries per site).
void fn_1_8C8(OMOBJ *obj)
{
    M561Player *p = PLAYER(obj);
    Vec v;
    Vec tgt;
    Vec cur;
    f32 dx, dy, dz;
    f32 dist;
    f32 pan;
    f32 yaw;
    f32 turn;
    f32 sx;

    if (lbl_1_bss_94 != 0) {
        if (GwPlayerConf[p->playerNo].type != 0) {
            p->input = fn_1_1DB4(p);
        } else {
            p->input = HuPadBtnDown[p->padNo];
        }
    }
    if (p->state == 0) {
        Hu3DCameraPosGet(1, &lbl_1_bss_58, &lbl_1_bss_4C, &lbl_1_bss_40);
        dx = p->pos.x - lbl_1_bss_58.x;
        dy = p->pos.y - lbl_1_bss_58.y;
        dz = p->pos.z - lbl_1_bss_58.z;
        dist = sqrtf(dz * dz + (dx * dx + dy * dy));
        CharModelVoiceVolSet(p->charNo, (s16)(127.0 * (fabs(6000.0f - dist) / 6000.0)));
    } else {
        Hu3D3Dto2D(&p->pos, 1, &v);
        pan = 32.0f + ((v.x / 576.0f) * 64.0f);
        CharModelVoicePanSet(p->charNo, (s16)pan);
        CharModelVoiceVolSet(p->charNo, 0x7F);
    }
    if ((u32)p->state <= 0xD) {
        switch (p->state) {
            case 0:
                p->dir.x = (lbl_1_data_6C[p->playerNo][p->pathIdx].x - p->pos.x)
                    / ((f32)lbl_1_data_6C[p->playerNo][p->pathIdx].frame - p->pathT);
                p->dir.y = (lbl_1_data_6C[p->playerNo][p->pathIdx].y - p->pos.y)
                    / ((f32)lbl_1_data_6C[p->playerNo][p->pathIdx].frame - p->pathT);
                p->dir.z = (lbl_1_data_6C[p->playerNo][p->pathIdx].z - p->pos.z)
                    / ((f32)lbl_1_data_6C[p->playerNo][p->pathIdx].frame - p->pathT);
                p->pos.x = p->pos.x + p->dir.x;
                p->pos.y = p->pos.y + p->dir.y;
                p->pos.z = p->pos.z + p->dir.z;
                p->rot.y = p->rot.y + p->rotStep;
                p->pathT = p->pathT + 1.0f;
                if (p->pathT >= (f32)lbl_1_data_6C[p->playerNo][p->pathIdx].frame) {
                    p->pathIdx = p->pathIdx + 1;
                    p->rot.y = lbl_1_data_6C[p->playerNo][p->pathIdx - 1].rot;
                    p->rotStep = fn_1_5C48(
                        lbl_1_data_6C[p->playerNo][p->pathIdx].frame - lbl_1_data_6C[p->playerNo][p->pathIdx - 1].frame,
                        lbl_1_data_6C[p->playerNo][p->pathIdx - 1].rot, lbl_1_data_6C[p->playerNo][p->pathIdx].rot);
                    if (p->pathIdx > 7) {
                        p->state = 1;
                        p->dir.z = 0.0f;
                        p->dir.y = 0.0f;
                        p->dir.x = 0.0f;
                        p->rot.y = 0.0f;
                        p->rotStep = 0.0f;
                        p->pos.x = lbl_1_data_6C[p->playerNo][6].x;
                        p->pos.y = lbl_1_data_6C[p->playerNo][6].y;
                        p->pos.z = lbl_1_data_6C[p->playerNo][6].z;
                        fn_1_2B50(obj, 1, 8.0f, 0x40000001);
                    }
                }
                break;
            case 2:
                if ((p->input & 0x100) != 0) {
                    p->targetIdx = 1;
                    p->target = &lbl_1_data_388[1];
                    p->state = 3;
                    p->locked = 1;
                }
                if ((p->input & 0x200) != 0) {
                    p->targetIdx = 0;
                    p->target = lbl_1_data_388;
                    p->state = 3;
                    p->locked = 1;
                }
                if ((p->input & 0x400) != 0) {
                    p->targetIdx = 2;
                    p->target = &lbl_1_data_388[2];
                    p->state = 3;
                    p->locked = 1;
                }
                break;
            case 3:
                fn_1_2B50(obj, 4, 8.0f, 0x40000001);
                p->prevPos.x = p->pos.x;
                p->prevPos.y = p->pos.y;
                p->prevPos.z = p->pos.z;
                p->dir.x = p->target->x - p->pos.x;
                p->dir.y = 0.0f;
                p->dir.z = p->target->z - p->pos.z;
                PSVECNormalize(&p->dir, &p->dir);
                p->rot.y = 180.0 * (atan2(p->dir.x, p->dir.z) / 3.141592653589793);
                p->off.x = 10.0 * sin((3.141592653589793 * p->rot.y) / 180.0);
                p->off.y = 0.0f;
                p->off.z = 10.0 * cos((3.141592653589793 * p->rot.y) / 180.0);
                PSVECAdd(&p->pos, &p->off, &p->pos);
                fn_1_2C4C(obj);
                fn_1_2F2C(obj);
                tgt.x = p->target->x;
                tgt.y = p->target->y;
                tgt.z = p->target->z;
                cur.x = p->pos.x;
                cur.y = p->pos.y;
                cur.z = p->pos.z;
                if (((cur.x - tgt.x) * (cur.x - tgt.x)) + ((cur.z - tgt.z) * (cur.z - tgt.z)) < 108900.0f) {
                    p->state = 4;
                    fn_1_2B50(obj, 0, 8.0f, 0x40000001);
                }
                break;
            case 6:
                if (p->timer > 5) {
                    p->state = 3;
                    p->timer = 0;
                } else {
                    p->timer = p->timer + 1;
                }
                break;
            case 4:
                if (fn_1_5BB8(p) != 0) {
                    p->state = 5;
                    p->baseY = p->pos.y;
                    p->dist = fn_1_5918(p->pos.x, p->pos.z, lbl_1_data_388[p->targetIdx].x, lbl_1_data_388[p->targetIdx].z);
                }
                break;
            case 5:
                if ((f32)p->timer > 60.0f) {
                    p->timer = 0;
                    p->state = 7;
                } else if ((p->timer == 0) && (fn_1_5BB8(p) == 0)) {
                    p->state = 4;
                } else {
                    fn_1_2B50(obj, 6, 8.0f, 0);
                    p->pos.y = (f64)p->baseY + (550.0 * sin((3.141592653589793 * p->angle) / 180.0));
                    p->pos.x = (f64)p->pos.x + (((f64)p->dist * sin((3.141592653589793 * p->rot.y) / 180.0)) / 60.0);
                    p->pos.z = (f64)p->pos.z + (((f64)p->dist * cos((3.141592653589793 * p->rot.y) / 180.0)) / 60.0);
                    p->angle = p->angle + 3.0f;
                    p->timer = p->timer + 1;
                }
                break;
            case 7:
                p->pos.x = lbl_1_data_388[p->targetIdx].x;
                p->pos.y = lbl_1_data_388[p->targetIdx].y;
                p->pos.z = lbl_1_data_388[p->targetIdx].z;
                p->rot.y = 0.0f;
                p->scale = 1.0f;
                fn_1_2BDC(obj, 0);
                break;
            case 8:
                if (p->waitCnt > p->waitMax) {
                    p->state = 9;
                    omVibrate(p->playerNo, 0x1E, 0xC, 0);
                    Hu3D3Dto2D(&p->pos, 1, &v);
                    CharFXPlayPan(p->charNo, 0x1F9, (s16)(32.0f + ((v.x / 576.0f) * 64.0f)));
                } else {
                    p->waitCnt = p->waitCnt + 1;
                }
                break;
            case 9:
                fn_1_2B50(obj, 8, 8.0f, 0);
                if ((f32)p->timer > 45.0f) {
                    p->timer = 0;
                    p->state = 7;
                } else {
                    p->pos.y = (f64)p->baseY + (500.0 * sin((3.141592653589793 * p->angle) / 180.0));
                    p->angle = p->angle + 4.0f;
                    p->timer = p->timer + 1;
                }
                break;
            case 10:
                if (p->waitCnt > p->waitMax) {
                    p->state = 0xB;
                    fn_1_2B50(obj, 6, 8.0f, 0);
                } else {
                    p->waitCnt = p->waitCnt + 1;
                }
                break;
            case 11:
                if ((f32)p->timer > 60.0f) {
                    p->timer = 0;
                    p->state = 0xD;
                    fn_1_2B50(obj, 0, 8.0f, 0x40000001);
                } else {
                    if ((f32)p->timer == 55.0f) {
                        fn_1_2B50(obj, 7, 8.0f, 0);
                    }
                    p->pos.y = (f64)p->baseY + (400.0 * sin((3.141592653589793 * p->angle) / 180.0));
                    if ((p->angle >= 0.0f) && (p->angle < 90.0f)) {
                        if (p->pos.y > 200.0f) {
                            p->pos.x = (f64)p->pos.x + (1.25 * (((f64)p->dist * sin((3.141592653589793 * p->rot.y) / 180.0)) / 60.0));
                            p->pos.z = (f64)p->pos.z + (1.25 * (((f64)p->dist * cos((3.141592653589793 * p->rot.y) / 180.0)) / 60.0));
                        }
                    } else if ((p->angle >= 90.0f) && (p->angle < 180.0f)) {
                        p->pos.x = (f64)p->pos.x + (1.25 * (((f64)p->dist * sin((3.141592653589793 * p->rot.y) / 180.0)) / 60.0));
                        p->pos.z = (f64)p->pos.z + (1.25 * (((f64)p->dist * cos((3.141592653589793 * p->rot.y) / 180.0)) / 60.0));
                    }
                    p->angle = p->angle + 3.0f;
                    p->timer = p->timer + 1;
                }
                break;
            case 12:
                fn_1_2B50(obj, 0, 8.0f, 0x40000001);
                if (p->timer > 0xF) {
                    fn_1_2B50(obj, 2, 8.0f, 0);
                    p->timer = 0;
                    p->state = 0xD;
                } else {
                    p->timer = p->timer + 1;
                }
                break;
            case 13:
                yaw = p->rot.y;
                turn = fmod(0.0f - yaw, 360.0);
                if (turn < 0.0f) {
                    turn += 360.0f;
                }
                if (turn > 180.0f) {
                    turn -= 360.0f;
                }
                sx = fmod(yaw + (turn * 0.4f), 360.0);
                if (sx < 0.0f) {
                    sx += 360.0f;
                }
                p->rot.y = sx;
                break;
        }
    }
    omSetSca(obj, p->scale, p->scale, p->scale);
    omSetTra(obj, p->pos.x, p->pos.y, p->pos.z);
    omSetRot(obj, p->rot.x, p->rot.y, p->rot.z);
}

void fn_1_1C8C(void)
{
    M561Player *p;
    s32 i;

    for (i = 0; i < 4; i++) {
        p = PLAYER(lbl_1_bss_C8[i]);
        if (p->locked == 0) {
            p->input = lbl_1_data_46C[frandmod(3)];
        }
    }
}

void fn_1_1D10(void)
{
    M561Player *players[4];
    s32 i;

    for (i = 0; i < 4; i++) {
        players[i] = PLAYER(lbl_1_bss_C8[i]);
    }
    while (1) {
        if ((players[0]->state == 7) && (players[1]->state == 7) && (players[2]->state == 7) && (players[3]->state == 7)) {
            break;
        }
        HuPrcVSleep();
    }
}

s32 fn_1_1DB4(M561Player *p)
{
    s32 input;

    if (p->aiTimer > p->aiDelay) {
        input = lbl_1_data_46C[frandmod(3)];
    } else {
        p->aiTimer = p->aiTimer + 1;
        input = 0;
    }
    return input;
}

void fn_1_1E28(void)
{
    s32 i;
    OMOBJ *obj;

    lbl_1_bss_A6[0] = Hu3DModelCreate(HuDataSelHeapReadNum(0x74000A, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelShadowMapSet(lbl_1_bss_A6[0]);
    lbl_1_bss_A6[3] = Hu3DModelCreate(HuDataSelHeapReadNum(0x740014, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_A6[4] = Hu3DModelCreate(HuDataSelHeapReadNum(0x740015, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_A6[5] = Hu3DModelCreate(HuDataSelHeapReadNum(0x740016, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_A6[6] = Hu3DModelCreate(HuDataSelHeapReadNum(0x740017, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DMotionTimingHookSet(lbl_1_bss_A6[4], fn_1_61DC);
    Hu3DModelAttrSet(lbl_1_bss_A6[3], 0x40000002);
    Hu3DModelAttrSet(lbl_1_bss_A6[4], 0x40000002);
    Hu3DModelAttrSet(lbl_1_bss_A6[5], 0x40000002);
    Hu3DModelAttrSet(lbl_1_bss_A6[6], 0x40000002);
    Hu3DModelAttrSet(lbl_1_bss_A6[4], 1);
    Hu3DModelAttrSet(lbl_1_bss_A6[5], 1);
    Hu3DModelAttrSet(lbl_1_bss_A6[6], 1);
    lbl_1_bss_A6[1] = Hu3DModelCreate(HuDataSelHeapReadNum(0x74000B, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_A6[7] = Hu3DModelCreate(HuDataSelHeapReadNum(0x74000D, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelAttrSet(lbl_1_bss_A6[7], 0x40000001);
    lbl_1_bss_A6[2] = Hu3DModelCreate(HuDataSelHeapReadNum(0x74000C, HU_MEMNUM_OVL, HEAP_MODEL));
    Hu3DModelAttrSet(lbl_1_bss_A6[2], 0x40000001);
    lbl_1_bss_A6[8] = Hu3DModelCreate(HuDataSelHeapReadNum(0x74000F, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_A6[9] = Hu3DModelCreate(HuDataSelHeapReadNum(0x74000E, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_A6[0xA] = Hu3DModelCreate(HuDataSelHeapReadNum(0x740010, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_A6[0xB] = Hu3DModelCreate(HuDataSelHeapReadNum(0x740011, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_A6[0xC] = Hu3DModelCreate(HuDataSelHeapReadNum(0x740011, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_A6[0xD] = Hu3DModelCreate(HuDataSelHeapReadNum(0x740011, HU_MEMNUM_OVL, HEAP_MODEL));
    for (i = 0; i < 3; i++) {
        Hu3DModelPosSet(lbl_1_bss_A6[i + 8], lbl_1_data_388[i].x, lbl_1_data_388[i].y, lbl_1_data_388[i].z);
        Hu3DModelAttrSet(lbl_1_bss_A6[i + 8], 0x40000002);
        Hu3DMotionTimeSet(lbl_1_bss_A6[i + 8], 0.0f);
        Hu3DModelPosSet(lbl_1_bss_A6[i + 0xB], lbl_1_data_388[i].x, lbl_1_data_388[i].y, lbl_1_data_388[i].z);
        Hu3DModelAttrSet(lbl_1_bss_A6[i + 0xB], 0x40000002);
        Hu3DMotionTimeSet(lbl_1_bss_A6[i + 0xB], 0.0f);
    }
    lbl_1_bss_98 = MgTimerCreate(0);
    MgTimerPosSet(lbl_1_bss_98, 288.0f, 64.0f);
    lbl_1_bss_94 = 0;
    Hu3DCameraPosSetV(1, &lbl_1_bss_7C, &lbl_1_bss_70, &lbl_1_bss_64);
    HuPrcVSleep();
    lbl_1_bss_90 = Hu3DModelCameraCreate(lbl_1_bss_92, 1);
    Hu3DMotionSpeedSet(lbl_1_bss_90, 1.0f);
    Hu3DCameraMotionStart(lbl_1_bss_90, 1);
    for (i = 0; i < 4; i++) {
        PLAYER(lbl_1_bss_C8[i])->state = 0;
    }
    obj = *lbl_1_bss_C4;
    GAME(obj)->state = 0;
    fn_1_5D6C();
    MgSeqModeNext();
}

void fn_1_23BC(void)
{
    s32 i;
    s32 j;
    M561Player *p;

    for (i = 0; i < 0x1E; i++) {
        HuPrcVSleep();
    }
    for (i = 0; (f32)i < lbl_1_bss_8C; i++) {
        if (i == 0x11D) {
            Hu3DModelAttrReset(lbl_1_bss_A6[3], 0x40000002);
        }
        if (i == 0x140) {
            HuAudFXPlay(0x994);
        }
        if (i == 0x14F) {
            for (j = 0; j < 4; j++) {
                p = PLAYER(lbl_1_bss_C8[j]);
                omVibrate(p->playerNo, 0xF, 0xC, 0);
            }
        }
        if (i == 0x163) {
            HuAudFXPlay(0x998);
        }
        HuPrcVSleep();
    }
    Hu3DCameraPosGet(1, &lbl_1_bss_7C, &lbl_1_bss_70, &lbl_1_bss_64);
    Hu3DModelKill(lbl_1_bss_90);
    HuPrcVSleep();
    MgSeqModeNext();
}

void fn_1_2514(void)
{
    M561Game *g;
    s32 i;
    M561Player *p;

    g = GAME(*lbl_1_bss_C4);
    fn_1_3210();
    if (MgSeqFrameNoGet() == 0) {
        Hu3DModelAttrSet(lbl_1_bss_A6[4], 0x40000001);
        Hu3DModelAttrReset(lbl_1_bss_A6[4], 0x40000002);
        Hu3DModelAttrSet(lbl_1_bss_A6[3], 1);
        Hu3DModelAttrReset(lbl_1_bss_A6[4], 1);
        Hu3DModelAttrSet(lbl_1_bss_A6[5], 1);
        Hu3DModelAttrSet(lbl_1_bss_A6[6], 1);
        for (i = 0; i < 4; i++) {
            p = PLAYER(lbl_1_bss_C8[i]);
            p->state = 2;
            fn_1_2B50(lbl_1_bss_C8[p->playerNo], 0, 8.0f, 0x40000001);
        }
        MgTimerParamSet(lbl_1_bss_98, 0x12C, 0, 0);
        MgTimerModeOnSet(lbl_1_bss_98, 1);
    }
    if ((MgTimerDoneCheck(lbl_1_bss_98) != 0) || (fn_1_5A88() != 0)) {
        if (lbl_1_bss_98->stopF == 0) {
            lbl_1_bss_98->stopF = 1;
            lbl_1_bss_98->mode = 1;
        }
        lbl_1_bss_94 = 0;
        fn_1_1C8C();
        fn_1_5A88();
        fn_1_1D10();
        g->state = 2;
        g->pos.x = lbl_1_data_358.x;
        g->pos.y = lbl_1_data_358.y;
        g->pos.z = lbl_1_data_358.z;
        do {
            HuPrcVSleep();
        } while (g->state != 0xC);
        MgSeqModeNext();
        return;
    }
    lbl_1_bss_94 = 1;
}

void fn_1_2860(void)
{
    if (MgSeqFrameNoGet() == 0) {
        fn_1_5D9C();
        MgTimerKill(lbl_1_bss_98);
    }
}

void fn_1_289C(void)
{
    M561Game *g;
    s32 i;

    g = GAME(*lbl_1_bss_C4);
    fn_1_5684();
    if (lbl_1_bss_0 == 0) {
        for (i = 0; i < 0x1E; i++) {
            HuPrcVSleep();
        }
    } else {
        for (i = 0; i < (s32)(60.0f * (f32)lbl_1_bss_0); i++) {
            HuPrcVSleep();
        }
        for (i = 0; i < 0x1E; i++) {
            HuPrcVSleep();
        }
    }
    MgSeqModeNext();
}

void fn_1_298C(void)
{
    M561Game *g;
    M561Player *p;
    s32 i;
    s32 n;

    g = GAME(*lbl_1_bss_C4);
    if (MgSeqFrameNoGet() == 0) {
        for (i = 0; i < 4; i++) {
            p = PLAYER(lbl_1_bss_C8[i]);
            if (p->done == 0) {
                n = p->playerNo;
                if (_CheckFlag(0x1000F) == 0) {
                    GwPlayer[n].mgCoinBonus = 0xA;
                }
            }
        }
        if (lbl_1_bss_0 == 0) {
            g->state = 0xD;
            HuAudFXPlayPan(0x2C9, fn_1_6078(&g->pos));
            HuAudJinglePlay(0x7A);
        } else if (lbl_1_bss_0 == 4) {
            g->state = 0xE;
            HuAudFXPlayPan(0x2CA, fn_1_6078(&g->pos));
            HuAudJinglePlay(0x7B);
        } else {
            g->state = 0xD;
            HuAudFXPlayPan(0x2C9, fn_1_6078(&g->pos));
            HuAudJinglePlay(0x7A);
        }
        for (i = 0; i < 4; i++) {
            p = PLAYER(lbl_1_bss_C8[i]);
            if (p->done == 0) {
                fn_1_2B50(lbl_1_bss_C8[i], 2, 8.0f, 0);
            }
        }
    }
}

void fn_1_2B50(OMOBJ *obj, s32 motion, f32 speed, u32 attr)
{
    M561Player *p = PLAYER(obj);

    if (p->motion != motion) {
        p->motion = (s16)motion;
        CharMotionShiftSet(p->charNo, obj->mtnId[p->motion], 0.0f, speed, attr);
    }
}

void fn_1_2BDC(OMOBJ *obj, s32 motion)
{
    M561Player *p = PLAYER(obj);

    if (p->motion != motion) {
        p->motion = (s16)motion;
        CharMotionSet(p->charNo, obj->mtnId[p->motion]);
    }
}

void fn_1_2C4C(OMOBJ *obj)
{
    M561Player *p;
    M561Player *o;
    Vec d;
    Vec n;
    Vec a;
    Vec b;
    f32 mag;
    f32 omag;
    f32 gap;
    f32 w;
    f32 w2;
    s32 i;

    p = PLAYER(obj);
    PSVECSubtract(&p->pos, &p->prevPos, &d);
    mag = PSVECMag(&d);
    if (mag != 0.0f) {
        for (i = 0; i < 4; i++) {
            if (i != p->playerNo) {
                o = PLAYER(lbl_1_bss_C8[i]);
                a.x = p->pos.x;
                a.y = p->pos.y;
                a.z = p->pos.z;
                a.y = 0.0f;
                b.x = o->pos.x;
                b.y = o->pos.y;
                b.z = o->pos.z;
                b.y = 0.0f;
                PSVECSubtract(&a, &b, &d);
                gap = PSVECMag(&d);
                if (gap < 159.2f) {
                    if (p->playerNo > o->playerNo) {
                        p->state = 6;
                        return;
                    }
                    PSVECSubtract(&o->pos, &o->prevPos, &d);
                    omag = PSVECMag(&d);
                    PSVECSubtract(&a, &b, &n);
                    PSVECNormalize(&n, &n);
                    gap = 160.0f - gap;
                    w = ((0.6f * omag) + (0.4f * mag)) / (mag + omag);
                    p->pos.x = p->pos.x + (w * (gap * n.x));
                    p->pos.y = p->pos.y + (w * (gap * n.y));
                    p->pos.z = p->pos.z + (w * (gap * n.z));
                    if ((o->state == 3) || (o->state == 6)) {
                        w2 = ((0.6f * mag) + (0.4f * mag)) / (mag + omag);
                        o->pos.x = o->pos.x + (w2 * (gap * -n.x));
                        o->pos.y = o->pos.y + (w2 * (gap * -n.y));
                        o->pos.z = o->pos.z + (w2 * (gap * -n.z));
                    }
                }
            }
        }
    }
}

void fn_1_2F2C(OMOBJ *obj)
{
    M561Player *p = PLAYER(obj);
    Vec t;
    f32 dx, dz;
    f32 dist;
    s32 i;

    for (i = 0; i < 3; i++) {
        if (p->targetIdx != i) {
            t.x = lbl_1_data_388[i].x;
            t.y = lbl_1_data_388[i].y;
            t.z = lbl_1_data_388[i].z;
            dx = p->pos.x - lbl_1_data_388[i].x;
            dz = p->pos.z - lbl_1_data_388[i].z;
            dist = sqrtf((dx * dx) + (dz * dz));
            if (((p->pos.x - t.x) * (p->pos.x - t.x)) + ((p->pos.z - t.z) * (p->pos.z - t.z)) < 108900.0f) {
                p->pos.x = (330.0f * (dx / dist)) + lbl_1_data_388[i].x;
                p->pos.z = (330.0f * (dz / dist)) + lbl_1_data_388[i].z;
            }
        }
    }
}

void fn_1_3210(void)
{
}

void fn_1_3214(void)
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
        omCameraViewSet(&lbl_1_data_2D0);
    }
}

/* Custom particle model hook data (Hu3DData[id].hookData), built by fn_1_7EF8. */
typedef struct M561FxPart_s {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ Vec origin;
    /* 0x10 */ f32 radius;
    /* 0x14 */ f32 angle;
    /* 0x18 */ f32 angleOff;
    /* 0x1C */ f32 speed;
    /* 0x20 */ f32 maxRadius;
    /* 0x24 */ f32 targetY;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 scale;
    /* 0x30 */ Vec pos;
    /* 0x3C */ s16 frame;
    /* 0x3E */ s16 idx;
} M561FxPart;

typedef struct M561Fx_s {
    /* 0x00 */ HU3DPARTICLEHOOK hook;
    /* 0x04 */ ANIMDATA *anim;
    /* 0x08 */ Vec *follow;
    /* 0x0C */ f32 *followRot;
    /* 0x10 */ M561FxPart *parts;
    /* 0x14 */ Vec *verts;
    /* 0x18 */ f32 (*uvs)[2];
    /* 0x1C */ void *unk1C;
    /* 0x20 */ u8 (*colors)[4];
    /* 0x24 */ void *unk24;
    /* 0x28 */ Vec corners[4];
    /* 0x58 */ u32 dlSize;
    /* 0x5C */ f32 uScale;
    /* 0x60 */ f32 vScale;
    /* 0x64 */ s16 count;
    /* 0x66 */ s16 active;
    /* 0x68 */ s16 frames;
    /* 0x6A */ u8 blend;
    /* 0x6B */ u8 oneshot;
} M561Fx;

#define FXOF(id) ((M561Fx *)Hu3DData[id].hookData)

// Remaining diff: original inlines fn_1_8A18 (defined later in the unit).
void fn_1_35D4(OMOBJ *obj)
{
    M561Game *g = GAME(obj);
    s32 i;

    *obj->mdlId = Hu3DModelCreate(HuDataSelHeapReadNum(0x740000, HU_MEMNUM_OVL, HEAP_MODEL));
    for (i = 0; i < 9; i++) {
        obj->mtnId[i] = Hu3DJointMotion(*obj->mdlId, HuDataSelHeapReadNum(lbl_1_data_48[i], HU_MEMNUM_OVL, HEAP_MODEL));
    }
    g->motion = 9;
    fn_1_5394(obj, 0, 8.0f, 0x40000001);
    g->modelId = *obj->mdlId;
    g->pos.x = lbl_1_data_34C.x;
    g->pos.y = lbl_1_data_34C.y;
    g->pos.z = lbl_1_data_34C.z;
    g->rot.x = 0.0f;
    g->rot.y = 0.0f;
    g->rot.z = 0.0f;
    g->state = 1;
    g->timer = 0;
    g->prevTime = 0.0f;
    g->effectA = -1;
    g->pathIdx = 0;
    g->pathT = 0.0f;
    g->flagA = 0;
    g->flagB = 0;
    g->effectA = fn_1_8A18(NULL, NULL, 0x80);
    g->effectB = fn_1_8A18(NULL, NULL, 0xA);
    Hu3DModelShadowSet(*obj->mdlId);
    omSetSca(obj, 1.5f, 1.5f, 1.5f);
    obj->objFunc = fn_1_38EC;
}

// Remaining diff: original inlines fn_1_87B8/fn_1_86FC (defined later in the unit) and uses a larger frame.
void fn_1_38EC(OMOBJ *obj)
{
    M561Game *g;
    Vec leftHand;
    Vec rightHand;
    Vec hand;
    Vec head;
    Vec aim;
    Vec out;
    Mtx mtx;
    Mtx mtx2;
    s32 snd1;
    s32 snd2;
    f32 t;
    f32 yaw;
    s16 vol;
    s32 i;
    OMOBJ *po;

    snd1 = 0;
    snd2 = 0;
    leftHand.x = -80.0f;
    leftHand.y = 0.0f;
    leftHand.z = 70.0f;
    rightHand.x = 80.0f;
    rightHand.y = 0.0f;
    rightHand.z = 70.0f;
    g = GAME(obj);
    switch (g->state) {
        case 0:
            Hu3DMotionSpeedSet(*obj->mdlId, 0.8f);
            fn_1_5394(obj, 8, 8.0f, 0x40000001);
            t = Hu3DMotionTimeGet(*obj->mdlId);
            if ((g->prevTime < 10.0f) && (t >= 10.0f)) {
                if (g->motion == 8) {
                    vol = fn_1_6078(&g->pos);
                    HuAudFXPlayVolPan(0x2C7, fn_1_60F4(&g->pos), vol);
                }
                PSMTXRotRad(mtx, 'Y', 0.017453292f * g->rot.y);
                PSMTXMultVec(mtx, &leftHand, &out);
                PSVECAdd(&out, &g->pos, &out);
                fn_1_8BCC(1, &out);
            } else if (g->prevTime < 60.0f) {
                if (t >= 60.0f) {
                    if (g->motion == 8) {
                        vol = fn_1_6078(&g->pos);
                        HuAudFXPlayVolPan(0x2C7, fn_1_60F4(&g->pos), vol);
                    }
                    PSMTXRotRad(mtx, 'Y', 0.017453292f * g->rot.y);
                    PSMTXMultVec(mtx, &rightHand, &hand);
                    PSVECAdd(&hand, &g->pos, &hand);
                    fn_1_8BCC(1, &hand);
                }
            }
            g->vel.x = (lbl_1_data_29C[g->pathIdx].x - g->pos.x) / ((f32)lbl_1_data_29C[g->pathIdx].frame - g->pathT);
            g->vel.y = (lbl_1_data_29C[g->pathIdx].y - g->pos.y) / ((f32)lbl_1_data_29C[g->pathIdx].frame - g->pathT);
            g->vel.z = (lbl_1_data_29C[g->pathIdx].z - g->pos.z) / ((f32)lbl_1_data_29C[g->pathIdx].frame - g->pathT);
            g->pos.x = g->pos.x + g->vel.x;
            g->pos.y = g->pos.y + g->vel.y;
            g->pos.z = g->pos.z + g->vel.z;
            g->pathT = g->pathT + 1.0f;
            if (g->pathT >= (f32)lbl_1_data_29C[g->pathIdx].frame) {
                g->pathIdx = g->pathIdx + 1;
                if (g->pathIdx > 2) {
                    g->state = 1;
                    g->vel.z = 0.0f;
                    g->vel.y = 0.0f;
                    g->vel.x = 0.0f;
                }
            } else {
                g->prevTime = t;
            }
            break;
        case 1:
            Hu3DMotionSpeedSet(*obj->mdlId, 1.0f);
            fn_1_5394(obj, 0, 8.0f, 0x40000001);
            break;
        case 2:
            if (g->timer > 0x1E) {
                g->timer = 0;
                g->state = 3;
                fn_1_5394(obj, 7, 8.0f, 0);
            } else {
                g->timer = g->timer + 1;
            }
            break;
        case 3:
            Hu3DMotionSpeedSet(*obj->mdlId, 1.0f);
            if (g->timer > 0x50) {
                g->timer = 0;
                g->state = 4;
                fn_1_5394(obj, 8, 8.0f, 0x40000001);
            } else {
                if (g->timer == 0x1E) {
                    fn_1_5DCC(0x1E, 2, 5.0f, 0.95f, 90.0f);
                    for (i = 0; i < 4; i++) {
                        po = lbl_1_bss_C8[i];
                        omVibrate(PLAYER(po)->playerNo, 0xF, 0xC, 0);
                    }
                    Hu3DModelAttrSet(*obj->mdlId, 0x40000002);
                    Hu3DModelAttrReset(lbl_1_bss_A6[5], 0x40000002);
                    Hu3DModelAttrReset(lbl_1_bss_A6[6], 0x40000002);
                    Hu3DModelAttrSet(lbl_1_bss_A6[3], 1);
                    Hu3DModelAttrSet(lbl_1_bss_A6[4], 1);
                    Hu3DModelAttrReset(lbl_1_bss_A6[5], 1);
                    Hu3DModelAttrReset(lbl_1_bss_A6[6], 1);
                    HuAudFXPlay(0x996);
                    HuAudFXPlay(0x999);
                } else if (g->timer == 0x32) {
                    Hu3DModelAttrReset(*obj->mdlId, 0x40000002);
                }
                g->timer = g->timer + 1;
            }
            break;
        case 4:
            Hu3DMotionSpeedSet(*obj->mdlId, 0.8f);
            t = Hu3DMotionTimeGet(*obj->mdlId);
            if (g->timer > 0xD7) {
                g->timer = 0;
                g->state = 5;
                fn_1_5394(obj, 3, 8.0f, 0);
            } else {
                if ((g->prevTime < 10.0f) && (t >= 10.0f)) {
                    if (g->motion == 8) {
                        fn_1_5DCC(0x3C, 1, 2.0f, 0.75f, 90.0f);
                        vol = fn_1_6078(&g->pos);
                        HuAudFXPlayVolPan(0x2C7, fn_1_60F4(&g->pos), vol);
                    }
                    PSMTXRotRad(mtx, 'Y', 0.017453292f * g->rot.y);
                    PSMTXMultVec(mtx, &leftHand, &out);
                    PSVECAdd(&out, &g->pos, &out);
                    fn_1_8BCC(1, &out);
                } else if (g->prevTime < 60.0f) {
                    if (t >= 60.0f) {
                        if (g->motion == 8) {
                            fn_1_5DCC(0x3C, 1, 2.0f, 0.75f, 90.0f);
                            vol = fn_1_6078(&g->pos);
                            HuAudFXPlayVolPan(0x2C7, fn_1_60F4(&g->pos), vol);
                        }
                        PSMTXRotRad(mtx, 'Y', 0.017453292f * g->rot.y);
                        PSMTXMultVec(mtx, &rightHand, &hand);
                        PSVECAdd(&hand, &g->pos, &hand);
                        fn_1_8BCC(1, &hand);
                    }
                }
                g->timer = g->timer + 1;
                g->pos.z = g->pos.z + 3.2558138f;
            }
            g->prevTime = t;
            break;
        case 5:
            if (g->timer > 0x8C) {
                g->timer = 0;
                g->state = 8;
                fn_1_5394(obj, 0, 8.0f, 0x40000001);
                g->prev = 1;
                g->cur = fn_1_5428();
                g->stage = 0;
            } else {
                Hu3DMotionSpeedSet(*obj->mdlId, 1.0f);
                g->timer = g->timer + 1;
            }
            break;
        case 8:
            if (g->timer > 0x1E) {
                g->timer = 0;
                g->state = 6;
            } else {
                g->timer = g->timer + 1;
            }
            break;
        case 6:
            if (g->timer > 0x82) {
                g->timer = 0;
                if (g->stage == 0) {
                    if (g->flagA == 0) {
                        if (frandmod(0xA) >= 5) {
                            g->state = 9;
                        } else {
                            g->state = 0xB;
                        }
                    } else {
                        g->state = 0xB;
                    }
                } else if (g->stage == 1) {
                    if (g->flagB == 0) {
                        if (frandmod(0xA) >= 5) {
                            g->state = 0xA;
                        } else {
                            g->state = 0xB;
                        }
                    } else {
                        g->state = 0xB;
                    }
                }
                g->rot.y = lbl_1_data_2C4[g->cur];
            } else {
                g->rotStep = fn_1_54BC((s32)lbl_1_data_2C4[g->prev], (s32)lbl_1_data_2C4[g->cur], 0x82);
                t = Hu3DMotionTimeGet(*obj->mdlId);
                if (g->rotStep == 0.0f) {
                    g->timer = 0x82;
                    g->motion = -1;
                } else {
                    fn_1_5394(obj, 1, 8.0f, 0x40000001);
                }
                if ((g->prevTime < 25.0f) && (t >= 25.0f)) {
                    if (g->motion == 1) {
                        fn_1_5DCC(0x1E, 1, 2.0f, 0.75f, 90.0f);
                        vol = fn_1_6078(&g->pos);
                        HuAudFXPlayVolPan(0x2C7, fn_1_60F4(&g->pos), vol);
                    }
                } else if (g->prevTime < 75.0f) {
                    if ((t >= 75.0f) && (g->motion == 1)) {
                        fn_1_5DCC(0x1E, 1, 2.0f, 0.75f, 90.0f);
                        vol = fn_1_6078(&g->pos);
                        HuAudFXPlayVolPan(0x2C7, fn_1_60F4(&g->pos), vol);
                    }
                }
                g->rot.y = g->rot.y + g->rotStep;
                g->timer = g->timer + 1;
            }
            g->prevTime = t;
            break;
        case 9:
            fn_1_5394(obj, 4, 8.0f, 0);
            if (g->timer > 0x50) {
                g->timer = 0;
                g->state = 8;
                g->prev = g->cur;
                g->cur = fn_1_5428();
                g->flagA = 1;
            } else {
                if (g->timer == 0x32) {
                    Hu3DModelAttrSet(*obj->mdlId, 0x40000002);
                } else if (g->timer == 0x3C) {
                    Hu3DModelAttrReset(*obj->mdlId, 0x40000002);
                }
                g->timer = g->timer + 1;
            }
            break;
        case 10:
            fn_1_5394(obj, 4, 8.0f, 0);
            if (g->timer > 0x50) {
                g->timer = 0;
                g->state = 8;
                g->prev = g->cur;
                g->cur = fn_1_5428();
                g->flagB = 1;
            } else {
                if (g->timer == 0x32) {
                    Hu3DModelAttrSet(*obj->mdlId, 0x40000002);
                    snd2 = 1;
                    if (lbl_1_data_20 < 0) {
                        lbl_1_data_20 = HuAudFXPlayPan(0x997, fn_1_6170());
                    }
                } else if (g->timer == 0x50) {
                    Hu3DModelAttrReset(*obj->mdlId, 0x40000002);
                    snd2 = 0;
                    if (lbl_1_data_20 > 0) {
                        HuAudFXStop(lbl_1_data_20);
                        lbl_1_data_20 = -1;
                    }
                }
                g->timer = g->timer + 1;
            }
            break;
        case 11:
            fn_1_5394(obj, 4, 8.0f, 0);
            if (g->timer > 0xC8) {
                if (lbl_1_data_1C > 0) {
                    HuAudFXStop(lbl_1_data_1C);
                    lbl_1_data_1C = -1;
                }
                g->timer = 0;
                fn_1_5394(obj, 0, 8.0f, 0x40000001);
                g->stage = g->stage + 1;
                if (g->stage == 2) {
                    g->state = 7;
                    g->prev = g->cur;
                    g->cur = 1;
                } else if (fn_1_5B20() != 0) {
                    g->state = 7;
                    g->prev = g->cur;
                    g->cur = 1;
                } else {
                    g->state = 8;
                    g->prev = g->cur;
                    g->cur = fn_1_5428();
                }
            } else {
                if (g->timer >= 0x32) {
                    Hu3DModelAttrSet(*obj->mdlId, 0x40000002);
                    snd1 = 1;
                    if (lbl_1_data_1C < 0) {
                        lbl_1_data_1C = HuAudFXPlayPan(0x97B, fn_1_6170());
                    }
                    if (g->timer == 0x50) {
                        Hu3DModelAttrReset(lbl_1_bss_A6[g->cur + 8], 0x40000002);
                        Hu3DModelAttrReset(lbl_1_bss_A6[g->cur + 0xB], 0x40000002);
                        fn_1_55E0(g->cur);
                        lbl_1_bss_10[g->cur] = 1;
                    }
                } else if (g->timer == 0xAA) {
                    Hu3DModelAttrReset(*obj->mdlId, 0x40000002);
                    snd1 = 0;
                }
                g->timer = g->timer + 1;
            }
            break;
        case 7:
            fn_1_5394(obj, 1, 8.0f, 0x40000001);
            if (g->timer > 0x82) {
                g->timer = 0;
                g->rot.y = lbl_1_data_2C4[g->cur];
                g->state = 0xC;
                fn_1_5394(obj, 0, 8.0f, 0x40000001);
            } else {
                g->rotStep = fn_1_54BC((s32)lbl_1_data_2C4[g->prev], (s32)lbl_1_data_2C4[g->cur], 0x82);
                t = Hu3DMotionTimeGet(*obj->mdlId);
                if (g->rotStep == 0.0f) {
                    g->timer = 0x82;
                }
                if ((g->prevTime < 25.0f) && (t >= 25.0f)) {
                    if (g->motion == 1) {
                        fn_1_5DCC(0x1E, 1, 2.0f, 0.75f, 90.0f);
                        vol = fn_1_6078(&g->pos);
                        HuAudFXPlayVolPan(0x2C7, fn_1_60F4(&g->pos), vol);
                    }
                } else if (g->prevTime < 75.0f) {
                    if ((t >= 75.0f) && (g->motion == 1)) {
                        fn_1_5DCC(0x1E, 1, 2.0f, 0.75f, 90.0f);
                        vol = fn_1_6078(&g->pos);
                        HuAudFXPlayVolPan(0x2C7, fn_1_60F4(&g->pos), vol);
                    }
                }
                g->rot.y = g->rot.y + g->rotStep;
                g->timer = g->timer + 1;
            }
            g->prevTime = t;
            break;
        case 13:
            fn_1_5394(obj, 5, 8.0f, 0);
            break;
        case 14:
            fn_1_5394(obj, 6, 8.0f, 0);
            break;
    }
    if (g->effectA >= 0) {
        if (snd1 != 0) {
            Hu3DModelObjMtxGet(*obj->mdlId, "g000m0-itemhook_M", mtx);
            hand.x = mtx[0][3];
            hand.y = mtx[1][3];
            hand.z = mtx[2][3];
            Hu3DModelObjMtxGet(*obj->mdlId, "g000m0-ns_hed", mtx2);
            head.x = mtx2[0][3];
            head.y = mtx2[1][3];
            head.z = mtx2[2][3];
            PSVECSubtract(&hand, &head, &aim);
            yaw = 180.0 * (atan2(aim.x, aim.z) / 3.141592653589793);
            fn_1_87B8(g->effectA, 1, &hand, yaw, 750.0f, 50.0f);
        }
        if (fn_1_86FC(g->effectA) != 0) {
            g->effectA = -1;
        }
    }
    if (g->effectB >= 0) {
        if (snd2 != 0) {
            Hu3DModelObjMtxGet(*obj->mdlId, "g000m0-itemhook_M", mtx);
            hand.x = mtx[0][3];
            hand.y = mtx[1][3];
            hand.z = mtx[2][3];
            Hu3DModelObjMtxGet(*obj->mdlId, "g000m0-ns_hed", mtx2);
            head.x = mtx2[0][3];
            head.y = mtx2[1][3];
            head.z = mtx2[2][3];
            PSVECSubtract(&hand, &head, &aim);
            yaw = 180.0 * (atan2(aim.x, aim.z) / 3.141592653589793);
            fn_1_87B8(g->effectB, 1, &hand, yaw, 350.0f, 50.0f);
        }
        if (fn_1_86FC(g->effectB) != 0) {
            g->effectB = -1;
        }
    }
    fn_1_5E28();
    omSetTra(obj, g->pos.x, g->pos.y, g->pos.z);
    omSetRot(obj, g->rot.x, g->rot.y, g->rot.z);
}

void fn_1_5394(OMOBJ *obj, s32 motion, f32 speed, u32 attr)
{
    M561Game *g = GAME(obj);

    if (g->motion != motion) {
        g->motion = motion;
        Hu3DMotionShiftSet(*obj->mdlId, obj->mtnId[motion], 0.0f, speed, attr);
        g->prevTime = Hu3DMotionTimeGet(*obj->mdlId);
    }
}

u32 fn_1_5428(void)
{
    u32 r;

    if ((lbl_1_bss_10[0] != 0) && (lbl_1_bss_10[1] != 0) && (lbl_1_bss_10[2] != 0)) {
        return -1;
    }
    do {
        r = frandmod(3);
    } while (lbl_1_bss_10[r] != 0);
    return r;
}

f32 fn_1_54BC(s32 from, s32 to, s32 frames)
{
    f32 d;
    f32 r;

    if (from == to) {
        return 0.0f;
    }
    d = fmod((f64)(to - from), 360.0);
    if (0.0f > d) {
        d += 360.0f;
    }
    if (180.0f < d) {
        d -= 360.0f;
    }
    r = d / (f32)frames;
    return r;
}

void fn_1_55E0(s32 target)
{
    M561Player *p;
    s32 i;
    s32 n;

    i = 0;
    n = 0;
    for (; i < 4; i++) {
        p = PLAYER(lbl_1_bss_C8[i]);
        if (p->targetIdx == target) {
            p->waitMax = n * 0x1E;
            p->waitCnt = 0;
            n++;
            p->state = 8;
            p->angle = 0.0f;
            p->baseY = p->pos.y;
            p->timer = 0;
            p->done = 1;
        }
    }
}

void fn_1_5684(void)
{
    M561Player *p;
    s32 sel;
    s32 i;
    s32 n;

    for (i = 0; i < 3; i++) {
        if (lbl_1_bss_10[i] == 0) {
            sel = i;
            break;
        }
    }
    i = 0;
    n = 0;
    for (; i < 4; i++) {
        p = PLAYER(lbl_1_bss_C8[i]);
        if (p->targetIdx == sel) {
            p->waitMax = n * 0x3C;
            p->waitCnt = 0;
            n++;
            p->state = 0xA;
            p->angle = 0.0f;
            p->baseY = p->pos.y;
            p->timer = 0;
        }
    }
    lbl_1_bss_0 = n;
    i = 0;
    n = 0;
    for (; i < 4; i++) {
        p = PLAYER(lbl_1_bss_C8[i]);
        if (p->targetIdx == sel) {
            p->dist = fn_1_5918(p->pos.x, p->pos.z, lbl_1_data_3AC[lbl_1_bss_0 - 1][n * 3], lbl_1_data_3AC[lbl_1_bss_0 - 1][n * 3 + 2]);
            p->target->x = lbl_1_data_3AC[lbl_1_bss_0 - 1][n * 3];
            p->target->y = 0.0f;
            p->target->z = lbl_1_data_3AC[lbl_1_bss_0 - 1][n * 3 + 2];
            p->dir.x = p->target->x - p->pos.x;
            p->dir.y = 0.0f;
            p->dir.z = p->target->z - p->pos.z;
            PSVECNormalize(&p->dir, &p->dir);
            p->rot.y = 180.0 * (atan2(p->dir.x, p->dir.z) / 3.141592653589793);
            n++;
        }
    }
}

f32 fn_1_5918(f32 x0, f32 z0, f32 x1, f32 z1)
{
    f32 dx;
    f32 dz;
    f32 d;

    dx = x0 - x1;
    dz = z0 - z1;
    d = sqrtf((dx * dx) + (dz * dz));
    return d;
}

s32 fn_1_5A88(void)
{
    M561Player *players[4];
    s32 i;

    for (i = 0; i < 4; i++) {
        players[i] = PLAYER(lbl_1_bss_C8[i]);
    }
    if ((players[0]->state == 7) && (players[1]->state == 7) && (players[2]->state == 7) && (players[3]->state == 7)) {
        return 1;
    }
    return 0;
}

s32 fn_1_5B20(void)
{
    M561Player *players[4];
    s32 i;

    for (i = 0; i < 4; i++) {
        players[i] = PLAYER(lbl_1_bss_C8[i]);
    }
    if ((players[0]->done != 0) && (players[1]->done != 0) && (players[2]->done != 0) && (players[3]->done != 0)) {
        return 1;
    }
    return 0;
}

s32 fn_1_5BB8(M561Player *p)
{
    M561Player *o;
    s32 i;

    for (i = 0; i < 4; i++) {
        o = PLAYER(lbl_1_bss_C8[i]);
        if ((p != o) && (o->state == 5) && (p->targetIdx == o->targetIdx) && (o->angle < 90.0f)) {
            return 0;
        }
    }
    return 1;
}

f32 fn_1_5C48(s32 frames, f32 from, f32 to)
{
    f32 d;
    f32 r;

    if (from == to) {
        return 0.0f;
    }
    d = fmod((f64)(to - from), 360.0);
    if (0.0f > d) {
        d += 360.0f;
    }
    if (180.0f < d) {
        d -= 360.0f;
    }
    r = d / (f32)frames;
    return r;
}

void fn_1_5D58(void)
{
    lbl_1_bss_88 = -1;
}

void fn_1_5D6C(void)
{
    lbl_1_bss_88 = HuAudSStreamPlay(0x3F);
}

void fn_1_5D9C(void)
{
    HuAudSStreamFadeOut(lbl_1_bss_88, 0x64);
}

void fn_1_5DCC(u16 frames, u16 mode, f32 amp, f32 decay, f32 speed)
{
    lbl_1_bss_3C = frames;
    lbl_1_bss_34 = amp;
    lbl_1_bss_30 = decay;
    lbl_1_bss_28 = speed;
    lbl_1_bss_2C = 90.0f;
    lbl_1_bss_20 = mode;
}

void fn_1_5E28(void)
{
    lbl_1_bss_24 = 0.0f;
    if (lbl_1_bss_3C > 0) {
        lbl_1_bss_24 = (f64)lbl_1_bss_34 * sin((3.141592653589793 * lbl_1_bss_2C) / 180.0);
        lbl_1_bss_2C += lbl_1_bss_28;
        lbl_1_bss_34 *= lbl_1_bss_30;
        lbl_1_bss_3C -= 1;
        switch (lbl_1_bss_20) {
            case 2:
                Hu3DCameraPosSet(1, lbl_1_bss_7C.x, lbl_1_bss_7C.y, lbl_1_bss_7C.z, lbl_1_bss_70.x, lbl_1_bss_70.y,
                    lbl_1_bss_70.z, lbl_1_bss_64.x + lbl_1_bss_24, lbl_1_bss_64.y + lbl_1_bss_24, lbl_1_bss_64.z);
                return;
            case 1:
                Hu3DCameraPosSet(1, lbl_1_bss_7C.x, lbl_1_bss_7C.y, lbl_1_bss_7C.z, lbl_1_bss_70.x, lbl_1_bss_70.y,
                    lbl_1_bss_70.z, lbl_1_bss_64.x, lbl_1_bss_64.y + lbl_1_bss_24, lbl_1_bss_64.z);
                break;
        }
    }
}

s16 fn_1_6078(Vec *pos)
{
    Vec v;
    s16 r;

    Hu3D3Dto2D(pos, 1, &v);
    r = (s16)(32.0f + (64.0f * (v.x / 576.0f)));
    return r;
}

s16 fn_1_60F4(Vec *pos)
{
    Vec v;
    s16 r;

    Hu3D3Dto2D(pos, 1, &v);
    r = (s16)(64.0f + (63.0f * (v.y / 480.0f)));
    return r;
}

s16 fn_1_6170(void)
{
    s16 r;
    M561Game *g = GAME(*lbl_1_bss_C4);

    if (g->cur == 0) {
        r = 0x20;
    } else if (g->cur == 1) {
        r = 0x40;
    } else if (g->cur == 2) {
        r = 0x60;
    }
    return r;
}

void fn_1_61DC(s16 modelId, s16 motId, int lagF)
{
    if (lagF != 0) {
        HuAudFXPlay(0x995);
    }
}

void fn_1_6210(void)
{
    lbl_1_bss_2D4.prev = NULL;
    lbl_1_bss_2D4.next = NULL;
}

void fn_1_6234(u16 id, s16 val)
{
    M561Node *node;
    M561Node *old;

    node = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M561Node), HU_MEMNUM_OVL);
    node->id = id;
    node->val = val;
    old = lbl_1_bss_2D4.next;
    lbl_1_bss_2D4.next = node;
    if (old != NULL) {
        old->prev = node;
    }
    node->prev = &lbl_1_bss_2D4;
    node->next = old;
}

s16 fn_1_62BC(s16 id)
{
    M561Node *node;
    M561Node *prev;
    M561Node *next;
    s16 val;

    node = lbl_1_bss_2D4.next;
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

void fn_1_6360(void)
{
    s32 i;

    for (i = 0; i < 0x20; i++) {
        lbl_1_bss_CC[i].key = 0;
        lbl_1_bss_CC[i].model = -1;
    }
}

s16 fn_1_63B8(u32 key)
{
    s32 i;
    s32 j;

    for (i = 0; i < 0x20; i++) {
        if (key == lbl_1_bss_CC[i].key) {
            return Hu3DModelLink(lbl_1_bss_CC[i].model);
        }
    }
    for (j = 0; j < 0x20; j++) {
        if (lbl_1_bss_CC[j].key == 0) {
            lbl_1_bss_CC[j].key = key;
            lbl_1_bss_CC[j].model = Hu3DModelCreate(HuDataSelHeapReadNum(key, HU_MEMNUM_OVL, HEAP_MODEL));
            return lbl_1_bss_CC[j].model;
        }
    }
    return Hu3DModelCreate(HuDataSelHeapReadNum(key, HU_MEMNUM_OVL, HEAP_MODEL));
}

void fn_1_64CC(Vec *from, Vec *to, Vec *out, f32 t)
{
    f32 e;

    if (1.0f < t) {
        t = 1.0f;
    }
    e = sin((3.141592653589793 * (90.0f * t)) / 180.0) * sin((3.141592653589793 * (90.0f * t)) / 180.0);
    out->x = from->x + (e * (to->x - from->x));
    out->y = from->y + (e * (to->y - from->y));
    out->z = from->z + (e * (to->z - from->z));
}

void fn_1_6628(Vec *pos, Vec *up, Vec *target)
{
    Hu3DCameraPosSetV(1, pos, up, target);
    lbl_1_bss_2C4 = *pos;
    lbl_1_bss_2AC = *target;
}

s32 fn_1_66B4(Vec *up, f32 t)
{
    Vec pos;
    Vec target;
    f32 e1;
    f32 e2;
    f32 c1;
    f32 c2;

    c1 = t;
    if (1.0f < c1) {
        c1 = 1.0f;
    }
    e1 = sin((3.141592653589793 * (90.0f * c1)) / 180.0) * sin((3.141592653589793 * (90.0f * c1)) / 180.0);
    pos.x = lbl_1_bss_2C4.x + (e1 * (lbl_1_bss_2B8.x - lbl_1_bss_2C4.x));
    pos.y = lbl_1_bss_2C4.y + (e1 * (lbl_1_bss_2B8.y - lbl_1_bss_2C4.y));
    pos.z = lbl_1_bss_2C4.z + (e1 * (lbl_1_bss_2B8.z - lbl_1_bss_2C4.z));
    c2 = t;
    if (1.0f < c2) {
        c2 = 1.0f;
    }
    e2 = sin((3.141592653589793 * (90.0f * c2)) / 180.0) * sin((3.141592653589793 * (90.0f * c2)) / 180.0);
    target.x = lbl_1_bss_2AC.x + (e2 * (lbl_1_bss_2A0.x - lbl_1_bss_2AC.x));
    target.y = lbl_1_bss_2AC.y + (e2 * (lbl_1_bss_2A0.y - lbl_1_bss_2AC.y));
    target.z = lbl_1_bss_2AC.z + (e2 * (lbl_1_bss_2A0.z - lbl_1_bss_2AC.z));
    Hu3DCameraPosSetV(1, &pos, up, &target);
    if (1.0f <= t) {
        return 1;
    }
    lbl_1_bss_1D0 = 1;
    return 0;
}

s32 fn_1_69E8(Vec *up, f32 t)
{
    Vec pos;
    Vec target;
    f32 e1;
    f32 e2;
    f32 c1;
    f32 c2;
    s32 i;

    for (i = 0; i < 4; i++) {
        c1 = t;
        if (1.0f < c1) {
            c1 = 1.0f;
        }
        e1 = sin((3.141592653589793 * (90.0f * c1)) / 180.0) * sin((3.141592653589793 * (90.0f * c1)) / 180.0);
        pos.x = lbl_1_bss_270[i].x + (e1 * (lbl_1_bss_240[i].x - lbl_1_bss_270[i].x));
        pos.y = lbl_1_bss_270[i].y + (e1 * (lbl_1_bss_240[i].y - lbl_1_bss_270[i].y));
        pos.z = lbl_1_bss_270[i].z + (e1 * (lbl_1_bss_240[i].z - lbl_1_bss_270[i].z));
        c2 = t;
        if (1.0f < c2) {
            c2 = 1.0f;
        }
        e2 = sin((3.141592653589793 * (90.0f * c2)) / 180.0) * sin((3.141592653589793 * (90.0f * c2)) / 180.0);
        target.x = lbl_1_bss_210[i].x + (e2 * (lbl_1_bss_1E0[i].x - lbl_1_bss_210[i].x));
        target.y = lbl_1_bss_210[i].y + (e2 * (lbl_1_bss_1E0[i].y - lbl_1_bss_210[i].y));
        target.z = lbl_1_bss_210[i].z + (e2 * (lbl_1_bss_1E0[i].z - lbl_1_bss_210[i].z));
        Hu3DCameraPosSetV(1 << i, &pos, up, &target);
    }
    if (1.0f <= t) {
        return 1;
    }
    lbl_1_bss_1D0 = 1;
    return 0;
}

f32 fn_1_6DCC(f32 from, f32 to, f32 rate)
{
    f32 d;
    f32 r;

    d = fmod((f64)(to - from), 360.0);
    if (0.0f > d) {
        d += 360.0f;
    }
    if (180.0f < d) {
        d -= 360.0f;
    }
    r = fmod((f64)(from + (d * rate)), 360.0);
    if (0.0f > r) {
        r += 360.0f;
    }
    return r;
}

void fn_1_6ED0(s32 time)
{
    if (lbl_1_bss_1D4 >= 0) {
        if (time < 0) {
            time = 0x3E8;
        }
        HuAudSeqFadeOut(lbl_1_bss_1D4, time);
    }
    lbl_1_bss_1D4 = -1;
}

void fn_1_6F3C(s16 model, char *name, Vec *out)
{
    Mtx m;

    Hu3DModelObjMtxGet(model, name, m);
    out->x = m[0][3];
    out->y = m[1][3];
    out->z = m[2][3];
}

s32 fn_1_6F94(s32 lo, s32 hi)
{
    return lo + frandmod(hi - lo);
}

s32 fn_1_6FD0(Vec *a, Vec *b, f32 r1, f32 r2)
{
    if ((((b->x - a->x) * (b->x - a->x)) + ((b->z - a->z) * (b->z - a->z))) < ((r1 + r2) * (r1 + r2))) {
        return 1;
    }
    return 0;
}

s32 fn_1_7030(Vec *pos, s32 a, s32 b)
{
    Vec v;
    s32 r;

    Hu3D3Dto2D(pos, 1, &v);
    r = (s32)((f32)a + ((v.x / 576.0f) * (f32)(b - a)));
    return r;
}

s32 fn_1_70F4(Vec *pos, s32 a, s32 b)
{
    Vec v;
    s32 r;

    Hu3D3Dto2D(pos, 1, &v);
    r = (s32)((f32)a + ((v.y / 480.0f) * (f32)(b - a)));
    return r;
}

void fn_1_71B8(HU3DMODEL *modelP, HU3DPARTICLE *particleP, f32 mtx[3][4])
{
    M561Fx *fx = (M561Fx *)particleP;
    M561FxPart *part;
    u8 (*col)[4];
    M561Player *players[4];
    f32 t;
    f32 a;
    f32 s;
    f32 big;
    s16 i;
    s16 k;

    part = fx->parts;
    col = fx->colors;
    fx->active = 0;
    for (i = 0; i < fx->count; i++, part++, col++) {
        if (part->idx >= 0) {
            part->radius = part->radius + part->speed;
            if (part->maxRadius < part->radius) {
                part->idx = -1;
            } else {
                fx->active = fx->active + 1;
                if (fx->follow != NULL) {
                    part->origin.x = fx->follow->x;
                    part->origin.y = fx->follow->y;
                    part->origin.z = fx->follow->z;
                }
                if (fx->followRot != NULL) {
                    part->angle = *fx->followRot;
                }
                part->pos.x = (f64)part->origin.x + ((f64)part->radius * sin((3.141592653589793 * (part->angle + part->angleOff)) / 180.0));
                part->pos.y = part->pos.y + (0.03f * (part->targetY - part->pos.y));
                part->pos.z = (f64)part->origin.z + ((f64)part->radius * cos((3.141592653589793 * (part->angle + part->angleOff)) / 180.0));
                t = part->radius / part->maxRadius;
                a = 128.0 * cos((3.141592653589793 * (90.0f * t)) / 180.0);
                if (a > 255.0f) {
                    big = 255.0f;
                } else {
                    big = a;
                }
                (*col)[3] = (u8)big;
                part->frame = (f32)fx->frames * t;
                if (fx->frames <= part->frame) {
                    part->frame = fx->frames - 1;
                }
                s = t * 2.0f;
                if (s > 1.5f) {
                    s = 1.5f;
                }
                part->scale = 1.5 + (3.0 * sin((3.141592653589793 * (10.0f + (100.0f * s))) / 180.0));
                part->unk28 = 40.0f * part->scale;
                for (k = 0; k < 4; k++) {
                    players[k] = PLAYER(lbl_1_bss_C8[k]);
                }
            }
        }
    }
}

void fn_1_7568(Mtx src, Mtx dst)
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

void fn_1_75E4(Vec *src, Vec *dst, s16 count, f32 scale)
{
    s16 i;

    for (i = 0; i < count; i++, src++, dst++) {
        dst->x = src->x * scale;
        dst->y = src->y * scale;
        dst->z = src->z * scale;
    }
}

void fn_1_7640(f32 *out, f32 (*tab)[2], s16 idx, f32 w, f32 h)
{
    out[0] = tab[idx][0];
    out[1] = tab[idx][1];
    out[2] = w + tab[idx][0];
    out[3] = tab[idx][1];
    out[4] = w + tab[idx][0];
    out[5] = h + tab[idx][1];
    out[6] = tab[idx][0];
    out[7] = h + tab[idx][1];
}

void fn_1_76E4(HU3DMODEL *model, f32 mtx[3][4])
{
#ifdef TARGET_PC
    M561Fx *fx = (M561Fx *)model->hookData;
#else
    /* GC layout: hookData sits at 0x120; the cast keeps MWCC register allocation unchanged. */
    M561Fx *fx = (M561Fx *)((M561Hu3DModel *)model)->parm;
#endif
    M561FxPart *part;
    Vec *vtx;
    f32 (*uv)[2];
    Vec billboard[4];
    Vec scaled[4];
    Vec rotated[4];
    Mtx rm;
    Mtx rz;
    Mtx comb;
    ROMtx ro;
    f32 uw;
    f32 uh;
    s16 fmt;
    s16 i;
    s16 k;
    s16 fr;

    if ((fx->oneshot == 0) || (fx->active != 0)) {
        GXLoadPosMtxImm(mtx, 0);
        GXSetNumTevStages(1);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, 0, 0x7D);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        fmt = fx->anim->bmp->dataFmt & 0xF;
        if ((fmt == 8) || (fmt == 7)) {
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXA, GX_CC_RASC, GX_CC_ZERO);
        } else {
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_ZERO);
        }
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_CLAMP, GX_AF_NONE);
        if ((model->attr & 2) != 0) {
            GXSetZMode(1, GX_LEQUAL, 1);
        } else {
            GXSetZMode(1, GX_LEQUAL, 0);
        }
        HuSprTexLoad(fx->anim, 0, 0, 1, 1, 1);
        GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
        GXSetZCompLoc(0);
        switch (fx->blend) {
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
        GXSETARRAY(GX_VA_POS, fx->verts, fx->count * 4 * sizeof(Vec), 0xC, TRUE);
        GXSetVtxDesc(GX_VA_CLR0, GX_INDEX16);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_TEX_ST, GX_RGBA8, 0);
        GXSETARRAY(GX_VA_CLR0, fx->colors, fx->count * 4, 4, TRUE);
        GXSetVtxDesc(GX_VA_TEX0, GX_INDEX16);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_RGBA6, 0);
        GXSETARRAY(GX_VA_TEX0, fx->uvs, fx->count * 4 * sizeof(f32[2]), 8, TRUE);
        rm[0][0] = mtx[0][0];
        rm[1][0] = mtx[0][1];
        rm[2][0] = mtx[0][2];
        rm[0][1] = mtx[1][0];
        rm[1][1] = mtx[1][1];
        rm[2][1] = mtx[1][2];
        rm[0][2] = mtx[2][0];
        rm[1][2] = mtx[2][1];
        rm[2][2] = mtx[2][2];
        rm[0][3] = 0.0f;
        rm[1][3] = 0.0f;
        rm[2][3] = 0.0f;
        PSMTXReorder(rm, ro);
        if ((omPauseChk() == 0) && (fx->hook != NULL)) {
            HU3DPARTICLEHOOK hook = fx->hook;
            hook(model, (HU3DPARTICLE *)fx, mtx);
        }
        part = fx->parts;
        vtx = fx->verts;
        uv = fx->uvs;
        PSMTXROMultVecArray(ro, fx->corners, billboard, 4);
        for (i = 0; i < fx->count; i++, part++, uv += 4) {
            if (part->idx < 0) {
                vtx->x = 0.0f;
                vtx->y = 0.0f;
                vtx->z = 0.0f;
                vtx++;
                vtx->x = 0.0f;
                vtx->y = 0.0f;
                vtx->z = 0.0f;
                vtx++;
                vtx->x = 0.0f;
                vtx->y = 0.0f;
                vtx->z = 0.0f;
                vtx++;
                vtx->x = 0.0f;
                vtx->y = 0.0f;
                vtx->z = 0.0f;
                vtx++;
            } else {
                if (part->unk28 == 0.0f) {
                    fn_1_75E4(billboard, scaled, 4, part->scale);
                    PSVECAdd(&scaled[0], &part->pos, vtx);
                    vtx++;
                    PSVECAdd(&scaled[1], &part->pos, vtx);
                    vtx++;
                    PSVECAdd(&scaled[2], &part->pos, vtx);
                    vtx++;
                    PSVECAdd(&scaled[3], &part->pos, vtx);
                    vtx++;
                } else {
                    fn_1_75E4(fx->corners, scaled, 4, part->scale);
                    PSMTXRotRad(rz, 'Z', 0.017453292f * part->unk28);
                    PSMTXConcat(rm, rz, comb);
                    PSMTXMultVecArray(comb, scaled, rotated, 4);
                    PSVECAdd(&rotated[0], &part->pos, vtx);
                    vtx++;
                    PSVECAdd(&rotated[1], &part->pos, vtx);
                    vtx++;
                    PSVECAdd(&rotated[2], &part->pos, vtx);
                    vtx++;
                    PSVECAdd(&rotated[3], &part->pos, vtx);
                    vtx++;
                }
                if (part->frame != -1) {
                    uh = fx->vScale;
                    uw = fx->uScale;
                    fr = part->frame;
                    fn_1_7640((f32 *)uv, (f32 (*)[2])fx->unk1C, fr, uw, uh);
                }
            }
        }
        DCFlushRangeNoSync(fx->verts, fx->count * 0xC * 4);
        DCFlushRangeNoSync(fx->uvs, fx->count * 8 * 4);
        GXCallDisplayList(fx->unk24, fx->dlSize);
    }
}

void fn_1_7E2C(f32 (*tab)[2], s16 count, s16 cols, f32 w, f32 h)
{
    s16 i;

    for (i = 0; i < count; i++, tab++) {
        (*tab)[0] = (f32)(s16)(i % cols) * w;
        (*tab)[1] = (f32)(s16)(i / cols) * h;
    }
}

s16 fn_1_7EF8(ANIMDATA *anim, Vec *follow, f32 *followRot, s16 count, s16 cellW, s16 cellH, f32 size)
{
    M561Fx *fx;
    HU3DMODEL *model;
    HU3DMODELID id;
    M561FxPart *part;
    Vec *vtx;
    u8 (*col)[4];
    f32 (*uv)[2];
    f32 (*tab)[2];
    void *dl;
    s16 i;
    s16 cols;
    s16 rows;
    s16 frames;
    s16 cell;
    s16 cx;
    s16 cy;
    f32 neg;

    id = Hu3DHookFuncCreate((HU3DMODELHOOK)fn_1_76E4);
    model = &Hu3DData[id];
    model->hookData = HuMemDirectMallocNum(HEAP_MODEL, sizeof(M561Fx), model->mallocNo);
    fx = (M561Fx *)model->hookData;
    anim->useNum = anim->useNum + 1;
    fx->anim = anim;
    fx->follow = follow;
    fx->followRot = followRot;
    fx->count = count;
    fx->blend = 0;
    fx->hook = NULL;
    fx->active = count;
    fx->oneshot = 0;
    fx->parts = HuMemDirectMallocNum(HEAP_MODEL, count << 6, model->mallocNo);
    part = fx->parts;
    for (i = 0; i < count; i++, part++) {
        part->idx = -1;
        part->frame = -1;
    }
    fx->verts = HuMemDirectMallocNum(HEAP_MODEL, count * 0xC * 4, model->mallocNo);
    vtx = fx->verts;
    for (i = 0; i < (s32)(count * 4); i++, vtx++) {
        vtx->x = 0.0f;
        vtx->y = 0.0f;
        vtx->z = 0.0f;
    }
    fx->colors = HuMemDirectMallocNum(HEAP_MODEL, count * 4, model->mallocNo);
    col = fx->colors;
    for (i = 0; i < count; i++, col++) {
        (*col)[0] = 0xFF;
        (*col)[1] = 0xFF;
        (*col)[2] = 0xFF;
        (*col)[3] = 0xFF;
    }
    fx->uvs = HuMemDirectMallocNum(HEAP_MODEL, count * 8 * 4, model->mallocNo);
    uv = fx->uvs;
    for (i = 0; i < count; i++) {
        uv[0][0] = 0.0f;
        uv[0][1] = 0.0f;
        uv[1][0] = 1.0f;
        uv[1][1] = 0.0f;
        uv[2][0] = 1.0f;
        uv[2][1] = 1.0f;
        uv[3][0] = 0.0f;
        uv[3][1] = 1.0f;
        uv += 4;
    }
    if ((cellW != 0) || (cellH != 0)) {
        cols = anim->bmp->sizeX / cellW;
        rows = anim->bmp->sizeY / cellH;
        fx->frames = cols * rows;
        fx->uScale = 1.0f / (f32)cols;
        fx->vScale = 1.0f / (f32)rows;
    } else {
        cols = 1;
        fx->frames = 1;
        fx->uScale = 1.0f;
        fx->vScale = 1.0f;
    }
    fx->unk1C = HuMemDirectMallocNum(HEAP_MODEL, fx->frames * 8, model->mallocNo);
    frames = fx->frames;
    tab = fx->unk1C;
    for (cell = 0; cell < frames; cell++, tab++) {
        cx = cell % cols;
        cy = cell / cols;
        (*tab)[0] = (f32)cx * fx->uScale;
        (*tab)[1] = (f32)cy * fx->vScale;
    }
    neg = -size;
    fx->corners[3].y = neg;
    fx->corners[2].y = neg;
    fx->corners[3].x = neg;
    fx->corners[0].x = neg;
    fx->corners[1].y = size;
    fx->corners[0].y = size;
    fx->corners[2].x = size;
    fx->corners[1].x = size;
    fx->corners[3].z = 0.0f;
    fx->corners[2].z = 0.0f;
    fx->corners[1].z = 0.0f;
    fx->corners[0].z = 0.0f;
    dl = HuMemDirectMallocNum(HEAP_MODEL, 0x20000, model->mallocNo);
    GXBeginDisplayList(dl, 0x20000);
    GXBegin(GX_QUADS, GX_VTXFMT0, (u16)(count * 4));
    for (i = 0; i < count; i++) {
        GXPosition1x16(i * 4);
        GXColor1x16(i);
        GXTexCoord1x16(i * 4);
        GXPosition1x16(i * 4 + 1);
        GXColor1x16(i);
        GXTexCoord1x16(i * 4 + 1);
        GXPosition1x16(i * 4 + 2);
        GXColor1x16(i);
        GXTexCoord1x16(i * 4 + 2);
        GXPosition1x16(i * 4 + 3);
        GXColor1x16(i);
        GXTexCoord1x16(i * 4 + 3);
    }
    fx->dlSize = GXEndDisplayList();
    DCFlushRangeNoSync(dl, fx->dlSize);
    fx->unk24 = HuMemDirectMallocNum(HEAP_MODEL, fx->dlSize, model->mallocNo);
    memcpy(fx->unk24, dl, fx->dlSize);
    DCFlushRangeNoSync(fx->unk24, fx->dlSize);
    HuMemDirectFree(dl);
    return id;
}

M561FxPart *fn_1_8550(s16 id, Vec *pos, u8 *color, f32 scale, f32 rot)
{
    M561Fx *fx = FXOF(id);
    M561FxPart *part;
    s16 i;

    if (fx->oneshot != 0) {
        return NULL;
    }
    i = 0;
    part = fx->parts;
    while (i < fx->count) {
        if (part->idx >= 0) {
            i++;
            part++;
            continue;
        }
        break;
    }
    if (i == fx->count) {
        return NULL;
    }
    fx->colors[i][0] = color[0];
    fx->colors[i][1] = color[1];
    fx->colors[i][2] = color[2];
    fx->colors[i][3] = color[3];
    part->pos.x = pos->x;
    part->pos.y = pos->y;
    part->pos.z = pos->z;
    part->scale = scale;
    part->unk28 = rot;
    part->frame = 0;
    part->unk00 = 0;
    part->idx = i;
    return part;
}

void fn_1_8658(s16 id, HU3DPARTICLEHOOK hook)
{
    HU3DMODEL *model;
    M561Fx *fx;

    model = &Hu3DData[id];
    fx = (M561Fx *)model->hookData;
    fx->hook = hook;
}

void fn_1_8690(s16 id, u8 blend)
{
    HU3DMODEL *model;

    model = &Hu3DData[id];
    ((M561Fx *)model->hookData)->blend = blend;
}

void fn_1_86C0(s16 id)
{
    HU3DMODEL *model;
    M561Fx *fx;

    model = &Hu3DData[id];
    fx = (M561Fx *)model->hookData;
    fx->oneshot = 1;
}

s32 fn_1_86FC(s16 id)
{
    M561Fx *fx;
    HU3DMODEL *model;

    model = &Hu3DData[id];
    fx = (M561Fx *)model->hookData;

    if ((fx->oneshot != 0) && (fx->active == 0)) {
        HuMemDirectFree(fx->parts);
        HuMemDirectFree(fx->verts);
        HuMemDirectFree(fx->colors);
        HuMemDirectFree(fx->uvs);
        HuMemDirectFree(fx->unk1C);
        HuMemDirectFree(fx->unk24);
        HuSprAnimKill(fx->anim);
        HuMemDirectFree(model->hookData);
        Hu3DModelKill(id);
        return 1;
    }
    return 0;
}

void fn_1_87B8(s16 id, s16 count, Vec *pos, f32 yaw, f32 range, f32 targetY)
{
    M561FxPart *part;
    u8 color[4];
    f32 r;
    s16 n;

    color[0] = 0xFF;
    color[1] = 0xFF;
    color[2] = 0xFF;
    color[3] = 0xFF;
    for (n = 0; n < count; n++) {
        part = fn_1_8550(id, pos, color, 0.0f, 0.0f);
        if (part == NULL) {
            break;
        }
        part->origin.x = pos->x;
        part->origin.y = pos->y;
        part->origin.z = pos->z;
        part->radius = 0.0f;
        part->angle = yaw;
        part->speed = 10.0f;
        part->maxRadius = range;
        part->targetY = targetY;
        r = 20.0f * frandf();
        part->angleOff = r - 10.0f;
        r = 0.8f + (0.4f * frandf());
        part->speed = part->speed * r;
        r = 0.9f + (0.3f * frandf());
        part->maxRadius = part->maxRadius * r;
    }
}

s16 fn_1_8A18(Vec *follow, f32 *followRot, s16 count)
{
    s16 id;

    id = fn_1_7EF8(lbl_1_bss_1C, follow, followRot, count, 0x40, 0x40, 40.0f);
    FXOF(id)->hook = fn_1_71B8;
    FXOF(id)->blend = 0;
    Hu3DModelLayerSet(id, 5);
    return id;
}
