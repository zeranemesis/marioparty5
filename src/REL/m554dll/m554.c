#include "types.h"
#include "math.h"
#include "game/data.h"
#include "game/hsfformat.h"
#include "game/hu3d.h"
#include "game/object.h"
#include "game/wipe.h"
#include "game/charman.h"
#include "game/mg/seqman.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

extern void fn_1_1F00(void);
extern void fn_1_2118(void);
extern void fn_1_22CC(void);
extern void fn_1_240C(void);
extern void fn_1_2654(void);
extern void fn_1_2800(void);
extern void fn_1_2870(void);
extern void fn_1_2C74(void);
extern void fn_1_2804(void);
extern void fn_1_31C8(void);
extern void fn_1_53B0(void);
extern void fn_1_62F0(void *work, void *ents, void *data);
extern void fn_1_653C(s32 arg0);
extern void fn_1_6C20(s16 *arg0, s16 *arg1);
extern void fn_1_6C44(s16 *arg0, s16 *arg1);
extern void fn_1_6C68(void);
extern void fn_1_6E80(void);
extern void fn_1_733C(void);
extern void fn_1_7490(void);
extern void fn_1_7880(void);

void ObjectSetup(void);
void fn_1_1C0(s16 arg0);
void fn_1_808(void);
void fn_1_84C(s16 arg0);
void fn_1_914(void);
void fn_1_958(s16 arg0);
void fn_1_990(void);
void fn_1_9D4(void);
void fn_1_9F8(void);
void fn_1_A2C(void);
void fn_1_A30(void);
void fn_1_A64(void);
void fn_1_EB4(void);
void fn_1_144C(void);

typedef struct {
    s16 model;                 // 0x00
    HuVecF pos;                // 0x04
    HuVecF rot;                // 0x10
} M554ENTRY;                   // 0x1c bytes

typedef struct {
    s16 padNo;                 // 0x00
    s16 charNo;                // 0x02
    s16 human;                 // 0x04
    u8 pad_06[0x1c - 0x06];
} M554PLAYER;                  // 0x1c bytes

typedef struct {
    s16 f_00;                  // 0x00
    u8 pad_02[6];
    s32 f_08;                  // 0x08
} M554ROW;                     // 0x0c bytes

typedef struct {
    s16 f_00;                  // 0x00
    s16 f_02;                  // 0x02
    f32 f_04;                  // 0x04
    u8 pad_08[4];
    f32 f_0c;                  // 0x0c
    u8 pad_10[8];
    u32 f_18;                  // 0x18
    u8 pad_1c[0x1c];
    s16 f_38;                  // 0x38
    s16 f_3a;                  // 0x3a
    u32 f_3c;                  // 0x3c
    f32 f_40;                  // 0x40
    f32 f_44;                  // 0x44
    u8 pad_48[0x0c];
} M554SLOT;                    // 0x54 bytes

typedef struct {
    OMOBJMAN *objman;          // 0x000
    M554PLAYER player[2];      // 0x004
    M554ENTRY *ents;           // 0x03c
    u8 pad_040[0x10];
    M554SLOT slot[2];          // 0x050
    u8 pad_0f8[8];
    s32 mode;                  // 0x100
    s32 f_104;                 // 0x104
    s32 f_108;                 // 0x108
    s32 f_10c;                 // 0x10c
    s32 count[4];              // 0x110
    s32 f_120;                 // 0x120
    s32 f_124[4];              // 0x124
    f32 f_134[8];              // 0x134
    u8 pad_154[0x814 - 0x154];
} M554WORK;                    // 0x814 bytes

/* ---- data ---- */
s32 lbl_1_data_0 = 0x700003;

s32 lbl_1_data_4[20] = {
    0x00700000, 0x00700001, 0x00700002, 0x00700004, 0x00700005,
    0x00700006, 0x00700007, 0x00700008, 0x00700009, 0x0070000a,
    0x0070000b, 0x0070000c, 0x0070000d, 0x0070000e, 0x0070000f,
    0x00700010, 0x00700011, 0x00700012, 0x00700013, 0x00700014,
};

