#include "game/audio.h"
#include "game/flag.h"
#include "game/frand.h"
#include "game/gamemes.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/pad.h"
#include "game/printfunc.h"
#include "game/wipe.h"
#include "game/data.h"
#include "game/mg/seqman.h"
#include "game/mg/timer.h"

#include "humath.h"
#include "math.h"

#include "REL/m523Dll.h"

#ifdef __MWERKS__
#define M523_RODATA __declspec(section ".rodata")
#else
#define M523_RODATA
#endif

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

void ObjectSetup(void);
void fn_1_12C(s16 mode, s16 frameNo);
void fn_1_524(s16 mode, s16 frameNo);
void fn_1_634(s16 mode, s16 frameNo);
void fn_1_674(s16 mode, s16 frameNo);
void fn_1_6CC(s16 mode, s16 frameNo);
void fn_1_714(s16 mode, s16 frameNo);
void fn_1_76C(s16 mode, s16 frameNo);
void fn_1_8C0(s16 mode, s16 frameNo);
void fn_1_8D4(s16 mode, s16 frameNo);
void fn_1_8E8(OMOBJ *obj);
void fn_1_BB0(void);
M523Camera *fn_1_1068(M523Camera *param);
void fn_1_1154(M523Camera *cam);
void fn_1_1200(OMOBJ *obj);
void fn_1_1754(M523Camera *cam);
void fn_1_1E18(M523Camera *cam);
void fn_1_221C(OMOBJ *obj);
OMOBJ *MgCameraMoveEntry(M523Camera *start, M523Camera *end, f32 time);
void fn_1_259C(OMOBJ *obj);
void fn_1_2A34(OMOBJ *obj);
void fn_1_2AE4(OMOBJ *obj);
void fn_1_2DD8(OMOBJ *obj, void *flow);
void fn_1_2E50(OMOBJ *obj, void *flow);
void fn_1_2EB4(OMOBJ *obj, s16 mesNo, s16 mesArg);
void fn_1_2F70(OMOBJ *obj, void *flow);
void fn_1_3024(OMOBJ *obj, void *flow);
void fn_1_30D8(OMOBJ *obj, void *flow);

char lbl_1_data_0[] = "MgCameraMoveEntry : Camera Number Different\n";
char lbl_1_data_2D[] = "MgFlowSet : Value is Max Over\n";
char lbl_1_data_4C[] = "MgFlowNext : Value is Max Over\n";

M523Camera lbl_1_data_6C = { 1, 1, 20.0f, 20.0f, 8000.0f, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 190.0f, 72.0f }, -20.0f, 0.0f, 0.0f, 1000.0f };

M523Camera lbl_1_data_B4 = { 1, 1, 20.0f, 20.0f, 8000.0f, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 259.0f, 72.0f }, -42.5f, 0.0f, 0.0f, 4886.4f };

M523Camera lbl_1_data_FC = { 1, 1, 20.0f, 20.0f, 8000.0f, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 300.0f, 72.0f }, -30.0f, 0.0f, 0.0f, 2800.0f };

M523Camera lbl_1_data_144 = { 1, 1, 20.0f, 20.0f, 8000.0f, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 400.0f, 72.0f }, -8.6f, 0.0f, 0.0f, 2800.0f };

Point3d lbl_1_data_18C = { 0.0f, 100000.0f, 1.0f };
Point3d lbl_1_data_198 = { 0.0f, 1.0f, 0.0f };
Point3d lbl_1_data_1A4 = { 0.0f, 0.0f, 0.0f };
Point3d lbl_1_data_1B0 = { 0.0f, 0.0f, 0.0f };

MGSEQ_PARAM lbl_1_data_1BC = {
    30,
    2,
    fn_1_12C,
    fn_1_524,
    fn_1_634,
    fn_1_6CC,
    fn_1_714,
    fn_1_76C,
    fn_1_8C0,
    NULL,
    fn_1_8D4,
};

Point3d lbl_1_data_1E4[3] = {
    { 0.0f, 0.0f, 600.0f },
    { -485.0f, 0.0f, -342.0f },
    { 570.0f, 0.0f, -120.0f },
};




OMOBJ *lbl_1_bss_0;
OMOBJ *lbl_1_bss_4;
s32 lbl_1_bss_8;
s32 lbl_1_bss_C;
s32 lbl_1_bss_10;
s32 lbl_1_bss_14;
s32 lbl_1_bss_18;
s32 lbl_1_bss_1C;
s32 lbl_1_bss_20;
s32 lbl_1_bss_24[8];
M523Camera lbl_1_bss_44;
s16 lbl_1_bss_8C[6];
M523Camera lbl_1_bss_98;
s16 lbl_1_bss_E0;
OMOBJMAN *lbl_1_bss_E4;

M523_RODATA const u32 lbl_1_rodata_8C[5] = { 0x00510000, 0x00510001, 0x00510002, 0x00510005, 0x00510003 };



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
    lbl_1_bss_E4 = omInitObjMan(0xC8, 0x1000);
    lbl_1_bss_E0 = -1;
    omGameSysInit(lbl_1_bss_E4);
    MgSeqCreatePrio(&lbl_1_data_1BC, 0x7D0);
    MgSeqModeHookAdd(4, fn_1_674);
    MgSeqModeDelaySet(2, 0x12C);
    MgSeqModeDelaySet(7, 0x12C);
}

