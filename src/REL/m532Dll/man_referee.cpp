// NONMATCHING: unit is logically complete but not byte-identical (inlined vector/destructor code simplified, register allocation differs); see per-function diffs
#include "REL/m532Dll/man_referee.h"

/*
 * m532 "man_referee": the referee singleton. It owns the referee model with
 * two hand-hook models, a vector of motion ids (see C1D8) and a
 * pointer-to-member state hook driven by the update slot (B8C8). The state
 * handlers pick the next motion, throw the ball along a parabola (BC50/C070)
 * and play the voice effects.
 */

extern "C" {

#ifdef TARGET_PC /* aurora only has the C versions of these */
#define PSVECAdd C_VECAdd
#define PSVECScale C_VECScale
#endif
void PSVECAdd(const M532RefVec3 *a, const M532RefVec3 *b, M532RefVec3 *dst);
void PSVECScale(const M532RefVec3 *src, M532RefVec3 *dst, f32 scale);
f32 PSVECMag(const M532RefVec3 *v);
float sqrtf(float x);

s16 Hu3DJointMotion(s16 model, void *data);
void Hu3DModelHookSet(s16 model, char *objName, s16 hookMdl);
s32 Hu3DMotionEndCheck(s16 model);
f32 Hu3DMotionMotionMaxTimeGet(s16 mot);
s16 Hu3DMotionShiftIDGet(s16 model);
void Hu3DMotionShiftSet(s16 model, s16 mot, f32 start, f32 end, u32 attr);
void Hu3DMotionShiftStartEndSet(s16 model, f32 start, f32 end);
f32 Hu3DMotionShiftTimeGet(s16 model);
f32 Hu3DMotionTimeGet(s16 model);
s32 HuAudFXPlay(s32 seId);
void *HuDataSelHeapReadNum(s32 dataNum, s32 num, s32 heap);

#ifdef TARGET_PC /* fn_1_1D7C ignores the stream */
#define __files ((u8 *)0)
#else
extern u8 __files[];
#endif
void abort(void);
void *memmove(void *dst, const void *src, u32 n);
void *memcpy(void *dst, const void *src, u32 n);

// ---- other units of the module
void fn_1_1D1C(const char *expr, const char *file, s32 line);
void fn_1_1D7C(void *file, const char *msg, ...);
void *fn_1_1B48(u32 size);
void fn_1_1C10(void *ptr);
void fn_1_32B0(M532RefNode *node);
void fn_1_32BC(M532RefNode *node, s32 flag);
void fn_1_337C(M532RefNode *node, M532RefNode *list);
void fn_1_4C54(M532RefModel *m, s32 dataNum);
void fn_1_4DC8(M532RefModel *m, u32 attr);
void fn_1_4EB4(M532RefModel *m, M532RefVec3 *pos);
void fn_1_500C(M532RefModel *m, s32 n);
void fn_1_50B4(M532RefModel *m, s32 flag);
void fn_1_51A4(M532RefModel *m);
s32 fn_1_A6C0(void *self, M532RefVec3 *pos);
void *fn_1_EB84(s32 id);
u8 fn_1_1498(void *obj);
void fn_1_F27C(void);
extern void *lbl_1_bss_8;
extern M532RefNode lbl_1_bss_24;
extern u8 lbl_1_data_B4[];

// ---- local prototypes
void *fn_1_B450(M532Referee *self, s16 flag);
void fn_1_B800(M532Referee *self, s32 ev);
void fn_1_B8C8(M532Referee *self);
void fn_1_B984(void);
void fn_1_B988(M532Referee *self);
void fn_1_BAAC(M532Referee *self);
void fn_1_BB58(M532Referee *self);
void fn_1_BBDC(M532Referee *self);
void fn_1_BC50(M532Referee *self, const M532RefVec3 *target);
void fn_1_C070(M532Referee *self);
void fn_1_C154(M532Referee *self);
void fn_1_C1D8(M532Referee *self);
void fn_1_C2B8(M532Referee *self, s32 dataNum);
void fn_1_C358(M532Referee *self, s32 motion, u8 loop);
u8 fn_1_C46C(M532Referee *self);
f32 fn_1_C4E4(M532Referee *self);
void fn_1_C570(M532Referee *self);
s16 *fn_1_C66C(M532RefVec *v, u32 idx);
void fn_1_C6E4(M532RefVec *v, s16 *pos, u32 n, s16 *val);
s16 *fn_1_CA94(M532RefVec *v, u32 idx);

// ---- data
const M532RefVec3 lbl_1_rodata_2A8 = { 0.0f, 98.0f, 370.0f };
const M532RefVec3 lbl_1_rodata_2B4 = { 0.0f, 4.0f, 500.0f };

M532RefPtmf lbl_1_data_4C8 = M532_REF_PTMF_INIT(fn_1_B984);
char lbl_1_data_4D4[] = "ms_pointer == 0";
char lbl_1_data_4E4[] = "singleton.h";
char lbl_1_data_4F0[] = "itemhook_R";
char lbl_1_data_4FB[] = "itemhook_L";
char lbl_1_data_506[] = "ms_pointer != 0";
M532RefPtmf lbl_1_data_518 = M532_REF_PTMF_INIT(fn_1_C154);
M532RefPtmf lbl_1_data_524 = M532_REF_PTMF_INIT(fn_1_B984);
M532RefPtmf lbl_1_data_530 = M532_REF_PTMF_INIT(fn_1_B984);
M532RefPtmf lbl_1_data_53C = M532_REF_PTMF_INIT(fn_1_BAAC);
M532RefPtmf lbl_1_data_548 = M532_REF_PTMF_INIT(fn_1_BB58);
M532RefPtmf lbl_1_data_554 = M532_REF_PTMF_INIT(fn_1_BBDC);
M532RefPtmf lbl_1_data_560 = M532_REF_PTMF_INIT(fn_1_C070);
char lbl_1_data_56C[] = "m_motion_id.size() == NUMBER_OF_MOTION";
char lbl_1_data_593[] = "man_referee.cpp";
M532RefVtbl lbl_1_data_5A4 = { { 0, 0 },
                               (void *(*)(void *, s16))fn_1_B450,
                               (void (*)(void *, s32))fn_1_B800,
                               (void (*)(void *))fn_1_B8C8 };
char lbl_1_data_5B8[] = "vector::at index out of range";
char lbl_1_data_5D6[] = "vector::insert length error\n";
char lbl_1_data_5F3[] = "Memory allocation failure";

M532Referee *lbl_1_bss_A24;
s32 lbl_1_bss_A50;
s32 lbl_1_bss_A54;

#define M532_SINGLETON_NOT_SET()                                  \
    if (lbl_1_bss_A24 == NULL) {                                  \
    } else {                                                      \
        fn_1_1D1C(lbl_1_data_4D4, lbl_1_data_4E4, 0x35);          \
    }

#define M532_SINGLETON_CLEAR()                                    \
    if (lbl_1_bss_A24 != NULL) {                                  \
    } else {                                                      \
        fn_1_1D1C(lbl_1_data_506, lbl_1_data_4E4, 0x3D);          \
    }                                                             \
    lbl_1_bss_A24 = 0

// Registers the referee in the object list, then builds model, hooks and motions.
M532Referee *fn_1_AFC0(M532Referee *self)
{
    fn_1_32B0(&self->node);
    self->vtbl = lbl_1_data_B4;
    self->flags = 0;
    fn_1_337C(&self->node, &lbl_1_bss_24);
    M532_SINGLETON_NOT_SET();
    lbl_1_bss_A24 = self;
    fn_1_4C54(&self->model, 0x5A000A);
    self->vtbl = &lbl_1_data_5A4;
    self->state = lbl_1_data_4C8;
    fn_1_4C54(&self->hookR, 0x5A0013);
    fn_1_4C54(&self->hookL, 0x5A0014);
    self->ballId = 0;
    self->pos = lbl_1_rodata_2A8;
    self->counter = 6;
    self->mot.cap = 0;
    self->mot.size = 0;
    self->mot.items = 0;
    self->motion = -1;
    self->lastTime = 0.0f;
    fn_1_500C(&self->model, 3);
    fn_1_51A4(&self->model);
    Hu3DModelHookSet(self->model.id, lbl_1_data_4F0, self->hookR.id);
    Hu3DModelHookSet(self->model.id, lbl_1_data_4FB, self->hookL.id);
    self->target.x = 0.0f;
    self->target.y = 0.0f;
    self->target.z = 0.0f;
    self->vel.x = 0.0f;
    self->vel.y = 0.0f;
    self->vel.z = 0.0f;
    fn_1_C1D8(self);
    fn_1_C358(self, 0, 1);
    return self;
}

// The three below are std::vector<s16> destructors (items at +8, deleting form).
void *fn_1_B220(M532RefVec *self, s16 flag)
{
    if (self != NULL) {
        if (self->items != NULL) {
            fn_1_1C10(self->items);
        }
        if (flag > 0) {
            fn_1_1C10(self);
        }
    }
    return self;
}

void *fn_1_B2B4(M532RefVec *self, s16 flag)
{
    if (self != NULL) {
        if (self->items != NULL) {
            fn_1_1C10(self->items);
        }
        if (flag > 0) {
            fn_1_1C10(self);
        }
    }
    return self;
}

void *fn_1_B340(M532RefVec *self, s16 flag)
{
    if (self != NULL) {
        if (self->items != NULL) {
            fn_1_1C10(self->items);
        }
        if (flag > 0) {
            fn_1_1C10(self);
        }
    }
    return self;
}

// Singleton<M532Referee> destructor.
void *fn_1_B3C4(M532Referee *self, s16 flag)
{
    if (self != NULL) {
        M532_SINGLETON_CLEAR();
        if (flag > 0) {
            fn_1_1C10(self);
        }
    }
    return self;
}

void *fn_1_B450(M532Referee *self, s16 flag)
{
    if (self != NULL) {
        self->vtbl = &lbl_1_data_5A4;
        if (self->mot.items != NULL) {
            fn_1_1C10(self->mot.items);
        }
        fn_1_50B4(&self->hookL, -1);
        fn_1_50B4(&self->hookR, -1);
        fn_1_50B4(&self->model, 0);
        M532_SINGLETON_CLEAR();
        self->vtbl = lbl_1_data_B4;
        fn_1_32BC(&self->node, 0);
        if (flag > 0) {
            fn_1_1C10(self);
        }
    }
    return self;
}

s32 fn_1_B59C(M532Referee *self)
{
    s32 r = 0;
    if (self->motion == 3 && fn_1_C4E4(self) == 0.0f) {
        r = 1;
    }
    return r;
}

void fn_1_B600(M532Referee *self)
{
    fn_1_C358(self, 7, 0);
    self->state = lbl_1_data_518;
}

s32 fn_1_B670(M532Referee *self)
{
    s32 r = 1;
    if (fn_1_C46C(self) == 0 && self->motion != 0) {
        r = 0;
    }
    return r;
}

void fn_1_B6CC(M532Referee *self, s32 kind)
{
    switch (kind) {
    case 0:
        fn_1_C358(self, 4, 0);
        HuAudFXPlay(0x7B3);
        break;
    case 1:
        fn_1_C358(self, 5, 0);
        HuAudFXPlay(0x7B3);
        break;
    default:
        fn_1_C358(self, 6, 0);
        HuAudFXPlay(0x7B4);
        break;
    }
    self->state = lbl_1_data_524;
}

f32 fn_1_B7A8(M532Referee *self)
{
    return Hu3DMotionMotionMaxTimeGet(*fn_1_CA94(&self->mot, 7)) / 60.0f;
}

void fn_1_B800(M532Referee *self, s32 ev)
{
    switch (ev) {
    case 0:
        fn_1_B988(self);
        return;
    case 1:
        self->flags = self->flags & ~(1 << ev);
        return;
    case 2:
        self->flags = self->flags & ~(1 << ev);
        return;
    case 3:
        self->flags = self->flags & ~(1 << ev);
        return;
    case 4:
        self->flags = self->flags & ~(1 << ev);
        return;
    }
}

void fn_1_B8C8(M532Referee *self)
{
    M532RefPtmfCall(self, &self->state);
    fn_1_4EB4(&self->model, &self->pos);
    fn_1_C570(self);
}

void fn_1_B914(M532Referee *self)
{
    fn_1_C358(self, 0, 1);
    self->state = lbl_1_data_530;
}

void fn_1_B984(void)
{
}

void fn_1_B988(M532Referee *self)
{
    M532RefVec3 pos;

    pos.x = 0.0f;
    pos.y = (f32)(150.0 + (f64)lbl_1_rodata_2A8.y);
    pos.z = lbl_1_rodata_2A8.z;
    if (lbl_1_bss_8 != NULL) {
    } else {
        fn_1_1D1C(lbl_1_data_506, lbl_1_data_4E4, 0x27);
    }
    self->ballId = fn_1_A6C0(lbl_1_bss_8, &pos);
    self->state = lbl_1_data_53C;
}

void fn_1_BAAC(M532Referee *self)
{
    void *ball = fn_1_EB84(self->ballId);

    if (ball == NULL || fn_1_1498(ball) != 0) {
        fn_1_C358(self, 2, 0);
        HuAudFXPlay(0x7AF);
        fn_1_F27C();
        self->state = lbl_1_data_548;
    }
}

void fn_1_BB58(M532Referee *self)
{
    if (fn_1_C46C(self) != 0) {
        fn_1_C358(self, 0, 1);
        self->state = lbl_1_data_554;
    }
}

void fn_1_BBDC(M532Referee *self)
{
    u8 expired = 0;

    if (self->counter > 0) {
        if (--self->counter == 0) {
            expired = 1;
        }
    }
    if (expired != 0) {
        fn_1_BC50(self, &lbl_1_rodata_2B4);
    }
}

// Starts the ball-toss: velocity needed to reach `target` along a parabola.
void fn_1_BC50(M532Referee *self, const M532RefVec3 *target)
{
    f32 mag, dy, sq, v, vy;

    self->target = *target;
    self->vel.x = target->x - self->pos.x;
    self->vel.y = 0.0f;
    self->vel.z = target->z - self->pos.z;
    mag = PSVECMag(&self->vel);
    dy = target->y - self->pos.y;
    sq = mag * mag;
    v = (0.27240694f * sq) / (2.0f * (mag - dy));
    vy = sqrtf(v);
    PSVECScale(&self->vel, &self->vel, vy / mag);
    self->vel.y = vy;
    fn_1_C358(self, 3, 0);
    self->state = lbl_1_data_560;
}

void fn_1_C070(M532Referee *self)
{
    PSVECAdd(&self->vel, &self->pos, &self->pos);
    self->vel.y = self->vel.y - 0.27240694f;
    if (self->pos.y <= self->target.y) {
        self->pos = self->target;
        if (fn_1_C46C(self) != 0) {
            self->flags = self->flags & ~1u;
            fn_1_C358(self, 0, 1);
            self->state = lbl_1_data_530;
        }
    }
}

void fn_1_C154(M532Referee *self)
{
    if (fn_1_C46C(self) != 0) {
        fn_1_C358(self, 0, 1);
        self->state = lbl_1_data_530;
    }
}

void fn_1_C1D8(M532Referee *self)
{
    self->mot.size = 0;
    fn_1_C2B8(self, 0x5A000B);
    fn_1_C2B8(self, 0x5A000C);
    fn_1_C2B8(self, 0x5A000D);
    fn_1_C2B8(self, 0x5A000E);
    fn_1_C2B8(self, 0x5A000F);
    fn_1_C2B8(self, 0x5A0010);
    fn_1_C2B8(self, 0x5A0011);
    fn_1_C2B8(self, 0x5A0012);
    if (self->mot.size == 8) {
        return;
    }
    fn_1_1D1C(lbl_1_data_56C, lbl_1_data_593, 0xE9);
}

// Appends the joint motion of data file `dataNum` to the motion vector.
void fn_1_C2B8(M532Referee *self, s32 dataNum)
{
    s16 mot;
    void *data = HuDataSelHeapReadNum(dataNum, 0x10000000, 2);

    mot = Hu3DJointMotion(self->model.id, data);
    fn_1_C6E4(&self->mot, self->mot.items + self->mot.size, 1, &mot);
}

void fn_1_C358(M532Referee *self, s32 motion, u8 loop)
{
    s16 model = self->model.id;
    s16 mot = *fn_1_C66C(&self->mot, motion);

    if (self->motion != motion || fn_1_C46C(self) != 0) {
        fn_1_4DC8(&self->model, 0x40000002);
        Hu3DMotionShiftSet(model, mot, 0.0f, 12.0f, loop != 0 ? 0x40000001 : 0);
        self->motion = motion;
        if (motion == 6) {
            Hu3DMotionShiftStartEndSet(model, 0.0f, 14.0f);
        }
    }
}

u8 fn_1_C46C(M532Referee *self)
{
    return (u8)(Hu3DMotionEndCheck(self->model.id) != 0);
}

f32 fn_1_C4E4(M532Referee *self)
{
    s16 model = self->model.id;

    if (Hu3DMotionShiftIDGet(model) >= 0) {
        return Hu3DMotionShiftTimeGet(model) - 1.0f;
    }
    return Hu3DMotionTimeGet(model);
}

// Plays the throw voice when the motion time crosses frame 30.
void fn_1_C570(M532Referee *self)
{
    s16 model = self->model.id;
    f32 t;

    if (Hu3DMotionShiftIDGet(model) >= 0) {
        t = Hu3DMotionShiftTimeGet(model) - 1.0f;
    } else {
        t = Hu3DMotionTimeGet(model);
    }
    if (self->motion == 7 && self->lastTime < 30.0f) {
        if (t >= 30.0f) {
            HuAudFXPlay(0x7B2);
        }
    }
    self->lastTime = t;
}

s16 *fn_1_C66C(M532RefVec *v, u32 idx)
{
    if (idx >= v->size) {
        fn_1_1D7C(__files + 0xA0, lbl_1_data_5B8);
        abort();
    }
    return v->items + idx;
}

// std::vector<s16>::insert(pos, n, val)
void fn_1_C6E4(M532RefVec *v, s16 *pos, u32 n, s16 *val)
{
    const u32 max = 0x7FFFFFFF;

    if (n == 0) {
        return;
    }
    if (n > max || v->size > max - n) {
        fn_1_1D7C(__files + 0xA0, lbl_1_data_5D6);
        abort();
    }
    if (v->size + n <= v->cap) {
        s16 *end = v->items + v->size;
        s32 after = (s32)(end - pos);
        s16 *src = val;
        s16 *p;
        u32 k;

        /* signed: on 64-bit the u32 wrap no longer cancels out */
        if (end - (after - (s32)n) <= src && src < end) {
            src += n;
        }
        if (after != 0) {
            memmove(end + n - after, pos, after * 2);
        }
        for (k = n, p = pos; k != 0; k--, p++) {
            *p = *src;
        }
        v->size = v->size + n;
    } else {
        s16 *old = v->items;
        s32 idx = (s32)(pos - old);
        u32 newSize = v->size + n;
        u32 cap = v->cap != 0 ? v->cap : 1;
        s16 *mem;
        s16 *p;
        u32 k;

        while (newSize > cap) {
            if (cap < (max >> 1)) {
                cap *= 2;
            } else {
                cap = max;
            }
        }
        mem = (s16 *)fn_1_1B48(cap * 2);
        if (mem == NULL) {
            fn_1_1D7C(__files + 0xA0, lbl_1_data_5F3);
            abort();
        }
        v->items = mem;
        for (k = n, p = mem + idx; k != 0; k--, p++) {
            *p = *val;
        }
        if (old != NULL) {
            memcpy(mem, old, idx * 2);
            memcpy(mem + idx + n, old + idx, ((s32)v->size - idx) * 2);
            fn_1_1C10(old);
        }
        v->size = newSize;
        v->cap = cap;
    }
}

s16 *fn_1_CA94(M532RefVec *v, u32 idx)
{
    if (idx >= v->size) {
        fn_1_1D7C(__files + 0xA0, lbl_1_data_5B8);
        abort();
    }
    return v->items + idx;
}

// Static initializer (.ctors entry in the original).
struct M532RefInit {
    M532RefInit()
    {
        lbl_1_bss_A50 = 300;
        lbl_1_bss_A54 = 60;
    }
};
static M532RefInit s_m532RefInit;

}
