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
#include "REL/m551Dll.h"

double sin(double x);
double cos(double x);
void DCStoreRangeNoSync(void *addr, u32 nBytes);

typedef void (*M551VoidFunc)(void);
extern const M551VoidFunc _ctors[];
extern const M551VoidFunc _dtors[];

/* game.c */
s32 fn_1_805C(M551State *state);
s32 fn_1_81F4(void);
s32 fn_1_822C(void);
s32 fn_1_828C(void);
s32 fn_1_89B0(void);
s32 fn_1_8CBC(void);
s32 fn_1_8D20(void);
s16 fn_1_8FF4(void);
s32 fn_1_9024(void);

/* stage.c */
void fn_1_169F4(M551State *state, M551StageModel *models, s32 *dataNums);
void fn_1_16C54(s16 arg0, s32 *arg1);
void fn_1_16C74(s16 arg0, s16 *arg1);
void fn_1_17264(OMOBJ *obj);

void ObjectSetup(void);
void fn_1_2C0(s16 mode, s16 frameNo);
void fn_1_2F4(void);
void fn_1_320(void);
void fn_1_34C(void);
void fn_1_3FC(void);
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
void fn_1_3D38(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);
s16 fn_1_3B58(M551PtclWork *arg0, s16 which, f32 x, f32 y, f32 z, f32 scale);

extern s16 lbl_1_bss_1799C[2];
extern void *lbl_1_bss_17998;

/* unit-owned .bss, declared in reverse address order so the offsets match the original */
s32 gap_17994;
OMOBJMAN *lbl_1_bss_17990;
M551State lbl_1_bss_176A8;
M551StageModel lbl_1_bss_16D94[83];
s16 lbl_1_bss_16D5C[2][14];
M551Cam *lbl_1_bss_38;
M551Light *lbl_1_bss_34;
M551Shadow *lbl_1_bss_30;
M551_WORK lbl_1_bss_0;

