#include "REL/m509Dll/m509.h"
extern "C" {
#include "game/gamework.h"
#include "game/pad.h"
}
#include <math.h>
#include <string.h>

#ifdef TARGET_PC
#include <stdio.h>
#define PLAYER_STDERR ((void *)stderr)
#else
extern "C" char __files[];
#define PLAYER_STDERR ((void *)(__files + 0xA0))
#endif

extern "C" {
void abort(void);
void *memmove(void *dst, const void *src, unsigned long n);
int fn_1_60(void *file, const char *fmt, ...);
void PSVECAdd(const Vec *a, const Vec *b, Vec *ab);
void PSVECSubtract(const Vec *a, const Vec *b, Vec *a_b);
void PSVECScale(const Vec *src, Vec *dst, f32 scale);
f32 PSVECMag(const Vec *v);
f32 PSVECDotProduct(const Vec *a, const Vec *b);
void __ptmf_scall(void);
void __register_global_object(void *obj, void *(*dtor)(void *, s16), void *reg);
extern u8 lbl_1_bss_1C[];
extern void *lbl_1_bss_48;
extern void *lbl_1_bss_34;
extern void *lbl_1_bss_44;
void omVibrate(s16 playerNo, s16 duration, s16 off, s16 on);
s32 HuAudFXPlayVolPan(s32 seId, s16 vol, s16 pan);
void CharLoseVoicePlay(s16 a, s16 b, s16 c, s16 d);
void CharModelVoicePanSet(s16 charNo, s16 pan);
void CharModelVoiceVolSet(s16 charNo, s16 vol);
s16 CharMotionCreate(s16 charNo, u32 dataNum);
void CharMotionDataClose(s16 charNo);
void CharMotionShiftSet(s16 charNo, s16 motId, u32 attr, f32 start, f32 end);
s32 CharNpcDustVoiceOffSet(s16 modelId, s16 motId, s16 type);
s16 Hu3DGLightCreateV(Vec *pos, Vec *dir, GXColor *color);
void Hu3DGLightInfinitytSet(s16 light);
void Hu3DLighInit(void);
void Hu3DMotionSpeedSet(s16 modelId, f32 speed);
s32 fn_1_318C(s32 player);
s32 fn_1_3B70(void);
s16 *fn_1_3C40(s16 *model, s32 charNo, s32 mode);
void fn_1_73A4(void *maze, s8 param);
void fn_1_BD44(void *maze, s32 val, f32 x, f32 z);
u8 fn_1_BE30(void *maze, f32 x, f32 z);
u8 fn_1_BEE8(void *maze, f32 x, f32 z);
s32 fn_1_C000(void *maze, f32 x, f32 z);
void *fn_1_C924(void *self, s32 *icons);
s32 fn_1_E1A0(Vec *pos);
s32 fn_1_E4B4(Vec *pos);
void fn_1_E6E0(void *camera);
void fn_1_EC0(void *node);
void *fn_1_104(s32 size);
}





extern "C" {
void *fn_1_EB14(PlayerMgr *self);
void *fn_1_EC9C(PlayerMgr *self, s16 flag);
void fn_1_ED28(void);
void fn_1_ED2C(PlayerMgr *self, s32 bit);
void *fn_1_ED54(PlayerMgr *self, s16 flag);
f32 fn_1_EE38(f32 a, f32 b);
f32 fn_1_EEF0(f32 a, f32 b, f32 limit);
void fn_1_F054(Vec *out, s32 idx);
void fn_1_F0F4(void);
void *fn_1_F1FC(Player *self, s32 idx);
void *fn_1_F568(PlayerVec *vec, s16 flag);
void *fn_1_F5FC(PlayerVec *vec, s16 flag);
void *fn_1_F688(PlayerVec *vec, s16 flag);
void *fn_1_F70C(Player *self, s16 flag);
s16 fn_1_F764(Player *self, s32 dataNum);
void fn_1_F7E0(void);
s32 fn_1_F864(void);
void fn_1_F924(void);
void fn_1_FA3C(void);
void fn_1_FAA0(Player *self);
u8 fn_1_FDB0(void);
void fn_1_FF00(Player *self);
void fn_1_FF30(Player *self);
void fn_1_FF60(void);
void fn_1_FFFC(Vec *out);
void fn_1_101E4(Player *self);
void fn_1_102D4(Player *self);
void fn_1_10608(Player *self, s32 motion, u8 flag);
void fn_1_106C8(Player *self, u8 on);
void fn_1_10774(Player *self);
void fn_1_10850(Player *self);
void fn_1_10B34(Player *self);
u8 fn_1_10BD4(Player *self, Vec *dir);
s32 fn_1_10F78(Player *self, f32 x, f32 z);
u8 fn_1_11020(Player *self);
u8 fn_1_111C8(f32 x, f32 z);
void fn_1_11394(Player *self, Vec *out);
void fn_1_11D1C(Player *self);
void fn_1_11EA8(Player *self);
void fn_1_123E4(Player *self, s32 i, s32 j, f32 ox, f32 oz, f32 fx, f32 fz);
s16 *fn_1_126CC(PlayerVec *vec, u32 idx);
void fn_1_12744(PlayerNode *head, void (*fn)(Player *));
void fn_1_127A4(PlayerNode *first, PlayerNode *end, void (*fn)(Player *));
void fn_1_12804(PlayerVec *vec, s16 *pos, u32 n, s16 *val);
void fn_1_12BB4(PlayerVec *vec, u32 n);
void fn_1_12CDC(void);
void *fn_1_12D64(PlayerNode *self, s16 flag);
}

