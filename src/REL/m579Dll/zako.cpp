#include "dolphin.h"
#include <math.h>
#include "game/data.h"
#include "game/hu3d.h"
#include "REL/m579Dll/m579objs.h"

extern "C" {
void *__register_global_object(void *object, void *destructor, void *regmem);
void __ptmf_scall(void *self, void *ptmf);
void fn_1_106CC(M579S16Vec *vec, s16 *pos, s32 n, s16 *val);
void fn_1_10A7C(M579S16Vec *vec, s32 n);
void fn_1_2C94(s16 *model, s32 dataNum);
void fn_1_2E40(s16 *model, s32 attr);
s16 *fn_1_7528(M579S16Vec *vec, u32 idx);
M579Puku *fn_1_6074(M579Puku *self, s16 *src, s32 type);
void fn_1_6388(void);
void fn_1_645C(void);
void *fn_1_BD18(void *self, Vec *pos, s32 type);
extern M579MotionMgr *lbl_1_bss_A14;
extern M579ItemMgr *lbl_1_bss_A10;
extern u8 lbl_1_bss_C[];
extern u8 lbl_1_data_1FC[];

void *fn_1_15AD8(M579MotionMgr *self);
M579MotionMgr *fn_1_15F3C(M579MotionMgr *self, s16 flag);
s16 fn_1_15FC8(M579MotionMgr *self, s32 dataNum);
M579MotionMgr *fn_1_16060(M579MotionMgr *self, s16 flag);
void fn_1_161A8(void);
void fn_1_161CC(M579MotionMgr *self, s32 bit);
M579Zako *fn_1_161F4(M579Zako *self, s16 *src, Vec *pos, u8 type);
M579Zako *fn_1_163A4(M579Zako *self, s16 flag);
void fn_1_163FC(void);
void fn_1_164D0(void);
void fn_1_16554(M579Zako *self);
void fn_1_165A8(M579Zako *self);
void fn_1_16624(M579Zako *self, s32 idx, u8 flag);
void fn_1_16724(M579Zako *self);
void fn_1_167B0(M579Zako *self);
s32 fn_1_169E0(M579Zako *self);
s32 fn_1_16C68(M579Zako *self);
void *fn_1_16F3C(M579ItemMgr *self);
M579ItemMgr *fn_1_17250(M579ItemMgr *self, s16 flag);
M579ItemMgr *fn_1_172DC(M579ItemMgr *self, s16 flag);
s16 fn_1_17430(M579ItemMgr *self, s32 dataNum);
void fn_1_174C8(M579ItemMgr *self);
void fn_1_17768(M579ItemMgr *self);
void fn_1_17944(M579ItemMgr *self);
void fn_1_17A08(M579ItemMgr *self);
void fn_1_17B40(M579ItemMgr *self);
void fn_1_17BD0(M579ItemMgr *self, s32 idx, u8 flag);
void fn_1_17C8C(M579ItemMgr *self);
void fn_1_17D64(HU3DMODEL *modelP, Mtx *mtx);
void fn_1_17DA8(M579ItemMgr *self);
void fn_1_17FD4(M579ItemMgr *self, s32 bit);
void fn_1_18048(M579Node *list, void (*fn)(M579Zako *));
void fn_1_180A8(M579Node *first, M579Node *list, void (*fn)(M579Zako *));
void fn_1_18108(void);
M579Node *fn_1_18160(M579Node *self, s16 flag);
}

char lbl_1_data_6B8[16] = "ms_pointer == 0";
char lbl_1_data_6C8[12] = "singleton.h";
char lbl_1_data_6D4[36] = "m_mot_id.size() == NUMBEROF_PMOTION";
char lbl_1_data_6F8[9] = "zako.cpp";
char lbl_1_data_701[19] = "ms_pointer != 0";
M579Ptmf lbl_1_data_714 = { 0, -1, (void *) fn_1_167B0 };
M579Ptmf lbl_1_data_720 = { 0, -1, (void *) fn_1_17768 };
char lbl_1_data_72C[12] = "itemhook_S";
M579Ptmf lbl_1_data_738 = { 0, -1, (void *) fn_1_17944 };
M579Ptmf lbl_1_data_744 = { 0, -1, (void *) fn_1_17A08 };
M579Ptmf lbl_1_data_750 = { 0, -1, (void *) fn_1_17B40 };
void *lbl_1_data_75C[5] = { 0, 0, (void *) fn_1_172DC, (void *) fn_1_17FD4, (void *) fn_1_174C8 };
void *lbl_1_data_770[5] = { 0, 0, (void *) fn_1_16060, (void *) fn_1_161CC, (void *) fn_1_161A8 };
char lbl_1_data_784[30] = "vector::at index out of range";
char lbl_1_data_7A2[29] = "vector::insert length error\n";
char lbl_1_data_7BF[26] = "Memory allocation failure";
char lbl_1_data_7D9[30] = "vector::reserve length error\n";

