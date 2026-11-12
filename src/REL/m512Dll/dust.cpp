// m512Dll/REL/m512Dll/dust
// Dust-puff particle objects: one CDust per puff, kept in a ring list; a
// singleton owner (fn_1_78A4) loads and locks the puff sprite.
#define _MATH_H
#include "types.h"
#include "game/hu3d.h"
#include "game/animdata.h"
#include "game/sprite.h"

// ring-list node: every object starts with one
struct DustNode {
    DustNode *next; // 0x00
    DustNode *prev; // 0x04
};

struct DustOwner {
    DustNode node;  // 0x00
    void **vtbl;    // 0x08
    s32 unkC;       // 0x0C
};

struct DustPuff {
    DustNode node;  // 0x00
    s16 modelId;    // 0x08
    s32 unkC;       // 0x0C
};

extern "C" {
void *__dl__FPv(void *);
void __register_global_object(void *obj, void *dtor, void *reg);
void *fn_1_1264(void *self);
void fn_1_1270(void *self, int flag);
void fn_1_1330(void *self, void *arg);
void fn_1_1D4(const char *msg, const char *file, int line);
f32 fn_1_4D1C(void);

extern DustNode lbl_1_bss_C;
extern void *lbl_1_data_1FC[4];
extern DustOwner *lbl_1_bss_A0C;
}

extern "C" void *fn_1_7A34(DustOwner *self, s16 flag);
extern "C" void fn_1_7B14(DustPuff *self, s32 bit);
extern "C" void fn_1_7B2C(void);
extern "C" void fn_1_7E5C(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);
extern "C" void fn_1_7CDC(void);
extern "C" void fn_1_8094(void);
extern "C" void fn_1_7DAC(void);
extern "C" void fn_1_7DDC(DustPuff *self);
extern "C" void fn_1_7FD4(DustNode *head, void (*fn)(DustPuff *));
extern "C" void fn_1_8034(DustNode *first, DustNode *end, void (*fn)(DustPuff *));
extern "C" void *fn_1_80EC(void *self, s16 flag);

// -------------------------------------------------------------------- data

char lbl_1_data_2F0[] = "ms_pointer == 0";
char lbl_1_data_300[] = "singleton.h";
char lbl_1_data_30C[] = "ms_pointer != 0";
void *lbl_1_data_31C[5] = { 0, 0, (void *)fn_1_7A34, (void *)fn_1_7B14, (void *)fn_1_7B2C };

ANIMDATA *lbl_1_bss_A28;
char lbl_1_bss_A2C[12];
DustNode lbl_1_bss_A38;


// ------------------------------------------------------------------ functions

extern "C" DustOwner *fn_1_78A4(DustOwner *self)
{
    fn_1_1264(self);
    self->vtbl = lbl_1_data_1FC;
    self->unkC = 0;
    fn_1_1330(self, &lbl_1_bss_C);
    if (lbl_1_bss_A0C == 0) {
        } else {
        fn_1_1D4(lbl_1_data_2F0, lbl_1_data_300, 0x35);
    }
    {
        DustOwner *s = (DustOwner *)((char *)self + 0x10);
        if (s != 0) {
            s = (DustOwner *)((char *)s - 0x10);
        }
        lbl_1_bss_A0C = s;
    }
    self->vtbl = lbl_1_data_31C;
    lbl_1_bss_A28 = HuSprAnimRead(HuDataSelHeapReadNum(0x460015, 0x10000000, HEAP_MODEL));
    HuSprAnimLock(lbl_1_bss_A28);
    return self;
}

