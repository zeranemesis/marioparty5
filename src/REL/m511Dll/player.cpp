#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dolphin.h"

#include "datadir_enum.h"
extern "C" {
#include "game/audio.h"
#include "game/charman.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/object.h"
#include "game/pad.h"
}

#include "REL/m511Dll/m511objs.h"

#ifdef TARGET_PC
#define M511_STDERR stderr
#else
/* MSL's FILE table (0x50 bytes per entry): entry 2 is stderr. */
extern "C" u8 __files[];
extern "C" int fprintf(void *fp, const char *fmt, ...);
extern "C" void abort(void);
extern "C" void *memmove(void *dst, const void *src, unsigned long n);
#define M511_STDERR ((void *)(__files + 0xA0))
#endif

/* Pointer-to-member-function image used by the CodeWarrior runtime
   (__ptmf_scall): this adjust, vtable index (-1 = plain function), address. */
struct Mgr511Ptmf {
    s32 type;
    s32 param;
    void (*func)(void);
};

struct Mgr511Player;

/* Model wrapper created by m511.cpp (holds a Hu3D model id). */
struct Mgr511ModelRef {
    s16 id;
};

/* Owning pointer to a splash ring. */
struct Mgr511RingPtr {
    Mgr511SplashRing *p;
};

/* std::vector<s16> as laid out by the MSL used by the original. */
struct Mgr511S16Vec {
    u32 cap;
    u32 size;
    s16 *data;
};

/* Intrusive list head shared by all players. */
struct Mgr511PlayerList {
    Mgr511Player *next;
    Mgr511Player *prev;
};

/* One boat. */
struct Mgr511Player {
    Mgr511Player *next;     // 0x00
    Mgr511Player *prev;     // 0x04
    u8 pad08[0x48];         // 0x08
    Mgr511Ptmf state;       // 0x50
    s32 idx;                // 0x5C
    s32 charNo;             // 0x60
    s32 padNo;              // 0x64
    s32 motion;             // 0x68
    s32 dif;                // 0x6C
    f32 angle;              // 0x70
    f32 turn;               // 0x74
    s32 hitTimer;           // 0x78
    f32 boost;              // 0x7C
    s32 unk80;              // 0x80
    u8 hit;                 // 0x84
    u8 accel;               // 0x85
    u8 boostBtn;            // 0x86
    u8 pad87;               // 0x87
    Vec pos;                // 0x88
    Vec prevPos;            // 0x94
    Vec vel;                // 0xA0
    f32 speed;              // 0xAC
    f32 speedKmh;           // 0xB0
    s32 score;              // 0xB4
    s32 rank;               // 0xB8
    f32 steer;              // 0xBC
    f32 unkC0;              // 0xC0
    Mgr511ModelRef modelA;  // 0xC4
    Mgr511ModelRef modelB;  // 0xC6
    u8 flagC8;              // 0xC8
    u8 finished;            // 0xC9
    f32 finishZ;            // 0xCC
    u8 offCourse;           // 0xD0
    s32 lap;                // 0xD4
    s32 hitCool;            // 0xD8
    s32 vibrate;            // 0xDC
    s32 sndA;               // 0xE0
    s32 sndB;               // 0xE4
    Mgr511Splash *splash;   // 0xE8
    Mgr511Shadow *shadow;   // 0xEC
    Mgr511S16Vec motions;   // 0xF0
    Mgr511RingPtr rings[4]; // 0xFC
    s32 ringTick;           // 0x10C
    s32 ringIdx;            // 0x110
};

/* Part of the game master used here: per-lap counters and timers. */
struct Mgr511GmWork {
    u8 pad00[0x10];
    s32 count[10];
    s32 timer[10];
};

