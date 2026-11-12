#include "dolphin.h"
#include <math.h>
#include "game/hu3d.h"
#include "REL/m579Dll/m579types.h"

extern "C" {
void *__register_global_object(void *object, void *destructor, void *regmem);
void __ptmf_scall(void *self, void *ptmf);
void fn_1_BD18(void *mem, Vec *pos, s32 type);
void fn_1_A084(void *mem, Vec *pos);
extern M579MotionMgr *lbl_1_bss_A14;
}
#ifndef TARGET_PC
extern "C" u8 __files[];
#endif
#ifdef TARGET_PC
#include <stdio.h>
#endif

extern "C" {
M579Puku *fn_1_6074(M579Puku *self, s32 dataNum, s32 type);
M579Puku *fn_1_625C(M579Puku *self, s16 flag);
M579Puku *fn_1_62B4(M579Fish *owner, s32 type);
void fn_1_6388(void);
void fn_1_645C(void);
void fn_1_64E0(M579Puku *self);
void fn_1_6534(M579Puku *self);
void fn_1_660C(M579Puku *self);
void fn_1_682C(M579Puku *self, s32 idx, u8 flag);
void fn_1_692C(M579Puku *self);
void fn_1_6990(M579Puku *self);
void fn_1_6AD4(M579Puku *self);
void fn_1_7324(M579Puku *self, u8 flag);
void fn_1_7468(M579Node *list, void (*fn)(M579Puku *));
void fn_1_74C8(M579Node *first, M579Node *list, void (*fn)(M579Puku *));
s16 *fn_1_7528(M579S16Vec *vec, u32 idx);
void fn_1_75A0(void);
M579Puku *fn_1_75F8(M579Puku *self, s16 flag);
}

M579Ptmf lbl_1_data_210 = { 0, -1, (void *) fn_1_692C };
M579Ptmf lbl_1_data_21C = { 0, -1, (void *) fn_1_6990 };
char lbl_1_data_228[16] = "ms_pointer != 0";
char lbl_1_data_238[12] = "singleton.h";
M579Ptmf lbl_1_data_244 = { 0, -1, (void *) fn_1_692C };
M579Ptmf lbl_1_data_250 = { 0, -1, (void *) fn_1_6AD4 };
char lbl_1_data_25C[36] = "vector::at index out of range";

M579_RODATA const Vec lbl_1_rodata_C0[5] = {
    { 0.0f, 0.0f, -1000.0f }, { 0.0f, 0.0f, 1000.0f }, { 1000.0f, 0.0f, 0.0f },
    { -1000.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }
};
M579_RODATA const Vec lbl_1_rodata_FC = { 0.0f, 0.0f, 0.0f };
M579_RODATA const Vec lbl_1_rodata_160 = { 0.0f, 0.0f, 0.0f };

u8 lbl_1_bss_9F0[12];
M579Node lbl_1_bss_9FC;

M579Puku *fn_1_6074(M579Puku *self, s32 dataNum, s32 type)
{
    f64 scale;
    M579Ptmf p;

    fn_1_1264(self);
    self->anim = -1;
    self->u18 = 0;
    self->frame = 0;
    fn_1_2DB4(&self->model, dataNum);
    self->busy = 0;
    self->type = type;
    self->state = 0;
    fn_1_1330(self, &lbl_1_bss_9FC);
    Hu3DModelCameraSet(self->model, 1);
    if (type != 0) {
        scale = 1.5;
    } else {
        scale = 1.0;
    }
    fn_1_2FCC(&self->model, (f32) scale);
    fn_1_2E08(&self->model, 1);
    fn_1_304C(&self->model, 0);
    fn_1_31E4(&self->model);
    self->pos.x = 0.0f;
    self->pos.y = 0.0f;
    self->pos.z = 0.0f;
    self->prev.x = 0.0f;
    self->prev.y = 0.0f;
    self->prev.z = 0.0f;
    self->rot.x = 0.0f;
    self->rot.y = 0.0f;
    self->rot.z = 0.0f;
    fn_1_2EF4(&self->model, &self->pos);
    p = lbl_1_data_210;
    self->update = p;
    return self;
}

M579Puku *fn_1_625C(M579Puku *self, s16 flag)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M579Puku *fn_1_62B4(M579Fish *owner, s32 type)
{
    M579Node *n;
    M579Puku *p;
    M579Ptmf t;
    f32 maxDist;

    n = lbl_1_bss_9FC.next;
    maxDist = 2000.0f;
    while (n != &lbl_1_bss_9FC) {
        p = (M579Puku *) n;
        if (p->busy == 0 && type == p->type) {
            p->busy = 1;
            p->owner = owner;
            t = lbl_1_data_21C;
            p->update = t;
            return p;
        }
        n = n->next;
    }
    return 0;
}

