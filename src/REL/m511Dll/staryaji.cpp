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
extern "C" void __register_global_object(void *obj, void *(*dtor)(void *, s16), void *reg);
extern "C" void fn_1_1D88(void *self);
extern "C" void fn_1_1D94(void *self, s32 arg1);
extern "C" void fn_1_1E54(void *self, void **arg1);
f32 fn_1_884(s32 arg0, f32 arg1, f32 arg2);
extern "C" void fn_1_CF8(char *cond, char *file, s32 line);
void fn_1_DAB8(s32 arg0, f32 *arg1);

struct Mgr511StarYaji;

struct Mgr511StarYajiMaster {
    void *unk0;
    void *unk4;
    void **vtable;
    s32 unkC;
};

void fn_1_EDD8(void);
void fn_1_EEA8(void);
void fn_1_F148(Mgr511StarYaji *arg0);
void fn_1_F47C(Mgr511StarYaji *arg0, Mtx mtx);
void fn_1_F95C(Mgr511StarYaji **arg0, void (*arg1)(Mgr511StarYaji *));
void fn_1_F9BC(Mgr511StarYaji *arg0, Mgr511StarYaji **arg1, void (*arg2)(Mgr511StarYaji *));
void *fn_1_FA74(void *arg0, s16 arg1);

struct Mgr511StarYaji {
    Mgr511StarYaji *next;
    u32 unk4;
    u8 visible;
    u8 active;
    u16 unkA;
    f32 angle;
    f32 speed;
    f32 posX;
    f32 posY;
    f32 posZ;
    f32 size;
    s32 kind;
    s16 modelId;
    u16 pad2A;
    s32 frame;
    s32 playerNo;
    s32 unk34;
    s32 anim;
};

struct Mgr511StarYajiList {
    Mgr511StarYaji *next;
    Mgr511StarYaji *prev;
};

struct Mgr511Scene {
    u8 pad[0x1C];
    s32 unk1C;
};

