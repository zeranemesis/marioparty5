#include "REL/m541Dll.h"
#include <math.h>
#include <string.h>
#include "game/printfunc.h"

/* ---- part_m1.c ---- */
typedef void (*M541VoidFunc)(void);
extern const M541VoidFunc _ctors[];
extern const M541VoidFunc _dtors[];

/* unit-owned .bss, in reverse address order */
OMOBJMAN *lbl_1_bss_3698;
M541_STAGE lbl_1_bss_33B0;
M541_MODEL lbl_1_bss_27E0[108];
s16 lbl_1_bss_27C0[2][8];
M541_FLOW lbl_1_bss_2734;
M541_PLAYERWORK lbl_1_bss_2614[2];
M541_G1WORK45C lbl_1_bss_45C;
M541_BOARD lbl_1_bss_2F0;
M541_LEAFWORK lbl_1_bss_20[M541_LEAFCNT];
M541_CAM *lbl_1_bss_1C;
M541_LIGHT *lbl_1_bss_18;
M541_SHADOW *lbl_1_bss_14;
M541_MG lbl_1_bss_0;

/* unit-owned .data, in original address order */
MGSEQ_PARAM lbl_1_data_0 = {
    0, 0,
    fn_1_2C0, fn_1_2F4, fn_1_320,
    fn_1_34C, fn_1_378, fn_1_3A4,
    fn_1_3D0, fn_1_3FC, fn_1_41C,
};
s32 lbl_1_data_28[13][8] = {
    { 0x8C0000, 0x8C0002, 0x8C000A, 0x8C0017, 0x8C001A, 0x8C0024, 0x8C0006, 0x8C0007 },
    { 0x330000, 0x330002, 0x33000A, 0x330017, 0x33001A, 0x330024, 0x330006, 0x330007 },
    { 0xB70000, 0xB70002, 0xB7000A, 0xB70017, 0xB7001A, 0xB70024, 0xB70006, 0xB70007 },
    { 0xED0000, 0xED0002, 0xED000A, 0xED0017, 0xED001A, 0xED0024, 0xED0006, 0xED0007 },
    { 0xE60000, 0xE60002, 0xE6000A, 0xE60017, 0xE6001A, 0xE60024, 0xE60006, 0xE60007 },
    { 0x170000, 0x170002, 0x17000A, 0x170017, 0x17001A, 0x170024, 0x170006, 0x170007 },
    { 0xE00000, 0xE00002, 0xE0000A, 0xE00017, 0xE0001A, 0xE00024, 0xE00006, 0xE00007 },
    { 0x2D0000, 0x2D0002, 0x2D000A, 0x2D0017, 0x2D001A, 0x2D0024, 0x2D0006, 0x2D0007 },
    { 0xC90000, 0xC90002, 0xC9000A, 0xC90017, 0xC9001A, 0xC90024, 0xC90006, 0xC90007 },
    { 0xB00000, 0xB00002, 0xB0000A, 0xB00017, 0xB0001A, 0xB00024, 0xB00006, 0xB00007 },
    { 0xB00000, 0xB00002, 0xB0000A, 0xB00017, 0xB0001A, 0xB00024, 0xB00006, 0xB00007 },
    { 0xB00000, 0xB00002, 0xB0000A, 0xB00017, 0xB0001A, 0xB00024, 0xB00006, 0xB00007 },
    { 0xB00000, 0xB00002, 0xB0000A, 0xB00017, 0xB0001A, 0xB00024, 0xB00006, 0xB00007 },
};
s32 lbl_1_data_1C8[12] = {
    0x630000, 0x630001, 0x630002, 0x630003, 0x630004, 0x630005,
    0x630006, 0x630007, 0x630008, 0x630009, 0x63000A, 0x63000B,
};
M541_CAMINIT lbl_1_data_1F8[4] = {
    { { 0.0f, 208.0f, 75.0f }, { -48.0f, 0.0f, 0.0f }, 1415.0f, 1.33f, 0.0f, 0.0f, 640.0f, 480.0f, 45.0f },
    { { 0.0f, 208.0f, 75.0f }, { -48.0f, 0.0f, 0.0f }, 1415.0f, 1.33f, 0.0f, 0.0f, 640.0f, 480.0f, 45.0f },
    { { 0.0f, 208.0f, 20.0f }, { -35.0f, 0.0f, 0.0f }, 1415.0f, 1.33f, 0.0f, 0.0f, 640.0f, 480.0f, 45.0f },
    { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 500.0f }, 500.0f, 1.33f, 0.0f, 0.0f, 640.0f, 480.0f, 45.0f },
};
s16 lbl_1_data_2C8[16] = {
    0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000, 0x8000,
};
char lbl_1_data_2E8[0x2E] = "OBJ X:%.1f Y:%.1f Z:%.1f RX%.1f RY%.1f RZ%.1f";
char lbl_1_data_316[8] = "OBJW %d";
char lbl_1_data_31E[0x25] = "CAMPOS X:%.1f Y:%.1f Z:%.1f ZOOM%.1f";
char lbl_1_data_343[0x1D] = "CAMROT X:%.1f Y:%.1f Z:%.1f";
s16 lbl_1_data_360[4] = { 0x809, 0x80A, 0x80B, 0x808 };