void fn_1_12C(s16 mode, s16 frameNo)
{
    s16 models[3];
    M523Light light = { { -800.0f, 1500.0f, 800.0f }, { 800.0f, -1500.0f, -800.0f }, { 0xFF, 0xFF, 0xFF, 0xFF } };
    s32 i;
    HU3DMODELID model;
    HU3DLIGHTID lightId;

    for (i = 0; i < 5; i++) {
        lbl_1_bss_8C[i] = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_rodata_8C[i], HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelAttrSet(lbl_1_bss_8C[i], 0x40000001);
        Hu3DModelPosSet(lbl_1_bss_8C[i], 0.0f, 0.0f, 0.0f);
        Hu3DModelRotSet(lbl_1_bss_8C[i], 0.0f, 0.0f, 0.0f);
        Hu3DModelLayerSet(lbl_1_bss_8C[i], 0);
    }
    Hu3DModelLayerSet(lbl_1_bss_8C[1], 3);
    Hu3DMotionSpeedSet(lbl_1_bss_8C[3], 0.4f);
    model = Hu3DModelCreate(HuDataSelHeapReadNum(M523_HSF_fuse, HU_MEMNUM_OVL, HEAP_MODEL));
    for (i = 0; i < 3; i++) {
        if (i == 0) {
            models[i] = model;
        } else {
            models[i] = Hu3DModelLink(model);
        }
        Hu3DModelPosSetV(models[i], &lbl_1_data_1E4[i]);
        Hu3DModelLayerSet(models[i], 2);
    }
    fn_1_1068(&lbl_1_data_6C);
    lbl_1_bss_44 = lbl_1_data_6C;
    lightId = Hu3DGLightCreateV(&light.pos, &light.target, &light.color);
    Hu3DGLightStaticSet(lightId, 1);
    Hu3DGLightInfinitytSet(lightId);
    Hu3DShadowCreate(1.0f, lbl_1_data_B4.fov, 100000.0f);
    Hu3DShadowTPLvlSet(0.35f);
    Hu3DShadowPosSet(&lbl_1_data_18C, &lbl_1_data_198, &lbl_1_data_1A4);
    Hu3DModelShadowMapSet(lbl_1_bss_8C[0]);
    Hu3DModelShadowMapSet(lbl_1_bss_8C[1]);
    for (i = 0; i < 3; i++) {
        Hu3DModelShadowMapSet(models[i]);
    }
    fn_1_39DC();
    lbl_1_bss_8 = -1;
    lbl_1_bss_24[0] = 10;
    lbl_1_bss_20 = 0;
    Hu3DModelAttrSet(lbl_1_bss_8C[0], 0x40000002);
    Hu3DModelAttrReset(lbl_1_bss_8C[0], 0x40000001);
    omAddObjEx(lbl_1_bss_E4, 0x1300, 0, 0, -1, fn_1_8E8);
    lbl_1_bss_C = HuAudFXPlay(0x751);
    lbl_1_bss_18 = 0;
    lbl_1_bss_14 = 0;
    MgSeqModeNext();
}

void fn_1_524(s16 mode, s16 frameNo)
{
    M523CamMove *move;

    if (frameNo == 30) {
        HuAudFXPlay(0x74A);
    }
    switch (lbl_1_bss_14) {
        case 3:
            lbl_1_bss_4 = MgCameraMoveEntry(&lbl_1_bss_44, &lbl_1_data_B4, 30.0f);
            lbl_1_bss_14 = 4;
            break;
        case 4:
            move = (M523CamMove *)lbl_1_bss_4->data;
            if ((move->moveF & 2) == 0) {
                omDelObjEx(lbl_1_bss_E4, lbl_1_bss_4);
                MgSeqModeNext();
            }
            break;
    }
}

void fn_1_634(s16 mode, s16 frameNo)
{
    if (frameNo == 0) {
        lbl_1_bss_10 = HuAudSStreamPlay(0x34);
    }
}

void fn_1_674(s16 mode, s16 frameNo)
{
    lbl_1_bss_1C = 0;
    fn_1_405C();
    omAddObjEx(lbl_1_bss_E4, 0x1100, 0, 0, -1, fn_1_7C84);
}

void fn_1_6CC(s16 mode, s16 frameNo)
{
    if (lbl_1_bss_13C->state == 2) {
        lbl_1_bss_1C = 1;
        MgSeqModeNext();
    }
}

void fn_1_714(s16 mode, s16 frameNo)
{
    lbl_1_bss_18 = 2;
    if (frameNo == 0) {
        HuAudSStreamFadeOut(lbl_1_bss_10, 100);
        fn_1_BB0();
        fn_1_4858();
    }
}

