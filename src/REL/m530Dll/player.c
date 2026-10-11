#include "game/charman.h"
#include "game/data.h"
#include "game/esprite.h"
#include "game/hsfex.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/pad.h"
#include "math.h"

#include "game/mg/seqman.h"

#include "REL/m530Dll.h"

#include "datadir_enum.h"

int HuAudFXPlay(int seId);



typedef struct m530playerwork_s {
    s16 playerNo; //0x00
    s16 charNo; //0x02
    s16 padNo; //0x04
    s16 side; //0x06
    s16 varNo; //0x08
    s16 padA; //0x0A
    s32 flag; //0x0C
    Vec pos; //0x10
    Vec rot; //0x1C
    s32 state; //0x28
    s32 unk2C; //0x2C
    s32 unk30; //0x30
    void *itemHook; //0x34
    s32 timer; //0x38
    s32 mode; //0x3C
    s32 timer2; //0x40
    s32 targetNo; //0x44
    Vec target; //0x48
    s16 espId; //0x54
    s16 pad56; //0x56
    Vec screen; //0x58
    Vec world; //0x64
    void *item; //0x70
    s32 unk74; //0x74
    f32 unk78; //0x78
    f32 unk7C; //0x7C
    f32 unk80; //0x80
    f32 unk84; //0x84
    s32 unk88; //0x88
} M530_PLAYER_WORK;

typedef struct m530item_s {
    s16 playerNo; //0x00
    s16 pad2; //0x02
    Vec pos; //0x04
    Vec vel; //0x10
    f32 pad1C; //0x1C
    f32 pad20; //0x20
    s32 pad24; //0x24
    s32 targetNo; //0x28
    s32 timer; //0x2C
    s32 alive; //0x30
    s16 mdlId; //0x34
    s16 pad36; //0x36
} M530_ITEM;

typedef struct m530cell_s {
    s32 unk0; //0x00
    s32 unk4; //0x04
    s32 unk8; //0x08
    s32 unkC; //0x0C
    Vec pos; //0x10
    u8 pad1C[0x20];
    s32 unk3C; //0x3C
    s32 unk40; //0x40
    u8 pad44[0xC];
} M530_CELL;

void fn_1_6F90(OMOBJ *obj);
void fn_1_6FFC(OMOBJ *obj);
void fn_1_7224(OMOBJ *obj);
void fn_1_7304(OMOBJ *obj);
void fn_1_74B0(OMOBJ *obj);
void fn_1_7674(OMOBJ *obj);
void fn_1_7748(OMOBJ *obj);
void fn_1_7900(OMOBJ *obj);
void fn_1_79D8(OMOBJ *obj);
void fn_1_7AFC(OMOBJ *obj);
void fn_1_7BEC(OMOBJ *obj);
void fn_1_7DD0(OMOBJ *obj);
void fn_1_7FF4(OMOBJ *obj);
s32 fn_1_80C4(Vec *arg0, Vec *arg1);
s32 fn_1_81DC(Vec *arg0);
void fn_1_825C(OMOBJ *obj);
void fn_1_8398(Vec *arg0, Vec *arg1);
void fn_1_8434(OMOBJ *obj);
void fn_1_85D4(OMOBJ *obj);
void fn_1_86DC(OMOBJ *obj);
void fn_1_87FC(OMOBJ *obj);

static Vec lbl_1_data_2E8[2] = {
    { -450.0f, 0.0f, 550.0f },
    { 450.0f, 0.0f, 550.0f }
};

static Vec lbl_1_data_300 = { 0.0f, 570.0f, -100.0f };

static u32 lbl_1_data_30C[2] = { 0x58000D, 0x58000E };

static Vec lbl_1_data_314[2] = {
    { 243.0f, 143.0f, 0.0f },
    { 333.0f, 143.0f, 0.0f }
};

static u32 lbl_1_data_32C[2] = { 0x58000A, 0x58000B };

static char lbl_1_data_334[12] = "itemhook_b";

static u32 lbl_1_data_340[2] = { 0x580008, 0x580009 };

