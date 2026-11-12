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
#include "game/mg/seqman.h"
#include "REL/m545Dll.h"

double sin(double x);
double cos(double x);
void DCStoreRangeNoSync(void *addr, u32 nBytes);

typedef void (*M545VoidFunc)(void);
extern const M545VoidFunc _ctors[];
extern const M545VoidFunc _dtors[];

/* game.c */
s32 fn_1_26BC(M545State *state);
s32 fn_1_2E88(M545State *state);
s32 fn_1_2FE8(M545State *state);
s32 fn_1_3030(M545State *state);
s32 fn_1_3480(M545State *state);
s32 fn_1_34EC(M545State *state);
s32 fn_1_3538(M545State *state);
s32 fn_1_3600(M545State *state);
s32 fn_1_3638(void);

/* stage.c */
void fn_1_77EC(M545State *state, M545StageModel *models, s32 *dataNums);
void fn_1_7A38(s16 arg0);
void fn_1_7CE4(s32 *arg0, s32 *arg1);
void fn_1_7D08(s16 *arg0, s16 *arg1);
void fn_1_7D2C(void);
void fn_1_803C(void);
void fn_1_8454(void);
void fn_1_8580(void);
void fn_1_88F0(void);

void ObjectSetup(void);
void fn_1_2F0(void);
void fn_1_324(void);
void fn_1_358(void);
void fn_1_380(void);
void fn_1_3B4(void);
void fn_1_3E8(void);
void fn_1_41C(void);
void fn_1_450(void);
void fn_1_478(void);
void fn_1_4A4(void);
void fn_1_4A8(s16 arg0);
void fn_1_8C4(s16 arg0);
void fn_1_E3C(void);
void fn_1_106C(void);
void fn_1_1344(void);
void fn_1_1474(void);
s16 fn_1_1D4C(M545PtclWork *arg0, f32 x, f32 y, f32 z, f32 scale);
void fn_1_1F20(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);

/* unit-owned .bss, declared in reverse address order so the offsets match the original */
s16 lbl_1_bss_8D4;
M545PtclWork *lbl_1_bss_8D0;
OMOBJMAN *lbl_1_bss_8CC;
M545State lbl_1_bss_67C;
M545StageModel lbl_1_bss_254[38];
s16 lbl_1_bss_240[10];
M545Game lbl_1_bss_19C;
M545Plr lbl_1_bss_C[2];
M545Cam *lbl_1_bss_8;
M545Light *lbl_1_bss_4;
M545Shadow *lbl_1_bss_0;

/* unit-owned .data, in original address order */
MGSEQ_PARAM lbl_1_data_0 = {
    0, 0,
    (MGSEQ_FUNC)fn_1_2F0, (MGSEQ_FUNC)fn_1_324, (MGSEQ_FUNC)fn_1_358,
    (MGSEQ_FUNC)fn_1_380, (MGSEQ_FUNC)fn_1_3B4, (MGSEQ_FUNC)fn_1_3E8,
    (MGSEQ_FUNC)fn_1_41C, (MGSEQ_FUNC)fn_1_450, (MGSEQ_FUNC)fn_1_478,
};
s32 lbl_1_data_28[13][5] = {
    { 0x8C0000, 0x87005B, 0x87005D, 0x87005E, 0x8C0006 },
    { 0x330000, 0x2E005B, 0x2E005D, 0x2E005E, 0x330006 },
    { 0xB70000, 0xB2005B, 0xB2005D, 0xB2005E, 0xB70006 },
    { 0xED0000, 0xE8005B, 0xE8005D, 0xE8005E, 0xED0006 },
    { 0xE60000, 0xE1005B, 0xE1005D, 0xE1005E, 0xE60006 },
    { 0x170000, 0x12005B, 0x12005D, 0x12005E, 0x170006 },
    { 0xE00000, 0xDB005B, 0xDB005D, 0xDB005E, 0xE00006 },
    { 0x2D0000, 0x28005B, 0x28005D, 0x28005E, 0x2D0006 },
    { 0xC90000, 0xC4005B, 0xC4005D, 0xC4005E, 0xC90006 },
    { 0xB00000, 0x9C005B, 0x9C005D, 0x9C005E, 0xB00006 },
    { 0xB00000, 0x9C005B, 0x9C005D, 0x9C005E, 0xB00006 },
    { 0xB00000, 0x9C005B, 0x9C005D, 0x9C005E, 0xB00006 },
    { 0xB00000, 0x9C005B, 0x9C005D, 0x9C005E, 0xB00006 },
};
s32 lbl_1_data_12C[19] = {
    0x67000C, 0x67000D, 0x67000E, 0x67000F, 0x670010, 0x670011, 0x670012,
    0x670000, 0x670001, 0x670002, 0x670003, 0x670004, 0x670005, 0x670006,
    0x670007, 0x670008, 0x670009, 0x67000A, 0x67000B,
};
M545CamInit lbl_1_data_178[4] = {
    { { 0.0f, 250.0f, 0.0f }, -20.0f, 0.0f, 500.0f, 1400.0f, 1.33f, 0.0f, 0.0f, 640.0f, 480.0f, 45.0f },
    { { 0.0f, 0.0f, 0.0f }, 0.0f, 0.0f, 500.0f, 500.0f, 1.33f, 0.0f, 0.0f, 640.0f, 480.0f, 45.0f },
    { { 0.0f, 0.0f, 0.0f }, 0.0f, 0.0f, 500.0f, 500.0f, 1.33f, 0.0f, 0.0f, 640.0f, 480.0f, 45.0f },
    { { 0.0f, 0.0f, 0.0f }, 0.0f, 0.0f, 500.0f, 500.0f, 1.33f, 0.0f, 0.0f, 640.0f, 480.0f, 45.0f },
};
s16 lbl_1_data_248[16] = {
    0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80, 0x100, 0x200, 0x400, 0x800, 0x1000, 0x2000, 0x4000, 0x8000,
};
M545PtclWork lbl_1_data_268 = {
    0, 0,
    { 0xFF, 0xFF, 0xFF, 0xFF }, { 0x00, 0x00, 0x00, 0xFF },
    0.0f, 2.0f, 1.0f,
    0.85f, 0.85f, 0.85f,
    0.2f,
    0.0f, 1.0f, -3.0f, 0.02f,
};
s16 lbl_1_data_2A0[12] = {
    0x822, 0x823, 0x824, 0x825, 0x826, 0x827, 0x828, 0x829, 0x82A, 0x82B, 0x82C, 0x82D,
};

