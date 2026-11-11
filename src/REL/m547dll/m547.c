#include "types.h"

#include "game/gamework.h"
#include "game/object.h"
#include "game/hu3d.h"
#include "game/esprite.h"
#include "game/printfunc.h"
#include "game/wipe.h"
#include "game/mg/seqman.h"
#include "game/mg/score.h"

s32 rand8(void);

typedef void (*M547VoidFunc)(void);
extern const M547VoidFunc _ctors[];
extern const M547VoidFunc _dtors[];

/* per-character slot, 0x1C bytes; the objman pointer of M547State sits in front of chars[0] */
typedef struct M547CharSlot_s {
    s16 playerNo;
    s16 charNo;
    s16 cpuLevel;
    s16 padNo;
    s32 btn;
    s32 btnDown;
    s8 stkX;
    s8 stkY;
    u16 btnRep;
    s8 subStkX;
    s8 subStkY;
    u8 trigL;
    u8 trigR;
    u8 dstk;
    u8 dstkRep;
    u8 pad_1A[2];
} M547CharSlot;

/* per-stage slot, 0x24 bytes */
typedef struct M547StageSlot_s {
    s16 motion;
    s16 id;
    f32 speed;
    f32 curSpeed;
    f32 time;
    f32 curTime;
    f32 maxTime;
    u32 attrSet;
    u32 attrCur;
    u32 attrReset;
} M547StageSlot;

typedef struct M547StageModel_s {
    s16 id;
    s16 pad_02;
    f32 x;
    f32 y;
    f32 z;
    f32 rotX;
    f32 rotY;
    f32 rotZ;
} M547StageModel;

typedef struct M547Vec3_s {
    f32 x;
    f32 y;
    f32 z;
} M547Vec3;

/* one 0x30 block of animation state per scripted prop (see stage.c) */
typedef struct M547AnimBlock_s {
    s32 hold;
    s32 timer;
    s32 timer2;
    s32 state;
    u8 pad_10[0x10];
    f32 f20;
    f32 f24;
    f32 f28;
    u8 pad_2C[4];
} M547AnimBlock;

/* module state block (lbl_1_bss_488, 0x210 bytes); stage.c and game.c use the same layout */
typedef struct M547State_s {
    OMOBJMAN *objman;
    M547CharSlot chars[2];
    union {
        M547StageModel *models;
        s16 (*modelIds)[14];
    };
    u32 *motionData[2];
    s16 *motionIds[2];
    M547StageSlot slots[2];
    s32 *dataNums;
    s16 field9C;
    u8 pad_9E[2];
    s32 stateA0;
    s32 stateA4;
    s32 fieldA8;
    s32 fieldAC;
    s32 counters[4];
    M547Vec3 pos[3];
    u8 pad_E4[0xC];
    M547AnimBlock anim[5];
    u8 pad_1E0[0x30];
} M547State;

void fn_1_4094(void);
void fn_1_41B0(void);
void fn_1_43A0(void);
void fn_1_47E0(void);
void fn_1_4A94(void);
void fn_1_5894(void);
void fn_1_5DC4(void);
void fn_1_6FF4(void);
void fn_1_750C(void);
void fn_1_7724(void);
void fn_1_7A08(void);
void fn_1_7DAC(void);
void fn_1_7F2C(void);
void fn_1_8298(void);
void fn_1_86B8(M547State *state, M547StageModel *models, s32 *dataNums);
void fn_1_8904(s16 arg0);
void fn_1_8B9C(u32 *arg0, u32 *arg1);
void fn_1_8BC0(s16 *arg0, s16 *arg1);
void fn_1_8BE4(void);
void fn_1_8D2C(void);
void fn_1_91E8(void);
void fn_1_933C(void);
void fn_1_954C(void);

void fn_1_17C(s16 mode);
void fn_1_A84(void);
void fn_1_ACC(s16 mode);
void fn_1_B24(s16 mode, s16 frame);
void fn_1_C70(s16 mode);
void fn_1_CF0(void);
void fn_1_D7C(void);
void fn_1_DA8(void);
void fn_1_DE4(void);
void fn_1_E88(void);
void fn_1_10D0(void);
void fn_1_1544(void);
void fn_1_162C(void);
void fn_1_1BA8(void);
void fn_1_21BC(s32 idx, f32 alpha);
void fn_1_223C(s16 arg0);
void fn_1_231C(void);
void fn_1_23A8(void);
void fn_1_243C(s32 idx, s16 level);
void fn_1_2648(s32 idx);
void fn_1_2678(void);
void fn_1_2CE4(void);
void fn_1_376C(void);
void fn_1_3ADC(void);
void ObjectSetup(void);

/* game.c / DOL data */
extern f32 lbl_1_data_280[3];
extern f32 lbl_1_data_28C[3];
extern f32 lbl_1_data_298[3];
extern s32 lbl_1_data_2A4[2];
extern s32 lbl_1_bss_6D4[5];