static Vec lbl_1_data_348[2] = {
    { -1700.0f, 0.0f, 245.0f },
    { 1700.0f, 0.0f, 245.0f }
};

static Vec lbl_1_data_360[2] = {
    { -200.0f, 0.0f, 245.0f },
    { 200.0f, 0.0f, 245.0f }
};

static Vec lbl_1_data_378[2] = {
    { 0.0f, 90.0f, 0.0f },
    { 0.0f, -90.0f, 0.0f }
};

static s32 lbl_1_bss_D0[3];

static M530_CELL lbl_1_bss_DC[21];

static const f32 lbl_1_rodata_184 = 110.0f;

void fn_1_6D7C(OMOBJ *obj) {
    M530_PLAYER_WORK *work = obj->data;
    s16 side = work->side;
    HU3DMODELID modelId;
    s16 espId;

    modelId = CharModelMotListCreate(work->charNo, 2, lbl_1_data_C0, obj->mtnId);
    obj->mdlId[0] = modelId;
    work->pos = lbl_1_data_2E8[side];
    omSetTra(obj, work->pos.x, work->pos.y, work->pos.z);
    Hu3DModelShadowSet(modelId);
    CharMotionSet(work->charNo, obj->mtnId[10]);
    CharMotionSpeedSet(work->charNo, 2.4444444f);
    Hu3DSubMotionSet(modelId, obj->mtnId[9], 0.0f);
    Hu3DMotionShiftSpeedSet(modelId, 2.4444444f);
    modelId = Hu3DModelCreate(HuDataSelHeapReadNum(0x58000C, HU_MEMNUM_OVL, HEAP_MODEL));
    obj->mdlId[2] = modelId;
    work->itemHook = CharModelItemHookGet(work->charNo, 2, 1);
    Hu3DModelHookSet(obj->mdlId[0], work->itemHook, modelId);
    Hu3DMotionSpeedSet(modelId, 2.4444444f);
    work->screen = lbl_1_data_314[side];
    espId = espEntry(lbl_1_data_30C[side], 0x100, 0);
    espDrawNoSet(espId, 0);
    espPosSet(espId, work->screen.x, work->screen.y);
    espDispOff(espId);
    work->espId = espId;
    fn_1_8398(&work->screen, &work->world);
    fn_1_825C(obj);
    fn_1_7304(obj);
    work->targetNo = -1;
    work->mode = 0;
    work->timer2 = 0;
    obj->objFunc = fn_1_6F90;
}

void fn_1_6F90(OMOBJ *obj) {
    M530_PLAYER_WORK *work;

    work = obj->data;
    if (work->state == 1) {
        fn_1_74B0(obj);
        work->timer = 0;
        work->state = 2;
        obj->objFunc = fn_1_6FFC;
    }
    fn_1_7224(obj);
}

void fn_1_6FFC(OMOBJ *obj) {
    M530_PLAYER_WORK *work;
    s16 vol;
    s16 pan;
    s32 playerNoTmp;

    work = obj->data;
    playerNoTmp = work->playerNo;
    fn_1_1A50(&work->pos.x, &vol, &pan);
    CharModelVoicePanSet(work->charNo, pan);
    CharModelVoiceVolSet(work->charNo, vol);
    fn_1_8398(&work->screen, &work->world);
    if (MgSeqModeGet() == 6) {
        work->timer = 0;
        obj->objFunc = fn_1_7674;
        return;
    }
    fn_1_7224(obj);
    if (MgSeqModeGet() == 5) {
        if (work->flag != 0) {
            fn_1_7900(obj);
        }
        else {
            work->screen.x = work->screen.x + (0.125f * (f32)(s8)HuPadStkX[work->padNo]);
            work->screen.y = work->screen.y - (0.125f * (f32)(s8)HuPadStkY[work->padNo]);
        }
        if (work->screen.x < 90.0f) {
            work->screen.x = 90.0f;
        }
        if (work->screen.x > 490.0f) {
            work->screen.x = 490.0f;
        }
        if (work->screen.y < 50.0f) {
            work->screen.y = 50.0f;
        }
        if (work->screen.y > 214.0f) {
            work->screen.y = 214.0f;
        }
        espPosSet(work->espId, work->screen.x, work->screen.y);
        fn_1_825C(obj);
    }
}

