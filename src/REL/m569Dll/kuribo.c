#include "REL/m569Dll.h"

#include "game/audio.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/frand.h"
#include "game/memory.h"

#include "math.h"

#define M569_RAD(x) (3.141592653589793 * (f64)(x) / 180.0)

float Hu3DMotionMaxTimeGet(HU3DMODELID modelId);

/* ---------------------------------------------------------------- data */

static const s32 lbl_1_rodata_F8[3] = { 0x7C0006, 0x7C0007, 0x7C0008 };

static HuVecF lbl_1_data_238 = { 150.0f, 100.0f, -800.0f };
static HuVecF lbl_1_data_244 = { 400.0f, 0.0f, 0.0f };
static HuVecF lbl_1_data_250[17] = {
    { -400.0f, -100.0f, 150.0f },
    { -300.0f, -100.0f, 150.0f },
    { -200.0f, -100.0f, 150.0f },
    { -100.0f, -100.0f, 150.0f },
    { 0.0f, -100.0f, 150.0f },
    { 100.0f, -100.0f, 150.0f },
    { 200.0f, -100.0f, 150.0f },
    { 300.0f, -100.0f, 150.0f },
    { 400.0f, -100.0f, 150.0f },
    { -350.0f, -100.0f, 200.0f },
    { -250.0f, -100.0f, 200.0f },
    { -150.0f, -100.0f, 200.0f },
    { -50.0f, -100.0f, 200.0f },
    { 50.0f, -100.0f, 200.0f },
    { 150.0f, -100.0f, 200.0f },
    { 250.0f, -100.0f, 200.0f },
    { 350.0f, -100.0f, 200.0f }
};

static s32 lbl_1_bss_100;
static s32 lbl_1_bss_104;
static M569SPECT *lbl_1_bss_108[18];

/* ------------------------------------------------------------- functions */

static void fn_1_5D5C(OMOBJ *obj);
static void fn_1_5F48(OMOBJ *obj);
static void fn_1_60F0(OMOBJ *obj);
static void fn_1_6418(OMOBJ *obj);
static void fn_1_6A48(OMOBJ *obj);
static void fn_1_702C(OMOBJ *obj);

M569KURIBO *fn_1_582C(void)
{
    M569KURIBO *kuribo;
    OMOBJ *obj;
    s16 mdl;
    s32 i;
    s16 mot;

    obj = omAddObjEx(lbl_1_bss_D4, 0x2000, 1, 3, -1, fn_1_5D5C);
    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M569KURIBO), HU_MEMNUM_OVL);
    kuribo = (M569KURIBO *)obj->data;
    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0x7C0005, HU_MEMNUM_OVL, HEAP_MODEL));
    obj->mdlId[0] = mdl;
    for (i = 0; i < 3; i++) {
        mot = Hu3DJointMotion(mdl, HuDataSelHeapReadNum(lbl_1_rodata_F8[i], HU_MEMNUM_OVL, HEAP_MODEL));
        obj->mtnId[i] = mot;
    }
    Hu3DModelShadowSet(mdl);
    Hu3DModelAttrSet(mdl, 0x40000001);
    Hu3DMotionSet(mdl, obj->mtnId[1]);
    Hu3DMotionSpeedSet(obj->mdlId[0], 3.0f);
    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0x7C0005, HU_MEMNUM_OVL, HEAP_MODEL));
    obj->mdlId[1] = mdl;
    for (i = 0; i < 3; i++) {
        kuribo->jointMot[i] = Hu3DJointMotion(mdl, HuDataSelHeapReadNum(lbl_1_rodata_F8[i], HU_MEMNUM_OVL, HEAP_MODEL));
    }
    Hu3DModelAttrSet(mdl, 0x800000);
    Hu3DModelAttrSet(mdl, 0x40000001);
    Hu3DModelAttrSet(mdl, 1);
    Hu3DModelScaleSet(mdl, 1.0f, -1.0f, 1.0f);
    Hu3DMotionSet(mdl, (s16)kuribo->jointMot[1]);
    Hu3DMotionSpeedSet(obj->mdlId[1], 3.0f);
    kuribo->motNo = 1;
    kuribo->flag = 0;
    kuribo->pos = lbl_1_data_238;
    kuribo->rot.x = kuribo->rot.y = kuribo->rot.z = 0.0f;
    kuribo->prevTime = 0.0f;
    return kuribo;
}