MGSEQ_PARAM lbl_1_data_0 = {
    0, 0,
    (MGSEQ_FUNC)fn_1_2C0, (MGSEQ_FUNC)fn_1_2F4, (MGSEQ_FUNC)fn_1_320,
    (MGSEQ_FUNC)fn_1_34C, (MGSEQ_FUNC)fn_1_378, (MGSEQ_FUNC)fn_1_3A4,
    (MGSEQ_FUNC)fn_1_3D0, (MGSEQ_FUNC)fn_1_3FC, (MGSEQ_FUNC)fn_1_41C,
};
s32 lbl_1_data_28[13][14] = {
    { 0x8C0000, 0x8C0002, 0x8C0003, 0x8C0004, 0x8C0009, 0x8C000A, 0x8C001B, 0x8C0024, 0x8C002B, 0x8C002F, 0x8C0031, 0x8C0039, 0x8C0006, 0x8C0007 },
    { 0x330000, 0x330002, 0x330003, 0x330004, 0x330009, 0x33000A, 0x33001B, 0x330024, 0x33002B, 0x33002F, 0x330031, 0x330039, 0x330006, 0x330007 },
    { 0xB70000, 0xB70002, 0xB70003, 0xB70004, 0xB70009, 0xB7000A, 0xB7001B, 0xB70024, 0xB7002B, 0xB7002F, 0xB70031, 0xB70039, 0xB70006, 0xB70007 },
    { 0xED0000, 0xED0002, 0xED0003, 0xED0004, 0xED0009, 0xED000A, 0xED001B, 0xED0024, 0xED002B, 0xED002F, 0xED0031, 0xED0039, 0xED0006, 0xED0007 },
    { 0xE60000, 0xE60002, 0xE60003, 0xE60004, 0xE60009, 0xE6000A, 0xE6001B, 0xE60024, 0xE6002B, 0xE6002F, 0xE60031, 0xE60039, 0xE60006, 0xE60007 },
    { 0x170000, 0x170002, 0x170003, 0x170004, 0x170009, 0x17000A, 0x17001B, 0x170024, 0x17002B, 0x17002F, 0x170031, 0x170039, 0x170006, 0x170007 },
    { 0xE00000, 0xE00002, 0xE00003, 0xE00004, 0xE00009, 0xE0000A, 0xE0001B, 0xE00024, 0xE0002B, 0xE0002F, 0xE00031, 0xE00039, 0xE00006, 0xE00007 },
    { 0x2D0000, 0x2D0002, 0x2D0003, 0x2D0004, 0x2D0009, 0x2D000A, 0x2D001B, 0x2D0024, 0x2D002B, 0x2D002F, 0x2D0031, 0x2D0039, 0x2D0006, 0x2D0007 },
    { 0xC90000, 0xC90002, 0xC90003, 0xC90004, 0xC90009, 0xC9000A, 0xC9001B, 0xC90024, 0xC9002B, 0xC9002F, 0xC90031, 0xC90039, 0xC90006, 0xC90007 },
    { 0xB00000, 0xB00002, 0xB00003, 0xB00004, 0xB00009, 0xB0000A, 0xB0001B, 0xB00024, 0xB0002B, 0xB0002F, 0xB00031, 0xB00039, 0xB00006, 0xB00007 },
    { 0xB00000, 0xB00002, 0xB00003, 0xB00004, 0xB00009, 0xB0000A, 0xB0001B, 0xB00024, 0xB0002B, 0xB0002F, 0xB00031, 0xB00039, 0xB00006, 0xB00007 },
    { 0xB00000, 0xB00002, 0xB00003, 0xB00004, 0xB00009, 0xB0000A, 0xB0001B, 0xB00024, 0xB0002B, 0xB0002F, 0xB00031, 0xB00039, 0xB00006, 0xB00007 },
    { 0xB00000, 0xB00002, 0xB00003, 0xB00004, 0xB00009, 0xB0000A, 0xB0001B, 0xB00024, 0xB0002B, 0xB0002F, 0xB00031, 0xB00039, 0xB00006, 0xB00007 },
};
s32 lbl_1_data_300[26] = {
    0x6D0000, 0x6D0001, 0x6D0002, 0x6D0003, 0x6D0004, 0x6D0005, 0x6D0006, 0x6D0007, 0x6D0008, 0x6D0009, 0x6D000A, 0x6D000B, 0x6D000C, 0x6D000D, 0x6D000E, 0x6D000F, 0x6D0010, 0x6D0011, 0x6D0012, 0x6D0013, 0x6D0014, 0x6D0015, 0x6D0016, 0x6D0017, 0x6D0018, 0x0,
};
M551CamInit lbl_1_data_368[4] = {
    { { 0.0f, 215.0f, -325.0f }, -35.0f, 0.0f, 0.0f, 119.0f, 1.33f, 0.0f, 0.0f, 640.0f, 480.0f, 45.0f },
    { { 0.0f, 208.0f, 20.0f }, -35.0f, 0.0f, 0.0f, 1415.0f, 1.33f, 0.0f, 0.0f, 640.0f, 480.0f, 45.0f },
    { { 0.0f, 208.0f, 20.0f }, -40.0f, 0.0f, 0.0f, 1500.0f, 1.33f, 0.0f, 0.0f, 640.0f, 480.0f, 45.0f },
    { { 0.0f, 0.0f, 0.0f }, 0.0f, 0.0f, 500.0f, 500.0f, 1.33f, 0.0f, 0.0f, 640.0f, 480.0f, 45.0f },
};
s16 lbl_1_data_438[16] = {
    0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000, 0x8000,
};
char lbl_1_data_458[0x2E] = "OBJ X:%.1f Y:%.1f Z:%.1f RX%.1f RY%.1f RZ%.1f";
char lbl_1_data_486[8] = "OBJW %d";
char lbl_1_data_48E[0x25] = "CAMPOS X:%.1f Y:%.1f Z:%.1f ZOOM%.1f";
char lbl_1_data_4B3[0x1D] = "CAMROT X:%.1f Y:%.1f Z:%.1f";
M551PtclWork lbl_1_data_4D0 = {
    0, 0,
    { 0xFF, 0xFF, 0xFF, 0xFF }, { 0x00, 0x00, 0x00, 0xFF },
    0.0f, 2.0f, 1.0f,
    0.85f, 0.85f, 0.85f,
    0.2f,
    0.0f, 1.0f, -3.0f, 0.02f,
};

