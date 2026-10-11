#include <math.h>

#include <dolphin.h>
#include <dolphin/mtx.h>

#include "game/hu3d.h"
#include "game/sprite.h"

extern "C" void HuSprTexLoad(ANIMDATA *anim, s16 bmpNo, s16 texMapId, GXTexWrapMode wrapS, GXTexWrapMode wrapT, GXTexFilter filter);

/* These are plain C in the original: m511.o defines them unmangled
   (m511.cpp is NonMatching, so the module links the original object).
   Left as C++ they mangle and the REL link fails with
   "Failed to find symbol fn_1_1D88__FPv in any module". */
extern "C" void *__dl__FPv(void *self);
extern "C" void *__nw__FUl(u32 size);
void __construct_array(void *arr, void (*ctor)(void *), void *(*dtor)(void *, s16), u32 size, u32 count);
void __destroy_arr(void *arr, void *(*dtor)(void *, s16), u32 size, u32 count);
extern "C" void fn_1_1D88(void *self);
extern "C" void fn_1_1D94(void *self, s32 arg1);
extern "C" void fn_1_1E54(void *self, void **arg1);
extern "C" void fn_1_372C(void *self, s32 arg1);
extern "C" void fn_1_3B8C(void *self, s32 arg1);
void *fn_1_8F38(void *arg0, Point3d *arg1, s32 arg2);
void *fn_1_9128(void *arg0, s16 arg1);
void fn_1_9700(void *arg0, Point3d *arg1, Point3d *arg2);
extern "C" void fn_1_CF8(char *cond, char *file, s32 line);

struct Mgr511Star;

void *fn_1_D7B4(Mgr511Star **arg0, s16 arg1);
void fn_1_D80C(void *arg0);
void *fn_1_D838(Mgr511Star **arg0, s16 arg1);
void fn_1_D890(void *arg0);
Mgr511Star *fn_1_DF70(Mgr511Star *arg0, s32 arg_sp0);
void *fn_1_E060(Mgr511Star *arg0, s16 arg1);
void fn_1_E0B4(Mgr511Star *arg0);
void fn_1_E220(Mgr511Star *arg0);
void fn_1_E268(Mgr511Star *arg0, Mtx mtx);
void fn_1_E8AC(void *arg0, s32 arg1);
void fn_1_E8B4(void *arg0, s32 arg1);

/* One twinkling star cluster: 0x28C bytes. */
struct Mgr511Star {
    s16 modelId;
    u16 pad2;
    s32 anim;
    f32 posX;
    f32 posY;
    f32 posZ;
    f32 scale;
    u8 alpha[0x30];
    f32 vert[0x30][3];
    f32 timer;
};

struct Mgr511StarMaster {
    void *unk0;
    void *unk4;
    void **vtable;
    s32 unkC;
    s32 flag[5];
    s32 angle[5];
    s32 timer[5];
    s16 modelId[6];
    s16 modelId2[5];
    Mgr511Star *star[5];
    s16 link[5];
    s32 tick;
    f32 spin;
    void *glow[5];
};

struct Mgr511StarRoot {
    u8 pad20[0x20];
    s32 unk20;
};

