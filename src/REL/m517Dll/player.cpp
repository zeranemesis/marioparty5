// m517Dll player: the minigame player. A node of the a_set list, it owns a model, a
// motion table (std::vector<s16>), an input source (pad or CPU), and -- for the solo
// player -- the ball it rides and a ground blob. A PTMF state hook drives the player.
#define _MATH_H
#include "REL/m517Dll.h"
extern "C" {
#include "game/gamework.h"
}
extern "C" {
#include "game/charman.h"
}
extern "C" {
#include "game/audio.h"
}
extern "C" {
#include "game/flag.h"
}

#ifdef __MWERKS__
#define M517_RODATA __declspec(section ".rodata")
#else
#define M517_RODATA
#endif

extern "C" {

void abort(void);
void *memcpy(void *dst, const void *src, u32 n);
double atan2(double y, double x);
double sin(double x);
double cos(double x);
double fmod(double x, double y);

void __construct_array(void *ptr, void (*ctor)(void *), void *dtor, u32 size, u32 count);
void __destroy_arr(void *ptr, void *dtor, u32 size, u32 count);

// other units
void fn_1_111A8(void *self);
void fn_1_1904(M517Ball *self, Vec *from, Vec *to);
s16 *fn_1_D778(M517Vec16 *v, u32 idx);
void fn_1_D7F0(M517Vec16 *v, s16 *pos, u32 n, s16 *val);
M517PaperEff *fn_1_142AC(M517PaperEff *self, Vec *pos, Vec *target, Vec *rot, s32 idx);

extern M517Node *lbl_1_bss_A08;

// this unit
M517PlayerModels *fn_1_15500(M517PlayerModels *self);
M517ModelPtr *fn_1_15614(M517ModelPtr *self, s16 flag);
void fn_1_1566C(M517Model **p);
M517PlayerModels *fn_1_15698(M517PlayerModels *self, s16 flag);
M517PlayerModels *fn_1_15724(M517PlayerModels *self, s16 flag);
s32 fn_1_157D0(M517PlayerModels *self, s32 idx, Vec *pos, Vec *rot);
void fn_1_158A8(s32 *cnt, M517Sphere *a, M517Sphere *b);
void fn_1_158FC(M517Player *self);
void fn_1_1593C(M517Player *self);
void fn_1_15A78(M517Player *self, f32 a, f32 b);
M517Ball *fn_1_15BF0(s32 playerNo);
M517Input *fn_1_15C6C(s32 playerNo);
void fn_1_15DC0(M517Model **p, M517Model *v);
M517Player *fn_1_15DC8(M517Player *self, s32 playerNo);
void fn_1_166A0(M517DustFan **self, s16 flag);
void fn_1_166F8(M517Task **self, s16 flag);
void fn_1_16774(M517Ball **self, s16 flag);
void fn_1_167CC(M517Input **self, s16 flag);
void fn_1_16848(M517Player *self, s16 flag);
void fn_1_168A0(M517Player *self, u32 dataNum);
void fn_1_16918(M517Player *self);
void fn_1_16A7C(M517Player *self, s32 motion, u8 loop, f32 speed);
void fn_1_16B64(void);
void fn_1_16B68(M517Player *self);
void fn_1_17614(M517Player *self);
f32 fn_1_177CC(f32 cur, f32 target, f32 k);
void fn_1_178D0(M517Player *self);
void fn_1_179B0(M517Player *self);
void fn_1_17FC0(M517Player *self);
void fn_1_183C4(M517Player *self);
void fn_1_184E8(M517Player *self, u8 win);
void fn_1_186F0(Vec *out, M517Player *self);
void fn_1_1877C(M517Player *self);
void fn_1_18EDC(M517Player *self);
void fn_1_19560(M517Player *self);
f32 fn_1_195A8(Vec *v);
void fn_1_19600(M517Player *self, Vec *in);
void fn_1_19AB8(M517Player *self);
void fn_1_19B60(M517Player *self, Vec *in);
void fn_1_1A0B4(M517Player *self);
void fn_1_1A230(M517Player *self, M517Player *other);
void fn_1_1A2B8(M517Node *head, s32 *count);
void fn_1_1A378(M517Vec16 *v, u32 cap);
void fn_1_1A4A0(void);

M517Ptmf lbl_1_data_6D0[3] = {
    { 0, -1, (void (*)(void *))fn_1_111A8 },
    { 0, -1, (void (*)(void *))fn_1_17614 },
    { 0, -1, (void (*)(void *))fn_1_16B68 },
};
s32 lbl_1_data_6F4[4] = { 0x4B000D, 0x4B000F, 0x4B0010, 0x4B000E };
char lbl_1_data_704[] = "ms_pointer == 0";
char lbl_1_data_714[] = "singleton.h";
char lbl_1_data_720[] = "ms_pointer != 0";
char lbl_1_data_730[] = "1p";
char lbl_1_data_733[] = "3p";
char lbl_1_data_736[] = "4p";
char lbl_1_data_739[] = "2p";
char *lbl_1_data_73C[4] = { lbl_1_data_730, lbl_1_data_733, lbl_1_data_736, lbl_1_data_739 };
M517Ptmf lbl_1_data_74C = { 0, -1, (void (*)(void *))fn_1_19560 };
char lbl_1_data_758[] = "sm_ptr != 0";
char lbl_1_data_764[] = "a_set.h";
M517Ptmf lbl_1_data_76C = { 0, -1, (void (*)(void *))fn_1_16B64 };
char lbl_1_data_778[] = "m_mot_id.size() == NUMBEROF_MOTION";
char lbl_1_data_79B[] = "player.cpp";
M517Ptmf lbl_1_data_7A8 = { 0, -1, (void (*)(void *))fn_1_178D0 };
M517Ptmf lbl_1_data_7B4 = { 0, -1, (void (*)(void *))fn_1_17FC0 };
M517Ptmf lbl_1_data_7C0 = { 0, -1, (void (*)(void *))fn_1_179B0 };
M517Ptmf lbl_1_data_7CC = { 0, -1, (void (*)(void *))fn_1_17FC0 };
M517Ptmf lbl_1_data_7D8 = { 0, -1, (void (*)(void *))fn_1_17FC0 };
// vector<s16> error strings that follow the hook in the original object
char lbl_1_data_7D8_str[] = "vector::at index out of range\0vector::insert length error\n";
char lbl_1_data_81F[] = "Memory allocation failure";
char lbl_1_data_839[] = "vector::reserve length error\n";

// three zero doubles that open the unit's .rodata (not referenced by its code)
M517_RODATA const f64 lbl_1_rodata_588[3] = { 0.0, 0.0, 0.0 };
M517_RODATA const GXColor lbl_1_rodata_5D8 = { 0xFF, 0xE0, 0x20, 0x00 };
M517_RODATA const GXColor lbl_1_rodata_610 = { 0xFF, 0xFF, 0xFF, 0x00 };
M517_RODATA const GXColor lbl_1_rodata_624 = { 0xFF, 0xFF, 0xFF, 0x00 };

s32 lbl_1_bss_A48;
s32 lbl_1_bss_A4C;
s32 lbl_1_bss_A50;

// Hook assignment goes through a temporary copy.
#define M517_SET_HOOK(self, data)                                                                      {                                                                                                      M517Ptmf hookTmp_ = data;                                                                          (self)->hook = hookTmp_;                                                                       }

// MWCC inlines definitions into their callers inside this translation unit, which deletes calls the
// original keeps. Keep every definition out of line.
// (no dont_inline: MWCC auto-inlines earlier definitions into later callers like the original)

// The four player models: each is a Hu3D model of the stage.
M517PlayerModels *fn_1_15500(M517PlayerModels *self)
{
    s32 i;

    M517_ASSERT(lbl_1_bss_4 == 0, lbl_1_data_704, lbl_1_data_714, 0x35);
    lbl_1_bss_4 = self;
#ifdef TARGET_PC
    for (i = 0; i < 4; i++) {
        fn_1_1566C(&self->m[i]);
    }
#else
    __construct_array(self, (void (*)(void *))fn_1_1566C, (void *)fn_1_15614, 4, 4);
#endif
    for (i = 0; i < 4; i++) {
        M517Model *model = (M517Model *)fn_1_1E84(sizeof(M517Model));

        if (model != 0) {
            model = fn_1_474C(model, lbl_1_data_6F4[i]);
        }
        if (self->m[i] != model) {
            fn_1_4BAC(self->m[i], 1);
            self->m[i] = model;
        }
    }
    return self;
}

M517ModelPtr *fn_1_15614(M517ModelPtr *self, s16 flag)
{
    if (self != 0) {
        fn_1_4BAC(self->p, 1);
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

void fn_1_1566C(M517Model **p)
{
    M517Model **sp8 = p;

    fn_1_15DC0(p, 0);
}

M517PlayerModels *fn_1_15698(M517PlayerModels *self, s16 flag)
{
    if (self != 0) {
        M517_ASSERT(lbl_1_bss_4 != 0, lbl_1_data_720, lbl_1_data_714, 0x3D);
        lbl_1_bss_4 = 0;
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

M517PlayerModels *fn_1_15724(M517PlayerModels *self, s16 flag)
{
    if (self != 0) {
#ifdef TARGET_PC
        s32 i;

        for (i = 3; i >= 0; i--) {
            fn_1_15614((M517ModelPtr *)&self->m[i], -1);
        }
#else
        __destroy_arr(self, (void *)fn_1_15614, 4, 4);
#endif
        if (self != 0) {
            M517_ASSERT(lbl_1_bss_4 != 0, lbl_1_data_720, lbl_1_data_714, 0x3D);
            lbl_1_bss_4 = 0;
        }
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

// Position / rotation of the hook object "1p".."4p" of model idx; true while the motion plays.
s32 fn_1_157D0(M517PlayerModels *self, s32 idx, Vec *pos, Vec *rot)
{
    HU3DMODELID id = self->m[idx]->id;
    HSFOBJECT *obj = Hu3DModelObjPtrGet(id, lbl_1_data_73C[idx]);

    if (obj != 0) {
        pos->x = obj->mesh.curr.pos.x;
        pos->y = obj->mesh.curr.pos.y;
        pos->z = obj->mesh.curr.pos.z;
        rot->x = obj->mesh.curr.rot.x;
        rot->y = obj->mesh.curr.rot.y;
        rot->z = obj->mesh.curr.rot.z;
        rot->y = rot->y + 90.0f;
    }
    return Hu3DMotionEndCheck(id) == 0;
}

void fn_1_158A8(s32 *cnt, M517Sphere *a, M517Sphere *b)
{
    if ((u8)fn_1_6AD4(a, b) != 0) {
        *cnt = *cnt + 1;
    }
}

void fn_1_158FC(M517Player *self)
{
    fn_1_15A78(self, 250.0f, -300.0f);
}

// Send the player to the spot of its slot in front of the stage.
void fn_1_1593C(M517Player *self)
{
    self->state = 1;
    self->flagAC = 1;
    if (self->busy == 0) {
        f32 z;

        if (self->cpu != 0) {
            fn_1_13A0(self->ball);
            self->goal.x = 0.0f;
        } else {
            self->goal.x = 300.0f * (f32)(self->slot - 2);
        }
        if (self->cpu != 0) {
            z = -300.0f;
        } else {
            z = 100.0f;
        }
        self->goal.z = z;
        self->goal.y = 0.0f;
        M517_SET_HOOK(self, lbl_1_data_74C)
    }
}

// Place the player: solo player on its ball, the others side by side away from each other.
void fn_1_15A78(M517Player *self, f32 a, f32 b)
{
    s32 i;
    f32 z;

    self->sph.pos.y = 0.0f;
    if (self->cpu != 0) {
        z = b;
    } else {
        z = a;
    }
    self->sph.pos.z = z;
    if (self->cpu != 0) {
        self->sph.pos.x = 0.0f;
        fn_1_11A8(self->ball, &self->sph.pos);
        self->sph.pos.y = self->sph.pos.y + 195.0f;
        return;
    }
    for (i = -1; i <= 1; i++) {
        s32 cnt;

        self->sph.pos.x = 100.0f * (3.0f * (f32)i);
        cnt = 0;
        M517_ASSERT(lbl_1_bss_A08 != 0, lbl_1_data_758, lbl_1_data_764, 0x2E);
        fn_1_1A2B8(lbl_1_bss_A08, &cnt);
        if (cnt == 0) {
            break;
        }
    }
}

// The ball of the solo player.
M517Ball *fn_1_15BF0(s32 playerNo)
{
    M517Ball *ball;

    if (GwPlayerConf[playerNo].grp != 0) {
        return 0;
    }
    ball = (M517Ball *)fn_1_1E84(sizeof(M517Ball));
    if (ball != 0) {
        ball = fn_1_B58(ball, playerNo);
    }
    return ball;
}

// Input source of a player: pad for humans, one of the two CPU brains otherwise.
M517Input *fn_1_15C6C(s32 playerNo)
{
    M517Input *in;

    if (GwPlayerConf[playerNo].grp == 0) {
        if (GwPlayerConf[playerNo].type != 0) {
            in = (M517Input *)fn_1_1E84(sizeof(M517CpuInput2));
            if (in != 0) {
                fn_1_9930(in, GwPlayerConf[playerNo].dif);
            }
            return in;
        }
        in = (M517Input *)fn_1_1E84(sizeof(M517PadInput));
        if (in != 0) {
            fn_1_7BD4(in, playerNo);
        }
        return in;
    }
    if (GwPlayerConf[playerNo].type != 0) {
        in = (M517Input *)fn_1_1E84(sizeof(M517CpuInput));
        if (in != 0) {
            fn_1_8000(in, playerNo, GwPlayerConf[playerNo].dif);
        }
        return in;
    }
    in = (M517Input *)fn_1_1E84(sizeof(M517PadInput));
    if (in != 0) {
        fn_1_7BD4(in, playerNo);
    }
    return in;
}

void fn_1_15DC0(M517Model **p, M517Model *v)
{
    *p = v;
}

M517Player *fn_1_15DC8(M517Player *self, s32 playerNo)
{
    HU3DMODELID id;
    M517Model *model;
    M517DustFan *dust;
    s32 slot;
    s32 i;

    fn_1_2DA8(&self->link);
    self->state = 0;
    self->busy = 0;
    self->playerNo = playerNo;
    self->charNo = GwPlayerConf[playerNo].charNo;
    self->motion = -1;
    self->cpu = (u8)(GwPlayerConf[playerNo].grp == 0);
    self->fxDone = 0;
    self->flagAC = 0;
    self->reached = 0;
    self->mot.cap = 0;
    self->mot.size = 0;
    self->mot.data = 0;
    model = (M517Model *)fn_1_1E84(sizeof(M517Model));
    if (model != 0) {
        model = fn_1_47D0(model, GwPlayerConf[playerNo].charNo, 2);
    }
    self->model = model;
    self->input = fn_1_15C6C(playerNo);
    self->ball = fn_1_15BF0(playerNo);
    self->paper = 0;
    self->dust = 0;
    slot = 0;
    if (GwPlayerConf[playerNo].grp == 0) {
        slot = 0;
    } else {
        for (i = 0; i <= playerNo; i++) {
            if (GwPlayerConf[i].grp != 0) {
                slot++;
            }
        }
    }
    self->slot = slot;
    M517_ASSERT(lbl_1_bss_A08 != 0, lbl_1_data_758, lbl_1_data_764, 0x2E);
    fn_1_2E74(&self->link, lbl_1_bss_A08);
    self->sph.radius = 60.000004f;
    self->mot.size = 0;
    fn_1_1A378(&self->mot, 0xE);
    fn_1_168A0(self, 0x8C0000);
    fn_1_168A0(self, 0x8C0001);
    fn_1_168A0(self, 0x8C0002);
    fn_1_168A0(self, 0x8C0003);
    fn_1_168A0(self, 0x8C0004);
    fn_1_168A0(self, 0x8C003A);
    fn_1_168A0(self, 0x8C0013);
    fn_1_168A0(self, 0x8C0014);
    fn_1_168A0(self, 0x8C0051);
    fn_1_168A0(self, 0x87002D);
    fn_1_168A0(self, 0x870097);
    fn_1_168A0(self, 0x870098);
    fn_1_168A0(self, 0x8C0030);
    if (self->cpu == 0) {
        fn_1_168A0(self, 0x8C0031);
    } else {
        fn_1_168A0(self, 0x8C002E);
    }
    M517_ASSERT(self->mot.size == 0xE, lbl_1_data_778, lbl_1_data_79B, 0xE3);
    CharMotionDataClose(self->charNo);
    id = self->model->id;
    Hu3DModelShadowSet(id);
    Hu3DModelShadowDispOn(id);
    Hu3DModelLayerSet(id, 3);
    Hu3DModelCameraSet(id, 1);
    CharEffectLayerSet(5);
    if (self->cpu == 0) {
        CharModelStepFxSet(self->charNo, 2);
        fn_1_16A7C(self, 0, 0, 8.0f);
    } else {
        CharModelFxFlagSet(self->charNo, 0);
        fn_1_16A7C(self, 9, 0, 8.0f);
        dust = (M517DustFan *)fn_1_1E84(sizeof(M517DustFan));
        if (dust != 0) {
            dust = fn_1_C42C(dust);
        }
        if (self->dust != dust) {
            fn_1_C4F4(self->dust, 1);
            self->dust = dust;
        }
    }
    fn_1_158FC(self);
    self->rot.x = 0.0f;
    self->rot.y = 0.0f;
    self->rot.z = 0.0f;
    self->vel.x = 0.0f;
    self->vel.y = 0.0f;
    self->vel.z = 0.0f;
    self->launch.x = 0.0f;
    self->launch.y = 0.0f;
    self->launch.z = 0.0f;
    self->goal.x = 0.0f;
    self->goal.y = 0.0f;
    self->goal.z = 0.0f;
    self->delta.x = 0.0f;
    self->delta.y = 0.0f;
    self->delta.z = 0.0f;
    self->scale.x = 1.0f;
    self->scale.y = 1.0f;
    self->scale.z = 1.0f;
    self->subState = 0;
    self->phase = 0;
    self->t = 0.0f;
    self->wait = 0x2D;
    CharMotionVoiceOnSet(self->charNo, 0x13, 0);
    M517_SET_HOOK(self, lbl_1_data_76C)
    return self;
}

void fn_1_166A0(M517DustFan **self, s16 flag)
{
    if (self != 0) {
        fn_1_C4F4(*self, 1);
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
}

void fn_1_166F8(M517Task **self, s16 flag)
{
    if (self != 0) {
        M517Task *t = *self;

        if (t != 0) {
            t->vtbl->dtor(t, 1);
        }
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
}

void fn_1_16774(M517Ball **self, s16 flag)
{
    if (self != 0) {
        fn_1_10F8(*self, 1);
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
}

void fn_1_167CC(M517Input **self, s16 flag)
{
    if (self != 0) {
        M517Input *in = *self;

        if (in != 0) {
            in->vtbl->dtor(in, 1);
        }
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
}

void fn_1_16848(M517Player *self, s16 flag)
{
    if (self != 0) {
        fn_1_2DB4(&self->link, 0);
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
}

// Append the motion `dataNum` of this character to the motion table.
void fn_1_168A0(M517Player *self, u32 dataNum)
{
    u32 sp8 = dataNum;
    s16 mot = CharMotionCreate(self->charNo, dataNum);

    fn_1_D7F0(&self->mot, self->mot.data + self->mot.size, 1, &mot);
}

// Per frame: run the state hook, then publish the pose.
void fn_1_16918(M517Player *self)
{
    Vec p;
    Vec q;

    if (M517_PTMF_TEST(&self->hook)) {
        M517_PTMF_CALL(&self->hook, self);
    }
    fn_1_49AC(self->model, &self->sph.pos);
    fn_1_49F4(self->model, &self->rot);
    if (self->cpu != 0) {
        M517Stage *stage;

        M517_ASSERT(lbl_1_bss_10 != 0, lbl_1_data_720, lbl_1_data_714, 0x27);
        stage = (M517Stage *)lbl_1_bss_10;
        stage->offs.x = self->sph.pos.x;
        stage->offs.y = self->sph.pos.y;
        stage->offs.z = self->sph.pos.z;
        stage->offs.y = 0.0f;
        q.x = self->sph.pos.x;
        q.y = self->sph.pos.y;
        q.z = self->sph.pos.z;
        q.y = q.y + 5.0f;
        self->dust->pos.x = q.x;
        self->dust->pos.y = q.y;
        self->dust->pos.z = q.z;
    }
    p.x = self->sph.pos.x;
    p.y = self->sph.pos.y;
    p.z = self->sph.pos.z;
    fn_1_1B9B8(self->charNo, &p);
}

// Switch to motion `motion` (loop == 0: play it once and hold, else loop it).
void fn_1_16A7C(M517Player *self, s32 motion, u8 loop, f32 speed)
{
    if (self->motion != 5 || CharMotionEndCheck(self->charNo) != 0) {
        HU3DMODELID id = self->model->id;
        s16 mot = *fn_1_D778(&self->mot, motion);

        if (self->motion != motion) {
            u32 attr;

            Hu3DModelAttrReset(id, 0x40000002);
            if (loop == 0) {
                attr = 0x40000001;
            } else {
                attr = 0;
            }
            CharMotionShiftSet(self->charNo, mot, 0.0f, speed, attr);
            self->motion = motion;
        }
    }
}

void fn_1_16B64(void)
{
}

// Intro: fly to the stage spot, then wait and settle.
void fn_1_16B68(M517Player *self)
{
    switch (self->phase) {
        case 0: {
            Vec old;

            old = self->sph.pos;
            M517_ASSERT(lbl_1_bss_4 != 0, lbl_1_data_720, lbl_1_data_714, 0x27);
            fn_1_157D0((M517PlayerModels *)lbl_1_bss_4, self->slot, &self->sph.pos, &self->rot);
            if (self->cpu != 0) {
                self->sph.pos.y = self->sph.pos.y - self->ball->sph.radius;
                fn_1_11A8(self->ball, &self->sph.pos);
                self->sph.pos.y = self->sph.pos.y + 195.0f;
                fn_1_1904(self->ball, &old, &self->sph.pos);
                fn_1_16A7C(self, 0xA, 0, 8.0f);
            } else {
                fn_1_16A7C(self, 2, 0, 8.0f);
            }
            self->t = (f32)((f64)self->t + 0.016666666666666666);
            if (self->t < 3.0f) {
                break;
            }
            if (self->cpu != 0) {
                self->home.x = 0.0f;
                self->home.y = 0.0f;
            } else {
                self->home.x = 250.0f * (f32)(self->slot - 2);
                self->home.y = 0.0f;
                CharEffectShoeHitCreate(1, &self->sph.pos, &self->rot);
                fn_1_16A7C(self, 6, 1, 8.0f);
            }
            if (self->cpu != 0) {
                self->home.z = -200.0f;
            } else {
                self->home.z = 250.0f;
            }
            self->t = 0.0f;
            CharFXPlay(self->charNo, 0x1FD);
            self->phase++;
        }
        // fall through
        case 1:
            if (self->cpu == 0) {
                self->sph.pos.x = self->sph.pos.x + ((self->home.x - self->sph.pos.x) / 18.0f);
                self->sph.pos.y = self->sph.pos.y + ((self->home.y - self->sph.pos.y) / 18.0f);
                self->sph.pos.z = self->sph.pos.z + ((self->home.z - self->sph.pos.z) / 18.0f);
                if (self->fxDone == 0 && self->t > 0.55f) {
                    GXColor col = lbl_1_rodata_5D8;
                    M517DustCloud *cloud = (M517DustCloud *)fn_1_1E84(sizeof(M517DustCloud));

                    (void)col;
                    if (cloud != 0) {
                        cloud = fn_1_1ACD4(cloud, &self->sph.pos);
                    }
                    self->fxDone = 1;
                }
            } else {
                Vec a;
                Vec b;
                Vec d;

                a = self->sph.pos;
                b = self->sph.pos;
                self->sph.pos.x = self->sph.pos.x + (-self->sph.pos.x / 18.0f);
                self->sph.pos.z = self->sph.pos.z + ((self->home.z - self->sph.pos.z) / 18.0f);
                b.y = 0.0f;
                fn_1_11A8(self->ball, &b);
                fn_1_1904(self->ball, &a, &self->sph.pos);
                PSVECSubtract(&b, &self->home, &d);
                if (PSVECMag(&d) < 10.0f) {
                    fn_1_16A7C(self, 9, 1, 16.0f);
                } else {
                    fn_1_16A7C(self, 0xB, 0, 8.0f);
                }
            }
            self->t = (f32)((f64)self->t + 0.016666666666666666);
            if (self->t < 1.8f) {
                break;
            }
            if (self->cpu == 0) {
                self->sph.pos.x = self->home.x;
                self->sph.pos.y = self->home.y;
                self->sph.pos.z = self->home.z;
                fn_1_16A7C(self, 7, 1, 16.0f);
            } else {
                fn_1_16A7C(self, 9, 0, 8.0f);
            }
            self->rot.y = 0.0f;
            self->phase++;
            break;
    }
}

// Idle pose, then wait for the state to advance.
void fn_1_17614(M517Player *self)
{
    if (self->cpu == 0) {
        fn_1_16A7C(self, 0, 0, 16.0f);
    } else {
        fn_1_16A7C(self, 9, 0, 8.0f);
    }
    M517_SET_HOOK(self, lbl_1_data_7A8)
}

// Move angle `cur` by the fraction `k` towards `target` along the short way (degrees, [0, 360)).
f32 fn_1_177CC(f32 cur, f32 target, f32 k)
{
    f32 d;
    f32 r;

    d = (f32)fmod(target - cur, 360.0);
    if (d < 0.0f) {
        d += 360.0f;
    }
    if (d > 180.0f) {
        d -= 360.0f;
    }
    r = (f32)fmod(cur + (k * d), 360.0);
    if (r < 0.0f) {
        r += 360.0f;
    }
    return r;
}

// Play: read the input and move (solo player on its ball, the others on their own legs).
void fn_1_178D0(M517Player *self)
{
    M517Input *in = self->input;
    Vec stick;
    Vec copy;
    Vec old;

    fn_1_7B74(in);
    stick.x = in->st.x;
    stick.z = in->st.y;
    stick.y = in->st.mag;
    if (self->cpu != 0) {
        copy = stick;
        fn_1_19600(self, &copy);
        return;
    }
    old = self->sph.pos;
    copy = stick;
    fn_1_19B60(self, &copy);
    PSVECSubtract(&self->sph.pos, &old, &self->delta);
}

// Knocked away by the ball: flatten, fly off on a sheet of paper, fall back down and get up.
void fn_1_179B0(M517Player *self)
{
    switch (self->subState) {
        case 0:
            omVibrate(self->playerNo, 0xC, 0xC, 0);
            self->t2 = (f32)((f64)self->t2 + 0.016666666666666666);
            if (self->t2 < 0.2f) {
                break;
            }
            self->t2 = 0.0f;
            self->subState++;
        // fall through
        case 1: {
            f32 s;
            f32 a;
            M517Task *paper;

            self->scale.y = self->scale.y + (-self->scale.y / 12.0f);
            s = self->scale.y;
            if (s < 0.0f) {
                a = -s;
            } else {
                a = s;
            }
            if (a < 0.1f) {
                break;
            }
            self->scale.y = 0.0f;
            self->launch.y = 800.0f;
            paper = (M517Task *)fn_1_1E84(sizeof(M517PaperEff));
            if (paper != 0) {
                paper = (M517Task *)fn_1_142AC((M517PaperEff *)paper, &self->sph.pos, &self->launch, &self->rot, self->playerNo);
            }
            if (self->paper != paper) {
                M517Task *old = self->paper;

                if (old != 0) {
                    old->vtbl->dtor(old, 1);
                }
                self->paper = paper;
            }
            self->sph.pos.x = self->launch.x;
            self->sph.pos.y = self->launch.y;
            self->sph.pos.z = self->launch.z;
            fn_1_48C0(self->model, 1);
            self->subState++;
        }
        // fall through
        case 2: {
            u8 landed = ((M517PaperEff *)self->paper)->landed;

            if (landed != 1) {
                break;
            }
            if (self->paper != 0) {
                M517Task *old = self->paper;

                if (old != 0) {
                    old->vtbl->dtor(old, 1);
                }
                self->paper = 0;
            }
            fn_1_48F8(self->model, 1);
            self->scale.y = 1.0f;
            self->rot.y = 0.0f;
            CharModelAttrReset(self->charNo, 0x40000002);
            fn_1_16A7C(self, 3, 1, 8.0f);
            {
                GXColor col = lbl_1_rodata_610;
                M517DustCloud *cloud = (M517DustCloud *)fn_1_1E84(sizeof(M517DustCloud));

                if (cloud != 0) {
                    GXColor c = col;

                    cloud = fn_1_1A518(cloud, &self->sph.pos, &c, 6);
                }
            }
            self->subState++;
        }
        // fall through
        case 3:
            fn_1_19AB8(self);
            if (self->sph.pos.y > 250.0f) {
                break;
            }
            fn_1_16A7C(self, 4, 1, 8.0f);
            self->subState++;
        // fall through
        case 4:
            if (CharMotionEndCheck(self->charNo) != 0) {
                fn_1_16A7C(self, 8, 0, 8.0f);
                self->sph.pos.y = 250.0f;
                self->vel.y = 0.0f;
                M517_SET_HOOK(self, lbl_1_data_7B4)
            }
            break;
    }
    fn_1_4A3C(self->model, &self->scale);
}

void fn_1_17FC0(M517Player *self)
{
    self->state = 0;
}

// The ball hits this player: if it moves fast enough the player is knocked away.
void fn_1_17FCC(M517Player *self, M517Ball *ball)
{
    if (self->busy == 0) {
        self->sph.pos.y = 0.0f;
        if (self->state != 1) {
            if (self->flagAC == 1) {
                fn_1_1593C(self);
                return;
            }
            {
                Vec v;
                f32 mag;

                v = ball->delta;
                mag = PSVECMag(&v);
                if (mag < 6.666666666666667) {
                    return;
                }
            }
            fn_1_16A7C(self, 6, 1, 8.0f);
            fn_1_186F0(&self->launch, self);
            self->vel.x = 0.0f;
            self->vel.y = 0.0f;
            self->vel.z = 0.0f;
            self->t2 = 0.0f;
            self->busy = 1;
            {
                Vec p;
                GXColor col = lbl_1_rodata_624;
                M517DustCloud *cloud;

                p = self->sph.pos;
                p.y = p.y + 100.0f;
                cloud = (M517DustCloud *)fn_1_1E84(sizeof(M517DustCloud));
                if (cloud != 0) {
                    GXColor c = col;

                    cloud = fn_1_1A518(cloud, &p, &c, 6);
                }
            }
            Hu3DModelShadowDispOff(self->model->id);
            CharFXPlay(self->charNo, 0x202);
            M517_SET_HOOK(self, lbl_1_data_7C0)
        }
    }
}

// Lost: crying pose.
void fn_1_183C4(M517Player *self)
{
    if (self->motion != 5) {
        fn_1_16A7C(self, 5, 1, 8.0f);
        CharEffectShoeHitCreate(1, &self->sph.pos, &self->rot);
        CharModelCryCreate(self->charNo, 30.0f, 0.8f);
        CharFXPlay(self->charNo, 0x1FD);
        self->wait = 0;
    }
}

// Result pose: `win` cheers (and scores the coin bonus), otherwise sulks.
void fn_1_184E8(M517Player *self, u8 win)
{
    if (win != 0) {
        s16 coin;
        s16 add;
        s32 p;

        fn_1_16A7C(self, 0xC, 1, 8.0f);
        coin = GwPlayer[self->playerNo].mgCoin;
        add = (s16)(coin + 10);
        p = self->playerNo;
        if (_CheckFlag(0x1000F) == 0) {
            GwPlayer[p].mgCoin = add;
        }
        fn_1_4014(self->playerNo);
        if (self->cpu != 0) {
            CharFXPlay(self->charNo, 0x1FA);
        }
    } else {
        fn_1_16A7C(self, 0xD, 1, 8.0f);
    }
}

// Point where a knocked-away player lands.
void fn_1_186F0(Vec *out, M517Player *self)
{
    Vec v;

    v.x = 150.0f * (f32)(self->slot - 2);
    v.y = 0.0f;
    v.z = -800.0f;
    out->x = v.x;
    out->y = v.y;
    out->z = v.z;
}

// Solo player walking to its goal; once there it turns to the front and goes idle.
void fn_1_1877C(M517Player *self)
{
    Vec cur;
    Vec delta;
    f32 dist;
    s32 mot;

    self->sph.pos.y = 0.0f;
    cur = self->sph.pos;
    PSVECSubtract(&self->goal, &cur, &delta);
    dist = PSVECMag(&delta);
    mot = 0xA;
    if (self->reached == 0) {
        f32 spd;

        if (dist < 50.0f) {
            f32 k = 1.0f - ((50.0f - dist) / 50.0f);
            f32 ang;

            spd = (f32)(5.0 * (f64)k);
            mot = 0xB;
            ang = (f32)(180.0 * (atan2(delta.x, delta.z) / 3.141592653589793));
            self->rot.y = fn_1_177CC(self->rot.y, ang, 0.2f * k);
        } else {
            f32 ang;

            spd = 10.0f;
            ang = (f32)(180.0 * (atan2(delta.x, delta.z) / 3.141592653589793));
            self->rot.y = fn_1_177CC(self->rot.y, ang, 0.2f);
        }
        if (dist < 20.0f) {
            self->sph.pos.x = self->goal.x;
            self->sph.pos.y = self->goal.y;
            self->sph.pos.z = self->goal.z;
            self->reached = 1;
        } else {
            f32 r = (f32)((3.141592653589793 * (f64)self->rot.y) / 180.0);

            self->sph.pos.x = (f32)((f64)self->sph.pos.x + ((f64)spd * sin(r)));
            self->sph.pos.z = (f32)((f64)self->sph.pos.z + ((f64)spd * cos(r)));
            fn_1_16A7C(self, mot, 0, 8.0f);
        }
    } else {
        self->rot.y = fn_1_177CC(self->rot.y, 0.0f, 0.1f);
        if (self->rot.y < 0.2f || self->rot.y > 359.8f) {
            self->rot.y = 0.0f;
            fn_1_16A7C(self, 9, 0, 8.0f);
            M517_SET_HOOK(self, lbl_1_data_7CC)
        }
    }
    fn_1_11A8(self->ball, &self->sph.pos);
    self->sph.pos.y = self->sph.pos.y + 195.0f;
    fn_1_1904(self->ball, &cur, &self->sph.pos);
}

// Team member walking to its goal.
void fn_1_18EDC(M517Player *self)
{
    Vec cur;
    Vec delta;
    f32 dist;
    f32 spd;
    f32 ang;

    cur = self->sph.pos;
    cur.y = 0.0f;
    PSVECSubtract(&self->goal, &cur, &delta);
    dist = PSVECMag(&delta);
    spd = 0.0f;
    if (dist < 50.0f) {
        f32 k = 1.0f - ((50.0f - dist) / 50.0f);

        spd = (f32)(7.5 * (f64)k);
        ang = (f32)(180.0 * (atan2(delta.x, delta.z) / 3.141592653589793));
        self->rot.y = fn_1_177CC(self->rot.y, ang, 0.2f * k);
        fn_1_16A7C(self, 1, 0, 8.0f);
    } else {
        spd = 10.0f;
        fn_1_16A7C(self, 2, 0, 8.0f);
        ang = (f32)(180.0 * (atan2(delta.x, delta.z) / 3.141592653589793));
        self->rot.y = fn_1_177CC(self->rot.y, ang, 0.2f);
    }
    if (dist < 30.000002f) {
        self->sph.pos.x = self->goal.x;
        self->sph.pos.y = self->goal.y;
        self->sph.pos.z = self->goal.z;
        self->rot.y = 0.0f;
        fn_1_16A7C(self, 0, 0, 8.0f);
        M517_SET_HOOK(self, lbl_1_data_7D8)
        return;
    }
    {
        f32 r = (f32)((3.141592653589793 * (f64)self->rot.y) / 180.0);

        self->sph.pos.x = (f32)((f64)self->sph.pos.x + ((f64)spd * sin(r)));
        self->sph.pos.z = (f32)((f64)self->sph.pos.z + ((f64)spd * cos(r)));
    }
}

void fn_1_19560(M517Player *self)
{
    if (self->cpu != 0) {
        fn_1_1877C(self);
    } else {
        fn_1_18EDC(self);
    }
}

// Heading (degrees) of the stick vector.
f32 fn_1_195A8(Vec *v)
{
    return (f32)((180.0 * atan2(v->x, v->z)) / 3.141592653589793);
}

// Solo player riding the ball: follow the ball, face the stick direction, pick the walk motion by speed.
void fn_1_19600(M517Player *self, Vec *in)
{
    f32 spd;

    self->sph.pos.x = self->ball->sph.pos.x;
    self->sph.pos.y = self->ball->sph.pos.y;
    self->sph.pos.z = self->ball->sph.pos.z;
    self->sph.pos.y = self->sph.pos.y + 95.0f;
    if (in->y > 0.2f) {
        f32 ang = (f32)((180.0 * atan2(in->x, in->z)) / 3.141592653589793);

        self->rot.y = fn_1_177CC(self->rot.y, ang, 0.2f);
    }
    spd = fn_1_12E8(self->ball);
    if (spd < 0.05f) {
        fn_1_16A7C(self, 9, 0, 8.0f);
    } else {
        f32 rate = 2.0f * spd;

        if (rate > 1.0f) {
            rate = 1.0f;
        }
        CharMotionSpeedSet(self->charNo, rate);
        if (spd < 0.4f) {
            fn_1_16A7C(self, 0xB, 0, 8.0f);
        } else {
            fn_1_16A7C(self, 0xA, 0, 8.0f);
        }
    }
    in->y = 0.0f;
    fn_1_1384(self->ball, in);
}

// Gravity step: the player falls with an increasing speed.
void fn_1_19AB8(M517Player *self)
{
    Vec v;
    Vec tmp;

    v = self->vel;
    self->vel.y = (f32)((f64)self->vel.y + 0.27240694444444447);
    v.y = -(0.5f * (v.y + self->vel.y));
    tmp = self->sph.pos;
    PSVECAdd(&v, &self->sph.pos, &self->sph.pos);
}

// Team member on its own legs: face the stick direction, walk or run, stay on the stage.
void fn_1_19B60(M517Player *self, Vec *in)
{
    f32 spd;
    f32 mag;
    f32 rad;

    if (self->wait < 0x2D) {
        self->wait++;
        in->y = 0.0f;
    } else if (in->y > 0.2f) {
        f32 ang = (f32)((180.0 * atan2(in->x, in->z)) / 3.141592653589793);

        self->rot.y = fn_1_177CC(self->rot.y, ang, 0.2f);
    }
    spd = 0.0f;
    mag = in->y;
    if (mag < 0.1f) {
        fn_1_16A7C(self, 0, 0, 8.0f);
    } else if (mag < 0.6f) {
        fn_1_16A7C(self, 1, 0, 8.0f);
        spd = (f32)(7.5 * (f64)mag);
    } else {
        fn_1_16A7C(self, 2, 0, 8.0f);
        spd = 10.0f;
    }
    if (self->motion == 5) {
        spd = 4.1666665f;
        omVibrate(self->playerNo, 0xC, 0xC, 0);
    }
    rad = 0.017453292f * self->rot.y;
    self->vel.x = (f32)((f64)spd * sin(rad));
    self->vel.z = (f32)((f64)spd * cos(rad));
    self->sph.pos.y = 0.0f;
    self->vel.y = 0.0f;
    fn_1_19AB8(self);
    fn_1_1A0B4(self);
}

// Keep the player inside the stage circle (radius 550).
void fn_1_1A0B4(M517Player *self)
{
    Vec p;
    Vec off;
    f32 m;

    M517_ASSERT(lbl_1_bss_10 != 0, lbl_1_data_720, lbl_1_data_714, 0x27);
    p.x = self->sph.pos.x;
    p.y = 0.0f;
    p.z = self->sph.pos.z;
    m = PSVECMag(&p);
    if (m > 550.0f) {
        PSVECScale(&p, &off, (550.0f - m) / m);
        PSVECAdd(&off, &self->sph.pos, &self->sph.pos);
        self->vel.x = 0.0f;
        self->vel.y = 0.0f;
        self->vel.z = 0.0f;
    }
}

// Player against player collision.
void fn_1_1A230(M517Player *self, M517Player *other)
{
    if (self->busy == 0 && other->busy == 0) {
        fn_1_6B7C(&self->sph, &other->sph, 0.5f, 1.0f);
    }
}

// Number of overlapping pairs of players of the list.
void fn_1_1A2B8(M517Node *head, s32 *count)
{
    M517Node *a = head->next;
    M517Node *last = head->prev;

    while (a != last) {
        M517Node *b = a->next;

        while (b != head) {
            if ((u8)fn_1_6AD4(&((M517Player *)a)->sph, &((M517Player *)b)->sph) != 0) {
                *count = *count + 1;
            }
            b = b->next;
        }
        a = a->next;
    }
}

// vector<s16>::reserve(cap)
void fn_1_1A378(M517Vec16 *v, u32 cap)
{
    if (cap > v->cap) {
        u32 max = 0x7FFFFFFF;
        s16 *old;
        s16 *mem;

        if (cap > max) {
            fn_1_20B8(M517_STDERR, lbl_1_data_839);
            abort();
        }
        old = v->data;
        mem = (s16 *)fn_1_1E84(cap * 2);
        if (mem == 0) {
            fn_1_20B8(M517_STDERR, lbl_1_data_81F);
            abort();
        }
        v->data = mem;
        if (old != 0) {
            memcpy(v->data, old, (u32)(&old[v->size] - old) * 2);
            fn_1_1F4C(old);
        }
        v->cap = cap;
    }
}

void fn_1_1A4A0(void)
{
    lbl_1_bss_A48 = 0x78;
    lbl_1_bss_A4C = 0xD2;
    lbl_1_bss_A50 = 0xD2;
}

}

// static initialiser of this unit (in .ctors on GameCube)
#ifdef __MWERKS__
__declspec(section ".ctors") void (*const lbl_ctor_1A4A0)(void) = fn_1_1A4A0;
#else
struct PortStaticInit_1A4A0 {
    PortStaticInit_1A4A0() { fn_1_1A4A0(); }
};
static PortStaticInit_1A4A0 lbl_port_init_1A4A0;
#endif