void fn_1_7224(OMOBJ *obj) {
    M530_PLAYER_WORK *work;

    work = obj->data;
    switch (work->state) {
    case 0:
        if (CharMotionEndCheck(work->charNo) != 0) {
            work->state = 1;
        }
        break;
    case 1:
        if (work->flag == 0 && MgSeqModeGet() == 5 && (HuPadBtnDown[work->padNo] & 0x100) != 0) {
            fn_1_74B0(obj);
        }
        break;
    case 2:
        if (CharMotionEndCheck(work->charNo) != 0) {
            fn_1_7304(obj);
            work->state = 0;
        }
        break;
    }
}

void fn_1_7304(OMOBJ *obj) {
    M530_PLAYER_WORK *work;
    M530_ITEM *item;
    OMOBJ *itemObj;
    HU3DMODELID modelId;

    work = obj->data;
    modelId = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_32C[work->side], HU_MEMNUM_OVL, HEAP_MODEL));
    itemObj = omAddObjEx(lbl_1_bss_B8, 0x2000, 1, 0, -1, fn_1_7DD0);
    itemObj->data = HuMemDirectMallocNum(HEAP_HEAP, 0x38, HU_MEMNUM_OVL);
    Hu3DModelAttrSet(modelId, 0x40000002);
    Hu3DModelLayerSet(modelId, 2);
    Hu3DModelHookSet(obj->mdlId[2], lbl_1_data_334, modelId);
    itemObj->mdlId[0] = modelId;
    item = itemObj->data;
    item->playerNo = work->playerNo;
    item->targetNo = -1;
    item->alive = 1;
    item->mdlId = modelId;
    work->item = item;
    CharMotionTimeSet(work->charNo, 0.0f);
    CharMotionStartEndSet(work->charNo, 0.0f, 59.0f);
    Hu3DMotionTimeSet(obj->mdlId[2], 0.0f);
    Hu3DMotionStartEndSet(obj->mdlId[2], 0.0f, 59.0f);
    Hu3DMotionShiftStartEndSet(obj->mdlId[0], 0.0f, 59.0f);
    Hu3DMotionShiftTimeSet(obj->mdlId[0], 0.0f);
}

void fn_1_74B0(OMOBJ *obj) {
    M530_PLAYER_WORK *work;
    M530_ITEM *item;
    Vec sp8;

    work = obj->data;
    item = work->item;
    item->alive = 0;
    item->timer = 0;
    CharMotionTimeSet(work->charNo, 60.0f);
    CharMotionStartEndSet(work->charNo, 60.0f, 109.0f);
    Hu3DMotionTimeSet(obj->mdlId[2], 60.0f);
    Hu3DMotionStartEndSet(obj->mdlId[2], 60.0f, 109.0f);
    Hu3DMotionShiftStartEndSet(obj->mdlId[0], 60.0f, 109.0f);
    Hu3DMotionShiftTimeSet(obj->mdlId[0], 60.0f);
    Hu3DMotionCalc(obj->mdlId[0]);
    Hu3DMotionCalc(obj->mdlId[2]);
    Hu3DModelObjPosGet(obj->mdlId[0], lbl_1_data_334, &item->pos);
    Hu3DModelHookObjReset(obj->mdlId[2], lbl_1_data_334);
    PSVECSubtract(&work->world, &item->pos, &sp8);
    PSVECNormalize(&sp8, &sp8);
    PSVECScale(&sp8, &item->vel, 30.0f);
    Hu3DModelPosSetV(item->mdlId, &item->pos);
    omVibrate(work->playerNo, 0xC, 6, 6);
    work->timer = 0;
    work->state = 2;
    if (work->side == 0) {
        HuAudFXPlay(0x798);
        return;
    }
    HuAudFXPlay(0x799);
}

