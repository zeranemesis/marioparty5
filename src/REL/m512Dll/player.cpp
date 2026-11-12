// m512Dll/REL/m512Dll/player
// Player: one runner per participant on the oval track.  Each owns two models
// (the character and a mirrored copy hooked to the water reflection), a
// std::vector<s16> motion table, a PTMF state hook (idle / run), the lap
// counter and the pad-or-CPU steering.  The module also keeps the player list
// (lbl_1_bss_A5C), the winner and the vector<s16> helpers.
#define _MATH_H
#include "types.h"
#include "game/hu3d.h"
#include "game/data.h"
#include "game/audio.h"
#include "game/pad.h"
#include "game/gamework.h"
#include "game/charman.h"
#include "game/object.h"

#ifdef __MWERKS__
#define PL_RODATA __declspec(section ".rodata")
#else
#define PL_RODATA
#endif

#define PL_ASSERT(cond, expr, file, line) \
    if (cond) {                           \
    } else {                              \
        fn_1_1D4(expr, file, line);       \
    }

// MSL sqrt(double) as the original inlines it (no static pool of its own)
extern "C" double __frsqrte(double x);
extern "C" int __float_nan[];
extern "C" int __float_huge[];

static inline double pl_sqrt(double x)
{
    if (x > 0.0) {
        double guess = __frsqrte(x);
        guess = .5 * guess * (3.0 - guess * guess * x);
        guess = .5 * guess * (3.0 - guess * guess * x);
        guess = .5 * guess * (3.0 - guess * guess * x);
        guess = .5 * guess * (3.0 - guess * guess * x);
        return x * guess;
    } else if (x == 0)
        return 0;
    else if (x)
        return *(float *)__float_nan;

    return *(float *)__float_huge;
}

struct PlNode {
    PlNode *next; // 0x00
    PlNode *prev; // 0x04
};

// GC pointer-to-member-function layout (this-adjust, vtable index, function)
struct PlPtmf {
    s32 adj;
    s32 idx;
    void (*fn)(void *);
};

// std::vector<s16> as built by this module
struct PlVec {
    u32 cap;   // 0x00
    u32 size;  // 0x04
    s16 *data; // 0x08
};

struct PlModelId {
    s16 id;
};

struct Player {
    PlNode node;        // 0x00
    u8 turnLeft;        // 0x08
    u8 hitWall;         // 0x09
    PlPtmf hook;        // 0x0C
    s32 idx;            // 0x18
    s32 charNo;         // 0x1C
    s32 pad;            // 0x20 pad number, -1 for a CPU
    s32 motion;         // 0x24
    s32 dif;            // 0x28
    f32 angle;          // 0x2C
    f32 angVel;         // 0x30
    f32 speed;          // 0x34
    s32 pushTimer;      // 0x38
    s32 unk3C;          // 0x3C
    HuVecF pos;         // 0x40
    HuVecF prevPos;     // 0x4C
    HuVecF vel;         // 0x58
    f32 speedMag;       // 0x64
    s32 ticks;          // 0x68
    f32 posAngle;       // 0x6C
    s32 rank;           // 0x70
    f32 radius;         // 0x74
    PlModelId chr;      // 0x78
    PlModelId model2;   // 0x7A
    u8 finished;        // 0x7C
    u8 winner;          // 0x7D
    f32 finishPos;      // 0x80
    u8 half;            // 0x84
    s32 laps;           // 0x88
    s16 link;           // 0x8C
    s16 hooks[4];       // 0x8E
    s32 vibTimer;       // 0x98
    u8 stepFlag;        // 0x9C
    s32 unkA0;          // 0xA0
    PlVec mot;          // 0xA4
};

extern "C" {
double atan2(double y, double x);
void *__dl__FPv(void *);
void *__nw__FUl(u32 size);
void __register_global_object(void *obj, void *dtor, void *reg);
void __ptmf_scall(void *obj);
int fprintf(void *unused, const char *fmt, ...);
void abort(void);
void *memmove(void *dst, const void *src, u32 n);
void *memcpy(void *dst, const void *src, u32 n);
extern u8 __files[];
void *fn_1_1264(void *self);
void fn_1_1270(void *self, int flag);
void fn_1_12DC(void *self);
void fn_1_1330(void *self, void *arg);
void fn_1_1D4(const char *msg, const char *file, int line);
s32 fn_1_1F30(void);
void fn_1_255C(s32 player);
void fn_1_2C94(void *self, s32 dataNum);
void *fn_1_2D18(void *self, s16 chr, s32 n);
void fn_1_2EF4(void *self, void *vec);
void fn_1_2F3C(void *self, void *vec);
void fn_1_30F4(void *self, s32 id);
u32 fn_1_4AE0(void);
void fn_1_7488(void *camera);
s32 fn_1_76BC(HuVecF *pos, s32 se);
void *fn_1_7B4C(void *self, HuVecF *pos);
void fn_1_DC94(void *score, s32 idx, s32 kind);
void fn_1_DDDC(void *score, s32 idx, s32 digits, s32 maxTime);

extern void *lbl_1_bss_9FC;
extern void *lbl_1_bss_A14;
// function-local statics of fn_1_B420 (the split put them in zako's bss)
extern f64 lbl_1_bss_A68;
extern char lbl_1_bss_A70[1];
extern f64 lbl_1_bss_A78;
extern char lbl_1_bss_A80[8];
}

