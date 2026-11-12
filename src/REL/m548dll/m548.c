#include "types.h"

#include "game/gamework.h"
#include "game/object.h"
#include "game/hu3d.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/audio.h"
#include "game/pad.h"
#include "game/printfunc.h"
#include "game/wipe.h"
#include "game/mg/seqman.h"
#include "game/mg/timer.h"
#include "REL/m548dll.h"

s32 rand8(void);
double sin(double x);

typedef void (*M548VoidFunc)(void);
extern const M548VoidFunc _ctors[];
extern const M548VoidFunc _dtors[];

/* game.c */
extern s32 lbl_1_data_398;
void fn_1_1D84(void);
void fn_1_202C(void);
void fn_1_23A4(void);
void fn_1_257C(void);
void fn_1_282C(void);
void fn_1_2AD4(void);
void fn_1_309C(void);
void fn_1_3DCC(void);
void fn_1_4F44(void);
void fn_1_5014(s32 arg0);
void fn_1_54C0(void);
void fn_1_59FC(void);
void fn_1_5D84(void);
void fn_1_5DF4(void);

/* stage.c */
void fn_1_6104(M548State *state, M548StageModel *models, s32 *dataNums);
void fn_1_6350(s16 arg0);
void fn_1_65E8(u32 *arg0, u32 *arg1);
void fn_1_660C(s16 *arg0, s16 *arg1);
void fn_1_6630(void);
void fn_1_6890(void);
void fn_1_6D4C(void);
void fn_1_6EA0(void);
void fn_1_70B0(void);

void ObjectSetup(void);
void fn_1_15C(s16 mode);
void fn_1_8C0(void);
void fn_1_934(s16 mode);
void fn_1_9B4(void);
void fn_1_9FC(s16 mode);
void fn_1_A54(void);
void fn_1_A78(void);
void fn_1_A9C(void);
void fn_1_AD0(void);
void fn_1_1204(void);
void fn_1_167C(void);
void fn_1_19A4(void);
void fn_1_1CB4(void);

/* unit-owned .bss, declared in reverse address order so the offsets match the original */
s16 lbl_1_bss_5D2[3];
s16 lbl_1_bss_5D0;
M548State lbl_1_bss_3F0;
M548Game lbl_1_bss_3D0;
M548Fx lbl_1_bss_3B8;
s16 lbl_1_bss_3B4;
M548CryPos lbl_1_bss_384[2];
M548StageModel lbl_1_bss_154[20];
s16 lbl_1_bss_138[14];
OMCAMERAVIEW lbl_1_bss_58[8];
MGTIMER *lbl_1_bss_54;
s16 lbl_1_bss_50;
M548Vec3 lbl_1_bss_44;
M548Vec3 lbl_1_bss_38;
f32 lbl_1_bss_28[4];
s32 lbl_1_bss_20[2];
M548Cry lbl_1_bss_0[2];

