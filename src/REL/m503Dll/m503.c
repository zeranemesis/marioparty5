#include "types.h"
#include "game/object.h"
#include "game/hu3d.h"
#include "game/audio.h"
#include "game/wipe.h"
#include "game/gamemes.h"
#include "game/mg/timer.h"
#include "game/charman.h"
#include "game/hsfex.h"
#include "game/memory.h"
#include "humath.h"
#include "REL/m503Dll.h"
#include <string.h>

#ifndef M_PI
#define M_PI 3.141592653589793
#endif

double atan2(double, double);
double fmod(double, double);

typedef void (*VoidFunc)(void);

/* Camera intro state kept in lbl_1_bss_1C->data (0xC bytes). */
typedef struct {
    s32 active;
    s32 stage;
    HU3DMOTID motId;
    HU3DMODELID modelId;
} M503CamData;

extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

extern OMOBJFUNC fn_1_2D4C(OMOBJMAN *objMan);
extern OMOBJFUNC fn_1_2E80(OMOBJMAN *objMan, OMOBJ *obj);
extern void fn_1_3440(OMOBJMAN *objMan);
extern void fn_1_34F4(void);
extern void fn_1_5810(OMOBJMAN *objMan);
extern void fn_1_5D4C(void);
extern void fn_1_A6FC(OMOBJMAN *objMan);
extern void fn_1_A78C(void);
extern void fn_1_C574(OMOBJMAN *objMan);
extern void fn_1_C598(void);
extern void fn_1_D1B4(OMOBJMAN *objMan, OMOBJ **obj);
extern void fn_1_D23C(void);
extern void fn_1_D7B0(OMOBJMAN *objMan);
extern void fn_1_D838(void);

s32 _prolog(void);
void _epilog(void);
void ObjectSetup(void);
void fn_1_348(OMOBJ *obj);
void fn_1_6D0(OMOBJ *obj);
void fn_1_A38(OMOBJ *obj);
void fn_1_DF4(OMOBJ *obj);
void fn_1_10A4(OMOBJ *obj);
void fn_1_13F4(OMOBJ *obj);
void fn_1_16F4(OMOBJ *obj);
s32 fn_1_1CB0(void);
void fn_1_22FC(OMOBJ *obj);
void fn_1_2354(OMOBJ *obj);
void fn_1_298C(OMOBJ *obj);
void fn_1_2A08(OMOBJ *obj);

HuVecF lbl_1_data_0 = { 100.0f, 800.0f, -100.0f };
HuVecF lbl_1_data_C = { 0.3f, -0.8f, 0.3f };
HuVecF lbl_1_data_18 = { 20.0f, 45.0f, 1000.0f };
GXColor lbl_1_data_24 = { 0xFF, 0xFF, 0xFF, 0xFF };
f32 lbl_1_data_28[9] = { 200.0f, 3000.0f, 100.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f };
f32 lbl_1_data_4C[4] = { 81.0f, 158.0f, 210.0f, 10000.0f };

MGTIMER *lbl_1_bss_0;
s16 lbl_1_bss_4;
s16 lbl_1_bss_6;
f32 lbl_1_bss_8;
int lbl_1_bss_C;
int lbl_1_bss_10;
s16 lbl_1_bss_14;
s32 lbl_1_bss_18;
OMOBJ *lbl_1_bss_1C;
OMOBJ *lbl_1_bss_20;
OMOBJMAN *lbl_1_bss_24;

