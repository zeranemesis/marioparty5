#include "math.h"
#include "string.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/printfunc.h"
#include "game/mg/seqman.h"
#include "game/mg/timer.h"

typedef void (*M555VoidFunc)(void);
extern const M555VoidFunc _ctors[];
extern const M555VoidFunc _dtors[];

int _prolog(void)
{
    const M555VoidFunc *ctors = _ctors;

    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const M555VoidFunc *dtors = _dtors;

    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

void ObjectSetup(void)
{
    s16 i;

    lbl_1_bss_3020 = omInitObjMan(0x32, 0x2000);
    omGameSysInit(lbl_1_bss_3020);
    fn_1_1A9A4(&lbl_1_bss_2D38, lbl_1_bss_1A84, lbl_1_data_90);
    lbl_1_bss_8 = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M555_CAMBLK), HU_MEMNUM_OVL);
    memset(lbl_1_bss_8, 0, sizeof(M555_CAMBLK));
    lbl_1_bss_4 = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M555_LIGHT), HU_MEMNUM_OVL);
    memset(lbl_1_bss_4, 0, sizeof(M555_LIGHT));
    lbl_1_bss_0 = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M555_SHADOW), HU_MEMNUM_OVL);
    memset(lbl_1_bss_0, 0, sizeof(M555_SHADOW));
    fn_1_494(0);
    fn_1_EDC();
    fn_1_13E4();
    fn_1_1AC04(0, lbl_1_data_28[lbl_1_bss_2D38.mdl0[0].charNo]);
    fn_1_1AC04(1, lbl_1_data_28[lbl_1_bss_2D38.mdl0[1].charNo]);
    for (i = 0; i < 2; i++) {
        fn_1_1AC24(i, &lbl_1_bss_1A7C[i]);
    }
    fn_1_1AC44();
    fn_1_1B2F8();
    omAddObjEx(lbl_1_bss_3020, 0x7FDA, 0, 0, -1, fn_1_1AF38);
    omAddObjEx(lbl_1_bss_3020, 0x7FDA, 0, 0, -1, fn_1_1B4B0);
    omAddObjEx(lbl_1_bss_3020, 0x7FDA, 0, 0, -1, fn_1_490);
    Hu3DBGColorSet(0, 0, 0);
    MgSeqCreate(&lbl_1_data_0);
    MgSeqModeDelaySet(3, 1);
    MgSeqModeDelaySet(2, 1);
    MgSeqModeDelaySet(2, 0x1770);
    MgSeqModeDelaySet(7, 0x1770);
    MgSeqModeDelaySet(9, 0x3C);
}

void fn_1_308(s16 mode, s16 frameNo)
{
    if (fn_1_69C4(&lbl_1_bss_2D38) != 0) {
        MgSeqModeNext();
    }
}

