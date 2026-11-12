#include "dolphin.h"
#include <math.h>
#ifdef TARGET_PC
#include <string.h>
#else
extern "C" {
void *memcpy(void *dst, const void *src, unsigned long n);
void *memmove(void *dst, const void *src, unsigned long n);
}
#endif
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/pad.h"
#include "REL/m579Dll/m579objs.h"

extern "C" {
void *__register_global_object(void *object, void *destructor, void *regmem);
void __ptmf_scall(void *self, void *ptmf);
void fn_1_2D18(s16 *model, s32 charNo, s32 flag);
void fn_1_255C(s32 idx, s32 flag);
void fn_1_8FCC(void *camera, void *target);
s16 fn_1_9344(Vec *pos);
s16 fn_1_93E4(Vec *pos);
void fn_1_14460(void *score, s32 idx, s32 type);
void fn_1_14590(void *score, s32 idx, s32 value);
s32 fn_1_1F30(void);
M579Fish *fn_1_11778(Vec *pos);
s32 fn_1_11844(Vec *pos);
M579Fish *fn_1_11990(Vec *pos);
f32 fn_1_11C34(M579Fish *f, Vec *pos);
void fn_1_13FE0(M579Fish *f);
void fn_1_1404C(M579Fish *f, u8 flag);
void CharLoseVoicePlay(s16 charNo1, s16 charNo2, s16 charNo3, s16 charNo4);
s32 CharNpcDustVoiceOffSet(s16 modelId, s16 motId, s16 type);
s16 CharMotionCreate(s16 charNo, u32 dataNum);
void CharMotionDataClose(s16 charNo);
char *CharModelItemHookGet(s16 charNo, s16 model, s16 hookNo);
void CharMotionShiftSet(s16 charNo, s16 motId, f32 start, f32 end, u32 attr);
void CharModelVoicePanSet(s16 charNo, s16 pan);
void CharModelVoiceVolSet(s16 charNo, s16 vol);
void omVibrate(s16 playerNo, s16 duration, s16 off, s16 on);
extern void *lbl_1_bss_A08;
extern void *lbl_1_bss_A2C;

f32 fn_1_C6D4(f64 a, f64 b);
f32 fn_1_C78C(f64 a, f64 b, f32 lim);
void fn_1_C8F0(Vec *out, s32 idx);
void fn_1_CA2C(void);
M579Player *fn_1_CAD0(M579Player *self, s32 idx);
M579S16Vec *fn_1_CF80(M579S16Vec *self, s16 flag);
M579S16Vec *fn_1_D014(M579S16Vec *self, s16 flag);
M579S16Vec *fn_1_D0A0(M579S16Vec *self, s16 flag);
M579Node *fn_1_D124(M579Node *self, s16 flag);
M579Player *fn_1_D17C(M579Player *self, s16 flag);
s16 fn_1_D244(M579Player *self, s32 dataNum);
void fn_1_D2C0(void);
void fn_1_D358(void);
void fn_1_D594(void);
void fn_1_D6A4(M579Player *p);
void fn_1_DA38(M579Player *p);
u8 fn_1_DE88(void);
void fn_1_DFBC(M579Player *p);
void fn_1_DFEC(M579Player *p);
void fn_1_E01C(M579Player *p);
void fn_1_E060(M579Player *p);
void fn_1_E0AC(M579Player *p);
void fn_1_E15C(M579Player *p);
void fn_1_E524(M579Player *p, s32 idx, u8 flag);
void fn_1_E5E4(M579Player *p);
void fn_1_E6C8(M579Player *p);
void fn_1_E9BC(M579Player *p, Mtx out);
void fn_1_EA28(M579Player *p);
void fn_1_EEF4(M579Player *p);
void fn_1_F050(M579Player *p);
void fn_1_F3A8(M579Player *p);
void fn_1_F810(M579Player *p);
u8 fn_1_F8B0(M579Player *p, Vec *dir);
void fn_1_FC6C(M579Player *p, Vec *out);
s32 fn_1_FE34(M579Player *p);
f32 fn_1_10100(M579Player *p);
void fn_1_101C8(M579Player *p, Vec *dir);
void fn_1_1060C(M579Node *list, void (*fn)(M579Player *));
void fn_1_1066C(M579Node *first, M579Node *list, void (*fn)(M579Player *));
void fn_1_106CC(M579S16Vec *vec, s16 *pos, u32 n, s16 *val);
void fn_1_10A7C(M579S16Vec *vec, u32 n);
void fn_1_10BA4(void);
M579Node *fn_1_10C2C(M579Node *self, s16 flag);
}

