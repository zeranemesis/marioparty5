#include "types.h"
#include "game/object.h"
#include "game/hu3d.h"
#include "game/hsfex.h"
#include "game/audio.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/memory.h"
#include "game/process.h"
#include "game/mg/seqman.h"
#include "humath.h"
#include "datadir_enum.h"
#include <string.h>

#include "REL/m580Dll.h"

#ifndef M_PI
#define M_PI 3.141592653589793
#endif

double atan2(double, double);
double fmod(double, double);

typedef void (*VoidFunc)(void);

extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

/* camera object work block (0x8C bytes, hung off OMOBJ::data) */
typedef struct M580CamData_s {
    /* 0x00 */ s32 motionActive;
    /* 0x04 */ HU3DMOTID motId;
    /* 0x06 */ HU3DMODELID modelId;
    /* 0x08 */ s32 follow;
    /* 0x0C */ s32 pointNum;
    /* 0x10 */ HuVecF points[8];
    /* 0x70 */ HuVecF center;
    /* 0x7C */ HuVecF rot;
    /* 0x88 */ f32 zoom;
} M580CamData;

s32 _prolog(void);
void _epilog(void);
void ObjectSetup(void);
void fn_1_37C(void);
void fn_1_424(s16 mode, s16 frame);
void fn_1_510(s16 mode, s16 frame);
void fn_1_544(void);
void fn_1_548(void);
void fn_1_574(s16 mode, s16 frame);
void fn_1_6FC(s16 mode, s16 frame);
void fn_1_740(s16 mode, s16 frame);
void fn_1_78C(void);
void fn_1_790(void);
void fn_1_7C4(OMOBJ *obj);
void fn_1_834(OMOBJ *obj);
void fn_1_898(OMOBJ *obj);
void fn_1_C5C(OMOBJ *obj);
void fn_1_10D8(OMOBJ *obj);
void fn_1_1838(void *motData);
s32 fn_1_18A8(void);
void fn_1_18D8(s32 follow);
void fn_1_246C(s32 time);
void fn_1_248C(void);

void fn_1_EE34(OMOBJMAN *objMan);
void fn_1_110A0(OMOBJMAN *objMan);
void fn_1_8358(OMOBJMAN *objMan);
void fn_1_2804(OMOBJMAN *objMan);
void fn_1_8A64(OMOBJMAN *objMan);
void fn_1_C8F8(OMOBJMAN *objMan);
void fn_1_D7E0(OMOBJMAN *objMan);
void fn_1_83F8(void);
void fn_1_C998(void);
void fn_1_29EC(void);
void fn_1_8B60(void);
void fn_1_11128(void);
void fn_1_EEBC(void);
s32 fn_1_1100C(s32 team);
s32 fn_1_11044(void);
s32 fn_1_1104C(void);
s32 fn_1_D544(void);
s32 fn_1_D6EC(void);

