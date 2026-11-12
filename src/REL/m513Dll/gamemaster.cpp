// m513Dll gamemaster: the game master. It owns the sets of players, leaves,
// heihos and kuribos, a model pointer, the score display and runs the game
// flow on its sub thread.
#define _MATH_H
#include "REL/m513Dll.h"
extern "C" {
#include "game/gamework.h"
#include "game/flag.h"
#include "game/audio.h"
#include "game/charman.h"
#include "game/mgdata.h"
}

#ifdef __MWERKS__
#define M513_RODATA __declspec(section ".rodata")
#else
#define M513_RODATA
#endif

// The four intrusive sets the singletons point into.
typedef struct M513Sets_s {
    M513Node players;
    M513Node leaves;
    M513Node heihos;
    M513Node kuribos;
} M513Sets;

typedef struct M513SetsPtr_s {
    M513Sets *p;
} M513SetsPtr;

typedef struct M513GameMaster_s {
    M513Master base;
    M513SubThread thread;
    M513SetsPtr sets;
    M513ModelPtr *mp;
    s32 record;
    u8 multi;
    s32 winner;
    M513TimerObj timer;
    u8 recordChanged;
} M513GameMaster;

// Vtable of the master followed by the one of its thread base and two PTMFs.
typedef struct M513GMVtbl_s {
    M513MasterVtbl master;
    void *thread[6];
    M513Ptmf ptmf[2];
} M513GMVtbl;