s32 _prolog(void)
{
    const VoidFunc *ctorPtr = _ctors;

    for (; *ctorPtr != 0; ctorPtr++) {
        (*ctorPtr)();
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const VoidFunc *dtorPtr = _dtors;

    while (*dtorPtr != 0) {
        (*dtorPtr)();
        dtorPtr++;
    }
}

void ObjectSetup(void)
{
    OMOBJMAN *objMan;
    HU3DLIGHT *light;
    HuVecF pos;
    HuVecF aim;
    Mtx mtx;

    lbl_1_bss_10 = -1;
    lbl_1_bss_C = -1;
    lbl_1_bss_8 = 127.0f;
    lbl_1_bss_0 = NULL;
    lbl_1_bss_14 = Hu3DGLightCreateV(&lbl_1_data_0, &lbl_1_data_C, &lbl_1_data_24);
    Hu3DGLightInfinitytSet(lbl_1_bss_14);
    light = &Hu3DGlobalLight[lbl_1_bss_14];
    light->type |= 0x8000;
    aim.z = 0.0f;
    aim.y = 0.0f;
    aim.x = 0.0f;
    pos.z = 0.0f;
    pos.x = 0.0f;
    pos.y = lbl_1_data_18.z;
    mtxRot(mtx, lbl_1_data_18.x, lbl_1_data_18.y, 0.0f);
    PSMTXMultVec(mtx, &pos, &pos);
    Hu3DGLightPosAimSetV(lbl_1_bss_14, &pos, &aim);
    objMan = omInitObjMan(0x32, 0x2000);
    lbl_1_bss_24 = objMan;
    omGameSysInit(objMan);
    lbl_1_bss_18 = 0;
    lbl_1_bss_4 = -1;
    lbl_1_bss_6 = -1;
    Hu3DFogSet(3000.0f, 70000.0f, 0xC8, 0xC8, 0xC8);
    Hu3DCameraCreate(1);
    Hu3DCameraPerspectiveSet(1, 45.0f, 10.0f, 30000.0f, 1.2f);
    lbl_1_bss_1C = omAddObjEx(objMan, 0x5A, 0, 0, OM_GRP_NONE, fn_1_22FC);
    lbl_1_bss_20 = omAddObjEx(objMan, 0xA, 0, 0, OM_GRP_NONE, fn_1_348);
    fn_1_D1B4(objMan, &lbl_1_bss_20);
    fn_1_D7B0(objMan);
    fn_1_C574(objMan);
    fn_1_A6FC(objMan);
    fn_1_3440(objMan);
    fn_1_5810(objMan);
}

void fn_1_348(OMOBJ *obj)
{
    M503Data *data;

    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M503Data), HU_MEMNUM_OVL);
    data = obj->data;
    memset(data, 0, sizeof(M503Data));
    data->state = 0;
    data->stateTime = 0;
    data->unk10 = 0;
    data->time14 = 0;
    data->phase = 0;
    data->phaseTime = 0;
    data->unk20 = 0;
    data->time24 = 0;
    data->lock = 0;
    data->wipeDone = 0;
    data->unk70 = 2;
    data->unk48[3] = -1;
    data->unk48[2] = -1;
    data->unk48[1] = -1;
    data->unk48[0] = -1;
    data->unk74 = 0;
    data->unk78 = 0;
    data->unk7C = 0;
    obj->objFunc = fn_1_6D0;
}

void fn_1_430(OMOBJ *obj)
{
    M503Data *data = obj->data;

    data->stateTime++;
    data->time14++;
    data->phaseTime++;
    data->time24++;
    if (data->wipeDone == 0) {
        if (data->state == 1 && (f32)data->time14 > 6.0) {
            WipeCreate(1, 5, 0x3C);
            data->wipeDone = 1;
        }
    } else if (data->state == 1 && data->time14 == 0x18) {
        lbl_1_bss_10 = HuAudBGMPlay(0x51);
        lbl_1_bss_C = HuAudFXPlay(0x3A3);
    }
    if (omSysExitReq != 0 && data->state != 5) {
        data->state = 4;
        if (lbl_1_bss_10 != -1) {
            HuAudSStreamFadeOut(lbl_1_bss_10, 0x3E8);
            lbl_1_bss_10 = -1;
        }
        HuAudFadeOut(0x3E8);
        fn_1_5B0(obj);
    }
}

void fn_1_5B0(OMOBJ *obj)
{
    M503Data *data = obj->data;

    if (data->lock == 0) {
        switch (data->state) {
            case 0:
                data->state = 1;
                data->time14 = 0;
                obj->objFunc = fn_1_2D4C(lbl_1_bss_24);
                break;
            case 1:
                data->state = 2;
                data->phaseTime = 0;
                obj->objFunc = fn_1_10A4;
                break;
            case 2:
                data->state = 3;
                data->time24 = 0;
                obj->objFunc = fn_1_2E80(lbl_1_bss_24, obj);
                break;
            case 3:
                data->lock = 1;
                data->state = 5;
                obj->objFunc = fn_1_A38;
                break;
            default:
                data->state = 5;
                obj->objFunc = fn_1_DF4;
                break;
        }
        data->stateTime = 0;
    }
}