int _prolog(void)
{
    const M545VoidFunc *ctors = _ctors;

    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const M545VoidFunc *dtors = _dtors;

    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

void ObjectSetup(void)
{
    lbl_1_bss_8CC = omInitObjMan(0x32, 0x2000);
    omGameSysInit(lbl_1_bss_8CC);
    fn_1_77EC(&lbl_1_bss_67C, lbl_1_bss_254, lbl_1_data_12C);
    lbl_1_bss_8 = HuMemDirectMallocNum(HEAP_HEAP, 0x148, 0x10000000);
    memset(lbl_1_bss_8, 0, 0x148);
    lbl_1_bss_4 = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M545Light), 0x10000000);
    memset(lbl_1_bss_4, 0, sizeof(M545Light));
    lbl_1_bss_0 = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M545Shadow), 0x10000000);
    memset(lbl_1_bss_0, 0, sizeof(M545Shadow));
    fn_1_4A8(0);
    fn_1_E3C();
    fn_1_1344();
    fn_1_7A38(1);
    fn_1_7CE4(lbl_1_data_28[lbl_1_bss_67C.chars[0].charNo], lbl_1_data_28[lbl_1_bss_67C.chars[1].charNo]);
    fn_1_7D08(lbl_1_bss_240, lbl_1_bss_240 + 5);
    fn_1_7D2C();
    fn_1_8454();
    omAddObjEx(lbl_1_bss_8CC, 0x7FDA, 0, 0, -1, (OMOBJFUNC)fn_1_803C);
    omAddObjEx(lbl_1_bss_8CC, 0x7FDA, 0, 0, -1, (OMOBJFUNC)fn_1_8580);
    omAddObjEx(lbl_1_bss_8CC, 0x7FDA, 0, 0, -1, (OMOBJFUNC)fn_1_4A4);
    omAddObjEx(lbl_1_bss_8CC, 0x7FDA, 0, 0, -1, (OMOBJFUNC)fn_1_88F0);
    Hu3DBGColorSet(0xBA, 0xF5, 0xFF);
    MgSeqCreate(&lbl_1_data_0);
    MgSeqModeDelaySet(2, 0x7D0);
    MgSeqModeDelaySet(7, 0xB4);
    MgSeqModeDelaySet(9, 0x3C);
}

void fn_1_2F0(void)
{
    if (fn_1_26BC(&lbl_1_bss_67C) != 0) {
        MgSeqModeNext();
    }
}

void fn_1_324(void)
{
    if (fn_1_2E88(&lbl_1_bss_67C) != 0) {
        MgSeqModeNext();
    }
}

void fn_1_358(void)
{
    fn_1_2FE8(&lbl_1_bss_67C);
}

void fn_1_380(void)
{
    if (fn_1_3030(&lbl_1_bss_67C) != 0) {
        MgSeqModeNext();
    }
}

