#include "dolphin.h"
#include <math.h>
#include "game/data.h"
#include "game/hu3d.h"
#include "game/sprite.h"
#include "REL/m579Dll/m579types.h"

extern "C" {
void *__register_global_object(void *object, void *destructor, void *regmem);
void fn_1_A680(void *mem, Vec *pos, f32 size);
extern u8 lbl_1_bss_C[];
extern u8 lbl_1_data_1FC[];
extern void *lbl_1_bss_A04;
}

/* Splash particle effect object. */
typedef struct M579Splash {
    M579Node node;
    s16 model;
    s32 count;
} M579Splash;

typedef struct M579EffMgr {
    M579Base base;
} M579EffMgr;

extern "C" {
void *fn_1_BA70(M579EffMgr *self);
M579EffMgr *fn_1_BB74(M579EffMgr *self, s16 flag);
M579EffMgr *fn_1_BC00(M579EffMgr *self, s16 flag);
void fn_1_BCE0(M579EffMgr *self, s32 bit);
void fn_1_BCF8(void);
M579Splash *fn_1_BD18(M579Splash *self, Vec *pos, s32 type);
M579Splash *fn_1_BE78(M579Splash *self, s16 flag);
M579Splash *fn_1_BED0(M579Splash *self, s16 flag);
void fn_1_BF38(void);
void fn_1_C008(void);
void fn_1_C038(M579Splash *self);
void fn_1_C0A0(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);
void fn_1_C2F0(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);
void fn_1_C534(M579Node *list, void (*fn)(M579Splash *));
void fn_1_C594(M579Node *first, M579Node *list, void (*fn)(M579Splash *));
void fn_1_C5F4(void);
M579Splash *fn_1_C67C(M579Splash *self, s16 flag);
}

char lbl_1_data_3B8[16] = "ms_pointer == 0";
char lbl_1_data_3C8[12] = "singleton.h";
char lbl_1_data_3D4[16] = "ms_pointer != 0";
void *lbl_1_data_3E4[5] = { 0, 0, (void *) fn_1_BC00, (void *) fn_1_BCE0, (void *) fn_1_BCF8 };

s32 lbl_1_bss_A90;
s32 lbl_1_bss_A94;
s32 lbl_1_bss_A98;
ANIMDATA *lbl_1_bss_A9C;
u8 lbl_1_bss_AA0[12];
M579Node lbl_1_bss_AAC;

void *fn_1_BA70(M579EffMgr *self)
{
    fn_1_1264(self);
    self->base.vtbl = (void **) lbl_1_data_1FC;
    self->base.flags = 0;
    fn_1_1330(self, lbl_1_bss_C);
    lbl_1_bss_A04 == 0 ? (void) 0 : fn_1_1D4(lbl_1_data_3B8, lbl_1_data_3C8, 0x35);
#ifdef TARGET_PC
    lbl_1_bss_A04 = self;
#else
    {
        u8 *q = (u8 *) self + 0x10;
        if (q != 0) {
            q -= 0x10;
        }
        lbl_1_bss_A04 = q;
    }
#endif
    self->base.vtbl = lbl_1_data_3E4;
    lbl_1_bss_A9C = HuSprAnimRead(HuDataSelHeapReadNum(0x850003, 0x10000000, HEAP_MODEL));
    HuSprAnimLock(lbl_1_bss_A9C);
    return self;
}