void fn_1_6D0(OMOBJ *obj)
{
    fn_1_430(obj);
    fn_1_5B0(obj);
}

void fn_1_A38(OMOBJ *obj)
{
    M503Data *data = obj->data;

    fn_1_430(obj);
    if (WipeCheckEnd() == 0) {
        WipeCreate(2, 6, 0x3C);
        if (lbl_1_bss_C != -1) {
            HuAudFXFadeOut(lbl_1_bss_C, 0x3E8);
            lbl_1_bss_C = -1;
        }
        data->lock = 0;
        fn_1_5B0(obj);
    }
}

void fn_1_DF4(OMOBJ *obj)
{
    fn_1_430(obj);
    if (WipeCheckEnd() == 0) {
        fn_1_C598();
        fn_1_A78C();
        fn_1_5D4C();
        fn_1_34F4();
        fn_1_D838();
        fn_1_D23C();
        GameMesClose();
        HuAudFadeOut(1);
        omOvlReturnEx(1, 1);
    }
}

void fn_1_10A4(OMOBJ *obj)
{
    M503Data *data = obj->data;

    fn_1_430(obj);
    switch (data->phase) {
        case 0:
            lbl_1_bss_6 = GameMesCreate(2, 0);
            data->phase = 1;
            data->phaseTime = 0;
            break;
        case 1:
            if (GameMesStatGet(lbl_1_bss_6) == 0 && data->lock == 0) {
                lbl_1_bss_0 = MgTimerCreate(0);
                MgTimerParamSet(lbl_1_bss_0, 0x708, 0, 0);
                MgTimerModeOnSet(lbl_1_bss_0, 1);
                data->phase = 2;
                data->phaseTime = 0;
                obj->objFunc = fn_1_13F4;
            }
            break;
    }
}

void fn_1_13F4(OMOBJ *obj)
{
    M503Data *data = obj->data;
    s32 done = 0;

    fn_1_430(obj);
    if (MgTimerDoneCheck(lbl_1_bss_0) != 0) {
        done = 1;
    }
    if (done != 0) {
        lbl_1_bss_4 = GameMesCreate(2, 1);
        if (lbl_1_bss_10 != -1) {
            HuAudSStreamFadeOut(lbl_1_bss_10, 0x64);
        }
        data->phase = 3;
        data->phaseTime = 0;
        if (data->lock == 0) {
            obj->objFunc = fn_1_16F4;
        }
    }
}

void fn_1_16F4(OMOBJ *obj)
{
    M503Data *data = obj->data;

    if (lbl_1_bss_0 != NULL) {
        MgTimerKill(lbl_1_bss_0);
        lbl_1_bss_0 = NULL;
    }
    fn_1_430(obj);
    if (fn_1_1CB0() != 0 && data->unk74 == 0x1111) {
        data->phase = 4;
        data->phaseTime = 0;
        fn_1_5B0(obj);
    }
}

s32 fn_1_1AB0(void)
{
    return ((M503Data *)lbl_1_bss_20->data)->state;
}

s32 fn_1_1AC8(void)
{
    return ((M503Data *)lbl_1_bss_20->data)->unk10;
}

s32 fn_1_1AE0(void)
{
    return ((M503Data *)lbl_1_bss_20->data)->phase;
}

s32 fn_1_1AF8(void)
{
    return ((M503Data *)lbl_1_bss_20->data)->unk20;
}

s32 fn_1_1B10(void)
{
    return ((M503Data *)lbl_1_bss_20->data)->unk70;
}

void fn_1_1B28(s32 val)
{
    ((M503Data *)lbl_1_bss_20->data)->unk70 = val;
}

s32 fn_1_1B40(s32 idx)
{
    M503Data *data = lbl_1_bss_20->data;
    s32 val = data->unk48[idx];

    return val;
}

void fn_1_1B70(s32 idx)
{
    M503Data *data = lbl_1_bss_20->data;

    data->unk74 |= 1 << ((idx & 3) * 4);
}