int _prolog(void)
{
    const M551VoidFunc *ctors = _ctors;

    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const M551VoidFunc *dtors = _dtors;

    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

void ObjectSetup(void)
{
    s16 i;

    lbl_1_bss_17990 = MgActorObjectSetup();
    fn_1_169F4(&lbl_1_bss_176A8, lbl_1_bss_16D94, lbl_1_data_300);
    lbl_1_bss_38 = HuMemDirectMallocNum(HEAP_HEAP, 0x148, 0x10000000);
    memset(lbl_1_bss_38, 0, 0x148);
    lbl_1_bss_34 = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M551Light), 0x10000000);
    memset(lbl_1_bss_34, 0, sizeof(M551Light));
    lbl_1_bss_30 = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M551Shadow), 0x10000000);
    memset(lbl_1_bss_30, 0, sizeof(M551Shadow));
    fn_1_44C(0);
    fn_1_E94();
    fn_1_139C();
    fn_1_16C54(0, lbl_1_data_28[lbl_1_bss_176A8.chars[0].charNo]);
    fn_1_16C54(1, lbl_1_data_28[lbl_1_bss_176A8.chars[1].charNo]);
    for (i = 0; i < 2; i++) {
        fn_1_16C74(i, lbl_1_bss_16D5C[i]);
    }
    omAddObjEx(lbl_1_bss_17990, 0x7FDA, 0, 0, -1, (OMOBJFUNC)fn_1_17264);
    omAddObjEx(lbl_1_bss_17990, 0x7FDA, 0, 0, -1, (OMOBJFUNC)fn_1_448);
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
    if (fn_1_5A6C(&lbl_1_bss_176A8) != 0) {
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
    lbl_1_bss_38[arg0].idx = arg0;
    lbl_1_bss_38[arg0].center.x = lbl_1_data_368[arg0].center.x;
    lbl_1_bss_38[arg0].center.y = lbl_1_data_368[arg0].center.y;
    lbl_1_bss_38[arg0].center.z = lbl_1_data_368[arg0].center.z;
    lbl_1_bss_38[arg0].rotX = lbl_1_data_368[arg0].rotX;
    lbl_1_bss_38[arg0].rotY = lbl_1_data_368[arg0].rotY;
    lbl_1_bss_38[arg0].rotZ = lbl_1_data_368[arg0].rotZ;
    lbl_1_bss_38[arg0].zoom = lbl_1_data_368[arg0].zoom;
    lbl_1_bss_38[arg0].aspect = lbl_1_data_368[arg0].aspect;
    lbl_1_bss_38[arg0].vpX = lbl_1_data_368[arg0].vpX;
    lbl_1_bss_38[arg0].vpY = lbl_1_data_368[arg0].vpY;
    lbl_1_bss_38[arg0].vpW = lbl_1_data_368[arg0].vpW;
    lbl_1_bss_38[arg0].vpH = lbl_1_data_368[arg0].vpH;
    lbl_1_bss_38[arg0].fov = lbl_1_data_368[arg0].fov;
    Hu3DCameraCreate(lbl_1_data_438[arg0]);
    Hu3DCameraViewportSet(lbl_1_data_438[arg0], lbl_1_bss_38[arg0].vpX, lbl_1_bss_38[arg0].vpY,
                          lbl_1_bss_38[arg0].vpW, lbl_1_bss_38[arg0].vpH, 0.0f, 1.0f);
    Hu3DCameraPerspectiveSet(lbl_1_data_438[arg0], lbl_1_bss_38[arg0].fov, 20.0f, 30000.0f, lbl_1_bss_38[arg0].aspect);
    fn_1_91C(arg0);
    Center.x = lbl_1_data_368[0].center.x;
    Center.y = lbl_1_data_368[0].center.y;
    Center.z = lbl_1_data_368[0].center.z;
    CRot.x = lbl_1_data_368[0].rotX;
    CRot.y = lbl_1_data_368[0].rotY;
    CRot.z = lbl_1_data_368[0].rotZ;
    CZoom = lbl_1_data_368[arg0].zoom;
}

