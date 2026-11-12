// m513Dll kuribo: the goomba enemies. Each is a node of an intrusive set with
// a model, a motion table (std::vector<s16>) and a PTMF state hook.
#define _MATH_H
#include "REL/m513Dll.h"
extern "C" {
#include "game/frand.h"
}

#ifdef __MWERKS__
#define M513_RODATA __declspec(section ".rodata")
#else
#define M513_RODATA
#endif



extern "C" {

extern M513Node *lbl_1_bss_A0C;
extern M513Ptmf lbl_1_data_5E0;
extern M513Ptmf lbl_1_data_5EC;

s16 *fn_1_E890(M513Vec16 *v, u32 idx);
void fn_1_E908(M513Vec16 *v, s16 *pos, u32 n, s16 *val);
void fn_1_ECB8(M513Vec16 *v, u32 cap);

void fn_1_F698(M513Kuribo *self);
void fn_1_F3EC(M513Kuribo *self);
void fn_1_F9F0(M513Kuribo *self);
void fn_1_F2AC(M513Kuribo *self, s32 dataNum);
void fn_1_F334(M513Kuribo *self, s32 motion, u8 loop, f32 speed);

char lbl_1_data_5F8[] = "sm_ptr != 0";
char lbl_1_data_604[] = "a_set.h";
char lbl_1_data_60C[] = "m_mot_id.size() == NUMBEROF_MOTION";
char lbl_1_data_62F[] = "kuribo.cpp";
M513Ptmf lbl_1_data_63C = { 0, -1, (void (*)(void *))fn_1_F698 };
M513Ptmf lbl_1_data_648 = { 0, -1, (void (*)(void *))fn_1_F9F0 };
// vector<s16> error strings (not referenced by this unit's code)
char lbl_1_data_654[] = "vector::at index out of range\0vector::insert length error\n\0Memory allocation failure\0vector::reserve length error\n";

// Singleton pointers of this module (set by the units that own the classes).
M513Node *lbl_1_bss_A10;
M513Node *lbl_1_bss_A14;
struct M513Camera_s *lbl_1_bss_A18;
struct M513Stage_s *lbl_1_bss_A1C;
struct M513Score_s *lbl_1_bss_A20;
s32 lbl_1_bss_A24;

M513_RODATA const s32 lbl_1_rodata_3EC[3] = { 0, 2, 3 };

// Inlined motion switch (looping, blend speed 16).
#define M513_KURIBO_MOTION(self, m)                                                                \
    {                                                                                              \
        HU3DMODELID mid_ = (self)->model.id;                                                       \
        s16 mot_ = *fn_1_E890(&(self)->mot, (m));                                                  \
        if ((self)->motion != (m)) {                                                               \
            Hu3DModelAttrReset(mid_, 0x40000002);                                                  \
            Hu3DMotionShiftSet(mid_, mot_, 0.0f, 16.0f, 0x40000001);                               \
            (self)->motion = (m);                                                                  \
        }                                                                                          \
    }

M513Kuribo *fn_1_EDE0(M513Kuribo *self, Vec *pos, Vec *rot)
{
    HU3DMODELID id;

    fn_1_1768(&self->link);
    fn_1_3198(&self->model, 0x47000F);
    self->motion = -1;
    self->target = -1;
    self->mot.cap = 0;
    self->mot.size = 0;
    self->mot.data = 0;
    self->state = 0;
    self->t = 0.0f;
    self->mode = -1;
    M513_ASSERT(lbl_1_bss_A0C != 0, lbl_1_data_5F8, lbl_1_data_604, 0x2E);
    fn_1_1834(&self->link, lbl_1_bss_A0C);
    self->unk18 = 60.000004f;
    self->pos.x = pos->x;
    self->pos.y = pos->y;
    self->pos.z = pos->z;
    self->rot.x = rot->x;
    self->rot.y = rot->y;
    self->rot.z = rot->z;
    id = self->model.id;
    Hu3DModelCameraSet(id, 1);
    Hu3DModelLayerSet(id, 3);
    self->mot.size = 0;
    fn_1_ECB8(&self->mot, 4);
    fn_1_F2AC(self, 0x470010);
    fn_1_F2AC(self, 0x470011);
    fn_1_F2AC(self, 0x470012);
    fn_1_F2AC(self, 0x470013);
    M513_ASSERT(self->mot.size == 4, lbl_1_data_60C, lbl_1_data_62F, 0x34);
    fn_1_F334(self, 0, 0, 16.0f);
    Hu3DMotionTimeSet(id, 4.0f * (f32)(frand() & 0x1F));
    fn_1_33F8(&self->model, &self->pos);
    fn_1_3440(&self->model, &self->rot);
    if (rot->y == 135.0f) {
        self->dir = 0;
    } else {
        self->dir = 1;
    }
    self->wait = 0.5f * (0.000015258789f * (f32)(u16)frand());
    if ((frand() & 7) == 0) {
        f32 y;

        if (self->dir == 0) {
            y = 90.0f;
        } else {
            y = 0.0f;
        }
        self->rot.y = y;
        fn_1_F334(self, 0, 0, 16.0f);
        self->state = 3;
        self->hook = lbl_1_data_5E0;
    } else {
        self->hook = lbl_1_data_5EC;
    }
    return self;
}

M513Kuribo *fn_1_F18C(M513Kuribo *self, s16 flag)
{
    if (self != 0) {
        fn_1_1774(&self->link, 0);
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M513Kuribo *fn_1_F1E4(M513Kuribo *self, s16 flag)
{
    if (self != 0) {
        if ((&self->mot != 0) && (&self->mot != 0) && (&self->mot != 0) && (*(s16 **)&self->mot.data != 0)) {
            s16 **q = &self->mot.data;
            s16 *d = *q;
            fn_1_C8(d);
        }
        fn_1_35F8(&self->model, -1);
        if (self != 0) {
            fn_1_1774(&self->link, 0);
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_F2AC(M513Kuribo *self, s32 dataNum)
{
    HU3DMODELID id = self->model.id;
    s16 mot = Hu3DJointMotion(id, HuDataSelHeapReadNum(dataNum, 0x10000000, HEAP_MODEL));

    fn_1_E908(&self->mot, self->mot.data + self->mot.size, 1, &mot);
}

void fn_1_F334(M513Kuribo *self, s32 motion, u8 loop, f32 speed)
{
    HU3DMODELID id = self->model.id;
    s16 mot = *fn_1_E890(&self->mot, motion);

    if (self->motion != motion) {
        u32 attr;

        Hu3DModelAttrReset(id, 0x40000002);
        if (loop == 0) {
            attr = 0x40000001;
        } else {
            attr = 0;
        }
        Hu3DMotionShiftSet(id, mot, 0.0f, speed, attr);
        self->motion = motion;
    }
}

// State hook: idle, then wander left / right.
void fn_1_F3EC(M513Kuribo *self)
{
    switch (self->state) {
        case 0:
            self->t = self->t + 0.016666666666666666;
            if (self->t < self->wait) {
                break;
            }
            self->t = 0.0f;
            M513_KURIBO_MOTION(self, 2);
            Hu3DMotionShiftStartEndSet(self->model.id, 60.0f, 159.0f);
            self->state++;
            // fall through
        case 1: {
            f32 spd;
            f32 dx;

            self->t = self->t + 0.016666666666666666;
            if (self->dir == 0) {
                spd = 0.22222222f;
            } else {
                spd = 0.25f;
            }
            if (self->dir == 0) {
                dx = -spd;
            } else {
                dx = spd;
            }
            self->rot.y = self->rot.y + dx;
            if (self->t < 3.0f) {
                break;
            }
            M513_KURIBO_MOTION(self, 0);
            self->state++;
        }
        // fall through
        case 2:
            self->hook = lbl_1_data_63C;
            self->state++;
            break;
    }
}

// State hook: pick a random motion and hold it for a while.
void fn_1_F698(M513Kuribo *self)
{
    switch (self->state) {
        case 0: {
            s32 m = self->target;

            self->t = self->t + 0.016666666666666666;
            if (self->t < self->wait) {
                break;
            }
            self->t = 0.0f;
            M513_KURIBO_MOTION(self, m);
            if (self->target == 3) {
                Hu3DMotionShiftStartEndSet(self->model.id, 40.0f, 159.0f);
            }
            self->t = 0.0f;
            self->wait = 1.0f + (3.0f * (0.000015258789f * (f32)(u16)frand()));
            self->state++;
        }
        // fall through
        case 1:
            self->t = self->t + 0.016666666666666666;
            if (self->t < self->wait) {
                break;
            }
            {
                s32 r = frand() % 3;
                s32 m = lbl_1_rodata_3EC[r];

                M513_KURIBO_MOTION(self, m);
                switch (lbl_1_rodata_3EC[r]) {
                    case 2:
                        Hu3DMotionShiftStartEndSet(self->model.id, 60.0f, 159.0f);
                        break;
                    case 3:
                        Hu3DMotionShiftStartEndSet(self->model.id, 40.0f, 159.0f);
                        break;
                }
                self->t = 0.0f;
                self->wait = 1.0f + (3.0f * (0.000015258789f * (f32)(u16)frand()));
            }
            break;
    }
}

// State hook: wait, then play motion 1 once.
void fn_1_F9F0(M513Kuribo *self)
{
    switch (self->state) {
        case 0:
            self->t = self->t + 0.016666666666666666;
            if (self->t < self->wait) {
                break;
            }
            M513_KURIBO_MOTION(self, 1);
            self->state++;
            // fall through
        case 1:
            break;
    }
}

void fn_1_FAE4(M513Kuribo *self)
{
    self->state = 0;
    self->t = 0.0f;
    self->wait = 0.5f * (0.000015258789f * (f32)(u16)frand());
    self->hook = lbl_1_data_648;
}

void fn_1_FBA8(M513Kuribo *self)
{
    if (M513_PTMF_TEST(&self->hook)) {
        M513_PTMF_CALL(&self->hook, self);
    }
    fn_1_33F8(&self->model, &self->pos);
    fn_1_3440(&self->model, &self->rot);
}

// Set the behaviour mode (0: wander, 1: stand and look).
void fn_1_FC08(M513Kuribo *self, s32 mode)
{
    if (self->mode != mode) {
        switch (mode) {
            case 0:
                self->target = 0;
                break;
            case 1:
                self->target = 3;
                break;
        }
        self->t = 0.0f;
        self->wait = 0.000015258789f * (f32)(u16)frand();
        self->state = 0;
        self->mode = mode;
    }
}

}
