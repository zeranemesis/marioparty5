#include "types.h"
#include "game/audio.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/flag.h"
#include "game/gamework.h"
#include "game/hsfex.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/mg/seqman.h"
#include "game/object.h"
#include "REL/m514Dll/m514types.h"

#include <math.h>
#include <string.h>

#ifdef __MWERKS__
#define M514_RODATA __declspec(section ".rodata")
#else
#define M514_RODATA
#endif

#define M514_RAD(x) ((3.141592653589793 * (f64)(x)) / 180.0)

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

extern void fn_1_510C(OMOBJMAN *objman);
extern void fn_1_51BC(void);

f32 lbl_1_data_0 = 20.0f;
f32 lbl_1_data_4 = 25000.0f;
f32 lbl_1_data_8 = 48.0f;

OMCAMERAVIEW lbl_1_data_C[2] = {
    { { -1500.0f, 1250.0f, 0.0f }, { -10.0f, 0.0f, 0.0f }, 1180.0f },
    { { -2500.0f, 120.0f, 0.0f }, { -15.0f, 0.0f, 0.0f }, 750.0f },
};

HuVecF lbl_1_data_44 = { 100.0f, 800.0f, -100.0f };
HuVecF lbl_1_data_50 = { 0.3f, -0.8f, 0.3f };
f32 lbl_1_data_5C[3] = { 40.0f, -60.0f, 1000.0f };
GXColor lbl_1_data_68 = { 0xFF, 0xFF, 0xFF, 0xFF };
f32 lbl_1_data_6C[9] = { 0.0f, 5000.0f, 1000.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f };

MGSEQ_PARAM lbl_1_data_90 = {
    0, 1,
    (MGSEQ_FUNC)fn_1_3A0, (MGSEQ_FUNC)fn_1_448, (MGSEQ_FUNC)fn_1_540,
    (MGSEQ_FUNC)fn_1_580, (MGSEQ_FUNC)fn_1_600, (MGSEQ_FUNC)fn_1_7B0,
    (MGSEQ_FUNC)fn_1_8A0, (MGSEQ_FUNC)fn_1_958, (MGSEQ_FUNC)fn_1_95C,
};

M514_RODATA const HuVecF lbl_1_rodata_10 = { 0.0f, 0.0f, 0.0f };
M514_RODATA const HuVecF lbl_1_rodata_1C = { 0.0f, 0.0f, -1.0f };
M514_RODATA const HuVecF lbl_1_rodata_88[2] = { { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } };

/* bss: MWCC lays it out in reverse declaration order */
static OMOBJMAN *lbl_1_bss_68;
OMOBJ *lbl_1_bss_64;
s32 lbl_1_bss_60;
s32 lbl_1_bss_5C;
s32 lbl_1_bss_58;
s16 lbl_1_bss_56;
s16 lbl_1_bss_54;
s32 lbl_1_bss_50;
s32 lbl_1_bss_4C;
HuVecF lbl_1_bss_40;
HuVecF lbl_1_bss_34;
HuVecF lbl_1_bss_28;
s16 lbl_1_bss_20[4];
s16 lbl_1_bss_18[4];
s16 lbl_1_bss_10[4];
s16 lbl_1_bss_8[4];
s32 lbl_1_bss_4;
s32 lbl_1_bss_0;