extern "C" {

extern void *lbl_1_bss_2C;

extern void *lbl_1_data_40[6];
extern void *lbl_1_data_4E4[6];

extern Mgr511StarYajiMaster *lbl_1_bss_A34;
extern Mgr511Scene *lbl_1_bss_A38;

extern Mgr511StarYajiList lbl_1_bss_A9C;
extern Mgr511StarYaji *lbl_1_bss_A90;

static const s32 lbl_1_rodata_430[4] = {1, 2, 4, 8};

static const struct {
    f32 x;
    f32 z;
} lbl_1_rodata_46C[8] = {
    {0.0f, 400.0f},
    {50.0f, 300.0f},
    {25.0f, 300.0f},
    {25.0f, 200.0f},
    {0.0f, 200.0f},
    {-25.0f, 200.0f},
    {-25.0f, 300.0f},
    {-50.0f, 300.0f},
};

static const struct {
    f32 u;
    f32 v;
} lbl_1_rodata_4AC[8] = {
    {0.0f, 0.0f},
    {-0.5f, 2.0f},
    {-0.25f, 2.0f},
    {-0.25f, 4.0f},
    {0.0f, 4.0f},
    {0.25f, 4.0f},
    {0.25f, 2.0f},
    {0.5f, 2.0f},
};

static const GXColor lbl_1_rodata_4EC = {0xFF, 0xFF, 0xFF, 0xFF};

// NONMATCHING: the vtable pointer differs.
Mgr511StarYajiMaster *fn_1_E8BC(Mgr511StarYajiMaster *arg0, s32 arg_sp0)
{
    fn_1_1D88(arg0);
    arg0->vtable = &lbl_1_data_40[2];
    arg0->unkC = 0;
    fn_1_1E54(arg0, (void **)lbl_1_bss_2C);
    if (lbl_1_bss_A34 == NULL) {
        fn_1_CF8("ms_pointer == 0", "singleton.h", 0x35);
    }
    lbl_1_bss_A34 = arg0;
    arg0->vtable = lbl_1_data_4E4;
    return arg0;
}

void *fn_1_E98C(void *arg0, s16 arg1)
{
    if (arg0 != NULL) {
        if (lbl_1_bss_A34 == NULL) {
            fn_1_CF8("ms_pointer != 0", "singleton.h", 0x3D);
        }
        lbl_1_bss_A34 = (Mgr511StarYajiMaster *)NULL;
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

// NONMATCHING: the vtable pointer differs.
void *fn_1_EA18(void *arg0, s16 arg1)
{
    Mgr511StarYajiMaster *self = (Mgr511StarYajiMaster *)arg0;

    if (arg0 != NULL) {
        self->vtable = lbl_1_data_4E4;
        fn_1_EDD8();
        if (lbl_1_bss_A34 == NULL) {
            fn_1_CF8("ms_pointer != 0", "singleton.h", 0x3D);
        }
        lbl_1_bss_A34 = (Mgr511StarYajiMaster *)NULL;
        self->vtable = &lbl_1_data_40[2];
        fn_1_1D94(arg0, 0);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

void fn_1_EAE8(void *arg0, s32 arg1)
{
    s32 *flag = (s32 *)((u8 *)arg0 + 0xC);

    *flag &= ~(1 << arg1);
}

void fn_1_EB00(void)
{
    fn_1_EEA8();
}

void fn_1_EB20(HU3DMODEL *arg0, Mtx *mtx)
{
    Mgr511StarYaji *self = (Mgr511StarYaji *)arg0->hookData;

    if (self->visible != 0 && self->active != 0) {
        fn_1_F47C(self, *mtx);
    }
}

// NONMATCHING: the hook data assignment order differs.
Mgr511StarYaji *fn_1_EB7C(Mgr511StarYaji *arg0, s32 arg1, s32 arg2, s32 arg_sp0)
{
    HU3DMODELID hookId;

    arg0->visible = 0;
    arg0->active = 1;
    arg0->angle = 0.0f;
    arg0->size = 0.0f;
    arg0->frame = 0;
    arg0->playerNo = arg1;
    arg0->unk34 = arg2;
    fn_1_1E54(arg0, (void **)&lbl_1_bss_A9C);
    hookId = Hu3DHookFuncCreate(fn_1_EB20);
    arg0->modelId = (s16)hookId;
    Hu3DModelPosSet(hookId, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(hookId, 0.0f, 0.0f, 0.0f);
    Hu3DModelLayerSet(hookId, 6);
    Hu3DModelCameraSet(hookId, (u16)lbl_1_rodata_430[arg1]);
    Hu3DData[hookId].hookData = arg0;
    if (lbl_1_bss_A38 == NULL) {
        fn_1_CF8("ms_pointer != 0", "singleton.h", 0x27);
    }
    arg0->anim = lbl_1_bss_A38->unk1C;
    return arg0;
}

void *fn_1_ED18(void *arg0, s16 arg1)
{
    if (arg0 != NULL) {
        fn_1_1D94(arg0, 0);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

void *fn_1_ED70(void *arg0, s16 arg1)
{
    Mgr511StarYaji *self = (Mgr511StarYaji *)arg0;

    if (arg0 != NULL) {
        Hu3DModelKill(self->modelId);
        fn_1_1D94(arg0, 0);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

void fn_1_EDD8(void)
{
    Mgr511StarYaji *node;

    while (lbl_1_bss_A9C.next != (Mgr511StarYaji *)&lbl_1_bss_A9C || lbl_1_bss_A9C.prev != (Mgr511StarYaji *)&lbl_1_bss_A9C) {
        node = lbl_1_bss_A9C.prev;
        if (node != NULL) {
            Hu3DModelKill(node->modelId);
            fn_1_1D94(node, 0);
            __dl__FPv(node);
        }
    }
}

void fn_1_EEA8(void)
{
    fn_1_F95C((Mgr511StarYaji **)&lbl_1_bss_A9C, fn_1_F148);
}

// NONMATCHING: the atan2 argument order is not reproduced.
void fn_1_EED8(s32 arg0, Point3d *arg1, s32 arg2, f32 arg3)
{
    Mgr511StarYaji *node;
    Point3d cur;
    f32 ang;

    for (node = lbl_1_bss_A9C.next; node != (Mgr511StarYaji *)&lbl_1_bss_A9C; node = node->next) {
        if (node->playerNo == arg0) {
            fn_1_DAB8(arg2, &cur.x);
            if (cur.x != arg1->x || cur.z != arg1->z) {
                ang = (f32)(180.0 * (atan2((f64)(cur.x - arg1->x), (f64)(cur.z - arg1->z)) / 3.141592653589793));
                node->speed = arg3;
                node->angle = ang;
                node->active = 1;
            } else {
                node->active = 0;
            }
            node->kind = arg2;
            node->posX = arg1->x;
            node->posY = arg1->y;
            node->posZ = arg1->z;
            return;
        }
    }
}

void fn_1_F010(void)
{
    Mgr511StarYaji *node;

    for (node = lbl_1_bss_A9C.next; node != (Mgr511StarYaji *)&lbl_1_bss_A9C; node = node->next) {
        node->visible = 1;
    }
}

void fn_1_F074(void)
{
    Mgr511StarYaji *node;

    for (node = lbl_1_bss_A9C.next; node != (Mgr511StarYaji *)&lbl_1_bss_A9C; node = node->next) {
        node->visible = 0;
    }
}

void fn_1_F0D8(s32 arg0, s32 arg_sp0)
{
    Mgr511StarYaji *node;

    for (node = lbl_1_bss_A9C.next; node != (Mgr511StarYaji *)&lbl_1_bss_A9C; node = node->next) {
        if (node->playerNo == arg0) {
            node->visible = 0;
        }
    }
}

// NONMATCHING: the float select is folded differently.
void fn_1_F148(Mgr511StarYaji *arg0)
{
    f32 temp_f1;
    f32 spin;
    s32 frame;

    if (arg0->visible != 0) {
        temp_f1 = arg0->size + 10.0f;
        arg0->size = temp_f1;
        if (temp_f1 > 192.0f) {
            arg0->size = 192.0f;
        }
    } else {
        temp_f1 = arg0->size - 10.0f;
        arg0->size = temp_f1;
        if (temp_f1 < 0.0f) {
            arg0->size = 0.0f;
        }
    }
    frame = arg0->frame;
    arg0->frame = frame + 1;
    spin = fn_1_884(frame, arg0->angle, arg0->speed);
    if (spin < 0.0f) {
        spin = -spin;
    }
    Hu3DModelRotSet(arg0->modelId, -(90.0f - spin) * 0.25f, arg0->angle, 0.0f);
    Hu3DModelPosSet(arg0->modelId, arg0->posX, 100.0f + arg0->posY, arg0->posZ);
}

void fn_1_F2C8(Mgr511StarYaji *arg0)
{
    f32 identMat[4][4];

    HuSprTexLoad((ANIMDATA *)arg0->anim, (s16)0, (s16)0, (GXTexWrapMode)1, (GXTexWrapMode)1, (GXTexFilter)1);
    PSMTXIdentity((f32 (*)[4])identMat);
    GXLoadTexMtxImm((f32 (*)[4])identMat, 0x1E, GX_MTX2x4);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x1E, 0, 0x7D);
    GXSetCullMode(GX_CULL_NONE);
    GXSetColorUpdate(1);
    GXSetAlphaUpdate(0);
    GXSetZCompLoc(1);
    GXSetZMode(0, GX_ALWAYS, 0);
    GXSetAlphaCompare(GX_ALWAYS, 0xFF, GX_AOP_OR, GX_ALWAYS, 0xFF);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_COPY);
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXClearVtxDesc();
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_TEX_ST, GX_RGBA6, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_RGBA6, 0);
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
}

// NONMATCHING: the immediate fifo writes and the tev register packing differ.
void fn_1_F47C(Mgr511StarYaji *arg0, Mtx mtx)
{
    f32 identMat[4][4];
    GXColor color;
    f32 var_f31;
    f32 temp_f29;
    f32 temp_f30;
    s32 i;

    GXLoadPosMtxImm((f32 (*)[4])mtx, 0);
    HuSprTexLoad((ANIMDATA *)arg0->anim, (s16)0, (s16)0, (GXTexWrapMode)1, (GXTexWrapMode)1, (GXTexFilter)1);
    PSMTXIdentity((f32 (*)[4])identMat);
    GXLoadTexMtxImm((f32 (*)[4])identMat, 0x1E, GX_MTX2x4);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x1E, 0, 0x7D);
    GXSetCullMode(GX_CULL_NONE);
    GXSetColorUpdate(1);
    GXSetAlphaUpdate(0);
    GXSetZCompLoc(1);
    GXSetZMode(0, GX_ALWAYS, 0);
    GXSetAlphaCompare(GX_ALWAYS, 0xFF, GX_AOP_OR, GX_ALWAYS, 0xFF);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_COPY);
    GXSetNumChans(0);
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXClearVtxDesc();
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_TEX_ST, GX_RGBA6, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_RGBA6, 0);
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    var_f31 = 1.0f;
    color.r = lbl_1_rodata_4EC.r;
    color.g = lbl_1_rodata_4EC.g;
    color.b = lbl_1_rodata_4EC.b;
    color.a = (u8)arg0->size;
    if (arg0->kind == 5) {
        color.g = (u8)(255.0f * ((f32)(arg0->frame & 0x1F) / 31.0f));
        var_f31 = 1.0f + ((f32)(arg0->frame & 0x1F) / 31.0f);
    }
    GXSetTevColor(GX_TEVREG0, color);
    temp_f30 = (f32)(arg0->frame & 0xF) * 0.0625f;
    temp_f29 = (100.0f * ((f32)(arg0->frame & 0x1F) * 0.03125f)) - 100.0f;
    GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 8);
    for (i = 0; i < 8; i++) {
        GXWGFifo.f32 = var_f31 * lbl_1_rodata_46C[i].x;
        GXWGFifo.f32 = 0.0f;
        GXWGFifo.f32 = (f32)(200.0f + (var_f31 * (lbl_1_rodata_46C[i].z - 200.0f)) + temp_f29);
        GXWGFifo.f32 = lbl_1_rodata_4AC[i].u;
        GXWGFifo.f32 = temp_f30 + lbl_1_rodata_4AC[i].v;
    }
}

void fn_1_F95C(Mgr511StarYaji **arg0, void (*arg1)(Mgr511StarYaji *))
{
    fn_1_F9BC(*arg0, arg0, arg1);
}

void fn_1_F9BC(Mgr511StarYaji *arg0, Mgr511StarYaji **arg1, void (*arg2)(Mgr511StarYaji *))
{
    Mgr511StarYaji *node = arg0;

    while (node != (Mgr511StarYaji *)arg1) {
        arg2(node);
        node = *(Mgr511StarYaji **)node;
    }
}

void fn_1_FA1C(void)
{
    fn_1_1D88(&lbl_1_bss_A9C);
    __register_global_object(&lbl_1_bss_A9C, fn_1_FA74, &lbl_1_bss_A90);
}

void *fn_1_FA74(void *arg0, s16 arg1)
{
    if (arg0 != NULL) {
        fn_1_1D94(arg0, 0);
        if (arg1 > 0) {
            __dl__FPv(arg0);
        }
    }
    return arg0;
}

}

/* The vtables this object defines: their entries are
   functions defined above, so the definitions sit here. */

/* lbl_1_data_4E4 is a vtable staryaji.o itself defines (6 words at .data+0x2c), and this unit is linked from source, so it has to be a definition rather than an extern declaration: with only the declaration the REL link reported "Failed to find symbol lbl_1_data_4E4 in any module". 2 leading words are zero and the rest are ADDR32 relocations to functions in this object. The definition is at the end of the unit because its entries are functions defined further down. */
extern "C" void *lbl_1_data_4E4[6] = {
    0,
    0,
    (void *) fn_1_EA18,
    (void *) fn_1_EAE8,
    (void *) fn_1_EB00,
};

