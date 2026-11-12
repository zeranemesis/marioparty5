// m542dll/m542.c - decompilation of the m542dll/REL/m542dll/m542 unit.

#define _MATH_H /* keeps MWCC from adding its 16 bytes of sqrt literals to .rodata */
#include "types.h"
#include "game/audio.h"
#include "game/data.h"
#include "game/hu3d.h"
#include "game/hsfformat.h"
#include "game/object.h"
#include "game/wipe.h"
#include "game/mg/seqman.h"
#include "REL/m542dll/m542types.h"

#ifdef __MWERKS__
#define M542_RODATA __declspec(section ".rodata")
#else
#define M542_RODATA
#endif

#ifdef TARGET_PC
/* the original indexes lbl_1_data_2A8 (13 rows) with the raw character number */
#define M542_TEXROW(c) ((c) > 12 ? 12 : ((c) < 0 ? 0 : (c)))
#else
#define M542_TEXROW(c) (c)
#endif

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

double sin(double x);

extern void fn_1_2858(void);
extern void fn_1_2978(void);
extern void fn_1_2A14(void);
extern void fn_1_2AD0(void);
extern void fn_1_2BDC(void);
extern void fn_1_2DAC(void);
extern void fn_1_2F90(void);
extern void fn_1_4994(void);
extern void fn_1_5120(void);
extern void fn_1_5420(void);
extern void fn_1_5D30(void);
extern void fn_1_6A30(void);
extern void fn_1_737C(void);
extern void fn_1_84D0(M542WORK *work, M542ENTRY *ents, s32 *data);
extern void fn_1_871C(s16 arg0);
extern void fn_1_89DC(s32 *arg0, s32 *arg1);
extern void fn_1_8A00(s16 *arg0, s16 *arg1);
extern void fn_1_8A24(void);
extern void fn_1_8C94(void);
extern void fn_1_9150(void);
extern void fn_1_92A4(void);
extern void fn_1_94B4(void);

void ObjectSetup(void);
void fn_1_248(s16 arg0);
void fn_1_7A0(void);
void fn_1_7E4(s16 arg0);
void fn_1_838(void);
void fn_1_9F0(s16 arg0);
void fn_1_B70(void);
void fn_1_CC4(void);
void fn_1_CE8(void);
void fn_1_D1C(void);
void fn_1_D3C(void);
void fn_1_FC8(void);
void fn_1_249C(void);
void fn_1_24F8(void);
void fn_1_2524(HU3DMODEL *model, Mtx *mtx);

/* ---- data ---- */
s32 lbl_1_data_0[0x20] = {
    0x00640000, 0x00640001, 0x00640002, 0x00640003, 0x00640004, 0x00640005, 0x00640010, 0x00640011,
    0x00640012, 0x00640013, 0x00640014, 0x00640015, 0x00640016, 0x00640006, 0x00640007, 0x00640008,
    0x00640009, 0x0064000a, 0x0064000b, 0x0064000c, 0x0064000d, 0x0064000e, 0x0064000f, 0x0064001a,
    0x00640019, 0x00640018, 0x00640017, 0x0064001b, 0x0064001c, 0x0064001d, 0x0064001e, 0x0064001f,
};

