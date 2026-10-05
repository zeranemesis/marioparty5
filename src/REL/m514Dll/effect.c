#define _MATH_H
#include "types.h"

typedef struct MgEffTab MgEffTab;
typedef struct MgHu3DEntry MgHu3DEntry;
typedef struct MgEffectMgr MgEffectMgr;
typedef struct MgEffect MgEffect;
typedef struct MgEffIndex MgEffIndex;
typedef struct MgEffNode MgEffNode;

typedef void (*MgEffectFn)(MgEffect *);

struct MgEffTab {
    s32 state;
    u8 pad4[0x40];
};

struct MgHu3DEntry {
    u8 pad0[0x120];
    MgEffTab *tab;
    u8 pad124[0x10];
};

struct MgEffIndex {
    s16 cur;
};

struct MgEffectMgr {
    u8 pad0[0x40];
    MgEffIndex *unk40;
    u8 pad44[0x18];
    MgEffNode *unk5C;
};

struct MgEffNode {
    u8 pad0[0x14];
    s16 unk14;
};

struct MgEffect {
    u8 pad0[0x14];
    MgEffectFn unk14;
};

extern MgHu3DEntry Hu3DData[];

MgEffectMgr *lbl_1_bss_218;

extern void fn_1_5C34(MgEffect *);
extern void fn_1_6588(MgEffect *);
extern void fn_1_6C44(MgEffect *);
extern void fn_1_76D0(MgEffect *);
extern void fn_1_7184(MgEffect *);
extern void fn_1_5CD4(MgEffect *);
extern void fn_1_6644(MgEffect *);
extern void fn_1_6D7C(MgEffect *);
extern void fn_1_77BC(MgEffect *);

void fn_1_5BEC(MgEffect *);

void fn_1_5B90(MgEffect *self)
{
    fn_1_5C34(self);
    fn_1_6588(self);
    fn_1_6C44(self);
    fn_1_76D0(self);
    fn_1_7184(self);
    self->unk14 = fn_1_5BEC;
}

void fn_1_5BEC(MgEffect *self)
{
    fn_1_5CD4(self);
    fn_1_6644(self);
    fn_1_6D7C(self);
    fn_1_77BC(self);
}

void fn_1_7B14(s32 i)
{
    MgEffectMgr *mgr;
    MgHu3DEntry *ent;
    MgEffTab *slot;
    s16 cur;
    MgEffTab *tab;

    mgr = lbl_1_bss_218;
    cur = mgr->unk40->cur;
    ent = &Hu3DData[cur];
    tab = ent->tab;

    if (i >= 0 && i < 3) {
        slot = &tab[i];
        slot->state = 0;
    }
}

void fn_1_7B8C(s32 i)
{
    MgEffTab *slot;
    MgEffectMgr *mgr;
    MgHu3DEntry *ent;
    s16 cur;
    MgEffTab *tab;

    mgr = lbl_1_bss_218;
    cur = mgr->unk40->cur;
    ent = &Hu3DData[cur];
    tab = ent->tab;

    if (i >= 0 && i < 3) {
        slot = &tab[i];
        if (slot->state != 0) {
            slot->state = 3;
        }
    }
}

void fn_1_8228(s16 v)
{
    MgEffectMgr *mgr;
    MgEffNode *dst;
    MgEffNode *src;

    mgr = lbl_1_bss_218;
    src = mgr->unk5C;
    dst = src;
    dst->unk14 = (s16)v;
}