#include <math.h>

#include <dolphin.h>
#include <dolphin/mtx.h>

#include "game/hu3d.h"

#include "REL/m511Dll/m511objs.h"

#ifdef TARGET_PC
#define SPL_VTX3(x, y, z) GXPosition3f32(x, y, z)
#define SPL_TEX2(s, t) GXTexCoord2f32(s, t)
#define SPL_END() GXEnd()
#else
#define SPL_VTX3(x, y, z) \
    do { \
        GXWGFifo.f32 = (x); \
        GXWGFifo.f32 = (y); \
        GXWGFifo.f32 = (z); \
    } while (0)
#define SPL_TEX2(s, t) \
    do { \
        GXWGFifo.f32 = (s); \
        GXWGFifo.f32 = (t); \
    } while (0)
#define SPL_END() ((void)0)
#endif

extern "C" {

void HuSprTexLoad(ANIMDATA *anim, s16 bmpNo, s16 texMapId, GXTexWrapMode wrapS, GXTexWrapMode wrapT, GXTexFilter filter);
void *__dl__FPv(void *self);
s32 frand(void);
void fn_1_CF8(char *cond, char *file, s32 line);
u16 fn_1_5578(void);
f32 fn_1_5A28(f32 scale);
f64 fn_1_5A60(f32 min, f32 max);

/* Same layout as the scene resource root declared in boatshadow.cpp. */
struct Mgr511SplashRoot {
    u32 unk0;
    u32 unk4;
    void **vtable;
    s32 unkC;
    ANIMDATA *unk10;
    ANIMDATA *unk14;
    ANIMDATA *unk18;
    ANIMDATA *unk1C;
    ANIMDATA *unk20;
};

extern Mgr511SplashRoot *lbl_1_bss_A38;

/* Burst of droplets. */
struct Mgr511SplashBurst {
    s16 modelId;
    HU3DPARTICLE *particle;
    u8 done;
    u8 started;
};

static inline void SplashRespawn(Mgr511Splash *arg0, HU3DPARTICLEDATA *arg1, s32 arg2)
{
    f32 sgn;
    f32 yaw;
    f32 dir;
    f32 mag;
    f32 sinY;
    f32 cosY;
    f32 dx;
    f32 dz;
    f32 vx;
    f32 vz;
    f32 tmp;
    const GXColor col = { 0xEF, 0xFF, 0xFF, 0x00 };

    arg1->time = 0;
    arg1->parManId = (s16)((frand() & 0xF) + 7);
    if ((arg2 & 1) == 0) {
        sgn = -1.0f;
    } else {
        sgn = 1.0f;
    }
    yaw = (f32)((3.141592653589793 * (f64)(180.0f + arg0->rot.y)) / 180.0);
    sinY = (f32)sin(yaw);
    cosY = (f32)cos(yaw);
    dz = arg0->pos.z + (100.0f * cosY);
    dx = arg0->pos.x + (100.0f * sinY);
    arg0->pos2.x = dx;
    arg0->pos2.y = arg0->pos.y;
    arg0->pos2.z = dz;
    if (sgn > -1.0f) {
        dir = (f32)((3.141592653589793 * (f64)(arg0->rot.y - 90.0f)) / 180.0);
    } else {
        dir = (f32)((3.141592653589793 * (f64)(90.0f + arg0->rot.y)) / 180.0);
    }
    mag = 7.0f * arg0->speed;
    if (mag > 1.0f) {
        mag = 1.0f;
    }
    vx = (f32)(mag * sin(dir));
    vz = (f32)(mag * cos(dir));
    arg1->accel.x = vx;
    arg1->accel.y = mag;
    arg1->accel.z = vz;
    arg1->vel.y = 1.0896277f;
    arg1->pos.x = arg0->pos2.x + (50.0f * vx);
    arg1->pos.y = arg0->pos2.y;
    arg1->pos.z = arg0->pos2.z + (50.0f * vz);
    tmp = 0.4f + (0.2f * (0.000015258789f * (f32)(s32)fn_1_5578()));
    arg1->scale = 100.0f * tmp;
    arg1->zRot = 0.017453292f * (360.0f * (0.000015258789f * (f32)(s32)fn_1_5578()));
    arg1->color = col;
}

void fn_1_1711C(Mgr511Splash *arg0, HU3DPARTICLEDATA *arg1, s32 arg2);
void fn_1_1757C(Mgr511Splash *arg0, Mtx mtx);
void fn_1_17EC8(Mgr511SplashRing *arg0, Mtx mtx);
void fn_1_1886C(Mgr511SplashBurst *arg0, Mtx mtx);

void fn_1_16E40(HU3DMODEL *arg0, HU3DPARTICLE *arg1, Mtx mtx)
{
    Mgr511Splash *o = (Mgr511Splash *)arg1->work;

    fn_1_1757C(o, mtx);
}

Mgr511Splash *fn_1_16E84(Mgr511Splash *arg0)
{
    HU3DMODELID pid;
    HU3DPARTICLEDATA *node;
    s32 i;

    arg0->flag = 0;
    if (lbl_1_bss_A38 == NULL) {
        fn_1_CF8("ms_pointer != 0", "singleton.h", 0x27);
    }
    pid = Hu3DParticleCreate(lbl_1_bss_A38->unk14, 0x20);
    arg0->particle = (HU3DPARTICLE *)Hu3DData[pid].hookData;
    arg0->particle->work = arg0;
    Hu3DParticleHookSet(pid, fn_1_16E40);
    Hu3DParticleColSet(pid, 0xEF, 0xFF, 0xFF);
    Hu3DParticleTPLvlSet(pid, 0.0f);
    Hu3DParticleBlendModeSet(pid, 0);
    Hu3DModelAttrSet(pid, 0x400000);
    Hu3DModelLayerSet(pid, 3);
    Hu3DModelCameraSet(pid, 0xF);
    Hu3DModelPosSet(pid, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(pid, 0.0f, 0.0f, 0.0f);
    arg0->pos.x = 0.0f;
    arg0->pos.y = 0.0f;
    arg0->pos.z = 0.0f;
    arg0->rot.x = 0.0f;
    arg0->rot.y = 0.0f;
    arg0->rot.z = 0.0f;
    arg0->speed = 0.0f;
    arg0->modelId = pid;
    node = arg0->particle->data;
    for (i = 0; i < arg0->particle->maxCnt; i++) {
        fn_1_1711C(arg0, node, i);
        node++;
    }
    arg0->particle->dataCnt = 0;
    arg0->speed = 0.0f;
    return arg0;
}

Mgr511Splash *fn_1_170C8(Mgr511Splash *arg0, s16 arg1)
{
    if (arg0) {
        Hu3DModelKill(arg0->modelId);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

void fn_1_1711C(Mgr511Splash *arg0, HU3DPARTICLEDATA *arg1, s32 arg2)
{
    SplashRespawn(arg0, arg1, arg2);
}

void fn_1_17544(Mgr511Splash *arg0, Vec *arg1, Vec *arg2, f32 arg3)
{
    arg0->pos.x = arg1->x;
    arg0->pos.y = arg1->y;
    arg0->pos.z = arg1->z;
    arg0->rot.x = arg2->x;
    arg0->rot.y = arg2->y;
    arg0->rot.z = arg2->z;
    arg0->speed = arg3;
}

void fn_1_1757C(Mgr511Splash *arg0, Mtx mtx)
{
    HU3DMODEL *model;
    HU3DPARTICLE *particle;
    HU3DPARTICLEDATA *node;
    f32 prevVy;
    f32 minY;
    s32 i;

    model = &Hu3DData[arg0->modelId];
    particle = arg0->particle;
    node = particle->data;
    for (i = 0; i < particle->maxCnt; i++, node++) {
        switch (node->time) {
        case 0:
            node->color.a += 0x10;
            if (node->color.a < 0x80) {
                goto move;
            }
            node->color.a = 0x80;
            node->time++;
            /* fallthrough */
        case 1:
            node->color.a -= node->parManId;
            if (node->color.a > 0x19) {
                goto move;
            }
            node->color.a = 0;
            node->time++;
            /* fallthrough */
        case 2:
            if (arg0->flag == 1) {
                fn_1_1711C(arg0, node, i);
            }
            break;
        default:
        move:
            node->zRot = node->zRot + 0.017453292f;
            node->scale = node->scale + 3.0f;
            if (node->scale > 100.0f) {
                node->scale = 100.0f;
            }
            prevVy = node->vel.y;
            node->vel.y = (f32)((f64)node->vel.y + -0.27240694444444447);
            node->pos.y = node->pos.y + (0.5f * (prevVy + node->vel.y));
            minY = (0.2f * node->scale) + arg0->pos.y;
            if (node->pos.y < minY) {
                node->pos.y = minY;
            }
            node->pos.x = node->pos.x + node->accel.x;
            node->pos.z = node->pos.z + node->accel.z;
            break;
        }
    }
    DCStoreRangeNoSync(particle->data, particle->maxCnt * sizeof(HU3DPARTICLEDATA));
}

void fn_1_17B94(HU3DMODEL *arg0, Mtx *mtx)
{
    Mgr511SplashRing *o = (Mgr511SplashRing *)arg0->hookData;

    fn_1_17EC8(o, *mtx);
}

Mgr511SplashRing *fn_1_17BD8(Mgr511SplashRing *arg0)
{
    HU3DMODELID id;

    arg0->active = 0;
    arg0->timer = 0.0f;
    arg0->scale = 0.0f;
    id = Hu3DHookFuncCreate(fn_1_17B94);
    Hu3DData[id].hookData = arg0;
    Hu3DModelLayerSet(id, 2);
    Hu3DModelCameraSet(id, 0xF);
    arg0->modelId = id;
    if (lbl_1_bss_A38 == NULL) {
        fn_1_CF8("ms_pointer != 0", "singleton.h", 0x27);
    }
    arg0->anim = lbl_1_bss_A38->unk18;
    return arg0;
}

Mgr511SplashRing *fn_1_17CC8(Mgr511SplashRing *arg0, s16 arg1)
{
    if (arg0) {
        Hu3DModelKill(arg0->modelId);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

void fn_1_17D1C(Mgr511SplashRing *arg0, Vec *arg1, Vec *arg2, f32 arg3)
{
    f32 yaw;
    f32 sinY;
    f32 cosY;
    f32 z;

    yaw = (f32)((3.141592653589793 * (f64)(180.0f + arg2->y)) / 180.0);
    sinY = (f32)sin(yaw);
    cosY = (f32)cos(yaw);
    z = arg1->z + (100.0f * cosY);
    arg0->pos.x = arg1->x + (100.0f * sinY);
    arg0->pos.y = arg1->y;
    arg0->pos.z = z;
    arg0->scale = arg3;
    arg0->active = 1;
}

void fn_1_17E6C(Mgr511SplashRing *arg0)
{
    if (arg0->active != 0) {
        arg0->timer = (f32)((f64)arg0->timer + 0.016666666666666666);
        if (!(arg0->timer < 0.5f)) {
            arg0->timer = 0.0f;
            arg0->active = 0;
        }
    }
}

void fn_1_17EC8(Mgr511SplashRing *arg0, Mtx mtx)
{
    Mtx trans;
    Mtx scale;
    f32 mag;
    f32 s;
    GXColor color = { 0xFF, 0xFF, 0xFF, 0x00 };

    if (arg0->active != 0) {
        PSMTXTrans(trans, arg0->pos.x, arg0->pos.y, arg0->pos.z);
        mag = 7.0f * arg0->scale;
        if (mag > 1.0f) {
            mag = 1.0f;
        }
        s = (1.0f + (1.5f * arg0->timer)) * mag;
        PSMTXScale(scale, s, 1.0f, s);
        PSMTXConcat(mtx, trans, mtx);
        PSMTXConcat(mtx, scale, scale);
        GXLoadPosMtxImm(scale, 0);
        HuSprTexLoad(arg0->anim, 0, 0, GX_CLAMP, GX_CLAMP, GX_LINEAR);
        GXSetZMode(1, GX_LEQUAL, 0);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
        GXSetZCompLoc(1);
        GXSetColorUpdate(1);
        GXSetAlphaUpdate(1);
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
        GXSetNumChans(0);
        GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, 0, 0x7D);
        color.a = (u8)(2.0f * (255.0f * (0.5f - arg0->timer)));
        GXSetTevColor(GX_TEVREG0, color);
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 0, GX_TEVPREV);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 0, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A0, GX_CA_ZERO);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        SPL_VTX3(-100.0f, 0.0f, -100.0f);
        SPL_TEX2(0.0f, 0.0f);
        SPL_VTX3(100.0f, 0.0f, -100.0f);
        SPL_TEX2(1.0f, 0.0f);
        SPL_VTX3(100.0f, 0.0f, 100.0f);
        SPL_TEX2(1.0f, 1.0f);
        SPL_VTX3(-100.0f, 0.0f, 100.0f);
        SPL_TEX2(0.0f, 1.0f);
        SPL_END();
    }
}

void fn_1_183C0(HU3DMODEL *arg0, HU3DPARTICLE *arg1, Mtx mtx)
{
    Mgr511SplashBurst *o = (Mgr511SplashBurst *)arg1->work;

    fn_1_1886C(o, mtx);
}

Mgr511SplashBurst *fn_1_18404(Mgr511SplashBurst *arg0)
{
    HU3DMODELID pid;
    HU3DPARTICLE *particle;
    HU3DPARTICLEDATA *node;
    s32 i;
    f32 ang;
    const GXColor col = { 0xEF, 0xFF, 0xFF, 0x00 };

    arg0->done = 0;
    arg0->started = 0;
    if (lbl_1_bss_A38 == NULL) {
        fn_1_CF8("ms_pointer != 0", "singleton.h", 0x27);
    }
    pid = Hu3DParticleCreate(lbl_1_bss_A38->unk14, 0x28);
    arg0->particle = (HU3DPARTICLE *)Hu3DData[pid].hookData;
    arg0->particle->work = arg0;
    Hu3DParticleHookSet(pid, fn_1_183C0);
    Hu3DParticleColSet(pid, 0xEF, 0xEF, 0xFF);
    Hu3DParticleBlendModeSet(pid, 0);
    Hu3DModelLayerSet(pid, 3);
    Hu3DModelCameraSet(pid, 0xF);
    Hu3DModelPosSet(pid, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(pid, 0.0f, 0.0f, 0.0f);
    Hu3DModelAttrSet(pid, 1);
    arg0->modelId = pid;
    particle = arg0->particle;
    node = particle->data;
    for (i = 0; i < arg0->particle->maxCnt; i++) {
        ang = 360.0f * (f32)i;
        node->vel.x = (f32)sin(0.017453292f * (ang / (f32)particle->maxCnt));
        node->vel.y = (f32)(0.27240694444444447 * fn_1_5A60(20.0f, 40.0f));
        ang = 360.0f * (f32)i;
        node->vel.z = (f32)cos(0.017453292f * (ang / (f32)particle->maxCnt));
        node->scale = 80.0f;
        node->pos.x = 0.5f * (100.0f * node->vel.x);
        node->pos.y = 0.0f;
        node->pos.z = 0.5f * (100.0f * node->vel.z);
        node->color = col;
        node->time = 0;
        node->parManId = 0;
        node->zRot = 0.017453292f * fn_1_5A28(360.0f);
        node++;
    }
    return arg0;
}

Mgr511SplashBurst *fn_1_187B0(Mgr511SplashBurst *arg0, s16 arg1)
{
    if (arg0) {
        Hu3DModelKill(arg0->modelId);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

void fn_1_18804(Mgr511SplashBurst *arg0, Vec *arg1)
{
    HU3DMODELID id;

    id = arg0->modelId;
    Hu3DModelAttrReset(id, 1);
    Hu3DModelPosSet(id, arg1->x, arg1->y, arg1->z);
    arg0->started = 1;
}

void fn_1_1886C(Mgr511SplashBurst *arg0, Mtx mtx)
{
    HU3DMODEL *model;
    HU3DPARTICLE *particle;
    HU3DPARTICLEDATA *node;
    f32 prevVy;
    s32 alive;
    s32 i;

    if (arg0->started != 0 && arg0->done != 1) {
        model = &Hu3DData[arg0->modelId];
        particle = arg0->particle;
        node = particle->data;
        alive = 0;
        for (i = 0; i < particle->maxCnt; i++, node++) {
            if (node->time != 2) {
                if (node->time == 0) {
                    node->color.a += 0x10;
                    if (node->color.a >= 0xC8) {
                        node->color.a = 0xC8;
                        node->time++;
                    }
                } else if (node->color.a != 0) {
                    node->color.a -= 4;
                } else {
                    node->time++;
                }
                alive++;
                node->scale = (f32)((f64)node->scale + 1.6666666666666667);
                prevVy = node->vel.y;
                node->vel.y = (f32)((f64)node->vel.y + -0.27240694444444447);
                node->pos.x = (f32)((f64)node->pos.x + (0.016666666666666666 * (f64)(100.0f * (1.5f * node->vel.x))));
                node->pos.y = node->pos.y + (0.5f * (prevVy + node->vel.y));
                node->pos.z = (f32)((f64)node->pos.z + (0.016666666666666666 * (f64)(100.0f * (1.5f * node->vel.z))));
            }
        }
        DCStoreRangeNoSync(particle->data, particle->maxCnt * sizeof(HU3DPARTICLEDATA));
        if (alive == 0) {
            Hu3DModelAttrSet(arg0->modelId, 1);
            arg0->done = 1;
        }
    }
}

}
