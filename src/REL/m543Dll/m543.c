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
#include "REL/m543Dll.h"

double sin(double x);
double cos(double x);
void DCStoreRangeNoSync(void *addr, u32 nBytes);

typedef void (*M543VoidFunc)(void);
extern const M543VoidFunc _ctors[];
extern const M543VoidFunc _dtors[];

/* game.c */
s32 fn_1_5A6C(M543State *state);
s16 fn_1_5B78(void);
s32 fn_1_5BB0(void);
s32 fn_1_5C20(void);
s32 fn_1_623C(void);
s16 fn_1_6314(void);
s32 fn_1_634C(void);
s16 fn_1_64AC(void);
s32 fn_1_64DC(void);

/* stage.c */
void fn_1_FA94(M543State *state, M543StageModel *models, s32 *dataNums);
void fn_1_FCF4(s16 arg0, s32 *arg1);
void fn_1_FD14(s16 arg0, s16 *arg1);
void fn_1_10304(OMOBJ *obj);

void ObjectSetup(void);
void fn_1_2C0(s16 mode, s16 frameNo);
void fn_1_378(s16 mode, s16 frameNo);
void fn_1_3A4(s16 mode, s16 frameNo);
void fn_1_3D0(s16 mode, s16 frameNo);
void fn_1_41C(s16 mode, s16 frameNo);
void fn_1_448(OMOBJ *obj);
void fn_1_44C(s16 arg0);
void fn_1_91C(s16 arg0);
void fn_1_E94(void);
void fn_1_10C4(void);
void fn_1_139C(void);
void fn_1_14C4(void);
s16 fn_1_2F3C(M543PtclWork *arg0, f32 x, f32 y, f32 z, f32 scale);
void fn_1_3110(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);

/* unit-owned .bss, declared in reverse address order so the offsets match the original */
s16 gap_05_000015DE_bss;
s16 lbl_1_bss_15DC;
void *lbl_1_bss_15D8;
OMOBJMAN *lbl_1_bss_15D4;
M543State lbl_1_bss_12EC;
M543StageModel lbl_1_bss_C24[62];
s16 lbl_1_bss_C08[2][7];
M543Game lbl_1_bss_B94;
M543Actor lbl_1_bss_8DC[2];
M543Tile lbl_1_bss_33C[45];
M543Node lbl_1_bss_1C[40];
M543Cam *lbl_1_bss_18;
M543Light *lbl_1_bss_14;
M543Shadow *lbl_1_bss_10;
M543Mg lbl_1_bss_0;