extern "C" {

extern s32 __ptmf_test(void *ptmf);
extern void __ptmf_scall(void *self, register const Mgr511Ptmf *ptmf);

void *__nw__FUl(u32 size);
void *__dl__FPv(void *self);
void __register_global_object(void *obj, void *(*dtor)(void *, s16), void *reg);
void fn_1_CF8(char *cond, char *file, s32 line);
void fn_1_1D88(void *self);
void fn_1_1D94(void *self, s32 arg1);
void fn_1_1E00(void *self);
void fn_1_1E54(void *self, void *list);
s32 fn_1_29C8(void);
void fn_1_372C(Mgr511ModelRef *self, s32 dataNum);
void fn_1_37B0(Mgr511ModelRef *self, s16 charNo, s32 arg2);
void fn_1_398C(Mgr511ModelRef *self, Vec *pos);
void fn_1_39D4(Mgr511ModelRef *self, Vec *rot);
void fn_1_3B8C(Mgr511ModelRef *self, s32 arg1);
void fn_1_504(s32 arg0, Vec *arg1, f32 arg2, f32 arg3);
u16 fn_1_5578(void);
f32 fn_1_884(f64 arg0, f64 arg1);
void *fn_1_84CC(void *self, void *player);
void fn_1_B2D4(void *camera, s32 arg1);
s32 fn_1_B6C0(Vec *pos, s32 seId, s32 camIdx);
s32 fn_1_B864(Vec *pos, s32 seId, s32 camIdx);
void fn_1_BA18(Vec *pos, s32 seId, s32 camIdx, f32 pitchT);
Mgr511Shadow *fn_1_C07C(Mgr511Shadow *self);
void *fn_1_C124(void *self, s32 arg1);
void fn_1_C180(Mgr511Shadow *self, Vec *pos, Vec *rot);
void fn_1_DAB8(s32 idx, Vec *out);
void *fn_1_EB7C(void *self, s32 idx, s32 charNo, s32 arg3);
void fn_1_EED8(s32 idx, Vec *pos, s32 lap, f32 angle);
void fn_1_F0D8(s32 idx);
void fn_1_1652C(void *root, s32 idx, s32 charNo);
void fn_1_16604(void *root, s32 idx, s32 lap, s32 score);
Mgr511Splash *fn_1_16E84(Mgr511Splash *self);
void *fn_1_170C8(void *self, s32 arg1);
void fn_1_17544(Mgr511Splash *self, Vec *pos, Vec *rot, f32 speed);
Mgr511SplashRing *fn_1_17BD8(Mgr511SplashRing *self);
void *fn_1_17CC8(void *self, s32 arg1);
void fn_1_17D1C(Mgr511SplashRing *self, Vec *pos, Vec *rot, f32 scale);
void fn_1_17E6C(Mgr511SplashRing *self);
s32 fn_1_19190(Vec *pos, s32 idx);
f32 fn_1_1A150(void *water, Vec *pos);
void fn_1_1A394(Vec *out, void *water, Vec *pos);
s32 fn_1_2FF4(s32 idx);

extern Mgr511PlayerList lbl_1_bss_AC4;
extern void *lbl_1_bss_A24;
extern Mgr511GmWork *lbl_1_bss_A2C;
extern void *lbl_1_bss_A3C;
extern void *lbl_1_bss_1C;

/* Mersenne twister output shared with the other units. */
void fn_1_FACC(Vec *out, s32 idx);
void fn_1_FB50(void);
void fn_1_FC40(void);
Mgr511Player *fn_1_FCE4(Mgr511Player *p, s32 idx);
Mgr511RingPtr *fn_1_10360(Mgr511RingPtr *p, s16 flag);
void fn_1_103B8(Mgr511RingPtr *p);
void *fn_1_10690(Mgr511Player *p, s16 flag);
s16 fn_1_107AC(Mgr511Player *p, u32 dataNum);
void fn_1_108EC(Mgr511Player *p);
void fn_1_10908(void);
void fn_1_11804(Mgr511Player *p);
void fn_1_11ADC(Mgr511Player *p);
void fn_1_11BFC(Mgr511Player *p);
void fn_1_11EA4(Mgr511Player *p);
void fn_1_11ECC(Mgr511Player *p);
s32 fn_1_12570(void *self, Vec *a, Vec *b, Vec *o1, Vec *o2);
void fn_1_1295C(Mgr511Player *p);
void fn_1_133E8(Mgr511Player *p, s32 motion, u8 flag);
void fn_1_134DC(Mgr511Player *p);
void fn_1_13554(Mgr511Player *p);
void fn_1_13630(Mgr511Player *p);
void fn_1_13704(Mgr511Player *p);
void fn_1_13794(Mgr511Player *p);
f32 fn_1_1402C(Mgr511Player *p);
f32 fn_1_14548(Mgr511Player *p);
void fn_1_14A40(Mgr511Player *p);
s32 fn_1_14C1C(Mgr511Player *p);
s16 *fn_1_152BC(Mgr511S16Vec *v, u32 index);
void fn_1_15334(s32 *p, s32 value);
void fn_1_1533C(Mgr511PlayerList *list, void (*fn)(Mgr511Player *));
void fn_1_1539C(Mgr511Player *first, Mgr511PlayerList *end, void (*fn)(Mgr511Player *));
void fn_1_153FC(Mgr511S16Vec *v, s16 *where, u32 count, s16 *value);
void fn_1_157AC(Mgr511S16Vec *v, u32 cap);
void *fn_1_1595C(void *p, s16 flag);

/* Local statics of the physics helpers (initialised on first use). */
static f64 lbl_1_bss_AD0;
static u8 lbl_1_bss_AD8;
static f64 lbl_1_bss_AE0;
static u8 lbl_1_bss_AE8;
static f64 lbl_1_bss_AF0;
static u8 lbl_1_bss_AF8;

s32 lbl_1_bss_AA8;
s32 lbl_1_bss_AAC;
s32 lbl_1_bss_AB0;
Mgr511Player *lbl_1_bss_AB4;
u8 lbl_1_bss_AB8[12];
Mgr511PlayerList lbl_1_bss_AC4;

s32 lbl_1_data_4F8[4] = { 0, 1, 2, 3 };
s32 lbl_1_data_508[4] = { 0, 1, 2, 3 };
static Mgr511Ptmf lbl_1_data_518 = { 0, -1, (void (*)(void))fn_1_134DC };
static Mgr511Ptmf lbl_1_data_570 = { 0, -1, (void (*)(void))fn_1_13554 };
static Mgr511Ptmf lbl_1_data_57C = { 0, -1, (void (*)(void))fn_1_13630 };

const s32 lbl_1_rodata_520[13] = {
    DATANUM(DATA_m511, 0x00), DATANUM(DATA_m511, 0x01), DATANUM(DATA_m511, 0x02), DATANUM(DATA_m511, 0x03),
    DATANUM(DATA_m511, 0x04), DATANUM(DATA_m511, 0x05), DATANUM(DATA_m511, 0x06), DATANUM(DATA_m511, 0x07),
    DATANUM(DATA_m511, 0x08), DATANUM(DATA_m511, 0x09), DATANUM(DATA_m511, 0x0A), DATANUM(DATA_m511, 0x0B),
    DATANUM(DATA_m511, 0x0C),
};
const s32 lbl_1_rodata_554[8] = { 0x6A2, 0x6A3, 0x6A4, 0x6A5, 0x6A6, 0x6A7, 0x6A8, 0x6A9 };
const s32 lbl_1_rodata_57C[5] = { 0, 1, 2, 3, 0 };
/* Per-character offset of the second model (rider) relative to the boat. */
const Vec lbl_1_rodata_620[13] = {
    { 0.0f, -5.8f, 23.6f },
    { 0.0f, -5.0f, 27.0f },
    { 0.0f, -15.0f, 20.0f },
    { 0.0f, -23.0f, 25.0f },
    { 0.0f, -23.0f, -5.0f },
    { 0.0f, -15.0f, 20.0f },
    { 0.0f, -3.7f, 8.0f },
    { 0.0f, 2.0f, 15.0f },
    { 0.0f, -17.5f, 7.0f },
    { 0.0f, -9.0f, -7.0f },
    { 0.0f, -9.0f, -7.0f },
    { 0.0f, -9.0f, -7.0f },
    { 0.0f, -9.0f, -7.0f },
};
const s32 lbl_1_rodata_6D8[5] = { 0x6AF, 0x6B0, 0x6B1, 0x6B2, 0x6B3 };
/* Collision probe points along the boat. */
const Vec lbl_1_rodata_710[3] = {
    { 0.0f, 0.0f, 100.0f },
    { 0.0f, 0.0f, 30.0f },
    { 0.0f, 0.0f, -40.0f },
};
const s32 lbl_1_rodata_734[5] = { 0x6AA, 0x6AB, 0x6AC, 0x6AD, 0 };
/* Computer boat speed factor by difficulty. */
const f32 lbl_1_rodata_778[4] = { 0.58f, 0.7f, 0.85f, 0.95f };
const s32 lbl_1_rodata_788[4] = { 0x6AA, 0x6AB, 0x6AC, 0x6AD };

/* Inlined members of the vector<s16> used by the original. */
static inline s16 **VecDataRef(Mgr511S16Vec *v)
{
    return &v->data;
}

static inline s16 *VecEnd(Mgr511S16Vec *v)
{
    s16 **d = VecDataRef(v);

    return *d + v->size;
}

static inline u32 VecMaxSize(Mgr511S16Vec *v)
{
    return 0x7FFFFFFF;
}

static inline u32 VecCapacity(Mgr511S16Vec *v)
{
    return v->cap;
}

/* allocator::allocate: the vector is carried along as the allocator owner. */
static inline s16 *VecAlloc(Mgr511S16Vec *v, u32 n)
{
    return (s16 *)__nw__FUl(n * sizeof(s16));
}

static inline void VecFree(Mgr511S16Vec *v)
{
    s16 **d = VecDataRef(v);

    if (*d != 0) {
        s16 **d2 = VecDataRef(v);
        s16 *q = *d2;

        __dl__FPv(q);
    }
}

void fn_1_FACC(Vec *out, s32 idx)
{
    out->x = (f32)(f64)((lbl_1_data_4F8[idx] * 0x12C) - 0x1C2);
    out->y = 0.0f;
    out->z = 7500.0f;
}

void fn_1_FB50(void)
{
    s32 i;
    s32 j;
    s32 a;
    s32 b;

    for (i = 0; i < 4; i++) {
        j = fn_1_5578() & 3;
        a = lbl_1_data_4F8[i];
        b = lbl_1_data_4F8[j];
        lbl_1_data_4F8[i] = b;
        lbl_1_data_4F8[j] = a;
        j = fn_1_5578() & 3;
        a = lbl_1_data_508[i];
        b = lbl_1_data_508[j];
        lbl_1_data_508[i] = b;
        lbl_1_data_508[j] = a;
    }
}

void fn_1_FC40(void)
{
    u8 empty;

    do {
        empty = 0;
        if (lbl_1_bss_AC4.next == (Mgr511Player *)&lbl_1_bss_AC4 && lbl_1_bss_AC4.next == lbl_1_bss_AC4.prev) {
            empty = 1;
        }
        if (empty == 0) {
            fn_1_10690(lbl_1_bss_AC4.prev, 1);
        }
    } while (empty == 0);
}

Mgr511Player *fn_1_FCE4(Mgr511Player *p, s32 idx)
{
    Mgr511Splash *splash;
    Mgr511Shadow *shadow;
    Mgr511SplashRing *ring;
    void *obj;
    s32 i;
    s16 charNo;
    s16 padNo;
    Mgr511ModelRef *modelA;
    Mgr511ModelRef *modelB;

    fn_1_1D88(p);
    p->hitTimer = 0;
    p->boost = 0.0f;
    p->unk80 = 0;
    p->hit = 0;
    p->speed = 0.0f;
    p->speedKmh = 0.0f;
    p->score = 1;
    p->rank = 0;
    p->steer = 0.0f;
    charNo = GwPlayerConf[idx].charNo;
    fn_1_372C(&p->modelA, lbl_1_rodata_520[charNo]);
    fn_1_37B0(&p->modelB, GwPlayerConf[idx].charNo, 1);
    p->flagC8 = 0;
    p->finished = 0;
    p->offCourse = 0;
    p->lap = 0;
    p->hitCool = 0x3C;
    p->vibrate = 0;
    p->splash = 0;
    p->shadow = 0;
    p->motions.cap = 0;
    p->motions.size = 0;
    p->motions.data = 0;
    for (i = 0; i < 4; i++) {
        fn_1_103B8(&p->rings[i]);
    }
    fn_1_1E54(p, &lbl_1_bss_AC4);
    p->idx = idx;
    p->charNo = GwPlayerConf[idx].charNo;
    p->dif = GwPlayerConf[idx].dif;
    if (GwPlayerConf[idx].type != 0) {
        padNo = -1;
    } else {
        padNo = GwPlayerConf[idx].padNo;
    }
    p->padNo = padNo;
    p->motion = -1;
    p->motions.size = 0;
    fn_1_157AC(&p->motions, 1);
    fn_1_107AC(p, 0x8700BE);
    if (p->motions.size == 1) {
    } else {
        fn_1_CF8("m_mot_id.size() == NUMBEROF_MOTION", "player.cpp", 0xA1);
    }
    CharMotionDataClose((s16)p->charNo);
    Hu3DMotionAttrSet(*fn_1_152BC(&p->motions, 0), 1);
    modelA = &p->modelA;
    Hu3DModelLayerSet(modelA->id, 4);
    Hu3DModelCameraSet(modelA->id, 0xF);
    Hu3DModelAttrSet(modelA->id, 0x40000002);
    Hu3DModelAttrSet(modelA->id, 0x40000080);
    Hu3DModelAttrSet(modelA->id, 0x40000010);
    Hu3DModelAttrSet(modelA->id, 0x4000);
    modelB = &p->modelB;
    Hu3DModelLayerSet(modelB->id, 4);
    Hu3DModelCameraSet(modelB->id, 0xF);
    CharModelAttrSet((s16)p->charNo, 0x40000002);
    CharModelAttrSet((s16)p->charNo, 0x4000);
    fn_1_FACC(&p->pos, idx);
    p->prevPos.x = p->pos.x;
    p->prevPos.y = p->pos.y;
    p->prevPos.z = p->pos.z;
    p->angle = 180.0f;
    p->turn = 0.0f;
    p->vel.z = 0.0f;
    p->vel.y = 0.0f;
    p->vel.x = 0.0f;
    p->unkC0 = 750.0f;
    fn_1_398C(&p->modelA, &p->pos);
    p->state = lbl_1_data_518;
    if (lbl_1_bss_A24 != 0) {
    } else {
        fn_1_CF8("ms_pointer != 0", "singleton.h", 0x27);
    }
    fn_1_1652C(lbl_1_bss_A24, p->idx, p->charNo);
    if (lbl_1_bss_A24 != 0) {
    } else {
        fn_1_CF8("ms_pointer != 0", "singleton.h", 0x27);
    }
    fn_1_16604(lbl_1_bss_A24, p->idx, p->lap, p->score);
    obj = __nw__FUl(0x3C);
    if (obj != 0) {
        fn_1_EB7C(obj, idx, p->charNo, 0);
    }
    splash = (Mgr511Splash *)__nw__FUl(sizeof(Mgr511Splash));
    if (splash != 0) {
        splash = fn_1_16E84(splash);
    }
    if (p->splash != splash) {
        fn_1_170C8(p->splash, 1);
        p->splash = splash;
    }
    shadow = (Mgr511Shadow *)__nw__FUl(sizeof(Mgr511Shadow));
    if (shadow != 0) {
        shadow = fn_1_C07C(shadow);
    }
    if (p->shadow != shadow) {
        fn_1_C124(p->shadow, 1);
        p->shadow = shadow;
    }
    for (i = 0; i < 4; i++) {
        ring = (Mgr511SplashRing *)__nw__FUl(sizeof(Mgr511SplashRing));
        if (ring != 0) {
            ring = fn_1_17BD8(ring);
        }
        if (p->rings[i].p != ring) {
            fn_1_17CC8(p->rings[i].p, 1);
            p->rings[i].p = ring;
        }
        p->ringTick = 0;
        p->ringIdx = 0;
    }
    p->sndA = fn_1_B6C0(&p->pos, lbl_1_rodata_554[p->idx], p->idx);
    p->sndB = fn_1_B6C0(&p->pos, lbl_1_rodata_554[p->idx + 4], p->idx);
    return p;
}

Mgr511RingPtr *fn_1_10360(Mgr511RingPtr *p, s16 flag)
{
    if (p != 0) {
        fn_1_17CC8(p->p, 1);
        if (flag > 0) {
            __dl__FPv(p);
        }
    }
    return p;
}

void fn_1_103B8(Mgr511RingPtr *p)
{
    fn_1_15334((s32 *)&p->p, 0);
}

void *fn_1_103E4(Mgr511S16Vec *v, s16 flag)
{
    if (v != 0) {
        if (v != 0 && v != 0) {
            VecFree(v);
        }
        if (flag > 0) {
            __dl__FPv(v);
        }
    }
    return v;
}

void *fn_1_10478(Mgr511S16Vec *v, s16 flag)
{
    if (v != 0) {
        if (v != 0) {
            VecFree(v);
        }
        if (flag > 0) {
            __dl__FPv(v);
        }
    }
    return v;
}

void *fn_1_10504(Mgr511S16Vec *v, s16 flag)
{
    if (v != 0) {
        VecFree(v);
        if (flag > 0) {
            __dl__FPv(v);
        }
    }
    return v;
}

void *fn_1_10588(Mgr511Shadow **p, s16 flag)
{
    if (p != 0) {
        fn_1_C124(*p, 1);
        if (flag > 0) {
            __dl__FPv(p);
        }
    }
    return p;
}

void *fn_1_105E0(Mgr511Splash **p, s16 flag)
{
    if (p != 0) {
        fn_1_170C8(*p, 1);
        if (flag > 0) {
            __dl__FPv(p);
        }
    }
    return p;
}

void *fn_1_10638(void *node, s16 flag)
{
    if (node != 0) {
        fn_1_1D94(node, 0);
        if (flag > 0) {
            __dl__FPv(node);
        }
    }
    return node;
}

void *fn_1_10690(Mgr511Player *p, s16 flag)
{
    s32 i;

    if (p != 0) {
        for (i = 3; i >= 0; i--) {
            fn_1_10360(&p->rings[i], -1);
        }
        if ((void *)&p->motions != 0 && (void *)&p->motions != 0) {
            VecFree(&p->motions);
        }
        if ((void *)&p->shadow != 0) {
            fn_1_C124(p->shadow, 1);
        }
        if ((void *)&p->splash != 0) {
            fn_1_170C8(p->splash, 1);
        }
        fn_1_3B8C(&p->modelB, -1);
        fn_1_3B8C(&p->modelA, -1);
        fn_1_1D94(p, 0);
        if (flag > 0) {
            __dl__FPv(p);
        }
    }
    return p;
}

s16 fn_1_107AC(Mgr511Player *p, u32 dataNum)
{
    s16 motion;

    motion = CharMotionCreate((s16)p->charNo, dataNum);
    fn_1_153FC(&p->motions, VecEnd(&p->motions), 1, &motion);
    return motion;
}

void fn_1_10828(void)
{
    Mgr511Player *first;

    fn_1_1533C(&lbl_1_bss_AC4, fn_1_108EC);
    fn_1_1533C(&lbl_1_bss_AC4, fn_1_11804);
    fn_1_1533C(&lbl_1_bss_AC4, fn_1_11ADC);
    fn_1_1533C(&lbl_1_bss_AC4, fn_1_11EA4);
    fn_1_10908();
    first = lbl_1_bss_AC4.next;
    if (first != (Mgr511Player *)&lbl_1_bss_AC4) {
        fn_1_1E00(first);
        fn_1_1E54(first, &lbl_1_bss_AC4);
    }
}

void fn_1_108EC(Mgr511Player *p)
{
    p->prevPos.x = p->pos.x;
    p->prevPos.y = p->pos.y;
    p->prevPos.z = p->pos.z;
}

void fn_1_10908(void)
{
    Mgr511Player *p;
    Mgr511Player *list[4];
    s32 order[4];
    f64 key[4];
    Vec wp;
    f32 dx;
    f32 dz;
    f32 t1;
    f32 t2;
    f64 d;
    s32 n;
    s32 i;
    s32 j;
    s32 rank;

    p = lbl_1_bss_AC4.next;
    order[0] = lbl_1_rodata_57C[0];
    order[1] = lbl_1_rodata_57C[1];
    order[2] = lbl_1_rodata_57C[2];
    order[3] = lbl_1_rodata_57C[3];
    n = 0;
    while (p != (Mgr511Player *)&lbl_1_bss_AC4) {
        list[n] = p;
        fn_1_DAB8(p->lap, &wp);
        dx = wp.x - p->pos.x;
        dz = wp.z - p->pos.z;
        d = sqrt(((f64)dx * (f64)dx) + ((f64)dz * (f64)dz));
        key[n] = (40000.0 - d) + (f64)(p->lap * 40000);
        p->rank = 0;
        n++;
        p = p->next;
    }
    if (fn_1_29C8() == 2) {
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 3; j++) {
                if (key[j] < key[j + 1]) {
                    t1 = (f32)key[j];
                    t2 = (f32)key[j + 1];
                    key[j] = t2;
                    key[j + 1] = t1;
                    t1 = (f32)order[j];
                    t2 = (f32)order[j + 1];
                    order[j] = (s32)t2;
                    order[j + 1] = (s32)t1;
                }
            }
        }
        rank = 0;
        for (i = 0; i < 4; i++) {
            list[order[i]]->rank = rank;
            if (i < 3 && (f32)key[rank] != (f32)key[rank + 1]) {
                rank++;
            }
        }
    }
}