OMCAMERAVIEW lbl_1_data_0 = { { 0.0f, 1350.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, 1180.0f };
HuVecF lbl_1_data_1C = { 100.0f, 800.0f, -100.0f };
HuVecF lbl_1_data_28 = { 0.3f, -0.8f, 0.3f };
HuVecF lbl_1_data_34 = { 20.0f, 45.0f, 1000.0f };
GXColor lbl_1_data_40 = { 0xFF, 0xFF, 0xFF, 0xFF };
HuVecF lbl_1_data_44 = { 0.0f, 1000.0f, 1500.0f };
HuVecF lbl_1_data_50 = { 0.0f, 0.0f, 1.0f };
HuVecF lbl_1_data_5C = { 0.0f, 0.0f, 0.0f };
MGSEQ_PARAM lbl_1_data_68 = {
    0,
    MGSEQ_TIMER_BOTTOM,
    (MGSEQ_FUNC)fn_1_37C,
    fn_1_424,
    fn_1_510,
    (MGSEQ_FUNC)fn_1_548,
    fn_1_574,
    fn_1_6FC,
    fn_1_740,
    (MGSEQ_FUNC)fn_1_78C,
    (MGSEQ_FUNC)fn_1_790,
};

s16 lbl_1_bss_0[4];
s16 lbl_1_bss_8[4];
s16 lbl_1_bss_10[4];
s16 lbl_1_bss_18[4];
s32 lbl_1_bss_20;
s32 lbl_1_bss_24;
s32 lbl_1_bss_28;
s32 lbl_1_bss_2C;
s16 lbl_1_bss_30;
s32 lbl_1_bss_34;
s32 lbl_1_bss_38;
s32 lbl_1_bss_3C;
s32 lbl_1_bss_40;
OMOBJ *lbl_1_bss_44;
OMOBJMAN *lbl_1_bss_48;

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
    HuVecF unused0 = { 0.0f, 0.0f, 0.0f };
    HuVecF unused1 = { 0.0f, 0.0f, -1.0f };
    HuVecF pos;
    HuVecF aim;
    Mtx mtx;
    s32 i;

    lbl_1_bss_40 = 0;
    lbl_1_bss_3C = 0;
    lbl_1_bss_34 = 0;
    lbl_1_bss_2C = -1;
    lbl_1_bss_28 = -1;
    for (i = 0; i < 4; i++) {
        lbl_1_bss_18[i] = 0;
    }
    lbl_1_bss_48 = omInitObjMan(0x50, 0x2000);
    omGameSysInit(lbl_1_bss_48);
    Hu3DCameraCreate(1);
    Hu3DCameraPerspectiveSet(1, 45.0f, 20.0f, 10000.0f, 1.2f);
    lbl_1_bss_44 = omAddObjEx(lbl_1_bss_48, 0x5A, 0, 0, OM_GRP_NONE, fn_1_7C4);
    omCameraViewSet(&lbl_1_data_0);
    lbl_1_bss_30 = Hu3DGLightCreateV(&lbl_1_data_1C, &lbl_1_data_28, &lbl_1_data_40);
    Hu3DGLightInfinitytSet(lbl_1_bss_30);
    Hu3DGLightStaticSet(lbl_1_bss_30, 1);
    aim.z = 0.0f;
    aim.y = 0.0f;
    aim.x = 0.0f;
    pos.z = 0.0f;
    pos.x = 0.0f;
    pos.y = lbl_1_data_34.z;
    mtxRot(mtx, lbl_1_data_34.x, lbl_1_data_34.y, 0.0f);
    PSMTXMultVec(mtx, &pos, &pos);
    Hu3DGLightPosAimSetV(lbl_1_bss_30, &pos, &aim);
    Hu3DShadowCreate(80.0f, 20.0f, 5000.0f);
    Hu3DShadowPosSet(&lbl_1_data_44, &lbl_1_data_50, &lbl_1_data_5C);
    Hu3DShadowColSet(0x32, 0x32, 0x32);
    Hu3DShadowSizeSet(0xF0);
    MgSeqCreate(&lbl_1_data_68);
    MgSeqModeHookAdd(4, (MGSEQ_FUNC)fn_1_544);
}

void fn_1_37C(void)
{
    fn_1_EE34(lbl_1_bss_48);
    fn_1_110A0(lbl_1_bss_48);
    fn_1_8358(lbl_1_bss_48);
    fn_1_2804(lbl_1_bss_48);
    fn_1_8A64(lbl_1_bss_48);
    fn_1_C8F8(lbl_1_bss_48);
    fn_1_D7E0(lbl_1_bss_48);
    fn_1_1838(HuDataSelHeapReadNum(DATANUM(DATA_m580, 0x0F), HU_MEMNUM_OVL, HEAP_MODEL));
    MgSeqModeNext();
}