int _prolog(void)
{
    const M541VoidFunc *ctors = _ctors;

    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const M541VoidFunc *dtors = _dtors;

    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

void ObjectSetup(void)
{
    s16 i;

    lbl_1_bss_3698 = MgActorObjectSetup();
    fn_1_19AB0(&lbl_1_bss_33B0, lbl_1_bss_27E0, lbl_1_data_1C8);
    lbl_1_bss_1C = HuMemDirectMallocNum(HEAP_HEAP, 0x148, 0x10000000);
    memset(lbl_1_bss_1C, 0, 0x148);
    lbl_1_bss_18 = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M541_LIGHT), 0x10000000);
    memset(lbl_1_bss_18, 0, sizeof(M541_LIGHT));
    lbl_1_bss_14 = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M541_SHADOW), 0x10000000);
    memset(lbl_1_bss_14, 0, sizeof(M541_SHADOW));
    fn_1_44C(0);
    fn_1_E94();
    fn_1_139C();
    fn_1_19D10(0, lbl_1_data_28[lbl_1_bss_33B0.player[0].charNo]);
    fn_1_19D10(1, lbl_1_data_28[lbl_1_bss_33B0.player[1].charNo]);
    for (i = 0; i < 2; i++) {
        fn_1_19D30(i, lbl_1_bss_27C0[i]);
    }
    omAddObjEx(lbl_1_bss_3698, 0x7FDA, 0, 0, -1, (OMOBJFUNC)fn_1_1A320);
    omAddObjEx(lbl_1_bss_3698, 0x7FDA, 0, 0, -1, (OMOBJFUNC)fn_1_448);
    Hu3DBGColorSet(0, 0, 0);
    MgSeqCreate(&lbl_1_data_0);
    MgSeqModeDelaySet(3, 1);
    MgSeqModeDelaySet(2, 1);
    MgSeqModeDelaySet(2, 0x1D6);
    MgSeqModeDelaySet(7, 0xB4);
    MgSeqModeDelaySet(9, 0x3C);
}

void fn_1_2C0(s16 mode, s16 frameNo)
{
    if (fn_1_5C98(&lbl_1_bss_33B0) != 0) {
        MgSeqModeNext();
    }
}