Vec lbl_1_data_3438 = { 0.0f, -1000.0f, 0.0f };
Vec lbl_1_data_3444 = { 0.0f, 10000.0f, 0.0f };
GXColor lbl_1_data_3450 = { 0xFF, 0xFF, 0xFF, 0xFF };
f32 lbl_1_data_3454[9] = { 2000.0f, 6000.0f, 2000.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f };
char lbl_1_data_3478[] = "ms_pointer == 0";
char lbl_1_data_3488[] = "singleton.h";
char lbl_1_data_3494[] = "ms_pointer != 0";
void *lbl_1_data_34A4[5] = { 0, 0, (void *)fn_1_ED54, (void *)fn_1_ED2C, (void *)fn_1_ED28 };
M509Ptmf lbl_1_data_34B8 = { 0, -1, (void *)fn_1_10774 };
char lbl_1_data_34C4[] = "m_mot_id.size() == NUMBEROF_MOTION";
char lbl_1_data_34E7[] = "player.cpp";
M509Ptmf lbl_1_data_34F4 = { 0, -1, (void *)fn_1_FAA0 };
char lbl_1_data_3500[] = "ms_pointer != 0";
char lbl_1_data_3510[] = "singleton.h";
M509Ptmf lbl_1_data_351C = { 0, -1, (void *)fn_1_FF00 };
M509Ptmf lbl_1_data_3528 = { 0, -1, (void *)fn_1_FF30 };
M509Ptmf lbl_1_data_3534 = { 0, -1, (void *)fn_1_10850 };
char lbl_1_data_3540[] = "vector::at index out of range";
char lbl_1_data_355E[] = "vector::insert length error\n";
char lbl_1_data_357B[] = "Memory allocation failure";
char lbl_1_data_3595[] = "vector::reserve length error\n";

s32 lbl_1_bss_88;
s32 lbl_1_bss_8C;
s32 lbl_1_bss_90;
Player *lbl_1_bss_94;
u8 lbl_1_bss_98[12];
PlayerNode lbl_1_bss_A4;

XX_RODATA const Vec lbl_1_rodata_400 = { 0.0f, 125.0f, 0.0f };

#define PLAYER_ASSERT(c_, t_, f_, l_) ((c_) ? (void)0 : fn_1_0(t_, f_, l_))

#ifdef TARGET_PC
#define PLAYER_CALL_STATE(p_) ((void (*)(Player *))(p_)->state.func)(p_)
#else
#define PLAYER_CALL_STATE(p_) __ptmf_scall()
#endif

void *fn_1_EB14(PlayerMgr *self)
{
    s16 m;
    s32 mot;
    u8 *q;

    fn_1_E48(self);
    self->base.vtbl = (void **)lbl_1_data_1C4;
    self->base.flags = 0;
    fn_1_F14(self, lbl_1_bss_1C);
    PLAYER_ASSERT(lbl_1_bss_44 == 0, lbl_1_data_3478, lbl_1_data_3488, 0x35);
    q = (u8 *)self + 0x10;
    if (q != 0) {
        q -= 0x10;
    }
    lbl_1_bss_44 = q;
    self->base.vtbl = lbl_1_data_34A4;
    fn_1_3BBC(&self->model[0], 0x430000);
    fn_1_3BBC(&self->model[1], 0x430001);
    Hu3DLighInit();
    Hu3DGLightInfinitytSet(Hu3DGLightCreateV(&lbl_1_data_3438, &lbl_1_data_3444, &lbl_1_data_3450));
    m = self->model[0];
    Hu3DModelLayerSet(m, 2);
    mot = Hu3DMotionIDGet(m);
    Hu3DModelAttrSet(m, 0x40000001);
    Hu3DMotionSet(m, mot);
    m = self->model[1];
    Hu3DModelLayerSet(m, 0);
    mot = Hu3DMotionIDGet(m);
    Hu3DModelAttrSet(m, 0x40000001);
    Hu3DMotionSet(m, mot);
    return self;
}

void *fn_1_EC9C(PlayerMgr *self, s16 flag)
{
    if (self != 0) {
        PLAYER_ASSERT(lbl_1_bss_44 != 0, lbl_1_data_3494, lbl_1_data_3488, 0x3D);
        lbl_1_bss_44 = 0;
        if (flag > 0) {
            fn_1_21C(self);
        }
    }
    return self;
}

void fn_1_ED28(void)
{
}

void fn_1_ED2C(PlayerMgr *self, s32 bit)
{
    self->base.flags &= ~(1 << bit);
    if (bit < 5) {
    }
}

void *fn_1_ED54(PlayerMgr *self, s16 flag)
{
    if (self != 0) {
        self->base.vtbl = lbl_1_data_34A4;
        fn_1_401C(&self->model[1], -1);
        fn_1_401C(&self->model[0], -1);
        if ((u8 *)self + 0x10 != 0) {
            PLAYER_ASSERT(lbl_1_bss_44 != 0, lbl_1_data_3494, lbl_1_data_3488, 0x3D);
            lbl_1_bss_44 = 0;
        }
        if (self != 0) {
            self->base.vtbl = (void **)lbl_1_data_1C4;
            if (self != 0) {
                fn_1_E54(self, 0);
            }
        }
        if (flag > 0) {
            fn_1_21C(self);
        }
    }
    return self;
}