extern "C" f32 fn_1_8E48(f32 a, f32 b);
extern "C" f32 fn_1_8F00(f32 a, f32 b, f32 lim);
extern "C" void fn_1_9064(HuVecF *out, s32 idx);
extern "C" void fn_1_9104(void);
extern "C" void fn_1_91AC(void);
extern "C" Player *fn_1_9250(Player *self, s32 idx);
extern "C" PlVec *fn_1_9818(PlVec *v, s16 flag);
extern "C" PlVec *fn_1_98AC(PlVec *v, s16 flag);
extern "C" PlVec *fn_1_9938(PlVec *v, s16 flag);
extern "C" void *fn_1_99BC(void *self, s16 flag);
extern "C" Player *fn_1_9A14(Player *self, s16 flag);
extern "C" s16 fn_1_9B08(Player *self, s32 dataNum);
extern "C" void fn_1_9B84(void);
extern "C" void fn_1_9C20(void);
extern "C" s32 fn_1_9EAC(u8 flag);
extern "C" s32 fn_1_A040(u8 flag);
extern "C" void fn_1_A160(void);
extern "C" void fn_1_A1F0(void);
extern "C" s32 fn_1_A23C(void);
extern "C" void fn_1_A2C8(void);
extern "C" void fn_1_A350(HuVecF *center, HuVecF *size);
extern "C" f32 fn_1_A4BC(HuVecF *out);
extern "C" void fn_1_A50C(Player *self);
extern "C" void fn_1_A604(Player *self);
extern "C" void fn_1_A6E0(Player *self);
extern "C" void fn_1_AB34(Player *self);
extern "C" void fn_1_ADEC(Player *self, s32 motion, u8 loop);
extern "C" void fn_1_AF04(Player *self);
extern "C" void fn_1_B030(Player *self);
extern "C" void fn_1_B420(Player *self);
extern "C" s32 fn_1_BE20(Player *self);
extern "C" f32 fn_1_BEFC(Player *self, f32 stick);
extern "C" void fn_1_C0C0(Player *self);
extern "C" s32 fn_1_C3C8(Player *self);
extern "C" f32 fn_1_C91C(Player *self);
extern "C" s16 *fn_1_CA88(PlVec *v, u32 idx);
extern "C" void fn_1_CB00(PlNode *head, void (*fn)(Player *));
extern "C" void fn_1_CB60(PlNode *first, PlNode *end, void (*fn)(Player *));
extern "C" void fn_1_CBC0(PlVec *v, s16 *pos, u32 n, s16 *val);
extern "C" void fn_1_CF70(PlVec *v, u32 cap);
extern "C" void fn_1_D098(void);
extern "C" void *fn_1_D120(void *self, s16 flag);

// -------------------------------------------------------------------- data

extern "C" {
s32 lbl_1_data_370[4] = { 0, 1, 2, 3 };
PlPtmf lbl_1_data_380 = { 0, -1, (void (*)(void *))fn_1_AF04 };
char lbl_1_data_38C[] = "m_mot_id.size() == NUMBEROF_MOTION";
char lbl_1_data_3AF[] = "player.cpp";
char lbl_1_data_3BA[] = "ms_pointer != 0";
char lbl_1_data_3CA[] = "singleton.h";
PlPtmf lbl_1_data_3D8 = { 0, -1, (void (*)(void *))fn_1_B030 };
char lbl_1_data_3E4[] = "vector::at index out of range";
char lbl_1_data_402[] = "vector::insert length error\n";
char lbl_1_data_41F[] = "Memory allocation failure";
char lbl_1_data_439[] = "vector::reserve length error\n";

// footstep sound ids, two per player
PL_RODATA const s32 lbl_1_rodata_274[9] = { 0x6BB, 0x6BC, 0x6BD, 0x6BE, 0x6BF, 0x6C0, 0x6C1, 0x6C2, 0 };

s32 lbl_1_bss_A40;
s32 lbl_1_bss_A44;
s32 lbl_1_bss_A48;
Player *lbl_1_bss_A4C;
char lbl_1_bss_A50[12];
PlNode lbl_1_bss_A5C;
}

static inline s16 PlId(PlModelId *p)
{
    return p->id;
}

#define PL_ITER_BEGIN                                         PlNode *pl_t1 = lbl_1_bss_A5C.next;                       PlNode *pl_t2 = pl_t1;                                    PlNode *n = pl_t2;                                        PlNode *pl_end = &lbl_1_bss_A5C;
#define PL_ITER_NEXT              {                                 PlNode *pl_nx = n->next;         n = pl_nx;                }

// singleton accessors (assert on a null instance)
static inline void *PlCamera(void)
{
    PL_ASSERT(lbl_1_bss_A14 != 0, lbl_1_data_3BA, lbl_1_data_3CA, 0x27);
    return lbl_1_bss_A14;
}

static inline void *PlScore(void)
{
    PL_ASSERT(lbl_1_bss_9FC != 0, lbl_1_data_3BA, lbl_1_data_3CA, 0x27);
    return lbl_1_bss_9FC;
}

// ------------------------------------------------------------------ functions

static inline s32 PlAngToS(f32 deg)
{
    return (s32)(182.04444444444445 * deg);
}

static inline f32 PlAngToF(s32 a)
{
    return 0.0054931640625 * a;
}

static inline f32 PlAngDiff(f32 a, f32 b)
{
    s32 ia = PlAngToS(a);
    s32 ib = PlAngToS(b);
    s32 w = (u16)((ia - ib) - 0x8000) - 0x8000;
    return PlAngToF(w);
}

// Signed angle difference a - b in degrees, wrapped to [-180, 180).
extern "C" f32 fn_1_8E48(f32 a, f32 b)
{
    return PlAngDiff(a, b);
}

extern "C" f32 fn_1_8F00(f32 a, f32 b, f32 lim)
{
    f32 d = PlAngDiff(a, b);
    f32 r;

    if (d < -lim) {
        r = -lim;
    } else if (d > lim) {
        r = lim;
    } else {
        r = d;
    }
    return r;
}

// Start position of lane idx (the lanes are shuffled by fn_1_9104).
extern "C" void fn_1_9064(HuVecF *out, s32 idx)
{
    f64 z = 100.0 + (750.0 + (f64)(lbl_1_data_370[idx] * 0x8C));
    out->x = -50.0f;
    out->y = 0.0f;
    out->z = z;
}

extern "C" void fn_1_9104(void)
{
    s32 i;
    s32 j;

    for (i = 0; i < 4; i++) {
        j = fn_1_4AE0() & 3;
        lbl_1_data_370[i] = lbl_1_data_370[j];
        lbl_1_data_370[j] = lbl_1_data_370[i];
    }
}

extern "C" void fn_1_91AC(void)
{
    u8 empty;

    do {
        empty = 0;
        if (lbl_1_bss_A5C.next == &lbl_1_bss_A5C && lbl_1_bss_A5C.next == lbl_1_bss_A5C.prev) {
            empty = 1;
        }
        if (empty == 0) {
            fn_1_9A14((Player *)lbl_1_bss_A5C.prev, 1);
        }
    } while (empty == 0);
}

