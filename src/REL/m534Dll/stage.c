#include "REL/m534Dll.h"

#include <math.h>

static void fn_1_33EC(OMOBJ *obj);
static void fn_1_3624(OMOBJ *obj);
static void fn_1_3674(s16 model, s16 staticF);
static void fn_1_3AC8(OMOBJ *obj);
static void fn_1_3B1C(OMOBJ *obj);
static void fn_1_3CAC(HU3DMODEL *modelP, Mtx *mtx);
static void fn_1_3D58(HU3DMODEL *modelP, Mtx *mtx);
static void fn_1_3E04(OMOBJ *obj);
static void fn_1_3F04(OMOBJ *obj);
static void fn_1_3FA0(OMOBJ *obj);
static void fn_1_406C(OMOBJ *obj);
static void fn_1_426C(OMOBJ *obj);
static void fn_1_430C(OMOBJ *obj);
static void fn_1_4450(OMOBJ *obj);
static void fn_1_4A3C(OMOBJ *obj);
static void fn_1_5B64(void);
static void fn_1_5CB0(s32 count);
static void fn_1_5E20(s32 count, HuVecF *center);
static void fn_1_5F94(void);

typedef struct M534Snd_s {
    s32 se;     /* 0x00 */
    s32 handle; /* 0x04 */
} M534SND;

typedef struct M534Cloud_s {
    s32 dataNum; /* 0x00 */
    HuVecF pos;  /* 0x04 */
    f32 scale;   /* 0x10 */
    f32 speed;   /* 0x14 */
} M534CLOUD;

/* stage controller (OMOBJ data of fn_1_3360), 0x200 bytes */
typedef struct M534Stage_s {
    M534SND snd[32];    /* 0x000 */
    HuVecF cloudPos[5]; /* 0x100 */
    f32 cloudSpd[5];    /* 0x13C */
    s32 splashOn[16];   /* 0x150 */
    f32 baseY[4];       /* 0x190 */
    f32 offY[4];        /* 0x1A0 */
    s32 anmF;           /* 0x1B0 */
    s32 state;          /* 0x1B4 */
    s32 timer;          /* 0x1B8 */
    s32 handle;         /* 0x1BC */
    s32 shake;          /* 0x1C0 */
    s32 wait;           /* 0x1C4 */
    f32 shakeAmp;       /* 0x1C8 */
    f32 shakePhase;     /* 0x1CC */
    f32 swayAmp;        /* 0x1D0 */
    f32 swayPhase;      /* 0x1D4 */
    f32 vel;            /* 0x1D8 */
    f32 camY;           /* 0x1DC */
    f32 bounceAmp;      /* 0x1E0 */
    f32 bouncePhase;    /* 0x1E4 */
    f32 follow;         /* 0x1E8 */
    f32 footTarget;     /* 0x1EC */
    f32 footY;          /* 0x1F0 */
    f32 armX;           /* 0x1F4 */
    f32 footRotY;       /* 0x1F8 */
    f32 armZ;           /* 0x1FC */
} M534STAGE;

#define M534_RND16 ((f32) ((s32) (frand() & 0xFFFF) - 0x8000))

M534CLOUD lbl_1_data_190[5] = {
    { 0x5C0001, { -5000.0f, 1900.0f, -4000.0f }, 0.5f, 3.0f },
    { 0x5C0002, { -1000.0f, 1700.0f, -4800.0f }, 0.45f, 2.0f },
    { 0x5C0003, { -4000.0f, 1500.0f, -6000.0f }, 0.4f, 1.5f },
    { 0x5C0004, { -2000.0f, 1300.0f, -6500.0f }, 0.35f, 1.2f },
    { 0x5C0005, { 1000.0f, 1200.0f, -7000.0f }, 0.25f, 0.8f },
};

