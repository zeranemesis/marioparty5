#include "types.h"
#include "game/audio.h"
#include "game/data.h"
#include "game/hsfformat.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/mg/seqman.h"
#include "game/object.h"
#include "REL/m514Dll/m514types.h"

#include <string.h>

void fn_1_4348(OMOBJ *obj);
void fn_1_4358(OMOBJ *obj);
void fn_1_435C(void);
HU3DMODELID fn_1_43F8(s32 dataNum);
HU3DMOTID fn_1_44E4(HU3DMODELID model, s32 dataNum);
void fn_1_45D4(OMOBJMAN *objman, M514OBJDEF *def);
void fn_1_48B0(OMOBJ *obj);
void fn_1_490C(OMOBJ *obj, s32 motNo);
s32 fn_1_49E4(OMOBJ *obj);
void fn_1_4A4C(OMOBJ *obj);
void fn_1_4C40(OMOBJ *obj);

M514OBJMOT lbl_1_data_300[2] = {
    { 0x480007, 0.2f, 0x40000001 },
    { 0x480008, 0.2f, 0x40000001 },
};

M514OBJMOT lbl_1_data_318[2] = {
    { 0x48000A, 0.2f, 0x40000001 },
    { 0x48000B, 0.2f, 0x40000001 },
};

M514OBJMOT lbl_1_data_330[2] = {
    { 0x48000D, 0.2f, 0x40000001 },
    { 0x48000E, 0.2f, 0x40000001 },
};

M514OBJMOT lbl_1_data_348[2] = {
    { 0x480010, 0.2f, 0x40000001 },
    { 0x480011, 0.2f, 0x40000001 },
};

M514OBJMOT lbl_1_data_360[2] = {
    { 0x480014, 0.2f, 0x40000001 },
    { 0x480015, 0.2f, 0x40000001 },
};

HuVecF lbl_1_data_378 = { -1500.0f, 70.0f, -400.0f };
HuVecF lbl_1_data_384 = { -500.0f, 70.0f, -400.0f };
HuVecF lbl_1_data_390 = { 500.0f, 70.0f, -400.0f };
HuVecF lbl_1_data_39C = { 1500.0f, 70.0f, -400.0f };
HuVecF lbl_1_data_3A8 = { 2500.0f, 70.0f, -400.0f };

M514OBJDEF lbl_1_data_3B4[6] = {
    { 0, 0, 0x480006, lbl_1_data_300, &lbl_1_data_378, 0.0f, fn_1_4A4C },
    { 1, 1, 0x480009, lbl_1_data_318, &lbl_1_data_384, 0.0f, fn_1_4A4C },
    { 2, 2, 0x48000C, lbl_1_data_330, &lbl_1_data_390, 0.0f, fn_1_4A4C },
    { 3, 3, 0x48000F, lbl_1_data_348, &lbl_1_data_39C, 0.0f, fn_1_4A4C },
    { 4, 4, 0x480013, lbl_1_data_360, &lbl_1_data_3A8, 0.0f, fn_1_4A4C },
    { -1, 0, 0, NULL, NULL, 0.0f, NULL },
};

s32 lbl_1_data_45C[5] = { 0x480018, 0x48001B, 0x48001C, 0x48001D, 0x48001E };

char lbl_1_data_470[8] = "obj12";

/* bss: MWCC lays it out in reverse declaration order */
static OMOBJ *lbl_1_bss_208;
static s32 lbl_1_bss_188[0x20];
static s32 lbl_1_bss_108[0x20];
static s16 lbl_1_bss_C8[0x20];
static s16 lbl_1_bss_88[0x20];

void fn_1_4270(OMOBJMAN *objman)
{
    OMOBJ *obj;
    M514OBJDEF *def;

    obj = omAddObjEx(objman, 0x32, 0, 0, -1, fn_1_4348);
    lbl_1_bss_208 = obj;
    obj->stat |= 0x100;
    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M514OBJMGR), 0x10000000);
    memset(obj->data, 0, sizeof(M514OBJMGR));
    fn_1_435C();
    for (def = lbl_1_data_3B4; def->kind != -1; def++) {
        fn_1_45D4(objman, def);
    }
}

void fn_1_4344(void)
{
}

void fn_1_4348(OMOBJ *obj)
{
    obj->objFunc = fn_1_4358;
}

void fn_1_4358(OMOBJ *obj)
{
}