extern "C" Player *fn_1_9250(Player *self, s32 idx)
{
    Player *self0 = self;
    s32 i;
    s16 hookMdl;
    char *hookName;
    s16 m;
    void *sc;
    s16 padv;
    u32 sz;

    fn_1_1264(self0);
    self->unk3C = 0;
    fn_1_2D18(&self->chr, GwPlayerConf[idx].charNo, 3);
    fn_1_2C94(&self->model2, 0x460006);
    self->stepFlag = 0;
    self->unkA0 = 0;
    {
        PlVec *v = &self->mot;
        PlVec *v2 = v;
        v2->cap = 0;
        v2->size = 0;
        v2->data = 0;
    }
    fn_1_1330(self, &lbl_1_bss_A5C);
    self->idx = idx;
    self->charNo = GwPlayerConf[idx].charNo;
    self->dif = GwPlayerConf[idx].dif;
    if (GwPlayerConf[idx].type != 0) {
        padv = -1;
    } else {
        padv = GwPlayerConf[idx].padNo;
    }
    self->pad = padv;
    self->hitWall = 0;
    self->angle = 0.0f;
    self->angVel = 0.0f;
    self->motion = -1;
    self->pushTimer = 0;
    self->half = 0;
    self->laps = 0;
    self->finished = 0;
    self->winner = 0;
    self->speed = 0.0f;
    self->vibTimer = 60;
    self->mot.size = 0;
    fn_1_CF70(&self->mot, 4);
    fn_1_9B08(self, 0x87008F);
    fn_1_9B08(self, 0x870090);
    fn_1_9B08(self, 0x8C006C);
    fn_1_9B08(self, 0x8C006D);
    sz = self->mot.size;
    PL_ASSERT(sz == 4, lbl_1_data_38C, lbl_1_data_3AF, 0x95);
    CharMotionDataClose((s16)self->charNo);
    m = PlId(&self->chr);
    Hu3DModelShadowSet(m);
    Hu3DModelShadowDispOn(m);
    Hu3DModelLayerSet(m, 3);
    Hu3DModelCameraSet(m, 1);
    Hu3DModelAttrSet(m, 0x4000);
    fn_1_9064(&self->pos, idx);
    self->angle = fn_1_C91C(self);
    fn_1_C3C8(self);
    self->prevPos.x = self->pos.x;
    self->prevPos.y = self->pos.y;
    self->prevPos.z = self->pos.z;
    self->vel.z = 0.0f;
    self->vel.y = 0.0f;
    self->vel.x = 0.0f;
    self->speedMag = 0.0f;
    self->rank = 0;
    self->ticks = 1;
    self->radius = 750.0f;
    fn_1_2EF4(&self->chr, &self->pos);
    PlPtmf t = lbl_1_data_380;
    self->hook = t;
    self->link = Hu3DModelLink(PlId(&self->chr));
    Hu3DModelShadowReset(self->link);
    Hu3DModelLayerSet(self->link, 0);
    Hu3DModelCameraSet(self->link, 1);
    Hu3DModelAttrReset(self->link, 1);
    fn_1_DC94(PlScore(), self->idx, self->charNo);
    sc = PlScore();
    fn_1_DDDC(sc, self->idx, self->laps, self->ticks);
    self->hooks[0] = self->model2.id;
    self->hooks[1] = Hu3DModelLink(self->model2.id);
    self->hooks[2] = Hu3DModelLink(self->model2.id);
    self->hooks[3] = Hu3DModelLink(self->model2.id);
    for (i = 0; i < 4; i++) {
        hookMdl = self->hooks[i];
        Hu3DModelShadowSet(hookMdl);
        Hu3DModelShadowDispOn(hookMdl);
        Hu3DModelCameraSet(hookMdl, 1);
        Hu3DModelAttrReset(self->link, 1);
        if ((i & 1) != 0) {
            hookName = CharModelItemHookGet((s16)self->charNo, 8, 2);
        } else {
            hookName = CharModelItemHookGet((s16)self->charNo, 8, 3);
        }
        if (i < 2) {
            Hu3DModelHookSet(self->chr.id, hookName, hookMdl);
            Hu3DModelLayerSet(hookMdl, 3);
        } else {
            Hu3DModelHookSet(self->link, hookName, hookMdl);
            Hu3DModelLayerSet(hookMdl, 0);
        }
    }
    return self;
}

// vector<s16> destructors (three inlining depths)
extern "C" PlVec *fn_1_9818(PlVec *v, s16 flag)
{
    if (v != 0) {
        if (&v->cap != 0) {
            if (&v->cap != 0) {
                s16 **dp = &v->data;
                if (*dp != 0) {
                    s16 **dp2 = &v->data;
                    s16 *d = *dp2;
                    __dl__FPv(d);
                }
            }
        }
        if (flag > 0) {
            __dl__FPv(v);
        }
    }
    return v;
}

extern "C" PlVec *fn_1_98AC(PlVec *v, s16 flag)
{
    if (v != 0) {
        if (&v->cap != 0) {
            s16 **dp = &v->data;
            if (*dp != 0) {
                s16 **dp2 = &v->data;
                s16 *d = *dp2;
                __dl__FPv(d);
            }
        }
        if (flag > 0) {
            __dl__FPv(v);
        }
    }
    return v;
}

extern "C" PlVec *fn_1_9938(PlVec *v, s16 flag)
{
    if (v != 0) {
        s16 **dp = &v->data;
        if (*dp != 0) {
            s16 **dp2 = &v->data;
            s16 *d = *dp2;
            __dl__FPv(d);
        }
        if (flag > 0) {
            __dl__FPv(v);
        }
    }
    return v;
}

extern "C" void *fn_1_99BC(void *self, s16 flag)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

extern "C" Player *fn_1_9A14(Player *self, s16 flag)
{
    if (self != 0) {
        Hu3DModelKill(self->hooks[1]);
        Hu3DModelKill(self->hooks[2]);
        Hu3DModelKill(self->hooks[3]);
        Hu3DModelKill(self->link);
        if (&self->mot != 0 && &self->mot != 0 && &self->mot != 0 && self->mot.data != 0) {
            __dl__FPv(self->mot.data);
        }
        fn_1_30F4(&self->model2, -1);
        fn_1_30F4(&self->chr, -1);
        if (self != 0) {
            fn_1_1270(self, 0);
        }
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

#pragma dont_inline on
extern "C" s16 fn_1_9B08(Player *self, s32 dataNum)
{
    s16 mot;
    s16 *end;

    mot = CharMotionCreate((s16)self->charNo, dataNum);
    end = self->mot.data + self->mot.size;
    fn_1_CBC0(&self->mot, end, 1, &mot);
    return mot;
}

#pragma dont_inline reset

extern "C" void fn_1_9B84(void)
{
    PlNode *first;
    PlNode *t1;
    PlNode *t2;

    fn_1_CB00(&lbl_1_bss_A5C, fn_1_A50C);
    fn_1_CB00(&lbl_1_bss_A5C, fn_1_A6E0);
    fn_1_9C20();
    t1 = lbl_1_bss_A5C.next;
    t2 = t1;
    first = t2;
    if (first != &lbl_1_bss_A5C) {
        fn_1_12DC(first);
        fn_1_1330(first, &lbl_1_bss_A5C);
    }
}

// Ranks the players by distance covered (laps * 360 + angle round the oval).
extern "C" void fn_1_9C20(void)
{
    Player *pl[4];
    f32 dist[4];
    PlNode *t1 = lbl_1_bss_A5C.next;
    PlNode *t2 = t1;
    PlNode *n = t2;
    PlNode *end = &lbl_1_bss_A5C;
    s32 order[4] = { 0, 1, 2, 3 };
    s32 cnt = 0;
    s32 i;
    s32 j;
    s32 rank;
    f32 ta;
    f32 tb;

    while (n != end) {
        Player *p = (Player *)n;
#ifdef TARGET_PC
        if (cnt >= 4) {
            break;
        }
#endif
        pl[cnt] = p;
        dist[cnt] = p->posAngle + (f32)(p->laps * 0x168);
        p->rank = 0;
        cnt++;
        {
            PlNode *nx = n->next;
            n = nx;
        }
    }
    if (fn_1_1F30() == 2) {
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                if (dist[j] < dist[j + 1]) {
                    ta = dist[j];
                    tb = dist[j + 1];
                    dist[j] = tb;
                    dist[j + 1] = ta;
                    ta = order[j];
                    tb = order[j + 1];
                    order[j] = (s32)tb;
                    order[j + 1] = (s32)ta;
                }
            }
        }
        rank = 0;
        for (i = 0; i < 4; i++) {
            pl[order[i]]->rank = rank;
            if (i < 3 && dist[i] != dist[i + 1]) {
                rank++;
            }
        }
    }
}

