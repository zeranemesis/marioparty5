#include "game/process.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/esprite.h"
#include "game/flag.h"
#include "game/frand.h"
#include "game/gamemes.h"
#include "game/gamework.h"
#include "game/hsfex.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/printfunc.h"
#include "game/sprite.h"
#include "game/audio.h"
#include "game/wipe.h"
#include "game/mg/actman.h"
#include "game/mg/score.h"
#include "game/mg/seqman.h"

#include "humath.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

/* per-player work (omObj data), 0x90 bytes */
typedef struct M573Player_s {
    s16 motNo;
    u8 pad02[0xE];
    s32 no;
    s32 charNo;
    s32 padNo;
    s32 score;
    s32 state;
    s32 timer;
    Vec pos;
    Vec prevPos;
    Vec lastPos;
    Vec rot;
    f32 speed;
    Vec vel;
    f32 stkX;
    f32 stkY;
    s32 dif;
    s32 aiState;
    Vec target;
    s32 aiCnt;
    s32 aiWait;
    s32 aiTimer;
} M573_PLAYER;

/* tile object work, 0x38 bytes */
typedef struct M573Tile_s {
    Vec pos;
    Vec rot;
    Vec scale;
    s32 type;
    s32 idx;
    s32 state;
    s32 timer;
    s32 hitNo;
} M573_TILE;

/* hit effect work, 0x18 bytes */
typedef struct M573Effect_s {
    s32 count;
    s32 timer;
    s32 hitNo;
    Vec pos;
} M573_EFFECT;

/* master/NPC work, 0xB0 bytes */
typedef struct M573Master_s {
    s16 model;
    Vec pos;
    u8 pad10[0xC];
    Vec rot;
    f32 unk28;
    s32 motNo;
    s32 state;
    s32 nextState;
    s32 timer;
    f32 angle;
    f32 baseY;
    s32 moveTime;
    s32 hopTime;
    s32 hopRest;
    f32 hopDist;
    s16 hookModel;
    s16 itemModelL;
    s16 itemModelR;
    u8 pad5A[6];
    s32 spinCnt;
    f32 hookRotZ;
    f32 hookRotZSwing;
    f32 hookSwingAng;
    f32 hookRotX;
    f32 hookRotY;
    u8 pad78[4];
    s32 itemCnt;
    Vec dst;
    Vec srcPos;
    Vec step;
    s32 side;
    s16 gateModel[2];
    s16 propModel;
    s16 pad_AE;
} M573_MASTER;

typedef struct M573Node_s {
    u16 id;
    s16 val;
    struct M573Node_s *prev;
    struct M573Node_s *next;
} M573_NODE;

/* item work, 0x48 bytes */
typedef struct M573Item_s {
    s32 state;
    s32 frame;
    Vec pos;
    Vec rot;
    Vec dst;
    Vec vel;
    Vec src2d;
    s32 player;
} M573_ITEM;

static s32 lbl_1_data_0[13] = {
    0x960020, 0x960021, 0x960022, 0x960023, 0x960024, 0x960026, 0x960027,
    0x960028, 0x960029, 0x96002A, 0x96002B, 0x96002C, 0x96002D,
};
static u32 lbl_1_data_34[15] = {
    0x7E0008, 0x7E0009, 0x7E000A, 0x7E000D, 0x7E000E, 0x7E000F, 0x7E0010, 0x7E0011,
    0x7E0012, 0x7E0013, 0x7E0014, 0x7E0015, 0x7E0016, 0x7E000B, 0x7E000C,
};
static Vec lbl_1_data_70 = { 0.0f, 100.0f, -750.0f };
static f32 lbl_1_data_7C[4][2] = {
    { 71.0f, 60.0f }, { 191.0f, 60.0f }, { 385.0f, 60.0f }, { 505.0f, 60.0f },
};
static Vec lbl_1_data_9C[2] = { { -880.0f, 0.0f, -140.0f }, { 880.0f, 0.0f, -140.0f } };
static Vec lbl_1_data_B4[2] = { { 0.0f, 90.0f, 0.0f }, { 0.0f, 270.0f, 0.0f } };
static Vec lbl_1_data_CC[2] = { { -700.0f, 0.0f, -140.0f }, { 700.0f, 0.0f, -140.0f } };
static Vec lbl_1_data_E4 = { 0.0f, 10000.0f, 10000.0f };
static Vec lbl_1_data_F0 = { 0.0f, 0.0f, 0.0f };
static u8 lbl_1_data_FC[4] = { 0xFF, 0xFF, 0xFF, 0xFF };
static s32 lbl_1_data_1A4[7] = { 0x8C0000, 0x8C0006, 0x8C0031, 0x8C0001, 0x8C0002, 0x8C0069, 0x8C0018 };
static OMCAMERAVIEW lbl_1_data_1C0 = { { 0.0f, 221.5f, -500.0f }, { 4.4f, 0.0f, 0.0f }, 520.0f };
static OMCAMERAVIEW lbl_1_data_1DC = { { 0.0f, 183.4f, 86.1f }, { -52.3f, 0.0f, 0.0f }, 2220.0f };
static OMCAMERAVIEW lbl_1_data_1F8 = { { 0.0f, 221.5f, -500.0f }, { 4.4f, 0.0f, 0.0f }, 1200.0f };
static Vec lbl_1_data_214[4] = {
    { -75.0f, 0.0f, -75.0f }, { 75.0f, 0.0f, -75.0f }, { -75.0f, 0.0f, 75.0f }, { 75.0f, 0.0f, 75.0f },
};
static Vec lbl_1_data_244[5] = {
    { -240.0f, 0.0f, -450.0f }, { -80.36f, 0.0f, -450.0f }, { 80.36f, 0.0f, -450.0f },
    { 240.66f, 0.0f, -450.0f }, { 0.0f, 100.0f, -800.0f },
};
static Vec lbl_1_data_280 = { -100.0f, 8000.0f, 0.0f };
static Vec lbl_1_data_28C = { 0.0f, 1.0f, 0.0f };
static Vec lbl_1_data_298 = { 0.0f, 0.0f, 0.0f };

static Vec lbl_1_data_2CC[0x51] = {
    { -480.0f, 0.0f, -480.0f }, { -360.0f, 0.0f, -480.0f }, { -240.0f, 0.0f, -480.0f },
    { -120.0f, 0.0f, -480.0f }, { 0.0f, 0.0f, -480.0f }, { 120.0f, 0.0f, -480.0f },
    { 240.0f, 0.0f, -480.0f }, { 360.0f, 0.0f, -480.0f }, { 480.0f, 0.0f, -480.0f },
    { -480.0f, 0.0f, -360.0f }, { -360.0f, 0.0f, -360.0f }, { -240.0f, 0.0f, -360.0f },
    { -120.0f, 0.0f, -360.0f }, { 0.0f, 0.0f, -360.0f }, { 120.0f, 0.0f, -360.0f },
    { 240.0f, 0.0f, -360.0f }, { 360.0f, 0.0f, -360.0f }, { 480.0f, 0.0f, -360.0f },
    { -480.0f, 0.0f, -240.0f }, { -360.0f, 0.0f, -240.0f }, { -240.0f, 0.0f, -240.0f },
    { -120.0f, 0.0f, -240.0f }, { 0.0f, 0.0f, -240.0f }, { 120.0f, 0.0f, -240.0f },
    { 240.0f, 0.0f, -240.0f }, { 360.0f, 0.0f, -240.0f }, { 480.0f, 0.0f, -240.0f },
    { -480.0f, 0.0f, -120.0f }, { -360.0f, 0.0f, -120.0f }, { -240.0f, 0.0f, -120.0f },
    { -120.0f, 0.0f, -120.0f }, { 0.0f, 0.0f, -120.0f }, { 120.0f, 0.0f, -120.0f },
    { 240.0f, 0.0f, -120.0f }, { 360.0f, 0.0f, -120.0f }, { 480.0f, 0.0f, -120.0f },
    { -480.0f, 0.0f, 0.0f }, { -360.0f, 0.0f, 0.0f }, { -240.0f, 0.0f, 0.0f },
    { -120.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 120.0f, 0.0f, 0.0f },
    { 240.0f, 0.0f, 0.0f }, { 360.0f, 0.0f, 0.0f }, { 480.0f, 0.0f, 0.0f },
    { -480.0f, 0.0f, 120.0f }, { -360.0f, 0.0f, 120.0f }, { -240.0f, 0.0f, 120.0f },
    { -120.0f, 0.0f, 120.0f }, { 0.0f, 0.0f, 120.0f }, { 120.0f, 0.0f, 120.0f },
    { 240.0f, 0.0f, 120.0f }, { 360.0f, 0.0f, 120.0f }, { 480.0f, 0.0f, 120.0f },
    { -480.0f, 0.0f, 240.0f }, { -360.0f, 0.0f, 240.0f }, { -240.0f, 0.0f, 240.0f },
    { -120.0f, 0.0f, 240.0f }, { 0.0f, 0.0f, 240.0f }, { 120.0f, 0.0f, 240.0f },
    { 240.0f, 0.0f, 240.0f }, { 360.0f, 0.0f, 240.0f }, { 480.0f, 0.0f, 240.0f },
    { -480.0f, 0.0f, 360.0f }, { -360.0f, 0.0f, 360.0f }, { -240.0f, 0.0f, 360.0f },
    { -120.0f, 0.0f, 360.0f }, { 0.0f, 0.0f, 360.0f }, { 120.0f, 0.0f, 360.0f },
    { 240.0f, 0.0f, 360.0f }, { 360.0f, 0.0f, 360.0f }, { 480.0f, 0.0f, 360.0f },
    { -480.0f, 0.0f, 480.0f }, { -360.0f, 0.0f, 480.0f }, { -240.0f, 0.0f, 480.0f },
    { -120.0f, 0.0f, 480.0f }, { 0.0f, 0.0f, 480.0f }, { 120.0f, 0.0f, 480.0f },
    { 240.0f, 0.0f, 480.0f }, { 360.0f, 0.0f, 480.0f }, { 480.0f, 0.0f, 480.0f },
};

static s32 lbl_1_data_698[3][0x51] = {
    {
        1, 1, 0, -1, 1, -1, -1, 5, 1,
        -1, -1, 1, -1, 0, 1, 1, 0, -1,
        1, 0, 0, 1, 1, -1, -1, 0, 1,
        -1, 1, -1, 0, 0, 0, -1, 1, -1,
        5, 1, 0, 0, 0, 0, -1, 1, 5,
        -1, -1, 1, 0, 0, 0, 1, 0, -1,
        -1, 1, 1, 0, -1, 0, 1, 0, 1,
        5, 0, -1, 1, 1, 1, -1, 1, -1,
        -1, 1, 1, -1, 1, -1, 1, 0, 1,
    },
    {
        -1, 5, -1, -1, 1, 1, -1, 5, -1,
        1, 0, 1, 0, 1, 0, -1, 0, 1,
        -1, 1, -1, 1, -1, 1, 1, 0, 0,
        -1, 0, 1, 0, 0, 0, 1, -1, -1,
        1, 1, -1, 0, 0, 0, 0, 1, 1,
        0, -1, 1, 0, 0, 0, -1, 0, 1,
        1, 1, 0, 1, 0, -1, -1, 1, -1,
        -1, 1, -1, 1, 0, 1, 5, 1, 1,
        -1, 1, -1, 5, -1, 1, -1, 1, -1,
    },
    {
        -1, -1, 1, 1, 1, -1, -1, 1, 1,
        5, 0, 1, -1, 0, 1, 1, 0, -1,
        0, -1, -1, 1, 0, 1, -1, 1, 5,
        1, 1, 0, 0, 0, 0, 1, -1, 1,
        -1, 0, -1, 0, 0, 0, 0, 1, 0,
        -1, 1, 1, 0, 0, 0, -1, 1, -1,
        1, 1, -1, -1, 1, 1, 0, 0, 5,
        -1, 0, 1, 1, 1, -1, -1, 1, 1,
        -1, 5, -1, -1, 0, 1, 0, 1, -1,
    },
};