s16 lbl_1_data_54[13][10] = {
    { 0x87, 0xaf, 0x87, 0xb0, 0x8c, 0x7e, 0x8c, 0x2f, 0x8c, 0x2b },
    { 0x2e, 0xaf, 0x2e, 0xb0, 0x33, 0x7e, 0x33, 0x2f, 0x33, 0x2b },
    { 0xb2, 0xaf, 0xb2, 0xb0, 0xb7, 0x7e, 0xb7, 0x2f, 0xb7, 0x2b },
    { 0xe8, 0xaf, 0xe8, 0xb0, 0xed, 0x7e, 0xed, 0x2f, 0xed, 0x2b },
    { 0xe1, 0xaf, 0xe1, 0xb0, 0xe6, 0x7e, 0xe6, 0x2f, 0xe6, 0x2b },
    { 0x12, 0xaf, 0x12, 0xb0, 0x17, 0x7e, 0x17, 0x2f, 0x17, 0x2b },
    { 0xdb, 0xaf, 0xdb, 0xb0, 0xe0, 0x7e, 0xe0, 0x2f, 0xe0, 0x2b },
    { 0x28, 0xaf, 0x28, 0xb0, 0x2d, 0x7e, 0x2d, 0x2f, 0x2d, 0x2b },
    { 0xc4, 0xaf, 0xc4, 0xb0, 0xc9, 0x7e, 0xc9, 0x2f, 0xc9, 0x2b },
    { 0x9c, 0xaf, 0x9c, 0xb0, 0xb0, 0x7e, 0xb0, 0x2f, 0xb0, 0x2b },
    { 0x9c, 0xaf, 0x9c, 0xb0, 0xb0, 0x7e, 0xb0, 0x2f, 0xb0, 0x2b },
    { 0x9c, 0xaf, 0x9c, 0xb0, 0xb0, 0x7e, 0xb0, 0x2f, 0xb0, 0x2b },
    { 0x9c, 0xaf, 0x9c, 0xb0, 0xb0, 0x7e, 0xb0, 0x2f, 0xb0, 0x2b },
};

MGSEQ_PARAM lbl_1_data_158 = {
    0, 0,
    (MGSEQ_FUNC)fn_1_1C0, (MGSEQ_FUNC)fn_1_808, (MGSEQ_FUNC)fn_1_84C,
    (MGSEQ_FUNC)fn_1_914, (MGSEQ_FUNC)fn_1_958, (MGSEQ_FUNC)fn_1_990,
    (MGSEQ_FUNC)fn_1_9D4, (MGSEQ_FUNC)fn_1_9F8, (MGSEQ_FUNC)fn_1_A2C,
};

VoidFunc lbl_1_data_180[3] = { fn_1_1F00, fn_1_2118, fn_1_22CC };
VoidFunc lbl_1_data_18C[3] = { fn_1_240C, fn_1_2654, fn_1_2800 };

struct {
    VoidFunc func[3];
    f32 f_0c;
} lbl_1_data_198 = { { fn_1_2870, fn_1_2C74, fn_1_31C8 }, 1.0f };

s32 lbl_1_data_1A8[2] = { 0, 0 };
f32 lbl_1_data_1B0[2] = { 0.5f, 0.2f };
f32 lbl_1_data_1B8[2] = { 0.0f, 2.0f };
s32 lbl_1_data_1C0 = 0x12;

f32 lbl_1_data_1C4[13][2] = {
    { 0.0f, 0.0f }, { 0.25f, 0.0f }, { 0.5f, 0.0f }, { 0.75f, 0.0f },
    { 0.0f, 0.25f }, { 0.25f, 0.25f }, { 0.5f, 0.25f }, { 0.75f, 0.25f },
    { 0.0f, 0.5f }, { 0.25f, 0.5f }, { 0.25f, 0.5f }, { 0.25f, 0.5f },
    { 0.25f, 0.5f },
};

char lbl_1_data_22C[12] = "CI8m554";