void fn_1_424(s16 mode, s16 frame)
{
    if (frame == 0) {
        lbl_1_bss_38 = 0;
    }
    if (lbl_1_bss_2C == -1 && frame >= 0x12) {
        lbl_1_bss_2C = HuAudBGMPlay(0x54);
        lbl_1_bss_28 = HuAudFXPlayVol(0x39C, 0x54);
    }
    switch (lbl_1_bss_38) {
        case 0:
            if (fn_1_18A8() == 0 && fn_1_D544() == 0) {
                lbl_1_bss_38++;
                MgSeqModeNext();
            }
            break;
        case 1:
            break;
    }
}

void fn_1_510(s16 mode, s16 frame)
{
    if (frame == 0) {
        fn_1_246C(0xB4);
    }
}

void fn_1_544(void)
{
}

void fn_1_548(void)
{
    if (fn_1_1104C() == 0) {
        MgSeqModeNext();
    }
}

void fn_1_574(s16 mode, s16 frame)
{
    s16 winner;
    s32 scoreA;
    s32 scoreB;
    s32 i;

    if (frame == 0) {
        if (lbl_1_bss_2C != -1) {
            HuAudSStreamFadeOut(lbl_1_bss_2C, 0x64);
            lbl_1_bss_2C = -1;
        }
        HuAudFXPlay(0x966);
        fn_1_246C(0xB4);
        scoreA = fn_1_1100C(0);
        scoreB = fn_1_1100C(1);
        for (i = 0; i < 4; i++) {
            lbl_1_bss_10[i] = -1;
            if ((scoreA > scoreB && lbl_1_bss_8[i] == 0) || (scoreA < scoreB && lbl_1_bss_8[i] == 1)) {
                lbl_1_bss_10[i] = lbl_1_bss_0[i];
            }
        }
        winner = MgSeqWinnerSet(lbl_1_bss_10[0], lbl_1_bss_10[1], lbl_1_bss_10[2], lbl_1_bss_10[3]);
        lbl_1_bss_34 = fn_1_11044();
    }
}

void fn_1_6FC(s16 mode, s16 frame)
{
    if (lbl_1_bss_34 == 0 && frame >= 0x78) {
        MgSeqModeNext();
    }
}

void fn_1_740(s16 mode, s16 frame)
{
    s32 i;

    if (frame == 0) {
        for (i = 0; i < 0x13; i++) {
            HuPrcVSleep();
        }
    }
}

void fn_1_78C(void)
{
}

void fn_1_790(void)
{
    fn_1_83F8();
    fn_1_C998();
    fn_1_29EC();
    fn_1_8B60();
    fn_1_11128();
    fn_1_EEBC();
}

void fn_1_7C4(OMOBJ *obj)
{
    M580CamData *data;

    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M580CamData), HU_MEMNUM_OVL);
    data = obj->data;
    memset(data, 0, sizeof(M580CamData));
    fn_1_18D8(1);
    obj->objFunc = fn_1_834;
}

void fn_1_834(OMOBJ *obj)
{
    M580CamData *data = obj->data;

    fn_1_898(obj);
    if (data->motionActive == 0) {
        fn_1_C5C(obj);
        fn_1_10D8(obj);
    }
    fn_1_248C();
    data->pointNum = 0;
}

void fn_1_898(OMOBJ *obj)
{
    M580CamData *data = obj->data;
    HuVecF pos;
    HuVecF up;
    HuVecF target;
    HuVecF dir;
    f32 xzMag;

    if (data->motionActive != 0) {
        Hu3DCameraPosGet(1, &pos, &up, &target);
        Center = target;
        data->center = Center;
        dir.x = pos.x - target.x;
        dir.y = pos.y - target.y;
        dir.z = pos.z - target.z;
        xzMag = HuMagXZVecF(&dir);
        CRot.x = HuAtan(-dir.y, xzMag);
        CRot.y = HuAtan(dir.x, dir.z);
        CRot.z = 0.0f;
        CZoom = HuMagVecF(&dir);
        if (Hu3DMotionEndCheck(data->modelId) != 0) {
            Hu3DModelKill(data->modelId);
            data->rot = CRot;
            data->zoom = CZoom;
            data->motionActive = 0;
        }
    }
}

