// m512Dll/REL/m512Dll/zako
// CZakoMgr owns the shared motion table and spawns eight CZako (the
// wandering enemies).  Each CZako runs a PTMF state (walk / flee / idle),
// steers towards the ring and is pushed out of its neighbours.
#define _MATH_H
#include "types.h"
#include "game/hu3d.h"
#include "game/data.h"

#ifdef __MWERKS__
#define ZAKO_RODATA __declspec(section ".rodata")
#else
#define ZAKO_RODATA
#endif

extern "C" {
double __frsqrte(double x);
extern int __float_nan[];
extern int __float_huge[];
}

// MSL sqrt(double) as the original inlines it (no static pool of its own)
static inline double zako_sqrt(double x)
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

struct ZakoNode {
    ZakoNode *next; // 0x00
    ZakoNode *prev; // 0x04
};

// GC pointer-to-member-function layout
struct ZakoPtmf {
    s32 adj;
    s32 idx;
    void (*fn)(void *);
};

struct ZakoVec {
    u32 cap;    // +0x00
    u32 size;   // +0x04
    s16 *data;  // +0x08
};

struct ZakoModelId {
    s16 id;
};

struct ZakoMgr {
    ZakoNode node;      // 0x00
    void **vtbl;        // 0x08
    s32 unkC;           // 0x0C
    ZakoVec motIds;     // 0x10
    ZakoModelId model;  // 0x1C
};

struct Zako {
    ZakoNode node;      // 0x00
    u8 unk8;            // 0x08
    u8 flags;           // 0x09
    ZakoPtmf hook;      // 0x0C
    s32 motion;         // 0x18
    f32 angle;          // 0x1C
    f32 angVel;         // 0x20
    f32 angVelTgt;      // 0x24
    f32 speed;          // 0x28
    s32 cooldown;       // 0x2C
    s32 unk30;          // 0x30
    HuVecF pos;         // 0x34
    HuVecF unk40;       // 0x40
    f32 unk4C;          // 0x4C
    s32 unk50;          // 0x50
    s32 unk54;          // 0x54
    f32 radius;         // 0x58
    ZakoModelId model;  // 0x5C
};

struct ZakoPosTable {
    f32 v[8][3];
};

extern "C" {
double atan2(double y, double x);
void *__dl__FPv(void *);
void *__nw__FUl(u32 size);
void __register_global_object(void *obj, void *dtor, void *reg);
void __ptmf_scall(void *obj);
void *fn_1_1264(void *self);
void fn_1_1270(void *self, int flag);
void fn_1_12DC(void *self);
void fn_1_1330(void *self, void *arg);
void fn_1_1D4(const char *msg, const char *file, int line);
void fn_1_2C94(void *self, s32 dataNum);
void fn_1_2EF4(void *self, void *vec);
void fn_1_2F3C(void *self, void *vec);
void fn_1_30F4(void *self, s32 id);
s32 fn_1_4AE0(void);
f32 fn_1_4D1C(void);
s16 *fn_1_CA88(void *vec, s32 idx);
void fn_1_CBC0(void *vec, s16 *pos, s32 count, s16 *val);
void fn_1_CF70(void *vec, s32 count);

extern ZakoNode lbl_1_bss_C;
extern void *lbl_1_data_1FC[4];
extern ZakoMgr *lbl_1_bss_A08;
extern f64 lbl_1_bss_AA8;
extern u8 lbl_1_bss_AB0;
}

extern "C" void *fn_1_E99C(ZakoMgr *self, s16 flag);
extern "C" void fn_1_EAD4(void);
extern "C" void fn_1_EAF4(ZakoMgr *self, s32 bit);
extern "C" void fn_1_EF9C(void);
extern "C" void fn_1_F070(void);
extern "C" void fn_1_F0F4(Zako *z);
extern "C" void fn_1_F168(Zako *z);
extern "C" void fn_1_F1E4(Zako *z);
extern "C" void fn_1_F4F0(Zako *z);
extern "C" void fn_1_F534(Zako *z);
extern "C" void fn_1_F7CC(Zako *z);
extern "C" void fn_1_F968(Zako *z);
extern "C" s32 fn_1_FB38(Zako *z);
extern "C" s32 fn_1_FCCC(Zako *z);
extern "C" f32 fn_1_10444(Zako *z);
extern "C" void fn_1_105B0(ZakoNode *head, void (*fn)(Zako *));
extern "C" void fn_1_10610(ZakoNode *first, ZakoNode *end, void (*fn)(Zako *));
extern "C" void fn_1_10670(void);
extern "C" void *fn_1_106F8(void *self, s16 flag);
extern "C" Zako *fn_1_ED38(Zako *z, HuVecF *pos);
extern "C" s16 fn_1_E904(ZakoMgr *self, s32 dataNum);