f32 fn_1_EE38(f32 a, f32 b)
{
    s32 sa = (s32)(182.04444444444445 * a);
    s32 ia = sa;
    s32 sb = (s32)(182.04444444444445 * b);
    s32 ib = sb;
    s32 d = ia - ib;
    s32 w = (u16)(d - 0x8000) - 0x8000;
    s32 w2 = w;
    return 0.0054931640625 * w2;
}

f32 fn_1_EEF0(f32 a, f32 b, f32 limit)
{
    f32 d;
    f32 ret;
    f32 r1 = fn_1_EE38(a, b);
    f32 r2 = r1;
    f32 r3 = r2;

    d = r3;
    if (d < -limit) {
        ret = -limit;
    } else if (d > limit) {
        ret = limit;
    } else {
        ret = d;
    }
    return ret;
}

void fn_1_F054(Vec *out, s32 idx)
{
    f32 x = 1540.0f;
    f32 z = 1100.0f;

    switch (idx) {
    case 0:
        z = -1100.0f;
        x = -1540.0f;
        break;
    case 1:
        z = -1100.0f;
        break;
    case 2:
        x = -1540.0f;
        break;
    }
    out->x = x;
    out->y = 125.0f;
    out->z = z;
}

void fn_1_F0F4(void)
{
    Player *p;

    while (!(lbl_1_bss_A4.next == &lbl_1_bss_A4 && lbl_1_bss_A4.next == lbl_1_bss_A4.prev)) {
        p = (Player *)lbl_1_bss_A4.prev;
        if (p != 0) {
            PlayerVec *v = &p->mot;
            s16 **pa;
            s16 **pb;

            if (v != 0 && v != 0 && v != 0) {
                pa = &v->data;
                if (*pa != 0) {
                    pb = &v->data;
                    fn_1_21C(*pb);
                }
            }
            fn_1_401C(&p->model, -1);
            if (p != 0) {
                fn_1_E54(p, 0);
            }
            fn_1_21C(p);
        }
    }
}

void *fn_1_F1FC(Player *self, s32 idx)
{
    M509Ptmf tmp;
    s16 pad;
    f32 *unused;

    fn_1_E48(self);
    self->act = 0;
    fn_1_3C40(&self->model, GwPlayerConf[idx].charNo, 1);
    self->mot.cap = 0;
    self->mot.size = 0;
    self->mot.data = 0;
    fn_1_F14(self, &lbl_1_bss_A4);
    self->idx = idx;
    self->charNo = GwPlayerConf[idx].charNo;
    self->dif = GwPlayerConf[idx].dif;
    if (GwPlayerConf[idx].type != 0) {
        pad = -1;
    } else {
        pad = GwPlayerConf[idx].padNo;
    }
    self->pad = pad;
    self->targetRotY = 0.0f;
    self->rotY = 0.0f;
    self->unk20 = 0;
    self->motion = -1;
    self->pushTimer = 0;
    self->stunTimer = 0;
    self->cellKey = -1;
    self->inGoal = 0;
    self->blink = 0;
    self->frame = 0;
    self->aiTimer = 0;
    self->mot.size = 0;
    fn_1_12BB4(&self->mot, 6);
    fn_1_F764(self, 0x8C0000);
    CharNpcDustVoiceOffSet(self->model, fn_1_F764(self, 0x8C0001), 0);
    CharNpcDustVoiceOffSet(self->model, fn_1_F764(self, 0x8C0002), 1);
    fn_1_F764(self, 0x8C0006);
    fn_1_F764(self, 0x8C0031);
    fn_1_F764(self, 0x8C0024);
    PLAYER_ASSERT(self->mot.size == 6, lbl_1_data_34C4, lbl_1_data_34E7, 0x94);
    CharMotionDataClose(self->charNo);
    Hu3DModelLayerSet(self->model, 3);
    Hu3DModelCameraSet(self->model, 1);
    fn_1_F054(&self->pos, idx);
    fn_1_3E1C(&self->model, &self->pos);
    tmp = lbl_1_data_34B8;
    self->state = tmp;
    return self;
}

void *fn_1_F568(PlayerVec *vec, s16 flag)
{
    s16 **pa;
    s16 **pb;
    s16 *d;

    if (vec != 0) {
        pa = &vec->data;
        if (vec != 0 && vec != 0 && *pa != 0) {
            pb = &vec->data;
            d = *pb;
            fn_1_21C(d);
        }
        if (flag > 0) {
            fn_1_21C(vec);
        }
    }
    return vec;
}

void *fn_1_F5FC(PlayerVec *vec, s16 flag)
{
    s16 **pa;
    s16 **pb;
    s16 *d;

    if (vec != 0) {
        pa = &vec->data;
        if (vec != 0 && *pa != 0) {
            pb = &vec->data;
            d = *pb;
            fn_1_21C(d);
        }
        if (flag > 0) {
            fn_1_21C(vec);
        }
    }
    return vec;
}