s32 lbl_1_data_80[13][9] = {
    { 0x008c0000, 0x008c0003, 0x008c0004, 0x008c0008, 0x008c0009, 0x008c000a, 0x008c002f, 0x008c002b, 0x008c0079 },
    { 0x00330000, 0x00330003, 0x00330004, 0x00330008, 0x00330009, 0x0033000a, 0x0033002f, 0x0033002b, 0x00330079 },
    { 0x00b70000, 0x00b70003, 0x00b70004, 0x00b70008, 0x00b70009, 0x00b7000a, 0x00b7002f, 0x00b7002b, 0x00b70079 },
    { 0x00ed0000, 0x00ed0003, 0x00ed0004, 0x00ed0008, 0x00ed0009, 0x00ed000a, 0x00ed002f, 0x00ed002b, 0x00ed0079 },
    { 0x00e60000, 0x00e60003, 0x00e60004, 0x00e60008, 0x00e60009, 0x00e6000a, 0x00e6002f, 0x00e6002b, 0x00e60079 },
    { 0x00170000, 0x00170003, 0x00170004, 0x00170008, 0x00170009, 0x0017000a, 0x0017002f, 0x0017002b, 0x00170079 },
    { 0x00e00000, 0x00e00003, 0x00e00004, 0x00e00008, 0x00e00009, 0x00e0000a, 0x00e0002f, 0x00e0002b, 0x00e00079 },
    { 0x002d0000, 0x002d0003, 0x002d0004, 0x002d0008, 0x002d0009, 0x002d000a, 0x002d002f, 0x002d002b, 0x002d0079 },
    { 0x00c90000, 0x00c90003, 0x00c90004, 0x00c90008, 0x00c90009, 0x00c9000a, 0x00c9002f, 0x00c9002b, 0x00c90079 },
    { 0x00b00000, 0x00b00003, 0x00b00004, 0x00b00008, 0x00b00009, 0x00b0000a, 0x00b0002f, 0x00b0002b, 0x00b00079 },
    { 0x00b00000, 0x00b00003, 0x00b00004, 0x00b00008, 0x00b00009, 0x00b0000a, 0x00b0002f, 0x00b0002b, 0x00b00079 },
    { 0x00b00000, 0x00b00003, 0x00b00004, 0x00b00008, 0x00b00009, 0x00b0000a, 0x00b0002f, 0x00b0002b, 0x00b00079 },
    { 0x00b00000, 0x00b00003, 0x00b00004, 0x00b00008, 0x00b00009, 0x00b0000a, 0x00b0002f, 0x00b0002b, 0x00b00079 },
};

MGSEQ_PARAM lbl_1_data_254 = {
    0, 0,
    (MGSEQ_FUNC)fn_1_248, (MGSEQ_FUNC)fn_1_7A0, (MGSEQ_FUNC)fn_1_7E4,
    (MGSEQ_FUNC)fn_1_838, (MGSEQ_FUNC)fn_1_9F0, (MGSEQ_FUNC)fn_1_B70,
    (MGSEQ_FUNC)fn_1_CC4, (MGSEQ_FUNC)fn_1_CE8, (MGSEQ_FUNC)fn_1_D1C,
};

VoidFunc lbl_1_data_27C[4] = { fn_1_2858, fn_1_2978, fn_1_2A14, fn_1_2AD0 };
VoidFunc lbl_1_data_28C[3] = { fn_1_2BDC, fn_1_2DAC, fn_1_2F90 };
s32 lbl_1_data_298 = -1;
VoidFunc lbl_1_data_29C[3] = { fn_1_5420, fn_1_5D30, fn_1_6A30 };

f32 lbl_1_data_2A8[13][2] = {
    { 0.0f, 0.0f }, { 0.25f, 0.0f }, { 0.5f, 0.0f }, { 0.75f, 0.0f },
    { 0.0f, 0.25f }, { 0.25f, 0.25f }, { 0.5f, 0.25f }, { 0.75f, 0.25f },
    { 0.0f, 0.25f }, { 0.25f, 0.5f }, { 0.25f, 0.5f }, { 0.25f, 0.5f },
    { 0.25f, 0.5f },
};

char lbl_1_data_310[12] = "m542count01";
char lbl_1_data_31C[20] = "TL32m542burst01";

/* ---- bss (declared in reverse: MWCC lays bss out in reverse order) ---- */
OMCAMERAVIEW lbl_1_bss_78[4];
s16 lbl_1_bss_54[2][9];
M542AITAB lbl_1_bss_14;
u8 lbl_1_bss_10;
s32 lbl_1_bss_C;
s32 lbl_1_bss_8;
s32 lbl_1_bss_4;
s16 lbl_1_bss_0;