// -------------------------------------------------------------------- data

extern "C" {
ZAKO_RODATA const ZakoPosTable lbl_1_rodata_3E8 = { {
    { -300.0f, 49.0f, -500.0f },
    { 0.0f, 49.0f, -500.0f },
    { 300.0f, 49.0f, -500.0f },
    { -800.0f, 49.0f, 0.0f },
    { 800.0f, 49.0f, 0.0f },
    { -300.0f, 49.0f, 500.0f },
    { 0.0f, 49.0f, 500.0f },
    { 300.0f, 49.0f, 500.0f },
} };

char lbl_1_data_4E8[] = "ms_pointer == 0";
char lbl_1_data_4F8[] = "singleton.h";
char lbl_1_data_504[] = "m_mot_id.size() == NUMBEROF_PMOTION";
char lbl_1_data_528[] = "zako.cpp";
char lbl_1_data_531[0x13] = "ms_pointer != 0";
ZakoPtmf lbl_1_data_544 = { 0, -1, (void (*)(void *))fn_1_F4F0 };
ZakoPtmf lbl_1_data_550 = { 0, -1, (void (*)(void *))fn_1_F534 };
ZakoPtmf lbl_1_data_55C = { 0, -1, (void (*)(void *))fn_1_F7CC };
ZakoPtmf lbl_1_data_568 = { 0, -1, (void (*)(void *))fn_1_F968 };
ZakoPtmf lbl_1_data_574 = { 0, -1, (void (*)(void *))fn_1_F534 };
void *lbl_1_data_580[5] = { 0, 0, (void *)fn_1_E99C, (void *)fn_1_EAF4, (void *)fn_1_EAD4 };
// vector<s16> error strings
char lbl_1_data_594[] = "vector::at index out of range\0vector::insert length error\n\0Memory allocation failure\0vector::reserve length error\n\0";
}

f64 lbl_1_bss_A68;
char lbl_1_bss_A70[1];
f64 lbl_1_bss_A78;
f64 lbl_1_bss_A80;
s32 lbl_1_bss_A88;
s32 lbl_1_bss_A8C;
s32 lbl_1_bss_A90;
char lbl_1_bss_A94[12];
ZakoNode lbl_1_bss_AA0;

// ------------------------------------------------------------------ functions

extern "C" ZakoMgr *fn_1_E698(ZakoMgr *self)
{
    ZakoPosTable table;
    ZakoVec *vec;
    s32 i;
    Zako *z;

    fn_1_1264(self);
    self->vtbl = lbl_1_data_1FC;
    self->unkC = 0;
    fn_1_1330(self, &lbl_1_bss_C);
    if (lbl_1_bss_A08 == 0) {
    } else {
        fn_1_1D4(lbl_1_data_4E8, lbl_1_data_4F8, 0x35);
    }
    {
        ZakoMgr *s = (ZakoMgr *)((char *)self + 0x10);
        if (s != 0) {
            s = (ZakoMgr *)((char *)s - 0x10);
        }
        lbl_1_bss_A08 = s;
    }
    self->vtbl = lbl_1_data_580;
    vec = &self->motIds;
    vec->cap = 0;
    vec->data = 0;
    vec->size = 0;
    fn_1_2C94(&self->model, 0x460009);
    Hu3DModelAttrSet(self->model.id, 1);
    self->motIds.size = 0;
    fn_1_CF70(&self->motIds, 3);
    fn_1_E904(self, 0x46000D);
    fn_1_E904(self, 0x46000E);
    fn_1_E904(self, 0x46000F);
    if (self->motIds.size == 3) {
    } else {
        fn_1_1D4(lbl_1_data_504, lbl_1_data_528, 0x37);
    }
    table = lbl_1_rodata_3E8;
    for (i = 0; i < 8; i++) {
        z = (Zako *)__nw__FUl(sizeof(Zako));
        if (z != 0) {
            z = fn_1_ED38(z, (HuVecF *)table.v[i]);
        }
    }
    return self;
}