void *fn_1_F688(PlayerVec *vec, s16 flag)
{
    s16 **pa;
    s16 **pb;
    s16 *d;

    if (vec != 0) {
        pa = &vec->data;
        if (*pa != 0) {
            pb = &vec->data;
            d = *pb;
            fn_1_21C(d);
        }
        if (flag > 0) {
            fn_1_21C(vec);
        }
    }
    return vec;
}

void *fn_1_F70C(Player *self, s16 flag)
{
    if (self != 0) {
        fn_1_E54(self, 0);
        if (flag > 0) {
            fn_1_21C(self);
        }
    }
    return self;
}

s16 fn_1_F764(Player *self, s32 dataNum)
{
    s16 id = CharMotionCreate(self->charNo, dataNum);
    s16 **pd = &self->mot.data;
    s16 *end = *pd + self->mot.size;
    fn_1_12804(&self->mot, end, 1, &id);
    return id;
}

void fn_1_F7E0(void)
{
    PlayerNode *first;
    PlayerNode *node;

    fn_1_12744(&lbl_1_bss_A4, fn_1_101E4);
    first = lbl_1_bss_A4.next;
    node = first;
    if (node != &lbl_1_bss_A4) {
        first = node;
        fn_1_EC0(first);
        fn_1_F14(first, &lbl_1_bss_A4);
    }
}

s32 fn_1_F864(void)
{
    Player *p;
    s32 count;

    p = (Player *)lbl_1_bss_A4.next;
    lbl_1_bss_94 = 0;
    count = 0;
    while (p != (Player *)&lbl_1_bss_A4) {
        if (p->inGoal != 0) {
            fn_1_318C(p->idx);
            lbl_1_bss_94 = p;
            count++;
        }
        p = (Player *)p->node.next;
    }
    if (count == 0) {
        return 0;
    }
    if (count != 1) {
        lbl_1_bss_94 = 0;
    }
    return 1;
}

void fn_1_F924(void)
{
    Player *p;
    M509Ptmf tmp;

    if (lbl_1_bss_94 != 0) {
        PLAYER_ASSERT(lbl_1_bss_48 != 0, lbl_1_data_3500, lbl_1_data_3510, 0x27);
        fn_1_E6E0(lbl_1_bss_48);
    }
    p = (Player *)lbl_1_bss_A4.next;
    while (p != (Player *)&lbl_1_bss_A4) {
        Hu3DModelAttrReset(p->model, 1);
        p->blink = 0;
        tmp = lbl_1_data_34F4;
        p->state = tmp;
        p->act = 1;
        p = (Player *)p->node.next;
    }
}

void fn_1_FA3C(void)
{
    PlayerNode *first = lbl_1_bss_A4.next;
    PlayerNode *it = first;
    PlayerNode *end = &lbl_1_bss_A4;
    PlayerNode *nx;

    while (it != end) {
        ((Player *)it)->cellKey = -1;
        nx = it->next;
        it = nx;
    }
}

void fn_1_FAA0(Player *self)
{
    Vec target;
    Vec dir;
    f32 mag;
    f32 step;
    f32 absStep;

    if (lbl_1_bss_94 != 0 && self->inGoal != 0) {
        target = lbl_1_rodata_400;
        PSVECSubtract(&target, &self->pos, &dir);
        mag = PSVECMag(&dir);
        self->speed = 0.6f;
        if (mag > 10.0f) {
            PSVECScale(&dir, &dir, 1.0f / mag);
            fn_1_10BD4(self, &dir);
            fn_1_10608(self, 2, 0);
            return;
        }
        self->pos.x = 0.0f;
        self->pos.z = 0.0f;
    }
    step = fn_1_EEF0(0.0f, self->rotY, 13.666667f);
    self->rotY = self->rotY + step;
    if (step < 0.0f) {
        absStep = -step;
    } else {
        absStep = step;
    }
    if (absStep < 0.01f) {
        fn_1_10608(self, 0, 0);
    } else {
        self->act = 0x12;
    }
    self->speed = 0.0f;
}

u8 fn_1_FDB0(void)
{
    Player *p;
    s16 chars[4];
    u8 lose;
    s32 n;
    M509Ptmf tmp;

    p = (Player *)lbl_1_bss_A4.next;
    lose = 1;
    n = 0;
    while (p != (Player *)&lbl_1_bss_A4) {
        chars[n++] = p->charNo;
        Hu3DModelAttrReset(p->model, 1);
        p->blink = 0;
        p->stunTimer = 0;
        if (p->inGoal != 0) {
            tmp = lbl_1_data_351C;
            p->state = tmp;
            lose = 0;
        } else {
            tmp = lbl_1_data_3528;
            p->state = tmp;
        }
        p = (Player *)p->node.next;
    }
    if (lose != 0) {
        CharLoseVoicePlay(chars[0], chars[1], chars[2], chars[3]);
    }
    return lose;
}

void fn_1_FF00(Player *self)
{
    fn_1_10608(self, 3, 1);
}

void fn_1_FF30(Player *self)
{
    fn_1_10608(self, 4, 1);
}

void fn_1_FF60(void)
{
    Player *p;
    s32 chars[4];
    s32 n;
    void *obj;

    p = (Player *)lbl_1_bss_A4.next;
    n = 0;
    while (p != (Player *)&lbl_1_bss_A4) {
        chars[n++] = p->charNo;
        p = (Player *)p->node.next;
    }
    obj = fn_1_104(sizeof(StartSpr));
    if (obj != 0) {
        fn_1_C924(obj, chars);
    }
}

