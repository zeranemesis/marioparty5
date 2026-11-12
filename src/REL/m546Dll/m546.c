#include "types.h"

#include "string.h"
#ifdef TARGET_PC
#include <stdio.h>
#endif
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
#include "REL/m546Dll.h"

#ifndef TARGET_PC
/* the original calls sprintf without the variadic cr1 clear */
int sprintf(char *s, const char *fmt, char *arg);
#endif
double sin(double x);
double cos(double x);
void DCStoreRangeNoSync(void *addr, u32 nBytes);

typedef void (*M546VoidFunc)(void);
extern const M546VoidFunc _ctors[];
extern const M546VoidFunc _dtors[];

/* game.c */
s32 fn_1_58D0(M546State *state);
s32 fn_1_6A90(void);
s32 fn_1_6AC4(void);
s32 fn_1_6B2C(void);
s32 fn_1_6CD4(void);
s32 fn_1_6D14(void);
s32 fn_1_6D4C(void);
s32 fn_1_6E94(void);
s32 fn_1_6EC4(void);

/* stage.c */
void fn_1_B15C(M546State *state, M546StageModel *models, s32 *dataNums);
void fn_1_B3A8(s16 arg0);
void fn_1_B654(s32 *arg0, s32 *arg1);
void fn_1_B678(s16 *arg0, s16 *arg1);
void fn_1_B69C(void);
void fn_1_B9B4(void);
void fn_1_BDCC(void);
void fn_1_BEF8(void);
void fn_1_C250(void);

void ObjectSetup(void);
void fn_1_2F0(void);
void fn_1_324(void);
void fn_1_350(void);
void fn_1_370(void);
void fn_1_39C(void);
void fn_1_3C0(void);
void fn_1_3EC(void);
void fn_1_418(void);
void fn_1_438(void);
void fn_1_464(void);
void fn_1_468(s16 arg0);
void fn_1_884(s16 arg0);
void fn_1_DFC(void);
void fn_1_102C(void);
void fn_1_1304(void);
void fn_1_1434(void);
s16 fn_1_2448(M546PtclWork *arg0, f32 x, f32 y, f32 z, f32 scale);
void fn_1_261C(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);
void fn_1_3BE0(HU3DMODEL *modelP, Mtx *mtx);
void fn_1_403C(void);
void fn_1_4BA8(HU3DMODEL *modelP, Mtx *mtx);
void fn_1_5190(void);

/* unit-owned .bss, declared in reverse address order so the offsets match the original */
s16 gap_05_00015286_bss;
s16 lbl_1_bss_15284;
M546PtclWork *lbl_1_bss_15280;
s32 lbl_1_bss_1527C_pad; /* original symbol lbl_1_bss_15278 is 8 bytes long */
OMOBJMAN *lbl_1_bss_15278;
M546State lbl_1_bss_15028;
M546StageModel lbl_1_bss_14CFC[29];
s16 lbl_1_bss_14CD8[18];
M546Game lbl_1_bss_14C40;
M546Plr lbl_1_bss_14B60[2];
M546Kami lbl_1_bss_14AB8;
M546Kami lbl_1_bss_14A10;
M546Scrap lbl_1_bss_1F0[1000];
M546Scrap lbl_1_bss_4C[5];
M546Cam *lbl_1_bss_48;
M546Light *lbl_1_bss_44;
M546DbgLight lbl_1_bss_4[1];
M546Shadow *lbl_1_bss_0;

/* unit-owned .data, in original address order */
MGSEQ_PARAM lbl_1_data_0 = {
    0, 0,
    (MGSEQ_FUNC)fn_1_2F0, (MGSEQ_FUNC)fn_1_324, (MGSEQ_FUNC)fn_1_350,
    (MGSEQ_FUNC)fn_1_370, (MGSEQ_FUNC)fn_1_39C, (MGSEQ_FUNC)fn_1_3C0,
    (MGSEQ_FUNC)fn_1_3EC, (MGSEQ_FUNC)fn_1_418, (MGSEQ_FUNC)fn_1_438,
};
s32 lbl_1_data_28[13][9] = {
    { 0x8C0000, 0x8C0001, 0x8C0002, 0x8C000D, 0x8C000E, 0x870064, 0x870065, 0x8C0006, 0x8C0007 },
    { 0x330000, 0x330001, 0x330002, 0x33000D, 0x33000E, 0x2E0064, 0x2E0065, 0x330006, 0x330007 },
    { 0xB70000, 0xB70001, 0xB70002, 0xB7000D, 0xB7000E, 0xB20064, 0xB20065, 0xB70006, 0xB70007 },
    { 0xED0000, 0xED0001, 0xED0002, 0xED000D, 0xED000E, 0xE80064, 0xE80065, 0xED0006, 0xED0007 },
    { 0xE60000, 0xE60001, 0xE60002, 0xE6000D, 0xE6000E, 0xE10064, 0xE10065, 0xE60006, 0xE60007 },
    { 0x170000, 0x170001, 0x170002, 0x17000D, 0x17000E, 0x120064, 0x120065, 0x170006, 0x170007 },
    { 0xE00000, 0xE00001, 0xE00002, 0xE0000D, 0xE0000E, 0xDB0064, 0xDB0065, 0xE00006, 0xE00007 },
    { 0x2D0000, 0x2D0001, 0x2D0002, 0x2D000D, 0x2D000E, 0x280064, 0x280065, 0x2D0006, 0x2D0007 },
    { 0xC90000, 0xC90001, 0xC90002, 0xC9000D, 0xC9000E, 0xC40064, 0xC40065, 0xC90006, 0xC90007 },
    { 0xB00000, 0xB00001, 0xB00002, 0xB0000D, 0xB0000E, 0x9C0064, 0x9C0065, 0xB00006, 0xB00007 },
    { 0xB00000, 0xB00001, 0xB00002, 0xB0000D, 0xB0000E, 0x9C0064, 0x9C0065, 0xB00006, 0xB00007 },
    { 0xB00000, 0xB00001, 0xB00002, 0xB0000D, 0xB0000E, 0x9C0064, 0x9C0065, 0xB00006, 0xB00007 },
    { 0xB00000, 0xB00001, 0xB00002, 0xB0000D, 0xB0000E, 0x9C0064, 0x9C0065, 0xB00006, 0xB00007 },
};
s32 lbl_1_data_1FC[21] = {
    0x680000, 0x680001, 0x680002, 0x680003, 0x680004, 0x680005, 0x680006, 0x680007, 0x680008, 0x680009, 0x68000A,
    0x68000B, 0x68000C, 0x68000D, 0x68000E, 0x68000F, 0x680010, 0x680011, 0x680012, 0x680013, 0x680014,
};
M546CamInit lbl_1_data_250[4] = {
    { { 34.0f, 250.0f, 0.0f }, -6.0f, 0.0f, 500.0f, 1000.0f, 1.33f, 0.0f, 0.0f, 640.0f, 480.0f, 45.0f },
    { { 0.0f, 0.0f, 0.0f }, 0.0f, 0.0f, 500.0f, 500.0f, 1.33f, 0.0f, 0.0f, 640.0f, 480.0f, 45.0f },
    { { 0.0f, 0.0f, 0.0f }, 0.0f, 0.0f, 500.0f, 500.0f, 1.33f, 0.0f, 0.0f, 640.0f, 480.0f, 45.0f },
    { { 0.0f, 0.0f, 0.0f }, 0.0f, 0.0f, 500.0f, 500.0f, 1.33f, 0.0f, 0.0f, 640.0f, 480.0f, 45.0f },
};
s16 lbl_1_data_320[16] = {
    0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000, 0x8000,
};
M546PtclWork lbl_1_data_340 = {
    0, 0,
    { 0x00, 0x00, 0x00, 0x00 }, { 0xFF, 0xFF, 0xFF, 0xFF },
    0.0f, 2.0f, 1.0f,
    0.85f, 0.85f, 0.85f,
    0.2f,
    0.0f, 1.0f, -3.0f, 0.02f,
};
char lbl_1_data_378[0x30] = "OBJ X:%.1f Y:%.1f Z:%.1f RX%.1f RY%.1f RZ%.1f";
f32 lbl_1_data_3A8[4][3] = {
    { 255.0f, 191.25f, 204.0f },
    { 255.0f, 255.0f, 153.0f },
    { 178.5f, 255.0f, 255.0f },
    { 153.0f, 255.0f, 191.25f },
};
f32 lbl_1_data_3D8[4][3] = {
    { 1.0f, 1.0f, 1.0f },
    { 1.0f, 1.0f, 1.0f },
    { 1.0f, 1.0f, 1.0f },
    { 1.0f, 1.0f, 1.0f },
};
char lbl_1_data_408[3] = "%s";
char lbl_1_data_40B[12] = "m546-kami01";
char lbl_1_data_417[12] = "m546-kami02";
char lbl_1_data_423[12] = "m546-kami03";
char lbl_1_data_42F[12] = "m546-kami04";
char lbl_1_data_43B[3] = "%f";
s16 lbl_1_data_43E[9] = { 0x82E, 0x82F, 0x830, 0x831, 0x832, 0x833, 0x834, 0, 0 };