void fn_1_91C(s16 arg0)
{
    f64 temp;

    temp = sin(3.141592653589793 * lbl_1_bss_38[arg0].rotY / 180.0);
    lbl_1_bss_38[arg0].pos.x = lbl_1_bss_38[arg0].center.x
        + lbl_1_bss_38[arg0].zoom * (cos(3.141592653589793 * lbl_1_bss_38[arg0].rotX / 180.0) * temp);
    lbl_1_bss_38[arg0].pos.y = lbl_1_bss_38[arg0].center.y
        + lbl_1_bss_38[arg0].zoom * -sin(3.141592653589793 * lbl_1_bss_38[arg0].rotX / 180.0);
    temp = cos(3.141592653589793 * lbl_1_bss_38[arg0].rotY / 180.0);
    lbl_1_bss_38[arg0].pos.z = lbl_1_bss_38[arg0].center.z
        + lbl_1_bss_38[arg0].zoom * (cos(3.141592653589793 * lbl_1_bss_38[arg0].rotX / 180.0) * temp);
    temp = sin(3.141592653589793 * lbl_1_bss_38[arg0].rotY / 180.0);
    lbl_1_bss_38[arg0].up.x = sin(3.141592653589793 * lbl_1_bss_38[arg0].rotX / 180.0) * temp;
    lbl_1_bss_38[arg0].up.y = cos(3.141592653589793 * lbl_1_bss_38[arg0].rotX / 180.0);
    temp = cos(3.141592653589793 * lbl_1_bss_38[arg0].rotY / 180.0);
    lbl_1_bss_38[arg0].up.z = sin(3.141592653589793 * lbl_1_bss_38[arg0].rotX / 180.0) * temp;
    Hu3DCameraPosSet(lbl_1_data_438[arg0], lbl_1_bss_38[arg0].pos.x, lbl_1_bss_38[arg0].pos.y, lbl_1_bss_38[arg0].pos.z,
                     lbl_1_bss_38[arg0].up.x, lbl_1_bss_38[arg0].up.y, lbl_1_bss_38[arg0].up.z,
                     lbl_1_bss_38[arg0].center.x, lbl_1_bss_38[arg0].center.y, lbl_1_bss_38[arg0].center.z);
}

void fn_1_E94(void)
{
    Hu3DLighInit();
    lbl_1_bss_34->pos.x = 0.0f;
    lbl_1_bss_34->pos.y = 0.0f;
    lbl_1_bss_34->pos.z = 0.0f;
    lbl_1_bss_34->target.x = 0.0f;
    lbl_1_bss_34->target.y = 0.0f;
    lbl_1_bss_34->target.z = 0.0f;
    lbl_1_bss_34->rotX = -23.0f;
    lbl_1_bss_34->rotY = 0.0f;
    lbl_1_bss_34->rotZ = 0.0f;
    lbl_1_bss_34->dist = 1000.0f;
    lbl_1_bss_34->color.r = 0xFF;
    lbl_1_bss_34->color.g = 0xFF;
    lbl_1_bss_34->color.b = 0xFF;
    lbl_1_bss_34->color.a = 0xFF;
    PSVECSubtract((Vec *)&lbl_1_bss_34->target, (Vec *)&lbl_1_bss_34->pos, (Vec *)&lbl_1_bss_34->dir);
    lbl_1_bss_34->id = Hu3DGLightCreateV((HuVecF *)&lbl_1_bss_34->pos, (HuVecF *)&lbl_1_bss_34->dir, &lbl_1_bss_34->color);
    fn_1_10C4();
    Hu3DGLightStaticSet(lbl_1_bss_34->id, 1);
    Hu3DGLightInfinitytSet(lbl_1_bss_34->id);
}

