#define _MATH_H

#ifdef __MWERKS__
#pragma dont_inline on
#endif

#include "game/frand.h"
#include "game/data.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/process.h"
#include "game/object.h"
#include "game/mgdata.h"
#include "game/gamework.h"
#include "game/flag.h"
#include "game/pad.h"
#include "game/wipe.h"
#include "game/audio.h"
#include "game/window.h"
#include "game/sprite.h"
#include "game/thpmain.h"
#include "game/charman.h"
#include "game/hsfex.h"

#include "REL/e3setupdll.h"

#ifndef M_PI
#define M_PI 3.141592653589793
#endif

double sin(double);
double cos(double);
double atan2(double, double);
float Hu3DMotionMaxTimeGet(HU3DMODELID modelId);

typedef struct {
    s32 dataNum;
    s16 motNum;
    s16 pad;
    HuVecF pos;
    HuVecF rot;
    HuVecF scale;
    s16 cameraBit;
    s16 layer;
} StageMdlTblEntry;

extern StageMdlTblEntry StageMdlTbl[5];
extern s16 StageMdlId[0x100];
extern s16 StageMotId[0x100];

#ifdef TARGET_PC
/* the GC build indexes the per-model tables with -1 here and hits a neighbouring array */
#define E3_SKIP_NOCHR(idx) if ((idx) < 0) { continue; }
#else
#define E3_SKIP_NOCHR(idx)
#endif

typedef void (*VoidFunc)(void);

extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