static s32 lbl_1_bss_0;
static s16 lbl_1_bss_4;
static s16 lbl_1_bss_6;
static s16 lbl_1_bss_8;
static s32 lbl_1_bss_C;
static s16 lbl_1_bss_10[2];
static OMOBJ *lbl_1_bss_14;
static OMOBJ *lbl_1_bss_18[0x51];
static OMOBJ **lbl_1_bss_15C;
static s16 lbl_1_bss_160;
static s32 lbl_1_bss_164;
static s32 lbl_1_bss_168;
static s16 lbl_1_bss_16C;
static void *lbl_1_bss_170[0x14];
static MGSCORE *lbl_1_bss_1C0[4];
static s16 lbl_1_bss_1D0[4];
static s16 lbl_1_bss_1D8[4];
static s16 lbl_1_bss_1E0[4];
static s16 lbl_1_bss_1E8[4];
static f32 lbl_1_bss_1F0[4];
static s32 lbl_1_bss_200[4];
static s32 lbl_1_bss_210[4];
static s32 lbl_1_bss_220[4];
static s32 lbl_1_bss_230[4];
static s32 lbl_1_bss_240;
static s32 lbl_1_bss_244[4];
static s16 lbl_1_bss_254[0x10];
static s32 lbl_1_bss_274;
static OMOBJ *lbl_1_bss_278[2];
static OMOBJ **lbl_1_bss_280;
static s32 lbl_1_bss_284_pad;
static struct {
    u32 unk0;
    s16 unk4;
} lbl_1_bss_284[0x20];
static s32 lbl_1_bss_388;
static s32 lbl_1_bss_38C[3];
static Vec lbl_1_bss_398[4];
static Vec lbl_1_bss_3C8[4];
static Vec lbl_1_bss_3F8[4];
static Vec lbl_1_bss_428[4];
static Vec lbl_1_bss_458;
static Vec lbl_1_bss_464;
static Vec lbl_1_bss_470;
static Vec lbl_1_bss_47C;
static OMOBJMAN *lbl_1_bss_488;
static M573_NODE lbl_1_bss_48C;

void ObjectSetup(void);
void fn_1_5C8(OMOBJ *obj);
void fn_1_8BC(OMOBJ *obj);
void fn_1_DAC(OMOBJ *obj);
s32 fn_1_EE4(OMOBJ *obj);
s32 fn_1_1028(Vec *pos, Vec *out);
void fn_1_12B8(M573_PLAYER *player, OMOBJ *obj);
s32 fn_1_171C(Vec *pos, f32 margin);
void fn_1_182C(Vec *a, Vec *b, f32 r1, f32 r2);
void fn_1_1A14(s16 mode, s16 frameNo);
void fn_1_1B78(s16 mode, s16 frameNo);
void fn_1_1C3C(s16 mode, s16 frameNo);
void fn_1_1CA4(s16 mode, s16 frameNo);
void fn_1_1DC4(s16 mode, s16 frameNo);
void fn_1_20B4(s16 mode, s16 frameNo);
void fn_1_2260(OMOBJ *obj, s32 motNo, f32 shift, u32 attr);
void fn_1_22EC(void);
void fn_1_22F0(OMOBJ *obj);
void fn_1_25B4(void);
void fn_1_2690(f32 x, f32 y, f32 z, s32 type, s32 idx);
s32 fn_1_2814(s32 idx, s32 hitNo);
void fn_1_2850(OMOBJ *obj);
void fn_1_28E8(void);
void fn_1_29CC(OMOBJ *obj);
void fn_1_2AE4(OMOBJ *obj);
s32 fn_1_2DA0(Vec *pos, f32 radius);
void fn_1_2F40(void);
void fn_1_3300(void);
void fn_1_3324(u16 id, s16 val);
s16 fn_1_33AC(s16 id);
void fn_1_3450(void);
s16 fn_1_34A8(u32 dataNum);
void fn_1_35BC(Vec *a, Vec *b, Vec *out, f32 t);
void fn_1_3718(Vec *pos, Vec *up, Vec *target);
s32 fn_1_37A4(Vec *up, f32 t);
s32 fn_1_3AD8(Vec *up, f32 t);
f32 fn_1_3EBC(f32 a, f32 b, f32 rate);
void fn_1_3FC0(s32 fade);
void fn_1_402C(s16 model, char *objName, Vec *out);
s32 fn_1_4084(s32 a, s32 b);
s32 fn_1_40C0(Vec *a, Vec *b, f32 r1, f32 r2);
s32 fn_1_4120(Vec *pos, s32 a, s32 b);
s32 fn_1_41E4(Vec *pos, s32 a, s32 b);
void fn_1_42A8(OMOBJ *obj, s32 mot, f32 shift, u32 attr);
void fn_1_43B0(OMOBJ *obj, s32 mot, f32 shift, u32 attr);
void fn_1_442C(OMOBJ *obj, s32 mot);
void fn_1_448C(OMOBJ *obj);
void fn_1_4614(OMOBJ *obj);
void fn_1_4794(OMOBJ *obj);
void fn_1_6BDC(OMOBJ *obj);
s32 fn_1_6FE4(void);
void fn_1_7034(OMOBJ *obj);
void fn_1_7364(OMOBJ *obj);
void fn_1_7898(OMOBJ *obj);
void fn_1_78CC(void);
void fn_1_792C(s32 value);
void fn_1_793C(s32 a, s32 b, s32 c, s32 d, s32 e);
void fn_1_79E4(s32 index, s32 value);
void fn_1_79FC(s32 type);
void fn_1_7AF0(OMOBJ *obj);
void fn_1_7BC0(OMOBJ *obj);
void fn_1_7EB4(OMOBJ *obj);
void fn_1_7FC4(Vec *pos, s32 player);
void fn_1_8114(OMOBJ *obj);
void fn_1_82C0(Vec *pos, s32 hitNo, s32 count);
s32 fn_1_8374(void);
void fn_1_83C8(void);
void fn_1_83DC(void);
void fn_1_840C(void);
s32 fn_1_843C(Vec *a, Vec *b, f32 r1, f32 r2);
s32 fn_1_8634(Vec *a, Vec *b, f32 r1, f32 r2);
f32 fn_1_8694(f32 x1, f32 z1, f32 x2, f32 z2);
s32 fn_1_8804(s32 a, s32 b);
s32 fn_1_8840(Vec *pos);
s32 fn_1_88BC(Vec *pos);

static MGSEQ_PARAM lbl_1_data_2A4 = {
    10, 0, (MGSEQ_FUNC)fn_1_1A14, (MGSEQ_FUNC)fn_1_1B78, NULL, (MGSEQ_FUNC)fn_1_1C3C,
    (MGSEQ_FUNC)fn_1_1CA4, (MGSEQ_FUNC)fn_1_1DC4, (MGSEQ_FUNC)fn_1_20B4, NULL, NULL,
};

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
    s32 i;
    OMOBJ *obj;
    s32 j;

    OSReport("******* M573 ObjectSetup *********\n");
    lbl_1_bss_488 = omInitObjMan(200, 0x2000);
    omGameSysInit(lbl_1_bss_488);
    Hu3DCameraCreate(1);
    Hu3DCameraPerspectiveSet(1, 30.0f, 20.0f, 15000.0f, 1.2f);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    omCameraViewSet(&lbl_1_data_1C0);
    lbl_1_bss_14 = omAddObjEx(lbl_1_bss_488, 0x7FDA, 0, 0, -1, omOutView);
    lbl_1_bss_14->work[0] = 2;
    lbl_1_bss_160 = Hu3DGLightCreate(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    Hu3DGLightPosAimSetV(lbl_1_bss_160, &lbl_1_data_E4, &lbl_1_data_F0);
    Hu3DGLightStaticSet(lbl_1_bss_160, 1);
    Hu3DGLightInfinitytSet(lbl_1_bss_160);
    Hu3DGLightColorSet(lbl_1_bss_160, lbl_1_data_FC[0], lbl_1_data_FC[1], lbl_1_data_FC[2], lbl_1_data_FC[3]);
    Hu3DShadowCreate(10.0f, 10.0f, 10000.0f);
    Hu3DShadowColSet(0x10, 0x10, 0x10);
    Hu3DShadowPosSet(&lbl_1_data_280, &lbl_1_data_28C, &lbl_1_data_298);
    Hu3DShadowTPLvlSet(0.6f);
    omMakeGroupEx(lbl_1_bss_488, 0, 4);
    lbl_1_bss_15C = omGetGroupMemberListEx(lbl_1_bss_488, 0);
    for (i = 0; i < 4; i++) {
        obj = omAddObjEx(lbl_1_bss_488, 0x64, 1, 7, 0, fn_1_5C8);
        lbl_1_bss_15C[i] = obj;
        obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M573_PLAYER), HU_MEMNUM_OVL);
        obj->work[0] = i;
    }
    omMakeGroupEx(lbl_1_bss_488, 2, 1);
    lbl_1_bss_280 = omGetGroupMemberListEx(lbl_1_bss_488, 2);
    obj = omAddObjEx(lbl_1_bss_488, 0x67, 0, 0, 2, fn_1_6BDC);
    *lbl_1_bss_280 = obj;
    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M573_MASTER), HU_MEMNUM_OVL);
    obj->work[0] = 2;
    lbl_1_bss_C = 0;
    omMakeGroupEx(lbl_1_bss_488, 1, 0x51);
    lbl_1_bss_8 = Hu3DModelCreateData(0x800003);
    lbl_1_bss_6 = Hu3DModelCreateData(0x800004);
    lbl_1_bss_4 = Hu3DModelCreateData(0x800005);
    Hu3DModelAttrSet(lbl_1_bss_8, 1);
    Hu3DModelAttrSet(lbl_1_bss_6, 1);
    Hu3DModelAttrSet(lbl_1_bss_4, 1);
    lbl_1_bss_16C = Hu3DModelCreateData(0x800003);
    Hu3DModelAttrSet(lbl_1_bss_16C, 1);
    for (j = 0; j < 0x14; j++) {
        lbl_1_bss_170[j] = NULL;
    }
    lbl_1_bss_164 = -1;
    fn_1_25B4();
    MgSeqCreatePrio(&lbl_1_data_2A4, 0x3E8);
}