void fn_1_10C4(void)
{
    f64 temp;

    temp = sin(3.141592653589793 * lbl_1_bss_34->rotY / 180.0);
    lbl_1_bss_34->pos.x = lbl_1_bss_34->target.x
        + lbl_1_bss_34->dist * (cos(3.141592653589793 * lbl_1_bss_34->rotX / 180.0) * temp);
    lbl_1_bss_34->pos.y = lbl_1_bss_34->target.y
        + lbl_1_bss_34->dist * -sin(3.141592653589793 * lbl_1_bss_34->rotX / 180.0);
    temp = cos(3.141592653589793 * lbl_1_bss_34->rotY / 180.0);
    lbl_1_bss_34->pos.z = lbl_1_bss_34->target.z
        + lbl_1_bss_34->dist * (cos(3.141592653589793 * lbl_1_bss_34->rotX / 180.0) * temp);
    PSVECSubtract((Vec *)&lbl_1_bss_34->target, (Vec *)&lbl_1_bss_34->pos, (Vec *)&lbl_1_bss_34->dir);
    PSVECNormalize((Vec *)&lbl_1_bss_34->dir, (Vec *)&lbl_1_bss_34->dir);
    Hu3DGLightPosSetV(lbl_1_bss_34->id, (HuVecF *)&lbl_1_bss_34->pos, (HuVecF *)&lbl_1_bss_34->dir);
    Hu3DGLightColorSet(lbl_1_bss_34->id, lbl_1_bss_34->color.r, lbl_1_bss_34->color.g, lbl_1_bss_34->color.b,
                       lbl_1_bss_34->color.a);
}

void fn_1_139C(void)
{
    Hu3DShadowCreate(30.0f, 20.0f, 30000.0f);
    lbl_1_bss_30->rotX = -90.0f;
    lbl_1_bss_30->rotY = 0.0f;
    lbl_1_bss_30->rotZ = 0.0f;
    lbl_1_bss_30->target.x = 0.0f;
    lbl_1_bss_30->target.y = 0.0f;
    lbl_1_bss_30->target.z = 0.0f;
    lbl_1_bss_30->tpLvl = 0.5f;
    lbl_1_bss_30->dist = 3500.0f;
    fn_1_14C4();
}

void fn_1_14C4(void)
{
    f64 temp;

    temp = sin(3.141592653589793 * lbl_1_bss_30->rotY / 180.0);
    lbl_1_bss_30->pos.x = lbl_1_bss_30->target.x
        + lbl_1_bss_30->dist * (cos(3.141592653589793 * lbl_1_bss_30->rotX / 180.0) * temp);
    lbl_1_bss_30->pos.y = lbl_1_bss_30->target.y
        + lbl_1_bss_30->dist * -sin(3.141592653589793 * lbl_1_bss_30->rotX / 180.0);
    temp = cos(3.141592653589793 * lbl_1_bss_30->rotY / 180.0);
    lbl_1_bss_30->pos.z = lbl_1_bss_30->target.z
        + lbl_1_bss_30->dist * (cos(3.141592653589793 * lbl_1_bss_30->rotX / 180.0) * temp);
    temp = sin(3.141592653589793 * lbl_1_bss_30->rotY / 180.0);
    lbl_1_bss_30->up.x = sin(3.141592653589793 * lbl_1_bss_30->rotX / 180.0) * temp;
    lbl_1_bss_30->up.y = cos(3.141592653589793 * lbl_1_bss_30->rotX / 180.0);
    temp = cos(3.141592653589793 * lbl_1_bss_30->rotY / 180.0);
    lbl_1_bss_30->up.z = sin(3.141592653589793 * lbl_1_bss_30->rotX / 180.0) * temp;
    Hu3DShadowTPLvlSet(lbl_1_bss_30->tpLvl);
    Hu3DShadowPosSet((HuVecF *)&lbl_1_bss_30->pos, (HuVecF *)&lbl_1_bss_30->up, (HuVecF *)&lbl_1_bss_30->target);
}

// NONMATCHING: dead fmr copies only
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

// NONMATCHING: dead fmr copies only
s32 fn_1_1970(s32 *arg0, s32 steps, s32 start, s32 target)
{
    s32 ret;
    s16 delta;
    s16 distA;
    s32 distB;

    ret = 0;
    delta = (target - start) / steps;
    *arg0 += delta;
    if (delta != 0) {
        ret = (s16)((target - *arg0) / delta);
    }
    ret++;
    if ((start - target) > 0) {
        distA = start - target;
    } else {
        distA = start - (target * -1);
    }
    if ((start - *arg0) > 0) {
        distB = start - *arg0;
    } else {
        distB = start - (*arg0 * -1);
    }
    if (distA <= distB) {
        *arg0 = target;
        ret = 0;
    }
    return ret;
}