s32 fn_1_10DB4(u8 arg0)
{
    Mgr511Player *p;
    f32 best;
    s32 n;

    p = lbl_1_bss_AC4.next;
    n = 0;
    lbl_1_bss_AB4 = 0;
    if (arg0 != 0) {
        while (p != (Mgr511Player *)&lbl_1_bss_AC4) {
            if (p->finished != 0) {
                n++;
            }
            p = p->next;
        }
        if (n >= 4) {
            return 1;
        }
        return 0;
    }
    best = 1.0f;
    while (p != (Mgr511Player *)&lbl_1_bss_AC4) {
        if (p->finished != 0 && p->finishZ < best) {
            if (lbl_1_bss_AB4 != 0) {
                lbl_1_bss_AB4->finished = 0;
            }
            best = p->finishZ;
            lbl_1_bss_AB4 = p;
            n++;
        }
        p = p->next;
    }
    if (n == 0) {
        return 0;
    }
    lbl_1_bss_AB4->state = lbl_1_data_570;
    fn_1_2FF4(lbl_1_bss_AB4->idx);
    if (lbl_1_bss_A3C != 0) {
    } else {
        fn_1_CF8("ms_pointer != 0", "singleton.h", 0x27);
    }
    fn_1_B2D4(lbl_1_bss_A3C, lbl_1_bss_AB4->idx);
    return 1;
}

