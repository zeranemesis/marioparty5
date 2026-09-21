#include "game/audio.h"
#include "game/data.h"
#include "game/esprite.h"
#include "game/gamemes.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/process.h"
#include "game/saveload.h"
#include "game/sprite.h"
#include "game/thpmain.h"
#include "game/window.h"
#include "game/wipe.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

void ObjectSetup(void);
static void StaffMain(OMOBJ *obj);
static void StaffInit(OMOBJ *obj);
static void StaffExit(OMOBJ *obj);

// Credits script: for each language, a stream of s32 "commands" read by
// StaffMain. Negative values are opcodes, non-negative values are message
// numbers passed to HuWinMesSet().
//   -1               end of the script
//   -2, <frames*60/10> hold for a duration (in tenths of a second)
//   -3, <winNo>       flag window winNo to slide off-screen
//   -4, <type>         set staffNextType
//   -5, <type>         set staffType
static s32 staffData[2][0x400] = {
    {
        -2, 50, 65646, 65536, -2, 30, 65647, 65537, -2, 30, 65648, 65538,
        -2, 30, 65649, 65539, -2, 30, -3, 0, 65649, 65540, 65541, 65542,
        -2, 30, -3, 0, 65649, 65543, 65544, -2, 30, -3, 0, 65649,
        65545, 65546, -2, 30, 65650, 65547, -2, 30, 65651, 65548, 65549, 65550,
        -2, 30, -3, 0, 65651, 65551, 65552, -2, 30, -3, 0, 65651,
        65553, 65554, -2, 30, -3, 0, 65651, 65555, 65556, -2, 30, -3,
        0, 65651, 65557, 65558, 65559, -2, 30, -3, 0, 65651, 65560, 65561,
        -2, 30, -3, 0, 65651, 65562, 65563, -2, 30, -3, 0, 65651,
        65564, 65565, 65566, -2, 30, -3, 0, 65651, 65567, 65568, -2, 30,
        65652, 65569, -2, 30, 65653, 65570, -2, 30, 65654, 65571, 65572, 65573,
        -2, 30, -3, 0, 65654, 65574, 65575, -2, 30, -3, 0, 65654,
        65576, 65577, -2, 30, -3, 0, 65654, 65578, 65579, -2, 30, -3,
        0, 65654, 65580, 65581, 65582, -2, 30, -3, 0, 65654, 65583, 65584,
        65585, -2, 30, -3, 0, 65654, 65586, 65587, -2, 30, -3, 0,
        65654, 65588, 65589, 65590, -2, 30, -3, 0, 65654, 65591, 65592, 65593,
        -2, 30, -3, 0, 65654, 65594, 65595, 65596, -2, 30, -3, 0,
        65654, 65597, 65598, 65599, -2, 30, -3, 0, 65654, 65600, 65601, -2,
        30, 65655, 65602, 65603, -2, 30, 65656, 65604, -2, 30, 65657, 65605,
        -2, 30, 65658, 65606, -2, 30, 65659, 65607, 65608, 65609, -2, 30,
        -3, 0, 65659, 65610, 65611, -2, 30, 65660, 65612, 65613, -2, 30,
        65661, 65614, 65615, -2, 30, 65662, 65616, -2, 30, 65663, 65618, 65619,
        -2, 30, -3, 0, 65663, 65620, 65621, -2, 30, -3, 0, 65663,
        65622, 65623, 65624, -2, 30, -3, 0, 65663, 65625, -2, 30, -3,
        0, 65663, 65626, 65627, 65628, -2, 30, 65665, 65631, 65632, -2, 30,
        65666, 65633, 65638, 65634, -2, 30, 65667, 65630, 65629, -2, 30, 65664,
        65635, -2, 30, 65668, 65636, 65637, -2, 30, -1,
    },
    {
        -2, 50, 65646, 65536, -2, 30, 65647, 65537, -2, 30, 65648, 65538,
        -2, 30, 65649, 65539, -2, 30, -3, 0, 65649, 65540, 65541, 65542,
        -2, 30, -3, 0, 65649, 65543, 65544, -2, 30, -3, 0, 65649,
        65545, 65546, -2, 30, -5, -24, 65650, 65547, -2, 30, 65651, 65548,
        65549, 65550, -2, 30, -3, 0, 65651, 65551, 65552, -2, 30, -3,
        0, 65651, 65553, 65554, -2, 30, -3, 0, 65651, 65555, 65556, -2,
        30, -3, 0, 65651, 65557, 65558, 65559, -2, 30, -3, 0, 65651,
        65560, 65561, -2, 30, -3, 0, 65651, 65562, 65563, -2, 30, -3,
        0, 65651, 65564, 65565, 65566, -2, 30, -3, 0, 65651, 65567, 65568,
        -2, 30, -5, -64, 65652, 65569, -2, 30, 65653, 65570, -2, 30,
        65654, 65571, 65572, 65573, -2, 30, -3, 0, 65654, 65574, 65575, -2,
        30, -3, 0, 65654, 65576, 65577, -2, 30, -3, 0, 65654, 65578,
        65579, -2, 30, -3, 0, 65654, 65580, 65581, 65582, -2, 30, -3,
        0, 65654, 65583, 65584, 65585, -2, 30, -3, 0, 65654, 65586, 65587,
        -2, 30, -3, 0, 65654, 65588, 65589, 65590, -2, 30, -3, 0,
        65654, 65591, 65592, 65593, -2, 30, -3, 0, 65654, 65594, 65595, 65596,
        -2, 30, -3, 0, 65654, 65597, 65598, 65599, -2, 30, -3, 0,
        65654, 65600, 65601, -2, 30, 65655, 65602, 65603, -2, 30, -5, -64,
        65656, 65604, -2, 30, 65657, 65605, -2, 30, -5, 16, 65658, 65606,
        -2, 30, 65659, 65607, 65608, 65609, -2, 30, -3, 0, 65659, 65610,
        65611, -2, 30, 65660, 65612, 65613, -2, 30, -5, -16, 65661, 65614,
        65615, -2, 30, 65662, 65616, -2, 30, -5, -64, 65669, 65672, 65673,
        65674, -2, 30, -3, 0, 65669, 65675, 65676, -2, 30, -5, -64,
        65670, 65677, 65678, -2, 30, 65671, 65679, 65680, 65681, -2, 30, -3,
        0, 65671, 65682, 65683, -2, 30, -3, 0, 65671, 65684, 65685, -2,
        30, -5, -16, 65663, 65686, 65687, -2, 30, -3, 0, 65618, 65619,
        -2, 30, -3, 0, 65663, 65620, 65621, -2, 30, -3, 0, 65663,
        65622, 65623, 65624, -2, 30, -3, 0, 65663, 65625, -2, 30, -3,
        0, 65663, 65626, 65627, 65628, -2, 30, 65665, 65631, 65632, -2, 30,
        65666, 65633, 65638, 65634, -2, 30, 65667, 65630, 65629, -2, 30, 65664,
        65635, -2, 30, -4, 1, 65668, 65636, 65637, -2, 30, -1,
    },
};