void fn_1_7674(OMOBJ *obj) {
    M530_PLAYER_WORK *work;
    Vec sp8;

    work = obj->data;
    if (work->timer == 0) {
        Hu3DModelObjPosGet(obj->mdlId[0], work->itemHook, &sp8);
        CharEffectSmokeCreate((s16)lbl_1_data_6C[0], &sp8);
        Hu3DModelAttrSet(obj->mdlId[2], 1);
        Hu3DSubMotionReset(obj->mdlId[0]);
        CharMotionSpeedSet(work->charNo, 1.0f);
        fn_1_10E8(obj, 0);
    }
    work->timer = work->timer + 1;
    if (work->timer == 0x3C) {
        work->timer = 0;
        obj->objFunc = fn_1_7748;
    }
}

void fn_1_7748(OMOBJ *obj) {
    M530_PLAYER_WORK *work;

    work = obj->data;
    if (work->timer++ == 0) {
        CharModelAttrReset(work->charNo, 0x40000002);
        CharModelAttrReset(work->charNo, 0x40000001);
        CharMotionSet(work->charNo, obj->mtnId[11]);
        if (work->side == 0) {
            CharMotionTimeSet(work->charNo, 0.0f);
            CharMotionStartEndSet(work->charNo, 0.0f, 59.0f);
        }
        else {
            CharMotionTimeSet(work->charNo, 60.0f);
            CharMotionStartEndSet(work->charNo, 60.0f, 119.0f);
        }
        return;
    }
    if (work->rot.y != 0.0f) {
        if (work->side == 0) {
            work->rot.y = work->rot.y - 3.0f;
            if (work->rot.y <= 0.0f) {
                work->rot.y = 0.0f;
            }
        }
        else {
            work->rot.y += 3.0f;
            if (work->rot.y >= 360.0f) {
                work->rot.y = 0.0f;
            }
        }
        omSetRot(obj, work->rot.x, work->rot.y, work->rot.z);
    }
}

void fn_1_7900(OMOBJ *obj) {
    M530_PLAYER_WORK *work;

    work = obj->data;
    if (work->targetNo != -1) {
        if (lbl_1_bss_DC[work->targetNo].unk40 != 4) {
            work->mode = 0;
        }
        if (lbl_1_bss_DC[work->targetNo].unk3C != 2) {
            work->mode = 0;
        }
    }
    switch (work->mode) {
    case 0:
        fn_1_79D8(obj);
        break;
    case 1:
        fn_1_7AFC(obj);
        break;
    case 2:
        fn_1_7BEC(obj);
        break;
    }
}

void fn_1_79D8(OMOBJ *obj) {
    M530_PLAYER_WORK *work = obj->data;
    Vec sp18;
    s32 cellNo;
    s32 sp8[4] = { 80, 60, 30, 0 };

    if (work->timer2++ > sp8[work->varNo] &&
        (cellNo = fn_1_6B80(&work->world.x, work->varNo)) != -1) {
        work->timer2 = 0;
        work->targetNo = cellNo;
        work->target = lbl_1_bss_DC[cellNo].pos;
        PSVECSubtract(&work->target, &work->screen, &sp18);
        sp18.z = 0.0f;
        if ((s32)PSVECMag(&sp18) == 0) {
            work->mode = 2;
            return;
        }
        work->mode = 1;
    }
}

void fn_1_7AFC(OMOBJ *obj) {
    M530_PLAYER_WORK *work;
    Vec sp8;
    f32 mag;

    work = obj->data;
    PSVECSubtract(&work->target, &work->screen, &sp8);
    sp8.z = 0.0f;
    mag = PSVECMag(&sp8);
    if ((s32)mag == 0) {
        work->mode = 2;
        return;
    }
    PSVECNormalize(&sp8, &sp8);
    PSVECScale(&sp8, &sp8, 7.0f);
    if (PSVECMag(&sp8) > mag) {
        work->screen = work->target;
        return;
    }
    PSVECAdd(&work->screen, &sp8, &work->screen);
}