/* ---- bss (declared in reverse: MWCC lays bss out in reverse order) ---- */
M554WORK lbl_1_bss_B1C;
M554ENTRY lbl_1_bss_10C[0x5c];
s16 lbl_1_bss_108[2];
OMCAMERAVIEW lbl_1_bss_98[4];
s16 lbl_1_bss_84[2][5];
M554ROW lbl_1_bss_6C[2];
struct {
    M554ROW row[7];
    s32 pad;
} lbl_1_bss_14;
HuVecF lbl_1_bss_8;
s32 lbl_1_bss_4;
s32 lbl_1_bss_0;

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
    for (i = 0; i < 0x5c; i++) {
        lbl_1_bss_10C[i].model = -1;
    }
    fn_1_62F0(&lbl_1_bss_B1C, lbl_1_bss_10C, lbl_1_data_4);
    lbl_1_bss_B1C.objman = omInitObjMan(0x32, 0x2000);
    omGameSysInit(lbl_1_bss_B1C.objman);
    omAddObjEx(lbl_1_bss_B1C.objman, 0x7fda, 0, 0, -1, omOutView);
    omAddObjEx(lbl_1_bss_B1C.objman, 0x7fda, 0, 0, -1, (OMOBJFUNC)fn_1_6E80);
    omAddObjEx(lbl_1_bss_B1C.objman, 0x7fda, 0, 0, -1, (OMOBJFUNC)fn_1_A30);
    MgSeqCreate(&lbl_1_data_158);
}

void fn_1_1C0(s16 arg0)
{
    HuVecF pos;
    HuVecF up;
    HuVecF target;
    HU3DLIGHTID light;

    fn_1_653C(2);
    fn_1_6C20(lbl_1_data_54[lbl_1_bss_B1C.player[0].charNo], lbl_1_data_54[lbl_1_bss_B1C.player[1].charNo]);
    fn_1_6C44(lbl_1_bss_84[0], lbl_1_bss_84[1]);
    fn_1_6C68();
    fn_1_733C();
    omAddObjEx(lbl_1_bss_B1C.objman, 0x7fda, 0, 0, -1, (OMOBJFUNC)fn_1_7490);
    fn_1_144C();
    omAddObjEx(lbl_1_bss_B1C.objman, 0x7fda, 0, 0, -1, (OMOBJFUNC)fn_1_7880);
    Center.x = 0.0f;
    Center.y = 390.0f;
    Center.z = 200.0f;
    CRot.x = -20.0f;
    CRot.y = 0.0f;
    CRot.z = 0.0f;
    CZoom = 220.0f;
    lbl_1_bss_98[0].center.x = 0.0f;
    lbl_1_bss_98[0].center.y = 400.0f;
    lbl_1_bss_98[0].center.z = 100.0f;
    lbl_1_bss_98[0].rot.x = -25.0f;
    lbl_1_bss_98[0].rot.y = 0.0f;
    lbl_1_bss_98[0].rot.z = 0.0f;
    lbl_1_bss_98[0].zoom = 1450.0f;
    lbl_1_bss_98[1].center.x = -40.0f;
    lbl_1_bss_98[1].center.y = 290.0f;
    lbl_1_bss_98[1].center.z = 200.0f;
    lbl_1_bss_98[1].rot.x = -14.0f;
    lbl_1_bss_98[1].rot.y = -26.0f;
    lbl_1_bss_98[1].rot.z = -8.0f;
    lbl_1_bss_98[1].zoom = 1560.0f;
    Hu3DCameraCreate(1);
    Hu3DCameraPerspectiveSet(1, 45.0f, 20.0f, 50000.0f, 1.2f);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(1, 0, 0, 640, 480);
    light = Hu3DGLightCreate(0.0f, 1000.0f, 1000.0f, 0.0f, -1.0f, -1.0f, 255, 255, 255);
    Hu3DGLightInfinitytSet(light);
    Hu3DGLightStaticSet(light, 1);
    omAddObjEx(lbl_1_bss_B1C.objman, 0x7fda, 0, 0, -1, (OMOBJFUNC)fn_1_EB4);
    Hu3DBGColorSet(0, 0, 0);
    Hu3DShadowCreate(45.0f, 20.0f, 15000.0f);
    pos.x = 0.0f;
    pos.y = 1200.0f;
    pos.z = 10.0f;
    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;
    target.x = 0.0f;
    target.y = 0.0f;
    target.z = 0.0f;
    Hu3DShadowTPLvlSet(0.5f);
    Hu3DShadowPosSet(&pos, &up, &target);
    Hu3DModelShadowSet(lbl_1_bss_B1C.ents[0x4e].model);
    Hu3DModelShadowSet(lbl_1_bss_B1C.ents[0x4f].model);
    lbl_1_bss_B1C.slot[0].f_00 = 0;
    lbl_1_bss_B1C.slot[0].f_04 = 1.0f;
    lbl_1_bss_B1C.slot[0].f_0c = 0.0f;
    lbl_1_bss_B1C.slot[0].f_18 = 0x40000001;
    lbl_1_bss_B1C.slot[1].f_00 = 0;
    lbl_1_bss_B1C.slot[1].f_04 = 1.0f;
    lbl_1_bss_B1C.slot[1].f_0c = 0.0f;
    lbl_1_bss_B1C.slot[1].f_18 = 0x40000001;
    lbl_1_bss_B1C.f_134[0] = 800.0f;
    lbl_1_bss_B1C.f_134[1] = -22.5f;
    lbl_1_bss_B1C.f_134[4] = 800.0f;
    lbl_1_bss_B1C.f_134[5] = 22.5f;
    MgSeqModeDelaySet((s16)(arg0 + 1), -1);
    MgSeqModeNext();
}