void fn_1_C5C(OMOBJ *obj)
{
    M580CamData *data = obj->data;
    HuVecF *pt = data->points;
    f32 tgtX;
    f32 tgtY;
    f32 tgtZ;
    f32 tgtRotX;
    f32 tgtZoom;
    s32 zoomOut;
    s32 hasRight;
    s32 hasLeft;
    s32 i;

    if (data->follow != 0) {
        tgtX = data->center.x;
        tgtY = data->center.y;
        tgtZ = data->center.z;
        tgtRotX = data->rot.x;
        tgtZoom = data->zoom;
        if (fn_1_1104C() == 0) {
            if (MgSeqModeGet() != 6) {
                tgtY = tgtY - 600.0;
                tgtZ = tgtZ - 1000.0;
                tgtRotX = tgtRotX + 20.0;
            }
        } else if (fn_1_D544() != 0 || fn_1_D6EC() != 0) {
            zoomOut = 0;
            lbl_1_bss_40++;
            for (i = 0; i < data->pointNum; i++, pt++) {
                if (pt->x > 650.0 || pt->x < -650.0) {
                    break;
                }
            }
            if (i >= data->pointNum) {
                if (MgSeqModeGet() == 5) {
                    if ((f32)lbl_1_bss_40 > 90.0) {
                        zoomOut = 1;
                    }
                } else if ((f32)lbl_1_bss_40 > 0.0) {
                    zoomOut = 1;
                }
            }
            if (zoomOut != 0) {
                tgtZ = tgtZ + 300.0;
                tgtZoom = tgtZoom - 1200.0;
            } else {
                goto track;
            }
        } else {
            lbl_1_bss_40 = 0;
track:
            hasRight = 0;
            hasLeft = 0;
            for (i = 0; i < data->pointNum; i++, pt++) {
                if (pt->x > 800.0) {
                    hasRight = 1;
                }
                if (pt->x < -800.0) {
                    hasLeft = 1;
                }
            }
            if (hasRight != 0) {
                tgtX = tgtX + 300.0;
                tgtZoom = tgtZoom + 300.0;
            }
            if (hasLeft != 0) {
                tgtX = tgtX - 300.0;
                tgtZoom = tgtZoom + 300.0;
                if (hasRight != 0) {
                    tgtZoom = tgtZoom + 300.0;
                }
            }
        }
        Center.x = Center.x + 0.03 * (tgtX - Center.x);
        Center.y = Center.y + 0.03 * (tgtY - Center.y);
        Center.z = Center.z + 0.03 * (tgtZ - Center.z);
        CRot.x = CRot.x + 0.03 * (tgtRotX - CRot.x);
        CZoom = CZoom + 0.03 * (tgtZoom - CZoom);
    }
}

void fn_1_10D8(OMOBJ *obj)
{
    HuVecF pos;
    HuVecF target;
    HuVecF base;
    HuVecF dir;
    HuVecF up;
    f32 rx = CRot.x;
    f32 ry = CRot.y;
    f32 rz = CRot.z;

    pos.x = Center.x + CZoom * (HuSin(ry) * HuCos(rx));
    pos.y = Center.y + CZoom * -HuSin(rx);
    pos.z = Center.z + CZoom * (HuCos(ry) * HuCos(rx));
    target.x = Center.x;
    target.y = Center.y;
    target.z = Center.z;
    base.x = HuSin(ry) * HuSin(rx);
    base.y = HuCos(rx);
    base.z = HuCos(ry) * HuSin(rx);
    VECSubtract(&pos, &target, &dir);
    VECNormalize(&dir, &dir);
    up.x = base.x * (dir.x * dir.x + (1.0f - dir.x * dir.x) * HuCos(rz))
         + base.y * (dir.x * dir.y * (1.0 - HuCos(rz)) - dir.z * HuSin(rz))
         + base.z * (dir.x * dir.z * (1.0 - HuCos(rz)) + dir.y * HuSin(rz));
    up.y = base.y * (dir.y * dir.y + (1.0f - dir.y * dir.y) * HuCos(rz))
         + base.x * (dir.x * dir.y * (1.0 - HuCos(rz)) + dir.z * HuSin(rz))
         + base.z * (dir.y * dir.z * (1.0 - HuCos(rz)) - dir.x * HuSin(rz));
    up.z = base.z * (dir.z * dir.z + (1.0f - dir.z * dir.z) * HuCos(rz))
         + (base.x * (dir.x * dir.z * (1.0 - HuCos(rz)) - dir.y * HuSin(rz))
            + base.y * (dir.y * dir.z * (1.0 - HuCos(rz)) + dir.x * HuSin(rz)));
    VECNormalize(&up, &up);
    Hu3DCameraPosSet(1, pos.x, pos.y, pos.z, up.x, up.y, up.z, target.x, target.y, target.z);
}