/* unit-owned .data, in original address order */
M548CryPos lbl_1_data_0[13] = {
    { 148.03f, 173.65f, 138.69f, 182.5f, 125.0f, 207.0f },
    { 154.95f, 180.53f, 142.9f, 190.31f, 141.0f, 217.0f },
    { 126.02f, 206.1f, 101.54f, 218.15f, 127.0f, 233.0f },
    { 131.71f, 199.45f, 90.45f, 191.7f, 130.0f, 210.0f },
    { 151.55f, 240.86f, 116.39f, 246.55f, 144.0f, 228.0f },
    { 124.06f, 202.75f, 100.98f, 214.31f, 126.0f, 227.0f },
    { 130.47f, 206.43f, 117.83f, 236.28f, 138.5f, 228.2f },
    { 126.35f, 195.32f, 111.63f, 202.39f, 126.0f, 196.0f },
    { 83.79f, 228.14f, 62.16f, 227.45f, 130.0f, 276.0f },
    { 122.57f, 164.46f, 108.48f, 208.13f, 109.0f, 181.0f },
    { 122.57f, 164.46f, 108.48f, 208.13f, 109.0f, 181.0f },
    { 122.57f, 164.46f, 108.48f, 208.13f, 109.0f, 181.0f },
    { 122.57f, 164.46f, 108.48f, 208.13f, 109.0f, 181.0f },
};
s32 lbl_1_data_138[12] = {
    0x6A0000, 0x6A0001, 0x6A0002, 0x6A0003, 0x6A0004, 0x6A0005,
    0x6A0006, 0x6A0007, 0x6A0008, 0x6A0009, 0x6A000A, 0x6A000B,
};
s32 lbl_1_data_168[7] = { 0x6A000C, 0x6A000D, 0x6A000E, 0x6A000E, 0x6A000E, 0x6A000E, 0x6A000E };
M548MotSet lbl_1_data_184[13] = {
    { 0x8C0000, 0x8C0023, 0x8C0022, 0x8C0006, 0x8C0079 },
    { 0x330000, 0x330023, 0x330022, 0x330006, 0x330079 },
    { 0xB70000, 0xB70023, 0xB70022, 0xB70006, 0xB70079 },
    { 0xED0000, 0xED0023, 0xED0022, 0xED0006, 0xED0079 },
    { 0xE60000, 0xE60023, 0xE60022, 0xE60006, 0xE60079 },
    { 0x170000, 0x170023, 0x170022, 0x170006, 0x170079 },
    { 0xE00000, 0xE00023, 0xE00022, 0xE00006, 0xE00079 },
    { 0x2D0000, 0x2D0023, 0x2D0022, 0x2D0006, 0x2D0079 },
    { 0xC90000, 0xC90023, 0xC90022, 0xC90006, 0xC90079 },
    { 0xB00000, 0xB00023, 0xB00022, 0xB00006, 0xB00079 },
    { 0xB00000, 0xB00023, 0xB00022, 0xB00006, 0xB00079 },
    { 0xB00000, 0xB00023, 0xB00022, 0xB00006, 0xB00079 },
    { 0xB00000, 0xB00023, 0xB00022, 0xB00006, 0xB00079 },
};
M548Vec2 lbl_1_data_288[13] = {
    { 0.0f, -0.15f }, { -5.0f, -0.3f }, { 10.0f, -0.25f }, { 0.0f, -0.1f }, { -5.0f, -0.35f },
    { 10.0f, -0.25f }, { -10.0f, -0.5f }, { 0.0f, 0.2f }, { 0.0f, -0.15f }, { 0.0f, -0.15f },
    { 0.0f, -0.15f }, { 0.0f, -0.15f }, { 0.0f, -0.15f },
};
MGSEQ_PARAM lbl_1_data_2F0 = {
    0, 0,
    (MGSEQ_FUNC)fn_1_15C, (MGSEQ_FUNC)fn_1_8C0, (MGSEQ_FUNC)fn_1_934,
    (MGSEQ_FUNC)fn_1_9B4, (MGSEQ_FUNC)fn_1_9FC, (MGSEQ_FUNC)fn_1_A54,
    (MGSEQ_FUNC)fn_1_A78, (MGSEQ_FUNC)fn_1_A9C, (MGSEQ_FUNC)fn_1_AD0,
};
void (*lbl_1_data_318[8])(void) = {
    fn_1_2AD4, fn_1_309C, fn_1_2AD4, fn_1_309C, fn_1_3DCC, fn_1_54C0, fn_1_59FC, fn_1_5D84,
};

#define M548 lbl_1_bss_3F0

