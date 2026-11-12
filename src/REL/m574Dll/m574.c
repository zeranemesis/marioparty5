#include "REL/m574Dll.h"
#include "game/audio.h"
#include "game/process.h"
#include "game/mg/seqman.h"

#include "dolphin/mtx.h"

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

void fn_1_1044(void);
void fn_1_2028(OMOBJMAN *om);
void fn_1_20C8(void);
void fn_1_6650(OMOBJMAN *om);
void fn_1_66F0(void);
s32 fn_1_6CC8(s32 grp);
void fn_1_7130(OMOBJMAN *om);
void fn_1_71B8(void);
s32 fn_1_7948(s32 grp);
s32 fn_1_797C(void);
void fn_1_7A48(OMOBJMAN *om);
void fn_1_7AD0(void);
void fn_1_E98(OMOBJMAN *om);

void ObjectSetup(void);
static void fn_1_348(s16 mode, s16 frameNo);
static void fn_1_3B8(s16 mode, s16 frameNo);
static void fn_1_548(s16 mode, s16 frameNo);
static void fn_1_54C(s16 mode, s16 frameNo);
static void fn_1_5A8(s16 mode, s16 frameNo);
static void fn_1_730(s16 mode, s16 frameNo);
static void fn_1_82C(s16 mode, s16 frameNo);
static void fn_1_8E4(s16 mode, s16 frameNo);
static void fn_1_8E8(s16 mode, s16 frameNo);

