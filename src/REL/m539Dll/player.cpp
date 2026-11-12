#include "REL/m539Dll.h"

/*
 * m539 "player": one object per participant. It owns a vector of 16 bit
 * motion ids, a state machine (idle / run / jump / ...) driven through
 * fn_1_DAF0 and a per-state tick (fn_1_E9EC), and a pointer-to-member hook.
 * fn_1_EA70 is the bounds-checked vector accessor ("vector::at ...").
 * The volatile temporaries in the vector code stand in for the stack slots
 * the original's inlined accessors leave behind.
 */

extern "C" {

typedef struct M539VObj_s {
    s32 pad0;
    s32 pad4;
    void (*dtor)(void *, s32);
} M539VObj;

typedef struct M539Dtorable_s {
    M539VObj *obj;
} M539Dtorable;

typedef struct M539Owner_s {
    s32 pad0;
    s32 pad4;
    void *obj;
} M539Owner;

typedef struct M539Conf_s {
    s16 charNo, padNo, dif, grp, type;
} M539Conf;

extern M539Conf GwPlayerConf[];
extern u8 __files[];
extern void *lbl_1_bss_A08;

void abort(void);
void *memmove(void *dst, const void *src, u32 n);
void *memcpy(void *dst, const void *src, u32 n);

void *fn_1_0(s32 size);
void *fn_1_63E0(void *p, s32 idx);
void *fn_1_64B8(void *p);
void fn_1_6398(void *p);
void *fn_1_2D18(void *p, s16 chr, s32 n);
void fn_1_3084(s16 *p, u16 mask);
void fn_1_304C(s16 *p, s32 n);
void fn_1_1264(void *self);
void fn_1_1270(void *self, s32 arg);
void fn_1_1330(void *self, void *arg);
void fn_1_1D4(const char *msg, const char *file, s32 line);
void fn_1_234(void *file, const char *msg, ...);
s32 fn_1_1F30(void);
void fn_1_10400(M539Game *g);
void fn_1_104A8(M539Game *g, M539Ptmf *p);
void fn_1_10A90(M539Game *g);
void fn_1_10BE8(M539Game *g);

void CharFXPlayPan(s16 chr, s16 se, s16 pan);
void CharModelFxFlagSet(s16 chr, s32 flag);
f32 CharModelHeightGet(s16 chr);
void CharModelVoicePanSet(s16 chr, s16 pan);
s16 CharMotionCreate(s16 a, s32 b);
void CharMotionDataClose(s16 chr);
s32 CharMotionEndCheck(s16 chr);
void CharMotionShiftSet(s16 chr, s16 mot, f32 start, f32 end, u32 attr);
void Hu3DModelShadowDispOn(s16 id);
void Hu3DModelShadowSet(s16 id);
f32 Hu3DMotionMotionMaxTimeGet(s16 id);
void Hu3DMotionSpeedSet(s16 id, f32 speed);
void omVibrate(s16 player, s16 dur, s16 off, s16 on);

void fn_1_D710(M539Player *self, s32 mot);
void fn_1_D9E4(M539Player *self);
void fn_1_DAF0(M539Player *self, s32 state);
s16 *fn_1_EA70(M539Vec *self, u32 index);
void fn_1_EAE8(M539Vec *v, s16 *pos, u32 n, s16 *val);
void fn_1_EE98(M539Vec *v, u32 cap);

s32 lbl_1_bss_A48;
s32 lbl_1_bss_A4C;
s32 lbl_1_bss_A50;

M539Ptmf lbl_1_data_430 = M539_PTMF_NULL_INIT;
char lbl_1_data_43C[] = "sm_ptr != 0";
char lbl_1_data_448[] = "a_set.h";
char lbl_1_data_450[] = "m_mot_id.size() == NUMBEROF_MOTION";
char lbl_1_data_473[] = "player.cpp";
M539Ptmf lbl_1_data_480 = M539_PTMF_NULL_INIT;
M539Ptmf lbl_1_data_48C = M539_PTMF_NULL_INIT;
M539Ptmf lbl_1_data_498 = M539_PTMF_NULL_INIT;
M539Ptmf lbl_1_data_4A4 = M539_PTMF_NULL_INIT;
M539Ptmf lbl_1_data_4B0 = M539_PTMF_NULL_INIT;
M539Ptmf lbl_1_data_4BC = M539_PTMF_NULL_INIT;
char lbl_1_data_4C8[] = "vector::at index out of range";
char lbl_1_data_4E6[] = "vector::insert length error\n";
char lbl_1_data_503[] = "Memory allocation failure";
char lbl_1_data_51D[] = "vector::reserve length error\n";

static inline s16 M539Deref(s16 *p)
{
    return *p;
}

void *fn_1_D010(s32 idx)
{
    void *obj;
    if (GwPlayerConf[idx].type != 0) {
        obj = fn_1_0(M539_ALLOC_SIZE(0x348));
        if (obj != 0) {
            obj = fn_1_64B8(obj);
        }
        return obj;
    }
    obj = fn_1_0(M539_ALLOC_SIZE(0x14));
    if (obj != 0) {
        obj = fn_1_63E0(obj, idx);
    }
    return obj;
}

M539Player *fn_1_D0B4(M539Player *self, s32 idx, M539Game *game)
{
    void *sub;
    s16 *mdl;

    fn_1_1264(self);
    if (GwPlayerConf[idx].type != 0) {
        sub = fn_1_0(M539_ALLOC_SIZE(0x348));
        if (sub != 0) {
            sub = fn_1_64B8(sub);
        }
    } else {
        sub = fn_1_0(M539_ALLOC_SIZE(0x14));
        if (sub != 0) {
            sub = fn_1_63E0(sub, idx);
        }
    }
    self->sub = (M539PlayerSub *)sub;
    self->mot.cap = 0;
    self->mot.size = 0;
    self->mot.items = 0;
    mdl = (s16 *)fn_1_0(2);
    if (mdl != 0) {
        mdl = (s16 *)fn_1_2D18(mdl, GwPlayerConf[idx].charNo, 3);
    }
    self->model = mdl;
    self->timer = 0;
    self->game = game;
    fn_1_3084(self->model, (u16)(1 << idx));
    if (lbl_1_bss_A08 == 0) {
        fn_1_1D4(lbl_1_data_43C, lbl_1_data_448, 0x20);
    }
    fn_1_1330(self, lbl_1_bss_A08);
    self->plyIdx = idx;
    self->charNo = GwPlayerConf[idx].charNo;
    self->height = CharModelHeightGet((s16)self->charNo);
    self->curMot = 0;
    self->mot.size = 0;
    fn_1_EE98(&self->mot, 7);
    fn_1_D710(self, 0x8C0000);
    fn_1_D710(self, 0x8C0003);
    fn_1_D710(self, 0x8C000B);
    fn_1_D710(self, 0x8700DD);
    fn_1_D710(self, 0x8C0004);
    fn_1_D710(self, 0x8C0030);
    fn_1_D710(self, 0x8C0031);
    if (self->mot.size != 7) {
        fn_1_1D4(lbl_1_data_450, lbl_1_data_473, 0x30);
    }
    CharMotionDataClose((s16)self->charNo);
    CharModelFxFlagSet((s16)self->charNo, 0);
    Hu3DModelShadowSet(*self->model);
    Hu3DModelShadowDispOn(*self->model);
    {
        M539Ptmf t = lbl_1_data_430;
        self->hook = t;
    }
    CharMotionShiftSet((s16)self->charNo, *fn_1_EA70(&self->mot, 0), 0.0f, 0.0f, 0x40000001);
    fn_1_DAF0(self, 0);
    self->vel.x = 0.0f;
    self->vel.y = 0.0f;
    self->vel.z = 0.0f;
    self->acc.x = 0.0f;
    self->acc.y = 0.0f;
    self->acc.z = 0.0f;
    fn_1_304C(self->model, 2);
    self->flag68 = 0;
    self->rot = 270.0f;
    return self;
}

M539Owner *fn_1_D498(M539Owner *self, s16 arg1)
{
    if (self != 0) {
        if (self != 0 && self != 0) {
            void **a = &self->obj;
            if (*a != 0) {
                void **b = &self->obj;
                void *c = *b;
                fn_1_C8(c);
            }
        }
        if (arg1 > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M539Owner *fn_1_D52C(M539Owner *self, s16 arg1)
{
    if (self != 0) {
        if (self != 0) {
            void **a = &self->obj;
            if (*a != 0) {
                void **b = &self->obj;
                void *c = *b;
                fn_1_C8(c);
            }
        }
        if (arg1 > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M539Owner *fn_1_D5B8(M539Owner *self, s16 arg1)
{
    if (self != 0) {
        void **a = &self->obj;
        if (*a != 0) {
            void **b = &self->obj;
            void *c = *b;
            fn_1_C8(c);
        }
        if (arg1 > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M539Dtorable *fn_1_D63C(M539Dtorable *self, s16 arg1)
{
    if (self != 0) {
        M539Dtorable *inner = (M539Dtorable *)self->obj;
        if (inner != 0) {
            inner->obj->dtor(inner, 1);
        }
        if (arg1 > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void *fn_1_D6B8(void *self, s16 arg1)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (arg1 > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_D710(M539Player *self, s32 arg1)
{
    s16 v = CharMotionCreate((s16)self->charNo, arg1);
    s16 *elem = self->mot.items + self->mot.size;
    fn_1_EAE8(&self->mot, elem, 1, &v);
}

void fn_1_D788(M539Player *self)
{
    s16 pan;
    M539PlayerSub *sub;
    Vec rot;

    if ((self->plyIdx & 1) == 0) {
        pan = 0x30;
    } else {
        pan = 0x50;
    }
    CharModelVoicePanSet((s16)self->charNo, pan);
    sub = self->sub;
    if (fn_1_1F30() == 2 && self->game->unkAC != 8) {
        fn_1_6398(sub);
    } else {
        sub->flag8 = 0;
    }
    if (M539PtmfTest(&self->hook) != 0) {
        M539PtmfCall(self, &self->hook);
    }
    fn_1_D9E4(self);
    fn_1_2EF4(self->model, &self->pos);
    rot.x = 0.0f;
    rot.y = self->rot;
    rot.z = 0.0f;
    fn_1_2F3C(self->model, &rot);
}

void fn_1_D8A0(M539Player *self, s32 idx, u8 flag)
{
    s16 *modelP = self->model;
    s16 model = M539Deref(modelP);
    s16 *mp = fn_1_EA70(&self->mot, idx);
    s16 mot = *mp;
    (void)model;
    if (self->curMot != idx) {
        s32 start = 0;
        s32 end = 3;
        if (idx == 1) {
            switch (GwPlayerConf[self->plyIdx].charNo) {
            case 5:
            case 2:
                start = (s32)Hu3DMotionMotionMaxTimeGet(mot);
                end = 2;
                break;
            }
        }
        CharMotionShiftSet((s16)self->charNo, mot, (f32)start, (f32)end, flag == 0 ? 0x40000001 : 0);
        self->curMot = idx;
    }
}

void fn_1_D9E4(M539Player *self)
{
    Vec tmp;

    self->acc.y = self->acc.y + -3.3333333333333335;
    tmp.x = self->vel.x;
    tmp.y = self->vel.y;
    tmp.z = self->vel.z;
    PSVECAdd(&self->acc, &self->vel, &self->vel);
    self->acc.x = 0.0f;
    self->acc.y = 0.0f;
    self->acc.z = 0.0f;
    PSVECAdd(&self->vel, &tmp, &tmp);
    PSVECScale(&tmp, &tmp, 0.5f);
    PSVECAdd(&tmp, &self->pos, &self->pos);
    if (self->pos.y < 0.0f) {
        self->pos.y = 0.0f;
        self->vel.y = 0.0f;
    }
}

void fn_1_DAF0(M539Player *self, s32 state)
{
    switch (state) {
    case 2:
        break;
    case 0:
        fn_1_D8A0(self, 0, 0);
        break;
    case 1:
        fn_1_D8A0(self, 1, 1);
        switch (GwPlayerConf[self->plyIdx].charNo) {
        case 5:
        case 2:
            self->acc.y = self->acc.y + 27.5;
            break;
        case 6:
            self->acc.y = self->acc.y + 30.833333333333336;
            break;
        default:
            self->acc.y = self->acc.y + 33.333333333333336;
            break;
        }
        break;
    case 3:
        Hu3DMotionSpeedSet(*self->model, 1.0f);
        fn_1_D8A0(self, 4, 1);
        break;
    case 4:
        self->vel.x = 0.0f;
        self->vel.y = 0.0f;
        self->vel.z = 0.0f;
        break;
    }
    self->state = state;
    self->unk30 = 0;
}

void fn_1_DEDC(M539Player *self)
{
    M539PlayerSub *sub;
    u8 *q;

    self->vel.x = 0.0f;
    self->vel.y = 0.0f;
    self->vel.z = 0.0f;
    sub = self->sub;
    q = (u8 *)sub + 4;
    if (q[4] != 0) {
        fn_1_DAF0(self, 1);
    }
}

void fn_1_DF6C(M539Player *self)
{
    if (self->vel.y < 0.0f) {
        if (self->flag68 == 0) {
            fn_1_10BE8(self->game);
            if (GwPlayerConf[self->plyIdx].type == 0) {
                omVibrate((s16)self->plyIdx, 10, 4, 2);
            }
        }
        fn_1_DAF0(self, 2);
    }
}

void fn_1_E008(M539Player *self)
{
    f32 v = self->pos.y - 30.0f;
    if (v <= 0.0f) {
        fn_1_DAF0(self, 3);
        return;
    }
}

void fn_1_E07C(M539Player *self)
{
    if (self->pos.y <= 0.0f) {
        self->vel.x = 0.0f;
        self->vel.y = 0.0f;
        self->vel.z = 0.0f;
        if (CharMotionEndCheck((s16)self->charNo) != 0) {
            fn_1_DAF0(self, 0);
        }
    }
}

void fn_1_E114(M539Player *self)
{
    s16 pan;
    fn_1_D8A0(self, 5, 1);
    fn_1_DAF0(self, 4);
    if ((self->plyIdx & 1) == 0) {
        pan = 0x30;
    } else {
        pan = 0x50;
    }
    CharFXPlayPan((s16)self->charNo, 0x1FA, pan);
}

void fn_1_E224(M539Player *self)
{
    fn_1_D8A0(self, 6, 1);
    fn_1_DAF0(self, 4);
}

void fn_1_E304(void)
{
}

void fn_1_E308(M539Player *self)
{
    self->pos.x = 125.0f;
    self->pos.y = 0.0f;
    self->pos.z = -210.0f;
    self->timer = 0x78;
    {
        M539Ptmf t = lbl_1_data_480;
        self->hook = t;
    }
    self->flag68 = 1;
}

void fn_1_E394(M539Player *self)
{
    u8 fire = 0;

    if (self->timer > 0) {
        s32 t = self->timer - 1;
        self->timer = t;
        if (t == 0) {
            fire = 1;
        }
    }
    if (fire != 0) {
        self->acc.x = self->acc.x - 8.333333333333334;
        self->acc.y = self->acc.y + 58.333333333333336;
        fn_1_D8A0(self, 1, 1);
        {
            M539Ptmf t = lbl_1_data_48C;
            self->hook = t;
        }
    }
}

void fn_1_E554(M539Player *self)
{
    if (self->vel.y > 0.0f && self->pos.y > 250.0f) {
        fn_1_D8A0(self, 3, 1);
    }
    if (self->vel.y < 0.0f) {
        fn_1_10400(self->game);
        {
            M539Ptmf t = lbl_1_data_498;
            self->hook = t;
        }
    }
}

void fn_1_E6C0(M539Player *self)
{
    M539Game *g = self->game;

    self->pos.x = g->pos.x;
    self->pos.y = g->pos.y;
    self->pos.z = g->pos.z;
    if (self->game->f78 > 0.0f) {
        self->pos.x = 50.0f;
        {
            M539Ptmf t = lbl_1_data_4A4;
            self->hook = t;
        }
        fn_1_D8A0(self, 4, 1);
        fn_1_10A90(self->game);
    }
    self->vel.x = 0.0f;
    self->vel.y = 0.0f;
    self->vel.z = 0.0f;
}

void fn_1_E864(M539Player *self)
{
    self->rot = self->rot + 10.0f;
    if (self->rot >= 345.0f) {
        self->rot = 345.0f;
    }
    if (self->pos.y <= 0.0f) {
        {
            M539Ptmf t = lbl_1_data_4B0;
            self->hook = t;
        }
        fn_1_DAF0(self, 0);
        self->timer = 0x1E;
    }
}

void fn_1_E940(M539Player *self)
{
    u8 fire = 0;

    if (self->timer > 0) {
        s32 t = self->timer - 1;
        self->timer = t;
        if (t == 0) {
            fire = 1;
        }
    }
    if (fire != 0) {
        {
            M539Ptmf t = lbl_1_data_4BC;
            self->hook = t;
        }
        self->flag68 = 0;
        fn_1_104A8(self->game, &lbl_1_data_4BC);
    }
}

void fn_1_E9EC(M539Player *self)
{
    switch (self->state) {
    case 0:
        fn_1_DEDC(self);
        return;
    case 1:
        fn_1_DF6C(self);
        return;
    case 2:
        fn_1_E008(self);
        return;
    case 3:
        fn_1_E07C(self);
    case 4:
        return;
    }
}

s16 *fn_1_EA70(M539Vec *self, u32 index)
{
    if (index >= self->size) {
        fn_1_234(__files + 0xA0, lbl_1_data_4C8);
        abort();
    }
    return self->items + index;
}

void fn_1_EAE8(M539Vec *v, s16 *pos, u32 n, s16 *val)
{
    s16 *srcP = val;
    if (n != 0) {
        M539Vec *t54 = v;
        volatile u32 max0 = 0x7FFFFFFF;
        volatile u32 max1 = max0;
        volatile u32 max = max1;
        M539Vec *t44;
        u32 capNow;
        volatile u32 capCopy;
        if (n > max || v->size > max - n) {
            fn_1_234(__files + 0xA0, lbl_1_data_4E6);
            abort();
        }
        t44 = v;
        capNow = t44->cap;
        capCopy = capNow;
        if (v->size + n <= capCopy) {
            s16 **itp = &v->items;
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
            s16 **itp = &v->items;
            s16 *old = *itp;
            volatile s16 *old2 = old;
            s32 idx = pos - old;
            s16 **itp2 = &v->items;
            u32 newSize = v->size + n;
            M539Vec *t28 = v;
            u32 cap;
            s16 *mem;
            u32 k;
            s16 *p;
            if (t28->cap != 0) {
                M539Vec *t24 = v;
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
                s16 **itp3 = &v->items;
                M539Vec *t1c = v;
                M539Vec *t78 = t1c;
                mem = (s16 *)fn_1_0(cap * 2);
                (void)itp3;
                (void)t78;
            }
            if (mem == 0) {
                fn_1_234(__files + 0xA0, lbl_1_data_503);
                abort();
            }
            {
                volatile s16 *m74 = mem;
                *itp2 = mem;
                (void)m74;
            }
            k = n;
            {
                s16 **itp4 = &v->items;
                p = *itp4 + idx;
            }
            while (k != 0) {
                *p = *srcP;
                p++;
                k--;
            }
            if (old != 0) {
                s16 **itp5 = &v->items;
                s16 *nw = *itp5;
                volatile s16 *nw2 = nw;
                s32 cnt = (old + idx) - old;
                volatile s32 cnt2 = cnt;
                s16 **itp6;
                s16 *dst;
                memcpy(nw, old, cnt * 2);
                itp6 = &v->items;
                dst = *itp6 + (idx + n);
                {
                    s16 *e = old + v->size;
                    s32 cnt3 = e - (old + idx);
                    volatile s32 cnt4 = cnt3;
                    memcpy(dst, old + idx, cnt3 * 2);
                    (void)cnt4;
                }
                fn_1_C8(old);
                (void)nw2;
                (void)cnt2;
            }
            v->size = newSize;
            {
                M539Vec *t0c = v;
                t0c->cap = cap;
            }
            (void)old2;
        }
        (void)t54;
    }
}

void fn_1_EE98(M539Vec *v, u32 cap)
{
    u32 *cp = &v->cap;
    if (cap > *cp) {
        M539Vec *self2 = v;
        volatile u32 max0 = 0x7FFFFFFF;
        volatile u32 max = max0;
        s16 **ip = &v->items;
        s16 *old;
        s16 **ip2;
        volatile M539Vec *sv;
        volatile M539Vec *sv2;
        s16 *mem;
        if (cap > max) {
            fn_1_234(__files + 0xA0, lbl_1_data_51D);
            abort();
        }
        old = *ip;
        ip2 = &v->items;
        sv = v;
        sv2 = sv;
        mem = (s16 *)fn_1_0(cap * 2);
        if (mem == 0) {
            fn_1_234(__files + 0xA0, lbl_1_data_503);
            abort();
        }
        *ip2 = mem;
        if (old != 0) {
            s16 **ip3 = &v->items;
            s16 *nw = *ip3;
            s16 *e = old + v->size;
            s32 cnt = e - old;
            memcpy(nw, old, cnt * 2);
            fn_1_C8(old);
        }
        {
            M539Vec *s3 = v;
            s3->cap = cap;
        }
        (void)self2;
        (void)sv2;
    }
}

void fn_1_EFC0(void)
{
    lbl_1_bss_A48 = 120;
    lbl_1_bss_A4C = 210;
    lbl_1_bss_A50 = 210;
}

}
