#include "REL/m553Dll.h"
#include "game/frand.h"
#include "game/mg/seqman.h"
#include "game/mg/actman.h"
#include "game/mg/timer.h"
#include "game/memory.h"
#include "game/sprite.h"
#include "game/printfunc.h"
#include "string.h"
#include "math.h"

typedef void (*M553VoidFunc)(void);
extern const M553VoidFunc _ctors[];
extern const M553VoidFunc _dtors[];

typedef struct M553CamInit_s {
    HuVecF target;
    HuVecF rot;
    f32 zoom;
    f32 aspect;
    f32 vpX;
    f32 vpY;
    f32 vpW;
    f32 vpH;
    f32 fov;
} M553CAMINIT;

typedef struct M553Light_s {
    HU3DLIGHTID id;
    HuVecF pos;
    HuVecF rot;
    HuVecF target;
    HuVecF dir;
    f32 dist;
    GXColor color;
} M553LIGHT;

typedef struct M553ParWork_s {
    s32 unk00;
    u8 colorStart[4];
    u8 colorEnd[4];
    HuVecF vel;
    HuVecF velMul;
    f32 accelY;
    f32 unk28;
    f32 scaleDelta;
    f32 alphaDelta;
    f32 colorFactor;
} M553PARWORK;

typedef struct M553Pair_s {
    s16 v[5];
} M553Pair;

typedef struct M553DebugObj_s {
    s32 unk00;
    f32 x;
    f32 y;
    f32 z;
    f32 rx;
    f32 ry;
    f32 rz;
} M553DEBUGOBJ;

typedef struct M553DebugWork_s {
    u8 pad00[0x58];
    M553DEBUGOBJ *obj;
} M553DEBUGWORK;

/* stage.c */
extern void fn_1_D9A4(M553STWORK *common, void *work, s32 *data);
extern void fn_1_DBF0(s32 arg);
extern void fn_1_E234(s16 index, s32 *data);
extern void fn_1_E254(s16 index, void *data);
extern void fn_1_E844(OMOBJ *obj);
extern void fn_1_EC64(OMOBJ *obj);
/* game.c */
extern s32 fn_1_6064(M553STWORK *common);
extern s32 fn_1_6170(void);
extern s32 fn_1_61A8(void);
extern s32 fn_1_6218(void);
extern s32 fn_1_6850(void);
extern s32 fn_1_6928(void);
extern s32 fn_1_6964(void);
extern s32 fn_1_6BDC(void);
extern s32 fn_1_6C0C(void);

void ObjectSetup(void);
void fn_1_2F0(s16 mode, s16 frameNo);
void fn_1_324(s16 mode, s16 frameNo);
void fn_1_350(s16 mode, s16 frameNo);
void fn_1_37C(s16 mode, s16 frameNo);
void fn_1_3A8(s16 mode, s16 frameNo);
void fn_1_3D4(s16 mode, s16 frameNo);
void fn_1_400(s16 mode, s16 frameNo);
void fn_1_42C(s16 mode, s16 frameNo);
void fn_1_44C(s16 mode, s16 frameNo);
void fn_1_478(OMOBJ *obj);
void fn_1_47C(s16 no);
void fn_1_94C(s16 no);
void fn_1_EC4(void);
void fn_1_10F4(M553LIGHT *light);
void fn_1_13CC(void);
void fn_1_14F4(M553SHADOW *shadow);
s16 fn_1_2D1C(s16 type, s16 life, M553PARWORK *work, f32 x, f32 y, f32 z, f32 scale, f32 scaleBase);
void fn_1_2E8C(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);

M553GSTATE lbl_1_bss_70C;
M553STWORK lbl_1_bss_16DC;

MGSEQ_PARAM lbl_1_data_0 = {
    0,
    0,
    fn_1_2F0,
    fn_1_324,
    fn_1_350,
    fn_1_37C,
    fn_1_3A8,
    fn_1_3D4,
    fn_1_400,
    fn_1_42C,
    fn_1_44C,
};

s32 lbl_1_data_28[13][5] = {
    { 0x8C0000, 0x8C0002, 0x8C001A, 0x8C002F, 0x8C0007 },
    { 0x330000, 0x330002, 0x33001A, 0x33002F, 0x330007 },
    { 0xB70000, 0xB70002, 0xB7001A, 0xB7002F, 0xB70007 },
    { 0xED0000, 0xED0002, 0xED001A, 0xED002F, 0xED0007 },
    { 0xE60000, 0xE60002, 0xE6001A, 0xE6002F, 0xE60007 },
    { 0x170000, 0x170002, 0x17001A, 0x17002F, 0x170007 },
    { 0xE00000, 0xE00002, 0xE0001A, 0xE0002F, 0xE00007 },
    { 0x2D0000, 0x2D0002, 0x2D001A, 0x2D002F, 0x2D0007 },
    { 0xC90000, 0xC90002, 0xC9001A, 0xC9002F, 0xC90007 },
    { 0xB00000, 0xB00002, 0xB0001A, 0xB0002F, 0xB00007 },
    { 0xB00000, 0xB00002, 0xB0001A, 0xB0002F, 0xB00007 },
    { 0xB00000, 0xB00002, 0xB0001A, 0xB0002F, 0xB00007 },
    { 0xB00000, 0xB00002, 0xB0001A, 0xB0002F, 0xB00007 },
};