void fn_1_98C(OMOBJ *obj);
void fn_1_9E4(OMOBJ *obj);
void fn_1_14F4(HuVecF *dir, HuVecF *target, f32 dist);
void fn_1_163C(void *motData);
s32 fn_1_16AC(void);

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
    Mtx mtx;
    HuVecF vecA = lbl_1_rodata_10;
    HuVecF vecB = lbl_1_rodata_1C;
    s32 i;

    lbl_1_bss_60 = 0;
    lbl_1_bss_58 = 0;
    lbl_1_bss_56 = 0;
    lbl_1_bss_50 = -1;
    lbl_1_bss_4C = -1;
    for (i = 0; i < 4; i++) {
        lbl_1_bss_10[i] = -1;
        lbl_1_bss_18[i] = -1;
        lbl_1_bss_8[i] = 0;
    }
    lbl_1_bss_68 = omInitObjMan(0x32, 0x2000);
    omGameSysInit(lbl_1_bss_68);
    Hu3DCameraCreate(1);
    Hu3DCameraPerspectiveSet(1, 45.0f, lbl_1_data_0, lbl_1_data_4, 1.2f);
    lbl_1_bss_64 = omAddObjEx(lbl_1_bss_68, 0x7FDA, 0, 0, -1, fn_1_98C);
    omCameraViewSet(&lbl_1_data_C[0]);
    lbl_1_bss_54 = Hu3DGLightCreateV(&lbl_1_data_44, &lbl_1_data_50, &lbl_1_data_68);
    Hu3DGLightInfinitytSet(lbl_1_bss_54);
    Hu3DGlobalLight[lbl_1_bss_54].type |= 0x8000;
    lbl_1_bss_40.x = lbl_1_bss_40.y = lbl_1_bss_40.z = 0.0f;
    lbl_1_bss_34.x = lbl_1_bss_34.z = 0.0f;
    lbl_1_bss_34.y = lbl_1_data_5C[2];
    mtxRot(mtx, lbl_1_data_5C[0], lbl_1_data_5C[1], 0.0f);
    PSMTXMultVec(mtx, &lbl_1_bss_34, &lbl_1_bss_34);
    Hu3DGLightPosAimSetV(lbl_1_bss_54, &lbl_1_bss_34, &lbl_1_bss_40);
    PSVECSubtract(&lbl_1_bss_40, &lbl_1_bss_34, &lbl_1_bss_28);
    MgSeqCreate(&lbl_1_data_90);
}

void fn_1_3A0(void)
{
    fn_1_8268(lbl_1_bss_68);
    fn_1_5B04(lbl_1_bss_68);
    fn_1_510C(lbl_1_bss_68);
    fn_1_1D9C(lbl_1_bss_68);
    fn_1_4270(lbl_1_bss_68);
    lbl_1_bss_4 = 0;
    lbl_1_bss_0 = 0;
    fn_1_163C(HuDataSelHeapReadNum(0x480005, 0x10000000, HEAP_MODEL));
    MgSeqModeNext();
}

void fn_1_448(s16 mode, s16 frameNo)
{
    if (frameNo == 0) {
        lbl_1_bss_5C = 0;
    }
    if (lbl_1_bss_4C == -1 && frameNo >= 0x12) {
        lbl_1_bss_4C = HuAudFXPlay(0x6DD);
    }
    switch (lbl_1_bss_5C) {
        case 0:
            if (frameNo < 0x1E) {
                break;
            }
            lbl_1_bss_5C++;
            /* fallthrough */
        case 1:
            if (fn_1_16AC() == 0) {
                lbl_1_bss_5C++;
                MgSeqModeNext();
            }
            break;
    }
}

void fn_1_540(s16 mode, s16 frameNo)
{
    if (frameNo == 0) {
        lbl_1_bss_50 = HuAudBGMPlay(0x34);
    }
}

void fn_1_580(void)
{
    s32 done = 0;

    if (lbl_1_bss_0 != 0) {
        lbl_1_bss_0 = 0;
        lbl_1_bss_4++;
        if (lbl_1_bss_4 >= 4) {
            done = 1;
        }
    }
    if (done != 0) {
        MgSeqModeNext();
    }
}