int _prolog(void)
{
    const VoidFunc *ctors = _ctors;
    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const VoidFunc *dtors = _dtors;
    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

void ObjectSetup(void)
{
    s16 i;
    s16 j;

    for (i = 0; i < 0x22; i++) {
        lbl_1_bss_1588[i].model = -1;
    }
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 0x1e; j++) {
            lbl_1_bss_E8[i][j].active = 0;
            lbl_1_bss_E8[i][j].model = -1;
        }
    }
    fn_1_84D0(&lbl_1_bss_1954, lbl_1_bss_1588, lbl_1_data_0);
    lbl_1_bss_1954.objman = omInitObjMan(0x32, 0x2000);
    omGameSysInit(lbl_1_bss_1954.objman);
    omAddObjEx(lbl_1_bss_1954.objman, 0x7fda, 0, 0, -1, omOutView);
    omAddObjEx(lbl_1_bss_1954.objman, 0x7fda, 0, 0, -1, (OMOBJFUNC)fn_1_8C94);
    omAddObjEx(lbl_1_bss_1954.objman, 0x7fda, 0, 0, -1, (OMOBJFUNC)fn_1_94B4);
    MgSeqCreate(&lbl_1_data_254);
}

void fn_1_248(s16 arg0)
{
    HuVecF pos;
    HuVecF up;
    HuVecF target;
    HU3DLIGHTID light;

    fn_1_871C(0);
    fn_1_89DC(lbl_1_data_80[lbl_1_bss_1954.player[0].playerNo], lbl_1_data_80[lbl_1_bss_1954.player[1].playerNo]);
    fn_1_8A00(lbl_1_bss_54[0], lbl_1_bss_54[1]);
    fn_1_8A24();
    fn_1_9150();
    omAddObjEx(lbl_1_bss_1954.objman, 0x7fda, 0, 0, -1, (OMOBJFUNC)fn_1_92A4);
    fn_1_FC8();
    Center.x = 0.0f;
    Center.y = 275.0f;
    Center.z = 0.0f;
    CRot.x = -50.0f;
    CRot.y = 0.0f;
    CRot.z = 0.0f;
    CZoom = 800.0f;
    lbl_1_bss_78[0].center.x = 0.0f;
    lbl_1_bss_78[0].center.y = 185.0f;
    lbl_1_bss_78[0].center.z = 0.0f;
    lbl_1_bss_78[0].rot.x = -10.0f;
    lbl_1_bss_78[0].rot.y = 0.0f;
    lbl_1_bss_78[0].rot.z = 0.0f;
    lbl_1_bss_78[0].zoom = 1120.0f;
    Hu3DCameraCreate(1);
    Hu3DCameraPerspectiveSet(1, 45.0f, 20.0f, 50000.0f, 1.2f);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(1, 0, 0, 640, 480);
    light = Hu3DGLightCreate(0.0f, 1000.0f, 1000.0f, 0.0f, -1.0f, -1.0f, 255, 255, 255);
    Hu3DGLightInfinitytSet(light);
    Hu3DGLightStaticSet(light, 1);
    omAddObjEx(lbl_1_bss_1954.objman, 0x7fda, 0, 0, -1, (OMOBJFUNC)fn_1_D3C);
    Hu3DBGColorSet(0, 0, 0);
    Hu3DShadowCreate(45.0f, 20.0f, 15000.0f);
    pos.x = 0.0f;
    pos.y = 1200.0f;
    pos.z = 600.0f;
    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;
    target.x = 0.0f;
    target.y = 0.0f;
    target.z = 0.0f;
    Hu3DShadowTPLvlSet(0.5f);
    Hu3DShadowPosSet(&pos, &up, &target);
    Hu3DModelShadowSet(lbl_1_bss_1954.ents[0x20].model);
    Hu3DModelShadowSet(lbl_1_bss_1954.ents[0x21].model);
    Hu3DModelShadowMapSet(lbl_1_bss_1954.ents[0].model);
    lbl_1_bss_1954.slot[0].mot = 0;
    lbl_1_bss_1954.slot[0].speed = 1.0f;
    lbl_1_bss_1954.slot[0].time = 0.0f;
    lbl_1_bss_1954.slot[0].attrSet = 0x40000001;
    lbl_1_bss_1954.slot[1].mot = 0;
    lbl_1_bss_1954.slot[1].speed = 1.0f;
    lbl_1_bss_1954.slot[1].time = 0.0f;
    lbl_1_bss_1954.slot[1].attrSet = 0x40000001;
    fn_1_4994();
    lbl_1_bss_10 = 0;
    fn_1_249C();
    Hu3DModelAttrReset(lbl_1_bss_0, 1);
    MgSeqModeDelaySet((s16)(arg0 + 1), -1);
    MgSeqModeNext();
}

