// m517Dll gamemaster: the game master. It owns the three sets (players, balls,
// choros), the timer, the player model holder and the paper textures, creates
// every object of the game and runs the game flow on its sub thread.
//
// Every definition keeps C linkage: the target symbols (fn_1_A168 ...) are
// unmangled.
#define _MATH_H
#include "REL/m517Dll.h"
extern "C" {
#include "game/audio.h"
}

#ifdef __MWERKS__
#define M517_RODATA __declspec(section ".rodata")
#else
#define M517_RODATA
#endif

// Vtable of the master followed by the one of its thread base.
typedef struct M517GMVtbl_s {
    M517MasterVtbl master;
    void *thread[6];
} M517GMVtbl;

// The paper texture holder lives in papereff.cpp: eight ANIMDATA pointers.
#define M517_PAPERTEX_SIZE (8 * sizeof(void *))

typedef void (*M517PairFn)(M517Node *, M517Node *);

extern "C" {

// Objects of other units.
void fn_1_111A8(void *self);
void fn_1_11850(void *self);
void fn_1_16918(void *self);
void fn_1_17614(void *self);
void fn_1_16B68(void *self);
M517Player *fn_1_15DC8(M517Player *self, s32 playerNo);
M517PlayerModels *fn_1_15500(M517PlayerModels *self);
M517PlayerModels *fn_1_15724(M517PlayerModels *self, s16 flag);
void fn_1_184E8(M517Player *self, s32 win);
void fn_1_1593C(M517Player *self);
void fn_1_1A230(M517Player *a, M517Player *b);
M517Choro *fn_1_10634(M517Choro *self);
M517Choro *fn_1_10F88(M517Choro *self, s16 flag);
M517ChoroMgr *fn_1_124B4(M517ChoroMgr *self);
void fn_1_121D8(M517Choro *choro, M517Player *player);
struct M517PaperTex_s *fn_1_13F7C(struct M517PaperTex_s *self);
struct M517PaperTex_s *fn_1_1415C(struct M517PaperTex_s *self, s16 flag);

// Empty vector hooks.
void fn_1_A740(void);
void fn_1_A744(void);

M517SetsPtr *fn_1_A430(M517SetsPtr *self, s16 flag);
M517Sets *fn_1_A488(M517Sets *self, s16 flag);
M517Node *fn_1_A748(M517Node *self, s16 flag);
M517Node *fn_1_A808(M517Node *self, s16 flag);
M517Node *fn_1_A860(M517Node *self, s16 flag);
M517Node *fn_1_A920(M517Node *self, s16 flag);
M517Node *fn_1_A978(M517Node *self, s16 flag);
M517Node *fn_1_AB38(M517Node *self, s16 flag);
u8 fn_1_B310(M517GameMaster *self);
u8 fn_1_B414(M517GameMaster *self);
void fn_1_B4C4(M517GameMaster *self);
void fn_1_B5C0(M517GameMaster *self);
void fn_1_B660(M517GameMaster *self);
void fn_1_B730(M517GameMaster *self);
void fn_1_B8A8(M517GameMaster *self);
void fn_1_BA58(M517GameMaster *self);
void fn_1_BAFC(M517GameMaster *self);
void fn_1_BBA0(M517Node *listA, M517Node *listB, M517PairFn fn);
void fn_1_BC3C(M517Node *listA, M517Node *listB, M517PairFn fn);
void fn_1_BCD8(M517Node *listA, M517PairFn fn);
void fn_1_BD6C(void);
void fn_1_BDA0(M517SubThread *thread);
M517Master *fn_1_BDA8(M517SubThread *thread, s16 flag);

// Plain pointers: the singletons of the sets (players: input.cpp, balls: effect.cpp).
extern void *lbl_1_bss_A08;
extern void *lbl_1_bss_0;

M517Node *lbl_1_bss_A1C;
M517Camera *lbl_1_bss_A20;
s32 lbl_1_bss_A10;
s32 lbl_1_bss_A14;
s32 lbl_1_bss_A18;

// Each registered singleton keeps the state hook run for it every frame.
M517Ptmf lbl_1_data_338 = { 0, -1, (void (*)(void *))fn_1_111A8 };
M517Ptmf lbl_1_data_344 = { 0, -1, (void (*)(void *))fn_1_17614 };
M517Ptmf lbl_1_data_350 = { 0, -1, (void (*)(void *))fn_1_16B68 };
char lbl_1_data_35C[] = "sm_ptr == NULL";
char lbl_1_data_36B[9] = "a_set.h";
M517Ptmf lbl_1_data_374 = { 0, -1, (void (*)(void *))fn_1_16918 };
M517Ptmf lbl_1_data_380 = { 0, -1, (void (*)(void *))fn_1_13D4 };
M517Ptmf lbl_1_data_38C = { 0, -1, (void (*)(void *))fn_1_11850 };
char lbl_1_data_398[] = "sm_ptr != 0";
char lbl_1_data_3A4[] = "ms_pointer != 0";
char lbl_1_data_3B4[] = "singleton.h";
M517GMVtbl lbl_1_data_3C0 = {
    { { 0, 0 }, fn_1_AB90, (void (*)(M517Master *, OMOBJMAN *))fn_1_AC64, (void (*)(M517Master *))fn_1_ADA8,
      (void (*)(M517Master *))fn_1_B2CC },
    { 0, 0, (void *)fn_1_BDA8, (void *)fn_1_BDA0, (void *)fn_1_B058, 0 },
};

// The original expands both helpers into every user.
#ifdef __MWERKS__
#pragma always_inline on
#endif
// True when the list holds no element besides its sentinel.
static inline u8 M517ListEmpty(M517Node *n)
{
    u8 r = 0;

    if ((n->next == n) && (n->next == n->prev)) {
        r = 1;
    }
    return r;
}

// Delete a player the way its destructor does.
static inline void M517PlayerDelete(M517Player *p)
{
    if (p != 0) {
        if (&p->dust != 0) {
            fn_1_C4F4(p->dust, 1);
        }
        if (&p->paper != 0) {
            M517Task *t = p->paper;

            if (t != 0) {
                t->vtbl->dtor(t, 1);
            }
        }
        if (&p->ball != 0) {
            fn_1_10F8(p->ball, 1);
        }
        if (&p->input != 0) {
            M517Input *in = p->input;

            if (in != 0) {
                in->vtbl->dtor(in, 1);
            }
        }
        if (&p->model != 0) {
            fn_1_4BAC(p->model, 1);
        }
        if ((&p->mot != 0) && (&p->mot != 0) && (&p->mot != 0) && (p->mot.data != 0)) {
            s16 *data;

            fn_1_A744();
            data = p->mot.data;
            fn_1_A740();
            fn_1_1F4C(data);
        }
        if (p != 0) {
            fn_1_2DB4(&p->link, 0);
        }
        fn_1_1F4C(p);
    }
}

#ifdef __MWERKS__
#pragma always_inline off
#endif
M517GameMaster *fn_1_A168(M517GameMaster *self)
{
    M517Sets *sets;
    M517TimerObj *timer;

    fn_1_39F8(&self->base);
    fn_1_6D20(&self->thread, 0x8000);
    self->base.vtbl = &lbl_1_data_3C0.master;
    self->thread.vtbl = (M517ThreadVtbl *)lbl_1_data_3C0.thread;
    sets = (M517Sets *)fn_1_1E84(sizeof(M517Sets));
    if (sets != 0) {
        fn_1_2DA8(&sets->players);
        M517_ASSERT(lbl_1_bss_A08 == 0, lbl_1_data_35C, lbl_1_data_36B, 0x15);
        lbl_1_bss_A08 = &sets->players;
        fn_1_2DA8(&sets->balls);
        M517_ASSERT(lbl_1_bss_0 == 0, lbl_1_data_35C, lbl_1_data_36B, 0x15);
        lbl_1_bss_0 = &sets->balls;
        fn_1_2DA8(&sets->choros);
        M517_ASSERT(lbl_1_bss_A1C == 0, lbl_1_data_35C, lbl_1_data_36B, 0x15);
        lbl_1_bss_A1C = &sets->choros;
    }
    self->sets.p = sets;
    timer = (M517TimerObj *)fn_1_1E84(sizeof(M517TimerObj));
    if (timer != 0) {
        timer = fn_1_7558(timer, 0);
    }
    self->timer.p = timer;
    self->models.p = 0;
    self->paper.p = 0;
    self->win = 0;
    self->active = 0;
    return self;
}

// Owning slot of the paper textures.
M517PaperTexPtr *fn_1_A328(M517PaperTexPtr *self, s16 flag)
{
    if (self != 0) {
        fn_1_1415C(self->p, 1);
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

// Owning slot of the player models.
M517PlayerModelsPtr *fn_1_A380(M517PlayerModelsPtr *self, s16 flag)
{
    if (self != 0) {
        fn_1_15724(self->p, 1);
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

// Owning slot of the timer.
M517TimerPtr *fn_1_A3D8(M517TimerPtr *self, s16 flag)
{
    if (self != 0) {
        fn_1_7620(self->p, 1);
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

// Owning slot of the sets.
M517SetsPtr *fn_1_A430(M517SetsPtr *self, s16 flag)
{
    if (self != 0) {
        fn_1_A488(self->p, 1);
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

// Destroy the three sets and every element in them.
M517Sets *fn_1_A488(M517Sets *self, s16 flag)
{
    if (self != 0) {
        if (&self->choros != 0) {
            while (!M517ListEmpty(&self->choros)) {
                fn_1_10F88((M517Choro *)self->choros.prev, 1);
            }
            if (&self->choros != 0) {
                fn_1_2DB4(&self->choros, 0);
            }
        }
        if (&self->balls != 0) {
            while (!M517ListEmpty(&self->balls)) {
                fn_1_10F8((M517Ball *)self->balls.prev, 1);
            }
            if (&self->balls != 0) {
                fn_1_2DB4(&self->balls, 0);
            }
        }
        if (&self->players != 0) {
            while (!M517ListEmpty(&self->players)) {
                M517Player *p = (M517Player *)self->players.prev;

                M517PlayerDelete(p);
            }
            if (&self->players != 0) {
                fn_1_2DB4(&self->players, 0);
            }
        }
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

void fn_1_A740(void)
{
}

void fn_1_A744(void)
{
}

// Set of choros.
M517Node *fn_1_A748(M517Node *self, s16 flag)
{
    if (self != 0) {
        while (!M517ListEmpty(self)) {
            fn_1_10F88((M517Choro *)self->prev, 1);
        }
        if (self != 0) {
            fn_1_2DB4(self, 0);
        }
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

M517Node *fn_1_A808(M517Node *self, s16 flag)
{
    if (self != 0) {
        fn_1_2DB4(self, 0);
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

// Set of balls.
M517Node *fn_1_A860(M517Node *self, s16 flag)
{
    if (self != 0) {
        while (!M517ListEmpty(self)) {
            fn_1_10F8((M517Ball *)self->prev, 1);
        }
        if (self != 0) {
            fn_1_2DB4(self, 0);
        }
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

M517Node *fn_1_A920(M517Node *self, s16 flag)
{
    if (self != 0) {
        fn_1_2DB4(self, 0);
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

// Set of players.
M517Node *fn_1_A978(M517Node *self, s16 flag)
{
    if (self != 0) {
        while (!M517ListEmpty(self)) {
            M517Player *p = (M517Player *)self->prev;

            M517PlayerDelete(p);
        }
        if (self != 0) {
            fn_1_2DB4(self, 0);
        }
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

M517Node *fn_1_AB38(M517Node *self, s16 flag)
{
    if (self != 0) {
        fn_1_2DB4(self, 0);
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

#ifdef __MWERKS__
#pragma dont_inline on // the original keeps the call to fn_1_A430
#endif
M517Master *fn_1_AB90(M517Master *self, s16 flag)
{
    M517GameMaster *gm = (M517GameMaster *)self;

    if (self != 0) {
        gm->base.vtbl = &lbl_1_data_3C0.master;
        gm->thread.vtbl = (M517ThreadVtbl *)lbl_1_data_3C0.thread;
        if ((void *)&gm->paper != 0) {
            fn_1_1415C(gm->paper.p, 1);
        }
        if ((void *)&gm->models != 0) {
            fn_1_15724(gm->models.p, 1);
        }
        if ((void *)&gm->timer != 0) {
            fn_1_7620(gm->timer.p, 1);
        }
        fn_1_A430(&gm->sets, -1);
        fn_1_6D9C(&gm->thread, 0);
        fn_1_3B84(&gm->base, 0);
        if (flag > 0) {
            fn_1_629C(self);
        }
    }
    return self;
}

#ifdef __MWERKS__
#pragma dont_inline reset
#endif

// Master setup: create every object of the game.
void fn_1_AC64(M517GameMaster *self)
{
    M517Stage *stage;
    M517Camera *camera;
    M517ChoroMgr *mgr;
    struct M517PaperTex_s *paper;
    s32 i;

    fn_1_64C0(OSGetTick());
    stage = (M517Stage *)fn_1_1E84(sizeof(M517Stage));
    if (stage != 0) {
        stage = fn_1_E3A4(stage);
    }
    camera = (M517Camera *)fn_1_1E84(sizeof(M517Camera));
    if (camera != 0) {
        camera = fn_1_DC4C(camera);
    }
    for (i = 0; i < 4; i++) {
        M517Player *p = (M517Player *)fn_1_1E84(sizeof(M517Player));

        if (p != 0) {
            p = fn_1_15DC8(p, i);
        }
    }
    for (i = 0; i < 0xC; i++) {
        M517Choro *c = (M517Choro *)fn_1_1E84(sizeof(M517Choro));

        if (c != 0) {
            c = fn_1_10634(c);
        }
    }
    mgr = (M517ChoroMgr *)fn_1_1E84(sizeof(M517ChoroMgr));
    if (mgr != 0) {
        mgr = fn_1_124B4(mgr);
    }
    paper = (struct M517PaperTex_s *)fn_1_1E84(M517_PAPERTEX_SIZE);
    if (paper != 0) {
        paper = fn_1_13F7C(paper);
    }
    if (self->paper.p != paper) {
        fn_1_1415C(self->paper.p, 1);
        self->paper.p = paper;
    }
}

// Master update: step the thread, run every element's per-frame hook and the collisions.
void fn_1_ADA8(M517GameMaster *self)
{
    M517Node *players;
    M517Node *balls;
    M517Node *choros;
    M517Node *it;
    M517Node *end;
    M517Ptmf hook;

    fn_1_6E50(&self->thread, 0);
    fn_1_38B0();
    M517_ASSERT(lbl_1_bss_A08 != 0, lbl_1_data_398, lbl_1_data_36B, 0x2E);
    players = (M517Node *)lbl_1_bss_A08;
    M517_ASSERT(lbl_1_bss_0 != 0, lbl_1_data_398, lbl_1_data_36B, 0x2E);
    balls = (M517Node *)lbl_1_bss_0;
    M517_ASSERT(lbl_1_bss_A1C != 0, lbl_1_data_398, lbl_1_data_36B, 0x2E);
    choros = lbl_1_bss_A1C;
    hook = lbl_1_data_374;
    it = players->next;
    end = players;
    while (it != end) {
        M517_PTMF_CALL(&hook, it);
        it = it->next;
    }
    if (self->active == 1) {
        fn_1_BCD8(players, (M517PairFn)fn_1_1A230);
        fn_1_BC3C(balls, players, (M517PairFn)fn_1_1CEC);
        fn_1_BBA0(choros, players, (M517PairFn)fn_1_121D8);
    }
    hook = lbl_1_data_380;
    it = balls->next;
    end = balls;
    while (it != end) {
        M517_PTMF_CALL(&hook, it);
        it = it->next;
    }
    hook = lbl_1_data_38C;
    it = choros->next;
    end = choros;
    while (it != end) {
        M517_PTMF_CALL(&hook, it);
        it = it->next;
    }
}

// The game flow (runs on the sub thread).
void fn_1_B058(M517GameMaster *self)
{
    M517Camera *camera;
    M517PlayerModels *models;
    s32 bgm;
    s32 i;

    bgm = HuAudBGMPlay(0x53);
    fn_1_378C(0);
    fn_1_46A4();
    models = (M517PlayerModels *)fn_1_1E84(sizeof(M517PlayerModels));
    if (models != 0) {
        models = fn_1_15500(models);
    }
    if (self->models.p != models) {
        fn_1_15724(self->models.p, 1);
        self->models.p = models;
    }
    fn_1_B660(self);
    while (fn_1_3824() == 0) {
        fn_1_6EE0(&self->thread, 0);
    }
    fn_1_378C(1);
    fn_1_41BC();
    while (fn_1_432C() == 0) {
        fn_1_6EE0(&self->thread, 0);
    }
    fn_1_B730(self);
    if (self->models.p != 0) {
        fn_1_15724(self->models.p, 1);
        self->models.p = 0;
    }
    self->active = 1;
    fn_1_378C(2);
    fn_1_767C(self->timer.p, 0xA8C, 0, 0);
    fn_1_7774(self->timer.p, 1);
    while (fn_1_B310(self) == 0) {
        fn_1_6EE0(&self->thread, 0);
    }
    fn_1_B5C0(self);
    HuAudSStreamFadeOut(bgm, 0x64);
    if (self->win != 0) {
        M517_ASSERT(lbl_1_bss_A20 != 0, lbl_1_data_3A4, lbl_1_data_3B4, 0x27);
        camera = lbl_1_bss_A20;
        camera->farView = 1;
    }
    fn_1_378C(3);
    fn_1_7868(self->timer.p);
    fn_1_4274();
    for (i = 0; i < 0x78; i++) {
        fn_1_6EE0(&self->thread, 0);
    }
    while (fn_1_B414(self) == 0) {
        fn_1_6EE0(&self->thread, 0);
    }
    self->active = 0;
    fn_1_378C(4);
    fn_1_B4C4(self);
    HuAudJinglePlay(0x79);
    fn_1_4400();
    while (fn_1_432C() == 0) {
        fn_1_6EE0(&self->thread, 0);
    }
    fn_1_46FC();
    while (1) {
        fn_1_6EE0(&self->thread, 0);
    }
}

// Master finish: delete every player, ball and choro.
void fn_1_B2CC(M517GameMaster *self)
{
    fn_1_B8A8(self);
    fn_1_BA58(self);
    fn_1_BAFC(self);
    fn_1_3920();
}

// Round over check: the players that stay safe and the timer decide.
u8 fn_1_B310(M517GameMaster *self)
{
    M517Node *set;
    M517Node *it;
    M517Node *end;
    u8 ok;

    M517_ASSERT(lbl_1_bss_A08 != 0, lbl_1_data_398, lbl_1_data_36B, 0x2E);
    set = (M517Node *)lbl_1_bss_A08;
    it = set->next;
    end = set;
    ok = 1;
    while (it != end) {
        M517Player *p = (M517Player *)it;

        if (p->cpu == 0) {
            if (p->busy != 1) {
                ok = 0;
            }
        }
        it = it->next;
    }
    if (ok != 0) {
        self->win = 1;
        return 1;
    }
    if (fn_1_78D0(self->timer.p) != 0) {
        self->win = 0;
        return 1;
    }
    return 0;
}

// True once no player is still in play.
u8 fn_1_B414(M517GameMaster *self)
{
    M517Node *set;
    M517Node *it;
    M517Node *end;

    M517_ASSERT(lbl_1_bss_A08 != 0, lbl_1_data_398, lbl_1_data_36B, 0x2E);
    set = (M517Node *)lbl_1_bss_A08;
    it = set->next;
    end = set;
    while (it != end) {
        M517Player *p = (M517Player *)it;

        if (p->state > 0) {
            return 0;
        }
        it = it->next;
    }
    return 1;
}

// Tell every player whether the round was won.
void fn_1_B4C4(M517GameMaster *self)
{
    M517Node *set;
    M517Node *it;
    M517Node *end;

    M517_ASSERT(lbl_1_bss_A08 != 0, lbl_1_data_398, lbl_1_data_36B, 0x2E);
    set = (M517Node *)lbl_1_bss_A08;
    it = set->next;
    end = set;
    while (it != end) {
        M517Player *p = (M517Player *)it;

        if (self->win != 0) {
            if (p->cpu != 0) {
                fn_1_184E8(p, 1);
            } else {
                fn_1_184E8(p, 0);
            }
        } else {
            if (p->cpu != 0) {
                fn_1_184E8(p, 0);
            } else {
                fn_1_184E8(p, 1);
            }
        }
        it = it->next;
    }
}

void fn_1_B5C0(M517GameMaster *self)
{
    M517Node *set;
    M517Node *it;
    M517Node *end;

    M517_ASSERT(lbl_1_bss_A08 != 0, lbl_1_data_398, lbl_1_data_36B, 0x2E);
    set = (M517Node *)lbl_1_bss_A08;
    it = set->next;
    end = set;
    while (it != end) {
        fn_1_1593C((M517Player *)it);
        it = it->next;
    }
}

// Give every player its waiting hook.
void fn_1_B660(M517GameMaster *self)
{
    M517Node *set;
    M517Node *it;
    M517Node *end;

    M517_ASSERT(lbl_1_bss_A08 != 0, lbl_1_data_398, lbl_1_data_36B, 0x2E);
    set = (M517Node *)lbl_1_bss_A08;
    it = set->next;
    end = set;
    while (it != end) {
        M517Ptmf tmp = lbl_1_data_350;

        ((M517Player *)it)->hook = tmp;
        it = it->next;
    }
}

// Give every player its playing hook and every choro its hook.
void fn_1_B730(M517GameMaster *self)
{
    M517Node *set;
    M517Node *it;
    M517Node *end;

    M517_ASSERT(lbl_1_bss_A08 != 0, lbl_1_data_398, lbl_1_data_36B, 0x2E);
    set = (M517Node *)lbl_1_bss_A08;
    it = set->next;
    end = set;
    while (it != end) {
        M517Ptmf tmp = lbl_1_data_344;

        ((M517Player *)it)->hook = tmp;
        it = it->next;
    }
    M517_ASSERT(lbl_1_bss_A1C != 0, lbl_1_data_398, lbl_1_data_36B, 0x2E);
    set = lbl_1_bss_A1C;
    it = set->next;
    end = set;
    while (it != end) {
        M517Ptmf tmp = lbl_1_data_338;

        ((M517Choro *)it)->hook = tmp;
        it = it->next;
    }
}

// Delete all players.
void fn_1_B8A8(M517GameMaster *self)
{
    M517Node *set;
    M517Node *it;

    M517_ASSERT(lbl_1_bss_A08 != 0, lbl_1_data_398, lbl_1_data_36B, 0x2E);
    set = (M517Node *)lbl_1_bss_A08;
    it = set->next;
    while (it != set) {
        M517Node *next = it->next; // the original reads it after the delete

        M517PlayerDelete((M517Player *)it);
        it = next;
    }
}

// Delete all balls.
void fn_1_BA58(M517GameMaster *self)
{
    M517Node *set;
    M517Node *it;

    M517_ASSERT(lbl_1_bss_0 != 0, lbl_1_data_398, lbl_1_data_36B, 0x2E);
    set = (M517Node *)lbl_1_bss_0;
    it = set->next;
    while (it != set) {
        M517Node *next = it->next;

        fn_1_10F8((M517Ball *)it, 1);
        it = next;
    }
}

// Delete all choros.
void fn_1_BAFC(M517GameMaster *self)
{
    M517Node *set;
    M517Node *it;

    M517_ASSERT(lbl_1_bss_A1C != 0, lbl_1_data_398, lbl_1_data_36B, 0x2E);
    set = lbl_1_bss_A1C;
    it = set->next;
    while (it != set) {
        M517Node *next = it->next;

        fn_1_10F88((M517Choro *)it, 1);
        it = next;
    }
}

// Call fn for every (element of listA, element of listB) pair.
void fn_1_BBA0(M517Node *listA, M517Node *listB, M517PairFn fn)
{
    M517Node *outer = listA->next;
    M517Node *outerEnd = listA;

    while (outer != outerEnd) {
        M517Node *inner = listB->next;
        M517Node *innerEnd = listB;

        while (inner != innerEnd) {
            fn(outer, inner);
            inner = inner->next;
        }
        outer = outer->next;
    }
}

// Twin of fn_1_BBA0.
void fn_1_BC3C(M517Node *listA, M517Node *listB, M517PairFn fn)
{
    M517Node *outer = listA->next;
    M517Node *outerEnd = listA;

    while (outer != outerEnd) {
        M517Node *inner = listB->next;
        M517Node *innerEnd = listB;

        while (inner != innerEnd) {
            fn(outer, inner);
            inner = inner->next;
        }
        outer = outer->next;
    }
}

// Call fn for every unordered pair of listA: the inner walk starts after the
// outer element, the outer one stops before the last element.
void fn_1_BCD8(M517Node *listA, M517PairFn fn)
{
    M517Node *outer = listA->next;
    M517Node *outerEnd = listA->prev;

    while (outer != outerEnd) {
        M517Node *inner = outer->next;

        while (inner != listA) {
            fn(outer, inner);
            inner = inner->next;
        }
        outer = outer->next;
    }
}

void fn_1_BD6C(void)
{
    lbl_1_bss_A10 = 0x78;
    lbl_1_bss_A14 = 0xD2;
    lbl_1_bss_A18 = 0xD2;
}

// Thread thunks: this adjusted from the thread subobject to the master.
#ifdef __MWERKS__
asm void fn_1_BDA0(M517SubThread *thread)
{
    nofralloc
    subi r3, r3, 0x20
    b fn_1_B058
}

asm M517Master *fn_1_BDA8(M517SubThread *thread, s16 flag)
{
    nofralloc
    subi r3, r3, 0x20
    b fn_1_AB90
}
#else
void fn_1_BDA0(M517SubThread *thread)
{
    fn_1_B058((M517GameMaster *)((char *)thread - offsetof(M517GameMaster, thread)));
}

M517Master *fn_1_BDA8(M517SubThread *thread, s16 flag)
{
    return fn_1_AB90(&((M517GameMaster *)((char *)thread - offsetof(M517GameMaster, thread)))->base, flag);
}
#endif

}

// static initialiser of this unit (in .ctors on GameCube)
#ifdef __MWERKS__
__declspec(section ".ctors") void (*const lbl_ctor_BD6C)(void) = fn_1_BD6C;
#else
struct PortStaticInit_BD6C {
    PortStaticInit_BD6C() { fn_1_BD6C(); }
};
static PortStaticInit_BD6C lbl_port_init_BD6C;
#endif