void fn_1_600(s16 mode, s16 frameNo)
{
    s32 scores[4];
    s32 max = 0;
    s32 i;

    if (frameNo == 0) {
        if (lbl_1_bss_50 != -1) {
            HuAudSStreamFadeOut(lbl_1_bss_50, 0x64);
            lbl_1_bss_50 = -1;
        }
        lbl_1_bss_56 = 0;
        for (i = 0; i < 4; i++) {
            s32 score = fn_1_95BC(i);
            if (score > max) {
                max = score;
            }
            scores[i] = score;
        }
        for (i = 0; i < 4; i++) {
            lbl_1_bss_20[i] = -1;
            if (lbl_1_bss_8[i] != 0 && max == scores[i]) {
                lbl_1_bss_20[i] = lbl_1_bss_10[i];
            }
        }
        MgSeqWinnerSet(lbl_1_bss_20[0], lbl_1_bss_20[1], lbl_1_bss_20[2], lbl_1_bss_20[3]);
        if (_CheckFlag(0x30002) == 0) {
            lbl_1_bss_58 = fn_1_9820();
            return;
        }
        fn_1_98F4();
    }
}

void fn_1_7B0(s16 mode, s16 frameNo)
{
    if (_CheckFlag(0x30002) != 0) {
        MgSeqModeSet(9);
        return;
    }
    if (frameNo == 0) {
        lbl_1_data_C[1].center.x = fn_1_9688();
        omCameraViewMove(&lbl_1_data_C[1], 0x48, 0);
    }
    if (frameNo == 0x5A && lbl_1_bss_58 != 0 && _CheckFlag(0x1000F) == 0) {
        HuAudJinglePlay(0x86);
    }
    if ((lbl_1_bss_58 == 0 || _CheckFlag(0x1000F) != 0) && frameNo >= 0x5A) {
        MgSeqModeNext();
    }
}

void fn_1_8A0(s16 mode, s16 frameNo)
{
    s32 i;

    if (frameNo == 0) {
        for (i = 0; i < 4; i++) {
            if (lbl_1_bss_20[i] >= 0) {
                s16 coin = GwPlayer[i].mgCoinBonus + 10;
                if (_CheckFlag(0x1000F) == 0) {
                    GwPlayer[i].mgCoinBonus = coin;
                }
            }
        }
    }
}

void fn_1_958(void)
{
}

void fn_1_95C(void)
{
    fn_1_5B8C();
    fn_1_51BC();
    fn_1_1EA0();
    fn_1_4344();
    fn_1_8300();
}

void fn_1_98C(OMOBJ *obj)
{
    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M514CAM), 0x10000000);
    memset(obj->data, 0, sizeof(M514CAM));
    obj->objFunc = fn_1_9E4;
}