void fn_1_7A0(void)
{
    lbl_1_data_27C[lbl_1_bss_1954.mode]();
}

void fn_1_7E4(s16 arg0)
{
    lbl_1_bss_1954.mode = 0;
    lbl_1_bss_1954.timer = 0;
    MgSeqModeDelaySet(arg0 + 1, -1);
}

void fn_1_838(void)
{
    lbl_1_data_28C[lbl_1_bss_1954.mode]();
    if (lbl_1_bss_4 != 0 || lbl_1_bss_8 != 0) {
        if (lbl_1_bss_C >= 300) {
            if (lbl_1_data_298 < 0) {
                lbl_1_data_298 = HuAudFXPlay(0x812);
            }
            lbl_1_bss_8 = (lbl_1_bss_8 + 1) % 48;
            lbl_1_bss_10 = (u8)(96.0 * sin(3.141592653589793 * (f64)((lbl_1_bss_8 * 0xb4) / 48) / 180.0));
        }
    } else if (lbl_1_data_298 >= 0) {
        HuAudFXFadeOut(lbl_1_data_298, 20);
        lbl_1_data_298 = -1;
    }
}

void fn_1_9F0(s16 arg0)
{
    if (lbl_1_data_298 >= 0) {
        HuAudFXFadeOut(lbl_1_data_298, 20);
        lbl_1_data_298 = -1;
    }
    MgSeqModeDelaySet(arg0 + 1, -1);
    fn_1_5120();
    if ((lbl_1_bss_4 != 0 || lbl_1_bss_8 != 0) && lbl_1_bss_C >= 300) {
        lbl_1_bss_8 = (lbl_1_bss_8 + 1) % 48;
        lbl_1_bss_10 = (u8)(96.0 * sin(3.141592653589793 * (f64)((lbl_1_bss_8 * 0xb4) / 48) / 180.0));
    }
}

void fn_1_B70(void)
{
    lbl_1_data_29C[lbl_1_bss_1954.mode]();
    if ((lbl_1_bss_4 != 0 || lbl_1_bss_8 != 0) && lbl_1_bss_C >= 300) {
        lbl_1_bss_8 = (lbl_1_bss_8 + 1) % 48;
        lbl_1_bss_10 = (u8)(96.0 * sin(3.141592653589793 * (f64)((lbl_1_bss_8 * 0xb4) / 48) / 180.0));
    }
}

void fn_1_CC4(void)
{
    MgSeqModeNext();
    fn_1_737C();
}

void fn_1_CE8(void)
{
    fn_1_737C();
    if ((u8)WipeCheckEnd() == 0) {
        MgSeqModeNext();
    }
}

void fn_1_D1C(void)
{
    fn_1_24F8();
}

void fn_1_D3C(void)
{
    M542ENTRY *e;
    s32 i;
    M542PART *p;
    s32 j;

    e = lbl_1_bss_1588;
    for (i = 0; i < 0x22; i++, e++) {
        if (e->model >= 0) {
            Hu3DModelPosSetV(e->model, &e->pos);
            Hu3DModelRotSetV(e->model, &e->rot);
        }
    }
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 0x1e; j++) {
            p = &lbl_1_bss_E8[i][j];
            if (p->active != 0) {
                Hu3DModelPosSetV(p->model, &p->pos);
                Hu3DModelRotSetV(p->model, &p->rot);
            }
        }
    }
}