int _prolog(void)
{
    const M546VoidFunc *ctors = _ctors;

    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const M546VoidFunc *dtors = _dtors;

    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

void ObjectSetup(void)
{
    lbl_1_bss_15278 = omInitObjMan(0x32, 0x2000);
    omGameSysInit(lbl_1_bss_15278);
    fn_1_B15C(&lbl_1_bss_15028, lbl_1_bss_14CFC, lbl_1_data_1FC);
    lbl_1_bss_48 = HuMemDirectMallocNum(HEAP_HEAP, 0x148, 0x10000000);
    memset(lbl_1_bss_48, 0, 0x148);
    lbl_1_bss_44 = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M546Light), 0x10000000);
    memset(lbl_1_bss_44, 0, sizeof(M546Light));
    lbl_1_bss_0 = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M546Shadow), 0x10000000);
    memset(lbl_1_bss_0, 0, sizeof(M546Shadow));
    fn_1_468(0);
    fn_1_DFC();
    fn_1_1304();
    fn_1_B3A8(1);
    fn_1_B654(lbl_1_data_28[lbl_1_bss_15028.chars[0].charNo], lbl_1_data_28[lbl_1_bss_15028.chars[1].charNo]);
    fn_1_B678(lbl_1_bss_14CD8, lbl_1_bss_14CD8 + 9);
    fn_1_B69C();
    fn_1_BDCC();
    omAddObjEx(lbl_1_bss_15278, 0x7FDA, 0, 0, -1, (OMOBJFUNC)fn_1_B9B4);
    omAddObjEx(lbl_1_bss_15278, 0x7FDA, 0, 0, -1, (OMOBJFUNC)fn_1_BEF8);
    omAddObjEx(lbl_1_bss_15278, 0x7FDA, 0, 0, -1, (OMOBJFUNC)fn_1_464);
    omAddObjEx(lbl_1_bss_15278, 0x7FDA, 0, 0, -1, (OMOBJFUNC)fn_1_C250);
    Hu3DBGColorSet(0xBA, 0xF5, 0xFF);
    MgSeqCreate(&lbl_1_data_0);
    MgSeqModeDelaySet(2, 0x190);
    MgSeqModeDelaySet(7, 0xB4);
    MgSeqModeDelaySet(9, 0x3C);
}

void fn_1_2F0(void)
{
    if (fn_1_58D0(&lbl_1_bss_15028) != 0) {
        MgSeqModeNext();
    }
}