void fn_1_1BA0(s32 idx, s32 val)
{
    M503Data *data = lbl_1_bss_20->data;
    s32 shift = (idx & 3) * 4;

    data->unk78 &= ~(0xF << shift);
    data->unk78 |= val << shift;
}

s32 fn_1_1BFC(s32 mode)
{
    s32 val = ((M503Data *)lbl_1_bss_20->data)->unk78;

    switch (mode) {
        case 1:
            return val == 0x1111;
        case 2:
            return val == 0x2222;
        case 0:
            return val == 0;
        default:
            return 0;
    }
}

void fn_1_1C80(s32 idx)
{
    M503Data *data = lbl_1_bss_20->data;

    data->unk7C |= 1 << ((idx & 3) * 4);
}

s32 fn_1_1CB0(void)
{
    return GameMesStatGet(lbl_1_bss_4) == 0;
}

s32 fn_1_1CE8(void)
{
    return ((M503Data *)lbl_1_bss_20->data)->unk58;
}

s32 fn_1_1D00(void)
{
    return ((M503Data *)lbl_1_bss_20->data)->unk5C;
}

void *fn_1_1D18(void)
{
    return ((M503Data *)lbl_1_bss_20->data)->unk60;
}

u32 fn_1_1D30(void)
{
    if (lbl_1_bss_0 == NULL) {
        return 0x1E;
    }
    return MgTimerValueGet(lbl_1_bss_0) / 60;
}

void fn_1_1D98(s32 idx, s32 val0, s32 val1)
{
    M503Data *data = lbl_1_bss_20->data;
    s32 i = idx & 3;

    data->unk28[i] = val0;
    data->unk38[i] = val1;
}

void fn_1_1DD8(void)
{
    M503Data *data = lbl_1_bss_20->data;

    lbl_1_bss_18++;
    if (lbl_1_bss_18 == 4) {
        CharLoseVoicePlay(data->unk38[0], data->unk38[1], data->unk38[2], data->unk38[3]);
    }
}

s32 fn_1_1E54(f32 x)
{
    s32 pan = 32.0f + x * 0.11111111f;

    if (pan < 0x20) {
        pan = 0x20;
    }
    if (pan > 0x60) {
        pan = 0x60;
    }
    return pan;
}

s32 fn_1_1EB0(HuVecF *pos)
{
    HuVecF scr;
    s32 pan;

    Hu3D3Dto2D(pos, 1, &scr);
    pan = 32.0f + scr.x * 0.11111111f;
    if (pan < 0x20) {
        pan = 0x20;
    }
    if (pan > 0x60) {
        pan = 0x60;
    }
    return pan;
}

s32 fn_1_1F44(HuVecF *pos)
{
    s32 vol = 112.0f + (f32)((pos->z - -400.0) / 800.0) * 15.0f;

    if (vol < 0x70) {
        vol = 0x70;
    }
    if (vol > 0x7F) {
        vol = 0x7F;
    }
    return vol;
}

s32 fn_1_1FD8(s32 seId, HuVecF *pos)
{
    HuVecF scr;
    s16 pan;
    s16 vol;

    Hu3D3Dto2D(pos, 1, &scr);
    pan = 32.0f + scr.x * 0.11111111f;
    if (pan < 0x20) {
        pan = 0x20;
    }
    if (pan > 0x60) {
        pan = 0x60;
    }
    vol = 112.0f + (f32)((pos->z - -400.0) / 800.0) * 15.0f;
    if (vol < 0x70) {
        vol = 0x70;
    }
    if (vol > 0x7F) {
        vol = 0x7F;
    }
    return HuAudFXPlayVolPan(seId, vol, pan);
}

void fn_1_210C(s32 seNo, HuVecF *pos)
{
    HuVecF scr;
    s16 pan;
    s16 vol;

    if (seNo != -1 && HuAudFXStatusGet(seNo) == 2) {
        Hu3D3Dto2D(pos, 1, &scr);
        pan = 32.0f + scr.x * 0.11111111f;
        if (pan < 0x20) {
            pan = 0x20;
        }
        if (pan > 0x60) {
            pan = 0x60;
        }
        HuAudFXPanning(seNo, pan);
        vol = 112.0f + (f32)((pos->z - -400.0) / 800.0) * 15.0f;
        if (vol < 0x70) {
            vol = 0x70;
        }
        if (vol > 0x7F) {
            vol = 0x7F;
        }
        HuAudFXVolSet(seNo, vol);
    }
}

