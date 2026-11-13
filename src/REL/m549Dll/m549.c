#include "types.h"

#include "string.h"
#include "game/gamework.h"
#include "game/object.h"
#include "game/hu3d.h"
#include "game/data.h"
#include "game/memory.h"
#include "game/sprite.h"
#include "game/audio.h"
#include "game/frand.h"
#include "game/pad.h"
#include "game/printfunc.h"
#include "game/mg/seqman.h"
#include "game/mg/timer.h"
#include "REL/m549Dll.h"

double sin(double x);
double cos(double x);

typedef void (*M549VoidFunc)(void);
extern const M549VoidFunc _ctors[];
extern const M549VoidFunc _dtors[];

/* unit-owned .bss, declared in reverse address order so the offsets match the original */
s16 gap_05_000015DE_bss;
s16 lbl_1_bss_15DC;
void *lbl_1_bss_15D8;
OMOBJMAN *lbl_1_bss_15D4;
M549State lbl_1_bss_DD0;
M549StageModel lbl_1_bss_628[70];
s16 lbl_1_bss_600[2][10];
M549Ctrl lbl_1_bss_580;
M549Char lbl_1_bss_3B0[2];
M549Obj lbl_1_bss_158[10];
M549Impact lbl_1_bss_18[10];
M549Cam *lbl_1_bss_14;
M549Light *lbl_1_bss_10;
M549Shadow *lbl_1_bss_C;
M549Globals lbl_1_bss_0;

/* unit-owned .data, in original address order */
MGSEQ_PARAM lbl_1_data_0 = {
    0, 0,
    fn_1_320, fn_1_354, fn_1_380,
    fn_1_3AC, fn_1_3D8, fn_1_404,
    fn_1_430, fn_1_45C, fn_1_47C,
};
s32 lbl_1_data_28[13][10] = {
    { 0x8C0000, 0x8C0002, 0x8C002B, 0x8C0006, 0x8C002E, 0x8700A7, 0x8700AA, 0x8C007C, 0x8C0081, 0x8700B8 },
    { 0x330000, 0x330002, 0x33002B, 0x330006, 0x33002E, 0x2E00A7, 0x2E00AA, 0x33007C, 0x330081, 0x2E00B8 },
    { 0xB70000, 0xB70002, 0xB7002B, 0xB70006, 0xB7002E, 0xB200A7, 0xB200AA, 0xB7007C, 0xB70081, 0xB200B8 },
    { 0xED0000, 0xED0002, 0xED002B, 0xED0006, 0xED002E, 0xE800A7, 0xE800AA, 0xED007C, 0xED0081, 0xE800B8 },
    { 0xE60000, 0xE60002, 0xE6002B, 0xE60006, 0xE6002E, 0xE100A7, 0xE100AA, 0xE6007C, 0xE60081, 0xE100B8 },
    { 0x170000, 0x170002, 0x17002B, 0x170006, 0x17002E, 0x1200A7, 0x1200AA, 0x17007C, 0x170081, 0x1200B8 },
    { 0xE00000, 0xE00002, 0xE0002B, 0xE00006, 0xE0002E, 0xDB00A7, 0xDB00AA, 0xE0007C, 0xE00081, 0xDB00B8 },
    { 0x2D0000, 0x2D0002, 0x2D002B, 0x2D0006, 0x2D002E, 0x2800A7, 0x2800AA, 0x2D007C, 0x2D0081, 0x2800B8 },
    { 0xC90000, 0xC90002, 0xC9002B, 0xC90006, 0xC9002E, 0xC400A7, 0xC400AA, 0xC9007C, 0xC90081, 0xC400B8 },
    { 0xB00000, 0xB00002, 0xB0002B, 0xB00006, 0xB0002E, 0x9C00A7, 0x9C00AA, 0xB0007C, 0xB00081, 0x9C00B8 },
    { 0xB00000, 0xB00002, 0xB0002B, 0xB00006, 0xB0002E, 0x9C00A7, 0x9C00AA, 0xB0007C, 0xB00081, 0x9C00B8 },
    { 0xB00000, 0xB00002, 0xB0002B, 0xB00006, 0xB0002E, 0x9C00A7, 0x9C00AA, 0xB0007C, 0xB00081, 0x9C00B8 },
    { 0xB00000, 0xB00002, 0xB0002B, 0xB00006, 0xB0002E, 0x9C00A7, 0x9C00AA, 0xB0007C, 0xB00081, 0x9C00B8 },
};
s32 lbl_1_data_230[22] = {
    0x6B0000, 0x6B0001, 0x6B0002, 0x6B0003, 0x6B0004, 0x6B0005, 0x6B0006, 0x6B0007,
    0x6B0009, 0x6B000A, 0x6B000B, 0x6B000C, 0x6B000D, 0x6B000E, 0x6B000F, 0x6B0010,
    0x6B0011, 0x6B0012, 0x6B0013, 0x6B0014, 0x6B0015, 0x6B0018,
};
M549CamInit lbl_1_data_288[4] = {
    { { 0.0f, 131.0f, 0.0f }, -26.0f, 0.0f, 0.0f, 300.0f, 1.33f, 0.0f, 0.0f, 640.0f, 480.0f, 45.0f },
    { { 0.0f, 131.0f, 0.0f }, -26.0f, 0.0f, 0.0f, 300.0f, 1.33f, 0.0f, 0.0f, 640.0f, 480.0f, 45.0f },
    { { 0.0f, 131.0f, 0.0f }, -26.0f, 0.0f, 0.0f, 1602.0f, 1.33f, 0.0f, 0.0f, 640.0f, 480.0f, 45.0f },
    { { 0.0f, 0.0f, 0.0f }, 0.0f, 0.0f, 500.0f, 500.0f, 1.33f, 0.0f, 0.0f, 640.0f, 480.0f, 45.0f },
};
s16 lbl_1_data_358[16] = {
    0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000, 0x8000,
};
char lbl_1_data_378[0x2E] = "OBJ X:%.1f Y:%.1f Z:%.1f RX%.1f RY%.1f RZ%.1f";
char lbl_1_data_3A6[8] = "OBJW %d";
char lbl_1_data_3AE[0x25] = "CAMPOS X:%.1f Y:%.1f Z:%.1f ZOOM%.1f";
char lbl_1_data_3D3[0x1D] = "CAMROT X:%.1f Y:%.1f Z:%.1f";
M549PtclWork lbl_1_data_3F0 = {
    0, 0,
    { 0xFF, 0xFF, 0xFF, 0xFF }, { 0x00, 0x00, 0x00, 0xFF },
    0.0f, 2.0f, 1.0f,
    0.85f, 0.85f, 0.85f,
    0.2f,
    0.0f, 1.0f, -5.0f, 0.02f,
};
s16 lbl_1_data_428[12] = { 0x845, 0x845, 0x846, 0x847, 0x848, 0x849, 0x84A, 0x84B, 0x84C, 0x84D, 0x84E, 0 };