static f32 staffEndMesTime;
static s32 staffPhase;
static s32 staffEndF;
static s32 staffLineNo;
static s32 staffNo;
static s32 staffTime;
static s16 staffNextType;
static s16 staffType;
static HuVec2f staffWinPos[5];
static HuVec2f staffWinSize[5];
static s16 staffWinMoveF[5];
static HUWINID staffWinId[5];
static s16 staffTHPSprGId;
static HUSPRID staffTHPSprId;
static s32 staffCopyrightSprId;
static s32 staffEndMesSprId;
static s32 staffEndSprId;
static s32 glightId;
static GAMEMESID staffMes;
static OMOBJMAN *objman;
static OMOBJ *staffOutView;
static OMOBJ *staffObj;
static s32 staffMode = -1;

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

static void StaffStub(OMOBJ *obj)
{
}

// Generic ease-in / hold / ease-out progress curve over a total span made
// of an intro of length "in", a hold of length "hold" and an outro of
// length "out", sampled at position "t".
// NONMATCHING: a few commutative float multiply/add operand orders differ (-O0 register allocation quirk); logic is identical.
static f32 StaffPosGet(f32 t, f32 in, f32 hold, f32 out)
{
    f32 total;
    f32 rOut;
    f32 rHold;
    f32 rIn;

    total = (out * 0.5f) + ((in * 0.5f) + hold);
    if (t < in) {
        rOut = (((1.0f + t) * 0.5f * t) / in) / total;
    } else {
        if (t < (in + hold)) {
            rHold = ((((1.0f + in) * 0.5f * in) / in) / total) + ((t - in) / total);
        } else {
            if (t < ((out + (in + hold)) - 1.0f)) {
                rIn = 1.0f - ((((1.0f + (((out + (in + hold)) - t) - 1.0f)) * 0.5f * ((((out + (in + hold)) - 1.0f) - t) - 1.0f)) / (out - 1.0f)) / total);
            } else {
                rIn = 1.0f;
            }
            rHold = rIn;
        }
        rOut = rHold;
    }
    return rOut;
}

