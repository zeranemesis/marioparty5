#include "dolphin/types.h"
#include <dolphin/gx.h>

#define M_PI 3.14159265358979323846

extern "C" {
typedef struct {
    f32 x;
    f32 y;
    f32 z;
} HuVecF;

double cos(double x);
double sin(double x);
double tan(double x);

struct M579StageModel {
    s16 field0;
    s16 field2;
};

class M579Stage {
public:
    u64 unk0;
    void **vtable;
    s32 field_C;
    M579StageModel model;
};

void fn_1_1264(void *self);
void fn_1_1270(void *self, s32 arg2);
void fn_1_1330(void *self, void *arg2);
void fn_1_1D4(const char *msg, const char *file, s32 line);
void fn_1_2C94(void *p, s32 v);
void fn_1_2E08(void *p, s32 v);
void fn_1_304C(void *p, s32 v);
void fn_1_30F4(void *p, s32 v);
void fn_1_3244(void *p, const char *name);
void *fn_1_C8(void *self);

void fn_1_B574(void);
void fn_1_B578(void *self, s32 bit);
void fn_1_B5F0(void *self, void *mtx);
void fn_1_B620();
void *fn_1_B98C(M579Stage *self, s16 flag);

extern void *lbl_1_bss_A30;
extern s32 lbl_1_bss_C;
extern void *lbl_1_data_1FC[];

void Hu3DModelObjDrawInit(void *obj, s32 arg1);
s32 Hu3DModelLightInfoSet(s16 modelId, s16 staticF);
void Hu3DShadowCreate(f32 fov, f32 near, f32 far);
void Hu3DShadowSizeSet(u16 size);
void Hu3DShadowTPLvlSet(f32 tpLvl);
void Hu3DShadowPosSet(HuVecF *camPos, HuVecF *camUp, HuVecF *camTarget);

f32 lbl_1_data_360[3] = { 0.0f, 0.0f, 0.0f };
char lbl_1_data_36C[16] = { 'm','s','_','p','o','i','n','t','e','r',' ','=','=',' ','0' };
char lbl_1_data_37C[12] = "singleton.h";
char lbl_1_data_388[11] = "feeld_born";
char lbl_1_data_393[17] = "ms_pointer != 0";
void *lbl_1_data_3A4[5] = { 0, 0, fn_1_B98C, fn_1_B578, fn_1_B574 };

const f32 lbl_1_rodata_388[3] = { 0.0f, 0.0f, 1.0f };
const f32 lbl_1_rodata_394[3] = { 0.0f, 0.0f, 4.296875f };

s8 lbl_1_bss_A88[8];

void *fn_1_B2C8(M579Stage *self)
{
    f64 scale;

    fn_1_1264(self);
    self->vtable = lbl_1_data_1FC;
    self->field_C = 0;
    fn_1_1330(self, &lbl_1_bss_C);
if (lbl_1_bss_A30 != 0) {
        fn_1_1D4(lbl_1_data_36C, lbl_1_data_37C, 0x35);
    }
    {
        u8 *base = (u8 *) &self->model;
        if (base != (u8 *) 0) {
            base -= 0x10;
        }
        lbl_1_bss_A30 = base;
    }
    self->vtable = lbl_1_data_3A4;
    fn_1_2C94(&self->model.field0, 0x85);
    fn_1_2C94(&self->model.field2, 0x86);
    Hu3DModelLightInfoSet(self->model.field0, 1);
    fn_1_3244(&self->model.field0, lbl_1_data_388);
    fn_1_304C(&self->model.field0, 2);
    fn_1_2E08(&self->model.field0, 0x4001);
    fn_1_2E08(&self->model.field0, 0x4040);
    fn_1_304C(&self->model.field2, 0);
    fn_1_2E08(&self->model.field2, 0x4001);

    scale = 0.0 / tan((f64) 0.39269908169872414);
    if (lbl_1_bss_A88[0] == 0) {
        lbl_1_data_360[1] = (f32) scale;
        lbl_1_bss_A88[0] = 1;
    }
    Hu3DShadowCreate(45.0f, (f32) (scale - 400.0),
                     (f32) (50.0 + scale));
    Hu3DShadowSizeSet(0xf0);
    Hu3DShadowTPLvlSet(0.4f);
    Hu3DShadowPosSet((HuVecF *) lbl_1_data_360, (HuVecF *) lbl_1_rodata_388,
                     (HuVecF *) lbl_1_rodata_394);
    return self;
}

void *fn_1_B4E8(void *self, s16 flag)
{
    if (self != 0) {
        if (lbl_1_bss_A30 == 0) {
            fn_1_1D4(lbl_1_data_393, lbl_1_data_37C, 0x3d);
        }
        lbl_1_bss_A30 = (void *) 0;
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_B574(void)
{
}

void fn_1_B578(void *self, s32 bit)
{
    *(u32 *) ((u8 *) self + 0xc) &= ~(u32) (1u << bit);
    if (bit >= 5 || bit < 0) {
    }
}

void fn_1_B5A0(void *self, void *arg1)
{
    M579Stage *p;
    M579Stage *q;
    Hu3DModelObjDrawInit(self, (s32) arg1);
    q = (M579Stage *) lbl_1_bss_A30;
    p = q;
    fn_1_B5F0(p, (void *) arg1);
}

void fn_1_B5F0(void *self, void *mtx)
{
    GXLoadPosMtxImm((float (*)[4]) mtx, 0);
    fn_1_B620();
}

void fn_1_B620(void)
{
    f32 stagePosX[80];
    f32 stagePosZ[80];
s32 i;
    s32 idx;
    f32 ang;
    f64 rad;

    idx = 0;
    i = 0;
    while (i < 33) {
        ang = 360.0f * ((i & 0x1f) * 0.03125f) - 180.0f + 90.0f;
        if (ang > 180.0f) {
            ang -= 360.0f;
        }
        rad = 3.141592653589793 * ang / 180.0;
        stagePosX[idx] = 100.0 * (8.5 * cos(rad));
        stagePosZ[idx] = 100.0 * (8.5 * sin(ang));
        idx++;
        stagePosX[idx] = 100.0 * (4.5 * cos(ang));
        stagePosZ[idx] = 100.0 * (4.5 * sin(ang));
        idx++;
        i++;
    }

    GXSetNumTevStages(1);
    GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE,
                  GX_AF_NONE);
    GXSetNumChans(1);
    GXSetNumTexGens(0);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetCullMode(GX_CULL_NONE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, (GXCompCnt) 1, GX_RGBA8, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, idx);

    i = 0;
    while (i < idx) {
        GXPosition3f32(stagePosX[i], 0.0f, stagePosZ[i]);
        GXColor4u8(0x40, 0x40, 0x40, 0xff);
        i++;
    }
}

void *fn_1_B98C(M579Stage *self, s16 flag)
{
    if (self != 0) {
        self->vtable = lbl_1_data_3A4;
        fn_1_30F4(&self->model.field2, -1);
        fn_1_30F4(&self->model.field0, -1);
        if (&self->model.field0 != 0) {
if (lbl_1_bss_A30 == 0) {
                fn_1_1D4(lbl_1_data_393, lbl_1_data_37C, 0x3d);
            }
            lbl_1_bss_A30 = (void *) 0;
        }
        if (self != 0) {
            self->vtable = lbl_1_data_1FC;
            if (self != 0) {
                fn_1_1270(self, 0);
            }
        }
        if (flag > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}
}