M579Ptmf lbl_1_data_3F8 = { 0, -1, (void *) fn_1_DA38 };
char lbl_1_data_404[35] = "m_mot_id.size() == NUMBEROF_MOTION";
char lbl_1_data_427[11] = "player.cpp";
char lbl_1_data_432[16] = "ms_pointer != 0";
char lbl_1_data_442[14] = "singleton.h";
M579Ptmf lbl_1_data_450 = { 0, -1, (void *) fn_1_D6A4 };
M579Ptmf lbl_1_data_45C = { 0, -1, (void *) fn_1_E5E4 };
M579Ptmf lbl_1_data_468 = { 0, -1, (void *) fn_1_DFBC };
M579Ptmf lbl_1_data_474 = { 0, -1, (void *) fn_1_DFEC };
M579Ptmf lbl_1_data_480 = { 0, -1, (void *) fn_1_E6C8 };
M579Ptmf lbl_1_data_48C = { 0, -1, (void *) fn_1_EA28 };
M579Ptmf lbl_1_data_498 = { 0, -1, (void *) fn_1_EEF4 };
M579Ptmf lbl_1_data_4A4 = { 0, -1, (void *) fn_1_E6C8 };
char lbl_1_data_4B0[30] = "vector::at index out of range";
char lbl_1_data_4CE[29] = "vector::insert length error\n";
char lbl_1_data_4EB[26] = "Memory allocation failure";
char lbl_1_data_505[35] = "vector::reserve length error\n";

s32 lbl_1_bss_AB8;
s32 lbl_1_bss_ABC;
s32 lbl_1_bss_AC0;
M579Player *lbl_1_bss_AC4;
u8 lbl_1_bss_AC8[12];
M579Node lbl_1_bss_AD4;

M579_RODATA const f32 lbl_1_rodata_490[4] = { 0.0f, 90.0f, 270.0f, 180.0f };
M579_RODATA const s32 lbl_1_rodata_4CC[4] = { 0, 1, 2, 3 };
M579_RODATA const f32 lbl_1_rodata_500[4] = { 0.0f, 90.0f, 270.0f, 180.0f };

f32 fn_1_C6D4(f64 a, f64 b)
{
    f64 x;
    f64 y;
    s32 ia;
    s32 ib;
    s32 t;
    s32 t1;
    s32 t2;

    x = a;
    y = b;
    ia = (s32) (182.04444444444445 * x);
    ib = (s32) (182.04444444444445 * y);
    t = (u16) ((ia - ib) - 0x8000) - 0x8000;
    t1 = t;
    t2 = t1;
    return (f32) (0.0054931640625 * (f64) t2);
}

f32 fn_1_C78C(f64 a, f64 b, f32 lim)
{
    f32 d;
    f32 r;
    f32 l;
    f64 x;
    f64 y;
    s32 ia;
    s32 ib;
    s32 t;
    s32 t1;
    s32 t2;

    x = a;
    y = b;
    l = lim;
    ia = (s32) (182.04444444444445 * x);
    ib = (s32) (182.04444444444445 * y);
    t = (u16) ((ia - ib) - 0x8000) - 0x8000;
    t1 = t;
    t2 = t1;
    d = (f32) (0.0054931640625 * (f64) t2);
    if (d < -l) {
        r = -l;
    } else if (d > l) {
        r = l;
    } else {
        r = d;
    }
    return r;
}

void fn_1_C8F0(Vec *out, s32 idx)
{
    f32 table[4];
    f32 a;
    f64 z;

    table[0] = lbl_1_rodata_490[0];
    table[1] = lbl_1_rodata_490[1];
    table[2] = lbl_1_rodata_490[2];
    table[3] = lbl_1_rodata_490[3];
    a = (f32) ((3.141592653589793 * (f64) (45.0f + (180.0f + table[idx]))) / 180.0);
    z = 900.0f * sin(a);
    out->x = (f32) (900.0f * cos(a));
    out->y = 0.0f;
    out->z = (f32) z;
}

void fn_1_CA2C(void)
{
    u8 empty;

    for (;;) {
        empty = 0;
        if (lbl_1_bss_AD4.next == &lbl_1_bss_AD4 && lbl_1_bss_AD4.next == lbl_1_bss_AD4.prev) {
            empty = 1;
        }
        if (empty != 0) {
            break;
        }
        fn_1_D17C((M579Player *) lbl_1_bss_AD4.prev, 1);
    }
}