// Picks the winner (flag == 0) or tells whether all four have finished.
extern "C" s32 fn_1_9EAC(u8 flag)
{
    f32 best;
    s32 cnt;

    PL_ITER_BEGIN
    cnt = 0;
    if (flag != 0) {
        while (n != pl_end) {
            if (((Player *)n)->finished != 0) {
                cnt++;
            }
            PL_ITER_NEXT
        }
        if (cnt >= 4) {
            return 1;
        }
        return 0;
    }
    best = -1.0f;
    while (n != pl_end) {
        Player *p = (Player *)n;
        if (p->finished != 0 && p->finishPos > best) {
            if (lbl_1_bss_A4C != 0) {
                lbl_1_bss_A4C->winner = 0;
            }
            best = p->finishPos;
            p->winner = 1;
            lbl_1_bss_A4C = p;
            cnt++;
        }
        PL_ITER_NEXT
    }
    if (cnt == 0) {
        return 0;
    }
    fn_1_255C(lbl_1_bss_A4C->idx);
    fn_1_7488(PlCamera());
    return 1;
}

extern "C" s32 fn_1_A040(u8 flag)
{
    s32 cnt;

    PL_ITER_BEGIN
    cnt = 0;
    if (flag != 0) {
        while (n != pl_end) {
            Player *p = (Player *)n;
            if (p->finished != 0) {
                GwPlayer[p->idx].mgScore = p->ticks;
                cnt++;
            } else {
                GwPlayer[p->idx].mgScore = 0;
            }
            PL_ITER_NEXT
        }
    } else {
        while (n != pl_end) {
            Player *p = (Player *)n;
            if (p->finished != 0) {
                p->finished = 0;
                cnt++;
            }
            PL_ITER_NEXT
        }
        if (lbl_1_bss_A4C != 0) {
            lbl_1_bss_A4C->finished = 1;
        }
    }
    if (cnt > 0) {
        return 0;
    }
    return 1;
}

extern "C" void fn_1_A160(void)
{
    s16 chars[4];
    s32 cnt;

#ifdef TARGET_PC
    chars[0] = chars[1] = chars[2] = chars[3] = 0;
#endif
    PL_ITER_BEGIN
    cnt = 0;
    while (n != pl_end) {
#ifdef TARGET_PC
        if (cnt >= 4) {
            break;
        }
#endif
        chars[cnt++] = ((Player *)n)->charNo;
        PL_ITER_NEXT
    }
    CharLoseVoicePlay(chars[0], chars[1], chars[2], chars[3]);
}

extern "C" void fn_1_A1F0(void)
{
    if (lbl_1_bss_A4C != 0) {
        CharFXPlay((s16)lbl_1_bss_A4C->charNo, 0x1FA);
    }
}

extern "C" s32 fn_1_A23C(void)
{
    s32 best;

    best = 0x4650;
    PL_ITER_BEGIN
    while (n != pl_end) {
        Player *p = (Player *)n;
        if (p->finished != 0 && p->pad >= 0 && p->ticks < best) {
            best = p->ticks;
        }
        PL_ITER_NEXT
    }
    return best;
}

extern "C" void fn_1_A2C8(void)
{
    if (lbl_1_bss_A4C != 0) {
        fn_1_7488(PlCamera());
    }
}

// Bounding box of all players: centre and extent on x/z.
extern "C" void fn_1_A350(HuVecF *center, HuVecF *size)
{
    f32 maxX;
    f32 minX;
    f32 maxZ;
    f32 minZ;

    PL_ITER_BEGIN
    maxX = 0.0f;
    minX = 0.0f;
    maxZ = 0.0f;
    minZ = 0.0f;
    while (n != pl_end) {
        Player *p = (Player *)n;
        if (p->pos.x > maxX) {
            maxX = p->pos.x;
        }
        if (p->pos.x < minX) {
            minX = p->pos.x;
        }
        if (p->pos.z > maxZ) {
            maxZ = p->pos.z;
        }
        if (p->pos.z < minZ) {
            minZ = p->pos.z;
        }
        PL_ITER_NEXT
    }
    center->x = (maxX + minX) * 0.5f;
    center->y = 0.0f;
    center->z = (maxZ + minZ) * 0.5f;
    size->x = maxX - minX;
    size->y = 0.0f;
    size->z = maxZ - minZ;
}

extern "C" f32 fn_1_A4BC(HuVecF *out)
{
    out->x = lbl_1_bss_A4C->pos.x;
    out->y = lbl_1_bss_A4C->pos.y;
    out->z = lbl_1_bss_A4C->pos.z;
    return lbl_1_bss_A4C->angle;
}

extern "C" void fn_1_A50C(Player *self)
{
#ifdef TARGET_PC
    self->hook.fn(self);
#else
    __ptmf_scall(self);
#endif
    self->hitWall = 0;
    fn_1_AB34(self);
    self->hitWall = self->hitWall | fn_1_C3C8(self);
    fn_1_A604(self);
    if (fn_1_1F30() == 2) {
        if ((f32)self->vibTimer < 6.0f && self->pad >= 0 && self->finished == 0) {
            omVibrate((s16)self->idx, 12, 12, 0);
        }
        self->vibTimer = self->vibTimer + 1;
    }
}

