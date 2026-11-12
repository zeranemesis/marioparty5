#include "types.h"
#include "string.h"
#include "game/audio.h"
#include "game/data.h"
#include "game/frand.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/printfunc.h"
#include "game/sprite.h"
#include "game/mg/seqman.h"
#include "game/mg/timer.h"
#include "REL/m544Dll.h"

double sin(double x);
double cos(double x);
void DCStoreRangeNoSync(void *addr, u32 nBytes);

typedef void (*M544VoidFunc)(void);
extern const M544VoidFunc _ctors[];
extern const M544VoidFunc _dtors[];

/* game.c */
s32 fn_1_4328(M544State *state);
s32 fn_1_4E08(void);
s32 fn_1_4E3C(void);
s32 fn_1_4EB4(void);
s32 fn_1_5098(void);
s16 fn_1_511C(void);
s32 fn_1_5158(void);
s32 fn_1_5514(void);
s32 fn_1_5544(void);
void fn_1_89AC(OMOBJ *obj);
void fn_1_86BC(void);
extern M544State *lbl_1_bss_914;
extern M544PtclWork *lbl_1_bss_820;
extern s16 lbl_1_bss_824;
extern s16 lbl_1_bss_826;

/* stage.c */
void fn_1_9EF8(M544State *state, M544Mdl *models, s32 *dataNums);
void fn_1_A144(s16 arg0);
void fn_1_A3F0(s16 charNo, s32 *motData);
void fn_1_A410(s16 charNo, s16 *motId);
void fn_1_A430(void);
void fn_1_A9A4(void);
void fn_1_ADBC(void);
void fn_1_AF60(void);
void fn_1_B318(void);

void ObjectSetup(void);
void fn_1_37C(void);
void fn_1_3B0(void);
void fn_1_3DC(void);
void fn_1_3FC(void);
void fn_1_428(void);
void fn_1_448(void);
void fn_1_474(void);
void fn_1_4A0(void);
void fn_1_4C0(void);
void fn_1_4EC(void);
void fn_1_4F0(s16 arg0);
void fn_1_9C0(s16 arg0);
void fn_1_F38(void);
void fn_1_1168(void);
void fn_1_1440(void);
void fn_1_1570(void);
s16 fn_1_2EE4(M544PtclWork *arg0, f32 x, f32 y, f32 z, f32 scale);
void fn_1_30B8(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);
void fn_1_39E8(HuVecF *arg0, HuVecF *arg1, f32 scale);
void fn_1_3AD4(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);

/* unit-owned .bss, declared in reverse address order */
OMOBJMAN *lbl_1_bss_818;
M544State lbl_1_bss_530;
M544Mdl lbl_1_bss_15C[35];
s16 lbl_1_bss_130[3][7];
M544Game lbl_1_bss_D0;
M544Obj lbl_1_bss_80[2];
M544Stage lbl_1_bss_C;
M544Cam *lbl_1_bss_8;
M544Light *lbl_1_bss_4;
M544Shadow *lbl_1_bss_0;

/* unit-owned .data, in original address order */
MGSEQ_PARAM lbl_1_data_0 = {
    0, 0,
    (MGSEQ_FUNC)fn_1_37C, (MGSEQ_FUNC)fn_1_3B0, (MGSEQ_FUNC)fn_1_3DC,
    (MGSEQ_FUNC)fn_1_3FC, (MGSEQ_FUNC)fn_1_428, (MGSEQ_FUNC)fn_1_448,
    (MGSEQ_FUNC)fn_1_474, (MGSEQ_FUNC)fn_1_4A0, (MGSEQ_FUNC)fn_1_4C0,
};
s32 lbl_1_data_28[13][7] = {
    { 0x8C0000, 0x8C001A, 0x870026, 0x870027, 0x870028, 0x8C0006, 0x8C0007 },
    { 0x330000, 0x33001A, 0x2E0026, 0x2E0027, 0x2E0028, 0x330006, 0x330007 },
    { 0xB70000, 0xB7001A, 0xB20026, 0xB20027, 0xB20028, 0xB70006, 0xB70007 },
    { 0xED0000, 0xED001A, 0xE80026, 0xE80027, 0xE80028, 0xED0006, 0xED0007 },
    { 0xE60000, 0xE6001A, 0xE10026, 0xE10027, 0xE10028, 0xE60006, 0xE60007 },
    { 0x170000, 0x17001A, 0x120026, 0x120027, 0x120028, 0x170006, 0x170007 },
    { 0xE00000, 0xE0001A, 0xDB0026, 0xDB0027, 0xDB0028, 0xE00006, 0xE00007 },
    { 0x2D0000, 0x2D001A, 0x280026, 0x280027, 0x280028, 0x2D0006, 0x2D0007 },
    { 0xC90000, 0xC9001A, 0xC40026, 0xC40027, 0xC40028, 0xC90006, 0xC90007 },
    { 0xB00000, 0xB0001A, 0x9C0026, 0x9C0027, 0x9C0028, 0xB00006, 0xB00007 },
    { 0xB00000, 0xB0001A, 0x9C0026, 0x9C0027, 0x9C0028, 0xB00006, 0xB00007 },
    { 0xB00000, 0xB0001A, 0x9C0026, 0x9C0027, 0x9C0028, 0xB00006, 0xB00007 },
    { 0xB00000, 0xB0001A, 0x9C0026, 0x9C0027, 0x9C0028, 0xB00006, 0xB00007 },
};
s32 lbl_1_data_194[4] = { 0x66001A, 0x66001B, 0x66001C, 0x66001D };
s32 lbl_1_data_1A4[27] = {
    0x660000, 0x660001, 0x660002, 0x660003, 0x660004, 0x660005, 0x660006, 0x660007, 0x660008,
    0x660009, 0x66000A, 0x66000B, 0x66000C, 0x66000D, 0x66000E, 0x66000F, 0x660010, 0x660011,
    0x660012, 0x660013, 0x660014, 0x660015, 0x660016, 0x660017, 0x660018, 0x660019, 0,
};
M544CamInit lbl_1_data_210[4] = {
    { { -12.0f, 208.0f, -120.0f }, -11.0f, 0.0f, 500.0f, 141.0f, 1.33f, 0.0f, 0.0f, 640.0f, 480.0f, 45.0f },
    { { -220.0f, 218.0f, 0.0f }, -11.0f, -29.0f, 0.0f, 275.0f, 1.33f, 0.0f, 0.0f, 640.0f, 480.0f, 45.0f },
    { { 0.0f, 238.0f, 0.0f }, -6.0f, 0.0f, 0.0f, 695.0f, 1.33f, 0.0f, 0.0f, 640.0f, 480.0f, 45.0f },
    { { 0.0f, 0.0f, 0.0f }, 0.0f, 0.0f, 500.0f, 500.0f, 1.33f, 0.0f, 0.0f, 640.0f, 480.0f, 45.0f },
};
s16 lbl_1_data_2E0[16] = {
    0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000, 0x8000,
};
M544PtclWork lbl_1_data_370 = {
    0, 0,
    { 0xFF, 0xFF, 0xFF, 0xFF }, { 0x00, 0x00, 0x00, 0xFF },
    0.0f, 2.0f, 1.0f,
    0.85f, 0.85f, 0.85f,
    0.2f,
    0.0f, 1.0f, -3.0f, 0.02f,
};
s16 lbl_1_data_3A8[8] = { 0x81D, 0x81E, 0x81F, 0x820, 0x821, 0x2C4, 0, 0 };