void fn_1_7BEC(OMOBJ *obj) {
    M530_PLAYER_WORK *work;

    work = obj->data;
    if (work->state == 1) {
        fn_1_74B0(obj);
        work->mode = 0;
    }
}

void fn_1_7DD0(OMOBJ *obj) {
    M530_ITEM *item;
    Vec sp8;
    s32 cellNo;

    item = obj->data;
    if (item->alive == 0) {
        item->timer = item->timer + 1;
        if (item->timer == 0x64) {
            Hu3DModelKill(obj->mdlId[0]);
            omDelObjEx(lbl_1_bss_B8, obj);
            return;
        }
        if (item->timer > 0x3C) {
            f32 scale = (f32)(0x64 - item->timer) / 40.0f;

            omSetSca(obj, scale, scale, scale);
            if (scale <= 0.6f) {
                Hu3DModelKill(obj->mdlId[0]);
                omDelObjEx(lbl_1_bss_B8, obj);
                return;
            }
        }
        sp8 = item->pos;
        PSVECAdd(&item->pos, &item->vel, &item->pos);
        omSetTra(obj, item->pos.x, item->pos.y, item->pos.z);
        cellNo = fn_1_6518(&item->pos.x, 110.0f);
        if (cellNo != -1) {
            fn_1_1B78(&item->pos.x, 0x79A);
            item->targetNo = cellNo;
            fn_1_66D8(cellNo);
            item->timer = 0;
            obj->objFunc = fn_1_7FF4;
            return;
        }
        if (fn_1_80C4(&sp8, &item->pos) != 0) {
            item->timer = 0;
            fn_1_1B78(&item->pos.x, 0x79B);
            obj->objFunc = fn_1_7FF4;
            return;
        }
        if (fn_1_81DC(&item->pos) != 0) {
            item->timer = 0;
            fn_1_1B78(&item->pos.x, 0x79B);
            obj->objFunc = fn_1_7FF4;
            return;
        }
    }
}

void fn_1_7FF4(OMOBJ *obj) {
    M530_ITEM *item;

    item = obj->data;
    obj->work[0] = obj->work[0] + 1;
    if (obj->work[0] < 3) {
        PSVECAdd(&item->pos, &item->vel, &item->pos);
        omSetTra(obj, item->pos.x, item->pos.y, item->pos.z);
    }
    if (item->timer++ == 0) {
        Hu3DModelAttrReset(obj->mdlId[0], 0x40000002);
        return;
    }
    if (Hu3DMotionEndCheck(obj->mdlId[0]) != 0) {
        Hu3DModelKill(obj->mdlId[0]);
        omDelObjEx(lbl_1_bss_B8, obj);
    }
}

s32 fn_1_80C4(Vec *arg0, Vec *arg1) {
    Vec sp14;
    Vec sp8;

    if (fn_1_4458(&lbl_1_bss_D0[0], &lbl_1_bss_DC[0].unk4, &arg0->x, &arg1->x, &sp14.x) == 0) {
        return 0;
    }
    if (fn_1_4414(&lbl_1_bss_D0[0], &lbl_1_bss_DC[0].unk4, &arg0->x) < 0.0f) {
        return 0;
    }
    if (fn_1_4414(&lbl_1_bss_D0[0], &lbl_1_bss_DC[0].unk4, &arg1->x) > 0.0f) {
        return 0;
    }
    PSVECSubtract(&sp14, &lbl_1_data_300, &sp8);
    if (PSVECMag(&sp8) <= 940.0f) {
        return 1;
    }
    return 0;
}

s32 fn_1_81DC(Vec *arg0) {
    if (arg0->z > -600.0f) {
        return 0;
    }
    if (arg0->y > 500.0f) {
        return 0;
    }
    if (arg0->x < -150.0f || arg0->x > 150.0f) {
        return 0;
    }
    return 1;
}