HuVecF lbl_1_data_208[16] = {
    { -257.24f, 17.81f, 98.81f },
    { -109.67f, 17.81f, 98.81f },
    { 109.67f, 17.81f, 98.81f },
    { 257.24f, 17.81f, 98.81f },
};

char lbl_1_data_2C8[4][8] = { "butt_ll", "butt_lr", "butt_rl", "butt_rr" };

HuVecF lbl_1_data_2E8[2] = {
    { -200.0f, 20.0f, 110.0f },
    { 200.0f, 20.0f, 110.0f },
};

char lbl_1_data_300[11] = "l_han_foot";
char lbl_1_data_30B[13] = "l_han_body";

OMOBJ *lbl_1_bss_48;

void fn_1_3360(HUPROCESS *objman)
{
    OMOBJ *obj;
    void *data;

    obj = omAddObjEx(objman, 0x14, 0x1C, 0, -1, fn_1_33EC);
    lbl_1_bss_48 = obj;
    data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M534STAGE), 0x10000000U);
    obj->data = data;
    memset(data, 0, sizeof(M534STAGE));
}

void fn_1_33E8(void)
{
}

static void fn_1_33EC(OMOBJ *obj)
{
    s16 model;

    model = Hu3DModelCreate(HuDataSelHeapReadNum(0x5C0000, 0x10000000, HEAP_MODEL));
    obj->mdlId[0] = model;
    Hu3DModelAttrSet(model, 0x40000001);
    Hu3DModelClusterAttrSet(model, 0, -0x3FFFFFFF);
    Hu3DModelShadowMapSet(model);
    Hu3DModelLayerSet(model, 0);
    model = Hu3DModelCreate(HuDataSelHeapReadNum(0x5C000B, 0x10000000, HEAP_MODEL));
    obj->mdlId[1] = model;
    Hu3DModelAttrSet(model, 0x40000001);
    Hu3DModelClusterAttrSet(model, 0, -0x3FFFFFFF);
    Hu3DModelLayerSet(model, 2);
    model = Hu3DModelCreate(HuDataSelHeapReadNum(0x5C0006, 0x10000000, HEAP_MODEL));
    obj->mdlId[2] = model;
    Hu3DModelShadowMapSet(model);
    Hu3DModelAttrSet(model, 0x40000002);
    Hu3DModelLayerSet(model, 0);
    model = Hu3DModelCreate(HuDataSelHeapReadNum(0x5C0007, 0x10000000, HEAP_MODEL));
    obj->mdlId[3] = model;
    Hu3DModelLayerSet(model, 0);
    fn_1_3674(model, 1);
    model = Hu3DModelCreate(HuDataSelHeapReadNum(0x5C0008, 0x10000000, HEAP_MODEL));
    obj->mdlId[4] = model;
    Hu3DModelAttrSet(model, 1);
    Hu3DModelAttrSet(model, 0x40000002);
    Hu3DModelAttrReset(model, 0x40000001);
    Hu3DModelLayerSet(model, 0);
    fn_1_3AC8(obj);
    model = Hu3DHookFuncCreate(fn_1_3CAC);
    obj->mdlId[0x1A] = model;
    Hu3DModelLayerSet(model, 1);
    model = Hu3DHookFuncCreate(fn_1_3D58);
    obj->mdlId[0x1B] = model;
    Hu3DModelLayerSet(model, 3);
    fn_1_4450(obj);
    fn_1_3E04(obj);
    fn_1_3FA0(obj);
    fn_1_426C(obj);
    obj->objFunc = fn_1_3624;
}

static void fn_1_3624(OMOBJ *obj)
{
    fn_1_3B1C(obj);
    fn_1_3F04(obj);
    fn_1_406C(obj);
    fn_1_4A3C(obj);
    fn_1_430C(obj);
}