void fn_1_76C(s16 mode, s16 frameNo)
{
    s32 i;
    s32 allDone;
    M523CamMove *move;

    allDone = 1;
    for (i = 0; i < 4; i++) {
        if (lbl_1_bss_140[i]->doneF == 0) {
            allDone = 0;
        }
    }
    if (lbl_1_bss_0 == NULL) {
        if (allDone != 0) {
            if (lbl_1_bss_1C == 0) {
                lbl_1_bss_0 = MgCameraMoveEntry(&lbl_1_bss_44, &lbl_1_data_FC, 30.0f);
            } else {
                lbl_1_bss_0 = MgCameraMoveEntry(&lbl_1_bss_44, &lbl_1_data_144, 30.0f);
            }
        }
    } else {
        move = (M523CamMove *)lbl_1_bss_0->data;
        if ((move->moveF & 2) == 0) {
            omDelObjEx(lbl_1_bss_E4, lbl_1_bss_0);
            MgSeqModeNext();
        }
    }
}

void fn_1_8C0(s16 mode, s16 frameNo)
{
    lbl_1_bss_18 = 4;
}

void fn_1_8D4(s16 mode, s16 frameNo)
{
    lbl_1_bss_18 = 5;
}

void fn_1_8E8(OMOBJ *obj)
{
    s32 i;
    s32 moving;
    s32 vol;
    f32 ratio;

    if (lbl_1_bss_18 != 5) {
        ratio = (lbl_1_bss_44.dist - lbl_1_data_6C.dist) / (lbl_1_data_B4.dist - lbl_1_data_6C.dist);
        vol = 127.0f - (63.0f * ratio);
        HuAudFXVolSet(lbl_1_bss_C, vol);
        moving = 0;
        for (i = 0; i < 4; i++) {
            if (lbl_1_bss_140[i]->grp != 0 && lbl_1_bss_140[i]->state != 6 && PSVECMag(&lbl_1_bss_140[i]->vel) > 0.1f) {
                moving = 1;
            }
        }
        if (moving != 0) {
            if (lbl_1_bss_8 == -1) {
                lbl_1_bss_8 = HuAudFXPlay(0x74B);
            }
        } else if (lbl_1_bss_8 != -1) {
            HuAudFXStop(lbl_1_bss_8);
            lbl_1_bss_8 = -1;
        }
        if (lbl_1_bss_20 != 0) {
            if (Hu3DMotionEndCheck(lbl_1_bss_8C[0])) {
                Hu3DModelAttrSet(lbl_1_bss_8C[0], 0x40000002);
                lbl_1_bss_24[0] = frandmod(0x78) + 0x78;
                lbl_1_bss_20 = 0;
            }
        } else if (lbl_1_bss_24[0] > 0) {
            lbl_1_bss_24[0]--;
            if (lbl_1_bss_24[0] == 0) {
                HuAudFXPlay(0x752);
                Hu3DModelAttrReset(lbl_1_bss_8C[0], 0x40000002);
                Hu3DMotionTimeSet(lbl_1_bss_8C[0], 0.0f);
                lbl_1_bss_20 = 1;
            }
        }
    }
}

void fn_1_BB0(void)
{
    s16 winner[4];
    s32 i;

    for (i = 0; i < 4; i++) {
        winner[i] = -1;
        if (lbl_1_bss_1C == lbl_1_bss_140[i]->grp) {
            winner[i] = lbl_1_bss_140[i]->charNo;
            GWMgCoinBonusSet(i, 10);
        }
    }
    MgSeqWinnerSet(winner[0], winner[1], winner[2], winner[3]);
}

f32 fn_1_C88(Point3d *from, Point3d *to)
{
    Point3d diff;

    PSVECSubtract(to, from, &diff);
    return (f32)(180.0 * (atan2(diff.x, diff.z) / 3.141592653589793));
}

f32 fn_1_CEC(f32 ang0, f32 ang1)
{
    f32 diff;

    diff = ang1 - ang0;
    if (diff < -180.0f) {
        diff += 360.0f;
    }
    if (diff > 180.0f) {
        diff -= 360.0f;
    }
    return diff;
}

f32 fn_1_D58(Point3d *pt, Point3d *a, Point3d *b, Point3d *out)
{
    Point3d ab;
    Point3d ap;
    Point3d proj;
    Point3d perp;
    f32 len2;
    f32 t;

    PSVECSubtract(b, a, &ab);
    PSVECSubtract(pt, a, &ap);
    len2 = PSVECSquareMag(&ab);
    t = PSVECDotProduct(&ab, &ap) / len2;
    if (t < 0.0f || t > 1.0f) {
        return -1.0f;
    }
    PSVECScale(&ab, &proj, t);
    PSVECAdd(a, &proj, out);
    PSVECSubtract(&proj, &ap, &perp);
    return PSVECMag(&perp);
}

void fn_1_E58(Point3d *pos, s16 *outY, s16 *outX)
{
    Point3d scr;

    Hu3D3Dto2D(pos, lbl_1_bss_44.camNo, &scr);
    *outX = 48.0f + (32.0f * (scr.x / 576.0f));
    *outY = 96.0f + (31.0f * (scr.y / 480.0f));
}

void fn_1_F34(Point3d *pos, s16 seId)
{
    Point3d scr;
    s32 pan;
    s32 vol;

    Hu3D3Dto2D(pos, lbl_1_bss_44.camNo, &scr);
    pan = 48.0f + (32.0f * (scr.x / 576.0f));
    vol = 96.0f + (31.0f * (scr.y / 480.0f));
    HuAudFXPlayVolPan(seId, vol, pan);
}