void fn_1_808(void)
{
    lbl_1_data_180[lbl_1_bss_B1C.mode]();
}

void fn_1_84C(s16 arg0)
{
    lbl_1_bss_B1C.mode = 0;
    lbl_1_bss_B1C.f_104 = 0;
    MgSeqModeDelaySet(arg0 + 1, -1);
    if (lbl_1_bss_0 == 0 && omCameraViewCheck(1) != 0) {
        omAddObjEx(lbl_1_bss_B1C.objman, 0x7fda, 0, 0, -1, (OMOBJFUNC)fn_1_53B0);
        Hu3DModelAttrReset(lbl_1_bss_B1C.ents[0x4d].model, 1);
        lbl_1_bss_0 = 1;
    }
}

void fn_1_914(void)
{
    lbl_1_data_18C[lbl_1_bss_B1C.mode]();
}

void fn_1_958(s16 arg0)
{
    MgSeqModeDelaySet(arg0 + 1, -1);
    fn_1_2804();
}

void fn_1_990(void)
{
    lbl_1_data_198.func[lbl_1_bss_B1C.mode]();
}

void fn_1_9D4(void)
{
    fn_1_31C8();
    MgSeqModeNext();
}

void fn_1_9F8(void)
{
    fn_1_31C8();
    if ((u8)WipeCheckEnd() == 0) {
        MgSeqModeNext();
    }
}

void fn_1_A2C(void)
{
}

void fn_1_A30(void)
{
}

void fn_1_A34(void)
{
    lbl_1_data_1C0 -= 1;
}

s32 fn_1_A54(void)
{
    return lbl_1_data_1C0;
}