void fn_1_1838(void *motData)
{
    M580CamData *data = lbl_1_bss_44->data;

    data->motId = Hu3DMotionCreate(motData);
    data->modelId = Hu3DModelCameraCreate(data->motId, 1);
    Hu3DCameraMotionStart(data->modelId, 1);
    data->motionActive = 1;
}

s32 fn_1_18A8(void)
{
    OMOBJ *obj = lbl_1_bss_44;
    M580CamData *data = obj->data;

    return data->motionActive;
}

void fn_1_18D8(s32 follow)
{
    OMOBJ *obj = lbl_1_bss_44;
    M580CamData *data = obj->data;

    data->follow = follow;
}

void fn_1_1908(HuVecF *pos)
{
    M580CamData *data = lbl_1_bss_44->data;

    if (data->pointNum < 8) {
        data->points[data->pointNum++] = *pos;
    }
}

f32 fn_1_1970(void)
{
    f32 y = Center.y + CZoom * -HuSin(CRot.x);
    f32 z = Center.z + CZoom * (HuCos(CRot.y) * HuCos(CRot.x));
    f32 mag = HuMagPoint2D(y, z);
    f32 dist = (mag - 800.0) / 2000.0;
    f32 angle;

    if (dist < 0.0f) {
        dist = 0.0f;
    }
    if (dist > 1.0f) {
        dist = 1.0f;
    }
    angle = (20.0 - (48.0 + CRot.x)) / 20.0;
    if (angle < 0.0f) {
        angle = 0.0f;
    }
    if (angle > 1.0f) {
        angle = 1.0f;
    }
    return dist * (0.5 + 0.5 * angle);
}

u32 fn_1_1D04(void)
{
    return fn_1_1104C() / 60;
}

s16 fn_1_1D40(s32 index)
{
    return lbl_1_bss_10[index];
}

u32 fn_1_1D58(u32 team)
{
    s32 i;

    for (i = 0; i < 4; i++) {
        if (lbl_1_bss_10[i] >= 0) {
            break;
        }
    }
    if (i >= 4) {
        return 0;
    }
    for (i = 0; i < 4; i++) {
        if (team == (u32)lbl_1_bss_8[i]) {
            if (lbl_1_bss_10[i] >= 0) {
                return 1;
            } else {
                return -1;
            }
        }
    }
    return team;
}

void fn_1_1E14(s32 index, s16 team, s16 charNo)
{
    lbl_1_bss_8[index & 3] = team;
    lbl_1_bss_0[index & 3] = charNo;
}

s16 fn_1_1E4C(u32 team, u32 nth)
{
    s32 i;
    u32 count = 0;

    for (i = 0; i < 4; i++) {
        if (team == (u32)lbl_1_bss_8[i]) {
            if (nth == count) {
                return lbl_1_bss_0[i];
            }
            count++;
        }
    }
    return -1;
}

void fn_1_1EC4(u32 index)
{
    /* NONMATCHING: MWCC hoists `li r6,1` ahead of the mask and puts the mask in r0
   (clrlwi r0,r3,30) where the original keeps it in place (clrlwi r3,r3,30).
   Register tie-break; the address form and every other row are identical. */
    lbl_1_bss_18[index & 3] = 1;
}