void fn_1_2260(s32 seId, f32 x)
{
    s16 pan = 32.0f + x * 0.11111111f;

    if (pan < 0x20) {
        pan = 0x20;
    }
    if (pan > 0x60) {
        pan = 0x60;
    }
    HuAudFXPlayPan(seId, pan);
}

void fn_1_22FC(OMOBJ *obj)
{
    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M503CamData), HU_MEMNUM_OVL);
    memset(obj->data, 0, sizeof(M503CamData));
    obj->objFunc = fn_1_2354;
}

void fn_1_2354(OMOBJ *obj)
{
    M503CamData *cam = obj->data;
    HuVecF pos;
    HuVecF up;
    HuVecF target;
    HuVecF dir;
    f32 rotX;
    f32 rotY;
    f32 rotZ;

    fn_1_2A08(obj);
    if (cam->active != 0) {
        fn_1_298C(obj);
        if (Hu3DMotionEndCheck(cam->modelId) != 0) {
            Hu3DModelKill(cam->modelId);
            Hu3DCameraPosGet(1, &pos, &up, &target);
            Center.x = target.x;
            Center.y = target.y;
            Center.z = target.z;
            dir.x = pos.x - target.x;
            dir.y = pos.y - target.y;
            dir.z = pos.z - target.z;
            CRot.x = 57.29578f * atan2(-dir.y, HuMagXZVecF(&dir));
            CRot.y = 57.29578f * atan2(dir.x, dir.z);
            CRot.z = 0.0f;
            CZoom = HuMagVecF(&dir);
            cam->active = 0;
        }
    }
    if (cam->active == 0) {
        rotX = CRot.x;
        rotY = CRot.y;
        rotZ = CRot.z;
        pos.x = Center.x + CZoom * (HuSin(rotY) * HuCos(rotX));
        pos.y = Center.y + CZoom * -HuSin(rotX);
        pos.z = Center.z + CZoom * (HuCos(rotY) * HuCos(rotX));
        target.x = Center.x;
        target.y = Center.y;
        target.z = Center.z;
        up.x = HuSin(rotY) * HuSin(rotX);
        up.y = HuCos(rotX);
        up.z = HuCos(rotY) * HuSin(rotX);
        Hu3DCameraPosSet(1, pos.x, pos.y, pos.z, up.x, up.y, up.z, target.x, target.y, target.z);
    }
}

void fn_1_298C(OMOBJ *obj)
{
    M503CamData *cam = obj->data;
    f32 time;

    time = Hu3DMotionTimeGet(cam->modelId);
    if (Hu3DMotionTimeGet(cam->modelId) > lbl_1_data_4C[cam->stage]) {
        cam->stage++;
        HuAudFXPlay(0x63D);
    }
}

void fn_1_2A08(OMOBJ *obj)
{
    if (((M503CamData *)obj->data)->stage >= 3) {
        if (lbl_1_bss_8 > 96.0f) {
            lbl_1_bss_8 = lbl_1_bss_8 - 1.0333333333333334;
        }
        if (lbl_1_bss_C != -1) {
            HuAudFXVolSet(lbl_1_bss_C, lbl_1_bss_8);
        }
    }
}

void fn_1_2AD4(void *motData)
{
    M503CamData *cam = lbl_1_bss_1C->data;

    cam->motId = Hu3DMotionCreate(motData);
    cam->modelId = Hu3DModelCameraCreate(cam->motId, 1);
    Hu3DCameraMotionStart(cam->modelId, 1);
    cam->active = 1;
    cam->stage = 0;
}

s32 fn_1_2B4C(void)
{
    OMOBJ *obj = lbl_1_bss_1C;
    M503CamData *cam = obj->data;

    return cam->active;
}

f32 fn_1_2B7C(f32 from, f32 to, f32 rate)
{
    f32 diff = fmod(to - from, 360.0);
    f32 res;

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

f32 fn_1_2C98(f32 from, f32 to)
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
