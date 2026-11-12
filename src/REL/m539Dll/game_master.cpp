// m539Dll "game_master": the minigame's top-level object. It owns the sub-thread
// that runs the whole game flow (fn_1_901C), the timer, the four players and
// their game objects, and the two intrusive lists they are threaded on. Also
// holds the introsort used to rank the four results.
#define _MATH_H
#define M539_USE_DOLPHIN_HEADERS
extern "C" {
#include "dolphin.h"
#include "humath.h"
#include "game/hu3d.h"
#include "game/object.h"
#include "game/memory.h"
#include "game/gamemes.h"
#include "game/flag.h"
#include "game/gamework.h"
#include "game/audio.h"
#include "game/charman.h"
#include "game/mg/timer.h"
}
#include "REL/m539Dll.h"
#include <stddef.h>

extern "C" {

// ---------------------------------------------------------------- types

typedef struct M539SortElem_s {
    s32 idx;
    s32 score;
} M539SortElem;

typedef u8 (*M539SortCmp)(M539SortElem *a, M539SortElem *b);

typedef void *(*M539DtorFn)(void *self, s16 flag);

#ifdef __MWERKS__
void __destroy_arr(void *block, void *dtor, u32 size, u32 count);
#else
static void __destroy_arr(void *block, void *dtor, u32 size, u32 count)
{
    u8 *p = (u8 *)block + (size_t)size * count;

    while (count-- != 0) {
        p -= size;
        ((M539DtorFn)dtor)(p, -1);
    }
}
#endif

// ---------------------------------------------------------------- data

char lbl_1_data_2C0[] = "sm_ptr == NULL";
char lbl_1_data_2CF[] = "a_set.h";

void fn_1_D788(M539Player *self);
void fn_1_105F4(M539Game *self);
void fn_1_1044C(M539Game *self);
void fn_1_10B58(M539Game *self);
void *fn_1_8A80(M539GM *self, s16 flag);
void fn_1_8B38(M539GM *self, void *objman);
void fn_1_8C4C(M539GM *self);
void fn_1_947C(M539GM *self);
void *fn_1_9BD4(M539SubThread *sub, s16 flag);
void fn_1_9BCC(M539SubThread *sub);
void fn_1_901C(M539GM *self);

M539Ptmf lbl_1_data_2D8 = M539_PTMF_INIT(fn_1_D788);
M539Ptmf lbl_1_data_2E4 = M539_PTMF_INIT(fn_1_105F4);
char lbl_1_data_2F0[] = "sm_ptr != 0";
M539Ptmf lbl_1_data_2FC = M539_PTMF_INIT(fn_1_1044C);
M539Ptmf lbl_1_data_308 = M539_PTMF_INIT(fn_1_10B58);

struct M539GMVtbls_s {
    M539MasterVtbl master;
    M539ThreadVtbl sub;
} lbl_1_data_314 = {
    { { 0, 0 }, (M539DtorFn)fn_1_8A80, (void (*)(void *, void *))fn_1_8B38, (void (*)(void *))fn_1_8C4C, (void (*)(void *))fn_1_947C },
    { { 0, 0 }, (M539DtorFn)fn_1_9BD4, (s32 (*)(void *, s32))fn_1_9BCC, (void (*)(void *))fn_1_901C },
};

s32 lbl_1_bss_9F8;
s32 lbl_1_bss_9FC;
s32 lbl_1_bss_A00;
M539Node *lbl_1_bss_A04;
M539Node *lbl_1_bss_A08;
void *lbl_1_bss_A0C;
void *lbl_1_bss_A10;
void *lbl_1_bss_A14;
s32 lbl_1_bss_A18;
u8 lbl_1_bss_A1C;

// ---------------------------------------------------------------- externs

void *fn_1_0(s32 size);
void fn_1_47E4(void *ptr);
void fn_1_1264(M539Node *node);
void *fn_1_1270(M539Node *node, s16 flag);
void fn_1_1D4(const char *expr, const char *file, s32 line);
void fn_1_1C48(s32 bit);
s32 fn_1_1CE0(void);
void fn_1_1D6C(void);
void fn_1_1DE0(void);
M539Master *fn_1_1F40(M539Master *self);
M539Master *fn_1_20CC(M539Master *self, s16 flag);
void fn_1_2704(void);
void fn_1_27BC(void);
s32 fn_1_2874(void);
void fn_1_2BEC(void);
void fn_1_2C44(void);
void *fn_1_30F4(M539Model *self, s16 flag);
void fn_1_4A08(u32 seed);
M539TimerObj *fn_1_5DC4(M539TimerObj *self, s32 type);
M539TimerObj *fn_1_5E8C(M539TimerObj *self, s16 flag);
void fn_1_5EE8(M539TimerObj *self, s32 maxTime, s32 endTime, s32 recordTime);
void fn_1_5FE0(M539TimerObj *self, s32 offType);
void fn_1_60D4(M539TimerObj *self);
u8 fn_1_613C(M539TimerObj *self);
void *fn_1_6974(s16 **self, s16 flag);
void *fn_1_6ED0(void *self);
void *fn_1_70E0(void *self, s16 flag);
void *fn_1_AB68(void *self);
void *fn_1_BF70(void *self);
void *fn_1_EFF4(void *self);
M539Game *fn_1_FCF0(M539Game *self, s32 idx);
M539Player *fn_1_D0B4(M539Player *self, s32 idx, M539Game *game);
void *fn_1_F40C(void *self, s16 flag);
void fn_1_E114(M539Player *p);
void fn_1_E224(M539Player *p);
s32 fn_1_10CEC(M539Game *g, s32 idx);
u8 fn_1_10E64(void);

extern void *lbl_1_bss_9F0;

#define M539_ASSERT(cond, expr, file, line) ((cond) ? (void)0 : fn_1_1D4((expr), (file), (line)))

// ---------------------------------------------------------------- game master

M539GM *fn_1_8248(M539GM *self)
{
    M539Holder *holder;
    M539Node *list;
    void *tree;

    fn_1_1F40(&self->base);
    fn_1_558C(&self->thread, 0x8000);
    self->base.vtbl = &lbl_1_data_314.master;
    self->thread.vtbl = &lbl_1_data_314.sub;
    fn_1_5DC4(&self->timer, 0);
    holder = (M539Holder *)fn_1_0(sizeof(M539Holder));
    if (holder != 0) {
        fn_1_1264(&holder->players);
        M539_ASSERT(lbl_1_bss_A08 == 0, lbl_1_data_2C0, lbl_1_data_2CF, 0xD);
        lbl_1_bss_A08 = &holder->players;
        list = &holder->games;
        fn_1_1264(list);
        M539_ASSERT(lbl_1_bss_A04 == 0, lbl_1_data_2C0, lbl_1_data_2CF, 0xD);
        lbl_1_bss_A04 = list;
    }
    self->holder = holder;
    fn_1_4A08(OSGetTick());
    tree = fn_1_0(M539_ALLOC_SIZE(0xC));
    if (tree != 0) {
        fn_1_6ED0(tree);
    }
    return self;
}

// Owner of the holder.
void *fn_1_83FC(M539Holder *holder, s16 flag);

M539Holder **fn_1_83A4(M539Holder **self, s16 flag)
{
    if (self != 0) {
        fn_1_83FC(*self, 1);
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_86A4(void *p);
void fn_1_86A8(void *p);
void *fn_1_86AC(void **self, s16 flag);

// Destroy one game object: its sprite, owned models and buffers, then unlink.
static inline void M539DestroyGame(M539Game *g)
{
    if (&g->effect != 0) {
        fn_1_F40C(g->effect, 1);
    }
    __destroy_arr(g->sprs, (void *)fn_1_6974, sizeof(g->sprs[0]), 3);
    if (&g->head != 0) {
        fn_1_30F4(g->head, 1);
    }
    if (&g->block != 0) {
        fn_1_30F4(g->block, 1);
    }
    __destroy_arr(g->digits, (void *)fn_1_86AC, sizeof(g->digits[0]), 9);
    if (&g->roller != 0) {
        fn_1_30F4(g->roller, 1);
    }
    if (&g->board != 0) {
        fn_1_30F4(g->board, 1);
    }
    if (g != 0) {
        fn_1_1270((M539Node *)g, 0);
    }
    fn_1_C8(g);
}

// Destroy one player: model, motion list, controller, then unlink.
static inline void M539DestroyPlayer(M539Player *p)
{
    M539PlayerSub *sub;
    s16 *items;

    if (&p->model != 0) {
        fn_1_30F4(p->model, 1);
    }
    if (&p->mot != 0 && &p->mot != 0 && &p->mot != 0 && p->mot.items != 0) {
        fn_1_86A8(&p->mot);
        items = p->mot.items;
        fn_1_86A4(&p->mot);
        fn_1_C8(items);
    }
    if (&p->sub != 0) {
        sub = p->sub;
        if (sub != 0) {
            sub->vtbl->dtor(sub, 1);
        }
    }
    if (p != 0) {
        fn_1_1270((M539Node *)p, 0);
    }
    fn_1_C8(p);
}

void *fn_1_83FC(M539Holder *holder, s16 flag)
{
    M539Game *g;
    M539Player *p;
    u8 empty;

    if (holder != 0) {
        if (&holder->games != 0) {
            while (1) {
                empty = 0;
                if (holder->games.next == &holder->games && holder->games.next == holder->games.prev) {
                    empty = 1;
                }
                if (empty != 0) {
                    break;
                }
                g = (M539Game *)holder->games.prev;
                if (g != 0) {
                    M539DestroyGame(g);
                }
            }
            if (&holder->games != 0) {
                fn_1_1270(&holder->games, 0);
            }
        }
        if (holder != 0) {
            while (1) {
                empty = 0;
                if (holder->players.next == &holder->players && holder->players.next == holder->players.prev) {
                    empty = 1;
                }
                if (empty != 0) {
                    break;
                }
                p = (M539Player *)holder->players.prev;
                if (p != 0) {
                    M539DestroyPlayer(p);
                }
            }
            if (holder != 0) {
                fn_1_1270(&holder->players, 0);
            }
        }
        if (flag > 0) {
            fn_1_C8(holder);
        }
    }
    return holder;
}

void fn_1_86A4(void *p)
{
}

void fn_1_86A8(void *p)
{
}

void *fn_1_86AC(void **self, s16 flag)
{
    if (self != 0) {
        fn_1_C8(*self);
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

// list<game object> destructor.
M539Node *fn_1_8700(M539Node *list, s16 flag)
{
    M539Game *g;
    u8 empty;

    if (list != 0) {
        while (1) {
            empty = 0;
            if (list->next == list && list->next == list->prev) {
                empty = 1;
            }
            if (empty != 0) {
                break;
            }
            g = (M539Game *)list->prev;
            if (g != 0) {
                M539DestroyGame(g);
            }
        }
        if (list != 0) {
            fn_1_1270(list, 0);
        }
        if (flag > 0) {
            fn_1_C8(list);
        }
    }
    return list;
}

void *fn_1_8874(M539Node *self, s16 flag)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

// list<player> destructor.
M539Node *fn_1_88CC(M539Node *list, s16 flag)
{
    M539Player *p;
    M539PlayerSub *sub;
    u8 empty;

    if (list != 0) {
        while (1) {
            empty = 0;
            if (list->next == list && list->next == list->prev) {
                empty = 1;
            }
            if (empty != 0) {
                break;
            }
            p = (M539Player *)list->prev;
            if (p != 0) {
                if (&p->model != 0) {
                    fn_1_30F4(p->model, 1);
                }
                if (&p->mot != 0 && &p->mot != 0 && &p->mot != 0 && p->mot.items != 0) {
                    fn_1_C8(p->mot.items);
                }
                if (&p->sub != 0) {
                    sub = p->sub;
                    if (sub != 0) {
                        sub->vtbl->dtor(sub, 1);
                    }
                }
                if (p != 0) {
                    fn_1_1270((M539Node *)p, 0);
                }
                fn_1_C8(p);
            }
        }
        if (list != 0) {
            fn_1_1270(list, 0);
        }
        if (flag > 0) {
            fn_1_C8(list);
        }
    }
    return list;
}

void *fn_1_8A28(M539Node *self, s16 flag)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void *fn_1_8A80(M539GM *self, s16 flag)
{
    if (self != 0) {
        self->base.vtbl = &lbl_1_data_314.master;
        self->thread.vtbl = &lbl_1_data_314.sub;
        fn_1_70E0(lbl_1_bss_9F0, 1);
        fn_1_83A4(&self->holder, -1);
        fn_1_5E8C(&self->timer, -1);
        fn_1_5608(&self->thread, 0);
        fn_1_20CC(&self->base, 0);
        if (flag > 0) {
            fn_1_47E4(self);
        }
    }
    return self;
}

void fn_1_8B38(M539GM *self, void *objman)
{
    void *cam;
    void *fx;
    void *scores;
    M539Game *game;
    M539Player *player;
    s32 i;

    fn_1_4A08(OSGetTick());
    cam = fn_1_0(M539_ALLOC_SIZE(0x6C));
    if (cam != 0) {
        fn_1_AB68(cam);
    }
    fx = fn_1_0(M539_ALLOC_SIZE(0x44));
    if (fx != 0) {
        fn_1_BF70(fx);
    }
    scores = fn_1_0(M539_ALLOC_SIZE(0x10));
    if (scores != 0) {
        fn_1_EFF4(scores);
    }
    i = 0;
    while (i < 4) {
        game = (M539Game *)fn_1_0(sizeof(M539Game));
        if (game != 0) {
            game = fn_1_FCF0(game, i);
        }
        self->games[i] = game;
        player = (M539Player *)fn_1_0(sizeof(M539Player));
        if (player != 0) {
            player = fn_1_D0B4(player, i, game);
        }
        self->players[i] = player;
        i++;
    }
}

// Per-frame update: run the tasks, then tick every player and game object.
void fn_1_8C4C(M539GM *self)
{
    M539Node *it;
    M539Node *end;
    M539Ptmf playerHook;
    M539Ptmf gameHook;

    fn_1_56BC(&self->thread, 0);
    fn_1_1D6C();
    M539_ASSERT(lbl_1_bss_A08 != 0, lbl_1_data_2F0, lbl_1_data_2CF, 0x20);
    playerHook = lbl_1_data_2D8;
    it = lbl_1_bss_A08->next;
    end = lbl_1_bss_A08;
    while (it != end) {
        M539PtmfCall(it, &playerHook);
        it = it->next;
    }
    M539_ASSERT(lbl_1_bss_A04 != 0, lbl_1_data_2F0, lbl_1_data_2CF, 0x20);
    end = lbl_1_bss_A04;
    gameHook = lbl_1_data_2E4;
    it = lbl_1_bss_A04->next;
    while (it != end) {
        M539PtmfCall(it, &gameHook);
        it = it->next;
    }
}

u8 fn_1_8DF0(M539SortElem *a, M539SortElem *b)
{
    return b->score < a->score;
}

void fn_1_949C(M539SortElem *first, M539SortElem *last, M539SortCmp cmp);

// Rank the four players by score; returns 0 if nobody scored.
s32 fn_1_8E10(M539GM *self)
{
    M539SortElem rank[4];
    s32 i;
    s32 j;
    s32 k;
    s32 top;
    s16 place;
    s32 idx;

    rank[0].idx = 0;
    rank[0].score = 0;
    rank[1].idx = 0;
    rank[1].score = 0;
    rank[2].idx = 0;
    rank[2].score = 0;
    rank[3].idx = 0;
    rank[3].score = 0;
    i = 0;
    while (i < 4) {
        rank[i].idx = i;
        rank[i].score = self->games[i]->rollValue;
        i++;
    }
    fn_1_949C(&rank[0], &rank[4], fn_1_8DF0);
    if (rank[0].score == 0) {
        j = 0;
        while (j < 4) {
            if (_CheckFlag(0x1000F) == 0) {
                GwPlayer[j].mgCoinBonus = 3;
            }
            CharFXPlay(GwPlayerConf[j].charNo, 0x200);
            fn_1_E224(self->players[j]);
            j++;
        }
        return 0;
    }
    top = rank[0].score;
    place = 0;
    k = 0;
    while (k < 4) {
        idx = rank[k].idx;
        if (rank[0].score == rank[k].score) {
            fn_1_E114(self->players[idx]);
        } else {
            fn_1_E224(self->players[idx]);
        }
        if (top != rank[k].score) {
            top = rank[k].score;
            place++;
        }
        if (_CheckFlag(0x1000F) == 0) {
            GwPlayer[idx].mgCoinBonus = place;
        }
        k++;
    }
    return 1;
}

// The whole game flow, run on the sub-thread: each fn_1_574C is one frame.
void fn_1_901C(M539GM *self)
{
    M539Node *it;
    M539Node *end;
    M539Ptmf hookA;
    M539Ptmf hookB;
    s32 stream;
    s32 i;
    s32 r;
    s32 c;
    s32 w;
    s32 sum;
    s32 wait;
    u8 lose;
    s16 jingle;

    stream = HuAudSStreamPlay(0x12);
    fn_1_1C48(0);
    fn_1_2BEC();
    while (fn_1_1CE0() == 0) {
        fn_1_574C(&self->thread, 0);
    }
    fn_1_1C48(1);
    fn_1_2704();
    while (fn_1_2874() == 0) {
        fn_1_574C(&self->thread, 0);
    }
    fn_1_1C48(2);
    fn_1_5EE8(&self->timer, 0x258, 0, 0);
    fn_1_5FE0(&self->timer, 1);
    while (fn_1_613C(&self->timer) == 0 && fn_1_10E64() == 0) {
        fn_1_574C(&self->thread, 0);
    }
    fn_1_60D4(&self->timer);
    fn_1_1C48(3);
    HuAudSStreamFadeOut(stream, 0x64);
    fn_1_27BC();
    i = 0;
    while ((f32)i < 108.0f) {
        fn_1_574C(&self->thread, 0);
        i++;
    }
    M539_ASSERT(lbl_1_bss_A04 != 0, lbl_1_data_2F0, lbl_1_data_2CF, 0x20);
    hookA = lbl_1_data_2FC;
    end = lbl_1_bss_A04;
    it = lbl_1_bss_A04->next;
    while (it != end) {
        M539PtmfCall(it, &hookA);
        it = it->next;
    }
    hookB = lbl_1_data_308;
    it = lbl_1_bss_A04->next;
    while (it != end) {
        M539PtmfCall(it, &hookB);
        it = it->next;
    }
    while (fn_1_2874() == 0) {
        fn_1_574C(&self->thread, 0);
    }
    i = 0;
    while ((f32)i < 48.0f) {
        fn_1_574C(&self->thread, 0);
        i++;
    }
    r = 0;
    while (r < 8) {
        sum = 0;
        c = 0;
        while (c < 4) {
            sum += fn_1_10CEC(self->games[c], r);
            c++;
        }
        if (sum != 0) {
            w = 0;
            while ((f32)w < 30.0f) {
                fn_1_574C(&self->thread, 0);
                w++;
            }
        }
        r++;
    }
    HuAudFXPlay(0x7FA);
    i = 0;
    while ((f32)i < 90.0f) {
        fn_1_574C(&self->thread, 0);
        i++;
    }
    fn_1_1C48(4);
    lose = fn_1_8E10(self) == 0;
    if (lose != 0) {
        jingle = 0x7B;
    } else {
        jingle = 0x79;
    }
    HuAudJinglePlay(jingle);
    wait = lbl_1_bss_A00;
    while (wait != 0) {
        fn_1_574C(&self->thread, 0);
        wait--;
    }
    fn_1_2C44();
    while (1) {
        fn_1_574C(&self->thread, -1);
    }
}

void fn_1_947C(M539GM *self)
{
    fn_1_1DE0();
}

static inline void M539Swap(M539SortElem *a, M539SortElem *b)
{
    M539SortElem t;

    t = *a;
    *a = *b;
    *b = t;
}

void fn_1_991C(M539SortElem *a, M539SortElem *b, M539SortElem *c, M539SortCmp cmp);
void fn_1_9ABC(M539SortElem *first, M539SortElem *last, M539SortCmp cmp);

// Introsort-style quicksort of 8 byte elements; small ranges fall to selection sort.
void fn_1_949C(M539SortElem *first, M539SortElem *last, M539SortCmp cmp)
{
    M539SortElem *i;
    M539SortElem *j;
    M539SortElem *p1;
    M539SortElem *p2;
    M539SortElem *pivot;
    s32 n;

    while (1) {
        n = last - first;
        if (n <= 1) {
            return;
        }
        if (n <= 0x14) {
            fn_1_9ABC(first, last, cmp);
            return;
        }
        if ((s8)lbl_1_bss_A1C == 0) {
            lbl_1_bss_A18 = -4;
            lbl_1_bss_A1C = 1;
        }
        p1 = first + (n / 4 + lbl_1_bss_A18 % 5);
        lbl_1_bss_A18 = lbl_1_bss_A18 + 1;
        if (lbl_1_bss_A18 >= 5) {
            lbl_1_bss_A18 = -4;
        }
        p2 = first + ((n * 3) / 4 + lbl_1_bss_A18 % 5);
        lbl_1_bss_A18 = lbl_1_bss_A18 + 1;
        if (lbl_1_bss_A18 >= 5) {
            lbl_1_bss_A18 = -4;
        }
        pivot = last - 1;
        fn_1_991C(p1, p2, pivot, cmp);
        i = first;
        j = pivot;
        while (cmp(i, pivot) != 0) {
            i++;
        }
        j--;
        while (i != j && cmp(j, pivot) == 0) {
            j--;
        }
        if (i < j) {
            M539Swap(i, j);
            i++;
            while (1) {
                while (cmp(i, pivot) != 0) {
                    i++;
                }
                do {
                    j--;
                } while (cmp(j, pivot) == 0);
                if (i < j) {
                    M539Swap(i, j);
                    i++;
                    continue;
                }
                break;
            }
        }
        if (i == first) {
            M539SortElem *k;
            M539SortElem *m;

            M539Swap(i, pivot);
            k = i + 1;
            m = last - 1;
            if (cmp(first, m) == 0) {
                while (k != last && cmp(first, k) == 0) {
                    k++;
                }
                if (k < m) {
                    M539Swap(k, m);
                }
            }
            if (k < m) {
                while (1) {
                    while (cmp(first, k) == 0) {
                        k++;
                    }
                    do {
                        m--;
                    } while (cmp(first, m) != 0);
                    if (k < m) {
                        M539Swap(k, m);
                        k++;
                        continue;
                    }
                    break;
                }
            }
            first = k;
        } else if ((i - first) < (last - i)) {
            fn_1_949C(first, i, cmp);
            first = i;
        } else {
            fn_1_949C(i, last, cmp);
            last = i;
        }
    }
}

// Median of three: leaves the three sorted according to `cmp`.
void fn_1_991C(M539SortElem *a, M539SortElem *b, M539SortElem *c, M539SortCmp cmp)
{
    u8 ca;
    u8 cb;

    ca = cmp(c, a) == 0;
    cb = cmp(b, c) == 0;
    if (ca == 0 || cb == 0) {
        if (ca == 0 && cb == 0) {
            M539Swap(a, b);
            return;
        }
        if (cmp(b, a) != 0) {
            M539Swap(a, b);
        }
        if (ca != 0) {
            M539Swap(b, c);
            return;
        }
        M539Swap(a, c);
    }
}

// Selection sort.
void fn_1_9ABC(M539SortElem *first, M539SortElem *last, M539SortCmp cmp)
{
    M539SortElem *i;
    M539SortElem *min;
    M539SortElem *j;

    i = first;
    if (i != last) {
        while (i < last - 1) {
            min = i;
            if (i != last) {
                j = i + 1;
                while (j != last) {
                    if (cmp(j, min) != 0) {
                        min = j;
                    }
                    j++;
                }
            }
            if (min != i) {
                M539Swap(min, i);
            }
            i++;
        }
    }
}

// Static initialiser: the module's frame counts (120, 210, 210).
void fn_1_9B98(void)
{
    lbl_1_bss_9F8 = 0x78;
    lbl_1_bss_9FC = 0xD2;
    lbl_1_bss_A00 = 0xD2;
}

// Secondary-base thunks: the sub-thread sits 0x20 bytes into the game master.
static inline M539GM *M539GMFromSub(M539SubThread *sub)
{
    return (M539GM *)((u8 *)sub - offsetof(M539GM, thread));
}

void fn_1_9BCC(M539SubThread *sub)
{
    fn_1_901C(M539GMFromSub(sub));
}

void *fn_1_9BD4(M539SubThread *sub, s16 flag)
{
    return fn_1_8A80(M539GMFromSub(sub), flag);
}

#ifdef __MWERKS__
__declspec(section ".ctors") void (*const lbl_ctor_9B98)(void) = fn_1_9B98;
#else
struct M539GMStaticInit {
    M539GMStaticInit() { fn_1_9B98(); }
};
static M539GMStaticInit lbl_gm_static_init;
#endif

}