static void fn_1_3674(s16 model, s16 staticF)
{
    HU3DMODEL *modelP;
    HSFDATA *hsf;
    HSFOBJECT *object;
    HU3DLLIGHTID light;
    Mtx mtx;
    HuVecF dir;
    HuVecF pos;
    HuVecF aim;
    f32 len;
    f32 hlen;
    f32 pitch;
    f32 yaw;
    s16 i;

    modelP = &Hu3DData[model];
    hsf = modelP->hsf;
    if (modelP->lLightId[0] == -1) {
        object = hsf->object;
        for (i = 0; i < hsf->objectNum; i++, object++) {
            if (object->type != 8) {
                continue;
            }
            dir.x = object->light.target.x - object->light.pos.x;
            dir.y = object->light.target.y - object->light.pos.y;
            dir.z = object->light.target.z - object->light.pos.z;
            light = Hu3DLLightCreate(model, object->light.pos.x, object->light.pos.y, object->light.pos.z, dir.x, dir.y, dir.z, object->light.r, object->light.g, object->light.b);
            Hu3DLLightStaticSet(model, light, staticF);
            switch (object->light.type) {
                case 0:
                    Hu3DLLightSpotSet(model, light, GX_SP_COS, object->light.cutoff);
                    break;
                case 1:
                    Hu3DLLightPointSet(model, light, object->light.refBrightness - object->light.refDistance, 1.0f, GX_DA_MEDIUM);
                    hlen = sqrtf(dir.x * dir.x + dir.z * dir.z);
                    pitch = atan2(dir.y, hlen);
                    yaw = atan2(-dir.x, -dir.z);
                    aim.x = 0.0f;
                    aim.y = 0.0f;
                    aim.z = 0.0f;
                    pos.x = 0.0f;
                    pos.z = 0.0f;
                    len = sqrtf(dir.z * dir.z + (dir.x * dir.x + dir.y * dir.y));
                    pos.y = len;
                    mtxRot(mtx, 57.29578f * pitch, 57.29578f * yaw, 0.0f);
                    PSMTXMultVec(mtx, &pos, &pos);
                    Hu3DLLightPosAimSetV(model, light, &pos, &aim);
                    break;
                case 2:
                    Hu3DLLightInfinitytSet(model, light);
                    break;
            }
        }
    }
}

static void fn_1_3AC8(OMOBJ *obj)
{
    M534STAGE *s = obj->data;
    M534SND *snd = s->snd;
    s32 i;

    for (i = 0; i < 32; i++, snd++) {
        snd->se = -1;
        snd->handle = -1;
    }
}

static void fn_1_3B1C(OMOBJ *obj)
{
    M534STAGE *s = obj->data;
    M534SND *snd = s->snd;
    s32 i;

    for (i = 0; i < 32; i++, snd++) {
        if (snd->handle != -1 && HuAudFXStatusGet(snd->handle) != 2) {
            snd->se = -1;
            snd->handle = -1;
        }
    }
}

void fn_1_3BA8(OMOBJ *obj, s32 se)
{
    M534STAGE *s = obj->data;
    M534SND *snd = s->snd;
    s32 i;

    for (i = 0; i < 32; i++, snd++) {
        if (snd->handle == -1) {
            snd->se = se;
            snd->handle = HuAudFXPlay(se);
            return;
        }
    }
}

void fn_1_3C1C(OMOBJ *obj, s32 se, s32 speed)
{
    M534STAGE *s = obj->data;
    M534SND *snd = s->snd;
    s32 i;

    for (i = 0; i < 32; i++, snd++) {
        if (snd->handle != -1 && snd->se == se) {
            HuAudFXFadeOut(snd->handle, speed);
        }
    }
}

static void fn_1_3CAC(HU3DMODEL *modelP, Mtx *mtx)
{
    GXColor color = { 0xC8, 0xC8, 0xFF, 0x00 };

    GXSetFog(GX_FOG_LIN, 1000.0f, 8000.0f, 10.0f, 20000.0f, color);
}