void fn_1_3B4(void)
{
    if (fn_1_3480(&lbl_1_bss_67C) != 0) {
        MgSeqModeNext();
    }
}

void fn_1_3E8(void)
{
    if (fn_1_34EC(&lbl_1_bss_67C) != 0) {
        MgSeqModeNext();
    }
}

void fn_1_41C(void)
{
    if (fn_1_3538(&lbl_1_bss_67C) != 0) {
        MgSeqModeNext();
    }
}

void fn_1_450(void)
{
    fn_1_3600(&lbl_1_bss_67C);
}

void fn_1_478(void)
{
    if (fn_1_3638() != 0) {
        MgSeqModeNext();
    }
}

void fn_1_4A4(void)
{
}

void fn_1_4A8(s16 arg0)
{
    lbl_1_bss_8[arg0].idx = arg0;
    lbl_1_bss_8[arg0].center.x = lbl_1_data_178[arg0].center.x;
    lbl_1_bss_8[arg0].center.y = lbl_1_data_178[arg0].center.y;
    lbl_1_bss_8[arg0].center.z = lbl_1_data_178[arg0].center.z;
    lbl_1_bss_8[arg0].rotX = lbl_1_data_178[arg0].rotX;
    lbl_1_bss_8[arg0].rotY = lbl_1_data_178[arg0].rotY;
    lbl_1_bss_8[arg0].rotZ = lbl_1_data_178[arg0].rotZ;
    lbl_1_bss_8[arg0].zoom = lbl_1_data_178[arg0].zoom;
    lbl_1_bss_8[arg0].aspect = lbl_1_data_178[arg0].aspect;
    lbl_1_bss_8[arg0].vpX = lbl_1_data_178[arg0].vpX;
    lbl_1_bss_8[arg0].vpY = lbl_1_data_178[arg0].vpY;
    lbl_1_bss_8[arg0].vpW = lbl_1_data_178[arg0].vpW;
    lbl_1_bss_8[arg0].vpH = lbl_1_data_178[arg0].vpH;
    lbl_1_bss_8[arg0].fov = lbl_1_data_178[arg0].fov;
    Hu3DCameraCreate(lbl_1_data_248[arg0]);
    Hu3DCameraViewportSet(lbl_1_data_248[arg0], lbl_1_bss_8[arg0].vpX, lbl_1_bss_8[arg0].vpY,
                          lbl_1_bss_8[arg0].vpW, lbl_1_bss_8[arg0].vpH, 0.0f, 1.0f);
    Hu3DCameraPerspectiveSet(lbl_1_data_248[arg0], lbl_1_bss_8[arg0].fov, 20.0f, 30000.0f, lbl_1_bss_8[arg0].aspect);
    fn_1_8C4(arg0);
}

void fn_1_8C4(s16 arg0)
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
    Hu3DCameraPosSet(lbl_1_data_248[arg0], lbl_1_bss_8[arg0].pos.x, lbl_1_bss_8[arg0].pos.y, lbl_1_bss_8[arg0].pos.z,
                     lbl_1_bss_8[arg0].up.x, lbl_1_bss_8[arg0].up.y, lbl_1_bss_8[arg0].up.z,
                     lbl_1_bss_8[arg0].center.x, lbl_1_bss_8[arg0].center.y, sp8);
}

void fn_1_E3C(void)
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
    fn_1_106C();
    Hu3DGLightStaticSet(lbl_1_bss_4->id, 1);
    Hu3DGLightInfinitytSet(lbl_1_bss_4->id);
}

void fn_1_106C(void)
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

void fn_1_1344(void)
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
    fn_1_1474();
}

void fn_1_1474(void)
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