void fn_1_E20(s32 arg0, f32 alpha)
{
    HU3DMODEL *model;
    HSFMATERIAL *mat;
    s32 i;
    HSFDATA *hsf;

    model = &Hu3DData[lbl_1_bss_1954.ents[arg0].model];
    hsf = model->hsf;
    mat = hsf->material;
    for (i = 0; i < model->hsf->materialNum; i++, mat++) {
        mat->invAlpha = alpha;
    }
}

void fn_1_E9C(M542ENTRY *e)
{
    Hu3DModelAttrSet(e->model, 1);
    Hu3DModelPosSet(e->model, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(e->model, 0.0f, 0.0f, 0.0f);
    e->pos.x = 0.0f;
    e->pos.y = 0.0f;
    e->pos.z = 0.0f;
    e->rot.x = 0.0f;
    e->rot.y = 0.0f;
    e->rot.z = 0.0f;
    Hu3DModelCameraSet(e->model, 1);
    Hu3DMotionTimeSet(e->model, 0.0f);
    Hu3DMotionSpeedSet(e->model, 0.0f);
    Hu3DModelAttrReset(e->model, 1);
}

void fn_1_FC8(void)
{
    M542ENTRY *e;
    s32 i;
    s32 j;

    e = &lbl_1_bss_1588[6];
    e->model = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_0[6], 0x10000000, HEAP_MODEL));
    fn_1_E9C(e);
    for (i = 0; i < 4; i++) {
        e = &lbl_1_bss_1588[i + 7];
        e->model = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_0[i + 7], 0x10000000, HEAP_MODEL));
        fn_1_E9C(e);
        Hu3DModelAttrReset(e->model, 0x40000001);
        if (i >= 2) {
            Hu3DModelShadowMapSet(e->model);
        }
    }
    for (i = 0; i < 2; i++) {
        e = &lbl_1_bss_1588[i + 0xb];
        if (i == 0) {
            e->model = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_0[0xb], 0x10000000, HEAP_MODEL));
        } else {
            e->model = Hu3DModelLink(lbl_1_bss_1954.ents[0xb].model);
        }
        fn_1_E9C(e);
        Hu3DModelPosSet(e->model, -210.0f + 420.0f * (f32)i, 0.0f, 254.0f);
        e->pos.x = -210.0f + 420.0f * (f32)i;
        e->pos.y = 0.0f;
        e->pos.z = 254.0f;
        Hu3DModelShadowMapSet(e->model);
    }
    for (i = 0; i < 2; i++) {
        e = &lbl_1_bss_1588[i + 0xd];
        if (i == 0) {
            e->model = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_0[0xc], 0x10000000, HEAP_MODEL));
        } else {
            e->model = Hu3DModelLink(lbl_1_bss_1954.ents[0xd].model);
        }
        fn_1_E9C(e);
        Hu3DModelPosSet(e->model, -210.0f + 420.0f * (f32)i, 0.0f, 254.0f);
        e->pos.x = -210.0f + 420.0f * (f32)i;
        e->pos.y = 0.0f;
        e->pos.z = 254.0f;
        Hu3DModelAttrSet(e->model, 1);
    }
    for (i = 0; i < 2; i++) {
        e = &lbl_1_bss_1588[i + 0xf];
        if (lbl_1_bss_1954.player[i].charNo > 9) {
            e->model = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_0[0x16], 0x10000000, HEAP_MODEL));
        } else {
            e->model = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_0[lbl_1_bss_1954.player[i].charNo + 0xd], 0x10000000, HEAP_MODEL));
        }
        fn_1_E9C(e);
        Hu3DModelLayerSet(e->model, 4);
        Hu3DModelPosSet(e->model, -290.0f + 580.0f * (f32)i, 132.0f, -128.0f);
        e->pos.x = -290.0f + 580.0f * (f32)i;
        e->pos.y = 132.0f;
        e->pos.z = -128.0f;
    }
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 2; j++) {
            e = &lbl_1_bss_1588[i + j * 5 + 0x11];
            e->model = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_0[i + 0x17], 0x10000000, HEAP_MODEL));
            fn_1_E9C(e);
            Hu3DModelPosSet(e->model, -210.0f + 420.0f * (f32)j, 0.0f, 0.0f);
            e->pos.x = -210.0f + 420.0f * (f32)j;
            e->pos.y = 0.0f;
            e->pos.z = 0.0f;
            if (i == 4) {
                Hu3DMotionTimeSet(e->model, 30.0f);
                Hu3DMotionSpeedSet(e->model, 0.0f);
                Hu3DModelAttrSet(e->model, 0x40000001);
            } else {
                lbl_1_bss_1940[j][i] = Hu3DTexScrollCreate(e->model, lbl_1_data_310);
            }
        }
    }
    for (i = 0; i < 2; i++) {
        e = &lbl_1_bss_1588[i + 0x1b];
        e->model = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_0[i + 0x1c], 0x10000000, HEAP_MODEL));
        fn_1_E9C(e);
        Hu3DModelLayerSet(e->model, 4);
        Hu3DModelPosSet(e->model, -290.0f + 580.0f * (f32)i, 132.0f, -128.0f);
        e->pos.x = -290.0f + 580.0f * (f32)i;
        e->pos.y = 132.0f;
        e->pos.z = -128.0f;
        Hu3DModelAttrSet(e->model, 1);
        lbl_1_bss_1940[i][4] = Hu3DTexScrollCreate(e->model, lbl_1_data_31C);
        Hu3DTexScrollPosSet(lbl_1_bss_1940[i][4], lbl_1_data_2A8[M542_TEXROW(lbl_1_bss_1954.player[i].charNo)][0],
                            lbl_1_data_2A8[M542_TEXROW(lbl_1_bss_1954.player[i].charNo)][1], 0.0f);
    }
    e = &lbl_1_bss_1588[0x1d];
    e->model = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_0[0x1e], 0x10000000, HEAP_MODEL));
    fn_1_E9C(e);
    Hu3DModelAttrSet(e->model, 1);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 0x1e; j++) {
            lbl_1_bss_E8[i][j].model = Hu3DModelLink(lbl_1_bss_1588[0x1d].model);
            fn_1_E9C(e);
            Hu3DModelAttrSet(lbl_1_bss_E8[i][j].model, 1);
            Hu3DModelLayerSet(lbl_1_bss_E8[i][j].model, 5);
        }
    }
    for (i = 0; i < 2; i++) {
        e = &lbl_1_bss_1588[i + 0x1e];
        e->model = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_0[0x1f], 0x10000000, HEAP_MODEL));
        fn_1_E9C(e);
        Hu3DModelLayerSet(e->model, 5);
        Hu3DModelPosSet(e->model, -290.0f + 580.0f * (f32)i, 464.0f, -128.0f);
        e->pos.x = -290.0f + 580.0f * (f32)i;
        e->pos.y = 464.0f;
        e->pos.z = -128.0f;
        Hu3DModelAttrSet(e->model, 1);
    }
}