static void fn_1_3D58(HU3DMODEL *modelP, Mtx *mtx)
{
    GXColor color = { 0xFF, 0xFF, 0xFF, 0x00 };

    GXSetFog(GX_FOG_NONE, 1000.0f, 8000.0f, 10.0f, 20000.0f, color);
}

static void fn_1_3E04(OMOBJ *obj)
{
    M534STAGE *s = obj->data;
    M534CLOUD *cloud = lbl_1_data_190;
    s16 model;
    f32 scale;
    s32 i;

    for (i = 0; i < 5; i++, cloud++) {
        model = Hu3DModelCreate(HuDataSelHeapReadNum(cloud->dataNum, 0x10000000, HEAP_MODEL));
        obj->mdlId[i + 5] = model;
        s->cloudPos[i] = cloud->pos;
        s->cloudSpd[i] = cloud->speed;
        scale = cloud->scale;
        Hu3DModelPosSetV(model, &cloud->pos);
        Hu3DModelScaleSet(model, scale, scale, scale);
        Hu3DModelAttrSet(model, 0x40000001);
        Hu3DModelLayerSet(model, 0);
    }
}

static void fn_1_3F04(OMOBJ *obj)
{
    M534STAGE *s = obj->data;
    s16 model;
    s32 i;

    for (i = 0; i < 5; i++) {
        s->cloudPos[i].x += s->cloudSpd[i];
        model = obj->mdlId[i + 5];
        Hu3DModelPosSetV(model, &s->cloudPos[i]);
    }
}

static void fn_1_3FA0(OMOBJ *obj)
{
    M534STAGE *s = obj->data;
    s32 i;
    s16 model;

    for (i = 0; i < 16; i++) {
        model = Hu3DModelCreate(HuDataSelHeapReadNum(0x5C000A, 0x10000000, HEAP_MODEL));
        obj->mdlId[i + 10] = model;
        Hu3DModelAttrSet(model, 1);
        Hu3DModelAttrSet(model, 0x40000002);
        Hu3DModelAttrReset(model, 0x40000001);
        Hu3DModelLayerSet(model, 6);
        s->splashOn[i] = 0;
    }
}

static void fn_1_406C(OMOBJ *obj)
{
    M534STAGE *s = obj->data;
    s16 model;
    s32 i;

    for (i = 0; i < 16; i++) {
        if (s->splashOn[i] != 0) {
            model = obj->mdlId[i + 10];
            if (Hu3DMotionEndCheck(model) != 0) {
                Hu3DModelAttrSet(model, 1);
                s->splashOn[i] = 0;
            }
        }
    }
}

void fn_1_4114(s32 slot)
{
    OMOBJ *obj = lbl_1_bss_48;
    M534STAGE *s = obj->data;
    s16 model;
    s32 i;

#ifdef TARGET_PC
    if (slot < 0 || slot > 15) {
        slot = 0;
    }
#endif
    for (i = 0; i < 16; i++) {
        if (s->splashOn[i] == 0) {
            s->splashOn[i] = 1;
            model = obj->mdlId[i + 10];
            Hu3DModelPosSetV(model, &lbl_1_data_208[slot]);
            Hu3DModelRotSet(model, 0.0f, 360.0 * (1.52587890625e-05 * (f32) (s32) (FRAND16() & 0xFFFF)), 0.0f);
            Hu3DModelAttrReset(model, 1);
            Hu3DModelAttrReset(model, 0x40000002);
            Hu3DMotionTimeSet(model, 0.0f);
            return;
        }
    }
}

static void fn_1_426C(OMOBJ *obj)
{
    M534STAGE *s = obj->data;
    s16 model = obj->mdlId[0];
    HSFOBJECT *object;
    s32 i;

    for (i = 0; i < 4; i++) {
        object = Hu3DModelObjPtrGet(model, lbl_1_data_2C8[i]);
        if (object != NULL) {
            s->baseY[i] = object->mesh.base.pos.y;
        }
        s->offY[i] = 0.0f;
    }
}