u8 lbl_1_bss_1288[12];
M579Node lbl_1_bss_1294;
f64 lbl_1_bss_12A0;
s8 lbl_1_bss_12A8;

M579_RODATA const Vec lbl_1_rodata_AB8 = { 0.0f, -500.0f, 0.0f };
M579_RODATA const Vec lbl_1_rodata_AD0 = { 0.0f, 0.0f, 0.0f };
M579_RODATA const Vec lbl_1_rodata_B18 = { 0.0f, 50.0f, 0.0f };

void *fn_1_15AD8(M579MotionMgr *self)
{
    Vec v;
    f32 a;
    s32 i;
    void *mem;

    fn_1_1264(self);
    self->base.vtbl = (void **) lbl_1_data_1FC;
    self->base.flags = 0;
    fn_1_1330(self, lbl_1_bss_C);
    lbl_1_bss_A14 == 0 ? (void) 0 : fn_1_1D4(lbl_1_data_6B8, lbl_1_data_6C8, 0x35);
#ifdef TARGET_PC
    lbl_1_bss_A14 = self;
#else
    {
        u8 *q = (u8 *) self + 0x10;
        if (q != 0) {
            q -= 0x10;
        }
        lbl_1_bss_A14 = (M579MotionMgr *) q;
    }
#endif
    self->base.vtbl = lbl_1_data_770;
    self->motion.cap = 0;
    self->motion.size = 0;
    self->motion.data = 0;
    fn_1_2C94(&self->model[0], 0x850006);
    fn_1_2C94(&self->model[1], 0x850007);
    Hu3DModelAttrSet(self->model[0], 1);
    Hu3DModelAttrSet(self->model[1], 1);
    self->motion.size = 0;
    fn_1_10A7C(&self->motion, 2);
    fn_1_15FC8(self, 0x850008);
    fn_1_15FC8(self, 0x850009);
    if (self->motion.size == 2) {
    } else {
        fn_1_1D4(lbl_1_data_6D4, lbl_1_data_6F8, 0x35);
    }
    for (i = 0; i < 0xC; i++) {
        a = (f32) ((3.141592653589793 * (f64) (360.0f * ((f32) i / 14.0f))) / 180.0);
        v.x = (f32) (400.0 * (fn_1_4D1C() - 0.5));
        v.y = (f32) (-220.0 - (f64) (i * 0x14));
        v.z = (f32) (400.0 * (fn_1_4D1C() - 0.5));
        mem = fn_1_0(sizeof(M579Zako));
        if (mem != 0) {
            fn_1_161F4((M579Zako *) mem, &self->model[0], &v, 0);
        }
    }
    for (i = 0; i < 6; i++) {
        a = (f32) ((3.141592653589793 * (f64) (360.0f * ((f32) i / 6.0f))) / 180.0);
        v.x = (f32) (900.0f * cos(a));
        v.y = -250.0f;
        v.z = (f32) (900.0f * sin(a));
        mem = fn_1_0(sizeof(M579Zako));
        if (mem != 0) {
            fn_1_161F4((M579Zako *) mem, &self->model[1], &v, 1);
        }
    }
    for (i = 0; i < 5; i++) {
        mem = fn_1_0(sizeof(M579Puku));
        if (mem != 0) {
            fn_1_6074((M579Puku *) mem, &self->model[0], 0);
        }
    }
    for (i = 0; i < 5; i++) {
        mem = fn_1_0(sizeof(M579Puku));
        if (mem != 0) {
            fn_1_6074((M579Puku *) mem, &self->model[1], 1);
        }
    }
    return self;
}