int _prolog(void)
{
    const M544VoidFunc *ctors = _ctors;

    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const M544VoidFunc *dtors = _dtors;

    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

void ObjectSetup(void)
{
    s16 i;

    lbl_1_bss_818 = omInitObjMan(0x32, 0x2000);
    omGameSysInit(lbl_1_bss_818);
    fn_1_9EF8(&lbl_1_bss_530, lbl_1_bss_15C, lbl_1_data_1A4);
    lbl_1_bss_8 = HuMemDirectMallocNum(HEAP_HEAP, 0x148, 0x10000000);
    memset(lbl_1_bss_8, 0, 0x148);
    lbl_1_bss_4 = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M544Light), 0x10000000);
    memset(lbl_1_bss_4, 0, sizeof(M544Light));
    lbl_1_bss_0 = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M544Shadow), 0x10000000);
    memset(lbl_1_bss_0, 0, sizeof(M544Shadow));
    fn_1_4F0(0);
    fn_1_F38();
    fn_1_1440();
    fn_1_A144(1);
    for (i = 0; i < 2; i++) {
        fn_1_A3F0(i, lbl_1_data_28[lbl_1_bss_530.chars[i].charNo]);
        fn_1_A410(i, lbl_1_bss_130[i]);
    }
    fn_1_A3F0(2, lbl_1_data_194);
    fn_1_A410(2, lbl_1_bss_130[2]);
    fn_1_A430();
    fn_1_ADBC();
    omAddObjEx(lbl_1_bss_818, 0x7FDA, 0, 0, -1, (OMOBJFUNC)fn_1_A9A4);
    omAddObjEx(lbl_1_bss_818, 0x7FDA, 0, 0, -1, (OMOBJFUNC)fn_1_AF60);
    omAddObjEx(lbl_1_bss_818, 0x7FDA, 0, 0, -1, (OMOBJFUNC)fn_1_4EC);
    fn_1_86BC();
    omAddObjEx(lbl_1_bss_818, 0x7FDA, 0, 0, -1, (OMOBJFUNC)fn_1_89AC);
    omAddObjEx(lbl_1_bss_818, 0x7FDA, 0, 0, -1, (OMOBJFUNC)fn_1_B318);
    Hu3DBGColorSet(0xBA, 0xF5, 0xFF);
    MgSeqCreate(&lbl_1_data_0);
    MgSeqModeDelaySet(3, 1);
    MgSeqModeDelaySet(2, 1);
    MgSeqModeDelaySet(2, 0x12C);
    MgSeqModeDelaySet(2, 0x140);
    MgSeqModeDelaySet(7, 0xB4);
    MgSeqModeDelaySet(9, 0x3C);
}

void fn_1_37C(void)
{
    if (fn_1_4328(&lbl_1_bss_530) != 0) {
        MgSeqModeNext();
    }
}