MGTIMER *fn_1_1A44(s32 type, s32 maxSec, s32 endSec, f32 x, f32 y)
{
    MGTIMER *timer;

    timer = MgTimerCreate(type);
    MgTimerParamSet(timer, maxSec * 60, endSec * 60, 0);
    MgTimerPosSet(timer, x, y);
    return timer;
}

void fn_1_1AB8(MGTIMER *timer)
{
    MgTimerModeOnSet(timer, 1);
}

BOOL fn_1_1AE4(MGTIMER *timer)
{
    return MgTimerDoneCheck(timer);
}

void fn_1_1B0C(MGTIMER *timer)
{
    MgTimerDispOff(timer);
    MgTimerKill(timer);
}

// NONMATCHING: prologue/epilogue register save form only
s32 fn_1_1B44(M551State *state, s16 pad, s16 idx)
{
    M551StageModel *model;

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
    print8(0x19, 0x88, 1.5f, lbl_1_data_458, model->x, model->y, model->z, model->rotX, model->rotY, model->rotZ);
    print8(0x19, 0x9A, 1.5f, lbl_1_data_486, idx);
    return 0;
}

void fn_1_1E98(void)
{
}

// NONMATCHING: stick reads typed u8 here, s8 in the original (debug-only, never called)
void fn_1_1E9C(s16 pad)
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
    print8(0x19, 0x1E, 1.5f, lbl_1_data_48E, Center.x, Center.y, Center.z, CZoom);
    print8(0x19, 0x30, 1.5f, lbl_1_data_4B3, CRot.x, CRot.y, CRot.z);
}