void fn_1_A64(void)
{
    s32 i;
    M554ENTRY *e;

    lbl_1_data_1A8[1]++;
    for (i = 0; i < 0x3c; i++) {
        e = &lbl_1_bss_10C[i + 0x11];
        if (i % 3 == 0) {
            e->pos.x -= 6.0f;
            e->pos.z -= 4.0f;
            e->pos.y -= 1.0f;
        }
        if (i % 3 == 1) {
            e->pos.y -= 1.0f;
            e->pos.z -= 2.0f;
        }
        if (i % 3 == 2) {
            e->pos.x += 6.0f;
            e->pos.z -= 4.0f;
            e->pos.y -= 1.0f;
        }
    }
    if (lbl_1_data_1A8[1]++ > lbl_1_data_1C0) {
        for (i = 0; i < 3; i++) {
            e = &lbl_1_bss_10C[lbl_1_bss_4 * 3 + 0x11 + i];
            e->pos = lbl_1_bss_8;
            e->pos.x = e->pos.x + 50.0 * sin(3.141592653589793 * (f64)(i * 0x2d - 0x2d + (i - 5) * 2) / 180.0);
            e->pos.z = e->pos.z + 40.0 * cos(3.141592653589793 * (f64)(i * 0x2d - 0x2d + (i - 5) * 2) / 180.0);
            e->pos.y -= 25.0f;
            Hu3DMotionTimeSet(e->model, 0.0f);
            Hu3DMotionSpeedSet(e->model, 1.0f);
            Hu3DModelAttrReset(e->model, 1);
            if (i == 1) {
                Hu3DModelScaleSet(e->model, 6.0f, 6.0f, 6.0f);
            } else {
                Hu3DModelScaleSet(e->model, 5.0f, 5.0f, 5.0f);
            }
        }
        lbl_1_data_1A8[1] = 0;
        lbl_1_bss_4 = (lbl_1_bss_4 + 1) % 10;
        if (lbl_1_bss_4 == 0 && lbl_1_data_1C0 > 8) {
            lbl_1_data_1C0 -= 1;
        }
    }
}

void fn_1_EB4(void)
{
    HuVecF pos;
    HuVecF rot;
    s32 i;
    M554ENTRY *e;

    e = lbl_1_bss_10C;
    for (i = 0; i < 0x5c; i++, e++) {
        if (e->model >= 0) {
            if (i == 2) {
                pos = e->pos;
                rot = e->rot;
                if (e->rot.y == 0.0f) {
                    lbl_1_data_1A8[0]++;
                    if (lbl_1_data_1A8[0] > 0x1e) {
                        lbl_1_data_1A8[0] = 0;
                        if (lbl_1_data_1B0[0] < 25.0f) {
                            lbl_1_data_1B0[0] += 1.25f;
                        }
                        if (lbl_1_data_1B8[1] < 25.0f) {
                            lbl_1_data_1B8[1] += 0.5f;
                        }
                        if (lbl_1_data_1B0[1] < 2.0f) {
                            lbl_1_data_1B0[1] += 0.1f;
                        }
                    }
                    lbl_1_data_1B8[0] -= lbl_1_data_1B8[1];
                    if (lbl_1_data_1B8[0] > 360.0f) {
                        lbl_1_data_1B8[0] -= 360.0f;
                    }
                    if (lbl_1_data_1B8[0] < 0.0f) {
                        lbl_1_data_1B8[0] += 360.0f;
                    }
                    pos.z = pos.z + lbl_1_data_1B0[0] * cos(3.141592653589793 * lbl_1_data_1B8[0] / 180.0);
                    pos.x = pos.x + lbl_1_data_1B0[0] * sin(3.141592653589793 * lbl_1_data_1B8[0] / 180.0);
                    rot.z = lbl_1_data_1B0[1] * cos(3.141592653589793 * lbl_1_data_1B8[0] / 180.0);
                    rot.x = lbl_1_data_1B0[1] * sin(3.141592653589793 * lbl_1_data_1B8[0] / 180.0);
                }
                lbl_1_bss_8 = pos;
                Hu3DModelPosSetV(e->model, &pos);
                Hu3DModelRotSetV(e->model, &rot);
                fn_1_A64();
            } else {
                Hu3DModelPosSetV(e->model, &e->pos);
                Hu3DModelRotSetV(e->model, &e->rot);
            }
        }
    }
}

void fn_1_12A4(s32 arg0, f32 value)
{
    HU3DMODEL *model;
    HSFMATERIAL *mat;
    s32 i;
    HSFDATA *hsf;

    model = &Hu3DData[lbl_1_bss_B1C.ents[arg0].model];
    hsf = model->hsf;
    mat = hsf->material;
    for (i = 0; i < model->hsf->materialNum; i++, mat++) {
        mat->invAlpha = value;
    }
}

