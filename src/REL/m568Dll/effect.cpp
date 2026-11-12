#define _MATH_H /* the original object has no math.h sqrt literals */
#include "types.h"
#include "game/hu3d.h"
#include "game/animdata.h"
#include "game/sprite.h"
#include "REL/m568Dll.h"

/*
 * effect.cpp: the weather/dust effect owner (a singleton with a sprite and a
 * snow particle emitter) plus two lists of short-lived puff objects, each
 * puff driving one particle model.
 */

struct EffectMgr {
    M568Link node;       // 0x00
    void **vtbl;         // 0x08
    u32 flags;           // 0x0C
    HU3DMODELID modelId; // 0x10
    s32 frame;           // 0x14
};

struct EffectPuff {
    M568Link node;       // 0x00
    HU3DMODELID modelId; // 0x08
    s32 flags;           // 0x0C
};

extern "C" {

void __register_global_object(void *obj, void *dtor, void *reg);
void *fn_1_1948(void *obj, void *list);
s32 fn_1_2548(void);
f32 fn_1_5334(void);
double sin(double x);
double cos(double x);

extern void *lbl_1_data_23C[4];
extern M568Link lbl_1_bss_C;

void *fn_1_B840(EffectMgr *self, s16 flag);
void fn_1_B9E4(EffectMgr *self, s32 bit);
void fn_1_BA04(EffectMgr *self);
void fn_1_BC9C(EffectMgr *self);
void fn_1_BD28(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);
void fn_1_C234(void);
void fn_1_C304(void);
void fn_1_C334(EffectPuff *self);
void fn_1_C3B4(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);
void fn_1_C788(void);
void fn_1_C858(void);
void fn_1_C888(EffectPuff *self);
void fn_1_C908(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);
void fn_1_CB5C(M568Link *head, void (*volatile fn)(EffectPuff *));
void fn_1_CBBC(M568Link *head, void (*volatile fn)(EffectPuff *));
void fn_1_CC1C(M568Link *first, M568Link *end, void (*fn)(EffectPuff *));
void fn_1_CC7C(M568Link *first, M568Link *end, void (*fn)(EffectPuff *));
void *fn_1_CD70(void *self, s16 flag);
void *fn_1_CDC8(void *self, s16 flag);

char lbl_1_data_3F8[] = "ms_pointer == 0";
char lbl_1_data_408[] = "singleton.h";
char lbl_1_data_414[] = "ms_pointer != 0";
HuVecF lbl_1_data_424 = { 0.0f, 0.0f, 0.0f };
void *lbl_1_data_430[6] = { 0, 0, (void *)fn_1_B840, (void *)fn_1_B9E4, (void *)fn_1_BA04, 0 };

ANIMDATA *lbl_1_bss_A28;
ANIMDATA *lbl_1_bss_A2C;
s32 lbl_1_bss_A30;
s32 lbl_1_bss_A34;
char lbl_1_bss_A38[12];
M568Link lbl_1_bss_A44;
char lbl_1_bss_A4C[12];
M568Link lbl_1_bss_A58;

// Level 0..1 of the snow emitter, as a share of the particle count.
static inline void SetSnowLevel(f32 v)
{
    f32 r;
    if (v < 0.0f) {
        r = 0.0f;
    } else if (v > 1.0f) {
        r = 1.0f;
    } else {
        r = v;
    }
    lbl_1_bss_A34 = (s32)((f32)lbl_1_bss_A30 * r);
}

EffectMgr *fn_1_B668(EffectMgr *self)
{
    EffectMgr *s;

    fn_1_187C(self);
    self->vtbl = lbl_1_data_23C;
    self->flags = 0;
    fn_1_1948(self, &lbl_1_bss_C);
    if (lbl_1_bss_A10 == 0) {
    } else {
        fn_1_7EC(lbl_1_data_3F8, lbl_1_data_408, 0x35);
    }
    s = self;
    lbl_1_bss_A10 = s;
    self->vtbl = lbl_1_data_430;
    lbl_1_bss_A28 = HuSprAnimRead(HuDataSelHeapReadNum(0x7B0016, 0x10000000, HEAP_MODEL));
    lbl_1_bss_A2C = HuSprAnimRead(HuDataSelHeapReadNum(0x7B0015, 0x10000000, HEAP_MODEL));
    HuSprAnimLock(lbl_1_bss_A28);
    lbl_1_bss_A34 = 0x200;
    lbl_1_bss_A30 = 0x200;
    fn_1_BC9C(self);
    return self;
}

EffectMgr *fn_1_B7B4(EffectMgr *self, s16 flag)
{
    if (self != 0) {
        if (lbl_1_bss_A10 != 0) {
        } else {
            fn_1_7EC(lbl_1_data_414, lbl_1_data_408, 0x3D);
        }
        lbl_1_bss_A10 = 0;
        if (flag > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

void *fn_1_B840(EffectMgr *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = lbl_1_data_430;
        Hu3DModelKill(self->modelId);
        fn_1_C234();
        fn_1_C788();
        HuSprAnimKill(lbl_1_bss_A28);
        if (self != 0) {
            if (lbl_1_bss_A10 != 0) {
            } else {
                fn_1_7EC(lbl_1_data_414, lbl_1_data_408, 0x3D);
            }
            lbl_1_bss_A10 = 0;
        }
        if (self != 0) {
            self->vtbl = lbl_1_data_23C;
            if (self != 0) {
                fn_1_1888(self, 0);
            }
        }
        if (flag > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

void fn_1_B92C(f32 v)
{
    SetSnowLevel(v);
}

void fn_1_B9E4(EffectMgr *self, s32 bit)
{
    self->flags &= ~(1 << bit);
    self->frame = 0;
}

void fn_1_BA04(EffectMgr *self)
{
    s32 lo, hi;
    f32 num, den, t, r1, r2, v2;

    if (fn_1_2548() == 2) {
        lo = 0;
        hi = 0x384;
        if (self->frame >= lo && self->frame <= hi) {
            num = (f32)(self->frame - lo);
            den = (f32)(hi - lo);
            t = 1.0 - num / den;
            if (t < 0.0f) {
                r1 = 0.0f;
            } else if (t > 1.0f) {
                r1 = 1.0f;
            } else {
                r1 = t;
            }
            lbl_1_bss_A34 = (s32)((f32)lbl_1_bss_A30 * r1);
        }
    } else if (fn_1_2548() == 3) {
        v2 = 0.2f;
        if (v2 < 0.0f) {
            r2 = 0.0f;
        } else if (v2 > 1.0f) {
            r2 = 1.0f;
        } else {
            r2 = v2;
        }
        lbl_1_bss_A34 = (s32)((f32)lbl_1_bss_A30 * r2);
    }
    fn_1_C304();
    fn_1_C858();
    self->frame++;
}

void fn_1_BC9C(EffectMgr *self)
{
    self->modelId = Hu3DParticleCreate(lbl_1_bss_A2C, (s16)lbl_1_bss_A30);
    Hu3DModelPosSetV(self->modelId, &lbl_1_data_424);
    Hu3DModelLayerSet(self->modelId, 7);
    Hu3DParticleScaleSet(self->modelId, 20.0f);
    Hu3DParticleHookSet(self->modelId, (HU3DPARTICLEHOOK)fn_1_BD28);
}

// Snow: respawns flakes above the camera and falls them downward.
void fn_1_BD28(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx)
{
    Vec cam, up, target;
    HU3DPARTICLEDATA *end;
    HU3DPARTICLEDATA *p;
    s16 n;

    Hu3DCameraPosGet(1, &cam, &up, &target);
    end = &particleP->data[particleP->maxCnt];
    if (particleP->count == 1) {
        for (p = particleP->data; p != end; p++) {
            p->pos.x = cam.x + 1500.0f * (2.0f * (fn_1_5334() - 0.5f));
            p->pos.y = cam.y * fn_1_5334();
            p->pos.z = cam.z - 6000.0f * fn_1_5334() - 1000.0f;
            p->vel.x = 3.0f * (2.0f * (fn_1_5334() - 0.5f));
            p->vel.y = -10.0f - 3.0f * fn_1_5334();
            p->vel.z = 3.0f * (2.0f * (fn_1_5334() - 0.5f));
            p->color.r = 0xE0;
            p->color.g = 0xE8;
            p->color.b = 0xFF;
        }
    }
    if (particleP->count != 0) {
        n = 0;
        for (p = particleP->data; p != end; p++) {
            PSVECAdd(&p->vel, &p->pos, &p->pos);
            if (p->pos.y <= 0.0f || p->pos.z > cam.z) {
                p->pos.x = cam.x + 1500.0f * (2.0f * (fn_1_5334() - 0.5f));
                p->pos.y = cam.y + 200.0f * fn_1_5334();
                p->pos.z = cam.z - 6000.0f * fn_1_5334() - 1000.0f;
                p->vel.x = 3.0f * (2.0f * (fn_1_5334() - 0.5f));
                p->vel.y = -10.0f - 3.0f * fn_1_5334();
                p->vel.z = 3.0f * (2.0f * (fn_1_5334() - 0.5f));
            }
            if (lbl_1_bss_A34 > n) {
                p->scale = 20.0f;
            } else {
                p->scale = 0.0f;
            }
            n++;
        }
        particleP->emitCnt = n;
        DCStoreRangeNoSync(particleP->data, particleP->maxCnt * sizeof(HU3DPARTICLEDATA));
    }
}

EffectPuff *fn_1_C0A8(EffectPuff *self, HuVecF *pos, f32 scale)
{
    fn_1_187C(self);
    fn_1_1948(self, &lbl_1_bss_A44);
    self->modelId = Hu3DParticleCreate(lbl_1_bss_A28, 2);
    Hu3DModelPosSetV(self->modelId, pos);
    Hu3DModelLayerSet(self->modelId, 6);
    Hu3DParticleScaleSet(self->modelId, scale);
    Hu3DParticleHookSet(self->modelId, (HU3DPARTICLEHOOK)fn_1_C3B4);
    Hu3DParticleTPLvlSet(self->modelId, 0.0f);
    Hu3DParticleBlendModeSet(self->modelId, 0);
    self->flags = 0;
    return self;
}

void *fn_1_C174(EffectPuff *self, s16 flag)
{
    if (self != 0) {
        fn_1_1888(self, 0);
        if (flag > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

void *fn_1_C1CC(EffectPuff *self, s16 flag)
{
    if (self != 0) {
        Hu3DModelKill(self->modelId);
        if (self != 0) {
            fn_1_1888(self, 0);
        }
        if (flag > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

// Kills every puff of list A44.
void fn_1_C234(void)
{
    EffectPuff *p;
    u8 empty;

    while (1) {
        empty = 0;
        if (lbl_1_bss_A44.prev == &lbl_1_bss_A44 && lbl_1_bss_A44.prev == lbl_1_bss_A44.next) {
            empty = 1;
        }
        if (empty != 0) {
            break;
        }
        p = (EffectPuff *)lbl_1_bss_A44.next;
        if (p != 0) {
            Hu3DModelKill(p->modelId);
            if (p != 0) {
                fn_1_1888(p, 0);
            }
            fn_1_6E0(p);
        }
    }
}

void fn_1_C304(void)
{
    fn_1_CB5C(&lbl_1_bss_A44, fn_1_C334);
}

void fn_1_C334(EffectPuff *self)
{
    if (*(s16 *)Hu3DData[self->modelId].hookData < 0 && self != 0) {
        Hu3DModelKill(self->modelId);
        if (self != 0) {
            fn_1_1888(self, 0);
        }
        fn_1_6E0(self);
    }
}

// Small ring of puffs spreading out from the emitter.
void fn_1_C3B4(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx)
{
    HU3DPARTICLEDATA *end = &particleP->data[particleP->maxCnt];
    HU3DPARTICLEDATA *p;
    s16 n = 0;
    s32 a;
    f32 ang;

    if (particleP->count == 0) {
        for (p = particleP->data; p != end; p++) {
            ang = 3.141592653589793 * (360.0f * fn_1_5334() - 180.0f) / 180.0;
            p->vel.x = cos(ang);
            p->vel.y = 0.0f;
            p->vel.z = sin(ang);
            p->pos.x = p->vel.x * p->scale;
            p->pos.y = 50.0f;
            p->pos.z = p->vel.z * p->scale;
            p->vel.x *= 5.0f;
            p->vel.z *= 5.0f;
            p->color.r = 0xE0;
            p->color.g = 0xE8;
            p->color.b = 0xFF;
            p->color.a = 0x80;
            p->scale = 100.0f;
        }
    }
    for (p = particleP->data; p != end; p++) {
        PSVECAdd(&p->vel, &p->pos, &p->pos);
        p->vel.x = p->vel.x * 0.95;
        p->vel.y = p->vel.y * 0.95;
        p->vel.z = p->vel.z * 0.95;
        a = p->color.a - 4;
        if (a < 0) {
            a = 0;
            p->scale = 0.0f;
            particleP->dataCnt = -1;
        }
        p->color.a = a;
        n++;
    }
    particleP->emitCnt = n;
    DCStoreRangeNoSync(particleP->data, particleP->maxCnt * sizeof(HU3DPARTICLEDATA));
}

EffectPuff *fn_1_C5F8(EffectPuff *self, HuVecF *pos)
{
    fn_1_187C(self);
    fn_1_1948(self, &lbl_1_bss_A58);
    self->modelId = Hu3DParticleCreate(lbl_1_bss_A28, 0x20);
    Hu3DModelPosSetV(self->modelId, pos);
    Hu3DModelLayerSet(self->modelId, 6);
    Hu3DParticleScaleSet(self->modelId, 0.0f);
    Hu3DParticleHookSet(self->modelId, (HU3DPARTICLEHOOK)fn_1_C908);
    Hu3DParticleTPLvlSet(self->modelId, 0.0f);
    Hu3DParticleBlendModeSet(self->modelId, 0);
    self->flags = 0;
    return self;
}

void *fn_1_C6C8(EffectPuff *self, s16 flag)
{
    if (self != 0) {
        fn_1_1888(self, 0);
        if (flag > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

void *fn_1_C720(EffectPuff *self, s16 flag)
{
    if (self != 0) {
        Hu3DModelKill(self->modelId);
        if (self != 0) {
            fn_1_1888(self, 0);
        }
        if (flag > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

// Kills every puff of list A58.
void fn_1_C788(void)
{
    EffectPuff *p;
    u8 empty;

    while (1) {
        empty = 0;
        if (lbl_1_bss_A58.prev == &lbl_1_bss_A58 && lbl_1_bss_A58.prev == lbl_1_bss_A58.next) {
            empty = 1;
        }
        if (empty != 0) {
            break;
        }
        p = (EffectPuff *)lbl_1_bss_A58.next;
        if (p != 0) {
            Hu3DModelKill(p->modelId);
            if (p != 0) {
                fn_1_1888(p, 0);
            }
            fn_1_6E0(p);
        }
    }
}

void fn_1_C858(void)
{
    fn_1_CBBC(&lbl_1_bss_A58, fn_1_C888);
}

void fn_1_C888(EffectPuff *self)
{
    if (*(s16 *)Hu3DData[self->modelId].hookData < 0 && self != 0) {
        Hu3DModelKill(self->modelId);
        if (self != 0) {
            fn_1_1888(self, 0);
        }
        fn_1_6E0(self);
    }
}

// Large ring of puffs, drifting upward.
void fn_1_C908(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx)
{
    HU3DPARTICLEDATA *end = &particleP->data[particleP->maxCnt];
    HU3DPARTICLEDATA *p;
    s16 n;
    s32 a;
    f32 ang;

    if (particleP->count == 0) {
        for (p = particleP->data; p != end; p++) {
            ang = 3.141592653589793 * (360.0f * fn_1_5334() - 180.0f) / 180.0;
            p->vel.x = cos(ang);
            p->vel.y = 0.0f;
            p->vel.z = sin(ang);
            p->pos.x = 100.0f * p->vel.x;
            p->pos.y = 50.0f;
            p->pos.z = 100.0f * p->vel.z;
            p->vel.x *= 10.0f;
            p->vel.z *= 10.0f;
            p->color.r = 0xE0;
            p->color.g = 0xE8;
            p->color.b = 0xFF;
            p->color.a = 0x80;
            p->scale = 150.0f;
        }
    }
    n = 0;
    for (p = particleP->data; p != end; p++) {
        PSVECAdd(&p->vel, &p->pos, &p->pos);
        p->vel.x = p->vel.x * 0.95;
        p->vel.y = p->vel.y + 0.2;
        p->vel.z = p->vel.z * 0.95;
        a = p->color.a - 4;
        if (a < 0) {
            a = 0;
            p->scale = 0.0f;
            particleP->dataCnt = -1;
        }
        p->color.a = a;
        n++;
    }
    particleP->emitCnt = n;
    DCStoreRangeNoSync(particleP->data, particleP->maxCnt * sizeof(HU3DPARTICLEDATA));
}

void fn_1_CB5C(M568Link *head, void (*volatile fn)(EffectPuff *))
{
    M568Link *h2 = head;
    M568Link *first = head->prev;
    M568Link *first2 = first;
    fn_1_CC7C(first2, h2, fn);
}

void fn_1_CBBC(M568Link *head, void (*volatile fn)(EffectPuff *))
{
    M568Link *h2 = head;
    M568Link *first = head->prev;
    M568Link *first2 = first;
    fn_1_CC1C(first2, h2, fn);
}

void fn_1_CC1C(M568Link *first, M568Link *end, void (*fn)(EffectPuff *))
{
    M568Link *p = first;
    while (p != end) {
        fn((EffectPuff *)p);
        p = p->prev;
    }
}

void fn_1_CC7C(M568Link *first, M568Link *end, void (*fn)(EffectPuff *))
{
    M568Link *p = first;
    while (p != end) {
        fn((EffectPuff *)p);
        p = p->prev;
    }
}

void fn_1_CCDC(void)
{
    M568Link *l = &lbl_1_bss_A44;
    M568Link *l2;
    M568Link *m;
    M568Link *m2;

    fn_1_187C(l);
    l2 = l;
    __register_global_object(l2, (void *)fn_1_CDC8, lbl_1_bss_A38);
    m = &lbl_1_bss_A58;
    fn_1_187C(m);
    m2 = m;
    __register_global_object(m2, (void *)fn_1_CD70, lbl_1_bss_A4C);
}

#ifdef __MWERKS__
__declspec(section ".ctors") void (*const effect_ctor)(void) = fn_1_CCDC;
#else
static struct EffectSinit { EffectSinit() { fn_1_CCDC(); } } effect_sinit;
#endif

void *fn_1_CD70(void *self, s16 flag)
{
    if (self != 0) {
        fn_1_1888(self, 0);
        if (flag > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

void *fn_1_CDC8(void *self, s16 flag)
{
    if (self != 0) {
        fn_1_1888(self, 0);
        if (flag > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

}