void fn_1_10FA4(void)
{
    if (lbl_1_bss_AB4 != 0) {
        CharFXPlay((s16)lbl_1_bss_AB4->charNo, 0x1FA);
    }
}

s32 fn_1_10FF0(u8 arg0)
{
    Mgr511Player *p;
    s32 n;

    p = lbl_1_bss_AC4.next;
    n = 0;
    if (arg0 != 0) {
        while (p != (Mgr511Player *)&lbl_1_bss_AC4) {
            if (p->finished != 0) {
                GwPlayer[p->idx].mgScore = p->score;
                n++;
            } else {
                GwPlayer[p->idx].mgScore = 0;
            }
            p = p->next;
        }
    } else {
        while (p != (Mgr511Player *)&lbl_1_bss_AC4) {
            if (p->finished != 0) {
                n++;
            }
            p = p->next;
        }
    }
    if (n > 0) {
        return 0;
    }
    return 1;
}

void fn_1_110E0(void)
{
    Mgr511Player *p;
    s16 chars[4];
    s32 n;

#ifdef TARGET_PC
    chars[0] = chars[1] = chars[2] = chars[3] = 0;
#endif
    p = lbl_1_bss_AC4.next;
    n = 0;
    while (p != (Mgr511Player *)&lbl_1_bss_AC4) {
        chars[n++] = (s16)p->charNo;
        p = p->next;
    }
    CharLoseVoicePlay(chars[0], chars[1], chars[2], chars[3]);
}

void fn_1_11170(void)
{
    if (lbl_1_bss_AB4 != 0) {
        CharFXPlay((s16)lbl_1_bss_AB4->charNo, 0x1FA);
    }
}

s32 fn_1_111BC(void)
{
    Mgr511Player *p;
    Mgr511Player *first;
    Mgr511Player *it;
    Mgr511Player *end;
    Mgr511Player *nx;
    s32 best;

    best = 0x4650;
    first = lbl_1_bss_AC4.next;
    it = first;
    p = it;
    end = (Mgr511Player *)&lbl_1_bss_AC4;
    while (p != end) {
        if (p->finished != 0 && p->padNo >= 0 && p->score < best) {
            best = p->score;
        }
        nx = p->next;
        p = nx;
    }
    return best;
}

void fn_1_11248(void)
{
    void *cam;
    void *cam2;

    if (lbl_1_bss_AB4 != 0) {
        if (lbl_1_bss_A3C != 0) {
        } else {
            fn_1_CF8("ms_pointer != 0", "singleton.h", 0x27);
        }
        cam = lbl_1_bss_A3C;
        cam2 = cam;
        fn_1_B2D4(cam2, lbl_1_bss_AB4->idx);
    }
}

void fn_1_112E0(s32 idx, Vec *pos, Vec *target)
{
    Mgr511Player *p;
    f32 a;

    p = lbl_1_bss_AC4.next;
    while (p != (Mgr511Player *)&lbl_1_bss_AC4) {
        if (p->idx == idx) {
            if (p->finished != 0) {
                a = 90.0f + -p->angle;
                pos->x = p->pos.x;
                pos->y = (f32)(100.0 + (f64)p->pos.y);
                pos->z = p->pos.z;
                target->x = (f32)((f64)p->pos.x + (100.0 * (8.0 * cos((3.141592653589793 * (f64)a) / 180.0))));
                target->y = 300.0f;
                target->z = (f32)((f64)p->pos.z + (100.0 * (8.0 * sin((3.141592653589793 * (f64)a) / 180.0))));
            } else {
                a = -p->angle - 90.0f;
                pos->x = (f32)((f64)p->pos.x - (100.0 * (2.0 * cos((3.141592653589793 * (f64)a) / 180.0))));
                pos->y = (f32)(100.0 + (f64)p->pos.y);
                pos->z = (f32)((f64)p->pos.z - (100.0 * (2.0 * sin((3.141592653589793 * (f64)a) / 180.0))));
                target->x = (f32)((f64)p->pos.x + (100.0 * (5.0 * cos((3.141592653589793 * (f64)a) / 180.0))));
                target->y = 300.0f + p->pos.y;
                target->z = (f32)((f64)p->pos.z + (100.0 * (5.0 * sin((3.141592653589793 * (f64)a) / 180.0))));
                if (PSVECMag(target) < 700.0f && target->y < 160.0f) {
                    target->y = 160.0f;
                }
            }
            return;
        }
        p = p->next;
    }
}