void fn_1_1014(void) {
    lbl_1_bss_E4 = omInitObjMan(0xC8, 0x1000);
    lbl_1_bss_E0 = -1;
    omGameSysInit(lbl_1_bss_E4);
}

M523Camera *fn_1_1068(M523Camera *param) {
    M523Camera *cam;

    cam = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M523Camera), HU_MEMNUM_OVL);
    *cam = *param;
    Hu3DCameraCreate(param->camNo);
    Hu3DCameraViewportSet(param->camNo, 0.0f, 0.0f, 640.0f, 480.0f, 0.0f, 1.0f);
    Hu3DCameraPerspectiveSet(param->camNo, param->nearZ, param->fov, param->farZ, 1.2f);
    fn_1_1154(param);
    return cam;
}

void fn_1_1154(M523Camera *cam)
{
    if (cam->mode == 1) {
        fn_1_1754(cam);
    }
    Hu3DCameraPosSetV(cam->camNo, &cam->pos, &cam->up, &cam->target);
}

s16 fn_1_11A4(M523Light *light)
{
    HU3DLIGHTID lightId;

    lightId = Hu3DGLightCreateV(&light->pos, &light->target, &light->color);
    Hu3DGLightStaticSet(lightId, 1);
    Hu3DGLightInfinitytSet(lightId);
    return lightId;
}

void fn_1_1200(OMOBJ *obj)
{
    M523Camera *cam = (M523Camera *)obj->work[0];

    if ((*HuPadBtn & 0x800) != 0) {
        if ((*HuPadBtnDown & 0x1000) != 0) {
            *cam = lbl_1_bss_98;
        }
        if ((*HuPadBtnDown & 0x200) != 0) {
            obj->work[1] += 1;
            if (obj->work[1] == 2) {
                obj->work[1] = 0;
            }
        }
        switch (obj->work[1]) {
            case 0:
                cam->pos.z = cam->pos.z - (0.1f * (f32)*HuPadTrigL);
                cam->pos.z = cam->pos.z + (0.1f * (f32)*HuPadTrigR);
                cam->pos.x = cam->pos.x + (0.1f * (f32)*HuPadStkX);
                cam->pos.y = cam->pos.y + (0.1f * (f32)*HuPadStkY);
                fn_1_1E18(cam);
                break;
            case 1:
                cam->angX = cam->angX - (0.1f * (f32)*HuPadStkY);
                cam->angY = cam->angY + (0.1f * (f32)*HuPadStkX);
                cam->dist = cam->dist - (0.1f * (f32)*HuPadTrigL);
                cam->dist = cam->dist + (0.1f * (f32)*HuPadTrigR);
                fn_1_1754(cam);
                break;
        }
        if ((*HuPadBtn & 0x400) != 0) {
            cam->target.z = cam->target.z - (0.1f * (f32)*HuPadSubStkY);
        } else {
            cam->target.x = cam->target.x + (0.1f * (f32)*HuPadSubStkX);
            cam->target.y = cam->target.y + (0.1f * (f32)*HuPadSubStkY);
        }
        Hu3DCameraPosSetV(cam->camNo, &cam->pos, &cam->up, &cam->target);
    }
}

void fn_1_1678(M523Camera *cam)
{
    OMOBJ *obj;

    obj = omAddObjEx(lbl_1_bss_E4, 0x4000, 0, 0, -1, fn_1_1200);
    obj->work[0] = (uintptr_t)cam;
    if (cam->mode == 0) {
        obj->work[1] = 0;
    } else {
        obj->work[1] = 1;
    }
    lbl_1_bss_98 = *cam;
}

void fn_1_1724(M523Camera *cam)
{
    lbl_1_bss_98 = *cam;
}