void fn_1_FFFC(Vec *out)
{
    Player *p;
    f32 maxX;
    f32 minX;
    f32 maxZ;
    f32 minZ;
    f32 px;
    f32 pz;
    f32 t;
    f32 w;
    f32 h;

    p = (Player *)lbl_1_bss_A4.next;
    maxX = 770.0f;
    minX = -770.0f;
    maxZ = 880.0f;
    minZ = -1100.0f;
    while (p != (Player *)&lbl_1_bss_A4) {
        px = p->pos.x;
        pz = p->pos.z;
        t = 550.0 + px;
        if (t > maxX) {
            maxX = t;
        }
        t = px - 550.0;
        if (t < minX) {
            minX = t;
        }
        t = 660.0f + pz;
        if (t > maxZ) {
            maxZ = t;
        }
        t = pz - 880.0f;
        if (t < minZ) {
            minZ = t;
        }
        p = (Player *)p->node.next;
    }
    out->x = (maxX + minX) * 0.5f;
    out->z = (maxZ + minZ) * 0.5f;
    w = maxX - minX;
    h = maxZ - minZ;
    if (w > h) {
        out->y = w;
        return;
    }
    out->y = h;
}

void fn_1_101E4(Player *self)
{
    Vec rot;

    self->prev.x = self->pos.x;
    self->prev.y = self->pos.y;
    self->prev.z = self->pos.z;
    PLAYER_CALL_STATE(self);
    self->frame = self->frame + 1;
    fn_1_102D4(self);
    fn_1_11EA8(self);
    rot.x = 0.0f;
    rot.y = self->rotY;
    rot.z = 0.0f;
    fn_1_3E64(&self->model, &rot);
    fn_1_3E1C(&self->model, &self->pos);
    CharModelVoicePanSet(self->charNo, fn_1_E4B4(&self->pos));
    CharModelVoiceVolSet(self->charNo, fn_1_E1A0(&self->pos));
}

void fn_1_102D4(Player *self)
{
    Player *other;
    Vec savePos;
    Vec delta;
    Vec shiftSelf;
    Vec shiftOther;
    f32 dist;
    f32 half;

    savePos = self->pos;
    if (self->pushTimer > 0) {
        self->pushTimer = self->pushTimer - 1;
    }
    other = (Player *)lbl_1_bss_A4.next;
    while (other != (Player *)&lbl_1_bss_A4) {
        if (self != other) {
            Vec otherPos = other->pos;

            PSVECSubtract(&other->pos, &self->pos, &delta);
            dist = PSVECMag(&delta);
            if (dist >= 120.0f) {
            } else {
                half = 1.0f - 0.5f;
                PSVECScale(&delta, &delta, (120.0f - dist) / dist);
                PSVECScale(&delta, &shiftSelf, 1.05f * -0.5f);
                PSVECScale(&delta, &shiftOther, 1.05f * half);
                PSVECAdd(&shiftSelf, &self->pos, &self->pos);
                PSVECAdd(&shiftOther, &other->pos, &other->pos);
                if (half >= 0.5f) {
                    other->pushTimer = 5;
                } else {
                    other->pushTimer = 0;
                }
                if ((s32)(((1650.0f + otherPos.x) / 100.0f) / 2.2f) != (s32)(((1650.0f + other->pos.x) / 100.0f) / 2.2f)) {
                    other->pos.x = otherPos.x;
                }
                if ((s32)(((1210.0f + otherPos.z) / 100.0f) / 2.2f) != (s32)(((1210.0f + other->pos.z) / 100.0f) / 2.2f)) {
                    other->pos.z = otherPos.z;
                }
            }
        }
        other = (Player *)other->node.next;
    }
}

void fn_1_10608(Player *self, s32 motion, u8 flag)
{
    s16 model = self->model;
    s16 mot = *fn_1_126CC(&self->mot, motion);

    if (self->motion != motion) {
        u32 attr;

        Hu3DModelAttrReset(model, 0x40000002);
        if (flag == 0) {
            attr = 0x40000001;
        } else {
            attr = 0;
        }
        CharMotionShiftSet(self->charNo, mot, attr, 0.0f, 8.0f);
        self->motion = motion;
    }
}

void fn_1_106C8(Player *self, u8 on)
{
    s16 model = self->model;

    if (on != 0) {
        if ((self->frame & 1) != 0) {
            Hu3DModelAttrSet(model, 1);
        } else {
            Hu3DModelAttrReset(model, 1);
        }
        self->blink = 1;
        return;
    }
    if (self->blink != 0) {
        Hu3DModelAttrReset(model, 1);
        self->blink = 0;
    }
}

void fn_1_10774(Player *self)
{
    M509Ptmf tmp;
    s16 model = self->model;
    s16 mot = *fn_1_126CC(&self->mot, 0);

    if (self->motion != 0) {
        Hu3DModelAttrReset(model, 0x40000002);
        CharMotionShiftSet(self->charNo, mot, 0x40000001, 0.0f, 8.0f);
        self->motion = 0;
    }
    tmp = lbl_1_data_3534;
    self->state = tmp;
}