void fn_1_9E4(OMOBJ *obj)
{
    M514CAM *cam = obj->data;
    HuVecF pos;
    HuVecF up;
    HuVecF target;
    HuVecF dir;
    HuVecF base;
    f32 rotX;
    f32 rotY;
    f32 rotZ;
    f32 lenXZ;
    f32 len;
    s32 update;

    lbl_1_data_8 += 0.4f;
    if (lbl_1_data_8 > 120.0f) {
        lbl_1_data_8 = 120.0f;
    }
    Hu3DCameraPosGet(1, &pos, &up, &target);
    fn_1_14F4(&lbl_1_bss_28, &target, 1200.0f);
    update = 1;
    if (cam->active != 0) {
        update = 0;
        if (Hu3DMotionEndCheck(cam->camMdl) != 0) {
            Hu3DModelKill(cam->camMdl);
            Center.x = target.x;
            Center.y = target.y;
            Center.z = target.z;
            dir.x = pos.x - target.x;
            dir.y = pos.y - target.y;
            dir.z = pos.z - target.z;
            lenXZ = sqrtf(dir.x * dir.x + dir.z * dir.z);
            CRot.x = 57.29578f * atan2(-dir.y, lenXZ);
            CRot.y = 57.29578f * atan2(dir.x, dir.z);
            CRot.z = 0.0f;
            len = sqrtf(dir.z * dir.z + (dir.x * dir.x + dir.y * dir.y));
            CZoom = len;
            cam->active = 0;
            update = 1;
        }
    }
    if (update != 0) {
        rotX = CRot.x;
        rotY = CRot.y;
        rotZ = CRot.z;
        pos.x = Center.x + CZoom * (sin(M514_RAD(rotY)) * cos(M514_RAD(rotX)));
        pos.y = Center.y + CZoom * -sin(M514_RAD(rotX));
        pos.z = Center.z + CZoom * (cos(M514_RAD(rotY)) * cos(M514_RAD(rotX)));
        target.x = Center.x;
        target.y = Center.y;
        target.z = Center.z;
        base.x = sin(M514_RAD(rotY)) * sin(M514_RAD(rotX));
        base.y = cos(M514_RAD(rotX));
        base.z = cos(M514_RAD(rotY)) * sin(M514_RAD(rotX));
        PSVECSubtract(&pos, &target, &dir);
        PSVECNormalize(&dir, &dir);
        up.x = base.x * (dir.x * dir.x + (1.0f - dir.x * dir.x) * cos(M514_RAD(rotZ)))
            + base.y * (dir.x * dir.y * (1.0 - cos(M514_RAD(rotZ))) - dir.z * sin(M514_RAD(rotZ)))
            + base.z * (dir.x * dir.z * (1.0 - cos(M514_RAD(rotZ))) + dir.y * sin(M514_RAD(rotZ)));
        up.y = base.y * (dir.y * dir.y + (1.0f - dir.y * dir.y) * cos(M514_RAD(rotZ)))
            + base.x * (dir.x * dir.y * (1.0 - cos(M514_RAD(rotZ))) + dir.z * sin(M514_RAD(rotZ)))
            + base.z * (dir.y * dir.z * (1.0 - cos(M514_RAD(rotZ))) - dir.x * sin(M514_RAD(rotZ)));
        up.z = base.z * (dir.z * dir.z + (1.0f - dir.z * dir.z) * cos(M514_RAD(rotZ)))
            + base.x * (dir.x * dir.z * (1.0 - cos(M514_RAD(rotZ))) - dir.y * sin(M514_RAD(rotZ)))
            + base.y * (dir.y * dir.z * (1.0 - cos(M514_RAD(rotZ))) + dir.x * sin(M514_RAD(rotZ)));
        PSVECNormalize(&up, &up);
        Hu3DCameraPosSet(1, pos.x, pos.y, pos.z, up.x, up.y, up.z, target.x, target.y, target.z);
    }
}

void fn_1_14F4(HuVecF *dir, HuVecF *target, f32 dist)
{
    HuVecF norm;
    HuVecF off;
    const HuVecF *upVec = &lbl_1_rodata_88[0];
    f32 d = dist / (f32)tan(0.1745329201221466);

    PSVECNormalize(dir, &norm);
    PSVECScale(&norm, &off, d);
    PSVECSubtract(target, &off, &off);
    if (fabs(norm.y) > 0.5) {
        upVec = &lbl_1_rodata_88[1];
    }
    Hu3DShadowCreate(20.0f, d - dist, d + dist);
    Hu3DShadowPosSet(&off, (HuVecF *)upVec, target);
    Hu3DShadowSizeSet(0x1E0);
    Hu3DShadowTPLvlSet(0.4f);
}

void fn_1_163C(void *motData)
{
    M514CAM *cam = lbl_1_bss_64->data;

    cam->motId = Hu3DMotionCreate(motData);
    cam->camMdl = Hu3DModelCameraCreate(cam->motId, 1);
    Hu3DCameraMotionStart(cam->camMdl, 1);
    cam->active = 1;
}

s32 fn_1_16AC(void)
{
    OMOBJ *obj = lbl_1_bss_64;
    M514CAM *cam = obj->data;

    return cam->active;
}

s16 fn_1_16DC(void)
{
    return lbl_1_bss_56;
}

s16 fn_1_16EC(s32 idx)
{
#ifdef TARGET_PC
    idx &= 3;
#endif
    return lbl_1_bss_20[idx];
}