void fn_1_1754(M523Camera *cam)
{
    Vec sp8;
    f32 temp_f26;
    f32 temp_f27;
    f32 temp_f28;
    f32 temp_f29;
    f32 temp_f30;
    f32 temp_f31;
    f64 temp_f25;
    f64 temp_f25_2;
    f64 temp_f25_3;
    f64 temp_f25_4;
    f64 temp_f24;
    f64 temp_f24_2;
    f64 temp_f24_3;
    f64 temp_f24_4;
    f64 temp_f24_5;
    f64 temp_f24_6;
    f64 temp_f24_7;
    f64 temp_f24_8;
    f64 temp_f24_9;
    f64 temp_f24_10;
    f64 temp_f24_11;
    f64 temp_f24_12;

    temp_f30 = cam->angX;
    temp_f29 = cam->angY;
    temp_f31 = cam->angZ;
    temp_f24 = cos((3.141592653589793 * (f64) temp_f30) / 180.0);
    cam->pos.x = (f32) ((f64) cam->target.x + ((f64) cam->dist * (sin((3.141592653589793 * (f64) temp_f29) / 180.0) * temp_f24)));
    cam->pos.y = (f32) ((f64) cam->target.y + ((f64) cam->dist * -sin((3.141592653589793 * (f64) temp_f30) / 180.0)));
    temp_f24_2 = cos((3.141592653589793 * (f64) temp_f30) / 180.0);
    cam->pos.z = (f32) ((f64) cam->target.z + ((f64) cam->dist * (cos((3.141592653589793 * (f64) temp_f29) / 180.0) * temp_f24_2)));
    PSVECSubtract(&cam->pos, &cam->target, &sp8);
    PSVECNormalize(&sp8, &sp8);
    temp_f24_3 = sin((3.141592653589793 * (f64) temp_f30) / 180.0);
    temp_f28 = (f32) (sin((3.141592653589793 * (f64) temp_f29) / 180.0) * temp_f24_3);
    temp_f27 = (f32) cos((3.141592653589793 * (f64) temp_f30) / 180.0);
    temp_f24_4 = sin((3.141592653589793 * (f64) temp_f30) / 180.0);
    temp_f26 = (f32) (cos((3.141592653589793 * (f64) temp_f29) / 180.0) * temp_f24_4);
    temp_f24_5 = (f64) sp8.y * sin((3.141592653589793 * (f64) temp_f31) / 180.0);
    temp_f25 = (f64) temp_f26 * (((f64) (sp8.x * sp8.z) * (1.0 - cos((3.141592653589793 * (f64) temp_f31) / 180.0))) + temp_f24_5);
    temp_f24_6 = (f64) sp8.z * sin((3.141592653589793 * (f64) temp_f31) / 180.0);
    temp_f24_7 = (f64) temp_f27 * (((f64) (sp8.x * sp8.y) * (1.0 - cos((3.141592653589793 * (f64) temp_f31) / 180.0))) - temp_f24_6);
    cam->up.x = (f32) (((f64) temp_f28 * ((f64) (sp8.x * sp8.x) + ((f64) (1.0f - (sp8.x * sp8.x)) * cos((3.141592653589793 * (f64) temp_f31) / 180.0)))) + temp_f24_7 + temp_f25);
    temp_f24_8 = (f64) sp8.x * sin((3.141592653589793 * (f64) temp_f31) / 180.0);
    temp_f25_2 = (f64) temp_f26 * (((f64) (sp8.y * sp8.z) * (1.0 - cos((3.141592653589793 * (f64) temp_f31) / 180.0))) - temp_f24_8);
    temp_f24_9 = (f64) sp8.z * sin((3.141592653589793 * (f64) temp_f31) / 180.0);
    temp_f24_10 = (f64) temp_f28 * (((f64) (sp8.x * sp8.y) * (1.0 - cos((3.141592653589793 * (f64) temp_f31) / 180.0))) + temp_f24_9);
    temp_f24_11 = (f64) sp8.x * sin((3.141592653589793 * (f64) temp_f31) / 180.0);
    temp_f24_12 = (f64) temp_f27 * (((f64) (sp8.y * sp8.z) * (1.0 - cos((3.141592653589793 * (f64) temp_f31) / 180.0))) + temp_f24_11);
    cam->up.y = (f32) (((f64) temp_f27 * ((f64) (sp8.y * sp8.y) + ((f64) (1.0f - (sp8.y * sp8.y)) * cos((3.141592653589793 * (f64) temp_f31) / 180.0)))) + temp_f24_10 + temp_f25_2);
    temp_f25_3 = (f64) sp8.y * sin((3.141592653589793 * (f64) temp_f31) / 180.0);
    temp_f25_4 = ((f64) temp_f28 * (((f64) (sp8.x * sp8.z) * (1.0 - cos((3.141592653589793 * (f64) temp_f31) / 180.0))) - temp_f25_3)) + temp_f24_12;
    cam->up.z = (f32) (((f64) temp_f26 * ((f64) (sp8.z * sp8.z) + ((f64) (1.0f - (sp8.z * sp8.z)) * cos((3.141592653589793 * (f64) temp_f31) / 180.0)))) + temp_f25_4);
    PSVECNormalize(&cam->up, &cam->up);
}

void fn_1_1E18(M523Camera *cam)
{
    f32 mtx[4][4];
    f32 dx = cam->pos.x - cam->target.x;
    f32 dy = cam->pos.y - cam->target.y;
    f32 dz = cam->pos.z - cam->target.z;
    f32 len2 = (dz * dz) + ((dx * dx) + (dy * dy));
    f32 dist;
    f32 angY;
    f32 a;
    f32 c;
    f32 s;
    f32 ry;
    f32 rx;
    f32 rz;
    int i;
    int j;

    if (len2 > 0.0f) {
        double r = __frsqrte(len2);

        r = 0.5 * r * (3.0 - (len2 * r * r));
        r = 0.5 * r * (3.0 - (len2 * r * r));
        r = 0.5 * r * (3.0 - (len2 * r * r));
        r = 0.5 * r * (3.0 - (len2 * r * r));
        dist = (f32) (len2 * r);
    } else {
        dist = len2;
    }
    cam->dist = dist;
    angY = (f32) (180.0 * (atan2(dz, dx) / M_PI));
    cam->angY = -(angY - 90.0f);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            mtx[i][j] = 0.0f;
        }
    }
    a = (f32) ((M_PI * -angY) / 180.0);
    c = (f32) cos(a);
    s = (f32) sin(a);
    mtx[0][0] = c;
    mtx[0][2] = s;
    mtx[1][1] = 1.0f;
    mtx[2][0] = -s;
    mtx[2][2] = c;
    mtx[3][3] = 1.0f;
    ry = (dz * mtx[2][0]) + ((dx * mtx[0][0]) + (dy * mtx[0][1]));
    rx = (dz * mtx[2][2]) + ((dx * mtx[1][2]) + (dy * mtx[1][1]));
    rz = (dz * mtx[2][3]) + ((dx * mtx[1][3]) + (dy * mtx[1][2]));
    cam->angX = -(f32) (180.0 * (atan2(rx, ry) / M_PI));
    cam->angZ = 0.0f;
}

