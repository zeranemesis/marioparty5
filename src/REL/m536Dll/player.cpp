#include "REL/m536Dll.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef TARGET_PC
#define M536_STDERR ((void *)stderr)
#else
extern "C" u8 __files[];
#define M536_STDERR ((void *)(__files + 0xA0))
#endif

extern "C" {

void abort(void);
void *memmove(void *dst, const void *src, size_t n);
extern void *lbl_1_bss_9FC;
extern void *lbl_1_bss_A00;
extern void *lbl_1_bss_A14;

// Score panel, bullet manager and camera singletons (defined in their own units).
void fn_1_DAC0(void *score, s32 idx, s32 charNo);
void fn_1_DBF4(void *score, s32 idx, s32 count);
s32 fn_1_E5B8(void *killer, s32 idx, Vec *pos, f32 rotY);
void fn_1_6DF4(void *camera);
s16 fn_1_6FCC(Vec *pos);
s16 fn_1_706C(Vec *pos);
void fn_1_FF9C(s32 idx, Vec *pos);
void fn_1_FF30(s32 idx, f32 rotY);
void fn_1_1001C(s32 idx);
void fn_1_100D0(s32 idx);
void fn_1_10180(s32 idx, s32 mode);

// std::vector<u16> as laid out by the original library: capacity, size, data.
struct M536VecU16 {
    u32 m_cap;      /* 0x00 */
    u32 m_size;     /* 0x04 */
    u16 *m_data;    /* 0x08 */
};

// One player's work object.
struct M536Player {
    M536_NODE m_node;       /* 0x00 */
    M536_PTMF m_hook;       /* 0x08 */
    u8 m_active;            /* 0x14 */
    s32 m_idx;              /* 0x18 */
    s32 m_charId;           /* 0x1C */
    s32 m_pad;              /* 0x20 */
    s32 m_motion;           /* 0x24 */
    s32 m_ai;               /* 0x28 */
    f32 m_spin;             /* 0x2C */
    Vec m_pos;              /* 0x30 */
    Vec m_muzzle;           /* 0x3C */
    Vec m_vel;              /* 0x48 */
    u8 m_dead;              /* 0x54 */
    u8 m_intro;             /* 0x55 */
    s32 m_introTimer;       /* 0x58 */
    u8 m_leader;            /* 0x5C */
    s32 m_score;            /* 0x60 */
    f32 m_rotY;             /* 0x64 */
    f32 m_rotSpeed;         /* 0x68 */
    s32 m_wait;             /* 0x6C */
    s32 m_hp;               /* 0x70 */
    s32 m_hitTimer;         /* 0x74 */
    u8 m_fireReq;           /* 0x78 */
    s32 m_fireTimer;        /* 0x7C */
    u8 m_sfxOn;             /* 0x80 */
    s32 m_sfxId;            /* 0x84 */
    s16 m_mdl;              /* 0x88 */
    M536VecU16 m_mot;       /* 0x8C */
};

typedef M536_NODE M536PlayerList;
typedef void (*M536PlayerFn)(M536Player *);

void fn_1_B1AC(M536Player *self);
void fn_1_B3D4(M536Player *self);
void fn_1_B79C(M536Player *self);
void fn_1_B978(M536Player *self);
void fn_1_BB30(M536Player *self);
void fn_1_BB80(M536Player *self);
void fn_1_BC28(M536Player *self);
void fn_1_C6DC(M536Player *self);
void fn_1_CF58(M536Player *self);
void fn_1_9E0C(M536Player *self);
void fn_1_D084(M536PlayerList *list, M536PlayerFn cb);
void fn_1_D0E4(M536Player *node, M536PlayerList *stop, M536PlayerFn cb);
void fn_1_D144(M536VecU16 *vec, u16 *pos, u32 count, u16 *value);
void fn_1_D4F4(M536VecU16 *vec, u32 newCap);
u16 *fn_1_D00C(M536VecU16 *vec, u32 index);
void *fn_1_D6A4(void *node, s16 freeIt);
s32 fn_1_B4B0(s32 owner, Vec *pos);
f32 fn_1_BCD0(Vec *a, Vec *b);

s32 lbl_1_data_3C8 = -1;
Vec lbl_1_data_3CC = { 0.0f, 10.0f, -150.0f };
Vec lbl_1_data_3D8 = { 0.0f, 50.0f, 140.0f };
M536_PTMF lbl_1_data_3E4 = { 0, -1, (void *)fn_1_B3D4 };
M536_PTMF lbl_1_data_464 = { 0, -1, (void *)fn_1_BB80 };
M536_PTMF lbl_1_data_470 = { 0, -1, (void *)fn_1_BC28 };
M536_PTMF lbl_1_data_47C = { 0, -1, (void *)fn_1_B79C };
M536_PTMF lbl_1_data_488 = { 0, -1, (void *)fn_1_B978 };
M536_PTMF lbl_1_data_494 = { 0, -1, (void *)fn_1_BB30 };
Vec lbl_1_data_4AC = { 0.0f, 0.0f, 1.0f };
Vec lbl_1_data_4B8 = { 0.0f, 0.0f, 0.0f };

s32 lbl_1_bss_A80;
s32 lbl_1_bss_A84;
s32 lbl_1_bss_A88;
u8 lbl_1_bss_A8C[12];
M536PlayerList lbl_1_bss_A98;
u8 lbl_1_bss_AA0;
u8 lbl_1_bss_AA1;
s32 lbl_1_bss_AA4;
u8 lbl_1_bss_AA8;
M536Player *lbl_1_bss_AAC;

// Signed angle difference a - b in degrees, through 16-bit binary angles.
f32 fn_1_9B50(f32 a, f32 b)
{
    s32 x;
    s32 xx;
    s32 y;
    s32 yy;
    s32 d;
    s32 dd;

    x = (s32)(182.04444444444445 * a);
    xx = x;
    y = (s32)(182.04444444444445 * b);
    yy = y;
    d = (u16)((xx - yy) - 0x8000) - 0x8000; // 16-bit wrap of the angle difference
    dd = d;
    return 0.0054931640625 * dd;
}

f32 fn_1_9C08(f32 a, f32 b, f32 limit)
{
    f32 diff;
    f32 d0;
    f32 d1;
    f32 d2;
    f32 ret;

    d0 = fn_1_9B50(a, b);
    d1 = d0;
    d2 = d1;
    diff = d2;
    if (diff < -limit) {
        ret = -limit;
    } else if (diff > limit) {
        ret = limit;
    } else {
        ret = diff;
    }
    return ret;
}

void fn_1_9D6C(void)
{
    if (lbl_1_bss_AA0 != 0) {
        if (lbl_1_bss_AA1 == 0) {
            lbl_1_bss_AA1 = 1;
            lbl_1_data_3C8 = HuAudFXPlay(0x7CF);
        }
    } else if (lbl_1_bss_AA1 != 0) {
        lbl_1_bss_AA1 = 0;
        HuAudFXStop(lbl_1_data_3C8);
    }
}

void fn_1_9E0C(M536Player *self)
{
    Mtx mtx;
    Vec p;
    f32 limit;
    f32 accel;
    f32 cx;
    f32 cz;
    f32 diff;
    f32 target;
    f32 t;
    f32 k;

    limit = 417.99997f;
    accel = 7.0f;
    switch (self->m_idx) {
    case 0:
        cx = -limit;
        cz = -limit;
        break;
    case 1:
        cx = limit;
        cz = -limit;
        break;
    case 2:
        cx = -limit;
        cz = limit;
        break;
    case 3:
        cx = limit;
        cz = limit;
        break;
    }
    self->m_wait--;
    if (self->m_wait < 0) {
        self->m_wait = 0;
        if (self->m_leader != 0) {
            diff = fn_1_9B50(-20.0f, self->m_rotY);
            target = 120.0f / 60.0f;
            if (diff < 0.0f || diff > 5.0f) {
                if (self->m_rotSpeed > target) {
                    self->m_rotSpeed -= 14.0f / 60.0f;
                    if (self->m_rotSpeed < target) {
                        self->m_rotSpeed = target;
                    }
                } else {
                    self->m_rotSpeed += accel / 60.0f;
                    if (self->m_rotSpeed > target) {
                        self->m_rotSpeed = target;
                    }
                }
            } else {
                self->m_rotSpeed = 0.0f;
            }
        } else {
            t = 200.0f + 100.0f * (f32)self->m_hp;
            if (self->m_rotSpeed < (t - accel) / 60.0f) {
                self->m_rotSpeed += accel / 60.0f;
            }
            if (self->m_rotSpeed > (accel + t) / 60.0f) {
                self->m_rotSpeed -= accel / 60.0f;
            }
        }
        self->m_rotY = self->m_rotY + self->m_rotSpeed;
        if (self->m_rotY > 180.0f) {
            self->m_rotY -= 360.0f;
        }
    }
    if (self->m_rotSpeed > 1.0f) {
        lbl_1_bss_AA0 = 1;
    }
    p.x = cx;
    p.y = accel;
    p.z = cz;
    if (self->m_intro != 0) {
        self->m_introTimer++;
        if (self->m_introTimer > 0x78) {
            self->m_introTimer = 0x78;
            if (self->m_sfxOn != 0) {
                self->m_sfxOn = 0;
                HuAudFXStop(self->m_sfxId);
            }
        } else if (self->m_sfxOn == 0) {
            self->m_sfxOn = 1;
            self->m_sfxId = HuAudFXPlay(0x7D6);
        }
        k = (f32)self->m_introTimer / (f32)0x78;
        p.x -= cx * k;
        p.z -= cz * k;
    }
    if (self->m_active != 0) {
        fn_1_FF9C(self->m_idx, &p);
        fn_1_FF30(self->m_idx, self->m_rotY);
    }
    self->m_active = 1;
    PSMTXRotRad(mtx, 'y', 0.017453292f * self->m_rotY);
    PSMTXTransApply(mtx, mtx, p.x, p.y, p.z);
    PSMTXMultVec(mtx, &lbl_1_data_3CC, &self->m_pos);
    PSMTXMultVec(mtx, &lbl_1_data_3D8, &self->m_muzzle);
}

void fn_1_A3A0(void)
{
    M536_NODE *back;
    M536_NODE *back2;
    M536Player *p;

    while (M536ListEmpty(&lbl_1_bss_A98) == 0) {
        back = lbl_1_bss_A98.prev;
        back2 = back;
        p = (M536Player *)back2;
        if (p != 0) {
            if (&p->m_mot != 0 && &p->m_mot != 0 && &p->m_mot != 0 && p->m_mot.m_data != 0) {
                fn_1_C8(p->m_mot.m_data);
            }
            fn_1_30F4(&p->m_mdl, -1);
            if (p != 0) {
                fn_1_1270(p, 0);
            }
            fn_1_C8(p);
        }
    }
}

void fn_1_AAB0(M536Player *self, s32 dataNum);

M536Player *fn_1_A4A8(M536Player *self, s32 idx)
{
    M536_PTMF tmp;
    Vec rot;
    f32 r;
    void *score;
    s16 pad;

    fn_1_1264(self);
    self->m_intro = 0;
    self->m_introTimer = 0;
    self->m_sfxOn = 0;
    fn_1_2D18(&self->m_mdl, GwPlayerConf[idx].charNo, 1);
    self->m_mot.m_cap = 0;
    self->m_mot.m_size = 0;
    self->m_mot.m_data = 0;
    lbl_1_bss_AA4 = 0;
    fn_1_1330(self, &lbl_1_bss_A98);
    self->m_idx = idx;
    self->m_charId = GwPlayerConf[idx].charNo;
    printf("m_char_id %d\n", self->m_charId);
    self->m_ai = GwPlayerConf[idx].dif;
    if (GwPlayerConf[idx].type != 0) {
        pad = -1;
    } else {
        pad = GwPlayerConf[idx].padNo;
    }
    self->m_pad = pad;
    self->m_leader = 0;
    self->m_active = 0;
    r = 360.0f * fn_1_4D1C();
    self->m_rotY = r - 180.0f;
    self->m_rotSpeed = 0.0f;
    self->m_wait = 0xF0;
    lbl_1_bss_AA0 = 0;
    lbl_1_bss_AA1 = 0;
    fn_1_9E0C(self);
    self->m_spin = 0.0f;
    self->m_dead = 0;
    self->m_score = 0;
    self->m_hp = 3;
    self->m_hitTimer = 0;
    self->m_fireTimer = 0;
    self->m_fireReq = 0;
    self->m_motion = -1;
    self->m_mot.m_size = 0;
    fn_1_D4F4(&self->m_mot, 6);
    fn_1_AAB0(self, 0x870028);
    fn_1_AAB0(self, 0x8C002F);
    fn_1_AAB0(self, 0x8C0031);
    fn_1_AAB0(self, 0x8C003F);
    fn_1_AAB0(self, 0x8C001A);
    fn_1_AAB0(self, 0x8C0005);
    if (self->m_mot.m_size == 6) {
    } else {
        fn_1_1D4("m_mot_id.size() == NUMBEROF_MOTION", "player.cpp", 0xF3);
    }
    CharModelFxFlagSet(self->m_charId, 0);
    CharMotionDataClose(self->m_charId);
    Hu3DModelLayerSet(self->m_mdl, 2);
    Hu3DModelCameraSet(self->m_mdl, 1);
    rot.x = 0.0f;
    rot.y = self->m_rotY;
    rot.z = 0.0f;
    fn_1_2F3C(&self->m_mdl, &rot);
    fn_1_2EF4(&self->m_mdl, &self->m_pos);
    tmp = lbl_1_data_3E4;
    self->m_hook = tmp;
    if (lbl_1_bss_9FC != 0) {
    } else {
        fn_1_1D4("ms_pointer != 0", "singleton.h", 0x27);
    }
    fn_1_DAC0(lbl_1_bss_9FC, self->m_idx, self->m_charId);
    if (lbl_1_bss_9FC != 0) {
    } else {
        fn_1_1D4("ms_pointer != 0", "singleton.h", 0x27);
    }
    fn_1_DBF4(lbl_1_bss_9FC, self->m_idx, self->m_hp);
    return self;
}

// std::vector<u16> destructor chain, inlined three levels deep.
M536VecU16 *fn_1_A8B4(M536VecU16 *vec, s16 freeIt)
{
    u16 **ref1;
    u16 **ref2;
    u16 *d;

    if (vec != 0) {
        if (vec != 0 && vec != 0) {
            ref1 = &vec->m_data;
            if (*ref1 != 0) {
                ref2 = &vec->m_data;
                d = *ref2;
                fn_1_C8(d);
            }
        }
        if (freeIt > 0) {
            fn_1_C8(vec);
        }
    }
    return vec;
}

M536VecU16 *fn_1_A948(M536VecU16 *vec, s16 freeIt)
{
    u16 **ref1;
    u16 **ref2;
    u16 *d;

    if (vec != 0) {
        if (vec != 0) {
            ref1 = &vec->m_data;
            if (*ref1 != 0) {
                ref2 = &vec->m_data;
                d = *ref2;
                fn_1_C8(d);
            }
        }
        if (freeIt > 0) {
            fn_1_C8(vec);
        }
    }
    return vec;
}

M536VecU16 *fn_1_A9D4(M536VecU16 *vec, s16 freeIt)
{
    u16 **ref1;
    u16 **ref2;
    u16 *d;

    if (vec != 0) {
        ref1 = &vec->m_data;
        if (*ref1 != 0) {
            ref2 = &vec->m_data;
            d = *ref2;
            fn_1_C8(d);
        }
        if (freeIt > 0) {
            fn_1_C8(vec);
        }
    }
    return vec;
}

M536Player *fn_1_AA58(M536Player *self, s16 freeIt)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_AAB0(M536Player *self, s32 dataNum)
{
    u16 mot;
    u16 **ref;
    u16 *end;

    mot = CharMotionCreate(self->m_charId, dataNum);
    ref = &self->m_mot.m_data;
    end = *ref + self->m_mot.m_size;
    fn_1_D144(&self->m_mot, end, 1, &mot);
}

void fn_1_AB28(void)
{
    M536_NODE *first;

    lbl_1_bss_AA0 = 0;
    fn_1_D084(&lbl_1_bss_A98, fn_1_B1AC);
    if (lbl_1_bss_AA0 != 0) {
        if (lbl_1_bss_AA1 == 0) {
            lbl_1_bss_AA1 = 1;
            lbl_1_data_3C8 = HuAudFXPlay(0x7CF);
        }
    } else if (lbl_1_bss_AA1 != 0) {
        lbl_1_bss_AA1 = 0;
        HuAudFXStop(lbl_1_data_3C8);
    }
    first = lbl_1_bss_A98.next;
    if (first != &lbl_1_bss_A98) {
        fn_1_12DC(first);
        fn_1_1330(first, &lbl_1_bss_A98);
    }
}

s32 fn_1_AC40(void)
{
    M536Player *p;
    M536Player *p0;
    M536Player *p1;
    M536Player *end;
    M536Player *nx;
    s32 count;

    lbl_1_bss_AA8 = 1;
    p0 = (M536Player *)lbl_1_bss_A98.next;
    p1 = p0;
    p = p1;
    end = (M536Player *)&lbl_1_bss_A98;
    count = 0;
    while (p != end) {
        if (p->m_dead != 0) {
            count++;
        }
        nx = (M536Player *)p->m_node.next;
        p = nx;
    }
    if (count < 3) {
        return 0;
    }
    return 1;
}

void fn_1_ACD4(void)
{
    M536Player *players[4];
    s32 scores[4];
    s32 order[4] = { 0, 1, 2, 3 };
    M536Player *p;
    M536Player *p0;
    M536Player *p1;
    M536Player *end;
    M536Player *nx;
    s16 rank;
    s32 n;
    s32 alive;
    s32 a;
    s32 b;
    s32 i;
    s32 t;
    s32 pl;

    lbl_1_bss_AA8 = 0;
    p0 = (M536Player *)lbl_1_bss_A98.next;
    p1 = p0;
    p = p1;
    end = (M536Player *)&lbl_1_bss_A98;
    alive = 0;
    n = 0;
    while (p != end) {
        players[n] = p;
        if (p->m_dead == 0) {
            lbl_1_bss_AAC = p;
            alive++;
            p->m_wait = 0;
            p->m_leader = 1;
            scores[n] = p->m_hp * 100000;
        } else {
            scores[n] = p->m_score;
        }
        n++;
        nx = (M536Player *)p->m_node.next;
        p = nx;
    }
    if (alive != 1) {
        lbl_1_bss_AAC = 0;
    } else {
        lbl_1_bss_AAC->m_intro = 1;
        if (lbl_1_bss_A14 != 0) {
        } else {
            fn_1_1D4("ms_pointer != 0", "singleton.h", 0x27);
        }
        fn_1_6DF4(lbl_1_bss_A14);
        fn_1_1001C(lbl_1_bss_AAC->m_idx);
    }
    for (a = 0; a < 3; a++) {
        for (b = 0; b < 3; b++) {
            if (scores[b] < scores[b + 1]) {
                t = scores[b];
                scores[b] = scores[b + 1];
                scores[b + 1] = t;
                t = order[b];
                order[b] = order[b + 1];
                order[b + 1] = t;
            }
        }
    }
    rank = 0;
    for (i = 0; i < 4; i++) {
        pl = players[order[i]]->m_idx;
        if (_CheckFlag(0x1000F) == 0) {
            GwPlayer[pl].mgCoinBonus = rank;
        }
        printf("player %d rank %d test %d\n", players[order[i]]->m_idx, rank, scores[i]);
        if (i < 3 && scores[i] != scores[i + 1]) {
            rank++;
        }
    }
}

u8 fn_1_AFF0(void)
{
    M536_PTMF tmp1;
    M536_PTMF tmp2;
    M536Player *p;
    M536Player *p0;
    M536Player *p1;
    M536Player *end;
    M536Player *nx;
    s16 chars[4];
    u8 allOut;
    s32 alive;
    s32 n;

    p0 = (M536Player *)lbl_1_bss_A98.next;
    p1 = p0;
    p = p1;
    end = (M536Player *)&lbl_1_bss_A98;
    allOut = 1;
    alive = 0;
    n = 0;
    while (p != end) {
        if (p->m_dead == 0) {
            alive++;
        }
        nx = (M536Player *)p->m_node.next;
        p = nx;
    }
    p0 = (M536Player *)lbl_1_bss_A98.next;
    p1 = p0;
    p = p1;
    end = (M536Player *)&lbl_1_bss_A98;
    if (alive < 4) {
        while (p != end) {
            chars[n++] = p->m_charId;
            if (p->m_dead == 0) {
                tmp1 = lbl_1_data_464;
                p->m_hook = tmp1;
                CharFXPlayPan(p->m_charId, 0x1FA, fn_1_6FCC(&p->m_pos));
                allOut = 0;
            }
            nx = (M536Player *)p->m_node.next;
        p = nx;
        }
    } else {
        while (p != end) {
            chars[n++] = p->m_charId;
            tmp2 = lbl_1_data_470;
            p->m_hook = tmp2;
            nx = (M536Player *)p->m_node.next;
        p = nx;
        }
    }
    if (allOut != 0) {
        CharLoseVoicePlay(chars[0], chars[1], chars[2], chars[3]);
    }
    return allOut;
}

void fn_1_B1AC(M536Player *self)
{
    Vec rot;

    __ptmf_scall(self);
    lbl_1_bss_AA4++;
    if (self->m_dead == 0) {
        self->m_score = lbl_1_bss_AA4;
    }
    self->m_hitTimer--;
    if (self->m_hitTimer < 0) {
        self->m_hitTimer = 0;
    } else if ((f32)self->m_hitTimer > 30.0f) {
        omVibrate(self->m_idx, 0xC, 0xC, 0);
    }
    rot.x = 0.0f;
    rot.y = self->m_rotY;
    rot.z = 0.0f;
    fn_1_2F3C(&self->m_mdl, &rot);
    fn_1_2EF4(&self->m_mdl, &self->m_pos);
    CharModelVoicePanSet(self->m_charId, fn_1_6FCC(&self->m_pos));
    CharModelVoiceVolSet(self->m_charId, fn_1_706C(&self->m_pos));
}

// Switch the character to motion `index`; `loop` 0 means looping (attribute 0x40000001).
void fn_1_B314(M536Player *self, s32 index, u8 loop)
{
    s16 mdl;
    s16 mot;
    u32 attr;

    mdl = self->m_mdl;
    mot = *fn_1_D00C(&self->m_mot, index);
    if (self->m_motion != index) {
        Hu3DModelAttrReset(mdl, 0x40000002);
        if (loop == 0) {
            attr = 0x40000001;
        } else {
            attr = 0;
        }
        CharMotionShiftSet(self->m_charId, mot, 0.0f, 8.0f, attr);
        self->m_motion = index;
    }
}

void fn_1_B3D4(M536Player *self)
{
    M536_PTMF tmp;

    fn_1_B314(self, 0, 0);
    tmp = lbl_1_data_47C;
    self->m_hook = tmp;
}

s32 fn_1_B4B0(s32 owner, Vec *pos)
{
    M536_PTMF tmp;
    M536Player *p;
    M536Player *p0;
    M536Player *p1;
    M536Player *end;
    M536Player *nx;
    void *score;
    f32 dx;
    f32 dz;

    p0 = (M536Player *)lbl_1_bss_A98.next;
    p1 = p0;
    p = p1;
    end = (M536Player *)&lbl_1_bss_A98;
    while (p != end) {
        if (p->m_idx != owner && p->m_hitTimer == 0 && p->m_hp > 0) {
            dx = p->m_pos.x - pos->x;
            dz = p->m_pos.z - pos->z;
            if ((dx * dx + dz * dz) < 40000.0) {
                if (lbl_1_bss_AA8 != 0) {
                    p->m_hp--;
                    if (p->m_hp <= 0) {
                        p->m_vel.x = 0.0f;
                        p->m_vel.y = 50.0f;
                        p->m_vel.z = 0.0f;
                        tmp = lbl_1_data_488;
                        p->m_hook = tmp;
                        p->m_dead = 1;
                        CharFXPlayPan(p->m_charId, 0x1F9, fn_1_6FCC(&p->m_pos));
                        Hu3DModelAttrReset(p->m_mdl, 1);
                    } else {
                        CharFXPlayPan(p->m_charId, 0x1FD, fn_1_6FCC(&p->m_pos));
                        fn_1_10180(p->m_idx, p->m_hp);
                        p->m_hitTimer = 0x5A;
                        p->m_fireReq = 0;
                    }
                    if (lbl_1_bss_9FC != 0) {
                    } else {
                        fn_1_1D4("ms_pointer != 0", "singleton.h", 0x27);
                    }
                    fn_1_DBF4(lbl_1_bss_9FC, p->m_idx, p->m_hp);
                }
                return 1;
            }
        }
        nx = (M536Player *)p->m_node.next;
        p = nx;
    }
    return 0;
}

void fn_1_B700(s32 owner, Vec *pos, f32 rotY)
{
    void *ms0;
    void *ms1;

    if (lbl_1_bss_A00 != 0) {
    } else {
        fn_1_1D4("ms_pointer != 0", "singleton.h", 0x27);
    }
    ms0 = lbl_1_bss_A00;
    ms1 = ms0;
    fn_1_E5B8(ms1, owner, pos, rotY);
    fn_1_100D0(owner);
}

void fn_1_B79C(M536Player *self)
{
    if (fn_1_1F30() == 2) {
        self->m_fireTimer--;
        if (self->m_fireTimer < 0) {
            self->m_fireTimer = 0;
        }
        fn_1_C6DC(self);
    }
    fn_1_CF58(self);
    if (self->m_hitTimer != 0) {
        fn_1_B314(self, 4, 1);
    } else if (self->m_fireTimer == 0) {
        fn_1_B314(self, 0, 0);
    } else {
        fn_1_B314(self, 5, 1);
    }
}

void fn_1_B978(M536Player *self)
{
    M536_PTMF tmp;

    omVibrate(self->m_idx, 0xC, 0xC, 0);
    fn_1_B314(self, 3, 1);
    self->m_pos.x = self->m_pos.x + self->m_vel.x;
    self->m_pos.y = self->m_pos.y + self->m_vel.y;
    self->m_pos.z = self->m_pos.z + self->m_vel.z;
    self->m_vel.y = self->m_vel.y - 0.8333333f;
    self->m_spin = self->m_spin - 10.0f;
    if (self->m_spin < -180.0f) {
        self->m_spin = self->m_spin + 360.0f;
    }
    if (self->m_vel.y < 0.0f) {
        tmp = lbl_1_data_494;
        self->m_hook = tmp;
    }
    Hu3DMotionForceSet(self->m_mdl, (char *)"root_motion", 0x80, self->m_spin);
}

void fn_1_BB30(M536Player *self)
{
    s16 attr;
    s16 id;

    id = self->m_mdl;
    attr = id;
    Hu3DModelAttrSet(attr, 1);
}

void fn_1_BB80(M536Player *self)
{
    fn_1_CF58(self);
    fn_1_B314(self, 1, 1);
}

void fn_1_BC28(M536Player *self)
{
    fn_1_CF58(self);
    fn_1_B314(self, 2, 1);
}

// Heading in degrees from b towards a on the XZ plane.
f32 fn_1_BCD0(Vec *a, Vec *b)
{
    return 180.0 * (atan2(a->x - b->x, a->z - b->z) / 3.141592653589793);
}

// The original compiled fn_1_BD44 into fn_1_C0FC and fn_1_C0FC into fn_1_C6DC;
// MWCC 1.3.2 will not inline bodies this large, so the bodies are macros.

// Finds the opponent closest to the cannon heading; found is false when nobody
// is within 60 degrees, alone is set when no other human is still playing.
#define M536_FIND_TARGET(self, outAngle, outAlone, found)                          \
    do {                                                                           \
        Mtx mtx;                                                                   \
        Vec dir;                                                                   \
        M536Player *p;                                                             \
        M536Player *p0;                                                            \
        M536Player *p1;                                                            \
        M536Player *end;                                                           \
        M536Player *nx;                                                            \
        f32 base;                                                                  \
        f32 ang;                                                               \
        f32 ang0;                                                              \
        f32 ang1;                                                              \
        f32 diff0;                                                             \
        f32 diff;                                                                  \
        f32 absDiff;                                                               \
        f32 best;                                                                  \
        f32 bestAbs;                                                               \
        s32 count;                                                                 \
        PSMTXRotRad(mtx, 'y', 0.017453292f * (self)->m_rotY);                      \
        PSMTXMultVec(mtx, &lbl_1_data_4AC, &dir);                                  \
        base = fn_1_BCD0(&dir, &lbl_1_data_4B8);                                   \
        best = 0.0f;                                                               \
        bestAbs = 90.0f;                                                           \
        p0 = (M536Player *)lbl_1_bss_A98.next;                                     \
        p1 = p0;                                                                   \
        p = p1;                                                                    \
        end = (M536Player *)&lbl_1_bss_A98;                                        \
        count = 0;                                                                 \
        while (p != end) {                                                         \
            if (p->m_idx != (self)->m_idx && p->m_hitTimer == 0 && p->m_hp > 0) { \
                ang0 = fn_1_BCD0(&p->m_pos, &(self)->m_pos);                   \
                ang1 = ang0;                                                   \
                ang = ang1;                                                    \
                diff0 = fn_1_9B50(ang, base);                                  \
                diff = diff0;                                                  \
                if (diff < 0.0f) {                                                 \
                    absDiff = -diff;                                               \
                } else {                                                           \
                    absDiff = diff;                                                \
                }                                                                  \
                if (absDiff < bestAbs) {                                           \
                    best = diff;                                                   \
                    if (diff < 0.0f) {                                             \
                        bestAbs = -diff;                                           \
                    } else {                                                       \
                        bestAbs = diff;                                            \
                    }                                                              \
                }                                                                  \
            }                                                                      \
            if (p->m_pad >= 0 && p->m_hp > 0) {                                    \
                count++;                                                           \
            }                                                                      \
            nx = (M536Player *)p->m_node.next;                                     \
            p = nx;                                                                \
        }                                                                          \
        if (count == 0) {                                                          \
            *(outAlone) = 1;                                                       \
        }                                                                          \
        *(outAngle) = best;                                                        \
        if (bestAbs >= 60.0f) {                                                    \
            (found) = 0;                                                           \
        } else {                                                                   \
            (found) = 1;                                                           \
        }                                                                          \
    } while (0)

// Decides whether an AI-controlled player fires this frame.
#define M536_AI_FIRE(self, fire)                                                   \
    do {                                                                           \
        f32 angle;                                                                 \
        f32 absAngle;                                                              \
        s8 alone;                                                                  \
        s32 found;                                                                 \
        (fire) = 0;                                                                \
        alone = 0;                                                                 \
        M536_FIND_TARGET(self, &angle, &alone, found);                             \
        if (found != 0) {                                                          \
            if (alone == 1) {                                                      \
                if (angle < 0.0f) {                                                \
                    absAngle = -angle;                                             \
                } else {                                                           \
                    absAngle = angle;                                              \
                }                                                                  \
                if (absAngle < 7.0f && (fn_1_4AE0() & 3) == 0) {                   \
                    (fire) = 1;                                                    \
                }                                                                  \
            } else {                                                               \
                switch ((self)->m_ai) {                                            \
                case 0:                                                            \
                    if ((fn_1_4AE0() & 0x3F) == 0) {                               \
                        (fire) = 1;                                                \
                    }                                                              \
                    break;                                                         \
                case 1:                                                            \
                    if (angle < 0.0f) {                                            \
                        absAngle = -angle;                                         \
                    } else {                                                       \
                        absAngle = angle;                                          \
                    }                                                              \
                    if (absAngle < 40.0f && (fn_1_4AE0() & 0x1F) == 0) {           \
                        (fire) = 1;                                                \
                    }                                                              \
                    break;                                                         \
                case 2:                                                            \
                    if (angle < 0.0f) {                                            \
                        absAngle = -angle;                                         \
                    } else {                                                       \
                        absAngle = angle;                                          \
                    }                                                              \
                    if (absAngle < 25.0f && (fn_1_4AE0() & 0x1F) == 0) {           \
                        (fire) = 1;                                                \
                    }                                                              \
                    break;                                                         \
                case 3:                                                            \
                    if (angle < 0.0f) {                                            \
                        absAngle = -angle;                                         \
                    } else {                                                       \
                        absAngle = angle;                                          \
                    }                                                              \
                    if (absAngle < 10.0f && (fn_1_4AE0() & 0xF) == 0) {            \
                        (fire) = 1;                                                \
                    }                                                              \
                    break;                                                         \
                }                                                                  \
            }                                                                      \
        }                                                                          \
    } while (0)

s32 fn_1_BD44(M536Player *self, f32 *outAngle, s8 *outAlone)
{
    s32 found;

    M536_FIND_TARGET(self, outAngle, outAlone, found);
    return found;
}

s32 fn_1_C0FC(M536Player *self)
{
    s32 fire;

    M536_AI_FIRE(self, fire);
    return fire;
}

void fn_1_C6DC(M536Player *self)
{
    Vec fire;
    f32 rotY;
    s32 idx;
    s16 se;
    u8 want;
    s32 aiFire;

    if (self->m_hitTimer == 0) {
        if ((f32)self->m_fireTimer < 120.0f && (f32)self->m_fireTimer > 114.0f) {
            omVibrate(self->m_idx, 0xC, 0xC, 0);
        }
        if ((f32)self->m_fireTimer == 30.0f) {
            if (self->m_pos.x > 0.0f) {
                se = 0x7D5;
            } else {
                se = 0x7D4;
            }
            HuAudFXPlay(se);
        }
        if (self->m_fireReq != 0) {
            if ((f32)self->m_fireTimer <= 120.0f) {
                rotY = self->m_rotY;
                idx = self->m_idx;
                if (lbl_1_bss_A00 != 0) {
                } else {
                    fn_1_1D4("ms_pointer != 0", "singleton.h", 0x27);
                }
                fn_1_E5B8(lbl_1_bss_A00, idx, &self->m_muzzle, rotY);
                fn_1_100D0(idx);
                self->m_fireReq = 0;
            }
        }
        if (self->m_fireTimer == 0) {
            fire.x = self->m_pos.x;
            fire.y = 70.0f + self->m_pos.y;
            fire.z = self->m_pos.z;
            want = 0;
            if (self->m_pad < 0) {
                M536_AI_FIRE(self, aiFire);
                if (aiFire != 0) {
                    want = 1;
                }
            } else if ((HuPadBtnDown[self->m_pad] & 0x100) != 0) {
                want = 1;
            }
            if (want != 0) {
                self->m_fireReq = 1;
                self->m_fireTimer = 0x81;
                self->m_wait = 0x1E;
                self->m_rotSpeed = 0.0f;
            }
        }
    }
}

void fn_1_CF58(M536Player *self)
{
    s16 mdl;

    fn_1_9E0C(self);
    mdl = self->m_mdl;
    if ((f32)self->m_hitTimer < 6.0f || (self->m_hitTimer & 1) == 0) {
        Hu3DModelAttrReset(mdl, 1);
        return;
    }
    Hu3DModelAttrSet(mdl, 1);
}

u16 *fn_1_D00C(M536VecU16 *vec, u32 index)
{
    if (index >= vec->m_size) {
        fn_1_234(M536_STDERR, "vector::at index out of range");
        abort();
    }
    return vec->m_data + index;
}

void fn_1_D084(M536PlayerList *list, M536PlayerFn cb)
{
    // Three named steps: the original keeps a live copy of the head, the load
    // of head->next and a second copy of that load in three separate
    // registers, so the forwarding cannot be collapsed to one expression.
    M536PlayerList *head;
    M536Player *first;
    M536Player *start;

    head = list;
    first = (M536Player *)list->next;
    start = first;
    fn_1_D0E4(start, head, cb);
}

void fn_1_D0E4(M536Player *node, M536PlayerList *stop, M536PlayerFn cb)
{
    M536Player *next;

    while (node != (M536Player *)stop) {
        cb(node);
        next = (M536Player *)node->m_node.next;
        node = next;
    }
}

// std::vector<u16>::insert(pos, count, value). The extra named copies mirror the
// inlined helpers (max_size(), end(), capacity()) the original compiled in.
void fn_1_D144(M536VecU16 *vec, u16 *pos, u32 count, u16 *value)
{
    M536VecU16 *self0;
    M536VecU16 *self1;
    M536VecU16 *self2;
    M536VecU16 *self3;
    volatile u32 max0;
    volatile u32 max1;
    volatile u32 max2;
    u32 cap0;
    u32 cap1;
    u16 **ref;
    u16 *end;
    u16 *base0;
    u16 *base1;
    u16 *src;
    u16 *dst;
    u16 *oldData;
    u16 *newData;
    u16 *newData1;
    s32 tail;
    s32 tail2;
    s32 n1;
    s32 n2;
    u16 *mid;
    u16 *tailEnd;
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
        if (count > max2 || vec->m_size > max2 - count) {
            fn_1_234(M536_STDERR, "vector::insert length error\n");
            abort();
        }
        self1 = vec;
        cap0 = self1->m_cap;
        cap1 = cap0;
        if (vec->m_size + count <= cap1) {
            ref = &vec->m_data;
            end = *ref + vec->m_size;
            tail = end - pos;
            src = value;
            if ((end - (tail - (s32)count)) <= src && src < end) {
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
            vec->m_size = vec->m_size + count;
            return;
        }
        ref = &vec->m_data;
        base0 = *ref;
        base1 = base0;
        index = pos - base1;
        ref = &vec->m_data;
        oldData = *ref;
        newSize = vec->m_size + count;
        self2 = vec;
        if (self2->m_cap != 0) {
            self3 = vec;
            cap2 = self3->m_cap;
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
        ref = &vec->m_data;
        newData = (u16 *)fn_1_0(newCap * 2);
        if (newData == 0) {
            fn_1_234(M536_STDERR, "Memory allocation failure");
            abort();
        }
        newData1 = newData;
        *ref = newData1;
        k = count;
        ref = &vec->m_data;
        dst = *ref + index;
        while (k != 0) {
            *dst = *value;
            dst++;
            k--;
        }
        if (oldData != 0) {
            ref = &vec->m_data;
            mid = oldData + index;
            n1 = mid - oldData;
            memcpy(*ref, oldData, n1 * 2);
            ref = &vec->m_data;
            tailEnd = oldData + vec->m_size;
            n2 = tailEnd - mid;
            memcpy(*ref + index + count, mid, n2 * 2);
            fn_1_C8(oldData);
        }
        vec->m_size = newSize;
        self0 = vec;
        self0->m_cap = newCap;
    }
}

// std::vector<u16>::reserve(newCap)
void fn_1_D4F4(M536VecU16 *vec, u32 newCap)
{
    M536VecU16 *self0;
    M536VecU16 *self1;
    volatile u32 max0;
    volatile u32 max1;
    u16 *oldData;
    u16 *newData;
    u16 *e0;
    s32 n;

    if (newCap > vec->m_cap) {
        max0 = 0x7FFFFFFF;
        max1 = max0;
        if (newCap > max1) {
            fn_1_234(M536_STDERR, "vector::reserve length error\n");
            abort();
        }
        oldData = vec->m_data;
        self0 = vec;
        self1 = self0;
        newData = (u16 *)fn_1_0(newCap * 2);
        if (newData == 0) {
            fn_1_234(M536_STDERR, "Memory allocation failure");
            abort();
        }
        vec->m_data = newData;
        if (oldData != 0) {
            e0 = oldData + vec->m_size;
            n = e0 - oldData;
            memcpy(vec->m_data, oldData, n * 2);
            fn_1_C8(oldData);
        }
        vec->m_cap = newCap;
    }
}

void fn_1_D61C(void)
{
    M536PlayerList *list;
    M536PlayerList *obj;

    lbl_1_bss_A80 = 0x78;
    lbl_1_bss_A84 = 0xD2;
    lbl_1_bss_A88 = 0xD2;
    list = &lbl_1_bss_A98;
    fn_1_1264(list);
    obj = list;
    __register_global_object(obj, (void *)fn_1_D6A4, &lbl_1_bss_A8C);
}

void *fn_1_D6A4(void *node, s16 freeIt)
{
    if (node != 0) {
        fn_1_1270(node, 0);
        if (freeIt > 0) {
            fn_1_C8(node);
        }
    }
    return node;
}

}