int _prolog(void)
{
    const M549VoidFunc *ctors = _ctors;

    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const M549VoidFunc *dtors = _dtors;

    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

void ObjectSetup(void)
{
    s16 i;

    lbl_1_bss_15D4 = MgActorObjectSetup();
    fn_1_11BC0(&lbl_1_bss_DD0, lbl_1_bss_628, lbl_1_data_230);
    lbl_1_bss_14 = HuMemDirectMallocNum(HEAP_HEAP, 0x148, 0x10000000);
    memset(lbl_1_bss_14, 0, 0x148);
    lbl_1_bss_10 = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M549Light), 0x10000000);
    memset(lbl_1_bss_10, 0, sizeof(M549Light));
    lbl_1_bss_C = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M549Shadow), 0x10000000);
    memset(lbl_1_bss_C, 0, sizeof(M549Shadow));
    fn_1_4AC(0);
    fn_1_4AC(1);
    fn_1_EF4();
    fn_1_13FC();
    fn_1_11E0C();
    fn_1_12450(0, lbl_1_data_28[lbl_1_bss_DD0.chars[0].charNo]);
    fn_1_12450(1, lbl_1_data_28[lbl_1_bss_DD0.chars[1].charNo]);
    for (i = 0; i < 2; i++) {
        fn_1_12470(i, lbl_1_bss_600[i]);
    }
    omAddObjEx(lbl_1_bss_15D4, 0x7FDA, 0, 0, -1, (OMOBJFUNC)fn_1_12494);
    omAddObjEx(lbl_1_bss_15D4, 0x7FDA, 0, 0, -1, (OMOBJFUNC)fn_1_12A60);
    omAddObjEx(lbl_1_bss_15D4, 0x7FDA, 0, 0, -1, (OMOBJFUNC)fn_1_4A8);
    omAddObjEx(lbl_1_bss_15D4, 0x7FDA, 0, 0, -1, (OMOBJFUNC)fn_1_12E80);
    Hu3DBGColorSet(0, 0, 0);
    MgSeqCreate(&lbl_1_data_0);
    MgSeqModeDelaySet(3, 1);
    MgSeqModeDelaySet(2, 1);
    MgSeqModeDelaySet(2, 0x1770);
    MgSeqModeDelaySet(7, 0x1770);
    MgSeqModeDelaySet(9, 0x3C);
}

void fn_1_320(s16 mode, s16 frameNo)
{
    if (fn_1_4EE0(&lbl_1_bss_DD0) != 0) {
        MgSeqModeNext();
    }
}

