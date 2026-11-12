#include "REL/m537Dll.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

#ifdef TARGET_PC
#include <stdio.h>
#define M537_STDERR ((void *)stderr)
#else
extern "C" u8 __files[];
#define M537_STDERR ((void *)(__files + 0xA0))
#endif

extern "C" {

void abort(void);
void *memmove(void *dst, const void *src, size_t n);
void fn_1_14414(M537Water *water, Vec *pos, f32 speed);

void fn_1_6308(M537Dorrie *self);
void fn_1_648C(M537Dorrie *self);
Vec *fn_1_65FC(M537Dorrie *self, s32 part, Vec *out);
void fn_1_6738(M537Dorrie *self);
void fn_1_6808(M537Dorrie *self);
void fn_1_68C0(M537Dorrie *self, s32 dataNum);
void fn_1_694C(M537Dorrie *self, u32 index, u8 flag);
s16 *fn_1_69E0(M537_VECS16 *vec, u32 index);
void fn_1_6A58(M537_CYCLE *cyc, f64 x);
u8 fn_1_6A94(M537_CYCLE *cyc, f64 x);
u8 fn_1_6ADC(M537_CYCLE *cyc, f64 x);
void fn_1_6B28(M537_CYCLE *cyc, f64 from, f64 to, f64 t);
f64 fn_1_6B8C(M537_CYCLE *cyc, f64 x);
f64 fn_1_6C18(M537_CYCLE *cyc);
M537_CYCLE *fn_1_6C20(M537_CYCLE *cyc, f64 a, f64 b, f64 x);
void fn_1_6D0C(M537_VECS16 *vec, u16 *pos, u32 count, s16 *value);
f64 fn_1_70BC(M537_CYCLE *cyc, f64 from, f64 to);

char *lbl_1_data_224[2] = { "stage_a", "spling_in" };

M537Water *lbl_1_bss_9F0;

#ifdef __MWERKS__
#define M537_RODATA __declspec(section ".rodata")
#else
#define M537_RODATA
#endif

M537_RODATA const s32 lbl_1_rodata_80[4] = { 0x5F0003, 0x5F0004, 0x5F0005, 0x5F0006 };

M537Dorrie *fn_1_5E4C(M537Dorrie *self, s32 index)
{
    s16 *shadow;
    M537_VECS16 *vec;
    s32 idx;

    idx = index;
    fn_1_2C94(&self->mdl, lbl_1_rodata_80[idx]);
    fn_1_6C20(&self->cyc, 0.0, 360.0, 0.0);
    self->from = 0.0;
    self->to = 0.0;
    self->t = 0.0;
    self->dur = 0.0;
    self->turn = 0;
    self->count = 0;
    vec = &self->mot;
    vec->cap = 0;
    vec->size = 0;
    vec->data = 0;
    self->timer = 0xAA;
    shadow = (s16 *)fn_1_0(2);
    if (shadow != 0) {
        shadow = fn_1_2C94(shadow, 0x5F0007);
    }
    self->shadow = shadow;
    fn_1_304C(&self->mdl, 5);
    fn_1_6808(self);
    fn_1_304C(self->shadow, 1);
    fn_1_2E08(self->shadow, 0x40000002);
    return self;
}

M537ModelPtr *fn_1_5FB0(M537ModelPtr *self, s16 freeIt)
{
    if (self != 0) {
        fn_1_30F4(self->p, 1);
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

// Deleting destructors of the nested vector holders (data pointer at +8 on GameCube).
void *fn_1_6008(M537_VECS16 *self, s16 freeIt)
{
    s16 **p0;
    s16 **p1;
    s16 *d;

    if (self != 0) {
        if (self != 0) {
            if (self != 0) {
                p0 = &self->data;
                if (*p0 != 0) {
                    p1 = &self->data;
                    d = *p1;
                    fn_1_C8(d);
                }
            }
        }
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void *fn_1_609C(M537_VECS16 *self, s16 freeIt)
{
    s16 **p0;
    s16 **p1;
    s16 *d;

    if (self != 0) {
        if (self != 0) {
            p0 = &self->data;
            if (*p0 != 0) {
                p1 = &self->data;
                d = *p1;
                fn_1_C8(d);
            }
        }
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void *fn_1_6128(M537_VECS16 *self, s16 freeIt)
{
    s16 **p0;
    s16 **p1;
    s16 *d;

    if (self != 0) {
        p0 = &self->data;
        if (*p0 != 0) {
            p1 = &self->data;
            d = *p1;
            fn_1_C8(d);
        }
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M537Dorrie *fn_1_61AC(M537Dorrie *self, s16 freeIt)
{
    s16 **p0;
    s16 **p1;
    s16 *d;

    if (self != 0) {
        if ((u8 *)self + 0x60 != 0) {
            fn_1_30F4(self->shadow, 1);
        }
        if ((u8 *)self + 0x50 != 0) {
            if ((u8 *)self + 0x50 != 0) {
                if ((u8 *)self + 0x50 != 0) {
                    p0 = &self->mot.data;
                    if (*p0 != 0) {
                        p1 = &self->mot.data;
                        d = *p1;
                        fn_1_C8(d);
                    }
                }
            }
        }
        fn_1_30F4(&self->mdl, 0);
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_6278(M537Dorrie *self)
{
    u8 expired;

    expired = 0;
    if (self->timer > 0) {
        self->timer -= 1;
        if (self->timer == 0) {
            expired = 1;
        }
    }
    if (expired != 0) {
        fn_1_3214(&self->mdl);
    }
    fn_1_6308(self);
    fn_1_6738(self);
    fn_1_648C(self);
    self->count = self->count + 1;
}

void fn_1_6308(M537Dorrie *self)
{
    Vec rot;
    f64 e;
    f64 sq;
    f64 sq2;

    if (fn_1_6A94(&self->cyc, self->to) == 0) {
        if (self->dur <= 0.0 || self->t >= self->dur) {
            fn_1_6A58(&self->cyc, self->to);
        } else {
            e = 1.0 - cos(3.141592653589793 * (self->t / self->dur));
            sq = e * e;
            sq2 = sq;
            fn_1_6B28(&self->cyc, self->from, self->to, sq2 * 0.25);
            self->t = self->t + 0.016666666666666666;
        }
        rot.x = 0.0f;
        rot.y = 0.0f;
        rot.z = 0.0f;
        rot.y = (f32)fn_1_6C18(&self->cyc);
        fn_1_2F3C(&self->mdl, &rot);
        fn_1_2F3C(self->shadow, &rot);
    }
}

void fn_1_648C(M537Dorrie *self)
{
    Vec pos;
    M537Water *w0;
    M537Water *w1;

    if (self->count % 300 == 0) {
        fn_1_65FC(self, 1, &pos);
        if (lbl_1_bss_9F0 != 0) {
        } else {
            fn_1_1D4("ms_pointer != 0", "singleton.h", 0x27);
        }
        w0 = lbl_1_bss_9F0;
        w1 = w0;
        fn_1_14414(w1, &pos, -0.2f);
    }
}

MtxPtr fn_1_6558(M537Dorrie *self, s32 part, Mtx mtx)
{
    s16 id;

    switch (part) {
    case 0:
    default:
        id = self->mdl;
        break;
    case 1:
        id = *self->shadow;
        break;
    }
    Hu3DModelObjMtxGet(id, lbl_1_data_224[part], mtx);
    return mtx;
}

Vec *fn_1_65FC(M537Dorrie *self, s32 part, Vec *out)
{
    Vec local = { 0.0f, 0.0f, 0.0f };
    Mtx mtx;
    s16 id;

    switch (part) {
    case 0:
    default:
        id = self->mdl;
        break;
    case 1:
        id = *self->shadow;
        break;
    }
    Hu3DModelObjMtxGet(id, lbl_1_data_224[part], mtx);
    PSMTXMultVec(mtx, &local, out);
    return out;
}

void fn_1_66D0(M537Dorrie *self, f64 target, f64 duration)
{
    self->from = fn_1_6C18(&self->cyc);
    self->to = target;
    self->t = 0.0;
    self->dur = duration;
}

void fn_1_672C(M537Dorrie *self)
{
    self->turn = 1;
}

void fn_1_6738(M537Dorrie *self)
{
    s16 id0;
    s16 *sh0;
    s16 *sh1;
    s16 *sh2;
    s16 id1;
    s16 id2;

    if (self->turn != 0 && fn_1_6ADC(&self->cyc, self->to) == 0) {
        fn_1_694C(self, 0, 0);
        id0 = self->mdl;
        Hu3DMotionOverlaySpeedSet(id0, 0.4f);
        fn_1_304C(&self->mdl, 2);
        sh0 = self->shadow;
        fn_1_2E40(sh0, 0x40000002);
        sh1 = self->shadow;
        sh2 = sh1;
        id1 = *sh2;
        id2 = id1;
        Hu3DMotionSpeedSet(id2, 0.4f);
        self->turn = 0;
    }
}

void fn_1_6808(M537Dorrie *self)
{
    s16 id;

    self->mot.size = 0;
    fn_1_68C0(self, 0x5F0009);
    fn_1_68C0(self, 0x5F0008);
    if (self->mot.size == 2) {
    } else {
        fn_1_1D4("m_motion_id.size() == NUMBER_OF_MOTION", "dorrie.cpp", 0xB3);
    }
    id = self->mdl;
    Hu3DMotionSet(id, *fn_1_69E0(&self->mot, 1));
    fn_1_2E08(&self->mdl, 0x40000001);
}

void fn_1_68C0(M537Dorrie *self, s32 dataNum)
{
    s16 mot;
    s32 num;
    s16 id;

    num = dataNum;
    id = self->mdl;
    mot = Hu3DJointMotion(id, HuDataSelHeapReadNum(num, 0x10000000, HEAP_MODEL));
    fn_1_6D0C(&self->mot, (u16 *)(self->mot.data + self->mot.size), 1, &mot);
}

void fn_1_694C(M537Dorrie *self, u32 index, u8 overlay)
{
    s16 id;

    id = self->mdl;
    Hu3DMotionOverlaySet(id, *fn_1_69E0(&self->mot, index));
    fn_1_2E40(&self->mdl, 0x40000E00);
    if (overlay != 0) {
        fn_1_2E08(&self->mdl, 0x40000200);
    }
}

s16 *fn_1_69E0(M537_VECS16 *vec, u32 index)
{
    if (index >= vec->size) {
        fn_1_234(M537_STDERR, "vector::at index out of range");
        abort();
    }
    return vec->data + index;
}

void fn_1_6A58(M537_CYCLE *cyc, f64 x)
{
    cyc->value = fn_1_6B8C(cyc, x);
}

u8 fn_1_6A94(M537_CYCLE *cyc, f64 x)
{
    return cyc->value == fn_1_6B8C(cyc, x);
}

u8 fn_1_6ADC(M537_CYCLE *cyc, f64 x)
{
    return cyc->value != fn_1_6B8C(cyc, x);
}

void fn_1_6B28(M537_CYCLE *cyc, f64 from, f64 to, f64 t)
{
    cyc->value = fn_1_6B8C(cyc, from + t * fn_1_70BC(cyc, from, to));
}

// Wraps x into [lo, hi).
f64 fn_1_6B8C(M537_CYCLE *cyc, f64 x)
{
    if (x < cyc->lo) {
        return cyc->hi - fmod(cyc->lo - x, cyc->range);
    }
    if (x >= cyc->hi) {
        return cyc->lo + fmod(x - cyc->hi, cyc->range);
    }
    return x;
}

f64 fn_1_6C18(M537_CYCLE *cyc)
{
    return cyc->value;
}

M537_CYCLE *fn_1_6C20(M537_CYCLE *cyc, f64 a, f64 b, f64 x)
{
    f64 lo;
    f64 hi;
    f64 v;

    if (a < b) {
        lo = a;
    } else {
        lo = b;
    }
    cyc->lo = lo;
    if (a > b) {
        hi = a;
    } else {
        hi = b;
    }
    cyc->hi = hi;
    cyc->range = cyc->hi - cyc->lo;
    if (x < cyc->lo) {
        v = cyc->hi - fmod(cyc->lo - x, cyc->range);
    } else if (x >= cyc->hi) {
        v = cyc->lo + fmod(x - cyc->hi, cyc->range);
    } else {
        v = x;
    }
    cyc->value = v;
    return cyc;
}

// std::vector<s16>::insert(pos, count, value). The extra named copies mirror the
// inlined helpers (max_size(), end(), capacity()) the original compiled in.
void fn_1_6D0C(M537_VECS16 *vec, u16 *pos, u32 count, s16 *value)
{
    M537_VECS16 *self0;
    M537_VECS16 *self1;
    M537_VECS16 *self2;
    M537_VECS16 *self3;
    volatile u32 max0;
    volatile u32 max1;
    volatile u32 max2;
    u32 cap0;
    u32 cap1;
    s16 **ref;
    u16 *end;
    s16 *base0;
    s16 *base1;
    s16 *src;
    u16 *dst;
    s16 *oldData;
    s16 *newData;
    s16 *newData1;
    s32 tail;
    s32 tail2;
    s32 n1;
    s32 n2;
    s16 *mid;
    s16 *tailEnd;
    s32 index;
    u32 k;
    u32 newSize;
    u32 newCap;
    u32 cap2;

    if (count != 0) {
        self0 = vec;
        max0 = 0x7FFFFFFF;
        max1 = max0;
        max2 = max1;
        if (count > max2 || vec->size > max2 - count) {
            fn_1_234(M537_STDERR, "vector::insert length error\n");
            abort();
        }
        self1 = vec;
        cap0 = self1->cap;
        cap1 = cap0;
        if (vec->size + count <= cap1) {
            ref = &vec->data;
            end = (u16 *)(*ref + vec->size);
            tail = end - pos;
            src = value;
            if ((u16 *)(end - (tail - (s32)count)) <= (u16 *)src && (u16 *)src < end) {
                src = src + count;
            }
            if (tail != 0) {
                tail2 = end - pos;
                memmove(end + count - tail2, pos, tail2 * 2);
            }
            k = count;
            dst = pos;
            while (k != 0) {
                *dst = *src;
                dst++;
                k--;
            }
            vec->size = vec->size + count;
            return;
        }
        ref = &vec->data;
        base0 = *ref;
        base1 = base0;
        index = (s16 *)pos - base1;
        ref = &vec->data;
        oldData = *ref;
        newSize = vec->size + count;
        self2 = vec;
        if (self2->cap != 0) {
            self3 = vec;
            cap2 = self3->cap;
        } else {
            cap2 = 1;
        }
        newCap = cap2;
        while (newSize > newCap) {
            if (newCap < (max2 >> 1)) {
                newCap *= 2;
            } else {
                newCap = max2;
            }
        }
        ref = &vec->data;
        newData = (s16 *)fn_1_0(newCap * 2);
        if (newData == 0) {
            fn_1_234(M537_STDERR, "Memory allocation failure");
            abort();
        }
        newData1 = newData;
        *ref = newData1;
        k = count;
        ref = &vec->data;
        dst = (u16 *)(*ref + index);
        while (k != 0) {
            *dst = *value;
            dst++;
            k--;
        }
        if (oldData != 0) {
            ref = &vec->data;
            mid = oldData + index;
            n1 = mid - oldData;
            memcpy(*ref, oldData, n1 * 2);
            ref = &vec->data;
            tailEnd = oldData + vec->size;
            n2 = tailEnd - mid;
            memcpy(*ref + index + count, mid, n2 * 2);
            fn_1_C8(oldData);
        }
        vec->size = newSize;
        self0 = vec;
        self0->cap = newCap;
    }
}

// Signed shortest angular distance from 'from' to 'to' on the cycle.
f64 fn_1_70BC(M537_CYCLE *cyc, f64 from, f64 to)
{
    f64 a;
    f64 b;
    f64 d;
    f64 ad;
    f64 r;
    s32 sign;

    if (to < cyc->lo) {
        a = cyc->hi - fmod(cyc->lo - to, cyc->range);
    } else if (to >= cyc->hi) {
        a = cyc->lo + fmod(to - cyc->hi, cyc->range);
    } else {
        a = to;
    }
    if (from < cyc->lo) {
        b = cyc->hi - fmod(cyc->lo - from, cyc->range);
    } else if (from >= cyc->hi) {
        b = cyc->lo + fmod(from - cyc->hi, cyc->range);
    } else {
        b = from;
    }
    d = a - b;
    if (d < 0.0) {
        ad = -d;
    } else {
        ad = d;
    }
    if (ad < cyc->range * 0.5) {
        r = d;
    } else {
        if (d < 0.0) {
            sign = 1;
        } else {
            sign = -1;
        }
        r = (cyc->range - ad) * (f64)sign;
    }
    return r;
}

}