s32 lbl_1_data_0[11] = {
    0x200030, 0x200031, 0x200032, 0x200033, 0x200034, 0x200035,
    0x200036, 0x200037, 0x200037, 0x200038, 0x200038,
};
s16 lbl_1_data_2C = 2;
GXColor lbl_1_data_2E = { 0xFF, 0xDC, 0xDC, 0 };
OMCAMERAVIEW lbl_1_data_34 = { { 0, 250, 0 }, { 0, 0, 0 }, 7000 };
OMCAMERAVIEW lbl_1_data_50 = { { 0, 0, 0 }, { 0, 0, 0 }, 1600 };
OMCAMERAVIEW lbl_1_data_6C = { { 0, 150, 0 }, { 0, 0, 0 }, 140 };
struct {
    s16 mgNo;
    s32 dataNo;
} lbl_1_data_88[8] = {
    { 1, 0x200042 }, { 2, 0x200043 }, { 4, 0x200044 }, { 5, 0x200045 },
    { 6, 0x200046 }, { 7, 0x200047 }, { 8, 0x200048 }, { 0x42, 0x200049 },
};
HuVecF lbl_1_data_C8 = { 2.5f, 2.5f, 2.5f };
s16 lbl_1_data_D4[4] = { -1, -1, -1, -1 };
HU3DPARMANPARAM lbl_1_data_138 = {
    0x1E, 0, 50.0f, 10.0f, 180.0f, { 0.0f, -0.05f, 0.0f }, 20.0f, 0.8f, 10.0f, 1.0f, 2,
    { { 0xFF, 0xFF, 0xFF, 0xFF }, { 0xFF, 0xFF, 0x40, 0xFF }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
    { { 0xFF, 0x80, 0x80, 0 }, { 0xFF, 0x40, 0x20, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
};
HU3DPARMANPARAM lbl_1_data_188 = {
    0x28, 0, 50.0f, 0.0f, 90.0f, { 0.0f, -0.05f, 0.0f }, 35.0f, 0.88f, 25.0f, 0.99f, 1,
    { { 0xFF, 0xFF, 0xFF, 0xFF }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
    { { 0xFF, 0x80, 0x80, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
};
HU3DPARMANPARAM lbl_1_data_1D8 = {
    0x1E, 0, 30.0f, 60.0f, 180.0f, { 0.0f, 0.0f, 0.0f }, 0.2f, 1.0f, 100.0f, 1.0f, 2,
    { { 0xFF, 0xFF, 0xFF, 0x20 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
    { { 0x40, 0x20, 0x20, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } },
};
HU3DPARMANPARAM lbl_1_data_228 = {
    0x32, 0, 2.0f, 30.0f, 180.0f, { 0.0f, -0.2f, 0.0f }, 1.0f, 0.8f, 30.0f, 0.95f, 3,
    { { 0x80, 0xC0, 0xFF, 0xFF }, { 0xFF, 0xFF, 0x40, 0xFF }, { 0xFF, 0x60, 0x80, 0xFF }, { 0, 0, 0, 0 } },
    { { 0x80, 0xC0, 0xFF, 0 }, { 0xFF, 0x40, 0x20, 0 }, { 0x80, 0x10, 0x10, 0 }, { 0, 0, 0, 0 } },
};
HuVecF lbl_1_data_278[4] = {
    { -360.0f, 227.0f, 0.0f }, { 360.0f, 227.0f, 0.0f },
    { -360.0f, 82.0f, 0.0f }, { 360.0f, 82.0f, 0.0f },
};
s32 lbl_1_data_104[9] = { 0x200004, 0x200004, 0x200004, 0x200004, 0x200004, 0x200004, 0x200004, 0x200004, 0x200004 };
s32 lbl_1_data_2A8[5] = { 0x420002, 0x420003, 0x420004, 0x420005, 0x420006 };
s32 lbl_1_data_2BC[13] = { 0 };
s32 lbl_1_data_2F0 = -1;

s32 lbl_1_bss_0;
s16 lbl_1_bss_4;
s32 lbl_1_bss_8;
s32 lbl_1_bss_C;
s16 lbl_1_bss_10;
s32 lbl_1_bss_14;
s32 lbl_1_bss_18;
s32 lbl_1_bss_1C;
s32 lbl_1_bss_20;
s32 lbl_1_bss_24;
s32 lbl_1_bss_28;
ANIMDATA *lbl_1_bss_2C[2];
s16 lbl_1_bss_34[4];
s16 lbl_1_bss_3C[4];
s32 lbl_1_bss_44[2];
ANIMDATA *lbl_1_bss_4C;
ANIMDATA *lbl_1_bss_50;
s16 lbl_1_bss_54;
s16 lbl_1_bss_56;
s16 lbl_1_bss_58;
s16 lbl_1_bss_5A[4];
s16 lbl_1_bss_62[10];
s16 lbl_1_bss_76[10];
s16 lbl_1_bss_8A[10][5];
s16 lbl_1_bss_EE[11];
s32 lbl_1_bss_104;
MGDATA *lbl_1_bss_108;
s16 lbl_1_bss_10C[2][4];
s16 lbl_1_bss_11C;
s16 lbl_1_bss_11E;
s16 lbl_1_bss_120;
s16 lbl_1_bss_122;
s16 lbl_1_bss_124;
s16 lbl_1_bss_126;
u8 lbl_1_bss_128;
s16 lbl_1_bss_12A;
f32 lbl_1_bss_12C;
s16 lbl_1_bss_130;
u16 lbl_1_bss_132;
u16 lbl_1_bss_134;
HUPROCESS *lbl_1_bss_138;
OMOBJ *lbl_1_bss_13C;

void fn_1_838(void);
void fn_1_D30(s32 arg0);
void fn_1_F24(void);
void fn_1_FE0(void);
void fn_1_22D8(void);
void fn_1_2540(void);
void fn_1_26FC(void);
void fn_1_28EC(s16 layerNo);
void fn_1_2A34(s16 arg);
void fn_1_2C28(void);
void fn_1_2E08(void);
void fn_1_2E58(void);
void fn_1_33F8(void);
void fn_1_39E4(void);
void fn_1_4DD8(s16 arg);
void fn_1_4E58(void);
void fn_1_5904(void);
void fn_1_6804(void);
void fn_1_71AC(float arg);
s32 fn_1_7374(void);
s32 fn_1_7AF0(void);
s32 fn_1_95B4(void);
void fn_1_9A50(OMOBJ *obj);
void fn_1_A5B0(void);
void fn_1_A6E8(void);

void _prolog(void) {
    const VoidFunc *ctorPtr = _ctors;

    for (; *ctorPtr != 0; ctorPtr++) {
        (*ctorPtr)();
    }
    ObjectSetup(NULL);
}

void _epilog(void) {
    const VoidFunc *dtorPtr = _dtors;
    while (*dtorPtr != 0) {
        (*dtorPtr)();
        dtorPtr++;
    }
}


void ObjectSetup(void *arg)
{
    s32 order[2][2];
    s32 cnt[2];
    s16 i;

    OSReport("******* E3Setup ObjectSetup *********\n");
    lbl_1_bss_138 = omInitObjMan(50, 0x2000);
    omGameSysInit(lbl_1_bss_138);
    lbl_1_bss_128 = GwCommon.languageNo;
    lbl_1_bss_134 = GwSystem.mgNo;
    for (lbl_1_bss_10 = 0; lbl_1_bss_10 < 8; lbl_1_bss_10++) {
        if (lbl_1_bss_134 == lbl_1_data_88[lbl_1_bss_10].mgNo - 1) {
            break;
        }
    }
    if (lbl_1_bss_10 == 8) {
        lbl_1_bss_10 = 0;
    }
    lbl_1_bss_108 = &mgDataTbl[lbl_1_data_88[lbl_1_bss_10].mgNo - 1];
    lbl_1_bss_130 = 0;
    lbl_1_bss_4 = 0;
    _ClearFlag(0x1000F);
    if (lbl_1_bss_108->ovl == 999) {
        if (GwSystem.subGameNo == -1) {
            GwSystem.subGameNo = frandmod(3);
        }
    } else {
        GwSystem.subGameNo = -1;
    }
    HuDataDirClose(lbl_1_bss_108->dataDir);
    {
        OMOVLHIS *his = omOvlHisGet(0);
        omOvlHisChg(0, DLL_e3setupdll, his->evtno, his->stat);
    }
    if (omovlevtno == 0 && lbl_1_bss_108->ovl == 44) {
        cnt[0] = 0;
        cnt[1] = 0;
        for (i = 0; i < 4; i++) {
            order[GwPlayerConf[i].grp][cnt[GwPlayerConf[i].grp]++] = i;
        }
        if (frand() & 1) {
            GwPlayerConf[order[0][0]].grp = 0;
            GwPlayerConf[order[0][1]].grp = 1;
        } else {
            GwPlayerConf[order[0][0]].grp = 1;
            GwPlayerConf[order[0][1]].grp = 0;
        }
        if (frand() & 1) {
            GwPlayerConf[order[1][0]].grp = 2;
            GwPlayerConf[order[1][1]].grp = 3;
        } else {
            GwPlayerConf[order[1][0]].grp = 3;
            GwPlayerConf[order[1][1]].grp = 2;
        }
    }
    lbl_1_bss_12C = 20.0f;
    lbl_1_bss_132 = GwSystem.boardNo;
    if (mgBoard2Force != 0) {
        lbl_1_bss_132 = 1;
    }
    omSysPauseEnable(FALSE);
    Hu3DCameraCreate(3);
    Hu3DCameraPerspectiveSet(1, 5.0f, 20.0f, 15000.0f, 1.2f);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraPerspectiveSet(2, 30.0f, 20.0f, 15000.0f, 1.2f);
    Hu3DCameraViewportSet(2, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    omCameraViewSetMulti(1, &lbl_1_data_34);
    omCameraViewSetMulti(2, &lbl_1_data_50);
    Hu3DGLightInfinitytSet(Hu3DGLightCreate(-1000.0f, 1500.0f, 1000.0f, 1.0f, -1.5f, -1.0f, 0xD0, 0xA0, 0xA0));
    Hu3DGLightInfinitytSet(Hu3DGLightCreate(0.0f, 0.0f, 1000.0f, 0.0f, 0.0f, -1.0f, 0x60, 0x60, 0x60));
    lbl_1_bss_13C = omAddObjEx(lbl_1_bss_138, 0x7FDA, 0, 0, -1, omOutViewMulti);
    lbl_1_bss_13C->work[0] = 2;
    omAddObjEx(lbl_1_bss_138, 0, 0x20, 0x20, -1, fn_1_9A50);
    HuAudSStreamPlay(0x30);
    Hu3DBGColorSet(0, 0, 0);
    HuWinInit(1);
    HuPrcChildCreate(fn_1_838, 0x3E8, 0x3000, 0, lbl_1_bss_138);
    if (omprevovl >= 0xF && omprevovl <= 0x57) {
        lbl_1_data_2C = 4;
        lbl_1_bss_104 = 1;
    } else {
        lbl_1_data_2C = 0;
        lbl_1_bss_104 = 0;
    }
    for (i = 0; i < 4; i++) {
        lbl_1_bss_5A[i] = GwPlayerConf[i].charNo;
    }
}

void fn_1_838(void)
{
    float t;
    s16 i;
    s32 quit = 0;

    fn_1_FE0();
    HuPrcChildCreate(fn_1_6804, 0x64, 0x3000, 0, HuPrcCurrentGet());
    HuPrcChildCreate(fn_1_5904, 0x64, 0x3000, 0, HuPrcCurrentGet());
    HuPrcChildCreate(fn_1_33F8, 0x64, 0x3000, 0, HuPrcCurrentGet());
    HuPrcChildCreate(fn_1_39E4, 0x64, 0x3000, 0, HuPrcCurrentGet());
    HuPrcChildCreate(fn_1_F24, 0x64, 0x1000, 0, HuPrcCurrentGet());
    if (lbl_1_data_2C == 2) {
        lbl_1_data_2C = 3;
        WipeCreate(1, 0, 10);
    } else {
        WipeCreate(1, 0, 30);
    }
    while (WipeCheckEnd() != 0) {
        HuPrcVSleep();
    }
loop_7:
    while (lbl_1_data_2C < 2) {
        HuPrcVSleep();
    }
    lbl_1_data_2C = 3;
    if (lbl_1_bss_104 == 0) {
        fn_1_22D8();
    }
    lbl_1_bss_104 = 0;
    lbl_1_data_2C = 4;
    fn_1_4DD8(0);
    while (1) {
        lbl_1_bss_122 = HuPadBtn[0];
        lbl_1_bss_11E = HuPadDStkRep[0];
        lbl_1_bss_120 = HuPadBtnDown[0];
        if (lbl_1_bss_C == 0) {
            if ((lbl_1_bss_120 & 0x1000) == 0) {
                if ((lbl_1_bss_120 & 0x200) != 0) {
                    HuAudFXPlay(0);
                    lbl_1_data_2C = 1;
                    for (i = 1; i <= 10; i++) {
                        t = i / 10.0;
                        HuSprGrpPosSet(lbl_1_bss_12A, 288.0f, 64.0 - 100.0 * (1.0 - cos(3.141592653589793 * (90.0f * t) / 180.0)));
                        HuSprGrpPosSet(lbl_1_bss_124, 288.0f, 64.0 - 100.0 * (1.0 - cos(3.141592653589793 * (90.0f * t) / 180.0)));
                        HuPrcVSleep();
                    }
                    HuPrcSleep(20);
                    HuTHPClose();
                    lbl_1_data_2C = 0;
                    goto loop_7;
                }
                if ((lbl_1_bss_11E & 1) != 0) {
                    HuAudFXPlay(0);
                    lbl_1_bss_20 = 1;
                    fn_1_4DD8(1);
                    while (lbl_1_bss_28 != 0) {
                        HuPrcVSleep();
                    }
                    HuPrcChildCreate(fn_1_A5B0, 0x7D0, 0x3000, 0, HuPrcCurrentGet());
                }
                if ((lbl_1_bss_11E & 2) != 0) {
                    HuAudFXPlay(0);
                    lbl_1_bss_20 = 1;
                    fn_1_4DD8(-1);
                    while (lbl_1_bss_28 != 0) {
                        HuPrcVSleep();
                    }
                    HuPrcChildCreate(fn_1_A5B0, 0x7D0, 0x3000, 0, HuPrcCurrentGet());
                }
            } else {
                break;
            }
        }
        if (omSysExitReq != 0) {
            quit = 1;
            break;
        }
        HuPrcVSleep();
    }
    if (quit == 0) {
        lbl_1_data_2C = 5;
        fn_1_2540();
    }
    fn_1_D30(quit);
    HuPrcEnd();
    while (1) {
        HuPrcVSleep();
    }
}

void fn_1_D30(s32 arg0)
{
    s16 i;

    if (lbl_1_bss_24 == 0) {
        lbl_1_bss_24 = 1;
        WipeCreate(2, 6, 40);
        HuAudSStreamAllFadeOut(1000);
        while (WipeCheckEnd() != 0) {
            HuPrcVSleep();
        }
        HuTHPClose();
        if (arg0 == 0) {
            for (i = 0; i < 4; i++) {
                GwPlayerConf[i].charNo = lbl_1_bss_5A[i];
                GwPlayerConf[i].dif = 0;
                if (lbl_1_bss_58 > i) {
                    GwPlayerConf[i].type = 0;
                } else {
                    GwPlayerConf[i].type = 1;
                }
                GwPlayerConf[i].padNo = i;
            }
            GwSystem.mgNo = lbl_1_data_88[lbl_1_bss_10].mgNo - 1;
            while (lbl_1_bss_1C != 0) {
                HuPrcVSleep();
            }
            omOvlCallEx(lbl_1_bss_108->ovl, 1, 0, 0);
            return;
        }
        lbl_1_bss_20 = 1;
        HuPrcVSleep();
        omOvlReturnEx(1, 1);
    }
}

void fn_1_F24(void)
{
    u32 timer;
    u32 i;

    while (1) {
        for (timer = 0; timer < 10800; timer++) {
            for (i = 0; i < 4; i++) {
                if (((HuPadStkY[i] & 0xF8) | ((HuPadStkX[i] & 0xF8) | HuPadBtn[i])) != 0) {
                    break;
                }
            }
            if (i != 4) {
                break;
            }
            HuPrcVSleep();
        }
        if (timer == 10800) {
            fn_1_D30(1);
            while (1) {
                HuPrcVSleep();
            }
        }
        HuPrcVSleep();
    }
}

void fn_1_FE0(void)
{
    s16 i;
    s16 j;
    s16 k;
    HuVecF pos3D;
    HuVecF pos2D;
    HU3DMODEL *modelP;
    s16 grp;
    ANIMDATA *anim;
    float f;

    lbl_1_bss_108->type = 0;
    StageMdlTbl[3].dataNum = lbl_1_data_104[lbl_1_bss_108->type];
    StageModelCreate(StageMdlTbl);
    MgNameInit();
    for (i = 0; i < 10; i++) {
        lbl_1_bss_EE[i] = Hu3DModelCreate(HuDataSelHeapReadNum(i * 4 + 0x200008, HU_MEMNUM_OVL, HEAP_MODEL));
        lbl_1_bss_8A[i][0] = Hu3DJointMotion(lbl_1_bss_EE[i], HuDataSelHeapReadNum(i * 4 + 0x200009, HU_MEMNUM_OVL, HEAP_MODEL));
        lbl_1_bss_8A[i][1] = Hu3DJointMotion(lbl_1_bss_EE[i], HuDataSelHeapReadNum(i * 4 + 0x20000A, HU_MEMNUM_OVL, HEAP_MODEL));
        lbl_1_bss_8A[i][2] = Hu3DJointMotion(lbl_1_bss_EE[i], HuDataSelHeapReadNum(i * 4 + 0x20000B, HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DMotionSet(lbl_1_bss_EE[i], lbl_1_bss_8A[i][0]);
        Hu3DModelCameraSet(lbl_1_bss_EE[i], 1);
        Hu3DModelPosSet(lbl_1_bss_EE[i], (i % 5) * 150 - 300, (i / 5) * 270, 0.0f);
        Hu3DModelScaleSet(lbl_1_bss_EE[i], 0.8f, 0.8f, 0.8f);
        Hu3DModelAttrSet(lbl_1_bss_EE[i], 0x40000001);
        lbl_1_bss_76[i] = Hu3DModelCreate(HuDataSelHeapReadNum(0x200001, HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelScaleSet(lbl_1_bss_76[i], 0.7f, 0.7f, 0.3f);
        Hu3DModelCameraSet(lbl_1_bss_76[i], 2);
        Hu3DModelLayerSet(lbl_1_bss_76[i], 2);
    }
    for (i = 0; i < 4; i++) {
        lbl_1_bss_3C[i] = Hu3DModelCreate(HuDataSelHeapReadNum(0x200005, HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelScaleSet(lbl_1_bss_3C[i], 0.7f, 0.7f, 0.3f);
        Hu3DModelCameraSet(lbl_1_bss_3C[i], 2);
        Hu3DModelLayerSet(lbl_1_bss_3C[i], 2);
        Hu3DModelAttrSet(lbl_1_bss_3C[i], 1);
    }
    HuPrcChildCreate(fn_1_2E08, 0x32, 0x3000, 0, HuPrcCurrentGet());
    Hu3DModelScaleSet(lbl_1_bss_EE[8], 0.5f, 0.5f, 0.5f);
    Hu3DModelScaleSet(lbl_1_bss_EE[9], 0.7f, 0.7f, 0.7f);
    Hu3DModelScaleSet(lbl_1_bss_EE[7], 0.7f, 0.7f, 0.7f);
    fn_1_2A34(lbl_1_bss_EE[8]);
    fn_1_2A34(lbl_1_bss_EE[9]);
    Hu3DModelAttrSet(StageMdlId[0], 0x40000001);
    Hu3DModelAttrSet(StageMdlId[3], 0x40000001);
    Hu3DModelLayerSet(StageMdlId[3], 0);
    Hu3DMotionSet(StageMdlId[1], StageMotId[2]);
    Hu3DModelAttrSet(StageMdlId[1], 0x40000001);
    Hu3DModelLayerSet(StageMdlId[1], 2);
    Hu3DLayerHookSet(1, fn_1_28EC);
    Hu3DLayerHookSet(9, fn_1_28EC);
    ToueiDispInit();
    for (i = 0; i < 8; i++) {
        lbl_1_bss_62[i] = Hu3DModelCreate(HuDataSelHeapReadNum(0x200000, HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DAnimCreate(HuSprAnimRead(HuDataSelHeapReadNum(lbl_1_data_88[i].dataNo, HU_MEMNUM_OVL, HEAP_MODEL)), lbl_1_bss_62[i], "S3TC_mg_movie");
        f = 600.0 * cos(3.141592653589793 * ((lbl_1_bss_10 - i) * 45) / 180.0) - 600.0 - 300.0;
        Hu3DModelPosSet(lbl_1_bss_62[i], 500.0 * sin(3.141592653589793 * ((lbl_1_bss_10 - i) * 45) / 180.0), 157.0f, f);
        Hu3DModelScaleSet(lbl_1_bss_62[i], 0.0f, 0.0f, 0.0f);
        Hu3DModelCameraSet(lbl_1_bss_62[i], 2);
        Hu3DModelLayerSet(lbl_1_bss_62[i], 1);
    }
    lbl_1_bss_12A = MgNameCreate((void *)(uintptr_t)lbl_1_bss_108->nameMes);
    HuSprGrpPosSet(lbl_1_bss_12A, 288.0f, 64.0f);
    HuSprGrpScaleSet(lbl_1_bss_12A, 0.9f, 1.0f);
    grp = HuSprGrpCreate(1);
    lbl_1_bss_126 = grp;
    HuSprGrpMemberSet(grp, 0, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0x20003D, HU_MEMNUM_OVL, HEAP_MODEL)), 100, 0));
    HuSprAttrSet(grp, 0, 8);
    modelP = &Hu3DData[StageMdlId[1]];
    pos3D = modelP->pos;
    pos3D.y = pos3D.y + 70.0f;
    Hu3D3Dto2D(&pos3D, 2, &pos2D);
    HuSprGrpPosSet(grp, pos2D.x, pos2D.y);
    HuPrcChildCreate(fn_1_2E58, 0x64, 0x3000, 0, HuPrcCurrentGet());
    grp = HuSprGrpCreate(1);
    lbl_1_bss_124 = grp;
    HuSprGrpMemberSet(grp, 0, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(0x20003E, HU_MEMNUM_OVL, HEAP_MODEL)), 100, 0));
    HuSprGrpPosSet(grp, 288.0f, 64.0f);
    HuSprExecLayerCameraSet(0x40, 2, 8);
    grp = HuSprGrpCreate(0x13);
    lbl_1_bss_11C = grp;
    for (i = 0; i < 11; i++) {
        HuSprGrpMemberSet(grp, i, HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(lbl_1_data_0[i], HU_MEMNUM_OVL, HEAP_MODEL)), 100, 0));
    }
    HuSprGrpDrawNoSet(grp, 0x40);
    HuSprPriSet(grp, 5, 50);
    HuSprBankSet(lbl_1_bss_11C, 7, 3);
    HuSprDrawNoSet(lbl_1_bss_11C, 7, 0);
    HuSprBankSet(lbl_1_bss_11C, 8, 2);
    HuSprDrawNoSet(lbl_1_bss_11C, 8, 0);
    HuSprBankSet(lbl_1_bss_11C, 9, 3);
    HuSprDrawNoSet(lbl_1_bss_11C, 9, 0);
    HuSprAttrSet(lbl_1_bss_11C, 9, 0x80);
    HuSprBankSet(lbl_1_bss_11C, 10, 2);
    HuSprDrawNoSet(lbl_1_bss_11C, 10, 0);
    HuSprAttrSet(lbl_1_bss_11C, 10, 0x80);
    HuPrcChildCreate(fn_1_2C28, 0x32, 0x3000, 0, HuPrcCurrentGet());
    HuSprPosSet(lbl_1_bss_11C, 0, 288.0f, 240.0f);
    HuSprColorSet(lbl_1_bss_11C, 0, 0x28, 0x28, 0x50);
    HuSprTPLvlSet(lbl_1_bss_11C, 0, 0.7f);
    HuSprPosSet(lbl_1_bss_11C, 1, 288.0f, 240.0f);
    HuSprAttrSet(lbl_1_bss_11C, 6, 0x80);
    HuSprTPLvlSet(lbl_1_bss_11C, 6, 0.5f);
    if (GwCommon.rumbleF != 0) {
        HuSprColorSet(lbl_1_bss_11C, 6, 0, 0xFF, 0xFF);
        HuSprBankSet(lbl_1_bss_11C, 5, 0);
    } else {
        HuSprColorSet(lbl_1_bss_11C, 6, 0xFF, 0, 0);
        HuSprBankSet(lbl_1_bss_11C, 5, 1);
    }
    for (i = 0; i < 3; i++) {
        HuSprPosSet(lbl_1_bss_11C, i + 2, 288.0f, 240.0f);
        HuSprDrawNoSet(lbl_1_bss_11C, i + 2, 0);
        HuSprPriSet(grp, i + 2, 30);
        HuSprColorSet(grp, i + 2, 0, 0, 0);
        HuSprTPLvlSet(grp, i + 2, 0.5f);
    }
    anim = HuSprAnimRead(HuDataSelHeapReadNum(0x200039, HU_MEMNUM_OVL, HEAP_MODEL));
    for (; i < 15; i++) {
        HuSprGrpMemberSet(grp, i, HuSprCreate(anim, 50, 0));
        HuSprDrawNoSet(grp, i, 0);
    }
    anim = HuSprAnimRead(HuDataSelHeapReadNum(0x20003A, HU_MEMNUM_OVL, HEAP_MODEL));
    for (; i < 19; i++) {
        HuSprGrpMemberSet(grp, i, HuSprCreate(anim, 50, 0));
        HuSprDrawNoSet(grp, i, 0);
    }
    HuSprGrpPosSet(grp, 0.0f, 0.0f);
    for (k = 0; k < 19; k++) {
        HuSprAttrSet(grp, k, 4);
    }
    for (j = 0; j < 2; j++) {
        for (k = 0; k < 4; k++) {
            lbl_1_bss_10C[j][k] = Hu3DModelCreate(HuDataSelHeapReadNum(j + 0x20003B, HU_MEMNUM_OVL, HEAP_MODEL));
            Hu3DModelCameraSet(lbl_1_bss_10C[j][k], 2);
            Hu3DModelAttrSet(lbl_1_bss_10C[j][k], 1);
            Hu3DModelAttrSet(lbl_1_bss_10C[j][k], 0x40000001);
            Hu3DModelLayerSet(lbl_1_bss_10C[j][k], 2);
        }
    }
    lbl_1_bss_50 = HuSprAnimRead(HuDataSelHeapReadNum(0x20003F, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_4C = HuSprAnimRead(HuDataSelHeapReadNum(0x210001, HU_MEMNUM_OVL, HEAP_MODEL));
    HuPrcChildCreate(fn_1_26FC, 0x32, 0x3000, 0, HuPrcCurrentGet());
    lbl_1_bss_2C[0] = HuSprAnimRead(HuDataSelHeapReadNum(0x200040, HU_MEMNUM_OVL, HEAP_MODEL));
    lbl_1_bss_2C[1] = HuSprAnimRead(HuDataSelHeapReadNum(0x200041, HU_MEMNUM_OVL, HEAP_MODEL));
    HuDataDirClose(0x210001);
    HuDataDirClose(0x200000);
    if (lbl_1_bss_104 == 0) {
        HuSprGrpPosSet(lbl_1_bss_12A, 1000.0f, 0.0f);
        HuSprGrpScaleSet(lbl_1_bss_124, 0.0f, 0.0f);
        for (i = 0; i < 10; i++) {
            Hu3DModelPosSet(lbl_1_bss_76[i], 1000.0f, 1000.0f, 0.0f);
        }
        HuSprGrpScaleSet(lbl_1_bss_126, 0.0f, 0.0f);
        Hu3DModelPosSet(StageMdlId[1], 0.0f, -1000.0f, 0.0f);
    }
}

void fn_1_22D8(void)
{
    float t;
    s16 i;

    HuSprGrpPosSet(lbl_1_bss_12A, 1000.0f, 0.0f);
    HuSprGrpPosSet(lbl_1_bss_124, 288.0f, 64.0f);
    HuSprGrpScaleSet(lbl_1_bss_124, 1.0f, 0.0f);
    for (i = 1; i <= 30; i++) {
        if (i < 20) {
            t = i / 20.0;
            HuSprGrpScaleSet(lbl_1_bss_124, 1.0f, 1.1 * sin(3.141592653589793 * (90.0f * t) / 180.0));
        }
        if (i == 20) {
            HuSprGrpScaleSet(lbl_1_bss_124, 1.0f, 1.0f);
        }
        if (i >= 20) {
            t = (i - 20) / 10.0;
            HuSprGrpPosSet(lbl_1_bss_12A, 288.0 + 500.0 * cos(3.141592653589793 * (90.0f * t) / 180.0), 64.0f);
        }
        HuPrcVSleep();
    }
}

void fn_1_2540(void)
{
    float t;
    s16 i;

    HuAudFXPlay(0);
    for (i = 1; i <= 10; i++) {
        t = i / 10.0;
        HuSprGrpPosSet(lbl_1_bss_12A, 288.0f, 64.0 - 100.0 * (1.0 - cos(3.141592653589793 * (90.0f * t) / 180.0)));
        HuSprGrpPosSet(lbl_1_bss_124, 288.0f, 64.0 - 100.0 * (1.0 - cos(3.141592653589793 * (90.0f * t) / 180.0)));
        HuPrcVSleep();
    }
    HuPrcSleep(20);
    omCameraViewMoveMulti(2, &lbl_1_data_6C, 40, 1);
}

void fn_1_26FC(void)
{
    HuVecF camPos;
    HuVecF diff;
    s16 i;
    s16 j;
    s16 k;
    HU3DCAMERA *cameraP = &Hu3DCamera[1];
    float rotY;

    while (1) {
        HuPrcVSleep();
        camPos = *(HuVecF *)&cameraP->pos;
        for (i = 0; i < 10; i++) {
            PSVECSubtract((Vec *)&camPos, &Hu3DData[lbl_1_bss_76[i]].pos, (Vec *)&diff);
            rotY = 180.0 * (atan2(diff.x, diff.z) / 3.141592653589793);
            Hu3DModelRotSet(lbl_1_bss_76[i], 0.0f, rotY, 0.0f);
        }
        for (j = 0; j < 4; j++) {
            for (k = 0; k < 2; k++) {
                PSVECSubtract((Vec *)&camPos, &Hu3DData[lbl_1_bss_10C[k][j]].pos, (Vec *)&diff);
                rotY = 180.0 * (atan2(diff.x, diff.z) / 3.141592653589793);
                Hu3DModelRotSet(lbl_1_bss_10C[k][j], 0.0f, rotY, 0.0f);
            }
        }
    }
}

void fn_1_28EC(s16 layerNo)
{
    HU3DCAMERA *cameraP = &Hu3DCamera[1];
    GXColor noFog = {};
    if (Hu3DCameraNo == 1) {
        if (layerNo == 1) {
            GXSetFog(GX_FOG_PERSP_EXP2, 1500.0f, 3700.0f, cameraP->near, cameraP->far, lbl_1_data_2E);
            return;
        }
        GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, noFog);
    }
}

void fn_1_2A34(s16 modelId)
{
    HSFDATA *hsf = Hu3DData[modelId].hsf;
    HSFMATERIAL *mat = hsf->material;
    s32 i;

    for (i = 0; i < hsf->materialNum; i++, mat++) {
        mat->litColor[0] = mat->litColor[0] * 0.1;
        mat->litColor[1] = mat->litColor[1] * 0.1;
        mat->litColor[2] = mat->litColor[2] * 0.1;
        mat->color[0] = mat->color[0] * 0.3;
        mat->color[1] = mat->color[1] * 0.3;
        mat->color[2] = mat->color[2] * 0.3;
    }
}

void fn_1_2C28(void)
{
    HUSPRITE *spr[2];
    HUSPRGRP *grpP;
    s16 i;

    grpP = &HuSprGrpData[lbl_1_bss_11C];
    spr[0] = &HuSprData[grpP->sprId[7]];
    spr[1] = &HuSprData[grpP->sprId[8]];
    HuSprTPLvlSet(lbl_1_bss_11C, 9, 0.5f);
    HuSprTPLvlSet(lbl_1_bss_11C, 10, 0.5f);
    lbl_1_bss_44[1] = 0;
    lbl_1_bss_44[0] = 0;
    while (1) {
        for (i = 0; i < 2; i++) {
            if ((spr[i]->attr & 4) != 0 || lbl_1_bss_44[i] == 0) {
                HuSprAttrSet(lbl_1_bss_11C, i + 9, 4);
            } else {
                HuSprAttrReset(lbl_1_bss_11C, i + 9, 4);
            }
            HuSprPosSet(lbl_1_bss_11C, i + 9, spr[i]->pos.x, spr[i]->pos.y);
            HuSprScaleSet(lbl_1_bss_11C, i + 9, spr[i]->scale.x, spr[i]->scale.y);
        }
        HuPrcVSleep();
    }
}

void fn_1_2E08(void)
{
    s16 i;
    while (1) {
        for (i = 0; i < 4; i++) {
            Hu3DModelAttrSet(lbl_1_bss_3C[i], 1);
        }
        HuPrcVSleep();
    }
}

void fn_1_2E58(void)
{
    float t;
    s16 i;
    s16 save[4];
    s16 timer[4];
    s16 state[4];
    s16 prev[4];

    for (i = 0; i < 4; i++) {
        prev[i] = -1;
        lbl_1_bss_34[i] = -1;
        state[i] = 0;
    }
    while (1) {
        for (i = 0; i < 4; i++) {
            if (prev[i] != lbl_1_bss_34[i]) {
                if (prev[i] == -1) {
                    state[i] = 1;
                } else if (lbl_1_bss_34[i] == -1) {
                    state[i] = 2;
                    save[i] = prev[i];
                } else {
                    state[i] = 3;
                }
                timer[i] = 0;
            }
            prev[i] = lbl_1_bss_34[i];
            switch (state[i]) {
                case 1:
                    t = timer[i] / 10.0;
                    HuSprAttrReset(lbl_1_bss_11C, i + 15, 4);
                    HuSprPosSet(lbl_1_bss_11C, i + 15, 128.0f + (lbl_1_bss_34[i] % 5) * 80 - 10.0f, (lbl_1_bss_34[i] / 5) * 90 + 210);
                    HuSprScaleSet(lbl_1_bss_11C, i + 15, t, t);
                    timer[i]++;
                    if (timer[i] > 10.0f) {
                        state[i] = 0;
                    }
                    break;
                case 2:
                    t = 1.0 - timer[i] / 10.0;
                    HuSprAttrReset(lbl_1_bss_11C, i + 15, 4);
                    HuSprPosSet(lbl_1_bss_11C, i + 15, 128.0f + (save[i] % 5) * 80 - 10.0f, (save[i] / 5) * 90 + 210);
                    HuSprScaleSet(lbl_1_bss_11C, i + 15, t, t);
                    timer[i]++;
                    if (timer[i] > 10.0f) {
                        state[i] = 0;
                    }
                    break;
            }
        }
        HuPrcVSleep();
    }
}

void fn_1_33F8(void)
{
    HuVecF pos3D;
    HuVecF pos2D;
    HU3DMODEL *modelP;
    float t;
    float ang;
    float amp;
    s16 i;
    s16 j;
    s16 k;
    s16 m;

    ang = 0.0f;
loop_2:
    if (lbl_1_data_2C < 2) {
        HuPrcVSleep();
        goto loop_2;
    }
    if (lbl_1_data_2C != 2) {
        if (lbl_1_data_2C == 3) {
            goto loop_7;
        }
    } else {
loop_7:
        if (lbl_1_data_2C != 3) {
            HuPrcVSleep();
            goto loop_7;
        }
        for (i = 0; i <= 20; i++) {
            t = 1.0 - sin(3.141592653589793 * (90.0 * (i / 20.0)) / 180.0);
            pos3D = StageMdlTbl[1].pos;
            Hu3DModelPosSet(StageMdlId[1], pos3D.x, pos3D.y - 300.0f * t, pos3D.z);
            HuPrcVSleep();
        }
        for (j = 0; j <= 20; j++) {
            t = sin(3.141592653589793 * (90.0 * (j / 20.0)) / 180.0);
            HuSprGrpScaleSet(lbl_1_bss_126, t, t);
            HuPrcVSleep();
        }
    }
    amp = 0.0;
loop_15:
    pos3D = StageMdlTbl[1].pos;
    modelP = &Hu3DData[StageMdlId[1]];
    pos3D = modelP->pos;
    pos3D.y = pos3D.y + 70.0f;
    Hu3D3Dto2D(&pos3D, 2, &pos2D);
    HuSprGrpPosSet(lbl_1_bss_126, pos2D.x, pos2D.y);
    t = 1.0 + 0.05 * (amp * sin(3.141592653589793 * (2.0f * ang) / 180.0));
    HuSprGrpScaleSet(lbl_1_bss_126, 1.0 + 0.05 * (amp * sin(3.141592653589793 * (2.0f * ang) / 180.0)), t);
    ang += 2.0f;
    if (ang > 720.0f) {
        ang -= 720.0f;
    }
    amp = amp + 0.002;
    if (amp > 1.0f) {
        amp = 1.0f;
    }
    if (lbl_1_data_2C != 5) {
        if (lbl_1_data_2C == 1) {
            for (k = 0; k <= 20; k++) {
                t = sin(3.141592653589793 * (90.0 * (k / 20.0)) / 180.0);
                pos3D = StageMdlTbl[1].pos;
                Hu3DModelPosSet(StageMdlId[1], pos3D.x, pos3D.y - 300.0f * t, pos3D.z);
                t = 1.0 - t;
                HuSprGrpScaleSet(lbl_1_bss_126, t, t);
                HuPrcVSleep();
            }
            goto loop_2;
        }
        HuPrcVSleep();
        goto loop_15;
    }
    for (m = 0; m <= 10; m++) {
        t = cos(3.141592653589793 * (90.0 * (m / 10.0)) / 180.0);
        HuSprGrpScaleSet(lbl_1_bss_126, t, t);
        HuPrcVSleep();
    }
    while (1) {
        HuPrcVSleep();
    }
}

void fn_1_39E4(void)
{
    float ang;
    float amp;
    float amp2;
    float t;
    float t2;
    float sgn;
    HuVecF pos;
    HuVecF scl;
    HuVecF diff;
    HuVecF tgt;
    s32 *grpA[4];
    s32 *grpB[4];
    s32 *parA;
    s32 *parB;
    ANIMDATA *animA;
    ANIMDATA *animB;
    HU3DMODEL *modelP;
    s16 mdl;
    s16 id;
    s16 i;
    s16 j;
    s16 k;

    ang = 0.0f;
    for (i = 0; i < 4; i++) {
        animA = lbl_1_bss_50;
        parA = HuMemDirectMallocNum(HEAP_HEAP, 0xC, HU_MEMNUM_OVL);
        id = Hu3DParManCreate(animA, 100, &lbl_1_data_138);
        parA[0] = id;
        Hu3DParManAttrSet(id, 0x65);
        Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(id), 1);
        Hu3DParManRotSet(id, 90.0f, 0.0f, 0.0f);
        Hu3DModelLayerSet(Hu3DParManModelIDGet(id), 3);
        Hu3DModelCameraSet(Hu3DParManModelIDGet(id), 2);
        id = Hu3DParManCreate(animA, 100, &lbl_1_data_188);
        parA[1] = id;
        Hu3DParManAttrSet(id, 0x163);
        Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(id), 1);
        j = frandmod(80) - 40;
        Hu3DParManRotSet(id, frandmod(40) + 70, 0.0f, j);
        Hu3DModelLayerSet(Hu3DParManModelIDGet(id), 3);
        Hu3DModelCameraSet(Hu3DParManModelIDGet(id), 2);
        id = Hu3DParManCreate(animA, 100, &lbl_1_data_1D8);
        parA[2] = id;
        Hu3DParManAttrSet(id, 0x21);
        Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(id), 1);
        Hu3DParManRotSet(id, 0.0f, 0.0f, 0.0f);
        Hu3DModelLayerSet(Hu3DParManModelIDGet(id), 3);
        Hu3DModelCameraSet(Hu3DParManModelIDGet(id), 2);
        grpA[i] = parA;
        animB = lbl_1_bss_4C;
        parB = HuMemDirectMallocNum(HEAP_HEAP, 0xC, HU_MEMNUM_OVL);
        id = Hu3DParManCreate(animB, 100, &lbl_1_data_228);
        parB[0] = id;
        Hu3DParManAttrSet(id, 0x65);
        Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(id), 1);
        Hu3DParManRotSet(id, 90.0f, 0.0f, 0.0f);
        Hu3DModelLayerSet(Hu3DParManModelIDGet(id), 3);
        Hu3DModelCameraSet(Hu3DParManModelIDGet(id), 2);
        grpB[i] = parB;
    }
    for (i = 0; i < 10; i++) {
        Hu3DModelPosSet(lbl_1_bss_76[i], 1000.0f, 1000.0f, 0.0f);
    }
loop_8:
    if (lbl_1_data_2C < 2) {
        HuPrcVSleep();
        goto loop_8;
    }
    if (lbl_1_data_2C != 2) {
        if (lbl_1_data_2C == 3) {
            goto loop_13;
        }
        scl.z = 1.5f;
        scl.y = 1.5f;
        scl.x = 1.5f;
        for (i = 0; i < 8; i++) {
            Hu3DModelScaleSetV(lbl_1_bss_62[i], &scl);
        }
        Hu3DModelScaleSetV(lbl_1_bss_62[lbl_1_bss_10], &lbl_1_data_C8);
        t = 600.0 * cos(0.0) - 600.0;
        Hu3DModelPosSet(lbl_1_bss_62[lbl_1_bss_10], 500.0 * sin(0.0), 157.0f, t);
    } else {
loop_13:
        if (lbl_1_data_2C != 3) {
            HuPrcVSleep();
            goto loop_13;
        }
        for (i = 0; i <= 30; i++) {
            if (i > 10) {
                for (j = 0; j < 4; j++) {
                    E3_SKIP_NOCHR(lbl_1_bss_5A[j])
                    pos = lbl_1_data_278[j];
                    if (pos.x < 0.0f) {
                        sgn = -1.0;
                    } else {
                        sgn = 1.0;
                    }
                    pos.x = pos.x + 250.0 * (sgn * (1.0 - sin(3.141592653589793 * (90.0f * (float)((i - 10) / 20.0)) / 180.0)));
                    Hu3DModelPosSet(lbl_1_bss_76[lbl_1_bss_5A[j]], pos.x, pos.y, pos.z);
                }
            }
            t = 1.5 * sin(3.141592653589793 * (90.0 * (i / 30.0)) / 180.0);
            scl.z = t;
            scl.y = t;
            scl.x = t;
            for (j = 0; j < 8; j++) {
                Hu3DModelScaleSetV(lbl_1_bss_62[j], &scl);
            }
            HuPrcVSleep();
        }
    }
    amp = 0.0;
loop_33:
    for (i = 0; i < 4; i++) {
        E3_SKIP_NOCHR(lbl_1_bss_5A[i])
        pos = lbl_1_data_278[i];
        Hu3DModelPosSet(lbl_1_bss_76[lbl_1_bss_5A[i]], pos.x, pos.y + amp * (3.0 * sin(3.141592653589793 * (ang + (float)(i * 120)) / 180.0)), pos.z);
    }
    for (i = 0; i < 8; i++) {
        modelP = &Hu3DData[lbl_1_bss_62[i]];
        modelP->pos.y = 157.0 + amp * (3.0 * sin(3.141592653589793 * (ang + (float)(i * 45)) / 180.0));
    }
    ang += 3.0f;
    if (ang > 720.0f) {
        ang -= 720.0f;
    }
    amp = amp + 0.05;
    if (amp > 1.0f) {
        amp = 1.0f;
    }
    if (lbl_1_data_2C != 5) {
        if (lbl_1_data_2C == 1) {
            for (i = 0; i <= 30; i++) {
                if (i <= 20) {
                    tgt.y = 0.0f;
                    tgt.x = 0.0f;
                    tgt.z = 1600.0f;
                    for (j = 0; j < 4; j++) {
                        E3_SKIP_NOCHR(lbl_1_bss_5A[j])
                        pos = lbl_1_data_278[j];
                        if (pos.x < 0.0f) {
                            sgn = -1.0;
                        } else {
                            sgn = 1.0;
                        }
                        pos.x = pos.x + 300.0 * (sgn * (1.0 - cos(3.141592653589793 * (90.0f * (float)(i / 20.0)) / 180.0)));
                        Hu3DModelPosSet(lbl_1_bss_76[lbl_1_bss_5A[j]], pos.x, pos.y, pos.z);
                        PSVECSubtract(&tgt, &pos, &diff);
                        Hu3DModelRotSet(lbl_1_bss_76[lbl_1_bss_5A[j]], 0.0f, 180.0 * (atan2(diff.x, diff.z) / 3.141592653589793), 0.0f);
                    }
                }
                t = cos(3.141592653589793 * (90.0 * (i / 30.0)) / 180.0);
                for (j = 0; j < 8; j++) {
                    if (lbl_1_bss_10 == j) {
                        PSVECScale(&lbl_1_data_C8, &scl, t);
                    } else {
                        t2 = 1.5f * t;
                        scl.z = t2;
                        scl.y = t2;
                        scl.x = t2;
                    }
                    Hu3DModelScaleSetV(lbl_1_bss_62[j], &scl);
                }
                HuPrcVSleep();
            }
            for (j = 0; j < 8; j++) {
                Hu3DModelMatHookSet(lbl_1_bss_62[j], NULL);
            }
            goto loop_8;
        }
        HuPrcVSleep();
        goto loop_33;
    }
    for (i = 0; i < 4; i++) {
        E3_SKIP_NOCHR(lbl_1_bss_5A[i])
        mdl = lbl_1_bss_EE[lbl_1_bss_5A[i]];
        Hu3DModelCameraSet(mdl, 2);
        Hu3DMotionSet(mdl, lbl_1_bss_8A[lbl_1_bss_5A[i]][1]);
        pos = lbl_1_data_278[i];
        pos.y -= 70.0f;
        Hu3DModelPosSetV(mdl, &pos);
        Hu3DModelScaleSet(mdl, 1.0f, 1.0f, 1.0f);
        Hu3DModelAttrReset(mdl, 0x40000001);
        Hu3DMotionTimeSet(mdl, Hu3DMotionMaxTimeGet(mdl));
        Hu3DModelAttrSet(lbl_1_bss_76[lbl_1_bss_5A[i]], 1);
        lbl_1_data_D4[i] = -1;
        parA = grpA[i];
        Hu3DParManPosSet(parA[0], lbl_1_data_278[i].x, lbl_1_data_278[i].y, lbl_1_data_278[i].z);
        Hu3DParManTimeLimitSet(parA[0], 2);
        Hu3DParManAttrReset(parA[0], 1);
        Hu3DParManPosSet(parA[1], lbl_1_data_278[i].x, lbl_1_data_278[i].y, lbl_1_data_278[i].z);
        Hu3DParManTimeLimitSet(parA[1], 2);
        Hu3DParManAttrReset(parA[1], 1);
        Hu3DParManPosSet(parA[2], lbl_1_data_278[i].x, lbl_1_data_278[i].y, lbl_1_data_278[i].z);
        Hu3DParManTimeLimitSet(parA[2], 5);
        Hu3DParManAttrReset(parA[2], 1);
    }
    amp2 = 1.0f;
    for (k = 0; k <= 50; k++) {
        t = k / 50.0;
        for (i = 0; i < 4; i++) {
            E3_SKIP_NOCHR(lbl_1_bss_5A[i])
            tgt.x = 0.0f;
            tgt.y = 157.0f;
            tgt.z = 0.0f;
            tgt.x = tgt.x + 0.3 * lbl_1_data_278[i].x;
            tgt.y = tgt.y + 0.3 * lbl_1_data_278[i].y;
            tgt.z -= 300.0f;
            pos = lbl_1_data_278[i];
            pos.y -= 70.0f;
            PSVECSubtract(&tgt, &pos, &diff);
            if (diff.x < 0.0f) {
                j = -1;
            } else {
                j = 1;
            }
            Hu3DModelRotSet(lbl_1_bss_EE[lbl_1_bss_5A[i]], 70.0f * t, 180.0f * t * j, 0.0f);
            PSVECScale(&diff, &diff, t);
            PSVECAdd(&pos, &diff, &diff);
            diff.z = diff.z + 300.0 * sin(3.141592653589793 * (180.0f * t) / 180.0);
            diff.y = diff.y + 100.0 * sin(3.141592653589793 * (180.0f * t) / 180.0);
            Hu3DModelPosSetV(lbl_1_bss_EE[lbl_1_bss_5A[i]], &diff);
            if (t > 0.5) {
                Hu3DModelScaleSet(lbl_1_bss_EE[lbl_1_bss_5A[i]], amp2, amp2, amp2);
            }
            parB = grpB[i];
            Hu3DParManPosSet(parB[0], diff.x, diff.y, diff.z);
            Hu3DParManTimeLimitSet(parB[0], 2);
            Hu3DParManAttrReset(parB[0], 1);
        }
        if (t > 0.5) {
            amp2 = amp2 - 0.02;
        }
        HuPrcVSleep();
    }
    while (1) {
        HuPrcVSleep();
    }
}

void fn_1_4DD8(s16 arg)
{
    HUPROCESS *proc;

    while (lbl_1_bss_28 != 0) {
        HuPrcVSleep();
    }
    lbl_1_bss_28 = 1;
    proc = HuPrcChildCreate(fn_1_4E58, 0xC8, 0x2000, 0, HuPrcCurrentGet());
    proc->property = (void *)(uintptr_t)arg;
}

void fn_1_4E58(void)
{
    HuVecF scl;
    s16 i;
    s16 j;
    s16 k;
    s16 dir;
    s32 sgn;
    float t;
    float ang;
    float ang2;
    float z;
    void *prop;

    prop = HuPrcCurrentGet()->property;
    dir = (s16)(uintptr_t)prop;
    if (dir != 0) {
        HuTHPClose();
        Hu3DModelMatHookSet(lbl_1_bss_62[lbl_1_bss_10], NULL);
        ang = lbl_1_bss_10 * 45;
        for (i = 1; i <= 10; i++) {
            t = i / 10.0;
            z = 600.0 * cos(0.0) - 600.0 - 300.0 * sin(3.141592653589793 * (90.0f * t) / 180.0);
            Hu3DModelPosSet(lbl_1_bss_62[lbl_1_bss_10], 500.0 * sin(0.0), 157.0f, z);
            scl.x = 1.5 + (lbl_1_data_C8.x - 1.5f) * cos(3.141592653589793 * (90.0f * t) / 180.0);
            scl.y = 1.5 + (lbl_1_data_C8.y - 1.5f) * cos(3.141592653589793 * (90.0f * t) / 180.0);
            scl.z = 1.5 + (lbl_1_data_C8.z - 1.5f) * cos(3.141592653589793 * (90.0f * t) / 180.0);
            Hu3DModelScaleSetV(lbl_1_bss_62[lbl_1_bss_10], &scl);
            HuSprGrpScaleSet(lbl_1_bss_12A, 0.9f, 1.0 - t);
            HuPrcVSleep();
        }
        for (j = 1; j <= 10; j++) {
            if (dir < 0) {
                sgn = -1;
            } else {
                sgn = 1;
            }
            ang2 = ang + 45.0 * (j / 10.0) * sgn;
            for (k = 0; k < 8; k++) {
                z = 600.0 * cos(3.141592653589793 * (ang2 - (float)(k * 45)) / 180.0) - 600.0 - 300.0;
                Hu3DModelPosSet(lbl_1_bss_62[k], 500.0 * sin(3.141592653589793 * (ang2 - (float)(k * 45)) / 180.0), 157.0f, z);
            }
            HuPrcVSleep();
        }
        lbl_1_bss_10 += dir;
        if (lbl_1_bss_10 >= 8) {
            lbl_1_bss_10 = 0;
        }
        if (lbl_1_bss_10 < 0) {
            lbl_1_bss_10 = 7;
        }
        HuSprGrpKill(lbl_1_bss_12A);
        lbl_1_bss_108 = &mgDataTbl[lbl_1_data_88[lbl_1_bss_10].mgNo - 1];
        lbl_1_bss_12A = MgNameCreate((void *)(uintptr_t)lbl_1_bss_108->nameMes);
        HuSprGrpPosSet(lbl_1_bss_12A, 288.0f, 64.0f);
        HuSprGrpScaleSet(lbl_1_bss_12A, 0.9f, 0.0f);
        lbl_1_bss_8 = 1;
    }
    if (Hu3DData[lbl_1_bss_62[lbl_1_bss_10]].scale.x != lbl_1_data_C8.x) {
loop_21:
        if (HuTHPProcCheck() != 0) {
            HuPrcVSleep();
            goto loop_21;
        }
        HuTHPSprCreateVol(lbl_1_bss_108->movie[0], 1, 100, 0.0f);
        Hu3DModelMatHookSet(lbl_1_bss_62[lbl_1_bss_10], HuTHPMatHook);
        for (i = 1; i <= 10; i++) {
            t = i / 10.0;
            z = 600.0 * cos(0.0) - 600.0 - 300.0 * cos(3.141592653589793 * (90.0f * t) / 180.0);
            Hu3DModelPosSet(lbl_1_bss_62[lbl_1_bss_10], 500.0 * sin(0.0), 157.0f, z);
            scl.x = 1.5 + (lbl_1_data_C8.x - 1.5f) * sin(3.141592653589793 * (90.0f * t) / 180.0);
            scl.y = 1.5 + (lbl_1_data_C8.y - 1.5f) * sin(3.141592653589793 * (90.0f * t) / 180.0);
            scl.z = 1.5 + (lbl_1_data_C8.z - 1.5f) * sin(3.141592653589793 * (90.0f * t) / 180.0);
            Hu3DModelScaleSetV(lbl_1_bss_62[lbl_1_bss_10], &scl);
            if (dir != 0) {
                HuSprGrpScaleSet(lbl_1_bss_12A, 0.9f, t);
            }
            HuPrcVSleep();
        }
    } else {
loop_29:
        if (HuTHPProcCheck() != 0) {
            HuPrcVSleep();
            goto loop_29;
        }
        HuTHPSprCreateVol(lbl_1_bss_108->movie[0], 1, 100, 0.0f);
        Hu3DModelMatHookSet(lbl_1_bss_62[lbl_1_bss_10], HuTHPMatHook);
    }
    lbl_1_bss_28 = 0;
    HuPrcEnd();
    while (1) {
        HuPrcVSleep();
    }
}

void fn_1_5904(void)
{
    float winW;
    float winH;
    float posX;
    float posY;
    float w2;
    float h2;
    float posX2;
    float posY2;
    GXColor bgCol = {};
    ANIMDATA *anims[2];
    s16 winA[2];
    s16 winB[2];
    s16 sprs[2][2];
    HUSPRITE *sprP;
    HUWIN *winP;
    float t;
    float t2;
    float t3;
    s16 i;
    s16 j;
    s16 k;
    s32 msgA;
    s32 msgB;
    s16 sel;

    anims[0] = lbl_1_bss_2C[0];
    anims[1] = lbl_1_bss_2C[1];
    for (i = 0; i < 2; i++) {
        winW = 400.0f;
        winH = 128.0f;
        posX = 560.0f - winW;
        posY = 440.0f - winH;
        winA[i] = HuWinCreate(posX, posY, winW, winH, 0);
        HuWinMesSpeedSet(winA[i], 0);
        HuWinMesSet(winA[i], lbl_1_bss_108->instMes[0][0]);
        w2 = anims[0]->bmp->sizeX;
        h2 = 48.0f;
        posX2 = 560.0f - w2;
        posY2 = 10.0f + (440.0f - winH - h2);
        winB[i] = HuWinCreate(posX2, posY2, w2, h2, 0);
        HuWinAttrSet(winB[i], 0x800);
        HuWinMesSpeedSet(winB[i], 0);
        HuWinMesPalSet(winB[i], 7, 0xFF, 0xFF, 0);
        HuWinMesSet(winB[i], lbl_1_data_2A8[0]);
        winP = &winData[winB[i]];
        for (j = 0; j < 2; j++) {
            sprs[i][j] = HuWinAnimSet(winB[i], anims[j], 0, 0.0f, 0.0f);
            HuWinSprPriSet(winB[i], sprs[i][j], winP->prio + 1);
            HuWinSprPosSet(winB[i], sprs[i][j], w2 * 0.5f, h2 * 0.5f - 4.0f);
        }
        sprP = &HuSprData[HuWinSprIDGet(winB[i], sprs[i][1])];
        sprP->r = bgCol.r;
        sprP->g = bgCol.g;
        sprP->b = bgCol.b;
        sprP->a = 178.5f;
        HuWinBGColSet(winB[i], &bgCol);
        HuWinBGTPLvlSet(winB[i], 0.0f);
        HuWinDispOff(winA[i]);
        HuWinDispOff(winB[i]);
    }
loop_8:
    if (lbl_1_data_2C < 2) {
        HuPrcVSleep();
        goto loop_8;
    }
    HuWinDispOn(winA[0]);
    HuWinDispOn(winB[0]);
    if (lbl_1_data_2C == 2 || lbl_1_data_2C == 3) {
        HuWinPosSet(winB[0], 1000.0f, 0.0f);
        HuWinPosSet(winA[0], 1000.0f, 0.0f);
loop_13:
        if (lbl_1_data_2C != 3) {
            HuPrcVSleep();
            goto loop_13;
        }
        HuPrcSleep(20);
        for (i = 0; i <= 20; i++) {
            HuWinPosSet(winB[0], posX2 + 400.0 * (1.0 - sin(3.141592653589793 * (90.0 * (i / 20.0)) / 180.0)), posY2);
            HuWinPosSet(winA[0], posX, posY + 200.0 * (1.0 - sin(3.141592653589793 * (90.0 * (i / 20.0)) / 180.0)));
            HuPrcVSleep();
        }
    }
    sel = 0;
    msgA = lbl_1_data_2A8[0];
    msgB = lbl_1_bss_108->instMes[0][0];
loop_18:
    if ((lbl_1_bss_122 & 0x2000) != 0) {
        HuAudFXPlay(0);
        lbl_1_bss_C = 1;
        HuWinMesSet(winB[1], msgA);
        HuWinMesSet(winA[1], msgB);
        HuWinDispOn(winA[1]);
        HuWinDispOn(winB[1]);
        HuWinPosSet(winB[1], 1000.0f, 0.0f);
        HuWinPosSet(winA[1], 1000.0f, 0.0f);
        HuPrcVSleep();
        sel++;
        if (sel > 4) {
            sel = 0;
        }
        if (sel == 2 && lbl_1_bss_108->instMes[0][2] == 0) {
            sel++;
        }
        if (sel == 4 && lbl_1_bss_108->instMes[0][4] == 0) {
            sel = 0;
        }
        msgA = lbl_1_data_2A8[sel];
        if (sel == 1 && lbl_1_bss_108->instMes[0][2] == 0) {
            msgA = 0x420007;
        }
        if (sel == 3 && lbl_1_bss_108->instMes[0][4] == 0) {
            msgA = 0x420008;
        }
        HuWinMesSet(winB[0], msgA);
        msgB = lbl_1_bss_108->instMes[0][sel];
        HuWinMesSet(winA[0], msgB);
        for (i = 0; i < 15; i++) {
            t = i / 15.0;
            HuWinPosSet(winB[1], posX2 + 30.0 * sin(3.141592653589793 * (90.0f * t) / 180.0), posY2 + 200.0 * (1.0 - cos(3.141592653589793 * (90.0f * t) / 180.0)));
            HuWinPosSet(winA[1], posX + 30.0 * sin(3.141592653589793 * (90.0f * t) / 180.0), posY + 200.0 * (1.0 - cos(3.141592653589793 * (90.0f * t) / 180.0)));
            HuPrcVSleep();
        }
        HuWinDispOff(winA[1]);
        HuWinDispOff(winB[1]);
        lbl_1_bss_C = 0;
    }
    if (lbl_1_bss_8 != 0) {
        lbl_1_bss_8 = 0;
        sel = 0;
        for (j = 1; j <= 20; j++) {
            t2 = j / 20.0;
            HuWinPosSet(winB[0], posX2 + 500.0 * sin(3.141592653589793 * (180.0f * t2) / 180.0), posY2);
            HuWinPosSet(winA[0], posX + 500.0 * sin(3.141592653589793 * (180.0f * t2) / 180.0), posY);
            if (j == 10) {
                msgA = lbl_1_data_2A8[0];
                msgB = lbl_1_bss_108->instMes[0][0];
                HuWinMesSet(winB[0], msgA);
                HuWinMesSet(winA[0], msgB);
            }
            HuPrcVSleep();
        }
    }
    if (lbl_1_data_2C != 5) {
        if (lbl_1_data_2C == 1) {
            for (k = 0; k <= 20; k++) {
                HuWinPosSet(winB[0], posX2 + 400.0 * (1.0 - cos(3.141592653589793 * (90.0 * (k / 20.0)) / 180.0)), posY2);
                HuWinPosSet(winA[0], posX, posY + 200.0 * (1.0 - cos(3.141592653589793 * (90.0 * (k / 20.0)) / 180.0)));
                HuPrcVSleep();
            }
            HuWinDispOff(winA[0]);
            HuWinDispOff(winB[0]);
            goto loop_8;
        }
        HuPrcVSleep();
        goto loop_18;
    }
    for (k = 0; k <= 10; k++) {
        HuWinPosSet(winB[0], posX2 + 400.0 * (1.0 - cos(3.141592653589793 * (90.0 * (k / 10.0)) / 180.0)), posY2);
        HuWinPosSet(winA[0], posX, posY + 200.0 * (1.0 - cos(3.141592653589793 * (90.0 * (k / 10.0)) / 180.0)));
        HuPrcVSleep();
    }
    while (1) {
        HuPrcVSleep();
    }
}

void fn_1_6804(void)
{
    HuVecF pos2D;
    HuVecF pos3D;
    float t;
    s16 i;
    s16 j;
    s16 state;

    for (i = 0; i < 4; i++) {
        if (HuPadStatGet(i) != 0) {
            break;
        }
    }
    lbl_1_bss_54 = i;
    if (lbl_1_bss_104 != 0) {
        state = 2;
        for (i = 0; i < 4; i++) {
            if (GwPlayerConf[i].type != 0) {
                break;
            }
        }
        lbl_1_bss_58 = i;
        lbl_1_bss_56 = 4 - i;
        for (i = 0; i < 4; i++) {
            HuSprAttrReset(lbl_1_bss_11C, i + 15, 4);
            HuSprPosSet(lbl_1_bss_11C, i + 15, 128.0f + (lbl_1_bss_5A[i] % 5) * 80 - 10.0f, (lbl_1_bss_5A[i] / 5) * 90 + 210);
            if (GwPlayerConf[i].type == 0) {
                HuSprBankSet(lbl_1_bss_11C, i + 15, i);
            } else {
                HuSprBankSet(lbl_1_bss_11C, i + 15, 4);
            }
            lbl_1_data_D4[i] = lbl_1_bss_5A[i];
            lbl_1_bss_34[i] = lbl_1_bss_5A[i];
        }
        HuSprGrpPosSet(lbl_1_bss_11C, 1000.0f, 0.0f);
    } else {
        state = 0;
        lbl_1_bss_58 = lbl_1_bss_54;
        lbl_1_bss_56 = 4 - lbl_1_bss_54;
        for (i = 0; i < 4; i++) {
            lbl_1_bss_5A[i] = -1;
        }
    }
loop_21:
    if (lbl_1_data_2C != 0) {
        HuPrcVSleep();
        goto loop_21;
    }
    HuSprAttrReset(lbl_1_bss_11C, 0, 4);
    HuSprAttrReset(lbl_1_bss_11C, 1, 4);
    HuSprPosSet(lbl_1_bss_11C, 5, 288.0f, 406.0f);
    HuSprAttrReset(lbl_1_bss_11C, 5, 4);
    HuSprPosSet(lbl_1_bss_11C, 6, 288.0f, 406.0f);
    HuSprAttrReset(lbl_1_bss_11C, 6, 4);
    for (i = 0; i <= 30; i++) {
        t = 480.0 * (1.0 - sin(3.141592653589793 * (90.0 * (i / 30.0)) / 180.0));
        HuSprGrpPosSet(lbl_1_bss_11C, 0.0f, t);
        for (j = 0; j < 10; j++) {
            pos2D.x = 128.0f + (j % 5) * 80;
            pos2D.y = t + ((j / 5) * 90 + 220);
            pos2D.z = 1600.0f;
            Hu3D2Dto3D(&pos2D, 2, &pos3D);
            Hu3DModelPosSetV(lbl_1_bss_76[j], &pos3D);
            Hu3DModelScaleSet(lbl_1_bss_76[j], 0.7f, 0.7f, 0.3f);
            Hu3DModelRotSet(lbl_1_bss_76[j], 0.0f, 0.0f, 0.0f);
        }
        fn_1_71AC(t);
        HuPrcVSleep();
    }
loop_28:
    switch (state) {
        case 0:
            if (fn_1_7374() == 0) {
                state--;
            } else {
                state++;
            }
            break;
        case 1:
            if (fn_1_7AF0() == 0) {
                state--;
            } else {
                state++;
            }
            break;
        case 2:
            if (fn_1_95B4() == 0) {
                state--;
            } else {
                state++;
            }
            break;
    }
    if (state >= 0 && state <= 2) {
        goto loop_28;
    }
    if (state < 0) {
        fn_1_D30(1);
        while (1) {
            HuPrcVSleep();
        }
    }
    for (i = 0; i <= 60; i++) {
        t = 480.0 * (1.0 - cos(3.141592653589793 * (90.0 * (i / 60.0)) / 180.0));
        HuSprGrpPosSet(lbl_1_bss_11C, 0.0f, t);
        for (j = 0; j < 10; j++) {
            pos2D.x = 128.0f + (j % 5) * 80;
            pos2D.y = t + ((j / 5) * 90 + 220);
            pos2D.z = 1600.0f;
            Hu3D2Dto3D(&pos2D, 2, &pos3D);
            Hu3DModelPosSetV(lbl_1_bss_76[j], &pos3D);
        }
        fn_1_71AC(t);
        HuPrcVSleep();
    }
    for (i = 0; i < 10; i++) {
        Hu3DMotionSet(lbl_1_bss_EE[i], lbl_1_bss_8A[i][0]);
        Hu3DModelAttrSet(lbl_1_bss_EE[i], 0x40000001);
    }
    while (lbl_1_bss_14 != 0) {
        HuPrcVSleep();
    }
    lbl_1_data_2C = 2;
    state = 2;
    goto loop_21;
}

void fn_1_71AC(float arg)
{
    HuVecF pos2D;
    HuVecF pos3D;
    s16 row;
    s16 i;

    row = 0;
    for (i = 0; i < 4; i++) {
        Hu3DModelAttrSet(lbl_1_bss_10C[0][i], 1);
        Hu3DModelAttrSet(lbl_1_bss_10C[1][i], 1);
        if (lbl_1_bss_58 <= i) {
            row = 1;
        }
        Hu3DModelAttrReset(lbl_1_bss_10C[row][i], 1);
        pos2D.x = 168.0f + i * 80;
        pos2D.y = 98.0f + arg;
        pos2D.z = 1600.0f;
        Hu3D2Dto3D(&pos2D, 2, &pos3D);
        Hu3DModelPosSetV(lbl_1_bss_10C[row][i], &pos3D);
        Hu3DModelScaleSet(lbl_1_bss_10C[row][i], 0.7f, 0.7f, 0.7f);
    }
}

s32 fn_1_7374(void)
{
    HuVecF pos2D;
    HuVecF pos3D;
    s16 row;
    s16 i;
    s16 j;

    HuSprAttrReset(lbl_1_bss_11C, 7, 4);
    HuSprPosSet(lbl_1_bss_11C, 7, 108.0f, 98.0f);
    HuSprAttrReset(lbl_1_bss_11C, 8, 4);
    HuSprPosSet(lbl_1_bss_11C, 8, 468.0f, 98.0f);
    HuSprAttrReset(lbl_1_bss_11C, 2, 4);
    HuSprAttrSet(lbl_1_bss_11C, 3, 4);
    HuSprAttrSet(lbl_1_bss_11C, 4, 4);
    while (1) {
        HuPrcVSleep();
        if ((HuPadDStkRep[0] & 1) != 0) {
            HuAudFXPlay(0);
            lbl_1_bss_58 -= 1;
            if (lbl_1_bss_58 < 1) {
                lbl_1_bss_58 = lbl_1_bss_54;
            }
            lbl_1_bss_44[0] = 1;
            for (i = 1; i <= 10; i++) {
                HuSprPosSet(lbl_1_bss_11C, 7, 108.0f - 20.0f * (float)sin(3.141592653589793 * (180.0 * (i / 10.0)) / 180.0), 98.0f);
                if (i == 5) {
                    row = 0;
                    for (j = 0; j < 4; j++) {
                        Hu3DModelAttrSet(lbl_1_bss_10C[0][j], 1);
                        Hu3DModelAttrSet(lbl_1_bss_10C[1][j], 1);
                        if (lbl_1_bss_58 <= j) {
                            row = 1;
                        }
                        Hu3DModelAttrReset(lbl_1_bss_10C[row][j], 1);
                        pos2D.x = 168.0f + j * 80;
                        pos2D.y = 98.0f;
                        pos2D.z = 1600.0f;
                        Hu3D2Dto3D(&pos2D, 2, &pos3D);
                        Hu3DModelPosSetV(lbl_1_bss_10C[row][j], &pos3D);
                        Hu3DModelScaleSet(lbl_1_bss_10C[row][j], 0.7f, 0.7f, 0.7f);
                    }
                }
                HuPrcVSleep();
            }
            lbl_1_bss_44[0] = 0;
        }
        if ((HuPadDStkRep[0] & 2) != 0) {
            HuAudFXPlay(0);
            lbl_1_bss_58 += 1;
            if (lbl_1_bss_58 > lbl_1_bss_54) {
                lbl_1_bss_58 = 1;
            }
            lbl_1_bss_44[1] = 1;
            for (i = 1; i <= 10; i++) {
                HuSprPosSet(lbl_1_bss_11C, 8, 468.0f + 20.0f * (float)sin(3.141592653589793 * (180.0 * (i / 10.0)) / 180.0), 98.0f);
                if (i == 5) {
                    row = 0;
                    for (j = 0; j < 4; j++) {
                        Hu3DModelAttrSet(lbl_1_bss_10C[0][j], 1);
                        Hu3DModelAttrSet(lbl_1_bss_10C[1][j], 1);
                        if (lbl_1_bss_58 <= j) {
                            row = 1;
                        }
                        Hu3DModelAttrReset(lbl_1_bss_10C[row][j], 1);
                        pos2D.x = 168.0f + j * 80;
                        pos2D.y = 98.0f;
                        pos2D.z = 1600.0f;
                        Hu3D2Dto3D(&pos2D, 2, &pos3D);
                        Hu3DModelPosSetV(lbl_1_bss_10C[row][j], &pos3D);
                        Hu3DModelScaleSet(lbl_1_bss_10C[row][j], 0.7f, 0.7f, 0.7f);
                    }
                }
                HuPrcVSleep();
            }
            lbl_1_bss_44[1] = 0;
        }
        if ((HuPadBtnDown[0] & 0x100) != 0) {
            HuAudFXPlay(0);
            HuSprAttrSet(lbl_1_bss_11C, 7, 4);
            HuSprAttrSet(lbl_1_bss_11C, 8, 4);
            return 1;
        }
        if ((HuPadBtnDown[0] & 0x200) != 0) {
            HuAudFXPlay(0);
            return 0;
        }
    }
}

s32 fn_1_7AF0(void)
{
    s16 done[4];
    s16 sel[4];
    s16 row[4];
    s16 col[4];
    s16 grid[5][2];
    s16 i;
    s16 j;
    s16 p;
    s16 dx;
    s16 dy;
    s16 k;

    lbl_1_bss_18 = 1;
    HuSprAttrSet(lbl_1_bss_11C, 2, 4);
    HuSprAttrReset(lbl_1_bss_11C, 3, 4);
    HuSprAttrSet(lbl_1_bss_11C, 4, 4);
    for (i = 0; i < 4; i++) {
        sel[i] = -1;
        col[i] = 0;
        row[i] = 0;
        done[i] = 0;
        if (lbl_1_bss_5A[i] != -1) {
            Hu3DMotionShiftSet(lbl_1_bss_EE[lbl_1_bss_5A[i]], lbl_1_bss_8A[lbl_1_bss_5A[i]][0], 0.0f, 8.0f, 0x40000001);
            lbl_1_data_D4[i] = lbl_1_bss_5A[i];
            lbl_1_bss_34[i] = lbl_1_bss_5A[i];
            Hu3DModelAttrReset(lbl_1_bss_3C[i], 1);
        }
    }
loop_5:
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 2; j++) {
            grid[i][j] = 0;
        }
    }
    grid[4][1] = 1;
    grid[3][1] = 1;
    for (i = 0; i < 4; i++) {
        HuSprAttrSet(lbl_1_bss_11C, i + 15, 4);
        HuSprAttrSet(lbl_1_bss_11C, i + 11, 4);
        if (done[i] != 0) {
            Hu3DMotionShiftSet(lbl_1_bss_EE[sel[i]], lbl_1_bss_8A[sel[i]][0], 0.0f, 8.0f, 0x40000001);
        }
        lbl_1_data_D4[i] = -1;
        lbl_1_bss_34[i] = -1;
    }
    for (i = 0; i < lbl_1_bss_58; i++) {
        if (sel[i] == -1) {
            sel[i] = i;
            col[i] = i;
            row[i] = 0;
        }
        grid[col[i]][row[i]] = 1;
        done[i] = 0;
        HuSprAttrReset(lbl_1_bss_11C, i + 11, 4);
        HuSprBankSet(lbl_1_bss_11C, i + 11, i);
        HuSprPosSet(lbl_1_bss_11C, i + 11, 128.0f + (sel[i] % 5) * 80 - 20.0f, (sel[i] / 5) * 90 + 200);
    }
loop_21:
    HuPrcVSleep();
    for (i = 0; i < lbl_1_bss_58; i++) {
        if (done[i] == 0) {
            dy = 0;
            dx = 0;
            if ((HuPadDStkRep[i] & 1) != 0) {
                dx = -1;
            }
            if ((HuPadDStkRep[i] & 2) != 0) {
                dx = 1;
            }
            if ((HuPadDStkRep[i] & 8) != 0) {
                dy = -1;
            }
            if ((HuPadDStkRep[i] & 4) != 0) {
                dy = 1;
            }
            if (dx != 0 || dy != 0) {
                HuAudFXPlay(0);
            }
            grid[col[i]][row[i]] = 0;
            col[i] += dx;
            if (col[i] < 0) {
                col[i] = 0;
            }
            if (col[i] >= 5) {
                col[i] = 4;
            }
            row[i] += dy;
            if (row[i] < 0) {
                row[i] = 0;
            }
            if (row[i] >= 2) {
                row[i] = 1;
            }
            if (grid[col[i]][row[i]] != 0) {
                if (dx != 0) {
                    if (dx > 0) {
                        for (k = col[i]; k < 5; k++) {
                            if (grid[k][row[i]] == 0) {
                                break;
                            }
                        }
                        if (k == 5) {
                            col[i] -= dx;
                        } else {
                            col[i] = k;
                        }
                    } else {
                        for (k = col[i]; k >= 0; k--) {
                            if (grid[k][row[i]] == 0) {
                                break;
                            }
                        }
                        if (k < 0) {
                            col[i] -= dx;
                        } else {
                            col[i] = k;
                        }
                    }
                } else {
                    row[i] -= dy;
                }
            }
            if (grid[col[i]][row[i]] != 0) {
                col[i] = col[i];
                row[i] = row[i];
            }
            sel[i] = col[i] + row[i] * 5;
            grid[col[i]][row[i]] = 1;
            HuSprPosSet(lbl_1_bss_11C, i + 11, 128.0f + (sel[i] % 5) * 80 - 20.0f, (sel[i] / 5) * 90 + 200);
            if ((HuPadBtnDown[i] & 0x100) != 0) {
                HuAudFXPlay(lbl_1_data_2BC[sel[i]]);
                HuSprAttrSet(lbl_1_bss_11C, i + 11, 4);
                HuSprAttrReset(lbl_1_bss_11C, i + 15, 4);
                HuSprBankSet(lbl_1_bss_11C, i + 15, i);
                HuSprPosSet(lbl_1_bss_11C, i + 15, 128.0f + (sel[i] % 5) * 80 - 10.0f, (sel[i] / 5) * 90 + 210);
                Hu3DMotionShiftSet(lbl_1_bss_EE[sel[i]], lbl_1_bss_8A[sel[i]][2], 0.0f, 8.0f, 0);
                lbl_1_data_D4[i] = sel[i];
                lbl_1_bss_34[i] = sel[i];
                Hu3DModelAttrReset(lbl_1_bss_3C[i], 1);
                done[i] = 1;
            }
            if ((HuPadBtnDown[0] & 0x200) != 0) {
                HuAudFXPlay(0);
                for (j = 0; j < 4; j++) {
                    HuSprAttrSet(lbl_1_bss_11C, j + 15, 4);
                    HuSprAttrSet(lbl_1_bss_11C, j + 11, 4);
                    lbl_1_data_D4[j] = -1;
                    lbl_1_bss_34[j] = -1;
                    if (done[j] != 0) {
                        Hu3DMotionShiftSet(lbl_1_bss_EE[sel[j]], lbl_1_bss_8A[sel[j]][0], 0.0f, 8.0f, 0x40000001);
                    }
                }
                return 0;
            }
        }
    }
    for (k = 0; k < lbl_1_bss_58; k++) {
        if (done[k] == 0) {
            break;
        }
    }
    if (k != lbl_1_bss_58) {
        goto loop_21;
    }
    for (p = lbl_1_bss_58; ; ) {
loop_139:
        if (p >= 4) {
            for (i = 0; i < 4; i++) {
                lbl_1_bss_5A[i] = sel[i];
            }
            HuPrcChildCreate(fn_1_A6E8, 0x7D0, 0x1000, 0, HuPrcCurrentGet());
            return 1;
        }
        if (sel[p] != -1 && grid[col[p]][row[p]] != 0) {
            sel[p] = -1;
        }
        if (sel[p] == -1) {
            for (i = 0; i < 5; i++) {
                if (grid[i][0] == 0) {
                    sel[p] = i;
                    col[p] = i;
                    row[p] = 0;
                    break;
                }
            }
        }
        grid[col[p]][row[p]] = 1;
        HuSprAttrReset(lbl_1_bss_11C, p + 11, 4);
        HuSprBankSet(lbl_1_bss_11C, p + 11, 4);
        HuSprPosSet(lbl_1_bss_11C, p + 11, 128.0f + (sel[p] % 5) * 80 - 20.0f, (sel[p] / 5) * 90 + 200);
        HuSprAttrSet(lbl_1_bss_11C, p + 15, 4);
loop_89:
        HuPrcVSleep();
        dy = 0;
        dx = 0;
        if ((HuPadDStkRep[0] & 1) != 0) {
            dx = -1;
        }
        if ((HuPadDStkRep[0] & 2) != 0) {
            dx = 1;
        }
        if ((HuPadDStkRep[0] & 8) != 0) {
            dy = -1;
        }
        if ((HuPadDStkRep[0] & 4) != 0) {
            dy = 1;
        }
        if (dx != 0 || dy != 0) {
            HuAudFXPlay(0);
        }
        grid[col[p]][row[p]] = 0;
        col[p] += dx;
        if (col[p] < 0) {
            col[p] = 0;
        }
        if (col[p] >= 5) {
            col[p] = 4;
        }
        row[p] += dy;
        if (row[p] < 0) {
            row[p] = 0;
        }
        if (row[p] >= 2) {
            row[p] = 1;
        }
        if (grid[col[p]][row[p]] != 0) {
            if (dx != 0) {
                if (dx > 0) {
                    for (k = col[p]; k < 5; k++) {
                        if (grid[k][row[p]] == 0) {
                            break;
                        }
                    }
                    if (k == 5) {
                        col[p] -= dx;
                    } else {
                        col[p] = k;
                    }
                } else {
                    for (k = col[p]; k >= 0; k--) {
                        if (grid[k][row[p]] == 0) {
                            break;
                        }
                    }
                    if (k < 0) {
                        col[p] -= dx;
                    } else {
                        col[p] = k;
                    }
                }
            } else {
                row[p] -= dy;
            }
        }
        if (grid[col[p]][row[p]] != 0) {
            col[p] = col[p];
            row[p] = row[p];
        }
        sel[p] = col[p] + row[p] * 5;
        grid[col[p]][row[p]] = 1;
        HuSprPosSet(lbl_1_bss_11C, p + 11, 128.0f + (sel[p] % 5) * 80 - 20.0f, (sel[p] / 5) * 90 + 200);
        if ((HuPadBtnDown[0] & 0x100) != 0) {
            HuAudFXPlay(lbl_1_data_2BC[sel[p]]);
            HuSprAttrSet(lbl_1_bss_11C, p + 11, 4);
            HuSprAttrReset(lbl_1_bss_11C, p + 15, 4);
            HuSprBankSet(lbl_1_bss_11C, p + 15, 4);
            HuSprPosSet(lbl_1_bss_11C, p + 15, 128.0f + (sel[p] % 5) * 80 - 10.0f, (sel[p] / 5) * 90 + 210);
            Hu3DMotionShiftSet(lbl_1_bss_EE[sel[p]], lbl_1_bss_8A[sel[p]][2], 0.0f, 8.0f, 0);
            lbl_1_data_D4[p] = sel[p];
            lbl_1_bss_34[p] = sel[p];
            Hu3DModelAttrReset(lbl_1_bss_3C[p], 1);
        } else if ((HuPadBtnDown[0] & 0x200) != 0) {
            HuAudFXPlay(0);
            HuSprAttrSet(lbl_1_bss_11C, p + 11, 4);
            grid[col[p]][row[p]] = 0;
            for (k = p; k < 4; k++) {
                sel[k] = -1;
            }
            i = p - 1;
#ifdef TARGET_PC
            if (i < 0) {
                i = 0;
            }
#endif
            if (i >= lbl_1_bss_58) {
                grid[col[i]][row[i]] = 0;
            }
            Hu3DMotionShiftSet(lbl_1_bss_EE[sel[i]], lbl_1_bss_8A[sel[i]][0], 0.0f, 8.0f, 0x40000001);
            lbl_1_data_D4[i] = -1;
            lbl_1_bss_34[i] = -1;
            p = i - 1;
        } else {
            goto loop_89;
        }
        if (p + 1 >= lbl_1_bss_58) {
            p += 1;
            goto loop_139;
        }
        goto loop_5;
    }
}

s32 fn_1_95B4(void)
{
    float t;
    s8 rumble;
    s8 tmp;
    s32 dirA;
    s32 dirB;
    s32 ret;
    s16 side;
    s16 i;
    s16 bank;

    rumble = GwCommon.rumbleF;
    HuSprAttrSet(lbl_1_bss_11C, 2, 4);
    HuSprAttrSet(lbl_1_bss_11C, 3, 4);
    HuSprAttrReset(lbl_1_bss_11C, 4, 4);
    HuSprAttrReset(lbl_1_bss_11C, 7, 4);
    HuSprPosSet(lbl_1_bss_11C, 7, 208.0f, 406.0f);
    HuSprAttrReset(lbl_1_bss_11C, 8, 4);
    HuSprPosSet(lbl_1_bss_11C, 8, 368.0f, 406.0f);
    omVibrate(0, 12, 4, 2);
loop_1:
    HuPrcVSleep();
    if ((HuPadDStkRep[0] & 3) != 0) {
        HuAudFXPlay(0);
        if (rumble != 0) {
            tmp = 0;
        } else {
            tmp = 1;
        }
        rumble = tmp;
        GwCommon.rumbleF = rumble;
        if (rumble == 0) {
            HuPadRumbleAllStop();
        }
        if ((HuPadDStkRep[0] & 1) != 0) {
            side = 0;
        } else {
            side = 1;
        }
        lbl_1_bss_44[side] = 1;
        for (i = 1; i <= 10; i++) {
            t = sin(3.141592653589793 * (180.0 * (i / 10.0)) / 180.0);
            if (side == 0) {
                dirA = 1;
            } else {
                dirA = -1;
            }
            if (side == 0) {
                dirB = 1;
            } else {
                dirB = -1;
            }
            HuSprPosSet(lbl_1_bss_11C, side + 7, (288.0f - dirA * 80) - 20.0f * t * dirB, 406.0f);
            if (i == 5) {
                omVibrate(0, 12, 4, 2);
                if (rumble != 0) {
                    HuSprColorSet(lbl_1_bss_11C, 6, 0, 0xFF, 0xFF);
                } else {
                    HuSprColorSet(lbl_1_bss_11C, 6, 0xFF, 0, 0);
                }
                if (rumble != 0) {
                    bank = 0;
                } else {
                    bank = 1;
                }
                HuSprBankSet(lbl_1_bss_11C, 5, bank);
            }
            HuPrcVSleep();
        }
        lbl_1_bss_44[side] = 0;
    }
    if ((HuPadBtnDown[0] & 0x100) != 0) {
        HuAudFXPlay(0);
        ret = 1;
        HuSprAttrSet(lbl_1_bss_11C, 2, 4);
        HuSprAttrSet(lbl_1_bss_11C, 3, 4);
        HuSprAttrSet(lbl_1_bss_11C, 4, 4);
    } else if ((HuPadBtnDown[0] & 0x200) != 0) {
        HuAudFXPlay(0);
        ret = 0;
    } else {
        goto loop_1;
    }
    HuSprAttrSet(lbl_1_bss_11C, 7, 4);
    HuSprAttrSet(lbl_1_bss_11C, 8, 4);
    return ret;
}


void fn_1_9A50(OMOBJ *obj)
{
    HuVecF pos;
    HuVecF offset;
    HuVecF dir;
    HuVecF yOfs;
    float rotZ;
    s8 stickPos;

    if (lbl_1_bss_0 != 0) {
        CRotM[1].y += 0.1f * HuPadStkX[0];
        CRotM[1].x += 0.1f * HuPadStkY[0];
        CZoomM[1] += HuPadTrigL[0] / 2;
        CZoomM[1] -= HuPadTrigR[0] / 2;
        if (CZoomM[1] < 100.0f) {
            CZoomM[1] = 100.0f;
        }
        pos.x = CenterM[1].x + (CZoomM[1] * (HuSin(CRotM[1].y) * HuCos(CRotM[1].x)));
        pos.y = (CenterM[1].y + (CZoomM[1] * -HuSin(CRotM[1].x)));
        pos.z = (CenterM[1].z + (CZoomM[1] * (HuCos(CRotM[1].y) * HuCos(CRotM[1].x))));
        offset.x = CenterM[1].x - pos.x;
        offset.y = CenterM[1].y - pos.y;
        offset.z = CenterM[1].z - pos.z;
        dir.x = (HuSin(CRotM[1].y) * HuSin(CRotM[1].x));
        dir.y = HuCos(CRotM[1].x);
        dir.z = (HuCos(CRotM[1].y) * HuSin(CRotM[1].x));
        rotZ = CRotM[1].z;
        yOfs.x = dir.x * (offset.x * offset.x + (1.0f - offset.x * offset.x) * HuCos(rotZ))
            + dir.y * (offset.x * offset.y * (1.0f - HuCos(rotZ)) - offset.z * HuSin(rotZ))
            + dir.z * (offset.x * offset.z * (1.0f - HuCos(rotZ)) + offset.y * HuSin(rotZ));

        yOfs.y = dir.y * (offset.y * offset.y + (1.0f - offset.y * offset.y) * HuCos(rotZ))
            + dir.x * (offset.x * offset.y * (1.0f - HuCos(rotZ)) + offset.z * HuSin(rotZ))
            + dir.z * (offset.y * offset.z * (1.0f - HuCos(rotZ)) - offset.x * HuSin(rotZ));

        yOfs.z = dir.z * (offset.z * offset.z + (1.0f - offset.z * offset.z) * HuCos(rotZ))
            + (dir.x * (offset.x * offset.z * (1.0 - HuCos(rotZ)) - offset.y * HuSin(rotZ))
                + dir.y * (offset.y * offset.z * (1.0 - HuCos(rotZ)) + offset.x * HuSin(rotZ)));

        PSVECCrossProduct(&dir, &offset, &offset);
        PSVECNormalize(&offset, &offset);
        stickPos = (HuPadSubStkX[0] & 0xF8);
        if (stickPos != 0) {
            CenterM[1].x += 0.05f * (offset.x * stickPos);
            CenterM[1].y += 0.05f * (offset.y * stickPos);
            CenterM[1].z += 0.05f * (offset.z * stickPos);
        }
        PSVECNormalize(&yOfs, &offset);
        stickPos = -(HuPadSubStkY[0] & 0xF8);
        if (stickPos != 0) {
            CenterM[1].x += 0.05f * (offset.x * stickPos);
            CenterM[1].y += 0.05f * (offset.y * stickPos);
            CenterM[1].z += 0.05f * (offset.z * stickPos);
        }
    }
}

void fn_1_A5B0(void)
{
    s16 statId;
    s32 dataDir;

    dataDir = lbl_1_bss_108->dataDir;
    lbl_1_bss_1C = 1;
    lbl_1_bss_20 = 0;
    if (lbl_1_data_2F0 != -1) {
        HuDataDirClose(lbl_1_data_2F0);
    }
    lbl_1_data_2F0 = dataDir;
    OSReport("Load MiniGame Start!\n");
    statId = HuDataDirReadAsync(dataDir);
    while (1) {
        if (HuDataGetAsyncStat(statId) != 0) {
            lbl_1_bss_1C = 0;
            OSReport("Load MiniGame Finished!\n");
            HuPrcEnd();
            while (1) {
                HuPrcVSleep();
            }
        }
        if (lbl_1_bss_20 != 0) {
            HuDataDirCancelAsync(statId);
            while (HuDataDirCloseAsync(statId) == 0) {
                HuPrcVSleep();
            }
            OSReport("Load MiniGame Canceled!\n");
            lbl_1_bss_1C = 0;
            HuPrcEnd();
            while (1) {
                HuPrcVSleep();
            }
        }
        HuPrcVSleep();
    }
}

void fn_1_A6E8(void)
{
    s16 charNo;
    s16 statId;
    s16 i;

    OSReport("Load Char Motion Start!\n");
    CharDataClose(-1);
    lbl_1_bss_18 = 0;
    lbl_1_bss_14 = 1;
    i = 0;
    while (1) {
        if (i >= 4) {
            lbl_1_bss_14 = 0;
            OSReport("Load Char Motion Finished!\n");
            HuMemHeapDump(HuMemHeapPtrGet(HEAP_DVD), -1);
            HuPrcEnd();
            while (1) {
                HuPrcVSleep();
            }
        }
        charNo = lbl_1_bss_5A[i];
        statId = HuDataDirReadAsync(charDataDirTbl[charNo][4]);
        while (1) {
            if (HuDataGetAsyncStat(statId) != 0) {
                if (lbl_1_bss_18 != 0) {
                    OSReport("Load Char Motion Canceled!\n");
                    CharDataClose(-1);
                    lbl_1_bss_14 = 0;
                    HuPrcEnd();
                    while (1) {
                        HuPrcVSleep();
                    }
                }
                CharMotionInit(charNo);
                i++;
                break;
            }
            if (lbl_1_bss_18 != 0) {
                HuDataDirCancelAsync(statId);
                while (HuDataDirCloseAsync(statId) == 0) {
                    HuPrcVSleep();
                }
                OSReport("Load Char Motion Canceled!\n");
                CharDataClose(-1);
                lbl_1_bss_14 = 0;
                HuPrcEnd();
                while (1) {
                    HuPrcVSleep();
                }
            }
            HuPrcVSleep();
        }
    }
}

s32 *fn_1_A894(ANIMDATA *anim, s16 layer, u16 cameraBit)
{
    s32 *par;
    s16 id;
    float rotZ;

    par = HuMemDirectMallocNum(HEAP_HEAP, 0xC, HU_MEMNUM_OVL);
    id = Hu3DParManCreate(anim, 100, &lbl_1_data_138);
    par[0] = id;
    Hu3DParManAttrSet(id, 0x65);
    Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(id), 1);
    Hu3DParManRotSet(id, 90.0f, 0.0f, 0.0f);
    Hu3DModelLayerSet(Hu3DParManModelIDGet(id), layer);
    Hu3DModelCameraSet(Hu3DParManModelIDGet(id), cameraBit);
    id = Hu3DParManCreate(anim, 100, &lbl_1_data_188);
    par[1] = id;
    Hu3DParManAttrSet(id, 0x163);
    Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(id), 1);
    rotZ = frandmod(80) - 40;
    Hu3DParManRotSet(id, frandmod(40) + 70, 0.0f, rotZ);
    Hu3DModelLayerSet(Hu3DParManModelIDGet(id), layer);
    Hu3DModelCameraSet(Hu3DParManModelIDGet(id), cameraBit);
    id = Hu3DParManCreate(anim, 100, &lbl_1_data_1D8);
    par[2] = id;
    Hu3DParManAttrSet(id, 0x21);
    Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(id), 1);
    Hu3DParManRotSet(id, 0.0f, 0.0f, 0.0f);
    Hu3DModelLayerSet(Hu3DParManModelIDGet(id), layer);
    Hu3DModelCameraSet(Hu3DParManModelIDGet(id), cameraBit);
    return par;
}

void fn_1_AAD8(s32 *par, HuVecF *pos)
{
    Hu3DParManPosSet(par[0], pos->x, pos->y, pos->z);
    Hu3DParManTimeLimitSet(par[0], 2);
    Hu3DParManAttrReset(par[0], 1);
    Hu3DParManPosSet(par[1], pos->x, pos->y, pos->z);
    Hu3DParManTimeLimitSet(par[1], 2);
    Hu3DParManAttrReset(par[1], 1);
    Hu3DParManPosSet(par[2], pos->x, pos->y, pos->z);
    Hu3DParManTimeLimitSet(par[2], 5);
    Hu3DParManAttrReset(par[2], 1);
}

void fn_1_ABB4(s32 *par)
{
    Hu3DParManKill(par[0]);
    Hu3DParManKill(par[1]);
    Hu3DParManKill(par[2]);
}

s32 *fn_1_AC00(ANIMDATA *anim, s16 layer, s16 cameraBit)
{
    s32 *par;
    s16 id;

    par = HuMemDirectMallocNum(HEAP_HEAP, 0xC, HU_MEMNUM_OVL);
    id = Hu3DParManCreate(anim, 100, &lbl_1_data_228);
    par[0] = id;
    Hu3DParManAttrSet(id, 0x65);
    Hu3DParticleBlendModeSet(Hu3DParManModelIDGet(id), 1);
    Hu3DParManRotSet(id, 90.0f, 0.0f, 0.0f);
    Hu3DModelLayerSet(Hu3DParManModelIDGet(id), layer);
    Hu3DModelCameraSet(Hu3DParManModelIDGet(id), cameraBit);
    return par;
}

void fn_1_ACDC(s32 *par, HuVecF *pos)
{
    Hu3DParManPosSet(par[0], pos->x, pos->y, pos->z);
    Hu3DParManTimeLimitSet(par[0], 2);
    Hu3DParManAttrReset(par[0], 1);
}

void fn_1_AD48(s32 *par)
{
    Hu3DParManKill(par[0]);
}