void fn_1_221C(OMOBJ *obj)
{
    M523CamMove *move = (M523CamMove *)obj->data;
    M523Camera *start = &move->start;
    M523Camera *end = &move->end;
    M523Camera *cam = move->cam;
    f32 ratio;

    if ((move->moveF & 2) == 0) {
        return;
    }
    move->time += 1.0f;
    ratio = move->time / move->totalTime;
    if (cam->mode == 0) {
        cam->pos.x = start->pos.x + (ratio * (end->pos.x - start->pos.x));
        cam->pos.y = start->pos.y + (ratio * (end->pos.y - start->pos.y));
        cam->pos.z = start->pos.z + (ratio * (end->pos.z - start->pos.z));
    } else {
        cam->dist = start->dist + (ratio * (end->dist - start->dist));
        cam->angX = start->angX + (ratio * (end->angX - start->angX));
        cam->angY = start->angY + (ratio * (end->angY - start->angY));
    }
    cam->target.x = start->target.x + (ratio * (end->target.x - start->target.x));
    cam->target.y = start->target.y + (ratio * (end->target.y - start->target.y));
    cam->target.z = start->target.z + (ratio * (end->target.z - start->target.z));
    if (move->time >= move->totalTime) {
        *cam = *end;
        move->moveF = move->moveF & ~2;
    }
    fn_1_1154(cam);
}

OMOBJ *MgCameraMoveEntry(M523Camera *start, M523Camera *end, f32 time)
{
    OMOBJ *obj;
    M523CamMove *move;

    if (start->camNo != end->camNo) {
        OSReport(lbl_1_data_0);
        return;
    }
    obj = omAddObjEx(lbl_1_bss_E4, 0x2000, 0, 0, -1, fn_1_221C);
    move = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M523CamMove), HU_MEMNUM_OVL);
    obj->data = move;
    move->totalTime = time;
    move->time = 0.0f;
    move->cam = start;
    move->start = *start;
    move->end = *end;
    move->moveF = 3;
    if (start->mode == 1) {
        fn_1_1754(&move->start);
        fn_1_1754(&move->end);
    }
    return obj;
}

s32 fn_1_2534(OMOBJ *obj)
{
    M523CamMove *move = (M523CamMove *)obj->data;

    if (move->moveF & 2) {
        return 1;
    }
    return 0;
}

void fn_1_2568(OMOBJ *obj)
{
    omDelObjEx(lbl_1_bss_E4, obj);
}

void fn_1_259C(OMOBJ *obj)
{
    M523Camera *cam = (M523Camera *)obj->data;
    M523Camera *src = (M523Camera *)obj->work[2];
    s32 range = obj->work[1];

    if (obj->work[0] == 0) {
        fn_1_1154(src);
        omDelObjEx(lbl_1_bss_E4, obj);
        return;
    }
    cam->target.x = src->target.x + (f32) frandmod(range * 2) - (f32) range;
    cam->target.y = src->target.y + (f32) frandmod(range * 2) - (f32) range;
    fn_1_1154(cam);
    obj->work[0] -= 1;
}

void fn_1_26E4(M523Camera *cam, s32 count, s32 range)
{
    OMOBJ *obj;

    obj = omAddObjEx(lbl_1_bss_E4, 0x2000, 0, 0, -1, fn_1_259C);
    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M523Camera), HU_MEMNUM_OVL);
    *(M523Camera *)obj->data = *cam;
    obj->work[0] = count;
    obj->work[1] = range;
    obj->work[2] = (uintptr_t)cam;
}

f32 fn_1_27A0(s16 padNo)
{
    s32 sum;
    f32 mag;

    sum = (s32)HuPadStkX[padNo] * (s32)HuPadStkX[padNo] + (s32)HuPadStkY[padNo] * (s32)HuPadStkY[padNo];
    mag = (f32) sum;
    if (mag > 0.0f) {
        mag = sqrtf(mag);
    }
    return mag;
}

f32 fn_1_2924(s16 padNo)
{
    f32 angle;

    angle = (f32) (180.0 * (atan2((f64) (s8) HuPadStkY[padNo], (f64) (s8) HuPadStkX[padNo]) / 3.141592653589793) - 90.0);
    if (angle < 0.0f) {
        angle += 360.0f;
    }
    return angle;
}