void fn_1_354(s16 mode, s16 frameNo)
{
    if (fn_1_4FFC() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_380(s16 mode, s16 frameNo)
{
    if (fn_1_50A4() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_3AC(s16 mode, s16 frameNo)
{
    if (fn_1_5184() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_3D8(s16 mode, s16 frameNo)
{
    if (fn_1_569C() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_404(s16 mode, s16 frameNo)
{
    if (fn_1_5788() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_430(s16 mode, s16 frameNo)
{
    if (fn_1_57D8() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_45C(s16 mode, s16 frameNo)
{
    fn_1_5A00();
}

void fn_1_47C(s16 mode, s16 frameNo)
{
    if (fn_1_5A40() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_4A8(OMOBJ *obj)
{
}

void fn_1_4AC(s16 arg0)
{
    lbl_1_bss_14[arg0].idx = arg0;
    lbl_1_bss_14[arg0].center.x = lbl_1_data_288[arg0].center.x;
    lbl_1_bss_14[arg0].center.y = lbl_1_data_288[arg0].center.y;
    lbl_1_bss_14[arg0].center.z = lbl_1_data_288[arg0].center.z;
    lbl_1_bss_14[arg0].rotX = lbl_1_data_288[arg0].rotX;
    lbl_1_bss_14[arg0].rotY = lbl_1_data_288[arg0].rotY;
    lbl_1_bss_14[arg0].rotZ = lbl_1_data_288[arg0].rotZ;
    lbl_1_bss_14[arg0].zoom = lbl_1_data_288[arg0].zoom;
    lbl_1_bss_14[arg0].aspect = lbl_1_data_288[arg0].aspect;
    lbl_1_bss_14[arg0].vpX = lbl_1_data_288[arg0].vpX;
    lbl_1_bss_14[arg0].vpY = lbl_1_data_288[arg0].vpY;
    lbl_1_bss_14[arg0].vpW = lbl_1_data_288[arg0].vpW;
    lbl_1_bss_14[arg0].vpH = lbl_1_data_288[arg0].vpH;
    lbl_1_bss_14[arg0].fov = lbl_1_data_288[arg0].fov;
    Hu3DCameraCreate(lbl_1_data_358[arg0]);
    Hu3DCameraViewportSet(lbl_1_data_358[arg0], lbl_1_bss_14[arg0].vpX, lbl_1_bss_14[arg0].vpY,
                          lbl_1_bss_14[arg0].vpW, lbl_1_bss_14[arg0].vpH, 0.0f, 1.0f);
    Hu3DCameraPerspectiveSet(lbl_1_data_358[arg0], lbl_1_bss_14[arg0].fov, 20.0f, 40000.0f, lbl_1_bss_14[arg0].aspect);
    fn_1_97C(arg0);
    Center.x = lbl_1_data_288[0].center.x;
    Center.y = lbl_1_data_288[0].center.y;
    Center.z = lbl_1_data_288[0].center.z;
    CRot.x = lbl_1_data_288[0].rotX;
    CRot.y = lbl_1_data_288[0].rotY;
    CRot.z = lbl_1_data_288[0].rotZ;
    CZoom = lbl_1_data_288[arg0].zoom;
}

void fn_1_97C(s16 arg0)
{
    f64 temp;

    temp = sin(3.141592653589793 * lbl_1_bss_14[arg0].rotY / 180.0);
    lbl_1_bss_14[arg0].pos.x = lbl_1_bss_14[arg0].center.x
        + lbl_1_bss_14[arg0].zoom * (cos(3.141592653589793 * lbl_1_bss_14[arg0].rotX / 180.0) * temp);
    lbl_1_bss_14[arg0].pos.y = lbl_1_bss_14[arg0].center.y
        + lbl_1_bss_14[arg0].zoom * -sin(3.141592653589793 * lbl_1_bss_14[arg0].rotX / 180.0);
    temp = cos(3.141592653589793 * lbl_1_bss_14[arg0].rotY / 180.0);
    lbl_1_bss_14[arg0].pos.z = lbl_1_bss_14[arg0].center.z
        + lbl_1_bss_14[arg0].zoom * (cos(3.141592653589793 * lbl_1_bss_14[arg0].rotX / 180.0) * temp);
    temp = sin(3.141592653589793 * lbl_1_bss_14[arg0].rotY / 180.0);
    lbl_1_bss_14[arg0].up.x = sin(3.141592653589793 * lbl_1_bss_14[arg0].rotX / 180.0) * temp;
    lbl_1_bss_14[arg0].up.y = cos(3.141592653589793 * lbl_1_bss_14[arg0].rotX / 180.0);
    temp = cos(3.141592653589793 * lbl_1_bss_14[arg0].rotY / 180.0);
    lbl_1_bss_14[arg0].up.z = sin(3.141592653589793 * lbl_1_bss_14[arg0].rotX / 180.0) * temp;
    Hu3DCameraPosSet(lbl_1_data_358[arg0], lbl_1_bss_14[arg0].pos.x, lbl_1_bss_14[arg0].pos.y, lbl_1_bss_14[arg0].pos.z,
                     lbl_1_bss_14[arg0].up.x, lbl_1_bss_14[arg0].up.y, lbl_1_bss_14[arg0].up.z,
                     lbl_1_bss_14[arg0].center.x, lbl_1_bss_14[arg0].center.y, lbl_1_bss_14[arg0].center.z);
}

void fn_1_EF4(void)
{
    Hu3DLighInit();
    lbl_1_bss_10->pos.x = 0.0f;
    lbl_1_bss_10->pos.y = 0.0f;
    lbl_1_bss_10->pos.z = 0.0f;
    lbl_1_bss_10->target.x = 0.0f;
    lbl_1_bss_10->target.y = 0.0f;
    lbl_1_bss_10->target.z = 0.0f;
    lbl_1_bss_10->rotX = -45.0f;
    lbl_1_bss_10->rotY = 0.0f;
    lbl_1_bss_10->rotZ = 0.0f;
    lbl_1_bss_10->dist = 1000.0f;
    lbl_1_bss_10->color.r = 0xFF;
    lbl_1_bss_10->color.g = 0xFF;
    lbl_1_bss_10->color.b = 0xFF;
    lbl_1_bss_10->color.a = 0xFF;
    PSVECSubtract((Vec *)&lbl_1_bss_10->target, (Vec *)&lbl_1_bss_10->pos, (Vec *)&lbl_1_bss_10->dir);
    lbl_1_bss_10->id = Hu3DGLightCreateV((HuVecF *)&lbl_1_bss_10->pos, (HuVecF *)&lbl_1_bss_10->dir, &lbl_1_bss_10->color);
    fn_1_1124();
    Hu3DGLightStaticSet(lbl_1_bss_10->id, 1);
    Hu3DGLightInfinitytSet(lbl_1_bss_10->id);
}

void fn_1_1124(void)
{
    f64 temp;

    temp = sin(3.141592653589793 * lbl_1_bss_10->rotY / 180.0);
    lbl_1_bss_10->pos.x = lbl_1_bss_10->target.x
        + lbl_1_bss_10->dist * (cos(3.141592653589793 * lbl_1_bss_10->rotX / 180.0) * temp);
    lbl_1_bss_10->pos.y = lbl_1_bss_10->target.y
        + lbl_1_bss_10->dist * -sin(3.141592653589793 * lbl_1_bss_10->rotX / 180.0);
    temp = cos(3.141592653589793 * lbl_1_bss_10->rotY / 180.0);
    lbl_1_bss_10->pos.z = lbl_1_bss_10->target.z
        + lbl_1_bss_10->dist * (cos(3.141592653589793 * lbl_1_bss_10->rotX / 180.0) * temp);
    PSVECSubtract((Vec *)&lbl_1_bss_10->target, (Vec *)&lbl_1_bss_10->pos, (Vec *)&lbl_1_bss_10->dir);
    PSVECNormalize((Vec *)&lbl_1_bss_10->dir, (Vec *)&lbl_1_bss_10->dir);
    Hu3DGLightPosSetV(lbl_1_bss_10->id, (HuVecF *)&lbl_1_bss_10->pos, (HuVecF *)&lbl_1_bss_10->dir);
    Hu3DGLightColorSet(lbl_1_bss_10->id, lbl_1_bss_10->color.r, lbl_1_bss_10->color.g, lbl_1_bss_10->color.b,
                       lbl_1_bss_10->color.a);
}

void fn_1_13FC(void)
{
    Hu3DShadowCreate(30.0f, 20.0f, 40000.0f);
    lbl_1_bss_C->rotX = -60.0f;
    lbl_1_bss_C->rotY = 0.0f;
    lbl_1_bss_C->rotZ = 0.0f;
    lbl_1_bss_C->target.x = 0.0f;
    lbl_1_bss_C->target.y = 0.0f;
    lbl_1_bss_C->target.z = 0.0f;
    lbl_1_bss_C->tpLvl = 0.5f;
    lbl_1_bss_C->dist = 2300.0f;
    fn_1_1524();
}

void fn_1_1524(void)
{
    f64 temp;

    temp = sin(3.141592653589793 * lbl_1_bss_C->rotY / 180.0);
    lbl_1_bss_C->pos.x = lbl_1_bss_C->target.x
        + lbl_1_bss_C->dist * (cos(3.141592653589793 * lbl_1_bss_C->rotX / 180.0) * temp);
    lbl_1_bss_C->pos.y = lbl_1_bss_C->target.y
        + lbl_1_bss_C->dist * -sin(3.141592653589793 * lbl_1_bss_C->rotX / 180.0);
    temp = cos(3.141592653589793 * lbl_1_bss_C->rotY / 180.0);
    lbl_1_bss_C->pos.z = lbl_1_bss_C->target.z
        + lbl_1_bss_C->dist * (cos(3.141592653589793 * lbl_1_bss_C->rotX / 180.0) * temp);
    temp = sin(3.141592653589793 * lbl_1_bss_C->rotY / 180.0);
    lbl_1_bss_C->up.x = sin(3.141592653589793 * lbl_1_bss_C->rotX / 180.0) * temp;
    lbl_1_bss_C->up.y = cos(3.141592653589793 * lbl_1_bss_C->rotX / 180.0);
    temp = cos(3.141592653589793 * lbl_1_bss_C->rotY / 180.0);
    lbl_1_bss_C->up.z = sin(3.141592653589793 * lbl_1_bss_C->rotX / 180.0) * temp;
    Hu3DShadowTPLvlSet(lbl_1_bss_C->tpLvl);
    Hu3DShadowPosSet((HuVecF *)&lbl_1_bss_C->pos, (HuVecF *)&lbl_1_bss_C->up, (HuVecF *)&lbl_1_bss_C->target);
}

s32 fn_1_18B0(f32 *arg0, f32 steps, f32 start, f32 target)
{
    f32 delta;
    f32 distA;
    f32 distB;
    s32 ret;

    ret = 0;
    delta = (target - start) / steps;
    *arg0 += delta;
    if (0.0f != delta) {
        ret = (s32)((target - *arg0) / delta);
    }
    ret++;
    if ((start - target) > 0.0f) {
        distA = start - target;
    } else {
        distA = start - (-1.0f * target);
    }
    if ((start - *arg0) > 0.0f) {
        distB = start - *arg0;
    } else {
        distB = start - (-1.0f * *arg0);
    }
    if (distA <= distB) {
        *arg0 = target;
        ret = 0;
    }
    return ret;
}

s32 fn_1_19D0(f32 *arg0, f32 steps, f32 start, f32 target)
{
    f32 delta;
    f32 distA;
    f32 distB;
    s32 ret;

    ret = 0;
    delta = (target - start) / steps;
    *arg0 += delta;
    if (0.0f != delta) {
        ret = (s32)((target - *arg0) / delta);
    }
    ret++;
    if ((start - target) > 0.0f) {
        distA = start - target;
    } else {
        distA = start - (-1.0f * target);
    }
    if ((start - *arg0) > 0.0f) {
        distB = start - *arg0;
    } else {
        distB = start - (-1.0f * *arg0);
    }
    if (distA <= distB) {
        ret = 0;
    }
    return ret;
}

MGTIMER *fn_1_1AEC(s32 type, s32 maxSec, s32 endSec, f32 x, f32 y)
{
    MGTIMER *timer;

    timer = MgTimerCreate(type);
    MgTimerParamSet(timer, maxSec * 60, endSec * 60, 0);
    MgTimerPosSet(timer, x, y);
    return timer;
}

void fn_1_1B60(MGTIMER *timer)
{
    MgTimerModeOnSet(timer, 1);
}

BOOL fn_1_1B8C(MGTIMER *timer)
{
    return MgTimerDoneCheck(timer);
}

void fn_1_1BB4(MGTIMER *timer)
{
    MgTimerDispOff(timer);
    MgTimerKill(timer);
}

s32 fn_1_1BEC(M549State *state, s16 pad, s16 idx)
{
    M549StageModel *model;

    model = &state->models[idx];
    if (HuPadStkX[pad] > 10) {
        model->x += 1.0f;
    }
    if (HuPadStkX[pad] < -10) {
        model->x -= 1.0f;
    }
    if (HuPadStkY[pad] > 10) {
        model->y += 1.0f;
    }
    if (HuPadStkY[pad] < -10) {
        model->y -= 1.0f;
    }
    if (HuPadTrigL[pad] > 10) {
        model->z += 1.0f;
    }
    if (HuPadTrigR[pad] > 10) {
        model->z -= 1.0f;
    }
    if (HuPadSubStkX[pad] > 10) {
        model->rotX += 1.0f;
    }
    if (HuPadSubStkX[pad] < -10) {
        model->rotX -= 1.0f;
    }
    if (HuPadSubStkY[pad] > 10) {
        model->rotY += 1.0f;
    }
    if (HuPadSubStkY[pad] < -10) {
        model->rotY -= 1.0f;
    }
    if ((HuPadBtnRep[pad] & 0x100) != 0) {
        model->rotZ += 1.0f;
    }
    if ((HuPadBtnRep[pad] & 0x200) != 0) {
        model->rotZ -= 1.0f;
    }
    print8(0x19, 0x88, 1.5f, lbl_1_data_378, model->x, model->y, model->z, model->rotX, model->rotY, model->rotZ);
    print8(0x19, 0x9A, 1.5f, lbl_1_data_3A6, idx);
    return 0;
}

void fn_1_1F40(void)
{
}

void fn_1_1F44(s16 pad)
{
    s32 btnDown;
    u16 btn;
    s8 absX;
    s8 stkX;
    s8 absY;
    s8 stkY;
    s8 absSX;
    s8 subX;
    s8 absSY;
    s8 subY;

    btn = HuPadBtn[pad];
    btnDown = HuPadBtnDown[pad];
    absX = HuPadStkX[pad];
    if (absX < 0) {
        absX = -absX;
    }
    if (absX < 10) {
        stkX = 0;
    } else {
        stkX = HuPadStkX[pad];
    }
    absY = HuPadStkY[pad];
    if (absY < 0) {
        absY = -absY;
    }
    if (absY < 10) {
        stkY = 0;
    } else {
        stkY = HuPadStkY[pad];
    }
    absSX = HuPadSubStkX[pad];
    if (absSX < 0) {
        absSX = -absSX;
    }
    if (absSX < 8) {
        subX = 0;
    } else {
        subX = HuPadSubStkX[pad];
    }
    absSY = HuPadSubStkY[pad];
    if (absSY < 0) {
        absSY = -absSY;
    }
    if (absSY < 8) {
        subY = 0;
    } else {
        subY = HuPadSubStkY[pad];
    }
    if ((btn & 0x400) != 0) {
        CZoom += 20.0f;
    }
    if ((btn & 0x800) != 0) {
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
    if ((btn & 0x2000) != 0) {
        CRot.z += 1.0f;
    }
    if ((btn & 0x4000) != 0) {
        CRot.z -= 1.0f;
    }
    if ((btn & 8) != 0) {
        Center.z += 10.0f;
    }
    if ((btn & 4) != 0) {
        Center.z -= 10.0f;
    }
    print8(0x19, 0x1E, 1.5f, lbl_1_data_3AE, Center.x, Center.y, Center.z, CZoom);
    print8(0x19, 0x30, 1.5f, lbl_1_data_3D3, CRot.x, CRot.y, CRot.z);
}

void fn_1_23FC(void)
{
    HU3DPARTICLE *particle;
    ANIMDATA *anim;
    HU3DPARTICLEDATA *data;
    s16 i;

    anim = HuSprAnimRead(HuDataSelHeapReadNum(0x6B0008, 0x10000000, 2));
    lbl_1_bss_15D8 = HuMemDirectMallocNum(HEAP_HEAP, 100 * sizeof(M549PtclWork), 0x10000000);
    lbl_1_bss_15DC = Hu3DParticleCreate(anim, 100);
    Hu3DModelLayerSet(lbl_1_bss_15DC, 5);
    Hu3DParticleHookSet(lbl_1_bss_15DC, fn_1_3144);
    Hu3DModelCameraSet(lbl_1_bss_15DC, 1);
    Hu3DParticleBlendModeSet(lbl_1_bss_15DC, 1);
    particle = (HU3DPARTICLE *)Hu3DData[lbl_1_bss_15DC].hookData;
    particle->emitCnt = 0;
    particle->work = lbl_1_bss_15D8;
    particle->count = 0;
    data = particle->data;
    for (i = 0; i < particle->maxCnt; i++, data++) {
        data->scale = 0.0f;
    }
}

void fn_1_2550(M549Vec3 *arg0)
{
    M549Vec3 sp;
    f32 speed;
    s16 t;
    s16 i;

    for (i = 0; i < 12; i++) {
        speed = 6.0f + frandmod(6);
        lbl_1_data_3F0.velX = speed * sin(3.141592653589793 * (30.0f * i) / 180.0);
        lbl_1_data_3F0.velY = 0.0f;
        lbl_1_data_3F0.velZ = speed * cos(3.141592653589793 * (30.0f * i) / 180.0);
        t = -frandmod(6) - 7;
        lbl_1_data_3F0.alphaStep = t;
        sp.x = arg0->x + 20.0 * sin(3.141592653589793 * (30.0f * i) / 180.0);
        sp.y = 10.0f + arg0->y;
        sp.z = arg0->z + 20.0 * cos(3.141592653589793 * (30.0f * i) / 180.0);
        lbl_1_data_3F0.accelY = 0.01f * frandmod(20);
        fn_1_2F70(&lbl_1_data_3F0, sp.x, sp.y, sp.z, 20.0f + frandmod(20));
    }
}

void fn_1_28CC(M549Vec3 *arg0, s16 mode)
{
    M549Vec3 sp;
    f32 speed;
    s16 t;
    s16 i;

    if (mode == 0) {
        for (i = 0; i < 48; i++) {
            speed = 5.0f + frandmod(5);
            lbl_1_data_3F0.velX = speed * sin(3.141592653589793 * (7.5f * i) / 180.0);
            lbl_1_data_3F0.velY = frandmod(2);
            lbl_1_data_3F0.velZ = frandmod(10) - 5.0f;
#ifdef TARGET_PC
            /* (f32)(u32)-n - 6.0f is ~4.29e9: fctiwz saturates to s16 -1 on GC, UB (0) on x64 */
            frandmod(6);
            t = -1;
#else
            t = (f32)-frandmod(6) - 6.0f;
#endif
            lbl_1_data_3F0.alphaStep = t;
            sp.x = arg0->x + 60.0 * sin(3.141592653589793 * (7.5f * i) / 180.0);
            sp.y = 5.0f + arg0->y;
            sp.z = arg0->z + frandmod(40) - 20.0f;
            lbl_1_data_3F0.accelY = 0.01f * frandmod(20);
            fn_1_2F70(&lbl_1_data_3F0, sp.x, sp.y, sp.z, 10.0f + frandmod(15));
        }
        return;
    }
    for (i = 0; i < 48; i++) {
        speed = 5.0f + frandmod(5);
        lbl_1_data_3F0.velX = frandmod(10) - 5.0f;
        lbl_1_data_3F0.velY = frandmod(2);
        lbl_1_data_3F0.velZ = speed * sin(3.141592653589793 * (7.5f * i) / 180.0);
#ifdef TARGET_PC
        /* (f32)(u32)-n - 6.0f is ~4.29e9: fctiwz saturates to s16 -1 on GC, UB (0) on x64 */
        frandmod(6);
        t = -1;
#else
        t = (f32)-frandmod(6) - 6.0f;
#endif
        lbl_1_data_3F0.alphaStep = t;
        sp.x = arg0->x + frandmod(40) - 20.0f;
        sp.y = 5.0f + arg0->y;
        sp.z = arg0->z + 60.0 * sin(3.141592653589793 * (7.5f * i) / 180.0);
        lbl_1_data_3F0.accelY = 0.01f * frandmod(20);
        fn_1_2F70(&lbl_1_data_3F0, sp.x, sp.y, sp.z, 10.0f + frandmod(15));
    }
}

s16 fn_1_2F70(M549PtclWork *arg0, f32 x, f32 y, f32 z, f32 scale)
{
    HU3DPARTICLEDATA *data;
    HU3DPARTICLE *particle;
    s16 i;
    s16 ret;
    M549PtclWork *work;

    ret = -1;
    particle = (HU3DPARTICLE *)Hu3DData[lbl_1_bss_15DC].hookData;
    work = (M549PtclWork *)particle->work;
    data = particle->data + particle->emitCnt;
    for (i = particle->emitCnt; i < particle->maxCnt; i++, data++) {
        if (data->scale == 0.0f) {
            break;
        }
    }
    if (i >= particle->maxCnt) {
        data = particle->data;
        for (i = 0; i < particle->maxCnt; i++, data++) {
            if (data->scale == 0.0f) {
                break;
            }
        }
    }
    if (i != particle->maxCnt) {
        work[i] = *arg0;
        data->pos.x = x;
        data->pos.y = y;
        data->pos.z = z;
        data->vel = *(HuVecF *)&arg0->velX;
        data->color.r = arg0->colStart[0];
        data->color.g = arg0->colStart[1];
        data->color.b = arg0->colStart[2];
        data->color.a = arg0->colStart[3];
        data->scaleBase = scale;
        data->scale = scale;
        data->parManId = 0x3C;
        particle->emitCnt = i;
        ret = i;
    }
    return ret;
}

void fn_1_3144(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx)
{
    M549PtclWork *work;
    HU3DPARTICLEDATA *data;
    s16 i;
    s16 v;
    s16 cnt;

    work = (M549PtclWork *)particleP->work;
    data = particleP->data;
    cnt = 0;
    i = 0;
    data->time = 0;
    for (; i < particleP->maxCnt; i++, data++) {
        if (data->scale > 0.0f) {
            cnt++;
            data->vel.x = data->vel.x * work[i].mulX;
            data->vel.y = data->vel.y * work[i].mulY;
            data->vel.z = data->vel.z * work[i].mulZ;
            PSVECAdd((Vec *)&data->vel, (Vec *)&data->pos, (Vec *)&data->pos);
            data->vel.y = data->vel.y + work[i].accelY;
            v = (f32)data->color.r + work[i].colorStep * (f32)(work[i].colEnd[0] - work[i].colStart[0]);
            if (v < 0) {
                v = 0;
            } else if (v > 0xFF) {
                v = 0xFF;
            }
            data->color.r = v;
            v = (f32)data->color.g + work[i].colorStep * (f32)(work[i].colEnd[1] - work[i].colStart[1]);
            if (v < 0) {
                v = 0;
            } else if (v > 0xFF) {
                v = 0xFF;
            }
            data->color.g = v;
            v = (f32)data->color.b + work[i].colorStep * (f32)(work[i].colEnd[2] - work[i].colStart[2]);
            if (v < 0) {
                v = 0;
            } else if (v > 0xFF) {
                v = 0xFF;
            }
            data->color.b = v;
            v = (f32)data->color.a + work[i].alphaStep;
            if (v < 10) {
                data->scale = 0.0f;
            }
            data->color.a = v;
            if (0.0f != data->scale) {
                data->scale = data->scaleBase;
                data->scaleBase = data->scaleBase + work[i].scaleStep;
                if (data->scaleBase <= 0.01f) {
                    data->scale = 0.0f;
                }
            }
            data->time += 1;
            data->parManId -= 1;
            if (data->parManId == 0) {
                data->scale = 0.0f;
            }
        }
    }
}

void fn_1_357C(void)
{
    lbl_1_bss_580.jingleHandle = HuAudJinglePlay(0x83);
}

void fn_1_35AC(void)
{
    lbl_1_bss_580.bgmHandle = HuAudBGMPlay(0x37);
}

void fn_1_35DC(s16 isBgm, s16 speed)
{
    s16 stream;

    if (isBgm != 0) {
        stream = lbl_1_bss_580.bgmHandle;
    } else {
        stream = lbl_1_bss_580.jingleHandle;
    }
    HuAudSStreamFadeOut(stream, speed);
}

void fn_1_3644(s16 isBgm)
{
    s16 stream;

    if (isBgm != 0) {
        stream = lbl_1_bss_580.bgmHandle;
    } else {
        stream = lbl_1_bss_580.jingleHandle;
    }
    HuAudSStreamStop(stream);
}

void fn_1_36A4(s16 id)
{
    if ((id != 0 && id != 1) || lbl_1_bss_580.sfxFlag[id] == -1) {
        lbl_1_bss_580.sfxFlag[id] = 1;
        lbl_1_bss_580.sfxHandle[id] = HuAudFXPlay(lbl_1_data_428[id]);
    }
}

void fn_1_3754(s16 id, s16 vol, s16 pan)
{
    if ((id != 0 && id != 1) || lbl_1_bss_580.sfxFlag[id] == -1) {
        lbl_1_bss_580.sfxFlag[id] = 1;
        lbl_1_bss_580.sfxHandle[id] = HuAudFXPlayVolPan(lbl_1_data_428[id], vol, pan);
    }
}

void fn_1_3814(s16 id, s16 pan)
{
    if (lbl_1_bss_580.sfxFlag[id] == 1 && HuAudFXStatusGet(lbl_1_bss_580.sfxHandle[id]) == 2) {
        HuAudFXPanning(lbl_1_bss_580.sfxHandle[id], pan);
    }
}

void fn_1_38A4(s16 id, s16 pitch)
{
    if (lbl_1_bss_580.sfxFlag[id] == 1 && HuAudFXStatusGet(lbl_1_bss_580.sfxHandle[id]) == 2) {
        HuAudFXPitchSet(lbl_1_bss_580.sfxHandle[id], pitch);
    }
}

void fn_1_3934(s16 id, s16 vol)
{
    if (lbl_1_bss_580.sfxFlag[id] == 1 && HuAudFXStatusGet(lbl_1_bss_580.sfxHandle[id]) == 2) {
        HuAudFXVolSet(lbl_1_bss_580.sfxHandle[id], vol);
    }
}

void fn_1_39C4(s16 id)
{
    if (lbl_1_bss_580.sfxFlag[id] == 1
        && (HuAudFXStatusGet(lbl_1_bss_580.sfxHandle[id]) == 2 || HuAudFXStatusGet(lbl_1_bss_580.sfxHandle[id]) == 3)) {
        lbl_1_bss_580.sfxFlag[id] = -1;
        HuAudFXStop(lbl_1_bss_580.sfxHandle[id]);
    }
}

void fn_1_3A8C(s16 id)
{
    if (lbl_1_bss_580.sfxFlag[id] == 1
        && (HuAudFXStatusGet(lbl_1_bss_580.sfxHandle[id]) == 2 || HuAudFXStatusGet(lbl_1_bss_580.sfxHandle[id]) == 3)) {
        lbl_1_bss_580.sfxFlag[id] = -1;
        HuAudFXStop(lbl_1_bss_580.sfxHandle[id]);
    }
}

void fn_1_3B54(void)
{
    HuAudAllStop();
}