extern "C" {

void __destroy_arr(void *ptr, void *dtor, u32 size, u32 count);
u32 OSGetTick(void);

void fn_1_8394(M513Leaf *leaf);
M513ModelPtr *fn_1_D738(M513ModelPtr *self);
M513ModelPtr *fn_1_D88C(M513ModelPtr *self, s16 flag);
M513Heiho *fn_1_DA0C(M513Heiho *self, s32 idx);
M513Heiho *fn_1_DE94(M513Heiho *self, s16 flag);
M513Leaf *fn_1_8310(M513Leaf *self, s16 flag);
void *fn_1_EDE0(void *self, Vec *pos, Vec *rot);
void *fn_1_F1E4(void *self, s16 flag);
void fn_1_FAE4(void *self);
void fn_1_FC08(void *self, s32 mode);
void *fn_1_104B0(void *self);
void *fn_1_FD60(void *self);
void *fn_1_6D50(void *self, Vec *pos);
void *fn_1_AE58(void *self);
M513Player *fn_1_11900(M513Player *self, s32 playerNo, u8 type);
s32 fn_1_12E88(M513Player *self);
void fn_1_12E90(M513Player *self, u8 win);
void fn_1_12FDC(M513Player *self);
u8 fn_1_13050(M513Player *self);
M513Score *fn_1_13174(M513Score *self, s32 recordTime, u8 multi);
void fn_1_13B98(M513Score *self);
void fn_1_13CDC(M513Score *self);
u8 fn_1_13D7C(M513Score *self);
s32 fn_1_13E34(M513Score *self, s32 idx);
void fn_1_12080(M513Player *self);
void fn_1_8E24(M513Leaf *self);
void fn_1_E5F0(M513Heiho *self);
void fn_1_FBA8(void *self);
void fn_1_1220C(void);
void fn_1_12208(void);
extern M513Ptmf lbl_1_data_3B8;
extern M513Ptmf lbl_1_data_3C4;

M513SetsPtr *fn_1_B79C(M513SetsPtr *self, s16 flag);
M513Sets *fn_1_B7F4(M513Sets *self, s16 flag);
M513Player **fn_1_BAF0(M513Player **self, s16 flag);
void fn_1_BB6C(void);
void fn_1_BB70(void);
M513Master *fn_1_C0A0(M513Master *self, s16 flag);
void fn_1_C150(M513GameMaster *self);
void fn_1_C43C(M513GameMaster *self);
void fn_1_C778(M513GameMaster *self);
void fn_1_CA7C(M513GameMaster *self);
void fn_1_CB44(M513GameMaster *self);
u8 fn_1_CB88(M513GameMaster *self);
void fn_1_CD44(M513GameMaster *self);
u8 fn_1_CE9C(M513GameMaster *self);
void fn_1_CF4C(M513GameMaster *self);
void fn_1_D034(M513GameMaster *self);
void fn_1_D1AC(M513GameMaster *self);
void fn_1_D27C(M513GameMaster *self);
void fn_1_D34C(M513GameMaster *self);
void fn_1_D4C8(M513GameMaster *self);
void fn_1_D56C(M513GameMaster *self);
void fn_1_D610(M513GameMaster *self);
void fn_1_D728(M513SubThread *thread);
M513Master *fn_1_D730(M513SubThread *thread, s16 flag);

extern M513Node *lbl_1_bss_9F8;
extern M513Node *lbl_1_bss_A10;
extern M513Node *lbl_1_bss_A14;
extern M513Score *lbl_1_bss_A20;

M513_RODATA const Vec lbl_1_rodata_258[8] = {
    { -287.5f, 40.0f, -1056.0f }, { -225.0f, 85.0f, -1125.0f }, { -162.5f, 40.0f, -1056.0f },
    { 62.5f, 40.0f, -1056.0f }, { -1056.0f, 40.0f, -287.5f }, { -1125.0f, 85.0f, -225.0f },
    { -1056.0f, 40.0f, -162.5f }, { -1056.0f, 40.0f, 62.5f },
};
M513_RODATA const Vec lbl_1_rodata_2B8[8] = {
    { 0.0f, -45.0f, 0.0f }, { 0.0f, -45.0f, 0.0f }, { 0.0f, -45.0f, 0.0f }, { 0.0f, -45.0f, 0.0f },
    { 0.0f, 135.0f, 0.0f }, { 0.0f, 135.0f, 0.0f }, { 0.0f, 135.0f, 0.0f }, { 0.0f, 135.0f, 0.0f },
};
M513_RODATA const Vec lbl_1_rodata_318[4] = {
    { -1018.70398f, 148.30652f, -48.944519f }, { -1018.70398f, 148.30652f, -400.05939f },
    { -398.4353f, 148.30652f, -978.414f }, { -49.52887f, 148.30652f, -978.414f },
};

char lbl_1_data_3D0[] = "sm_ptr == NULL";
char lbl_1_data_3DF[] = "a_set.h";
M513Ptmf lbl_1_data_3E8 = { 0, -1, (void (*)(void *))fn_1_12080 };
M513Ptmf lbl_1_data_3F4 = { 0, -1, (void (*)(void *))fn_1_8E24 };
M513Ptmf lbl_1_data_400 = { 0, -1, (void (*)(void *))fn_1_E5F0 };
M513Ptmf lbl_1_data_40C = { 0, -1, (void (*)(void *))fn_1_FBA8 };
char lbl_1_data_418[] = "sm_ptr != 0";
char lbl_1_data_424[] = "ms_pointer != 0";
char lbl_1_data_434[] = "singleton.h";
M513GMVtbl lbl_1_data_440 = {
    { { 0, 0 }, fn_1_C0A0, (void (*)(M513Master *, OMOBJMAN *))fn_1_C150, (void (*)(M513Master *))fn_1_C43C, (void (*)(M513Master *))fn_1_CB44 },
    { 0, 0, (void *)fn_1_D730, (void *)fn_1_D728, (void *)fn_1_C778, 0 },
    { { 0, -1, (void (*)(void *))fn_1_1220C }, { 0, -1, (void (*)(void *))fn_1_12208 } },
};

s32 lbl_1_bss_A00;
s32 lbl_1_bss_A04;
s32 lbl_1_bss_A08;
M513Node *lbl_1_bss_A0C;

// True when the list holds no element besides its sentinel.
static inline u8 M513ListEmpty(M513Node *n)
{
    u8 r = 0;

    if ((n->next == n) && (n->next == n->prev)) {
        r = 1;
    }
    return r;
}

// Delete a player the way its destructor does.
static void M513PlayerDelete(M513Player *p)
{
    s32 i;

    if (p != 0) {
        if (&p->leaf != 0) {
            fn_1_8310(p->leaf, 1);
        }
        if (&p->input != 0) {
            M513Input *in = p->input;

            if (in != 0) {
                in->vtbl->dtor(in, 1);
            }
        }
        if (&p->model != 0) {
            fn_1_35F8(p->model, 1);
        }
        if ((&p->mot != 0) && (&p->mot != 0) && (&p->mot != 0) && (p->mot.data != 0)) {
            fn_1_BB70();
            fn_1_BB6C();
            fn_1_C8(p->mot.data);
        }
#ifdef TARGET_PC
        for (i = 15; i >= 0; i--) {
            fn_1_BAF0(&p->eff[i], -1);
        }
#else
        __destroy_arr(p->eff, (void *)fn_1_BAF0, 4, 0x10);
#endif
        if (p != 0) {
            fn_1_1774(&p->link, 0);
        }
        fn_1_C8(p);
    }
}

M513GameMaster *fn_1_B570(M513GameMaster *self)
{
    M513Sets *sets;

    fn_1_2444(&self->base);
    fn_1_576C(&self->thread, 0x8000);
    self->base.vtbl = &lbl_1_data_440.master;
    self->thread.vtbl = (M513ThreadVtbl *)lbl_1_data_440.thread;
    sets = (M513Sets *)fn_1_0(sizeof(M513Sets));
    if (sets != 0) {
        fn_1_1768(&sets->players);
        M513_ASSERT(lbl_1_bss_A10 == 0, lbl_1_data_3D0, lbl_1_data_3DF, 0x15);
        lbl_1_bss_A10 = &sets->players;
        fn_1_1768(&sets->leaves);
        M513_ASSERT(lbl_1_bss_9F8 == 0, lbl_1_data_3D0, lbl_1_data_3DF, 0x15);
        lbl_1_bss_9F8 = &sets->leaves;
        fn_1_1768(&sets->heihos);
        M513_ASSERT(lbl_1_bss_A14 == 0, lbl_1_data_3D0, lbl_1_data_3DF, 0x15);
        lbl_1_bss_A14 = &sets->heihos;
        fn_1_1768(&sets->kuribos);
        M513_ASSERT(lbl_1_bss_A0C == 0, lbl_1_data_3D0, lbl_1_data_3DF, 0x15);
        lbl_1_bss_A0C = &sets->kuribos;
    }
    self->sets.p = sets;
    self->mp = 0;
    fn_1_5FA4(&self->timer, 0);
    return self;
}

M513ModelPtr **fn_1_B744(M513ModelPtr **self, s16 flag)
{
    if (self != 0) {
        fn_1_D88C(*self, 1);
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M513SetsPtr *fn_1_B79C(M513SetsPtr *self, s16 flag)
{
    if (self != 0) {
        fn_1_B7F4(self->p, 1);
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

// Destroy the four sets and every element in them.
M513Sets *fn_1_B7F4(M513Sets *self, s16 flag)
{
    if (self != 0) {
        if (&self->kuribos != 0) {
            while (!M513ListEmpty(&self->kuribos)) {
                fn_1_F1E4(self->kuribos.prev, 1);
            }
            if (&self->kuribos != 0) {
                fn_1_1774(&self->kuribos, 0);
            }
        }
        if (&self->heihos != 0) {
            while (!M513ListEmpty(&self->heihos)) {
                fn_1_DE94((M513Heiho *)self->heihos.prev, 1);
            }
            if (&self->heihos != 0) {
                fn_1_1774(&self->heihos, 0);
            }
        }
        if (&self->leaves != 0) {
            while (!M513ListEmpty(&self->leaves)) {
                M513Leaf *leaf = (M513Leaf *)self->leaves.prev;

                fn_1_8310(leaf, 1);
            }
            if (&self->leaves != 0) {
                fn_1_1774(&self->leaves, 0);
            }
        }
        if (&self->players != 0) {
            while (!M513ListEmpty(&self->players)) {
                M513Player *p = (M513Player *)self->players.prev;

                M513PlayerDelete(p);
            }
            if (&self->players != 0) {
                fn_1_1774(&self->players, 0);
            }
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M513Player **fn_1_BAF0(M513Player **self, s16 flag)
{
    if (self != 0) {
        M513Player *obj = *self;

        if (obj != 0) {
            ((M513TaskVtbl *)((M513PowerEff *)obj)->vtbl)->dtor((M513Task *)obj, 1);
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_BB6C(void)
{
}

void fn_1_BB70(void)
{
}

// Set of kuribos.
M513Node *fn_1_BB74(M513Node *self, s16 flag)
{
    if (self != 0) {
        while (!M513ListEmpty(self)) {
            fn_1_F1E4(self->prev, 1);
        }
        if (self != 0) {
            fn_1_1774(self, 0);
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M513Node *fn_1_BC34(M513Node *self, s16 flag)
{
    if (self != 0) {
        fn_1_1774(self, 0);
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

// Set of heihos.
M513Node *fn_1_BC8C(M513Node *self, s16 flag)
{
    if (self != 0) {
        while (!M513ListEmpty(self)) {
            fn_1_DE94((M513Heiho *)self->prev, 1);
        }
        if (self != 0) {
            fn_1_1774(self, 0);
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M513Node *fn_1_BD4C(M513Node *self, s16 flag)
{
    if (self != 0) {
        fn_1_1774(self, 0);
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

// Set of leaves.
M513Node *fn_1_BDA4(M513Node *self, s16 flag)
{
    if (self != 0) {
        while (!M513ListEmpty(self)) {
            fn_1_8310((M513Leaf *)self->prev, 1);
        }
        if (self != 0) {
            fn_1_1774(self, 0);
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M513Node *fn_1_BE64(M513Node *self, s16 flag)
{
    if (self != 0) {
        fn_1_1774(self, 0);
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

// Set of players.
M513Node *fn_1_BEBC(M513Node *self, s16 flag)
{
    if (self != 0) {
        while (!M513ListEmpty(self)) {
            M513Player *p = (M513Player *)self->prev;

            M513PlayerDelete(p);
        }
        if (self != 0) {
            fn_1_1774(self, 0);
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M513Node *fn_1_C048(M513Node *self, s16 flag)
{
    if (self != 0) {
        fn_1_1774(self, 0);
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M513Master *fn_1_C0A0(M513Master *self, s16 flag)
{
    M513GameMaster *gm = (M513GameMaster *)self;

    if (self != 0) {
        gm->base.vtbl = &lbl_1_data_440.master;
        gm->thread.vtbl = (M513ThreadVtbl *)lbl_1_data_440.thread;
        fn_1_606C(&gm->timer, -1);
        if ((void *)&gm->mp != 0) {
            fn_1_D88C(gm->mp, 1);
        }
        fn_1_B79C(&gm->sets, -1);
        fn_1_57E8(&gm->thread, 0);
        fn_1_25D0(&gm->base, 0);
        if (flag > 0) {
            fn_1_4CE8(self);
        }
    }
    return self;
}

// Master setup: create every object of the game.
void fn_1_C150(M513GameMaster *self)
{
    Vec kPos[8];
    Vec kRot[8];
    Vec torch[4];
    M513ModelPtr *mp;
    s32 i;

    fn_1_4F0C(OSGetTick());
    mp = (M513ModelPtr *)fn_1_0(sizeof(M513ModelPtr));
    if (mp != 0) {
        mp = fn_1_D738(mp);
    }
    if (self->mp != mp) {
        fn_1_D88C(self->mp, 1);
        self->mp = mp;
    }
    self->record = GWMgRecordGet(4);
    self->multi = (u8)(_CheckFlag(0x30002) != 0);
    self->recordChanged = 0;
    {
        M513Score *score = (M513Score *)fn_1_0(sizeof(M513Score));

        if (score != 0) {
            fn_1_13174(score, self->record, self->multi);
        }
    }
    {
        void *ind = fn_1_0(sizeof(M513IndHook));

        if (ind != 0) {
            fn_1_AE58(ind);
        }
    }
    {
        void *stage = fn_1_0(sizeof(M513Stage));

        if (stage != 0) {
            fn_1_104B0(stage);
        }
    }
    {
        void *cam = fn_1_0(sizeof(M513Camera));

        if (cam != 0) {
            fn_1_FD60(cam);
        }
    }
    for (i = 0; i < 4; i++) {
        M513Player *p = (M513Player *)fn_1_0(sizeof(M513Player));
        M513Heiho *h;

        if (p != 0) {
            fn_1_11900(p, i, self->multi);
        }
        h = (M513Heiho *)fn_1_0(sizeof(M513Heiho));
        if (h != 0) {
            fn_1_DA0C(h, i);
        }
    }
    for (i = 0; i < 8; i++) {
        kPos[i] = lbl_1_rodata_258[i];
        kRot[i] = lbl_1_rodata_2B8[i];
    }
    for (i = 0; i < 8; i++) {
        void *k = fn_1_0(sizeof(M513Kuribo));

        if (k != 0) {
            fn_1_EDE0(k, &kPos[i], &kRot[i]);
        }
    }
    for (i = 0; i < 4; i++) {
        torch[i] = lbl_1_rodata_318[i];
    }
    for (i = 0; i < 4; i++) {
        void *t = fn_1_0(sizeof(M513Torch));

        if (t != 0) {
            fn_1_6D50(t, &torch[i]);
        }
    }
    self->winner = -1;
}

// Master update: run every element's per-frame hook.
void fn_1_C43C(M513GameMaster *self)
{
    M513Node *players;
    M513Node *players2;
    M513Node *leaves;
    M513Node *leaves2;
    M513Node *heihos;
    M513Node *heihos2;
    M513Node *kuribos;
    M513Node *kuribos2;
    M513Node *it;
    M513Node *first;
    M513Node *first2;
    M513Node *end;
    M513Node *next;
    M513Node *next2;
    M513Ptmf hook;

    fn_1_589C(&self->thread, 0);
    fn_1_2270();
    M513_ASSERT(lbl_1_bss_A10 != 0, lbl_1_data_418, lbl_1_data_3DF, 0x2E);
    players = lbl_1_bss_A10;
    players2 = lbl_1_bss_A10;
    M513_ASSERT(lbl_1_bss_9F8 != 0, lbl_1_data_418, lbl_1_data_3DF, 0x2E);
    leaves = lbl_1_bss_9F8;
    leaves2 = lbl_1_bss_9F8;
    M513_ASSERT(lbl_1_bss_A14 != 0, lbl_1_data_418, lbl_1_data_3DF, 0x2E);
    heihos = lbl_1_bss_A14;
    heihos2 = lbl_1_bss_A14;
    M513_ASSERT(lbl_1_bss_A0C != 0, lbl_1_data_418, lbl_1_data_3DF, 0x2E);
    kuribos = lbl_1_bss_A0C;
    kuribos2 = lbl_1_bss_A0C;
    hook = lbl_1_data_3E8;
    first = players2->next;
    first2 = first;
    it = first2;
    end = players2;
    while (it != end) {
        M513_PTMF_CALL(&hook, it);
        next = it->next;
        next2 = next;
        it = next2;
    }
    hook = lbl_1_data_3F4;
    first = leaves2->next;
    first2 = first;
    it = first2;
    end = leaves2;
    while (it != end) {
        M513_PTMF_CALL(&hook, it);
        next = it->next;
        next2 = next;
        it = next2;
    }
    hook = lbl_1_data_400;
    first = heihos2->next;
    first2 = first;
    it = first2;
    end = heihos2;
    while (it != end) {
        M513_PTMF_CALL(&hook, it);
        next = it->next;
        next2 = next;
        it = next2;
    }
    hook = lbl_1_data_40C;
    first = kuribos2->next;
    first2 = first;
    it = first2;
    end = kuribos2;
    while (it != end) {
        M513_PTMF_CALL(&hook, it);
        next = it->next;
        next2 = next;
        it = next2;
    }
    (void)players;
    (void)leaves;
    (void)heihos;
    (void)kuribos;
}

// The game flow (runs on the sub thread).
void fn_1_C778(M513GameMaster *self)
{
    M513Score *score;
    s32 bgm;
    s32 i;
    u8 warned;
    s16 jingle;

    fn_1_214C(0);
    fn_1_30F0();
    bgm = HuAudBGMPlay(0x58);
    fn_1_D1AC(self);
    while (fn_1_21E4() == 0) {
        fn_1_592C(&self->thread, 0);
    }
    for (i = 0; (f32)i < 60.0f; i++) {
        fn_1_592C(&self->thread, 0);
    }
    fn_1_214C(1);
    fn_1_2C08();
    while (fn_1_2D78() == 0) {
        fn_1_592C(&self->thread, 0);
    }
    if (self->mp != 0) {
        fn_1_D88C(self->mp, 1);
        self->mp = 0;
    }
    fn_1_D27C(self);
    M513_ASSERT(lbl_1_bss_A20 != 0, lbl_1_data_424, lbl_1_data_434, 0x27);
    score = lbl_1_bss_A20;
    warned = 0;
    fn_1_214C(2);
    fn_1_13B98(score);
    fn_1_60C8(&self->timer, 0x708, 0, 0);
    fn_1_6140(&self->timer, 286.0f, 416.0f);
    while (fn_1_CB88(self) == 0) {
        fn_1_CD44(self);
        if ((warned == 0) && (fn_1_13E34(score, 0) > 0x3F48)) {
            fn_1_61C0(&self->timer, 1);
            warned = 1;
        }
        fn_1_592C(&self->thread, 0);
    }
    fn_1_CF4C(self);
    HuAudSStreamFadeOut(bgm, 0x64);
    fn_1_214C(3);
    fn_1_13CDC(score);
    if ((self->multi == 0) && (warned == 1)) {
        fn_1_62B4(&self->timer);
    }
    fn_1_2CC0();
    for (i = 0; i < 0x78; i++) {
        fn_1_592C(&self->thread, 0);
    }
    fn_1_D034(self);
    while (fn_1_CE9C(self) == 0) {
        fn_1_592C(&self->thread, 0);
    }
    fn_1_214C(4);
    if (self->multi == 0) {
        fn_1_D610(self);
        if (self->winner != -1) {
            jingle = 0x79;
        } else {
            jingle = 0x7B;
        }
        HuAudJinglePlay(jingle);
        fn_1_2E4C();
        if (self->winner == -1) {
            fn_1_CA7C(self);
        }
        while (fn_1_2D78() == 0) {
            fn_1_592C(&self->thread, 0);
        }
    }
    fn_1_3148();
    while (1) {
        fn_1_592C(&self->thread, 0);
    }
}

// Everyone lost: play the lose voices.
void fn_1_CA7C(M513GameMaster *self)
{
    M513Node *it;
    s16 chars[4];
    s32 i;

    M513_ASSERT(lbl_1_bss_A10 != 0, lbl_1_data_418, lbl_1_data_3DF, 0x2E);
    it = lbl_1_bss_A10->next;
    i = 0;
    while (it != lbl_1_bss_A10) {
#ifdef TARGET_PC
        if (i < 4) {
            chars[i] = ((M513Player *)it)->charNo;
        }
#else
        chars[i] = ((M513Player *)it)->charNo;
#endif
        it = it->next;
        i++;
    }
    CharLoseVoicePlay(chars[0], chars[1], chars[2], chars[3]);
}

void fn_1_CB44(M513GameMaster *self)
{
    fn_1_D34C(self);
    fn_1_D4C8(self);
    fn_1_D56C(self);
    fn_1_22E4();
}

// True when the game is over.
u8 fn_1_CB88(M513GameMaster *self)
{
    M513Score *score;
    M513Node *set;
    M513Node *first;
    M513Node *it;
    u8 all;

    M513_ASSERT(lbl_1_bss_A20 != 0, lbl_1_data_424, lbl_1_data_434, 0x27);
    score = lbl_1_bss_A20;
    M513_ASSERT(lbl_1_bss_A10 != 0, lbl_1_data_418, lbl_1_data_3DF, 0x2E);
    set = lbl_1_bss_A10;
    first = set->next;
    fn_1_17E0(first);
    fn_1_1834(first, set);
    it = set->next;
    all = 1;
    while (it != set) {
        M513Player *p = (M513Player *)it;

        if (p->active > 0) {
            all = 0;
        } else if (p->done == 0) {
            all = 0;
        } else if ((self->multi == 0) && (self->winner == -1)) {
            self->winner = p->playerNo;
            return 1;
        }
        it = it->next;
    }
    if (all == 1) {
        return 1;
    }
    if ((self->multi != 0) && (fn_1_13D7C(score) != 0)) {
        return 1;
    }
    if (fn_1_631C(&self->timer) != 0) {
        return 1;
    }
    return 0;
}

// Kuribos relax while somebody is still crying.
void fn_1_CD44(M513GameMaster *self)
{
    M513Node *players;
    M513Node *kuribos;
    M513Node *it;
    M513Node *kit;
    M513Node *first;
    M513Node *first2;
    M513Node *end;
    M513Node *kend;
    M513Node *next;
    M513Node *next2;
    u8 calm;
    s32 mode;

    M513_ASSERT(lbl_1_bss_A10 != 0, lbl_1_data_418, lbl_1_data_3DF, 0x2E);
    players = lbl_1_bss_A10;
    M513_ASSERT(lbl_1_bss_A0C != 0, lbl_1_data_418, lbl_1_data_3DF, 0x2E);
    kuribos = lbl_1_bss_A0C;
    first = players->next;
    it = first;
    end = players;
    first2 = kuribos->next;
    kit = first2;
    kend = kuribos;
    calm = 1;
    while (it != end) {
        if (fn_1_12E88((M513Player *)it) == 3) {
            calm = 0;
        }
        next = it->next;
        next2 = next;
        it = next2;
    }
    if (calm == 1) {
        mode = 0;
    } else {
        mode = 1;
    }
    while (kit != kend) {
        fn_1_FC08(kit, mode);
        next = kit->next;
        next2 = next;
        kit = next2;
    }
}

// True when no player is active any more.
u8 fn_1_CE9C(M513GameMaster *self)
{
    M513Node *it;

    M513_ASSERT(lbl_1_bss_A10 != 0, lbl_1_data_418, lbl_1_data_3DF, 0x2E);
    it = lbl_1_bss_A10->next;
    while (it != lbl_1_bss_A10) {
        if (((M513Player *)it)->active > 0) {
            return 0;
        }
        it = it->next;
    }
    return 1;
}

// Settle the scores of the finished game.
void fn_1_CF4C(M513GameMaster *self)
{
    M513Node *it;
    M513Node *set;

    M513_ASSERT(lbl_1_bss_A10 != 0, lbl_1_data_418, lbl_1_data_3DF, 0x2E);
    it = lbl_1_bss_A10->next;
    set = lbl_1_bss_A10;
    while (it != set) {
        M513Player *p = (M513Player *)it;

        if (p->done == 0) {
            fn_1_12FDC(p);
        } else if (fn_1_13050(p) == 0) {
            u32 score = p->score;

            if (score < (u32)self->record) {
                self->record = score;
                self->recordChanged = 1;
            }
        }
        it = it->next;
    }
}

// Reset the kuribos and store a new record.
void fn_1_D034(M513GameMaster *self)
{
    M513Node *it;
    M513Node *set;
    M513Score *score;
    s32 i;

    M513_ASSERT(lbl_1_bss_A0C != 0, lbl_1_data_418, lbl_1_data_3DF, 0x2E);
    it = lbl_1_bss_A0C->next;
    set = lbl_1_bss_A0C;
    while (it != set) {
        fn_1_FAE4(it);
        it = it->next;
    }
    M513_ASSERT(lbl_1_bss_A20 != 0, lbl_1_data_424, lbl_1_data_434, 0x27);
    score = lbl_1_bss_A20;
    if ((self->recordChanged != 0) && (self->multi == 0)) {
        for (i = 0; i < 0x3C; i++) {
            fn_1_592C(&self->thread, 0);
        }
        MgTimerRecordSet(score->timer, self->record);
        if (_CheckFlag(0x1000F) == 0) {
            fn_1_3018(self->record);
            for (i = 0; i < 0xB4; i++) {
                fn_1_592C(&self->thread, 0);
            }
        }
        GWMgRecordSet(4, self->record);
    }
}

// Give every player the waiting hook.
void fn_1_D1AC(M513GameMaster *self)
{
    M513Node *it;

    M513_ASSERT(lbl_1_bss_A10 != 0, lbl_1_data_418, lbl_1_data_3DF, 0x2E);
    it = lbl_1_bss_A10->next;
    while (it != lbl_1_bss_A10) {
        M513Ptmf tmp = lbl_1_data_3C4;

        ((M513Player *)it)->hook = tmp;
        it = it->next;
    }
}

void fn_1_D27C(M513GameMaster *self)
{
    M513Node *it;

    M513_ASSERT(lbl_1_bss_A10 != 0, lbl_1_data_418, lbl_1_data_3DF, 0x2E);
    it = lbl_1_bss_A10->next;
    while (it != lbl_1_bss_A10) {
        M513Ptmf tmp = lbl_1_data_3B8;

        ((M513Player *)it)->hook = tmp;
        it = it->next;
    }
}

// Delete all players.
void fn_1_D34C(M513GameMaster *self)
{
    M513Node *it;
    M513Node *set;

    M513_ASSERT(lbl_1_bss_A10 != 0, lbl_1_data_418, lbl_1_data_3DF, 0x2E);
    it = lbl_1_bss_A10->next;
    set = lbl_1_bss_A10;
    while (it != set) {
        M513Node *next = it->next;

        M513PlayerDelete((M513Player *)it);
        it = next;
    }
}

// Delete all heihos.
void fn_1_D4C8(M513GameMaster *self)
{
    M513Node *it;
    M513Node *set;

    M513_ASSERT(lbl_1_bss_A14 != 0, lbl_1_data_418, lbl_1_data_3DF, 0x2E);
    it = lbl_1_bss_A14->next;
    set = lbl_1_bss_A14;
    while (it != set) {
        M513Node *next = it->next;

        fn_1_DE94((M513Heiho *)it, 1);
        it = next;
    }
}

// Delete all kuribos.
void fn_1_D56C(M513GameMaster *self)
{
    M513Node *it;
    M513Node *set;

    M513_ASSERT(lbl_1_bss_A0C != 0, lbl_1_data_418, lbl_1_data_3DF, 0x2E);
    it = lbl_1_bss_A0C->next;
    set = lbl_1_bss_A0C;
    while (it != set) {
        M513Node *next = it->next;

        fn_1_F1E4(it, 1);
        it = next;
    }
}

// Winner / loser poses.
void fn_1_D610(M513GameMaster *self)
{
    M513Node *it;
    M513Node *set;

    M513_ASSERT(lbl_1_bss_A10 != 0, lbl_1_data_418, lbl_1_data_3DF, 0x2E);
    it = lbl_1_bss_A10->next;
    set = lbl_1_bss_A10;
    while (it != set) {
        M513Player *p = (M513Player *)it;

        if (self->winner != -1) {
            if (self->winner == p->playerNo) {
                fn_1_12E90(p, 1);
            } else {
                fn_1_12E90(p, 0);
            }
        } else {
            fn_1_12E90(p, 0);
        }
        it = it->next;
    }
}

void fn_1_D6F4(void)
{
    lbl_1_bss_A00 = 0x78;
    lbl_1_bss_A04 = 0xD2;
    lbl_1_bss_A08 = 0xD2;
}

// Thread thunks: this adjusted from the thread subobject to the master.
void fn_1_D728(M513SubThread *thread)
{
    fn_1_C778((M513GameMaster *)((char *)thread - offsetof(M513GameMaster, thread)));
}

M513Master *fn_1_D730(M513SubThread *thread, s16 flag)
{
    return fn_1_C0A0(&((M513GameMaster *)((char *)thread - offsetof(M513GameMaster, thread)))->base, flag);
}

}