void fn_1_5A9C(OMOBJ *obj, s32 motNo)
{
    M569KURIBO *kuribo;
    f32 start;
    f32 shift;
    u32 attr;

    kuribo = (M569KURIBO *)obj->data;
    start = 0.0f;
    shift = 10.0f;
    attr = 0x40000001;
    if (kuribo->motNo != motNo) {
        kuribo->motNo = motNo;
        Hu3DModelAttrReset(obj->mdlId[0], 0x40000003);
        Hu3DModelAttrReset(obj->mdlId[1], 0x40000003);
        switch (motNo) {
        case 0:
            Hu3DMotionSpeedSet(obj->mdlId[0], 1.0f);
            Hu3DMotionSpeedSet(obj->mdlId[1], 1.0f);
            break;
        case 1:
            Hu3DMotionSpeedSet(obj->mdlId[0], 3.0f);
            Hu3DMotionSpeedSet(obj->mdlId[1], 3.0f);
            break;
        case 2:
            Hu3DMotionSpeedSet(obj->mdlId[0], 1.0f);
            Hu3DMotionSpeedSet(obj->mdlId[1], 1.0f);
            attr = 0;
            break;
        }
        Hu3DModelAttrSet(obj->mdlId[0], attr);
        Hu3DMotionSet(obj->mdlId[0], obj->mtnId[motNo]);
        Hu3DModelAttrSet(obj->mdlId[1], attr);
        Hu3DMotionSet(obj->mdlId[1], obj->mtnId[motNo]);
    }
}

void fn_1_5C54(OMOBJ *obj)
{
    M569KURIBO *kuribo;
    f32 time;

    kuribo = (M569KURIBO *)obj->data;
    time = Hu3DMotionTimeGet(obj->mdlId[0]);
    switch (kuribo->motNo) {
    case 1:
        if (time < kuribo->prevTime) {
            fn_1_109C(&kuribo->pos, 0x2CE);
        }
        if (kuribo->prevTime < 20.0f) {
            if (20.0f <= time) {
                fn_1_109C(&kuribo->pos, 0x2CE);
            }
        }
        break;
    case 2:
        if (kuribo->prevTime < 10.0f) {
            if (10.0f <= time) {
                fn_1_109C(&kuribo->pos, 0x2DE);
            }
        }
        break;
    }
    kuribo->prevTime = time;
}

static void fn_1_5D5C(OMOBJ *obj)
{
    M569KURIBO *kuribo = (M569KURIBO *)obj->data;
    HuVecF target = { 0.0f, 100.0f, -800.0f };

    fn_1_5C54(obj);
    if (lbl_1_bss_100 == 0) {
        if (fn_1_3378(&kuribo->pos, &target, &kuribo->rot, 4.0f) != 0) {
            lbl_1_bss_100 = 1;
            obj->work[0] = 0;
            Hu3DModelAttrReset(obj->mdlId[0], 0x40000001);
        }
    } else {
        if (obj->work[0]++ == 5) {
            obj->work[0] = 0;
            obj->objFunc = fn_1_5F48;
        }
    }
    omSetTra(obj, kuribo->pos.x, kuribo->pos.y, kuribo->pos.z);
    omSetRot(obj, kuribo->rot.x, kuribo->rot.y, kuribo->rot.z);
}

static void fn_1_5F48(OMOBJ *obj)
{
    M569KURIBO *kuribo;

    kuribo = (M569KURIBO *)obj->data;
    fn_1_5C54(obj);
    kuribo->rot.y = fn_1_349C(kuribo->rot.y, 0.0f, 10.0f);
    if (kuribo->rot.y == 0.0f) {
        lbl_1_bss_14 = 1;
        Hu3DModelAttrSet(obj->mdlId[0], 0x40000001);
        obj->objFunc = fn_1_60F0;
    }
    omSetTra(obj, kuribo->pos.x, kuribo->pos.y, kuribo->pos.z);
    omSetRot(obj, kuribo->rot.x, kuribo->rot.y, kuribo->rot.z);
}