void fn_1_5C8(OMOBJ *obj)
{
    M573_PLAYER *player;
    s32 i;

    player = obj->data;
    player->no = obj->work[0];
    player->charNo = GwPlayerConf[player->no].charNo;
    player->padNo = GwPlayerConf[player->no].padNo;
    player->score = 0;
    player->stkX = player->stkY = 0.0f;
    player->state = 0;
    player->timer = 0;
    obj->mdlId[0] = CharModelCreate(player->charNo, 2);
    for (i = 0; i < 7; i++) {
        obj->mtnId[i] = CharMotionCreate(player->charNo, lbl_1_data_1A4[i]);
        CharMotionSet(player->charNo, obj->mtnId[i]);
    }
    player->motNo = 0;
    Hu3DModelAttrSet(obj->mdlId[0], 1);
    Hu3DModelAttrSet(obj->mdlId[0], 0x40000001);
    Hu3DMotionSet(obj->mdlId[0], obj->mtnId[0]);
    Hu3DModelShadowSet(obj->mdlId[0]);
    player->dif = GwPlayerConf[player->no].dif;
    player->aiState = 0;
    player->aiTimer = 0;
    player->aiCnt = 0;
    switch (player->dif) {
        case 0:
            player->aiWait = frandmod(20) + 60;
            break;
        case 1:
            player->aiWait = frandmod(15) + 30;
            break;
        case 2:
            player->aiWait = frandmod(15) + 10;
            break;
        case 3:
            player->aiWait = frandmod(4) + 1;
            break;
    }
    player->rot.x = player->rot.y = player->rot.z = 0.0f;
    player->pos.x = lbl_1_data_214[player->no].x;
    player->pos.y = lbl_1_data_214[player->no].y;
    player->pos.z = lbl_1_data_214[player->no].z;
    omSetTra(obj, player->pos.x, player->pos.y, player->pos.z);
    omSetRot(obj, player->rot.x, player->rot.y, player->rot.z);
    Hu3DModelAttrReset(obj->mdlId[0], 1);
    CharMotionDataClose(player->charNo);
    obj->objFunc = fn_1_8BC;
}

void fn_1_8BC(OMOBJ *obj)
{
    M573_PLAYER *player;
    Vec pos2d;
    Vec pos2d2;
    f32 pan;
    s16 vol;

    player = obj->data;
    if (GwPlayerConf[player->no].type != 0) {
        fn_1_DAC(obj);
    }
    else {
        player->stkX = HuPadStkX[player->padNo];
        player->stkY = HuPadStkY[player->padNo];
    }
    switch (player->state) {
        case 0:
            if (MgSeqModeGet() == 5) {
                player->lastPos.x = player->pos.x;
                player->lastPos.y = player->pos.y;
                player->lastPos.z = player->pos.z;
                fn_1_12B8(player, obj);
                {
                    s32 hit = fn_1_2DA0(&player->pos, 50.0f);
                    if (hit != -1) {
                        if (fn_1_2814(hit, player->no) == -1) {
                            fn_1_2260(lbl_1_bss_15C[player->no], 5, 8.0f, 0);
                            player->state = 1;
                            omVibrate(player->padNo, 15, 12, 0);
                            Hu3D3Dto2D(&player->pos, 1, &pos2d);
                            HuAudFXPlayVolPan(0x97A, 96.0f + 31.0f * (pos2d.y / 480.0f),
                                32.0f + 64.0f * (pos2d.x / 576.0f));
                        }
                        else {
                            Hu3D3Dto2D(&player->pos, 1, &pos2d2);
                            vol = 96.0f + 31.0f * (pos2d2.y / 480.0f);
                            HuAudFXPlayVolPan(0x96C, vol, 32.0f + 64.0f * (pos2d2.x / 576.0f));
                        }
                    }
                }
            }
            break;
        case 1:
            player->stkX = player->stkY = 0.0f;
            if (player->timer > 0x3F) {
                player->timer = 0;
                player->state = 2;
                CharMotionUpdateSet(player->charNo, 0x8C0018, 0);
                fn_1_2260(lbl_1_bss_15C[player->no], 6, 8.0f, 0);
                Hu3DModelAttrReset(obj->mdlId[0], 1);
            }
            else {
                player->timer++;
            }
            break;
        case 2:
            player->stkX = player->stkY = 0.0f;
            if (player->timer > 0x26) {
                player->timer = 0;
                player->state = 0;
                fn_1_2260(lbl_1_bss_15C[player->no], 0, 8.0f, 0x40000001);
            }
            else {
                player->timer++;
            }
            break;
    }
    Hu3D3Dto2D(&player->pos, 1, &pos2d);
    pan = pos2d.x / 576.0f;
    vol = 32.0f + pan * 64.0f;
    CharModelVoicePanSet(player->charNo, vol);
    omSetTra(obj, player->pos.x, player->pos.y, player->pos.z);
    omSetRot(obj, player->rot.x, player->rot.y, player->rot.z);
}

void fn_1_DAC(OMOBJ *obj)
{
    M573_PLAYER *player;

    player = obj->data;
    switch (player->aiState) {
        case 0:
            if (fn_1_1028(&player->pos, &player->target) != 0) {
                player->aiState = 1;
                return;
            }
            player->aiState = 2;
            player->stkX = player->stkY = 0.0f;
            return;
        case 1:
            player->aiTimer++;
            if (player->aiTimer > 0x78) {
                player->aiTimer = 0;
                player->aiState = 0;
                return;
            }
            if (fn_1_EE4(obj) != 0) {
                player->aiState = 2;
                player->stkX = player->stkY = 0.0f;
                return;
            }
            return;
        case 2:
            player->stkX = player->stkY = 0.0f;
            player->aiCnt++;
            if (player->aiCnt > player->aiWait) {
                player->aiState = 0;
                player->aiCnt = 0;
            }
            break;
    }
}

s32 fn_1_EE4(OMOBJ *obj)
{
    f32 limit;
    M573_PLAYER *player;
    Vec dir;

    limit = 25.001f;
    player = obj->data;
    PSVECSubtract(&player->target, &player->pos, &dir);
    dir.y = 0.0f;
    if (PSVECSquareMag(&dir) < limit) {
        return 1;
    }
    PSVECNormalize(&dir, &dir);
    player->stkX = (s8)(56.0f * dir.x);
    player->stkY = (s8)(56.0f * -dir.z);
    return 0;
}

s32 fn_1_1028(Vec *pos, Vec *out)
{
    f32 dx;
    f32 dz;
    f32 dist;
    f32 minDist;
    Vec best;
    M573_TILE *tile;
    s32 i;

    minDist = 1000000.0f;
    for (i = 0; i < 0x51; i++) {
        if (lbl_1_bss_18[i] != NULL) {
            tile = lbl_1_bss_18[i]->data;
            if (tile->type != -1) {
                dx = pos->x - tile->pos.x;
                dz = pos->z - tile->pos.z;
                dist = sqrtf(dx * dx + dz * dz);
                if (minDist > dist) {
                    best.x = tile->pos.x;
                    best.y = tile->pos.y;
                    best.z = tile->pos.z;
                    minDist = dist;
                }
            }
        }
    }
    if (minDist != 1000000.0f) {
        out->x = best.x;
        out->y = best.y;
        out->z = best.z;
        return 1;
    }
    return 0;
}

void fn_1_12B8(M573_PLAYER *player, OMOBJ *obj)
{
    s32 unused;
    f32 len2;
    f32 ang;
    f32 newAng;
    s16 mot;

    if (player->stkX == 1.0f) {
        unused = 0;
    }
    player->vel.x = 0.0f;
    player->vel.y = 0.0f;
    player->vel.z = 0.0f;
    player->vel.x = player->stkX * 0.25f;
    player->vel.z = -player->stkY * 0.25f;
    mot = 0;
    len2 = player->vel.x * player->vel.x + player->vel.z * player->vel.z;
    player->speed = sqrtf(len2);
    if (player->speed > 0.0f) {
        if (10.0f <= player->speed) {
            player->vel.x = player->vel.x / player->speed;
            player->vel.z = player->vel.z / player->speed;
            player->speed = 10.0f;
            player->vel.x = 10.0f * player->vel.x;
            player->vel.z = 10.0f * player->vel.z;
        }
        newAng = player->rot.y;
        ang = fmod((f32)(180.0 * (atan2(player->vel.x, player->vel.z) / 3.141592653589793)) - newAng, 360.0);
        if (ang < 0.0f) {
            ang += 360.0f;
        }
        if (ang > 180.0f) {
            ang -= 360.0f;
        }
        newAng = fmod(newAng + ang * 0.4f, 360.0);
        if (newAng < 0.0f) {
            newAng += 360.0f;
        }
        player->rot.y = newAng;
        if (7.0f <= player->speed) {
            mot = 4;
        }
        else {
            mot = 3;
        }
    }
    PSVECAdd(&player->pos, &player->vel, &player->pos);
    fn_1_171C(&player->pos, 50.0f);
    fn_1_22F0(obj);
    fn_1_171C(&player->pos, 50.0f);
    player->prevPos.x = player->pos.x;
    player->prevPos.y = player->pos.y;
    player->prevPos.z = player->pos.z;
    if (player->motNo != mot) {
        player->motNo = mot;
        Hu3DMotionShiftSet(obj->mdlId[0], obj->mtnId[player->motNo], 0.0f, 8.0f, 0x40000001);
    }
}

s32 fn_1_171C(Vec *pos, f32 margin)
{
    s32 ret;
    f32 limit;
    f32 absX;
    f32 absZ;

    ret = 0;
    limit = 575.0f - margin;
    if (limit < pos->x) {
        pos->x = limit;
    }
    if (limit < pos->z) {
        pos->z = limit;
    }
    if (-limit > pos->x) {
        pos->x = -limit;
    }
    if (-limit > pos->z) {
        pos->z = -limit;
    }
    if (pos->x < 0.0f) {
        absX = -pos->x;
    }
    else {
        absX = pos->x;
    }
    if (limit == absX || limit == (pos->z < 0.0f ? -pos->z : pos->z)) {
        ret = 1;
    }
    return ret;
}

void fn_1_182C(Vec *a, Vec *b, f32 r1, f32 r2)
{
    f32 dx;
    f32 dz;
    f32 dist;
    f32 nx;
    f32 nz;

    dx = a->x - b->x;
    dz = a->z - b->z;
    dist = sqrtf(dx * dx + dz * dz);
    if (r1 + r2 > dist) {
        if (dist == 0.0f) {
            nx = 0.0f;
            nz = -1.0f;
        }
        else {
            nx = dx / dist;
            nz = dz / dist;
        }
        a->x = b->x + nx * (r1 + r2);
        a->z = b->z + nz * (r1 + r2);
    }
}

void fn_1_1A14(s16 mode, s16 frameNo)
{
    s32 i;
    s32 charNo[4];

    lbl_1_bss_10[0] = Hu3DModelCreateData(0x7E0000);
    lbl_1_bss_10[1] = Hu3DModelCreateData(0x7E0001);
    Hu3DModelShadowMapSet(lbl_1_bss_10[0]);
    Hu3DModelAttrSet(lbl_1_bss_10[1], 0x40000001);
    for (i = 0; i < 4; i++) {
        charNo[i] = ((M573_PLAYER *)lbl_1_bss_15C[i]->data)->charNo;
    }
    lbl_1_bss_244[0] = charNo[0];
    lbl_1_bss_244[1] = charNo[1];
    lbl_1_bss_244[2] = charNo[2];
    lbl_1_bss_244[3] = charNo[3];
    lbl_1_bss_240 = 2;
    omAddObjEx(lbl_1_bss_488, 0x65, 0, 0, -1, fn_1_7364);
    MgSeqModeNext();
    lbl_1_bss_164 = HuAudSStreamPlay(0x3D);
}