void fn_1_2F4(s16 mode, s16 frameNo)
{
    if (fn_1_5DA4() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_320(s16 mode, s16 frameNo)
{
    if (fn_1_5DD8() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_34C(s16 mode, s16 frameNo)
{
    if (fn_1_5E48() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_378(s16 mode, s16 frameNo)
{
    if (fn_1_6728() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_3A4(s16 mode, s16 frameNo)
{
    if (fn_1_6BF0() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_3D0(s16 mode, s16 frameNo)
{
    if (fn_1_6C38() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_3FC(s16 mode, s16 frameNo)
{
    fn_1_6E20();
}

void fn_1_41C(s16 mode, s16 frameNo)
{
    if (fn_1_6E50() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_448(OMOBJ *obj)
{
}

void fn_1_44C(s16 idx)
{
    lbl_1_bss_1C[idx].idx = idx;
    lbl_1_bss_1C[idx].center.x = lbl_1_data_1F8[idx].center.x;
    lbl_1_bss_1C[idx].center.y = lbl_1_data_1F8[idx].center.y;
    lbl_1_bss_1C[idx].center.z = lbl_1_data_1F8[idx].center.z;
    lbl_1_bss_1C[idx].rot.x = lbl_1_data_1F8[idx].rot.x;
    lbl_1_bss_1C[idx].rot.y = lbl_1_data_1F8[idx].rot.y;
    lbl_1_bss_1C[idx].rot.z = lbl_1_data_1F8[idx].rot.z;
    lbl_1_bss_1C[idx].zoom = lbl_1_data_1F8[idx].zoom;
    lbl_1_bss_1C[idx].aspect = lbl_1_data_1F8[idx].aspect;
    lbl_1_bss_1C[idx].vpX = lbl_1_data_1F8[idx].vpX;
    lbl_1_bss_1C[idx].vpY = lbl_1_data_1F8[idx].vpY;
    lbl_1_bss_1C[idx].vpW = lbl_1_data_1F8[idx].vpW;
    lbl_1_bss_1C[idx].vpH = lbl_1_data_1F8[idx].vpH;
    lbl_1_bss_1C[idx].fov = lbl_1_data_1F8[idx].fov;
    Hu3DCameraCreate(lbl_1_data_2C8[idx]);
    Hu3DCameraViewportSet(lbl_1_data_2C8[idx], lbl_1_bss_1C[idx].vpX, lbl_1_bss_1C[idx].vpY,
                          lbl_1_bss_1C[idx].vpW, lbl_1_bss_1C[idx].vpH, 0.0f, 1.0f);
    Hu3DCameraPerspectiveSet(lbl_1_data_2C8[idx], lbl_1_bss_1C[idx].fov, 20.0f, 30000.0f,
                             lbl_1_bss_1C[idx].aspect);
    fn_1_91C(idx);
    Center.x = lbl_1_data_1F8[0].center.x;
    Center.y = lbl_1_data_1F8[0].center.y;
    Center.z = lbl_1_data_1F8[0].center.z;
    CRot.x = lbl_1_data_1F8[0].rot.x;
    CRot.y = lbl_1_data_1F8[0].rot.y;
    CRot.z = lbl_1_data_1F8[0].rot.z;
    CZoom = lbl_1_data_1F8[idx].zoom;
}

void fn_1_91C(s16 idx)
{
    f64 temp;

    temp = sin(3.141592653589793 * lbl_1_bss_1C[idx].rot.y / 180.0);
    lbl_1_bss_1C[idx].pos.x = lbl_1_bss_1C[idx].center.x
        + lbl_1_bss_1C[idx].zoom * (cos(3.141592653589793 * lbl_1_bss_1C[idx].rot.x / 180.0) * temp);
    lbl_1_bss_1C[idx].pos.y = lbl_1_bss_1C[idx].center.y
        + lbl_1_bss_1C[idx].zoom * -sin(3.141592653589793 * lbl_1_bss_1C[idx].rot.x / 180.0);
    temp = cos(3.141592653589793 * lbl_1_bss_1C[idx].rot.y / 180.0);
    lbl_1_bss_1C[idx].pos.z = lbl_1_bss_1C[idx].center.z
        + lbl_1_bss_1C[idx].zoom * (cos(3.141592653589793 * lbl_1_bss_1C[idx].rot.x / 180.0) * temp);
    temp = sin(3.141592653589793 * lbl_1_bss_1C[idx].rot.y / 180.0);
    lbl_1_bss_1C[idx].up.x = sin(3.141592653589793 * lbl_1_bss_1C[idx].rot.x / 180.0) * temp;
    lbl_1_bss_1C[idx].up.y = cos(3.141592653589793 * lbl_1_bss_1C[idx].rot.x / 180.0);
    temp = cos(3.141592653589793 * lbl_1_bss_1C[idx].rot.y / 180.0);
    lbl_1_bss_1C[idx].up.z = sin(3.141592653589793 * lbl_1_bss_1C[idx].rot.x / 180.0) * temp;
    Hu3DCameraPosSet(lbl_1_data_2C8[idx], lbl_1_bss_1C[idx].pos.x, lbl_1_bss_1C[idx].pos.y, lbl_1_bss_1C[idx].pos.z,
                     lbl_1_bss_1C[idx].up.x, lbl_1_bss_1C[idx].up.y, lbl_1_bss_1C[idx].up.z,
                     lbl_1_bss_1C[idx].center.x, lbl_1_bss_1C[idx].center.y, lbl_1_bss_1C[idx].center.z);
}

void fn_1_E94(void)
{
    Hu3DLighInit();
    lbl_1_bss_18->pos.x = 0.0f;
    lbl_1_bss_18->pos.y = 0.0f;
    lbl_1_bss_18->pos.z = 0.0f;
    lbl_1_bss_18->target.x = 0.0f;
    lbl_1_bss_18->target.y = 0.0f;
    lbl_1_bss_18->target.z = 0.0f;
    lbl_1_bss_18->rot.x = -23.0f;
    lbl_1_bss_18->rot.y = 0.0f;
    lbl_1_bss_18->rot.z = 0.0f;
    lbl_1_bss_18->dist = 1000.0f;
    lbl_1_bss_18->color.r = 0xFF;
    lbl_1_bss_18->color.g = 0xFF;
    lbl_1_bss_18->color.b = 0xFF;
    lbl_1_bss_18->color.a = 0xFF;
    PSVECSubtract(&lbl_1_bss_18->target, &lbl_1_bss_18->pos, &lbl_1_bss_18->dir);
    lbl_1_bss_18->id = Hu3DGLightCreateV(&lbl_1_bss_18->pos, &lbl_1_bss_18->dir, &lbl_1_bss_18->color);
    fn_1_10C4();
    Hu3DGLightStaticSet(lbl_1_bss_18->id, 1);
    Hu3DGLightInfinitytSet(lbl_1_bss_18->id);
}

void fn_1_10C4(void)
{
    f64 temp;

    temp = sin(3.141592653589793 * lbl_1_bss_18->rot.y / 180.0);
    lbl_1_bss_18->pos.x = lbl_1_bss_18->target.x
        + lbl_1_bss_18->dist * (cos(3.141592653589793 * lbl_1_bss_18->rot.x / 180.0) * temp);
    lbl_1_bss_18->pos.y = lbl_1_bss_18->target.y
        + lbl_1_bss_18->dist * -sin(3.141592653589793 * lbl_1_bss_18->rot.x / 180.0);
    temp = cos(3.141592653589793 * lbl_1_bss_18->rot.y / 180.0);
    lbl_1_bss_18->pos.z = lbl_1_bss_18->target.z
        + lbl_1_bss_18->dist * (cos(3.141592653589793 * lbl_1_bss_18->rot.x / 180.0) * temp);
    PSVECSubtract(&lbl_1_bss_18->target, &lbl_1_bss_18->pos, &lbl_1_bss_18->dir);
    PSVECNormalize(&lbl_1_bss_18->dir, &lbl_1_bss_18->dir);
    Hu3DGLightPosSetV(lbl_1_bss_18->id, &lbl_1_bss_18->pos, &lbl_1_bss_18->dir);
    Hu3DGLightColorSet(lbl_1_bss_18->id, lbl_1_bss_18->color.r, lbl_1_bss_18->color.g, lbl_1_bss_18->color.b,
                       lbl_1_bss_18->color.a);
}

void fn_1_139C(void)
{
    Hu3DShadowCreate(30.0f, 20.0f, 30000.0f);
    lbl_1_bss_14->rot.x = -90.0f;
    lbl_1_bss_14->rot.y = 0.0f;
    lbl_1_bss_14->rot.z = 0.0f;
    lbl_1_bss_14->target.x = 0.0f;
    lbl_1_bss_14->target.y = 0.0f;
    lbl_1_bss_14->target.z = 0.0f;
    lbl_1_bss_14->tpLvl = 0.5f;
    lbl_1_bss_14->dist = 2600.0f;
    fn_1_14C4();
}

void fn_1_14C4(void)
{
    f64 temp;

    temp = sin(3.141592653589793 * lbl_1_bss_14->rot.y / 180.0);
    lbl_1_bss_14->pos.x = lbl_1_bss_14->target.x
        + lbl_1_bss_14->dist * (cos(3.141592653589793 * lbl_1_bss_14->rot.x / 180.0) * temp);
    lbl_1_bss_14->pos.y = lbl_1_bss_14->target.y
        + lbl_1_bss_14->dist * -sin(3.141592653589793 * lbl_1_bss_14->rot.x / 180.0);
    temp = cos(3.141592653589793 * lbl_1_bss_14->rot.y / 180.0);
    lbl_1_bss_14->pos.z = lbl_1_bss_14->target.z
        + lbl_1_bss_14->dist * (cos(3.141592653589793 * lbl_1_bss_14->rot.x / 180.0) * temp);
    temp = sin(3.141592653589793 * lbl_1_bss_14->rot.y / 180.0);
    lbl_1_bss_14->up.x = sin(3.141592653589793 * lbl_1_bss_14->rot.x / 180.0) * temp;
    lbl_1_bss_14->up.y = cos(3.141592653589793 * lbl_1_bss_14->rot.x / 180.0);
    temp = cos(3.141592653589793 * lbl_1_bss_14->rot.y / 180.0);
    lbl_1_bss_14->up.z = sin(3.141592653589793 * lbl_1_bss_14->rot.x / 180.0) * temp;
    Hu3DShadowTPLvlSet(lbl_1_bss_14->tpLvl);
    Hu3DShadowPosSet(&lbl_1_bss_14->pos, &lbl_1_bss_14->up, &lbl_1_bss_14->target);
}

s32 fn_1_1850(float *val, float steps, float start, float target)
{
    float delta;
    float distA;
    float distB;
    s32 ret;

    ret = 0;
    delta = (target - start) / steps;
    *val += delta;
    if (0.0f != delta) {
        ret = (s32)((target - *val) / delta);
    }
    ret++;
    if ((start - target) > 0.0f) {
        distA = start - target;
    } else {
        distA = start - (-1.0f * target);
    }
    if ((start - *val) > 0.0f) {
        distB = start - *val;
    } else {
        distB = start - (-1.0f * *val);
    }
    if (distA <= distB) {
        *val = target;
        ret = 0;
    }
    return ret;
}

MGTIMER *fn_1_1970(s32 type, s32 maxSec, s32 endSec, float x, float y)
{
    MGTIMER *timer;

    timer = MgTimerCreate(type);
    MgTimerParamSet(timer, maxSec * 60, endSec * 60, 0);
    MgTimerPosSet(timer, x, y);
    return timer;
}

void fn_1_19E4(MGTIMER *timer)
{
    MgTimerModeOnSet(timer, 1);
}

BOOL fn_1_1A10(MGTIMER *timer)
{
    return MgTimerDoneCheck(timer);
}

void fn_1_1A38(MGTIMER *timer)
{
    MgTimerDispOff(timer);
    MgTimerKill(timer);
}

/* ---- part_m2.c ---- */
/* debug model pose editor: pad moves the model, prints the values */
s32 fn_1_1A70(M541_STAGE *stage, s16 pad, s16 idx)
{
    s32 ret = 0;
    M541_MODEL *m = &stage->model[idx];

    if (HuPadStkX[pad] > 10) {
        m->posX += 1.0f;
    }
    if (HuPadStkX[pad] < -10) {
        m->posX -= 1.0f;
    }
    if (HuPadStkY[pad] > 10) {
        m->posY += 1.0f;
    }
    if (HuPadStkY[pad] < -10) {
        m->posY -= 1.0f;
    }
    if (HuPadTrigL[pad] > 10) {
        m->posZ += 1.0f;
    }
    if (HuPadTrigR[pad] > 10) {
        m->posZ -= 1.0f;
    }
    if (HuPadSubStkX[pad] > 10) {
        m->rotX += 1.0f;
    }
    if (HuPadSubStkX[pad] < -10) {
        m->rotX -= 1.0f;
    }
    if (HuPadSubStkY[pad] > 10) {
        m->rotY += 1.0f;
    }
    if (HuPadSubStkY[pad] < -10) {
        m->rotY -= 1.0f;
    }
    if (HuPadBtnRep[pad] & 0x100) {
        m->rotZ += 1.0f;
    }
    if (HuPadBtnRep[pad] & 0x200) {
        m->rotZ -= 1.0f;
    }
    print8(25, 136, 1.5f, "OBJ X:%.1f Y:%.1f Z:%.1f RX%.1f RY%.1f RZ%.1f", m->posX, m->posY, m->posZ, m->rotX, m->rotY, m->rotZ);
    print8(25, 154, 1.5f, "OBJW %d", idx);
    return ret;
}

void fn_1_1DC4(void)
{
}

/* debug camera editor */
#define DEADZONE(v, lim) ((((v) < 0) ? -(v) : (v)) < (lim) ? 0 : (v))

void fn_1_1DC8(s16 pad)
{
    u16 btn = HuPadBtn[pad];
    s32 btnDown = HuPadBtnDown[pad];
    s8 stkX = HuPadStkX[pad];
    s8 stkY;
    s8 subX;
    s8 subY;

    stkX = DEADZONE(stkX, 10);
    stkY = HuPadStkY[pad];
    stkY = DEADZONE(stkY, 10);
    subX = HuPadSubStkX[pad];
    subX = DEADZONE(subX, 8);
    subY = HuPadSubStkY[pad];
    subY = DEADZONE(subY, 8);

    if (btn & 0x400) {
        CZoom += 20.0f;
    }
    if (btn & 0x800) {
        CZoom -= 20.0f;
    }
    if (stkX > 0) {
        Center.x += 10.0f;
    }
    if (stkX < 0) {
        Center.x -= 10.0f;
    }
    if (stkY > 0) {
        Center.y += 10.0f;
    }
    if (stkY < 0) {
        Center.y -= 10.0f;
    }
    if (subX > 0) {
        CRot.y += 1.0f;
    }
    if (subX < 0) {
        CRot.y -= 1.0f;
    }
    if (subY > 0) {
        CRot.x += 1.0f;
    }
    if (subY < 0) {
        CRot.x -= 1.0f;
    }
    if (btn & 0x2000) {
        CRot.z += 1.0f;
    }
    if (btn & 0x4000) {
        CRot.z -= 1.0f;
    }
    if (btn & 0x8) {
        Center.z += 10.0f;
    }
    if (btn & 0x4) {
        Center.z -= 10.0f;
    }
    print8(25, 30, 1.5f, "CAMPOS X:%.1f Y:%.1f Z:%.1f ZOOM%.1f", Center.x, Center.y, Center.z, CZoom);
    print8(25, 48, 1.5f, "CAMROT X:%.1f Y:%.1f Z:%.1f", CRot.x, CRot.y, CRot.z);
}

/* save the model's material colours */
void fn_1_2280(s16 idx)
{
    HU3DMODEL *mdl = &Hu3DData[lbl_1_bss_0.player[idx]->actor->mdlId];
    HSFMATERIAL *mat;
    u8 *buf;
    s16 i;

    buf = HuMemDirectMallocNum(HEAP_HEAP, mdl->hsf->materialNum * 3, HU_MEMNUM_OVL);
    lbl_1_bss_2614[idx].colBuf = buf;
    mat = mdl->hsf->material;
    for (i = 0; i < mdl->hsf->materialNum; i++, mat++) {
        *buf++ = mat->color[0];
        *buf++ = mat->color[1];
        *buf++ = mat->color[2];
    }
}

/* fade the material colours; returns TRUE once the fade level is below 40 */
s32 fn_1_2378(s16 idx, float scale)
{
    s32 done = 0;
    HU3DMODEL *mdl = &Hu3DData[lbl_1_bss_0.player[idx]->actor->mdlId];
    HSFMATERIAL *mat;
    s16 i;

    lbl_1_bss_2614[idx].tpLvl *= scale;
    if (lbl_1_bss_2614[idx].tpLvl < 40.0f) {
        done = 1;
    }
    mat = mdl->hsf->material;
    for (i = 0; i < mdl->hsf->materialNum; i++, mat++) {
        mat->color[0] = mat->color[0] * scale;
        mat->color[1] = mat->color[1] * scale;
        mat->color[2] = mat->color[2] * scale;
    }
    return done;
}

/* restore the saved material colours */
void fn_1_24E8(s16 idx)
{
    HU3DMODEL *mdl;
    HSFMATERIAL *mat;
    u8 *buf;
    s16 i;

    lbl_1_bss_2614[idx].tpLvl = 255.0f;
    mdl = &Hu3DData[lbl_1_bss_0.player[idx]->actor->mdlId];
    buf = lbl_1_bss_2614[idx].colBuf;
    mat = mdl->hsf->material;
    for (i = 0; i < mdl->hsf->materialNum; i++, mat++) {
        mat->color[0] = *buf++;
        mat->color[1] = *buf++;
        mat->color[2] = *buf++;
    }
}

void fn_1_25E4(void)
{
    lbl_1_bss_2734.jingleId = HuAudJinglePlay(0x83);
}

void fn_1_2614(void)
{
    lbl_1_bss_2734.bgmId = HuAudBGMPlay(0x56);
}

void fn_1_2644(s16 which, s16 speed)
{
    s16 id;

    if (which != 0) {
        id = lbl_1_bss_2734.bgmId;
    } else {
        id = lbl_1_bss_2734.jingleId;
    }
    HuAudSStreamFadeOut(id, speed);
}

void fn_1_26AC(s16 which)
{
    s16 id;

    if (which != 0) {
        id = lbl_1_bss_2734.bgmId;
    } else {
        id = lbl_1_bss_2734.jingleId;
    }
    HuAudSStreamStop(id);
}

void fn_1_270C(s16 idx)
{
    if ((idx == 0 || idx == 1 || idx == 2) && lbl_1_bss_2734.playerHook[idx] != -1) {
        return;
    }
    lbl_1_bss_2734.playerHook[idx] = 1;
    lbl_1_bss_2734.fxId[idx] = HuAudFXPlay(lbl_1_data_360[idx]);
}

void fn_1_27C8(s16 idx, s16 vol, s16 pan)
{
    if ((idx == 0 || idx == 1 || idx == 2) && lbl_1_bss_2734.playerHook[idx] != -1) {
        return;
    }
    lbl_1_bss_2734.playerHook[idx] = 1;
    lbl_1_bss_2734.fxId[idx] = HuAudFXPlayVolPan(lbl_1_data_360[idx], vol, pan);
}

void fn_1_2894(s16 idx, s16 pan)
{
    if (lbl_1_bss_2734.playerHook[idx] == 1 && HuAudFXStatusGet(lbl_1_bss_2734.fxId[idx]) == 2) {
        HuAudFXPanning(lbl_1_bss_2734.fxId[idx], pan);
    }
}

void fn_1_2924(s16 idx)
{
    if (lbl_1_bss_2734.playerHook[idx] == 1
        && (HuAudFXStatusGet(lbl_1_bss_2734.fxId[idx]) == 2 || HuAudFXStatusGet(lbl_1_bss_2734.fxId[idx]) == 3)) {
        lbl_1_bss_2734.playerHook[idx] = -1;
        HuAudFXStop(lbl_1_bss_2734.fxId[idx]);
    }
}

void fn_1_29EC(s16 idx)
{
    if (lbl_1_bss_2734.playerHook[idx] == 1
        && (HuAudFXStatusGet(lbl_1_bss_2734.fxId[idx]) == 2 || HuAudFXStatusGet(lbl_1_bss_2734.fxId[idx]) == 3)) {
        lbl_1_bss_2734.playerHook[idx] = -1;
        HuAudFXStop(lbl_1_bss_2734.fxId[idx]);
    }
}

void fn_1_2AB4(void)
{
    HuAudAllStop();
}
