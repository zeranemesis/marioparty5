// m532Dll "camera": singleton camera task. State functions are pointer-to-member
// hooks: the default one mirrors the live pose to Hu3D, the demo one lets a
// camera motion drive the pose until it ends.
#define _MATH_H
extern "C" {
#include "dolphin.h"
#include "game/hu3d.h"
}
#include "REL/m532Dll/m532.h"
#include "REL/m532Dll/camera.h"

extern "C" {

// Task base vtable (ball.cpp) and the singleton pointer (game_master.cpp).
extern M532TaskVtbl lbl_1_data_B4;
extern void *lbl_1_bss_A2C;
// man_map.cpp: start / stop the camera motion model.
void fn_1_9B9C(M532Camera *self);
void fn_1_9C00(M532Camera *self);

void fn_1_9B6C(M532Camera *self);
void fn_1_9AD8(M532Camera *self);

M532Ptmf lbl_1_data_398 = M532_PTMF_INIT(fn_1_9B6C);
char lbl_1_data_3A4[] = "ms_pointer == 0";
char lbl_1_data_3B4[] = "singleton.h";
char lbl_1_data_3C0[] = "ms_pointer != 0";
M532Ptmf lbl_1_data_3D0 = M532_PTMF_INIT(fn_1_9AD8);
M532Ptmf lbl_1_data_3DC = M532_PTMF_INIT(fn_1_9B6C);

M532Camera *fn_1_98B0(M532Camera *self, s16 flag);
void *lbl_1_data_3E8[6] = { 0, 0, (void *)fn_1_98B0, (void *)fn_1_997C, (void *)fn_1_9A84, 0 };

// Initial camera pose: position (0, 104.54, 1265.4), rotation quaternion.
const u32 lbl_1_rodata_218[8] = { 0, 0x42D114E9, 0x449E2CDA, 0x3DE130EC, 0, 0, 0x3F7E729F, 0 };

M532Camera *fn_1_96EC(M532Camera *self)
{
    fn_1_32B0(&self->base.link);
    self->base.vtbl = &lbl_1_data_B4;
    self->base.flags = 0;
    fn_1_337C(&self->base.link, &lbl_1_bss_24);
    if (lbl_1_bss_A2C != 0) {
        fn_1_1D1C(lbl_1_data_3A4, lbl_1_data_3B4, 0x35);
    }
    lbl_1_bss_A2C = self;
    self->base.vtbl = (M532TaskVtbl *)lbl_1_data_3E8;
    self->state = lbl_1_data_398;
    self->camMdl = -1;
    self->frame = 0;
    self->freeCam = 0;
    self->hold = 0;
    Hu3DCameraCreate(1);
    Hu3DBGColorSet(0, 0, 0);
    fn_1_219C((M532Cam *)&self->pos2, (M532Pose *)lbl_1_rodata_218);
    return self;
}

M532Camera *fn_1_9824(M532Camera *self, s16 flag)
{
    if (self != 0) {
        if (lbl_1_bss_A2C == 0) {
            fn_1_1D1C(lbl_1_data_3C0, lbl_1_data_3B4, 0x3D);
        }
        lbl_1_bss_A2C = 0;
        if (flag > 0) {
            fn_1_1C10(self);
        }
    }
    return self;
}

M532Camera *fn_1_98B0(M532Camera *self, s16 flag)
{
    if (self != 0) {
        self->base.vtbl = (M532TaskVtbl *)lbl_1_data_3E8;
        if (self != 0) {
            if (lbl_1_bss_A2C == 0) {
                fn_1_1D1C(lbl_1_data_3C0, lbl_1_data_3B4, 0x3D);
            }
            lbl_1_bss_A2C = 0;
        }
        if (self != 0) {
            self->base.vtbl = &lbl_1_data_B4;
            if (self != 0) {
                fn_1_32BC(&self->base.link, 0);
            }
        }
        if (flag > 0) {
            fn_1_1C10(self);
        }
    }
    return self;
}

void fn_1_997C(M532Camera *self, s32 bit)
{
    switch (bit) {
    case 0:
        fn_1_9B9C(self);
        self->state = lbl_1_data_3D0;
        break;
    case 1:
        fn_1_9C00(self);
        self->base.flags = self->base.flags & ~(1 << bit);
        break;
    case 2:
        self->base.flags = self->base.flags & ~(1 << bit);
        break;
    case 3:
        self->base.flags = self->base.flags & ~(1 << bit);
        break;
    case 4:
        self->base.flags = self->base.flags & ~(1 << bit);
        break;
    }
}

void fn_1_9A84(M532Camera *self)
{
    if (M532PtmfTest(&self->state) != 0) {
        M532PtmfCall(self, &self->state);
    }
    self->frame = self->frame + 1;
}

// State: follow the camera motion; when it ends clear the flag and go back to the default state.
void fn_1_9AD8(M532Camera *self)
{
    if (self->camMdl != -1) {
        fn_1_2528((M532Cam *)&self->pos, 1);
        if (Hu3DMotionEndCheck(self->camMdl) != 0) {
            self->base.flags = self->base.flags & ~1;
            self->state = lbl_1_data_3DC;
        }
    }
}

// State: mirror the pose to the Hu3D camera.
void fn_1_9B6C(M532Camera *self)
{
    fn_1_24E8((M532Cam *)&self->pos, 1);
}

}