void fn_1_17EC(void)
{
    s16 i;
    s16 j;
    ANIMDATA *anim;
    HU3DPARTICLE *particle;
    HU3DPARTICLEDATA *data;

    for (i = 0; i < 16; i++) {
        lbl_1_bss_8D4 = -1;
    }
    anim = HuSprAnimRead(HuDataSelHeapReadNum(0x670013, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_8D0 = HuMemDirectMallocNum(HEAP_HEAP, 0x2300, HU_MEMNUM_OVL);
    lbl_1_bss_8D4 = Hu3DParticleCreate(anim, 0xA0);
    Hu3DParticleHookSet(lbl_1_bss_8D4, fn_1_1F20);
    Hu3DParticleBlendModeSet(lbl_1_bss_8D4, 1);
    Hu3DModelLayerSet(lbl_1_bss_8D4, 4);
    Hu3DModelCameraSet(lbl_1_bss_8D4, 1);
    Hu3DModelAttrSet(lbl_1_bss_8D4, 1);
    Hu3DModelAttrReset(lbl_1_bss_8D4, 1);
    particle = (HU3DPARTICLE *)Hu3DData[lbl_1_bss_8D4].hookData;
    particle->emitCnt = 0;
    particle->work = lbl_1_bss_8D0;
    particle->count = 0;
    data = particle->data;
    for (j = 0; j < particle->maxCnt; j++, data++) {
        data->scale = 0.0f;
    }
}

void fn_1_1980(M545Vec3 *arg0)
{
    M545Vec3 sp;
    s16 sp8;
    f32 speed;
    f32 mul;
    s16 t;
    s16 i;

    sp8 = 0x10;
    speed = 0.8f;
    mul = 1.0f;
    for (i = 0; i < 12; i++) {
        speed = mul * (6.0 + frandmod(6));
        lbl_1_data_268.velX = speed * sin(3.141592653589793 * (30.0f * i) / 180.0);
        lbl_1_data_268.velY = 0.0f;
        lbl_1_data_268.velZ = speed * cos(3.141592653589793 * (30.0f * i) / 180.0);
        t = -frandmod(6) - 7;
        lbl_1_data_268.alphaStep = t;
        speed = mul * (20.0 + frandmod(20));
        sp.x = arg0->x + mul * (20.0 * sin(3.141592653589793 * (30.0f * i) / 180.0));
        sp.y = arg0->y + 10.0f * mul;
        sp.z = arg0->z + mul * (20.0 * cos(3.141592653589793 * (30.0f * i) / 180.0));
        lbl_1_data_268.accelY = 0.01 * frandmod(20);
        fn_1_1D4C(&lbl_1_data_268, sp.x, sp.y, sp.z, speed);
    }
}

s16 fn_1_1D4C(M545PtclWork *arg0, f32 x, f32 y, f32 z, f32 scale)
{
    HU3DPARTICLEDATA *data;
    HU3DPARTICLE *particle;
    s16 i;
    s16 ret;
    HU3DMODEL *model;
    M545PtclWork *work;

    ret = -1;
    model = &Hu3DData[lbl_1_bss_8D4];
    particle = (HU3DPARTICLE *)model->hookData;
    work = (M545PtclWork *)particle->work;
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

void fn_1_1F20(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx)
{
    M545PtclWork *work;
    HU3DPARTICLEDATA *data;
    s16 i;
    s16 v;

    work = (M545PtclWork *)particleP->work;
    data = particleP->data;
    data->time = 0;
    for (i = 0; i < particleP->maxCnt; i++, data++) {
        if (data->scale != 0.0f) {
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

void fn_1_2338(void)
{
    lbl_1_bss_19C.jingleId = HuAudJinglePlay(0x83);
}

void fn_1_2368(void)
{
    lbl_1_bss_19C.bgmId = HuAudBGMPlay(0x3B);
}

void fn_1_2398(s16 arg0, s16 arg1)
{
    s16 id;

    if (arg0 != 0) {
        id = lbl_1_bss_19C.bgmId;
    } else {
        id = lbl_1_bss_19C.jingleId;
    }
    HuAudSStreamFadeOut(id, arg1);
}

void fn_1_2400(s16 arg0)
{
    s16 id;

    if (arg0 != 0) {
        id = lbl_1_bss_19C.bgmId;
    } else {
        id = lbl_1_bss_19C.jingleId;
    }
    HuAudSStreamStop(id);
}

void fn_1_2460(s16 arg0)
{
    lbl_1_bss_19C.fxFlag[arg0] = 1;
    lbl_1_bss_19C.fxId[arg0] = HuAudFXPlay(lbl_1_data_2A0[arg0]);
}

void fn_1_24D8(s16 arg0, s16 arg1, s16 arg2)
{
    lbl_1_bss_19C.fxFlag[arg0] = 1;
    lbl_1_bss_19C.fxId[arg0] = HuAudFXPlayVolPan(lbl_1_data_2A0[arg0], arg1, arg2);
}

void fn_1_2560(s16 arg0, s16 arg1)
{
    if (lbl_1_bss_19C.fxFlag[arg0] == 1) {
        if (HuAudFXStatusGet(lbl_1_bss_19C.fxId[arg0]) == 2) {
            HuAudFXPanning(lbl_1_bss_19C.fxId[arg0], arg1);
        }
    }
}

void fn_1_25F0(s16 arg0)
{
    if (lbl_1_bss_19C.fxFlag[arg0] == 1) {
        if (HuAudFXStatusGet(lbl_1_bss_19C.fxId[arg0]) == 2 || HuAudFXStatusGet(lbl_1_bss_19C.fxId[arg0]) == 3) {
            HuAudFXStop(lbl_1_bss_19C.fxId[arg0]);
        }
    }
}

void fn_1_269C(void)
{
    HuAudAllStop();
}