void fn_1_6388(void)
{
    M579Puku *p;
    u8 empty;

    for (;;) {
        empty = 0;
        if (lbl_1_bss_9FC.next == &lbl_1_bss_9FC && lbl_1_bss_9FC.next == lbl_1_bss_9FC.prev) {
            empty = 1;
        }
        if (empty != 0) {
            break;
        }
        p = (M579Puku *) lbl_1_bss_9FC.prev;
        if (p != 0) {
            fn_1_30F4(&p->model, -1);
            if (p != 0) {
                fn_1_1270(p, 0);
            }
            fn_1_C8(p);
        }
    }
}

void fn_1_645C(void)
{
    M579Node *n;
    M579Node *a;
    M579Node *b;

    fn_1_7468(&lbl_1_bss_9FC, fn_1_64E0);
    a = lbl_1_bss_9FC.next;
    b = a;
    n = b;
    if (n != &lbl_1_bss_9FC) {
        fn_1_12DC(n);
        fn_1_1330(n, &lbl_1_bss_9FC);
    }
}

void fn_1_64E0(M579Puku *self)
{
#ifdef TARGET_PC
    if (self->update.func != 0) {
        ((void (*)(M579Puku *)) self->update.func)(self);
    }
#else
    __ptmf_scall(self, &self->update);
#endif
    fn_1_660C(self);
    self->frame = self->frame + 1;
    fn_1_6534(self);
}

void fn_1_6534(M579Puku *self)
{
    void *mem;

    if ((self->pos.y > -150.0 && self->prev.y <= -150.0) || (self->pos.y < -150.0 && self->prev.y >= -150.0)) {
        mem = fn_1_0(0x10);
        if (mem != 0) {
            fn_1_BD18(mem, &self->pos, self->type);
        }
    }
    self->prev.x = self->pos.x;
    self->prev.y = self->pos.y;
    self->prev.z = self->pos.z;
}

void fn_1_660C(M579Puku *self)
{
    s32 se;
    s32 se2;
    f32 tp;
    f64 t;
    f64 c;
    s16 layer;

    if (self->state == 0 && self->pos.y > -150.0) {
        self->state = 1;
        if (self->type != 0) {
            se = 0x95F;
        } else {
            se = 0x956;
        }
        fn_1_9668(&self->pos, se);
    }
    if (self->state == 1 && self->pos.y < -150.0) {
        self->state = 0;
        if (self->type != 0) {
            se2 = 0x95A;
        } else {
            se2 = 0x959;
        }
        fn_1_9668(&self->pos, se2);
    }
    fn_1_2F3C(&self->model, &self->rot);
    fn_1_2EF4(&self->model, &self->pos);
    if (self->pos.y < -150.0) {
        layer = 0;
    } else {
        layer = 5;
    }
    fn_1_304C(&self->model, layer);
    tp = 1.0f;
    if (self->pos.y < -150.0) {
        t = (-150.0 - (f64) self->pos.y) / 550.0;
        if ((f32) t < 1.0) {
            c = t;
        } else {
            c = 1.0;
        }
        tp = (f32) (0.5 * (1.0 - c));
    }
    fn_1_3014(&self->model, tp);
}

void fn_1_682C(M579Puku *self, s32 idx, u8 flag)
{
    s32 attr;

    lbl_1_bss_A14 != 0 ? (void) 0 : fn_1_1D4(lbl_1_data_228, lbl_1_data_238, 0x27);
    s16 model = self->model;
    s16 mot = *fn_1_7528(&lbl_1_bss_A14->motion, idx);
    if (self->anim != idx) {
        fn_1_2E40(&self->model, 0x40000002);
        if (flag == 0) {
            attr = 0x40000001;
        } else {
            attr = 0;
        }
        Hu3DMotionShiftSet(model, mot, 0.0f, 8.0f, attr);
        self->anim = idx;
    }
}

void fn_1_692C(M579Puku *self)
{
    self->busy = 0;
    fn_1_2E08(&self->model, 1);
    self->pos.y = -500.0f;
    self->prev.x = self->pos.x;
    self->prev.y = self->pos.y;
    self->prev.z = self->pos.z;
}

void fn_1_6990(M579Puku *self)
{
    void *mem;

    if (self->type != 0) {
        mem = fn_1_0(0x10);
        if (mem != 0) {
            fn_1_A084(mem, &self->pos);
        }
    }
    fn_1_2E40(&self->model, 1);
    Hu3DMotionSpeedSet(self->model, 1.0f);
    fn_1_682C(self, 0, 0);
}