M579Player *fn_1_CAD0(M579Player *self, s32 idx)
{
    f32 table[4];
    f32 a;
    f64 z;
    f32 yaw;
    s16 model;
    s16 conf;
    M579Ptmf p;
    M579S16Vec *vec;

    fn_1_1264(self);
    self->f38 = 0.0f;
    self->stick = 0.0f;
    self->state = 0;
    fn_1_2D18(&self->model, GwPlayerConf[idx].charNo, 1);
    self->loser = 0;
    self->fish = 0;
    self->target = 0;
    self->count = 0;
    self->score = 0;
    self->timer = 0;
    self->active = 0;
    vec = &self->motion;
    self->motion.cap = 0;
    vec->size = 0;
    vec->data = 0;
    fn_1_1330(self, &lbl_1_bss_AD4);
    self->idx = idx;
    self->charNo = GwPlayerConf[idx].charNo;
    self->dif = GwPlayerConf[idx].dif;
    if (GwPlayerConf[idx].type != 0) {
        conf = -1;
    } else {
        conf = GwPlayerConf[idx].padNo;
    }
    self->padNo = conf;
    self->anim = -1;
    self->motion.size = 0;
    fn_1_10A7C(&self->motion, 7);
    fn_1_D244(self, 0x8C0000);
    CharNpcDustVoiceOffSet(self->model, fn_1_D244(self, 0x8C0001), 0);
    CharNpcDustVoiceOffSet(self->model, fn_1_D244(self, 0x8C0002), 1);
    fn_1_D244(self, 0x8C0076);
    fn_1_D244(self, 0x8C0077);
    fn_1_D244(self, 0x8C0030);
    fn_1_D244(self, 0x8C0031);
    if (self->motion.size == 7) {
    } else {
        fn_1_1D4(lbl_1_data_404, lbl_1_data_427, 0x86);
    }
    CharMotionDataClose(self->charNo);
    model = self->model;
    Hu3DModelShadowSet(model);
    Hu3DModelShadowDispOn(model);
    Hu3DModelLayerSet(model, 3);
    Hu3DModelCameraSet(model, 1);
    table[0] = lbl_1_rodata_490[0];
    table[1] = lbl_1_rodata_490[1];
    table[2] = lbl_1_rodata_490[2];
    table[3] = lbl_1_rodata_490[3];
    a = (f32) ((3.141592653589793 * (f64) (45.0f + (180.0f + table[idx]))) / 180.0);
    z = 900.0f * sin(a);
    self->pos.x = (f32) (900.0f * cos(a));
    self->pos.y = 0.0f;
    self->pos.z = (f32) z;
    yaw = 90.0f + fn_1_10100(self);
    self->yaw = yaw;
    self->yawTarget = yaw;
    self->yaw = yaw;
    self->dist = 500.0f;
    fn_1_2EF4(&self->model, &self->pos);
    p = lbl_1_data_3F8;
    self->update = p;
    lbl_1_bss_A2C != 0 ? (void) 0 : fn_1_1D4(lbl_1_data_432, lbl_1_data_442, 0x27);
    fn_1_14460(lbl_1_bss_A2C, self->idx, self->charNo);
    lbl_1_bss_A2C != 0 ? (void) 0 : fn_1_1D4(lbl_1_data_432, lbl_1_data_442, 0x27);
    fn_1_14590(lbl_1_bss_A2C, self->idx, 0);
    return self;
}

