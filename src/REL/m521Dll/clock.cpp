#include "dolphin/types.h"
#include "dolphin/mtx.h"
#include "game/audio.h"
#include "game/hu3d.h"
#include "REL/m521Dll/m521Types.h"

extern "C" {
double fmod(double x, double y);
double sin(double x);
double cos(double x);

void __dl__FPv(void *ptr);
void *__construct_array(void *ptr, void (*ctor)(void *), void *dtor, u32 size, u32 n);
void __destroy_arr(void *ptr, void *dtor, void *ctor, u32 size, u32 n, s32 flags);
CModel *__ct__6CModelFUi(CModel *self, u32 modelIdx);
void __dt__6CModelFv(CModel *self, s32 freeIt);
CClockHand *__ct__Q24m52110CClockHandFUiPA4_f(CClockHand *self, u32 dataNum, Mtx baseMtx);
void __dt__Q24m52110CClockHandFv(CClockHand *self, s32 freeIt);
void setAngle__Q24m52110CClockHandFf(CClockHand *self, f32 angle);

void *fn_1_4A68(u32 n);
void fn_1_4C3C(const char *cond, const char *file, s32 line);
s32 fn_1_6998(void);
void fn_1_7870(CModel *m, u32 flags);

extern s8 lbl_1_bss_A5C;

s32 lbl_1_bss_0;
s32 lbl_1_bss_4;
s32 lbl_1_bss_8;
s32 lbl_1_bss_C;

#ifdef __MWERKS__
#define CK_RODATA __declspec(section ".rodata")
#else
#define CK_RODATA
#endif

// Guide model data ids of each clock kind (hour, minute).
CK_RODATA const s32 lbl_1_rodata_10[2][2] = { { 0x4F000A, 0x4F000B }, { 0x4F000C, 0x4F000D } };
// Hand model data ids of each clock kind (hour, minute).
CK_RODATA const s32 lbl_1_rodata_20[3][2] = { { 0x4F0005, 0x4F0004 }, { 0x4F0009, 0x4F0008 }, { 0x4F0007, 0x4F0006 } };
// Tick sound ids of each clock kind and hand.
CK_RODATA const s32 lbl_1_rodata_108[2][2] = { { 0x734, 0x736 }, { 0x735, 0x737 } };

typedef struct CModelSlot {
    CModel *m_p;
} CModelSlot;

typedef struct CHandSlot {
    CClockHand *m_p;
} CHandSlot;

s32 fn_1_0(CClock *self);
CClock *fn_1_38(CClock *self, s32 kind, const M521Vec *pos);
CModelSlot *fn_1_4C8(CModelSlot *self, s16 flag);
void fn_1_520(CModelSlot *self);
CHandSlot *fn_1_54C(CHandSlot *self, s16 flag);
void fn_1_5A4(CHandSlot *self);
void fn_1_5D0(s32 *self);
CClock *fn_1_5DC(CClock *self, s16 flag);
void fn_1_658(CClock *self, M521Vec *out, s32 idx);
void fn_1_6B8(CClock *self, M521Vec *out, s32 idx, f32 along, f32 side);
void fn_1_7A0(CClock *self);
s32 fn_1_7E8(CClock *self);
void fn_1_7F8(CClock *self, s32 hour, s32 minute);
void fn_1_894(CClock *self, s32 hour, s32 minute);
void fn_1_924(CClock *self);
f32 fn_1_938(double a, double b);
u32 fn_1_9A8(f32 *ang, f32 target, f32 delta);
f32 fn_1_B88(f32 total);
void fn_1_C4C(CClock *self);
void fn_1_F20(CClock *self);
f32 fn_1_1034(f32 t);
s32 fn_1_1090(M521Vec *outNormal, f32 *outPen, const M521Vec *pt, Mtx inv, f32 len, f32 halfW);
u32 fn_1_1288(const M521Vec *a, const M521Vec *b, Mtx inv, f32 len);
u8 fn_1_1370(CClock *self, s32 idx);
s32 fn_1_139C(CClock *self, s32 idx, f32 delta);
s32 fn_1_1824(CClock *self, M521Vec *pt);
s32 fn_1_1CA0(CClock *self, const M521Vec *pt, M521Vec *outNormal, f32 *outPen);
s32 fn_1_20B8(CClock *self, const M521Vec *a, const M521Vec *b);
s32 fn_1_22BC(CClock *self, s32 arg);
f32 fn_1_231C(f32 a, f32 b);
M521Vec *fn_1_23D8(M521Vec *out, f32 deg);
void fn_1_2480(CClock *self, s32 idx);
s32 fn_1_24D0(CClock *self, f32 angle);
s32 fn_1_2658(CClock *self, s32 idx, s32 dir);
s32 fn_1_2998(CClock *self, f32 deg);
s32 fn_1_2C98(CClock *self, f32 deg);
s32 fn_1_2EFC(CClock *self, s32 idx);
f32 fn_1_3268(CClock *self, s32 idx);
s32 fn_1_35B0(CClock *self);
void fn_1_42A0(CClock *self);
void fn_1_42B4(CClock *self);
s32 fn_1_42C8(CClock *self, const M521Vec *pt, s32 id);
void fn_1_4790(CClock *self, s32 idx);
void fn_1_4844(CClock *self, s32 idx);
void fn_1_48B8(CModelSlot *slot, CModel *p);
void fn_1_48C0(CHandSlot *slot, CClockHand *p);
void fn_1_48C8(void);

s32 fn_1_0(CClock *self)
{
    s32 result;

    result = 0;
    if (self->m_stopped[0] != 0 && self->m_stopped[1] != 0) {
        result = 1;
    }
    return result;
}

CClock *fn_1_38(CClock *self, s32 kind, const M521Vec *pos)
{
    CModel *model0;
    CModel *model1;
    CClockHand *hand0;
    CClockHand *hand1;
    Mtx rot;
    Mtx mtx0;
    Mtx mtx1;

    self->m_kind = kind;
    self->m_pos.x = pos->x;
    self->m_pos.y = pos->y;
    self->m_pos.z = pos->z;
#ifdef TARGET_PC
    {
        s32 i;
        for (i = 0; i < 2; i++) {
            fn_1_5D0(&self->m_timer[i]);
            fn_1_5A4((CHandSlot *)&self->m_hand[i]);
            fn_1_520((CModelSlot *)&self->m_model[i]);
        }
    }
#else
    __construct_array(self->m_timer, (void (*)(void *))fn_1_5D0, 0, sizeof(s32), 2);
    __construct_array(self->m_hand, (void (*)(void *))fn_1_5A4, (void *)fn_1_54C, sizeof(CClockHand *), 2);
    __construct_array(self->m_model, (void (*)(void *))fn_1_520, (void *)fn_1_4C8, sizeof(CModel *), 2);
#endif
#ifdef TARGET_PC
    if (kind >= 0 && kind <= 1) {
#else
    if (kind >= 0 || kind <= 1) {
#endif
        model0 = (CModel *)fn_1_4A68(sizeof(CModel));
        if (model0 != 0) {
            model0 = __ct__6CModelFUi(model0, lbl_1_rodata_10[kind][0]);
        }
        model1 = (CModel *)fn_1_4A68(sizeof(CModel));
        if (model1 != 0) {
            model1 = __ct__6CModelFUi(model1, lbl_1_rodata_10[kind][1]);
        }
        fn_1_7870(model0, 0x40000002);
        fn_1_7870(model1, 0x40000002);
        Hu3DModelShadowMapSet(model0->m_id);
        Hu3DModelShadowMapSet(model1->m_id);
        if (self->m_model[1] != model0) {
            __dt__6CModelFv(self->m_model[1], 1);
            self->m_model[1] = model0;
        }
        if (self->m_model[0] != model1) {
            __dt__6CModelFv(self->m_model[0], 1);
            self->m_model[0] = model1;
        }
    }
    if (kind == 2) {
        PSMTXTrans(mtx0, 0.0f, -8.0f, 8.5f);
        PSMTXTrans(mtx1, 0.0f, 0.0f, 8.5f);
    } else {
        PSMTXTrans(mtx0, 0.0f, -7.235f, 0.0f);
        PSMTXTrans(mtx1, 0.0f, 7.545f, 0.0f);
    }
    if (kind == 2) {
        PSMTXRotRad(rot, 'X', 0.6981317f);
        PSMTXConcat(rot, mtx0, mtx0);
        PSMTXConcat(rot, mtx1, mtx1);
    }
    PSMTXTransApply(mtx0, mtx0, pos->x, pos->y, pos->z);
    PSMTXTransApply(mtx1, mtx1, pos->x, pos->y, pos->z);
    hand0 = (CClockHand *)fn_1_4A68(sizeof(CClockHand));
    if (hand0 != 0) {
        hand0 = __ct__Q24m52110CClockHandFUiPA4_f(hand0, lbl_1_rodata_20[kind][0], mtx0);
    }
    if (self->m_hand[0] != hand0) {
        __dt__Q24m52110CClockHandFv(self->m_hand[0], 1);
        self->m_hand[0] = hand0;
    }
    hand1 = (CClockHand *)fn_1_4A68(sizeof(CClockHand));
    if (hand1 != 0) {
        hand1 = __ct__Q24m52110CClockHandFUiPA4_f(hand1, lbl_1_rodata_20[kind][1], mtx1);
    }
    if (self->m_hand[1] != hand1) {
        __dt__Q24m52110CClockHandFv(self->m_hand[1], 1);
        self->m_hand[1] = hand1;
    }
    fn_1_7F8(self, 6, 0);
    fn_1_894(self, 6, 0);
    fn_1_924(self);
    self->m_speed = 0.0f;
    self->m_phase = 0;
    fn_1_42A0(self);
    self->m_total[0] = 0.0f;
    self->m_total[1] = 0.0f;
    self->m_sfx[0] = -1;
    self->m_sfx[1] = -1;
    self->m_frame = 0;
    self->m_last[0] = -1;
    self->m_last[1] = -1;
    return self;
}

CModelSlot *fn_1_4C8(CModelSlot *self, s16 flag)
{
    if (self != 0) {
        __dt__6CModelFv(self->m_p, 1);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

void fn_1_520(CModelSlot *self)
{
    fn_1_48B8(self, 0);
}

CHandSlot *fn_1_54C(CHandSlot *self, s16 flag)
{
    if (self != 0) {
        __dt__Q24m52110CClockHandFv(self->m_p, 1);
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

void fn_1_5A4(CHandSlot *self)
{
    fn_1_48C0(self, 0);
}

void fn_1_5D0(s32 *self)
{
    *self = 0;
}

CClock *fn_1_5DC(CClock *self, s16 flag)
{
    if (self != 0) {
#ifdef TARGET_PC
        {
            s32 i;
            for (i = 1; i >= 0; i--) {
                fn_1_4C8((CModelSlot *)&self->m_model[i], 0);
                fn_1_54C((CHandSlot *)&self->m_hand[i], 0);
            }
        }
#else
        __destroy_arr(self->m_model, (void *)fn_1_4C8, 0, sizeof(CModel *), 2, 2);
        __destroy_arr(self->m_hand, (void *)fn_1_54C, 0, sizeof(CClockHand *), 2, 2);
#endif
        if (flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

void fn_1_658(CClock *self, M521Vec *out, s32 idx)
{
    CClockHand *hand;
    f32 (*m)[4];

    hand = self->m_hand[idx];
    m = hand->getMtx();
    out->x = m[0][0];
    out->y = m[1][0];
    out->z = m[2][0];
}

void fn_1_6B8(CClock *self, M521Vec *out, s32 idx, f32 along, f32 side)
{
    f32 (*m)[4];

    m = 0;
    switch (idx) {
    case 0:
        along *= -250.0f;
        m = self->m_hand[0]->m_mtx;
        break;
    case 1:
        along *= -330.0f;
        m = self->m_hand[1]->m_mtx;
        break;
    }
    out->x = side;
    out->y = 0.0f;
    out->z = along;
#ifdef TARGET_PC
    if (m == 0) {
        return;
    }
#endif
    PSMTXMultVec(m, out, out);
}

void fn_1_7A0(CClock *self)
{
    self->m_speed = 30.0f;
    self->m_phase = 1;
    fn_1_42B4(self);
}

s32 fn_1_7E8(CClock *self)
{
    return self->m_phase == 0;
}

void fn_1_7F8(CClock *self, s32 hour, s32 minute)
{
    self->m_time = (f32)(minute + hour * 0x3C);
    self->m_ang[0] = (-360.0f * self->m_time) / 720.0f;
    self->m_ang[1] = (-360.0f * self->m_time) / 60.0f;
    self->m_cnt[0] = 0;
    self->m_cnt[1] = 0;
}

void fn_1_894(CClock *self, s32 hour, s32 minute)
{
    f32 t;

    t = (f32)(minute + hour * 0x3C);
    self->m_target[0] = (-360.0f * t) / 720.0f;
    self->m_target[1] = (-360.0f * t) / 60.0f;
}

void fn_1_924(CClock *self)
{
    self->m_goal[0] = self->m_target[0];
    self->m_goal[1] = self->m_target[1];
}

f32 fn_1_938(double a, double b)
{
    f32 r;

    r = (f32)fmod(a, b);
    if (r < 0.0f) {
        r += (f32)b;
    }
    return r;
}

// Advances *ang by delta (wrapped to 0..360); snaps to target and returns 1 when it gets there.
u32 fn_1_9A8(f32 *ang, f32 target, f32 delta)
{
    f32 next;
    f32 tgt;
    f32 d;
    f32 step;
    u8 reached;

    next = fn_1_938((f64)(*ang + delta), 360.0);
    tgt = fn_1_938(target, 360.0);
    if (next - tgt < 0.0f) {
        d = -(next - tgt);
    } else {
        d = next - tgt;
    }
    if (delta < 0.0f) {
        step = -delta;
    } else {
        step = delta;
    }
    reached = d < step;
    if (reached != 0) {
        *ang = tgt;
    } else {
        *ang = next;
    }
    return reached;
}

f32 fn_1_B88(f32 total)
{
    f32 v;

    v = fn_1_938((f64)(2.0f * total), 720.0);
    return 59.0f * (v / 720.0f);
}

void fn_1_C4C(CClock *self)
{
    CModel *m1;
    CModel *m0;
    u8 expired0;
    u8 expired1;

    fn_1_F20(self);
    setAngle__Q24m52110CClockHandFf(self->m_hand[1], self->m_ang[1]);
    setAngle__Q24m52110CClockHandFf(self->m_hand[0], self->m_ang[0]);
    m1 = self->m_model[1];
    if (m1 != 0) {
        Hu3DMotionTimeSet(m1->m_id, fn_1_B88(self->m_total[1]));
    }
    m0 = self->m_model[0];
    if (m0 != 0) {
        Hu3DMotionTimeSet(m0->m_id, fn_1_B88(self->m_total[0]));
    }
    expired0 = 0;
    if (self->m_timer[0] > 0) {
        if (--self->m_timer[0] == 0) {
            expired0 = 1;
        }
    }
    if (expired0 != 0) {
        fn_1_4844(self, 0);
    }
    expired1 = 0;
    if (self->m_timer[1] > 0) {
        if (--self->m_timer[1] == 0) {
            expired1 = 1;
        }
    }
    if (expired1 != 0) {
        fn_1_4844(self, 1);
    }
    self->m_cnt[0]--;
    self->m_cnt[1]--;
    self->m_frame++;
}

void fn_1_F20(CClock *self)
{
    u8 done;

    switch (self->m_phase) {
    case 1:
        self->m_ang[0] = self->m_ang[0] - self->m_speed;
        self->m_ang[1] = self->m_ang[1] - self->m_speed;
        self->m_speed = (f32)((f64)self->m_speed - 0.16666666666666666);
        if (self->m_speed < 10.0f) {
            self->m_phase++;
            return;
        }
        return;
    case 2:
        fn_1_139C(self, 0, -self->m_speed);
        fn_1_139C(self, 1, -self->m_speed);
        done = 0;
        if (self->m_stopped[0] != 0 && self->m_stopped[1] != 0) {
            done = 1;
        }
        if (done != 0) {
            self->m_phase = 0;
        }
        break;
    }
}

f32 fn_1_1034(f32 t)
{
    return (f32)(0.2 + (0.8f * sin(3.141592653589793 * (f64)t)));
}

// Tests a point against one hand (a thin diamond of length len and half width halfW in hand space).
s32 fn_1_1090(M521Vec *outNormal, f32 *outPen, const M521Vec *pt, Mtx inv, f32 len, f32 halfW)
{
    M521Vec local;
    f32 width;
    f32 x;
    f32 absX;
    f32 limit;

    PSMTXMultVec(inv, pt, &local);
    if (local.z > 0.0f || local.z < -len) {
        return 0;
    }
    if (local.y > 30.000002f) {
        return 0;
    }
    width = halfW * (f32)(0.2 + (0.8f * sin(3.141592653589793 * (f64)(-local.z / len))));
    x = local.x;
    if (x < 0.0f) {
        absX = -x;
    } else {
        absX = x;
    }
    if (absX > width) {
        return 0;
    }
    if (local.x < 0.0f) {
        limit = -width;
    } else {
        limit = width;
    }
    *outPen = limit - local.x;
    outNormal->x = inv[0][0];
    outNormal->y = inv[0][1];
    outNormal->z = inv[0][2];
    return 1;
}

u32 fn_1_1288(const M521Vec *a, const M521Vec *b, Mtx inv, f32 len)
{
    M521Vec la;
    M521Vec lb;
    u8 cross;

    PSMTXMultVec(inv, a, &la);
    PSMTXMultVec(inv, b, &lb);
    if (la.z > 0.0f || la.z < -len) {
        return 0;
    }
    if (lb.z > 0.0f || lb.z < -len) {
        return 0;
    }
    cross = lb.x * la.x < 0.0f;
    return cross;
}

u8 fn_1_1370(CClock *self, s32 idx)
{
    if (idx < 2) {
        if (idx < 0) {
            return 0;
        }
        return self->m_stopped[idx];
    }
    return 0;
}

s32 fn_1_139C(CClock *self, s32 idx, f32 delta)
{
    f32 old;
    s32 result;
    u8 valid;
    f32 next;
    u8 reached;
    s32 side;

    valid = 0;
    if (idx >= 0 && idx < 2) {
        valid = 1;
    }
    if (valid != 0) {
    } else {
        fn_1_4C3C("id >= 0 && id < 2", "clock.cpp", 0x115);
    }
    result = -1;
    old = self->m_ang[idx];
    if (self->m_stopped[idx] == 0) {
        if (self->m_kind == 2 || lbl_1_bss_A5C != 0) {
            reached = fn_1_9A8(&self->m_ang[idx], self->m_target[idx], delta);
            self->m_stopped[idx] = reached;
        } else {
            next = fn_1_938((f64)(self->m_ang[idx] + delta), 360.0);
            self->m_ang[idx] = next;
        }
        if (self->m_stopped[idx] != 0) {
            switch (self->m_kind) {
            case 0:
                fn_1_4844(self, 0);
                HuAudFXPlay(0x738);
                break;
            case 1:
                fn_1_4844(self, 1);
                HuAudFXPlay(0x739);
                break;
            }
        } else {
            fn_1_4790(self, idx);
        }
        if (self->m_stopped[idx] != 0) {
            side = 1;
        } else {
            side = -1;
        }
        result = side;
    } else if (self->m_last[idx] != self->m_frame) {
        self->m_last[idx] = self->m_frame;
        if (self->m_cnt[idx] < 0) {
            self->m_cnt[idx] = 0;
        }
        self->m_cnt[idx] = self->m_cnt[idx] + 2;
        if (self->m_cnt[idx] > 0x28) {
            self->m_cnt[idx] = 0;
            self->m_stopped[idx] = 0;
            self->m_ang[idx] = self->m_ang[idx] + (2.0f * delta);
        }
    }
    self->m_total[idx] = self->m_total[idx] + (self->m_ang[idx] - old);
    return result;
}

// Pushes the point out of both hands.
s32 fn_1_1824(CClock *self, M521Vec *pt)
{
    CClockHand *h0;
    Mtx inv1;
    Mtx inv0;
    M521Vec normal;
    M521Vec delta;
    f32 pen;
    s32 i;

    h0 = self->m_hand[0];
    PSMTXInverse(self->m_hand[1]->m_mtx, inv1);
    PSMTXInverse(h0->m_mtx, inv0);
    for (i = 0; i < 10; i++) {
        if (fn_1_1090(&normal, &pen, pt, inv1, 330.0f, 70.0f) != 0) {
            PSVECScale(&normal, &delta, pen);
            PSVECAdd(&delta, pt, pt);
        }
        if (fn_1_1090(&normal, &pen, pt, inv0, 250.0f, 80.0f) != 0) {
            PSVECScale(&normal, &delta, pen);
            PSVECAdd(&delta, pt, pt);
        }
    }
    return 0;
}

// Which hand (0 / 1) the point is touching, or -1.
s32 fn_1_1CA0(CClock *self, const M521Vec *pt, M521Vec *outNormal, f32 *outPen)
{
    CClockHand *h0;
    Mtx inv1;
    Mtx inv0;

    h0 = self->m_hand[0];
    PSMTXInverse(self->m_hand[1]->m_mtx, inv1);
    PSMTXInverse(h0->m_mtx, inv0);
    if (fn_1_1090(outNormal, outPen, pt, inv0, 250.0f, 80.0f) != 0) {
        return 0;
    }
    if (fn_1_1090(outNormal, outPen, pt, inv1, 330.0f, 70.0f) != 0) {
        return 1;
    }
    return -1;
}

// Which hand (0 / 1) the segment a-b crosses, or -1.
s32 fn_1_20B8(CClock *self, const M521Vec *a, const M521Vec *b)
{
    CClockHand *h0;
    Mtx inv1;
    Mtx inv0;

    h0 = self->m_hand[0];
    PSMTXInverse(self->m_hand[1]->m_mtx, inv1);
    PSMTXInverse(h0->m_mtx, inv0);
    if (fn_1_1288(a, b, inv0, 250.0f) != 0) {
        return 0;
    }
    if (fn_1_1288(a, b, inv1, 330.0f) != 0) {
        return 1;
    }
    return -1;
}

s32 fn_1_22BC(CClock *self, s32 arg)
{
    if (arg == 0) {
        if (self->m_stopped[0] == 0) {
            return 0;
        }
        if (self->m_stopped[1] == 0) {
            return 1;
        }
        return -1;
    }
    if (self->m_stopped[1] == 0) {
        return 1;
    }
    if (self->m_stopped[0] == 0) {
        return 0;
    }
    return -1;
}

// Signed angle (degrees) from b to a, wrapped to the 16 bit angle range.
f32 fn_1_231C(f32 a, f32 b)
{
    s32 ia;
    s32 ib;
    u16 wrapped;

    ia = (s32)(182.04444444444445 * a);
    ib = (s32)(182.04444444444445 * b);
    wrapped = (u16)((ia - ib) - 0x8000);
    return (f32)(0.0054931640625 * (f64)((s32)wrapped - 0x8000));
}

M521Vec *fn_1_23D8(M521Vec *out, f32 deg)
{
    out->x = (f32)cos((3.141592653589793 * deg) / 180.0);
    out->y = (f32)sin((3.141592653589793 * deg) / 180.0);
    out->z = 0.0f;
    return out;
}

void fn_1_2480(CClock *self, s32 idx)
{
    fn_1_2658(self, idx, fn_1_2EFC(self, idx));
}

s32 fn_1_24D0(CClock *self, f32 angle)
{
    M521Vec v0;
    M521Vec v1;
    f32 a;
    f32 b;
    f32 dot;

    a = self->m_ang[0];
    v0.x = (f32)cos((3.141592653589793 * (f64)a) / 180.0);
    v0.y = (f32)sin((3.141592653589793 * (f64)a) / 180.0);
    v0.z = 0.0f;
    b = self->m_ang[1];
    v1.x = (f32)cos((3.141592653589793 * (f64)b) / 180.0);
    v1.y = (f32)sin((3.141592653589793 * (f64)b) / 180.0);
    v1.z = 0.0f;
    dot = PSVECDotProduct(&v0, &v1);
    return dot > cos((3.141592653589793 * (f64)(angle * 0.5f)) / 180.0);
}

s32 fn_1_2658(CClock *self, s32 idx, s32 dir)
{
    M521Vec other;
    M521Vec cur;
    f32 a;
    f32 b;
    f32 c;
    f32 d;
    f32 base;
    s32 result;

    a = self->m_ang[idx ^ 1];
    other.x = (f32)cos((3.141592653589793 * (f64)a) / 180.0);
    other.y = (f32)sin((3.141592653589793 * (f64)a) / 180.0);
    other.z = 0.0f;
    b = self->m_ang[idx];
    cur.x = (f32)cos((3.141592653589793 * (f64)b) / 180.0);
    cur.y = (f32)sin((3.141592653589793 * (f64)b) / 180.0);
    cur.z = 0.0f;
    base = PSVECDotProduct(&cur, &other);
    c = self->m_ang[idx ^ 1];
    other.x = (f32)cos((3.141592653589793 * (f64)c) / 180.0);
    other.y = (f32)sin((3.141592653589793 * (f64)c) / 180.0);
    other.z = 0.0f;
    d = self->m_ang[idx] + (f32)dir;
    cur.x = (f32)cos((3.141592653589793 * (f64)d) / 180.0);
    cur.y = (f32)sin((3.141592653589793 * (f64)d) / 180.0);
    cur.z = 0.0f;
    result = 0;
    if (PSVECDotProduct(&cur, &other) > base && base > cos(0.7853981633974483)) {
        result = 1;
    }
    return result;
}

s32 fn_1_2998(CClock *self, f32 deg)
{
    M521Vec t0;
    M521Vec t1;
    M521Vec g0;
    M521Vec g1;
    f32 cosv;
    f32 a;
    f32 b;
    f32 c;
    f32 d;
    s32 result;
    u8 near;

    cosv = (f32)cos((3.141592653589793 * (f64)deg) / 180.0);
    a = self->m_target[0];
    t0.x = (f32)cos((3.141592653589793 * (f64)a) / 180.0);
    t0.y = (f32)sin((3.141592653589793 * (f64)a) / 180.0);
    t0.z = 0.0f;
    b = self->m_target[1];
    t1.x = (f32)cos((3.141592653589793 * (f64)b) / 180.0);
    t1.y = (f32)sin((3.141592653589793 * (f64)b) / 180.0);
    t1.z = 0.0f;
    c = self->m_goal[0];
    g0.x = (f32)cos((3.141592653589793 * (f64)c) / 180.0);
    g0.y = (f32)sin((3.141592653589793 * (f64)c) / 180.0);
    g0.z = 0.0f;
    d = self->m_goal[1];
    g1.x = (f32)cos((3.141592653589793 * (f64)d) / 180.0);
    g1.y = (f32)sin((3.141592653589793 * (f64)d) / 180.0);
    g1.z = 0.0f;
    result = 1;
    near = 1;
    if (!(PSVECDotProduct(&t0, &t1) > cosv) && !(PSVECDotProduct(&t0, &g0) > cosv)) {
        near = 0;
    }
    if (near == 0 && !(PSVECDotProduct(&t1, &g1) > cosv)) {
        result = 0;
    }
    return result;
}

s32 fn_1_2C98(CClock *self, f32 deg)
{
    M521Vec t0;
    M521Vec t1;
    M521Vec cur;
    f32 cosv;
    f32 a;
    f32 b;
    f32 c;
    s32 i;

    cosv = (f32)cos((3.141592653589793 * (f64)deg) / 180.0);
    a = self->m_target[0];
    t0.x = (f32)cos((3.141592653589793 * (f64)a) / 180.0);
    t0.y = (f32)sin((3.141592653589793 * (f64)a) / 180.0);
    t0.z = 0.0f;
    b = self->m_target[1];
    t1.x = (f32)cos((3.141592653589793 * (f64)b) / 180.0);
    t1.y = (f32)sin((3.141592653589793 * (f64)b) / 180.0);
    t1.z = 0.0f;
    for (i = 0; i < 2; i++) {
        c = self->m_ang[i];
        cur.x = (f32)cos((3.141592653589793 * (f64)c) / 180.0);
        cur.y = (f32)sin((3.141592653589793 * (f64)c) / 180.0);
        cur.z = 0.0f;
        if (PSVECDotProduct(&t0, &cur) > cosv) {
            return 1;
        }
        if (PSVECDotProduct(&t1, &cur) > cosv) {
            return 1;
        }
    }
    return 0;
}

// Direction (-1, 0, 1) hand idx has to turn to reach its target without crossing the other hand.
s32 fn_1_2EFC(CClock *self, s32 idx)
{
    f32 toTarget;
    f32 toOther;
    f32 a;
    f32 b;
    f32 sign;

    toTarget = fn_1_231C(self->m_target[idx], self->m_ang[idx]);
    toOther = fn_1_231C(self->m_ang[idx ^ 1], self->m_ang[idx]);
    if (toTarget * toOther >= 0.0f) {
        if (toTarget < 0.0f) {
            a = -toTarget;
        } else {
            a = toTarget;
        }
        if (toOther < 0.0f) {
            b = -toOther;
        } else {
            b = toOther;
        }
        if (a > b) {
            toTarget *= -1.0f;
        }
    }
    if (toTarget == 0.0f) {
        sign = 0.0f;
    } else {
        if (toTarget < 0.0f) {
            sign = -1.0f;
        } else {
            sign = 1.0f;
        }
    }
    return (s32)sign;
}

// Angular distance hand idx still has to travel to its target.
f32 fn_1_3268(CClock *self, s32 idx)
{
    f32 toTarget;
    f32 toOther;
    f32 a;
    f32 b;
    f32 c;
    f32 d;

    toTarget = fn_1_231C(self->m_target[idx], self->m_ang[idx]);
    toOther = fn_1_231C(self->m_ang[idx ^ 1], self->m_ang[idx]);
    if (!(toTarget * toOther < 0.0f)) {
        if (toTarget < 0.0f) {
            a = -toTarget;
        } else {
            a = toTarget;
        }
        if (toOther < 0.0f) {
            b = -toOther;
        } else {
            b = toOther;
        }
        if (a < b) {
            goto shortWay;
        }
        if (toTarget < 0.0f) {
            c = -toTarget;
        } else {
            c = toTarget;
        }
        return 360.0f - c;
    }
shortWay:
    if (toTarget < 0.0f) {
        d = -toTarget;
    } else {
        d = toTarget;
    }
    return d;
}

s32 fn_1_35B0(CClock *self)
{
    s32 result;

    if (self->m_stopped[1] != 0 && self->m_stopped[0] != 0) {
        return -1;
    }
    if (self->m_stopped[0] == 0 && self->m_stopped[1] != 0) {
        return 0;
    }
    if (self->m_stopped[1] == 0 && self->m_stopped[0] != 0) {
        return 1;
    }
    if (fn_1_3268(self, 0) + fn_1_3268(self, 1) <= fn_1_3268(self, 1) + fn_1_3268(self, 0)) {
        result = 0;
    } else {
        result = 1;
    }
    return result;
}

void fn_1_42A0(CClock *self)
{
    self->m_stopped[0] = 1;
    self->m_stopped[1] = 1;
}

void fn_1_42B4(CClock *self)
{
    self->m_stopped[0] = 0;
    self->m_stopped[1] = 0;
}

// Is the point on the side of hand id that the hand is turning away from?
s32 fn_1_42C8(CClock *self, const M521Vec *pt, s32 id)
{
    Mtx inv;
    M521Vec local;
    u8 valid;
    f32 toTarget;
    f32 toOther;
    f32 a;
    f32 b;
    f32 absX;
    f32 sign;

    valid = 0;
    if (id >= 0 && id < 2) {
        valid = 1;
    }
    if (valid != 0) {
    } else {
        fn_1_4C3C("id >= 0 && id < 2", "clock.cpp", 0x217);
    }
    PSMTXInverse(self->m_hand[id]->m_mtx, inv);
    PSMTXMultVec(inv, pt, &local);
    if (local.z > 0.0f) {
        return 0;
    }
    if (local.x < 0.0f) {
        absX = -local.x;
    } else {
        absX = local.x;
    }
    if (absX > 220.00000000000003) {
        return 0;
    }
    toTarget = fn_1_231C(self->m_target[id], self->m_ang[id]);
    toOther = fn_1_231C(self->m_ang[id ^ 1], self->m_ang[id]);
    if (toTarget * toOther >= 0.0f) {
        if (toTarget < 0.0f) {
            a = -toTarget;
        } else {
            a = toTarget;
        }
        if (toOther < 0.0f) {
            b = -toOther;
        } else {
            b = toOther;
        }
        if (a > b) {
            toTarget *= -1.0f;
        }
    }
    if (toTarget == 0.0f) {
        sign = 0.0f;
    } else {
        if (toTarget < 0.0f) {
            sign = -1.0f;
        } else {
            sign = 1.0f;
        }
    }
    if (local.x * (f32)(s32)sign < 0.0f) {
        return 0;
    }
    return 1;
}

void fn_1_4790(CClock *self, s32 idx)
{
    if (self->m_kind < 2 && fn_1_6998() == 2) {
        if (self->m_sfx[idx] != -1) {
            self->m_timer[idx] = 5;
            return;
        }
        self->m_sfx[idx] = HuAudFXPlay(lbl_1_rodata_108[self->m_kind][idx]);
        self->m_timer[idx] = 5;
    }
}

void fn_1_4844(CClock *self, s32 idx)
{
    if (self->m_kind < 2 && self->m_sfx[idx] != -1) {
        HuAudFXStop(self->m_sfx[idx]);
        self->m_sfx[idx] = -1;
    }
}

void fn_1_48B8(CModelSlot *slot, CModel *p)
{
    slot->m_p = p;
}

void fn_1_48C0(CHandSlot *slot, CClockHand *p)
{
    slot->m_p = p;
}

void fn_1_48C8(void)
{
    lbl_1_bss_0 = 0x78;
    lbl_1_bss_4 = 0xD2;
    lbl_1_bss_8 = 0xD2;
}
}