// NONMATCHING: large state machine rewritten from the m2c draft into readable
// loops/switches; control flow and computed values match but register
// allocation and instruction scheduling differ from the original -O0 output.
static void StaffMain(OMOBJ *obj)
{
    u32 lang;
    u32 langOther;
    s32 *data;
    f32 winScaleX;
    f32 progress;
    s32 i;

    lang = GwCommon.languageNo;
    switch (lang) {
    case 0:
        lang = 0;
        langOther = 0x12;
        break;
    case 1:
        lang = 1;
        langOther = 9;
        break;
    default:
        langOther = 0;
        break;
    }

    // .thp intro movie state machine.
    if (staffPhase == 0) {
        staffTHPSprId = HuSprGrpCreate(1);
        staffPhase++;
    }
    if (staffPhase == 1) {
        staffTHPSprGId = HuTHPSprCreateVol("movie/st_movie.thp", 0, 5, 105.0f);
        if (staffTHPSprGId != -1) {
            HuSprGrpMemberSet(staffTHPSprId, 0, staffTHPSprGId);
            HuSprPosSet(staffTHPSprId, 0, 288.0f, 192.0f);
            HuSprGrpDrawNoSet(staffTHPSprId, 0x7F);
            HuSprPriSet(staffTHPSprId, 0, 5);
            staffPhase++;
        }
    } else if (staffPhase == 2 || staffPhase == 3) {
        staffPhase++;
    } else if (staffPhase == 4) {
        if (HuTHPFrameGet() <= 0x78 && HuTHPFrameGet() >= 0x3C) {
            espDispOn((s16) staffEndSprId);
        }
        if (HuTHPEndCheck()) {
            HuTHPClose();
            staffPhase++;
        }
    } else if (staffPhase == 5) {
        if (!HuTHPProcCheck()) {
            HuSprGrpKill(staffTHPSprId);
            staffPhase++;
            staffEndMesTime = 0.0f;
        }
    } else if (staffPhase == 6) {
        staffEndMesTime += 1.0f;
        espDispOn((s16) staffEndMesSprId);
        espTPLvlSet((s16) staffEndMesSprId, staffEndMesTime / 120.0f);
        if (staffEndMesTime >= 120.0f) {
            staffPhase++;
        }
    }

    // Credits scroll state machine, driven once the intro is done.
    if (staffPhase >= 4) {
        data = staffData[lang];
        winScaleX = (lang == 0) ? 0.75f : 1.0f;

        if (staffMode == -1) {
            if (data[staffNo] != -2) {
                staffMode++;
                staffTime = 0;
            } else {
                staffTime++;
                if (staffTime >= (data[staffNo + 1] * 0x3C) / 10) {
                    staffMode++;
                    staffTime = 0;
                    staffNo += 2;
                }
            }
        } else if (staffMode == 0) {
            staffLineNo = 0;
            if (data[staffNo] == -4) {
                staffNo++;
                staffNextType = (s16) data[staffNo];
                staffNo++;
            } else if (data[staffNo] == -5) {
                staffNo++;
                staffType = (s16) data[staffNo];
                staffNo++;
            } else {
                f32 height = 0.0f;

                for (i = 0; i < 5; i++) {
                    if (data[staffNo] < 0) {
                        break;
                    }
                    HuWinMesMaxSizeGet(1, &staffWinSize[i], data[staffNo]);
                    staffWinSize[i].y = 0x1C;
                    if (staffWinMoveF[i] == 0) {
                        staffWinId[i] = HuWinCreate(-200.0f, -200.0f, (s16) staffWinSize[i].x, (s16) staffWinSize[i].y, 1);
                        HuWinBGTPLvlSet(staffWinId[i], 0.0f);
                        HuWinMesSpeedSet(staffWinId[i], 0);
                        HuWinMesSet(staffWinId[i], data[staffNo]);
                        HuWinScaleSet(staffWinId[i], winScaleX, 0.75f);
                    }
                    staffLineNo++;
                    staffNo++;
                }

                for (i = 0; i < staffLineNo; i++) {
                    height += (i != 0) ? 0.75f * staffWinSize[i].y : 0.0f;
                }

                for (i = 0; i < staffLineNo; i++) {
                    if (staffWinMoveF[i] != 1) {
                        BOOL indent = (staffNextType == 0 && i == 0);
                        staffWinPos[i].x = indent ? 82.28571f : 288.0f - (winScaleX * staffWinSize[i].x * 0.5f);
                    }
                    if (i == 0) {
                        staffWinPos[i].y = 346.0f;
                    } else if (i == 1) {
                        staffWinPos[i].y = (((417.0f + (28.0f * (1.0f - 0.75f))) - (2.0f * (28.0f * 0.75f))) - (height * 0.5f)) + (0.75f * staffWinSize[i].y);
                    } else {
                        staffWinPos[i].y = (0.75f * staffWinSize[i].y * 0.5f) + (staffWinPos[i - 1].y + (0.75f * staffWinSize[i - 1].y * 0.5f));
                    }
                }
                staffMode++;
                staffTime = 0;
                staffType = 0;
            }
        } else if (staffMode == 1 || staffMode == 3) {
            f32 half;
            f32 other;

            staffTime++;
            half = (f32) (langOther / 2);
            other = (f32) (langOther - langOther / 2);
            if (staffMode == 1) {
                progress = StaffPosGet((f32) staffTime, 1.0f, half, 1.0f + other) - 1.0f;
            } else {
                progress = StaffPosGet((f32) staffTime, (f32) langOther, 0.0f, 1.0f) - 1.0f;
            }

            for (i = staffLineNo - 1; i >= 0; i--) {
                if (staffWinMoveF[i] != 1) {
                    HuWinPosSet(staffWinId[i], staffWinPos[i].x - (480.0f * progress), staffWinPos[i].y);
                }
            }
            if (staffTime >= (s32) langOther) {
                staffMode++;
                staffTime = 0;
                if (staffMode == 2) {
                    for (i = 0; i < 5; i++) {
                        staffWinMoveF[i] = 0;
                    }
                }
            }
        } else if (staffMode == 2) {
            if (data[staffNo] != -2) {
                staffMode++;
                staffTime = 0;
            } else {
                staffTime++;
                if (staffTime >= (data[staffNo + 1] * 0x3C) / 10) {
                    staffMode++;
                    staffTime = 0;
                    staffNo += 2;
                    if (data[staffNo] == -3) {
                        staffNo++;
                        staffWinMoveF[data[staffNo]] = 1;
                        staffNo++;
                    }
                }
            }
        } else if (staffMode == 4) {
            for (i = 0; i < staffLineNo; i++) {
                if (staffWinMoveF[i] == 0) {
                    HuWinExKill(staffWinId[i]);
                }
            }
            staffMode = 0;
            if (data[staffNo] == -1) {
                staffNo = 0;
                staffMode = 5;
                staffTime = 0;
            }
        } else if (staffMode == 5) {
            f32 x;

            staffTime++;
            progress = StaffPosGet((f32) staffTime, 1.0f, 10.0f, 11.0f) - 1.0f;
            espDispOn((s16) staffCopyrightSprId);
            x = 288.0f - (640.0f * progress);
            espPosSet((s16) staffCopyrightSprId, x, 392.0f);
            if (staffTime >= 0x16) {
                staffMode++;
                staffTime = 0;
            }
        } else if (staffMode == 6) {
            staffTime++;
            if (staffTime >= 0x4B0) {
                staffMode++;
                staffTime = 0;
            }
        } else if (staffMode == 7) {
            progress = StaffPosGet((f32) ++staffTime, 11.0f, 0.0f, 11.0f);
            espDispOn((s16) staffCopyrightSprId);
            espPosSet((s16) staffCopyrightSprId, (f32) (s32) (288.0f - (640.0f * progress)), 392.0f);
            if (staffTime >= 0x16) {
                staffMode++;
                staffTime = 0;
            }
        }
    }

    // Exit conditions: any player pressing start (or +/- once the credits
    // reached the copyright screen), or a system exit request.
    if (staffEndF == 0) {
        s32 storyPlayNum = 0;
        s32 p;

        for (p = 0; p < 10; p++) {
            storyPlayNum += GwCommon.storyPlayNum[p];
        }

        for (p = 0; p < 4; p++) {
            u16 btn = HuPadBtn[GwPlayerConf[p].padNo];

            if (GwPlayerConf[p].type == 0
                && ((storyPlayNum > 1 && !WipeCheckEnd() && (btn & 0x1000))
                    || (staffPhase >= 7 && (btn & 0x1300)))) {
                staffEndF = 1;
            }
        }

        if (omSysExitReq) {
            staffEndF = 1;
        }

        if (staffEndF == 1) {
            s32 fadeSpeed = (staffPhase == 7) ? 0xDAC : 0x4E2;

            HuAudFadeOut(fadeSpeed);
            if (HuTHPProcCheck()) {
                HuTHPSetVolume(0, fadeSpeed);
            }
            WipeCreate(2, 0, (staffPhase == 7) ? 0x78 : 0x2D);
        }
    }

    if (staffEndF > 0) {
        staffEndF++;
    }
    if (staffEndF > 2 && !WipeCheckEnd()) {
        obj->objFunc = StaffExit;
    }
}