void fn_1_3B0(void)
{
    if (fn_1_4E08() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_3DC(void)
{
    fn_1_4E3C();
}

void fn_1_3FC(void)
{
    if (fn_1_4EB4() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_428(void)
{
    fn_1_5098();
}

void fn_1_448(void)
{
    if (fn_1_511C() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_474(void)
{
    if (fn_1_5158() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_4A0(void)
{
    fn_1_5514();
}

void fn_1_4C0(void)
{
    if (fn_1_5544() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_4EC(void)
{
}

void fn_1_4F0(s16 arg0)
{
    lbl_1_bss_8[arg0].idx = arg0;
    lbl_1_bss_8[arg0].center.x = lbl_1_data_210[arg0].center.x;
    lbl_1_bss_8[arg0].center.y = lbl_1_data_210[arg0].center.y;
    lbl_1_bss_8[arg0].center.z = lbl_1_data_210[arg0].center.z;
    lbl_1_bss_8[arg0].rotX = lbl_1_data_210[arg0].rotX;
    lbl_1_bss_8[arg0].rotY = lbl_1_data_210[arg0].rotY;
    lbl_1_bss_8[arg0].rotZ = lbl_1_data_210[arg0].rotZ;
    lbl_1_bss_8[arg0].zoom = lbl_1_data_210[arg0].zoom;
    lbl_1_bss_8[arg0].aspect = lbl_1_data_210[arg0].aspect;
    lbl_1_bss_8[arg0].vpX = lbl_1_data_210[arg0].vpX;
    lbl_1_bss_8[arg0].vpY = lbl_1_data_210[arg0].vpY;
    lbl_1_bss_8[arg0].vpW = lbl_1_data_210[arg0].vpW;
    lbl_1_bss_8[arg0].vpH = lbl_1_data_210[arg0].vpH;
    lbl_1_bss_8[arg0].fov = lbl_1_data_210[arg0].fov;
    Hu3DCameraCreate(lbl_1_data_2E0[arg0]);
    Hu3DCameraViewportSet(lbl_1_data_2E0[arg0], lbl_1_bss_8[arg0].vpX, lbl_1_bss_8[arg0].vpY,
                          lbl_1_bss_8[arg0].vpW, lbl_1_bss_8[arg0].vpH, 0.0f, 1.0f);
    Hu3DCameraPerspectiveSet(lbl_1_data_2E0[arg0], lbl_1_bss_8[arg0].fov, 20.0f, 30000.0f, lbl_1_bss_8[arg0].aspect);
    fn_1_9C0(arg0);
    Center.x = lbl_1_data_210[0].center.x;
    Center.y = lbl_1_data_210[0].center.y;
    Center.z = lbl_1_data_210[0].center.z;
    CRot.x = lbl_1_data_210[0].rotX;
    CRot.y = lbl_1_data_210[0].rotY;
    CRot.z = lbl_1_data_210[0].rotZ;
    CZoom = lbl_1_data_210[arg0].zoom;
}

void fn_1_9C0(s16 arg0)
{
    f32 sp8;
    f64 temp;

    temp = sin(3.141592653589793 * lbl_1_bss_8[arg0].rotY / 180.0);
    lbl_1_bss_8[arg0].pos.x = lbl_1_bss_8[arg0].center.x
        + lbl_1_bss_8[arg0].zoom * (cos(3.141592653589793 * lbl_1_bss_8[arg0].rotX / 180.0) * temp);
    lbl_1_bss_8[arg0].pos.y = lbl_1_bss_8[arg0].center.y
        + lbl_1_bss_8[arg0].zoom * -sin(3.141592653589793 * lbl_1_bss_8[arg0].rotX / 180.0);
    temp = cos(3.141592653589793 * lbl_1_bss_8[arg0].rotY / 180.0);
    lbl_1_bss_8[arg0].pos.z = lbl_1_bss_8[arg0].center.z
        + lbl_1_bss_8[arg0].zoom * (cos(3.141592653589793 * lbl_1_bss_8[arg0].rotX / 180.0) * temp);
    temp = sin(3.141592653589793 * lbl_1_bss_8[arg0].rotY / 180.0);
    lbl_1_bss_8[arg0].up.x = sin(3.141592653589793 * lbl_1_bss_8[arg0].rotX / 180.0) * temp;
    lbl_1_bss_8[arg0].up.y = cos(3.141592653589793 * lbl_1_bss_8[arg0].rotX / 180.0);
    temp = cos(3.141592653589793 * lbl_1_bss_8[arg0].rotY / 180.0);
    lbl_1_bss_8[arg0].up.z = sin(3.141592653589793 * lbl_1_bss_8[arg0].rotX / 180.0) * temp;
    sp8 = lbl_1_bss_8[arg0].center.z;
    Hu3DCameraPosSet(lbl_1_data_2E0[arg0], lbl_1_bss_8[arg0].pos.x, lbl_1_bss_8[arg0].pos.y, lbl_1_bss_8[arg0].pos.z,
                     lbl_1_bss_8[arg0].up.x, lbl_1_bss_8[arg0].up.y, lbl_1_bss_8[arg0].up.z,
                     lbl_1_bss_8[arg0].center.x, lbl_1_bss_8[arg0].center.y, sp8);
}

void fn_1_F38(void)
{
    Hu3DLighInit();
    lbl_1_bss_4->pos.x = 0.0f;
    lbl_1_bss_4->pos.y = 0.0f;
    lbl_1_bss_4->pos.z = 0.0f;
    lbl_1_bss_4->target.x = 0.0f;
    lbl_1_bss_4->target.y = 0.0f;
    lbl_1_bss_4->target.z = 0.0f;
    lbl_1_bss_4->rotX = -23.0f;
    lbl_1_bss_4->rotY = 0.0f;
    lbl_1_bss_4->rotZ = 0.0f;
    lbl_1_bss_4->dist = 1000.0f;
    lbl_1_bss_4->color.r = 0xFF;
    lbl_1_bss_4->color.g = 0xFF;
    lbl_1_bss_4->color.b = 0xFF;
    lbl_1_bss_4->color.a = 0xFF;
    PSVECSubtract((Vec *)&lbl_1_bss_4->target, (Vec *)&lbl_1_bss_4->pos, (Vec *)&lbl_1_bss_4->dir);
    lbl_1_bss_4->id = Hu3DGLightCreateV((HuVecF *)&lbl_1_bss_4->pos, (HuVecF *)&lbl_1_bss_4->dir, &lbl_1_bss_4->color);
    fn_1_1168();
    Hu3DGLightStaticSet(lbl_1_bss_4->id, 1);
    Hu3DGLightInfinitytSet(lbl_1_bss_4->id);
}

void fn_1_1168(void)
{
    f64 temp;

    temp = sin(3.141592653589793 * lbl_1_bss_4->rotY / 180.0);
    lbl_1_bss_4->pos.x = lbl_1_bss_4->target.x
        + lbl_1_bss_4->dist * (cos(3.141592653589793 * lbl_1_bss_4->rotX / 180.0) * temp);
    lbl_1_bss_4->pos.y = lbl_1_bss_4->target.y
        + lbl_1_bss_4->dist * -sin(3.141592653589793 * lbl_1_bss_4->rotX / 180.0);
    temp = cos(3.141592653589793 * lbl_1_bss_4->rotY / 180.0);
    lbl_1_bss_4->pos.z = lbl_1_bss_4->target.z
        + lbl_1_bss_4->dist * (cos(3.141592653589793 * lbl_1_bss_4->rotX / 180.0) * temp);
    PSVECSubtract((Vec *)&lbl_1_bss_4->target, (Vec *)&lbl_1_bss_4->pos, (Vec *)&lbl_1_bss_4->dir);
    PSVECNormalize((Vec *)&lbl_1_bss_4->dir, (Vec *)&lbl_1_bss_4->dir);
    Hu3DGLightPosSetV(lbl_1_bss_4->id, (HuVecF *)&lbl_1_bss_4->pos, (HuVecF *)&lbl_1_bss_4->dir);
    Hu3DGLightColorSet(lbl_1_bss_4->id, lbl_1_bss_4->color.r, lbl_1_bss_4->color.g, lbl_1_bss_4->color.b,
                       lbl_1_bss_4->color.a);
}

void fn_1_1440(void)
{
    Hu3DShadowCreate(30.0f, 20.0f, 30000.0f);
    lbl_1_bss_0->rotX = -90.0f;
    lbl_1_bss_0->rotY = 0.0f;
    lbl_1_bss_0->rotZ = 0.0f;
    lbl_1_bss_0->target.x = 0.0f;
    lbl_1_bss_0->target.y = 0.0f;
    lbl_1_bss_0->target.z = -250.0f;
    lbl_1_bss_0->tpLvl = 0.25f;
    lbl_1_bss_0->dist = 1500.0f;
    Hu3DShadowSizeSet(200);
    fn_1_1570();
}

void fn_1_1570(void)
{
    f64 temp;

    temp = sin(3.141592653589793 * lbl_1_bss_0->rotY / 180.0);
    lbl_1_bss_0->pos.x = lbl_1_bss_0->target.x
        + lbl_1_bss_0->dist * (cos(3.141592653589793 * lbl_1_bss_0->rotX / 180.0) * temp);
    lbl_1_bss_0->pos.y = lbl_1_bss_0->target.y
        + lbl_1_bss_0->dist * -sin(3.141592653589793 * lbl_1_bss_0->rotX / 180.0);
    temp = cos(3.141592653589793 * lbl_1_bss_0->rotY / 180.0);
    lbl_1_bss_0->pos.z = lbl_1_bss_0->dist * (cos(3.141592653589793 * lbl_1_bss_0->rotX / 180.0) * temp);
    temp = sin(3.141592653589793 * lbl_1_bss_0->rotY / 180.0);
    lbl_1_bss_0->up.x = sin(3.141592653589793 * lbl_1_bss_0->rotX / 180.0) * temp;
    lbl_1_bss_0->up.y = cos(3.141592653589793 * lbl_1_bss_0->rotX / 180.0);
    temp = cos(3.141592653589793 * lbl_1_bss_0->rotY / 180.0);
    lbl_1_bss_0->up.z = sin(3.141592653589793 * lbl_1_bss_0->rotX / 180.0) * temp;
    Hu3DShadowTPLvlSet(lbl_1_bss_0->tpLvl);
    Hu3DShadowPosSet((HuVecF *)&lbl_1_bss_0->pos, (HuVecF *)&lbl_1_bss_0->up, (HuVecF *)&lbl_1_bss_0->target);
}

s16 fn_1_18E8(f32 *arg0, f32 farg0, f32 farg1, f32 farg2)
{
    f32 step;
    f32 d1;
    f32 d2;
    s16 ret;

    step = (farg2 - farg1) / farg0;
    *arg0 += step;
    if (step != 0.0f) {
        ret = (s16)((farg2 - *arg0) / step);
    } else {
        ret = 0;
    }
    if (ret == 0) {
        ret = 1;
    }
    if ((farg1 - farg2) > 0.0f) {
        d1 = farg1 - farg2;
    } else {
        d1 = farg1 - (-1.0f * farg2);
    }
    if ((farg1 - *arg0) > 0.0f) {
        d2 = farg1 - *arg0;
    } else {
        d2 = farg1 - (-1.0f * *arg0);
    }
    if (d1 <= d2) {
        *arg0 = farg2;
        ret = 0;
    }
    return ret;
}

MGTIMER *fn_1_1A1C(s32 type, s32 maxSec, s32 endSec, f32 x, f32 y)
{
    MGTIMER *timer;

    timer = MgTimerCreate(type);
    MgTimerParamSet(timer, maxSec * 0x3C, endSec * 0x3C, 0);
    MgTimerPosSet(timer, x, y);
    return timer;
}

void fn_1_1A90(MGTIMER *timer)
{
    MgTimerModeOnSet(timer, 1);
}

void fn_1_1ABC(MGTIMER *timer)
{
    MgTimerDoneCheck(timer);
}

void fn_1_1AE4(MGTIMER *timer)
{
    MgTimerDispOff(timer);
    MgTimerKill(timer);
}

s32 fn_1_1B1C(M544State *state, s16 pad, s16 idx)
{
    M544Mdl *mdl;

    mdl = &state->models[idx];
    if (HuPadStkX[pad] > 0xA) {
        mdl->pos.x += 1.0f;
    }
    if (HuPadStkX[pad] < -0xA) {
        mdl->pos.x -= 1.0f;
    }
    if (HuPadStkY[pad] > 0xA) {
        mdl->pos.y += 1.0f;
    }
    if (HuPadStkY[pad] < -0xA) {
        mdl->pos.y -= 1.0f;
    }
    if (HuPadTrigL[pad] > 0xA) {
        mdl->pos.z += 1.0f;
    }
    if (HuPadTrigR[pad] > 0xA) {
        mdl->pos.z -= 1.0f;
    }
    if (HuPadSubStkX[pad] > 0xA) {
        mdl->rot.x += 1.0f;
    }
    if (HuPadSubStkX[pad] < -0xA) {
        mdl->rot.x -= 1.0f;
    }
    if (HuPadSubStkY[pad] > 0xA) {
        mdl->rot.y += 1.0f;
    }
    if (HuPadSubStkY[pad] < -0xA) {
        mdl->rot.y -= 1.0f;
    }
    if ((HuPadBtnRep[pad] & 0x100) != 0) {
        mdl->rot.z += 1.0f;
    }
    if ((HuPadBtnRep[pad] & 0x200) != 0) {
        mdl->rot.z -= 1.0f;
    }
    fontcolor = 0;
    print8(0x19, 0x88, 1.5f, "OBJ X:%.1f Y:%.1f Z:%.1f RX%.1f RY%.1f RZ%.1f", mdl->pos.x, mdl->pos.y, mdl->pos.z,
           mdl->rot.x, mdl->rot.y, mdl->rot.z);
    return 0;
}

void fn_1_1E68(s16 arg0)
{
    s32 down;
    s8 subY;
    s8 absSubY;
    s8 subX;
    s8 absSubX;
    s8 stkY;
    s8 absY;
    s8 stkX;
    s8 absX;
    u16 btn;

    btn = HuPadBtn[arg0];
    down = HuPadBtnDown[arg0];
    if (HuPadStkX[arg0] >= 0) {
        absX = HuPadStkX[arg0];
    } else {
        absX = -HuPadStkX[arg0];
    }
    if (absX < 0xA) {
        stkX = 0;
    } else {
        stkX = HuPadStkX[arg0];
    }
    if (HuPadStkY[arg0] >= 0) {
        absY = HuPadStkY[arg0];
    } else {
        absY = -HuPadStkY[arg0];
    }
    if (absY < 0xA) {
        stkY = 0;
    } else {
        stkY = HuPadStkY[arg0];
    }
    if (HuPadSubStkX[arg0] >= 0) {
        absSubX = HuPadSubStkX[arg0];
    } else {
        absSubX = -HuPadSubStkX[arg0];
    }
    if (absSubX < 8) {
        subX = 0;
    } else {
        subX = HuPadSubStkX[arg0];
    }
    if (HuPadSubStkY[arg0] >= 0) {
        absSubY = HuPadSubStkY[arg0];
    } else {
        absSubY = -HuPadSubStkY[arg0];
    }
    if (absSubY < 8) {
        subY = 0;
    } else {
        subY = HuPadSubStkY[arg0];
    }
    if ((btn & 0x400) != 0) {
        CZoom += 20.0f;
    }
    if ((btn & 0x800) != 0) {
        CZoom -= 20.0f;
    }
    if (stkX > 0) {
        Center.x = Center.x + 10.0f;
    }
    if (stkX < 0) {
        Center.x = Center.x - 10.0f;
    }
    if (stkY > 0) {
        Center.y = Center.y + 10.0f;
    }
    if (stkY < 0) {
        Center.y = Center.y - 10.0f;
    }
    if (subX > 0) {
        CRot.y = CRot.y + 1.0f;
    }
    if (subX < 0) {
        CRot.y = CRot.y - 1.0f;
    }
    if (subY > 0) {
        CRot.x += 1.0f;
    }
    if (subY < 0) {
        CRot.x -= 1.0f;
    }
    if ((btn & 0x2000) != 0) {
        CRot.z = CRot.z + 1.0f;
    }
    if ((btn & 0x4000) != 0) {
        CRot.z = CRot.z - 1.0f;
    }
    if ((btn & 8) != 0) {
        Center.z = Center.z + 10.0f;
    }
    if ((btn & 4) != 0) {
        Center.z = Center.z - 10.0f;
    }
    fontcolor = 0;
    print8(0x19, 0x1E, 1.5f, "CAMPOS X:%.1f Y:%.1f Z:%.1f ZOOM%.1f", Center.x, Center.y, Center.z, CZoom);
    print8(0x19, 0x30, 1.5f, "CAMROT X:%.1f Y:%.1f Z:%.1f", CRot.x, CRot.y, CRot.z);
}

void fn_1_2330(void)
{
    HU3DPARTICLE *particle;
    HU3DPARTICLEDATA *data;
    s16 j;
    ANIMDATA *anim;

    anim = HuSprAnimRead(HuDataSelHeapReadNum(0x66001F, 0x10000000, 2));
    lbl_1_bss_820 = HuMemDirectMallocNum(HEAP_HEAP, 0xAF0, 0x10000000);
    lbl_1_bss_826 = Hu3DParticleCreate(anim, 0x32);
    Hu3DParticleHookSet(lbl_1_bss_826, fn_1_30B8);
    Hu3DModelLayerSet(lbl_1_bss_826, 3);
    Hu3DModelCameraSet(lbl_1_bss_826, 1);
    Hu3DModelAttrSet(lbl_1_bss_826, 1);
    Hu3DModelAttrReset(lbl_1_bss_826, 1);
    Hu3DParticleBlendModeSet(lbl_1_bss_826, 1);
    particle = (HU3DPARTICLE *)Hu3DData[lbl_1_bss_826].hookData;
    particle->emitCnt = 0;
    particle->work = lbl_1_bss_820;
    particle->count = 0;
    data = particle->data;
    for (j = 0; j < particle->maxCnt; j++, data++) {
        data->scale = 0.0f;
    }
}

void fn_1_24AC(HuVecF *arg0)
{
    HuVecF sp;
    f32 speed;
    s16 i;

    for (i = 0; i < 0xC; i++) {
        speed = 6.0 + frandmod(6);
        lbl_1_data_370.velX = speed * sin(3.141592653589793 * (30.0f * i) / 180.0);
        lbl_1_data_370.velY = 0.0f;
        lbl_1_data_370.velZ = speed * cos(3.141592653589793 * (30.0f * i) / 180.0);
        lbl_1_data_370.alphaStep = (s16)(-frandmod(6) - 7);
        sp.x = arg0->x + 20.0 * sin(3.141592653589793 * (30.0f * i) / 180.0);
        sp.y = 10.0f + arg0->y;
        sp.z = arg0->z + 20.0 * cos(3.141592653589793 * (30.0f * i) / 180.0);
        lbl_1_data_370.accelY = 0.01 * frandmod(0x14);
        fn_1_2EE4(&lbl_1_data_370, sp.x, sp.y, sp.z, 20.0 + frandmod(0x14));
    }
}

void fn_1_2834(HuVecF *arg0, s16 arg1)
{
    HuVecF sp;
    s16 sp8;
    f32 rnd;
    s16 i;

    sp8 = arg1;
    if (sp8 == 0) {
        arg0->y += 50.0f;
        for (i = 0; i < 0x30; i++) {
            rnd = frandmod(5);
            lbl_1_data_370.velX = (f32)frandmod(0xA) - 5.0f;
            lbl_1_data_370.velY = (5.0f + rnd) * sin(3.141592653589793 * (7.5f * i) / 180.0);
            lbl_1_data_370.velZ = frandmod(2);
            lbl_1_data_370.alphaStep = (s16)(-frandmod(6) - 5);
            sp.x = (arg0->x + (f32)frandmod(0x1E)) - 15.0f;
            sp.y = arg0->y + 40.0 * sin(3.141592653589793 * (7.5f * i) / 180.0);
            sp.z = 20.0f + arg0->z;
            lbl_1_data_370.accelY = 0.01f * (f32)frandmod(0x14);
            fn_1_2EE4(&lbl_1_data_370, sp.x, sp.y, sp.z, 20.0 + frandmod(0x14));
        }
        return;
    }
    arg0->x -= 20.0f;
    arg0->z += 50.0f;
    for (i = 0; i < 0x24; i++) {
        rnd = frandmod(5);
        lbl_1_data_370.velX = (f32)frandmod(4) - 2.0f;
        lbl_1_data_370.velY = (f32)frandmod(6) - 3.0f;
        lbl_1_data_370.velZ = (5.0f + rnd) * sin(3.141592653589793 * (10.0f * i) / 180.0);
        lbl_1_data_370.alphaStep = (s16)(-frandmod(6) - 5);
        rnd = frandmod(3);
        sp.x = arg0->x + rnd;
        sp.y = (arg0->y + (f32)frandmod(0x1E)) - 15.0f;
        sp.z = arg0->z + 40.0 * sin(3.141592653589793 * (10.0f * i) / 180.0);
        lbl_1_data_370.accelY = 0.01f * (f32)frandmod(0x14);
        fn_1_2EE4(&lbl_1_data_370, sp.x, sp.y, sp.z, 20.0 + frandmod(0x14));
    }
}

s16 fn_1_2EE4(M544PtclWork *arg0, f32 x, f32 y, f32 z, f32 scale)
{
    HU3DPARTICLEDATA *data;
    HU3DPARTICLE *particle;
    s16 i;
    s16 ret;
    M544PtclWork *work;

    ret = -1;
    particle = (HU3DPARTICLE *)Hu3DData[lbl_1_bss_826].hookData;
    work = (M544PtclWork *)particle->work;
    data = particle->data + particle->emitCnt;
    for (i = particle->emitCnt; i < particle->maxCnt; i++, data++) {
        if (0.0f == data->scale) {
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
        data->parManId = -1;
        particle->emitCnt = i;
        ret = i;
    }
    return ret;
}

void fn_1_30B8(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx)
{
    M544PtclWork *work;
    HU3DPARTICLEDATA *data;
    s16 i;
    s16 v;

    work = (M544PtclWork *)particleP->work;
    data = particleP->data;
    data->time = 0;
    for (i = 0; i < particleP->maxCnt; i++, data++) {
        if (data->scale > 0.0f) {
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
            if (v < 1) {
                data->scale = 0.0f;
            }
            data->color.a = v;
            if (data->scale != 0.0f) {
                data->scale = data->scaleBase;
                data->scaleBase = data->scaleBase + work[i].scaleStep;
                if (data->scaleBase <= 0.01f) {
                    data->scale = 0.0f;
                }
            }
            data->time += 1;
        }
    }
    DCStoreRangeNoSync(particleP->data, particleP->maxCnt * sizeof(HU3DPARTICLEDATA));
}

void fn_1_34D0(void)
{
    HU3DPARTICLE *particle;

    HuDataSelHeapReadNum(0x66001E, 0x10000000, 2);
    lbl_1_bss_824 = Hu3DParticleCreate(HuSprAnimRead(HuDataSelHeapReadNum(0x66001E, 0x10000000, 2)), 1);
    Hu3DParticleHookSet(lbl_1_bss_824, fn_1_3AD4);
    Hu3DParticleBlendModeSet(lbl_1_bss_824, 0);
    Hu3DModelLayerSet(lbl_1_bss_824, 7);
    Hu3DModelAttrSet(lbl_1_bss_824, 1);
    Hu3DModelAttrReset(lbl_1_bss_824, 1);
    lbl_1_bss_D0.flag2C = 0;
    particle = (HU3DPARTICLE *)Hu3DData[lbl_1_bss_824].hookData;
    particle->data->scale = 0.0f;
}

void fn_1_35DC(s16 arg0)
{
    f32 yOff[13] = { 150.0f, 170.0f, 188.0f, 169.0f, 164.0f, 180.0f, 204.0f, 103.0f, 150.0f, 150.0f, 150.0f, 150.0f, 150.0f };
    f32 tblA[13][6] = {
        { 0.0f, -50.0f, 0.0f, 0.0f, 50.0f, 0.0f },
        { -10.0f, -50.0f, 0.0f, -10.0f, 50.0f, 0.0f },
        { -10.0f, -50.0f, 0.0f, -10.0f, 50.0f, 0.0f },
        { 0.0f, -50.0f, 0.0f, 0.0f, 50.0f, 0.0f },
        { 0.0f, -50.0f, 0.0f, 0.0f, 50.0f, 0.0f },
        { -10.0f, -50.0f, 0.0f, -10.0f, 50.0f, 0.0f },
        { 40.0f, -50.0f, 0.0f, 40.0f, 50.0f, 0.0f },
        { -5.0f, -50.0f, -10.0f, -5.0f, 60.0f, -10.0f },
        { 0.0f, -50.0f, 0.0f, 0.0f, 50.0f, 0.0f },
        { 0.0f, -50.0f, 0.0f, 0.0f, 50.0f, 0.0f },
        { 0.0f, -50.0f, 0.0f, 0.0f, 50.0f, 0.0f },
        { 0.0f, -50.0f, 0.0f, 0.0f, 50.0f, 0.0f },
        { 0.0f, -50.0f, 0.0f, 0.0f, 50.0f, 0.0f },
    };
    f32 tblB[13][6] = {
        { 20.0f, -50.0f, 0.0f, 20.0f, 50.0f, 0.0f },
        { 20.0f, -50.0f, 0.0f, 20.0f, 50.0f, 0.0f },
        { 20.0f, -50.0f, -5.0f, 20.0f, 50.0f, -5.0f },
        { 10.0f, -50.0f, 0.0f, 10.0f, 50.0f, 0.0f },
        { 10.0f, -50.0f, 0.0f, 10.0f, 50.0f, 0.0f },
        { 20.0f, -50.0f, -5.0f, 20.0f, 50.0f, -5.0f },
        { -25.0f, -50.0f, 0.0f, -25.0f, 50.0f, 0.0f },
        { 30.0f, -50.0f, 0.0f, 30.0f, 60.0f, 0.0f },
        { 10.0f, -50.0f, 0.0f, 10.0f, 50.0f, 0.0f },
        { 10.0f, -50.0f, 0.0f, 10.0f, 50.0f, 0.0f },
        { 10.0f, -50.0f, 0.0f, 10.0f, 50.0f, 0.0f },
        { 10.0f, -50.0f, 0.0f, 10.0f, 50.0f, 0.0f },
        { 10.0f, -50.0f, 0.0f, 10.0f, 50.0f, 0.0f },
    };
    HuVecF base;
    HuVecF vA;
    HuVecF vB;
    HuVecF sp8;
    HuVecF sp14;
    f32 scale;
    s16 id;
    s16 ch;
    HuVecF *p;

    id = arg0;
    ch = lbl_1_bss_914->chars[id].charNo;
    if (ch >= 0xD) {
        ch = 6;
    }
    if (id >= 2) {
        id = (lbl_1_bss_D0.plrMask & 2) >> 1;
    }
    p = &lbl_1_bss_914->models[id + 0x20].pos;
    base.x = p->x;
    base.y = p->y;
    base.z = p->z;
    vA.x = p->x;
    vA.y = p->y;
    vA.z = p->z;
    vB.x = p->x;
    vB.y = p->y;
    vB.z = p->z;
    if (id == 0) {
        vA.x += tblA[ch][0];
        vA.y += tblA[ch][1] + yOff[ch];
        vA.z += tblA[ch][2];
        vB.x += tblA[ch][3];
        vB.y += tblA[ch][4] + yOff[ch];
        vB.z += tblA[ch][5];
        scale = 35.0f;
    } else {
        vA.x += tblB[ch][0];
        vA.y += tblB[ch][1] + yOff[ch];
        vA.z += tblB[ch][2];
        vB.x += tblB[ch][3];
        vB.y += tblB[ch][4] + yOff[ch];
        vB.z += tblB[ch][5];
        scale = 50.0f;
    }
    sp8.x = vB.x;
    sp8.y = vB.y;
    sp8.z = vB.z;
    sp14.x = vA.x;
    sp14.y = vA.y;
    sp14.z = vA.z;
    fn_1_39E8(&sp14, &sp8, scale);
}

void fn_1_39E8(HuVecF *arg0, HuVecF *arg1, f32 scale)
{
    HU3DPARTICLEDATA *data;

    data = ((HU3DPARTICLE *)Hu3DData[lbl_1_bss_824].hookData)->data;
    data->time = 0;
    data->parManId = 0;
    data->pos.x = arg0->x;
    data->pos.y = arg0->y;
    data->pos.z = arg0->z;
    data->vel.x = arg0->x;
    data->vel.y = arg0->y;
    data->vel.z = arg0->z;
    data->accel.x = arg1->x;
    data->accel.y = arg1->y;
    data->accel.z = arg1->z;
    data->color.r = 0xFF;
    data->color.g = 0xFF;
    data->color.b = 0xFF;
    data->color.a = 0xFF;
    data->scale = scale;
    if (data->scale != 0.0f) {
        lbl_1_bss_D0.flag2C = 1;
    }
}

void fn_1_3AD4(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx)
{
    HU3DPARTICLEDATA *data;
    s16 r1;
    s16 r2;
    u8 a;

    data = particleP->data;
    if (data->scale > 0.0f) {
        switch (data->time) {
        case 0:
            r1 = fn_1_18E8(&data->pos.y, 10.0f, data->vel.y, data->accel.y);
            if (r1 == 0) {
                data->vel.x = data->pos.x;
                data->vel.y = data->pos.y;
                data->vel.z = data->pos.z;
                data->accel.x = data->pos.x;
                data->accel.y = data->pos.y;
                data->accel.z = data->pos.z;
                data->accel.y = data->accel.y - 20.0f;
                data->speedDecay = 3.0f;
                data->time += 1;
            }
            break;
        case 1:
            r2 = fn_1_18E8(&data->pos.y, 3.0f, data->vel.y, data->accel.y);
            if (r2 == 0) {
                data->time += 1;
            }
            break;
        case 2:
            if (data->parManId++ > 0x28) {
                data->time += 1;
            }
            break;
        case 3:
            a = data->color.a - 0x32;
            data->color.a = a;
            if (a < 0x32) {
                data->color.a = 0;
                data->scale = 0.0f;
            }
            break;
        }
    }
    DCStoreRangeNoSync(particleP->data, particleP->maxCnt * sizeof(HU3DPARTICLEDATA));
}

void fn_1_3EF8(void)
{
    lbl_1_bss_D0.jingleId = HuAudJinglePlay(0x83);
}

void fn_1_3F28(void)
{
    lbl_1_bss_D0.bgmId = HuAudBGMPlay(0x3B);
}

void fn_1_3F58(s16 arg0, s16 arg1)
{
    s16 id;

    if (arg0 != 0) {
        id = lbl_1_bss_D0.bgmId;
    } else {
        id = lbl_1_bss_D0.jingleId;
    }
    HuAudSStreamFadeOut(id, arg1);
}

void fn_1_3FC0(s16 arg0)
{
    s16 id;

    if (arg0 != 0) {
        id = lbl_1_bss_D0.bgmId;
    } else {
        id = lbl_1_bss_D0.jingleId;
    }
    HuAudSStreamStop(id);
}

void fn_1_4020(s16 arg0)
{
    lbl_1_bss_D0.fxFlag[arg0] = 1;
    lbl_1_bss_D0.fxId[arg0] = HuAudFXPlay(lbl_1_data_3A8[arg0]);
}

void fn_1_4098(s16 arg0, s16 arg1, s16 arg2)
{
    lbl_1_bss_D0.fxFlag[arg0] = 1;
    lbl_1_bss_D0.fxId[arg0] = HuAudFXPlayVolPan(lbl_1_data_3A8[arg0], arg1, arg2);
}

void fn_1_4120(s16 arg0, s16 arg1)
{
    if (lbl_1_bss_D0.fxFlag[arg0] == 1) {
        if (HuAudFXStatusGet(lbl_1_bss_D0.fxId[arg0]) == 2) {
            HuAudFXPanning(lbl_1_bss_D0.fxId[arg0], arg1);
        }
    }
}

void fn_1_41B0(s16 arg0)
{
    if (lbl_1_bss_D0.fxFlag[arg0] == 1) {
        if (HuAudFXStatusGet(lbl_1_bss_D0.fxId[arg0]) == 2 || HuAudFXStatusGet(lbl_1_bss_D0.fxId[arg0]) == 3) {
            HuAudFXStop(lbl_1_bss_D0.fxId[arg0]);
        }
    }
}

void fn_1_425C(s16 arg0)
{
    if (lbl_1_bss_D0.fxFlag[arg0] == 1) {
        if (HuAudFXStatusGet(lbl_1_bss_D0.fxId[arg0]) == 2 || HuAudFXStatusGet(lbl_1_bss_D0.fxId[arg0]) == 3) {
            HuAudFXStop(lbl_1_bss_D0.fxId[arg0]);
        }
    }
}

void fn_1_4308(void)
{
    HuAudAllStop();
}