void fn_1_10850(Player *self)
{
    Vec move;
    s32 motion;

    fn_1_11394(self, &move);
    if (self->stunTimer > 0) {
        fn_1_10608(self, 5, 0);
        if ((f64)self->stunTimer > 15.0) {
            omVibrate(self->idx, 0xC, 0xC, 0);
        }
        self->pos.x = self->pos.x + move.x;
        self->pos.z = self->pos.z + move.z;
        fn_1_106C8(self, 1);
        return;
    }
    fn_1_106C8(self, 0);
    if (fn_1_10BD4(self, &move) != 0) {
        if (self->stunTimer == 0) {
            if (self->speed > 0.5f) {
                motion = 2;
            } else {
                motion = 1;
            }
            fn_1_10608(self, motion, 0);
        }
    } else {
        fn_1_10608(self, 0, 0);
    }
}

void fn_1_10B34(Player *self)
{
    fn_1_10608(self, 3, 1);
}

u8 fn_1_10BD4(Player *self, Vec *dir)
{
    Vec fwd;
    Vec step;
    f32 d;
    f32 limited;
    f32 rad;

    if (self->speed < 0.01f) {
        Hu3DMotionSpeedSet(self->model, 1.0f);
        return 0;
    }
    Hu3DMotionSpeedSet(self->model, 0.75 * (1.5f * self->speed));
    self->targetRotY = 180.0 * (atan2(dir->x, dir->z) / 3.141592653589793);
    limited = fn_1_EEF0(self->targetRotY, self->rotY, 13.666667f);
    self->rotY = self->rotY + limited;
    rad = (3.141592653589793 * self->rotY) / 180.0;
    fwd.x = sin(rad);
    fwd.y = 0.0f;
    fwd.z = cos(rad);
    d = PSVECDotProduct(&fwd, dir);
    if (d < 0.0f) {
        return 1;
    }
    PSVECScale(&fwd, &step, (f32)(6.75f * (1.6666666666666667 * d)) * self->speed);
    PSVECAdd(&step, &self->pos, &self->pos);
    return 1;
}

s32 fn_1_10F78(Player *self, f32 x, f32 z)
{
    s32 changed = 0;
    s32 key = (s32)((1650.0f + x) / 220.0f) + ((s32)((1210.0f + z) / 220.0f) << 8);

    if (key != self->cellKey) {
        self->cellKey = key;
        changed = 1;
    }
    return changed;
}

u8 fn_1_11020(Player *self)
{
    u8 walls;
    u8 info;

    PLAYER_ASSERT(lbl_1_bss_34 != 0, lbl_1_data_3494, lbl_1_data_3488, 0x27);
    walls = fn_1_BE30(lbl_1_bss_34, self->pos.x, self->pos.z);
    PLAYER_ASSERT(lbl_1_bss_34 != 0, lbl_1_data_3494, lbl_1_data_3488, 0x27);
    info = fn_1_BEE8(lbl_1_bss_34, self->pos.x, self->pos.z);
    if ((s32)((u8)((info & 0x80) > 0) + ((u8)((info & 0x40) > 0) + (u8)((info & 8) > 0) + (u8)((info & 4) > 0))) < 2) {
        switch (self->dif) {
        case 0:
            self->aiTimer = 0x3C;
            break;
        case 1:
            self->aiTimer = 0x24;
            break;
        case 2:
            self->aiTimer = 0x12;
            break;
        case 3:
            self->aiTimer = 0;
            break;
        }
    }
    return walls;
}

u8 fn_1_111C8(f32 x, f32 z)
{
    f32 fx;
    f32 fz;
    f32 ux;
    f32 uz;
    u8 ret;

    fx = 1650.0f + x;
    fz = 1210.0f + z;
    ret = 0;
    ux = (((fx - (220.0f * (f32)(s32)(fx / 220.0f))) / 220.0f) - 0.5);
    uz = (((fz - (220.0f * (f32)(s32)(fz / 220.0f))) / 220.0f) - 0.5);
    if (ux > 0.2) {
        ret = 1;
    }
    if (ux < -0.2) {
        ret |= 2;
    }
    if (uz > 0.2) {
        ret |= 4;
    }
    if (uz < -0.2) {
        ret |= 8;
    }
    return ret;
}