extern "C" {

extern void *lbl_1_bss_2C;

extern Mgr511StarMaster *lbl_1_bss_A2C;
extern Mgr511StarRoot *lbl_1_bss_A38;

extern void *lbl_1_data_40[6];
extern void *lbl_1_data_4A4[6];

static const struct {
    f32 x;
    f32 y;
    f32 z;
} lbl_1_rodata_388[6] = {
    {-2500.0f, 280.0f, -2000.0f},
    {2700.0f, 280.0f, -3200.0f},
    {-2000.0f, 280.0f, 3000.0f},
    {2180.0f, 280.0f, 1600.0f},
    {-3500.0f, 280.0f, 0.0f},
    {0.0f, 280.0f, -5500.0f},
};

// NONMATCHING: the two cluster construction loops are not reproduced.
Mgr511StarMaster *fn_1_D318(Mgr511StarMaster *arg0, s32 arg_sp0)
{
    Mgr511Star *cluster;
    void *glow;
    f32 sp20;
    f32 sp24;
    f32 sp28;
    s16 temp_r30;
    s32 i;
    s32 j;

    fn_1_1D88(arg0);
    arg0->vtable = &lbl_1_data_40[2];
    arg0->unkC = 0;
    fn_1_1E54(arg0, (void **)lbl_1_bss_2C);
    if (lbl_1_bss_A2C == NULL) {
        fn_1_CF8("ms_pointer == 0", "singleton.h", 0x35);
    }
    lbl_1_bss_A2C = arg0;
    arg0->vtable = lbl_1_data_4A4;
    fn_1_372C(&arg0->modelId[6], DATANUM(DATA_m511, 0x16));
    fn_1_372C(&arg0->modelId2[0], DATANUM(DATA_m511, 0x17));
    fn_1_372C(&arg0->modelId2[1], DATANUM(DATA_m511, 0x18));
    fn_1_372C(&arg0->modelId2[2], DATANUM(DATA_m511, 0x19));
    fn_1_372C(&arg0->modelId2[3], DATANUM(DATA_m511, 0x1A));
    fn_1_372C(&arg0->modelId2[4], DATANUM(DATA_m511, 0x1B));
    __construct_array(arg0->star, fn_1_D890, (void *(*)(void *, s16))fn_1_D838, 4, 5);
    arg0->tick = 0;
    arg0->spin = 0.0f;
    __construct_array(arg0->glow, fn_1_D80C, (void *(*)(void *, s16))fn_1_D7B4, 4, 5);
    arg0->link[0] = (s16)arg0->modelId[0];
    arg0->link[1] = (s16)Hu3DModelLink(arg0->modelId[0]);
    arg0->link[2] = (s16)Hu3DModelLink(arg0->modelId[0]);
    arg0->link[3] = (s16)Hu3DModelLink(arg0->modelId[0]);
    arg0->link[4] = (s16)Hu3DModelLink(arg0->modelId[0]);
    for (i = 0; i < 5; i++) {
        cluster = (Mgr511Star *)__nw__FUl(0x28C);
        if (cluster != NULL) {
            cluster = (Mgr511Star *)fn_1_DF70(cluster, 0);
        }
        if ((u32)arg0->star[i] != (u32)cluster) {
            fn_1_E060(arg0->star[i], 1);
            arg0->star[i] = cluster;
        }
        sp20 = lbl_1_rodata_388[i].x;
        sp24 = lbl_1_rodata_388[i].y;
        sp28 = lbl_1_rodata_388[i].z;
        sp24 = sp24 - 300.0f;
        cluster->posX = sp20;
        cluster->posY = sp24;
        cluster->posZ = sp28;
    }
    for (i = 0; i < 5; i++) {
        temp_r30 = arg0->link[i];
        Hu3DModelLayerSet(temp_r30, 5);
        Hu3DModelPosSetV(temp_r30, (Point3d *)&lbl_1_rodata_388[i]);
        Hu3DModelAttrReset(temp_r30, 1);
        Hu3DModelAttrSet(temp_r30, 0x4000);
        Hu3DModelCameraSet(temp_r30, 0xF);
    }
    arg0->link[0] = arg0->modelId2[0];
    arg0->link[1] = arg0->modelId2[1];
    arg0->link[2] = arg0->modelId2[2];
    arg0->link[3] = arg0->modelId2[3];
    arg0->link[4] = arg0->modelId2[4];
    for (i = 0; i < 5; i++) {
        temp_r30 = arg0->link[i];
        Hu3DModelLayerSet(temp_r30, 5);
        Hu3DModelPosSetV(temp_r30, (Point3d *)&lbl_1_rodata_388[i]);
        Hu3DModelAttrReset(temp_r30, 1);
        Hu3DModelAttrSet(temp_r30, 0x4000);
        Hu3DModelCameraSet(temp_r30, 0xF);
        glow = (void *)__nw__FUl(0x28);
        if (cluster != NULL) {
            glow = fn_1_8F38(glow, (Point3d *)&lbl_1_rodata_388[i], i * 0xC);
        }
        if ((u32)arg0->glow[i] != (u32)glow) {
            fn_1_9128(arg0->glow[i], 1);
            arg0->glow[i] = glow;
        }
    }
    for (j = 0; j < 5; j++) {
        arg0->flag[j] = 0;
        arg0->angle[j] = 0;
        arg0->timer[j] = 0;
    }
    return arg0;
}

void *fn_1_D7B4(Mgr511Star **arg0, s16 arg1)
{
    if (arg0 != NULL) {
        fn_1_9128(*arg0, 1);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

void fn_1_D80C(void *arg0)
{
    fn_1_E8AC(arg0, 0);
}

void *fn_1_D838(Mgr511Star **arg0, s16 arg1)
{
    if (arg0 != NULL) {
        fn_1_E060(*arg0, 1);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

void fn_1_D890(void *arg0)
{
    fn_1_E8B4(arg0, 0);
}

// NONMATCHING: the vtable pointer differs.
void *fn_1_D8BC(void *arg0, s16 arg1)
{
    if (arg0 != NULL) {
        if (lbl_1_bss_A2C == NULL) {
            fn_1_CF8("ms_pointer != 0", "singleton.h", 0x3D);
        }
        lbl_1_bss_A2C = (Mgr511StarMaster *)NULL;
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

// NONMATCHING: the array destruction order differs.
void *fn_1_D948(void *arg0, s16 arg1)
{
    Mgr511StarMaster *self = (Mgr511StarMaster *)arg0;
    s32 i;

    if (arg0 != NULL) {
        self->vtable = lbl_1_data_4A4;
        for (i = 1; i < 5; i++) {
            Hu3DModelKill(self->link[i]);
        }
        __destroy_arr(self->glow, (void *(*)(void *, s16))fn_1_D7B4, 4, 5);
        __destroy_arr(self->star, (void *(*)(void *, s16))fn_1_D838, 4, 5);
        fn_1_3B8C(&self->modelId2[4], -1);
        fn_1_3B8C(&self->modelId2[3], -1);
        fn_1_3B8C(&self->modelId2[2], -1);
        fn_1_3B8C(&self->modelId2[1], -1);
        fn_1_3B8C(&self->modelId2[0], -1);
        fn_1_3B8C(&self->modelId[0], -1);
        if (lbl_1_bss_A2C == NULL) {
            fn_1_CF8("ms_pointer != 0", "singleton.h", 0x3D);
        }
        lbl_1_bss_A2C = (Mgr511StarMaster *)NULL;
        self->vtable = &lbl_1_data_40[2];
        fn_1_1D94(arg0, 0);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

void fn_1_DAB8(s32 arg0, Point3d *arg1)
{
    arg1->x = lbl_1_rodata_388[arg0].x;
    arg1->y = lbl_1_rodata_388[arg0].y;
    arg1->z = lbl_1_rodata_388[arg0].z;
}

// NONMATCHING: the per-star update is not reproduced.
void fn_1_DB04(Mgr511StarMaster *arg0)
{
    Mgr511StarMaster *self = arg0;
    Point3d sp14;
    f32 sp8;
    f32 spC;
    f32 sp10;
    f32 var_f30;
    f32 var_f31;
    s16 temp_r29;
    s32 i;

    for (i = 0; i < 5; i++) {
        sp14.x = lbl_1_rodata_388[i].x;
        sp14.y = lbl_1_rodata_388[i].y;
        sp14.z = lbl_1_rodata_388[i].z;
        var_f31 = 0.0f;
        var_f30 = 1.0f;
        if (self->timer[i] != 0) {
            self->timer[i] = self->timer[i] - 1;
            var_f30 = 1.0f + ((f32)(self->timer[i] * 2) / 18.0f);
        }
        if (self->flag[i] != 0) {
            self->angle[i] = self->angle[i] + self->flag[i];
            if ((f32)self->angle[i] > 120.0f) {
                self->angle[i] = (s32)((f32)self->angle[i] - 120.0f);
            }
            var_f31 = (360.0f * (f32)self->angle[i]) / 120.0f;
            if (var_f31 > 180.0f) {
                var_f31 -= 360.0f;
            }
            sp14.y += 50.0f * (f32)sin((3.141592653589793 * (f64)var_f31) / 180.0);
        }
        temp_r29 = self->link[i];
        Hu3DModelPosSetV(temp_r29, &sp14);
        Hu3DModelRotSet(temp_r29, 0.0f, var_f31, 0.0f);
        Hu3DModelScaleSet(temp_r29, var_f30, var_f30, var_f30);
        temp_r29 = self->link[i];
        Hu3DModelPosSetV(temp_r29, &sp14);
        Hu3DModelRotSet(temp_r29, 0.0f, self->spin, 0.0f);
        fn_1_E220(self->star[i]);
        sp8 = 0.0f;
        spC = self->spin;
        sp10 = 0.0f;
        fn_1_9700(self->glow[i], &sp14, (Point3d *)&sp8);
    }
    self->tick = self->tick + 1;
    self->spin = (f32)(self->spin + 2.0f);
    if (self->spin > 180.0f) {
        self->spin = (f32)(self->spin - 360.0f);
    }
}

void fn_1_DF04(void *arg0, s32 arg1)
{
    s32 *flag = (s32 *)((u8 *)arg0 + 0xC);

    *flag &= ~(1 << arg1);
}

void fn_1_DF2C(HU3DMODEL *arg0, Mtx *mtx)
{
    fn_1_E268((Mgr511Star *)arg0->hookData, *mtx);
}

Mgr511Star *fn_1_DF70(Mgr511Star *arg0, s32 arg_sp0)
{
    HU3DMODELID hookId;

    arg0->scale = 1.0f;
    arg0->timer = 0.0f;
    hookId = Hu3DHookFuncCreate(fn_1_DF2C);
    Hu3DData[hookId].hookData = arg0;
    Hu3DModelLayerSet(hookId, 5);
    Hu3DModelCameraSet(hookId, 0xF);
    arg0->modelId = (s16)hookId;
    if (lbl_1_bss_A38 == NULL) {
        fn_1_CF8("ms_pointer != 0", "singleton.h", 0x27);
    }
    arg0->anim = lbl_1_bss_A38->unk20;
    fn_1_E0B4(arg0);
    return arg0;
}

void *fn_1_E060(Mgr511Star *arg0, s16 arg1)
{
    if (arg0 != NULL) {
        Hu3DModelKill(arg0->modelId);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

void fn_1_E0B4(Mgr511Star *arg0)
{
    Mgr511Star *self = arg0;
    f32 temp_f31;
    s32 i;

    for (i = 0; i < 0x30; i++) {
        if (i < 0x20) {
            self->alpha[i] = 0x80;
        } else {
            self->alpha[i] = 0;
        }
        temp_f31 = (f32)((3.141592653589793 * (f64)(22.5f * (f32)i)) / 180.0);
        self->vert[i][0] = (f32)(150.0 * sin((f64)temp_f31));
        self->vert[i][1] = 180.0f * (f32)(i / 16);
        self->vert[i][2] = (f32)(150.0 * cos((f64)temp_f31));
    }
}

void fn_1_E220(Mgr511Star *arg0)
{
    arg0->timer = (f32)((f64)arg0->timer + 0.016666666666666666);
    if (!(arg0->timer < 1.0f)) {
        arg0->timer = 0.0f;
    }
}

// NONMATCHING: the immediate fifo writes and the quad index maths differ.
void fn_1_E268(Mgr511Star *arg0, Mtx mtx)
{
    f32 sp98[4][4];
    f32 sp68[4][4];
    f32 sp38[4][4];
    f32 sp34;
    f32 sp30;
    f32 sp2C;
    f32 sp28;
    f32 sp24;
    f32 sp20;
    f32 sp1C;
    f32 sp18;
    f32 sp14;
    f32 sp10;
    f32 spC;
    f32 sp8;
    s32 temp_r29;
    s32 temp_r24;
    s32 temp_r28;
    s32 temp_r27;
    s32 temp_r26;
    s32 temp_r25;
    s32 i;

    PSMTXTrans((f32 (*)[4])sp98, arg0->posX, arg0->posY, arg0->posZ);
    PSMTXScale((f32 (*)[4])sp68, 1.0f, arg0->scale, 1.0f);
    PSMTXConcat((f32 (*)[4])mtx, (f32 (*)[4])sp98, (f32 (*)[4])mtx);
    PSMTXConcat((f32 (*)[4])sp68, (f32 (*)[4])sp68, (f32 (*)[4])sp68);
    GXLoadPosMtxImm((f32 (*)[4])sp68, 0);
    HuSprTexLoad((ANIMDATA *)arg0->anim, (s16)0, (s16)0, (GXTexWrapMode)1, (GXTexWrapMode)1, (GXTexFilter)1);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_NOOP);
    GXSetZCompLoc(1);
    GXSetColorUpdate(1);
    GXSetAlphaUpdate(1);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetZMode(1, GX_LEQUAL, 0);
    GXSetCullMode(GX_CULL_NONE);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(1);
    PSMTXTrans((f32 (*)[4])sp38, 0.0f, 2.0f * (1.0f - arg0->timer), 0.0f);
    GXLoadTexMtxImm((f32 (*)[4])sp38, 0x1E, GX_MTX2x4);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x1E, 0, 0x7D);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_RASC, GX_CC_HALF, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 0, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 0, GX_TEVPREV);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_TEX_ST, GX_RGBA6, 0);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_TEX_ST, GX_RGBA8, 0);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_RGBA6, 0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 0x80);
    for (i = 0; i < 0x20; i++) {
        temp_r29 = (i / 16) * 0x10;
        temp_r24 = temp_r29 + 0x10;
        temp_r28 = (i & 0xF) + temp_r29;
        temp_r27 = ((i + 1) & 0xF) + temp_r29;
        temp_r26 = ((i + 1) & 0xF) + temp_r24;
        temp_r25 = (i & 0xF) + temp_r24;
        sp2C = arg0->vert[temp_r28][0];
        sp30 = arg0->vert[temp_r28][1];
        sp34 = arg0->vert[temp_r28][2];
        sp20 = arg0->vert[temp_r27][0];
        sp24 = arg0->vert[temp_r27][1];
        sp28 = arg0->vert[temp_r27][2];
        sp14 = arg0->vert[temp_r26][0];
        sp18 = arg0->vert[temp_r26][1];
        sp1C = arg0->vert[temp_r26][2];
        sp8 = arg0->vert[temp_r25][0];
        spC = arg0->vert[temp_r25][1];
        sp10 = arg0->vert[temp_r25][2];
        GXWGFifo.f32 = sp2C;
        GXWGFifo.f32 = sp30;
        GXWGFifo.f32 = sp34;
        GXWGFifo.u8 = 0x80;
        GXWGFifo.u8 = 0x80;
        GXWGFifo.u8 = 0x50;
        GXWGFifo.u8 = arg0->alpha[temp_r28];
        GXWGFifo.f32 = 0.0f;
        GXWGFifo.f32 = 0.0f;
        GXWGFifo.f32 = sp20;
        GXWGFifo.f32 = sp24;
        GXWGFifo.f32 = sp28;
        GXWGFifo.u8 = 0x80;
        GXWGFifo.u8 = 0x80;
        GXWGFifo.u8 = 0x50;
        GXWGFifo.u8 = arg0->alpha[temp_r27];
        GXWGFifo.f32 = 4.0f;
        GXWGFifo.f32 = 0.0f;
        GXWGFifo.f32 = sp14;
        GXWGFifo.f32 = sp18;
        GXWGFifo.f32 = sp1C;
        GXWGFifo.u8 = 0x80;
        GXWGFifo.u8 = 0x80;
        GXWGFifo.u8 = 0x50;
        GXWGFifo.u8 = arg0->alpha[temp_r26];
        GXWGFifo.f32 = 4.0f;
        GXWGFifo.f32 = 2.0f;
        GXWGFifo.f32 = sp8;
        GXWGFifo.f32 = spC;
        GXWGFifo.f32 = sp10;
        GXWGFifo.u8 = 0x80;
        GXWGFifo.u8 = 0x80;
        GXWGFifo.u8 = 0x50;
        GXWGFifo.u8 = arg0->alpha[temp_r25];
        GXWGFifo.f32 = 0.0f;
        GXWGFifo.f32 = 2.0f;
    }
}

void fn_1_E8AC(void *arg0, s32 arg1)
{
    *(s32 *)arg0 = arg1;
}

void fn_1_E8B4(void *arg0, s32 arg1)
{
    *(s32 *)arg0 = arg1;
}

}

/* The vtables this object defines: their entries are
   functions defined above, so the definitions sit here. */

/* lbl_1_data_4A4 is a vtable star.o itself defines (6 words at .data+0x2c), and this unit is linked from source, so it has to be a definition rather than an extern declaration: with only the declaration the REL link reported "Failed to find symbol lbl_1_data_4A4 in any module". 2 leading words are zero and the rest are ADDR32 relocations to functions in this object. The definition is at the end of the unit because its entries are functions defined further down. */
extern "C" void *lbl_1_data_4A4[6] = {
    0,
    0,
    (void *) fn_1_D948,
    (void *) fn_1_DF04,
    (void *) fn_1_DB04,
};