M579S16Vec *fn_1_CF80(M579S16Vec *self, s16 flag)
{
    M579S16Vec *a;
    M579S16Vec *b;
    s16 **p;
    s16 **q;
    s16 *d;

    if (self != 0) {
        a = self;
        b = a;
        if (self != 0 && a != 0 && b != 0) {
            p = &self->data;
            if (*p != 0) {
                q = &self->data;
                d = *q;
                fn_1_C8(d);
            }
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M579S16Vec *fn_1_D014(M579S16Vec *self, s16 flag)
{
    M579S16Vec *a;
    s16 **p;
    s16 **q;
    s16 *d;

    if (self != 0) {
        a = self;
        if (self != 0 && a != 0) {
            p = &self->data;
            if (*p != 0) {
                q = &self->data;
                d = *q;
                fn_1_C8(d);
            }
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M579S16Vec *fn_1_D0A0(M579S16Vec *self, s16 flag)
{
    s16 **p;
    s16 **q;
    s16 *d;

    if (self != 0) {
        p = &self->data;
        if (*p != 0) {
            q = &self->data;
            d = *q;
            fn_1_C8(d);
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M579Node *fn_1_D124(M579Node *self, s16 flag)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M579Player *fn_1_D17C(M579Player *self, s16 flag)
{
    s16 **vp;
    s16 **vq;
    s16 *vd;

    if (self != 0) {
        if (&self->motion != 0 && &self->motion != 0 && &self->motion != 0) {
            vp = &self->motion.data;
            if (*vp != 0) {
                vq = &self->motion.data;
                vd = *vq;
                fn_1_C8(vd);
            }
        }
        fn_1_30F4(&self->model, -1);
        if (self != 0) {
            fn_1_1270(self, 0);
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

s16 fn_1_D244(M579Player *self, s32 dataNum)
{
    s16 mot;
    s32 n;

    n = dataNum;
    mot = CharMotionCreate(self->charNo, dataNum);
    fn_1_106CC(&self->motion, self->motion.data + self->motion.size, 1, &mot);
    return mot;
}

void fn_1_D2C0(void)
{
    M579Node *n;
    M579Node *a;
    M579Node *b;

    fn_1_1060C(&lbl_1_bss_AD4, fn_1_E01C);
    fn_1_1060C(&lbl_1_bss_AD4, fn_1_E060);
    a = lbl_1_bss_AD4.next;
    b = a;
    n = b;
    if (n != &lbl_1_bss_AD4) {
        fn_1_12DC(n);
        fn_1_1330(n, &lbl_1_bss_AD4);
    }
}

void fn_1_D358(void)
{
    M579Player *players[4];
    s32 scores[4];
    s32 order[4];
    M579Node *n;
    s32 cnt;
    s32 i;
    s32 j;
    s32 k;
    s32 m;
    s32 ties;
    s32 t;

    n = lbl_1_bss_AD4.next;
    lbl_1_bss_AC4 = 0;
    order[0] = lbl_1_rodata_4CC[0];
    order[1] = lbl_1_rodata_4CC[1];
    order[2] = lbl_1_rodata_4CC[2];
    order[3] = lbl_1_rodata_4CC[3];
    cnt = 0;
    while (n != &lbl_1_bss_AD4) {
        players[cnt] = (M579Player *) n;
        scores[cnt] = ((M579Player *) n)->score;
        cnt += 1;
        n = n->next;
    }
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (scores[j] < scores[j + 1]) {
                t = scores[j];
                scores[j] = scores[j + 1];
                scores[j + 1] = t;
                t = order[j];
                order[j] = order[j + 1];
                order[j + 1] = t;
            }
        }
    }
    ties = 1;
    for (k = 1; k < 4; k++) {
        if (scores[0] == scores[k]) {
            ties += 1;
        }
    }
    if (ties != 4 || scores[0] != 0) {
        if (ties == 1) {
            lbl_1_bss_AC4 = players[order[0]];
        }
        for (m = 0; m < ties; m++) {
            players[order[m]]->loser = 1;
            fn_1_255C(players[order[m]]->idx, 1);
        }
    }
}

void fn_1_D594(void)
{
    M579Node *n;
    M579Ptmf p;

    if (lbl_1_bss_AC4 != 0) {
        lbl_1_bss_A08 != 0 ? (void) 0 : fn_1_1D4(lbl_1_data_432, lbl_1_data_442, 0x27);
        fn_1_8FCC(lbl_1_bss_A08, lbl_1_bss_AC4);
    }
    n = lbl_1_bss_AD4.next;
    while (n != &lbl_1_bss_AD4) {
        p = lbl_1_data_450;
        ((M579Player *) n)->update = p;
        ((M579Player *) n)->state = 1;
        n = n->next;
    }
}

void fn_1_D6A4(M579Player *p)
{
    Vec target;
    Vec d;
    f32 mag;
    f32 dist;
    f32 c;
    f32 ac;
    s32 st;

    if (p->fish != 0) {
        fn_1_F3A8(p);
        return;
    }
    mag = PSVECMag(&p->pos);
    target.x = (700.0f * p->pos.x) / mag;
    target.y = (700.0f * p->pos.y) / mag;
    target.z = (700.0f * p->pos.z) / mag;
    PSVECScale(&p->pos, &target, (f32) (700.0 / PSVECMag(&p->pos)));
    PSVECSubtract(&target, &p->pos, &d);
    dist = PSVECMag(&d);
    if (dist > 25.0f) {
        PSVECScale(&d, &d, 1.0f / dist);
        fn_1_F8B0(p, &d);
        if (p->stick > 0.5f) {
            st = 2;
        } else {
            st = 1;
        }
        fn_1_E524(p, st, 0);
        return;
    }
    c = fn_1_C78C(0.0, (f64) p->yaw, 13.666667f);
    p->yaw = (f32) (p->yaw + c);
    if (c < 0.0f) {
        ac = -c;
    } else {
        ac = c;
    }
    if (ac < 0.01f) {
        fn_1_E524(p, 0, 0);
    } else {
        p->state = 0x12;
    }
    p->stick = 0.0f;
}

void fn_1_DA38(M579Player *p)
{
    f32 table[4];
    Vec target;
    Vec d;
    f32 a;
    f32 dist;
    f32 c;
    f32 ac;
    f64 z;
    s32 st;
    M579Ptmf pt;

    if ((f32) p->timer < 240.0f) {
        fn_1_E524(p, 0, 0);
        return;
    }
    table[0] = lbl_1_rodata_500[0];
    table[1] = lbl_1_rodata_500[1];
    table[2] = lbl_1_rodata_500[2];
    table[3] = lbl_1_rodata_500[3];
    a = (f32) ((3.141592653589793 * (f64) (45.0f + (180.0f + table[p->idx]))) / 180.0);
    z = 580.0f * sin(a);
    target.x = (f32) (580.0f * cos(a));
    target.y = 0.0f;
    target.z = (f32) z;
    PSVECSubtract(&target, &p->pos, &d);
    dist = PSVECMag(&d);
    if (dist > 25.0f) {
        PSVECScale(&d, &d, 1.0f / dist);
        fn_1_F8B0(p, &d);
        if (p->stick > 0.5f) {
            st = 2;
        } else {
            st = 1;
        }
        fn_1_E524(p, st, 0);
        return;
    }
    c = fn_1_C78C(0.0, (f64) p->yaw, 13.666667f);
    p->yaw = (f32) (p->yaw + c);
    if (c < 0.0f) {
        ac = -c;
    } else {
        ac = c;
    }
    if (ac < 0.01f) {
        fn_1_E524(p, 0, 0);
        pt = lbl_1_data_45C;
        p->update = pt;
        p->active = 1;
    } else {
        p->state = 0x12;
    }
    p->stick = 0.0f;
}

u8 fn_1_DE88(void)
{
    s16 chars[4];
    M579Node *n;
    M579Player *p;
    M579Ptmf pt;
    u8 flag;
    s32 i;

    n = lbl_1_bss_AD4.next;
    flag = 1;
    i = 0;
    while (n != &lbl_1_bss_AD4) {
        p = (M579Player *) n;
        chars[i] = (s16) p->charNo;
        i += 1;
        if (p->loser != 0) {
            pt = lbl_1_data_468;
            p->update = pt;
            flag = 0;
        } else {
            pt = lbl_1_data_474;
            p->update = pt;
        }
        n = n->next;
    }
    if (flag != 0) {
        CharLoseVoicePlay(chars[0], chars[1], chars[2], chars[3]);
    }
    return flag;
}

void fn_1_DFBC(M579Player *p)
{
    M579Player *q;

    q = p;
    fn_1_E524(p, 5, 1);
}

void fn_1_DFEC(M579Player *p)
{
    M579Player *q;

    q = p;
    fn_1_E524(p, 6, 1);
}

void fn_1_E01C(M579Player *p)
{
#ifdef TARGET_PC
    if (p->update.func != 0) {
        ((void (*)(M579Player *)) p->update.func)(p);
    }
#else
    __ptmf_scall(p, &p->update);
#endif
    p->timer = p->timer + 1;
}

void fn_1_E060(M579Player *p)
{
    if (p->active != 0) {
        fn_1_E15C(p);
        fn_1_FE34(p);
    }
    fn_1_E0AC(p);
}

void fn_1_E0AC(M579Player *p)
{
    Vec rot;

    rot.x = 0.0f;
    rot.y = p->yaw;
    rot.z = 0.0f;
    fn_1_2F3C(&p->model, &rot);
    fn_1_2EF4(&p->model, &p->pos);
    CharModelVoicePanSet(p->charNo, fn_1_9344(&p->pos));
    CharModelVoiceVolSet(p->charNo, fn_1_93E4(&p->pos));
}

void fn_1_E15C(M579Player *p)
{
    M579Node *n;
    M579Player *q;
    f32 dx;
    f32 dz;
    f32 sq;
    f64 dist;
    f64 ox;
    f64 oz;
    f64 mx;
    f64 mz;
    f64 hx;
    f64 hz;

    if (p->u64 != 0) {
        n = lbl_1_bss_AD4.next;
        while (n != &lbl_1_bss_AD4) {
            q = (M579Player *) n;
            if (p != q) {
                dx = q->pos.x - p->pos.x;
                dz = q->pos.z - p->pos.z;
                sq = (f32) (((f64) dx * (f64) dx) + ((f64) dz * (f64) dz));
                dist = sqrt((f64) sq);
                if (!((f32) dist > 130.0) && (0.0 != (f32) dist)) {
                    ox = (130.0 * (f64) dx) / dist;
                    oz = (130.0 * (f64) dz) / dist;
                    if (q->u64 == 0) {
                        p->pos.x = (f32) ((f64) q->pos.x - ox);
                        p->pos.z = (f32) ((f64) q->pos.z - oz);
                    } else {
                        mx = 0.5 * (f64) (q->pos.x + p->pos.x);
                        mz = 0.5 * (f64) (q->pos.z + p->pos.z);
                        hx = ox * 0.5;
                        hz = oz * 0.5;
                        p->pos.x = (f32) (mx - hx);
                        p->pos.z = (f32) (mz - hz);
                        q->pos.x = (f32) (mx + hx);
                        q->pos.z = (f32) (mz + hz);
                    }
                    fn_1_FE34(p);
                    fn_1_FE34(q);
                }
            }
            n = n->next;
        }
    }
}

void fn_1_E524(M579Player *p, s32 idx, u8 flag)
{
    s32 attr;
    s16 model;
    s16 mot;

    model = p->model;
    mot = *fn_1_7528(&p->motion, idx);
    if (p->anim != idx) {
        Hu3DModelAttrReset(model, 0x40000002);
        if (flag == 0) {
            attr = 0x40000001;
        } else {
            attr = 0;
        }
        CharMotionShiftSet(p->charNo, mot, 0.0f, 8.0f, attr);
        p->anim = idx;
    }
}

void fn_1_E5E4(M579Player *p)
{
    M579Ptmf pt;

    p->u70 = 0;
    fn_1_E524(p, 0, 0);
    pt = lbl_1_data_480;
    p->update = pt;
}

void fn_1_E6C8(M579Player *p)
{
    Vec v;
    M579Fish *f;
    M579Ptmf pt;
    u8 act;
    s32 mode;
    s32 st;

    act = 0;
    p->u64 = 1;
    p->u70 = 0;
    if (p->padNo < 0) {
        if (fn_1_11844(&p->pos) != 0) {
            act = 1;
        }
    } else if ((HuPadBtnDown[p->padNo] & 0x100) != 0) {
        act = 1;
    }
    if (act != 0) {
        f = fn_1_11778(&p->pos);
        p->fish = f;
        if (f != 0) {
            p->fish->caught = 1;
            fn_1_13FE0(p->fish);
            p->frame = 0;
            p->u64 = 0;
            mode = p->fish->mode;
            if (mode < 7) {
                p->score = p->score + 1;
            }
            if (mode == 7) {
                p->score = p->score + 3;
            }
            lbl_1_bss_A2C != 0 ? (void) 0 : fn_1_1D4(lbl_1_data_432, lbl_1_data_442, 0x27);
            fn_1_14590(lbl_1_bss_A2C, p->idx, p->score);
            fn_1_9668(&p->pos, 0x95D);
            pt = lbl_1_data_48C;
            p->update = pt;
            fn_1_EA28(p);
            return;
        }
    }
    fn_1_FC6C(p, &v);
    if (fn_1_F8B0(p, &v) != 0) {
        if (p->stick > 0.5f) {
            st = 2;
        } else {
            st = 1;
        }
        fn_1_E524(p, st, 0);
    } else {
        fn_1_E524(p, 0, 0);
    }
}

void fn_1_E9BC(M579Player *p, Mtx out)
{
    f32 (*o)[4];

    o = out;
    Hu3DModelObjMtxGet(p->model, CharModelItemHookGet(p->charNo, 2, 0), o);
}

void fn_1_EA28(M579Player *p)
{
    f32 c;
    M579Ptmf pt;
    M579Fish *f;
    s32 mode;

    p->u70 = 1;
    p->yawTarget = (f32) (180.0 * (atan2(-p->pos.x, -p->pos.z) / 3.141592653589793));
    c = fn_1_C78C((f64) p->yawTarget, (f64) p->yaw, 21.333334f);
    p->yaw = (f32) (p->yaw + c);
    p->frame = p->frame + 1;
    if ((f32) p->frame > 48.0f) {
        p->frame = 0;
        if (p->fish != 0) {
            fn_1_1404C(p->fish, 1);
        }
        p->u70 = 2;
        pt = lbl_1_data_498;
        p->update = pt;
        fn_1_E524(p, 4, 1);
        return;
    }
    if ((f32) p->frame < 12.0f) {
        Hu3DMotionTimeSet(p->model, 0.0f);
    } else {
        f = p->fish;
        f->held = 1;
        f->holder = p;
    }
    if (p->fish != 0 && (f32) p->frame < 30.0f) {
        mode = p->fish->mode;
        if (mode < 7) {
            omVibrate((s16) p->idx, 0xC, 6, 6);
        }
        if (mode == 7) {
            omVibrate((s16) p->idx, 0xC, 0xC, 0);
        }
    }
    fn_1_E524(p, 3, 1);
}

void fn_1_EEF4(M579Player *p)
{
    M579Ptmf pt;
    M579Fish *f;

    p->u70 = 2;
    p->frame = p->frame + 1;
    if ((f32) p->frame > 60.0f) {
        p->frame = 0;
        pt = lbl_1_data_4A4;
        p->update = pt;
        f = p->fish;
        f->held = 0;
        f->b1E = 0;
        f->mode = 8;
        f->caught = 0;
        p->fish = 0;
    }
    fn_1_E524(p, 4, 1);
}

void fn_1_F050(M579Player *p)
{
    f32 c;
    M579Fish *f;

    p->yawTarget = (f32) (180.0 * (atan2(-p->pos.x, -p->pos.z) / 3.141592653589793));
    c = fn_1_C78C((f64) p->yawTarget, (f64) p->yaw, 21.333334f);
    p->yaw = (f32) (p->yaw + c);
    if ((f32) p->frame > 48.0f) {
        p->frame = 0;
        p->u70 = 2;
        if (p->fish != 0) {
            fn_1_1404C(p->fish, 1);
        }
    } else if ((f32) p->frame < 12.0f) {
        Hu3DMotionTimeSet(p->model, 0.0f);
    } else {
        f = p->fish;
        f->held = 1;
        f->holder = p;
    }
    fn_1_E524(p, 3, 1);
}

void fn_1_F3A8(M579Player *p)
{
    f32 c;
    M579Fish *f;

    p->frame = p->frame + 1;
    if (p->u70 == 1) {
        p->yawTarget = (f32) (180.0 * (atan2(-p->pos.x, -p->pos.z) / 3.141592653589793));
        c = fn_1_C78C((f64) p->yawTarget, (f64) p->yaw, 21.333334f);
        p->yaw = (f32) (p->yaw + c);
        if ((f32) p->frame > 48.0f) {
            p->frame = 0;
            p->u70 = 2;
            if (p->fish != 0) {
                fn_1_1404C(p->fish, 1);
            }
        } else if ((f32) p->frame < 12.0f) {
            Hu3DMotionTimeSet(p->model, 0.0f);
        } else {
            f = p->fish;
            f->held = 1;
            f->holder = p;
        }
        fn_1_E524(p, 3, 1);
        return;
    }
    if ((f32) p->frame > 60.0f) {
        p->frame = 0;
        f = p->fish;
        f->held = 0;
        f->b1E = 0;
        f->mode = 8;
        f->caught = 0;
        p->fish = 0;
    }
    fn_1_E524(p, 4, 1);
}

void fn_1_F810(M579Player *p)
{
    fn_1_E524(p, 5, 1);
}

u8 fn_1_F8B0(M579Player *p, Vec *dir)
{
    Vec fwd;
    Vec mv;
    f32 speed;
    f32 c;
    f32 a;
    f32 dot;

    p->stick = PSVECMag(dir);
    if (p->stick < 0.01f) {
        Hu3DMotionSpeedSet(p->model, 1.0f);
        return 0;
    }
    speed = p->stick;
    if (p->stick < 0.5) {
        speed = 2.0f * p->stick;
    }
    Hu3DMotionSpeedSet(p->model, speed);
    p->yawTarget = (f32) (180.0 * (atan2(dir->x, dir->z) / 3.141592653589793));
    c = fn_1_C78C((f64) p->yawTarget, (f64) p->yaw, 13.666667f);
    p->yaw = (f32) (p->yaw + c);
    a = (f32) ((3.141592653589793 * (f64) p->yaw) / 180.0);
    fwd.z = (f32) cos(a);
    fwd.x = (f32) sin(a);
    fwd.y = 0.0f;
    dot = PSVECDotProduct(&fwd, dir);
    if (dot < 0.0f) {
        return 1;
    }
    PSVECScale(&fwd, &mv, (f32) (7.0f * (1.6666666666666667 * (f64) dot)) * p->stick);
    PSVECAdd(&mv, &p->pos, &p->pos);
    return 1;
}

void fn_1_FC6C(M579Player *p, Vec *out)
{
    out->x = 0.0f;
    out->y = 0.0f;
    out->z = 0.0f;
    if (fn_1_1F30() == 2) {
        if (p->padNo < 0) {
            fn_1_101C8(p, out);
            if (p->dif == 0) {
                PSVECScale(out, out, 0.7f);
            }
            if (p->dif == 1) {
                PSVECScale(out, out, 0.85f);
            }
        } else {
            out->x = 0.017857144f * (f32) HuPadStkX[p->padNo];
            out->y = 0.0f;
            out->z = -(0.017857144f * (f32) HuPadStkY[p->padNo]);
        }
    }
}

s32 fn_1_FE34(M579Player *p)
{
    f32 x;
    f32 z;
    f32 sq;
    f64 dist;

    x = p->pos.x;
    z = p->pos.z;
    sq = (f32) (((f64) x * (f64) x) + ((f64) z * (f64) z));
    dist = sqrt((f64) sq);
    p->dist = (f32) dist;
    if ((f32) dist < 580.0) {
        p->pos.x = (f32) (580.0 * ((f64) x / dist));
        p->pos.z = (f32) (580.0 * ((f64) z / dist));
        p->dist = 580.0f;
        return 0;
    }
    if ((f32) dist > 750.0) {
        p->pos.x = (f32) (750.0 * ((f64) x / dist));
        p->pos.z = (f32) (750.0 * ((f64) z / dist));
        p->dist = 750.0f;
        return 1;
    }
    return 0;
}

f32 fn_1_10100(M579Player *p)
{
    f32 a;
    f32 x;
    f32 z;

    x = p->pos.x;
    z = p->pos.z;
    a = (f32) (90.0 + (180.0 * (atan2(x, z) / 3.141592653589793)));
    if (a > 180.0f) {
        a -= 360.0f;
    }
    return a;
}

void fn_1_101C8(M579Player *p, Vec *dir)
{
    M579Fish *t;
    f32 a;
    f32 var;
    f32 r;
    f64 s;

    t = fn_1_11990(&p->pos);
    if (t == p->target) {
        p->count = p->count + 1;
    } else {
        p->target = t;
        p->count = 0;
    }
    if ((f32) (f64) p->count < (f32) ((f64) (4 - p->dif) * 30.0)) {
        dir->x = 0.0f;
        dir->y = 0.0f;
        dir->z = 0.0f;
        return;
    }
    if (t == 0) {
        dir->x = 0.0f;
        dir->y = 0.0f;
        dir->z = 0.0f;
        p->count = 0;
        return;
    }
    if (fn_1_11C34(t, &p->pos) < 300.0f) {
        PSVECSubtract(&t->pos, &p->pos, dir);
        PSVECScale(dir, dir, 1.0f / PSVECMag(dir));
        return;
    }
    a = (f32) (180.0 * (atan2(p->pos.x, p->pos.z) / 3.141592653589793));
    if (fn_1_C6D4((f64) (f32) (180.0 * (atan2(t->pos.x, t->pos.z) / 3.141592653589793)), (f64) a) > 0.0f) {
        var = a;
    } else {
        var = 180.0f + a;
        if (var > 180.0f) {
            var -= 360.0f;
        }
    }
    r = (f32) ((3.141592653589793 * (f64) var) / 180.0);
    s = -sin(r);
    dir->x = (f32) cos(r);
    dir->y = 0.0f;
    dir->z = (f32) s;
}

void fn_1_1060C(M579Node *list, void (*fn)(M579Player *))
{
    M579Node *l;
    M579Node *first;
    M579Node *n;

    l = list;
    first = l->next;
    n = first;
    while (n != l) {
        fn((M579Player *) n);
        n = n->next;
    }
}

void fn_1_1066C(M579Node *first, M579Node *list, void (*fn)(M579Player *))
{
    M579Node *n;

    n = first;
    while (n != list) {
        fn((M579Player *) n);
        n = n->next;
    }
}

void fn_1_106CC(M579S16Vec *vec, s16 *pos, u32 n, s16 *val)
{
    M579S16Vec *a0;
    M579S16Vec *a1;
    M579S16Vec *a2;
    M579S16Vec *a3;
    M579S16Vec *a4;
    M579S16Vec *a5;
    s16 **d0;
    s16 **d1;
    s16 **d2;
    s16 **d3;
    s16 **d4;
    s16 *base;
    s16 *endp;
    s16 *src;
    s16 *dst;
    s16 *old;
    s16 *buf;
    s16 *fillDst;
    u32 maxn;
    volatile u32 maxn2;
    volatile u32 maxn3;
    u32 cap;
    u32 cap0;
    u32 newSize;
    u32 k;
    u32 k2;
    s32 tail;
    s32 idx;
    s32 cnt;
    s32 cnt2;
    s32 s60;
    s32 s5c;
    s32 s58;
    s16 *s6c;
    s16 *s64;
    s16 *s68;

    if (n != 0) {
        a0 = vec;
        maxn = 0x7FFFFFFF;
        maxn2 = maxn;
        maxn3 = maxn;
        if (n > maxn3 || vec->size > maxn3 - n) {
            fn_1_234(M579_STDERR, lbl_1_data_4CE);
            abort();
        }
        a1 = vec;
        cap0 = a1->cap;
        if (vec->size + n <= cap0) {
            d0 = &vec->data;
            endp = *d0 + vec->size;
            tail = (s32) (endp - pos);
            src = val;
            if (endp - (tail - n) <= src && src < endp) {
                src = src + n;
            }
            if (tail != 0) {
                cnt = (s32) (endp - pos);
                s60 = cnt;
                memmove(endp + n - cnt, pos, cnt * 2);
            }
            k = n;
            dst = pos;
            while (k != 0) {
                *dst = *src;
                dst++;
                k--;
            }
            vec->size = vec->size + n;
            return;
        }
        d1 = &vec->data;
        base = *d1;
        idx = (s32) (pos - base);
        d2 = &vec->data;
        old = *d2;
        newSize = vec->size + n;
        a2 = vec;
        if (a2->cap != 0) {
            a3 = vec;
            cap = a3->cap;
        } else {
            cap = 1;
        }
        while (newSize > cap) {
            if (cap < (maxn3 >> 1)) {
                cap *= 2;
            } else {
                cap = maxn3;
            }
        }
        d3 = &vec->data;
        a4 = vec;
        buf = (s16 *) fn_1_0(cap * 2);
        if (buf == 0) {
            fn_1_234(M579_STDERR, lbl_1_data_4EB);
            abort();
        }
        *d3 = buf;
        k2 = n;
        d4 = &vec->data;
        fillDst = *d4 + idx;
        while (k2 != 0) {
            *fillDst = *val;
            fillDst++;
            k2--;
        }
        if (old != 0) {
            cnt2 = idx;
            s5c = cnt2;
            s6c = vec->data;
            memcpy(vec->data, old, cnt2 * 2);
            s64 = vec->data + idx + n;
            s68 = old + vec->size;
            cnt = (s32) (((u8 *) (old + vec->size) - (u8 *) (old + idx)) / 2);
            s58 = cnt;
            memcpy(vec->data + idx + n, old + idx, cnt * 2);
            fn_1_C8(old);
        }
        vec->size = newSize;
        a5 = vec;
        a5->cap = cap;
    }
}

void fn_1_10A7C(M579S16Vec *vec, u32 n)
{
    M579S16Vec *a;
    M579S16Vec *b;
    u32 maxn;
    volatile u32 maxn2;
    volatile u32 maxn3;
    M579S16Vec *c;
    s16 *old;
    s16 *buf;
    s32 cnt;
    s32 s14;

    c = vec;
    if (n > c->cap) {
        maxn = 0x7FFFFFFF;
        maxn2 = maxn;
        maxn3 = maxn2;
        if (n > maxn3) {
            fn_1_234(M579_STDERR, lbl_1_data_505);
            abort();
        }
        old = vec->data;
        a = vec;
        b = a;
        buf = (s16 *) fn_1_0(n * 2);
        if (buf == 0) {
            fn_1_234(M579_STDERR, lbl_1_data_4EB);
            abort();
        }
        vec->data = buf;
        if (old != 0) {
            cnt = (s32) (((u8 *) (old + vec->size) - (u8 *) old) / 2);
            s14 = cnt;
            memcpy(vec->data, old, cnt * 2);
            fn_1_C8(old);
        }
        vec->cap = n;
    }
}

void fn_1_10BA4(void)
{
    M579Node *a;
    M579Node *b;

    lbl_1_bss_AB8 = 0x78;
    lbl_1_bss_ABC = 0xD2;
    lbl_1_bss_AC0 = 0xD2;
    a = &lbl_1_bss_AD4;
    fn_1_1264(a);
    b = a;
#ifndef TARGET_PC
    __register_global_object(b, (void *) fn_1_10C2C, lbl_1_bss_AC8);
#endif
}

M579Node *fn_1_10C2C(M579Node *self, s16 flag)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}