extern "C" void fn_1_A604(Player *self)
{
    HuVecF rot;

    rot.x = 0.0f;
    rot.y = self->angle;
    rot.z = 0.0f;
    fn_1_2F3C(&self->chr, &rot);
    fn_1_2EF4(&self->chr, &self->pos);
    Hu3DModelScaleSet(self->link, 1.0f, -1.0f, 1.0f);
    Hu3DModelAttrSet(self->link, 0x800000);
    Hu3DModelRotSet(self->link, rot.x, rot.y, rot.z);
    Hu3DModelPosSet(self->link, self->pos.x, self->pos.y, self->pos.z);
}

// Speed vector, lap counting at the x = 0 crossings, angle round the oval.
extern "C" void fn_1_A6E0(Player *self)
{
    f32 dx;
    f32 dz;
    f32 ang;
    s32 t;

    dx = self->pos.x - self->prevPos.x;
    self->vel.x = dx;
    self->vel.y = self->pos.y - self->prevPos.y;
    dz = self->pos.z - self->prevPos.z;
    self->vel.z = dz;
    self->speedMag = pl_sqrt(dx * dx + dz * dz) / 100.0;
    if (fn_1_1F30() == 2 && self->finished == 0) {
        if (self->pos.z > 0.0f) {
            if (self->half != 0) {
                if (self->pos.x >= 0.0f && self->prevPos.x < 0.0f) {
                    self->laps = self->laps + 1;
                    self->half = 0;
                    if (self->laps >= 5) {
                        self->finished = 1;
                        self->finishPos = self->pos.x;
                    }
                    fn_1_DDDC(PlScore(), self->idx, self->laps, self->ticks);
                    if ((self->idx & 1) != 0) {
                        HuAudFXPlay(0x6C7);
                    } else {
                        HuAudFXPlay(0x6C6);
                    }
                }
            }
        } else {
            if (self->pos.x <= 0.0f && self->prevPos.x > 0.0f) {
                self->half = 1;
            }
        }
    }
    self->prevPos.x = self->pos.x;
    self->prevPos.y = self->pos.y;
    self->prevPos.z = self->pos.z;
    if (fn_1_1F30() != 2 || self->finished == 1) {
        return;
    }
    t = self->ticks;
    self->ticks = t + 1;
    ang = 180.0 * (atan2(self->pos.x, self->pos.z) / 3.141592653589793);
    if (ang < 0.0f) {
        ang += 360.0f;
    }
    self->posAngle = ang;
}

// Pushes overlapping players apart.
extern "C" void fn_1_AB34(Player *self)
{
    HuVecF a;
    HuVecF b;
    HuVecF d;
    HuVecF other;
    f32 dist;
    f32 w;
    f32 w2;

    a.x = self->pos.x;
    a.y = self->pos.y;
    a.z = self->pos.z;
    if (self->pushTimer > 0) {
        self->pushTimer = self->pushTimer - 1;
    }
    PL_ITER_BEGIN
    while (n != pl_end) {
        Player *p = (Player *)n;
        if (self != p) {
            other.x = p->pos.x;
            other.y = p->pos.y;
            other.z = p->pos.z;
            PSVECSubtract((Vec *)&p->pos, (Vec *)&self->pos, (Vec *)&d);
            dist = PSVECMag((Vec *)&d);
            if (dist < 120.0f) {
                w = 0.5f;
                w2 = 1.0f - w;
                PSVECScale((Vec *)&d, (Vec *)&d, (120.0f - dist) / dist);
                PSVECScale((Vec *)&d, (Vec *)&a, 1.05f * -w);
                PSVECScale((Vec *)&d, (Vec *)&b, 1.05f * w2);
                PSVECAdd((Vec *)&a, (Vec *)&self->pos, (Vec *)&self->pos);
                PSVECAdd((Vec *)&b, (Vec *)&p->pos, (Vec *)&p->pos);
                if (w2 >= 0.5f) {
                    p->pushTimer = 5;
                } else {
                    p->pushTimer = 0;
                }
                if ((f32)self->vibTimer > 30.0f) {
                    self->vibTimer = 0;
                }
                if ((f32)p->vibTimer > 30.0f) {
                    p->vibTimer = 0;
                }
                if (self->vibTimer == 0 || p->vibTimer == 0) {
                    fn_1_76BC(&self->pos, 0x6C3);
                }
                self->hitWall = self->hitWall | fn_1_C3C8(self);
                p->hitWall = p->hitWall | fn_1_C3C8(p);
            }
        }
        PL_ITER_NEXT
    }
}

extern "C" void fn_1_ADEC(Player *self, s32 motion, u8 loop)
{
    s16 mdl;
    s16 mot;
    u32 attr;

    mdl = self->chr.id;
    mot = *fn_1_CA88(&self->mot, motion);
    if (self->motion != motion) {
        CharModelAttrReset((s16)self->charNo, 0x40000002);
        if (loop == 0) {
            attr = 0x40000001;
        } else {
            attr = 0;
        }
        CharMotionShiftSet((s16)self->charNo, mot, 0.0f, 8.0f, attr);
        Hu3DModelAttrReset(self->link, 0x40000002);
        if (loop == 0) {
            attr = 0x40000001;
        } else {
            attr = 0;
        }
        Hu3DMotionShiftSet(self->link, mot, 0.0f, 8.0f, attr);
        self->motion = motion;
    }
}

extern "C" void fn_1_AF04(Player *self)
{
    s16 mdl;
    s16 mot;
    PlPtmf t;

    mdl = self->chr.id;
    mot = *fn_1_CA88(&self->mot, 0);
    if (self->motion != 0) {
        CharModelAttrReset((s16)self->charNo, 0x40000002);
        CharMotionShiftSet((s16)self->charNo, mot, 0.0f, 8.0f, 0x40000001);
        Hu3DModelAttrReset(self->link, 0x40000002);
        Hu3DMotionShiftSet(self->link, mot, 0.0f, 8.0f, 0x40000001);
        self->motion = 0;
    }
    t = lbl_1_data_3D8;
    self->hook = t;
}