void fn_1_2A34(OMOBJ *obj)
{
    M523Flow *flow = (M523Flow *)obj->data;

    if (omSysExitReq != 0) {
        if (lbl_1_bss_E0 != -1) {
            HuAudFadeOut(0x3E8);
            lbl_1_bss_E0 = -1;
        }
        obj->objFunc = (OMOBJFUNC)fn_1_2AE4;
        WipeCreate(2, 6, 0x3C);
    } else {
        flow->funcTbl[flow->index](obj, flow->funcTbl);
    }
}

void fn_1_2AE4(OMOBJ *obj)
{
    if (!WipeCheckEnd()) {
        omOvlReturnEx(1, 1);
    }
}

OMOBJ *fn_1_2B1C(M523FlowFunc *funcTbl)
{
    OMOBJ *obj;
    M523Flow *flow;
    s32 i;
    s32 j;

    i = 0;
    while (funcTbl[i] != NULL) {
        i++;
    }
    obj = omAddObjEx(lbl_1_bss_E4, 0x2000, 0, 0, -1, fn_1_2A34);
    flow = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M523Flow), HU_MEMNUM_OVL);
    obj->data = flow;
    flow->funcNum = i;
    flow->funcTbl = funcTbl;
    flow->index = 0;
    flow->gameMesId = -1;
    for (j = 0; j < 0x10; j++) {
        flow->work[j] = 0;
    }
    return obj;
}

void MgFlowSet(OMOBJ *obj, s32 index)
{
    M523Flow *flow = (M523Flow *)obj->data;

    if (index >= flow->funcNum) {
        OSReport(lbl_1_data_2D);
    } else {
        flow->index = index;
    }
}

void MgFlowNext(OMOBJ *obj)
{
    M523Flow *flow = (M523Flow *)obj->data;

    if (flow->index == flow->funcNum - 1) {
        OSReport(lbl_1_data_4C);
    } else {
        flow->index++;
    }
}

s32 fn_1_2CBC(OMOBJ *obj)
{
    M523Flow *flow = (M523Flow *)obj->data;

    return flow->index;
}

s32 fn_1_2CD8(OMOBJ *obj, s32 index)
{
    M523Flow *flow = (M523Flow *)obj->data;

    if (flow->index == index) {
        return 1;
    }
    return 0;
}

void fn_1_2D24(OMOBJ *obj, s32 type, s32 maxTime, s32 endTime, s32 recordTime)
{
    M523Flow *flow = (M523Flow *)obj->data;

    flow->timerType = type;
    flow->maxTime = maxTime * 60;
    flow->endTime = endTime * 60;
    flow->recordTime = recordTime * 60;
}

BOOL fn_1_2D58(OMOBJ *obj)
{
    M523Flow *flow = (M523Flow *)obj->data;

    return MgTimerDoneCheck(flow->timer);
}

void fn_1_2D94(OMOBJ *obj)
{
    M523Flow *flow = (M523Flow *)obj->data;

    if (flow->timer->stopF == 0) {
        flow->timer->stopF = 1;
        flow->timer->mode = 1;
    }
}

void fn_1_2DD4(void)
{
}

void fn_1_2DD8(OMOBJ *obj, void *flow)
{
    if (!WipeCheckEnd()) {
        M523Flow *flowP = (M523Flow *)obj->data;

        if (flowP->index == flowP->funcNum - 1) {
            OSReport(lbl_1_data_4C);
        } else {
            flowP->index++;
        }
    }
}

void fn_1_2E50(OMOBJ *obj, void *flow)
{
    if (lbl_1_bss_E0 != -1) {
        HuAudFadeOut(0x3e8);
        lbl_1_bss_E0 = -1;
    }
    if (!WipeCheckEnd()) {
        omOvlReturnEx(1, 1);
    }
}

void fn_1_2EB4(OMOBJ *obj, s16 mesNo, s16 mesArg)
{
    M523Flow *flowP = (M523Flow *)obj->data;

    if (flowP->gameMesId == -1) {
        flowP->gameMesId = GameMesCreate(mesNo, mesArg);
    } else if (!GameMesStatGet(flowP->gameMesId)) {
        flowP->gameMesId = -1;
        if (flowP->index == flowP->funcNum - 1) {
            OSReport(lbl_1_data_4C);
        } else {
            flowP->index++;
        }
    }
}

void fn_1_2F70(OMOBJ *obj, void *flow)
{
    M523Flow *flowP = (M523Flow *)obj->data;

    if (flowP->gameMesId == -1) {
        flowP->gameMesId = GameMesCreate(2, 0);
    } else if (!GameMesStatGet(flowP->gameMesId)) {
        flowP->gameMesId = -1;
        if (flowP->index == flowP->funcNum - 1) {
            OSReport(lbl_1_data_4C);
        } else {
            flowP->index++;
        }
    }
}

void fn_1_3024(OMOBJ *obj, void *flow)
{
    M523Flow *flowP = (M523Flow *)obj->data;

    if (flowP->gameMesId == -1) {
        flowP->gameMesId = GameMesCreate(2, 1);
    } else if (!GameMesStatGet(flowP->gameMesId)) {
        flowP->gameMesId = -1;
        if (flowP->index == flowP->funcNum - 1) {
            OSReport(lbl_1_data_4C);
        } else {
            flowP->index++;
        }
    }
}