void fn_1_1B78(s16 mode, s16 frameNo)
{
    s32 i;
    M573_MASTER *master;

    master = (*lbl_1_bss_280)->data;
    for (i = 0; i < 0x1E; i++) {
        HuPrcVSleep();
    }
    master->state = 1;
    for (i = 0; i < 0x96; i++) {
        HuPrcVSleep();
    }
    omCameraViewMoveSimple(&lbl_1_data_1DC, 0x5A);
    for (i = 0; i < 0x1E; i++) {
        HuPrcVSleep();
    }
    master->timer = 0;
    master->unk28 = 1300.0f;
    master->state = 2;
    MgSeqModeNext();
}

void fn_1_1C3C(s16 mode, s16 frameNo)
{
    M573_MASTER *master;

    master = (*lbl_1_bss_280)->data;
    if (MgSeqFrameNoGet() == 0) {
        master->state = 0x18;
    }
    fn_1_22EC();
    if (lbl_1_bss_C == 0) {
        MgSeqModeNext();
    }
}

void fn_1_1CA4(s16 mode, s16 frameNo)
{
    M573_PLAYER *player;
    s32 i;
    s32 j;
    s32 ret;

    if (MgSeqFrameNoGet() == 0) {
        for (i = 0; i < 4; i++) {
            player = lbl_1_bss_15C[i]->data;
            if (player->state == 0) {
                fn_1_2260(lbl_1_bss_15C[player->no], 0, 8.0f, 0x40000001);
            }
        }
        HuAudSStreamFadeOut(lbl_1_bss_164, 100);
    }
    if (MgSeqFrameNoGet() > 300) {
        for (j = 0; j < 0x14; j++) {
            if (lbl_1_bss_170[j] != NULL) {
                ret = 1;
                break;
            }
        }
        if (j >= 0x14) {
            ret = 1;
        }
        if (ret != 0) {
            MgSeqModeNext();
        }
    }
}

void fn_1_1DC4(s16 mode, s16 frameNo)
{
    s32 i;
    M573_MASTER *master;
    M573_PLAYER *player;
    Vec pos2d;

    master = (*lbl_1_bss_280)->data;
    if (MgSeqFrameNoGet() == 0x1E) {
        WipeCreate(2, 6, 0x1E);
        WipeColorSet(0xFF, 0xFF, 0xFF);
        do {
            HuPrcVSleep();
        } while (WipeCheckEnd() != 0);
        omCameraViewSet(&lbl_1_data_1F8);
        for (i = 0; i < 4; i++) {
            player = lbl_1_bss_15C[i]->data;
            player->pos.x = lbl_1_data_244[i].x;
            player->pos.y = lbl_1_data_244[i].y;
            player->pos.z = lbl_1_data_244[i].z;
            player->rot.x = player->rot.y = player->rot.z = 0.0f;
            Hu3DModelAttrReset(lbl_1_bss_15C[player->no]->mdlId[0], 1);
            fn_1_2260(lbl_1_bss_15C[player->no], 0, 0.0f, 0x40000001);
            player->score = lbl_1_bss_230[player->no];
            Hu3D3Dto2D(&player->pos, 1, &pos2d);
            CharModelVoicePanSet(player->charNo, 32.0f + 64.0f * (pos2d.x / 576.0f));
        }
        master->state = 0;
        fn_1_42A8(*lbl_1_bss_280, 0, 0.0f, 0x40000001);
        Hu3DModelAttrReset(master->model, 1);
        master->pos.x = lbl_1_data_70.x;
        master->pos.y = lbl_1_data_70.y;
        master->pos.z = lbl_1_data_70.z;
        master->rot.x = master->rot.y = master->rot.z = 0.0f;
        fn_1_28E8();
        WipeCreate(1, 5, 0x1E);
        do {
            HuPrcVSleep();
        } while (WipeCheckEnd() != 0);
        MgSeqModeNext();
    }
}

void fn_1_20B4(s16 mode, s16 frameNo)
{
    s32 i;
    M573_MASTER *master;
    M573_PLAYER *player;
    s16 coin;
    s32 no;

    master = (*lbl_1_bss_280)->data;
    if (MgSeqFrameNoGet() == 0) {
        master->state = 6;
        master->timer = 0;
        for (i = 0; i < 4; i++) {
            player = lbl_1_bss_15C[i]->data;
            if (player->score >= 1) {
                fn_1_2260(lbl_1_bss_15C[player->no], 1, 8.0f, 0);
            }
            else {
                fn_1_2260(lbl_1_bss_15C[player->no], 2, 8.0f, 0);
                if (lbl_1_bss_230[0] == 0 && lbl_1_bss_230[1] == 0 && lbl_1_bss_230[2] == 0 && lbl_1_bss_230[3] == 0) {
                    CharFXPlay(player->charNo, 0x200);
                }
            }
            coin = player->score + GwPlayer[player->no].mgCoin;
            no = player->no;
            if (_CheckFlag(FLAG_MG_PRACTICE) == 0) {
                GwPlayer[no].mgCoin = coin;
            }
        }
    }
}

void fn_1_2260(OMOBJ *obj, s32 motNo, f32 shift, u32 attr)
{
    M573_PLAYER *player;

    player = obj->data;
    if (player->motNo != motNo) {
        player->motNo = motNo;
        CharMotionShiftSet(player->charNo, obj->mtnId[player->motNo], 0.0f, shift, attr);
    }
}

void fn_1_22EC(void)
{
}

void fn_1_22F0(OMOBJ *obj)
{
    M573_PLAYER *player;
    M573_PLAYER *other;
    Vec delta;
    Vec dir;
    Vec a;
    Vec b;
    f32 mag;
    f32 mag2;
    f32 dist;
    f32 push;
    f32 w;
    s32 i;

    player = obj->data;
    PSVECSubtract(&player->pos, &player->lastPos, &delta);
    mag = PSVECMag(&delta);
    if (mag != 0.0f) {
        for (i = 0; i < 4; i++) {
            if (i != player->no) {
                other = lbl_1_bss_15C[i]->data;
                a.x = player->pos.x;
                a.y = player->pos.y;
                a.z = player->pos.z;
                a.y = 0.0f;
                b.x = other->pos.x;
                b.y = other->pos.y;
                b.z = other->pos.z;
                b.y = 0.0f;
                PSVECSubtract(&a, &b, &delta);
                dist = PSVECMag(&delta);
                if (dist >= 99.5f) {
                }
                else {
                    PSVECSubtract(&other->pos, &other->lastPos, &delta);
                    mag2 = PSVECMag(&delta);
                    PSVECSubtract(&a, &b, &dir);
                    PSVECNormalize(&dir, &dir);
                    push = 100.0f - dist;
                    w = (0.6f * mag2 + 0.4f * mag) / (mag + mag2);
                    player->pos.x = player->pos.x + w * (push * dir.x);
                    player->pos.y = player->pos.y + w * (push * dir.y);
                    player->pos.z = player->pos.z + w * (push * dir.z);
                    w = (0.6f * mag + 0.4f * mag) / (mag + mag2);
                    other->pos.x = other->pos.x + w * (push * -dir.x);
                    other->pos.y = other->pos.y + w * (push * -dir.y);
                    other->pos.z = other->pos.z + w * (push * -dir.z);
                    fn_1_22F0(lbl_1_bss_15C[i]);
                }
            }
        }
    }
}

void fn_1_25B4(void)
{
    s32 map;
    s32 i;

    map = frandmod(3);
    lbl_1_bss_0 = map;
    for (i = 0; i < 0x51; i++) {
        if (lbl_1_data_698[map][i] != 0) {
            fn_1_2690(lbl_1_data_2CC[i].x, lbl_1_data_2CC[i].y, lbl_1_data_2CC[i].z, lbl_1_data_698[map][i], i);
        }
    }
}

void fn_1_2690(f32 x, f32 y, f32 z, s32 type, s32 idx)
{
    OMOBJ *obj;
    M573_TILE *tile;

    if (lbl_1_bss_C <= 0x50) {
        obj = omAddObjEx(lbl_1_bss_488, 0x65, 1, 0, 1, fn_1_29CC);
        obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M573_TILE), HU_MEMNUM_OVL);
        obj->work[0] = type;
        tile = obj->data;
        tile->pos.x = x;
        tile->pos.y = y;
        if (type == 1) {
            tile->rot.x = -30.0f;
            tile->pos.z = 30.0f + z;
        }
        else {
            tile->rot.x = 0.0f;
            tile->pos.z = z;
        }
        tile->rot.y = 0.0f;
        tile->rot.z = 0.0f;
        tile->type = type;
        tile->scale.x = tile->scale.y = tile->scale.z = 1.0f;
        tile->state = 0;
        tile->timer = 0;
        tile->idx = idx;
        if (tile->type != -1) {
            lbl_1_bss_C++;
        }
    }
}

s32 fn_1_2814(s32 idx, s32 hitNo)
{
    M573_TILE *tile;

    tile = lbl_1_bss_18[idx]->data;
    tile->state = 1;
    tile->hitNo = hitNo;
    return tile->type;
}

void fn_1_2850(OMOBJ *obj)
{
    M573_TILE *tile;

    tile = obj->data;
    Hu3DModelKill(obj->mdlId[0]);
    if (tile->type != -1) {
        lbl_1_bss_C--;
    }
    lbl_1_bss_18[tile->idx] = NULL;
    omDelObjEx(lbl_1_bss_488, obj);
}

void fn_1_28E8(void)
{
    s32 i;
    OMOBJ *obj;

    for (i = 0; i < 0x51; i++) {
        if (lbl_1_bss_18[i] != NULL) {
            obj = lbl_1_bss_18[i];
            fn_1_2850(obj);
        }
    }
}

void fn_1_29CC(OMOBJ *obj)
{
    M573_TILE *tile;

    tile = obj->data;
    if (obj->work[0] == 1) {
        obj->mdlId[0] = Hu3DModelLink(lbl_1_bss_8);
    }
    else if (obj->work[0] == 5) {
        obj->mdlId[0] = Hu3DModelLink(lbl_1_bss_6);
    }
    else {
        obj->mdlId[0] = Hu3DModelLink(lbl_1_bss_4);
    }
    Hu3DModelShadowSet(obj->mdlId[0]);
    omSetTra(obj, tile->pos.x, tile->pos.y, tile->pos.z);
    omSetRot(obj, tile->rot.x, tile->rot.y, tile->rot.z);
    omSetSca(obj, tile->scale.x, tile->scale.y, tile->scale.z);
    lbl_1_bss_18[tile->idx] = obj;
    Hu3DModelAttrReset(obj->mdlId[0], 1);
    obj->objFunc = fn_1_2AE4;
}