extern "C" void fn_1_B030(Player *self)
{
    s16 mdl;
    s16 mot;

    self->angle = fn_1_C91C(self);
    fn_1_C0C0(self);
    self->angle = self->angle + self->angVel;
    if (self->angle > 180.0f) {
        self->angle = self->angle - 360.0f;
    }
    if (self->angle < -180.0f) {
        self->angle = self->angle + 360.0f;
    }
    fn_1_B420(self);
    if (fn_1_BE20(self) != 0) {
        if (self->winner != 0) {
            CharMotionSpeedSet((s16)self->charNo, 1.0f);
            Hu3DMotionSpeedSet(self->link, 1.0f);
            mdl = self->chr.id;
            mot = *fn_1_CA88(&self->mot, 3);
            if (self->motion != 3) {
                CharModelAttrReset((s16)self->charNo, 0x40000002);
                CharMotionShiftSet((s16)self->charNo, mot, 0.0f, 8.0f, 0x40000001);
                Hu3DModelAttrReset(self->link, 0x40000002);
                Hu3DMotionShiftSet(self->link, mot, 0.0f, 8.0f, 0x40000001);
                self->motion = 3;
            }
        } else if (self->turnLeft != 0) {
            mdl = self->chr.id;
            mot = *fn_1_CA88(&self->mot, 2);
            if (self->motion != 2) {
                CharModelAttrReset((s16)self->charNo, 0x40000002);
                CharMotionShiftSet((s16)self->charNo, mot, 0.0f, 8.0f, 0x40000001);
                Hu3DModelAttrReset(self->link, 0x40000002);
                Hu3DMotionShiftSet(self->link, mot, 0.0f, 8.0f, 0x40000001);
                self->motion = 2;
            }
        } else {
            mdl = self->chr.id;
            mot = *fn_1_CA88(&self->mot, 1);
            if (self->motion != 1) {
                CharModelAttrReset((s16)self->charNo, 0x40000002);
                CharMotionShiftSet((s16)self->charNo, mot, 0.0f, 8.0f, 0x40000001);
                Hu3DModelAttrReset(self->link, 0x40000002);
                Hu3DMotionShiftSet(self->link, mot, 0.0f, 8.0f, 0x40000001);
                self->motion = 1;
            }
        }
    } else {
        mdl = self->chr.id;
        mot = *fn_1_CA88(&self->mot, 0);
        if (self->motion != 0) {
            CharModelAttrReset((s16)self->charNo, 0x40000002);
            CharMotionShiftSet((s16)self->charNo, mot, 0.0f, 8.0f, 0x40000001);
            Hu3DModelAttrReset(self->link, 0x40000002);
            Hu3DMotionShiftSet(self->link, mot, 0.0f, 8.0f, 0x40000001);
            self->motion = 0;
        }
    }
}

// Integrates the velocity in the player's own frame, with drag, and
// spawns footstep dust / sounds.
extern "C" void fn_1_B420(Player *self)
{
    Mtx m;
    Mtx inv;
    HuVecF dir;
    HuVecF out;
    HuVecF tmp;
    HuVecF p1;
    HuVecF p2;
    f64 vx;
    f64 vy;
    f64 vz;
    f64 mag;
    f32 mult;
    f32 w;
    f32 ax;
    char *name1;
    char *name2;
    s32 k;
    void *puff;

    if ((s8)lbl_1_bss_A70[0] == 0) {
        lbl_1_bss_A68 = 60.0;
        lbl_1_bss_A70[0] = 1;
    }
    vx = (lbl_1_bss_A68 * (f64)self->vel.x) / 100.0;
    vy = (lbl_1_bss_A68 * (f64)self->vel.y) / 100.0;
    vz = (lbl_1_bss_A68 * (f64)self->vel.z) / 100.0;
    mag = pl_sqrt(vx * vx + vz * vz);
    dir.x = 0.0f;
    dir.y = 0.0f;
    dir.z = mag;
    PSMTXRotRad(m, 'y', 0.017453292f * self->angle);
    PSMTXMultVec(m, (Vec *)&dir, (Vec *)&out);
    mult = 1.0f;
    if (self->pad < 0) {
        f32 tbl[4] = { 0.43f, 0.7f, 0.9f, 1.0f };
        mult = tbl[self->dif];
    }
    dir.x = 0.0f;
    dir.y = 0.0f;
    dir.z = (f64)mult * (0.003 * (f64)self->speed * (f64)(1.0f + 0.1f * (f32)self->rank));
    if (self->angle > 180.0f) {
        self->angle = self->angle - 360.0f;
    }
    if (self->angle < -180.0f) {
        self->angle = self->angle + 360.0f;
    }
    PSMTXRotRad(m, 'y', 0.017453292f * (f32)((f64)self->angle + (0.05 * (f64)self->angVel)));
    PSMTXMultVec(m, (Vec *)&dir, (Vec *)&out);
    if ((s8)lbl_1_bss_A80[0] == 0) {
        lbl_1_bss_A78 = 3600.0 / lbl_1_bss_A68;
        lbl_1_bss_A80[0] = 1;
    }
    w = 1.0f;
    dir.x = (0.016666666666666666 * ((59.0 * vx) + (f64)out.x)) + (lbl_1_bss_A78 * (f64)out.x);
    dir.y = (0.016666666666666666 * ((59.0 * vy) + (f64)out.y)) + (lbl_1_bss_A78 * (f64)out.y);
    dir.z = (0.016666666666666666 * ((59.0 * vz) + (f64)out.z)) + (lbl_1_bss_A78 * (f64)out.z);
    PSMTXInverse(m, inv);
    PSMTXMultVec(inv, (Vec *)&dir, (Vec *)&out);
    if (self->hitWall != 0) {
        if ((f32)self->vibTimer > 30.0f) {
            self->vibTimer = 0;
        }
        w = 1.0f * 10.0f;
    }
    if (self->speed <= 0.3f) {
        w *= 1.3f;
    }
    if (self->finished == 1) {
        w *= 3.0f;
    }
    tmp.z = (f64)out.z - (0.003 * (f64)out.z * (f64)w);
    tmp.x = (f64)out.x - (0.008 * (f64)out.x * (f64)w);
    tmp.y = 0.0f;
    PSMTXMultVec(m, (Vec *)&tmp, (Vec *)&out);
    self->pos.x = (f64)self->pos.x + (100.0 * ((f64)out.x / lbl_1_bss_A68));
    self->pos.y = (f64)self->pos.y + (100.0 * ((f64)out.y / lbl_1_bss_A68));
    self->pos.z = (f64)self->pos.z + (100.0 * ((f64)out.z / lbl_1_bss_A68));
    self->hitWall = 0;
    name1 = CharModelItemHookGet((s16)self->charNo, 8, 2);
    name2 = CharModelItemHookGet((s16)self->charNo, 8, 3);
    Hu3DModelObjPosGet(self->chr.id, name1, &p1);
    Hu3DModelObjPosGet(self->chr.id, name2, &p2);
    if (self->speedMag > 0.01f) {
        if (p1.y < p2.y) {
            if (self->stepFlag == 1) {
                self->stepFlag = 0;
                k = self->idx * 2 + 1;
                fn_1_76BC(&self->pos, lbl_1_rodata_274[k]);
            }
            if (out.x < 0.0f) {
                ax = -out.x;
            } else {
                ax = out.x;
            }
            if (ax > 10.0f && (self->ticks & 1) != 0) {
                puff = __nw__FUl(0x10);
                if (puff != 0) {
                    fn_1_7B4C(puff, &p1);
                }
            }
        } else {
            if (self->stepFlag == 0) {
                self->stepFlag = 1;
                k = self->idx * 2;
                fn_1_76BC(&self->pos, lbl_1_rodata_274[k]);
            }
            if (out.x < 0.0f) {
                ax = -out.x;
            } else {
                ax = out.x;
            }
            if (ax > 10.0f && (self->ticks & 1) != 0) {
                puff = __nw__FUl(0x10);
                if (puff != 0) {
                    fn_1_7B4C(puff, &p2);
                }
            }
        }
    }
}