s32 fn_1_1EE4(void)
{
    s32 i;

    for (i = 0; i < 4; i++) {
        if (lbl_1_bss_18[i] == 0) {
            return 0;
        }
    }
    return 1;
}

void fn_1_1F34(void)
{
    if (++lbl_1_bss_3C == 4) {
        CharLoseVoicePlay(lbl_1_bss_0[0], lbl_1_bss_0[1], lbl_1_bss_0[2], lbl_1_bss_0[3]);
    }
}

s32 fn_1_1FA8(f32 x)
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

s32 fn_1_2004(HuVecF *pos)
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

s32 fn_1_2098(HuVecF *pos)
{
    s32 vol = 96.0f + (f32)((pos->z - -700.0) / 1400.0) * 31.0f;

    if (vol < 0x60) {
        vol = 0x60;
    }
    if (vol > 0x7F) {
        vol = 0x7F;
    }
    return vol;
}

void fn_1_212C(s32 seId, HuVecF *pos)
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
    vol = 96.0f + (f32)((pos->z - -700.0) / 1400.0) * 31.0f;
    if (vol < 0x60) {
        vol = 0x60;
    }
    if (vol > 0x7F) {
        vol = 0x7F;
    }
    HuAudFXPlayVolPan(seId, vol, pan);
}

void fn_1_2260(s32 seId, HuVecF *pos)
{
    s16 pan = fn_1_2004(pos);

    HuAudFXPlayPan(seId, pan);
}

void fn_1_2318(s32 seId, HuVecF *pos)
{
    HuVecF scr;
    s16 pan;
    s16 vol;

    if (seId != -1 && HuAudFXStatusGet(seId) == 2) {
        Hu3D3Dto2D(pos, 1, &scr);
        pan = 32.0f + scr.x * 0.11111111f;
        if (pan < 0x20) {
            pan = 0x20;
        }
        if (pan > 0x60) {
            pan = 0x60;
        }
        HuAudFXPanning(seId, pan);
        vol = 96.0f + (f32)((pos->z - -700.0) / 1400.0) * 31.0f;
        if (vol < 0x60) {
            vol = 0x60;
        }
        if (vol > 0x7F) {
            vol = 0x7F;
        }
        HuAudFXVolSet(seId, vol);
    }
}

void fn_1_246C(s32 time)
{
    lbl_1_bss_24 = time;
    lbl_1_bss_20 = 0x12;
}

void fn_1_248C(void)
{
    s16 vol;

    if (lbl_1_bss_28 != -1) {
        if (lbl_1_bss_24 > 0) {
            lbl_1_bss_24--;
        }
        if (lbl_1_bss_20 > 0) {
            lbl_1_bss_20--;
        }
        if (lbl_1_bss_24 == 0) {
            HuAudFXVolSet(lbl_1_bss_28, 0x54);
            return;
        }
        if (lbl_1_bss_20 != 0) {
            vol = 0x7F - (s32)(2.388888888888889 * lbl_1_bss_20);
        } else {
            vol = (s32)(0.7166666666666667 * lbl_1_bss_24) + 0x54;
        }
        if (vol < 0x54) {
            vol = 0x54;
        }
        if (vol > 0x7F) {
            vol = 0x7F;
        }
        HuAudFXVolSet(lbl_1_bss_28, vol);
    }
}

f32 fn_1_2634(f32 from, f32 to, f32 t)
{
    f32 diff = fmod(to - from, 360.0);
    f32 res;

    if (diff < 0.0f) {
        diff += 360.0f;
    }
    if (diff > 180.0f) {
        diff -= 360.0f;
    }
    res = fmod(from + t * diff, 360.0);
    if (res < 0.0f) {
        res += 360.0f;
    }
    return res;
}

f32 fn_1_2750(f32 from, f32 to)
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
