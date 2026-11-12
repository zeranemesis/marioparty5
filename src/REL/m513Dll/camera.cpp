// m513Dll camera: the camera singleton. A PTMF state hook runs the intro
// motion and then follows its destination.
#define _MATH_H
#include "REL/m513Dll.h"
extern "C" {
#include "game/frand.h"
}

extern "C" {
double atan2(double y, double x);

void fn_1_FF9C(M513Camera *self);
void fn_1_FFCC(M513Camera *self);
void fn_1_1008C(M513Camera *self);
void fn_1_101E0(M513Camera *self, s32 bit);
void fn_1_10358(M513Camera *self);
M513Camera *fn_1_103E4(M513Camera *self, s16 flag);

M513Ptmf lbl_1_data_6EC = { 0, -1, (void (*)(void *))fn_1_FF9C };
char lbl_1_data_6F8[] = "ms_pointer == 0";
char lbl_1_data_708[] = "singleton.h";
char lbl_1_data_714[] = "ms_pointer != 0";
M513Ptmf lbl_1_data_724 = { 0, -1, (void (*)(void *))fn_1_FF9C };
M513Ptmf lbl_1_data_730 = { 0, -1, (void (*)(void *))fn_1_FFCC };
M513Ptmf lbl_1_data_73C = { 0, -1, (void (*)(void *))fn_1_1008C };
void *lbl_1_data_748[6] = { 0, 0, (void *)fn_1_103E4, (void *)fn_1_101E0, (void *)fn_1_10358, 0 };

f32 lbl_1_data_6C8[9] = { 0.0f, 250.0f, -500.0f, 0.0f, 20.0f, 0.0f, 0.0f, 0.0f, 0.0f };

M513Camera *fn_1_FD60(M513Camera *self)
{
    M513Ptmf tmp;

    fn_1_1768(&self->link);
    self->vtbl = (void **)&lbl_1_data_218;
    self->flags = 0;
    fn_1_1834(&self->link, &lbl_1_bss_C);
    M513_ASSERT(lbl_1_bss_A18 == 0, lbl_1_data_6F8, lbl_1_data_708, 0x35);
    M513_SINGLETON_SET(lbl_1_bss_A18, self, M513Camera);
    self->vtbl = lbl_1_data_748;
    tmp = lbl_1_data_6EC;
    self->pmf = tmp;
    self->angle = 0.0f;
    Hu3DCameraCreate(1);
    Hu3DCameraPerspectiveSet(1, 35.0f, 100.0f, 6000.0f, 1.2f);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(1, 0, 0, 0x280, 0x1E0);
    fn_1_5C0(&self->cam);
    return self;
}

M513Camera *fn_1_FF10(M513Camera *self, s16 flag)
{
    if (self != 0) {
        M513_ASSERT(lbl_1_bss_A18 != 0, lbl_1_data_714, lbl_1_data_708, 0x3D);
        lbl_1_bss_A18 = 0;
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_FF9C(M513Camera *self)
{
    fn_1_9A0(&self->cam, 1);
}

// Intro motion state: follow the motion until it ends.
void fn_1_FFCC(M513Camera *self)
{
    M513Ptmf tmp;

    fn_1_9E0(&self->cam, 1);
    if (Hu3DMotionEndCheck(self->motionId) != 0) {
        self->destPos = self->cam.pos;
        self->destTarget = self->cam.target;
        Hu3DModelKill(self->motionId);
        self->flags &= ~1;
        tmp = lbl_1_data_724;
        self->pmf = tmp;
    }
}

// Follow state: ease towards the destination.
void fn_1_1008C(M513Camera *self)
{
    Vec destPos;
    Vec destTarget;

    destPos = self->destPos;
    destTarget = self->destTarget;
    self->cam.pos.x += (destPos.x - self->cam.pos.x) / 24.0f;
    self->cam.pos.y += (destPos.y - self->cam.pos.y) / 24.0f;
    self->cam.pos.z += (destPos.z - self->cam.pos.z) / 24.0f;
    self->cam.target.x += (destTarget.x - self->cam.target.x) / 24.0f;
    self->cam.target.y += (destTarget.y - self->cam.target.y) / 24.0f;
    self->cam.target.z += (destTarget.z - self->cam.target.z) / 24.0f;
    Hu3DCameraPosSetV(1, &self->cam.pos, &self->cam.up, &self->cam.target);
}

// Virtual update(): bit 0 starts the intro motion, bit 3 switches to follow.
void fn_1_101E0(M513Camera *self, s32 bit)
{
    M513Ptmf tmp;

    switch (bit) {
        case 0:
            self->motionId = Hu3DModelCameraCreate(Hu3DMotionCreateData(M513_ANM_camera), 1);
            Hu3DCameraMotionStart(self->motionId, 1);
            tmp = lbl_1_data_730;
            self->pmf = tmp;
            break;
        case 1:
            Hu3DModelKill(self->motionId);
            self->flags &= ~(1 << bit);
            break;
        case 2:
            self->flags &= ~(1 << bit);
            break;
        case 3:
            tmp = lbl_1_data_73C;
            self->pmf = tmp;
            self->flags &= ~(1 << bit);
            break;
        case 4:
            self->flags &= ~(1 << bit);
            break;
    }
}

void fn_1_10358(M513Camera *self)
{
    Vec diff;

    if (M513_PTMF_TEST(&self->pmf)) {
        M513_PTMF_CALL(&self->pmf, self);
    }
    PSVECSubtract(&self->cam.pos, &self->cam.target, &diff);
    self->angle = 180.0 * (atan2(diff.x, diff.z) / 3.141592653589793);
}

M513Camera *fn_1_103E4(M513Camera *self, s16 flag)
{
    if (self != 0) {
        self->vtbl = lbl_1_data_748;
        if (M513_SUBOBJ_NONNULL(self)) {
            M513_ASSERT(lbl_1_bss_A18 != 0, lbl_1_data_714, lbl_1_data_708, 0x3D);
            lbl_1_bss_A18 = 0;
        }
        if (self != 0) {
            self->vtbl = (void **)&lbl_1_data_218;
            if (self != 0) {
                fn_1_1774(&self->link, 0);
            }
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

}
