#include "REL/m539Dll.h"

#ifdef __MWERKS__
#define M539_RODATA __declspec(section ".rodata")
#else
#define M539_RODATA
#endif

/*
 * m539 camera: a singleton that splits the screen into four viewports
 * (1, 2, 4, 8) and animates the split. A310 programs one viewport, A4D0 the
 * four of them around a centre point (320 * scale, 240 * scale). The object
 * also owns two sub-objects fed from the rodata tables and a pointer-to-member
 * hook used for the per-state tick.
 */

extern "C" {

typedef struct M539CamSub_s {
    u8 data[0x24];
} M539CamSub;

typedef struct M539Camera_s {
    void *node0;
    void *node4;
    void **vtable;
    u32 flags;
    M539CamSub sub0;
    M539CamSub sub1;
    s32 frame;
    M539Ptmf hook;
} M539Camera;

typedef void (*M539Fn)(void);

extern u16 HuPadBtnDown[4];
extern void *lbl_1_bss_A14;
extern s32 lbl_1_bss_C;
extern void *lbl_1_data_1FC;

void Hu3DBGColorSet(u8 r, u8 g, u8 b);
void Hu3DCameraCreate(int cameraBit);
void Hu3DCameraPerspectiveSet(int cameraBit, float fov, float near, float far, float aspect);
void Hu3DCameraViewportSet(int cameraBit, float vpX, float vpY, float vpW, float vpH, float vpNearZ, float vpFarZ);
void Hu3DCameraScissorSet(int cameraBit, unsigned int x, unsigned int y, unsigned int w, unsigned int h);
void Hu3DFogSet(float start, float end, u8 r, u8 g, u8 b);

void fn_1_1264(void *self);
void fn_1_1270(void *self, s32 arg);
void fn_1_1330(void *self, void *arg);
void fn_1_1D4(const char *msg, const char *file, s32 line);
void fn_1_654(void *sub, const void *table);
void fn_1_9A0(void *sub, s32 arg);

void fn_1_B3C0(void);
void fn_1_B3C4(M539Camera *self);
M539Camera *fn_1_BDF0(M539Camera *self, s16 arg1);
void fn_1_BB14(M539Camera *self, s32 state);
void fn_1_BC24(M539Camera *self);

char lbl_1_data_340[] = "ms_pointer == 0";
char lbl_1_data_350[] = "singleton.h";
char lbl_1_data_35C[] = "ms_pointer != 0";
M539Ptmf lbl_1_data_36C = M539_PTMF_INIT(fn_1_B3C4);
M539Ptmf lbl_1_data_378 = M539_PTMF_INIT(fn_1_B3C0);
f32 lbl_1_data_384[5] = { 1024.0f, 18939.0f, 37.0f, 40.0f, 57.0f };
void *lbl_1_data_398[6] = { 0, 0, (void *)fn_1_BDF0, (void *)fn_1_BB14, (void *)fn_1_BC24, 0 };

s32 lbl_1_bss_A30;
u8 lbl_1_bss_A34;

// Two 7-float tables (position/colour style data) handed to fn_1_654.
M539_RODATA const u32 lbl_1_rodata_178[14] = {
    0xC3662B61, 0x4386BF06, 0x44898000, 0x80000000, 0x00000000, 0x80000000, 0x3F800000,
    0xC3662B61, 0x434B0D16, 0x44898000, 0x80000000, 0x00000000, 0x80000000, 0x3F800000,
};

// Program camera `idx` for the rectangle (x0, y0)-(x1, y1); a degenerate
// rectangle gets a far-away dummy projection instead.
void fn_1_A310(s32 idx, f32 x0, f32 y0, f32 x1, f32 y1)
{
    f32 w = x1 - x0;
    f32 h = y1 - y0;
    if (w < 0.0f || h < 0.0f) {
        Hu3DCameraPerspectiveSet(1 << idx, -1.0f, 100.0f, 35000.0f, 1.2f);
        return;
    }
    Hu3DCameraPerspectiveSet(1 << idx, 35.0f, 100.0f, 10000.0f, 1.2f);
    Hu3DCameraViewportSet(1 << idx, x0, y0, w, h, 0.0f, 1.0f);
    Hu3DCameraScissorSet(1 << idx, (u32)x0, (u32)y0, (u32)w, (u32)h);
}

void fn_1_A4D0(f32 scale)
{
    f32 cx = 320.0f * scale;
    f32 cy = 240.0f * scale;

    fn_1_A310(0, 0.0f, 0.0f, cx - 0.5f, cy - 0.5f);
    fn_1_A310(1, 0.5f + cx, 0.0f, 640.0f, cy - 0.5f);
    fn_1_A310(2, 0.0f, 0.5f + cy, cx - 0.5f, 480.0f);
    fn_1_A310(3, 0.5f + cx, 0.5f + cy, 639.0f, 480.0f);
}

M539Camera *fn_1_AB68(M539Camera *self)
{
    void *single;

    fn_1_1264(self);
    self->vtable = &lbl_1_data_1FC;
    self->flags = 0;
    fn_1_1330(self, &lbl_1_bss_C);
    if (lbl_1_bss_A14 != 0) {
        fn_1_1D4(lbl_1_data_340, lbl_1_data_350, 0x35);
    }
    single = &self->sub0;
    if (single != 0) {
        single = self;
    }
    lbl_1_bss_A14 = single;
    self->vtable = lbl_1_data_398;
    self->hook = M539_PTMF_NULL;
    Hu3DCameraCreate(0xF);
    fn_1_A4D0(2.0f);
    Hu3DBGColorSet(0, 0, 0);
    fn_1_654(&self->sub0, lbl_1_rodata_178);
    fn_1_654(&self->sub1, lbl_1_rodata_178 + 7);
    Hu3DBGColorSet(0, 0, 0);
    return self;
}

M539Camera *fn_1_B2DC(M539Camera *self, s16 arg1)
{
    if (self != 0) {
        if (lbl_1_bss_A14 == 0) {
            fn_1_1D4(lbl_1_data_35C, lbl_1_data_350, 0x3D);
        }
        lbl_1_bss_A14 = 0;
        if (arg1 > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

M539Camera *fn_1_B368(M539Camera *self, s16 arg1)
{
    if (self != 0) {
        fn_1_1270(self, 0);
        if (arg1 > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_B3C0(void)
{
}

// Zoom-in phase: after 300 frames the split slides in over 60 frames.
void fn_1_B3C4(M539Camera *self)
{
    s32 start = 300;

    if (self->frame > start) {
        f32 t = (f32)(self->frame - start) / 60.0f;
        if (t > 1.0f) {
            t = 1.0f;
            self->flags = self->flags & ~1;
        }
        fn_1_A4D0(t + 2.0f * (1.0f - t));
    }
}

void fn_1_BB14(M539Camera *self, s32 state)
{
    self->frame = 0;
    switch (state) {
    case 0: {
        M539Ptmf t = lbl_1_data_36C;
        self->hook = t;
        return;
    }
    case 1: {
        M539Ptmf t = lbl_1_data_378;
        self->hook = t;
        self->flags = self->flags & ~(1 << state);
        return;
    }
    case 2:
        self->flags = self->flags & ~(1 << state);
        return;
    case 3:
        self->flags = self->flags & ~(1 << state);
        return;
    case 4:
        self->flags = self->flags & ~(1 << state);
        return;
    }
}

void fn_1_BC24(M539Camera *self)
{
    if (M539PtmfTest(&self->hook) != 0) {
        M539PtmfCall(self, &self->hook);
    }
    self->frame = self->frame + 1;
    fn_1_9A0(&self->sub0, 3);
    fn_1_9A0(&self->sub1, 0xC);
    if ((s8)lbl_1_bss_A34 == 0) {
        lbl_1_bss_A30 = 0;
        lbl_1_bss_A34 = 1;
    }
    lbl_1_bss_A30 -= (u8)((HuPadBtnDown[0] & 8) != 0);
    lbl_1_bss_A30 += (u8)((HuPadBtnDown[0] & 4) != 0);
    if (lbl_1_bss_A30 < 0) {
        lbl_1_bss_A30 = 4;
    } else if (lbl_1_bss_A30 > 4) {
        lbl_1_bss_A30 = 0;
    }
    Hu3DFogSet(lbl_1_data_384[0], lbl_1_data_384[1], (u8)(s32)lbl_1_data_384[2], (u8)(s32)lbl_1_data_384[3],
               (u8)(s32)lbl_1_data_384[4]);
}

M539Camera *fn_1_BDF0(M539Camera *self, s16 arg1)
{
    if (self != 0) {
        self->vtable = lbl_1_data_398;
        if (&self->sub0 != 0) {
            if (lbl_1_bss_A14 == 0) {
                fn_1_1D4(lbl_1_data_35C, lbl_1_data_350, 0x3D);
            }
            lbl_1_bss_A14 = 0;
        }
        if (self != 0) {
            self->vtable = &lbl_1_data_1FC;
            if (self != 0) {
                fn_1_1270(self, 0);
            }
        }
        if (arg1 > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

}