static void fn_1_430C(OMOBJ *obj)
{
    M534STAGE *s = obj->data;
    s16 model = obj->mdlId[0];
    HSFOBJECT *object;
    f32 y;
    s32 i;

    for (i = 0; i < 4; i++) {
        y = s->offY[i] * 0.5;
        s->offY[i] = y;
        y = y + s->baseY[i];
        object = Hu3DModelObjPtrGet(model, lbl_1_data_2C8[i]);
        if (object != NULL) {
            object->mesh.base.pos.y = y;
        }
    }
}

void fn_1_43D4(s32 slot)
{
    OMOBJ *obj = lbl_1_bss_48;
    M534STAGE *s = obj->data;

#ifdef TARGET_PC
    if (slot < 0 || slot > 3) {
        slot = 0;
    }
#endif
    s->offY[slot] = -12.0f;
    if (slot < 2) {
        HuAudFXPlay(0x7BD);
        return;
    }
    HuAudFXPlay(0x7BE);
}

static void fn_1_4450(OMOBJ *obj)
{
    M534STAGE *s = obj->data;

    s->state = 0;
    s->wait = 12;
    s->follow = 0.5f;
    s->footTarget = s->footY = -710.0f;
}

void fn_1_449C(s32 team)
{
    OMOBJ *obj = lbl_1_bss_48;
    M534STAGE *s = obj->data;
    s32 lead;

    if (s->state == 4) {
        fn_1_74B0(team, 1);
        if (--s->wait > 0) {
            return;
        }
        s->wait = 60;
        if (s->footTarget < 0.0f) {
            s->footTarget = s->footTarget + 60.0;
        }
        fn_1_5CB0(8);
        s->shake = 20;
        s->shakeAmp = 3.0f;
        s->swayAmp = 0.2f;
        s->swayPhase = 0.0f;
        lead = fn_1_4624();
        if (lead != -1) {
            fn_1_3BA8(obj, 0x7BF);
        }
        if (lead != 1) {
            fn_1_3BA8(obj, 0x7C0);
        }
    }
}

s32 fn_1_4624(void)
{
    OMOBJ *obj = lbl_1_bss_48;
    M534STAGE *s = obj->data;
    s32 a = fn_1_74E8(0);
    s32 b = fn_1_74E8(1);

    if (a > b) {
        return 1;
    }
    if (a < b) {
        return -1;
    }
    return 0;
}

f32 fn_1_46A8(void)
{
    OMOBJ *obj = lbl_1_bss_48;
    M534STAGE *s = obj->data;
    f32 b;
    f32 d;

    b = fn_1_74E8(1);
    d = fn_1_74E8(0) - b;
    if (d >= 0.0) {
        if (d <= 4.0) {
            d = 0.0f;
        } else {
            d = d - 4.0;
        }
    } else {
        if (d >= -4.0) {
            d = 0.0f;
        } else {
            d = d + 4.0;
        }
    }
    d = d * 5.0;
    if (d > 20.0) {
        d = 20.0f;
    }
    if (d < -20.0) {
        d = -20.0f;
    }
    return d;
}

f32 fn_1_4848(void)
{
    OMOBJ *obj = lbl_1_bss_48;
    M534STAGE *s = obj->data;
    f32 v;

    if (fn_1_4624() != 0) {
        v = s->armX / 76.0;
    } else {
        v = s->armZ / 90.0;
    }
    if (v < 0.0f) {
        v = 0.0f;
    }
    if (v > 1.0f) {
        v = 1.0f;
    }
    return v;
}

void fn_1_4970(void)
{
    s32 i;

    for (i = 0; i < 4; i++) {
        omVibrate(i, 0x28, 0xC, 0);
    }
}

BOOL fn_1_49BC(void)
{
    OMOBJ *obj = lbl_1_bss_48;
    M534STAGE *s = obj->data;
    return s->state == 12;
}