void fn_1_324(void)
{
    if (fn_1_6A90() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_350(void)
{
    fn_1_6AC4();
}

void fn_1_370(void)
{
    if (fn_1_6B2C() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_39C(void)
{
    fn_1_6CD4();
    MgSeqModeNext();
}

void fn_1_3C0(void)
{
    if (fn_1_6D14() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_3EC(void)
{
    if (fn_1_6D4C() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_418(void)
{
    fn_1_6E94();
}

void fn_1_438(void)
{
    if (fn_1_6EC4() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_464(void)
{
}

void fn_1_468(s16 arg0)
{
    lbl_1_bss_48[arg0].idx = arg0;
    lbl_1_bss_48[arg0].center.x = lbl_1_data_250[arg0].center.x;
    lbl_1_bss_48[arg0].center.y = lbl_1_data_250[arg0].center.y;
    lbl_1_bss_48[arg0].center.z = lbl_1_data_250[arg0].center.z;
    lbl_1_bss_48[arg0].rotX = lbl_1_data_250[arg0].rotX;
    lbl_1_bss_48[arg0].rotY = lbl_1_data_250[arg0].rotY;
    lbl_1_bss_48[arg0].rotZ = lbl_1_data_250[arg0].rotZ;
    lbl_1_bss_48[arg0].zoom = lbl_1_data_250[arg0].zoom;
    lbl_1_bss_48[arg0].aspect = lbl_1_data_250[arg0].aspect;
    lbl_1_bss_48[arg0].vpX = lbl_1_data_250[arg0].vpX;
    lbl_1_bss_48[arg0].vpY = lbl_1_data_250[arg0].vpY;
    lbl_1_bss_48[arg0].vpW = lbl_1_data_250[arg0].vpW;
    lbl_1_bss_48[arg0].vpH = lbl_1_data_250[arg0].vpH;
    lbl_1_bss_48[arg0].fov = lbl_1_data_250[arg0].fov;
    Hu3DCameraCreate(lbl_1_data_320[arg0]);
    Hu3DCameraViewportSet(lbl_1_data_320[arg0], lbl_1_bss_48[arg0].vpX, lbl_1_bss_48[arg0].vpY,
                          lbl_1_bss_48[arg0].vpW, lbl_1_bss_48[arg0].vpH, 0.0f, 1.0f);
    Hu3DCameraPerspectiveSet(lbl_1_data_320[arg0], lbl_1_bss_48[arg0].fov, 20.0f, 30000.0f, lbl_1_bss_48[arg0].aspect);
    fn_1_884(arg0);
}

void fn_1_884(s16 arg0)
{
    f64 temp;

    temp = sin(3.141592653589793 * lbl_1_bss_48[arg0].rotY / 180.0);
    lbl_1_bss_48[arg0].pos.x = lbl_1_bss_48[arg0].center.x
        + lbl_1_bss_48[arg0].zoom * (cos(3.141592653589793 * lbl_1_bss_48[arg0].rotX / 180.0) * temp);
    lbl_1_bss_48[arg0].pos.y = lbl_1_bss_48[arg0].center.y
        + lbl_1_bss_48[arg0].zoom * -sin(3.141592653589793 * lbl_1_bss_48[arg0].rotX / 180.0);
    temp = cos(3.141592653589793 * lbl_1_bss_48[arg0].rotY / 180.0);
    lbl_1_bss_48[arg0].pos.z = lbl_1_bss_48[arg0].center.z
        + lbl_1_bss_48[arg0].zoom * (cos(3.141592653589793 * lbl_1_bss_48[arg0].rotX / 180.0) * temp);
    temp = sin(3.141592653589793 * lbl_1_bss_48[arg0].rotY / 180.0);
    lbl_1_bss_48[arg0].up.x = sin(3.141592653589793 * lbl_1_bss_48[arg0].rotX / 180.0) * temp;
    lbl_1_bss_48[arg0].up.y = cos(3.141592653589793 * lbl_1_bss_48[arg0].rotX / 180.0);
    temp = cos(3.141592653589793 * lbl_1_bss_48[arg0].rotY / 180.0);
    lbl_1_bss_48[arg0].up.z = sin(3.141592653589793 * lbl_1_bss_48[arg0].rotX / 180.0) * temp;
    Hu3DCameraPosSet(lbl_1_data_320[arg0], lbl_1_bss_48[arg0].pos.x, lbl_1_bss_48[arg0].pos.y, lbl_1_bss_48[arg0].pos.z,
                     lbl_1_bss_48[arg0].up.x, lbl_1_bss_48[arg0].up.y, lbl_1_bss_48[arg0].up.z,
                     lbl_1_bss_48[arg0].center.x, lbl_1_bss_48[arg0].center.y, lbl_1_bss_48[arg0].center.z);
}

void fn_1_DFC(void)
{
    Hu3DLighInit();
    lbl_1_bss_44->pos.x = 0.0f;
    lbl_1_bss_44->pos.y = 0.0f;
    lbl_1_bss_44->pos.z = 0.0f;
    lbl_1_bss_44->target.x = 0.0f;
    lbl_1_bss_44->target.y = 0.0f;
    lbl_1_bss_44->target.z = 0.0f;
    lbl_1_bss_44->rotX = -23.0f;
    lbl_1_bss_44->rotY = 0.0f;
    lbl_1_bss_44->rotZ = 0.0f;
    lbl_1_bss_44->dist = 1000.0f;
    lbl_1_bss_44->color.r = 0xFF;
    lbl_1_bss_44->color.g = 0xFF;
    lbl_1_bss_44->color.b = 0xFF;
    lbl_1_bss_44->color.a = 0xFF;
    PSVECSubtract((Vec *)&lbl_1_bss_44->target, (Vec *)&lbl_1_bss_44->pos, (Vec *)&lbl_1_bss_44->dir);
    lbl_1_bss_44->id = Hu3DGLightCreateV((HuVecF *)&lbl_1_bss_44->pos, (HuVecF *)&lbl_1_bss_44->dir, &lbl_1_bss_44->color);
    fn_1_102C();
    Hu3DGLightStaticSet(lbl_1_bss_44->id, 1);
    Hu3DGLightInfinitytSet(lbl_1_bss_44->id);
}

void fn_1_102C(void)
{
    f64 temp;

    temp = sin(3.141592653589793 * lbl_1_bss_44->rotY / 180.0);
    lbl_1_bss_44->pos.x = lbl_1_bss_44->target.x
        + lbl_1_bss_44->dist * (cos(3.141592653589793 * lbl_1_bss_44->rotX / 180.0) * temp);
    lbl_1_bss_44->pos.y = lbl_1_bss_44->target.y
        + lbl_1_bss_44->dist * -sin(3.141592653589793 * lbl_1_bss_44->rotX / 180.0);
    temp = cos(3.141592653589793 * lbl_1_bss_44->rotY / 180.0);
    lbl_1_bss_44->pos.z = lbl_1_bss_44->target.z
        + lbl_1_bss_44->dist * (cos(3.141592653589793 * lbl_1_bss_44->rotX / 180.0) * temp);
    PSVECSubtract((Vec *)&lbl_1_bss_44->target, (Vec *)&lbl_1_bss_44->pos, (Vec *)&lbl_1_bss_44->dir);
    PSVECNormalize((Vec *)&lbl_1_bss_44->dir, (Vec *)&lbl_1_bss_44->dir);
    Hu3DGLightPosSetV(lbl_1_bss_44->id, (HuVecF *)&lbl_1_bss_44->pos, (HuVecF *)&lbl_1_bss_44->dir);
    Hu3DGLightColorSet(lbl_1_bss_44->id, lbl_1_bss_44->color.r, lbl_1_bss_44->color.g, lbl_1_bss_44->color.b,
                       lbl_1_bss_44->color.a);
}

void fn_1_1304(void)
{
    Hu3DShadowCreate(30.0f, 20.0f, 30000.0f);
    lbl_1_bss_0->rotX = -100.0f;
    lbl_1_bss_0->rotY = 0.0f;
    lbl_1_bss_0->rotZ = 0.0f;
    lbl_1_bss_0->target.x = 0.0f;
    lbl_1_bss_0->target.y = 0.0f;
    lbl_1_bss_0->target.z = -250.0f;
    lbl_1_bss_0->dist = 2500.0f;
    lbl_1_bss_0->tpLvl = 0.3f;
    Hu3DShadowSizeSet(200);
    fn_1_1434();
}

void fn_1_1434(void)
{
    f64 temp;

    temp = sin(3.141592653589793 * lbl_1_bss_0->rotY / 180.0);
    lbl_1_bss_0->pos.x = lbl_1_bss_0->target.x
        + lbl_1_bss_0->dist * (cos(3.141592653589793 * lbl_1_bss_0->rotX / 180.0) * temp);
    lbl_1_bss_0->pos.y = lbl_1_bss_0->target.y
        + lbl_1_bss_0->dist * -sin(3.141592653589793 * lbl_1_bss_0->rotX / 180.0);
    temp = cos(3.141592653589793 * lbl_1_bss_0->rotY / 180.0);
    lbl_1_bss_0->pos.z = 300.0 + lbl_1_bss_0->dist * (cos(3.141592653589793 * lbl_1_bss_0->rotX / 180.0) * temp);
    temp = sin(3.141592653589793 * lbl_1_bss_0->rotY / 180.0);
    lbl_1_bss_0->up.x = sin(3.141592653589793 * lbl_1_bss_0->rotX / 180.0) * temp;
    lbl_1_bss_0->up.y = cos(3.141592653589793 * lbl_1_bss_0->rotX / 180.0);
    temp = cos(3.141592653589793 * lbl_1_bss_0->rotY / 180.0);
    lbl_1_bss_0->up.z = sin(3.141592653589793 * lbl_1_bss_0->rotX / 180.0) * temp;
    Hu3DShadowTPLvlSet(lbl_1_bss_0->tpLvl);
    Hu3DShadowPosSet((HuVecF *)&lbl_1_bss_0->pos, (HuVecF *)&lbl_1_bss_0->up, (HuVecF *)&lbl_1_bss_0->target);
}

s32 fn_1_17BC(f32 *arg0, f32 steps, f32 start, f32 target)
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

MGTIMER *fn_1_18DC(s32 type, s32 maxSec, s32 endSec, f32 x, f32 y)
{
    MGTIMER *timer;

    timer = MgTimerCreate(type);
    MgTimerParamSet(timer, maxSec * 60, endSec * 60, 0);
    MgTimerPosSet(timer, x, y);
    return timer;
}

void fn_1_1950(MGTIMER *timer)
{
    MgTimerModeOnSet(timer, 1);
}

BOOL fn_1_197C(MGTIMER *timer)
{
    return MgTimerDoneCheck(timer);
}

void fn_1_19A4(MGTIMER *timer)
{
    MgTimerDispOff(timer);
    MgTimerKill(timer);
}

void fn_1_19DC(void)
{
    HU3DPARTICLE *particle;
    s16 i;
    HU3DPARTICLEDATA *data;
    ANIMDATA *anim;

    for (i = 0; i < 16; i++) {
        lbl_1_bss_15284 = -1;
    }
    anim = HuSprAnimRead(HuDataSelHeapReadNum(0x680015, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_15280 = HuMemDirectMallocNum(HEAP_HEAP, 0xDC * sizeof(M546PtclWork), HU_MEMNUM_OVL);
    lbl_1_bss_15284 = Hu3DParticleCreate(anim, 0xDC);
    Hu3DParticleHookSet(lbl_1_bss_15284, fn_1_261C);
    Hu3DParticleBlendModeSet(lbl_1_bss_15284, 1);
    Hu3DModelLayerSet(lbl_1_bss_15284, 4);
    Hu3DModelCameraSet(lbl_1_bss_15284, 1);
    Hu3DModelAttrSet(lbl_1_bss_15284, 1);
    Hu3DModelAttrReset(lbl_1_bss_15284, 1);
    particle = (HU3DPARTICLE *)Hu3DData[lbl_1_bss_15284].hookData;
    particle->emitCnt = 0;
    particle->work = lbl_1_bss_15280;
    particle->count = 0;
    data = particle->data;
    for (i = 0; i < particle->maxCnt; i++, data++) {
        data->scale = 0.0f;
    }
}

void fn_1_1B80(M546Vec3 *arg0)
{
    M546Vec3 sp;
    s16 sp8;
    f32 speed;
    f32 mul;
    s16 t;
    s16 i;

    sp8 = 0x10;
    speed = 0.8f;
    mul = 1.0f;
    for (i = 0; i < 12; i++) {
        sp.x = arg0->x + mul * (20.0 * sin(3.141592653589793 * (30.0f * i) / 180.0));
        sp.y = arg0->y + 10.0f * mul;
        sp.z = arg0->z + mul * (20.0 * cos(3.141592653589793 * (30.0f * i) / 180.0));
        speed = mul * (6.0 + frandmod(6));
        lbl_1_data_340.velX = speed * sin(3.141592653589793 * (30.0f * i) / 180.0);
        lbl_1_data_340.velY = 0.0f;
        lbl_1_data_340.velZ = speed * cos(3.141592653589793 * (30.0f * i) / 180.0);
        t = -frandmod(6) - 7;
        lbl_1_data_340.alphaStep = t;
        lbl_1_data_340.accelY = 0.01 * frandmod(20);
        speed = mul * (20.0 + frandmod(20));
        fn_1_2448(&lbl_1_data_340, sp.x, sp.y, sp.z, speed);
    }
}

void fn_1_1F4C(M546Vec3 *arg0)
{
    M546Vec3 ring;
    M546Vec3 sp;
    f32 spC;
    s16 sp8;
    f32 speed;
    f32 mul;
    s16 t;
    s16 i;
    s16 j;

    spC = 1.0f;
    for (j = 0; j < 18; j++) {
        ring.x = arg0->x + 170.0 * sin(3.141592653589793 * (20.0f * j) / 180.0);
        ring.y = arg0->y;
        ring.z = arg0->z + 170.0 * cos(3.141592653589793 * (20.0f * j) / 180.0);
        sp8 = 0x10;
        speed = 0.8f;
        mul = 1.0f;
        for (i = 0; i < 12; i++) {
            sp.x = ring.x + mul * (20.0 * sin(3.141592653589793 * (30.0f * i) / 180.0));
            sp.y = ring.y + 10.0f * mul;
            sp.z = ring.z + mul * (20.0 * cos(3.141592653589793 * (30.0f * i) / 180.0));
            speed = mul * (6.0 + frandmod(6));
            lbl_1_data_340.velX = speed * sin(3.141592653589793 * (30.0f * i) / 180.0);
            lbl_1_data_340.velY = 0.0f;
            lbl_1_data_340.velZ = speed * cos(3.141592653589793 * (30.0f * i) / 180.0);
            t = -frandmod(6) - 7;
            lbl_1_data_340.alphaStep = t;
            lbl_1_data_340.accelY = 0.01 * frandmod(20);
            speed = mul * (20.0 + frandmod(20));
        fn_1_2448(&lbl_1_data_340, sp.x, sp.y, sp.z, speed);
        }
    }
}

s16 fn_1_2448(M546PtclWork *arg0, f32 x, f32 y, f32 z, f32 scale)
{
    HU3DPARTICLEDATA *data;
    HU3DPARTICLE *particle;
    s16 i;
    s16 ret;
    HU3DMODEL *model;
    M546PtclWork *work;

    ret = -1;
    model = &Hu3DData[lbl_1_bss_15284];
    particle = (HU3DPARTICLE *)model->hookData;
    work = (M546PtclWork *)particle->work;
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

void fn_1_261C(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx)
{
    M546PtclWork *work;
    HU3DPARTICLEDATA *data;
    s16 i;
    s16 v;

    work = (M546PtclWork *)particleP->work;
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

s32 fn_1_2A34(M546State *state, s16 pad, s16 idx)
{
    M546StageModel *model;
    s32 ret;

    ret = 0;
    model = &state->models[idx];
    model->x += (f32)(HuPadStkX[pad] / 60);
    model->y += (f32)(HuPadStkY[pad] / 60);
    model->z += (f32)(HuPadTrigL[pad] / 60);
    model->z -= (f32)(HuPadTrigR[pad] / 60);
    model->rotX += (f32)(HuPadSubStkX[pad] / 50);
    model->rotY += (f32)(HuPadSubStkY[pad] / 50);
    if ((HuPadBtnRep[pad] & 0x100) != 0) {
        model->rotZ += 1.0f;
    }
    if ((HuPadBtnRep[pad] & 0x200) != 0) {
        model->rotZ -= 1.0f;
    }
    fontcolor = 0;
    print8(0x19, 0x88, 1.5f, lbl_1_data_378, model->x, model->y, model->z, model->rotX, model->rotY, model->rotZ);
    return ret;
}

f32 fn_1_2DA0(void)
{
    s32 r;
    f32 v;

    r = frandmod(0xC9) - 0x64;
    v = r;
    v = v / 100.0f;
    return v;
}

void fn_1_2E18(M546State *state, s16 idx, M546Vec3 *pos)
{
    memset(&lbl_1_bss_14AB8, 0, sizeof(M546Kami));
    lbl_1_bss_14AB8.floorY = 105.0f;
    lbl_1_bss_14AB8.active = 1;
    lbl_1_bss_14AB8.model[0] = state->models[idx].id;
    lbl_1_bss_14AB8.model[1] = state->models[idx + 1].id;
    lbl_1_bss_14AB8.model[2] = state->models[idx + 2].id;
    lbl_1_bss_14AB8.model[3] = state->models[idx + 3].id;
    sprintf(lbl_1_bss_14AB8.name[0], lbl_1_data_408, lbl_1_data_40B);
    sprintf(lbl_1_bss_14AB8.name[1], lbl_1_data_408, lbl_1_data_417);
    sprintf(lbl_1_bss_14AB8.name[2], lbl_1_data_408, lbl_1_data_423);
    sprintf(lbl_1_bss_14AB8.name[3], lbl_1_data_408, lbl_1_data_42F);
    lbl_1_bss_14AB8.pos = *pos;
    lbl_1_bss_14AB8.spawnTimer = 10;
    lbl_1_bss_14AB8.spawnMax = 1;
    lbl_1_bss_14AB8.hook = Hu3DHookFuncCreate(fn_1_3BE0);
    Hu3DModelCameraSet(lbl_1_bss_14AB8.hook, lbl_1_data_320[0]);
    Hu3DModelLayerSet(lbl_1_bss_14AB8.hook, 4);
}

void fn_1_301C(void)
{
    Hu3DModelKill(lbl_1_bss_14AB8.hook);
}

void fn_1_3048(void)
{
    s16 i;
    f32 ang;
    f32 spC;
    f32 sp14;
    f32 sp1C;
    f32 sp24;

    for (i = 0; i < 1000; i++) {
        if (lbl_1_bss_1F0[i].active == 0) {
            lbl_1_bss_1F0[i].active = 1;
            lbl_1_bss_1F0[i].life = frandmod(30) + 0x88;
            ang = 360.0f * fn_1_2DA0();
            lbl_1_bss_1F0[i].pos.x = lbl_1_bss_14AB8.pos.x + 50.0 * sin(3.141592653589793 * ang / 180.0);
            lbl_1_bss_1F0[i].pos.y = lbl_1_bss_14AB8.pos.y;
            lbl_1_bss_1F0[i].pos.z = lbl_1_bss_14AB8.pos.z + 50.0 * cos(3.141592653589793 * ang / 180.0);
            lbl_1_bss_1F0[i].rot.x = 0.0f;
            lbl_1_bss_1F0[i].rot.y = 0.0f;
            lbl_1_bss_1F0[i].rot.z = 0.0f;
            lbl_1_bss_1F0[i].vel.x = 0.8f * fn_1_2DA0();
            lbl_1_bss_1F0[i].vel.y = -1.0f * (6.5f * fn_1_2DA0());
            if (lbl_1_bss_1F0[i].vel.y > 0.0f) {
                lbl_1_bss_1F0[i].vel.y *= -1.0f;
            }
            if (lbl_1_bss_1F0[i].vel.y > -2.6000001f) {
                lbl_1_bss_1F0[i].vel.y = -2.6000001f;
            }
            sp24 = fn_1_2DA0();
            lbl_1_bss_1F0[i].vel.z = 0.5f * sp24;
            sp1C = fn_1_2DA0();
            lbl_1_bss_1F0[i].rotVel.x = 14.0f * sp1C;
            sp14 = fn_1_2DA0();
            lbl_1_bss_1F0[i].rotVel.y = 14.0f * sp14;
            spC = fn_1_2DA0();
            lbl_1_bss_1F0[i].rotVel.z = 14.0f * spC;
            lbl_1_bss_1F0[i].colorIdx = frandmod(4);
            lbl_1_bss_1F0[i].color.x = lbl_1_data_3A8[lbl_1_bss_1F0[i].colorIdx][0];
            lbl_1_bss_1F0[i].color.y = lbl_1_data_3A8[lbl_1_bss_1F0[i].colorIdx][1];
            lbl_1_bss_1F0[i].color.z = lbl_1_data_3A8[lbl_1_bss_1F0[i].colorIdx][2];
            {
                s16 k = frandmod(4);
                lbl_1_bss_1F0[i].color2.x = lbl_1_data_3D8[k][0];
                lbl_1_bss_1F0[i].color2.y = lbl_1_data_3D8[k][1];
                lbl_1_bss_1F0[i].color2.z = lbl_1_data_3D8[k][2];
            }
            lbl_1_bss_1F0[i].alpha = 1.0f;
            lbl_1_bss_14AB8.total++;
            if (lbl_1_bss_14AB8.spawnCnt++ >= lbl_1_bss_14AB8.spawnMax) {
                break;
            }
        }
    }
}

void fn_1_38A0(void)
{
    s16 i;

    if (lbl_1_bss_14AB8.active != 0) {
        for (i = 0; i < 1000; i++) {
            if (lbl_1_bss_1F0[i].active != 0) {
                switch (lbl_1_bss_1F0[i].state) {
                case 0:
                    lbl_1_bss_1F0[i].pos.x += lbl_1_bss_1F0[i].vel.x;
                    lbl_1_bss_1F0[i].pos.y += lbl_1_bss_1F0[i].vel.y;
                    lbl_1_bss_1F0[i].pos.z += lbl_1_bss_1F0[i].vel.z;
                    lbl_1_bss_1F0[i].rot.x += lbl_1_bss_1F0[i].rotVel.x;
                    lbl_1_bss_1F0[i].rot.y += lbl_1_bss_1F0[i].rotVel.y;
                    lbl_1_bss_1F0[i].rot.z += lbl_1_bss_1F0[i].rotVel.z;
                    if (lbl_1_bss_1F0[i].pos.y <= 105.0f) {
                        lbl_1_bss_1F0[i].pos.y = lbl_1_bss_14AB8.floorY;
                        lbl_1_bss_1F0[i].rot.x = 0.0f;
                        lbl_1_bss_1F0[i].rot.z = 0.0f;
                        lbl_1_bss_14AB8.floorY += 0.001f;
                        lbl_1_bss_1F0[i].state++;
                    }
                    break;
                case 1:
                    break;
                }
            }
        }
        if (lbl_1_bss_14AB8.spawnTimer-- <= 0 && lbl_1_bss_14AB8.total < 1000) {
            lbl_1_bss_14AB8.spawnTimer = frandmod(2) + 1;
            lbl_1_bss_14AB8.spawnMax = 1;
            lbl_1_bss_14AB8.spawnCnt = 0;
            fn_1_3048();
        }
    }
}

void fn_1_3BE0(HU3DMODEL *modelP, Mtx *mtx)
{
    Mtx sp3C;
    Mtx spC;
    s16 i;

    if (lbl_1_bss_14AB8.active != 0) {
        Hu3DModelObjDrawInit();
        for (i = 0; i < 1000; i++) {
            if (lbl_1_bss_1F0[i].active != 0) {
                PSMTXRotRad(spC, 'z', 0.017453292f * lbl_1_bss_1F0[i].rot.z);
                PSMTXRotRad(sp3C, 'x', 0.017453292f * lbl_1_bss_1F0[i].rot.x);
                PSMTXConcat(spC, sp3C, sp3C);
                PSMTXRotRad(spC, 'y', 0.017453292f * lbl_1_bss_1F0[i].rot.y);
                PSMTXConcat(spC, sp3C, sp3C);
                PSMTXTrans(spC, lbl_1_bss_1F0[i].pos.x, lbl_1_bss_1F0[i].pos.y, lbl_1_bss_1F0[i].pos.z);
                PSMTXConcat(spC, sp3C, sp3C);
                PSMTXConcat(*mtx, sp3C, sp3C);
                Hu3DModelTPLvlSet(lbl_1_bss_14AB8.model[lbl_1_bss_1F0[i].colorIdx], lbl_1_bss_1F0[i].alpha);
                Hu3DModelObjDraw(lbl_1_bss_14AB8.model[lbl_1_bss_1F0[i].colorIdx],
                                 lbl_1_bss_14AB8.name[lbl_1_bss_1F0[i].colorIdx], sp3C);
            }
        }
    }
}

void fn_1_3E2C(M546State *state, s16 idx, M546Vec3 *pos, s16 count)
{
    memset(&lbl_1_bss_14A10, 0, sizeof(M546Kami));
    lbl_1_bss_14A10.active = 1;
    lbl_1_bss_14A10.model[0] = state->models[idx].id;
    lbl_1_bss_14A10.model[1] = state->models[idx + 1].id;
    lbl_1_bss_14A10.model[2] = state->models[idx + 2].id;
    lbl_1_bss_14A10.model[3] = state->models[idx + 3].id;
    sprintf(lbl_1_bss_14A10.name[0], lbl_1_data_408, lbl_1_data_40B);
    sprintf(lbl_1_bss_14A10.name[1], lbl_1_data_408, lbl_1_data_417);
    sprintf(lbl_1_bss_14A10.name[2], lbl_1_data_408, lbl_1_data_423);
    sprintf(lbl_1_bss_14A10.name[3], lbl_1_data_408, lbl_1_data_42F);
    lbl_1_bss_14A10.pos = *pos;
    lbl_1_bss_14A10.spawnMax = count;
    lbl_1_bss_14A10.hook = Hu3DHookFuncCreate(fn_1_4BA8);
    Hu3DModelCameraSet(lbl_1_bss_14A10.hook, lbl_1_data_320[0]);
    Hu3DModelLayerSet(lbl_1_bss_14A10.hook, 4);
    fn_1_403C();
}

void fn_1_4010(void)
{
    Hu3DModelKill(lbl_1_bss_14A10.hook);
}

void fn_1_403C(void)
{
    s16 i;
    f32 ang;
    f32 spC;
    f32 sp14;
    f32 sp1C;
    f32 sp24;

    for (i = 0; i < lbl_1_bss_14A10.spawnMax; i++) {
        if (lbl_1_bss_4C[i].active == 0) {
            lbl_1_bss_4C[i].active = 1;
            lbl_1_bss_4C[i].life = frandmod(30) + 0x88;
            ang = 360.0f * fn_1_2DA0();
            lbl_1_bss_4C[i].pos.x = lbl_1_bss_14A10.pos.x + 50.0 * sin(3.141592653589793 * ang / 180.0);
            lbl_1_bss_4C[i].pos.y = lbl_1_bss_14A10.pos.y;
            lbl_1_bss_4C[i].pos.z = lbl_1_bss_14A10.pos.z + 50.0 * cos(3.141592653589793 * ang / 180.0);
            lbl_1_bss_4C[i].rot.x = 0.0f;
            lbl_1_bss_4C[i].rot.y = 0.0f;
            lbl_1_bss_4C[i].rot.z = 0.0f;
            lbl_1_bss_4C[i].vel.x = 0.8f * fn_1_2DA0();
            lbl_1_bss_4C[i].vel.y = -1.0f * (6.5f * fn_1_2DA0());
            if (lbl_1_bss_4C[i].vel.y > 0.0f) {
                lbl_1_bss_4C[i].vel.y *= -1.0f;
            }
            if (lbl_1_bss_4C[i].vel.y > -2.6000001f) {
                lbl_1_bss_4C[i].vel.y = -2.6000001f;
            }
            sp24 = fn_1_2DA0();
            lbl_1_bss_4C[i].vel.z = sp24;
            sp1C = fn_1_2DA0();
            lbl_1_bss_4C[i].rotVel.x = 14.0f * sp1C;
            sp14 = fn_1_2DA0();
            lbl_1_bss_4C[i].rotVel.y = 14.0f * sp14;
            spC = fn_1_2DA0();
            lbl_1_bss_4C[i].rotVel.z = 14.0f * spC;
            lbl_1_bss_4C[i].colorIdx = frandmod(4);
            lbl_1_bss_4C[i].color.x = lbl_1_data_3A8[lbl_1_bss_4C[i].colorIdx][0];
            lbl_1_bss_4C[i].color.y = lbl_1_data_3A8[lbl_1_bss_4C[i].colorIdx][1];
            lbl_1_bss_4C[i].color.z = lbl_1_data_3A8[lbl_1_bss_4C[i].colorIdx][2];
            {
                s16 k = frandmod(4);
                lbl_1_bss_4C[i].color2.x = lbl_1_data_3D8[k][0];
                lbl_1_bss_4C[i].color2.y = lbl_1_data_3D8[k][1];
                lbl_1_bss_4C[i].color2.z = lbl_1_data_3D8[k][2];
            }
            lbl_1_bss_4C[i].alpha = 1.0f;
            if (++lbl_1_bss_14A10.spawnCnt >= lbl_1_bss_14A10.spawnMax) {
                break;
            }
        }
    }
}

void fn_1_487C(void)
{
    s16 i;

    if (lbl_1_bss_14A10.active != 0) {
        for (i = 0; i < lbl_1_bss_14A10.spawnMax; i++) {
            if (lbl_1_bss_4C[i].active != 0) {
                lbl_1_bss_4C[i].pos.x += lbl_1_bss_4C[i].vel.x;
                lbl_1_bss_4C[i].pos.y += lbl_1_bss_4C[i].vel.y;
                lbl_1_bss_4C[i].pos.z += lbl_1_bss_4C[i].vel.z;
                lbl_1_bss_4C[i].rot.x += lbl_1_bss_4C[i].rotVel.x;
                lbl_1_bss_4C[i].rot.y += lbl_1_bss_4C[i].rotVel.y;
                lbl_1_bss_4C[i].rot.z += lbl_1_bss_4C[i].rotVel.z;
                if (lbl_1_bss_4C[i].pos.y <= 105.0f) {
                    lbl_1_bss_4C[i].alpha = 1.0f;
                    lbl_1_bss_4C[i].pos.x -= lbl_1_bss_4C[i].vel.x;
                    lbl_1_bss_4C[i].pos.y = 105.0f;
                    lbl_1_bss_4C[i].pos.z -= lbl_1_bss_4C[i].vel.z;
                    lbl_1_bss_4C[i].rot.x = 0.0f;
                    lbl_1_bss_4C[i].rot.y = 45.0f;
                    lbl_1_bss_4C[i].rot.z = 0.0f;
                    lbl_1_bss_4C[i].life = 100;
                }
            }
        }
    }
}

void fn_1_4BA8(HU3DMODEL *modelP, Mtx *mtx)
{
    Mtx sp3C;
    Mtx spC;
    s16 i;

    if (lbl_1_bss_14A10.active != 0) {
        Hu3DModelObjDrawInit();
        for (i = 0; i < lbl_1_bss_14A10.spawnMax; i++) {
            if (lbl_1_bss_4C[i].active != 0) {
                PSMTXRotRad(spC, 'z', 0.017453292f * lbl_1_bss_4C[i].rot.z);
                PSMTXRotRad(sp3C, 'x', 0.017453292f * lbl_1_bss_4C[i].rot.x);
                PSMTXConcat(spC, sp3C, sp3C);
                PSMTXRotRad(spC, 'y', 0.017453292f * lbl_1_bss_4C[i].rot.y);
                PSMTXConcat(spC, sp3C, sp3C);
                PSMTXTrans(spC, lbl_1_bss_4C[i].pos.x, lbl_1_bss_4C[i].pos.y, lbl_1_bss_4C[i].pos.z);
                PSMTXConcat(spC, sp3C, sp3C);
                PSMTXConcat(*mtx, sp3C, sp3C);
                Hu3DModelTPLvlSet(lbl_1_bss_14A10.model[lbl_1_bss_4C[i].colorIdx], lbl_1_bss_4C[i].alpha);
                Hu3DModelObjDraw(lbl_1_bss_14A10.model[lbl_1_bss_4C[i].colorIdx],
                                 lbl_1_bss_14A10.name[lbl_1_bss_4C[i].colorIdx], sp3C);
            }
        }
    }
}

void fn_1_4E00(void)
{
    s16 i;

    Hu3DLighInit();
    for (i = 0; i < 1; i++) {
        lbl_1_bss_4[i].enable = 1;
        lbl_1_bss_4[i].pos.x = 0.0f;
        lbl_1_bss_4[i].pos.y = 0.0f;
        lbl_1_bss_4[i].pos.z = -2000.0f;
        lbl_1_bss_4[i].target.x = 0.0f;
        lbl_1_bss_4[i].target.y = 0.0f;
        lbl_1_bss_4[i].target.z = 0.0f;
        lbl_1_bss_4[i].rotX = -90.0f;
        lbl_1_bss_4[i].rotY = 0.0f;
        lbl_1_bss_4[i].rotZ = 0.0f;
        lbl_1_bss_4[i].dist = 700.0f;
        lbl_1_bss_4[i].color.r = 0xFF;
        lbl_1_bss_4[i].color.g = 0xFF;
        lbl_1_bss_4[i].color.b = 0xFF;
        lbl_1_bss_4[i].color.a = 0xFF;
        lbl_1_bss_4[i].f38 = 120.0f * i;
        PSVECSubtract((Vec *)&lbl_1_bss_4[i].target, (Vec *)&lbl_1_bss_4[i].pos, (Vec *)&lbl_1_bss_4[i].dir);
        lbl_1_bss_4[i].id = Hu3DGLightCreateV((HuVecF *)&lbl_1_bss_4[i].pos, (HuVecF *)&lbl_1_bss_4[i].dir,
                                              &lbl_1_bss_4[i].color);
        fn_1_5190();
        Hu3DGLightStaticSet(lbl_1_bss_4[i].id, 1);
        Hu3DGLightPointSet(lbl_1_bss_4[i].id, 0.0f, 0.5f, 0);
    }
}

void fn_1_5190(void)
{
    s16 i;

    for (i = 0; i < 1; i++) {
        if (lbl_1_bss_4[i].enable != 0) {
            lbl_1_bss_4[i].pos.x += (f32)(HuPadSubStkX[0] / 5);
            lbl_1_bss_4[i].pos.y += (f32)(HuPadStkX[0] / 5);
            lbl_1_bss_4[i].pos.z += (f32)(HuPadSubStkY[0] / 5);
            fontcolor = 0xF;
            print8(0x32, 0x64, 1.5f, lbl_1_data_43B, lbl_1_bss_4[i].pos.x);
            fontcolor = 0;
            print8(0x12C, 0x64, 1.5f, lbl_1_data_43B, lbl_1_bss_4[i].pos.z);
            PSVECSubtract((Vec *)&lbl_1_bss_4[i].target, (Vec *)&lbl_1_bss_4[i].pos, (Vec *)&lbl_1_bss_4[i].dir);
            PSVECNormalize((Vec *)&lbl_1_bss_4[i].dir, (Vec *)&lbl_1_bss_4[i].dir);
            Hu3DGLightPosSet(lbl_1_bss_4[i].id, lbl_1_bss_4[i].pos.x, lbl_1_bss_4[i].pos.y, lbl_1_bss_4[i].pos.z,
                             0.0f, 1.0f, 0.0f);
            Hu3DGLightColorSet(lbl_1_bss_4[i].id, lbl_1_bss_4[i].color.r, lbl_1_bss_4[i].color.g,
                               lbl_1_bss_4[i].color.b, lbl_1_bss_4[i].color.a);
        }
    }
}

void fn_1_554C(void)
{
    lbl_1_bss_14C40.jingleId = HuAudJinglePlay(0x83);
}

void fn_1_557C(void)
{
    lbl_1_bss_14C40.bgmId = HuAudBGMPlay(0x3B);
}

void fn_1_55AC(s16 arg0, s16 arg1)
{
    s16 id;

    if (arg0 != 0) {
        id = lbl_1_bss_14C40.bgmId;
    } else {
        id = lbl_1_bss_14C40.jingleId;
    }
    HuAudSStreamFadeOut(id, arg1);
}

void fn_1_5614(s16 arg0)
{
    s16 id;

    if (arg0 != 0) {
        id = lbl_1_bss_14C40.bgmId;
    } else {
        id = lbl_1_bss_14C40.jingleId;
    }
    HuAudSStreamStop(id);
}

void fn_1_5674(s16 arg0)
{
    lbl_1_bss_14C40.fxFlag[arg0] = 1;
    lbl_1_bss_14C40.fxId[arg0] = HuAudFXPlay(lbl_1_data_43E[arg0]);
}

void fn_1_56EC(s16 arg0, s16 arg1, s16 arg2)
{
    lbl_1_bss_14C40.fxFlag[arg0] = 1;
    lbl_1_bss_14C40.fxId[arg0] = HuAudFXPlayVolPan(lbl_1_data_43E[arg0], arg1, arg2);
}

void fn_1_5774(s16 arg0, s16 arg1)
{
    if (lbl_1_bss_14C40.fxFlag[arg0] == 1) {
        if (HuAudFXStatusGet(lbl_1_bss_14C40.fxId[arg0]) == 2) {
            HuAudFXPanning(lbl_1_bss_14C40.fxId[arg0], arg1);
        }
    }
}

void fn_1_5804(s16 arg0)
{
    if (lbl_1_bss_14C40.fxFlag[arg0] == 1) {
        if (HuAudFXStatusGet(lbl_1_bss_14C40.fxId[arg0]) == 2 || HuAudFXStatusGet(lbl_1_bss_14C40.fxId[arg0]) == 3) {
            HuAudFXStop(lbl_1_bss_14C40.fxId[arg0]);
        }
    }
}

void fn_1_58B0(void)
{
    HuAudAllStop();
}