void fn_1_2AE4(OMOBJ *obj)
{
    M573_TILE *tile;
    M573_TILE *tile2;
    M573_EFFECT *eff;
    OMOBJ *effObj;
    s32 hitNo;

    tile = obj->data;
    switch (tile->state) {
        case 1:
            if (tile->type == -1) {
                if (tile->timer > 0x1E) {
                    fn_1_2850(obj);
                    return;
                }
                tile->timer++;
                if ((tile->timer & 1) != 0) {
                    Hu3DModelAttrSet(obj->mdlId[0], 1);
                }
                else {
                    Hu3DModelAttrReset(obj->mdlId[0], 1);
                }
                break;
            }
            fn_1_2850(obj);
            if (tile->type == 1) {
                hitNo = tile->hitNo;
                effObj = omAddObjEx(lbl_1_bss_488, 0x66, 0, 0, -1, fn_1_8114);
                effObj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M573_EFFECT), HU_MEMNUM_OVL);
                eff = effObj->data;
                eff->pos.x = tile->pos.x;
                eff->pos.y = tile->pos.y;
                eff->pos.z = tile->pos.z;
                eff->hitNo = hitNo;
                eff->count = 1;
                eff->timer = 0;
            }
            else if (tile->type == 5) {
                hitNo = tile->hitNo;
                effObj = omAddObjEx(lbl_1_bss_488, 0x66, 0, 0, -1, fn_1_8114);
                effObj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M573_EFFECT), HU_MEMNUM_OVL);
                eff = effObj->data;
                eff->pos.x = tile->pos.x;
                eff->pos.y = tile->pos.y;
                eff->pos.z = tile->pos.z;
                eff->hitNo = hitNo;
                eff->count = 5;
                eff->timer = 0;
            }
            break;
    }
    omSetTra(obj, tile->pos.x, tile->pos.y, tile->pos.z);
    omSetRot(obj, tile->rot.x, tile->rot.y, tile->rot.z);
    omSetSca(obj, tile->scale.x, tile->scale.y, tile->scale.z);
}

s32 fn_1_2DA0(Vec *pos, f32 radius)
{
    M573_TILE *tile;
    Vec a;
    Vec b;
    f32 r;
    s32 hit;
    s32 i;

    for (i = 0; i < 0x51; i++) {
        if (lbl_1_bss_18[i] != NULL) {
            tile = lbl_1_bss_18[i]->data;
            if (tile->state == 0) {
                a.x = lbl_1_data_2CC[i].x;
                a.y = lbl_1_data_2CC[i].y;
                a.z = lbl_1_data_2CC[i].z;
                b.x = pos->x;
                b.y = pos->y;
                b.z = pos->z;
                if (tile->type == 1) {
                    r = 25.0f;
                }
                else if (tile->type == 5) {
                    r = 40.0f;
                }
                else {
                    r = 40.0f;
                }
                if ((b.x - a.x) * (b.x - a.x) + (b.z - a.z) * (b.z - a.z) < (r + radius) * (r + radius)) {
                    hit = 1;
                }
                else {
                    hit = 0;
                }
                if (hit != 0) {
                    return i;
                }
            }
        }
    }
    return -1;
}

void fn_1_2F40(void)
{
    GXColor col;

    col.r = 0;
    col.g = 0;
    col.b = 0x90;
    col.a = 0xC0;
    CZoom += (f32)(HuPadTrigL[0] / 5);
    CZoom -= (f32)(HuPadTrigR[0] / 5);
    Center.x += 10.0f * ((f32)HuPadSubStkX[0] / 59.0f);
    Center.z = Center.z - 10.0f * ((f32)HuPadSubStkY[0] / 59.0f);
    CRot.x += 5.0f * ((f32)HuPadSubStkY[1] / 59.0f);
    CRot.y = CRot.y - 5.0f * ((f32)HuPadSubStkX[1] / 59.0f);
    printWin(0x10, 0x28, 0x180, 0x24, &col);
    fontcolor = 0xE;
    print8(0x10, 0x28, 1.5f, "Center X:%.02f Y:%.02f Z:%.02f", Center.x, Center.y, Center.z);
    print8(0x10, 0x34, 1.5f, "CRot X:%.02f Y:%.02f Z:%.02f", CRot.x, CRot.y, CRot.z);
    print8(0x10, 0x40, 1.5f, "CZoom:%.02f", CZoom);
    if ((HuPadBtnDown[0] & 0x400) != 0) {
        omCameraViewSet(&lbl_1_data_1DC);
    }
}

#define M573_SQ(x) ((x) * (x))

void fn_1_3300(void)
{
    lbl_1_bss_48C.prev = NULL;
    lbl_1_bss_48C.next = NULL;
}

void fn_1_3324(u16 id, s16 val)
{
    M573_NODE *node;
    M573_NODE *head;
    M573_NODE *old;

    node = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M573_NODE), HU_MEMNUM_OVL);
    node->id = id;
    node->val = val;
    head = &lbl_1_bss_48C;
    old = head->next;
    head->next = node;
    if (old != NULL) {
        old->prev = node;
    }
    node->prev = head;
    node->next = old;
}