/* unit-owned .data, in original address order */
MGSEQ_PARAM lbl_1_data_0 = {
    0, 0,
    (MGSEQ_FUNC)fn_1_2C0, (MGSEQ_FUNC)fn_1_2F4, (MGSEQ_FUNC)fn_1_320,
    (MGSEQ_FUNC)fn_1_34C, (MGSEQ_FUNC)fn_1_378, (MGSEQ_FUNC)fn_1_3A4,
    (MGSEQ_FUNC)fn_1_3D0, (MGSEQ_FUNC)fn_1_3FC, (MGSEQ_FUNC)fn_1_41C,
};
s32 lbl_1_data_28[13][7] = {
    { 0x8C0000, 0x8C0002, 0x8C000F, 0x8C0017, 0x8C0018, 0x8C0006, 0x8C0031 },
    { 0x330000, 0x330002, 0x33000F, 0x330017, 0x330018, 0x330006, 0x330031 },
    { 0xB70000, 0xB70002, 0xB7000F, 0xB70017, 0xB70018, 0xB70006, 0xB70031 },
    { 0xED0000, 0xED0002, 0xED000F, 0xED0017, 0xED0018, 0xED0006, 0xED0031 },
    { 0xE60000, 0xE60002, 0xE6000F, 0xE60017, 0xE60018, 0xE60006, 0xE60031 },
    { 0x170000, 0x170002, 0x17000F, 0x170017, 0x170018, 0x170006, 0x170031 },
    { 0xE00000, 0xE00002, 0xE0000F, 0xE00017, 0xE00018, 0xE00006, 0xE00031 },
    { 0x2D0000, 0x2D0002, 0x2D000F, 0x2D0017, 0x2D0018, 0x2D0006, 0x2D0031 },
    { 0xC90000, 0xC90002, 0xC9000F, 0xC90017, 0xC90018, 0xC90006, 0xC90031 },
    { 0xB00000, 0xB00002, 0xB0000F, 0xB00017, 0xB00018, 0xB00006, 0xB00031 },
    { 0xB00000, 0xB00002, 0xB0000F, 0xB00017, 0xB00018, 0xB00006, 0xB00031 },
    { 0xB00000, 0xB00002, 0xB0000F, 0xB00017, 0xB00018, 0xB00006, 0xB00031 },
    { 0xB00000, 0xB00002, 0xB0000F, 0xB00017, 0xB00018, 0xB00006, 0xB00031 },
};
s32 lbl_1_data_194[17] = {
    0x650000, 0x650001, 0x650002, 0x650003, 0x650004, 0x650005, 0x650006, 0x650007, 0x650008, 0x650009, 0x65000A, 0x65000B, 0x65000C, 0x65000D, 0x65000E, 0x65000F, 0x0
};
M543CamInit lbl_1_data_1D8[4] = {
    { { 500.0f, 208.0f, -5.0f }, -45.0f, 0.0f, 0.0f, 800.0f, 1.33f, 0.0f, 0.0f, 640.0f, 480.0f, 45.0f },
    { { 0.0f, 208.0f, -5.0f }, -89.0f, 0.0f, 0.0f, 1605.0f, 1.33f, 0.0f, 0.0f, 640.0f, 480.0f, 45.0f },
    { { 0.0f, 208.0f, 20.0f }, -35.0f, 0.0f, 0.0f, 1415.0f, 1.33f, 0.0f, 0.0f, 640.0f, 480.0f, 45.0f },
    { { 0.0f, 0.0f, 0.0f }, 0.0f, 0.0f, 500.0f, 500.0f, 1.33f, 0.0f, 0.0f, 640.0f, 480.0f, 45.0f },
};
s16 lbl_1_data_2A8[16] = {
    0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000, 0x8000,
};
char lbl_1_data_2C8[0x2E] = "OBJ X:%.1f Y:%.1f Z:%.1f RX%.1f RY%.1f RZ%.1f";
char lbl_1_data_2F6[8] = "OBJW %d";
char lbl_1_data_2FE[0x25] = "CAMPOS X:%.1f Y:%.1f Z:%.1f ZOOM%.1f";
char lbl_1_data_323[0x1D] = "CAMROT X:%.1f Y:%.1f Z:%.1f";
M543PtclWork lbl_1_data_340 = {
    0, 0,
    { 0xFF, 0xFF, 0xFF, 0xFF }, { 0x00, 0x00, 0x00, 0xFF },
    0.0f, 2.0f, 1.0f,
    0.85f, 0.85f, 0.85f,
    0.2f,
    0.0f, 1.0f, -3.0f, 0.02f,
};
s16 lbl_1_data_378[4] = { 0x819, 0x81A, 0x81B, 0x81C };