extern "C" s32 fn_1_BE20(Player *self)
{
    if (self->speed < 0.01f) {
        CharMotionSpeedSet((s16)self->charNo, 1.0f);
        Hu3DMotionSpeedSet(self->link, 1.0f);
        return 0;
    }
    CharMotionSpeedSet((s16)self->charNo, 0.75 * (1.5f * self->speed));
    Hu3DMotionSpeedSet(self->link, 0.75 * (1.5f * self->speed));
    return 1;
}

// CPU steering: stick value for the given lateral error.
extern "C" f32 fn_1_BEFC(Player *self, f32 stick)
{
    u8 inStraight;
    u8 nearEnd;
    f32 k;
    f32 r;

    inStraight = 0;
    nearEnd = 0;
    k = 0.55f;
    switch (self->dif) {
    case 1:
        k = 1.0f;
        break;
    case 2:
        k = 1.65f;
        break;
    case 3:
        k = 1.8f;
        break;
    }
    if (self->dif > 0) {
        if (self->pos.x > -300.0 && self->pos.x < 300.0) {
            inStraight = 1;
        }
        if (self->pos.x > 150.0f && self->pos.z > 0.0f) {
            nearEnd = 1;
        }
        if (self->pos.x < -150.0f && self->pos.z < 0.0f) {
            nearEnd = 1;
        }
        if ((self->laps > 0 || self->half != 0) && inStraight == 0 && nearEnd != 0) {
            k *= 2.0f;
        }
    }
    r = stick * k;
    if (r < -1.0f) {
        r = -1.0f;
    }
    if (inStraight != 0 && nearEnd == 0) {
        r = 0.0f;
    }
    return r;
}

// Reads the pad (or the CPU) and updates speed / angular velocity.
extern "C" void fn_1_C0C0(Player *self)
{
    f32 stick;
    u8 run;
    u16 btn;
    s32 mode;

#ifdef TARGET_PC
    btn = self->pad >= 0 ? HuPadBtn[self->pad] : 0;
#else
    btn = HuPadBtn[self->pad];
#endif
    run = 0;
    mode = fn_1_1F30();
    if (mode == 0 || mode == 1) {
        return;
    }
    if (self->pad < 0 || self->finished == 1 || mode != 2) {
        if (self->finished == 0) {
            stick = fn_1_BEFC(self, -((f64)self->radius - 750.0) / 700.0);
            run = 1;
        } else {
            stick = (f32)(-((f64)self->radius - 750.0) / 700.0) + 0.5f;
            if (stick < 0.0f) {
                stick *= 2.0f;
            }
            if (stick < -1.0f) {
                stick = -1.0f;
            }
            run = 1;
        }
    } else {
        stick = 0.017857144f * (f32)HuPadStkX[self->pad];
        if ((btn & 0x100) != 0) {
            run = 1;
        }
    }
    self->turnLeft = 0;
    if (stick < -0.5f) {
        self->turnLeft = 1;
    }
    if (run != 0) {
        self->speed = 1.0f;
    } else {
        self->speed = (f64)self->speed - 0.15;
        if (self->speed < 0.0f) {
            self->speed = 0.0f;
        }
    }
    if (self->speed > 1.0f) {
        self->speed = 1.0f;
    }
    if ((self->finished == 1 || mode != 2) && self->speed > 0.3f) {
        self->speed = 0.3f;
    }
    self->angVel = self->angVel - 8.0f * stick;
    self->angVel = self->angVel * 0.9f;
}

#pragma dont_inline on
// Keeps a player inside the arena (two end caps and the straight sections);
// returns 1 when it hit a wall.
extern "C" s32 fn_1_C3C8(Player *self)
{
    f32 z;
    f32 a;
    f64 cx;
    f64 ex;
    f64 d;

    if (self->pos.x > -300.0 && self->pos.x < 300.0) {
        if (self->pos.z > 0.0f) {
            if (self->pos.z > 1450.0f) {
                self->pos.z = 1450.0f;
                self->radius = 1450.0f;
                return 1;
            }
            if (self->pos.z < 750.0f) {
                self->pos.z = 750.0f;
                self->radius = 750.0f;
                return 1;
            }
        } else {
            if (self->pos.z < -1450.0f) {
                self->pos.z = -1450.0f;
                self->radius = 1450.0f;
                return 1;
            }
            if (self->pos.z > -750.0f) {
                self->pos.z = -750.0f;
                self->radius = 750.0f;
                return 1;
            }
        }
        z = self->pos.z;
        if (z < 0.0f) {
            a = -z;
        } else {
            a = z;
        }
        self->radius = a;
        return 0;
    }
    cx = 300.0;
    if (self->pos.x < 0.0f) {
        cx = -300.0;
    }
    ex = (f64)self->pos.x - cx;
    z = self->pos.z;
    d = pl_sqrt(ex * ex + (f64)z * (f64)z);
    self->radius = d;
    if (self->radius < 750.0f) {
        self->pos.x = cx + 750.0 * (ex / d);
        self->pos.z = 750.0 * ((f64)z / d);
        self->radius = 750.0f;
        if ((f32)self->vibTimer > 30.0f) {
            fn_1_76BC(&self->pos, 0x6C5);
        }
        return 1;
    }
    if (self->radius > 1450.0f) {
        self->pos.x = cx + 1450.0 * (ex / d);
        self->pos.z = 1450.0 * ((f64)z / d);
        self->radius = 1450.0f;
        if ((f32)self->vibTimer > 30.0f) {
            fn_1_76BC(&self->pos, 0x6C4);
        }
        return 1;
    }
    return 0;
}