void fn_1_1167C(s32 idx, Vec *pos, Vec *target)
{
    Mgr511Player *p;
    f32 a;

    p = lbl_1_bss_AC4.next;
    while (p != (Mgr511Player *)&lbl_1_bss_AC4) {
        if (p->idx == idx) {
            a = 90.0f + -p->angle;
            pos->x = p->pos.x;
            pos->y = (f32)(100.0 + (f64)p->pos.y);
            pos->z = p->pos.z;
            target->x = (f32)((f64)p->pos.x + (100.0 * (8.0 * cos((3.141592653589793 * (f64)a) / 180.0))));
            target->y = 600.0f;
            target->z = (f32)((f64)p->pos.z + (100.0 * (8.0 * sin((3.141592653589793 * (f64)a) / 180.0))));
            return;
        }
        p = p->next;
    }
}

void fn_1_11804(Mgr511Player *p)
{
    Vec pos2;
    Vec rot;
    Vec normal;
    Vec normalCopy;
    f32 speed;
    f32 h;
    f32 oldY;
    void *water;
    Mgr511Splash *splash;
    s32 i;

    __ptmf_scall(p, &p->state);
    speed = p->speed;
    if (lbl_1_bss_1C != 0) {
    } else {
        fn_1_CF8("ms_pointer != 0", "singleton.h", 0x27);
    }
    water = lbl_1_bss_1C;
    h = fn_1_1A150(water, &p->pos);
    oldY = p->pos.y;
    p->pos.y = (f32)((f64)oldY + (0.15 * (f64)(h - p->pos.y)));
    pos2.x = p->pos.x;
    pos2.y = h;
    pos2.z = p->pos.z;
    rot.x = 0.0f;
    rot.y = p->angle;
    rot.z = 0.0f;
    splash = p->splash;
    fn_1_17544(splash, &pos2, &rot, speed);
    if (speed > 0.023148147f) {
        splash->flag = 1;
    } else {
        splash->flag = 0;
    }
    fn_1_C180(p->shadow, &pos2, &rot);
    if (1.0f + (0.001f * ((speed * 216.0f) - 5.0f)) >= 1.01f) {
        if ((p->ringTick & 7) == 0) {
            fn_1_17D1C(p->rings[p->ringIdx].p, &pos2, &rot, speed);
            p->ringIdx = p->ringIdx + 1;
            p->ringIdx = p->ringIdx & 3;
        }
        p->ringTick = p->ringTick + 1;
    }
    for (i = 0; i < 4; i++) {
        fn_1_17E6C(p->rings[i].p);
    }
    fn_1_1A394(&normal, water, &p->pos);
    normalCopy.x = normal.x;
    normalCopy.y = normal.y;
    normalCopy.z = normal.z;
}

void fn_1_11ADC(Mgr511Player *p)
{
    p->hit = 0;
    fn_1_1295C(p);
    p->hit = (u8)(p->hit | fn_1_14C1C(p));
    if (fn_1_29C8() == 2) {
        if (p->vibrate > 0) {
            p->vibrate = p->vibrate - 1;
            if (p->finished == 0) {
                omVibrate((s16)p->idx, 0xC, 4, 2);
            }
        } else if ((f32)p->hitCool < 6.0f && p->padNo >= 0 && p->finished == 0) {
            omVibrate((s16)p->idx, 0xC, 0xC, 0);
        }
        p->hitCool = p->hitCool + 1;
    }
}

void fn_1_11BFC(Mgr511Player *p)
{
    Vec pos;
    Vec rot;
    Vec off;
    Mtx mx;
    Mtx my;
    Mtx mz;
    f32 tilt;

    pos.x = p->pos.x;
    pos.y = p->pos.y;
    pos.z = p->pos.z;
    tilt = -p->speedKmh * 0.5f;
    if (tilt < -20.0f) {
        tilt = -20.0f;
    }
    rot.x = tilt;
    rot.y = p->angle;
    rot.z = 0.0f;
    pos.y = (f32)((f64)p->pos.y - (1.5 * (f64)tilt));
    if (p->flagC8 != 0) {
        Hu3DModelAttrSet(p->modelA.id, 1);
        CharModelAttrReset((s16)p->charNo, 1);
        fn_1_39D4(&p->modelB, &rot);
        PSMTXRotRad(mx, 'X', 0.017453292f * rot.x);
        PSMTXRotRad(my, 'Y', 0.017453292f * rot.y);
        PSMTXConcat(my, mx, mx);
        PSMTXRotRad(mz, 'Z', 0.017453292f * rot.z);
        PSMTXConcat(mz, mx, mx);
        PSMTXMultVec(mx, (Vec *)&lbl_1_rodata_620[p->charNo], &off);
        PSVECAdd(&pos, &off, &off);
        fn_1_398C(&p->modelB, &off);
        fn_1_133E8(p, 0, 0);
    } else {
        Hu3DModelAttrReset(p->modelA.id, 1);
        CharModelAttrSet((s16)p->charNo, 1);
        fn_1_39D4(&p->modelA, &rot);
        fn_1_398C(&p->modelA, &pos);
    }
    fn_1_504(p->idx, &pos, tilt, p->angle);
    fn_1_BA18(&p->pos, p->sndA, p->idx, p->speedKmh / 60.0f);
    fn_1_BA18(&p->pos, p->sndB, p->idx, p->speedKmh / 60.0f);
}

void fn_1_11EA4(Mgr511Player *p)
{
    fn_1_11ECC(p);
}

void fn_1_11ECC(Mgr511Player *p)
{
    Vec wp;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 ex;
    f32 ez;
    f32 t;
    f64 d;
    s32 i;
    void *obj;

    dx = p->pos.x - p->prevPos.x;
    p->vel.x = dx;
    dy = p->pos.y - p->prevPos.y;
    p->vel.y = dy;
    dz = p->pos.z - p->prevPos.z;
    p->vel.z = dz;
    d = sqrt((dx * dx) + (dz * dz));
    p->speed = (f32)(d / 100.0);
    t = p->speed * 216000.0f;
    p->speedKmh = (f32)(t / 1000.0f);
    if (fn_1_29C8() == 2) {
        if (p->lap < 5) {
            fn_1_DAB8(p->lap, &wp);
            ex = wp.x - p->pos.x;
            ez = wp.z - p->pos.z;
            d = sqrt(((f64)ex * (f64)ex) + ((f64)ez * (f64)ez));
            if ((f32)d < 150.0f) {
                HuAudFXPlay(lbl_1_rodata_6D8[p->lap]);
                p->lap = p->lap + 1;
                p->vibrate = 0x3C;
                if (lbl_1_bss_A24 != 0) {
                } else {
                    fn_1_CF8("ms_pointer != 0", "singleton.h", 0x27);
                }
                fn_1_16604(lbl_1_bss_A24, p->idx, p->lap, p->score);
                i = p->lap - 1;
                if (lbl_1_bss_A2C != 0) {
                } else {
                    fn_1_CF8("ms_pointer != 0", "singleton.h", 0x27);
                }
                lbl_1_bss_A2C->count[i] = lbl_1_bss_A2C->count[i] + 1;
                lbl_1_bss_A2C->timer[i] = 0x12;
                obj = __nw__FUl(0x44);
                if (obj != 0) {
                    fn_1_84CC(obj, p);
                }
            }
        } else if (p->finished == 0 && p->pos.z < -5000.0f) {
            p->finished = 1;
            p->finishZ = p->pos.z;
            p->lap = p->lap + 1;
            fn_1_F0D8(p->idx);
            if (lbl_1_bss_A24 != 0) {
            } else {
                fn_1_CF8("ms_pointer != 0", "singleton.h", 0x27);
            }
            fn_1_16604(lbl_1_bss_A24, p->idx, p->lap, p->score);
        }
    }
    fn_1_11BFC(p);
    if (fn_1_29C8() != 2 || p->finished == 1) {
        return;
    }
    fn_1_EED8(p->idx, &p->pos, p->lap, p->angle);
    p->score = p->score + 1;
}