int _prolog(void)
{
    const M543VoidFunc *ctors = _ctors;

    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const M543VoidFunc *dtors = _dtors;

    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

void ObjectSetup(void)
{
    s16 i;

    lbl_1_bss_15D4 = MgActorObjectSetup();
    fn_1_FA94(&lbl_1_bss_12EC, lbl_1_bss_C24, lbl_1_data_194);
    lbl_1_bss_18 = HuMemDirectMallocNum(HEAP_HEAP, 0x148, 0x10000000);
    memset(lbl_1_bss_18, 0, 0x148);
    lbl_1_bss_14 = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M543Light), 0x10000000);
    memset(lbl_1_bss_14, 0, sizeof(M543Light));
    lbl_1_bss_10 = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M543Shadow), 0x10000000);
    memset(lbl_1_bss_10, 0, sizeof(M543Shadow));
    fn_1_44C(0);
    fn_1_E94();
    fn_1_139C();
    fn_1_FCF4(0, lbl_1_data_28[lbl_1_bss_12EC.chars[0].charNo]);
    fn_1_FCF4(1, lbl_1_data_28[lbl_1_bss_12EC.chars[1].charNo]);
    for (i = 0; i < 2; i++) {
        fn_1_FD14(i, lbl_1_bss_C08[i]);
    }
    omAddObjEx(lbl_1_bss_15D4, 0x7FDA, 0, 0, -1, (OMOBJFUNC)fn_1_10304);
    omAddObjEx(lbl_1_bss_15D4, 0x7FDA, 0, 0, -1, (OMOBJFUNC)fn_1_448);
    Hu3DBGColorSet(0, 0, 0);
    MgSeqCreate(&lbl_1_data_0);
    MgSeqModeDelaySet(3, 1);
    MgSeqModeDelaySet(2, 1);
    MgSeqModeDelaySet(2, 0x230);
    MgSeqModeDelaySet(7, 0x258);
    MgSeqModeDelaySet(9, 0x3C);
}

void fn_1_2C0(s16 mode, s16 frameNo)
{
    if (fn_1_5A6C(&lbl_1_bss_12EC) != 0) {
        MgSeqModeNext();
    }
}

