#include "REL/m537Player.h"
#include <math.h>

// Players of the minigame (part A: game logic, motions, shift helpers). Part B (deque/vector/list
// templates and the angle helpers) lives in player_b.inc.

#ifdef TARGET_PC
#define M537_PTMF_CALL(obj) ((void (*)(M537Player *))(obj)->hook.fn)(obj)
#else
#define M537_PTMF_CALL(obj) __ptmf_scall(obj)
#endif

#define M537_EACH_PLAYER(p)                                                                              \
    for (M537Player *p = (M537Player *)lbl_1_bss_A40.next; (M537_NODE *)p != &lbl_1_bss_A40;             \
         p = (M537Player *)p->node.next)

extern "C" {

M537_NODE lbl_1_bss_A40;
s32 lbl_1_bss_A48;

M537_RODATA const Vec lbl_1_rodata_32C[4] = {
    { 150.0f, 0.0f, -100.0f }, { -150.0f, 0.0f, -100.0f }, { 60.0f, 0.0f, -200.0f }, { -60.0f, 0.0f, -200.0f }
};
M537_RODATA const f32 lbl_1_rodata_378[16][4] = {
    { 45.0f, -45.0f, 135.0f, -135.0f }, { 45.0f, -45.0f, 135.0f, 0.0f },   { 45.0f, -45.0f, 0.0f, -135.0f },
    { 45.0f, -45.0f, 90.0f, -90.0f },   { 45.0f, 0.0f, 135.0f, -135.0f },  { 45.0f, 0.0f, 135.0f, -180.0f },
    { 45.0f, -90.0f, 90.0f, -135.0f },  { 45.0f, 0.0f, 120.0f, -120.0f },  { 0.0f, -45.0f, 135.0f, -135.0f },
    { 90.0f, -45.0f, 135.0f, -90.0f },  { 0.0f, -45.0f, 180.0f, -135.0f }, { 0.0f, -45.0f, 120.0f, -120.0f },
    { 90.0f, -90.0f, 135.0f, -135.0f }, { 60.0f, -60.0f, 135.0f, -180.0f }, { 60.0f, -60.0f, 180.0f, -135.0f },
    { 45.0f, -45.0f, 135.0f, -135.0f }
};
// The original clamps the character number to 13 on a 13 entry table, so entry 13 reads the next
// literal (-100.0f); it is part of the table here to keep that read in bounds.
M537_RODATA const f32 lbl_1_rodata_498[14] = { 120.0f, 120.0f, 40.0f,  0.0f,   20.0f,  40.0f, 0.0f,
                                               120.0f, 120.0f, 120.0f, 120.0f, 120.0f, 120.0f, -100.0f };

M537_PTMF lbl_1_data_45C = { 0, -1, (void *)fn_1_AED4 };
M537_PTMF lbl_1_data_468 = { 0, -1, (void *)fn_1_B478 };
M537_PTMF lbl_1_data_474 = { 0, -1, (void *)fn_1_B4DC };
M537_PTMF lbl_1_data_480 = { 0, -1, (void *)fn_1_B4DC };
M537_PTMF lbl_1_data_48C = { 0, -1, (void *)fn_1_AE1C };
M537_PTMF lbl_1_data_498 = { 0, -1, (void *)fn_1_B0F8 };
M537_PTMF lbl_1_data_4A4 = { 0, -1, (void *)fn_1_AE98 };
M537_PTMF lbl_1_data_4B0 = { 0, -1, (void *)fn_1_B2D8 };
u8 lbl_1_data_4C5 = 0;

// Stereo pan (0..127) of a world position.
static s16 M537PanOf(Vec *pos)
{
    Vec out;
    s32 v;

    Hu3D3Dto2D(pos, 1, &out);
    out.x = out.x * 0.055555556f;
    v = (s32)(48.0f + out.x);
    if (v < 0) {
        return 0;
    }
    if (v > 0x7F) {
        return 0x7F;
    }
    return (s16)v;
}

static void M537CheckA0C(void)
{
    if (lbl_1_bss_A0C != 0) {
    } else {
        fn_1_1D4("ms_pointer != 0", "singleton.h", 0x27);
    }
}

s16 fn_1_9930(Vec *pos)
{
    return M537PanOf(pos);
}

void fn_1_99D0(void)
{
    s32 i;

    if (lbl_1_bss_A14 != 0) {
    } else {
        fn_1_1D4("CMapMan::GetSingletonPtr() != NULL", "player.cpp", 0x34);
    }
    for (i = 0; i < 4; i++) {
#ifdef TARGET_PC
        M537Player *p = (M537Player *)fn_1_0(sizeof(M537Player));
#else
        M537Player *p = (M537Player *)fn_1_0(0xC0);
#endif
        if (p != 0) {
            if (lbl_1_bss_A14 != 0) {
            } else {
                fn_1_1D4("ms_pointer != 0", "singleton.h", 0x27);
            }
            fn_1_A164(p, i, fn_1_96CC(lbl_1_bss_A14, i));
        }
    }
    CharEffectLayerSet(5);
    lbl_1_bss_A48 = 4;
}

// Per frame update of every player, then the first one moves to the back of the list.
void fn_1_9ACC(void)
{
    M537_NODE *first;

    fn_1_C0B0(&lbl_1_bss_A40, fn_1_A67C);
    fn_1_C0B0(&lbl_1_bss_A40, fn_1_A7E0);
    first = lbl_1_bss_A40.next;
    if (first != &lbl_1_bss_A40) {
        fn_1_12DC(first);
        fn_1_1330(first, &lbl_1_bss_A40);
    }
}

void fn_1_9B64(void)
{
    M537_EACH_PLAYER(p)
    {
        p->hook = lbl_1_data_45C;
    }
    fn_1_AC88();
}

void fn_1_9BFC(void)
{
    M537_EACH_PLAYER(p)
    {
        if (p->done == 0) {
            s32 idx;

            if (lbl_1_bss_A48 < 4) {
                fn_1_255C(p->idx);
                idx = p->idx;
                if (_CheckFlag(0x1000F) == 0) {
                    GwPlayer[idx].mgCoin = 0;
                }
            } else {
                idx = p->idx;
                if (_CheckFlag(0x1000F) == 0) {
                    GwPlayer[idx].mgCoin = 3;
                }
            }
            p->hook = lbl_1_data_468;
            fn_1_BCA4(p, 5, 0);
        }
    }
    M537CheckA0C();
    fn_1_10300(lbl_1_bss_A0C);
}

void fn_1_9D74(void)
{
    M537Player *p = (M537Player *)lbl_1_bss_A40.next;

    if (lbl_1_bss_A48 >= 4) {
        s16 ids[4] = { -1, -1, -1, -1 };
        s16 *w = ids;

        for (; (M537_NODE *)p != &lbl_1_bss_A40; p = (M537Player *)p->node.next, w++) {
            fn_1_BCA4(p, 9, 0);
            p->hook = lbl_1_data_474;
            *w = (s16)p->charNo;
        }
        CharLoseVoicePlay(ids[0], ids[1], ids[2], ids[3]);
    } else {
        for (; (M537_NODE *)p != &lbl_1_bss_A40; p = (M537Player *)p->node.next) {
            if (p->done == 0) {
                fn_1_BCA4(p, 8, 0);
                p->hook = lbl_1_data_480;
            }
        }
    }
}

void fn_1_9EE4(void)
{
    while (!M537ListEmpty(&lbl_1_bss_A40)) {
        fn_1_A574((M537Player *)lbl_1_bss_A40.prev, 1);
    }
}

// Average of the alive players' item hook positions.
Vec *fn_1_9F88(Vec *out)
{
    f32 inv;

    if (lbl_1_bss_A48 <= 1) {
        return out;
    }
    out->x = 0.0f;
    out->y = 0.0f;
    out->z = 0.0f;
    M537_EACH_PLAYER(p)
    {
        if (p->done == 0) {
            Vec tmp;

            fn_1_BA98(p, &tmp);
            PSVECAdd(&tmp, out, out);
        }
    }
    inv = (f32)(1.0 / (f64)lbl_1_bss_A48);
    PSVECScale(out, out, inv);
    return out;
}

s32 fn_1_A0C4(void)
{
    return lbl_1_bss_A48;
}

// Player number of the last one alive, -1 if there is more or less than one.
s32 fn_1_A0D4(void)
{
    if (lbl_1_bss_A48 != 1) {
        return -1;
    }
    M537_EACH_PLAYER(p)
    {
        if (p->done == 0) {
            return p->idx;
        }
    }
    return -1;
}

M537Player *fn_1_A164(M537Player *self, s32 idx, M537Dorrie *dorrie)
{
    M537_DEQUE tmp;
    s32 pad;
    s32 alloc;

    fn_1_1264(self);
    fn_1_2D18(&self->mdl, GwPlayerConf[idx].charNo, 2);
    self->hook = lbl_1_data_48C;
    self->idx = idx;
    self->charNo = GwPlayerConf[idx].charNo;
    if (GwPlayerConf[idx].type != 0) {
        pad = -1;
    } else {
        pad = GwPlayerConf[idx].padNo;
    }
    self->pad = pad;
    self->dif = GwPlayerConf[idx].dif;
    self->aiBonus = 0;
    self->vibCool = 0;
    self->fallSec = 1;
    self->maxVal = 170.0;
    self->midVal = 80.0;
    self->minVal = -40.0;
    self->dorrie = dorrie;
    self->timer = 0;
    self->done = 0;
    self->beatCnt = 0;
    fn_1_CBCC(&tmp, &alloc);
    fn_1_C8A4(&self->beats, &tmp);
    fn_1_C170(&tmp);
    if (tmp.map.data != 0) {
        fn_1_C8(tmp.map.data);
    }
    self->cur = 0.0;
    self->smooth = 0.0;
    self->rotY = 0.0f;
    self->mots.cap = 0;
    self->mots.size = 0;
    self->mots.data = 0;
    self->motion = -1;
    self->motTime = 0.0f;
    fn_1_1330(self, &lbl_1_bss_A40);
    fn_1_304C(&self->mdl, 5);
    fn_1_31E4(&self->mdl);
    self->pos.x = 0.0f;
    self->pos.y = 0.0f;
    self->pos.z = 0.0f;
    self->vel.x = 0.0f;
    self->vel.y = 0.0f;
    self->vel.z = 0.0f;
    fn_1_BB18(self);
    fn_1_B80C(self);
    return self;
}

M537_DEQUE *fn_1_A3FC(M537_DEQUE *self, s16 freeIt)
{
    if (self != 0) {
        fn_1_C170(self);
        if (self->map.data != 0) {
            fn_1_C8(self->map.data);
        }
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M537_DEQUE *fn_1_A490(M537_DEQUE *self, s16 freeIt)
{
    if (self != 0) {
        fn_1_C170(self);
        if (self->map.data != 0) {
            fn_1_C8(self->map.data);
        }
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M537_NODE *fn_1_A51C(M537_NODE *self, s16 freeIt)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M537Player *fn_1_A574(M537Player *self, s16 freeIt)
{
    if (self != 0) {
        if (self->mots.data != 0) {
            fn_1_C8(self->mots.data);
        }
        fn_1_C170(&self->beats);
        if (self->beats.map.data != 0) {
            fn_1_C8(self->beats.map.data);
        }
        fn_1_30F4(&self->mdl, 0);
        fn_1_1270(self, 0);
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

// Per frame: run the state hook, then place the model and the voice.
void fn_1_A67C(M537Player *self)
{
    Vec rot;

    M537_PTMF_CALL(self);
    rot.x = 0.0f;
    rot.y = self->rotY;
    rot.z = 0.0f;
    fn_1_2F3C(&self->mdl, &rot);
    fn_1_2EF4(&self->mdl, &self->pos);
    CharModelVoicePanSet(self->charNo, M537PanOf(&self->pos));
    if (self->vibCool > 0) {
        self->vibCool--;
    }
    if (self->timer > 0) {
        self->timer--;
    }
    fn_1_6278(self->dorrie);
    self->motTime = fn_1_BDA4(self);
}

// Player reached the end of its gauge: it is thrown towards the water.
void fn_1_A7E0(M537Player *self)
{
    if (self->done == 0 && self->timer <= 0 && fn_1_B9D8(self) == 0) {
        const Vec *target = &lbl_1_rodata_32C[self->idx];
        f32 dz = target->z - self->pos.z;
        f32 dx = target->x - self->pos.x;
        f32 t;
        f32 inv;
        s32 count;
        s32 idx;

        self->vel.x = dx;
        self->vel.y = 0.0f;
        self->vel.z = dz;
        t = 2.0f * self->pos.y / 0.27240694f;
        inv = (f32)(1.0 / sqrt((f64)t));
        PSVECScale(&self->vel, &self->vel, inv);
        self->hook = lbl_1_data_498;
        fn_1_BCA4(self, 6, 1);
        fn_1_BE10(self, 1);
        fn_1_BED0(self, 0x7DA);
        self->done = 1;
        fn_1_672C(self->dorrie);
        lbl_1_bss_A48--;
        fn_1_AB34();
        count = (s16)lbl_1_bss_A48;
        idx = self->idx;
        if (_CheckFlag(0x1000F) == 0) {
            GwPlayer[idx].mgCoin = (s16)count;
        }
        fn_1_AC88();
    }
}

// Assigns every remaining player its slot of the formation table.
void fn_1_AB34(void)
{
    u8 mask = 0;
    const f32 *row;

    M537_EACH_PLAYER(p)
    {
        if (p->done == 0) {
            mask = (u8)(mask | (1 << (3 - p->idx)));
        }
    }
    row = lbl_1_rodata_378[mask];
    M537_EACH_PLAYER(p)
    {
        if (p->done != 0 || lbl_1_bss_A48 != 1) {
            fn_1_66D0(p->dorrie, (f64)row[p->idx], (f64)p->fallSec);
            p->timer = p->fallSec * 0x3C;
            fn_1_B80C(p);
        }
    }
}

// Spreads the CPU levels so that the CPUs are not all alike when no human is alive.
void fn_1_AC88(void)
{
    if (lbl_1_bss_A48 < 2) {
        return;
    }
    M537_EACH_PLAYER(p)
    {
        if (p->done == 0 && p->pad >= 0) {
            return;
        }
    }
    {
        s32 lo = 3;
        s32 hi = 0;

        M537_EACH_PLAYER(p)
        {
            if (p->done == 0 && p->pad < 0) {
                p->aiBonus = 0;
                if (p->dif < lo) {
                    lo = p->dif;
                }
                if (p->dif > hi) {
                    hi = p->dif;
                }
            }
        }
        if (hi - lo < 2) {
            s32 bonus = 2;

            M537_EACH_PLAYER(p)
            {
                if (p->done == 0 && p->pad < 0) {
                    p->aiBonus = bonus - p->dif;
                    bonus = 0;
                }
            }
        }
    }
}

void fn_1_AE1C(M537Player *self)
{
    self->smooth = self->midVal;
    self->cur = self->midVal;
    fn_1_BCA4(self, 1, 1);
    self->hook = lbl_1_data_4A4;
}

void fn_1_AE98(M537Player *self)
{
    fn_1_B4E0(self, 0);
    fn_1_B9F0(self);
}

// Hook while the gauge is playable: record this frame's press, pick the idle motion.
void fn_1_AED4(M537Player *self)
{
    u8 pressed;
    u32 q;
    u8 **slot;
    u8 *elem;
    f64 r;

    pressed = fn_1_B71C(self);
    q = self->beats.offset / self->beats.chunk;
    slot = fn_1_C084(&self->beats.map, q);
    elem = *slot + (self->beats.offset - q * self->beats.chunk);
    if (*elem != 0) {
        self->beatCnt--;
    }
    fn_1_C420(&self->beats);
    fn_1_C54C(&self->beats, &pressed);
    if (pressed != 0) {
        f64 rate = (f64)self->beatCnt / 60.0;

        fn_1_B8B8(self, 120.0 * rate);
        fn_1_BED0(self, 0x7D9);
        self->beatCnt++;
    }
    r = (self->cur - self->midVal) / (self->maxVal - self->midVal);
    if (r < 0.2) {
        fn_1_BCA4(self, self->beatCnt > 0 ? 2 : 1, 1);
    } else if (self->motTime >= fn_1_BDA4(self)) {
        if (r < 0.6) {
            fn_1_BCA4(self, 3, 1);
        } else if (r < 1.0) {
            fn_1_BCA4(self, 4, 1);
        }
    }
    fn_1_B4E0(self, self->timer > 0);
    fn_1_B9F0(self);
}

// Hook while falling.
void fn_1_B0F8(M537Player *self)
{
    s32 c = self->charNo;
    f32 force;

    if (c < 0) {
        c = 0;
    } else if (c > 13) {
        c = 13;
    }
    force = lbl_1_rodata_498[c];
    self->vel.y = self->vel.y - 0.27240694f;
    PSVECAdd(&self->vel, &self->pos, &self->pos);
    if (self->pos.y <= -100.0f) {
        Hu3DMotionForceSet(self->mdl, (char *)"ske_head", 0x200, force);
        if (lbl_1_bss_9F0 != 0) {
        } else {
            fn_1_1D4("ms_pointer != 0", "singleton.h", 0x27);
        }
        fn_1_14764(lbl_1_bss_9F0, &self->pos, -0.5f);
        fn_1_304C(&self->mdl, 2);
        fn_1_BE10(self, 2);
        fn_1_BCA4(self, 7, 1);
        self->vel.x = 0.0f;
        self->vel.y = 0.0f;
        self->vel.z = 0.0f;
        self->hook = lbl_1_data_4B0;
    }
}

// Hook after the splash: settle on the water and turn to the centre.
void fn_1_B2D8(M537Player *self)
{
    M537_WRAP wrap;
    Vec head;
    f32 ang;
    f32 h;

    fn_1_CD4C(&wrap, 0.0f, 360.0f, self->rotY);
    ang = (f32)atan2((f64)-self->pos.x, (f64)-self->pos.z);
    self->rotY = fn_1_BF8C(&wrap, (f32)(180.0 * ((f64)ang / 3.141592653589793)), 0.01);
    if (lbl_1_bss_9F0 != 0) {
    } else {
        fn_1_1D4("ms_pointer != 0", "singleton.h", 0x27);
    }
    h = fn_1_145A0(lbl_1_bss_9F0, &self->pos);
    Hu3DModelObjPosGet(self->mdl, (char *)"ske_head", &head);
    self->pos.y = self->pos.y + (h - head.y);
}

void fn_1_B478(M537Player *self)
{
    if (self->motion != 0 && CharMotionEndCheck(self->charNo) != 0) {
        fn_1_BCA4(self, 0, 1);
    }
    fn_1_B4E0(self, 0);
}

void fn_1_B4DC(M537Player *self)
{
}

// Smooths the gauge value and places the player on the circle around the dorrie.
void fn_1_B4E0(M537Player *self, u8 reset)
{
    Vec local;
    Mtx mtx;
    f32 ang;

    if (reset != 0) {
        self->cur = self->midVal;
    }
    self->smooth = 0.9 * self->smooth + 0.1 * self->cur;
    local.x = 0.0f;
    local.y = 0.0f;
    local.z = (f32)self->smooth;
    fn_1_6558(self->dorrie, 0, mtx);
    PSMTXMultVec(mtx, &local, &self->pos);
    ang = (f32)atan2((f64)-self->pos.x, (f64)-self->pos.z);
    self->rotY = (f32)(180.0 * ((f64)ang / 3.141592653589793));
}

u8 fn_1_B66C(s32 pad)
{
    return (HuPadBtnDown[pad] & 0x100) != 0;
}

// CPU press chance: (2 * level + 4) / 60 per frame.
u8 fn_1_B6AC(s32 n)
{
    return (f64)fn_1_4D1C() < (1.0 / 60.0) * (f64)(n * 2 + 4);
}

u8 fn_1_B71C(M537Player *self)
{
    if (self->timer > 0) {
        return 0;
    }
    if (self->pad >= 0) {
        return fn_1_B66C(self->pad);
    }
    return fn_1_B6AC(self->dif + self->aiBonus);
}

// Empties the press history and refills it with FPS (60) released frames.
void fn_1_B80C(M537Player *self)
{
    s32 i;

    while (self->beats.size != 0) {
        fn_1_C420(&self->beats);
    }
    for (i = 0; i < 0x3C; i++) {
        fn_1_C54C(&self->beats, &lbl_1_data_4C5);
    }
    self->beatCnt = 0;
    if (self->beats.size != 0x3C) {
        fn_1_1D4("m_beat_rec.size() == FPS", "player.cpp", 0x246);
    }
}

// A press lowers this player's gauge by amount (to at least minVal) and raises the others'.
void fn_1_B8B8(M537Player *self, f64 amount)
{
    f64 lo = self->minVal;
    f64 v = self->cur - amount;

    if (v <= lo) {
        v = lo;
    }
    self->cur = v;
    if (lbl_1_bss_A48 >= 2) {
        f64 share = amount / (f64)(lbl_1_bss_A48 - 1);

        M537_EACH_PLAYER(p)
        {
            if (p->done == 0 && p->idx != self->idx) {
                p->cur = p->cur + share;
                fn_1_BE10(p, 0);
            }
        }
    }
}

u8 fn_1_B9D8(M537Player *self)
{
    return self->cur < self->maxVal;
}

void fn_1_B9F0(M537Player *self)
{
    Vec v;
    M537Arm *arm;

    fn_1_BA98(self, &v);
    M537CheckA0C();
    arm = fn_1_102F0(lbl_1_bss_A0C, self->idx);
    fn_1_10BB0(arm, &v);
}

Vec *fn_1_BA98(M537Player *self, Vec *out)
{
    char *hook = CharModelItemHookGet(self->charNo, 4, 0);

    Hu3DModelObjPosGet(self->mdl, hook, out);
    return out;
}

void fn_1_BB18(M537Player *self)
{
    self->mots.size = 0;
    fn_1_CC24(&self->mots, 10);
    fn_1_BC2C(self, 0x8C0000);
    fn_1_BC2C(self, 0x87006E);
    fn_1_BC2C(self, 0x87006F);
    fn_1_BC2C(self, 0x8C0057);
    fn_1_BC2C(self, 0x8C0058);
    fn_1_BC2C(self, 0x870072);
    fn_1_BC2C(self, 0x8C002B);
    fn_1_BC2C(self, 0x8C001D);
    fn_1_BC2C(self, 0x8C002F);
    fn_1_BC2C(self, 0x8C0031);
    if (self->mots.size != 10) {
        fn_1_1D4("m_motion_id.size() == NUMBER_OF_MOTION", "player.cpp", 0x28A);
    }
    CharMotionDataClose(self->charNo);
}

void fn_1_BC2C(M537Player *self, s32 dataNum)
{
    s16 id = CharMotionCreate(self->charNo, dataNum);

    fn_1_6D0C(&self->mots, (u16 *)(self->mots.data + self->mots.size), 1, &id);
}

// Starts motion index `motion` (blended when `shift`), unless it already runs.
void fn_1_BCA4(M537Player *self, s32 motion, u8 shift)
{
    s16 mot = *fn_1_69E0(&self->mots, motion);

    if (self->motion != motion || CharMotionEndCheck(self->charNo) != 0) {
        u32 attr;

        fn_1_2E08(&self->mdl, 0x40000002);
        attr = shift != 0 ? 0x40000001 : 0;
        CharMotionShiftSet(self->charNo, mot, 0.0f, 16.0f, attr);
        self->motion = motion;
        self->motTime = 0.0f;
    }
}

f32 fn_1_BDA4(M537Player *self)
{
    if (CharMotionShiftIDGet(self->charNo) >= 0) {
        return CharMotionShiftTimeGet(self->charNo) - 1.0f;
    }
    return CharMotionTimeGet(self->charNo);
}

void fn_1_BE10(M537Player *self, s32 kind)
{
    switch (kind) {
    case 0:
        if (self->vibCool <= 0) {
            omVibrate(self->idx, 6, 3, 3);
            self->vibCool = 0x12;
        }
        break;
    case 1:
        omVibrate(self->idx, 0xC, 4, 2);
        break;
    case 2:
        omVibrate(self->idx, 0x18, 0xC, 0);
        break;
    }
}

void fn_1_BED0(M537Player *self, s32 seId)
{
    HuAudFXPlayPan(seId, M537PanOf(&self->pos));
}

f32 fn_1_BF8C(M537_WRAP *self, f32 delta, f64 scale)
{
    f32 d = fn_1_CEB0(self, delta);

    self->value = fn_1_BFE8(self, (f32)((f64)self->value + scale * (f64)d));
    return self->value;
}

// Wraps value into [lo, hi).
f32 fn_1_BFE8(M537_WRAP *self, f32 value)
{
    if (value < self->lo) {
        return self->hi - (f32)fmod((f64)(self->lo - value), (f64)self->range);
    }
    if (value >= self->hi) {
        return self->lo + (f32)fmod((f64)(value - self->hi), (f64)self->range);
    }
    return value;
}

} // extern "C"

#include "REL/m537Dll/player_b.inc"