s16 fn_1_33AC(s16 id)
{
    M573_NODE *node;
    M573_NODE *prev;
    M573_NODE *next;
    s16 val;

    node = lbl_1_bss_48C.next;
    while (1) {
        if (node == NULL) {
            return -1;
        }
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
}

void fn_1_3450(void)
{
    s32 i;

    for (i = 0; i < 0x20; i++) {
        lbl_1_bss_284[i].unk0 = 0;
        lbl_1_bss_284[i].unk4 = -1;
    }
}

s16 fn_1_34A8(u32 dataNum)
{
    s32 i;
    s32 j;

    for (i = 0; i < 0x20; i++) {
        if (dataNum == lbl_1_bss_284[i].unk0) {
            return Hu3DModelLink(lbl_1_bss_284[i].unk4);
        }
    }
    for (j = 0; j < 0x20; j++) {
        if (lbl_1_bss_284[j].unk0 == 0) {
            lbl_1_bss_284[j].unk0 = dataNum;
            lbl_1_bss_284[j].unk4 = Hu3DModelCreateData(dataNum);
            return lbl_1_bss_284[j].unk4;
        }
    }
    return Hu3DModelCreateData(dataNum);
}

void fn_1_35BC(Vec *a, Vec *b, Vec *out, f32 t)
{
    f32 k;
    f32 s;

    s = t;
    if (s > 1.0f) {
        s = 1.0f;
    }
    k = M573_SQ(sin(3.141592653589793 * (90.0f * s) / 180.0));
    out->x = a->x + k * (b->x - a->x);
    out->y = a->y + k * (b->y - a->y);
    out->z = a->z + k * (b->z - a->z);
}

void fn_1_3718(Vec *pos, Vec *up, Vec *target)
{
    Hu3DCameraPosSetV(1, pos, up, target);
    lbl_1_bss_47C = *pos;
    lbl_1_bss_464 = *target;
}

s32 fn_1_37A4(Vec *up, f32 t)
{
    Vec camPos;
    Vec camTarget;
    f32 k;
    f32 k2;
    f32 s;
    f32 s2;

    s = t;
    if (s > 1.0f) {
        s = 1.0f;
    }
    k = M573_SQ(sin(3.141592653589793 * (90.0f * s) / 180.0));
    camPos.x = lbl_1_bss_47C.x + k * (lbl_1_bss_470.x - lbl_1_bss_47C.x);
    camPos.y = lbl_1_bss_47C.y + k * (lbl_1_bss_470.y - lbl_1_bss_47C.y);
    camPos.z = lbl_1_bss_47C.z + k * (lbl_1_bss_470.z - lbl_1_bss_47C.z);
    s2 = t;
    if (s2 > 1.0f) {
        s2 = 1.0f;
    }
    k2 = M573_SQ(sin(3.141592653589793 * (90.0f * s2) / 180.0));
    camTarget.x = lbl_1_bss_464.x + k2 * (lbl_1_bss_458.x - lbl_1_bss_464.x);
    camTarget.y = lbl_1_bss_464.y + k2 * (lbl_1_bss_458.y - lbl_1_bss_464.y);
    camTarget.z = lbl_1_bss_464.z + k2 * (lbl_1_bss_458.z - lbl_1_bss_464.z);
    Hu3DCameraPosSetV(1, &camPos, up, &camTarget);
    if (t >= 1.0f) {
        return 1;
    }
    lbl_1_bss_388 = 1;
    return 0;
}

s32 fn_1_3AD8(Vec *up, f32 t)
{
    Vec camPos;
    Vec camTarget;
    f32 k;
    f32 k2;
    f32 s;
    f32 s2;
    s32 i;

    for (i = 0; i < 4; i++) {
        s = t;
        if (s > 1.0f) {
            s = 1.0f;
        }
        k = M573_SQ(sin(3.141592653589793 * (90.0f * s) / 180.0));
        camPos.x = lbl_1_bss_428[i].x + k * (lbl_1_bss_3F8[i].x - lbl_1_bss_428[i].x);
        camPos.y = lbl_1_bss_428[i].y + k * (lbl_1_bss_3F8[i].y - lbl_1_bss_428[i].y);
        camPos.z = lbl_1_bss_428[i].z + k * (lbl_1_bss_3F8[i].z - lbl_1_bss_428[i].z);
        s2 = t;
        if (s2 > 1.0f) {
            s2 = 1.0f;
        }
        k2 = M573_SQ(sin(3.141592653589793 * (90.0f * s2) / 180.0));
        camTarget.x = lbl_1_bss_3C8[i].x + k2 * (lbl_1_bss_398[i].x - lbl_1_bss_3C8[i].x);
        camTarget.y = lbl_1_bss_3C8[i].y + k2 * (lbl_1_bss_398[i].y - lbl_1_bss_3C8[i].y);
        camTarget.z = lbl_1_bss_3C8[i].z + k2 * (lbl_1_bss_398[i].z - lbl_1_bss_3C8[i].z);
        Hu3DCameraPosSetV(1 << i, &camPos, up, &camTarget);
    }
    if (t >= 1.0f) {
        return 1;
    }
    lbl_1_bss_388 = 1;
    return 0;
}

f32 fn_1_3EBC(f32 a, f32 b, f32 rate)
{
    f32 diff;
    f32 res;

    diff = fmod(b - a, 360.0);
    if (diff < 0.0f) {
        diff += 360.0f;
    }
    if (diff > 180.0f) {
        diff -= 360.0f;
    }
    res = fmod(a + diff * rate, 360.0);
    if (res < 0.0f) {
        res += 360.0f;
    }
    return res;
}

void fn_1_3FC0(s32 fade)
{
    if (lbl_1_bss_38C[0] >= 0) {
        if (fade < 0) {
            fade = 0x3E8;
        }
        HuAudSeqFadeOut(lbl_1_bss_38C[0], fade);
    }
    lbl_1_bss_38C[0] = -1;
}

void fn_1_402C(s16 model, char *objName, Vec *out)
{
    Mtx mtx;

    Hu3DModelObjMtxGet(model, objName, mtx);
    out->x = mtx[0][3];
    out->y = mtx[1][3];
    out->z = mtx[2][3];
}

s32 fn_1_4084(s32 a, s32 b)
{
    return a + frandmod(b - a);
}

s32 fn_1_40C0(Vec *a, Vec *b, f32 r1, f32 r2)
{
    if ((b->x - a->x) * (b->x - a->x) + (b->z - a->z) * (b->z - a->z) < (r1 + r2) * (r1 + r2)) {
        return 1;
    }
    return 0;
}

s32 fn_1_4120(Vec *pos, s32 a, s32 b)
{
    Vec out;
    s32 ret;

    Hu3D3Dto2D(pos, 1, &out);
    ret = (s32)((f32)a + (out.x / 576.0f) * (f32)(b - a));
    return ret;
}

s32 fn_1_41E4(Vec *pos, s32 a, s32 b)
{
    Vec out;
    s32 ret;

    Hu3D3Dto2D(pos, 1, &out);
    ret = (s32)((f32)a + (out.y / 480.0f) * (f32)(b - a));
    return ret;
}

void fn_1_42A8(OMOBJ *obj, s32 mot, f32 shift, u32 attr)
{
    M573_MASTER *master;

    master = obj->data;
    if (mot == 8) {
        Hu3DMotionSpeedSet(master->model, 2.0f);
    }
    else {
        Hu3DMotionSpeedSet(master->model, 1.0f);
    }
    if (master->motNo != mot) {
        if (mot == 2 || mot == 13) {
            HuAudFXPlay(0x2DB);
        }
        else if (mot == 14) {
            HuAudFXPlay(0x2DD);
        }
        master->motNo = mot;
        Hu3DMotionShiftSet(master->model, lbl_1_bss_254[mot], 0.0f, shift, attr);
        if (mot == 0xE) {
            HuAudJinglePlay(0x7A);
        }
        if (mot == 0xD) {
            HuAudJinglePlay(0x79);
        }
    }
}

void fn_1_43B0(OMOBJ *obj, s32 mot, f32 shift, u32 attr)
{
    M573_MASTER *master;

    master = obj->data;
    master->motNo = mot;
    Hu3DMotionShiftSet(master->model, lbl_1_bss_254[mot], 0.0f, shift, attr);
}

void fn_1_442C(OMOBJ *obj, s32 mot)
{
    M573_MASTER *master;

    master = obj->data;
    master->motNo = mot;
    Hu3DMotionSet(master->model, lbl_1_bss_254[mot]);
}

void fn_1_448C(OMOBJ *obj)
{
    M573_MASTER *master;

    master = obj->data;
    if (master->itemCnt % 3 == 0) {
        Hu3DModelHookObjReset(master->model, "a-itemhook-l");
        Hu3DModelHookSet(master->model, "a-itemhook-r", master->itemModelL);
        Hu3DModelAttrReset(master->itemModelL, 1);
        Hu3DModelAttrSet(master->itemModelR, 1);
        return;
    }
    if (master->itemCnt % 3 == 1) {
        Hu3DModelHookObjReset(master->model, "a-itemhook-r");
        Hu3DModelHookSet(master->model, "a-itemhook-l", master->itemModelR);
        Hu3DModelAttrReset(master->itemModelR, 1);
        Hu3DModelAttrSet(master->itemModelR, 0x40000002);
        Hu3DMotionTimeSet(master->itemModelR, 0.0f);
        Hu3DModelAttrSet(master->itemModelL, 1);
        return;
    }
    if (master->itemCnt % 3 == 2) {
        Hu3DModelHookObjReset(master->model, "a-itemhook-r");
        Hu3DModelHookObjReset(master->model, "a-itemhook-l");
        Hu3DModelAttrSet(master->itemModelL, 1);
        Hu3DModelAttrSet(master->itemModelR, 1);
    }
}

void fn_1_4614(OMOBJ *obj)
{
    M573_MASTER *master;

    master = obj->data;
    if (master->itemCnt % 3 == 0) {
        master->rot.y = 90.0f;
        Hu3DModelHookObjReset(master->hookModel, "itemhook1");
        Hu3DModelHookSet(master->hookModel, "itemhook2", master->model);
        master->hookRotY = 180.0f;
        return;
    }
    if (master->itemCnt % 3 == 1) {
        master->rot.y = 270.0f;
        Hu3DModelHookObjReset(master->hookModel, "itemhook2");
        Hu3DModelHookSet(master->hookModel, "itemhook1", master->model);
        master->hookRotY = 0.0f;
        return;
    }
    if (master->itemCnt % 3 == 2) {
        master->rot.y = 90.0f;
        Hu3DModelHookObjReset(master->hookModel, "itemhook1");
        Hu3DModelHookSet(master->hookModel, "itemhook2", master->model);
        master->hookRotY = 180.0f;
    }
}

void fn_1_4794(OMOBJ *obj)
{
    M573_MASTER *master;
    s32 mot;

    master = obj->data;
    switch (master->state) {
        case 1:
            fn_1_42A8(obj, 2, 8.0f, 0);
            break;
        case 2:
            if (master->timer == 0) {
                fn_1_42A8(obj, 3, 8.0f, 0);
                HuAudFXPlay(0x969);
            }
            else if (master->timer > 5) {
                if (master->timer > 0x1E) {
                    master->state = 0;
                    fn_1_42A8(obj, 0, 8.0f, 0x40000001);
                    Hu3DModelAttrSet(master->model, 1);
                }
                else {
                    master->pos.y = master->baseY + 300.0 * sin(3.141592653589793 * master->angle / 180.0);
                    master->angle += 3.6f;
                }
            }
            master->timer++;
            break;
        case 3:
            master->rot.y = 90.0f;
            master->pos.x = master->pos.y = master->pos.z = 0.0f;
            master->itemCnt = 0;
            fn_1_4614(obj);
            fn_1_448C(obj);
            mot = master->itemCnt % 3 + 5;
            fn_1_42A8(obj, mot, 0.0f, 0x40000002);
            Hu3DMotionSpeedSet(master->model, 0.0f);
            master->hookRotZSwing = -60.0f;
            master->hookRotZ = 0.0f;
            master->hookSwingAng = -90.0f;
            master->hookRotX = 0.0f;
            master->hookRotY = 180.0f;
            Hu3DModelRotSet(master->hookModel, master->hookRotX, master->hookRotY, master->hookRotZ + master->hookRotZSwing);
            Hu3DModelAttrReset(master->model, 1);
            Hu3DModelAttrReset(master->hookModel, 1);
            master->state = 4;
            break;
        case 4:
            master->hookRotZSwing = (90.0 * sin(3.141592653589793 * master->hookSwingAng / 180.0)) / 1.5;
            master->hookSwingAng += 1.0f;
            master->spinCnt++;
            Hu3DModelRotSet(master->hookModel, master->hookRotX, master->hookRotY, master->hookRotZ + master->hookRotZSwing);
            if (master->spinCnt > 0xB4) {
                master->state = 5;
                master->timer = 0;
            }
            else {
                if (master->spinCnt == 0x1E) {
                    Hu3DModelAttrReset(master->model, 0x40000002);
                    Hu3DMotionSpeedSet(master->model, 1.0f);
                    if (master->itemCnt % 3 == 1) {
                        Hu3DModelAttrReset(master->itemModelR, 0x40000002);
                        Hu3DModelAttrSet(master->itemModelR, 0);
                    }
                }
                if (master->itemCnt == 1 && master->spinCnt == 0x66) {
                    HuAudFXPlay(0x96A);
                }
            }
            break;
        case 5:
            if (master->timer > 0x1E) {
                master->state = 4;
                master->timer = 0;
                master->spinCnt = 0;
                master->itemCnt++;
                fn_1_4614(obj);
                fn_1_448C(obj);
                mot = master->itemCnt % 3 + 5;
                fn_1_42A8(obj, mot, 0.0f, 0x40000002);
                Hu3DMotionSpeedSet(master->model, 0.0f);
                if (master->itemCnt >= 3) {
                    master->state = 0;
                }
            }
            else {
                master->timer++;
            }
            break;
        case 6:
            if (master->timer == 0) {
                if (fn_1_6FE4() == 0) {
                    fn_1_42A8(obj, 0xE, 8.0f, 0);
                }
                else {
                    fn_1_42A8(obj, 0xD, 8.0f, 0);
                }
            }
            break;
        case 7:
            master->srcPos.x = master->pos.x;
            master->srcPos.y = master->pos.y;
            master->srcPos.z = master->pos.z;
            master->step.x = (master->dst.x - master->srcPos.x) / (f32)master->moveTime;
            master->step.y = (master->dst.y - master->srcPos.y) / (f32)master->moveTime;
            master->step.z = (master->dst.z - master->srcPos.z) / (f32)master->moveTime;
            master->rot.y = 180.0 * (atan2(master->dst.x - master->pos.x, master->dst.z - master->pos.z) / 3.141592653589793);
            master->state = 8;
            fn_1_42A8(obj, 1, 8.0f, 0x40000001);
            break;
        case 8:
            if (master->timer > master->moveTime) {
                if (master->nextState != -1) {
                    master->state = master->nextState;
                    master->nextState = -1;
                    master->timer = 0;
                }
                else {
                    master->state = 0;
                }
            }
            else {
                master->timer++;
                master->pos.x = master->pos.x + master->step.x;
                master->pos.y = master->pos.y + master->step.y;
                master->pos.z = master->pos.z + master->step.z;
            }
            break;
        case 9:
            master->angle = 0.0f;
            master->baseY = master->pos.y;
            master->state = 10;
            break;
        case 10:
            if (master->timer > master->hopTime) {
                if (master->hopRest > 0) {
                    master->state = 11;
                    master->timer = 0;
                }
                else if (master->nextState != -1) {
                    master->state = master->nextState;
                    master->nextState = -1;
                    master->timer = 0;
                }
                else {
                    master->state = 0;
                }
                fn_1_42A8(obj, 0, 8.0f, 0x40000001);
            }
            else {
                fn_1_42A8(obj, 3, 8.0f, 0);
                master->pos.y = master->baseY + 100.0 * sin(3.141592653589793 * master->angle / 180.0);
                master->pos.x = master->pos.x + (master->hopDist * sin(3.141592653589793 * master->rot.y / 180.0)) / master->hopTime;
                master->pos.z = master->pos.z + (master->hopDist * cos(3.141592653589793 * master->rot.y / 180.0)) / master->hopTime;
                master->timer++;
                master->angle += 180.0f / (f32)master->hopTime;
            }
            break;
        case 11:
            if (master->timer > master->hopRest) {
                if (master->nextState != -1) {
                    master->state = master->nextState;
                    master->nextState = -1;
                    master->timer = 0;
                }
                else {
                    master->state = 0;
                }
            }
            else {
                master->timer++;
                master->pos.y = master->baseY + 100.0 * sin(3.141592653589793 * (master->angle + (f32)master->timer) / 180.0);
            }
            break;
        case 12:
            fn_1_42A8(obj, 1, 8.0f, 0x40000001);
            if (master->timer > 0x1E) {
                master->state = 7;
                master->dst.x = 180.0f;
                master->dst.y = master->pos.y;
                master->dst.z = -800.0f;
                master->moveTime = 0x1E;
                master->nextState = 0xD;
                master->timer = 0;
            }
            else {
                master->timer++;
                master->rot.y += 3.6666667f;
            }
            break;
        case 13:
            master->state = 9;
            master->nextState = 0xF;
            master->hopTime = 0x28;
            master->hopRest = 0;
            master->hopDist = 200.0f;
            master->timer = 0;
            break;
        case 14:
            master->state = 7;
            master->dst.x = 410.0f;
            master->dst.y = 0.0f;
            master->dst.z = -880.0f;
            master->moveTime = 0x1E;
            master->nextState = 0xF;
            master->timer = 0;
            break;
        case 15:
            fn_1_42A8(obj, 8, 8.0f, 0x40000001);
            if (master->timer > 0xB4) {
                master->rot.x = 0.0f;
                master->state = 9;
                master->nextState = 0x10;
                master->hopTime = 0x2D;
                master->hopRest = 0xF;
                master->hopDist = 100.0f;
                master->timer = 0;
            }
            else {
                master->timer++;
                master->pos.x = master->pos.x + -0.075f;
                master->pos.y = master->pos.y + 4.3333335f;
                master->pos.z = master->pos.z + -0.575f;
                master->rot.y = 215.0f;
                master->rot.x = 10.0f;
            }
            break;
        case 16:
            master->state = 7;
            master->dst.x = 220.0f;
            master->dst.y = 860.0f;
            master->dst.z = -1050.0f;
            master->moveTime = 0x1E;
            master->nextState = 0x11;
            master->timer = 0;
            break;
        case 17:
            fn_1_42A8(obj, 0xA, 8.0f, 0x40000001);
            if (master->timer > 0x78) {
                master->state = 7;
                master->dst.x = 10.0f;
                master->dst.y = 860.0f;
                master->dst.z = -1070.0f;
                master->moveTime = 0x1E;
                master->nextState = 0x12;
                master->timer = 0;
            }
            else {
                master->rot.y = fn_1_3EBC(master->rot.y, 0.0f, 0.5f);
                master->timer++;
            }
            break;
        case 18:
            fn_1_42A8(obj, 8, 8.0f, 0x40000001);
            if (master->timer > 0xB4) {
                master->rot.x = 0.0f;
                master->state = 9;
                master->nextState = 0x13;
                master->hopTime = 0x2D;
                master->hopRest = 0x14;
                master->hopDist = 100.0f;
                master->timer = 0;
            }
            else {
                master->timer++;
                master->pos.y = master->pos.y + 3.2777777f;
                master->rot.y = -161.0f;
                master->rot.x = 10.0f;
            }
            break;
        case 19:
            master->state = 7;
            master->dst.x = -180.0f;
            master->dst.y = 1450.0f;
            master->dst.z = -1150.0f;
            master->moveTime = 0x1E;
            master->nextState = 0x15;
            master->baseY = master->pos.y;
            master->angle = 0.0f;
            master->timer = 0;
            break;
        case 20:
            fn_1_42A8(obj, 9, 8.0f, 0x40000001);
            if (master->timer > 0x78) {
                master->state = 0;
                master->timer = 0;
            }
            else {
                master->rot.y = fn_1_3EBC(master->rot.y, 0.0f, 0.5f);
                master->timer++;
            }
            break;
        case 21:
            if ((f32)master->timer > 60.0f) {
                master->state = 0x16;
                fn_1_42A8(obj, 4, 8.0f, 0);
                master->timer = 0;
            }
            else {
                fn_1_42A8(obj, 3, 8.0f, 0);
                master->pos.y = master->baseY + 450.0 * sin(3.141592653589793 * master->angle / 180.0);
                master->pos.x = master->pos.x + (200.0 * sin(3.141592653589793 * master->rot.y / 180.0)) / 90.0;
                master->pos.z = master->pos.z + (200.0 * cos(3.141592653589793 * master->rot.y / 180.0)) / 90.0;
                master->timer++;
                master->angle += 2.0f;
            }
            break;
        case 22:
            master->timer++;
            if (master->timer > 0x1E) {
                fn_1_42A8(obj, 9, 8.0f, 0x40000001);
            }
            master->rot.y = fn_1_3EBC(master->rot.y, 0.0f, 0.5f);
            break;
        case 24:
            if (master->timer > 0x28) {
                master->timer = 0;
                master->state = 0x19;
            }
            else {
                master->timer++;
            }
            break;
        case 25:
            master->pos.x = lbl_1_data_9C[master->side].x;
            master->pos.y = lbl_1_data_9C[master->side].y;
            master->pos.z = lbl_1_data_9C[master->side].z;
            master->rot.x = lbl_1_data_B4[master->side].x;
            master->rot.y = lbl_1_data_B4[master->side].y;
            master->rot.z = lbl_1_data_B4[master->side].z;
            Hu3DModelAttrReset(master->model, 1);
            master->state = 0x1A;
            fn_1_43B0(obj, 1, 8.0f, 0x40000001);
            Hu3DModelAttrReset(master->gateModel[master->side], 0x40000002);
            Hu3DMotionTimeSet(master->gateModel[master->side], 0.0f);
            if (master->side == 0) {
                HuAudFXPlay(0x978);
            }
            else if (master->side == 1) {
                HuAudFXPlay(0x979);
            }
            break;
        case 26:
            if (master->side % 2 == 0) {
                master->pos.x = master->pos.x + 2.0f;
            }
            else {
                master->pos.x = master->pos.x - 2.0f;
            }
            if (master->timer > 0x64) {
                master->timer = 0;
                master->state = 0x1C;
                if (master->side % 2 == 0) {
                    fn_1_42A8(obj, 0xB, 8.0f, 0);
                    Hu3DModelHookSet(master->model, "a-itemhook-l", master->propModel);
                    Hu3DModelAttrReset(master->propModel, 1);
                    Hu3DModelAttrReset(master->propModel, 0x40000002);
                }
                else {
                    fn_1_42A8(obj, 0xC, 8.0f, 0);
                }
            }
            else {
                master->timer++;
            }
            break;
        case 27:
            if (master->side % 2 == 0) {
                master->pos.x = master->pos.x - 3.3333333f;
            }
            else {
                master->pos.x = master->pos.x + 3.3333333f;
            }
            if (master->timer > 0x3C) {
                master->timer = 0;
                master->state = 0x18;
                master->side++;
                if (master->side >= 2) {
                    master->side = 0;
                    fn_1_43B0(obj, 0, 0.0f, 0);
                    master->state = 0x17;
                }
            }
            else {
                master->timer++;
            }
            break;
        case 28:
            if (master->timer > 0x70) {
                master->timer = 0;
                master->state = 0x1B;
                fn_1_43B0(obj, 1, 8.0f, 0);
                Hu3DModelHookObjReset(master->model, "a-itemhook-l");
                Hu3DModelAttrSet(master->propModel, 1);
            }
            else {
                master->timer++;
            }
            if (master->side == 0 && master->timer == 0x34) {
                HuAudFXPlayPan(0x96A, 0x20);
            }
            break;
    }
    Hu3DModelPosSet(master->model, master->pos.x, master->pos.y, master->pos.z);
    Hu3DModelRotSet(master->model, master->rot.x, master->rot.y, master->rot.z);
}

void fn_1_6BDC(OMOBJ *obj)
{
    M573_MASTER *master;
    s32 i;
    s32 j;

    master = obj->data;
    master->model = Hu3DModelCreateData(0x7E0007);
    switch (obj->work[0]) {
        case 0:
            master->hookModel = Hu3DModelCreateData(0x7E0017);
            Hu3DModelAttrSet(master->hookModel, 1);
            Hu3DModelPosSet(master->hookModel, 0.0f, 1050.0f, 300.0f);
            Hu3DModelRotSet(master->hookModel, 0.0f, 180.0f, 0.0f);
            Hu3DMotionClusterSpeedSet(master->hookModel, 0, 1.0f);
            Hu3DModelClusterAttrSet(master->hookModel, 0, 0xC0000001);
            master->itemModelL = Hu3DModelCreateData(0x7E001D);
            Hu3DModelAttrSet(master->itemModelL, 1);
            master->itemModelR = Hu3DModelCreateData(0x7E001B);
            Hu3DModelAttrSet(master->itemModelR, 0x40000002);
            Hu3DModelAttrSet(master->itemModelR, 1);
            break;
        case 1:
            break;
        case 2:
            for (i = 0; i < 2; i++) {
                master->gateModel[i] = Hu3DModelCreateData(0x800002);
                Hu3DModelPosSet(master->gateModel[i], lbl_1_data_CC[i].x, lbl_1_data_CC[i].y, lbl_1_data_CC[i].z);
                Hu3DModelAttrSet(master->gateModel[i], 0x40000002);
                if (i == 0) {
                    Hu3DModelScaleSet(master->gateModel[i], 1.0f, 1.0f, 1.0f);
                }
                else {
                    Hu3DModelScaleSet(master->gateModel[i], -1.0f, 1.0f, 1.0f);
                }
            }
            master->propModel = Hu3DModelCreateData(0x7E001C);
            Hu3DModelAttrSet(master->propModel, 0x40000002);
            Hu3DModelAttrSet(master->propModel, 1);
            break;
    }
    for (j = 0; j < 0xF; j++) {
        lbl_1_bss_254[j] = Hu3DJointMotionData(master->model, lbl_1_data_34[j]);
    }
    master->motNo = 0;
    Hu3DMotionSet(master->model, lbl_1_bss_254[0]);
    Hu3DModelAttrSet(master->model, 0x40000001);
    Hu3DModelShadowSet(master->model);
    master->pos.x = lbl_1_data_70.x;
    master->pos.y = lbl_1_data_70.y;
    master->pos.z = lbl_1_data_70.z;
    master->rot.x = master->rot.y = master->rot.z = 0.0f;
    master->state = 0;
    master->nextState = -1;
    master->timer = 0;
    master->unk28 = 0.0f;
    master->spinCnt = 0;
    master->itemCnt = 0;
    master->baseY = master->pos.y;
    master->angle = 0.0f;
    master->side = 0;
    obj->objFunc = fn_1_4794;
}

s32 fn_1_6FE4(void)
{
    s32 total;
    s32 i;

    total = 0;
    for (i = 0; i < 4; i++) {
        total += lbl_1_bss_230[i];
    }
    return total;
}

void fn_1_7034(OMOBJ *obj)
{
    s32 i;

    if (MgSeqModeGet() >= 5) {
        for (i = 0; i < 4; i++) {
            MgScoreWinDispSet(lbl_1_bss_1E8[i], 1);
            espDispOn(lbl_1_bss_1E0[i]);
            espDispOn(lbl_1_bss_1D8[i]);
            espDispOn(lbl_1_bss_1D0[i]);
            MgScoreDispOn(lbl_1_bss_1C0[i]);
            if (lbl_1_bss_230[i] > 0x63) {
                MgScoreValueSet(lbl_1_bss_1C0[i], 0x63);
            }
            else {
                MgScoreValueSet(lbl_1_bss_1C0[i], lbl_1_bss_230[i]);
            }
            if (lbl_1_bss_210[i] != 0) {
                if (lbl_1_bss_220[i] > 0xC) {
                    lbl_1_bss_210[i] = 0;
                    lbl_1_bss_220[i] = 0;
                    lbl_1_bss_1F0[i] = 1.0f;
                    MgScoreDigitScaleSet(lbl_1_bss_1C0[i], lbl_1_bss_1F0[i], lbl_1_bss_1F0[i]);
                }
                else {
                    if (lbl_1_bss_220[i] < 4) {
                        if ((lbl_1_bss_1F0[i] += 0.2f) >= 1.8f) {
                            lbl_1_bss_1F0[i] = 1.8f;
                        }
                    }
                    else {
                        if ((lbl_1_bss_1F0[i] -= 0.1f) <= 1.0f) {
                            lbl_1_bss_1F0[i] = 1.0f;
                        }
                    }
                    lbl_1_bss_220[i] += 1;
                    MgScoreDigitScaleSet(lbl_1_bss_1C0[i], lbl_1_bss_1F0[i], lbl_1_bss_1F0[i]);
                }
            }
        }
    }
}

void fn_1_7364(OMOBJ *obj)
{
    s32 i;

    for (i = 0; i < 4; i++) {
        lbl_1_bss_1E8[i] = MgScoreWinCreate(0x70, 0x28);
        MgScoreWinTPLvlSet(lbl_1_bss_1E8[i], 0.85f);
        MgScoreWinPosSet(lbl_1_bss_1E8[i], lbl_1_data_7C[i][0], lbl_1_data_7C[i][1]);
        MgScoreWinDispSet(lbl_1_bss_1E8[i], 0);
        lbl_1_bss_1E0[i] = espEntry(lbl_1_data_0[lbl_1_bss_244[i]], 0x63, 0);
        espDispOff(lbl_1_bss_1E0[i]);
        espDrawNoSet(lbl_1_bss_1E0[i], 0);
        espPosSet(lbl_1_bss_1E0[i], lbl_1_data_7C[i][0] - 38.0f, lbl_1_data_7C[i][1]);
        espAttrSet(lbl_1_bss_1E0[i], 1);
        if (lbl_1_bss_240 == 0) {
            lbl_1_bss_1D8[i] = espEntry(0x7E001E, 0x63, 0);
        }
        else if (lbl_1_bss_240 == 1) {
            lbl_1_bss_1D8[i] = espEntry(0x7F0014, 0x63, 0);
        }
        else {
            lbl_1_bss_1D8[i] = espEntry(0x800006, 0x63, 0);
        }
        espDispOff(lbl_1_bss_1D8[i]);
        espDrawNoSet(lbl_1_bss_1D8[i], 0);
        espPosSet(lbl_1_bss_1D8[i], lbl_1_data_7C[i][0] - 15.0f, lbl_1_data_7C[i][1]);
        espAttrSet(lbl_1_bss_1D8[i], 1);
        lbl_1_bss_1D0[i] = espEntry(0x960030, 0x63, 0xF);
        espDispOff(lbl_1_bss_1D0[i]);
        espDrawNoSet(lbl_1_bss_1D0[i], 0);
        espPosSet(lbl_1_bss_1D0[i], 4.0f + lbl_1_data_7C[i][0], lbl_1_data_7C[i][1]);
        espAttrSet(lbl_1_bss_1D0[i], 1);
        lbl_1_bss_1C0[i] = MgScoreCreate(0x960031, -1, 1);
        MgScoreValueSet(lbl_1_bss_1C0[i], 0);
        MgScorePosSet(lbl_1_bss_1C0[i], 22.0f + lbl_1_data_7C[i][0], lbl_1_data_7C[i][1]);
        MgScoreMaxDigitSet(lbl_1_bss_1C0[i], 2);
        MgScoreDispOff(lbl_1_bss_1C0[i]);
        MgScorePriSet(lbl_1_bss_1C0[i], 0x63);
        lbl_1_bss_210[i] = 0;
        lbl_1_bss_220[i] = 0;
        lbl_1_bss_200[i] = 0;
        lbl_1_bss_1F0[i] = 1.0f;
    }
    obj->objFunc = fn_1_7034;
}

void fn_1_7898(OMOBJ *obj)
{
    if (lbl_1_bss_274 != 0) {
        MgActorExec();
    }
}

void fn_1_78CC(void)
{
    lbl_1_bss_274 = 1;
    lbl_1_bss_278[0] = omAddObjEx(lbl_1_bss_488, 0x64, 0, 0, -1, fn_1_7898);
}

void fn_1_792C(s32 value)
{
    lbl_1_bss_274 = value;
}

void fn_1_793C(s32 a, s32 b, s32 c, s32 d, s32 e)
{
    lbl_1_bss_244[0] = a;
    lbl_1_bss_244[1] = b;
    lbl_1_bss_244[2] = c;
    lbl_1_bss_244[3] = d;
    lbl_1_bss_240 = e;
    omAddObjEx(lbl_1_bss_488, 0x65, 0, 0, -1, fn_1_7364);
}

void fn_1_79E4(s32 index, s32 value)
{
    lbl_1_bss_230[index] = value;
}

void fn_1_79FC(s32 type)
{
    s32 i;

    if (type == 0) {
        lbl_1_bss_16C = Hu3DModelCreateData(0x7E001A);
    }
    else if (type == 1) {
        lbl_1_bss_16C = Hu3DModelCreateData(0x7F0011);
    }
    else {
        lbl_1_bss_16C = Hu3DModelCreateData(0x800003);
    }
    Hu3DModelAttrSet(lbl_1_bss_16C, 1);
    for (i = 0; i < 0x14; i++) {
        lbl_1_bss_170[i] = NULL;
    }
}

void fn_1_7AF0(OMOBJ *obj)
{
    M573_ITEM *item;
    s32 i;

    item = obj->data;
    Hu3DModelAttrSet(obj->mdlId[0], 1);
    Hu3DModelKill(obj->mdlId[0]);
    lbl_1_bss_168--;
    for (i = 0; i < 0x14; i++) {
        if (obj == lbl_1_bss_170[i]) {
            lbl_1_bss_170[i] = NULL;
            break;
        }
    }
    omDelObjEx(lbl_1_bss_488, obj);
}

void fn_1_7BC0(OMOBJ *obj)
{
    M573_ITEM *item;

    item = obj->data;
    if (item->state == 0) {
        Hu3D2Dto3D(&item->src2d, 1, &item->dst);
        item->vel.x = (item->dst.x - item->pos.x) / (30.0f - (f32)item->frame);
        item->vel.y = (item->dst.y - item->pos.y) / (30.0f - (f32)item->frame);
        item->vel.z = (item->dst.z - item->pos.z) / (30.0f - (f32)item->frame);
        item->pos.x = item->pos.x + item->vel.x;
        item->pos.y = item->pos.y + item->vel.y;
        item->pos.z = item->pos.z + item->vel.z;
        item->rot.y = item->rot.y + 10.0f;
        item->frame++;
        if ((f32)item->frame > 30.0f) {
            item->pos.y = item->dst.y;
            fn_1_7AF0(obj);
            lbl_1_bss_230[item->player] += 1;
            lbl_1_bss_210[item->player] = 1;
            HuAudFXPlay(0x96D);
        }
    }
    omSetTra(obj, item->pos.x, item->pos.y, item->pos.z);
    omSetRot(obj, item->rot.x, item->rot.y, item->rot.z);
}

void fn_1_7EB4(OMOBJ *obj)
{
    M573_ITEM *item;
    s32 i;

    item = obj->data;
    obj->mdlId[0] = Hu3DModelLink(lbl_1_bss_16C);
    omSetTra(obj, item->pos.x, item->pos.y, item->pos.z);
    omSetRot(obj, item->rot.x, item->rot.y, item->rot.z);
    omSetSca(obj, 1.0f, 1.0f, 1.0f);
    for (i = 0; i < 0x14; i++) {
        if (lbl_1_bss_170[i] == NULL) {
            lbl_1_bss_170[i] = obj;
            break;
        }
    }
    Hu3DModelAttrReset(obj->mdlId[0], 1);
    obj->objFunc = fn_1_7BC0;
}

void fn_1_7FC4(Vec *pos, s32 player)
{
    OMOBJ *obj;
    M573_ITEM *item;

    lbl_1_bss_168++;
    obj = omAddObjEx(lbl_1_bss_488, 0x66, 1, 0, -1, fn_1_7EB4);
    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M573_ITEM), HU_MEMNUM_OVL);
    item = obj->data;
    item->state = 0;
    item->frame = 0;
    item->src2d.x = lbl_1_data_7C[player][0];
    item->src2d.y = 20.0f + lbl_1_data_7C[player][1];
    item->src2d.z = 1500.0f;
    item->rot.x = item->rot.y = item->rot.z = 0.0f;
    item->pos.x = pos->x;
    item->pos.y = pos->y;
    item->pos.z = pos->z;
    item->vel.x = item->dst.y = item->vel.z = 0.0f;
    item->player = player;
}