void fn_1_249C(void)
{
    lbl_1_bss_0 = Hu3DHookFuncCreate(fn_1_2524);
    Hu3DModelLayerSet(lbl_1_bss_0, 6);
    Hu3DModelCameraSet(lbl_1_bss_0, 1);
}

void fn_1_24F8(void)
{
    Hu3DModelKill(lbl_1_bss_0);
}

void fn_1_2524(HU3DMODEL *model, Mtx *mtx)
{
    GXColor color = { 0, 0, 0, 0 };
    Mtx44 proj;

    color.r = 255;
    color.a = lbl_1_bss_10;
    GXSetViewport(0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    C_MTXOrtho(proj, 0.0f, 480.0f, 0.0f, 640.0f, 0.0f, -16777215.0f);
    GXSetProjection(proj, GX_ORTHOGRAPHIC);
    PSMTXIdentity(*mtx);
    GXLoadPosMtxImm(*mtx, GX_PNMTX0);
    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaUpdate(GX_FALSE);
    GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetNumChans(1);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_SPEC);
    GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_SPEC);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevColor(GX_TEVREG0, color);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3s16(0, 0, 0);
    GXPosition3s16(640, 0, 0);
    GXPosition3s16(640, 480, 0);
    GXPosition3s16(0, 480, 0);
}
