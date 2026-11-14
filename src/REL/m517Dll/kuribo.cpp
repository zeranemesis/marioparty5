// m517Dll kuribo: the goomba-like walkers. Each is a model plus a task on the
// module list, a motion table (std::vector<s16>) and a small dust object.
#define _MATH_H
#include "REL/m517Dll.h"
extern "C" {
#include "game/frand.h"
}

extern "C" {

#include <string.h>
#ifndef TARGET_PC
void *memmove(void *dst, const void *src, size_t n);
#endif
double sin(double);
double cos(double);
void abort(void);
void *fn_1_1E84(u32 size);
void *fn_1_BDF4(void *self);
void *fn_1_BEDC(void *self, s16 flag);

/* The 0x18 byte dust object owned by a kuribo (its position is at +4). */
typedef struct M517KuriboDust_s {
    void *anim;
    Vec pos;
} M517KuriboDust;

typedef struct M517Kuribo_s {
    M517Model model;
    M517Task task;
    M517Vec16 mot;
    M517KuriboDust *dust;
    s32 motion;
    s32 timer;
    s32 type;
    Vec pos;
    Vec rot;
    s32 state;
    s32 wait;
    s32 dir;
    void **vtbl;
} M517Kuribo;

#define M517_KURIBO_OF(t) ((M517Kuribo *)((char *)(t) - offsetof(M517Kuribo, task)))

M517Kuribo *fn_1_C994(M517Kuribo *self, Vec *pos, Vec *rot, s32 type);
void *fn_1_CBF4(void **self, s16 flag);
M517Vec16 *fn_1_CC4C(M517Vec16 *self, s16 flag);
M517Vec16 *fn_1_CCE0(M517Vec16 *self, s16 flag);
M517Vec16 *fn_1_CD6C(M517Vec16 *self, s16 flag);
M517Kuribo *fn_1_CDF0(M517Kuribo *self, s16 flag);
void fn_1_CF08(void *unused, s16 id);
void fn_1_CFD4(void *unused, s16 id);
void fn_1_D020(M517Kuribo *self, s32 bit);
void fn_1_D038(M517Kuribo *self);
void fn_1_D1FC(M517Kuribo *self);
void fn_1_D31C(M517Kuribo *self);
void fn_1_D658(M517Kuribo *self, s32 dataNum);
void fn_1_D6E4(M517Kuribo *self, s32 idx);
s16 *fn_1_D778(M517Vec16 *v, u32 idx);
void fn_1_D7F0(M517Vec16 *v, s16 *pos, u32 n, s16 *val);
M517Task *fn_1_DBA0(M517Task *t);
void fn_1_DBA8(M517Task *t, s32 bit);
M517Task *fn_1_DBB0(M517Task *t, s16 flag);

Vec lbl_1_data_3F0 = { 1.0f, 4179.73388671875f, 3252.0009765625f };
GXColor lbl_1_data_3FC = { 0xFF, 0xFF, 0xFF, 0xFF };
/* Kuribo vtable: the task vtable starts two words in. */
void *lbl_1_data_400[10] = { 0, 0, 0, 0, (void *)fn_1_DBB0, (void *)fn_1_DBA8, (void *)fn_1_DBA0,
                             (void *)fn_1_CDF0, (void *)fn_1_D020, (void *)fn_1_D31C };
char lbl_1_data_428[] = "vector::at index out of range";
char lbl_1_data_446[] = "vector::insert length error\n";
char lbl_1_data_463[] = "Memory allocation failure\0";

M517Kuribo *fn_1_C994(M517Kuribo *self, Vec *pos, Vec *rot, s32 type)
{
    HU3DMODELID id;
    s16 *mot;
    M517KuriboDust *dust;

    fn_1_474C(&self->model, 0x4B000A);
    fn_1_2DA8(&self->task.link);
    self->task.vtbl = &lbl_1_data_68.vtbl;
    self->task.flags = 0;
    fn_1_2E74(&self->task.link, &lbl_1_bss_24);
    self->vtbl = lbl_1_data_400;
    self->task.vtbl = (M517TaskVtbl *)(lbl_1_data_400 + 2);
    self->mot.cap = 0;
    self->mot.size = 0;
    self->mot.data = 0;
    self->dust = 0;
    self->type = type;
    self->pos = *pos;
    self->rot = *rot;
    self->state = 0;
    id = self->model.id;
    Hu3DModelLayerSet(id, 0);
    Hu3DModelLightInfoSet(id, 0);
    Hu3DModelPosSet(id, pos->x, pos->y, pos->z);
    Hu3DModelRotSet(id, rot->x, rot->y, rot->z);
    fn_1_D658(self, 0x4B000B);
    fn_1_D658(self, 0x4B000C);
    mot = fn_1_D778(&self->mot, 0);
    Hu3DMotionSet(id, *mot);
    fn_1_48C0(&self->model, 0x40000001);
    self->motion = 0;
    self->timer = 0;
    self->wait = (s32)(frand() % 5) * 60;
    switch (self->type) {
        case 0:
            self->dir = frand() & 1;
            break;
        case 1:
            self->dir = 0;
            break;
        case 2:
            self->dir = 1;
            break;
    }
    dust = (M517KuriboDust *)fn_1_1E84(0x18);
    if (dust != 0) {
        dust = (M517KuriboDust *)fn_1_BDF4(dust);
    }
    if (self->dust != dust) {
        fn_1_BEDC(self->dust, 1);
        self->dust = dust;
    }
    return self;
}

// Smart-pointer holder destructor.
void *fn_1_CBF4(void **self, s16 flag)
{
    if (self != 0) {
        fn_1_BEDC(*self, 1);
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

// vector<s16> destructors (three instantiations).
M517Vec16 *fn_1_CC4C(M517Vec16 *self, s16 flag)
{
    if (self != 0) {
        if (self->data != 0) {
            fn_1_1F4C(self->data);
        }
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

M517Vec16 *fn_1_CCE0(M517Vec16 *self, s16 flag)
{
    if (self != 0) {
        if (self->data != 0) {
            fn_1_1F4C(self->data);
        }
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

M517Vec16 *fn_1_CD6C(M517Vec16 *self, s16 flag)
{
    if (self != 0) {
        if (self->data != 0) {
            fn_1_1F4C(self->data);
        }
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

M517Kuribo *fn_1_CDF0(M517Kuribo *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = lbl_1_data_400;
        self->task.vtbl = (M517TaskVtbl *)(lbl_1_data_400 + 2);
        fn_1_BEDC(self->dust, 1);
        if (self->mot.data != 0) {
            fn_1_1F4C(self->mot.data);
        }
        self->task.vtbl = &lbl_1_data_68.vtbl;
        fn_1_2DB4(&self->task.link, 0);
        fn_1_4BAC(&self->model, 0);
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

// Give every object of the model a static directional light.
void fn_1_CF08(void *unused, s16 id)
{
    Vec dir;
    HU3DMODEL *m;
    HSFDATA *hsf;
    s32 i;

    PSVECNormalize(&lbl_1_data_3F0, &dir);
    PSVECScale(&dir, &dir, -1.0f);
    m = &Hu3DData[id];
    hsf = m->hsf;
    if (m->lLightId[0] == -1) {
        for (i = 0; i < hsf->objectNum; i++) {
            HU3DLLIGHTID light = Hu3DLLightCreateV(id, &lbl_1_data_3F0, &dir, &lbl_1_data_3FC);
            Hu3DLLightStaticSet(id, light, 1);
        }
    }
}

void fn_1_CFD4(void *unused, s16 id)
{
    Hu3DModelAmbSet(id, 1.0f, 1.0f, 1.0f);
}

void fn_1_D020(M517Kuribo *self, s32 bit)
{
    self->task.flags &= ~(1u << bit);
}

// Update, type 0: stand, turn for a second, then walk a circle step.
void fn_1_D038(M517Kuribo *self)
{
    f32 ang;
    f32 step;

    switch (self->state) {
        case 0:
            if (self->timer < self->wait) {
                break;
            }
            self->timer = 0;
            fn_1_D6E4(self, 1);
            self->state++;
            // fall through
        case 1:
            if (self->dir != 0) {
                self->rot.y = self->rot.y + 2.9508197f;
            } else {
                self->rot.y = self->rot.y - 2.9508197f;
            }
            if (self->timer < 0x3C) {
                break;
            }
            self->timer = 0;
            self->state++;
            // fall through
        case 2:
            ang = (f32)((3.141592653589793 * self->rot.y) / 180.0);
            step = 1.6666666f;
            self->pos.x = self->pos.x + step * sin(ang);
            self->pos.z = self->pos.z + step * cos(ang);
            if (self->timer < 0xF0) {
                break;
            }
            fn_1_D6E4(self, 0);
            self->wait = (s32)(frand() % 5) * 60;
            self->dir = frand() & 1;
            self->timer = 0;
            self->state = 0;
            break;
    }
}

// Update, types 1 and 2: stand, then turn on the spot.
void fn_1_D1FC(M517Kuribo *self)
{
    switch (self->state) {
        case 0:
            if (self->timer < self->wait) {
                break;
            }
            self->timer = 0;
            fn_1_D6E4(self, 1);
            self->state++;
            // fall through
        case 1:
            if (self->dir == 0) {
                self->rot.y = self->rot.y + 0.73770493f;
            } else {
                self->rot.y = self->rot.y - 0.73770493f;
            }
            if (self->timer < 0x3C) {
                break;
            }
            self->timer = 0;
            self->state++;
            fn_1_D6E4(self, 0);
            self->wait = (s32)(frand() % 5) * 60;
            self->dir = self->dir ^ 1;
            self->timer = 0;
            self->state = 0;
            break;
    }
}

void fn_1_D31C(M517Kuribo *self)
{
    if (self->type != -1) {
        switch (self->type) {
            case 0:
                fn_1_D038(self);
                break;
            case 1:
            case 2:
                fn_1_D1FC(self);
                break;
        }
        fn_1_49AC(&self->model, &self->pos);
        fn_1_49F4(&self->model, &self->rot);
        self->dust->pos.x = self->pos.x;
        self->dust->pos.y = self->pos.y;
        self->dust->pos.z = self->pos.z;
        self->timer++;
    }
}

// Load a motion from the data archive and append it to the motion table.
void fn_1_D658(M517Kuribo *self, s32 dataNum)
{
    HU3DMODELID id = self->model.id;
    void *data = HuDataSelHeapReadNum(dataNum, 0x10000000, HEAP_MODEL);
    s16 mot = Hu3DJointMotion(id, data);

    fn_1_D7F0(&self->mot, self->mot.data + self->mot.size, 1, &mot);
}

// Shift to the motion with table index idx.
void fn_1_D6E4(M517Kuribo *self, s32 idx)
{
    s16 mot = *fn_1_D778(&self->mot, idx);

    if (self->motion != (s32)mot) {
        Hu3DMotionShiftSet(self->model.id, mot, 0.0f, 8.0f, 0x40000001);
        self->motion = idx;
    }
}

s16 *fn_1_D778(M517Vec16 *v, u32 idx)
{
    if (idx >= v->size) {
        fn_1_20B8(M517_STDERR, lbl_1_data_428);
        abort();
    }
    return v->data + idx;
}

// vector<s16>::insert(pos, n, value)
void fn_1_D7F0(M517Vec16 *v, s16 *pos, u32 n, s16 *val)
{
    const u32 max = 0x7FFFFFFF;

    if (n == 0) {
        return;
    }
    if (n > max || v->size > max - n) {
        fn_1_20B8(M517_STDERR, lbl_1_data_446);
        abort();
    }
    if (v->size + n <= v->cap) {
        s16 *end = v->data + v->size;
        s32 after = (s32)(end - pos);
        s16 *src = val;
        s16 *p;
        u32 k;

        if (end - (after - (s32)n) <= src && src < end) {
            src += n;
        }
        if (after != 0) {
            memmove(end + n - after, pos, after * 2);
        }
        for (k = n, p = pos; k != 0; k--) {
            *p++ = *src;
        }
        v->size += n;
    } else {
        s16 *old = v->data;
        s32 idx = (s32)(pos - old);
        u32 newSize = v->size + n;
        u32 cap = (v->cap != 0) ? v->cap : 1;
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
        mem = (s16 *)fn_1_1E84(cap * 2);
        if (mem == 0) {
            fn_1_20B8(M517_STDERR, lbl_1_data_463);
            abort();
        }
        v->data = mem;
        for (k = n, p = mem + idx; k != 0; k--) {
            *p++ = *val;
        }
        if (old != 0) {
            memcpy(mem, old, idx * 2);
            memcpy(mem + idx + n, old + idx, (s32)(v->size - idx) * 2);
            fn_1_1F4C(old);
        }
        v->size = newSize;
        v->cap = cap;
    }
}

// Task vtable thunks (this adjusted from the task subobject to the kuribo).
M517Task *fn_1_DBA0(M517Task *t)
{
    fn_1_D31C(M517_KURIBO_OF(t));
    return t;
}

void fn_1_DBA8(M517Task *t, s32 bit)
{
    fn_1_D020(M517_KURIBO_OF(t), bit);
}

M517Task *fn_1_DBB0(M517Task *t, s16 flag)
{
    return (M517Task *)fn_1_CDF0(M517_KURIBO_OF(t), flag);
}

}