static void StaffInit(OMOBJ *obj)
{
    staffEndSprId = espEntry(0xC00000, 0xA, 0);
    espDispOff((s16) staffEndSprId);
    espPosSet((s16) staffEndSprId, 288.0f, 192.0f);
    espDrawNoSet((s16) staffEndSprId, 0x7F);

    staffEndMesSprId = espEntry(0xC00001, 8, 0);
    espDispOff((s16) staffEndMesSprId);
    espTPLvlSet((s16) staffEndMesSprId, 0.0f);
    espPosSet((s16) staffEndMesSprId, 291.0f, 200.0f);
    espDrawNoSet((s16) staffEndMesSprId, 0);

    staffCopyrightSprId = espEntry(0xC00002, 5, 0);
    espDispOff((s16) staffCopyrightSprId);
    espDrawNoSet((s16) staffCopyrightSprId, 0);
    espPosSet((s16) staffCopyrightSprId, 288.0f, 392.0f);
    espAttrSet((s16) staffCopyrightSprId, 8);

    WipeCreate(1, 0, 0x3C);
    obj->objFunc = StaffMain;
}

// NONMATCHING: light constant struct locals differ slightly in stack layout
// from the original; behavior is identical.
void ObjectSetup(void)
{
    HuVecF lightPos = { 0.0f, 5000.0f, 0.0f };
    HuVecF lightDir = { 0.0f, -1.0f, -0.5f };
    GXColor lightColor = { 0xFF, 0xFF, 0xFF, 0xFF };

    OSReport("******* Staff ObjectSetup *********\n");

    objman = omInitObjMan(0x258, 0x2000);
    omGameSysInit(objman);

    Hu3DCameraCreate(1);
    Hu3DCameraPerspectiveSet(1, 20.0f, 20.0f, 25000.0f, 1.2f);
    Hu3DCameraViewportSet(1, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraScissorSet(1, 0, 0, 0x280, 0x1E0);

    CZoom = 4000.0f;
    Center.x = 1800.0f;
    Center.y = -150.0f;
    Center.z = 0.0f;
    CRot.x = -40.0f;
    CRot.y = 0.0f;
    CRot.z = 0.0f;

    glightId = Hu3DGLightCreateV(&lightPos, &lightDir, &lightColor);
    Hu3DGLightStaticSet((s16) glightId, 1);
    Hu3DGLightInfinitytSet((s16) glightId);

    staffMes = -1;
    staffOutView = omAddObjEx(objman, 0x7FDA, 0, 0, OM_GRP_NONE, omOutView);
    staffOutView->work[0] = 1;
    staffObj = omAddObjEx(objman, 0, 0, 0, OM_GRP_NONE, StaffInit);
}

static void StaffExit(OMOBJ *obj)
{
    if (WipeCheckEnd()) {
        return;
    }

    HuTHPClose();
    while (HuTHPProcCheck()) {
        HuPrcVSleep();
    }

    if (staffMes >= 0) {
        GameMesKill(staffMes);
    }
    GameMesClose();
    HuAudAllStop();
    SLSaveStaffExec();
    omOvlReturnEx(2, 1);

    while (1) {
        HuPrcVSleep();
    }
}