static void fn_1_60F0(OMOBJ *obj)
{
    M569KURIBO *kuribo = (M569KURIBO *)obj->data;
    HuVecF path[3] = {
        { 0.0f, 100.0f, -670.0f },
        { 0.0f, 0.0f, -600.0f },
        { 0.0f, 0.0f, 0.0f }
    };
    s32 reached;

    fn_1_5C54(obj);
    if (obj->work[2] != 0) {
        obj->work[2]--;
        return;
    }
    if (obj->work[0] == 1) {
        if (obj->work[1] == 0) {
            if (kuribo->pos.y < 70.0f) {
                obj->work[1] = 1;
                obj->work[2] = 5;
            }
        } else if (obj->work[1] == 1 && kuribo->pos.y < 35.0f) {
            obj->work[1] = 2;
            obj->work[2] = 5;
        }
    }
    reached = fn_1_3378(&kuribo->pos, &path[obj->work[0]], &kuribo->rot, 10.0f);
    if (reached != 0) {
        obj->work[0]++;
        if (obj->work[0] == 3) {
            Hu3DModelAttrReset(obj->mdlId[0], 0x40000001);
            obj->work[0] = 0;
            obj->work[1] = 0;
            obj->work[2] = 0;
            obj->objFunc = fn_1_6418;
        }
        if (obj->work[0] == 1) {
            Hu3DMotionSpeedSet(obj->mdlId[0], 3.0f);
            Hu3DMotionSpeedSet(obj->mdlId[1], 3.0f);
            Hu3DModelAttrReset(obj->mdlId[1], 1);
        }
    }
    Hu3DModelPosSet(obj->mdlId[1], kuribo->pos.x, 0.0f, kuribo->pos.z);
    Hu3DModelRotSetV(obj->mdlId[1], &kuribo->rot);
    omSetTra(obj, kuribo->pos.x, kuribo->pos.y, kuribo->pos.z);
    omSetRot(obj, kuribo->rot.x, kuribo->rot.y, kuribo->rot.z);
}

static void fn_1_6418(OMOBJ *obj)
{
    M569KURIBO *kuribo;
    f32 ang;

    kuribo = (M569KURIBO *)obj->data;
    fn_1_5C54(obj);
    switch (obj->work[0]) {
    case 0:
        fn_1_5A9C(obj, 2);
        fn_1_735C();
        lbl_1_bss_14 = 2;
        obj->work[0] = 1;
        obj->work[1] = 0;
        break;
    case 1:
        obj->work[1]++;
        if (obj->work[1] == 0x19) {
            CharEffectDustCreate(1, 1.0f, &kuribo->pos);
        }
        if (Hu3DMotionEndCheck(obj->mdlId[0]) != 0) {
            obj->work[0] = 2;
            obj->work[1] = 0;
        }
        break;
    case 2:
        if (obj->work[1]++ == 0x1E) {
            fn_1_5A9C(obj, 1);
            obj->work[0] = 3;
            obj->work[1] = 0;
        }
        break;
    case 3:
        obj->work[1]++;
        if (fn_1_3378(&kuribo->pos, &lbl_1_data_244, &kuribo->rot, 7.0f) != 0) {
            obj->work[1] = 0;
            obj->work[0] = 4;
        }
        break;
    case 4:
        ang = (f32)(180.0 * (atan2(lbl_1_bss_34.pos.x - kuribo->pos.x, lbl_1_bss_34.pos.z - kuribo->pos.z) / 3.141592653589793));
        if (ang < 0.0f) {
            ang += 360.0f;
        }
        kuribo->rot.y = fn_1_349C(kuribo->rot.y, ang, 10.0f);
        if (fabs(kuribo->rot.y - ang) <= 0.1f) {
            kuribo->rot.y = ang;
            obj->work[0] = 0;
            fn_1_5A9C(obj, 0);
            kuribo->flag = 1;
            obj->objFunc = fn_1_6A48;
        }
        break;
    }
    Hu3DModelPosSetV(obj->mdlId[1], &kuribo->pos);
    Hu3DModelRotSetV(obj->mdlId[1], &kuribo->rot);
    omSetTra(obj, kuribo->pos.x, kuribo->pos.y, kuribo->pos.z);
    omSetRot(obj, kuribo->rot.x, kuribo->rot.y, kuribo->rot.z);
}

