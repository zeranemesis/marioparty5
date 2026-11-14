// m517Dll input: player input sources. A base object with a virtual update()
// that fills a stick state, a pad reader and two CPU brains (a "chase" brain
// that steers towards a target and a "dodge" brain that picks between three
// enemies).  The CPU brains read the player list hanging off lbl_1_bss_A08.
#include "REL/m517Dll.h"
extern "C" {
#include "game/gamework.h"
}
extern "C" {
#include "game/pad.h"
}
extern "C" {
#include "game/frand.h"
}
#include "math.h"

extern "C" {

extern void *lbl_1_bss_10;

// fn_1_12E8 comes from the shared header (M517Ball view of the body)
void fn_1_111A8(void *self);
void fn_1_17614(void *self);
void fn_1_16B68(void *self);

void fn_1_96AC(M517CpuInput *self);
u8 fn_1_83E0(M517CpuInput *self, f32 dist);
void fn_1_881C(Vec *out, M517CpuInput *self, f32 dist, f32 angle, u8 flag);
void fn_1_8B4C(Vec *out, M517CpuInput *self, f32 dist);
f32 fn_1_8BD4(void *self, Vec *a, Vec *b, Vec *c);
u8 fn_1_8C88(void *self, Vec *a, Vec *b, Vec *c, Vec *d);
u8 fn_1_8EB8(void *self, Vec *a, Vec *b, Vec *c, Vec *d);
void fn_1_9C54(M517CpuInput2 *self);
void fn_1_9E48(M517CpuInput2 *self);

M517Ptmf lbl_1_data_260[3] = {
    { 0, -1, (void (*)(void *))fn_1_111A8 },
    { 0, -1, (void (*)(void *))fn_1_17614 },
    { 0, -1, (void (*)(void *))fn_1_16B68 },
};
f32 lbl_1_data_284[4] = { 100.0f, 150.0f, 250.0f, 300.0f };
char lbl_1_data_294[] = "ms_pointer != 0";
char lbl_1_data_2A4[] = "singleton.h";
char lbl_1_data_2B0[] = "sm_ptr != 0";
char lbl_1_data_2BC[] = "a_set.h";
f32 lbl_1_data_2C4[4] = { 0.7f, 0.8f, 0.9f, 1.0f };
f32 lbl_1_data_2D4[5] = { 10.0f, 8.0f, 3.0f, 2.0f, 3.0f };
f32 lbl_1_data_2E8[4] = { 0.3f, 0.4f, 0.4f, 0.5f };
M517InputVtbl lbl_1_data_2F8 = { { 0, 0 }, fn_1_A090, fn_1_99EC };
M517InputVtbl lbl_1_data_308 = { { 0, 0 }, fn_1_8134, fn_1_81A0 };
M517InputVtbl lbl_1_data_318 = { { 0, 0 }, fn_1_A0FC, fn_1_7C9C };
M517InputVtbl lbl_1_data_328 = { { 0, 0 }, fn_1_7C44, 0 };

u8 lbl_1_bss_A00;
M517Master *lbl_1_bss_A04;
void *lbl_1_bss_A08;

extern const Vec lbl_1_rodata_F0 = { 0.0f, 0.0f, 0.0f };

// Reading through a volatile lvalue is what keeps the named .rodata
// relocation; a plain read of this const is folded into an anonymous @N.
extern const f32 lbl_1_rodata_108 = 0.0f;

static void inp_baseInit(M517Input *self)
{
    self->vtbl = &lbl_1_data_328;
    self->st.x = *(volatile f32 *)&lbl_1_rodata_108;
    self->st.y = *(volatile f32 *)&lbl_1_rodata_108;
    self->st.mag = *(volatile f32 *)&lbl_1_rodata_108;
    self->level = 0;
}

// 2D (x/z) orientation of c relative to the line a->b.
static f32 inp_orient(const Vec *a, const Vec *b, const Vec *c)
{
    return c->x * (a->z - b->z) + (a->x * (b->z - c->z) + b->x * (c->z - a->z));
}

static f32 inp_sgn(f32 v)
{
    if (v > 0.0f) {
        return 1.0f;
    }
    if (v < 0.0f) {
        return -1.0f;
    }
    return 0.0f;
}

void fn_1_7B2C(M517Input *self)
{
    inp_baseInit(self);
}

void fn_1_7B74(M517Input *self)
{
    self->st.x = *(volatile f32 *)&lbl_1_rodata_108;
    self->st.y = *(volatile f32 *)&lbl_1_rodata_108;
    self->vtbl->update(self, &self->st);
}

M517InputCtorRet fn_1_7BD4(M517Input *base, s32 playerNo)
{
    M517PadInput *self = (M517PadInput *)base;

    inp_baseInit(base);
    self->base.vtbl = &lbl_1_data_318;
    self->playerNo = playerNo;
    self->padNo = GwPlayerConf[playerNo].padNo;
#ifdef TARGET_PC
    return base;
#endif
}

M517Input *fn_1_7C44(M517Input *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = &lbl_1_data_328;
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

// Pad reader: map the analog stick onto a unit disc.
void fn_1_7C9C(M517Input *base, M517InputState *out)
{
    M517PadInput *self = (M517PadInput *)base;
    s32 pad = self->padNo;
    f32 sx = 0.017857144f * (f32)HuPadStkX[pad];
    f32 sy = 0.017857144f * (f32)HuPadStkY[pad];
    f64 x = sin(M_PI * (f64)sx * 0.5);
    f64 y = sin((M_PI * (f64)sy) / -2.0);
    f64 mag = sqrt((x * x) + (y * y));
    f64 inv;

    if (mag > 1.0) {
        inv = 1.0 / mag;
        x *= inv;
        y *= inv;
        mag = 1.0;
    }
    out->x = x;
    out->y = y;
    out->mag = mag;
}

M517InputCtorRet fn_1_8000(M517Input *base, s32 playerNo, s32 level)
{
    M517CpuInput *self = (M517CpuInput *)base;

    inp_baseInit(base);
    self->base.vtbl = &lbl_1_data_308;
    self->playerNo = playerNo;
    self->timer = 0.0f;
    self->flag = 1;
    self->base.level = level;
    self->out.x = 0.0f;
    self->out.y = 0.0f;
    self->out.z = 0.0f;
    self->c.x = 0.0f;
    self->c.y = 0.0f;
    self->c.z = 0.0f;
    self->dest.x = 0.0f;
    self->dest.y = 0.0f;
    self->dest.z = 0.0f;
    self->smooth.x = 0.0f;
    self->smooth.y = 0.0f;
    self->smooth.z = 0.0f;
#ifdef TARGET_PC
    return base;
#endif
}

M517Input *fn_1_8134(M517Input *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = &lbl_1_data_308;
        if (self != 0) {
            self->vtbl = &lbl_1_data_328;
        }
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

// CPU chase brain update.
void fn_1_81A0(M517Input *base, M517InputState *out)
{
    M517CpuInput *self = (M517CpuInput *)base;
    Vec d;
    f32 dist;

    fn_1_96AC(self);
    PSVECSubtract(&self->b, &self->a, &d);
    dist = PSVECMag(&d);
    if (PSVECMag(&self->c) > 0.0f) {
        self->flag = fn_1_83E0(self, dist);
        if (self->flag == 0) {
            self->out.y = self->out.y + (-out->mag * 0.02f);
        } else {
            self->out.y = 1.0f;
        }
        self->smooth.x = self->smooth.x + (self->dest.x - self->smooth.x) / 54.0f;
        self->smooth.y = self->smooth.y + (self->dest.y - self->smooth.y) / 54.0f;
        self->smooth.z = self->smooth.z + (self->dest.z - self->smooth.z) / 54.0f;
        PSVECSubtract(&self->smooth, &self->a, &d);
        if (PSVECMag(&d) > 100.0f) {
            PSVECNormalize(&d, &d);
            PSVECScale(&d, &d, self->out.y);
            self->out.x = d.x;
            self->out.z = d.z;
        }
        out->x = self->out.x;
        out->y = self->out.z;
        out->mag = self->out.y;
        self->timer = self->timer + 0.016666666666666666;
    }
}

// Decide where the chase brain wants to go; returns 0 when the target is out
// of range or too slow.
u8 fn_1_83E0(M517CpuInput *self, f32 dist)
{
    Vec a, b;
    f32 limit, magA, magB, rndA, rndB;
    s32 mask;

    M517_ASSERT(lbl_1_bss_10 != 0, lbl_1_data_294, lbl_1_data_2A4, 0x27);
    limit = 600.0f - lbl_1_data_284[self->base.level];
    a = self->a;
    b = self->b;
    mask = 0;
    magA = PSVECMag(&a);
    if (magA > limit) {
        mask |= 1;
    }
    magB = PSVECMag(&b);
    if (magB > limit) {
        mask |= 2;
    }
    rndA = 100.0f * (3.0f * (0.000015258789f * (f32)(u16)frand()));
    rndB = 10.0f * (0.000015258789f * (f32)(u16)frand());
    if (dist > 500.0f) {
        return 0;
    }
    if (self->speed < 0.05) {
        return 0;
    }
    magB = PSVECMag(&self->b);
    switch (mask) {
    case 0:
        fn_1_881C(&a, self, 700.0f + rndA, 90.0f + rndB, 1);
        self->dest = a;
        break;
    case 1:
        fn_1_881C(&a, self, 600.0f + rndA, 90.0f + rndB, 1);
        self->dest = a;
        break;
    case 2:
        fn_1_8B4C(&a, self, 700.0f + rndA);
        self->dest = a;
        break;
    case 3:
        if (magB < 300.0f) {
            u8 isFar = dist > 400.0f;
            fn_1_881C(&a, self, 600.0f + rndA, 130.0f + rndB, isFar);
            self->dest = a;
        } else {
            fn_1_8B4C(&a, self, 700.0f + rndA);
            self->dest = a;
        }
        break;
    }
    return 1;
}

// Point at `dist` from the target, rotated by `angle` degrees around Y from
// the direction towards us (flag == 1: from the target velocity).  If the
// path to it crosses the line the target moves along, try the opposite side.
void fn_1_881C(Vec *out, M517CpuInput *self, f32 dist, f32 angle, u8 flag)
{
    Vec a, b, c;
    Vec dir, cand, tmp, ray;
    Mtx rot;
    u8 hit1, hit2;

    a = self->a;
    b = self->b;
    c = self->c;
    if (flag != 1) {
        PSVECSubtract(&a, &b, &dir);
        PSVECNormalize(&dir, &dir);
    } else {
        dir = c;
        PSVECNormalize(&dir, &dir);
    }
    tmp = dir;
    PSMTXRotRad(rot, 'Y', 0.017453292f * angle);
    PSMTXMultVecSR(rot, &tmp, &tmp);
    PSVECScale(&tmp, &tmp, dist);
    PSVECAdd(&tmp, &self->b, &tmp);
    cand = tmp;
    {
        Vec e;

        ray = self->c;
        e = self->b;
        PSVECNormalize(&ray, &ray);
        PSVECScale(&ray, &ray, dist);
        PSVECAdd(&e, &ray, &e);
        hit1 = fn_1_8EB8(self, &self->a, &tmp, &self->b, &e);
        ray = self->c;
        e = self->b;
        PSVECNormalize(&ray, &ray);
        PSVECScale(&ray, &ray, -dist);
        PSVECAdd(&e, &ray, &e);
        hit2 = fn_1_8EB8(self, &self->a, &tmp, &self->b, &e);
    }
    if (hit1 != 0 || hit2 != 0) {
        tmp = dir;
        PSMTXRotRad(rot, 'Y', 0.017453292f * (180.0f + angle));
        PSMTXMultVecSR(rot, &tmp, &tmp);
        PSVECScale(&tmp, &tmp, dist);
        PSVECAdd(&tmp, &self->b, &tmp);
        cand = tmp;
    }
    *out = cand;
}

// Point behind the target, `dist` away along its position vector.
void fn_1_8B4C(Vec *out, M517CpuInput *self, f32 dist)
{
    Vec v = self->b;

    PSVECNormalize(&v, &v);
    PSVECScale(&v, &v, -dist);
    *out = v;
}

// Sign of the orientation of c relative to a->b.
f32 fn_1_8BD4(void *self, Vec *a, Vec *b, Vec *c)
{
    return inp_sgn(inp_orient(a, b, c));
}

// True when c and d lie on opposite sides (or on) the line a->b.
u8 fn_1_8C88(void *self, Vec *a, Vec *b, Vec *c, Vec *d)
{
    f32 s1 = inp_sgn(inp_orient(a, b, c));
    f32 s2 = inp_sgn(inp_orient(a, b, d));

    return 0.0f >= s1 * s2;
}

static f32 inp_max(f32 p, f32 q)
{
    return p > q ? p : q;
}

static f32 inp_min(f32 p, f32 q)
{
    return p < q ? p : q;
}

// 2D (x/z) segment intersection: segment a-b against segment c-d.
u8 fn_1_8EB8(void *self, Vec *a, Vec *b, Vec *c, Vec *d)
{
    if (inp_min(a->x, b->x) > inp_max(c->x, d->x)) {
        return 0;
    }
    if (inp_min(a->z, b->z) > inp_max(c->z, d->z)) {
        return 0;
    }
    if (inp_min(c->x, d->x) > inp_max(a->x, b->x)) {
        return 0;
    }
    if (inp_min(c->z, d->z) > inp_max(a->z, b->z)) {
        return 0;
    }
    if (0.0f >= inp_sgn(inp_orient(a, b, c)) * inp_sgn(inp_orient(a, b, d))) {
        return 0.0f >= inp_sgn(inp_orient(c, d, a)) * inp_sgn(inp_orient(c, d, b));
    }
    return 0;
}

// Refresh the chase brain's own position and the flagged target from the
// player list.
void fn_1_96AC(M517CpuInput *self)
{
    M517Node *head;
    M517Player *n;

    M517_ASSERT(lbl_1_bss_A08 != 0, lbl_1_data_2B0, lbl_1_data_2BC, 0x2E);
    head = (M517Node *)lbl_1_bss_A08;
    for (n = (M517Player *)head->next; (M517Node *)n != head; n = (M517Player *)n->link.next) {
        if (n->cpu != 0) {
            self->b.x = n->sph.pos.x;
            self->b.y = n->sph.pos.y;
            self->b.z = n->sph.pos.z;
            self->b.y = 0.0f;
            self->speed = fn_1_12E8(n->ball);
            if (fn_1_12E8(n->ball) > 0.05f) {
                self->c = n->ball->delta;
            }
        } else if (self->playerNo == n->playerNo) {
            self->a.x = n->sph.pos.x;
            self->a.y = n->sph.pos.y;
            self->a.z = n->sph.pos.z;
            self->a.y = 0.0f;
        }
    }
}

M517InputCtorRet fn_1_9930(M517Input *base, s32 level)
{
    M517CpuInput2 *self = (M517CpuInput2 *)base;
    s32 i;

    inp_baseInit(base);
    self->base.vtbl = &lbl_1_data_2F8;
    self->timer = 0.0f;
    self->target = 0;
    self->mode = 0;
    self->count = 0;
    self->base.level = level;
    for (i = 0; i < 3; i++) {
        self->flag[i] = 0;
        self->near[i] = 0;
    }
#ifdef TARGET_PC
    return base;
#endif
}

// CPU dodge brain update.
void fn_1_99EC(M517Input *base, M517InputState *out)
{
    M517CpuInput2 *self = (M517CpuInput2 *)base;
    Vec d, e, t;
    s32 level;

    fn_1_9E48(self);
    level = self->base.level;
    if (self->mode != 2) {
        fn_1_9C54(self);
        e = self->vel[self->target];
        if (PSVECMag(&e) <= 0.0f || self->mode == 0) {
            PSVECSubtract(&self->pos[self->target], &self->a, &d);
        } else {
            t = self->pos[self->target];
            PSVECNormalize(&e, &e);
            PSVECScale(&e, &e, 500.0f);
            PSVECAdd(&e, &t, &t);
            PSVECSubtract(&t, &self->a, &d);
        }
    } else {
        Vec z;

        z.x = *(volatile f32 *)&lbl_1_rodata_108;
        z.y = *(volatile f32 *)&lbl_1_rodata_108;
        z.z = *(volatile f32 *)&lbl_1_rodata_108;
        PSVECSubtract(&z, &self->a, &d);
        if (PSVECMag(&d) < 100.0f) {
            self->mode = frand() & 1;
        }
    }
    PSVECNormalize(&d, &d);
    out->x = out->x + (d.x - out->x) * lbl_1_data_2C4[level];
    out->y = out->y + (d.z - out->y) * lbl_1_data_2C4[level];
    out->mag = 1.0f;
    self->timer = self->timer + 0.016666666666666666;
}

// Choose which enemy the dodge brain reacts to.
void fn_1_9C54(M517CpuInput2 *self)
{
    s32 level = self->base.level;
    s32 i;
    f32 minD, maxD, dd;
    Vec d;

    for (i = 0; i < 3; i++) {
        if (self->flag[i] == 0 && self->near[i] != 0 && level > 1) {
            self->target = i;
            self->mode = 1;
            return;
        }
    }
    if (self->timer < lbl_1_data_2D4[level] && self->flag[self->target] == 0) {
        return;
    }
    minD = 1000.0f;
    maxD = 0.0f;
    for (i = 0; i < 3; i++) {
        if (self->flag[i] == 0) {
            PSVECSubtract(&self->a, &self->pos[i], &d);
            dd = PSVECMag(&d);
            if (self->f68 > lbl_1_data_2E8[level]) {
                if (dd < minD) {
                    minD = dd;
                    self->target = i;
                }
            } else if (dd > maxD) {
                maxD = dd;
                self->target = i;
            }
        }
    }
    if ((self->count & 1) != 0) {
        self->mode = 2;
    } else {
        self->mode = frand() & 1;
    }
    self->count++;
    self->timer = 0.0f;
}

// Refresh the dodge brain's own position and its three enemies from the
// player list.
void fn_1_9E48(M517CpuInput2 *self)
{
    M517Node *head;
    M517Player *n;
    s32 i = 0;

    M517_ASSERT(lbl_1_bss_A08 != 0, lbl_1_data_2B0, lbl_1_data_2BC, 0x2E);
    head = (M517Node *)lbl_1_bss_A08;
    for (n = (M517Player *)head->next; (M517Node *)n != head; n = (M517Player *)n->link.next) {
        if (n->cpu != 0) {
            self->a.x = n->sph.pos.x;
            self->a.y = n->sph.pos.y;
            self->a.z = n->sph.pos.z;
            self->a.y = 0.0f;
            self->f68 = fn_1_12E8(n->ball);
        } else {
            self->pos[i].x = n->sph.pos.x;
            self->pos[i].y = n->sph.pos.y;
            self->pos[i].z = n->sph.pos.z;
            self->pos[i].y = 0.0f;
            self->vel[i] = n->delta;
            self->flag[i] = n->busy;
            self->near[i] = (n->motion == 5);
            i++;
        }
    }
}

M517Input *fn_1_A090(M517Input *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = &lbl_1_data_2F8;
        if (self != 0) {
            self->vtbl = &lbl_1_data_328;
        }
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

M517Input *fn_1_A0FC(M517Input *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = &lbl_1_data_318;
        if (self != 0) {
            self->vtbl = &lbl_1_data_328;
        }
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

}