extern "C" DustOwner *fn_1_79A8(DustOwner *self, s16 flag)
{
    if (self != 0) {
        if (lbl_1_bss_A0C != 0) {
            } else {
            fn_1_1D4(lbl_1_data_30C, lbl_1_data_300, 0x3D);
        }
        lbl_1_bss_A0C = 0;
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

extern "C" void *fn_1_7A34(DustOwner *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = lbl_1_data_31C;
        fn_1_7CDC();
        HuSprAnimKill(lbl_1_bss_A28);
        if ((void *)((char *)self + 0x10) != 0) {
            if (lbl_1_bss_A0C != 0) {
                } else {
                fn_1_1D4(lbl_1_data_30C, lbl_1_data_300, 0x3D);
            }
            lbl_1_bss_A0C = 0;
        }
        if (self != 0) {
            self->vtbl = lbl_1_data_1FC;
            if (self != 0) {
                fn_1_1270(self, 0);
            }
        }
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

extern "C" void fn_1_7B14(DustPuff *self, s32 bit)
{
    self->unkC &= ~(1 << bit);
}

extern "C" void fn_1_7B2C(void)
{
    fn_1_7DAC();
}

extern "C" DustPuff *fn_1_7B4C(DustPuff *self, HuVecF *pos)
{
    fn_1_1264(self);
    fn_1_1330(self, &lbl_1_bss_A38);
    self->modelId = Hu3DParticleCreate(lbl_1_bss_A28, 1);
    Hu3DModelPosSetV(self->modelId, pos);
    Hu3DModelLayerSet(self->modelId, 6);
    Hu3DParticleScaleSet(self->modelId, 30.0f);
    Hu3DParticleHookSet(self->modelId, (HU3DPARTICLEHOOK)fn_1_7E5C);
    Hu3DParticleTPLvlSet(self->modelId, 0.0f);
    Hu3DParticleBlendModeSet(self->modelId, 0);
    self->unkC = 0;
    return self;
}

extern "C" void *fn_1_7C1C(void *self, s16 flag)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

extern "C" DustPuff *fn_1_7C74(DustPuff *self, s16 flag)
{
    if (self != 0) {
        Hu3DModelKill(self->modelId);
        if (self != 0) {
            fn_1_1270(self, 0);
        }
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

extern "C" void fn_1_7CDC(void)
{
    DustPuff *p;
    u8 empty;

    while (1) {
        empty = 0;
        if (lbl_1_bss_A38.next == &lbl_1_bss_A38 && lbl_1_bss_A38.next == lbl_1_bss_A38.prev) {
            empty = 1;
        }
        if (empty != 0) {
            break;
        }
        p = (DustPuff *)lbl_1_bss_A38.prev;
        if (p != 0) {
            Hu3DModelKill(p->modelId);
            if (p != 0) {
                fn_1_1270(p, 0);
            }
            __dl__FPv(p);
        }
    }
}

extern "C" void fn_1_7DAC(void)
{
    fn_1_7FD4(&lbl_1_bss_A38, fn_1_7DDC);
}

extern "C" void fn_1_7DDC(DustPuff *self)
{
    if (*(s16 *)Hu3DData[self->modelId].hookData < 0 && self != 0) {
        Hu3DModelKill(self->modelId);
        if (self != 0) {
            fn_1_1270(self, 0);
        }
        __dl__FPv(self);
    }
}

extern "C" void fn_1_7E5C(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx)
{
    HU3DPARTICLEDATA *end = &particleP->data[particleP->maxCnt];
    HU3DPARTICLEDATA *p;
    s16 n = 0;
    s32 a;

    if (particleP->count == 0) {
        for (p = particleP->data; p != end; p++) {
            p->vel.x = 4.0f * (fn_1_4D1C() - 0.5f);
            p->vel.y = 5.0f + 2.0f * fn_1_4D1C();
            p->vel.z = 4.0f * (fn_1_4D1C() - 0.5f);
            p->pos.z = 0.0f;
            p->pos.y = 0.0f;
            p->pos.x = 0.0f;
            p->color.r = 0xFF;
            p->color.g = 0xFF;
            p->color.b = 0xFF;
            p->color.a = 0x80;
        }
    }
    for (p = particleP->data; p != end; p++) {
        PSVECAdd((Vec *)&p->vel, (Vec *)&p->pos, (Vec *)&p->pos);
        a = p->color.a - 12;
        if (a < 0) {
            a = 0;
            p->scale = 0.0f;
            particleP->dataCnt = -1;
        }
        p->color.a = a;
        n++;
    }
    particleP->emitCnt = n;
}

extern "C" void fn_1_7FD4(DustNode *head, void (*fn)(DustPuff *))
{
    DustNode *h2 = head;
    DustNode *first = head->next;
    DustNode *first2 = first;
    fn_1_8034(first2, h2, fn);
}

extern "C" void fn_1_8034(DustNode *first, DustNode *end, void (*fn)(DustPuff *))
{
    DustNode *p = first;
    while (p != end) {
        fn((DustPuff *)p);
        p = p->next;
    }
}

extern "C" void fn_1_8094(void)
{
    DustNode *l = &lbl_1_bss_A38;
    fn_1_1264(l);
    DustNode *l2 = l;
    __register_global_object(l2, (void *)fn_1_80EC, lbl_1_bss_A2C);
}

#ifdef __MWERKS__
__declspec(section ".ctors") void (*const dust_ctor)(void) = fn_1_8094;
#else
static struct DustSinit { DustSinit() { fn_1_8094(); } } dust_sinit;
#endif

extern "C" void *fn_1_80EC(void *self, s16 flag)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}