// Heading (degrees) along the track at the player's position.
extern "C" f32 fn_1_C91C(Player *self)
{
    f32 cx;
    f32 ang;

    if (self->pos.x > -300.0 && self->pos.x < 300.0) {
        if (self->pos.z > 0.0f) {
            return 90.0f;
        }
        return -90.0f;
    }
    cx = 300.0f;
    if (self->pos.x < 0.0f) {
        cx = -300.0f;
    }
    ang = 90.0 + 180.0 * (atan2(self->pos.x - cx, self->pos.z) / 3.141592653589793);
    if (ang > 180.0f) {
        ang -= 360.0f;
    }
    return ang;
}

#pragma dont_inline reset

extern "C" s16 *fn_1_CA88(PlVec *v, u32 idx)
{
    if (idx >= v->size) {
        fprintf(__files + 0xA0, lbl_1_data_3E4);
        abort();
    }
    {
        s16 **dp = &v->data;
        return *dp + idx;
    }
}

extern "C" void fn_1_CB00(PlNode *head, void (*fn)(Player *))
{
    PlNode *h2 = head;
    PlNode *first = head->next;
    PlNode *first2 = first;
    fn_1_CB60(first2, h2, fn);
}

extern "C" void fn_1_CB60(PlNode *first, PlNode *end, void (*fn)(Player *))
{
    PlNode *p = first;
    while (p != end) {
        fn((Player *)p);
        p = p->next;
    }
}

#pragma dont_inline on
// vector<s16>::insert(pos, n, value)
extern "C" void fn_1_CBC0(PlVec *v, s16 *pos, u32 n, s16 *val)
{
    s16 *srcP = val;
    if (n != 0) {
        PlVec *t54 = v;
        volatile u32 max0 = 0x7FFFFFFF;
        volatile u32 max1 = max0;
        volatile u32 max = max1;
        PlVec *t44;
        u32 capNow;
        volatile u32 capCopy;
        if (n > max || v->size > max - n) {
            fprintf(__files + 0xA0, lbl_1_data_402);
            abort();
        }
        t44 = v;
        capNow = t44->cap;
        capCopy = capNow;
        if (v->size + n <= capCopy) {
            s16 **itp = &v->data;
            s16 *end = *itp + v->size;
            volatile s16 *endCopy = end;
            s32 after = end - pos;
            s16 *src = srcP;
            u32 k;
            s16 *p;
            if (end - (after - n) <= src && src < end) {
                src += n;
            }
            if (after != 0) {
                s32 cnt = end - pos;
                volatile s32 cnt2 = cnt;
                memmove(end + n - cnt, pos, cnt * 2);
            }
            k = n;
            p = pos;
            while (k != 0) {
                *p = *src;
                p++;
                k--;
            }
            v->size = v->size + n;
            (void)endCopy;
        } else {
            s16 **itp = &v->data;
            s16 *old = *itp;
            volatile s16 *old2 = old;
            s32 idx = pos - old;
            s16 **itp2 = &v->data;
            u32 newSize = v->size + n;
            PlVec *t28 = v;
            u32 cap;
            s16 *mem;
            u32 k;
            s16 *p;
            if (t28->cap != 0) {
                PlVec *t24 = v;
                volatile u32 c20 = t24->cap;
                cap = c20;
            } else {
                volatile u32 c20 = 1;
                cap = c20;
            }
            while (newSize > cap) {
                if (cap < (max >> 1)) {
                    cap *= 2;
                } else {
                    cap = max;
                }
            }
            {
                s16 **itp3 = &v->data;
                PlVec *t1c = v;
                PlVec *t78 = t1c;
                mem = (s16 *)__nw__FUl(cap * 2);
                (void)itp3;
                (void)t78;
            }
            if (mem == 0) {
                fprintf(__files + 0xA0, lbl_1_data_41F);
                abort();
            }
            {
                volatile s16 *m74 = mem;
                *itp2 = mem;
                (void)m74;
            }
            k = n;
            {
                s16 **itp4 = &v->data;
                p = *itp4 + idx;
            }
            while (k != 0) {
                *p = *srcP;
                p++;
                k--;
            }
            if (old != 0) {
                s16 **itp5 = &v->data;
                s16 *nw = *itp5;
                volatile s16 *nw2 = nw;
                s32 cnt = (old + idx) - old;
                volatile s32 cnt2 = cnt;
                s16 **itp6;
                s16 *dst;
                memcpy(nw, old, cnt * 2);
                itp6 = &v->data;
                dst = *itp6 + (idx + n);
                {
                    s16 *e = old + v->size;
                    s32 cnt3 = e - (old + idx);
                    volatile s32 cnt4 = cnt3;
                    memcpy(dst, old + idx, cnt3 * 2);
                    (void)cnt4;
                }
                __dl__FPv(old);
                (void)nw2;
                (void)cnt2;
            }
            v->size = newSize;
            {
                PlVec *t0c = v;
                t0c->cap = cap;
            }
            (void)old2;
        }
        (void)t54;
    }
}

// vector<s16>::reserve(cap)
extern "C" void fn_1_CF70(PlVec *v, u32 cap)
{
    u32 *cp = &v->cap;
    if (cap > *cp) {
        PlVec *self2 = v;
        volatile u32 max0 = 0x7FFFFFFF;
        volatile u32 max = max0;
        s16 **ip = &v->data;
        s16 *old;
        s16 **ip2;
        volatile PlVec *sv;
        volatile PlVec *sv2;
        s16 *mem;
        if (cap > max) {
            fprintf(__files + 0xA0, lbl_1_data_439);
            abort();
        }
        old = *ip;
        ip2 = &v->data;
        sv = v;
        sv2 = sv;
        mem = (s16 *)__nw__FUl(cap * 2);
        if (mem == 0) {
            fprintf(__files + 0xA0, lbl_1_data_41F);
            abort();
        }
        *ip2 = mem;
        if (old != 0) {
            s16 **ip3 = &v->data;
            s16 *nw = *ip3;
            s16 *e = old + v->size;
            s32 cnt = e - old;
            memcpy(nw, old, cnt * 2);
            __dl__FPv(old);
        }
        {
            PlVec *s3 = v;
            s3->cap = cap;
        }
        (void)self2;
        (void)sv2;
    }
}

#pragma dont_inline reset

extern "C" void fn_1_D098(void)
{
    lbl_1_bss_A40 = 120;
    lbl_1_bss_A44 = 210;
    lbl_1_bss_A48 = 210;
    fn_1_1264(&lbl_1_bss_A5C);
    __register_global_object(&lbl_1_bss_A5C, (void *)fn_1_D120, lbl_1_bss_A50);
}

#ifdef __MWERKS__
__declspec(section ".ctors") void (*const player_ctor)(void) = fn_1_D098;
#else
static struct PlayerSinit { PlayerSinit() { fn_1_D098(); } } player_sinit;
#endif

extern "C" void *fn_1_D120(void *self, s16 flag)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}