void fn_1_11394(Player *self, Vec *out)
{
    f32 fx;
    f32 fz;
    f32 fx2;
    f32 fz2;
    u8 bits;
    s32 pan;

    self->speed = 0.0f;
    out->x = 0.0f;
    out->y = 0.0f;
    out->z = 0.0f;
    if (fn_1_3B70() == 2) {
        fx = 0.0f;
        fz = 0.0f;
        if (self->pad < 0) {
            if (fn_1_10F78(self, self->pos.x, self->pos.z) != 0) {
                self->cellWalls = fn_1_11020(self);
            }
            switch (self->cellWalls & 7) {
            case 1:
                fx = 0.0f;
                fz = 1.0f;
                break;
            case 2:
                fx = 0.0f;
                fz = -1.0f;
                break;
            case 3:
                fx = -1.0f;
                fz = 0.0f;
                break;
            case 4:
                fx = 1.0f;
                fz = 0.0f;
                break;
            case 5:
                if (self->pos.x > 30.0) {
                    fx = -1.0f;
                }
                if (self->pos.x < -30.0) {
                    fx = 1.0f;
                }
                if (self->pos.z > 30.0) {
                    fz = 1.0f;
                }
                if (self->pos.z < -30.0) {
                    fz = -1.0f;
                }
                break;
            }
            if (self->aiTimer > 0) {
                self->aiTimer = self->aiTimer - 1;
                fz = 0.0f;
                fx = 0.0f;
                bits = fn_1_111C8(self->pos.x, self->pos.z);
                if ((bits & 1) != 0) {
                    fx = -1.0f;
                }
                if ((bits & 2) != 0) {
                    fx = 1.0f;
                }
                if ((bits & 4) != 0) {
                    fz = 1.0f;
                }
                if ((bits & 8) != 0) {
                    fz = -1.0f;
                }
            }
        } else {
            fx = 0.017857144f * (f32)HuPadStkX[self->pad];
            fz = 0.017857144f * (f32)HuPadStkY[self->pad];
        }
        if (self->stunTimer == 0) {
            PLAYER_ASSERT(lbl_1_bss_34 != 0, lbl_1_data_3494, lbl_1_data_3488, 0x27);
            if (fn_1_C000(lbl_1_bss_34, self->pos.x, self->pos.z) != 0) {
                self->stunTimer = 0x3C;
                self->cellKey = -1;
                pan = fn_1_E4B4(&self->pos);
                HuAudFXPlayVolPan(0x691, fn_1_E1A0(&self->pos), pan);
            }
        }
        if (self->stunTimer > 0) {
            self->stunTimer = self->stunTimer - 1;
            fx2 = 1650.0f + self->pos.x;
            fz2 = 1210.0f + self->pos.z;
            fx = 4.0f * -(((fx2 - (220.0f * (f32)(s32)(fx2 / 220.0f))) / 220.0f) - 0.5f);
            fz = 4.0f * (((fz2 - (220.0f * (f32)(s32)(fz2 / 220.0f))) / 220.0f) - 0.5f);
        }
        out->x = fx;
        out->y = 0.0f;
        out->z = -fz;
        self->speed = PSVECMag(out);
    }
}

void fn_1_11D1C(Player *self)
{
    f32 gx0 = (1650.0f + self->prev.x) / 100.0f;
    f32 gz0 = (1210.0f + self->prev.z) / 100.0f;
    s32 ix0 = (s32)(gx0 / 2.2f);
    s32 iz0 = (s32)(gz0 / 2.2f);
    f32 gx1 = (1650.0f + self->pos.x) / 100.0f;
    f32 gz1 = (1210.0f + self->pos.z) / 100.0f;
    s32 ix1 = (s32)(gx1 / 2.2f);
    s32 iz1 = (s32)(gz1 / 2.2f);

    if (ix0 != ix1) {
        self->pos.x = self->prev.x;
    }
    if (iz0 != iz1) {
        self->pos.z = self->prev.z;
    }
}

void fn_1_11EA8(Player *self)
{
    f32 fx;
    f32 fz;
    f32 gx;
    f32 gz;
    s32 i;
    s32 j;

    if (self->stunTimer > 0) {
        fn_1_11D1C(self);
    }
    if (self->pos.x < -1630.0f) {
        self->pos.x = -1630.0f;
    }
    if (self->pos.x > 1630.0f) {
        self->pos.x = 1630.0f;
    }
    if (self->pos.z < -1190.0f) {
        self->pos.z = -1190.0f;
    }
    if (self->pos.z > 1190.0f) {
        self->pos.z = 1190.0f;
    }
    gx = (1650.0f + self->pos.x) / 100.0f;
    gz = (1210.0f + self->pos.z) / 100.0f;
    i = (s32)(gx / 2.2f);
    j = (s32)(gz / 2.2f);
    fx = gx - (2.2f * (f32)i);
    fz = gz - (2.2f * (f32)j);
    fn_1_123E4(self, i, j, 0.0f, 0.0f, fx, fz);
    fn_1_123E4(self, i, j, 2.2f, 0.0f, fx, fz);
    fn_1_123E4(self, i, j, 0.0f, 2.2f, fx, fz);
    fn_1_123E4(self, i, j, 2.2f, 2.2f, fx, fz);
    if (self->pos.x < 80.0 && self->pos.x > -80.0 && self->pos.z < 80.0 && self->pos.z > -80.0) {
        self->inGoal = 1;
        PLAYER_ASSERT(lbl_1_bss_34 != 0, lbl_1_data_3494, lbl_1_data_3488, 0x27);
        fn_1_73A4(lbl_1_bss_34, (u8)self->charNo);
    }
    PLAYER_ASSERT(lbl_1_bss_34 != 0, lbl_1_data_3494, lbl_1_data_3488, 0x27);
    fn_1_BD44(lbl_1_bss_34, (u8)self->charNo, self->pos.x, self->pos.z);
}

void fn_1_123E4(Player *self, s32 i, s32 j, f32 ox, f32 oz, f32 fx, f32 fz)
{
    f32 dx;
    f32 dz;
    f32 dist;

    dx = fx - ox;
    dz = fz - oz;
    dist = sqrt(dx * dx + dz * dz);
    if (dist < 1.0f && dist > 0.001f) {
        self->pos.x = 100.0f * ((ox + (dx / dist) + (2.2f * (f32)i)) - 16.5f);
        self->pos.z = 100.0f * ((oz + (dz / dist) + (2.2f * (f32)j)) - 12.1f);
    }
}