s32 fn_1_12570(void *self, Vec *a, Vec *b, Vec *o1, Vec *o2)
{
    f32 dx;
    f32 dz;
    f64 d;
    f64 mx;
    f64 mz;
    f64 ox;
    f64 oz;

    if ((s8)lbl_1_bss_AD8 == 0) {
        lbl_1_bss_AD0 = 160.0;
        lbl_1_bss_AD8 = 1;
    }
    dx = b->x - a->x;
    dz = b->z - a->z;
    d = sqrt(((f64)dx * (f64)dx) + ((f64)dz * (f64)dz));
    if ((f32)d > (f32)lbl_1_bss_AD0 || 0.0f == (f32)d) {
        o1->x = 0.0f;
        o1->y = 0.0f;
        o1->z = 0.0f;
        o2->x = 0.0f;
        o2->y = 0.0f;
        o2->z = 0.0f;
        return 0;
    }
    mx = 0.5 * (f64)(b->x + a->x);
    mz = 0.5 * (f64)(b->z + a->z);
    ox = (((f64)dx * lbl_1_bss_AD0) / d) * 0.5;
    oz = (((f64)dz * lbl_1_bss_AD0) / d) * 0.5;
    o1->x = (f32)((mx - ox) - (f64)a->x);
    o1->z = (f32)((mz - oz) - (f64)a->z);
    o2->x = (f32)((mx + ox) - (f64)b->x);
    o2->z = (f32)((mz + oz) - (f64)b->z);
    return 1;
}

void fn_1_1295C(Mgr511Player *p)
{
    Vec probe[3];
    Vec pa[3];
    Vec pb[3];
    Vec o1;
    Vec o2;
    Vec pt;
    Mtx rotA;
    Mtx rotB;
    Mgr511Player *other;
    f32 dx;
    f32 dz;
    f64 d;
    s32 i;
    s32 j;
    s32 k;
    u8 hit;
    u8 snd;

    for (k = 0; k < 3; k++) {
        probe[k] = lbl_1_rodata_710[k];
    }
    other = lbl_1_bss_AC4.next;
    while (other != (Mgr511Player *)&lbl_1_bss_AC4) {
        if (p != other) {
            dx = other->pos.x - p->pos.x;
            dz = other->pos.z - p->pos.z;
            d = sqrt(((f64)dx * (f64)dx) + ((f64)dz * (f64)dz));
            if (!((f32)d > 400.0f) && 0.0f != (f32)d) {
                PSMTXRotRad(rotA, 'y', 0.017453292f * p->angle);
                PSMTXRotRad(rotB, 'y', 0.017453292f * other->angle);
                for (k = 0; k < 3; k++) {
                    PSMTXMultVec(rotA, &probe[k], &pa[k]);
                    PSMTXMultVec(rotB, &probe[k], &pb[k]);
                    pa[k].x = pa[k].x + p->pos.x;
                    pa[k].z = pa[k].z + p->pos.z;
                    pb[k].x = pb[k].x + other->pos.x;
                    pb[k].z = pb[k].z + other->pos.z;
                }
                for (i = 0; i < 3; i++) {
                    for (j = 0; j < 3; j++) {
                        hit = (u8)fn_1_12570(p, &pa[i], &pb[j], &o1, &o2);
                        p->pos.x = p->pos.x + o1.x;
                        p->pos.z = p->pos.z + o1.z;
                        other->pos.x = other->pos.x + o2.x;
                        other->pos.z = other->pos.z + o2.z;
                        for (k = 0; k < 3; k++) {
                            pa[k].x = pa[k].x + o1.x;
                            pa[k].z = pa[k].z + o1.z;
                            pb[k].x = pb[k].x + o2.x;
                            pb[k].z = pb[k].z + o2.z;
                        }
                        if (hit != 0) {
                            snd = 0;
                            if ((f32)p->hitCool > 30.0f) {
                                p->hitCool = 0;
                                snd = 1;
                            }
                            if ((f32)other->hitCool > 30.0f) {
                                other->hitCool = 0;
                                snd = 1;
                            }
                            if (snd != 0) {
                                HuAudFXPlay(0x6AE);
                            }
                        }
                    }
                }
                fn_1_14C1C(p);
                fn_1_14C1C(other);
            }
        }
        other = other->next;
    }
    for (k = 0; k < 3; k++) {
        PSMTXRotRad(rotA, 'y', 0.017453292f * p->angle);
        PSMTXMultVec(rotA, &probe[k], &pt);
        pt.x = pt.x + p->pos.x;
        pt.z = pt.z + p->pos.z;
        if (fn_1_19190(&pt, p->idx) != 0) {
            p->hitTimer = 0x3C;
            p->boost = 0.0f;
            if ((f32)p->hitCool > 18.0f) {
                p->hitCool = 0;
                fn_1_B864(&p->pos, lbl_1_rodata_734[p->idx], p->idx);
            }
        }
    }
}

void fn_1_133E8(Mgr511Player *p, s32 motion, u8 flag)
{
    if (p->motion != motion) {
        CharMotionSet((s16)p->charNo, *fn_1_152BC(&p->motions, motion));
        CharModelAttrReset((s16)p->charNo, 0x40000002);
        CharMotionTimeSet((s16)p->charNo, 0.0f);
        CharMotionSpeedSet((s16)p->charNo, 1.0f);
        if (flag != 0) {
            CharModelAttrReset((s16)p->charNo, 0x40000001);
        } else {
            CharModelAttrSet((s16)p->charNo, 0x40000001);
        }
        p->motion = motion;
    }
}

void fn_1_134DC(Mgr511Player *p)
{
    p->pos.z = (f32)((f64)p->pos.z - 8.75);
    if (p->pos.z < 4500.0f) {
        p->state = lbl_1_data_57C;
    }
}

void fn_1_13554(Mgr511Player *p)
{
    p->flagC8 = 1;
    fn_1_14A40(p);
    p->angle = (f32)((f64)p->angle + ((f64)(1.5f * p->turn) / 60.0));
    if (p->angle > 180.0f) {
        p->angle = p->angle - 360.0f;
    }
    if (p->angle < -180.0f) {
        p->angle = p->angle + 360.0f;
    }
    fn_1_13704(p);
    fn_1_13794(p);
}

void fn_1_13630(Mgr511Player *p)
{
    fn_1_14A40(p);
    p->angle = (f32)((f64)p->angle + ((f64)(1.5f * p->turn) / 60.0));
    if (p->angle > 180.0f) {
        p->angle = p->angle - 360.0f;
    }
    if (p->angle < -180.0f) {
        p->angle = p->angle + 360.0f;
    }
    fn_1_13704(p);
    fn_1_13794(p);
}

void fn_1_13704(Mgr511Player *p)
{
    if (p->accel != 0) {
        p->boost = 1.0f;
    } else {
        p->boost = (f32)((f64)p->boost - 0.15);
        if (p->boost < 0.0f) {
            p->boost = 0.0f;
        }
    }
    if (p->boost > 1.0f) {
        p->boost = 1.0f;
    }
}

