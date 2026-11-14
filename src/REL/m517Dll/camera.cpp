// m517Dll camera: the camera singleton. A PTMF state hook runs the intro
// motion and then eases the camera towards one of two fixed views.
#define _MATH_H
#include "REL/m517Dll.h"

#ifdef __MWERKS__
#define M517_RODATA __declspec(section ".rodata")
#else
#define M517_RODATA
#endif

extern "C" {

void fn_1_DE68(M517Camera *self);
void fn_1_DE98(M517Camera *self);
void fn_1_DF28(M517Camera *self);
void fn_1_E11C(M517Camera *self, s32 bit);
void fn_1_E290(M517Camera *self);
M517Camera *fn_1_E2D8(M517Camera *self, s16 flag);

extern M517Camera *lbl_1_bss_A20;

// {position, look-at target} of the two views.
M517_RODATA const Vec lbl_1_rodata_248[2] = { { 0.0f, 500.0f, 900.0f }, { 0.0f, 200.0f, 0.0f } };
M517_RODATA const Vec lbl_1_rodata_260[2] = { { 0.0f, 600.0f, 500.0f }, { 0.0f, 400.0f, 0.0f } };

M517Ptmf lbl_1_data_480 = { 0, -1, (void (*)(void *))fn_1_DE68 };
char lbl_1_data_48C[] = "ms_pointer == 0";
char lbl_1_data_49C[] = "singleton.h";
char lbl_1_data_4A8[] = "ms_pointer != 0";
M517Ptmf lbl_1_data_4B8 = { 0, -1, (void (*)(void *))fn_1_DE68 };
M517Ptmf lbl_1_data_4C4 = { 0, -1, (void (*)(void *))fn_1_DE98 };
M517Ptmf lbl_1_data_4D0 = { 0, -1, (void (*)(void *))fn_1_DF28 };
void *lbl_1_data_4DC[5] = { 0, 0, (void *)fn_1_E2D8, (void *)fn_1_E11C, (void *)fn_1_E290 };

M517Camera *fn_1_DC4C(M517Camera *self)
{
    fn_1_2DA8(&self->task.link);
    self->task.vtbl = &lbl_1_data_68.vtbl;
    self->task.flags = 0;
    fn_1_2E74(&self->task.link, &lbl_1_bss_24);
    M517_ASSERT(lbl_1_bss_A20 == 0, lbl_1_data_48C, lbl_1_data_49C, 0x35);
    M517_SINGLETON_SET(lbl_1_bss_A20, self, M517Camera);
    self->task.vtbl = (M517TaskVtbl *)lbl_1_data_4DC;
    {
        M517Ptmf tmp = lbl_1_data_480;

        self->state = tmp;
    }
    self->farView = 0;
    Hu3DCameraCreate(1);
    Hu3DCameraPerspectiveSet(1, 45.0f, 100.0f, 50000.0f, 1.2f);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    fn_1_2444(&self->cam);
    return self;
}

M517Camera *fn_1_DDDC(M517Camera *self, s16 flag)
{
    if (self != 0) {
        M517_ASSERT(lbl_1_bss_A20 != 0, lbl_1_data_4A8, lbl_1_data_49C, 0x3D);
        lbl_1_bss_A20 = 0;
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

// Idle state: keep the camera object's pose applied.
void fn_1_DE68(M517Camera *self)
{
    fn_1_2824(&self->cam, 1);
}

// Intro motion state: follow the motion until it ends.
void fn_1_DE98(M517Camera *self)
{
    fn_1_2864(&self->cam, 1);
    if (Hu3DMotionEndCheck(self->motionId) != 0) {
        Hu3DModelKill(self->motionId);
        self->task.flags &= ~1;
        M517Ptmf tmp = lbl_1_data_4B8;
        self->state = tmp;
    }
}

// Follow state: ease towards the near or the far view.
void fn_1_DF28(M517Camera *self)
{
    Vec destPos;
    Vec destTarget;
    f32 k;
    Vec *pos;
    Vec *up;
    Vec *target;

    if (self->farView == 0) {
        destPos = lbl_1_rodata_248[0];
        destTarget = lbl_1_rodata_248[1];
    } else {
        destPos = lbl_1_rodata_260[0];
        destTarget = lbl_1_rodata_260[1];
    }
    k = 30.0f;
    pos = &self->cam.pos;
    up = &self->cam.up;
    target = &self->cam.target;
    pos->x += (destPos.x - pos->x) / k;
    pos->y += (destPos.y - pos->y) / k;
    pos->z += (destPos.z - pos->z) / k;
    target->x += (destTarget.x - target->x) / k;
    target->y += (destTarget.y - target->y) / k;
    target->z += (destTarget.z - target->z) / k;
    Hu3DCameraPosSetV(1, pos, up, target);
}

// Virtual onFlag(): bit 0 starts the intro motion, bit 3 switches to follow.
void fn_1_E11C(M517Camera *self, s32 bit)
{
    switch (bit) {
        case 0:
            self->motionId = Hu3DModelCameraCreate(Hu3DMotionCreateData(M517_ANM_camera), 1);
            Hu3DCameraMotionStart(self->motionId, 1);
            {
                M517Ptmf tmp = lbl_1_data_4C4;

                self->state = tmp;
            }
            break;
        case 1:
            Hu3DModelKill(self->motionId);
            self->task.flags &= ~(1 << bit);
            break;
        case 2:
            self->task.flags &= ~(1 << bit);
            break;
        case 3:
            {
                M517Ptmf tmp = lbl_1_data_4D0;

                self->state = tmp;
            }
            self->task.flags &= ~(1 << bit);
            break;
        case 4:
            self->task.flags &= ~(1 << bit);
            break;
    }
}

// Virtual update(): run the state hook.
void fn_1_E290(M517Camera *self)
{
    if (M517_PTMF_TEST(&self->state)) {
        M517_PTMF_CALL(&self->state, self);
    }
}

M517Camera *fn_1_E2D8(M517Camera *self, s16 flag)
{
    if (self != 0) {
        self->task.vtbl = (M517TaskVtbl *)lbl_1_data_4DC;
        if (M517_SUBOBJ_NONNULL(self)) {
            M517_ASSERT(lbl_1_bss_A20 != 0, lbl_1_data_4A8, lbl_1_data_49C, 0x3D);
            lbl_1_bss_A20 = 0;
        }
        if (self != 0) {
            self->task.vtbl = &lbl_1_data_68.vtbl;
            if (self != 0) {
                fn_1_2DB4(&self->task.link, 0);
            }
        }
        if (flag > 0) {
            fn_1_1F4C(self);
        }
    }
    return self;
}

}