void fn_1_825C(OMOBJ *obj) {
    M530_PLAYER_WORK *work = obj->data;
    f32 dx;
    f32 time;
    f32 angle;
    f32 dz;

    dx = work->world.x - work->pos.x;
    dz = work->screen.z - (1000.0f + work->pos.z);
    angle = (f32)(atan2(dx, dz) / 3.141592653589793 * 180.0);
    if (angle < 0.0f) {
        angle += 360.0f;
    }
    work->rot.y = angle;
    omSetRot(obj, work->rot.x, work->rot.y, work->rot.z);
    time = (214.0f - work->screen.y) / 164.0f;
    Hu3DSubMotionTimeSet(obj->mdlId[0], time);
}

void fn_1_8398(Vec *arg0, Vec *arg1) {
    Vec sp18;
    Vec spC;

    sp18 = *arg0;
    sp18.z = 3000.0f;
    Hu3D2Dto3D(&sp18, (s16)lbl_1_data_6C[0], &spC);
    fn_1_4458(&lbl_1_bss_D0[0], &lbl_1_bss_DC[0].unk4, (f32 *)&lbl_1_data_6C[5], &spC.x, &arg1->x);
}

void fn_1_8434(OMOBJ *obj) {
    M530_PLAYER_WORK *work = obj->data;
    HU3DMODELID modelId;

    modelId = CharModelMotListCreate(work->charNo, 2, lbl_1_data_C0, obj->mtnId);
    obj->mdlId[0] = modelId;
    work->pos = lbl_1_data_348[work->side];
    work->rot = lbl_1_data_378[work->side];
    omSetTra(obj, work->pos.x, work->pos.y, work->pos.z);
    omSetRot(obj, work->rot.x, work->rot.y, work->rot.z);
    work->unk84 = work->rot.y;
    work->unk88 = 0;
    work->targetNo = -1;
    work->mode = 0;
    work->unk74 = 0;
    work->timer2 = 0;
    Hu3DModelShadowSet(modelId);
    CharModelAttrSet(work->charNo, 0x40000001);
    CharMotionSet(work->charNo, obj->mtnId[5]);
    modelId = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_data_340[work->side], HU_MEMNUM_OVL, HEAP_MODEL));
    obj->mdlId[1] = modelId;
    work->itemHook = CharModelItemHookGet(work->charNo, 2, 0);
    Hu3DModelHookSet(obj->mdlId[0], work->itemHook, modelId);
    obj->objFunc = fn_1_85D4;
}

void fn_1_85D4(OMOBJ *obj) {
    M530_PLAYER_WORK *work;
    Vec sp8;

    work = obj->data;
    sp8 = work->pos;
    if (work->side == 0) {
        sp8.x += 12.0f;
    }
    else {
        sp8.x = sp8.x - 12.0f;
    }
    if (fn_1_8E2C(work, &sp8) != 0) {
        HuAudFXPlay(0x7A0);
        fn_1_10E8(obj, 6);
        fn_1_8848();
        omVibrate(work->playerNo, 0xC, 0xC, 0);
        obj->objFunc = fn_1_86DC;
    }
    else {
        work->pos = sp8;
    }
    omSetTra(obj, work->pos.x, work->pos.y, work->pos.z);
}

void fn_1_86DC(OMOBJ *obj) {
    M530_PLAYER_WORK *work;
    s32 done;

    work = obj->data;
    done = 0;
    if (work->side == 0) {
        work->pos.x = work->pos.x - 5.0f;
        if (work->pos.x < lbl_1_data_360[work->side].x) {
            done = 1;
        }
    }
    else {
        work->pos.x += 5.0f;
        if (work->pos.x > lbl_1_data_360[work->side].x) {
            done = 1;
        }
    }
    if (done != 0) {
        work->pos.x = lbl_1_data_360[work->side].x;
        fn_1_10E8(obj, 7);
        obj->objFunc = fn_1_87FC;
    }
    omSetTra(obj, work->pos.x, work->pos.y, work->pos.z);
}

void fn_1_87FC(OMOBJ *obj) {
    if (MgSeqModeGet() == 3) {
        fn_1_10E8(obj, 3);
        obj->objFunc = fn_1_8958;
    }
}





