s16 *fn_1_126CC(PlayerVec *vec, u32 idx)
{
    if (idx >= vec->size) {
        fn_1_60(PLAYER_STDERR, lbl_1_data_3540);
        abort();
    }
    return vec->data + idx;
}

void fn_1_12744(PlayerNode *head, void (*fn)(Player *))
{
    PlayerNode *h = head;
    PlayerNode *first = h->next;
    PlayerNode *first2 = first;

    fn_1_127A4(first2, h, fn);
}

void fn_1_127A4(PlayerNode *first, PlayerNode *end, void (*fn)(Player *))
{
    PlayerNode *node = first;
    PlayerNode *e = end;
    void (*f)(Player *) = fn;

    while (node != e) {
        f((Player *)node);
        node = node->next;
    }
}

void fn_1_12804(PlayerVec *vec, s16 *pos, u32 n, s16 *val)
{
    s16 *src = val;
    u32 maxSize;
    u32 max1;
    u32 max2;
    u32 max3;
    PlayerVec *v1;
    PlayerVec *v2;
    PlayerVec *v3;
    s16 *base;
    s16 *newData;
    s16 *newData2;
    s16 **pd;
    s16 *end;
    s16 *old;
    s16 *mem;
    s16 *dst;
    s32 tail;
    s32 off;
    s32 cnt;
    u32 capNow;
    u32 newSize;
    u32 cap;
    u32 k;

    if (n != 0) {
        v1 = vec;
        maxSize = 0x7FFFFFFF;
        max1 = maxSize;
        max2 = max1;
        max3 = max2;
        if (n > max3 || vec->size > max3 - n) {
            fn_1_60(PLAYER_STDERR, lbl_1_data_355E);
            abort();
        }
        v3 = vec;
        u32 *pcap = &v3->cap;
        capNow = *pcap;
        if (vec->size + n <= capNow) {
            pd = &vec->data;
            end = *pd + vec->size;
            tail = end - pos;
            if (end - (tail - n) <= src && src < end) {
                src += n;
            }
            if (tail != 0) {
                cnt = end - pos;
                memmove(end + n - cnt, pos, cnt * 2);
            }
            dst = pos;
            for (k = n; k != 0; k--) {
                *dst = *src;
                dst++;
            }
            vec->size = vec->size + n;
            return;
        }
        pd = &vec->data;
        base = *pd;
        off = pos - base;
        pd = &vec->data;
        old = *pd;
        newSize = vec->size + n;
        v1 = vec;
        if (v1->cap != 0) {
            v2 = vec;
            cap = v2->cap;
        } else {
            cap = 1;
        }
        while (newSize > cap) {
            if (cap < (max2 >> 1)) {
                cap *= 2;
            } else {
                cap = max2;
            }
        }
        pd = &vec->data;
        mem = (s16 *)fn_1_104(cap * 2);
        if (mem == 0) {
            fn_1_60(PLAYER_STDERR, lbl_1_data_357B);
            abort();
        }
        newData = mem;
        *pd = newData;
        pd = &vec->data;
        dst = *pd + off;
        for (k = n; k != 0; k--) {
            *dst = *src;
            dst++;
        }
        if (old != 0) {
            pd = &vec->data;
            newData2 = *pd;
            cnt = (old + off) - old;
            memcpy(newData2, old, cnt * 2);
            pd = &vec->data;
            newData2 = *pd + (off + n);
            cnt = (old + vec->size) - (old + off);
            memcpy(newData2, old + off, cnt * 2);
            fn_1_21C(old);
        }
        vec->size = newSize;
        v1 = vec;
        v1->cap = cap;
    }
}

void fn_1_12BB4(PlayerVec *vec, u32 n)
{
    u32 maxSize;
    u32 max1;
    PlayerVec *v1;
    PlayerVec *v2;
    s16 **pd;
    s16 *old;
    s16 *mem;
    s32 cnt;

    if (n > vec->cap) {
        maxSize = 0x7FFFFFFF;
        max1 = maxSize;
        if (n > max1) {
            fn_1_60(PLAYER_STDERR, lbl_1_data_3595);
            abort();
        }
        pd = &vec->data;
        old = *pd;
        v1 = vec;
        v2 = v1;
        mem = (s16 *)fn_1_104(n * 2);
        if (mem == 0) {
            fn_1_60(PLAYER_STDERR, lbl_1_data_357B);
            abort();
        }
        vec->data = mem;
        if (old != 0) {
            cnt = (old + vec->size) - old;
            memcpy(vec->data, old, cnt * 2);
            fn_1_21C(old);
        }
        vec->cap = n;
    }
}

void fn_1_12CDC(void)
{
    lbl_1_bss_88 = 0x78;
    lbl_1_bss_8C = 0xD2;
    lbl_1_bss_90 = 0xD2;
    fn_1_E48(&lbl_1_bss_A4);
    __register_global_object(&lbl_1_bss_A4, (void *(*)(void *, s16))fn_1_12D64, lbl_1_bss_98);
}

void *fn_1_12D64(PlayerNode *self, s16 flag)
{
    if (self != 0) {
        fn_1_E54(self, 0);
        if (flag > 0) {
            fn_1_21C(self);
        }
    }
    return self;
}
