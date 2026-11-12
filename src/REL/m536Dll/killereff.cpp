#include "REL/m536Dll.h"
#include <math.h>

extern "C" {

extern void *lbl_1_bss_A08;
extern void *lbl_1_data_1FC[];

struct M536EffMgr {
    M536_NODE m_node;   /* 0x00 */
    void **m_vptr;      /* 0x08 */
    s32 m_flag;         /* 0x0C */
};

// The list head the effect objects hang off; plain node.
typedef M536_NODE M536EffList;
typedef void (*M536EffFn)(M536_EFFOBJ *);

M536EffMgr *fn_1_754C(M536EffMgr *self, s16 freeIt);
void fn_1_7634(M536EffMgr *self, s32 bit);
void fn_1_764C(void);
void fn_1_7984(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);
void fn_1_7E74(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);
void fn_1_83D4(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx);
void fn_1_7804(void);
void fn_1_7CF4(void);
void fn_1_8254(void);
void fn_1_78D4(void);
void fn_1_7DC4(void);
void fn_1_8324(void);
void fn_1_7904(M536_EFFOBJ *eff);
void fn_1_7DF4(M536_EFFOBJ *eff);
void fn_1_8354(M536_EFFOBJ *eff);
void fn_1_867C(M536EffList *list, M536EffFn cb);
void fn_1_86DC(M536EffList *list, M536EffFn cb);
void fn_1_873C(M536EffList *list, M536EffFn cb);
void fn_1_879C(M536_EFFOBJ *node, M536EffList *stop, M536EffFn cb);
void fn_1_87FC(M536_EFFOBJ *node, M536EffList *stop, M536EffFn cb);
void fn_1_885C(M536_EFFOBJ *node, M536EffList *stop, M536EffFn cb);
void *fn_1_896C(void *self, s16 freeIt);
void *fn_1_89C4(void *self, s16 freeIt);
void *fn_1_8A1C(void *self, s16 freeIt);

void HuSprAnimKill(ANIMDATA *anim);

Vec lbl_1_data_314 = { 0.0f, 0.0f, 10.0f };
void *lbl_1_data_320[6] = { 0, 0, (void *)fn_1_754C, (void *)fn_1_7634, (void *)fn_1_764C, 0 };

ANIMDATA *lbl_1_bss_A28;
u8 lbl_1_bss_A2C[12];
M536EffList lbl_1_bss_A38;
u8 lbl_1_bss_A40[12];
M536EffList lbl_1_bss_A4C;
u8 lbl_1_bss_A54[12];
M536EffList lbl_1_bss_A60;

M536EffMgr *fn_1_73BC(M536EffMgr *self)
{
    M536EffMgr *ms;

    fn_1_1264(self);
    self->m_vptr = lbl_1_data_1FC;
    self->m_flag = 0;
    fn_1_1330(self, &lbl_1_bss_C);
    if (lbl_1_bss_A08 != 0) {
        fn_1_1D4("ms_pointer == 0", "singleton.h", 0x35);
    }
    ms = (M536EffMgr *)((u8 *)self + 0x10);
    if (ms != 0) {
        ms = (M536EffMgr *)((u8 *)ms - 0x10);
    }
    lbl_1_bss_A08 = ms;
    self->m_vptr = lbl_1_data_320;
    lbl_1_bss_A28 = HuSprAnimRead(HuDataSelHeapReadNum(0x5E000E, 0x10000000, HEAP_MODEL));
    HuSprAnimLock(lbl_1_bss_A28);
    return self;
}

M536EffMgr *fn_1_74C0(M536EffMgr *self, s16 freeIt)
{
    if (self != 0) {
        if (lbl_1_bss_A08 == 0) {
            fn_1_1D4("ms_pointer != 0", "singleton.h", 0x3D);
        }
        lbl_1_bss_A08 = NULL;
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M536EffMgr *fn_1_754C(M536EffMgr *self, s16 freeIt)
{
    if (self != 0) {
        self->m_vptr = lbl_1_data_320;
        fn_1_7804();
        fn_1_7CF4();
        fn_1_8254();
        HuSprAnimKill(lbl_1_bss_A28);
        if ((M536EffMgr *)((u8 *)self + 0x10) != 0) {
            if (lbl_1_bss_A08 == 0) {
                fn_1_1D4("ms_pointer != 0", "singleton.h", 0x3D);
            }
            lbl_1_bss_A08 = NULL;
        }
        if (self != 0) {
            self->m_vptr = lbl_1_data_1FC;
            if (self != 0) {
                fn_1_1270(self, 0);
            }
        }
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_7634(M536EffMgr *self, s32 bit)
{
    self->m_flag &= ~(1 << bit);
}

void fn_1_764C(void)
{
    fn_1_78D4();
    fn_1_7DC4();
    fn_1_8324();
}

// ---- spark burst (list A38)

M536_EFFOBJ *fn_1_7674(M536_EFFOBJ *self, Vec *pos)
{
    fn_1_1264(self);
    fn_1_1330(self, &lbl_1_bss_A38);
    self->mdl = Hu3DParticleCreate(lbl_1_bss_A28, 1);
    Hu3DModelPosSetV(self->mdl, (HuVecF *)pos);
    Hu3DModelLayerSet(self->mdl, 6);
    Hu3DParticleScaleSet(self->mdl, 100.0f);
    Hu3DParticleHookSet(self->mdl, fn_1_7984);
    Hu3DParticleTPLvlSet(self->mdl, 0.0f);
    Hu3DParticleBlendModeSet(self->mdl, 0);
    self->unkC = 0;
    return self;
}

M536_EFFOBJ *fn_1_7744(M536_EFFOBJ *self, s16 freeIt)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M536_EFFOBJ *fn_1_779C(M536_EFFOBJ *self, s16 freeIt)
{
    if (self != 0) {
        Hu3DModelKill(self->mdl);
        if (self != 0) {
            fn_1_1270(self, 0);
        }
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_7804(void)
{
    M536_NODE *back;
    M536_NODE *back2;
    M536_EFFOBJ *eff;

    while (M536ListEmpty(&lbl_1_bss_A38) == 0) {
        back = lbl_1_bss_A38.prev;
        back2 = back;
        eff = (M536_EFFOBJ *)back2;
        if (eff != 0) {
            Hu3DModelKill(eff->mdl);
            if (eff != 0) {
                fn_1_1270(eff, 0);
            }
            fn_1_C8(eff);
        }
    }
}

void fn_1_78D4(void)
{
    fn_1_867C(&lbl_1_bss_A38, fn_1_7904);
}

void fn_1_7904(M536_EFFOBJ *eff)
{
    if (((HU3DPARTICLE *)Hu3DData[eff->mdl].hookData)->dataCnt < 0 && eff != 0) {
        Hu3DModelKill(eff->mdl);
        if (eff != 0) {
            fn_1_1270(eff, 0);
        }
        fn_1_C8(eff);
    }
}

void fn_1_7984(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx)
{
    HU3DPARTICLEDATA *end;
    HU3DPARTICLEDATA *p;
    HU3DPARTICLEDATA *q;
    s16 cnt;
    s32 g;
    s32 b;
    s32 a;

    end = &particleP->data[particleP->maxCnt];
    cnt = 0;
    if (particleP->count == 0) {
        for (p = particleP->data; p != end; p++) {
            p->vel.x = 5.0f * (fn_1_4D1C() - 0.5f);
            p->vel.y = 5.0f * (fn_1_4D1C() - 0.5f);
            p->vel.z = 5.0f * (fn_1_4D1C() - 0.5f);
            p->pos.z = 0.0f;
            p->pos.y = 0.0f;
            p->pos.x = 0.0f;
            p->color.r = 0xFF;
            p->color.g = 0x80;
            p->color.b = 0;
            p->color.a = 0xFF;
        }
    }
    for (q = particleP->data; q != end; q++) {
        PSVECAdd(&q->vel, &q->pos, &q->pos);
        q->color.r = 0xFF;
        g = q->color.g + 0x20;
        if (g > 0xFF) {
            g = 0xFF;
        }
        q->color.g = g;
        b = q->color.b + 0x20;
        if (b > 0xFF) {
            b = 0xFF;
        }
        q->color.b = b;
        a = q->color.a - 8;
        if (a < 0) {
            a = 0;
            q->scale = 0.0f;
            particleP->dataCnt = -1;
        }
        q->color.a = a;
        cnt++;
    }
    particleP->emitCnt = cnt;
}

// ---- ring burst (list A4C)

M536_EFFOBJ *fn_1_7B3C(M536_EFFOBJ *self, Vec *pos, f32 rotY)
{
    fn_1_1264(self);
    fn_1_1330(self, &lbl_1_bss_A4C);
    self->mdl = Hu3DParticleCreate(lbl_1_bss_A28, 0x20);
    Hu3DModelPosSetV(self->mdl, (HuVecF *)pos);
    Hu3DModelRotSet(self->mdl, 0.0f, rotY, 0.0f);
    Hu3DModelLayerSet(self->mdl, 6);
    Hu3DParticleScaleSet(self->mdl, 0.0f);
    Hu3DParticleHookSet(self->mdl, fn_1_7E74);
    Hu3DParticleTPLvlSet(self->mdl, 0.0f);
    Hu3DParticleBlendModeSet(self->mdl, 0);
    self->unkC = 0;
    return self;
}

M536_EFFOBJ *fn_1_7C34(M536_EFFOBJ *self, s16 freeIt)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M536_EFFOBJ *fn_1_7C8C(M536_EFFOBJ *self, s16 freeIt)
{
    if (self != 0) {
        Hu3DModelKill(self->mdl);
        if (self != 0) {
            fn_1_1270(self, 0);
        }
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_7CF4(void)
{
    M536_NODE *back;
    M536_NODE *back2;
    M536_EFFOBJ *eff;

    while (M536ListEmpty(&lbl_1_bss_A4C) == 0) {
        back = lbl_1_bss_A4C.prev;
        back2 = back;
        eff = (M536_EFFOBJ *)back2;
        if (eff != 0) {
            Hu3DModelKill(eff->mdl);
            if (eff != 0) {
                fn_1_1270(eff, 0);
            }
            fn_1_C8(eff);
        }
    }
}

void fn_1_7DC4(void)
{
    fn_1_86DC(&lbl_1_bss_A4C, fn_1_7DF4);
}

void fn_1_7DF4(M536_EFFOBJ *eff)
{
    if (((HU3DPARTICLE *)Hu3DData[eff->mdl].hookData)->dataCnt < 0 && eff != 0) {
        Hu3DModelKill(eff->mdl);
        if (eff != 0) {
            fn_1_1270(eff, 0);
        }
        fn_1_C8(eff);
    }
}

void fn_1_7E74(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx)
{
    HU3DPARTICLEDATA *end;
    HU3DPARTICLEDATA *p;
    HU3DPARTICLEDATA *q;
    f32 ang;
    s16 cnt;
    s32 a;

    end = &particleP->data[particleP->maxCnt];
    if (particleP->count == 0) {
        for (p = particleP->data; p != end; p++) {
            ang = M_PI * (360.0f * fn_1_4D1C() - 180.0f) / 180.0;
            p->vel.x = 10.0 * cos(ang);
            p->vel.y = 10.0 * sin(ang);
            p->vel.z = 0.0f;
            p->pos.x = 0.0f;
            p->pos.y = 0.0f;
            p->pos.z = 0.0f;
            p->color.r = 0xE0;
            p->color.g = 0xE8;
            p->color.b = 0xFF;
            p->color.a = 0x80;
            p->scale = 150.0f;
        }
    }
    cnt = 0;
    for (q = particleP->data; q != end; q++) {
        PSVECAdd(&q->vel, &q->pos, &q->pos);
        q->vel.x = q->vel.x * 0.95;
        q->vel.y = q->vel.y * 0.95;
        q->vel.z = q->vel.z * 0.95;
        a = q->color.a - 4;
        if (a < 0) {
            a = 0;
            q->scale = 0.0f;
            particleP->dataCnt = -1;
        }
        q->color.a = a;
        cnt++;
    }
    particleP->emitCnt = cnt;
}

// ---- hit burst (list A60)

M536_EFFOBJ *fn_1_8098(M536_EFFOBJ *self, Vec *pos)
{
    fn_1_1264(self);
    fn_1_1330(self, &lbl_1_bss_A60);
    self->mdl = Hu3DParticleCreate(lbl_1_bss_A28, 0x20);
    Hu3DModelPosSetV(self->mdl, (HuVecF *)pos);
    Hu3DModelRotSet(self->mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelLayerSet(self->mdl, 6);
    Hu3DParticleScaleSet(self->mdl, 0.0f);
    Hu3DParticleHookSet(self->mdl, fn_1_83D4);
    Hu3DParticleTPLvlSet(self->mdl, 0.0f);
    Hu3DParticleBlendModeSet(self->mdl, 0);
    self->unkC = 0;
    return self;
}

M536_EFFOBJ *fn_1_8194(M536_EFFOBJ *self, s16 freeIt)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M536_EFFOBJ *fn_1_81EC(M536_EFFOBJ *self, s16 freeIt)
{
    if (self != 0) {
        Hu3DModelKill(self->mdl);
        if (self != 0) {
            fn_1_1270(self, 0);
        }
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_8254(void)
{
    M536_NODE *back;
    M536_NODE *back2;
    M536_EFFOBJ *eff;

    while (M536ListEmpty(&lbl_1_bss_A60) == 0) {
        back = lbl_1_bss_A60.prev;
        back2 = back;
        eff = (M536_EFFOBJ *)back2;
        if (eff != 0) {
            Hu3DModelKill(eff->mdl);
            if (eff != 0) {
                fn_1_1270(eff, 0);
            }
            fn_1_C8(eff);
        }
    }
}

void fn_1_8324(void)
{
    fn_1_873C(&lbl_1_bss_A60, fn_1_8354);
}

void fn_1_8354(M536_EFFOBJ *eff)
{
    if (((HU3DPARTICLE *)Hu3DData[eff->mdl].hookData)->dataCnt < 0 && eff != 0) {
        Hu3DModelKill(eff->mdl);
        if (eff != 0) {
            fn_1_1270(eff, 0);
        }
        fn_1_C8(eff);
    }
}

void fn_1_83D4(HU3DMODEL *modelP, HU3DPARTICLE *particleP, Mtx mtx)
{
    Vec v;
    Mtx m1;
    Mtx m2;
    HU3DPARTICLEDATA *end;
    HU3DPARTICLEDATA *p;
    HU3DPARTICLEDATA *q;
    f32 a;
    f32 b;
    s16 cnt;
    s32 r;
    s32 g;
    s32 al;

    end = &particleP->data[particleP->maxCnt];
    if (particleP->count == 0) {
        for (p = particleP->data; p != end; p++) {
            a = 360.0f * fn_1_4D1C();
            b = 360.0f * fn_1_4D1C() - 180.0f;
            PSMTXRotRad(m1, 'x', 0.017453292f * (a - 180.0f));
            PSMTXRotRad(m2, 'y', 0.017453292f * b);
            PSMTXConcat(m1, m2, m1);
            PSMTXMultVec(m1, &lbl_1_data_314, &v);
            p->vel.x = v.x;
            p->vel.y = v.y;
            p->vel.z = v.z;
            p->pos.x = 0.0f;
            p->pos.y = 0.0f;
            p->pos.z = 0.0f;
            p->color.r = 0xE0;
            p->color.g = 0xE8;
            p->color.b = 0;
            p->color.a = 0xFF;
            p->scale = 50.0f;
        }
    }
    cnt = 0;
    for (q = particleP->data; q != end; q++) {
        PSVECAdd(&q->vel, &q->pos, &q->pos);
        q->vel.x = q->vel.x * 0.95;
        q->vel.y = q->vel.y * 0.95;
        q->vel.z = q->vel.z * 0.95;
        r = q->color.r - 8;
        if (r < 0) {
            r = 0;
        }
        q->color.r = r;
        g = q->color.g - 0x10;
        if (g < 0) {
            g = 0;
        }
        q->color.g = g;
        al = q->color.a - 8;
        if (al < 0) {
            al = 0;
            q->scale = 0.0f;
            particleP->dataCnt = -1;
        }
        q->color.a = al;
        q->scale += 5.0f;
        cnt++;
    }
    particleP->emitCnt = cnt;
}

// ---- list walkers

void fn_1_867C(M536EffList *list, M536EffFn cb)
{
    M536EffList *head;
    M536_EFFOBJ *first;
    M536_EFFOBJ *start;

    head = list;
    first = (M536_EFFOBJ *)list->next;
    start = first;
    fn_1_885C(start, head, cb);
}

void fn_1_86DC(M536EffList *list, M536EffFn cb)
{
    M536EffList *head;
    M536_EFFOBJ *first;
    M536_EFFOBJ *start;

    head = list;
    first = (M536_EFFOBJ *)list->next;
    start = first;
    fn_1_87FC(start, head, cb);
}

void fn_1_873C(M536EffList *list, M536EffFn cb)
{
    M536EffList *head;
    M536_EFFOBJ *first;
    M536_EFFOBJ *start;

    head = list;
    first = (M536_EFFOBJ *)list->next;
    start = first;
    fn_1_879C(start, head, cb);
}

void fn_1_879C(M536_EFFOBJ *node, M536EffList *stop, M536EffFn cb)
{
    while (node != (M536_EFFOBJ *)stop) {
        cb(node);
        node = (M536_EFFOBJ *)node->node.next;
    }
}

void fn_1_87FC(M536_EFFOBJ *node, M536EffList *stop, M536EffFn cb)
{
    while (node != (M536_EFFOBJ *)stop) {
        cb(node);
        node = (M536_EFFOBJ *)node->node.next;
    }
}

void fn_1_885C(M536_EFFOBJ *node, M536EffList *stop, M536EffFn cb)
{
    while (node != (M536_EFFOBJ *)stop) {
        cb(node);
        node = (M536_EFFOBJ *)node->node.next;
    }
}

void fn_1_88BC(void)
{
    M536EffList *l1;
    M536EffList *o1;
    M536EffList *l2;
    M536EffList *o2;
    M536EffList *l3;
    M536EffList *o3;

    l1 = &lbl_1_bss_A38;
    fn_1_1264(l1);
    o1 = l1;
    __register_global_object(o1, (void *)fn_1_8A1C, &lbl_1_bss_A2C);
    l2 = &lbl_1_bss_A4C;
    fn_1_1264(l2);
    o2 = l2;
    __register_global_object(o2, (void *)fn_1_89C4, &lbl_1_bss_A40);
    l3 = &lbl_1_bss_A60;
    fn_1_1264(l3);
    o3 = l3;
    __register_global_object(o3, (void *)fn_1_896C, &lbl_1_bss_A54);
}

void *fn_1_896C(void *self, s16 freeIt)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void *fn_1_89C4(void *self, s16 freeIt)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void *fn_1_8A1C(void *self, s16 freeIt)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

}