M579EffMgr *fn_1_BB74(M579EffMgr *self, s16 flag)
{
    if (self != 0) {
        lbl_1_bss_A04 != 0 ? (void) 0 : fn_1_1D4(lbl_1_data_3D4, lbl_1_data_3C8, 0x3D);
        lbl_1_bss_A04 = 0;
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M579EffMgr *fn_1_BC00(M579EffMgr *self, s16 flag)
{
    if (self != 0) {
        self->base.vtbl = lbl_1_data_3E4;
        fn_1_BF38();
        HuSprAnimKill(lbl_1_bss_A9C);
        if (self != 0) {
            lbl_1_bss_A04 != 0 ? (void) 0 : fn_1_1D4(lbl_1_data_3D4, lbl_1_data_3C8, 0x3D);
            lbl_1_bss_A04 = 0;
        }
        if (self != 0) {
            self->base.vtbl = (void **) lbl_1_data_1FC;
            if (self != 0) {
                fn_1_1270(self, 0);
            }
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_BCE0(M579EffMgr *self, s32 bit)
{
    self->base.flags = self->base.flags & ~(1 << bit);
}

void fn_1_BCF8(void)
{
    fn_1_C008();
}

M579Splash *fn_1_BD18(M579Splash *self, Vec *pos, s32 type)
{
    s32 size;
    void *mem;

    fn_1_1264(self);
    self->count = 0;
    fn_1_1330(self, &lbl_1_bss_AAC);
    self->model = Hu3DParticleCreate(lbl_1_bss_A9C, 0x20);
    Hu3DModelPosSetV(self->model, pos);
    Hu3DModelLayerSet(self->model, 5);
    if (type == 0) {
        Hu3DParticleScaleSet(self->model, 20.0f);
        Hu3DParticleHookSet(self->model, fn_1_C0A0);
    } else {
        Hu3DParticleScaleSet(self->model, 30.0f);
        Hu3DParticleHookSet(self->model, fn_1_C2F0);
    }
    Hu3DParticleTPLvlSet(self->model, 0.0f);
    Hu3DParticleBlendModeSet(self->model, 0);
    mem = fn_1_0(0x24);
    if (mem != 0) {
        if (type == 0) {
            size = 0xC8;
        } else {
            size = 0x190;
        }
        fn_1_A680(mem, pos, (f32) size);
    }
    return self;
}

M579Splash *fn_1_BE78(M579Splash *self, s16 flag)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M579Splash *fn_1_BED0(M579Splash *self, s16 flag)
{
    if (self != 0) {
        Hu3DModelKill(self->model);
        if (self != 0) {
            fn_1_1270(self, 0);
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_BF38(void)
{
    M579Splash *p;
    u8 empty;

    for (;;) {
        empty = 0;
        if (lbl_1_bss_AAC.next == &lbl_1_bss_AAC && lbl_1_bss_AAC.next == lbl_1_bss_AAC.prev) {
            empty = 1;
        }
        if (empty != 0) {
            break;
        }
        p = (M579Splash *) lbl_1_bss_AAC.prev;
        if (p != 0) {
            Hu3DModelKill(p->model);
            if (p != 0) {
                fn_1_1270(p, 0);
            }
            fn_1_C8(p);
        }
    }
}

void fn_1_C008(void)
{
    fn_1_C534(&lbl_1_bss_AAC, fn_1_C038);
}

void fn_1_C038(M579Splash *self)
{
    self->count = self->count + 1;
    if (self->count > 0x40 && self != 0) {
        Hu3DModelKill(self->model);
        if (self != 0) {
            fn_1_1270(self, 0);
        }
        fn_1_C8(self);
    }
}

void fn_1_C0A0(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx)
{
    void *hookData;
    HU3DPARTICLEDATA *end;
    HU3DPARTICLEDATA *p;
    HU3DPARTICLEDATA *q;
    f32 a;
    f32 c;
    f32 s;
    s16 n;
    s32 alpha;

    end = &particleP->data[particleP->maxCnt];
    n = 0;
    if ((u32) particleP->count == 0) {
        hookData = modelP->hookData;
        for (p = particleP->data; p != end; p++) {
            a = (f32) ((3.141592653589793 * (f64) ((360.0f * fn_1_4D1C()) - 180.0f)) / 180.0);
            c = (f32) cos((f64) a);
            s = (f32) sin((f64) a);
            p->pos.x = c * 10.0f;
            p->pos.y = 10.0f;
            p->pos.z = s * 10.0f;
            p->vel.x = 3.0f * c;
            p->vel.y = (f32) (10.89627777777778 * (0.5 + (0.5 * fn_1_4D1C())));
            p->vel.z = 3.0f * s;
            p->color.r = 0xE0;
            p->color.g = 0xF0;
            p->color.b = 0xFF;
            p->color.a = 0xFF;
        }
    }
    for (q = particleP->data; q != end; q++) {
        q->vel.y = (f32) ((f64) q->vel.y - 0.27240694444444447);
        PSVECScale(&q->vel, &q->vel, 0.9f);
        PSVECAdd(&q->vel, &q->pos, &q->pos);
        alpha = q->color.a - 4;
        if (alpha < 0) {
            alpha = 0;
            q->scale = 0.0f;
        }
        q->color.a = (u8) alpha;
        n += 1;
    }
    particleP->emitCnt = n;
}

void fn_1_C2F0(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx)
{
    HU3DPARTICLEDATA *end;
    HU3DPARTICLEDATA *p;
    HU3DPARTICLEDATA *q;
    f32 a;
    f32 c;
    f32 s;
    s16 n;
    s32 alpha;

    end = &particleP->data[particleP->maxCnt];
    n = 0;
    if ((u32) particleP->count == 0) {
        for (p = particleP->data; p != end; p++) {
            a = (f32) ((3.141592653589793 * (f64) ((360.0f * fn_1_4D1C()) - 180.0f)) / 180.0);
            c = (f32) cos((f64) a);
            s = (f32) sin((f64) a);
            p->pos.x = c * 30.0f;
            p->pos.y = 15.0f;
            p->pos.z = s * 30.0f;
            p->vel.x = 3.0f * c;
            p->vel.y = (f32) (10.89627777777778 * (0.5 + (0.5 * fn_1_4D1C())));
            p->vel.z = 3.0f * s;
            p->color.r = 0xE0;
            p->color.g = 0xF0;
            p->color.b = 0xFF;
            p->color.a = 0xFF;
        }
    }
    for (q = particleP->data; q != end; q++) {
        q->vel.y = (f32) ((f64) q->vel.y - 0.27240694444444447);
        PSVECScale(&q->vel, &q->vel, 0.9f);
        PSVECAdd(&q->vel, &q->pos, &q->pos);
        alpha = q->color.a - 4;
        if (alpha < 0) {
            alpha = 0;
            q->scale = 0.0f;
        }
        q->color.a = (u8) alpha;
        n += 1;
    }
    particleP->emitCnt = n;
}

void fn_1_C534(M579Node *list, void (*fn)(M579Splash *))
{
    M579Node *l;
    M579Node *n;

    l = list;
    n = l->next;
    while (n != l) {
        fn((M579Splash *) n);
        n = n->next;
    }
}

void fn_1_C594(M579Node *first, M579Node *list, void (*fn)(M579Splash *))
{
    M579Node *n;

    n = first;
    while (n != list) {
        fn((M579Splash *) n);
        n = n->next;
    }
}

void fn_1_C5F4(void)
{
    M579Node *a;
    M579Node *b;

    lbl_1_bss_A90 = 0x78;
    lbl_1_bss_A94 = 0xD2;
    lbl_1_bss_A98 = 0xD2;
    a = &lbl_1_bss_AAC;
    fn_1_1264(a);
    b = a;
#ifndef TARGET_PC
    __register_global_object(b, (void *) fn_1_C67C, lbl_1_bss_AA0);
#endif
}

M579Splash *fn_1_C67C(M579Splash *self, s16 flag)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}