BOOL fn_1_49FC(void)
{
    OMOBJ *obj = lbl_1_bss_48;
    M534STAGE *s = obj->data;
    return s->state == 8;
}

static void fn_1_4A3C(OMOBJ *obj)
{
    M534STAGE *s = obj->data;
    HSFOBJECT *object;
    HuVecF v;
    f32 f;
    s32 lead;
    s32 done;
    s32 vi;
    s16 model;

    switch (s->state) {
        case 0:
            if (fn_1_16AC() == 5) {
                Hu3DModelAttrReset(obj->mdlId[2], 0x40000002);
                for (vi = 0; vi < 4; vi++) {
                    omVibrate(vi, 0x28, 0xC, 0);
                }
                s->state = 1;
            }
            break;
        case 1:
            fn_1_5B64();
            if (fn_1_16AC() == 6) {
                s->state = 2;
                s->timer = 0;
                s->follow = 0.1f;
                s->footTarget = -400.0f;
                Hu3DModelAttrReset(obj->mdlId[4], 0x40000002);
                Hu3DModelAttrReset(obj->mdlId[4], 1);
                s->anmF = 1;
                HuAudFXPlay(0x7BC);
            }
            break;
        case 2:
            if ((++s->timer & 1) == 0) {
                fn_1_5B64();
            }
            if (s->footY < -500.0) {
                Hu3DModelShadowSet(obj->mdlId[3]);
                s->shakeAmp = 3.0f;
                s->swayAmp = 0.2f;
                s->swayPhase = 0.0f;
            }
            if (!(s->footY < -400.1)) {
                s->follow = 0.02f;
                s->state = 3;
            }
            break;
        case 3:
            s->shake = 5;
            if (fn_1_1694() == 2) {
                s->state = 4;
                s->timer = 0;
            }
            break;
        case 4:
            if (fn_1_1694() != 3) {
                if (fn_1_179C() == 0) {
                    s->footRotY = s->footRotY + 0.1 * (fn_1_46A8() - s->footRotY);
                }
            } else {
                s->state = 5;
                s->timer = 0;
                s->camY = Center.y;
                lead = fn_1_4624();
                if (lead != -1) {
                    fn_1_3BA8(obj, 0x7BF);
                }
                if (lead != 1) {
                    fn_1_3BA8(obj, 0x7C0);
                }
            }
            break;
        case 5:
            s->swayAmp = 0.15f;
            s->swayPhase = 0.0f;
            if ((++s->timer & 3) == 0) {
                fn_1_5CB0(3);
                s->shake = 8;
                s->shakeAmp = 5.0f;
                s->footTarget = s->footTarget + 20.0;
                if (!(s->footTarget < 0.0f)) {
                    s->footTarget = 0.0f;
                    s->state = 6;
                    s->timer = 0;
                    fn_1_3C1C(obj, 0x7BF, 500);
                    fn_1_3C1C(obj, 0x7C0, 500);
                }
            }
            break;
        case 6:
            s->swayAmp = 0.1f;
            s->state = 7;
            s->timer = 0;
            break;
        case 7:
            s->swayAmp = 0.1f;
            s->timer++;
            if (!((f32) (f64) s->timer < 42.0)) {
                s->state = 8;
                s->timer = 0;
            }
            break;
        case 8:
            s->vel = s->vel - 0.1;
            lead = fn_1_4624();
            if (lead != 0) {
                s->armX = s->armX + s->vel;
                if (!(s->armX > -40.0)) {
                    s->state = 9;
                    s->timer = 0;
                }
            } else {
                s->footRotY = s->footRotY + 0.1 * (90.0f - s->footRotY);
                s->armZ = s->armZ + s->vel;
                if (!(s->armZ > -40.0)) {
                    s->state = 9;
                    s->timer = 0;
                }
            }
            break;
        case 9:
            s->vel = s->vel + 0.2;
            if (s->vel > 0.0f) {
                s->vel = 0.0f;
            }
            lead = fn_1_4624();
            if (lead != 0) {
                s->armX = s->armX + s->vel;
            } else {
                s->footRotY = s->footRotY + 0.1 * (90.0f - s->footRotY);
                s->armZ = s->armZ + s->vel;
            }
            if (!(s->vel < 0.0f)) {
                if (++s->timer >= 15) {
                    s->state = 10;
                    s->timer = 0;
                    s->handle = -1;
                }
            }
            break;
        case 10:
            done = 0;
            lead = fn_1_4624();
            s->vel = s->vel + 0.3;
            if (lead != 0) {
                s->armX = s->armX + s->vel;
                if (s->armX >= 76.0) {
                    s->swayAmp = 0.0f;
                    s->shakeAmp = 0.0f;
                    s->armX = 76.0f;
                    s->vel = 0.5 * -s->vel;
                    if (s->vel < -1.0) {
                        s->bounceAmp = -20.0 * s->vel;
                    } else {
                        done = 1;
                    }
                }
                if (s->armX >= 61.0) {
                    if (++s->timer < 10) {
                        if (lead == -1) {
                            fn_1_5E20(2, &lbl_1_data_2E8[0]);
                        } else {
                            fn_1_5E20(2, &lbl_1_data_2E8[1]);
                        }
                    }
                }
                s->footRotY = s->footRotY + 0.05 * ((20.0f * (f32) lead) - s->footRotY);
                if (s->handle == -1) {
                    if (s->armX >= 50.0) {
                        if (lead == -1) {
                            s->handle = HuAudFXPlay(0x7C1);
                        }
                        if (lead == 1) {
                            s->handle = HuAudFXPlay(0x7C2);
                        }
                    }
                }
            } else {
                s->armZ = s->armZ + s->vel;
                if (s->armZ >= 90.0) {
                    s->swayAmp = 0.0f;
                    s->shakeAmp = 0.0f;
                    s->armZ = 90.0f;
                    s->vel = 0.3 * -s->vel;
                    if (s->vel < -1.0) {
                        s->bounceAmp = -20.0 * s->vel;
                    } else {
                        done = 1;
                    }
                }
                if (s->armZ >= 75.0) {
                    if (++s->timer < 10) {
                        fn_1_5E20(2, &lbl_1_data_2E8[0]);
                        fn_1_5E20(2, &lbl_1_data_2E8[1]);
                    }
                }
                s->footRotY = s->footRotY + 0.1 * (90.0f - s->footRotY);
                if (s->handle == -1) {
                    if (s->armZ >= 60.0) {
                        s->handle = HuAudFXPlay(0x7C1);
                        s->handle = HuAudFXPlay(0x7C2);
                    }
                }
            }
            if (done != 0) {
                s->state = 11;
                s->timer = 0;
            }
            break;
        case 11:
            if (s->bounceAmp == 0.0f) {
                s->state = 12;
                s->timer = 0;
                Center.y = s->camY;
            }
            break;
        case 12:
            break;
    }
    if (s->bounceAmp >= 1.0) {
        Center.y = s->camY + s->bounceAmp * sin(3.141592653589793 * s->bouncePhase / 180.0);
        s->bouncePhase = s->bouncePhase + 70.0f;
        s->bounceAmp = s->bounceAmp * 0.9;
    } else {
        s->bounceAmp = 0.0f;
    }
    s->footY = s->footY + s->follow * (s->footTarget - s->footY);
    s->shakeAmp = 0.98 * s->shakeAmp;
    s->shakePhase = s->shakePhase + 70.0f;
    s->swayAmp = 0.98 * s->swayAmp;
    s->swayPhase = s->swayPhase + 8.0f;
    model = obj->mdlId[3];
    object = Hu3DModelObjPtrGet(model, lbl_1_data_300);
    if (object != NULL) {
        object->mesh.base.pos.x = s->shakeAmp * sin(3.141592653589793 * s->shakePhase / 180.0);
        object->mesh.base.pos.y = s->footY;
        object->mesh.base.rot.y = s->footRotY;
    }
    object = Hu3DModelObjPtrGet(model, lbl_1_data_30B);
    if (object != NULL) {
        object->mesh.base.rot.x = s->armX + 2.0 * (s->shakeAmp * sin(3.141592653589793 * s->swayPhase / 180.0));
        object->mesh.base.rot.z = s->armZ + s->swayAmp * sin(3.141592653589793 * s->swayPhase / 180.0);
    }
    if (s->anmF != 0) {
        model = obj->mdlId[4];
        if (Hu3DMotionEndCheck(model) != 0) {
            Hu3DModelAttrSet(model, 1);
            s->anmF = 0;
        } else {
            v.x = 0.0f;
            v.y = s->footY;
            v.z = 0.0f;
            Hu3DModelPosSetV(model, &v);
        }
    }
    if (s->shake > 0) {
        f = s->shake;
        while (f > 0.0f) {
            fn_1_5F94();
            f -= 2.0f;
        }
        s->shake -= 1.0;
    }
}