static void fn_1_6A48(OMOBJ *obj)
{
    M569KURIBO *kuribo;

    kuribo = (M569KURIBO *)obj->data;
    fn_1_5C54(obj);
    switch (kuribo->flag) {
    case 1:
        break;
    case 2:
        fn_1_5A9C(obj, 2);
        kuribo->flag = 3;
        obj->work[0] = 0;
        return;
    case 3:
        obj->work[0]++;
        if (obj->work[0] == 0x19) {
            CharEffectDustCreate(1, 1.0f, &kuribo->pos);
        }
        if (Hu3DMotionEndCheck(obj->mdlId[0]) != 0) {
            fn_1_5A9C(obj, 0);
            kuribo->flag = 1;
        }
        return;
    }
}

void fn_1_6D9C(M569KURIBO *kuribo)
{
    if (kuribo->flag == 1) {
        kuribo->flag = 2;
    }
}

void fn_1_6DB4(void)
{
    s32 i;
    OMOBJ *obj;
    s16 mdl;
    s16 mot;
    s32 rnd;

    for (i = 0; i < 17; i++) {
        obj = omAddObjEx(lbl_1_bss_D4, 0x2000, 1, 1, -1, fn_1_702C);
        obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M569SPECT), HU_MEMNUM_OVL);
        lbl_1_bss_108[i] = (M569SPECT *)obj->data;
        if (i == 4) {
            mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0x7C0009, HU_MEMNUM_OVL, HEAP_MODEL));
        } else if (i == 3) {
            mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0x7C000B, HU_MEMNUM_OVL, HEAP_MODEL));
        } else {
            mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0x7C000D, HU_MEMNUM_OVL, HEAP_MODEL));
        }
        obj->mdlId[0] = mdl;
        mot = Hu3DJointMotion(mdl, HuDataSelHeapReadNum(0x7C000E, HU_MEMNUM_OVL, HEAP_MODEL));
        obj->mtnId[0] = mot;
        Hu3DModelAttrSet(mdl, 0x40000001);
        Hu3DMotionSet(mdl, obj->mtnId[0]);
        rnd = frandmod((u32)Hu3DMotionMaxTimeGet(mdl));
        Hu3DMotionTimeSet(mdl, (f32)rnd);
        lbl_1_bss_108[i]->pos = lbl_1_data_250[i];
        lbl_1_bss_108[i]->ofs.x = 0.0f;
        lbl_1_bss_108[i]->ofs.y = 0.0f;
        lbl_1_bss_108[i]->ofs.z = 0.0f;
        lbl_1_bss_108[i]->state = 0;
        lbl_1_bss_108[i]->angle = 0;
    }
}

static void fn_1_702C(OMOBJ *obj)
{
    M569SPECT *spect;

    spect = (M569SPECT *)obj->data;
    switch (spect->state) {
    case 1:
        spect->angle += 8;
        if (spect->dir != 0) {
            spect->ofs.x = 5.0 * sin(M569_RAD((f32)(spect->angle % 180)));
        } else {
            spect->ofs.y = 5.0 * sin(M569_RAD((f32)(spect->angle % 180)));
        }
        if (obj->work[0]++ == 0x5A) {
            HuAudFXStop(lbl_1_bss_104);
            obj->work[0] = 0;
            spect->state = 0;
        }
        break;
    case 2:
        spect->angle += 8;
        if (spect->dir != 0) {
            spect->ofs.x = 5.0 * sin(M569_RAD((f32)(spect->angle % 180)));
        } else {
            spect->ofs.y = 5.0 * sin(M569_RAD((f32)(spect->angle % 180)));
        }
        break;
    }
    omSetRot(obj, 0.0f, 180.0f, 0.0f);
    omSetTra(obj, spect->pos.x + spect->ofs.x, spect->pos.y + spect->ofs.y, spect->pos.z);
}

void fn_1_735C(void)
{
    s32 i;

    for (i = 0; i < 17; i++) {
        if (lbl_1_bss_108[i]->state == 0) {
            lbl_1_bss_108[i]->state = 1;
            lbl_1_bss_108[i]->angle += frandmod(0x5A) * 8;
            lbl_1_bss_108[i]->dir = frandmod(2);
        }
    }
}

void fn_1_741C(void)
{
    s32 i;

    for (i = 0; i < 17; i++) {
        if (lbl_1_bss_108[i]->state == 0) {
            lbl_1_bss_108[i]->state = 2;
            lbl_1_bss_108[i]->dir = frandmod(2);
        }
    }
}

void fn_1_74B0(void)
{
    lbl_1_bss_104 = HuAudFXPlay(0x39D);
}