int _prolog(void)
{
    const M548VoidFunc *ctors = _ctors;

    while (*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const M548VoidFunc *dtors = _dtors;

    while (*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

void ObjectSetup(void)
{
    fn_1_6104(&M548, lbl_1_bss_154, lbl_1_data_138);
    M548.objman = omInitObjMan(0x32, 0x2000);
    omGameSysInit(M548.objman);
    omAddObjEx(M548.objman, 0x7FDA, 0, 0, -1, (OMOBJFUNC)omOutView);
    omAddObjEx(M548.objman, 0x7FDA, 0, 0, -1, (OMOBJFUNC)fn_1_6890);
    MgSeqCreate(&lbl_1_data_2F0);
}

void fn_1_15C(s16 mode)
{
    HuVecF pos;
    HuVecF up;
    HuVecF target;
    s32 i;
    HU3DLIGHTID light;

    lbl_1_bss_3D0.unk4 = 9;
    fn_1_6350(1);
    fn_1_65E8((u32 *)&lbl_1_data_184[M548.chars[0].charNo], (u32 *)&lbl_1_data_184[M548.chars[1].charNo]);
    fn_1_660C(lbl_1_bss_138, lbl_1_bss_138 + 7);
    fn_1_6630();
    for (i = 0; i < 2; i++) {
        lbl_1_bss_384[i].unk0 = lbl_1_data_0[M548.chars[i].charNo].unk0;
        lbl_1_bss_384[i].unk4 = lbl_1_data_0[M548.chars[i].charNo].unk4;
        lbl_1_bss_384[i].unk8 = lbl_1_data_0[M548.chars[i].charNo].unk8;
        lbl_1_bss_384[i].unkC = lbl_1_data_0[M548.chars[i].charNo].unkC;
        lbl_1_bss_384[i].unk10 = lbl_1_data_0[M548.chars[i].charNo].unk10;
        lbl_1_bss_384[i].unk14 = lbl_1_data_0[M548.chars[i].charNo].unk14;
    }
    fn_1_167C();
    fn_1_6D4C();
    omAddObjEx(M548.objman, 0x7FDA, 0, 0, -1, (OMOBJFUNC)fn_1_6EA0);
    for (i = 0; i < 2; i++) {
        CharMotionUpdateSet(M548.chars[i].charNo, lbl_1_data_184[M548.chars[i].charNo].unk4, 0);
    }
    Center.x = lbl_1_bss_58[0].center.x = -lbl_1_bss_384[0].unk10;
    Center.y = lbl_1_bss_58[0].center.y = lbl_1_bss_384[0].unk14;
    Center.z = lbl_1_bss_58[0].center.z = 0.0f;
    lbl_1_bss_58[0].rot.x = CRot.x = -20.0f;
    lbl_1_bss_58[0].rot.y = CRot.y = 0.0f;
    lbl_1_bss_58[0].rot.z = CRot.z = 0.0f;
    lbl_1_bss_58[0].zoom = CZoom = 200.0f;
    Hu3DCameraCreate(1);
    Hu3DCameraPerspectiveSet(1, 45.0f, 20.0f, 30000.0f, 1.2f);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    light = Hu3DGLightCreate(0.0f, 1000.0f, 1000.0f, 0.0f, -1.0f, -1.0f, 255, 255, 255);
    Hu3DGLightInfinitytSet(light);
    Hu3DGLightStaticSet(light, 1);
    omAddObjEx(M548.objman, 0x7FDA, 0, 0, -1, (OMOBJFUNC)fn_1_1204);
    fn_1_1CB4();
    omAddObjEx(M548.objman, 0x7FDA, 0, 0, -1, (OMOBJFUNC)fn_1_1D84);
    fn_1_4F44();
    Hu3DBGColorSet(0xBA, 0xF5, 0xFF);
    Hu3DShadowCreate(45.0f, 20.0f, 10000.0f);
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
    Hu3DShadowPosSet(&pos, &up, &target);
    Hu3DShadowSizeSet(200);
    Hu3DModelShadowSet(M548.models[12].id);
    Hu3DModelShadowSet(M548.models[13].id);
    Hu3DModelShadowSet(M548.models[14].id);
    M548.slots[0].motion = 1;
    M548.slots[0].speed = 0.0f;
    M548.slots[0].time = 50.0f;
    M548.slots[0].attrReset = 0x40000001;
    M548.slots[1].motion = 0;
    M548.slots[1].speed = 1.0f;
    M548.slots[1].time = 0.0f;
    M548.slots[1].attrSet = 0x40000001;
    omAddObjEx(M548.objman, 0x7FDA, 0, 0, -1, (OMOBJFUNC)fn_1_19A4);
    lbl_1_bss_20[0] = lbl_1_bss_20[1] = 0;
    omAddObjEx(M548.objman, 0x7FDA, 0, 0, -1, (OMOBJFUNC)fn_1_70B0);
    lbl_1_bss_54 = NULL;
    MgSeqModeDelaySet(mode + 1, -1);
    MgSeqModeNext();
}

void fn_1_8C0(void)
{
    if (WipeCheckEnd() == 0) {
        switch (M548.stateA0) {
        case 0:
            fn_1_23A4();
            break;
        case 1:
            fn_1_257C();
            break;
        case 2:
            fn_1_282C();
            break;
        }
    }
    fn_1_202C();
}

void fn_1_934(s16 mode)
{
    M548.stateA0 = 0;
    M548.stateA4 = 0;
    MgSeqModeDelaySet(mode + 1, -1);
    fn_1_202C();
    if (lbl_1_data_398 < 0) {
        lbl_1_data_398 = HuAudBGMPlay(0x3B);
    }
}

void fn_1_9B4(void)
{
    lbl_1_data_318[M548.stateA0]();
    fn_1_202C();
}

void fn_1_9FC(s16 mode)
{
    MgSeqModeDelaySet(mode + 1, -1);
    M548.stateA0 = 0;
    M548.stateA4 = 0;
    fn_1_202C();
}

void fn_1_A54(void)
{
    fn_1_5DF4();
    fn_1_202C();
}

void fn_1_A78(void)
{
    fn_1_202C();
    MgSeqModeNext();
}

void fn_1_A9C(void)
{
    fn_1_202C();
    if (WipeCheckEnd() == 0) {
        MgSeqModeNext();
    }
}

void fn_1_AD0(void)
{
}

/* debug: nudges the "BOM" model offset with the d-pad of pad `pad` */
void fn_1_AD4(s16 pad)
{
    s32 down;
    s8 stkX;
    s8 stkY;
    s8 subX;
    s8 subY;
    s32 absX;
    s32 newX;
    s32 absY;
    s32 newY;
    s32 absSX;
    s32 newSX;
    s32 absSY;
    s32 newSY;
    s32 btn;

    btn = HuPadBtn[pad];
    down = HuPadBtnDown[pad];
    stkX = (s8)HuPadStkX[pad];
    if (stkX >= 0) {
        absX = stkX;
    } else {
        absX = -stkX;
    }
    if (absX < 10) {
        newX = 0;
    } else {
        newX = stkX;
    }
    stkX = newX;
    stkY = (s8)HuPadStkY[pad];
    if (stkY >= 0) {
        absY = stkY;
    } else {
        absY = -stkY;
    }
    if (absY < 10) {
        newY = 0;
    } else {
        newY = stkY;
    }
    stkY = newY;
    subX = (s8)HuPadSubStkX[pad];
    if (subX >= 0) {
        absSX = subX;
    } else {
        absSX = -subX;
    }
    if (absSX < 8) {
        newSX = 0;
    } else {
        newSX = subX;
    }
    subX = newSX;
    subY = (s8)HuPadSubStkY[pad];
    if (subY >= 0) {
        absSY = subY;
    } else {
        absSY = -subY;
    }
    if (absSY < 8) {
        newSY = 0;
    } else {
        newSY = subY;
    }
    subY = newSY;
    if (down & 0x400) {
        lbl_1_bss_44.z += 10.0f;
    }
    if (down & 0x800) {
        lbl_1_bss_44.z -= 10.0f;
    }
    if (down & 8) {
        lbl_1_bss_44.y += 1.0f;
    }
    if (down & 4) {
        lbl_1_bss_44.y -= 1.0f;
    }
    if (down & 1) {
        lbl_1_bss_44.x -= 1.0f;
    }
    if (down & 2) {
        lbl_1_bss_44.x += 1.0f;
    }
}

void fn_1_D6C(void)
{
    HuVecF objPos;
    s32 down;
    s8 subY;
    s8 subX;
    s8 stkY;
    s8 stkX;
    s32 absX;
    s32 newX;
    s32 absY;
    s32 newY;
    s32 absSX;
    s32 newSX;
    s32 absSY;
    s32 newSY;
    s32 btn;

#ifdef TARGET_PC
    objPos.x = objPos.y = objPos.z = 0.0f;
#endif
    if (M548.models[12].id != 0) {
        Hu3DModelObjPosGet(M548.models[12].id, "test11_tex_we-itemhook-r", &objPos);
    }
    print8(8, 0x90, 1.5f, "POStest");
    print8(8, 0x9C, 1.5f, "X=%.2f", objPos.x);
    print8(8, 0xA8, 1.5f, "Y=%.2f", objPos.y);
    print8(8, 0xB4, 1.5f, "Z=%.2f", objPos.z);
    if (M548.models[13].id != 0) {
        Hu3DModelObjPosGet(M548.models[13].id, "test11_tex_we-itemhook-r", &objPos);
    }
    print8(8, 0xC0, 1.5f, "POStest");
    print8(8, 0xCC, 1.5f, "X=%.2f", objPos.x);
    print8(8, 0xD8, 1.5f, "Y=%.2f", objPos.y);
    print8(8, 0xE4, 1.5f, "Z=%.2f", objPos.z);
    btn = HuPadBtn[2];
    down = HuPadBtnDown[2];
    stkX = (s8)HuPadStkX[2];
    if (stkX >= 0) {
        absX = stkX;
    } else {
        absX = -stkX;
    }
    if (absX < 10) {
        newX = 0;
    } else {
        newX = stkX;
    }
    stkX = newX;
    stkY = (s8)HuPadStkY[2];
    if (stkY >= 0) {
        absY = stkY;
    } else {
        absY = -stkY;
    }
    if (absY < 10) {
        newY = 0;
    } else {
        newY = stkY;
    }
    stkY = newY;
    subX = (s8)HuPadSubStkX[2];
    if (subX >= 0) {
        absSX = subX;
    } else {
        absSX = -subX;
    }
    if (absSX < 8) {
        newSX = 0;
    } else {
        newSX = subX;
    }
    subX = newSX;
    subY = (s8)HuPadSubStkY[2];
    if (subY >= 0) {
        absSY = subY;
    } else {
        absSY = -subY;
    }
    if (absSY < 8) {
        newSY = 0;
    } else {
        newSY = subY;
    }
    subY = newSY;
    if (down & 0x400) {
        lbl_1_bss_44.z += 10.0f;
    }
    if (down & 0x800) {
        lbl_1_bss_44.z -= 10.0f;
    }
    if (down & 8) {
        lbl_1_bss_44.y += 1.0f;
    }
    if (down & 4) {
        lbl_1_bss_44.y -= 1.0f;
    }
    if (down & 1) {
        lbl_1_bss_44.x -= 1.0f;
    }
    if (down & 2) {
        lbl_1_bss_44.x += 1.0f;
    }
    print8(8, 0xF0, 1.5f, "BOM X=%.2f", lbl_1_bss_44.x);
    print8(8, 0xFC, 1.5f, "BOM Y=%.2f", lbl_1_bss_44.y);
    print8(8, 0x108, 1.5f, "BOM Z=%.2f", lbl_1_bss_44.z);
}

void fn_1_1204(void)
{
    Hu3DModelPosSet(lbl_1_bss_50, lbl_1_bss_44.x, lbl_1_bss_44.y + lbl_1_bss_28[0], lbl_1_bss_44.z);
}

/* debug: free camera on pad `pad` */
void fn_1_1264(s16 pad)
{
    s8 stkX;
    s8 stkY;
    s8 subX;
    s8 subY;
    s32 btn;
    s32 absX;
    s32 newX;
    s32 absY;
    s32 newY;
    s32 absSX;
    s32 newSX;
    s32 absSY;
    s32 newSY;
    s32 down;

    btn = HuPadBtn[pad];
    down = HuPadBtnDown[pad];
    stkX = (s8)HuPadStkX[pad];
    if (stkX >= 0) {
        absX = stkX;
    } else {
        absX = -stkX;
    }
    if (absX < 10) {
        newX = 0;
    } else {
        newX = stkX;
    }
    stkX = newX;
    stkY = (s8)HuPadStkY[pad];
    if (stkY >= 0) {
        absY = stkY;
    } else {
        absY = -stkY;
    }
    if (absY < 10) {
        newY = 0;
    } else {
        newY = stkY;
    }
    stkY = newY;
    subX = (s8)HuPadSubStkX[pad];
    if (subX >= 0) {
        absSX = subX;
    } else {
        absSX = -subX;
    }
    if (absSX < 8) {
        newSX = 0;
    } else {
        newSX = subX;
    }
    subX = newSX;
    subY = (s8)HuPadSubStkY[pad];
    if (subY >= 0) {
        absSY = subY;
    } else {
        absSY = -subY;
    }
    if (absSY < 8) {
        newSY = 0;
    } else {
        newSY = subY;
    }
    subY = newSY;
    if (btn & 0x400) {
        CZoom += 20.0f;
    }
    if (btn & 0x800) {
        CZoom -= 20.0f;
    }
    if (stkX > 0) {
        Center.x += 10.0f;
    }
    if (stkX < 0) {
        Center.x -= 10.0f;
    }
    if (stkY > 0) {
        Center.y += 10.0f;
    }
    if (stkY < 0) {
        Center.y -= 10.0f;
    }
    if (subX > 0) {
        CRot.y += 1.0f;
    }
    if (subX < 0) {
        CRot.y -= 1.0f;
    }
    if (subY > 0) {
        CRot.x += 1.0f;
    }
    if (subY < 0) {
        CRot.x -= 1.0f;
    }
    if (btn & 0x2000) {
        CRot.z += 1.0f;
    }
    if (btn & 0x4000) {
        CRot.z -= 1.0f;
    }
    if (btn & 8) {
        Center.z += 10.0f;
    }
    if (btn & 4) {
        Center.z -= 10.0f;
    }
}

void fn_1_167C(void)
{
    HU3DMODELID model;
    s32 i;

    model = M548.models[14].id = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_168[0], 0x10000000, 2));
    lbl_1_bss_50 = model;
    Hu3DModelAttrSet(model, 1);
    Hu3DModelPosSet(model, -140.0f, 215.0f, 0.0f);
    Hu3DModelRotSet(model, 0.0f, 0.0f, 0.0f);
    Hu3DModelCameraSet(model, 1);
    Hu3DModelAttrReset(model, 1);
    lbl_1_bss_3B4 = Hu3DTexScrollCreate(model, "m548time");
    lbl_1_bss_44.x = -lbl_1_bss_384[0].unk10;
    lbl_1_bss_44.y = lbl_1_bss_384[0].unk14;
    lbl_1_bss_44.z = 0.0f;
    lbl_1_bss_38.x = 0.0f;
    lbl_1_bss_38.y = 0.0f;
    lbl_1_bss_38.z = 0.0f;
    for (i = 0; i < 5; i++) {
        model = M548_MDLID(&M548, i, 15) = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_168[i + 1], 0x10000000, 2));
        Hu3DModelAttrSet(model, 1);
        Hu3DModelPosSet(model, -500.0f + 1000.0f * i, 500.0f, 200.0f);
        Hu3DModelRotSet(model, 0.0f, 0.0f, 0.0f);
        Hu3DModelLayerSet(model, 7);
        Hu3DModelCameraSet(model, 1);
        Hu3DMotionSpeedSet(model, 1.0f);
        Hu3DMotionTimeSet(model, 0.0f);
    }
    fn_1_5014(1);
}

