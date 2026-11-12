#include "REL/m537Dll.h"
#include <math.h>

extern "C" {

extern void *lbl_1_data_1FC[];

// Look-at camera: position, up vector and target.
struct M537CamLook {
    Vec m_pos;      /* 0x00 */
    Vec m_up;       /* 0x0C */
    Vec m_target;   /* 0x18 */
};

// Camera as position plus orientation quaternion.
struct M537CamQuat {
    Vec m_pos;          /* 0x00 */
    Quaternion m_rot;   /* 0x0C */
};

// Constant camera view followed by the pad word of the original table.
struct M537CamQuatConst {
    M537CamQuat m_view;
    f32 m_pad;
};

// The camera work object (singleton at lbl_1_bss_A10).
struct M537Camera {
    M537_NODE m_node;       /* 0x00 */
    void **m_vptr;          /* 0x08 */
    s32 m_flag;             /* 0x0C */
    M537CamLook m_look;     /* 0x10 */
    M537_PTMF m_hook;       /* 0x34 */
    s16 m_camMdl;           /* 0x40 */
    f64 m_duration;         /* 0x48 */
    s32 m_timer;            /* 0x50 */
    M537CamQuat m_from;     /* 0x54 */
    M537CamQuat m_to;       /* 0x70 */
    u8 m_debug;             /* 0x8C */
    u8 m_debugLock;         /* 0x8D */
    M537CamLook m_dbg;      /* 0x90 */
};

void fn_1_654(M537CamLook *cam, const M537CamQuat *src);
void fn_1_6F4(M537CamLook *cam, M537CamQuat *dst);
void fn_1_76C(M537CamLook *out, M537CamQuat *a, M537CamQuat *b, f32 t);
void fn_1_9A0(M537CamLook *cam, u16 cameraBit);
void fn_1_9E0(M537CamLook *cam, u16 cameraBit);
void fn_1_C08(Vec *camVec, s32 pad);
s32 fn_1_A0D4(void);
M537Dorrie *fn_1_96CC(M537Stage *stage, s32 index);
MtxPtr fn_1_6558(M537Dorrie *dorrie, s32 part, Mtx mtx);

M537Camera *fn_1_8B30(M537Camera *self, s16 freeIt);
void fn_1_8BFC(M537Camera *self, s32 mode);
void fn_1_8D0C(M537Camera *self);
void fn_1_8D54(M537Camera *self);
void fn_1_8DE8(M537Camera *self);
void fn_1_8E18(M537Camera *self);
void fn_1_8F4C(M537Camera *self);
void fn_1_90B0(M537Camera *self);
void fn_1_9114(M537Camera *self);

M537_PTMF lbl_1_data_350 = { 0, -1, (void *)fn_1_8DE8 };
M537_PTMF lbl_1_data_388 = { 0, -1, (void *)fn_1_8D54 };
M537_PTMF lbl_1_data_394 = { 0, -1, (void *)fn_1_8DE8 };
M537_PTMF lbl_1_data_3A0 = { 0, -1, (void *)fn_1_8F4C };
M537_PTMF lbl_1_data_3AC = { 0, -1, (void *)fn_1_8DE8 };
void *lbl_1_data_3B8[6] = { 0, 0, (void *)fn_1_8B30, (void *)fn_1_8BFC, (void *)fn_1_8D0C, 0 };

#ifdef __MWERKS__
#define M537_RODATA __declspec(section ".rodata")
#else
#define M537_RODATA
#endif

M537_RODATA const M537CamQuatConst lbl_1_rodata_238 = {
    { { 1.0529807f, 1812.263f, 900.0021f }, { -0.33956355f, -0.0016347608f, -0.00059535523f, 0.94058156f } }, 0.0f
};
M537_RODATA const Vec lbl_1_rodata_260 = { 0.0f, 96.0f, 420.0f };
M537_RODATA const Vec lbl_1_rodata_26C = { 0.0f, 96.04822f, 419.00116f };
M537_RODATA const f32 lbl_1_rodata_278[4] = { 0.0f, 0.998837f, -0.048218f, 0.0f };

M537Camera *fn_1_8964(M537Camera *self)
{
    M537Camera *ms;

    fn_1_1264(self);
    self->m_vptr = lbl_1_data_1FC;
    self->m_flag = 0;
    fn_1_1330(self, &lbl_1_bss_C);
    if (lbl_1_bss_A10 == 0) {
    } else {
        fn_1_1D4("ms_pointer == 0", "singleton.h", 0x35);
    }
    ms = (M537Camera *)((u8 *)self + 0x10);
    if (ms != 0) {
        ms = (M537Camera *)((u8 *)ms - 0x10);
    }
    lbl_1_bss_A10 = ms;
    self->m_vptr = lbl_1_data_3B8;
    self->m_hook = lbl_1_data_350;
    self->m_camMdl = -1;
    self->m_duration = 180.0;
    self->m_debug = 0;
    self->m_debugLock = 0;
    Hu3DCameraCreate(1);
    Hu3DBGColorSet(0, 0, 0);
    fn_1_654(&self->m_dbg, &lbl_1_rodata_238.m_view);
    return self;
}

M537Camera *fn_1_8AA4(M537Camera *self, s16 freeIt)
{
    if (self != 0) {
        if (lbl_1_bss_A10 != 0) {
        } else {
            fn_1_1D4("ms_pointer != 0", "singleton.h", 0x3D);
        }
        lbl_1_bss_A10 = 0;
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M537Camera *fn_1_8B30(M537Camera *self, s16 freeIt)
{
    if (self != 0) {
        self->m_vptr = lbl_1_data_3B8;
        if ((M537Camera *)((u8 *)self + 0x10) != 0) {
            if (lbl_1_bss_A10 != 0) {
            } else {
                fn_1_1D4("ms_pointer != 0", "singleton.h", 0x3D);
            }
            lbl_1_bss_A10 = 0;
        }
        if (self != 0) {
            self->m_vptr = lbl_1_data_1FC;
            if (self != 0) {
                fn_1_1270(self, 0);
            }
        }
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_8BFC(M537Camera *self, s32 mode)
{
    M537_PTMF tmp;

    switch (mode) {
    case 0:
        fn_1_90B0(self);
        tmp = lbl_1_data_388;
        self->m_hook = tmp;
        return;
    case 1:
        fn_1_9114(self);
        self->m_flag &= ~(1 << mode);
        return;
    case 2:
        self->m_flag &= ~(1 << mode);
        return;
    case 3:
        fn_1_8E18(self);
        self->m_flag &= ~(1 << mode);
        return;
    case 4:
        self->m_flag &= ~(1 << mode);
        return;
    }
}

void fn_1_8D0C(M537Camera *self)
{
    if (__ptmf_test(&self->m_hook) != 0) {
        __ptmf_scall(self);
    }
}

void fn_1_8D54(M537Camera *self)
{
    M537_PTMF tmp;

    if (self->m_camMdl != -1) {
        fn_1_9E0(&self->m_look, 1);
        if (Hu3DMotionEndCheck(self->m_camMdl) != 0) {
            self->m_flag &= ~1;
            tmp = lbl_1_data_394;
            self->m_hook = tmp;
        }
    }
}

void fn_1_8DE8(M537Camera *self)
{
    fn_1_9A0(&self->m_look, 1);
}

// Starts the camera blend towards the dorrie chosen by the player check.
void fn_1_8E18(M537Camera *self)
{
    Vec v2;
    Vec v1;
    Vec v0;
    Mtx mtx;
    M537_PTMF tmp;
    s32 idx;
    M537Stage *stage;
    M537Dorrie *dorrie;

    idx = fn_1_A0D4();
    if (idx >= 0) {
        fn_1_6F4(&self->m_look, &self->m_from);
        if (lbl_1_bss_A14 != 0) {
        } else {
            fn_1_1D4("ms_pointer != 0", "singleton.h", 0x27);
        }
        stage = lbl_1_bss_A14;
        dorrie = fn_1_96CC(stage, idx);
        fn_1_6558(dorrie, 0, mtx);
        PSMTXMultVec(mtx, &lbl_1_rodata_260, &v0);
        PSMTXMultVec(mtx, &lbl_1_rodata_26C, &v2);
        PSMTXMultVec(mtx, (Vec *)lbl_1_rodata_278, &v1);
        fn_1_6F4((M537CamLook *)&v0, &self->m_to);
        self->m_timer = 0;
        tmp = lbl_1_data_3A0;
        self->m_hook = tmp;
    }
}

void fn_1_8F4C(M537Camera *self)
{
    M537_PTMF tmp;
    f64 t;

    t = 1.0 - cos(3.141592653589793 * ((f64)self->m_timer / self->m_duration));
    fn_1_76C(&self->m_look, &self->m_from, &self->m_to, (f32)(t * t * 0.25));
    fn_1_9A0(&self->m_look, 1);
    self->m_timer++;
    if ((f32)(f64)self->m_timer > (f32)self->m_duration) {
        tmp = lbl_1_data_3AC;
        self->m_hook = tmp;
    }
}

void fn_1_90B0(M537Camera *self)
{
    s16 mot;

    mot = Hu3DMotionCreate(HuDataSelHeapReadNum(0x5F0000, 0x10000000, HEAP_MODEL));
    self->m_camMdl = Hu3DModelCameraCreate(mot, 1);
    Hu3DCameraMotionStart(self->m_camMdl, 1);
}

void fn_1_9114(M537Camera *self)
{
    Hu3DModelKill(self->m_camMdl);
    self->m_camMdl = -1;
}

// Debug free camera toggled from the pad.
void fn_1_914C(M537Camera *self)
{
    Vec *posP;
    Vec *tgtP;
    Vec *upP;
    u8 col;
    u16 down;

    down = HuPadBtnDown[0];
    if ((down & 0x800) != 0) {
        self->m_debug = (u8)(self->m_debug == 0);
    }
    if (self->m_debug != 0) {
        if ((down & 0x40) != 0) {
            self->m_debugLock = (u8)(self->m_debugLock == 0);
        }
        if (self->m_debugLock == 0) {
            fn_1_C08(&self->m_dbg.m_pos, 0);
        }
        if ((HuPadBtnDown[0] & 0x20) != 0) {
            self->m_dbg = self->m_look;
        }
        fn_1_9A0(&self->m_dbg, 1);
        posP = &self->m_dbg.m_pos;
        tgtP = &self->m_dbg.m_target;
        upP = &self->m_dbg.m_up;
        if (self->m_debugLock != 0) {
            col = 0xFF;
        } else {
            col = 0;
        }
    }
}

}