void fn_1_1320(M554ENTRY *arg0)
{
    Hu3DModelAttrSet(arg0->model, 1);
    Hu3DModelPosSet(arg0->model, 0, 0, 0);
    Hu3DModelRotSet(arg0->model, 0, 0, 0);
    arg0->pos.x = 0;
    arg0->pos.y = 0;
    arg0->pos.z = 0;
    arg0->rot.x = 0;
    arg0->rot.y = 0;
    arg0->rot.z = 0;
    Hu3DModelCameraSet(arg0->model, 1);
    Hu3DMotionTimeSet(arg0->model, 0);
    Hu3DMotionSpeedSet(arg0->model, 0);
    Hu3DModelAttrReset(arg0->model, 1);
}

void fn_1_144C(void)
{
    s32 i;
    M554ENTRY *e;
    HU3DMOTID mot;

    e = &lbl_1_bss_10C[2];
    e->model = Hu3DModelCreateData(lbl_1_data_4[2]);
    mot = Hu3DJointMotionData(e->model, lbl_1_data_0);
    Hu3DMotionSet(e->model, mot);
    fn_1_1320(e);
    Hu3DMotionSpeedSet(e->model, 1.0f);
    Hu3DModelScaleSet(e->model, 5.0f, 5.0f, 5.0f);
    Hu3DModelAttrSet(e->model, 0x40000001);
    e->pos.x = 0.0f;
    e->pos.y = -200.0f;
    e->pos.z = -600.0f;
    e->rot.x = 0.0f;
    e->rot.y = -180.0f;
    e->rot.z = 0.0f;
    Hu3DModelLayerSet(e->model, 3);
    for (i = 0; i < 2; i++) {
        e = &lbl_1_bss_10C[i + 3];
        e->model = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_4[3], 0x10000000, HEAP_MODEL));
        fn_1_1320(e);
        lbl_1_bss_108[i] = Hu3DTexScrollCreate(e->model, lbl_1_data_22C);
        if (lbl_1_bss_108[i] >= 0) {
            Hu3DTexScrollPosSet(lbl_1_bss_108[i], lbl_1_data_1C4[lbl_1_bss_B1C.player[i].charNo][0], lbl_1_data_1C4[lbl_1_bss_B1C.player[i].charNo][1], 0.0f);
        }
        Hu3DModelHookSet(lbl_1_bss_10C[i + 0x4e].model, CharModelItemHookGet(lbl_1_bss_B1C.player[i].charNo, 1, 0), e->model);
        e = &lbl_1_bss_10C[i + 5];
        e->model = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_4[4], 0x10000000, HEAP_MODEL));
        fn_1_1320(e);
        Hu3DMotionSpeedSet(e->model, 1.0f);
        Hu3DModelAttrSet(e->model, 0x40000001);
        Hu3DModelShadowMapSet(e->model);
        Hu3DModelLayerSet(e->model, 4);
    }
    for (i = 0; i < 7; i++) {
        e = &lbl_1_bss_10C[i + 7];
        e->model = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_4[i + 5], 0x10000000, HEAP_MODEL));
        fn_1_1320(e);
        Hu3DModelLayerSet(e->model, 4);
        Hu3DModelAttrSet(e->model, 1);
    }
    for (i = 0; i < 0x3c; i++) {
        e = &lbl_1_bss_10C[i + 0x11];
        if (i == 0) {
            e->model = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_4[0xc], 0x10000000, HEAP_MODEL));
        } else {
            e->model = Hu3DModelLink(lbl_1_bss_B1C.ents[0x11].model);
        }
        fn_1_1320(e);
        Hu3DModelLayerSet(e->model, 4);
        Hu3DModelAttrSet(e->model, 1);
        Hu3DModelScaleSet(e->model, 6.0f, 6.0f, 6.0f);
    }
    e = &lbl_1_bss_10C[0x4d];
    e->model = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_4[0xd], 0x10000000, HEAP_MODEL));
    fn_1_1320(e);
    Hu3DModelLayerSet(e->model, 3);
    Hu3DMotionTimeSet(e->model, 0.0f);
    Hu3DMotionSpeedSet(e->model, 1.0f);
    Hu3DModelAttrSet(e->model, 1);
}