void fn_1_8114(OMOBJ *obj)
{
    M573_EFFECT *eff;
    s32 hitNo;

    eff = obj->data;
    if (eff->timer % 15 == 0) {
        hitNo = eff->hitNo;
        fn_1_7FC4(&eff->pos, hitNo);
        eff->count--;
        if (eff->count <= 0) {
            omDelObjEx(lbl_1_bss_488, obj);
        }
    }
    eff->timer++;
}

void fn_1_82C0(Vec *pos, s32 hitNo, s32 count)
{
    OMOBJ *obj;
    M573_EFFECT *eff;

    obj = omAddObjEx(lbl_1_bss_488, 0x66, 0, 0, -1, fn_1_8114);
    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M573_EFFECT), HU_MEMNUM_OVL);
    eff = obj->data;
    eff->pos.x = pos->x;
    eff->pos.y = pos->y;
    eff->pos.z = pos->z;
    eff->hitNo = hitNo;
    eff->count = count;
    eff->timer = 0;
}

s32 fn_1_8374(void)
{
    s32 i;

    for (i = 0; i < 0x14; i++) {
        if (lbl_1_bss_170[i] != NULL) {
            return 1;
        }
    }
    return 1;
}

void fn_1_83C8(void)
{
    lbl_1_bss_164 = -1;
}