/* unit-owned .bss, declared in reverse address order so the offsets match the original */
M547State lbl_1_bss_488;
OMOBJ *lbl_1_bss_484;
s16 lbl_1_bss_480[2];
s16 lbl_1_bss_470[8];
f32 lbl_1_bss_464[3];
f32 lbl_1_bss_458[3];
M547Vec3 lbl_1_bss_440[2];
s32 lbl_1_bss_430[4];
M547StageModel lbl_1_bss_E8[30];
OMCAMERAVIEW lbl_1_bss_78[4];
s16 lbl_1_bss_5C[14];
M547Vec3 lbl_1_bss_38[3];
s32 lbl_1_bss_34;
s32 lbl_1_bss_24[4];
s16 lbl_1_bss_20;
MGSCORE *lbl_1_bss_1C;
MGSCORE *lbl_1_bss_18;
s32 lbl_1_bss_14;
s32 lbl_1_bss_10;
s32 lbl_1_bss_C;
s32 lbl_1_bss_8;
s32 lbl_1_bss_4;
s16 lbl_1_bss_2;
s16 lbl_1_bss_0;

/* unit-owned .data, in original address order */
s32 lbl_1_data_0[13] = {
    0x690000, 0x690001, 0x690002, 0x690003, 0x690004, 0x690005, 0x690006,
    0x690007, 0x690008, 0x690009, 0x69000A, 0x69000B, 0x69000C,
};
s32 lbl_1_data_34[11] = {
    0x69000D, 0x69000E, 0x69000F, 0x690010, 0x690011, 0x690013,
    0x690014, 0x690015, 0x690016, 0x690017, 0x690018,
};
s32 lbl_1_data_60 = 0x690012;
u32 lbl_1_data_64[13][7] = {
    { 0x8C0000, 0x8C0003, 0x8C0004, 0x8C004E, 0x8C002B, 0x8C004F, 0x8C0007 },
    { 0x330000, 0x330003, 0x330004, 0x33004E, 0x33002B, 0x33004F, 0x330007 },
    { 0xB70000, 0xB70003, 0xB70004, 0xB7004E, 0xB7002B, 0xB7004F, 0xB70007 },
    { 0xED0000, 0xED0003, 0xED0004, 0xED004E, 0xED002B, 0xED004F, 0xED0007 },
    { 0xE60000, 0xE60003, 0xE60004, 0xE6004E, 0xE6002B, 0xE6004F, 0xE60007 },
    { 0x170000, 0x170003, 0x170004, 0x17004E, 0x17002B, 0x17004F, 0x170007 },
    { 0xE00000, 0xE00003, 0xE00004, 0xE0004E, 0xE0002B, 0xE0004F, 0xE00007 },
    { 0x8C0000, 0x8C0003, 0x8C0004, 0x8C004E, 0x8C002B, 0x8C004F, 0x8C0007 },
    { 0x8C0000, 0x8C0003, 0x8C0004, 0x8C004E, 0x8C002B, 0x8C004F, 0x8C0007 },
    { 0x8C0000, 0x8C0003, 0x8C0004, 0x8C004E, 0x8C002B, 0x8C004F, 0x8C0007 },
    { 0x8C0000, 0x8C0003, 0x8C0004, 0x8C004E, 0x8C002B, 0x8C004F, 0x8C0007 },
    { 0x8C0000, 0x8C0003, 0x8C0004, 0x8C004E, 0x8C002B, 0x8C004F, 0x8C0007 },
    { 0x8C0000, 0x8C0003, 0x8C0004, 0x8C004E, 0x8C002B, 0x8C004F, 0x8C0007 },
};
MGSEQ_PARAM lbl_1_data_1D0 = {
    0, 0,
    (MGSEQ_FUNC)fn_1_17C, (MGSEQ_FUNC)fn_1_A84, (MGSEQ_FUNC)fn_1_ACC,
    (MGSEQ_FUNC)fn_1_B24, (MGSEQ_FUNC)fn_1_C70, (MGSEQ_FUNC)fn_1_CF0,
    (MGSEQ_FUNC)fn_1_D7C, (MGSEQ_FUNC)fn_1_DA8, (MGSEQ_FUNC)fn_1_DE4,
};
void (*lbl_1_data_1F8[2])(void) = { fn_1_41B0, fn_1_43A0 };
void (*lbl_1_data_200[5])(void) = { fn_1_47E0, fn_1_4A94, fn_1_5894, fn_1_5DC4, fn_1_6FF4 };
struct {
    void (*fn[3])(void);
    f32 one;
} lbl_1_data_214 = { { fn_1_7A08, fn_1_7DAC, fn_1_7F2C }, 1.0f };

#define M547 lbl_1_bss_488
/* model id of the 0x1C-byte model entry at byte offset off, indexed by row */
#define MID(row, off) (M547.modelIds[row][(off) / 2])