M579MotionMgr *fn_1_15F3C(M579MotionMgr *self, s16 flag)
{
    if (self != 0) {
        lbl_1_bss_A14 != 0 ? (void) 0 : fn_1_1D4(lbl_1_data_701, lbl_1_data_6C8, 0x3D);
        lbl_1_bss_A14 = 0;
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

s16 fn_1_15FC8(M579MotionMgr *self, s32 dataNum)
{
    s16 mot;
    s16 model;
    s32 n;

    n = dataNum;
    model = self->model[0];
    mot = Hu3DJointMotion(model, HuDataSelHeapReadNum(n, 0x10000000, HEAP_MODEL));
    fn_1_106CC(&self->motion, self->motion.data + self->motion.size, 1, &mot);
    return mot;
}

M579MotionMgr *fn_1_16060(M579MotionMgr *self, s16 flag)
{
    if (self != 0) {
        self->base.vtbl = lbl_1_data_770;
        fn_1_163FC();
        fn_1_6388();
        fn_1_30F4(&self->model[1], -1);
        fn_1_30F4(&self->model[0], -1);
        if (&self->motion != 0 && &self->motion != 0 && &self->motion != 0 && self->motion.data != 0) {
            fn_1_C8(self->motion.data);
        }
        if (self != 0) {
            lbl_1_bss_A14 != 0 ? (void) 0 : fn_1_1D4(lbl_1_data_701, lbl_1_data_6C8, 0x3D);
            lbl_1_bss_A14 = 0;
        }
        if (self != 0) {
            self->base.vtbl = (void **) lbl_1_data_1FC;
            if (self != 0) {
                fn_1_1270(self, 0);
            }
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_161A8(void)
{
    fn_1_164D0();
    fn_1_645C();
}

void fn_1_161CC(M579MotionMgr *self, s32 bit)
{
    self->base.flags = self->base.flags & ~(1 << bit);
    switch (bit) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
        break;
    }
}

M579Zako *fn_1_161F4(M579Zako *self, s16 *src, Vec *pos, u8 type)
{
    s16 model;
    M579Ptmf p;

    fn_1_1264(self);
    self->u1C = 0;
    fn_1_2DB4(&self->model, src);
    self->type = type;
    fn_1_1330(self, &lbl_1_bss_1294);
    self->yaw = 0.0f;
    self->anim = -1;
    self->speed = 0.0f;
    self->pos.x = pos->x;
    self->pos.y = pos->y;
    self->pos.z = pos->z;
    model = self->model;
    Hu3DModelScaleSet(model, 1.0f, 1.0f, 1.0f);
    Hu3DModelAttrReset(model, 1);
    Hu3DModelLayerSet(model, 0);
    Hu3DModelCameraSet(model, 1);
    self->yaw = 360.0f * fn_1_4D1C();
    fn_1_16C68(self);
    self->frame = 0;
    self->phase = (360.0f * fn_1_4D1C()) - 180.0f;
    fn_1_2EF4(&self->model, &self->pos);
    p = lbl_1_data_714;
    self->update = p;
    fn_1_16724(self);
    return self;
}

M579Zako *fn_1_163A4(M579Zako *self, s16 flag)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_163FC(void)
{
    M579Zako *p;
    u8 empty;

    for (;;) {
        empty = 0;
        if (lbl_1_bss_1294.next == &lbl_1_bss_1294 && lbl_1_bss_1294.next == lbl_1_bss_1294.prev) {
            empty = 1;
        }
        if (empty != 0) {
            break;
        }
        p = (M579Zako *) lbl_1_bss_1294.prev;
        if (p != 0) {
            fn_1_30F4(&p->model, -1);
            if (p != 0) {
                fn_1_1270(p, 0);
            }
            fn_1_C8(p);
        }
    }
}

void fn_1_164D0(void)
{
    M579Node *n;
    M579Node *a;
    M579Node *b;

    fn_1_18048(&lbl_1_bss_1294, fn_1_16554);
    a = lbl_1_bss_1294.next;
    b = a;
    n = b;
    if (n != &lbl_1_bss_1294) {
        fn_1_12DC(n);
        fn_1_1330(n, &lbl_1_bss_1294);
    }
}

void fn_1_16554(M579Zako *self)
{
#ifdef TARGET_PC
    if (self->update.func != 0) {
        ((void (*)(M579Zako *)) self->update.func)(self);
    }
#else
    __ptmf_scall(self, &self->update);
#endif
    fn_1_16C68(self);
    fn_1_165A8(self);
    self->frame = self->frame + 1;
}

void fn_1_165A8(M579Zako *self)
{
    Vec rot;
    f32 y;

    y = self->yaw;
    rot.x = 0.0f;
    rot.y = y;
    rot.z = 0.0f;
    fn_1_2F3C(&self->model, &rot);
    fn_1_2EF4(&self->model, &self->pos);
}

void fn_1_16624(M579Zako *self, s32 idx, u8 flag)
{
    s32 attr;
    s16 model;
    s16 mot;

    lbl_1_bss_A14 != 0 ? (void) 0 : fn_1_1D4(lbl_1_data_701, lbl_1_data_6C8, 0x27);
    model = self->model;
    mot = *fn_1_7528(&lbl_1_bss_A14->motion, idx);
    if (self->anim != idx) {
        Hu3DModelAttrReset(model, 0x40000002);
        if (flag == 0) {
            attr = 0x40000001;
        } else {
            attr = 0;
        }
        Hu3DMotionShiftSet(model, mot, 0.0f, 8.0f, attr);
        self->anim = idx;
    }
}

void fn_1_16724(M579Zako *self)
{
    f32 k;

    self->timer = (s32) (fn_1_4AE0() & 0x82);
    k = 2.0f;
    self->roll = (f32) ((f64) (k * fn_1_4D1C()) - (0.5 * (f64) k));
    self->target = 0.5f + fn_1_4D1C();
}

void fn_1_167B0(M579Zako *self)
{
    s16 model;
    s16 mot;

    self->timer = self->timer - 1;
    if (self->timer < 0) {
        fn_1_16724(self);
    }
    self->speed = (f32) (self->speed + (0.05f * (self->target - self->speed)));
    self->yaw = (f32) (self->yaw + self->roll);
    if (self->yaw > 180.0f) {
        self->yaw = (f32) (self->yaw - 360.0f);
    }
    if (self->yaw < -180.0f) {
        self->yaw = (f32) (self->yaw + 360.0f);
    }
    self->speed = 1.0f;
    fn_1_169E0(self);
    Hu3DMotionSpeedSet(self->model, 1.0f);
    lbl_1_bss_A14 != 0 ? (void) 0 : fn_1_1D4(lbl_1_data_701, lbl_1_data_6C8, 0x27);
    model = self->model;
    mot = *fn_1_7528(&lbl_1_bss_A14->motion, 0);
    if (self->anim != 0) {
        Hu3DModelAttrReset(model, 0x40000002);
        Hu3DMotionShiftSet(model, mot, 0.0f, 8.0f, 0x40000001);
        self->anim = 0;
    }
}

s32 fn_1_169E0(M579Zako *self)
{
    Mtx m;
    Vec v;
    Vec out;
    f32 a;

    if (lbl_1_bss_12A8 == 0) {
        lbl_1_bss_12A0 = 60.0;
        lbl_1_bss_12A8 = 1;
    }
    v.x = 0.0f;
    v.y = 0.0f;
    v.z = (f32) ((100.0 * (f64) self->speed) / lbl_1_bss_12A0);
    self->phase = (f32) (self->phase + 0.8333333f);
    if (self->phase > 180.0f) {
        self->phase = (f32) (self->phase - 360.0f);
    }
    a = (f32) ((f64) self->yaw + (45.0 * sin((f32) ((3.141592653589793 * (f64) self->phase) / 180.0))));
    if (a > 180.0f) {
        a -= 360.0f;
    }
    if (a < -180.0f) {
        a += 360.0f;
    }
    PSMTXRotRad(m, 'y', 0.017453292f * a);
    PSMTXMultVec(m, &v, &out);
    self->pos.x = (f32) ((f64) self->pos.x + (100.0 * ((f64) out.x / lbl_1_bss_12A0)));
    self->pos.y = (f32) ((f64) self->pos.y + (100.0 * ((f64) out.y / lbl_1_bss_12A0)));
    self->pos.z = (f32) ((f64) self->pos.z + (100.0 * ((f64) out.z / lbl_1_bss_12A0)));
    if (self->speed <= 0.01f) {
        return 0;
    }
    return 1;
}

s32 fn_1_16C68(M579Zako *self)
{
    f32 x;
    f32 z;
    f32 sq;
    f64 dist;
    f64 r;

    x = self->pos.x;
    z = self->pos.z;
    sq = (f32) (((f64) x * (f64) x) + ((f64) z * (f64) z));
    dist = sqrt((f64) sq);
    if (self->type == 0) {
        if ((f32) dist > 450.0) {
            r = 450.0 / dist;
            self->pos.x = (f32) ((f64) x * r);
            self->pos.z = (f32) ((f64) z * r);
            return 1;
        }
        return 0;
    }
    if ((f32) dist > 1050.0) {
        r = 1050.0 / dist;
        self->pos.x = (f32) ((f64) x * r);
        self->pos.z = (f32) ((f64) z * r);
        return 1;
    }
    if ((f32) dist < 900.0) {
        r = 900.0 / dist;
        self->pos.x = (f32) ((f64) x * r);
        self->pos.z = (f32) ((f64) z * r);
        return 1;
    }
    return 0;
}

void *fn_1_16F3C(M579ItemMgr *self)
{
    M579Ptmf p;

    fn_1_1264(self);
    self->base.vtbl = (void **) lbl_1_data_1FC;
    self->base.flags = 0;
    fn_1_1330(self, lbl_1_bss_C);
    lbl_1_bss_A10 == 0 ? (void) 0 : fn_1_1D4(lbl_1_data_6B8, lbl_1_data_6C8, 0x35);
#ifdef TARGET_PC
    lbl_1_bss_A10 = self;
#else
    {
        u8 *q = (u8 *) self + 0x10;
        if (q != 0) {
            q -= 0x10;
        }
        lbl_1_bss_A10 = (M579ItemMgr *) q;
    }
#endif
    self->base.vtbl = lbl_1_data_75C;
    self->motion.cap = 0;
    self->motion.size = 0;
    self->motion.data = 0;
    fn_1_2C94(&self->model[0], 0x85000A);
    fn_1_2C94(&self->model[1], 0x85000B);
    fn_1_2C94(&self->model[2], 0x850006);
    self->landed = 0;
    self->hidden = 0;
    self->frame = 0;
    self->u62 = 0;
    fn_1_17C8C(self);
    Hu3DModelAttrReset(self->model[0], 1);
    Hu3DModelLayerSet(self->model[0], 3);
    Hu3DModelCameraSet(self->model[0], 1);
    Hu3DModelAttrReset(self->model[1], 1);
    Hu3DModelLayerSet(self->model[1], 3);
    Hu3DModelCameraSet(self->model[1], 1);
    Hu3DModelAttrSet(self->model[2], 1);
    Hu3DModelLayerSet(self->model[2], 0);
    Hu3DModelCameraSet(self->model[2], 1);
    fn_1_2EF4(&self->model[2], (Vec *) &lbl_1_rodata_AB8);
    self->motion.size = 0;
    fn_1_10A7C(&self->motion, 2);
    fn_1_17430(self, 0x85000C);
    fn_1_17430(self, 0x85000D);
    if (self->motion.size == 2) {
    } else {
        fn_1_1D4(lbl_1_data_6D4, lbl_1_data_6F8, 0x160);
    }
    self->anim = -1;
    self->pos.x = 1000.0f;
    self->pos.y = 1000.0f;
    self->pos.z = -1000.0f;
    self->vel.x = 0.0f;
    self->vel.y = 0.0f;
    self->vel.z = 0.0f;
    p = lbl_1_data_720;
    self->update = p;
    self->f5C = -100.0f;
    self->f50 = 0.0f;
    self->f54 = 0.0f;
    self->f58 = 0.0f;
    return self;
}

M579ItemMgr *fn_1_17250(M579ItemMgr *self, s16 flag)
{
    if (self != 0) {
        lbl_1_bss_A10 != 0 ? (void) 0 : fn_1_1D4(lbl_1_data_701, lbl_1_data_6C8, 0x3D);
        lbl_1_bss_A10 = 0;
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M579ItemMgr *fn_1_172DC(M579ItemMgr *self, s16 flag)
{
    if (self != 0) {
        self->base.vtbl = lbl_1_data_75C;
        Hu3DModelKill(self->hook);
        fn_1_30F4(&self->model[2], -1);
        fn_1_30F4(&self->model[1], -1);
        fn_1_30F4(&self->model[0], -1);
        if (&self->motion != 0 && &self->motion != 0 && &self->motion != 0 && self->motion.data != 0) {
            fn_1_C8(self->motion.data);
        }
        if (self != 0) {
            lbl_1_bss_A10 != 0 ? (void) 0 : fn_1_1D4(lbl_1_data_701, lbl_1_data_6C8, 0x3D);
            lbl_1_bss_A10 = 0;
        }
        if (self != 0) {
            self->base.vtbl = (void **) lbl_1_data_1FC;
            if (self != 0) {
                fn_1_1270(self, 0);
            }
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

s16 fn_1_17430(M579ItemMgr *self, s32 dataNum)
{
    s16 mot;
    s16 model;
    s32 n;

    n = dataNum;
    model = self->model[0];
    mot = Hu3DJointMotion(model, HuDataSelHeapReadNum(n, 0x10000000, HEAP_MODEL));
    fn_1_106CC(&self->motion, self->motion.data + self->motion.size, 1, &mot);
    return mot;
}

void fn_1_174C8(M579ItemMgr *self)
{
    Vec p;
    Vec rot;
    void *mem;

#ifdef TARGET_PC
    if (self->update.func != 0) {
        ((void (*)(M579ItemMgr *)) self->update.func)(self);
    }
#else
    __ptmf_scall(self, &self->update);
#endif
    self->frame = self->frame + 1;
    if (self->hidden == 0) {
        fn_1_2F3C(&self->model[0], (Vec *) &lbl_1_rodata_AD0);
        fn_1_2EF4(&self->model[0], &self->pos);
        Hu3DModelObjPosGet(self->model[0], lbl_1_data_72C, &p);
        p.y = p.y + self->f5C;
        fn_1_2F3C(&self->model[1], (Vec *) &lbl_1_rodata_AD0);
        fn_1_2EF4(&self->model[1], &p);
        if (self->landed != 0) {
            self->f58 = (f32) ((f64) self->f58 + 0.05);
            if (self->f58 > 1.0) {
                self->f58 = 1.0f;
            }
            fn_1_3014(&self->model[2], self->f58);
            self->f5C = self->f5C + 13.333333f;
            if (self->f5C > -100.0f) {
                self->f5C = -100.0f;
            }
            self->f50 = (f32) (self->f50 + 6.6666665f);
            if (self->f50 > 180.0f) {
                self->f50 = (f32) (self->f50 - 360.0f);
            }
            self->f54 = (f32) ((f64) self->f54 - 4.5);
            if (self->f54 < -90.0) {
                self->f54 = -90.0f;
            }
            rot.x = self->f54;
            rot.y = self->f50;
            rot.z = 0.0f;
            p.y = p.y - 50.0f;
            fn_1_2F3C(&self->model[2], &rot);
            fn_1_2EF4(&self->model[2], &p);
            Hu3DModelAttrReset(self->model[2], 1);
            if (self->u62 == 0 && p.y > -150.0) {
                self->u62 = 1;
                mem = fn_1_0(sizeof(M579Spray));
                if (mem != 0) {
                    fn_1_BD18(mem, &p, 1);
                }
                fn_1_9668(&self->pos, 0x956);
            }
        }
    }
}

void fn_1_17768(M579ItemMgr *self)
{
    s16 model;
    s16 mot;
    M579Ptmf p;

    self->f5C = self->f5C - 6.6666665f;
    if (self->f5C < -400.0f) {
        self->f5C = -1000.0f;
    }
    PSVECSubtract(&lbl_1_rodata_B18, &self->pos, &self->vel);
    PSVECScale(&self->vel, &self->vel, 0.1f);
    PSVECAdd(&self->pos, &self->vel, &self->pos);
    if ((f32) self->frame > 90.0f) {
        self->frame = 0;
        self->landed = 1;
        lbl_1_bss_A14 != 0 ? (void) 0 : fn_1_1D4(lbl_1_data_701, lbl_1_data_6C8, 0x27);
        model = self->model[2];
        mot = *fn_1_7528(&lbl_1_bss_A14->motion, 0);
        Hu3DModelAttrReset(model, 0x40000002);
        Hu3DMotionShiftSet(model, mot, 0.0f, 8.0f, 0x40000001);
        p = lbl_1_data_738;
        self->update = p;
    }
    fn_1_17BD0(self, 0, 0);
}

void fn_1_17944(M579ItemMgr *self)
{
    M579Ptmf p;

    if ((f32) self->frame > 120.0f) {
        self->frame = 0;
        self->f64 = 0.0f;
        p = lbl_1_data_744;
        self->update = p;
    }
    fn_1_17BD0(self, 1, 1);
}

void fn_1_17A08(M579ItemMgr *self)
{
    f32 a;
    f32 r;

    self->vel.y = (f32) ((f64) self->vel.y + 0.3333333333333333);
    self->f64 = (f32) (self->f64 + 6.6666665f);
    if (self->f64 > 180.0f) {
        self->f64 = (f32) (self->f64 - 360.0f);
    }
    a = (f32) ((3.141592653589793 * (f64) self->f64) / 180.0);
    r = (f32) (self->frame * 5);
    self->pos.x = (f32) ((f64) r * cos((f64) a));
    self->pos.z = (f32) ((f64) r * sin((f64) a));
    PSVECAdd(&self->pos, &self->vel, &self->pos);
}

void fn_1_17B40(M579ItemMgr *self)
{
    s16 a;
    s16 b;
    s16 m;

    a = self->model[0];
    Hu3DModelAttrSet(a, 1);
    b = self->model[1];
    Hu3DModelAttrSet(b, 1);
    m = self->model[2];
    Hu3DModelAttrSet(m, 1);
    Hu3DModelAttrSet(m, 0x40000002);
    self->hidden = 1;
}

void fn_1_17BD0(M579ItemMgr *self, s32 idx, u8 flag)
{
    s32 attr;
    s16 model;
    s16 mot;

    model = self->model[0];
    mot = *fn_1_7528(&self->motion, idx);
    if (self->anim != idx) {
        Hu3DModelAttrReset(model, 0x40000002);
        if (flag == 0) {
            attr = 0x40000001;
        } else {
            attr = 0;
        }
        Hu3DMotionShiftSet(model, mot, 0.0f, 8.0f, attr);
        self->anim = idx;
    }
}

void fn_1_17C8C(M579ItemMgr *self)
{
    s16 m;

    m = Hu3DHookFuncCreate(fn_1_17D64);
    self->hook = m;
    Hu3DModelLayerSet(m, 4);
    Hu3DModelCameraSet(m, 1);
    Hu3DModelPosSet(m, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(m, 0.0f, 0.0f, 0.0f);
    Hu3DData[m].hookData = self;
}

void fn_1_17D64(HU3DMODEL *modelP, Mtx *mtx)
{
    Mtx *m;

    m = mtx;
    fn_1_17DA8((M579ItemMgr *) modelP->hookData);
}

void fn_1_17DA8(M579ItemMgr *self)
{
    Vec p;

    GXSetNumTevStages(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumChans(1);
    GXSetNumTexGens(0);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetCullMode(GX_CULL_NONE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    Hu3DModelObjPosGet(self->model[0], lbl_1_data_72C, &p);
    GXBegin(GX_LINESTRIP, GX_VTXFMT0, 2);
    GXPosition3f32(p.x, p.y, p.z);
    GXColor4u8(0xFF, 0xFF, 0xFF, 0xFF);
    GXPosition3f32(p.x, (f32) (p.y + self->f5C), p.z);
    GXColor4u8(0xFF, 0xFF, 0xFF, 0xFF);
    GXEnd();
}

void fn_1_17FD4(M579ItemMgr *self, s32 bit)
{
    M579Ptmf p;

    self->base.flags = self->base.flags & ~(1 << bit);
    if (bit != 2) {
        if (bit < 2) {
            return;
        }
        return;
    }
    p = lbl_1_data_750;
    self->update = p;
}

void fn_1_18048(M579Node *list, void (*fn)(M579Zako *))
{
    M579Node *l;
    M579Node *first;
    M579Node *n;

    l = list;
    first = l->next;
    n = first;
    while (n != l) {
        fn((M579Zako *) n);
        n = n->next;
    }
}

void fn_1_180A8(M579Node *first, M579Node *list, void (*fn)(M579Zako *))
{
    M579Node *n;

    n = first;
    while (n != list) {
        fn((M579Zako *) n);
        n = n->next;
    }
}

void fn_1_18108(void)
{
    M579Node *a;
    M579Node *b;

    a = &lbl_1_bss_1294;
    fn_1_1264(a);
    b = a;
#ifndef TARGET_PC
    __register_global_object(b, (void *) fn_1_18160, lbl_1_bss_1288);
#endif
}

M579Node *fn_1_18160(M579Node *self, s16 flag)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}
