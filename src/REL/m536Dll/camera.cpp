#include "REL/m536Dll.h"
#include <math.h>

extern "C" {


extern void *lbl_1_bss_A14;
extern void *lbl_1_data_1FC[];

// The camera work object: list node, vtable, flags, then pos/up/target at 0x10.
struct M536Camera {
    M536_NODE m_node;   /* 0x00 */
    void **m_vptr;      /* 0x08 */
    s32 m_flag;         /* 0x0C */
    Vec m_pos;          /* 0x10 */
    Vec m_up;           /* 0x1C */
    Vec m_target;       /* 0x28 */
    s32 m_timer;        /* 0x34 */
    M536_PTMF m_hook;   /* 0x38 */
    s16 m_camMdl;       /* 0x44 */
    s32 m_seId;         /* 0x48 */
};

void fn_1_6954(M536Camera *self);
void fn_1_6A70(M536Camera *self);
void fn_1_6C74(M536Camera *self);

M536Camera *fn_1_72F0(M536Camera *self, s16 freeIt);
void fn_1_6E38(M536Camera *self, s32 mode);
void fn_1_6F80(M536Camera *self);

M536_PTMF lbl_1_data_268 = { 0, -1, (void *)fn_1_6954 };
M536_PTMF lbl_1_data_2A0 = { 0, -1, (void *)fn_1_6954 };
M536_PTMF lbl_1_data_2AC = { 0, -1, (void *)fn_1_6954 };
M536_PTMF lbl_1_data_2B8 = { 0, -1, (void *)fn_1_6C74 };
M536_PTMF lbl_1_data_2C4 = { 0, -1, (void *)fn_1_6A70 };
void *lbl_1_data_2D0[6] = { 0, 0, (void *)fn_1_72F0, (void *)fn_1_6E38, (void *)fn_1_6F80, 0 };

M536Camera *fn_1_6628(M536Camera *self)
{
    M536Camera *ms;
    M536_PTMF tmp;

    fn_1_1264(self);
    self->m_vptr = lbl_1_data_1FC;
    self->m_flag = 0;
    fn_1_1330(self, &lbl_1_bss_C);
    if (lbl_1_bss_A14 != NULL) {
        fn_1_1D4("ms_pointer == 0", "singleton.h", 0x35);
    }
    ms = (M536Camera *)((u8 *)self + 0x10);
    if (ms != NULL) {
        ms = (M536Camera *)((u8 *)ms - 0x10);
    }
    lbl_1_bss_A14 = ms;
    self->m_vptr = lbl_1_data_2D0;
    tmp = lbl_1_data_268;
    self->m_hook = tmp;
    Hu3DCameraCreate(1);
    Hu3DCameraPerspectiveSet(1, 45.0f, 100.0f, 30000.0f, 1.2f);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCamera->fov = 45.0f;
    self->m_seId = HuAudFXPlayVol(0x7D7, 0x7F);
    {
        Vec pos = { 0.0f, 1700.0f, 480.0f };
        Vec up = { 0.0f, 1.0f, 0.0f };
        Vec target = { 0.0f, 0.0f, 20.0f };
        self->m_pos = pos;
        self->m_up = up;
        self->m_target = target;
    }
    self->m_timer = 0;
    fn_1_6954(self);
    return self;
}

M536Camera *fn_1_6870(M536Camera *self, s16 freeIt)
{
    if (self != NULL) {
        if (lbl_1_bss_A14 == NULL) {
            fn_1_1D4("ms_pointer != 0", "singleton.h", 0x3D);
        }
        lbl_1_bss_A14 = NULL;
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M536Camera *fn_1_68FC(M536Camera *self, s16 freeIt)
{
    if (self != NULL) {
        fn_1_1270(self, 0);
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_6954(M536Camera *self)
{
    Vec *pos = &self->m_pos;
    Vec *up = &self->m_up;
    Vec *target = &self->m_target;

    Hu3DCameraPosSetV(1, pos, up, target);
}

void fn_1_69B4(M536Camera *self)
{
    M536_PTMF tmp;

    fn_1_C08(&self->m_pos, 0);
    if ((HuPadBtnDown[0] & 0x800) != 0) {
        tmp = lbl_1_data_2A0;
        self->m_hook = tmp;
    }
    Hu3DCameraPosSetV(1, &self->m_pos, &self->m_up, &self->m_target);
}

void fn_1_6A70(M536Camera *self)
{
    M536_PTMF tmp;
    Vec pos;
    Vec up;
    Vec target;

    if (Hu3DMotionEndCheck(self->m_camMdl) != 0) {
        tmp = lbl_1_data_2AC;
        self->m_hook = tmp;
        self->m_flag &= ~1;
        Hu3DCameraPosGet(1, &pos, &up, &target);
        self->m_pos.x = Hu3DCamera->pos.x;
        self->m_pos.y = Hu3DCamera->pos.y;
        self->m_pos.z = Hu3DCamera->pos.z;
        self->m_up.x = Hu3DCamera->up.x;
        self->m_up.y = Hu3DCamera->up.y;
        self->m_up.z = Hu3DCamera->up.z;
        self->m_target.x = Hu3DCamera->target.x;
        self->m_target.y = Hu3DCamera->target.y;
        self->m_target.z = Hu3DCamera->target.z;
    }
    if ((f32)self->m_timer > 120.0f && (f32)self->m_timer < 180.0f) {
        HuAudFXVolSet(self->m_seId, (s16)(127.0f - 39.0f * (((f32)self->m_timer - 120.0f) / 60.0f)));
    }
    self->m_timer++;
}

void fn_1_6C74(M536Camera *self)
{
    Vec *pos = &self->m_pos;
    Vec *target = &self->m_target;

    pos->x += 0.05f * (0.0f - pos->x);
    pos->y += 0.05f * (600.0f - pos->y);
    pos->z += 0.05f * (400.0f - pos->z);
    target->x += 0.05f * (0.0f - target->x);
    target->y += 0.05f * (0.0f - target->y);
    target->z += 0.05f * (-100.0f - target->z);
    Hu3DCameraPosSetV(1, pos, &self->m_up, target);
}

void fn_1_6DF4(M536Camera *self)
{
    M536_PTMF tmp;

    tmp = lbl_1_data_2B8;
    self->m_hook = tmp;
}

void fn_1_6E38(M536Camera *self, s32 mode)
{
    M536_PTMF tmp;

    switch (mode) {
    case 0:
        self->m_timer = 0;
        tmp = lbl_1_data_2C4;
        self->m_hook = tmp;
        self->m_camMdl = Hu3DModelCameraCreate(Hu3DMotionCreate(HuDataSelHeapReadNum(0x5E0007, 0x10000000, HEAP_MODEL)), 1);
        Hu3DCameraMotionStart(self->m_camMdl, 1);
        return;
    case 1:
        Hu3DModelKill(self->m_camMdl);
        self->m_flag &= ~(1 << mode);
        return;
    case 2:
        self->m_flag &= ~(1 << mode);
        return;
    case 3:
        self->m_flag &= ~(1 << mode);
        return;
    case 4:
        self->m_flag &= ~(1 << mode);
        return;
    }
}

void fn_1_6F80(M536Camera *self)
{
    if (__ptmf_test(&self->m_hook) != 0) {
        __ptmf_scall(self);
        Hu3DFogClear();
    }
}

s16 fn_1_6FCC(Vec *pos)
{
    Vec screen;
    s16 pan;
    s16 ret;

    Hu3D3Dto2D((HuVecF *)pos, 1, (HuVecF *)&screen);
    pan = (s16)(48.0f + screen.x * 0.055555556f);
    if (pan < 0) {
        ret = 0;
    } else if (pan > 0x7F) {
        ret = 0x7F;
    } else {
        ret = pan;
    }
    return ret;
}

s16 fn_1_706C(Vec *pos)
{
    Vec camPos;
    Vec camUp;
    Vec camTarget;
    Vec diff;
    f64 dist;
    s16 vol;
    s16 ret;

    Hu3DCameraPosGet(1, &camPos, &camUp, &camTarget);
    PSVECSubtract(pos, &camPos, &diff);
    dist = sqrt(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
    vol = (s16)(127.0f - 32.0f * (f32)((dist / 100.0 - 17.0) / 6.0));
    if (vol < 0x60) {
        ret = 0x60;
    } else if (vol > 0x7F) {
        ret = 0x7F;
    } else {
        ret = vol;
    }
    return ret;
}

M536Camera *fn_1_72F0(M536Camera *self, s16 freeIt)
{
    if (self != NULL) {
        self->m_vptr = lbl_1_data_2D0;
        if ((M536Camera *)((u8 *)self + 0x10) != NULL) {
            if (lbl_1_bss_A14 == NULL) {
                fn_1_1D4("ms_pointer != 0", "singleton.h", 0x3D);
            }
            lbl_1_bss_A14 = NULL;
        }
        if (self != NULL) {
            self->m_vptr = lbl_1_data_1FC;
            if (self != NULL) {
                fn_1_1270(self, 0);
            }
        }
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

}
