#include "REL/m501Dll.h"
#include "datanum/m501.h"
#include "game/hu3d.h"
#include "game/sprite.h"
#include "game/data.h"
#include <math.h>

extern "C" {
void __dl__FPv(void *ptr);
}

#ifdef __MWERKS__
#define M501_RODATA __declspec(section ".rodata")
#else
#define M501_RODATA
#endif

namespace m501 {

static ANIMDATA *s_anim;

CParticle *CParticleSingleton::ms_pointer = 0;

CParticle::CParticle() :
    m_model((u32)DATANUM(DATA_m501, 7))
{
#ifdef TARGET_PC
    ms_pointer = this;
#endif
    s_anim = HuSprAnimRead(HuDataSelHeapReadNum(DATANUM(DATA_m501, 8), 0x10000000, HEAP_MODEL));
    init__Q24m5017CAsiatoManFv();
}

extern "C" void *fn_1_A4B0(void *self, s16 flag)
{
    if (self != (void *)0) {
        if (CParticleSingleton::ms_pointer == 0) {
            __assert_fail("ms_pointer != 0", "singleton.h", 0x3D);
        }
        CParticleSingleton::ms_pointer = (CParticle *)0;
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

CParticle::~CParticle()
{
    close__Q24m5017CAsiatoManFv();
}

void CParticle::on_new_mode(int mode)
{
    m_execMode &= ~(1 << mode);
}

void CParticle::on_update()
{
    update__Q24m5017CAsiatoManFv();
}

CModel *CParticle::getInstance()
{
    if (CParticleSingleton::ms_pointer != 0) {
    } else {
        __assert_fail("ms_pointer != 0", "singleton.h", 0x27);
    }
    CParticle *a = CParticleSingleton::ms_pointer;
    CParticle *b = a;
    return &b->m_model;
}

static M501_RODATA const u8 s_colors[8][3] = {
    { 0xF9, 0xB5, 0xCD }, { 0xF1, 0xD4, 0xA3 }, { 0x6B, 0xC0, 0xB7 }, { 0x94, 0x61, 0x3E },
    { 0x74, 0xE4, 0x82 }, { 0xF0, 0xDF, 0x6F }, { 0xFF, 0xB0, 0x6C }, { 0xF1, 0xD4, 0xA3 },
};

extern "C" void fn_1_A810(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);
extern "C" void fn_1_AAA4(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);

/* the colour pointer rides in the model's hookData */
#ifdef TARGET_PC
#define PARTICLE_COLOR_SLOT(id) (Hu3DData[id].hookData)
#else
#define PARTICLE_COLOR_SLOT(id) (*(void **)&Hu3DData[id].motShapeWork.start)
#endif

extern "C" void fn_1_A6C0(Vec *pos, s32 colorIdx)
{
    HU3DMODELID id = Hu3DParticleCreate(s_anim, 0x20);
    Hu3DModelPosSetV(id, (HuVecF *)pos);
    Hu3DParticleScaleSet(id, 1.0f);
    Hu3DParticleHookSet(id, fn_1_A810);
    PARTICLE_COLOR_SLOT(id) = (void *)s_colors[colorIdx];
}

extern "C" void fn_1_A768(Vec *pos, s32 colorIdx)
{
    HU3DMODELID id = Hu3DParticleCreate(s_anim, 0x20);
    Hu3DModelPosSetV(id, (HuVecF *)pos);
    Hu3DParticleScaleSet(id, 1.0f);
    Hu3DParticleHookSet(id, fn_1_AAA4);
    PARTICLE_COLOR_SLOT(id) = (void *)s_colors[colorIdx];
}

extern "C" void fn_1_A810(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx)
{
    HU3DPARTICLEDATA *end = &particleP->data[particleP->maxCnt];
    HU3DPARTICLEDATA *p;
    s16 alive;

    if (particleP->count == 0) {
        const u8 *color = (const u8 *)PARTICLE_COLOR_SLOT(modelP - Hu3DData);
        for (p = particleP->data; p != end; p++) {
            f32 ang, spd;
            p->pos.x = 0.0f;
            p->pos.y = 0.0f;
            p->pos.z = 0.0f;
            ang = CRandom::frandom(-3.1415927f, 3.1415927f);
            spd = CRandom::frandom(10.0f, 20.0f);
            p->vel.x = spd * sin(ang);
            p->vel.y = CRandom::frandom(10.0f, 20.0f);
            p->vel.z = spd * cos(ang);
            p->color.r = color[0];
            p->color.g = color[1];
            p->color.b = color[2];
            p->scale = 20.0f;
        }
    }
    alive = 0;
    for (p = particleP->data; p != end; p++) {
        if (p->scale != 0.0f) {
            PSVECAdd((Vec *)&p->vel, (Vec *)&p->pos, (Vec *)&p->pos);
            p->vel.x *= 0.999f;
            p->vel.y -= 3.0f;
            p->vel.z *= 0.999f;
            if (p->pos.y <= 1.0f) {
                p->pos.y = 1.0f;
                p->vel.y *= -0.8f;
            }
            p->scale -= 1.25f;
            if (p->scale <= 0.0f) {
                p->scale = 0.0f;
            }
            alive++;
        }
    }
    particleP->emitCnt = alive;
}

extern "C" void fn_1_AAA4(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx)
{
    HU3DPARTICLEDATA *end = &particleP->data[particleP->maxCnt];
    HU3DPARTICLEDATA *p;
    s16 alive;

    if (particleP->count == 0) {
        const u8 *color = (const u8 *)PARTICLE_COLOR_SLOT(modelP - Hu3DData);
        for (p = particleP->data; p != end; p++) {
            f32 ang, spd;
            p->pos.x = 0.0f;
            p->pos.y = 0.0f;
            p->pos.z = 0.0f;
            ang = CRandom::frandom(-3.1415927f, 3.1415927f);
            spd = CRandom::frandom(10.0f, 20.0f);
            p->vel.x = spd * sin(ang);
            p->vel.z = spd * cos(ang);
            p->vel.y = 0.0f;
            p->color.r = color[0];
            p->color.g = color[1];
            p->color.b = color[2];
            p->scale = 20.0f;
        }
    }
    alive = 0;
    for (p = particleP->data; p != end; p++) {
        if (p->scale != 0.0f) {
            PSVECAdd((Vec *)&p->vel, (Vec *)&p->pos, (Vec *)&p->pos);
            p->vel.x *= 0.999f;
            p->vel.z *= 0.999f;
            p->scale -= 2.0f;
            if (p->scale <= 0.0f) {
                p->scale = 0.0f;
            }
            alive++;
        }
    }
    particleP->emitCnt = alive;
}

} // namespace m501