void fn_1_2F4(void)
{
    if (fn_1_5B78() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_320(void)
{
    if (fn_1_5BB0() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_34C(void)
{
    if (fn_1_5C20() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_378(s16 mode, s16 frameNo)
{
    if (fn_1_623C() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_3A4(s16 mode, s16 frameNo)
{
    if (fn_1_6314() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_3D0(s16 mode, s16 frameNo)
{
    if (fn_1_634C() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_3FC(void)
{
    fn_1_64AC();
}

void fn_1_41C(s16 mode, s16 frameNo)
{
    if (fn_1_64DC() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_448(OMOBJ *obj)
{
}

void fn_1_44C(s16 arg0)
{
    lbl_1_bss_18[arg0].idx = arg0;
    lbl_1_bss_18[arg0].center.x = lbl_1_data_1D8[arg0].center.x;
    lbl_1_bss_18[arg0].center.y = lbl_1_data_1D8[arg0].center.y;
    lbl_1_bss_18[arg0].center.z = lbl_1_data_1D8[arg0].center.z;
    lbl_1_bss_18[arg0].rotX = lbl_1_data_1D8[arg0].rotX;
    lbl_1_bss_18[arg0].rotY = lbl_1_data_1D8[arg0].rotY;
    lbl_1_bss_18[arg0].rotZ = lbl_1_data_1D8[arg0].rotZ;
    lbl_1_bss_18[arg0].zoom = lbl_1_data_1D8[arg0].zoom;
    lbl_1_bss_18[arg0].aspect = lbl_1_data_1D8[arg0].aspect;
    lbl_1_bss_18[arg0].vpX = lbl_1_data_1D8[arg0].vpX;
    lbl_1_bss_18[arg0].vpY = lbl_1_data_1D8[arg0].vpY;
    lbl_1_bss_18[arg0].vpW = lbl_1_data_1D8[arg0].vpW;
    lbl_1_bss_18[arg0].vpH = lbl_1_data_1D8[arg0].vpH;
    lbl_1_bss_18[arg0].fov = lbl_1_data_1D8[arg0].fov;
    Hu3DCameraCreate(lbl_1_data_2A8[arg0]);
    Hu3DCameraViewportSet(lbl_1_data_2A8[arg0], lbl_1_bss_18[arg0].vpX, lbl_1_bss_18[arg0].vpY,
                          lbl_1_bss_18[arg0].vpW, lbl_1_bss_18[arg0].vpH, 0.0f, 1.0f);
    Hu3DCameraPerspectiveSet(lbl_1_data_2A8[arg0], lbl_1_bss_18[arg0].fov, 20.0f, 30000.0f, lbl_1_bss_18[arg0].aspect);
    fn_1_91C(arg0);
    Center.x = lbl_1_data_1D8[0].center.x;
    Center.y = lbl_1_data_1D8[0].center.y;
    Center.z = lbl_1_data_1D8[0].center.z;
    CRot.x = lbl_1_data_1D8[0].rotX;
    CRot.y = lbl_1_data_1D8[0].rotY;
    CRot.z = lbl_1_data_1D8[0].rotZ;
    CZoom = lbl_1_data_1D8[arg0].zoom;
}

void fn_1_91C(s16 arg0)
{
    f64 temp;

    temp = sin(3.141592653589793 * lbl_1_bss_18[arg0].rotY / 180.0);
    lbl_1_bss_18[arg0].pos.x = lbl_1_bss_18[arg0].center.x
        + lbl_1_bss_18[arg0].zoom * (cos(3.141592653589793 * lbl_1_bss_18[arg0].rotX / 180.0) * temp);
    lbl_1_bss_18[arg0].pos.y = lbl_1_bss_18[arg0].center.y
        + lbl_1_bss_18[arg0].zoom * -sin(3.141592653589793 * lbl_1_bss_18[arg0].rotX / 180.0);
    temp = cos(3.141592653589793 * lbl_1_bss_18[arg0].rotY / 180.0);
    lbl_1_bss_18[arg0].pos.z = lbl_1_bss_18[arg0].center.z
        + lbl_1_bss_18[arg0].zoom * (cos(3.141592653589793 * lbl_1_bss_18[arg0].rotX / 180.0) * temp);
    temp = sin(3.141592653589793 * lbl_1_bss_18[arg0].rotY / 180.0);
    lbl_1_bss_18[arg0].up.x = sin(3.141592653589793 * lbl_1_bss_18[arg0].rotX / 180.0) * temp;
    lbl_1_bss_18[arg0].up.y = cos(3.141592653589793 * lbl_1_bss_18[arg0].rotX / 180.0);
    temp = cos(3.141592653589793 * lbl_1_bss_18[arg0].rotY / 180.0);
    lbl_1_bss_18[arg0].up.z = sin(3.141592653589793 * lbl_1_bss_18[arg0].rotX / 180.0) * temp;
    Hu3DCameraPosSet(lbl_1_data_2A8[arg0], lbl_1_bss_18[arg0].pos.x, lbl_1_bss_18[arg0].pos.y, lbl_1_bss_18[arg0].pos.z,
                     lbl_1_bss_18[arg0].up.x, lbl_1_bss_18[arg0].up.y, lbl_1_bss_18[arg0].up.z,
                     lbl_1_bss_18[arg0].center.x, lbl_1_bss_18[arg0].center.y, lbl_1_bss_18[arg0].center.z);
}

void fn_1_E94(void)
{
    Hu3DLighInit();
    lbl_1_bss_14->pos.x = 0.0f;
    lbl_1_bss_14->pos.y = 0.0f;
    lbl_1_bss_14->pos.z = 0.0f;
    lbl_1_bss_14->target.x = 0.0f;
    lbl_1_bss_14->target.y = 0.0f;
    lbl_1_bss_14->target.z = 0.0f;
    lbl_1_bss_14->rotX = -23.0f;
    lbl_1_bss_14->rotY = 0.0f;
    lbl_1_bss_14->rotZ = 0.0f;
    lbl_1_bss_14->dist = 1000.0f;
    lbl_1_bss_14->color.r = 0xFF;
    lbl_1_bss_14->color.g = 0xFF;
    lbl_1_bss_14->color.b = 0xFF;
    lbl_1_bss_14->color.a = 0xFF;
    PSVECSubtract((Vec *)&lbl_1_bss_14->target, (Vec *)&lbl_1_bss_14->pos, (Vec *)&lbl_1_bss_14->dir);
    lbl_1_bss_14->id = Hu3DGLightCreateV((HuVecF *)&lbl_1_bss_14->pos, (HuVecF *)&lbl_1_bss_14->dir, &lbl_1_bss_14->color);
    fn_1_10C4();
    Hu3DGLightStaticSet(lbl_1_bss_14->id, 1);
    Hu3DGLightInfinitytSet(lbl_1_bss_14->id);
}

void fn_1_10C4(void)
{
    f64 temp;

    temp = sin(3.141592653589793 * lbl_1_bss_14->rotY / 180.0);
    lbl_1_bss_14->pos.x = lbl_1_bss_14->target.x
        + lbl_1_bss_14->dist * (cos(3.141592653589793 * lbl_1_bss_14->rotX / 180.0) * temp);
    lbl_1_bss_14->pos.y = lbl_1_bss_14->target.y
        + lbl_1_bss_14->dist * -sin(3.141592653589793 * lbl_1_bss_14->rotX / 180.0);
    temp = cos(3.141592653589793 * lbl_1_bss_14->rotY / 180.0);
    lbl_1_bss_14->pos.z = lbl_1_bss_14->target.z
        + lbl_1_bss_14->dist * (cos(3.141592653589793 * lbl_1_bss_14->rotX / 180.0) * temp);
    PSVECSubtract((Vec *)&lbl_1_bss_14->target, (Vec *)&lbl_1_bss_14->pos, (Vec *)&lbl_1_bss_14->dir);
    PSVECNormalize((Vec *)&lbl_1_bss_14->dir, (Vec *)&lbl_1_bss_14->dir);
    Hu3DGLightPosSetV(lbl_1_bss_14->id, (HuVecF *)&lbl_1_bss_14->pos, (HuVecF *)&lbl_1_bss_14->dir);
    Hu3DGLightColorSet(lbl_1_bss_14->id, lbl_1_bss_14->color.r, lbl_1_bss_14->color.g, lbl_1_bss_14->color.b,
                       lbl_1_bss_14->color.a);
}

void fn_1_139C(void)
{
    Hu3DShadowCreate(30.0f, 20.0f, 30000.0f);
    lbl_1_bss_10->rotX = -90.0f;
    lbl_1_bss_10->rotY = 0.0f;
    lbl_1_bss_10->rotZ = 0.0f;
    lbl_1_bss_10->target.x = 0.0f;
    lbl_1_bss_10->target.y = 0.0f;
    lbl_1_bss_10->target.z = 0.0f;
    lbl_1_bss_10->tpLvl = 0.5f;
    lbl_1_bss_10->dist = 3500.0f;
    fn_1_14C4();
}

void fn_1_14C4(void)
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
    temp = sin(3.141592653589793 * lbl_1_bss_10->rotY / 180.0);
    lbl_1_bss_10->up.x = sin(3.141592653589793 * lbl_1_bss_10->rotX / 180.0) * temp;
    lbl_1_bss_10->up.y = cos(3.141592653589793 * lbl_1_bss_10->rotX / 180.0);
    temp = cos(3.141592653589793 * lbl_1_bss_10->rotY / 180.0);
    lbl_1_bss_10->up.z = sin(3.141592653589793 * lbl_1_bss_10->rotX / 180.0) * temp;
    Hu3DShadowTPLvlSet(lbl_1_bss_10->tpLvl);
    Hu3DShadowPosSet((HuVecF *)&lbl_1_bss_10->pos, (HuVecF *)&lbl_1_bss_10->up, (HuVecF *)&lbl_1_bss_10->target);
}

s32 fn_1_1850(f32 *arg0, f32 steps, f32 start, f32 target)
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

s32 fn_1_1970(f32 *arg0, f32 steps, f32 start, f32 target)
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

MGTIMER *fn_1_1A8C(s32 type, s32 maxSec, s32 endSec, f32 x, f32 y)
{
    MGTIMER *timer;

    timer = MgTimerCreate(type);
    MgTimerParamSet(timer, maxSec * 60, endSec * 60, 0);
    MgTimerPosSet(timer, x, y);
    return timer;
}

void fn_1_1B00(MGTIMER *timer)
{
    MgTimerModeOnSet(timer, 1);
}

BOOL fn_1_1B2C(MGTIMER *timer)
{
    return MgTimerDoneCheck(timer);
}

void fn_1_1B54(MGTIMER *timer)
{
    MgTimerDispOff(timer);
    MgTimerKill(timer);
}

s32 fn_1_1B8C(M543State *state, s16 pad, s16 idx)
{
    M543StageModel *model;

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
    print8(0x19, 0x88, 1.5f, lbl_1_data_2C8, model->x, model->y, model->z, model->rotX, model->rotY, model->rotZ);
    print8(0x19, 0x9A, 1.5f, lbl_1_data_2F6, idx);
    return 0;
}

void fn_1_1EE0(void)
{
}

void fn_1_1EE4(s16 pad)
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
    print8(0x19, 0x1E, 1.5f, lbl_1_data_2FE, Center.x, Center.y, Center.z, CZoom);
    print8(0x19, 0x30, 1.5f, lbl_1_data_323, CRot.x, CRot.y, CRot.z);
}

void fn_1_239C(void)
{
    HU3DPARTICLE *particle;
    ANIMDATA *anim;
    HU3DPARTICLEDATA *data;
    s16 i;

    anim = HuSprAnimRead(HuDataSelHeapReadNum(0x650013, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_15D8 = HuMemDirectMallocNum(HEAP_HEAP, 1000 * sizeof(M543PtclWork), HU_MEMNUM_OVL);
    lbl_1_bss_15DC = Hu3DParticleCreate(anim, 1000);
    Hu3DParticleHookSet(lbl_1_bss_15DC, fn_1_3110);
    Hu3DParticleBlendModeSet(lbl_1_bss_15DC, 1);
    Hu3DModelLayerSet(lbl_1_bss_15DC, 0);
    Hu3DModelCameraSet(lbl_1_bss_15DC, 1);
    Hu3DModelAttrSet(lbl_1_bss_15DC, 1);
    Hu3DModelAttrReset(lbl_1_bss_15DC, 1);
    particle = (HU3DPARTICLE *)Hu3DData[lbl_1_bss_15DC].hookData;
    particle->emitCnt = 0;
    particle->work = lbl_1_bss_15D8;
    particle->count = 0;
    data = particle->data;
    for (i = 0; i < particle->maxCnt; i++, data++) {
        data->scale = 0.0f;
    }
}

void fn_1_251C(M543Vec3 *arg0)
{
    M543Vec3 sp;
    f32 speed;
    s16 t;
    s16 i;

    for (i = 0; i < 12; i++) {
        speed = 6.0f + frandmod(6);
        lbl_1_data_340.velX = speed * sin(3.141592653589793 * (30.0f * i) / 180.0);
        lbl_1_data_340.velY = 0.0f;
        lbl_1_data_340.velZ = speed * cos(3.141592653589793 * (30.0f * i) / 180.0);
        t = -frandmod(6) - 7;
        lbl_1_data_340.alphaStep = t;
        sp.x = arg0->x + 20.0 * sin(3.141592653589793 * (30.0f * i) / 180.0);
        sp.y = 10.0f + arg0->y;
        sp.z = arg0->z + 20.0 * cos(3.141592653589793 * (30.0f * i) / 180.0);
        lbl_1_data_340.accelY = 0.01f * frandmod(20);
        fn_1_2F3C(&lbl_1_data_340, sp.x, sp.y, sp.z, 20.0f + frandmod(20));
    }
}

void fn_1_2898(M543Vec3 *arg0, s16 mode)
{
    M543Vec3 sp;
    s16 t;
    s16 i;

    if (mode == 0) {
        for (i = 0; i < 48; i++) {
            lbl_1_data_340.velX = (5.0f + frandmod(5)) * sin(3.141592653589793 * (7.5f * i) / 180.0);
            lbl_1_data_340.velY = frandmod(2);
            lbl_1_data_340.velZ = frandmod(10) - 5.0f;
            t = (f32)-frandmod(6) - 6.0f;
            lbl_1_data_340.alphaStep = t;
            sp.x = arg0->x + 60.0 * sin(3.141592653589793 * (7.5f * i) / 180.0);
            sp.y = 5.0f + arg0->y;
            sp.z = arg0->z + frandmod(40) - 20.0f;
            lbl_1_data_340.accelY = 0.01f * frandmod(20);
            fn_1_2F3C(&lbl_1_data_340, sp.x, sp.y, sp.z, 10.0f + frandmod(15));
        }
        return;
    }
    for (i = 0; i < 48; i++) {
        lbl_1_data_340.velX = frandmod(10) - 5.0f;
        lbl_1_data_340.velY = frandmod(2);
        lbl_1_data_340.velZ = (5.0f + frandmod(5)) * sin(3.141592653589793 * (7.5f * i) / 180.0);
        t = (f32)-frandmod(6) - 6.0f;
        lbl_1_data_340.alphaStep = t;
        sp.x = arg0->x + frandmod(40) - 20.0f;
        sp.y = 5.0f + arg0->y;
        sp.z = arg0->z + 60.0 * sin(3.141592653589793 * (7.5f * i) / 180.0);
        lbl_1_data_340.accelY = 0.01f * frandmod(20);
        fn_1_2F3C(&lbl_1_data_340, sp.x, sp.y, sp.z, 10.0f + frandmod(15));
    }
}

s16 fn_1_2F3C(M543PtclWork *arg0, f32 x, f32 y, f32 z, f32 scale)
{
    HU3DPARTICLEDATA *data;
    HU3DPARTICLE *particle;
    s16 i;
    s16 ret;
    M543PtclWork *work;

    ret = -1;
    particle = (HU3DPARTICLE *)Hu3DData[lbl_1_bss_15DC].hookData;
    work = (M543PtclWork *)particle->work;
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

void fn_1_3110(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx)
{
    M543PtclWork *work;
    HU3DPARTICLEDATA *data;
    s16 i;
    s16 v;

    work = (M543PtclWork *)particleP->work;
    data = particleP->data;
    i = 0;
    data->time = 0;
    for (; i < particleP->maxCnt; i++, data++) {
        if (0.0f != data->scale) {
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
    DCStoreRangeNoSync(particleP->data, particleP->maxCnt * sizeof(HU3DPARTICLEDATA));
}

void fn_1_3558(s16 arg0)
{
    s16 i;
    HSFMATERIAL *mat;
    HU3DMODEL *model;
    u8 *buf;
    s16 sp8;

    sp8 = 0;
    model = &Hu3DData[lbl_1_bss_0.player[arg0]->actor->mdlId];
    buf = HuMemDirectMallocNum(HEAP_HEAP, model->hsf->materialNum * 3, 0x10000000);
    lbl_1_bss_8DC[arg0].matColors = buf;
    mat = model->hsf->material;
    for (i = 0; i < model->hsf->materialNum; i++, mat++) {
        *buf++ = mat->color[0];
        *buf++ = mat->color[1];
        *buf++ = mat->color[2];
    }
}

s32 fn_1_3650(s16 arg0, f32 scale)
{
    HSFMATERIAL *mat;
    HU3DMODEL *model;
    s16 i;
    s32 ret;
    M543Actor *actor;

    ret = 0;
    model = &Hu3DData[lbl_1_bss_0.player[arg0]->actor->mdlId];
    actor = &lbl_1_bss_8DC[arg0];
    actor->colorScale = actor->colorScale * scale;
    if (actor->colorScale < 0.9f) {
        ret = 1;
    }
    mat = model->hsf->material;
    for (i = 0; i < model->hsf->materialNum; i++, mat++) {
        mat->color[0] = (s32)(mat->color[0] * scale);
        mat->color[1] = (s32)(mat->color[1] * scale);
        mat->color[2] = (s32)(mat->color[2] * scale);
    }
    return ret;
}

void fn_1_37C0(s16 arg0)
{
    s16 i;
    HSFMATERIAL *mat;
    HU3DMODEL *model;
    u8 *buf;
    s16 sp8;

    sp8 = 0;
    lbl_1_bss_8DC[arg0].colorScale = 255.0f;
    model = &Hu3DData[lbl_1_bss_0.player[arg0]->actor->mdlId];
    buf = lbl_1_bss_8DC[arg0].matColors;
    mat = model->hsf->material;
    for (i = 0; i < model->hsf->materialNum; i++, mat++) {
        mat->color[0] = *buf++;
        mat->color[1] = *buf++;
        mat->color[2] = *buf++;
    }
}

void fn_1_38BC(void)
{
    lbl_1_bss_B94.jingleStream = HuAudJinglePlay(0x83);
}

void fn_1_38EC(void)
{
    lbl_1_bss_B94.bgmStream = HuAudBGMPlay(0x56);
}

void fn_1_391C(s16 isBgm, s16 speed)
{
    s16 stream;

    if (isBgm != 0) {
        stream = lbl_1_bss_B94.bgmStream;
    } else {
        stream = lbl_1_bss_B94.jingleStream;
    }
    HuAudSStreamFadeOut(stream, speed);
}

void fn_1_3984(s16 isBgm)
{
    s16 stream;

    if (isBgm != 0) {
        stream = lbl_1_bss_B94.bgmStream;
    } else {
        stream = lbl_1_bss_B94.jingleStream;
    }
    HuAudSStreamStop(stream);
}

void fn_1_39E4(s16 arg0)
{
    lbl_1_bss_B94.fxOn[arg0] = 1;
    lbl_1_bss_B94.fxId[arg0] = HuAudFXPlay(lbl_1_data_378[arg0]);
}

void fn_1_3A5C(s16 arg0, s16 vol, s16 pan)
{
    lbl_1_bss_B94.fxOn[arg0] = 1;
    lbl_1_bss_B94.fxId[arg0] = HuAudFXPlayVolPan(lbl_1_data_378[arg0], vol, pan);
}

void fn_1_3AE4(s16 arg0, s16 pan)
{
    if (lbl_1_bss_B94.fxOn[arg0] == 1 && HuAudFXStatusGet(lbl_1_bss_B94.fxId[arg0]) == 2) {
        HuAudFXPanning(lbl_1_bss_B94.fxId[arg0], pan);
    }
}

void fn_1_3B74(s16 arg0)
{
    if (lbl_1_bss_B94.fxOn[arg0] == 1
        && (HuAudFXStatusGet(lbl_1_bss_B94.fxId[arg0]) == 2 || HuAudFXStatusGet(lbl_1_bss_B94.fxId[arg0]) == 3)) {
        HuAudFXStop(lbl_1_bss_B94.fxId[arg0]);
    }
}

void fn_1_3C20(s16 arg0)
{
    if (lbl_1_bss_B94.fxOn[arg0] == 1
        && (HuAudFXStatusGet(lbl_1_bss_B94.fxId[arg0]) == 2 || HuAudFXStatusGet(lbl_1_bss_B94.fxId[arg0]) == 3)) {
        HuAudFXStop(lbl_1_bss_B94.fxId[arg0]);
    }
}

void fn_1_3CCC(void)
{
    HuAudAllStop();
}