void fn_1_194C(void)
{
    lbl_1_bss_20[0] = lbl_1_bss_20[1] = 0;
}

s32 fn_1_196C(void)
{
    return lbl_1_bss_20[lbl_1_bss_3D0.unk6] % 32;
}

void fn_1_19A4(void)
{
    f32 scaleY;
    s32 i;

    lbl_1_bss_28[0] = 0.0f;
    for (i = 0; i < 2; i++) {
        if (M548.slots[i].time == 50.0f || M548.slots[i].time == 51.0f) {
            if (M548.slots[i].motion == 1) {
                lbl_1_bss_20[i] = (lbl_1_bss_20[i] + 1) % 64;
                scaleY = 1.0 - 0.01f * sin(3.141592653589793 * (lbl_1_bss_20[i] * 360 / 64) / 180.0);
                Hu3DModelScaleSet(M548_MDLID(&M548, i, 12), 1.0f, scaleY, 1.0f);
                lbl_1_bss_28[0] = -1.0 * sin(3.141592653589793 * (lbl_1_bss_20[i] * 360 / 64) / 180.0);
            } else {
                lbl_1_bss_20[i] = 0;
                Hu3DModelScaleSet(M548_MDLID(&M548, i, 12), 1.0f, 1.0f, 1.0f);
            }
        } else {
            lbl_1_bss_20[i] = 0;
            Hu3DModelScaleSet(M548_MDLID(&M548, i, 12), 1.0f, 1.0f, 1.0f);
        }
    }
}

void fn_1_1CB4(void)
{
    s32 i;

    for (i = 0; i < 2; i++) {
        lbl_1_bss_0[i].unk0 = 0;
        lbl_1_bss_0[i].unk4 = 0;
        lbl_1_bss_0[i].unk8 = rand8() % 10 + 30;
        lbl_1_bss_0[i].unkC = rand8() % 2 + 6;
    }
}