int _prolog(void)
{
    const M547VoidFunc *ctors = _ctors;

    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const M547VoidFunc *dtors = _dtors;

    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

#ifdef TARGET_PC
/* game.c writes work[0] of the view object; the GC build pokes the raw offset 0x4C */
void M547ObjWork0Set(s32 value)
{
    lbl_1_bss_484->work[0] = value;
}
#endif

void ObjectSetup(void)
{
    fn_1_86B8(&M547, lbl_1_bss_E8, lbl_1_data_0);
    M547.objman = omInitObjMan(0x32, 0x2000);
    omGameSysInit(M547.objman);
    lbl_1_bss_484 = omAddObjEx(M547.objman, 0x7FDA, 0, 0, -1, omOutViewMulti);
    lbl_1_bss_484->work[0] = 2;
    omAddObjEx(M547.objman, 0x7FDA, 0, 0, -1, (OMOBJFUNC)fn_1_8D2C);
    MgSeqCreate(&lbl_1_data_1D0);
}

void fn_1_17C(s16 mode)
{
    HuVecF pos;
    HuVecF up;
    HuVecF target;
    s32 i;
    HU3DLIGHTID light;

    fn_1_8904(1);
    fn_1_8B9C(lbl_1_data_64[M547.chars[0].charNo], lbl_1_data_64[M547.chars[1].charNo]);
    fn_1_8BC0(lbl_1_bss_5C, lbl_1_bss_5C + 7);
    fn_1_8BE4();
    fn_1_2CE4();
    fn_1_376C();
    Hu3DModelPosSet(M547.models[13].id, -150.0f, 800.0f, 0.0f);
    Hu3DModelPosSet(M547.models[14].id, 150.0f, 800.0f, 0.0f);
    M547.pos[0].x = -150.0f;
    M547.pos[0].y = 800.0f;
    M547.pos[0].z = 0.0f;
    M547.pos[1].x = 150.0f;
    M547.pos[1].y = 800.0f;
    M547.pos[1].z = 0.0f;
    fn_1_91E8();
    omAddObjEx(M547.objman, 0x7FDA, 0, 0, -1, (OMOBJFUNC)fn_1_933C);
    CenterM[0].x = 0.0f;
    CenterM[0].y = 275.0f;
    CenterM[0].z = 0.0f;
    CRotM[0].x = -40.0f;
    CRotM[0].y = 0.0f;
    CRotM[0].z = 0.0f;
    CZoomM[0] = 1000.0f;
    lbl_1_bss_78[0].center.x = 0.0f;
    lbl_1_bss_78[0].center.y = 275.0f;
    lbl_1_bss_78[0].center.z = 0.0f;
    lbl_1_bss_78[0].rot.x = -20.0f;
    lbl_1_bss_78[0].rot.y = 0.0f;
    lbl_1_bss_78[0].rot.z = 0.0f;
    lbl_1_bss_78[0].zoom = 600.0f;
    CenterM[1].x = 0.0f;
    CenterM[1].y = 100.0f;
    CenterM[1].z = 0.0f;
    CRotM[1].x = 0.0f;
    CRotM[1].y = 0.0f;
    CRotM[1].z = 0.0f;
    CZoomM[1] = 500.0f;
    Hu3DCameraCreate(3);
    Hu3DCameraPerspectiveSet(1, 45.0f, 20.0f, 50000.0f, 1.2f);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(1, 0, 0, 640, 480);
    Hu3DCameraPerspectiveSet(2, 45.0f, 20.0f, 30000.0f, 1.2f);
    Hu3DCameraViewportSet(2, 360.0f, 256.0f, 240.0f, 180.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(2, 320, 240, 0, 0);
    light = Hu3DGLightCreate(0.0f, 1000.0f, 1000.0f, 0.0f, -1.0f, -1.0f, 255, 255, 255);
    Hu3DGLightInfinitytSet(light);
    Hu3DGLightStaticSet(light, 1);
    omAddObjEx(M547.objman, 0x7FDA, 0, 0, -1, (OMOBJFUNC)fn_1_954C);
    omAddObjEx(M547.objman, 0x7FDA, 0, 0, -1, (OMOBJFUNC)fn_1_E88);
    omAddObjEx(M547.objman, 0x7FDA, 0, 0, -1, (OMOBJFUNC)fn_1_10D0);
    omAddObjEx(M547.objman, 0x7FDA, 0, 0, -1, (OMOBJFUNC)fn_1_1544);
    omAddObjEx(M547.objman, 0x7FDA, 0, 0, -1, (OMOBJFUNC)fn_1_2678);
    omAddObjEx(M547.objman, 0x7FDA, 0, 0, -1, (OMOBJFUNC)fn_1_3ADC);
    Hu3DBGColorSet(0xBA, 0xF5, 0xFF);
    Hu3DShadowCreate(45.0f, 20.0f, 15000.0f);
    pos.x = 0.0f;
    pos.y = 1500.0f;
    pos.z = 250.0f;
    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;
    target.x = 0.0f;
    target.y = 100.0f;
    target.z = 0.0f;
    Hu3DShadowTPLvlSet(0.25f);
    Hu3DModelShadowSet(M547.models[13].id);
    Hu3DModelShadowSet(M547.models[14].id);
    Hu3DModelShadowMapSet(M547.models[0].id);
    Hu3DModelShadowMapSet(M547.models[15].id);
    Hu3DModelShadowMapSet(M547.models[16].id);
    Hu3DShadowPosSet(&pos, &up, &target);
    M547.slots[0].motion = 2;
    M547.slots[0].speed = 0.0f;
    M547.slots[0].time = 0.0f;
    M547.slots[0].attrReset = 0x40000001;
    M547.slots[1].motion = 2;
    M547.slots[1].speed = 0.0f;
    M547.slots[1].time = 0.0f;
    M547.slots[1].attrReset = 0x40000001;
    lbl_1_bss_34 = GWMgRecordGet(7);
    for (i = 0; i < 4; i++) {
        lbl_1_bss_430[i] = 0;
        lbl_1_bss_24[i] = 0;
    }
    lbl_1_bss_430[2] = rand8() % 50 + 100;
    Hu3DZClearLayerSet(14);
    MgSeqModeDelaySet(mode + 1, -1);
    MgSeqModeNext();
    fn_1_162C();
}

void fn_1_A84(void)
{
    lbl_1_data_1F8[M547.stateA0]();
    fn_1_4094();
}

void fn_1_ACC(s16 mode)
{
    M547.stateA0 = 0;
    M547.stateA4 = 0;
    MgSeqModeDelaySet(mode + 1, -1);
    fn_1_4094();
}

void fn_1_B24(s16 mode, s16 frame)
{
    lbl_1_data_200[M547.stateA0]();
    if (lbl_1_bss_2 >= 40) {
        if ((frame / 16) % 2 != 0) {
            espPosSet(lbl_1_bss_470[6], 350.0f, 416.0f);
        } else {
            espPosSet(lbl_1_bss_470[6], -100.0f, 0.0f);
        }
    }
    if (lbl_1_bss_2 < 100) {
        fn_1_162C();
    }
    if (lbl_1_bss_2 > 100) {
        fn_1_1BA8();
        espPosSet(lbl_1_bss_470[6], -100.0f, 0.0f);
    }
    fn_1_231C();
    if (lbl_1_bss_2 != 0) {
        fn_1_23A8();
    }
    fn_1_4094();
}

void fn_1_C70(s16 mode)
{
    MgSeqModeDelaySet(mode + 1, -1);
    fn_1_7724();
    if (lbl_1_bss_2 > 100) {
        fn_1_1BA8();
        espPosSet(lbl_1_bss_470[6], -100.0f, 0.0f);
    }
    fn_1_23A8();
    fn_1_4094();
}

void fn_1_CF0(void)
{
    lbl_1_data_214.fn[M547.stateA0]();
    if (lbl_1_bss_2 > 100) {
        fn_1_1BA8();
        espPosSet(lbl_1_bss_470[6], -100.0f, 0.0f);
    }
    fn_1_23A8();
    fn_1_4094();
}

void fn_1_D7C(void)
{
    fn_1_750C();
    fn_1_23A8();
    fn_1_4094();
    MgSeqModeNext();
}

void fn_1_DA8(void)
{
    fn_1_750C();
    fn_1_23A8();
    fn_1_4094();
    if (WipeCheckEnd() == 0) {
        MgSeqModeNext();
    }
}

void fn_1_DE4(void)
{
}

void fn_1_DE8(void)
{
    print8(8, 0x78, 1.5f, "A BUTTON");
    print8(8, 0x84, 1.5f, "1P=%04d", M547.counters[1]);
    print8(8, 0x90, 1.5f, "2P=%04d", M547.counters[2]);
}

void fn_1_E88(void)
{
    f32 yoff[13] = { 102.0f, 102.0f, 114.0f, 98.0f, 102.0f, 114.0f, 90.0f, 100.0f,
                     102.0f, 102.0f, 102.0f, 102.0f, 102.0f };
    s32 i;

    for (i = 0; i < 2; i++) {
        Hu3DModelPosSet(MID(i, 0x16C), M547.pos[i].x, M547.pos[i].y, M547.pos[i].z);
        if (M547.counters[0] != 0 && MgSeqModeGet() == 5) {
            if (M547.chars[i].cpuLevel != 0) {
                fn_1_243C(i, M547.chars[i].cpuLevel - 1);
            }
            if ((M547.chars[i].btnDown & 0x100) != 0 && M547.counters[i + 1] < 200) {
                M547.counters[i + 1]++;
            }
        }
        if (lbl_1_data_2A4[0] == i) {
            Hu3DModelPosSet(M547.models[22].id, M547.pos[i].x, M547.pos[i].y + yoff[M547.chars[i].charNo],
                            M547.pos[i].z);
        }
    }
}

void fn_1_10D0(void)
{
    f32 f31;
    s32 i;

    if (MgSeqModeGet() == 5) {
        if (M547.stateA0 == 2) {
            for (i = 0; i < 2; i++) {
                Hu3DModelPosSet(MID(i, 0x1F8), M547.pos[i].x, M547.pos[i].y, M547.pos[i].z);
                f31 = 1.0f;
                if (M547.pos[i].y >= 7500.0f) {
                    f31 = 1.0 - (M547.pos[i].y - 7500.0f) / 2500.0f;
                }
                if (M547.pos[i].y == 10000.0f) {
                    f31 = 0.0f;
                }
                if (f31 > 1.0f) {
                    f31 = 1.0f;
                }
                if (f31 < 0.0f) {
                    f31 = 0.0f;
                }
                Hu3DModelScaleSet(MID(i, 0x1F8), 0.5 + 0.3f * (1.0 - f31), 0.5 + 0.3f * (1.0 - f31),
                                  0.5 + 0.3f * (1.0 - f31));
                fn_1_21BC(i, f31);
            }
        }
        if (M547.stateA0 == 3) {
            for (i = 0; i < 2; i++) {
                if (i != lbl_1_data_2A4[0] && i != lbl_1_data_2A4[1]) {
                    Hu3DModelPosSet(MID(i, 0x1F8), M547.pos[i + 2].x, M547.pos[i + 2].y,
                                    M547.pos[i + 2].z);
                } else if (i == lbl_1_data_2A4[1] && lbl_1_bss_6D4[0] == 1) {
                    Hu3DModelPosSet(MID(i, 0x1F8), M547.pos[i + 2].x, M547.pos[i].y, M547.pos[i].z);
                    f31 = 1.0f;
                    if (M547.pos[i].y >= 7500.0f) {
                        f31 = 1.0 - (M547.pos[i].y - 7500.0f) / 2500.0f;
                    }
                    if (M547.pos[i].y > 10000.0f) {
                        f31 = 0.0f;
                    }
                    fn_1_21BC(i, f31);
                }
            }
        }
    }
}

void fn_1_1544(void)
{
    f32 f31;
    s32 id;
    s32 i;

    for (i = 0; i < 2; i++) {
        id = MID(i, 0x1A4);
        if (M547.pos[i].y >= 200.0f) {
            Hu3DMotionTimeSet(id, 0.0f);
        } else {
            f31 = 200.0f - M547.pos[i].y;
            Hu3DMotionTimeSet(id, 2.0f * f31);
        }
    }
}

void fn_1_162C(void)
{
    f32 f31;
    f32 f30;
    f32 f29;
    f32 f28;
    s16 a;
    s32 j;
    s16 sp;

    fn_1_223C(lbl_1_bss_2);
    if (lbl_1_bss_2 > 10) {
        a = 10;
    } else {
        a = lbl_1_bss_2;
    }
    f31 = (a * 240) / 10 + 1;
    f29 = (1.5f * a) / 10.0f;
    f29 = (576.0f * f29) / 640.0f;
    if (lbl_1_bss_2 < 10) {
        a = 0;
    } else if (lbl_1_bss_2 > 20) {
        a = 10;
    } else {
        a = lbl_1_bss_2 - 10;
    }
    f30 = (a * 180) / 10 + 3;
    f28 = (1.5 * a) / 10.0;
    if (lbl_1_bss_2 == 0) {
        f31 = f30 = f29 = f28 = 0.0f;
    }
    espPosSet(lbl_1_bss_470[0], 0.9f * (480.0f + 0.5f * f31), 346.0f);
    espPosSet(lbl_1_bss_470[1], 0.9f * (480.0f - 0.5f * f31), 346.0f);
    espPosSet(lbl_1_bss_470[2], 432.0f, 346.0f + 0.5f * f30);
    espPosSet(lbl_1_bss_470[3], 432.0f, 346.0f - 0.5f * f30);
    for (j = 0; j < 4; j++) {
        if (j < 2) {
            espScaleSet(lbl_1_bss_470[j], 1.0f, f30 / 480.0f);
        } else {
            espScaleSet(lbl_1_bss_470[j], f31 / 640.0f, 1.0f);
        }
    }
    if (lbl_1_bss_2 % 2 != 0) {
        sp = 5;
    } else {
        sp = 4;
    }
    espPosSet(lbl_1_bss_470[sp], 432.0f, 346.0f);
    espScaleSet(lbl_1_bss_470[sp], f29, f28);
    espScaleSet(lbl_1_bss_470[sp ^ 1], 0.0f, 0.0f);
    if (lbl_1_bss_2 >= 30 && lbl_1_bss_2 <= 90) {
        f31 = 1.0f - (f32)(lbl_1_bss_2 - 30) / 60.0f;
        if (lbl_1_bss_2 > 90) {
            f31 = 0.0f;
        }
        espTPLvlSet(lbl_1_bss_470[sp], f31);
    }
}

void fn_1_1BA8(void)
{
    s16 t;
    s32 j;
    u32 x;
    u32 y;
    u32 w;
    u32 h;

    t = lbl_1_bss_2 - 101;
    if (t > 60) {
        t = 60;
    }
    w = (t * 400) / 60 + 240;
    h = (t * 300) / 60 + 180;
    x = 360 - (t * 360) / 60;
    y = 256 - ((t << 8) / 60);
    Hu3DCameraScissorSet(2, x, y, w, h);
    Hu3DCameraViewportSet(2, x, y, w, h, 0.0f, 1.0f);
    espPosSet(lbl_1_bss_470[0], 0.9f * (f32)(x + w), (f32)(y + (h >> 1)));
    espPosSet(lbl_1_bss_470[1], 0.9f * (f32)x, (f32)(y + (h >> 1)));
    espPosSet(lbl_1_bss_470[2], 0.9f * (f32)(x + (w >> 1)), (f32)(y + h));
    espPosSet(lbl_1_bss_470[3], 0.9f * (f32)(x + (w >> 1)), (f32)y);
    lbl_1_data_280[0] = (t * (120 - lbl_1_data_2A4[1] * 240)) / 60;
    lbl_1_data_298[0] = (t * (120 - lbl_1_data_2A4[1] * 240)) / 60;
    lbl_1_data_280[1] = 200.0f + (((M547.pos[lbl_1_data_2A4[0]].y - 100.0f) * (f32)t) / 60.0f);
    lbl_1_data_298[1] = 200.0f + (((M547.pos[lbl_1_data_2A4[0]].y - 100.0f) * (f32)t) / 60.0f);
    Hu3DCameraPosSetV(2, (Vec *)lbl_1_data_280, (Vec *)lbl_1_data_28C, (Vec *)lbl_1_data_298);
    for (j = 0; j < 4; j++) {
        if (j < 2) {
            espScaleSet(lbl_1_bss_470[j], 1.0f, (f32)h / 480.0f);
        } else {
            espScaleSet(lbl_1_bss_470[j], (f32)w / 640.0f, 1.0f);
        }
    }
}

void fn_1_21BC(s32 idx, f32 alpha)
{
    HU3DMODEL *model;
    HSFMATERIAL *mat;
    s32 i;
    HSFDATA *hsf;

    model = &Hu3DData[MID(idx, 0x1F8)];
    hsf = model->hsf;
    mat = hsf->material;
    for (i = 0; i < model->hsf->materialNum; i++, mat++) {
        mat->invAlpha = alpha;
    }
}

void fn_1_223C(s16 arg0)
{
    u32 w;
    u32 h;
    u32 x;
    u32 y;

    if (arg0 < 10) {
        w = (arg0 * 240) / 10;
        x = 480 - (w >> 1);
        y = 345;
        h = 2;
    } else if (arg0 < 20) {
        x = 360;
        w = 240;
        h = ((arg0 - 10) * 180) / 10 + 2;
        y = 346 - (h >> 1);
    } else {
        x = 360;
        w = 240;
        y = 256;
        h = 180;
    }
    Hu3DCameraScissorSet(2, x, y, w, h);
}

void fn_1_231C(void)
{
    Hu3DModelPosSet(M547.models[20].id, lbl_1_bss_464[0], lbl_1_bss_464[1], lbl_1_bss_464[2]);
    Hu3DModelRotSet(M547.models[20].id, lbl_1_bss_458[0], lbl_1_bss_458[1], lbl_1_bss_458[2]);
}

void fn_1_23A8(void)
{
    s32 i;

    fn_1_8298();
    for (i = 0; i < 2; i++) {
        Hu3DModelPosSet(MID(i, 0x2F4), lbl_1_bss_440[i].x, lbl_1_bss_440[i].y, lbl_1_bss_440[i].z);
    }
}

/* per-player button-mash pacing for the CPU */
s32 lbl_1_data_240[2] = { 0, 0 };
struct {
    s32 interval;
    s32 count;
} lbl_1_data_248[2] = { { 0, 0 }, { 0, 0 } };

void fn_1_243C(s32 idx, s16 level)
{
    s32 base[4] = { 40, 50, 85, 110 };
    s32 rnd;

    if (lbl_1_data_240[idx] == 0) {
        if (level == 3) {
            rnd = rand8() % 15;
            lbl_1_data_240[idx] = rnd + base[level];
        } else {
            rnd = rand8() % 20;
            lbl_1_data_240[idx] = rnd + base[level];
        }
        lbl_1_data_248[idx].interval = 600 / lbl_1_data_240[idx];
    }
    lbl_1_data_248[idx].count += 1;
    if (lbl_1_data_248[idx].count >= lbl_1_data_248[idx].interval) {
        lbl_1_data_248[idx].count = 0;
        if (M547.counters[idx + 1] < lbl_1_data_240[idx]) {
            M547.chars[idx].btnDown = 0x100;
        }
    }
}

void fn_1_2648(s32 idx)
{
    M547.counters[idx + 1] = lbl_1_data_240[idx];
}

void fn_1_2678(void)
{
    switch (lbl_1_bss_24[0]) {
    case 0:
        lbl_1_bss_20 = MgScoreWinCreate(0x8C, 0x30);
        MgScoreWinPosSet(lbl_1_bss_20, 288.0f, 68.0f);
        MgScoreWinTPLvlSet(lbl_1_bss_20, 0.7f);
        MgScoreWinColorSet(lbl_1_bss_20, 0, 0, 0);
        lbl_1_bss_1C = MgScoreCreate(0x960030, 0x960035, 0);
        MgScoreMaxDigitSet(lbl_1_bss_1C, 3);
        MgScoreValueSet(lbl_1_bss_1C, lbl_1_bss_34);
        MgScoreColorSet(lbl_1_bss_1C, 0x42, 0xFF, 0x7A);
        MgScorePosSet(lbl_1_bss_1C, 259.0f, 60.0f);
        espPosSet(lbl_1_bss_470[7], 238.0f, 60.0f);
        lbl_1_bss_18 = MgScoreCreate(0x960030, 0x960035, 0);
        MgScoreMaxDigitSet(lbl_1_bss_18, 3);
        MgScoreValueSet(lbl_1_bss_18, lbl_1_bss_24[1]);
        MgScoreColorSet(lbl_1_bss_18, 0xFF, 0xFF, 0xFF);
        MgScorePosSet(lbl_1_bss_18, 259.0f, 76.0f);
        lbl_1_bss_24[1] = lbl_1_bss_24[2] = 0;
        lbl_1_bss_24[0] = 99;
        break;
    case 1:
        if (lbl_1_bss_24[3] == 0) {
            lbl_1_bss_24[1] += 1;
        }
        MgScoreValueSet(lbl_1_bss_18, lbl_1_bss_24[1]);
        break;
    case 2:
        MgScoreValueSet(lbl_1_bss_1C, lbl_1_bss_24[1]);
        MgScoreValueSet(lbl_1_bss_18, lbl_1_bss_24[1]);
        lbl_1_bss_24[0] = 3;
        lbl_1_bss_24[2] = 0;
        lbl_1_bss_24[3] = 0;
        break;
    case 3:
        if (lbl_1_bss_24[2] == 0) {
            lbl_1_bss_24[3] += 1;
            MgScoreDigitScaleSet(lbl_1_bss_1C, 1.0f + 0.02f * lbl_1_bss_24[3], 1.0f + 0.02f * lbl_1_bss_24[3]);
            espScaleSet(lbl_1_bss_470[7], 1.0f + 0.02f * lbl_1_bss_24[3], 1.0f + 0.02f * lbl_1_bss_24[3]);
            if (lbl_1_bss_24[3] == 10) {
                lbl_1_bss_24[2] = 1;
                lbl_1_bss_24[3] = 0;
            }
        } else {
            lbl_1_bss_24[3] += 1;
            MgScoreDigitScaleSet(lbl_1_bss_1C, 1.2f - 0.04f * lbl_1_bss_24[3], 1.2f - 0.04f * lbl_1_bss_24[3]);
            espScaleSet(lbl_1_bss_470[7], 1.2f - 0.04f * lbl_1_bss_24[3], 1.2f - 0.04f * lbl_1_bss_24[3]);
            if (lbl_1_bss_24[3] == 5) {
                lbl_1_bss_24[2] = 0;
                lbl_1_bss_24[3] = 0;
            }
        }
        break;
    }
}

void fn_1_2CE4(void)
{
    s16 id;
    s32 i;
    HSFMATERIAL *mat;
    HU3DMODEL *model;
    s32 j;
    HSFDATA *hsf;

    for (i = 0; i < 2; i++) {
        id = MID(i, 0x1A4) = Hu3DModelCreateData(lbl_1_data_34[0]);
        Hu3DModelAttrSet(id, 1);
        Hu3DModelPosSet(id, -150.0f + 300.0f * i, 100.0f, 5.0f);
        Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
        Hu3DModelCameraSet(id, 1);
        Hu3DMotionSpeedSet(id, 0.0f);
        Hu3DModelAttrReset(id, 1);
    }
    id = M547.models[17].id = Hu3DModelCreateData(lbl_1_data_34[1]);
    Hu3DModelAttrSet(id, 1);
    Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(id, 2);
    Hu3DModelAttrReset(id, 1);
    lbl_1_bss_480[0] = Hu3DTexScrollCreate(id, "m547sky01");
    for (i = 0; i < 2; i++) {
        id = MID(i, 0x1F8) = Hu3DModelCreateData(lbl_1_data_34[i + 2]);
        Hu3DModelAttrSet(id, 1);
        Hu3DModelPosSet(id, -150.0f + 300.0f * i, 250.0f, 0.0f);
        Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
        Hu3DModelScaleSet(id, 0.5f, 0.5f, 0.5f);
        Hu3DModelCameraSet(id, 1);
        Hu3DModelLayerSet(id, 7);
        Hu3DModelAttrReset(id, 1);
        model = &Hu3DData[MID(i, 0x1F8)];
        hsf = model->hsf;
        mat = hsf->material;
        for (j = 0; j < model->hsf->materialNum; j++, mat++) {
            mat->invAlpha = 1.0f;
        }
    }
    for (i = 0; i < 2; i++) {
        id = MID(i, 0x230) = Hu3DModelCreateData(lbl_1_data_34[i + 4]);
        Hu3DModelAttrSet(id, 1);
        Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
        Hu3DModelCameraSet(id, 1);
        if (i != 0) {
            Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
            Hu3DModelHookSet(M547.models[20].id, "itemhook_sao", id);
        } else {
            lbl_1_bss_464[0] = 2000.0f;
            Hu3DModelPosSet(id, 2000.0f, 0.0f, 0.0f);
            lbl_1_bss_0 = Hu3DJointMotionData(id, lbl_1_data_60);
            Hu3DMotionSet(id, lbl_1_bss_0);
            Hu3DMotionSpeedSet(id, 1.0f);
            Hu3DModelAttrSet(id, 0x40000001);
        }
        Hu3DModelAttrReset(id, 1);
    }
    id = M547.models[22].id = Hu3DModelCreateData(lbl_1_data_34[6]);
    Hu3DModelAttrSet(id, 1);
    Hu3DModelPosSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(id, 2);
    Hu3DMotionSpeedSet(id, 0.0f);
    Hu3DMotionTimeSet(id, 0.0f);
    Hu3DModelAttrReset(id, 0x40000001);
    lbl_1_bss_480[1] = Hu3DTexScrollCreate(id, "m547para01");
    for (i = 0; i < 2; i++) {
        if (i == 0) {
            id = M547.models[23].id = Hu3DModelCreateData(lbl_1_data_34[7]);
        } else {
            id = M547.models[24].id = Hu3DModelLink(M547.models[23].id);
        }
        Hu3DModelAttrSet(id, 1);
        Hu3DModelPosSet(id, -130.0f + 260.0f * i, 180.0f, 85.0f);
        Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
        Hu3DModelCameraSet(id, 1);
        Hu3DMotionSpeedSet(id, 0.0f);
        Hu3DModelAttrReset(id, 0x40000001);
        Hu3DModelScaleSet(id, 1.9f, 1.6f, 1.5f);
    }
    for (i = 0; i < 2; i++) {
        id = MID(i, 0x2BC) = Hu3DModelCreateData(lbl_1_data_34[i + 8]);
        Hu3DModelAttrSet(id, 1);
        if (i != 0) {
            Hu3DModelPosSet(id, -4000.0f, 18000.0f, 3000.0f);
        } else {
            Hu3DModelPosSet(id, -5000.0f, 19000.0f, 4000.0f);
        }
        Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
        Hu3DModelCameraSet(id, 1);
        Hu3DModelLayerSet(id, 7);
        Hu3DModelAttrReset(id, 1);
    }
    for (i = 0; i < 2; i++) {
        if (i == 0) {
            id = M547.models[27].id = Hu3DModelCreateData(lbl_1_data_34[10]);
        } else {
            id = M547.models[28].id = Hu3DModelLink(M547.models[27].id);
        }
        Hu3DModelAttrSet(id, 1);
        Hu3DModelPosSet(id, 0.0f, 0.0f, -100.0f);
        lbl_1_bss_440[i].x = i * 200 - 100;
        lbl_1_bss_440[i].y = i * 100 - 50;
        lbl_1_bss_440[i].z = -100.0f;
        Hu3DModelRotSet(id, 0.0f, 0.0f, 0.0f);
        Hu3DModelCameraSet(id, 2);
        Hu3DModelLayerSet(id, 3);
    }
}

void fn_1_376C(void)
{
    s32 i;

    for (i = 0; i < 4; i++) {
        if (i < 2) {
            lbl_1_bss_470[i] = espEntry(0x690019, 0xD2, 0);
        } else {
            lbl_1_bss_470[i] = espEntry(0x69001A, 0xD2, 0);
        }
        espDispOff(lbl_1_bss_470[i]);
        espDrawNoSet(lbl_1_bss_470[i], 0);
        espPosSet(lbl_1_bss_470[i], 288.0f, 240.0f);
        espAttrSet(lbl_1_bss_470[i], 1);
        espDispOn(lbl_1_bss_470[i]);
    }
    for (i = 0; i < 2; i++) {
        if (i != 0) {
            lbl_1_bss_470[i + 4] = espEntry(0x69001C, 0xD7, 0);
        } else {
            lbl_1_bss_470[i + 4] = espEntry(0x69001B, 0xD7, 0);
        }
        espDispOff(lbl_1_bss_470[i + 4]);
        espDrawNoSet(lbl_1_bss_470[i + 4], 0);
        espPosSet(lbl_1_bss_470[i + 4], 288.0f, 240.0f);
        espAttrSet(lbl_1_bss_470[i + 4], 1);
        espDispOn(lbl_1_bss_470[i + 4]);
    }
    lbl_1_bss_470[6] = espEntry(0x69001D, 0xF, 0);
    espDispOff(lbl_1_bss_470[6]);
    espDrawNoSet(lbl_1_bss_470[6], 0);
    espPosSet(lbl_1_bss_470[6], -100.0f, 0.0f);
    espAttrSet(lbl_1_bss_470[6], 1);
    espDispOn(lbl_1_bss_470[6]);
    lbl_1_bss_470[7] = espEntry(0x69001E, 0xF, 0);
    espDispOff(lbl_1_bss_470[7]);
    espDrawNoSet(lbl_1_bss_470[7], 0);
    espPosSet(lbl_1_bss_470[7], -100.0f, 0.0f);
    espAttrSet(lbl_1_bss_470[7], 1);
    espDispOn(lbl_1_bss_470[7]);
}

/* fade value of the drifting cloud material */
f32 lbl_1_data_27C = 1.0f;

static inline void M547CloudAlphaSet(f32 alpha)
{
    HU3DMODEL *model;
    HSFDATA *hsf;
    HSFMATERIAL *mat;
    s32 j;

    model = &Hu3DData[M547.models[26].id];
    hsf = model->hsf;
    mat = hsf->material;
    for (j = 0; j < model->hsf->materialNum; j++, mat++) {
        mat->invAlpha = alpha;
    }
}

void fn_1_3ADC(void)
{
    lbl_1_bss_14 = (lbl_1_bss_14 + 1) % 360;
    M547.models[26].rotY = 2.0 * sin(3.141592653589793 * lbl_1_bss_14 / 180.0);
    Hu3DModelRotSet(M547.models[26].id, 0.0f, M547.models[26].rotY, 0.0f);
    switch (lbl_1_bss_8) {
    case 0:
        M547CloudAlphaSet(lbl_1_data_27C);
        lbl_1_bss_8 += 1;
        break;
    case 1:
        if (MgSeqModeGet() == 5 && M547.stateA0 == 2) {
            if (lbl_1_bss_4++ > 30) {
                lbl_1_bss_8 += 1;
                lbl_1_bss_4 = 0;
            }
        }
        break;
    case 2:
        lbl_1_bss_4 += 1;
        lbl_1_data_27C = 1.0 - 0.4 * sin(3.141592653589793 * ((lbl_1_bss_4 * 90) / 16) / 180.0);
        M547CloudAlphaSet(lbl_1_data_27C);
        if (lbl_1_bss_4 == 16) {
            lbl_1_bss_4 = 0;
            lbl_1_bss_8 += 1;
            lbl_1_bss_C = rand8() % 60 + 64;
        }
        break;
    case 3:
        if (lbl_1_bss_4++ >= lbl_1_bss_C) {
            M547CloudAlphaSet(1.0f);
            lbl_1_bss_10 = (lbl_1_bss_10 + 1) % 5;
            if (lbl_1_bss_10 == 1 || lbl_1_bss_10 == 4) {
                lbl_1_bss_8 = 2;
            } else {
                lbl_1_bss_8 += 1;
            }
            lbl_1_bss_4 = 0;
        }
        break;
    case 4:
        if (lbl_1_bss_4++ >= 6) {
            M547CloudAlphaSet(0.6f);
            lbl_1_bss_8 = 2;
            lbl_1_bss_4 = 0;
        }
        break;
    }
}