void fn_1_435C(void)
{
    s32 i;

    for (i = 0; i < 0x20; i++) {
        lbl_1_bss_188[i] = -1;
        lbl_1_bss_C8[i] = -1;
    }
    for (i = 0; i < 0x20; i++) {
        lbl_1_bss_108[i] = -1;
        lbl_1_bss_88[i] = -1;
    }
}

HU3DMODELID fn_1_43F8(s32 dataNum)
{
    HU3DMODELID model;
    s32 i;

    for (i = 0; i < 0x20; i++) {
        if (lbl_1_bss_188[i] == -1) {
            break;
        }
        if (dataNum == lbl_1_bss_188[i]) {
            return Hu3DModelLink(lbl_1_bss_C8[i]);
        }
    }
    if (i < 0x20) {
        model = Hu3DModelCreate(HuDataSelHeapReadNum(dataNum, 0x10000000, HEAP_MODEL));
        lbl_1_bss_188[i] = dataNum;
        lbl_1_bss_C8[i] = model;
        return model;
    }
    return -1;
}

HU3DMOTID fn_1_44E4(HU3DMODELID model, s32 dataNum)
{
    HU3DMOTID mot;
    s32 i;

    for (i = 0; i < 0x20; i++) {
        if (lbl_1_bss_108[i] == -1) {
            break;
        }
        if (dataNum == lbl_1_bss_108[i]) {
            return lbl_1_bss_88[i];
        }
    }
    if (i < 0x20) {
        mot = Hu3DJointMotion(model, HuDataSelHeapReadNum(dataNum, 0x10000000, HEAP_MODEL));
        lbl_1_bss_108[i] = dataNum;
        lbl_1_bss_88[i] = mot;
        return mot;
    }
    return -1;
}

void fn_1_45D4(OMOBJMAN *objman, M514OBJDEF *def)
{
    OMOBJ *obj;
    M514OBJ *w;
    HU3DMODELID model;
    s32 i;

    obj = omAddObjEx(objman, 0x32, 3, 2, 0, def->func);
    w = HuMemDirectMallocNum(HEAP_HEAP, sizeof(M514OBJ), 0x10000000);
    obj->data = w;
    memset(w, 0, sizeof(M514OBJ));
    obj->stat |= 0x100;
    w->def = def;
    w->motNo = -1;
    w->sfx1 = -1;
    w->sfx2 = -1;
    for (i = 0; i < 3; i++) {
        obj->mdlId[i] = -1;
    }
    for (i = 0; i < 2; i++) {
        obj->mtnId[i] = -1;
    }
    model = fn_1_43F8(def->modelNum);
    obj->mdlId[0] = model;
    for (i = 0; i < 2; i++) {
        s32 dataNum = def->mot[i].dataNum;
        if (dataNum == -2) {
            break;
        }
        if (dataNum != -1) {
            obj->mtnId[i] = fn_1_44E4(model, dataNum);
        }
    }
    w->pos.x = def->pos->x;
    w->pos.y = def->pos->y;
    w->pos.z = def->pos->z;
    w->rot.y = def->rotY;
    Hu3DModelLayerSet(model, 4);
    fn_1_48B0(obj);
}

void fn_1_48B0(OMOBJ *obj)
{
    M514OBJ *w = obj->data;
    HU3DMODELID model = obj->mdlId[0];

    Hu3DModelPosSetV(model, &w->pos);
    Hu3DModelRotSetV(model, &w->rot);
}

void fn_1_490C(OMOBJ *obj, s32 motNo)
{
    M514OBJ *w = obj->data;

#ifdef TARGET_PC
    if (motNo < 0) {
        return;
    }
#endif
    if (w->motNo != motNo && motNo < 2) {
        HU3DMOTID mot = obj->mtnId[motNo];
        if (mot != -1) {
            M514OBJMOT *info = &w->def->mot[motNo];
            HU3DMODELID model = obj->mdlId[0];
            if (w->motNo < 0) {
                Hu3DMotionSet(model, mot);
                Hu3DModelAttrSet(model, info->attr);
            } else {
                Hu3DMotionShiftSet(model, mot, 0.0f, 60.0f * info->shift, info->attr);
            }
            w->motNo = motNo;
        }
    }
}

s32 fn_1_49E4(OMOBJ *obj)
{
    HU3DMODELID model = obj->mdlId[0];

    if (Hu3DMotionEndCheck(model) != 0 && Hu3DMotionShiftIDGet(model) < 0) {
        return 1;
    }
    return 0;
}