static void fn_1_5B64(void)
{
    HuVecF pos;

    pos.x = 250.0 * (3.0517578125e-05 * M534_RND16);
    pos.y = 80.0 + 100.0 * (3.0517578125e-05 * M534_RND16);
    pos.z = 250.0 * (3.0517578125e-05 * M534_RND16) - 500.0;
    fn_1_6458(&pos);
}

static void fn_1_5CB0(s32 count)
{
    HuVecF pos;
    s32 i;

    for (i = 0; i < count; i++) {
        pos.x = 150.0 * (3.0517578125e-05 * M534_RND16);
        pos.y = 50.0 * (3.0517578125e-05 * M534_RND16) - 50.0;
        pos.z = 150.0 * (3.0517578125e-05 * M534_RND16) - 500.0;
        fn_1_6458(&pos);
    }
}

static void fn_1_5E20(s32 count, HuVecF *center)
{
    HuVecF pos;
    s32 i;

    for (i = 0; i < count; i++) {
        pos.x = center->x + 100.0 * (3.0517578125e-05 * M534_RND16);
        pos.y = center->y + 100.0 * (3.0517578125e-05 * M534_RND16);
        pos.z = center->z + 100.0 * (3.0517578125e-05 * M534_RND16);
        fn_1_6458(&pos);
    }
}