static OMCAMERAVIEW cameraView[2] = {
    { { 0.0f, 1350.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, 1180.0f },
    { { 0.0f, 800.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, 2750.0f },
};
static HuVecF lightPos = { 100.0f, 800.0f, -100.0f };
static HuVecF lightDir = { 0.3f, -0.8f, 0.3f };
static HuVecF lightRot = { 20.0f, 45.0f, 1000.0f };
static GXColor lightColor = { 0xFF, 0xFF, 0xFF, 0xFF };
static HuVecF shadowPos = { 0.0f, 1000.0f, 1500.0f };
static HuVecF shadowUp = { 0.0f, 0.0f, 1.0f };
static HuVecF shadowTarget = { 0.0f, 0.0f, 0.0f };
static MGSEQ_PARAM seqParam = {
    0x3C,
    1,
    fn_1_348,
    fn_1_3B8,
    fn_1_548,
    fn_1_54C,
    fn_1_5A8,
    fn_1_730,
    fn_1_82C,
    fn_1_8E4,
    fn_1_8E8,
};

static s16 charNoTbl[4];
static s16 grpTbl[4];
static s16 winnerTbl[4];
static s32 bgmStream;
static s16 lightId;
static s16 timeLeft;
static s32 winFlag;
static s32 timer;
static s32 state;
static s32 loseCount;
static OMOBJ *outViewObj;
static OMOBJMAN *objman;

s32 _prolog(void)
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
    HuVecF unused0 = { 0.0f, 0.0f, 0.0f };
    HuVecF unused1 = { 0.0f, 0.0f, -1.0f };
    HU3DLIGHT *light;
    HuVecF aim;
    HuVecF pos;
    Mtx rotMtx;

    loseCount = 0;
    winFlag = 0;
    timeLeft = 0x3C;
    bgmStream = -1;
    objman = omInitObjMan(0x32, 0x2000);
    omGameSysInit(objman);
    Hu3DCameraCreate(1);
    Hu3DCameraPerspectiveSet(1, 45.0f, 20.0f, 10000.0f, 1.2f);
    outViewObj = omAddObjEx(objman, 0x7FDA, 0, 0, -1, omOutView);
    omCameraViewSet(cameraView);
    lightId = Hu3DGLightCreateV(&lightPos, &lightDir, &lightColor);
    Hu3DGLightInfinitytSet(lightId);
    light = &Hu3DGlobalLight[lightId];
    light->type |= 0x8000;
    aim.x = aim.y = aim.z = 0.0f;
    pos.x = pos.z = 0.0f;
    pos.y = lightRot.z;
    mtxRot(rotMtx, lightRot.x, lightRot.y, 0.0f);
    PSMTXMultVec(rotMtx, &pos, &pos);
    Hu3DGLightPosAimSetV(lightId, &pos, &aim);
    Hu3DShadowCreate(80.0f, 20.0f, 5000.0f);
    Hu3DShadowPosSet(&shadowPos, &shadowUp, &shadowTarget);
    Hu3DShadowColSet(0x32, 0x32, 0x32);
    Hu3DShadowSizeSet(0xF0);
    MgSeqCreate(&seqParam);
}

static void fn_1_348(s16 mode, s16 frameNo)
{
    fn_1_7130(objman);
    fn_1_7A48(objman);
    fn_1_2028(objman);
    fn_1_E98(objman);
    fn_1_6650(objman);
    MgSeqModeNext();
}

/* Remaining difference: the original switch has an extra empty case 4 compare without a branch. */
static void fn_1_3B8(s16 mode, s16 frameNo)
{
    if (frameNo == 0) {
        state = 0;
        timer = 0;
    }
    if (bgmStream == -1 && frameNo >= 0x12) {
        bgmStream = HuAudBGMPlay(0x54);
    }
    switch (state) {
        case 0:
            if (++timer < 0x1E) {
                break;
            }
            state++;
            omCameraViewMove(&cameraView[1], 0x5A, 1);
            // fallthrough
        case 1:
            if (!omCameraViewCheck(1)) {
                break;
            }
            state++;
            timer = 0;
            // fallthrough
        case 2:
            if (++timer < 0x5A) {
                break;
            }
            state++;
            MgSeqModeNext();
            break;
        case 4:
            break;
    }
}

static void fn_1_548(s16 mode, s16 frameNo)
{
}

static void fn_1_54C(s16 mode, s16 frameNo)
{
    timeLeft = 0x3C - (frameNo / 60);
    if (timeLeft < 0) {
        timeLeft = 0;
    }
}

static void fn_1_5A8(s16 mode, s16 frameNo)
{
    s32 i;
    s32 score0;
    s32 score1;
    u16 res;

    if (frameNo == 0) {
        if (bgmStream != -1) {
            HuAudSStreamFadeOut(bgmStream, 0x64);
            bgmStream = -1;
        }
        timeLeft = 0;
        score0 = fn_1_7948(0);
        score1 = fn_1_7948(1);
        for (i = 0; i < 4; i++) {
            winnerTbl[i] = -1;
            if ((score0 > score1 && grpTbl[i] == 0) || (score0 < score1 && grpTbl[i] == 1)) {
                winnerTbl[i] = charNoTbl[i];
            }
        }
        res = MgSeqWinnerSet(winnerTbl[0], winnerTbl[1], winnerTbl[2], winnerTbl[3]);
        winFlag = fn_1_797C();
    }
}

static void fn_1_730(s16 mode, s16 frameNo)
{
    s32 wait;

    if (frameNo == 0) {
        wait = 0;
        while (fn_1_6CC8(0) != 0 || fn_1_6CC8(1) != 0) {
            if (omSysExitReq != 0) {
                break;
            }
            wait++;
            if (wait > 0x12C) {
                break;
            }
            HuPrcVSleep();
        }
    }
    if (frameNo == 0x5A && winFlag != 0 && _CheckFlag(0x1000F) == 0) {
        HuAudJinglePlay(0x86);
    }
    if ((winFlag == 0 || _CheckFlag(0x1000F) != 0) && frameNo >= 0x5A) {
        MgSeqModeNext();
    }
}

static void fn_1_82C(s16 mode, s16 frameNo)
{
    s32 i;
    s16 bonus;
    s16 cur;

    if (frameNo == 0) {
        for (i = 0; i < 4; i++) {
            if (winnerTbl[i] >= 0) {
                cur = GwPlayer[i].mgCoinBonus;
                bonus = cur + 0xA;
                if (_CheckFlag(0x1000F) == 0) {
                    GwPlayer[i].mgCoinBonus = bonus;
                }
            }
        }
    }
}

static void fn_1_8E4(s16 mode, s16 frameNo)
{
}

static void fn_1_8E8(s16 mode, s16 frameNo)
{
    fn_1_20C8();
    fn_1_66F0();
    fn_1_1044();
    fn_1_7AD0();
    fn_1_71B8();
}

s32 fn_1_918(void)
{
    return timeLeft;
}

s32 fn_1_928(s32 no)
{
    return winnerTbl[no];
}

s32 fn_1_940(u32 grp)
{
    s32 i;
    u32 ret;

    for (i = 0; i < 4; i++) {
        if (winnerTbl[i] >= 0) {
            break;
        }
    }
    if (i >= 4) {
        return 0;
    }
    for (i = 0; i < 4; i++) {
        if (grp == grpTbl[i]) {
            if (winnerTbl[i] >= 0) {
                ret = 1;
            }
            else {
                ret = -1;
            }
            return ret;
        }
    }
    return grp;
}

void fn_1_9FC(s32 no, s32 grp, s32 charNo)
{
    no &= 3;
    grpTbl[no] = grp;
    charNoTbl[no] = charNo;
}

s16 fn_1_A34(u32 grp, u32 idx)
{
    s32 i;
    u32 n = 0;

    for (i = 0; i < 4; i++) {
        if (grp == grpTbl[i]) {
            if (idx == n) {
                return charNoTbl[i];
            }
            n++;
        }
    }
    return -1;
}

void fn_1_AAC(s32 no)
{
    if (++loseCount == 4) {
        CharLoseVoicePlay(charNoTbl[0], charNoTbl[1], charNoTbl[2], charNoTbl[3]);
    }
}

s32 fn_1_B20(f32 x)
{
    s32 pan;

    x *= 0.11111111f;
    pan = 32.0f + x;
    if (pan < 0x20) {
        pan = 0x20;
    }
    if (pan > 0x60) {
        pan = 0x60;
    }
    return pan;
}

s32 fn_1_B7C(HuVecF *pos)
{
    HuVecF scr;
    f32 x;
    s32 pan;

    Hu3D3Dto2D(pos, 1, &scr);
    x = scr.x;
    x *= 0.11111111f;
    pan = 32.0f + x;
    if (pan < 0x20) {
        pan = 0x20;
    }
    if (pan > 0x60) {
        pan = 0x60;
    }
    return pan;
}

void fn_1_C10(s32 seId, HuVecF *pos)
{
    HuVecF scr;
    f32 x;
    s32 pan;
    s32 b;
    s32 a;

    Hu3D3Dto2D(pos, 1, &scr);
    x = scr.x;
    x *= 0.11111111f;
    pan = 32.0f + x;
    if (pan < 0x20) {
        pan = 0x20;
    }
    if (pan > 0x60) {
        pan = 0x60;
    }
    a = pan;
    b = a;
    HuAudFXPlayPan(seId, b);
}

f32 fn_1_CC8(f32 cur, f32 target, f32 rate)
{
    f32 tmp1 = fmod(target - cur, 360.0);
    f32 diff = tmp1;
    f32 tmp2;

    if (diff < 0.0f) {
        diff += 360.0f;
    }
    if (diff > 180.0f) {
        diff -= 360.0f;
    }
    tmp2 = fmod(cur + rate * diff, 360.0);
    diff = tmp2;
    if (diff < 0.0f) {
        diff += 360.0f;
    }
    return diff;
}

f32 fn_1_DE4(f32 from, f32 to)
{
    f32 tmp = fmod(to - from, 360.0);
    f32 diff = tmp;

    if (diff < 0.0f) {
        diff += 360.0f;
    }
    if (diff > 180.0f) {
        diff -= 360.0f;
    }
    return diff;
}
