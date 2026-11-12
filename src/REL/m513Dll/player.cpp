// m513Dll player: the minigame player (a character model that hits the leaf
// when the input fires), its input source and the smoke effects it spawns.
#define _MATH_H
#include "REL/m513Dll.h"
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
#include "game/frand.h"
}

#ifdef __MWERKS__
#define M513_RODATA __declspec(section ".rodata")
#else
#define M513_RODATA
#endif



extern "C" {

void __construct_array(void *ptr, void (*ctor)(void *), void *dtor, u32 size, u32 count);
void __destroy_arr(void *ptr, void *dtor, u32 size, u32 count);

void fn_1_2A60(s32 playerNo);
void fn_1_BAF0(void);
void fn_1_E790(M513Heiho *self);
void fn_1_6578(M513Input *self);
M513Input *fn_1_65E0(M513Input *self, s32 playerNo);
M513Input *fn_1_69EC(M513Input *self, s32 playerNo, s32 level);
M513Leaf *fn_1_7F04(M513Leaf *self, s32 idx, Vec *pos, Vec *rot);
M513Leaf *fn_1_8310(M513Leaf *self, s16 flag);
void fn_1_85F4(M513Leaf *self, f32 speed);
M513PowerEff *fn_1_98A0(M513PowerEff *self);
void fn_1_9FE4(M513PowerEff *self, Vec *pos, Vec *target, f32 limit);
void fn_1_13C0C(M513Score *self);
void fn_1_13C78(M513Score *self, s32 idx);
s32 fn_1_13E34(M513Score *self, s32 idx);
void fn_1_145A8(s32 seId, Vec *pos);
void fn_1_14918(s16 charNo, Vec *pos);
s16 *fn_1_E890(M513Vec16 *v, u32 idx);
void fn_1_E908(M513Vec16 *v, s16 *pos, u32 n, s16 *val);
void fn_1_ECB8(M513Vec16 *v, u32 cap);
M513Model *fn_1_321C(M513Model *self, s32 charNo, s32 type);

extern M513Node *lbl_1_bss_A10;
extern M513Node *lbl_1_bss_A14;
extern M513Camera *lbl_1_bss_A18;
extern M513Score *lbl_1_bss_A20;

void fn_1_11F84(M513PowerEff **elem);
void fn_1_12008(M513Player *self, u32 dataNum);
void fn_1_1212C(M513Player *self, s32 motion, u8 force, u8 loop, f32 speed);
void fn_1_12204(void);
void fn_1_12208(void);
void fn_1_1220C(M513Player *self);
void fn_1_122F8(M513Player *self);
void fn_1_128AC(M513Player *self);
void fn_1_12B20(M513Player *self);
void fn_1_12D94(M513Player *self);
void fn_1_13080(M513Player *self);
void fn_1_13138(M513PowerEff **p, M513PowerEff *v);

M513_RODATA const f32 lbl_1_rodata_498[12] = {
    -300.0f, 0.0f, -250.0f, -100.0f, 0.0f, -250.0f, 100.0f, 0.0f, -250.0f, 300.0f, 0.0f, -250.0f,
};
M513_RODATA const f32 lbl_1_rodata_4C8[3] = { 0.0f, 180.0f, 0.0f };
M513_RODATA const f32 lbl_1_rodata_500[13] = {
    90.0f, 90.0f, 110.0f, 80.0f, 100.0f, 110.0f, 120.00001f, 50.0f, 80.0f, 100.0f, 100.0f, 100.0f, 100.0f,
};

M513Ptmf lbl_1_data_7E0 = { 0, -1, (void (*)(void *))fn_1_1220C };
M513Ptmf lbl_1_data_7EC = { 0, -1, (void (*)(void *))fn_1_12208 };
M513Ptmf lbl_1_data_7F8 = { 0, -1, (void (*)(void *))fn_1_12204 };
char lbl_1_data_804[] = "sm_ptr != 0";
char lbl_1_data_810[] = "a_set.h";
char lbl_1_data_818[] = "m_mot_id.size() == NUMBEROF_MOTION";
char lbl_1_data_83B[] = "player.cpp";
M513Ptmf lbl_1_data_848 = { 0, -1, (void (*)(void *))fn_1_122F8 };
M513Ptmf lbl_1_data_854 = { 0, -1, (void (*)(void *))fn_1_128AC };
char lbl_1_data_860[] = "ms_pointer != 0";
char lbl_1_data_870[] = "singleton.h";
M513Ptmf lbl_1_data_87C = { 0, -1, (void (*)(void *))fn_1_12D94 };
M513Ptmf lbl_1_data_888 = { 0, -1, (void (*)(void *))fn_1_12D94 };
M513Ptmf lbl_1_data_894 = { 0, -1, (void (*)(void *))fn_1_12B20 };
// vector<s16> error strings (not referenced by this unit's code)
char lbl_1_data_8A0[] = "vector::at index out of range\0vector::insert length error\n\0Memory allocation failure\0vector::reserve length error\n";

s32 lbl_1_bss_A38;
s32 lbl_1_bss_A3C;
s32 lbl_1_bss_A40;
u8 lbl_1_bss_A44;

// Hook assignment goes through a temporary copy.
#define M513_SET_HOOK(self, data)                                                                      {                                                                                                      M513Ptmf hookTmp_ = data;                                                                          (self)->hook = hookTmp_;                                                                       }

// Input source of a player: pad for humans, CPU otherwise.
M513Input *fn_1_11844(s32 playerNo)
{
    M513Input *p;

    if ((s16)GwPlayerConf[playerNo].type != 0) {
        p = (M513Input *)fn_1_0(sizeof(M513Input));
        if (p != 0) {
            p = fn_1_69EC(p, playerNo, GwPlayerConf[playerNo].dif);
        }
        return p;
    }
    p = (M513Input *)fn_1_0(M513_PADINPUT_SIZE);
    if (p != 0) {
        p = fn_1_65E0(p, playerNo);
    }
    return p;
}

M513Player *fn_1_11900(M513Player *self, s32 playerNo, u8 type)
{
    Vec start[4];
    Vec rot;
    M513Model *model;
    M513Input *input;
    M513Leaf *leaf;
    HU3DMODELID id;
    f32 x;
    s32 i;

    fn_1_1768(&self->link);
    self->active = 1;
    self->charNo = GwPlayerConf[playerNo].charNo;
    self->motion = -1;
    self->playerNo = playerNo;
    self->state = 0;
    self->t = 0.0f;
    self->score = 0;
    self->cryT = 0.0f;
    self->done = 0;
    self->type = type;
#ifdef TARGET_PC
    for (i = 0; i < 16; i++) {
        fn_1_11F84(&self->eff[i]);
    }
#else
    __construct_array(self->eff, (void (*)(void *))fn_1_11F84, (void *)fn_1_BAF0, 4, 0x10);
#endif
    self->effIdx = 0;
    self->mot.cap = 0;
    self->mot.size = 0;
    self->mot.data = 0;
    model = (M513Model *)fn_1_0(sizeof(M513Model));
    if (model != 0) {
        model = fn_1_321C(model, GwPlayerConf[playerNo].charNo, 3);
    }
    self->model = model;
    input = fn_1_11844(playerNo);
    self->input = input;
    self->leaf = 0;
    M513_ASSERT(lbl_1_bss_A10 != 0, lbl_1_data_804, lbl_1_data_810, 0x2E);
    fn_1_1834(&self->link, lbl_1_bss_A10);
    self->unk14 = 60.000004f;
    self->mot.size = 0;
    fn_1_ECB8(&self->mot, 6);
    fn_1_12008(self, 0x8C0000);
    fn_1_12008(self, 0x8C0001);
    fn_1_12008(self, 0x8C0002);
    fn_1_12008(self, 0x8C0074);
    fn_1_12008(self, 0x8C0006);
    fn_1_12008(self, 0x8C0007);
    M513_ASSERT(self->mot.size == 6, lbl_1_data_818, lbl_1_data_83B, 0x43);
    CharMotionDataClose(self->charNo);
    id = self->model->id;
    Hu3DModelShadowSet(id);
    Hu3DModelShadowDispOn(id);
    Hu3DModelLayerSet(id, 3);
    Hu3DModelCameraSet(id, 1);
    CharEffectLayerSet(5);
    fn_1_1212C(self, 0, 0, 0, 8.0f);
    x = 100.0f * (2.0f * (f32)playerNo);
    self->pos.x = x - 300.0f;
    self->pos.y = 0.0f;
    self->pos.z = 250.0f;
    self->rot.x = 0.0f;
    self->rot.y = 180.0f;
    self->rot.z = 0.0f;
    for (i = 0; i < 4; i++) {
        start[i].x = lbl_1_rodata_498[i * 3];
        start[i].y = lbl_1_rodata_498[i * 3 + 1];
        start[i].z = lbl_1_rodata_498[i * 3 + 2];
    }
    rot.x = lbl_1_rodata_4C8[0];
    rot.y = lbl_1_rodata_4C8[1];
    rot.z = lbl_1_rodata_4C8[2];
    leaf = (M513Leaf *)fn_1_0(sizeof(M513Leaf));
    if (leaf != 0) {
        leaf = fn_1_7F04(leaf, playerNo, &start[playerNo], &rot);
    }
    if (self->leaf != leaf) {
        fn_1_8310(self->leaf, 1);
        self->leaf = leaf;
    }
    for (i = 0; i < 0x10; i++) {
        M513PowerEff *eff = (M513PowerEff *)fn_1_0(sizeof(M513PowerEff));

        if (eff != 0) {
            eff = fn_1_98A0(eff);
        }
        if (self->eff[i] != eff) {
            M513PowerEff *old = self->eff[i];

            if (old != 0) {
                ((M513TaskVtbl *)old->vtbl)->dtor((M513Task *)old, 1);
            }
            self->eff[i] = eff;
        }
    }
    M513_SET_HOOK(self, lbl_1_data_7F8)
    return self;
}

M513LeafPtr *fn_1_11EB0(M513LeafPtr *self, s16 flag)
{
    if (self != 0) {
        fn_1_8310(self->p, 1);
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M513InputPtr *fn_1_11F08(M513InputPtr *self, s16 flag)
{
    if (self != 0) {
        M513Input *in = self->p;

        if (in != 0) {
            in->vtbl->dtor(in, 1);
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_11F84(M513PowerEff **elem)
{
    M513PowerEff **sp8 = elem;

    fn_1_13138(elem, 0);
}

M513Player *fn_1_11FB0(M513Player *self, s16 flag)
{
    if (self != 0) {
        fn_1_1774(&self->link, 0);
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

// Append the motion `dataNum` of this character to the motion table.
void fn_1_12008(M513Player *self, u32 dataNum)
{
    u32 sp8 = dataNum;
    s16 mot = CharMotionCreate(self->charNo, dataNum);

    fn_1_E908(&self->mot, self->mot.data + self->mot.size, 1, &mot);
}

void fn_1_12080(M513Player *self)
{
    Vec p;

    if (M513_PTMF_TEST(&self->hook)) {
        M513_PTMF_CALL(&self->hook, self);
    }
    M513Model *m1 = self->model;
    fn_1_33F8(m1, &self->pos);
    M513Model *m2 = self->model;
    fn_1_3440(m2, &self->rot);
    p.x = self->pos.x;
    p.y = self->pos.y;
    p.z = self->pos.z;
    Vec *sp = &p;
    fn_1_14918(self->charNo, sp);
}

// Switch to motion `motion` (restart when `force`).
void fn_1_1212C(M513Player *self, s32 motion, u8 force, u8 loop, f32 speed)
{
    HU3DMODELID id = self->model->id;
    s16 mot = *fn_1_E890(&self->mot, motion);

    if ((self->motion != motion) || (force != 0)) {
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

void fn_1_12204(void)
{
}

void fn_1_12208(void)
{
}

// Reset to the idle motion.
void fn_1_1220C(M513Player *self)
{
    HU3DMODELID id = self->model->id;
    s16 mot = *fn_1_E890(&self->mot, 0);

    if (self->motion == 0) {
    } else {
        Hu3DModelAttrReset(id, 0x40000002);
        CharMotionShiftSet(self->charNo, mot, 0.0f, 8.0f, 0x40000001);
        self->motion = 0;
    }
    M513_SET_HOOK(self, lbl_1_data_848)
}

// Motion hook while waiting for the input.
void fn_1_122F8(M513Player *self)
{
    M513Input *input = self->input;
    f32 sx;
    f32 sy;
    f32 smag;
    HU3DMODELID id;

    fn_1_6578(input);
    sx = input->st.x;
    sy = input->st.y;
    smag = input->st.mag;
    id = self->model->id;
    if (lbl_1_bss_A44 != 1) {
        if ((input->st.btn & 0x100) != 0) {
            M513Model *mpa = self->model;
            HU3DMODELID ida = mpa->id;
            HU3DMODELID idb = ida;
            HU3DMODELID id2 = idb;
            s16 mot = *fn_1_E890(&self->mot, 3);
            Vec start;
            Vec target;
            Vec p;
            f32 noise;
            f32 frac;
            M513PowerEff *e1;
            M513PowerEff *e2;

            if (self->motion != 3) {
                Hu3DModelAttrReset(id2, 0x40000002);
                CharMotionShiftSet(self->charNo, mot, 0.0f, 8.0f, 0x40000001);
                self->motion = 3;
            }
            CharModelCryCreate(self->charNo, 30.0f, 0.8f);
            self->cryT = 0.0f;
            fn_1_85F4(self->leaf, 0.5f);
            frac = self->leaf->frac;
            target.x = self->pos.x;
            target.y = self->pos.y;
            target.z = self->pos.z;
            start.x = target.x;
            start.y = target.y;
            start.z = target.z;
            noise = 50.0f * (0.000030517578f * ((f32)(u16)frand() - 32768.0f));
            target.y = self->pos.y + (200.0f + (150.0f * frac) + noise);
            target.z += -500.0f;
            start.y += lbl_1_rodata_500[self->charNo];
            e1 = self->eff[self->effIdx];
            e2 = e1;
            fn_1_9FE4(e2, &start, &target, 0.0f);
            self->effIdx++;
            self->effIdx &= 0xF;
            Hu3DModelAttrReset(id, 0x40000002);
            p.x = self->pos.x;
            p.y = self->pos.y;
            p.z = self->pos.z;
            fn_1_145A8(0x6CC, &p);
        }
        if (self->leaf->flag != 0) {
            M513Score *score;

            self->t = 0.0f;
            M513Score *sc1;

            M513_ASSERT(lbl_1_bss_A20 != 0, lbl_1_data_860, lbl_1_data_870, 0x27);
            sc1 = lbl_1_bss_A20;
            score = lbl_1_bss_A20;
            self->score = fn_1_13E34(score, self->playerNo);
            self->done = 1;
            fn_1_13C78(score, self->playerNo);
            if (self->type == 1) {
                GwPlayer[self->playerNo].mgScore = self->score;
            }
            M513_SET_HOOK(self, lbl_1_data_854)
            if (self->type == 0) {
                fn_1_13C0C(score);
                lbl_1_bss_A44 = 1;
            }
            fn_1_13080(self);
            omVibrate(self->playerNo, 0x28, 0xC, 0);
        }
        if (self->motion == 3) {
            if (self->cryT > 1.5f) {
                M513Model *mpc = self->model;
                HU3DMODELID idc = mpc->id;
                HU3DMODELID idd = idc;
                HU3DMODELID id3 = idd;
                s16 mot = *fn_1_E890(&self->mot, 0);

                if (self->motion == 0) {
                } else {
                    Hu3DModelAttrReset(id3, 0x40000002);
                    CharMotionShiftSet(self->charNo, mot, 0.0f, 8.0f, 0x40000001);
                    self->motion = 0;
                }
            } else if (self->cryT > 0.5f) {
                Hu3DModelAttrSet(id, 0x40000002);
            }
            self->cryT = self->cryT + 0.016666666666666666;
        }
    }
}

// Motion hook after the hit: approach the leaf, then idle (variant for type 1).
void fn_1_128AC(M513Player *self)
{
    switch (self->state) {
        case 0: {
            HU3DMODELID id;
            s16 mot;

            M513Camera *cam;

            M513_ASSERT(lbl_1_bss_A18 != 0, lbl_1_data_860, lbl_1_data_870, 0x27);
            M513Camera *cam2;
            f32 ang;
            f32 ang2;

            cam = lbl_1_bss_A18;
            cam2 = cam;
            cam = cam2;
            ang = cam->angle;
            ang2 = ang;
            self->rot.y = self->rot.y + ((ang2 - self->rot.y) / 12.0f);
            M513Model *mp1 = self->model;
            id = mp1->id;
            mot = *fn_1_E890(&self->mot, 1);
            if (self->motion == 1) {
            } else {
                Hu3DModelAttrReset(id, 0x40000002);
                CharMotionShiftSet(self->charNo, mot, 0.0f, 8.0f, 0x40000001);
                self->motion = 1;
            }
            self->t = self->t + 0.016666666666666666;
            if (self->t < 1.0f) {
                break;
            }
            {
                M513Model *mp2 = self->model;
                HU3DMODELID id1 = mp2->id;
                HU3DMODELID id2 = id1;
                s16 mot2 = *fn_1_E890(&self->mot, 0);

                if (self->motion == 0) {
                } else {
                    Hu3DModelAttrReset(id2, 0x40000002);
                    CharMotionShiftSet(self->charNo, mot2, 0.0f, 8.0f, 0x40000001);
                    self->motion = 0;
                }
            }
            self->state++;
        }
        // fall through
        case 1:
            M513_SET_HOOK(self, lbl_1_data_87C)
            self->state++;
            break;
    }
}

void fn_1_12B20(M513Player *self)
{
    switch (self->state) {
        case 0: {
            HU3DMODELID id;
            s16 mot;

            M513Camera *cam;

            M513_ASSERT(lbl_1_bss_A18 != 0, lbl_1_data_860, lbl_1_data_870, 0x27);
            M513Camera *cam2;
            f32 ang;
            f32 ang2;

            cam = lbl_1_bss_A18;
            cam2 = cam;
            cam = cam2;
            ang = cam->angle;
            ang2 = ang;
            self->rot.y = self->rot.y + ((ang2 - self->rot.y) / 12.0f);
            M513Model *mp1 = self->model;
            id = mp1->id;
            mot = *fn_1_E890(&self->mot, 1);
            if (self->motion == 1) {
            } else {
                Hu3DModelAttrReset(id, 0x40000002);
                CharMotionShiftSet(self->charNo, mot, 0.0f, 8.0f, 0x40000001);
                self->motion = 1;
            }
            self->t = self->t + 0.016666666666666666;
            if (self->t < 1.0f) {
                break;
            }
            {
                M513Model *mp2 = self->model;
                HU3DMODELID id1 = mp2->id;
                HU3DMODELID id2 = id1;
                s16 mot2 = *fn_1_E890(&self->mot, 0);

                if (self->motion == 0) {
                } else {
                    Hu3DModelAttrReset(id2, 0x40000002);
                    CharMotionShiftSet(self->charNo, mot2, 0.0f, 8.0f, 0x40000001);
                    self->motion = 0;
                }
            }
            self->state++;
        }
        // fall through
        case 1:
            M513_SET_HOOK(self, lbl_1_data_888)
            self->state++;
            break;
    }
}

// Turn towards the camera angle; stop being active when close enough.
void fn_1_12D94(M513Player *self)
{
    f32 target;
    M513Camera *c1;
    M513Camera *c2;

    M513_ASSERT(lbl_1_bss_A18 != 0, lbl_1_data_860, lbl_1_data_870, 0x27);
    c1 = lbl_1_bss_A18;
    c2 = lbl_1_bss_A18;
    target = c1->angle;
    self->rot.y = self->rot.y + ((target - self->rot.y) / 60.0f);
    if ((self->rot.y - target) < 1.0f) {
        self->active = 0;
    }
}

s32 fn_1_12E88(M513Player *self)
{
    return self->motion;
}

// Win / lose pose.
void fn_1_12E90(M513Player *self, u8 win)
{
    if (win != 0) {
        HU3DMODELID id = self->model->id;
        s16 mot = *fn_1_E890(&self->mot, 4);

        if (self->motion == 4) {
        } else {
            Hu3DModelAttrReset(id, 0x40000002);
            CharMotionShiftSet(self->charNo, mot, 0.0f, 8.0f, 0);
            self->motion = 4;
        }
        fn_1_2A60(self->playerNo);
        return;
    } else {
        HU3DMODELID id = self->model->id;
        s16 mot = *fn_1_E890(&self->mot, 5);

        if (self->motion == 5) {
            return;
        }
        Hu3DModelAttrReset(id, 0x40000002);
        CharMotionShiftSet(self->charNo, mot, 0.0f, 8.0f, 0);
        self->motion = 5;
    }
}

void fn_1_12FDC(M513Player *self)
{
    if (self->type == 1) {
        GwPlayer[self->playerNo].mgScore = 0;
    }
    M513_SET_HOOK(self, lbl_1_data_894)
}

u8 fn_1_13050(M513Player *self)
{
    return (u8)((s16)GwPlayerConf[self->playerNo].type != 0);
}

// Tell the heiho of this player's slot to start its throw.
void fn_1_13080(M513Player *self)
{
    M513Node *it;

    M513_ASSERT(lbl_1_bss_A14 != 0, lbl_1_data_804, lbl_1_data_810, 0x2E);
    it = lbl_1_bss_A14->next;
    while (it != lbl_1_bss_A14) {
        if (self->playerNo == ((M513Heiho *)it)->idx) {
            fn_1_E790((M513Heiho *)it);
            return;
        }
        it = it->next;
    }
}

void fn_1_13138(M513PowerEff **p, M513PowerEff *v)
{
    *p = v;
}

void fn_1_13140(void)
{
    lbl_1_bss_A38 = 0x78;
    lbl_1_bss_A3C = 0xD2;
    lbl_1_bss_A40 = 0xD2;
}

}