static void fn_1_5F94(void)
{
    M534STAGE *s = lbl_1_bss_48->data;
    HuVecF pos;
    HuVecF vel;
    f32 sn;
    f32 cs;
    f32 ang;
    f32 dist;
    f32 off;

    pos.y = s->footY + (640.0 + 10.0 * (3.0517578125e-05 * M534_RND16));
    vel.y = 0.2 * (1.52587890625e-05 * (f32) (s32) (FRAND16() & 0xFFFF)) - 1.5;
    ang = s->footRotY;
    sn = sin(3.141592653589793 * ang / 180.0);
    cs = cos(3.141592653589793 * ang / 180.0);
    if (fabs(ang) < 8.0) {
        dist = 130.0f;
    } else {
        dist = 140.0f;
    }
    off = 200.0 * (3.0517578125e-05 * M534_RND16);
    pos.x = cs * -dist + sn * off;
    pos.z = sn * -dist + cs * off - 500.0;
    vel.x = -5.0 * cs;
    vel.z = -5.0 * sn;
    fn_1_6938(&pos, &vel);
    off = 200.0 * (3.0517578125e-05 * M534_RND16);
    pos.x = cs * dist + sn * off;
    pos.z = sn * dist + cs * off - 500.0;
    vel.x = 5.0 * cs;
    vel.z = 5.0 * sn;
    fn_1_6938(&pos, &vel);
}