void fn_1_13794(Mgr511Player *p)
{
    Vec out;
    Vec v;
    Vec t;
    Mtx rot;
    Mtx inv;
    f64 vx;
    f64 vy;
    f64 vz;
    f64 mag;
    f32 k;
    f32 drag;

    if ((s8)lbl_1_bss_AE8 == 0) {
        lbl_1_bss_AE0 = 60.0;
        lbl_1_bss_AE8 = 1;
    }
    vx = (lbl_1_bss_AE0 * (f64)p->vel.x) / 100.0;
    vy = (lbl_1_bss_AE0 * (f64)p->vel.y) / 100.0;
    vz = (lbl_1_bss_AE0 * (f64)p->vel.z) / 100.0;
    mag = sqrt((vx * vx) + (vz * vz));
    v.x = 0.0f;
    v.y = 0.0f;
    v.z = (f32)mag;
    PSMTXRotRad(rot, 'y', 0.017453292f * p->angle);
    PSMTXMultVec(rot, &v, &out);
    k = 1.0f;
    if (p->padNo < 0) {
        k = lbl_1_rodata_778[p->dif];
    }
    if (p->offCourse != 0) {
        k = 0.8f;
    }
    v.x = 0.0f;
    v.y = 0.0f;
    v.z = (f32)((f64)k * (0.002 * (f64)p->boost * (f64)(1.0f + (0.1f * (f32)p->rank))));
    if (p->angle > 180.0f) {
        p->angle = p->angle - 360.0f;
    }
    if (p->angle < -180.0f) {
        p->angle = p->angle + 360.0f;
    }
    PSMTXRotRad(rot, 'y', 0.017453292f * (f32)((f64)p->angle + (0.05 * (f64)p->turn)));
    PSMTXMultVec(rot, &v, &out);
    if ((s8)lbl_1_bss_AF8 == 0) {
        lbl_1_bss_AF0 = 3000.0 / lbl_1_bss_AE0;
        lbl_1_bss_AF8 = 1;
    }
    drag = 1.0f;
    v.x = (f32)((0.01 * ((99.0 * vx) + (f64)out.x)) + (lbl_1_bss_AF0 * (f64)out.x));
    v.y = (f32)((0.01 * ((99.0 * vy) + (f64)out.y)) + (lbl_1_bss_AF0 * (f64)out.y));
    v.z = (f32)((0.01 * ((99.0 * vz) + (f64)out.z)) + (lbl_1_bss_AF0 * (f64)out.z));
    PSMTXInverse(rot, inv);
    PSMTXMultVec(inv, &v, &out);
    if (p->hit != 0) {
        if ((f32)p->hitCool > 60.0f) {
            p->hitCool = 0;
            fn_1_B864(&p->pos, lbl_1_rodata_788[p->idx], p->idx);
        }
        drag = 1.0f * 3.0f;
    }
    p->hitTimer = p->hitTimer - 1;
    if (p->hitTimer < 0) {
        p->hitTimer = 0;
    } else {
        drag *= 20.0f;
    }
    if (p->boostBtn != 0) {
        drag = (f32)((f64)drag * 5.0);
    }
    t.z = (f32)((f64)out.z - (0.005 * (f64)out.z * (f64)drag));
    t.x = (f32)((f64)out.x - (0.007 * (f64)out.x * (f64)drag));
    t.y = 0.0f;
    PSMTXMultVec(rot, &t, &out);
    p->pos.x = (f32)((f64)p->pos.x + (100.0 * ((f64)out.x / lbl_1_bss_AE0)));
    p->pos.y = (f32)((f64)p->pos.y + (100.0 * ((f64)out.y / lbl_1_bss_AE0)));
    p->pos.z = (f32)((f64)p->pos.z + (100.0 * ((f64)out.z / lbl_1_bss_AE0)));
    p->hit = 0;
}

f32 fn_1_1402C(Mgr511Player *p)
{
    Vec wp;
    f32 steer;
    f32 s;
    f32 a;
    f32 r2;
    f64 r;
    u8 avoid;

    steer = 0.0f;
    avoid = 1;
    fn_1_DAB8(p->lap, &wp);
    if (p->lap > 4) {
        a = p->pos.x;
        if (a < 0.0f) {
            a = -a;
        }
        if (a > 500.0f) {
            wp.z = -4500.0f;
        }
    }
    r2 = (p->pos.x * p->pos.x) + (p->pos.z * p->pos.z);
    r = sqrt(r2);
    if ((f32)r < 1250.0f) {
        steer = fn_1_884((f32)(180.0 * (atan2(-p->pos.x, -p->pos.z) / 3.141592653589793)), p->angle);
        a = steer;
        if (a < 0.0f) {
            a = -a;
        }
        if (a < 75.0f) {
            avoid = 0;
            if (steer >= 0.0f) {
                steer = 75.0f - steer;
            } else {
                steer += -75.0f;
            }
        }
    }
    if (avoid != 0) {
        steer = -fn_1_884((f32)(180.0 * (atan2((f64)(wp.x - p->pos.x), (f64)(wp.z - p->pos.z)) / 3.141592653589793)), p->angle);
    }
    s = steer / 20.0f;
    if (s > 1.0f) {
        s = 1.0f;
    }
    if (s < -1.0f) {
        s = -1.0f;
    }
    if (avoid == 0) {
        p->accel = 1;
        return s;
    }
    a = steer;
    if (a < 0.0f) {
        a = -a;
    }
    if (a < 30.0f) {
        p->accel = 1;
    } else {
        p->accel = 0;
    }
    a = steer;
    if (a < 0.0f) {
        a = -a;
    }
    if (a > (f32)((lbl_1_data_508[p->idx] * 0xF) + 0x3C)) {
        p->boostBtn = 1;
    }
    return s;
}

f32 fn_1_14548(Mgr511Player *p)
{
    Vec wp;
    f32 steer;
    f32 s;
    f32 a;
    f32 b;
    s32 side;

    p->offCourse = 1;
    side = 4;
    if (p->pos.x < 5200.0f && p->pos.z < -5000.0f) {
        side = 0;
    }
    if (p->pos.x > 5000.0f && p->pos.z < 5200.0f) {
        side = 1;
    }
    if (p->pos.x > -5200.0f && p->pos.z > 5000.0f) {
        side = 2;
    }
    if (p->pos.x < -5000.0f && p->pos.z > -5200.0f) {
        side = 3;
    }
    a = p->pos.x;
    if (a < 0.0f) {
        a = -a;
    }
    if (a < 5200.0f) {
        b = p->pos.z;
        if (b < 0.0f) {
            b = -b;
        }
        if (b < 5200.0f) {
            side = 4;
        }
    }
    switch (side) {
    case 0:
        wp.x = 1000.0f + p->pos.x;
        wp.z = -5400.0f;
        break;
    case 1:
        wp.x = 5400.0f;
        wp.z = (f32)(1000.0 + (f64)p->pos.z);
        break;
    case 2:
        wp.x = p->pos.x - 1000.0f;
        wp.z = 5400.0f;
        break;
    case 3:
        wp.x = -5400.0f;
        wp.z = (f32)((f64)p->pos.z - 1000.0);
        break;
    case 4:
        a = p->pos.x;
        if (a < 0.0f) {
            a = -a;
        }
        b = p->pos.z;
        if (b < 0.0f) {
            b = -b;
        }
        if (a > b) {
            if (p->pos.x > 0.0f) {
                wp.x = 5400.0f;
            } else {
                wp.x = -5400.0f;
            }
            wp.z = p->pos.z;
        } else {
            wp.x = p->pos.x;
            if (p->pos.z > 0.0f) {
                wp.z = 5400.0f;
            } else {
                wp.z = -5400.0f;
            }
        }
        break;
    }
    steer = -fn_1_884((f32)(180.0 * (atan2((f64)(wp.x - p->pos.x), (f64)(wp.z - p->pos.z)) / 3.141592653589793)), p->angle);
    s = steer / 20.0f;
    if (s > 1.0f) {
        s = 1.0f;
    }
    if (s < -1.0f) {
        s = -1.0f;
    }
    a = steer;
    if (a < 0.0f) {
        a = -a;
    }
    if (a < 30.0f) {
        p->accel = 1;
    } else {
        p->accel = 0;
    }
    return s;
}

void fn_1_14A40(Mgr511Player *p)
{
    u16 btn;
    s32 gameState;
    f32 steer;

    btn = HuPadBtn[p->padNo];
    p->accel = 0;
    p->boostBtn = 0;
    gameState = fn_1_29C8();
    if (gameState == 0 || gameState == 1) {
        return;
    }
    steer = 0.0f;
    if (p->padNo < 0 || p->finished == 1 || gameState != 2) {
        if (p->finished != 0 || gameState != 2) {
            if (p->finished != 0) {
                steer = fn_1_14548(p);
            } else {
                p->accel = 0;
                p->accel = p->boostBtn;
            }
        } else {
            steer = fn_1_1402C(p);
        }
    } else {
        steer = 0.017857144f * (f32)(s8)HuPadStkX[p->padNo];
        if ((btn & 0x100) != 0) {
            p->accel = 1;
        }
        if ((btn & 0x200) != 0) {
            p->boostBtn = 1;
        }
    }
    p->steer = (f32)((f64)p->steer + (0.1 * (f64)(steer - p->steer)));
    p->turn = p->turn - (8.0f * steer);
    p->turn = p->turn * 0.9f;
}