s32 lbl_1_data_12C[25] = {
    0x6F0000, 0x6F0001, 0x6F0002, 0x6F0003, 0x6F0004, 0x6F0005, 0x6F0006, 0x6F0007, 0x6F0008,
    0x6F0009, 0x6F000A, 0x6F000B, 0x6F000D, 0x6F000E, 0x6F000F, 0x6F0010, 0x6F0011, 0x6F0012,
    0x6F0013, 0x6F0014, 0x6F0015, 0x6F0016, 0x6F0017, 0x6F0018, 0x6F001B,
};

M553CAMINIT lbl_1_data_190[4] = {
    { { 0.0f, 194.0f, 0.0f }, { -26.0f, 0.0f, 0.0f }, 50.0f, 1.33f, 0.0f, 0.0f, 640.0f, 480.0f, 45.0f },
    { { 0.0f, 194.0f, 0.0f }, { -26.0f, 0.0f, 0.0f }, 1732.0f, 1.33f, 0.0f, 0.0f, 640.0f, 480.0f, 45.0f },
    { { 0.0f, 206.0f, 0.0f }, { -32.0f, 0.0f, 0.0f }, 1732.0f, 1.33f, 0.0f, 0.0f, 640.0f, 480.0f, 45.0f },
    { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 500.0f }, 500.0f, 1.33f, 0.0f, 0.0f, 640.0f, 480.0f, 45.0f },
};

s16 lbl_1_data_260[16] = {
    1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000, 0x8000,
};

char lbl_1_data_280[] = "OBJ X:%.1f Y:%.1f Z:%.1f RX%.1f RY%.1f RZ%.1f";
char lbl_1_data_2AE[] = "OBJW %d";
char lbl_1_data_2B6[] = "CAMPOS X:%.1f Y:%.1f Z:%.1f ZOOM%.1f";
char lbl_1_data_2DB[] = "CAMROT X:%.1f Y:%.1f Z:%.1f";

M553PARWORK lbl_1_data_2F8 = {
    0,
    { 0xFF, 0xFF, 0xFF, 0xFF },
    { 0, 0, 0, 0xFF },
    { 0.0f, 2.0f, 1.0f },
    { 0.85f, 0.85f, 0.85f },
    0.2f,
    0.0f,
    1.0f,
    -5.0f,
    0.02f,
};

s16 lbl_1_data_330[24] = {
    0x88C, 0x878, 0x879, 0x87A, 0x87B, 0x87C, 0x87D, 0x87E, 0x87F, 0x880, 0x881, 0x882,
    0x883, 0x884, 0x885, 0x886, 0x887, 0x888, 0x889, 0x88A, 0x88B, 0x319, 0, 0,
};

M553GPLAYERS lbl_1_bss_0;
M553SHADOW *lbl_1_bss_C;
M553LIGHT *lbl_1_bss_10;
M553CAM *lbl_1_bss_14;
M553GCELL lbl_1_bss_18[35];
s16 lbl_1_bss_478[10];
M553GPLAYERSTATE lbl_1_bss_48C[2];
M553Pair lbl_1_bss_7E8[2];
M553STMODEL lbl_1_bss_7FC[0x88];
OMOBJMAN *lbl_1_bss_1EE0;
void *lbl_1_bss_1EE8;
s16 lbl_1_bss_1EEC;

int _prolog(void)
{
    const M553VoidFunc *ctors = _ctors;

    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const M553VoidFunc *dtors = _dtors;

    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

void ObjectSetup(void)
{
    s16 i;

    lbl_1_bss_1EE0 = MgActorObjectSetup();
    fn_1_D9A4(&lbl_1_bss_16DC, lbl_1_bss_7FC, lbl_1_data_12C);
    lbl_1_bss_14 = HuMemDirectMallocNum(HEAP_HEAP, 0x148, HU_MEMNUM_OVL);
    memset(lbl_1_bss_14, 0, 0x148);
    lbl_1_bss_10 = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M553LIGHT), HU_MEMNUM_OVL);
    memset(lbl_1_bss_10, 0, sizeof(M553LIGHT));
    lbl_1_bss_C = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M553SHADOW), HU_MEMNUM_OVL);
    memset(lbl_1_bss_C, 0, sizeof(M553SHADOW));
    fn_1_47C(0);
    fn_1_EC4();
    fn_1_13CC();
    fn_1_DBF0(1);
    fn_1_E234(0, lbl_1_data_28[lbl_1_bss_16DC.player[0].charNo]);
    fn_1_E234(1, lbl_1_data_28[lbl_1_bss_16DC.player[1].charNo]);
    for (i = 0; i < 2; i++) {
        fn_1_E254(i, &lbl_1_bss_7E8[i]);
    }
    omAddObjEx(lbl_1_bss_1EE0, 0x7FDA, 0, 0, -1, fn_1_E844);
    omAddObjEx(lbl_1_bss_1EE0, 0x7FDA, 0, 0, -1, fn_1_478);
    omAddObjEx(lbl_1_bss_1EE0, 0x7FDA, 0, 0, -1, fn_1_EC64);
    Hu3DBGColorSet(0, 0, 0);
    MgSeqCreate(&lbl_1_data_0);
    MgSeqModeDelaySet(3, 1);
    MgSeqModeDelaySet(2, 1);
    MgSeqModeDelaySet(2, 0x1770);
    MgSeqModeDelaySet(7, 0x1770);
    MgSeqModeDelaySet(9, 0x3C);
}