void fn_1_4A4C(OMOBJ *obj)
{
    M514OBJ *w = obj->data;
    HU3DMODELID model;
    HSFOBJECT *hsfObj;

    Hu3DModelShadowSet(obj->mdlId[0]);
    fn_1_490C(obj, 0);
    model = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_45C[w->def->idx], 0x10000000, HEAP_MODEL));
    obj->mdlId[1] = model;
    Hu3DModelPosSet(model, w->pos.x, w->pos.y, (f32)((f64)w->pos.z - 50.0));
    Hu3DModelLayerSet(model, 4);
    if (w->def->kind == 3) {
        model = Hu3DModelCreate(HuDataSelHeapReadNum(0x480012, 0x10000000, HEAP_MODEL));
        obj->mdlId[2] = model;
        Hu3DModelPosSet(model, w->pos.x, w->pos.y, w->pos.z);
        Hu3DModelAttrSet(model, 1);
        Hu3DModelLayerSet(model, 4);
    }
    if (w->def->kind == 4) {
        hsfObj = Hu3DModelObjPtrGet(obj->mdlId[0], lbl_1_data_470);
        if (hsfObj != NULL) {
            hsfObj->flags |= 0x400;
        }
    }
    obj->objFunc = fn_1_4C40;
}

void fn_1_4C40(OMOBJ *obj)
{
    M514OBJMGR *mgr = lbl_1_bss_208->data;
    M514OBJ *w = obj->data;
    HU3DMODELID model;

    switch (w->state) {
        case 0:
            w->state++;
            /* fallthrough */
        case 1:
            if (MgSeqModeGet() != 5) {
                break;
            }
            w->state++;
            /* fallthrough */
        case 2:
            w->state++;
            w->snapshot = fn_1_1814();
            fn_1_490C(obj, 0);
            /* fallthrough */
        case 3:
            if (fn_1_95F4(fn_1_1814()) < w->pos.x) {
                break;
            }
            w->state++;
            w->timer = 0;
            fn_1_490C(obj, 1);
            switch (w->def->kind) {
                case 4:
                    break;
                case 0:
                    w->sfx1 = HuAudFXPlay(0x2CF);
                    break;
                case 1:
                    w->sfx1 = HuAudFXPlay(0x2C4);
                    break;
                case 2:
                    w->sfx1 = HuAudFXPlay(0x2D4);
                    break;
                case 3:
                    w->sfx1 = HuAudFXPlay(0x2D7);
                    w->state = 5;
                    mgr->flag = 1;
                    break;
            }
            break;
        case 4:
            if (w->snapshot != fn_1_1814()) {
                w->state = 2;
            }
            break;
        case 5:
            w->timer++;
            if ((f32)w->timer < 90.0) {
                break;
            }
            w->state++;
            w->timer = 0;
            model = obj->mdlId[2];
            Hu3DMotionTimeSet(model, 0.0f);
            Hu3DModelAttrReset(model, 0x40000002);
            Hu3DModelAttrReset(model, 1);
            w->sfx2 = HuAudFXPlay(0x6DE);
            /* fallthrough */
        case 6:
            w->timer++;
            if ((f32)w->timer < 6.0) {
                break;
            }
            w->state++;
            Hu3DModelAttrSet(obj->mdlId[0], 1);
            /* fallthrough */
        case 7:
            model = obj->mdlId[2];
            if (Hu3DMotionEndCheck(model) == 0) {
                break;
            }
            Hu3DModelAttrSet(model, 1);
            w->state++;
            mgr->flag = 0;
            /* fallthrough */
        case 8:
            if (!(Center.x > -1000.0)) {
                w->state = 2;
                Hu3DModelAttrReset(obj->mdlId[0], 1);
            }
            break;
    }
    PSVECAdd(&w->pos, &w->vel, &w->pos);
    if (w->sfx1 != -1) {
        if (HuAudFXStatusGet(w->sfx1) == 2) {
            fn_1_1AF4(w->sfx1, &w->pos);
        } else {
            w->sfx1 = -1;
        }
    }
    if (w->sfx2 != -1) {
        if (HuAudFXStatusGet(w->sfx2) == 2) {
            fn_1_1AF4(w->sfx2, &w->pos);
        } else {
            w->sfx2 = -1;
        }
    }
    model = obj->mdlId[0];
    Hu3DModelPosSetV(model, &w->pos);
    Hu3DModelRotSetV(model, &w->rot);
}

s32 fn_1_50E4(void)
{
    M514OBJMGR *mgr = lbl_1_bss_208->data;

    return mgr->flag;
}