void fn_1_33C(s16 mode, s16 frameNo)
{
    if (fn_1_6A0C() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_368(s16 mode, s16 frameNo)
{
    if (fn_1_6A60() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_394(s16 mode, s16 frameNo)
{
    if (fn_1_6AF0() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_3C0(s16 mode, s16 frameNo)
{
    if (fn_1_6DF0() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_3EC(s16 mode, s16 frameNo)
{
    if (fn_1_6ECC() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_418(s16 mode, s16 frameNo)
{
    if (fn_1_6F3C() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_444(s16 mode, s16 frameNo)
{
    fn_1_71B4();
}

void fn_1_464(s16 mode, s16 frameNo)
{
    if (fn_1_7364() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_490(OMOBJ *obj)
{
}

#define CAM(no) (((M555_CAM *)lbl_1_bss_8)[no])

void fn_1_494(s16 no)
{
    CAM(no).no = no;
    CAM(no).target.x = lbl_1_data_150[no][0];
    CAM(no).target.y = lbl_1_data_150[no][1];
    CAM(no).target.z = lbl_1_data_150[no][2];
    CAM(no).rot.x = lbl_1_data_150[no][3];
    CAM(no).rot.y = lbl_1_data_150[no][4];
    CAM(no).rot.z = lbl_1_data_150[no][5];
    CAM(no).zoom = lbl_1_data_150[no][6];
    CAM(no).aspect = lbl_1_data_150[no][7];
    CAM(no).vpX = lbl_1_data_150[no][8];
    CAM(no).vpY = lbl_1_data_150[no][9];
    CAM(no).vpW = lbl_1_data_150[no][10];
    CAM(no).vpH = lbl_1_data_150[no][11];
    CAM(no).fov = lbl_1_data_150[no][12];
    Hu3DCameraCreate(lbl_1_data_220[no]);
    Hu3DCameraViewportSet(lbl_1_data_220[no], CAM(no).vpX, CAM(no).vpY, CAM(no).vpW, CAM(no).vpH, 0.0f, 1.0f);
    Hu3DCameraPerspectiveSet(lbl_1_data_220[no], CAM(no).fov, 20.0f, 40000.0f, CAM(no).aspect);
    fn_1_964(no);
    Center.x = lbl_1_data_150[0][0];
    Center.y = lbl_1_data_150[0][1];
    Center.z = lbl_1_data_150[0][2];
    CRot.x = lbl_1_data_150[0][3];
    CRot.y = lbl_1_data_150[0][4];
    CRot.z = lbl_1_data_150[0][5];
    CZoom = lbl_1_data_150[no][6];
}

void fn_1_964(s16 no)
{
    f32 unused;

    CAM(no).pos.x = CAM(no).target.x + (CAM(no).zoom * (cos((3.141592653589793 * CAM(no).rot.x) / 180.0) * sin((3.141592653589793 * CAM(no).rot.y) / 180.0)));
    CAM(no).pos.y = CAM(no).target.y + (CAM(no).zoom * -sin((3.141592653589793 * CAM(no).rot.x) / 180.0));
    CAM(no).pos.z = CAM(no).target.z + (CAM(no).zoom * (cos((3.141592653589793 * CAM(no).rot.x) / 180.0) * cos((3.141592653589793 * CAM(no).rot.y) / 180.0)));
    CAM(no).up.x = sin((3.141592653589793 * CAM(no).rot.x) / 180.0) * sin((3.141592653589793 * CAM(no).rot.y) / 180.0);
    CAM(no).up.y = cos((3.141592653589793 * CAM(no).rot.x) / 180.0);
    CAM(no).up.z = sin((3.141592653589793 * CAM(no).rot.x) / 180.0) * cos((3.141592653589793 * CAM(no).rot.y) / 180.0);
    unused = CAM(no).target.z;
    Hu3DCameraPosSet(lbl_1_data_220[no], CAM(no).pos.x, CAM(no).pos.y, CAM(no).pos.z, CAM(no).up.x, CAM(no).up.y, CAM(no).up.z, CAM(no).target.x, CAM(no).target.y, CAM(no).target.z);
}

void fn_1_EDC(void)
{
    Hu3DLighInit();
    lbl_1_bss_4->pos.x = 0.0f;
    lbl_1_bss_4->pos.y = 0.0f;
    lbl_1_bss_4->pos.z = 0.0f;
    lbl_1_bss_4->target.x = 0.0f;
    lbl_1_bss_4->target.y = 0.0f;
    lbl_1_bss_4->target.z = 0.0f;
    lbl_1_bss_4->rot.x = -23.0f;
    lbl_1_bss_4->rot.y = 0.0f;
    lbl_1_bss_4->rot.z = 0.0f;
    lbl_1_bss_4->dist = 1000.0f;
    lbl_1_bss_4->color.r = 0xFF;
    lbl_1_bss_4->color.g = 0xFF;
    lbl_1_bss_4->color.b = 0xFF;
    lbl_1_bss_4->color.a = 0xFF;
    PSVECSubtract(&lbl_1_bss_4->target, &lbl_1_bss_4->pos, &lbl_1_bss_4->dir);
    lbl_1_bss_4->id = Hu3DGLightCreateV(&lbl_1_bss_4->pos, &lbl_1_bss_4->dir, &lbl_1_bss_4->color);
    fn_1_110C();
    Hu3DGLightStaticSet(lbl_1_bss_4->id, FALSE);
    Hu3DGLightInfinitytSet(lbl_1_bss_4->id);
}

void fn_1_110C(void)
{
    lbl_1_bss_4->pos.x = lbl_1_bss_4->target.x + (lbl_1_bss_4->dist * (cos((3.141592653589793 * lbl_1_bss_4->rot.x) / 180.0) * sin((3.141592653589793 * lbl_1_bss_4->rot.y) / 180.0)));
    lbl_1_bss_4->pos.y = lbl_1_bss_4->target.y + (lbl_1_bss_4->dist * -sin((3.141592653589793 * lbl_1_bss_4->rot.x) / 180.0));
    lbl_1_bss_4->pos.z = lbl_1_bss_4->target.z + (lbl_1_bss_4->dist * (cos((3.141592653589793 * lbl_1_bss_4->rot.x) / 180.0) * cos((3.141592653589793 * lbl_1_bss_4->rot.y) / 180.0)));
    PSVECSubtract(&lbl_1_bss_4->target, &lbl_1_bss_4->pos, &lbl_1_bss_4->dir);
    PSVECNormalize(&lbl_1_bss_4->dir, &lbl_1_bss_4->dir);
    Hu3DGLightPosSetV(lbl_1_bss_4->id, &lbl_1_bss_4->pos, &lbl_1_bss_4->dir);
    Hu3DGLightColorSet(lbl_1_bss_4->id, lbl_1_bss_4->color.r, lbl_1_bss_4->color.g, lbl_1_bss_4->color.b, lbl_1_bss_4->color.a);
}

void fn_1_13E4(void)
{
    Hu3DShadowCreate(30.0f, 20.0f, 40000.0f);
    lbl_1_bss_0->rot.x = -90.0f;
    lbl_1_bss_0->rot.y = 0.0f;
    lbl_1_bss_0->rot.z = 0.0f;
    lbl_1_bss_0->target.x = 0.0f;
    lbl_1_bss_0->target.y = 0.0f;
    lbl_1_bss_0->target.z = 0.0f;
    lbl_1_bss_0->tpLvl = 0.5f;
    lbl_1_bss_0->dist = 3500.0f;
    fn_1_150C();
}

void fn_1_150C(void)
{
    lbl_1_bss_0->pos.x = lbl_1_bss_0->target.x + (lbl_1_bss_0->dist * (cos((3.141592653589793 * lbl_1_bss_0->rot.x) / 180.0) * sin((3.141592653589793 * lbl_1_bss_0->rot.y) / 180.0)));
    lbl_1_bss_0->pos.y = lbl_1_bss_0->target.y + (lbl_1_bss_0->dist * -sin((3.141592653589793 * lbl_1_bss_0->rot.x) / 180.0));
    lbl_1_bss_0->pos.z = lbl_1_bss_0->target.z + (lbl_1_bss_0->dist * (cos((3.141592653589793 * lbl_1_bss_0->rot.x) / 180.0) * cos((3.141592653589793 * lbl_1_bss_0->rot.y) / 180.0)));
    lbl_1_bss_0->up.x = sin((3.141592653589793 * lbl_1_bss_0->rot.x) / 180.0) * sin((3.141592653589793 * lbl_1_bss_0->rot.y) / 180.0);
    lbl_1_bss_0->up.y = cos((3.141592653589793 * lbl_1_bss_0->rot.x) / 180.0);
    lbl_1_bss_0->up.z = sin((3.141592653589793 * lbl_1_bss_0->rot.x) / 180.0) * cos((3.141592653589793 * lbl_1_bss_0->rot.y) / 180.0);
    Hu3DShadowTPLvlSet(lbl_1_bss_0->tpLvl);
    Hu3DShadowPosSet(&lbl_1_bss_0->pos, &lbl_1_bss_0->up, &lbl_1_bss_0->target);
}

s32 fn_1_1898(f32 *value, f32 time, f32 from, f32 to)
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
    step = absA;
    if (step <= absB) {
        *value = to;
        ret = 0;
    }
    return ret;
}

s32 fn_1_19B8(f32 *value, f32 time, f32 from, f32 to)
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

MGTIMER *fn_1_1AD4(int type, int maxTime, int endTime, f32 x, f32 y)
{
    MGTIMER *timer = MgTimerCreate(type);

    MgTimerParamSet(timer, maxTime * 60, endTime * 60, 0);
    MgTimerPosSet(timer, x, y);
    return timer;
}

void fn_1_1B48(MGTIMER *timer)
{
    MgTimerModeOnSet(timer, 1);
}

void fn_1_1B74(MGTIMER *timer)
{
    MgTimerDoneCheck(timer);
}

void fn_1_1B9C(MGTIMER *timer)
{
    MgTimerDispOff(timer);
    MgTimerKill(timer);
}

s32 fn_1_1BD4(M555_CTRL *ctrl, s16 pad, s16 no)
{
    M555_MODELENTRY *obj = &ctrl->mdl[no];

    obj->pos.x += HuPadStkX[pad];
    obj->pos.y += HuPadStkY[pad];
    obj->pos.z += HuPadTrigL[pad];
    obj->pos.z -= HuPadTrigR[pad];
    obj->rot.x += HuPadSubStkX[pad] / 10;
    obj->rot.y += HuPadSubStkY[pad] / 10;
    if (HuPadSubStkX[pad] > 10) {
        obj->rot.x += 1.0f;
    }
    if (HuPadSubStkX[pad] < -10) {
        obj->rot.x -= 1.0f;
    }
    if (HuPadSubStkY[pad] > 10) {
        obj->rot.y += 1.0f;
    }
    if (HuPadSubStkY[pad] < -10) {
        obj->rot.y -= 1.0f;
    }
    if (HuPadBtnRep[pad] & 0x100) {
        obj->rot.z += 1.0f;
    }
    if (HuPadBtnRep[pad] & 0x200) {
        obj->rot.z -= 1.0f;
    }
    fontcolor = 0;
    print8(0x19, 0x88, 1.5f, lbl_1_data_240, obj->pos.x, obj->pos.y, obj->pos.z, obj->rot.x, obj->rot.y, obj->rot.z);
    print8(0x19, 0x9A, 1.5f, lbl_1_data_26E, no);
    return 0;
}