void fn_1_2354(void)
{
    HU3DPARTICLE *particle;
    ANIMDATA *anim;
    HU3DPARTICLEDATA *data;
    s16 i;

    anim = HuSprAnimRead(HuDataSelHeapReadNum(0x6D0019, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_17998 = HuMemDirectMallocNum(HEAP_HEAP, 150 * sizeof(M551PtclWork), HU_MEMNUM_OVL);
    lbl_1_bss_1799C[0] = Hu3DParticleCreate(anim, 150);
    Hu3DParticleHookSet(lbl_1_bss_1799C[0], fn_1_3D38);
    Hu3DParticleBlendModeSet(lbl_1_bss_1799C[0], 1);
    Hu3DModelLayerSet(lbl_1_bss_1799C[0], 4);
    Hu3DModelCameraSet(lbl_1_bss_1799C[0], 1);
    Hu3DModelAttrSet(lbl_1_bss_1799C[0], 1);
    Hu3DModelAttrReset(lbl_1_bss_1799C[0], 1);
    particle = (HU3DPARTICLE *)Hu3DData[lbl_1_bss_1799C[0]].hookData;
    particle->emitCnt = 0;
    particle->count = 0;
    particle->work = lbl_1_bss_17998;
    data = particle->data;
    for (i = 0; i < particle->maxCnt; i++, data++) {
        data->scale = 0.0f;
    }
    anim = HuSprAnimRead(HuDataSelHeapReadNum(0x6D001A, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_17998 = HuMemDirectMallocNum(HEAP_HEAP, 150 * sizeof(M551PtclWork), HU_MEMNUM_OVL);
    lbl_1_bss_1799C[1] = Hu3DParticleCreate(anim, 150);
    Hu3DParticleHookSet(lbl_1_bss_1799C[1], fn_1_3D38);
    Hu3DParticleBlendModeSet(lbl_1_bss_1799C[1], 1);
    Hu3DModelLayerSet(lbl_1_bss_1799C[1], 4);
    Hu3DModelCameraSet(lbl_1_bss_1799C[1], 1);
    Hu3DModelAttrSet(lbl_1_bss_1799C[1], 1);
    Hu3DModelAttrReset(lbl_1_bss_1799C[1], 1);
    particle = (HU3DPARTICLE *)Hu3DData[lbl_1_bss_1799C[1]].hookData;
    particle->emitCnt = 0;
    particle->count = 0;
    particle->work = lbl_1_bss_17998;
    data = particle->data;
    for (i = 0; i < particle->maxCnt; i++, data++) {
        data->scale = 0.0f;
    }
}

void fn_1_2610(M551Vec3 *arg0)
{
    M551Vec3 sp;
    f32 speed;
    f32 scale;
    s16 t;
    s16 i;
    s16 unused;

    unused = 0x10;
    speed = 0.8f;
    scale = 1.0f;
    for (i = 0; i < 12; i++) {
        sp.x = arg0->x + scale * (20.0 * sin(3.141592653589793 * (30.0f * i) / 180.0));
        sp.y = arg0->y + 10.0f * scale;
        sp.z = arg0->z + scale * (20.0 * cos(3.141592653589793 * (30.0f * i) / 180.0));
        speed = scale * (6.0f + frandmod(6));
        lbl_1_data_4D0.velX = speed * sin(3.141592653589793 * (30.0f * i) / 180.0);
        lbl_1_data_4D0.velY = 0.0f;
        lbl_1_data_4D0.velZ = speed * cos(3.141592653589793 * (30.0f * i) / 180.0);
        t = -frandmod(6) - 7;
        lbl_1_data_4D0.alphaStep = t;
        lbl_1_data_4D0.accelY = 0.01 * frandmod(20);
        speed = scale * (20.0 + frandmod(20));
        fn_1_3B58(&lbl_1_data_4D0, 1, sp.x, sp.y, sp.z, speed);
    }
}

void fn_1_29DC(M551Vec3 *arg0)
{
    M551Vec3 sp;
    f32 spd;
    f32 rad;
    f32 step;
    s16 num;
    s16 t;
    s16 i;

    num = 0x24;
    step = 360.0f / num;
    for (i = 0; i < num; i++) {
        rad = 120.0f + frandmod(20);
        sp.x = arg0->x + sin(3.141592653589793 * (step * i) / 180.0) * rad;
        sp.y = 10.0f + arg0->y;
        rad = 120.0f + frandmod(20);
        sp.z = arg0->z + cos(3.141592653589793 * (step * i) / 180.0) * rad;
        spd = 3.0f + frandmod(5);
        lbl_1_data_4D0.velX = spd * sin(3.141592653589793 * (step * i) / 180.0);
        lbl_1_data_4D0.velY = frandmod(5);
        lbl_1_data_4D0.velZ = spd * cos(3.141592653589793 * (step * i) / 180.0);
        t = -frandmod(6) - 7;
        lbl_1_data_4D0.alphaStep = t;
        lbl_1_data_4D0.accelY = 0.01 * frandmod(20);
        fn_1_3B58(&lbl_1_data_4D0, 1, sp.x, sp.y, sp.z, 30.0f + frandmod(30));
    }
    for (i = 0; i < num; i++) {
        rad = 80.0f + frandmod(50);
        sp.x = arg0->x + sin(3.141592653589793 * ((f32)(num >> 1) + step * i) / 180.0) * rad;
        sp.y = 10.0f + arg0->y;
        rad = 80.0f + frandmod(50);
        sp.z = arg0->z + cos(3.141592653589793 * ((f32)(num >> 1) + step * i) / 180.0) * rad;
        spd = 3.0f + frandmod(5);
        lbl_1_data_4D0.velX = spd * sin(3.141592653589793 * ((f32)(num >> 1) + step * i) / 180.0);
        lbl_1_data_4D0.velY = frandmod(5);
        lbl_1_data_4D0.velZ = spd * cos(3.141592653589793 * ((f32)(num >> 1) + step * i) / 180.0);
        t = -frandmod(6) - 7;
        lbl_1_data_4D0.alphaStep = t;
        lbl_1_data_4D0.accelY = 0.01 * frandmod(20);
        fn_1_3B58(&lbl_1_data_4D0, 1, sp.x, sp.y, sp.z, 30.0f + frandmod(30));
    }
}

void fn_1_3270(M551Vec3 *arg0)
{
    M551Vec3 sp;
    f32 size;
    s16 n1;
    s16 n2;
    s16 n3;
    s16 n4;
    s16 t;
    s16 i;

    n1 = 0x23;
    n2 = 0x14;
    n3 = 10;
    n4 = 10;

    for (i = 0; i < n1; i++) {
        sp.x = arg0->x + (frandmod(220) - 110.0f);
        sp.y = arg0->y + frandmod(200);
        sp.z = 130.0f + arg0->z;
        lbl_1_data_4D0.velX = frandmod(4) - 2.0f;
        lbl_1_data_4D0.velY = 5.0f + frandmod(10);
        lbl_1_data_4D0.velZ = 0.0f;
        t = -frandmod(6) - 5;
        lbl_1_data_4D0.alphaStep = t;
        lbl_1_data_4D0.accelY = 0.01f * frandmod(20);
        size = 40.0f + frandmod(40);
        fn_1_3B58(&lbl_1_data_4D0, 0, sp.x, sp.y, sp.z, size);
    }
    for (i = 0; i < n2; i++) {
        sp.x = arg0->x + (frandmod(220) - 110.0f);
        sp.y = 200.0f + arg0->y;
        sp.z = arg0->z + frandmod(100);
        lbl_1_data_4D0.velX = frandmod(4) - 2.0f;
        lbl_1_data_4D0.velY = 5.0f + frandmod(10);
        lbl_1_data_4D0.velZ = 0.0f;
        t = -frandmod(6) - 5;
        lbl_1_data_4D0.alphaStep = t;
        lbl_1_data_4D0.accelY = 0.01f * frandmod(20);
        size = 40.0f + frandmod(40);
        fn_1_3B58(&lbl_1_data_4D0, 0, sp.x, sp.y, sp.z, size);
    }
    for (i = 0; i < n3; i++) {
        sp.x = 100.0f + arg0->x;
        sp.y = arg0->y + frandmod(200);
        sp.z = arg0->z + frandmod(100);
        lbl_1_data_4D0.velX = frandmod(4) - 2.0f;
        lbl_1_data_4D0.velY = 5.0f + frandmod(10);
        lbl_1_data_4D0.velZ = 0.0f;
        t = -frandmod(6) - 5;
        lbl_1_data_4D0.alphaStep = t;
        lbl_1_data_4D0.accelY = 0.01f * frandmod(20);
        size = 40.0f + frandmod(40);
        fn_1_3B58(&lbl_1_data_4D0, 0, sp.x, sp.y, sp.z, size);
    }
    for (i = 0; i < n4; i++) {
        sp.x = arg0->x - 100.0f;
        sp.y = arg0->y + frandmod(200);
        sp.z = arg0->z + frandmod(100);
        lbl_1_data_4D0.velX = frandmod(4) - 2.0f;
        lbl_1_data_4D0.velY = 5.0f + frandmod(10);
        lbl_1_data_4D0.velZ = 0.0f;
        t = -frandmod(6) - 5;
        lbl_1_data_4D0.alphaStep = t;
        lbl_1_data_4D0.accelY = 0.01f * frandmod(20);
        size = 40.0f + frandmod(40);
        fn_1_3B58(&lbl_1_data_4D0, 0, sp.x, sp.y, sp.z, size);
    }
}

s16 fn_1_3B58(M551PtclWork *arg0, s16 which, f32 x, f32 y, f32 z, f32 scale)
{
    HU3DPARTICLEDATA *data;
    HU3DPARTICLE *particle;
    s16 i;
    s16 ret;
    M551PtclWork *work;

    ret = -1;
    particle = (HU3DPARTICLE *)Hu3DData[lbl_1_bss_1799C[which]].hookData;
    work = (M551PtclWork *)particle->work;
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
        data->parManId = -1;
        particle->emitCnt = i;
        ret = i;
    }
    return ret;
}

void fn_1_3D38(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx)
{
    M551PtclWork *work;
    HU3DPARTICLEDATA *data;
    s16 i;
    s16 v;
    s16 cnt;

    work = (M551PtclWork *)particleP->work;
    data = particleP->data;
    cnt = 0;
    i = 0;
    data->time = 0;
    for (; i < particleP->maxCnt; i++, data++) {
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
            if (0.0f != data->scale) {
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
