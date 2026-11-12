// m568Dll game master: the scene object that owns the actor / effect / group
// lists (a_set.h), the script that runs on its sub-thread, and the helpers
// that walk those lists.
#define _MATH_H
#include "REL/m568Dll.h"
extern "C" {
#include "dolphin.h"
#include "game/audio.h"
#include "game/wipe.h"
}
#include <stddef.h>

struct Camera568;

// An object with a virtual destructor in vtable slot 2.
struct GmPoly {
    void **vtbl;
};

// Element of the lbl_1_bss_A0C list: owns a model, an actor and a buffer.
struct GroupNode {
    M568Link link;  // 0x00
    GmPoly *obj;    // 0x08
    u8 pad0C[0x38];
    u8 sub44[8];    // 0x44 empty-bodied sub-object (fn_1_837C / fn_1_8380)
    void *buf;      // 0x4C
    void *model;    // 0x50 model deleted with flag 1
    void *actor;    // 0x54 actor deleted with fn_1_123DC
};

// Pointer-to-member record, as MWCC lays it out for __ptmf_scall.
struct PTMF {
    s32 this_delta;
    s32 v_offset;
    void (*f_addr)(void *);
};

extern "C" {

void fn_1_10E9C(void *node);
void fn_1_1290C(void *node);
void fn_1_FDEC(void *node);
void fn_1_FE8C(void *node);
void fn_1_FF34(void *node);
void fn_1_FF8C(void *node);
void fn_1_123DC(void *actor, s32 flag);
void fn_1_13F08(void *a, void *b);
void fn_1_140D8(void *a, void *b);
void fn_1_F990(M568Effect *effect, SceneNode *actor);
void *fn_1_D260(void *self);
Camera568 *fn_1_9D60(Camera568 *self);
EffectMgr *fn_1_B668(EffectMgr *self);
void *fn_1_104FC(void *self, s32 idx);
void fn_1_B320(Camera568 *self, Vec *pos, f32 z);

void *fn_1_618(u32 size);
void fn_1_6E0(void *mem);
void fn_1_4DFC(void *mem);
void fn_1_370C(void *model, s16 flag);
void fn_1_837C(void *self);
void fn_1_8380(void *self);

void fn_1_2558(M568Master *self);
M568Master *fn_1_26E4(M568Master *self, s16 flag);
void fn_1_5020(u32 seed);
u32 fn_1_5634(M568Sphere *a, M568Sphere *b);
void fn_1_5880(M568SubThread *self, u32 stackSize);
M568SubThread *fn_1_58FC(M568SubThread *self, s16 flag);
s32 fn_1_59B0(M568SubThread *self, s32 arg);
s32 fn_1_5A40(M568SubThread *self, s32 arg);

void fn_1_2260(s32 phase);
s32 fn_1_22F8(void);
void fn_1_2384(void);
void fn_1_23F8(void);
void fn_1_2D1C(void);
void fn_1_2DD4(void);
s32 fn_1_2E8C(void);
void fn_1_2F60(void);
void fn_1_3204(void);
void fn_1_325C(void);

u32 OSGetTick(void);

M568Set *fn_1_80E8(M568Set *self, s16 flag);
M568Set **fn_1_8090(M568Set **self, s16 flag);
GameMaster *fn_1_8798(GameMaster *self, s16 flag);
void fn_1_8824(M568Master *self, void *objman);
void fn_1_8A08(M568Master *self);
void fn_1_8DC8(GameMaster *self);
void fn_1_970C(M568Master *self);
void fn_1_9890(M568SubThread *self);
GameMaster *fn_1_9898(M568SubThread *self, s16 flag);
void fn_1_898C(SceneNode *actor, M568Effect *effect);
void fn_1_972C(M568Link *self, void (*func)(void *, void *));
void fn_1_97C0(M568Link *self, M568Link *other, void (*func)(void *, void *));

M568Link *lbl_1_bss_A0C;
s32 lbl_1_bss_A00;
s32 lbl_1_bss_A04;
s32 lbl_1_bss_A08;
EffectMgr *lbl_1_bss_A10;
Camera568 *lbl_1_bss_A14;

char lbl_1_data_2A8[] = "sm_ptr == NULL";
char lbl_1_data_2B7[] = "a_set.h";
char lbl_1_data_2BF[0xD] = "sm_ptr != 0";

PTMF lbl_1_data_2CC = { 0, -1, fn_1_10E9C };
PTMF lbl_1_data_2D8 = { 0, -1, fn_1_1290C };
PTMF lbl_1_data_2E4 = { 0, -1, fn_1_FDEC };
PTMF lbl_1_data_2F0 = { 0, -1, fn_1_FE8C };
PTMF lbl_1_data_2FC = { 0, -1, fn_1_FF34 };
PTMF lbl_1_data_308 = { 0, -1, fn_1_FF8C };
char lbl_1_data_314[0x10] = "ms_pointer != 0";
char lbl_1_data_324[0xC] = "singleton.h";

// Two vtables back to back: the master part {dtor, setup, update, finish} and
// the sub-thread part (its slots point at this-adjusting thunks).
void *lbl_1_data_330[12] = {
    NULL, NULL, (void *)fn_1_8798, (void *)fn_1_8824, (void *)fn_1_8A08, (void *)fn_1_970C,
    NULL, NULL, (void *)fn_1_9898, (void *)fn_1_9890, (void *)fn_1_8DC8, NULL
};

// Intrusive-list iteration helpers (a_set.h); the original inlines each of
// them, which is where its register-to-register copies come from.
template <class T>
inline T *ListEnd(T *head)
{
    return head;
}

template <class T>
inline T *ListBegin(T *head)
{
    return (T *)head->link.next;
}

template <class T>
inline T *ListNext(T *node)
{
    return (T *)node->link.next;
}

#define SET_ASSERT(p) \
    if ((p) != 0) { \
    } else { \
        fn_1_7EC(lbl_1_data_2BF, lbl_1_data_2B7, 0x3A); \
    }

inline SceneNode *ActorSet()
{
    SET_ASSERT(lbl_1_bss_9F8)
    return lbl_1_bss_9F8;
}

inline M568Effect *EffectSet()
{
    SET_ASSERT(lbl_1_bss_9F4)
    return lbl_1_bss_9F4;
}

inline M568Link *GroupSet()
{
    SET_ASSERT(lbl_1_bss_A0C)
    return lbl_1_bss_A0C;
}

inline u8 IsInactive(SceneNode *n)
{
    return n->inactive;
}

inline u8 IsDead(SceneNode *n)
{
    return n->dead;
}

inline M568Link *LinkEnd(M568Link *l)
{
    return l;
}

inline M568Link *LinkNext(M568Link *l)
{
    return l->next;
}

// Call a pointer-to-member record on `obj`. Every record in this unit has
// this_delta 0 and v_offset -1, i.e. a direct call of f_addr (what
// __ptmf_scall resolves to for a non-virtual member).
inline void PtmfCall(const PTMF *pmf, void *obj)
{
    pmf->f_addr((char *)obj + pmf->this_delta);
}

// True when no actor is both active and alive.
inline u8 ActorsAllOut()
{
    SceneNode *set;
    SceneNode *end;
    SceneNode *n;

    set = ActorSet();
    end = ListEnd(set);
    n = ListBegin(set);
    while (1) {
        if (n == end) {
            return 1;
        }
        if (IsInactive(n) == 0 && IsDead(n) == 0) {
            return 0;
        }
        n = ListNext(n);
    }
}

// True when an active, alive actor has fallen below the stage.
inline u8 AnyActorFallen()
{
    SceneNode *set;
    SceneNode *end;
    SceneNode *n;

    set = ActorSet();
    end = ListEnd(set);
    n = ListBegin(set);
    while (1) {
        if (n == end) {
            return 0;
        }
        if (IsInactive(n) == 0 && IsDead(n) == 0 && n->pos.z < -39000.0f) {
            return 1;
        }
        n = ListNext(n);
    }
}

// Construct the master: base part, script thread, vtables and the scene set.
GameMaster *fn_1_7F1C(GameMaster *self)
{
    M568Set *set;

    fn_1_2558(&self->base);
    fn_1_5880(&self->thread, 0x8000);
    self->base.vtbl = (M568MasterVtbl *)&lbl_1_data_330[0];
    self->thread.vtbl = (M568ThreadVtbl *)&lbl_1_data_330[6];
#ifdef TARGET_PC
    set = (M568Set *)fn_1_618(sizeof(M568Set));
#else
    set = (M568Set *)fn_1_618(0x18);
#endif
    if (set != 0) {
        fn_1_187C(&set->groups);
        if (lbl_1_bss_A0C == 0) {
        } else {
            fn_1_7EC(lbl_1_data_2A8, lbl_1_data_2B7, 0x1D);
        }
        lbl_1_bss_A0C = &set->groups;
        fn_1_187C(&set->actors);
        if (lbl_1_bss_9F8 == 0) {
        } else {
            fn_1_7EC(lbl_1_data_2A8, lbl_1_data_2B7, 0x1D);
        }
        lbl_1_bss_9F8 = (SceneNode *)&set->actors;
        fn_1_187C(&set->effects);
        if (lbl_1_bss_9F4 == 0) {
        } else {
            fn_1_7EC(lbl_1_data_2A8, lbl_1_data_2B7, 0x1D);
        }
        lbl_1_bss_9F4 = (M568Effect *)&set->effects;
    }
    self->set = set;
    return self;
}

// Smart-pointer holder: destroy the set it points at.
M568Set **fn_1_8090(M568Set **self, s16 flag)
{
    if (self != 0) {
        fn_1_80E8(*self, 1);
        if (flag > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

// Destroy the set: effects first, then actors, then groups.
M568Set *fn_1_80E8(M568Set *self, s16 flag)
{
    M568Effect *e;
    GroupNode *g;
    u8 empty;

    if (self != 0) {
        if (&self->effects != 0) {
            while (1) {
                empty = 0;
                if (self->effects.next == &self->effects && self->effects.next == self->effects.prev) {
                    empty = 1;
                }
                if (empty != 0) {
                    break;
                }
                e = (M568Effect *)self->effects.prev;
                if (e != 0) {
                    fn_1_370C(&e->model, -1);
                    if (e != 0) {
                        fn_1_1888(e, 0);
                    }
                    fn_1_6E0(e);
                }
            }
            if (&self->effects != 0) {
                fn_1_1888(&self->effects, 0);
            }
        }
        if (&self->actors != 0) {
            while (1) {
                empty = 0;
                if (self->actors.next == &self->actors && self->actors.next == self->actors.prev) {
                    empty = 1;
                }
                if (empty != 0) {
                    break;
                }
                fn_1_123DC(self->actors.prev, 1);
            }
            if (&self->actors != 0) {
                fn_1_1888(&self->actors, 0);
            }
        }
        if (self != 0) {
            while (1) {
                empty = 0;
                if (self->groups.next == &self->groups && self->groups.next == self->groups.prev) {
                    empty = 1;
                }
                if (empty != 0) {
                    break;
                }
                g = (GroupNode *)self->groups.prev;
                if (g != 0) {
                    if (&g->actor != 0) {
                        fn_1_123DC(g->actor, 1);
                    }
                    if (&g->model != 0) {
                        fn_1_370C(g->model, 1);
                    }
                    if (&g->sub44 != 0 && &g->sub44 != 0 && &g->sub44 != 0 && g->buf != 0) {
                        void *buf;

                        fn_1_8380(&g->sub44);
                        buf = g->buf;
                        fn_1_837C(&g->sub44);
                        fn_1_6E0(buf);
                    }
                    if (&g->obj != 0) {
                        GmPoly *obj = g->obj;

                        if (obj != 0) {
                            ((void (*)(GmPoly *, s32))obj->vtbl[2])(obj, 1);
                        }
                    }
                    if (g != 0) {
                        fn_1_1888(g, 0);
                    }
                    fn_1_6E0(g);
                }
            }
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

void fn_1_837C(void *self)
{
}

void fn_1_8380(void *self)
{
}

// Effect list holder destructor.
M568Link *fn_1_8384(M568Link *self, s16 flag)
{
    M568Effect *e;
    u8 empty;

    if (self != 0) {
        while (1) {
            empty = 0;
            if (self->next == self && self->next == self->prev) {
                empty = 1;
            }
            if (empty != 0) {
                break;
            }
            e = (M568Effect *)self->prev;
            if (e != 0) {
                fn_1_370C(&e->model, -1);
                if (e != 0) {
                    fn_1_1888(e, 0);
                }
                fn_1_6E0(e);
            }
        }
        if (self != 0) {
            fn_1_1888(self, 0);
        }
        if (flag > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

M568Link *fn_1_845C(M568Link *self, s16 flag)
{
    if (self != 0) {
        fn_1_1888(self, 0);
        if (flag > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

// Actor list holder destructor.
M568Link *fn_1_84B4(M568Link *self, s16 flag)
{
    u8 empty;

    if (self != 0) {
        while (1) {
            empty = 0;
            if (self->next == self && self->next == self->prev) {
                empty = 1;
            }
            if (empty != 0) {
                break;
            }
            fn_1_123DC(self->prev, 1);
        }
        if (self != 0) {
            fn_1_1888(self, 0);
        }
        if (flag > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

M568Link *fn_1_8574(M568Link *self, s16 flag)
{
    if (self != 0) {
        fn_1_1888(self, 0);
        if (flag > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

// Group list holder destructor.
M568Link *fn_1_85CC(M568Link *self, s16 flag)
{
    GroupNode *g;
    u8 empty;

    if (self != 0) {
        while (1) {
            empty = 0;
            if (self->next == self && self->next == self->prev) {
                empty = 1;
            }
            if (empty != 0) {
                break;
            }
            g = (GroupNode *)self->prev;
            if (g != 0) {
                if (&g->actor != 0) {
                    fn_1_123DC(g->actor, 1);
                }
                if (&g->model != 0) {
                    fn_1_370C(g->model, 1);
                }
                if (&g->sub44 != 0 && &g->sub44 != 0 && &g->sub44 != 0 && g->buf != 0) {
                    fn_1_8380(&g->sub44);
                    fn_1_837C(&g->sub44);
                    fn_1_6E0(g->buf);
                }
                if (&g->obj != 0) {
                    GmPoly *obj = g->obj;

                    if (obj != 0) {
                        ((void (*)(GmPoly *, s32))obj->vtbl[2])(obj, 1);
                    }
                }
                if (g != 0) {
                    fn_1_1888(g, 0);
                }
                fn_1_6E0(g);
            }
        }
        if (self != 0) {
            fn_1_1888(self, 0);
        }
        if (flag > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

M568Link *fn_1_8740(M568Link *self, s16 flag)
{
    if (self != 0) {
        fn_1_1888(self, 0);
        if (flag > 0) {
            fn_1_6E0(self);
        }
    }
    return self;
}

// Master destructor (vtable slot 2).
GameMaster *fn_1_8798(GameMaster *self, s16 flag)
{
    if (self != 0) {
        self->base.vtbl = (M568MasterVtbl *)&lbl_1_data_330[0];
        self->thread.vtbl = (M568ThreadVtbl *)&lbl_1_data_330[6];
        fn_1_8090(&self->set, -1);
        fn_1_58FC(&self->thread, 0);
        fn_1_26E4(&self->base, 0);
        if (flag > 0) {
            fn_1_4DFC(self);
        }
    }
    return self;
}

// Scene setup: seed the RNG, build the stage, camera, effects and the four
// players, then number the active actors.
void fn_1_8824(M568Master *self, void *objman)
{
    SceneNode *set;
    SceneNode *end;
    SceneNode *n;
    s32 num;
    s32 i;
    void *obj;

    fn_1_5020(OSGetTick());
    obj = fn_1_618(0x60);
    if (obj != 0) {
        fn_1_D260(obj);
    }
    obj = fn_1_618(0x8C);
    if (obj != 0) {
        fn_1_9D60((Camera568 *)obj);
    }
    obj = fn_1_618(0x18);
    if (obj != 0) {
        fn_1_B668((EffectMgr *)obj);
    }
    for (i = 0; i < 4; i++) {
        obj = fn_1_618(0x58);
        if (obj != 0) {
            fn_1_104FC(obj, i);
        }
    }
    set = ActorSet();
    n = ListBegin(set);
    num = 1;
    end = set;
    while (n != end) {
        if (IsInactive(n) != 0) {
            n->slot = 0;
        } else {
            n->slot = num;
            num++;
        }
        n = ListNext(n);
    }
}

// Actor/effect collision: if the spheres overlap, notify both.
void fn_1_898C(SceneNode *actor, M568Effect *effect)
{
    M568Sphere *ep = (M568Sphere *)&effect->pos;
    M568Sphere *ap = (M568Sphere *)&actor->pos;

    if ((u8)fn_1_5634(ap, ep) != 0) {
        fn_1_140D8(actor, effect);
        fn_1_F990(effect, actor);
    }
}

// Per-frame update (vtable slot 4).
void fn_1_8A08(M568Master *base)
{
    GameMaster *self = (GameMaster *)base;
    M568Link *groups;
    SceneNode *actors;
    M568Effect *effects;
    M568Link *g;
    SceneNode *a;
    PTMF pmf1;
    PTMF pmf2;

    if (fn_1_59B0(&self->thread, 0) >= 0) {
        fn_1_2384();
        groups = GroupSet();
        actors = ActorSet();
        effects = EffectSet();
        pmf1 = lbl_1_data_2CC;
        for (g = groups->next; g != groups; g = g->next) {
            PtmfCall(&pmf1, g);
        }
        fn_1_972C(&actors->link, fn_1_13F08);
        fn_1_97C0(&actors->link, &effects->link, (void (*)(void *, void *))fn_1_898C);
        pmf2 = lbl_1_data_2D8;
        a = ListBegin(actors);
        while (a != actors) {
            PtmfCall(&pmf2, a);
            a = ListNext(a);
        }
    }
}

// Does an active, alive actor lie below the stage?
s32 fn_1_8C24(void)
{
    SceneNode *set;
    SceneNode *end;
    SceneNode *n;

    set = ActorSet();
    end = ListEnd(set);
    n = ListBegin(set);
    while (n != end) {
        if (IsInactive(n) == 0 && IsDead(n) == 0 && n->pos.z < -39000.0f) {
            return 1;
        }
        n = ListNext(n);
    }
    return 0;
}

// Is every actor inactive or dead?
s32 fn_1_8D04(void)
{
    SceneNode *set;
    SceneNode *end;
    SceneNode *n;

    set = ActorSet();
    end = ListEnd(set);
    n = ListBegin(set);
    while (1) {
        if (n == end) {
            return 1;
        }
        if (IsInactive(n) == 0 && IsDead(n) == 0) {
            return 0;
        }
        n = ListNext(n);
    }
}

// The scene script, run on the sub-thread.
void fn_1_8DC8(GameMaster *self)
{
    static const Vec lookAt = { 0.0f, 0.0f, 0.0f };
    s32 stream;
    s32 n;
    s32 i;
    M568Link *groups;
    M568Link *g;
    PTMF pmf;

    stream = HuAudSStreamPlay(0x35);
    fn_1_2260(0);
    fn_1_3204();
    while (fn_1_22F8() == 0) {
        fn_1_5A40(&self->thread, 0);
    }
    fn_1_2260(1);
    fn_1_2D1C();
    while (fn_1_2E8C() == 0) {
        fn_1_5A40(&self->thread, 0);
    }
    fn_1_2260(2);
    n = 0x708;
    while (--n != 0) {
        if (ActorsAllOut() != 0) {
            break;
        }
        if (AnyActorFallen() != 0) {
            break;
        }
        fn_1_5A40(&self->thread, 0);
    }
    fn_1_2260(3);
    HuAudSStreamFadeOut(stream, 0x64);
    fn_1_2DD4();
    i = 0;
    while ((f32)i < 150.0f) {
        fn_1_5A40(&self->thread, 0);
        i++;
    }
    WipeCreate(2, 6, 0x3C);
    i = 0;
    while (i < 0x3C) {
        fn_1_5A40(&self->thread, 0);
        i++;
    }
    HuAudFadeOut(0x96);
    groups = GroupSet();
    if (ActorsAllOut() != 0) {
        pmf = lbl_1_data_2E4;
    } else {
        pmf = lbl_1_data_2F0;
    }
    for (g = groups->next; g != groups; g = g->next) {
        PtmfCall(&pmf, g);
    }
    fn_1_2260(4);
    WipeCreate(1, 6, 0x3C);
    i = 0;
    while (i < 0x78) {
        fn_1_5A40(&self->thread, 0);
        i++;
    }
    if (ActorsAllOut() != 0) {
        pmf = lbl_1_data_2FC;
    } else {
        pmf = lbl_1_data_308;
    }
    for (g = groups->next; g != groups; g = g->next) {
        PtmfCall(&pmf, g);
    }
    HuAudJinglePlay(0x79);
    fn_1_2F60();
    i = 0;
    while ((f32)i < 30.0) {
        fn_1_5A40(&self->thread, 0);
        i++;
    }
    if (lbl_1_bss_A14 != 0) {
    } else {
        fn_1_7EC(lbl_1_data_314, lbl_1_data_324, 0x27);
    }
    fn_1_B320(lbl_1_bss_A14, (Vec *)&lookAt, 2500.0f);
    while (fn_1_2E8C() == 0) {
        fn_1_5A40(&self->thread, 0);
    }
    fn_1_325C();
    while (1) {
        fn_1_5A40(&self->thread, -1);
    }
}

// Finish (vtable slot 5).
void fn_1_970C(M568Master *self)
{
    fn_1_23F8();
}

// Call `func` on every pair of nodes (i, j) with i before j.
void fn_1_972C(M568Link *self, void (*func)(void *, void *))
{
    M568Link *outer;
    M568Link *inner;
    M568Link *stop;

    outer = self->next;
    stop = self->prev;
    while (outer != stop) {
        inner = outer->next;
        while (inner != self) {
            func(outer, inner);
            inner = inner->next;
        }
        outer = outer->next;
    }
}

// Call `func` on every pair (a in self, b in other).
void fn_1_97C0(M568Link *self, M568Link *other, void (*func)(void *, void *))
{
    M568Link *outer;
    M568Link *inner;

    outer = LinkNext(self);
    while (outer != LinkEnd(self)) {
        inner = LinkNext(other);
        while (inner != LinkEnd(other)) {
            func(outer, inner);
            inner = LinkNext(inner);
        }
        outer = LinkNext(outer);
    }
}

void fn_1_985C(void)
{
    lbl_1_bss_A00 = 0x78;
    lbl_1_bss_A04 = 0xD2;
    lbl_1_bss_A08 = 0xD2;
}

// Sub-thread run thunk: adjust `this` from the thread part to the master.
void fn_1_9890(M568SubThread *self)
{
    fn_1_8DC8((GameMaster *)((char *)self - offsetof(GameMaster, thread)));
}

GameMaster *fn_1_9898(M568SubThread *self, s16 flag)
{
    return fn_1_8798((GameMaster *)((char *)self - offsetof(GameMaster, thread)), 0);
}

}