void fn_1_2F0(s16 mode, s16 frameNo)
{
    if (fn_1_6064(&lbl_1_bss_16DC) != 0) {
        MgSeqModeNext();
    }
}

void fn_1_324(s16 mode, s16 frameNo)
{
    if (fn_1_6170() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_350(s16 mode, s16 frameNo)
{
    if (fn_1_61A8() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_37C(s16 mode, s16 frameNo)
{
    if (fn_1_6218() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_3A8(s16 mode, s16 frameNo)
{
    if (fn_1_6850() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_3D4(s16 mode, s16 frameNo)
{
    if (fn_1_6928() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_400(s16 mode, s16 frameNo)
{
    if (fn_1_6964() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_42C(s16 mode, s16 frameNo)
{
    fn_1_6BDC();
}

void fn_1_44C(s16 mode, s16 frameNo)
{
    if (fn_1_6C0C() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_478(OMOBJ *obj)
{
}

void fn_1_47C(s16 no)
{
    lbl_1_bss_14[no].no = no;
    lbl_1_bss_14[no].target.x = lbl_1_data_190[no].target.x;
    lbl_1_bss_14[no].target.y = lbl_1_data_190[no].target.y;
    lbl_1_bss_14[no].target.z = lbl_1_data_190[no].target.z;
    lbl_1_bss_14[no].rot.x = lbl_1_data_190[no].rot.x;
    lbl_1_bss_14[no].rot.y = lbl_1_data_190[no].rot.y;
    lbl_1_bss_14[no].rot.z = lbl_1_data_190[no].rot.z;
    lbl_1_bss_14[no].zoom = lbl_1_data_190[no].zoom;
    lbl_1_bss_14[no].aspect = lbl_1_data_190[no].aspect;
    lbl_1_bss_14[no].vpX = lbl_1_data_190[no].vpX;
    lbl_1_bss_14[no].vpY = lbl_1_data_190[no].vpY;
    lbl_1_bss_14[no].vpW = lbl_1_data_190[no].vpW;
    lbl_1_bss_14[no].vpH = lbl_1_data_190[no].vpH;
    lbl_1_bss_14[no].fov = lbl_1_data_190[no].fov;
    Hu3DCameraCreate(lbl_1_data_260[no]);
    Hu3DCameraViewportSet(lbl_1_data_260[no], lbl_1_bss_14[no].vpX, lbl_1_bss_14[no].vpY, lbl_1_bss_14[no].vpW, lbl_1_bss_14[no].vpH, 0.0f, 1.0f);
    Hu3DCameraPerspectiveSet(lbl_1_data_260[no], lbl_1_bss_14[no].fov, 20.0f, 40000.0f, lbl_1_bss_14[no].aspect);
    fn_1_94C(no);
    Center.x = lbl_1_data_190[0].target.x;
    Center.y = lbl_1_data_190[0].target.y;
    Center.z = lbl_1_data_190[0].target.z;
    CRot.x = lbl_1_data_190[0].rot.x;
    CRot.y = lbl_1_data_190[0].rot.y;
    CRot.z = lbl_1_data_190[0].rot.z;
    CZoom = lbl_1_data_190[no].zoom;
}

void fn_1_94C(s16 no)
{
    f32 unused;

    lbl_1_bss_14[no].pos.x = lbl_1_bss_14[no].target.x + (lbl_1_bss_14[no].zoom * (cos((3.141592653589793 * lbl_1_bss_14[no].rot.x) / 180.0) * sin((3.141592653589793 * lbl_1_bss_14[no].rot.y) / 180.0)));
    lbl_1_bss_14[no].pos.y = lbl_1_bss_14[no].target.y + (lbl_1_bss_14[no].zoom * -sin((3.141592653589793 * lbl_1_bss_14[no].rot.x) / 180.0));
    lbl_1_bss_14[no].pos.z = lbl_1_bss_14[no].target.z + (lbl_1_bss_14[no].zoom * (cos((3.141592653589793 * lbl_1_bss_14[no].rot.x) / 180.0) * cos((3.141592653589793 * lbl_1_bss_14[no].rot.y) / 180.0)));
    lbl_1_bss_14[no].up.x = sin((3.141592653589793 * lbl_1_bss_14[no].rot.x) / 180.0) * sin((3.141592653589793 * lbl_1_bss_14[no].rot.y) / 180.0);
    lbl_1_bss_14[no].up.y = cos((3.141592653589793 * lbl_1_bss_14[no].rot.x) / 180.0);
    lbl_1_bss_14[no].up.z = sin((3.141592653589793 * lbl_1_bss_14[no].rot.x) / 180.0) * cos((3.141592653589793 * lbl_1_bss_14[no].rot.y) / 180.0);
    unused = lbl_1_bss_14[no].target.z;
    Hu3DCameraPosSet(lbl_1_data_260[no], lbl_1_bss_14[no].pos.x, lbl_1_bss_14[no].pos.y, lbl_1_bss_14[no].pos.z, lbl_1_bss_14[no].up.x, lbl_1_bss_14[no].up.y, lbl_1_bss_14[no].up.z, lbl_1_bss_14[no].target.x, lbl_1_bss_14[no].target.y, lbl_1_bss_14[no].target.z);
}

void fn_1_EC4(void)
{
    Hu3DLighInit();
    lbl_1_bss_10->pos.x = 0.0f;
    lbl_1_bss_10->pos.y = 0.0f;
    lbl_1_bss_10->pos.z = 0.0f;
    lbl_1_bss_10->target.x = 0.0f;
    lbl_1_bss_10->target.y = 0.0f;
    lbl_1_bss_10->target.z = 0.0f;
    lbl_1_bss_10->rot.x = -45.0f;
    lbl_1_bss_10->rot.y = 0.0f;
    lbl_1_bss_10->rot.z = 0.0f;
    lbl_1_bss_10->dist = 1000.0f;
    lbl_1_bss_10->color.r = 0xFF;
    lbl_1_bss_10->color.g = 0xFF;
    lbl_1_bss_10->color.b = 0xFF;
    lbl_1_bss_10->color.a = 0xFF;
    PSVECSubtract(&lbl_1_bss_10->target, &lbl_1_bss_10->pos, &lbl_1_bss_10->dir);
    lbl_1_bss_10->id = Hu3DGLightCreateV(&lbl_1_bss_10->pos, &lbl_1_bss_10->dir, &lbl_1_bss_10->color);
    fn_1_10F4(lbl_1_bss_10);
    Hu3DGLightStaticSet(lbl_1_bss_10->id, TRUE);
    Hu3DGLightInfinitytSet(lbl_1_bss_10->id);
}

void fn_1_10F4(M553LIGHT *light)
{
    lbl_1_bss_10->pos.x = lbl_1_bss_10->target.x + (lbl_1_bss_10->dist * (cos((3.141592653589793 * lbl_1_bss_10->rot.x) / 180.0) * sin((3.141592653589793 * lbl_1_bss_10->rot.y) / 180.0)));
    lbl_1_bss_10->pos.y = lbl_1_bss_10->target.y + (lbl_1_bss_10->dist * -sin((3.141592653589793 * lbl_1_bss_10->rot.x) / 180.0));
    lbl_1_bss_10->pos.z = lbl_1_bss_10->target.z + (lbl_1_bss_10->dist * (cos((3.141592653589793 * lbl_1_bss_10->rot.x) / 180.0) * cos((3.141592653589793 * lbl_1_bss_10->rot.y) / 180.0)));
    PSVECSubtract(&lbl_1_bss_10->target, &lbl_1_bss_10->pos, &lbl_1_bss_10->dir);
    PSVECNormalize(&lbl_1_bss_10->dir, &lbl_1_bss_10->dir);
    Hu3DGLightPosSetV(lbl_1_bss_10->id, &lbl_1_bss_10->pos, &lbl_1_bss_10->dir);
    Hu3DGLightColorSet(lbl_1_bss_10->id, lbl_1_bss_10->color.r, lbl_1_bss_10->color.g, lbl_1_bss_10->color.b, lbl_1_bss_10->color.a);
}

void fn_1_13CC(void)
{
    Hu3DShadowCreate(30.0f, 20.0f, 40000.0f);
    lbl_1_bss_C->rot.x = -70.0f;
    lbl_1_bss_C->rot.y = 0.0f;
    lbl_1_bss_C->rot.z = 0.0f;
    lbl_1_bss_C->target.x = 0.0f;
    lbl_1_bss_C->target.y = 0.0f;
    lbl_1_bss_C->target.z = 0.0f;
    lbl_1_bss_C->tpLvl = 0.5f;
    lbl_1_bss_C->dist = 2800.0f;
    fn_1_14F4(lbl_1_bss_C);
}

void fn_1_14F4(M553SHADOW *shadow)
{
    lbl_1_bss_C->pos.x = lbl_1_bss_C->target.x + (lbl_1_bss_C->dist * (cos((3.141592653589793 * lbl_1_bss_C->rot.x) / 180.0) * sin((3.141592653589793 * lbl_1_bss_C->rot.y) / 180.0)));
    lbl_1_bss_C->pos.y = lbl_1_bss_C->target.y + (lbl_1_bss_C->dist * -sin((3.141592653589793 * lbl_1_bss_C->rot.x) / 180.0));
    lbl_1_bss_C->pos.z = lbl_1_bss_C->target.z + (lbl_1_bss_C->dist * (cos((3.141592653589793 * lbl_1_bss_C->rot.x) / 180.0) * cos((3.141592653589793 * lbl_1_bss_C->rot.y) / 180.0)));
    lbl_1_bss_C->up.x = sin((3.141592653589793 * lbl_1_bss_C->rot.x) / 180.0) * sin((3.141592653589793 * lbl_1_bss_C->rot.y) / 180.0);
    lbl_1_bss_C->up.y = cos((3.141592653589793 * lbl_1_bss_C->rot.x) / 180.0);
    lbl_1_bss_C->up.z = sin((3.141592653589793 * lbl_1_bss_C->rot.x) / 180.0) * cos((3.141592653589793 * lbl_1_bss_C->rot.y) / 180.0);
    Hu3DShadowTPLvlSet(lbl_1_bss_C->tpLvl);
    Hu3DShadowPosSet(&lbl_1_bss_C->pos, &lbl_1_bss_C->up, &lbl_1_bss_C->target);
}

s32 fn_1_1880(f32 *value, f32 time, f32 from, f32 to)
{
    f32 step;
    f32 absB;
    f32 absA;
    s32 ret = 0;

    step = (to - from) / time;
    *value += step;
    if (step == 0.0f) {
    } else {
        ret = (s32)((to - *value) / step);
    }
    ret += 1;
    if ((from - to) > 0.0f) {
        absA = from - to;
    } else {
        absA = from - (-1.0f * to);
    }
    if ((from - *value) > 0.0f) {
        absB = from - *value;
    } else {
        absB = from - (-1.0f * *value);
    }
    if (step <= absB) {
        *value = to;
        ret = 0;
    }
    return ret;
}

s32 fn_1_19A0(f32 *value, f32 time, f32 from, f32 to)
{
    f32 step;
    f32 absB;
    f32 absA;
    s32 ret = 0;

    step = (to - from) / time;
    *value += step;
    if (step == 0.0f) {
    } else {
        ret = (s32)((to - *value) / step);
    }
    ret += 1;
    if ((from - to) > 0.0f) {
        absA = from - to;
    } else {
        absA = from - (-1.0f * to);
    }
    if ((from - *value) > 0.0f) {
        absB = from - *value;
    } else {
        absB = from - (-1.0f * *value);
    }
    if (step <= absB) {
        ret = 0;
    }
    return ret;
}

MGTIMER *fn_1_1ABC(int type, int maxTime, int endTime, f32 x, f32 y)
{
    MGTIMER *timer = MgTimerCreate(type);

    MgTimerParamSet(timer, maxTime * 60, endTime * 60, 0);
    MgTimerPosSet(timer, x, y);
    return timer;
}

void fn_1_1B30(MGTIMER *timer)
{
    MgTimerModeOnSet(timer, 1);
}

s32 fn_1_1B5C(MGTIMER *timer)
{
    return MgTimerDoneCheck(timer);
}

void fn_1_1B84(MGTIMER *timer)
{
    MgTimerDispOff(timer);
    MgTimerKill(timer);
}

s32 fn_1_1BBC(M553DEBUGWORK *work, s16 pad, s16 no)
{
    M553DEBUGOBJ *obj = &work->obj[no];

    if (HuPadStkX[pad] > 10) {
        obj->x += 1.0f;
    }
    if (HuPadStkX[pad] < -10) {
        obj->x -= 1.0f;
    }
    if (HuPadStkY[pad] > 10) {
        obj->y += 1.0f;
    }
    if (HuPadStkY[pad] < -10) {
        obj->y -= 1.0f;
    }
    if (HuPadTrigL[pad] > 10) {
        obj->z += 1.0f;
    }
    if (HuPadTrigR[pad] > 10) {
        obj->z -= 1.0f;
    }
    if (HuPadSubStkX[pad] > 10) {
        obj->rx += 1.0f;
    }
    if (HuPadSubStkX[pad] < -10) {
        obj->rx -= 1.0f;
    }
    if (HuPadSubStkY[pad] > 10) {
        obj->ry += 1.0f;
    }
    if (HuPadSubStkY[pad] < -10) {
        obj->ry -= 1.0f;
    }
    if (HuPadBtnRep[pad] & 0x100) {
        obj->rz += 1.0f;
    }
    if (HuPadBtnRep[pad] & 0x200) {
        obj->rz -= 1.0f;
    }
    print8(25, 136, 1.5f, lbl_1_data_280, obj->x, obj->y, obj->z, obj->rx, obj->ry, obj->rz);
    print8(25, 154, 1.5f, lbl_1_data_2AE, no);
    return 0;
}

void fn_1_1F10(void)
{
}

void fn_1_1F14(s16 pad)
{
    s8 x;
    s8 y;
    s8 sx;
    s8 sy;
    u16 btn;
    s32 btnDown;

    btn = HuPadBtn[pad];
    btnDown = HuPadBtnDown[pad];
    {
        s32 absX;
        s8 stkX;
        s32 absY;
        s8 stkY;
        s32 absSX;
        s8 subX;
        s32 absSY;
        s8 subY;

        x = HuPadStkX[pad];
        absX = (x >= 0) ? x : -x;
        stkX = (absX < 10) ? 0 : x;
        x = stkX;
        y = HuPadStkY[pad];
        absY = (y >= 0) ? y : -y;
        stkY = (absY < 10) ? 0 : y;
        y = stkY;
        sx = HuPadSubStkX[pad];
        absSX = (sx >= 0) ? sx : -sx;
        subX = (absSX < 8) ? 0 : sx;
        sx = subX;
        sy = HuPadSubStkY[pad];
        absSY = (sy >= 0) ? sy : -sy;
        subY = (absSY < 8) ? 0 : sy;
        sy = subY;
        if (btn & 0x400) {
            CZoom += 20.0f;
        }
        if (btn & 0x800) {
            CZoom -= 20.0f;
        }
        if (x > 0) {
            Center.x += 10.0f;
        }
        if (x < 0) {
            Center.x -= 10.0f;
        }
        if (y > 0) {
            Center.y += 10.0f;
        }
        if (y < 0) {
            Center.y -= 10.0f;
        }
        if (sx > 0) {
            CRot.y += 1.0f;
        }
        if (sx < 0) {
            CRot.y -= 1.0f;
        }
        if (sy > 0) {
            CRot.x += 1.0f;
        }
        if (sy < 0) {
            CRot.x -= 1.0f;
        }
        if (btn & 0x2000) {
            CRot.z += 1.0f;
        }
        if (btn & 0x4000) {
            CRot.z -= 1.0f;
        }
        if (btn & 8) {
            Center.z += 10.0f;
        }
        if (btn & 4) {
            Center.z -= 10.0f;
        }
        print8(25, 30, 1.5f, lbl_1_data_2B6, Center.x, Center.y, Center.z, CZoom);
        print8(25, 48, 1.5f, lbl_1_data_2DB, CRot.x, CRot.y, CRot.z);
    }
}

void fn_1_23CC(void)
{
    s16 i;
    ANIMDATA *anim;
    HU3DPARTICLE *particle;
    HU3DPARTICLEDATA *data;

    anim = HuSprAnimRead(HuDataSelHeapReadNum(0x6F000C, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_1EE8 = HuMemDirectMallocNum(HEAP_HEAP, 0xDAC0, HU_MEMNUM_OVL);
    lbl_1_bss_1EEC = Hu3DParticleCreate(anim, 1000);
    Hu3DParticleHookSet(lbl_1_bss_1EEC, fn_1_2E8C);
    Hu3DParticleBlendModeSet(lbl_1_bss_1EEC, 1);
    Hu3DModelLayerSet(lbl_1_bss_1EEC, 6);
    Hu3DModelCameraSet(lbl_1_bss_1EEC, 1);
    Hu3DModelAttrSet(lbl_1_bss_1EEC, 1);
    Hu3DModelAttrReset(lbl_1_bss_1EEC, 1);
    particle = Hu3DData[lbl_1_bss_1EEC].hookData;
    particle->emitCnt = 0;
    particle->work = lbl_1_bss_1EE8;
    particle->count = 0;
    data = particle->data;
    for (i = 0; i < particle->maxCnt; i++, data++) {
        data->scale = 0.0f;
    }
}

void fn_1_254C(HuVecF *pos)
{
    s16 i;
    HuVecF p;
    s16 alpha;
    f32 scale;

    for (i = 0; i < 12; i++) {
        scale = 6.0f + (f32)frandmod(6);
        lbl_1_data_2F8.vel.x = scale * sin((3.141592653589793 * (30.0f * (f32)i)) / 180.0);
        lbl_1_data_2F8.vel.y = 0.0f;
        lbl_1_data_2F8.vel.z = scale * cos((3.141592653589793 * (30.0f * (f32)i)) / 180.0);
        alpha = -frandmod(6) - 7;
        lbl_1_data_2F8.alphaDelta = alpha;
        p.x = pos->x + (20.0 * sin((3.141592653589793 * (30.0f * (f32)i)) / 180.0));
        p.y = 10.0f + pos->y;
        p.z = pos->z + (20.0 * cos((3.141592653589793 * (30.0f * (f32)i)) / 180.0));
        lbl_1_data_2F8.accelY = 0.01f * (f32)frandmod(20);
        scale = 20.0f + (f32)frandmod(20);
        fn_1_2D1C(0, 60, &lbl_1_data_2F8, p.x, p.y, p.z, scale, scale);
    }
}

void fn_1_28D4(HuVecF *pos)
{
    s16 i;
    HuVecF p;
    s16 alpha;
    f32 scale;

    for (i = 0; i < 30; i++) {
        p.x = pos->x + ((f32)frandmod(120) - 60.0f);
        p.y = pos->y + ((f32)frandmod(100) - 100.0f);
        p.z = 60.0f + pos->z;
        lbl_1_data_2F8.vel.x = (f32)frandmod(4) - 2.0f;
        lbl_1_data_2F8.vel.y = (f32)frandmod(8);
        lbl_1_data_2F8.vel.z = 0.0f;
        alpha = -frandmod(6) - 5;
        lbl_1_data_2F8.alphaDelta = alpha;
        lbl_1_data_2F8.accelY = 0.01f * (f32)frandmod(20);
        scale = 20.0f + (f32)frandmod(40);
        fn_1_2D1C(0, 60, &lbl_1_data_2F8, p.x, p.y, p.z, scale, scale);
    }
}

void fn_1_2B4C(HuVecF *pos)
{
    s16 i;
    HuVecF p;
    f32 scale;

    for (i = 0; i < 10; i++) {
        p.x = pos->x + ((f32)frandmod(110) - 55.0f);
        p.y = pos->y - 10.0f;
        p.z = 55.0f + pos->z;
        lbl_1_data_2F8.vel.x = 0.1f;
        lbl_1_data_2F8.vel.y = 0.1f * (6.0f + (f32)frandmod(8));
        lbl_1_data_2F8.vel.z = 0.0f;
        lbl_1_data_2F8.accelY = 0.0f;
        scale = 7.0f + (f32)frandmod(10);
        fn_1_2D1C(1, 40, &lbl_1_data_2F8, p.x, p.y, p.z, scale, 1.0f);
    }
}

s16 fn_1_2D1C(s16 type, s16 life, M553PARWORK *work, f32 x, f32 y, f32 z, f32 scale, f32 scaleBase)
{
    HU3DPARTICLEDATA *data;
    HU3DPARTICLE *particle;
    s16 i;
    HU3DMODEL *model;
    M553PARWORK *parWork;
    s16 ret;

    ret = -1;
    model = &Hu3DData[lbl_1_bss_1EEC];
    particle = model->hookData;
    parWork = particle->work;
    i = 0;
    data = particle->data;
    for (; i < particle->maxCnt; i++, data++) {
        if (data->scale == 0.0f) {
            parWork[i] = *work;
            data->pos.x = x;
            data->pos.y = y;
            data->pos.z = z;
            data->vel = work->vel;
            data->color.r = work->colorStart[0];
            data->color.g = work->colorStart[1];
            data->color.b = work->colorStart[2];
            data->color.a = work->colorStart[3];
            data->scaleBase = scaleBase;
            data->scale = scale;
            data->time = type;
            data->parManId = life;
            particle->emitCnt = i;
            ret = i;
            break;
        }
    }
    return ret;
}

void fn_1_2E8C(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx)
{
    M553PARWORK *work = particleP->work;
    HU3DPARTICLEDATA *data = particleP->data;
    s16 i;
    s16 v;

    for (i = 0; i < particleP->maxCnt; i++, data++) {
        if (data->scale > 0.0f) {
            if (data->time == 0) {
                data->vel.x = data->vel.x * work[i].velMul.x;
                data->vel.y = data->vel.y * work[i].velMul.y;
                data->vel.z = data->vel.z * work[i].velMul.z;
                PSVECAdd(&data->vel, &data->pos, &data->pos);
                data->vel.y = data->vel.y + work[i].accelY;
                v = (s16)((f32)data->color.r + (work[i].colorFactor * (f32)(work[i].colorEnd[0] - work[i].colorStart[0])));
                if (v < 0) {
                    v = 0;
                } else if (v > 0xFF) {
                    v = 0xFF;
                }
                data->color.r = v;
                v = (s16)((f32)data->color.g + (work[i].colorFactor * (f32)(work[i].colorEnd[1] - work[i].colorStart[1])));
                if (v < 0) {
                    v = 0;
                } else if (v > 0xFF) {
                    v = 0xFF;
                }
                data->color.g = v;
                v = (s16)((f32)data->color.b + (work[i].colorFactor * (f32)(work[i].colorEnd[2] - work[i].colorStart[2])));
                if (v < 0) {
                    v = 0;
                } else if (v > 0xFF) {
                    v = 0xFF;
                }
                data->color.b = v;
                v = (s16)((f32)data->color.a + work[i].alphaDelta);
                if (v < 0) {
                    data->scale = 0.0f;
                }
                data->color.a = v;
                if (data->scale != 0.0f) {
                    data->scale = data->scaleBase;
                    data->scaleBase = data->scaleBase + work[i].scaleDelta;
                    if (data->scaleBase <= 0.01f) {
                        data->scale = 0.0f;
                    }
                }
                data->parManId = data->parManId - 1;
                if (data->parManId <= 0) {
                    data->scale = 0.0f;
                }
            } else {
                PSVECAdd(&data->vel, &data->pos, &data->pos);
                v = data->color.a - 6;
                if (v < 0) {
                    v = 0;
                } else if (v > 0xFF) {
                    v = 0xFF;
                }
                data->color.a = v;
                data->scale = data->scale + data->scaleBase;
                data->parManId = data->parManId - 1;
                if (data->parManId <= 0) {
                    data->scale = 0.0f;
                }
            }
        }
    }
    DCStoreRangeNoSync(particleP->data, particleP->maxCnt * sizeof(HU3DPARTICLEDATA));
}

void fn_1_3350(s16 no)
{
    s16 unused = 0;
    HU3DMODEL *model;
    HSFMATERIAL *mat;
    s8 *dst;
    s16 i;

    model = &Hu3DData[lbl_1_bss_0.player[no]->actor->mdlId];
    dst = lbl_1_bss_48C[no].color = HuMemDirectMallocNum(HEAP_HEAP, model->hsf->materialNum * 3, HU_MEMNUM_OVL);
    i = 0;
    mat = model->hsf->material;
    for (; i < model->hsf->materialNum; i++, mat++) {
        *dst = mat->color[0];
        dst++;
        *dst = mat->color[1];
        dst++;
        *dst = mat->color[2];
        dst++;
    }
}

s32 fn_1_3448(s16 no, f32 rate)
{
    HSFMATERIAL *mat;
    HU3DMODEL *model;
    s16 i;
    s32 ret;

    ret = 0;
    model = &Hu3DData[lbl_1_bss_0.player[no]->actor->mdlId];
    if ((lbl_1_bss_48C[no].rate *= rate) < 0.9f) {
        ret = 1;
    }
    i = 0;
    mat = model->hsf->material;
    for (; i < model->hsf->materialNum; i++, mat++) {
        mat->color[0] = (f32)mat->color[0] * rate;
        mat->color[1] = (f32)mat->color[1] * rate;
        mat->color[2] = (f32)mat->color[2] * rate;
    }
    return ret;
}

void fn_1_35B8(s16 no)
{
    s16 unused = 0;
    HU3DMODEL *model;
    HSFMATERIAL *mat;
    s8 *src;
    s16 i;

    lbl_1_bss_48C[no].rate = 255.0f;
    model = &Hu3DData[lbl_1_bss_0.player[no]->actor->mdlId];
    src = lbl_1_bss_48C[no].color;
    i = 0;
    mat = model->hsf->material;
    for (; i < model->hsf->materialNum; i++, mat++) {
        mat->color[0] = *src;
        src++;
        mat->color[1] = *src;
        src++;
        mat->color[2] = *src;
        src++;
    }
}

void fn_1_36B4(void)
{
    lbl_1_bss_70C.jingle = HuAudJinglePlay(0x83);
}

void fn_1_36E4(void)
{
    lbl_1_bss_70C.bgm = HuAudBGMPlay(0x36);
}

void fn_1_3714(s16 which, s16 speed)
{
    s16 stream;

    if (which != 0) {
        stream = lbl_1_bss_70C.bgm;
    } else {
        stream = lbl_1_bss_70C.jingle;
    }
    HuAudSStreamFadeOut(stream, speed);
}

void fn_1_377C(s16 which)
{
    s16 stream;

    if (which != 0) {
        stream = lbl_1_bss_70C.bgm;
    } else {
        stream = lbl_1_bss_70C.jingle;
    }
    HuAudSStreamStop(stream);
}

void fn_1_37DC(s16 no)
{
    lbl_1_bss_70C.seState[no] = 1;
    lbl_1_bss_70C.se[no] = HuAudFXPlay(lbl_1_data_330[no]);
}

void fn_1_3854(s16 no, s16 vol, s16 pan)
{
    lbl_1_bss_70C.seState[no] = 1;
    lbl_1_bss_70C.se[no] = HuAudFXPlayVolPan(lbl_1_data_330[no], vol, pan);
}

void fn_1_38DC(s16 no, s16 pan)
{
    if (lbl_1_bss_70C.seState[no] == 1) {
        if (HuAudFXStatusGet(lbl_1_bss_70C.se[no]) == 2) {
            HuAudFXPanning(lbl_1_bss_70C.se[no], pan);
        }
    }
}

void fn_1_396C(s16 no)
{
    if (lbl_1_bss_70C.seState[no] == 1 && (HuAudFXStatusGet(lbl_1_bss_70C.se[no]) == 2 || HuAudFXStatusGet(lbl_1_bss_70C.se[no]) == 3)) {
        HuAudFXStop(lbl_1_bss_70C.se[no]);
    }
}

void fn_1_3A18(s16 no)
{
    if (lbl_1_bss_70C.seState[no] == 1 && (HuAudFXStatusGet(lbl_1_bss_70C.se[no]) == 2 || HuAudFXStatusGet(lbl_1_bss_70C.se[no]) == 3)) {
        HuAudFXStop(lbl_1_bss_70C.se[no]);
    }
}

void fn_1_3AC4(void)
{
    HuAudAllStop();
}