extern "C" ZakoMgr *fn_1_E878(ZakoMgr *self, s16 flag)
{
    if (self != 0) {
        if (lbl_1_bss_A08 != 0) {
        } else {
            fn_1_1D4(lbl_1_data_531, lbl_1_data_4F8, 0x3D);
        }
        lbl_1_bss_A08 = 0;
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

extern "C" s16 fn_1_E904(ZakoMgr *self, s32 dataNum)
{
    s16 mot;
    s16 body;
    s16 **dp;
    s16 *end;

    body = self->model.id;
    mot = Hu3DJointMotion(body, HuDataSelHeapReadNum(dataNum, 0x10000000, HEAP_MODEL));
    dp = &self->motIds.data;
    end = *dp + self->motIds.size;
    fn_1_CBC0(&self->motIds, end, 1, &mot);
    return mot;
}

extern "C" void *fn_1_E99C(ZakoMgr *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = lbl_1_data_580;
        fn_1_EF9C();
        fn_1_30F4(&self->model, -1);
        if ((ZakoVec *)((char *)self + 0x10) != 0) {
            if ((ZakoVec *)((char *)self + 0x10) != 0) {
                if ((ZakoVec *)((char *)self + 0x10) != 0) {
                    s16 **dp = &self->motIds.data;
                    if (*dp != 0) {
                        s16 **dp2 = &self->motIds.data;
                        s16 *d = *dp2;
                        __dl__FPv(d);
                    }
                }
            }
        }
        if ((void *)((char *)self + 0x10) != 0) {
            if (lbl_1_bss_A08 != 0) {
            } else {
                fn_1_1D4(lbl_1_data_531, lbl_1_data_4F8, 0x3D);
            }
            lbl_1_bss_A08 = 0;
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

extern "C" void fn_1_EAD4(void)
{
    fn_1_F070();
}

extern "C" void fn_1_EAF4(ZakoMgr *self, s32 bit)
{
    self->unkC &= ~(1 << bit);
    if (bit < 5) {
    }
}

// shortest signed difference between two angles in degrees
extern "C" f32 fn_1_EB1C(f32 a, f32 b)
{
    s32 ia = (s32)(182.04444444444445 * a);
    s32 ib = (s32)(182.04444444444445 * b);
    s32 ja = ia;
    s32 jb = ib;
    s32 diff = ja - jb;
    s32 wrapped = (u16)(diff - 0x8000) - 0x8000;
    s32 w2 = wrapped;
    return 0.0054931640625 * w2;
}

extern "C" f32 fn_1_EBD4(f32 a, f32 b, f32 limit)
{
    f32 d;
    f32 r;
    f32 d0;
    f32 d1;
    f32 d2;

    d0 = fn_1_EB1C(a, b);
    d1 = d0;
    d2 = d1;
    d = d2;
    if (d < -limit) {
        r = -limit;
    } else if (d > limit) {
        r = limit;
    } else {
        r = d;
    }
    return r;
}

extern "C" Zako *fn_1_ED38(Zako *z, HuVecF *pos)
{
    s16 m;
    ZakoPtmf tmp;
    f32 r;

    fn_1_1264(z);
    z->unk30 = 0;
    fn_1_2C94(&z->model, 0x460009);
    fn_1_1330(z, &lbl_1_bss_AA0);
    z->flags = 0;
    z->angle = 0.0f;
    z->motion = -1;
    z->cooldown = 0;
    z->speed = 0.0f;
    z->pos.x = pos->x;
    z->pos.y = pos->y;
    z->pos.z = pos->z;
    m = z->model.id;
    Hu3DModelShadowSet(m);
    Hu3DModelShadowDispOn(m);
    Hu3DModelScaleSet(m, 1.0f, 1.0f, 1.0f);
    Hu3DModelAttrReset(m, 1);
    Hu3DModelLayerSet(m, 3);
    Hu3DModelCameraSet(m, 1);
    Hu3DModelAttrSet(m, 0x4000);
    z->angle = fn_1_10444(z);
    fn_1_FCCC(z);
    z->unk40.z = 0.0f;
    z->unk40.y = 0.0f;
    z->unk40.x = 0.0f;
    z->unk4C = 0.0f;
    z->unk54 = 0;
    z->unk50 = 0;
    z->angVel = 0.0f;
    r = fn_1_4D1C();
    z->angVelTgt = 70.0f * r - 40.0f;
    z->radius = 750.0f;
    fn_1_2EF4(&z->model, &z->pos);
    tmp = lbl_1_data_544;
    z->hook = tmp;
    return z;
}

extern "C" void *fn_1_EF44(void *self, s16 flag)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

extern "C" void fn_1_EF9C(void)
{
    ZakoNode *p;
    u8 empty;

    while (1) {
        empty = 0;
        if (lbl_1_bss_AA0.next == &lbl_1_bss_AA0 && lbl_1_bss_AA0.next == lbl_1_bss_AA0.prev) {
            empty = 1;
        }
        if (empty != 0) {
            break;
        }
        p = lbl_1_bss_AA0.prev;
        if (p != 0) {
            fn_1_30F4(&((Zako *)p)->model, -1);
            if (p != 0) {
                fn_1_1270(p, 0);
            }
            __dl__FPv(p);
        }
    }
}

extern "C" void fn_1_F070(void)
{
    ZakoNode *a;
    ZakoNode *b;
    ZakoNode *first;

    fn_1_105B0(&lbl_1_bss_AA0, fn_1_F0F4);
    a = lbl_1_bss_AA0.next;
    b = a;
    first = b;
    if (first != &lbl_1_bss_AA0) {
        fn_1_12DC(first);
        fn_1_1330(first, &lbl_1_bss_AA0);
    }
}

extern "C" void fn_1_F0F4(Zako *z0)
{
    Zako *z = z0;
#ifdef TARGET_PC
    z->hook.fn(z);
#else
    __ptmf_scall(z);
#endif
    z->flags = 0;
    fn_1_F1E4(z);
    z->flags = z->flags | fn_1_FCCC(z);
    fn_1_F168(z);
}

extern "C" void fn_1_F168(Zako *z0)
{
    HuVecF rot;
    Zako *z = z0;

    rot.x = 0.0f;
    rot.y = z->angle;
    rot.z = 0.0f;
    fn_1_2F3C(&z->model, &rot);
    fn_1_2EF4(&z->model, &z->pos);
}

extern "C" void fn_1_F1E4(Zako *z)
{
    HuVecF selfPos;
    HuVecF otherPos;
    HuVecF d;
    HuVecF a;
    HuVecF b;
    ZakoNode *n;
    f32 dist;
    f32 w;
    f32 w2;
    s32 cd;

    selfPos = z->pos;
    if (z->cooldown > 0) {
        z->cooldown = z->cooldown - 1;
    }
    for (n = lbl_1_bss_AA0.next; n != &lbl_1_bss_AA0; n = n->next) {
        if (z != (Zako *)n) {
            Zako *o = (Zako *)n;
            otherPos = o->pos;
            PSVECSubtract((Vec *)&o->pos, (Vec *)&z->pos, (Vec *)&d);
            dist = PSVECMag((Vec *)&d);
            if (!(dist >= 120.0f)) {
                w = 0.5f;
                w2 = 1.0f - w;
                PSVECScale((Vec *)&d, (Vec *)&d, (120.0f - dist) / dist);
                PSVECScale((Vec *)&d, (Vec *)&a, 1.05f * -w);
                PSVECScale((Vec *)&d, (Vec *)&b, 1.05f * w2);
                PSVECAdd((Vec *)&a, (Vec *)&z->pos, (Vec *)&z->pos);
                PSVECAdd((Vec *)&b, (Vec *)&o->pos, (Vec *)&o->pos);
                if (w2 >= 0.5f) {
                    cd = 5;
                } else {
                    cd = 0;
                }
                o->cooldown = cd;
                z->flags = z->flags | fn_1_FCCC(z);
                o->flags = o->flags | fn_1_FCCC(o);
            }
        }
    }
}

extern "C" void fn_1_F3F0(Zako *z, s32 motion, u8 loop)
{
    s16 id;
    s16 mot;
    u32 attr;

    if (lbl_1_bss_A08 != 0) {
    } else {
        fn_1_1D4(lbl_1_data_531, lbl_1_data_4F8, 0x27);
    }
    id = z->model.id;
    mot = *fn_1_CA88(&lbl_1_bss_A08->motIds, motion);
    if (z->motion != motion) {
        Hu3DModelAttrReset(id, 0x40000002);
        if (loop == 0) {
            attr = 0x40000001;
        } else {
            attr = 0;
        }
        Hu3DMotionShiftSet(id, mot, 0.0f, 8.0f, attr);
        z->motion = motion;
    }
}

extern "C" void fn_1_F4F0(Zako *z)
{
    ZakoPtmf tmp;

    tmp = lbl_1_data_550;
    z->hook = tmp;
}

extern "C" void fn_1_F534(Zako *z)
{
    ZakoPtmf tmp;
    f32 s;

    z->angle = fn_1_10444(z);
    if (z->angVelTgt > z->angVel) {
        z->angVel = z->angVel + 2.0;
        if (z->angVelTgt < z->angVel) {
            z->angVel = z->angVelTgt;
        }
    }
    if (z->angVelTgt < z->angVel) {
        z->angVel = z->angVel - 2.0;
        if (z->angVelTgt > z->angVel) {
            z->angVel = z->angVelTgt;
        }
    }
    z->angle = z->angle + z->angVel;
    if (z->angle > 180.0f) {
        z->angle = z->angle - 360.0f;
    }
    if (z->angle < -180.0f) {
        z->angle = z->angle + 360.0f;
    }
    s = z->speed + 0.016666668f;
    z->speed = s;
    if (s > 1.0f) {
        z->speed = 1.0f;
        if ((fn_1_4AE0() & 0x7F) == 0) {
            tmp = lbl_1_data_55C;
            z->hook = tmp;
            z->unk50 = 0;
        }
    }
    fn_1_FB38(z);
    Hu3DMotionSpeedSet(z->model.id, z->speed);
    fn_1_F3F0(z, 0, 1);
}

extern "C" void fn_1_F7CC(Zako *z)
{
    ZakoPtmf tmp;

    z->unk50 = z->unk50 + 1;
    if ((f32)z->unk50 > 60.0f) {
        tmp = lbl_1_data_568;
        z->hook = tmp;
    }
    z->speed = 3.0f;
    fn_1_FB38(z);
    Hu3DMotionSpeedSet(z->model.id, 1.0f);
    fn_1_F3F0(z, 1, 0);
}

extern "C" void fn_1_F968(Zako *z)
{
    ZakoPtmf tmp;
    u8 done;
    f32 s;
    f32 r;

    done = 0;
    s = z->speed - 0.033333335f;
    z->speed = s;
    if (s < 0.0f) {
        z->speed = 0.0f;
        done = 1;
    }
    if (done != 0) {
        z->speed = 0.0f;
        tmp = lbl_1_data_574;
        z->hook = tmp;
        r = fn_1_4D1C();
        z->angVelTgt = 70.0f * r - 10.0f;
    }
    fn_1_FB38(z);
    Hu3DMotionSpeedSet(z->model.id, 1.0f);
    fn_1_F3F0(z, 2, 1);
}

extern "C" s32 fn_1_FB38(Zako *z)
{
    HuVecF dir;
    HuVecF out;
    Mtx m;

    if ((s8)lbl_1_bss_AB0 == 0) {
        lbl_1_bss_AA8 = 60.0;
        lbl_1_bss_AB0 = 1;
    }
    dir.x = 0.0f;
    dir.y = 0.0f;
    dir.z = 1.5f * z->speed;
    PSMTXRotRad(m, 'y', 0.017453292f * z->angle);
    PSMTXMultVec(m, (Vec *)&dir, (Vec *)&out);
    z->pos.x = z->pos.x + 100.0 * (out.x / lbl_1_bss_AA8);
    z->pos.y = z->pos.y + 100.0 * (out.y / lbl_1_bss_AA8);
    z->pos.z = z->pos.z + 100.0 * (out.z / lbl_1_bss_AA8);
    if (z->speed <= 0.01f) {
        return 0;
    }
    return 1;
}

// Keeps a zako inside the arena (two end caps and the straight sections);
// returns 1 when it hit the outer wall.
extern "C" s32 fn_1_FCCC(Zako *z)
{
    f32 dx;
    f32 dz;
    f32 d;
    f32 dist;
    f64 ex;
    f64 ez;
    f64 edist;
    f32 lim;
    f64 cx;

    dx = z->pos.x - -104.0f;
    dz = z->pos.z - 550.0f;
    d = zako_sqrt(dx * dx + dz * dz);
    if (d < 150.0) {
        z->pos.x = -104.0 + 150.0 * (dx / d);
        z->pos.z = 550.0 + 150.0 * (dz / d);
        if (z->pos.z > 550.0f) {
            z->pos.z = 550.0f;
        }
        if (z->pos.z < 380.0f) {
            z->pos.z = 380.0f;
        }
        return 1;
    }
    if (z->pos.x > -300.0 && z->pos.x < 300.0) {
        if (z->pos.z > 0.0f) {
            if (z->pos.z > 550.0f) {
                z->pos.z = 550.0f;
                z->radius = 550.0f;
                return 1;
            }
            if (z->pos.z < 380.0f) {
                z->pos.z = 380.0f;
                z->radius = 380.0f;
                return 0;
            }
        } else {
            if (z->pos.z < -550.0f) {
                z->pos.z = -550.0f;
                z->radius = 550.0f;
                return 1;
            }
            if (z->pos.z > -380.0f) {
                z->pos.z = -380.0f;
                z->radius = 380.0f;
                return 0;
            }
        }
        lim = z->pos.z;
        if (lim < 0.0f) {
            dist = -lim;
        } else {
            dist = lim;
        }
        z->radius = dist;
        return 0;
    }
    cx = 300.0;
    if (z->pos.x < 0.0f) {
        cx = -300.0;
    }
    ex = z->pos.x - cx;
    ez = z->pos.z;
    edist = zako_sqrt(ex * ex + ez * ez);
    z->radius = edist;
    if (edist < 380.0) {
        z->pos.x = cx + 380.0 * (ex / edist);
        z->pos.z = 380.0 * (ez / edist);
        z->radius = 380.0f;
        return 0;
    }
    if (edist > 550.0) {
        z->pos.x = cx + 550.0 * (ex / edist);
        z->pos.z = 550.0 * (ez / edist);
        z->radius = 550.0f;
        return 1;
    }
    return 0;
}

extern "C" f32 fn_1_10444(Zako *z)
{
    f32 cx;
    f32 ang;

    if (z->pos.x > -300.0 && z->pos.x < 300.0) {
        if (z->pos.z > 0.0f) {
            return 90.0f;
        }
        return -90.0f;
    }
    cx = 300.0f;
    if (z->pos.x < 0.0f) {
        cx = -300.0f;
    }
    ang = 90.0 + 180.0 * (atan2(z->pos.x - cx, z->pos.z) / 3.141592653589793);
    if (ang > 180.0f) {
        ang -= 360.0f;
    }
    return ang;
}

extern "C" void fn_1_105B0(ZakoNode *head, void (*fn)(Zako *))
{
    ZakoNode *h2 = head;
    ZakoNode *first = head->next;
    ZakoNode *first2 = first;
    fn_1_10610(first2, h2, fn);
}

extern "C" void fn_1_10610(ZakoNode *first, ZakoNode *end, void (*fn)(Zako *))
{
    ZakoNode *p = first;
    while (p != end) {
        fn((Zako *)p);
        p = p->next;
    }
}

extern "C" void fn_1_10670(void)
{
    lbl_1_bss_A88 = 120;
    lbl_1_bss_A8C = 210;
    lbl_1_bss_A90 = 210;
    fn_1_1264(&lbl_1_bss_AA0);
    __register_global_object(&lbl_1_bss_AA0, (void *)fn_1_106F8, lbl_1_bss_A94);
}

#ifdef __MWERKS__
__declspec(section ".ctors") void (*const zako_ctor)(void) = fn_1_10670;
#else
static struct ZakoSinit { ZakoSinit() { fn_1_10670(); } } zako_sinit;
#endif

extern "C" void *fn_1_106F8(void *self, s16 flag)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}