s32 fn_1_14C1C(Mgr511Player *p)
{
    f32 ax;
    f32 az;
    f32 dxIn;
    f32 dxOut;
    f32 dz;
    f32 x;
    f32 z;
    f64 r;
    s32 gameState;
    s32 hit;

    if (p->offCourse != 0) {
        x = p->pos.x;
        if (x < 0.0f) {
            ax = -x;
        } else {
            ax = x;
        }
        z = p->pos.z;
        if (z < 0.0f) {
            az = -z;
        } else {
            az = z;
        }
        if (ax < 5200.0f && ax > 500.0f && az < 5200.0f) {
            dxOut = 5200.0f - ax;
            dxIn = ax - 500.0f;
            dz = 5200.0f - az;
            if (dz < dxOut && dz < dxIn) {
                if (p->pos.z > 0.0f) {
                    p->pos.z = 5200.0f;
                } else {
                    p->pos.z = -5200.0f;
                }
            } else if (dxOut < dxIn) {
                if (p->pos.x > 0.0f) {
                    p->pos.x = 5200.0f;
                } else {
                    p->pos.x = -5200.0f;
                }
            } else if (p->pos.x > 0.0f) {
                p->pos.x = 500.0f;
            } else {
                p->pos.x = -500.0f;
            }
        }
        return 0;
    }
    gameState = fn_1_29C8();
    if (gameState == 0 || gameState == 1) {
        return 0;
    }
    hit = 0;
    if (p->pos.x > 4800.0f) {
        p->pos.x = 4800.0f;
        hit = 1;
    }
    if (p->pos.x < -4800.0f) {
        p->pos.x = -4800.0f;
        hit = 1;
    }
    if (p->pos.z > 4800.0f) {
        p->pos.z = 4800.0f;
        hit = 1;
    }
    x = p->pos.x;
    z = p->pos.z;
    r = sqrt(((f64)x * (f64)x) + ((f64)z * (f64)z));
    if ((f32)r < 700.0f) {
        p->pos.x = (f32)(700.0 * ((f64)x / r));
        p->pos.z = (f32)(700.0 * ((f64)z / r));
        return 1;
    }
    if (p->pos.z < -4800.0f) {
        if (p->lap < 5) {
            p->pos.z = -4800.0f;
            hit = 1;
        } else {
            dz = -p->pos.z - 4800.0f;
            x = p->pos.x;
            if (x < 0.0f) {
                ax = -x;
            } else {
                ax = x;
            }
            dxIn = ax - 500.0f;
            if (dxIn > 0.0f) {
                if (dz >= dxIn) {
                    p->pos.z = -4800.0f;
                    hit = 1;
                } else if (p->pos.x > 0.0f) {
                    p->pos.x = 500.0f;
                    hit = 1;
                } else {
                    p->pos.x = -500.0f;
                    hit = 1;
                }
            }
            if (p->pos.z < -8000.0f) {
                p->pos.z = -8000.0f;
            }
        }
    }
    return hit;
}

s16 *fn_1_152BC(Mgr511S16Vec *v, u32 index)
{
    if (index >= v->size) {
        fprintf(M511_STDERR, "vector::at index out of range");
        abort();
    }
    return v->data + index;
}

void fn_1_15334(s32 *p, s32 value)
{
    *p = value;
}

void fn_1_1533C(Mgr511PlayerList *list, void (*fn)(Mgr511Player *))
{
    Mgr511PlayerList *e = list;
    Mgr511Player *a = list->next;
    Mgr511Player *b = a;

    fn_1_1539C(b, e, fn);
}

void fn_1_1539C(Mgr511Player *first, Mgr511PlayerList *end, void (*fn)(Mgr511Player *))
{
    Mgr511Player *n;

    n = first;
    while (n != (Mgr511Player *)end) {
        fn(n);
        n = n->next;
    }
}

void fn_1_153FC(Mgr511S16Vec *v, s16 *where, u32 count, s16 *value)
{
    u32 newCap;
    u32 need;
    u32 i;
    u32 pos;
    u32 maxSize;
    s16 *oldData;
    s16 *newData;
    s16 *src;
    s16 *dst;

    if (count != 0) {
        maxSize = VecMaxSize(v);
        if (count > maxSize || v->size > maxSize - count) {
            fprintf(M511_STDERR, "vector::insert length error\n");
            abort();
        }
        if (v->size + count <= VecCapacity(v)) {
            s16 *end = VecEnd(v);
            u32 after = (u32)(end - where);

            src = value;
            if (end - (after - count) <= src && src < end) {
                src = src + count;
            }
            if (after != 0) {
                u32 cnt = (u32)(end - where);

                memmove(end + count - cnt, where, cnt * sizeof(s16));
            }
            dst = where;
            for (i = count; i != 0; i--) {
                *dst = *src;
                dst++;
            }
            v->size = v->size + count;
            return;
        }
        pos = (u32)(where - *VecDataRef(v));
        oldData = *VecDataRef(v);
        need = v->size + count;
        if (VecCapacity(v) != 0) {
            newCap = VecCapacity(v);
        } else {
            newCap = 1;
        }
        while (need > newCap) {
            if (newCap < (maxSize >> 1)) {
                newCap *= 2;
            } else {
                newCap = maxSize;
            }
        }
        newData = (s16 *)__nw__FUl(newCap * sizeof(s16));
        if (newData == 0) {
            fprintf(M511_STDERR, "Memory allocation failure");
            abort();
        }
        *VecDataRef(v) = newData;
        dst = *VecDataRef(v) + pos;
        for (i = count; i != 0; i--) {
            *dst = *value;
            dst++;
        }
        if (oldData != 0) {
            s16 *head = *VecDataRef(v);
            u32 headCnt = (u32)((oldData + pos) - oldData);
            s16 *tail = *VecDataRef(v) + (pos + count);
            u32 tailCnt = (u32)((oldData + v->size) - (oldData + pos));

            memcpy(head, oldData, headCnt * sizeof(s16));
            memcpy(tail, oldData + pos, tailCnt * sizeof(s16));
            __dl__FPv(oldData);
        }
        v->size = need;
        v->cap = newCap;
    }
}

void fn_1_157AC(Mgr511S16Vec *v, u32 cap)
{
    s16 *oldData;
    s16 *newData;
    u32 maxSize;
    u32 cnt;

    if (cap > VecCapacity(v)) {
        maxSize = VecMaxSize(v);
        if (cap > maxSize) {
            fprintf(M511_STDERR, "vector::reserve length error");
            abort();
        }
        oldData = *VecDataRef(v);
        newData = VecAlloc(v, cap);
        if (newData == 0) {
            fprintf(M511_STDERR, "Memory allocation failure");
            abort();
        }
        *VecDataRef(v) = newData;
        if (oldData != 0) {
            cnt = (u32)((oldData + v->size) - oldData);
            memcpy(*VecDataRef(v), oldData, cnt * sizeof(s16));
            __dl__FPv(oldData);
        }
        v->cap = cap;
    }
}

void fn_1_158D4(void)
{
    lbl_1_bss_AA8 = 0x78;
    lbl_1_bss_AAC = 0xD2;
    lbl_1_bss_AB0 = 0xD2;
    fn_1_1D88(&lbl_1_bss_AC4);
    __register_global_object(&lbl_1_bss_AC4, (void *(*)(void *, s16))fn_1_1595C, &lbl_1_bss_AB8);
}

void *fn_1_1595C(void *p, s16 flag)
{
    if (p != 0) {
        fn_1_1D94(p, 0);
        if (flag > 0) {
            __dl__FPv(p);
        }
    }
    return p;
}

}