s32 fn_1_1704(s32 grp)
{
    s32 i;

    for (i = 0; i < 4; i++) {
        if (lbl_1_bss_20[i] < 0) {
            break;
        }
    }
    if (i >= 4) {
        for (i = 0; i < 4; i++) {
            if (grp == lbl_1_bss_18[i] && lbl_1_bss_8[i] != 0) {
                return 1;
            }
        }
        return 0;
    }
    for (i = 0; i < 4; i++) {
        if (grp == lbl_1_bss_18[i]) {
            return (lbl_1_bss_20[i] >= 0) ? 1 : -1;
        }
    }
    return grp;
}

s32 fn_1_1814(void)
{
    return lbl_1_bss_4;
}

void fn_1_1824(s32 playerNo)
{
    lbl_1_bss_0 = 1;
}

void fn_1_1838(s32 idx, s32 grp, s32 charNo)
{
    idx &= 3;
    lbl_1_bss_18[idx] = grp;
    lbl_1_bss_10[idx] = charNo;
}

void fn_1_1870(s32 idx)
{
    lbl_1_bss_8[idx & 3] = 1;
}

void fn_1_1890(s32 playerNo)
{
    lbl_1_bss_60++;
    if (lbl_1_bss_60 == 4) {
        CharLoseVoicePlay(lbl_1_bss_10[0], lbl_1_bss_10[1], lbl_1_bss_10[2], lbl_1_bss_10[3]);
    }
}

s32 fn_1_1904(f32 x)
{
    s32 pan = (s32)(x * 0.055555556f + 48.0f);

    if (pan < 0x30) {
        pan = 0x30;
    }
    if (pan > 0x50) {
        pan = 0x50;
    }
    return pan;
}

s32 fn_1_1960(HuVecF *pos)
{
    HuVecF scr;
    s32 pan;

    Hu3D3Dto2D(pos, 1, &scr);
    pan = (s32)(scr.x * 0.055555556f + 48.0f);
    if (pan < 0x30) {
        pan = 0x30;
    }
    if (pan > 0x50) {
        pan = 0x50;
    }
    return pan;
}

s32 fn_1_19F4(void)
{
    return (s32)lbl_1_data_8;
}

void fn_1_1A18(s32 seId, HuVecF *pos)
{
    HuVecF scr;
    s16 vol = (s16)lbl_1_data_8;
    s16 pan;

    Hu3D3Dto2D(pos, 1, &scr);
    pan = (s16)(scr.x * 0.055555556f + 48.0f);
    if (pan < 0x30) {
        pan = 0x30;
    }
    if (pan > 0x50) {
        pan = 0x50;
    }
    HuAudFXPlayVolPan(seId, vol, pan);
}

void fn_1_1AF4(s32 seId, HuVecF *pos)
{
    HuVecF scr;
    s16 pan;

    if (seId != -1 && HuAudFXStatusGet(seId) == 2) {
        Hu3D3Dto2D(pos, 1, &scr);
        pan = (s16)(scr.x * 0.055555556f + 48.0f);
        if (pan < 0x30) {
            pan = 0x30;
        }
        if (pan > 0x50) {
            pan = 0x50;
        }
        HuAudFXPanning(seId, pan);
    }
}

f32 fn_1_1BCC(f32 from, f32 to, f32 rate)
{
    f32 diff;
    f32 res;

    diff = fmod(to - from, 360.0);
    if (diff < 0.0f) {
        diff += 360.0f;
    }
    if (diff > 180.0f) {
        diff -= 360.0f;
    }
    res = fmod(from + rate * diff, 360.0);
    if (res < 0.0f) {
        res += 360.0f;
    }
    return res;
}

f32 fn_1_1CE8(f32 from, f32 to)
{
    f32 diff = fmod(to - from, 360.0);

    if (diff < 0.0f) {
        diff += 360.0f;
    }
    if (diff > 180.0f) {
        diff -= 360.0f;
    }
    return diff;
}