void fn_1_30D8(OMOBJ *obj, void *flow)
{
    M523Flow *flowP = (M523Flow *)obj->data;

    flowP->timer = MgTimerCreate(flowP->timerType);
    MgTimerParamSet(flowP->timer, flowP->maxTime, flowP->endTime, flowP->recordTime);
    MgTimerModeOnSet(flowP->timer, 1);
    if (flowP->index == flowP->funcNum - 1) {
        OSReport(lbl_1_data_4C);
    } else {
        flowP->index++;
    }
}

void fn_1_3178(M523Player *player, s16 index)
{
    player->index = index;
    player->charNo = GwPlayerConf[index].charNo;
    player->padNo = GwPlayerConf[index].padNo;
    player->grp = GwPlayerConf[index].grp;
    player->type = GwPlayerConf[index].type;
    player->dif = GwPlayerConf[index].dif;
    player->state = 0;
    player->pos.x = 0.0f;
    player->pos.y = 0.0f;
    player->pos.z = 0.0f;
    player->rot.x = 0.0f;
    player->rot.y = 0.0f;
    player->rot.z = 0.0f;
}

s32 fn_1_3244(Vec *pos, Vec *target, Vec *angle, f32 dist)
{
    Vec vec;
    f32 mag;

    PSVECSubtract(target, pos, &vec);
    mag = PSVECMag(&vec);
    if (mag == 0.0f) {
        return 1;
    }
    PSVECNormalize(&vec, &vec);
    PSVECScale(&vec, &vec, dist);
    if (PSVECMag(&vec) >= mag) {
        *pos = *target;
        return 1;
    }
    PSVECAdd(pos, &vec, pos);
    angle->y = (f32) (180.0 * (atan2((f64) vec.x, (f64) vec.z) / 3.141592653589793));
    return 0;
}

f32 fn_1_3368(f32 ang0, f32 ang1, f32 max)
{
    f32 result;
    f32 diff;

    diff = (f32) fmod((f64) (ang1 - ang0), 360.0);
    if (diff < 0.0f) {
        diff += 360.0f;
    }
    if (diff > 180.0f) {
        diff -= 360.0f;
    }
    if (fabs(diff) > max) {
        if (diff < 0.0f) {
            diff = -max;
        } else {
            diff = max;
        }
    }
    result = (f32) fmod((f64) (ang0 + diff), 360.0);
    if (result < 0.0f) {
        result += 360.0f;
    }
    return result;
}

s32 fn_1_34D8(Vec *pos, Vec *target, Vec *cur, f32 dist)
{
    Vec sp20;
    Vec sp14;
    Vec sp8;
    f32 temp_f1;
    f32 temp_f30;
    f32 temp_f31;

    sp8 = *target;
    while (TRUE) {
        PSVECSubtract(target, pos, &sp20);
        if (PSVECMag(&sp20) == 0.0f) {
            return 0;
        }
        PSVECSubtract(target, cur, &sp14);
        temp_f1 = PSVECMag(&sp14);
        if (temp_f1 >= dist) {
            return 0;
        }
        if (temp_f1 == 0.0f) {
            *target = *pos;
        } else {
            PSVECNormalize(&sp14, &sp14);
            PSVECScale(&sp14, &sp14, dist);
            PSVECAdd(cur, &sp14, target);
        }
        PSVECSubtract(target, pos, &sp14);
        if (PSVECMag(&sp14) >= 0.001f) {
            return 1;
        }
        temp_f31 = PSVECMag(&sp20);
        temp_f30 = (f32) (180.0 * (atan2((f64) sp14.z, (f64) sp14.x) / 3.141592653589793));
        temp_f30 += 30.0f;
        sp20.x = (f32) ((f64) temp_f31 * sin((3.141592653589793 * (f64) temp_f30) / 180.0));
        sp20.z = (f32) ((f64) temp_f31 * cos((3.141592653589793 * (f64) temp_f30) / 180.0));
        PSVECAdd(pos, &sp20, target);
    }
}

void fn_1_3710(Vec *pos, Vec *target, Vec *cur, Vec *out)
{
    Vec sp1C;
    Vec sp10;

    PSVECSubtract(cur, target, &sp1C);
    PSVECSubtract(target, pos, &sp10);
    PSVECCrossProduct(&sp1C, &sp10, out);
    PSVECNormalize(out, out);
}

f32 fn_1_3788(Vec *a, Vec *b, Vec *c)
{
    Vec vec;

    PSVECSubtract(c, b, &vec);
    return PSVECDotProduct(a, &vec);
}

s32 fn_1_37CC(Vec *dir, Vec *pos, Vec *target, Vec *out, Vec *result)
{
    Vec sp28;
    Vec sp1C;
    Vec sp10;
    f32 temp_f1;
    f32 temp_f30;

    PSVECSubtract(out, target, &sp28);
    PSVECSubtract(target, pos, &sp1C);
    temp_f30 = PSVECDotProduct(dir, &sp1C);
    PSVECSubtract(target, out, &sp10);
    temp_f1 = PSVECDotProduct(dir, &sp10);
    if (temp_f1 == 0.0f) {
        return 0;
    }
    PSVECScale(&sp28, &sp28, temp_f30 / temp_f1);
    PSVECAdd(target, &sp28, result);
    return 1;
}