void fn_1_83DC(void)
{
    lbl_1_bss_164 = HuAudSStreamPlay(0x3D);
}

void fn_1_840C(void)
{
    HuAudSStreamFadeOut(lbl_1_bss_164, 100);
}

s32 fn_1_843C(Vec *a, Vec *b, f32 r1, f32 r2)
{
    f32 dx;
    f32 dy;
    f32 dz;
    f32 dist;
    f32 sum;

    dx = a->x - b->x;
    dy = a->y - b->y;
    dz = a->z - b->z;
    dist = sqrtf(dz * dz + (dx * dx + dy * dy));
    sum = r1 + r2;
    if (sum > dist) {
        return 1;
    }
    return 0;
}

s32 fn_1_8634(Vec *a, Vec *b, f32 r1, f32 r2)
{
    if ((b->x - a->x) * (b->x - a->x) + (b->z - a->z) * (b->z - a->z) < (r1 + r2) * (r1 + r2)) {
        return 1;
    }
    return 0;
}

f32 fn_1_8694(f32 x1, f32 z1, f32 x2, f32 z2)
{
    f32 dx;
    f32 dz;
    f32 ret;

    dx = x1 - x2;
    dz = z1 - z2;
    ret = sqrtf(dx * dx + dz * dz);
    return ret;
}

s32 fn_1_8804(s32 a, s32 b)
{
    return a + frandmod(b - a);
}

s32 fn_1_8840(Vec *pos)
{
    Vec out;
    s32 ret;

    Hu3D3Dto2D(pos, 1, &out);
    ret = (s32)(32.0f + 64.0f * (out.x / 576.0f));
    return ret;
}

s32 fn_1_88BC(Vec *pos)
{
    Vec out;
    s32 ret;

    Hu3D3Dto2D(pos, 1, &out);
    ret = (s32)(96.0f + 31.0f * (out.y / 480.0f));
    return ret;
}