void fn_1_6AD4(M579Puku *self)
{
    Vec target[5];
    Vec dir;
    Vec home;
    M579Ptmf p;
    s32 k;
    f32 dist;
    f32 lim;
    f32 a;
    f32 b;
    f32 r;
    s32 se;
    void *mem;

    target[0] = lbl_1_rodata_C0[0];
    target[1] = lbl_1_rodata_C0[1];
    target[2] = lbl_1_rodata_C0[2];
    target[3] = lbl_1_rodata_C0[3];
    target[4] = lbl_1_rodata_C0[4];
    k = 0;
    if (self->pos.x > 0.0f) {
        b = self->pos.z;
        if (b < 0.0f) {
            r = -b;
        } else {
            r = b;
        }
        if (self->pos.x > r) {
            k = 2;
        } else if (self->pos.z > 0.0f) {
            k = 1;
        }
    } else {
        a = self->pos.z;
        if (a < 0.0f) {
            r = -a;
        } else {
            r = a;
        }
        if (-self->pos.x > r) {
            k = 3;
        } else if (self->pos.z > 0.0f) {
            k = 1;
        }
    }
    if (self->dead == 0) {
        k = 4;
    }
    dir = lbl_1_rodata_FC;
    dir.x = self->pos.x;
    dir.z = self->pos.z;
    PSVECSubtract(&target[k], &self->pos, &dir);
    PSVECScale(&dir, &dir, 1.0f / PSVECMag(&dir));
    PSVECScale(&dir, &dir, 5.0f);
    PSVECSubtract(&dir, &self->vel, &dir);
    PSVECScale(&dir, &dir, 0.05f);
    dir.y = -0.4086104f;
    PSVECAdd(&dir, &self->vel, &self->vel);
    PSVECAdd(&self->vel, &self->pos, &self->pos);
    a = self->pos.x;
    b = self->pos.z;
    dist = (f32) sqrt(a * a + b * b);
    if (dist > 500.0f && dist < 800.0f) {
        if (self->type != 0) {
            lim = (f32) 70.0;
        } else {
            lim = (f32) 20.0;
        }
        if (self->pos.y < lim) {
            self->pos.y = lim;
            self->vel.y = self->vel.y * -0.5f;
            if (self->type != 0) {
                se = 0x958;
            } else {
                se = 0x957;
            }
            fn_1_9668(&self->pos, se);
        }
    }
    self->rot.x = self->rot.x - 0.05f * self->rot.x;
    self->rot.y = (f32) ((f64) self->rot.y + 3.0);
    if (self->rot.y > 180.0f) {
        self->rot.y = self->rot.y - 360.0f;
    }
    if (self->pos.y < -700.0) {
        p = lbl_1_data_244;
        self->update = p;
    }
    fn_1_2E40(&self->model, 1);
    Hu3DMotionSpeedSet(self->model, 1.0f);
    if (self->vel.y > 0.0f) {
        fn_1_682C(self, 1, 0);
    } else {
        fn_1_682C(self, 0, 0);
    }
    if (self->type != 0) {
        mem = fn_1_0(0x10);
        if (mem != 0) {
            fn_1_A084(mem, &self->pos);
        }
    }
}

void fn_1_7324(M579Puku *self, u8 flag)
{
    Vec dir;
    M579Ptmf p;

    self->dead = flag;
    self->vel.x = 0.0f;
    self->vel.y = 0.0f;
    self->vel.z = 0.0f;
    p = lbl_1_data_250;
    self->update = p;
    dir = lbl_1_rodata_160;
    dir.x = self->pos.x;
    dir.z = self->pos.z;
    PSVECScale(&dir, &dir, 1.0f / PSVECMag(&dir));
    PSVECScale(&dir, &self->vel, 5.0f);
}

void fn_1_7468(M579Node *list, void (*fn)(M579Puku *))
{
    M579Node *l;
    M579Node *first;
    M579Node *n;

    l = list;
    first = l->next;
    n = first;
    while (n != l) {
        fn((M579Puku *) n);
        n = n->next;
    }
}

void fn_1_74C8(M579Node *first, M579Node *list, void (*fn)(M579Puku *))
{
    M579Node *n;

    n = first;
    while (n != list) {
        fn((M579Puku *) n);
        n = n->next;
    }
}

s16 *fn_1_7528(M579S16Vec *vec, u32 idx)
{
    if (idx >= vec->size) {
#ifdef TARGET_PC
        fn_1_234(stderr, lbl_1_data_25C);
#else
        fn_1_234(__files + 0xA0, lbl_1_data_25C);
#endif
        abort();
    }
    return &vec->data[idx];
}

void fn_1_75A0(void)
{
    M579Node *a;
    M579Node *b;

    a = &lbl_1_bss_9FC;
    fn_1_1264(a);
    b = a;
#ifndef TARGET_PC
    __register_global_object(b, (void *) fn_1_75F8, lbl_1_bss_9F0);
#endif
}

M579Puku *fn_1_75F8(M579Puku *self, s16 flag)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}
