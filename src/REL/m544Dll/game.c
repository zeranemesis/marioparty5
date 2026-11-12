#include "types.h"
#include "string.h"
#include "stdio.h"
#include "game/audio.h"
#include "game/charman.h"
#include "game/data.h"
#include "game/flag.h"
#include "game/frand.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/object.h"
#include "game/mg/seqman.h"
#include "game/mg/timer.h"
#include "REL/m544Dll.h"

/* m544.c */
s16 fn_1_18E8(f32 *arg0, f32 farg0, f32 farg1, f32 farg2);
MGTIMER *fn_1_1A1C(s32 type, s32 maxSec, s32 endSec, f32 x, f32 y);
void fn_1_1A90(MGTIMER *timer);
void fn_1_1AE4(MGTIMER *timer);
void fn_1_9C0(s16 arg0);
void fn_1_1168(void);
void fn_1_1570(void);
void fn_1_2330(void);
void fn_1_2834(HuVecF *pos, s16 arg1);
void fn_1_34D0(void);
void fn_1_35DC(s16 arg0);
void fn_1_3EF8(void);
void fn_1_3F28(void);
void fn_1_3F58(s16 arg0, s16 arg1);
void fn_1_4020(s16 arg0);
void fn_1_4098(s16 arg0, s16 arg1, s16 arg2);
void fn_1_4120(s16 arg0, s16 arg1);
void fn_1_425C(s16 arg0);
void fn_1_4308(void);
/* stage.c */
void fn_1_B36C(void);

/* forward declarations (address order) */
s32 fn_1_5EFC(s16 arg0);
s32 fn_1_614C(s16 arg0);
s32 fn_1_6274(s16 arg0);
s32 fn_1_65EC(void);
s32 fn_1_6B64(void);
void fn_1_6E9C(s16 arg0);
s32 fn_1_7F30(void);
s16 fn_1_753C(void);
s16 fn_1_817C(void);
void fn_1_5C48(void);
s16 fn_1_5E38(void);
void fn_1_8710(s32 arg0, s16 arg1);
void fn_1_8E98(s32 arg0, s32 arg1);
void fn_1_91B4(s32 arg0, s32 arg1);
s32 fn_1_9364(s32 arg0, s32 arg1);
void fn_1_95D8(s32 arg0, s32 arg1);
void fn_1_9798(s16 arg0);
s16 fn_1_9C84(s16 arg0);

/* unit-owned .bss, declared in reverse address order */
M544State *lbl_1_bss_914;
M544Rnd lbl_1_bss_8D8[10];
M544Item lbl_1_bss_828[4];
s16 lbl_1_bss_826;
s16 lbl_1_bss_824;
M544PtclWork *lbl_1_bss_820;

/* unit-owned .data */
s16 lbl_1_data_3EC[4][5] = {
    { 0x46, 0x5A, 0x55, 0x46, 0x5A },
    { 0x32, 0x46, 0x5A, 0x50, 0x5F },
    { 0x23, 0x2D, 0x5F, 0x5A, 0x64 },
    { 0x1E, 0x23, 0x64, 0x64, 0x64 },
};

s32 fn_1_4328(M544State *state)
{
    s16 tbl[10] = { 5, 6, 7, 8, 9, 9, 9, 9, 9, 9 };
    s16 i;
    s16 k;
    s16 j;
    s32 ret = 1;
    s16 r;

    memset(&lbl_1_bss_D0, 0, sizeof(M544Game));
    memset(lbl_1_bss_80, 0, sizeof(lbl_1_bss_80));
    memset(&lbl_1_bss_C, 0, sizeof(M544Stage));
    for (i = 0; i < 6; i++) {
        lbl_1_bss_D0.fxFlag[i] = -1;
    }
    lbl_1_bss_914 = state;
    for (i = 0; i < 2; i++) {
        lbl_1_bss_80[i].scale.x = 1.0f;
        lbl_1_bss_80[i].scale.y = 1.0f;
        lbl_1_bss_80[i].scale.z = 1.0f;
        lbl_1_bss_80[i].hook = CharModelItemHookGet(lbl_1_bss_914->chars[i].charNo, 1, 0);
    }
    sprintf(lbl_1_bss_C.name, "%s", "g003m1-itemhook_R");
    lbl_1_bss_C.tpLvl = 1.0f;
    for (i = 0; i < 10; i++) {
        lbl_1_bss_8D8[i].a = (frandmod(0x2D) + 5) * 6;
        lbl_1_bss_8D8[i].b = (frandmod(0xF) + 5) * 6;
        r = frandmod(0xB);
        if (i >= 10 || r <= tbl[i]) {
            lbl_1_bss_8D8[i].c = frandmod(6);
        } else {
            lbl_1_bss_8D8[i].c = frandmod(4) + 6;
        }
    }
    for (i = 0; i < 2; i++) {
        if (lbl_1_bss_914->chars[i].cpuLevel != 0) {
            fn_1_9798(i);
        }
    }
    i = 0;
    j = 0;
    k = 0;
    for (; i < 10; i++, j++, k++) {
        lbl_1_bss_914->models[k].id = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_bss_914->dataNums[j], 0x10000000, 2));
        Hu3DModelAttrSet(lbl_1_bss_914->models[k].id, 1);
        Hu3DModelCameraSet(lbl_1_bss_914->models[k].id, 1);
        Hu3DModelLayerSet(lbl_1_bss_914->models[k].id, 2);
        Hu3DModelShadowSet(lbl_1_bss_914->models[k].id);
        lbl_1_bss_914->models[k].pos.x = 0.0f;
        lbl_1_bss_914->models[k].pos.y = 0.0f;
        lbl_1_bss_914->models[k].pos.z = 0.0f;
        lbl_1_bss_914->models[k].rot.x = 0.0f;
        lbl_1_bss_914->models[k].rot.y = 0.0f;
        lbl_1_bss_914->models[k].rot.z = -50.0f;
    }
    Hu3DModelAttrReset(lbl_1_bss_914->models[lbl_1_bss_8D8[0].c].id, 1);
    Hu3DModelHookSet(lbl_1_bss_914->models[0x22].id, lbl_1_bss_C.name, lbl_1_bss_914->models[lbl_1_bss_8D8[0].c].id);
    i = 0;
    k = 0xA;
    for (; i < 2; i++, k++) {
        lbl_1_bss_914->models[k].id = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_bss_914->dataNums[10], 0x10000000, 2));
        Hu3DModelAttrSet(lbl_1_bss_914->models[k].id, 1);
        Hu3DModelCameraSet(lbl_1_bss_914->models[k].id, 1);
        Hu3DModelLayerSet(lbl_1_bss_914->models[k].id, 0);
        Hu3DModelShadowSet(lbl_1_bss_914->models[k].id);
        Hu3DModelAttrReset(lbl_1_bss_914->models[k].id, 1);
        lbl_1_bss_914->models[k].pos.x = 0.0f;
        lbl_1_bss_914->models[k].pos.y = 0.0f;
        lbl_1_bss_914->models[k].pos.z = 0.0f;
        lbl_1_bss_914->models[k].rot.x = 0.0f;
        lbl_1_bss_914->models[k].rot.y = 0.0f;
        lbl_1_bss_914->models[k].rot.z = 0.0f;
        Hu3DModelHookSet(lbl_1_bss_914->models[i + 0x20].id, lbl_1_bss_80[i].hook, lbl_1_bss_914->models[k].id);
    }
    i = 0;
    for (; i < 4; i++) {
        lbl_1_bss_914->models[i + 0xC].id = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_bss_914->dataNums[11], 0x10000000, 2));
        Hu3DModelAttrSet(lbl_1_bss_914->models[i + 0xC].id, 1);
        Hu3DModelCameraSet(lbl_1_bss_914->models[i + 0xC].id, 1);
        Hu3DModelLayerSet(lbl_1_bss_914->models[i + 0xC].id, 0);
        Hu3DMotionSpeedSet(lbl_1_bss_914->models[i + 0xC].id, 0.0f);
        Hu3DMotionTimeSet(lbl_1_bss_914->models[i + 0xC].id, 0.0f);
    }
    k = 0x10;
    for (; k <= 0x13; k++) {
        lbl_1_bss_914->models[k].id = Hu3DModelCreate(HuDataSelHeapReadNum(lbl_1_bss_914->dataNums[12], 0x10000000, 2));
        Hu3DModelAttrSet(lbl_1_bss_914->models[k].id, 1);
        Hu3DModelCameraSet(lbl_1_bss_914->models[k].id, 1);
        Hu3DModelLayerSet(lbl_1_bss_914->models[k].id, 0);
        Hu3DMotionSpeedSet(lbl_1_bss_914->models[i + 0xC].id, 0.0f);
        Hu3DMotionTimeSet(lbl_1_bss_914->models[i + 0xC].id, 0.0f);
    }
    fn_1_2330();
    fn_1_34D0();
    fn_1_5C48();
    lbl_1_bss_D0.timer = fn_1_1A1C(0, 5, 0, 292.0f, 64.0f);
    return ret;
}

s32 fn_1_4E08(void)
{
    s32 ret = 0;
    fn_1_7F30();
    fn_1_5C48();
    return ret;
}

s32 fn_1_4E3C(void)
{
    s32 ret = 1;
    if (lbl_1_bss_D0.flag2E == 0) {
        fn_1_3F28();
        lbl_1_bss_D0.flag2E = 1;
    }
    lbl_1_bss_D0.phase = 0;
    lbl_1_bss_D0.timer6 = 0;
    fn_1_5C48();
    return ret;
}

s32 fn_1_4EB4(void)
{
    s16 plr;
    s32 t;
    s32 ret;

    ret = 0;
    switch (lbl_1_bss_D0.phase) {
    case 0:
        t = fn_1_6B64();
        if ((s16)(t + fn_1_65EC()) != 0) {
            fn_1_1AE4(lbl_1_bss_D0.timer);
            lbl_1_bss_D0.phase++;
        }
        break;
    case 1:
        if (fn_1_5E38() != 0) {
            lbl_1_bss_D0.phase++;
        }
        break;
    case 2:
        lbl_1_bss_D0.phase++;
        break;
    case 3:
        if (lbl_1_bss_D0.plrMask != 0) {
            MgSeqWinnerSet(lbl_1_bss_914->chars[lbl_1_bss_D0.plrMask & 1].charNo, -1, -1, -1);
            plr = lbl_1_bss_914->chars[lbl_1_bss_D0.plrMask & 1].playerNo;
            if (_CheckFlag(0x1000F) == 0) {
                GwPlayer[plr].mgCoinBonus = 0xA;
            }
        } else {
            MgSeqWinnerSet(-1, -1, -1, -1);
        }
        lbl_1_bss_D0.flag2E = 0;
        ret = 1;
        break;
    }
    fn_1_5C48();
    return ret;
}

s32 fn_1_5098(void)
{
    if (lbl_1_bss_D0.flag2E == 0) {
        fn_1_3F58(1, 0x64);
        lbl_1_bss_D0.flag2E = 1;
    }
    lbl_1_bss_D0.timer6 = 0;
    lbl_1_bss_D0.phase = 0;
    fn_1_753C();
    fn_1_5C48();
    return 0;
}

s16 fn_1_511C(void)
{
    s16 ret;

    ret = fn_1_817C();
    fn_1_753C();
    fn_1_5C48();
    return ret;
}

s32 fn_1_5158(void)
{
    M544Slot *slot;
    M544Slot *slot2;

    if (lbl_1_bss_914->field_178 == 0) {
        if (lbl_1_bss_D0.plrMask != 0) {
            CharModelVoicePanSet(lbl_1_bss_914->chars[lbl_1_bss_D0.plrMask & 1].charNo, 0x40);
            CharModelFxFlagSet(lbl_1_bss_914->chars[lbl_1_bss_D0.plrMask & 1].charNo, 1);
            slot = &lbl_1_bss_914->slots[lbl_1_bss_D0.plrMask & 1];
            slot->motion = 5;
            slot->speed = 1.0f;
            slot->time = 0.0f;
            slot->attrSet = 0;
            slot->attrReset = 0x40000001;
            slot2 = &lbl_1_bss_914->slots[(lbl_1_bss_D0.plrMask & 2) >> 1];
            slot2->motion = 6;
            slot2->speed = 1.0f;
            slot2->time = 0.0f;
            slot2->attrSet = 0;
            slot2->attrReset = 0x40000001;
        } else {
            slot = &lbl_1_bss_914->slots[0];
            slot->motion = 6;
            slot->speed = 1.0f;
            slot->time = 0.0f;
            slot->attrSet = 0;
            slot->attrReset = 0x40000001;
            slot2 = &lbl_1_bss_914->slots[1];
            slot2->motion = 6;
            slot2->speed = 1.0f;
            slot2->time = 0.0f;
            slot2->attrSet = 0;
            slot2->attrReset = 0x40000001;
            CharModelVoicePanSet(lbl_1_bss_914->chars[0].charNo, 0x30);
            CharModelVoicePanSet(lbl_1_bss_914->chars[1].charNo, 0x50);
            CharLoseVoicePlay(lbl_1_bss_914->chars[0].charNo, lbl_1_bss_914->chars[1].charNo, -1, -1);
        }
        lbl_1_bss_914->field_178 = 1;
    }
    if (lbl_1_bss_D0.plrMask == 0) {
        fn_1_18E8(&lbl_1_bss_914->models[0x20].rot.y, 10.0f, 90.0f, 0.0f);
        fn_1_18E8(&lbl_1_bss_914->models[0x21].rot.y, 10.0f, -90.0f, 0.0f);
    } else if ((lbl_1_bss_D0.plrMask & 1) != 0) {
        fn_1_18E8(&lbl_1_bss_914->models[0x21].rot.y, 10.0f, -90.0f, -30.0f);
    } else {
        fn_1_18E8(&lbl_1_bss_914->models[0x20].rot.y, 10.0f, 90.0f, 29.0f);
    }
    fn_1_5C48();
    return 0;
}

s32 fn_1_5514(void)
{
    s32 ret = 1;
    fn_1_5C48();
    return ret;
}

s32 fn_1_5544(void)
{
    s32 ret = 1;
    fn_1_4308();
    return ret;
}

void fn_1_5574(void)
{
    M544Slot *slot;
    HU3DMODEL *model;

    slot = &lbl_1_bss_914->slots[2];
    Hu3DModelObjMtxGet(lbl_1_bss_914->models[0x22].id, lbl_1_bss_C.name, lbl_1_bss_C.mtx);
    model = &Hu3DData[lbl_1_bss_914->models[lbl_1_bss_8D8[lbl_1_bss_C.idx].c].id];
    if (slot->motion == 0) {
        model->rot.z = -50.0f;
        if (slot->motion == slot->curMotion && slot->time == 1.0f) {
            Hu3DModelHookSet(lbl_1_bss_914->models[0x22].id, lbl_1_bss_C.name,
                             lbl_1_bss_914->models[lbl_1_bss_8D8[lbl_1_bss_C.idx].c].id);
            lbl_1_bss_C.mode = 0;
        }
    }
    if (slot->motion == 1) {
        if (slot->motion != slot->curMotion) {
            Hu3DModelHookObjReset(lbl_1_bss_914->models[0x22].id, lbl_1_bss_C.name);
        }
        lbl_1_bss_C.mode = 1;
    }
    if (lbl_1_bss_C.flag16 == 1) {
        lbl_1_bss_C.mode = 2;
    }
    switch (lbl_1_bss_C.mode) {
    case 0:
        PSMTXIdentity(model->mtx);
        lbl_1_bss_914->models[lbl_1_bss_8D8[lbl_1_bss_C.idx].c].pos.x = 0.0f;
        lbl_1_bss_914->models[lbl_1_bss_8D8[lbl_1_bss_C.idx].c].pos.y = 0.0f;
        lbl_1_bss_914->models[lbl_1_bss_8D8[lbl_1_bss_C.idx].c].pos.z = 0.0f;
        lbl_1_bss_914->models[lbl_1_bss_8D8[lbl_1_bss_C.idx].c].rot.x = 0.0f;
        lbl_1_bss_914->models[lbl_1_bss_8D8[lbl_1_bss_C.idx].c].rot.y = 0.0f;
        lbl_1_bss_914->models[lbl_1_bss_8D8[lbl_1_bss_C.idx].c].rot.z = -50.0f;
        return;
    case 1:
        PSMTXIdentity(model->mtx);
        lbl_1_bss_914->models[lbl_1_bss_8D8[lbl_1_bss_C.idx].c].pos.x = lbl_1_bss_C.mtx[0][3];
        lbl_1_bss_914->models[lbl_1_bss_8D8[lbl_1_bss_C.idx].c].pos.y = lbl_1_bss_C.mtx[1][3];
        lbl_1_bss_914->models[lbl_1_bss_8D8[lbl_1_bss_C.idx].c].pos.z = lbl_1_bss_C.mtx[2][3];
        lbl_1_bss_914->models[lbl_1_bss_8D8[lbl_1_bss_C.idx].c].rot.x = -7.5f * (1.0f + slot->time);
        lbl_1_bss_914->models[lbl_1_bss_8D8[lbl_1_bss_C.idx].c].rot.y = 0.0f;
        lbl_1_bss_914->models[lbl_1_bss_8D8[lbl_1_bss_C.idx].c].rot.z = 0.0f;
        return;
    }
}

void fn_1_5B0C(void)
{
    s16 i;

    for (i = 0; i < 0x23; i++) {
        Hu3DModelPosSet(lbl_1_bss_914->models[i].id, lbl_1_bss_914->models[i].pos.x, lbl_1_bss_914->models[i].pos.y,
                        lbl_1_bss_914->models[i].pos.z);
        Hu3DModelRotSet(lbl_1_bss_914->models[i].id, lbl_1_bss_914->models[i].rot.x, lbl_1_bss_914->models[i].rot.y,
                        lbl_1_bss_914->models[i].rot.z);
    }
}

void fn_1_5C48(void)
{
    s16 i;

    fn_1_5574();
    for (i = 0; i < 0x23; i++) {
        Hu3DModelPosSet(lbl_1_bss_914->models[i].id, lbl_1_bss_914->models[i].pos.x, lbl_1_bss_914->models[i].pos.y,
                        lbl_1_bss_914->models[i].pos.z);
        Hu3DModelRotSet(lbl_1_bss_914->models[i].id, lbl_1_bss_914->models[i].rot.x, lbl_1_bss_914->models[i].rot.y,
                        lbl_1_bss_914->models[i].rot.z);
    }
    fn_1_9C0(0);
    lbl_1_bss_4->rotX = lbl_1_bss_8->rotX;
    lbl_1_bss_4->rotY = lbl_1_bss_8->rotY;
    lbl_1_bss_4->dist = lbl_1_bss_8->zoom;
    lbl_1_bss_4->target.x = lbl_1_bss_8->center.x;
    lbl_1_bss_4->target.z = lbl_1_bss_8->center.y;
    fn_1_1168();
    fn_1_1570();
}

s16 fn_1_5E38(void)
{
    s16 ret;
    s16 i;

    ret = 1;
    for (i = 0; i < 2; i++) {
        switch (lbl_1_bss_80[i].mode) {
        case 0:
            break;
        case 1:
            ret *= fn_1_5EFC(i);
            break;
        case 2:
            ret *= fn_1_614C(i);
            break;
        case 3:
            ret *= fn_1_6274(i);
            break;
        }
    }
    return ret;
}

s32 fn_1_5EFC(s16 arg0)
{
    M544Slot *slot;
    s32 ret;

    slot = &lbl_1_bss_914->slots[arg0];
    ret = 0;
    switch (lbl_1_bss_80[arg0].state) {
    case 0:
        slot->motion = 2;
        slot->speed = 1.0f;
        slot->time = 0.0f;
        slot->attrSet = 0;
        slot->attrReset = 0x40000001;
        lbl_1_bss_80[arg0].state++;
        break;
    case 1:
        if (slot->endFlag != 0) {
            if (lbl_1_bss_80[arg0].count >= 1) {
                lbl_1_bss_80[arg0].state++;
            } else {
                fn_1_8710(arg0, lbl_1_bss_80[arg0].count);
                slot->motion = 3;
                slot->speed = 1.0f;
                slot->time = 0.0f;
                slot->attrSet = 0;
                slot->attrReset = 0;
                fn_1_4098(2, 0x7F, (arg0 << 5) + 0x30);
            }
            lbl_1_bss_80[arg0].count++;
        }
        break;
    case 2:
        if (lbl_1_bss_80[arg0].timer++ > 0x3C) {
            lbl_1_bss_80[arg0].timer = 0;
            lbl_1_bss_80[arg0].state++;
        }
        break;
    case 3:
        ret = 1;
        break;
    }
    return ret;
}

s32 fn_1_614C(s16 arg0)
{
    M544Slot *slot;
    s16 i;
    s32 ret;

    slot = &lbl_1_bss_914->slots[arg0];
    ret = 0;
    if (lbl_1_bss_80[arg0].flagA == 0) {
        for (i = 0x10; i <= 0x13; i++) {
            if ((Hu3DModelAttrGet(lbl_1_bss_914->models[i].id) & 1) == 0) {
                fn_1_4098(3, 0x7F, (arg0 << 5) + 0x30);
                lbl_1_bss_80[arg0].flagA = 1;
            }
        }
    }
    if (lbl_1_bss_80[arg0].timer++ > 0x3C) {
        ret = 1;
    }
    return ret;
}

s32 fn_1_6274(s16 arg0)
{
    M544Slot *slot;
    s32 ret;

    slot = &lbl_1_bss_914->slots[arg0];
    ret = 0;
    switch (lbl_1_bss_80[arg0].state) {
    case 0:
        slot->motion = 2;
        slot->speed = 1.0f;
        slot->time = 0.0f;
        slot->attrSet = 0;
        slot->attrReset = 0x40000001;
        lbl_1_bss_80[arg0].state++;
        break;
    case 1:
        if (slot->endFlag != 0) {
            lbl_1_bss_80[arg0].state++;
        }
        break;
    case 2:
        if (lbl_1_bss_80[arg0].timer++ > 0x3C) {
            fn_1_35DC(arg0);
            lbl_1_bss_80[arg0].timer = 0;
            lbl_1_bss_80[arg0].state++;
        }
        break;
    case 3:
        if (lbl_1_bss_D0.flag2C == 0) {
            fn_1_35DC(arg0);
        }
        if (lbl_1_bss_80[arg0].timer++ > 0xB4 || lbl_1_bss_D0.flag2C != 0) {
            lbl_1_bss_80[arg0].timer = 0;
            lbl_1_bss_80[arg0].state++;
        }
        break;
    case 4:
        if (lbl_1_bss_80[arg0].timer++ > 0x3C) {
            fn_1_4098(4, 0x7F, (arg0 << 5) + 0x30);
            lbl_1_bss_80[arg0].timer = 0;
            lbl_1_bss_80[arg0].state++;
        }
        break;
    case 5:
        if (lbl_1_bss_80[0].mode == 3 && lbl_1_bss_80[1].mode == 3) {
            if (arg0 == 0) {
                fn_1_95D8(1, 2);
            }
        } else if (arg0 != 0) {
            fn_1_95D8(-1, 1);
        } else {
            fn_1_95D8(1, -1);
        }
        lbl_1_bss_80[arg0].state++;
        break;
    case 6:
        if (lbl_1_bss_80[arg0].timer++ > 0x3C) {
            ret = 1;
        }
        break;
    }
    return ret;
}

s32 fn_1_65EC(void)
{
    s16 code;
    s16 i;
    s16 t;
    s32 ret;

    ret = 0;
    for (i = 1; i >= 0; i--) {
        code = 0;
        if (lbl_1_bss_914->chars[i].cpuLevel == 0) {
            if ((lbl_1_bss_914->chars[i].btnDown & 0x100) != 0) {
                code = 0x80;
            }
            if ((lbl_1_bss_914->chars[i].btnDown & 0x200) != 0) {
                code += 0x81;
            }
            if ((lbl_1_bss_914->chars[i].btnDown & 0x400) != 0) {
                code += 0x82;
            }
            if ((lbl_1_bss_914->chars[i].btnDown & 0x800) != 0) {
                code += 0x83;
            }
            if ((lbl_1_bss_914->chars[i].btnDown & 0x40) != 0) {
                code += 0x84;
            }
            if ((lbl_1_bss_914->chars[i].btnDown & 0x20) != 0) {
                code += 0x85;
            }
        } else {
            code = fn_1_9C84(i);
        }
        if (code != 0) {
            ret = 1;
            if (lbl_1_bss_D0.flag28 != 0) {
                s16 sel = code & 0x7F;
                if (sel == lbl_1_bss_8D8[lbl_1_bss_C.idx].c) {
                    if ((lbl_1_bss_D0.plrMask & 1) != 0) {
                        lbl_1_bss_D0.plrMask = 0;
                        t = frandmod(2);
                        lbl_1_bss_D0.plrMask = t | ((t ^ 1) * 2);
                        lbl_1_bss_80[lbl_1_bss_D0.plrMask & 1].mode = 1;
                        lbl_1_bss_80[(lbl_1_bss_D0.plrMask & 2) >> 1].mode = 2;
                    } else {
                        lbl_1_bss_80[i].mode = 1;
                        lbl_1_bss_80[i ^ 1].mode = 2;
                        lbl_1_bss_D0.plrMask |= i;
                        lbl_1_bss_D0.plrMask |= (i ^ 1) * 2;
                    }
                } else if (sel != lbl_1_bss_8D8[lbl_1_bss_C.idx].c) {
                    if (lbl_1_bss_80[i].mode == 0) {
                        lbl_1_bss_80[i].mode = 3;
                    }
                    if ((lbl_1_bss_D0.plrMask & 2) != 0) {
                        lbl_1_bss_D0.plrMask = 0;
                        t = frandmod(2);
                        lbl_1_bss_D0.plrMask = t | ((t ^ 1) * 2);
                        lbl_1_bss_80[lbl_1_bss_D0.plrMask & 1].mode = 0;
                        lbl_1_bss_80[(lbl_1_bss_D0.plrMask & 2) >> 1].mode = 3;
                    } else {
                        lbl_1_bss_D0.plrMask |= i * 2;
                        lbl_1_bss_D0.plrMask |= i ^ 1;
                    }
                }
            } else if ((lbl_1_bss_D0.plrMask & 2) != 0) {
                lbl_1_bss_D0.plrMask = 0;
                t = frandmod(2);
                lbl_1_bss_D0.plrMask = t | ((t ^ 1) * 2);
                lbl_1_bss_80[lbl_1_bss_D0.plrMask & 1].mode = 0;
                lbl_1_bss_80[(lbl_1_bss_D0.plrMask & 2) >> 1].mode = 3;
            } else {
                lbl_1_bss_80[i].mode = 3;
                lbl_1_bss_80[i ^ 1].mode = 0;
                lbl_1_bss_D0.plrMask |= i * 2;
                lbl_1_bss_D0.plrMask |= i ^ 1;
            }
        }
    }
    return ret;
}

s32 fn_1_6B64(void)
{
    M544Slot *slot;
    s32 ret;

    slot = &lbl_1_bss_914->slots[2];
    ret = 0;
    switch (lbl_1_bss_C.phase) {
    case 0:
        if (lbl_1_bss_C.timerC++ >= lbl_1_bss_8D8[lbl_1_bss_C.idx].a) {
            if (lbl_1_bss_8D8[lbl_1_bss_C.idx].c < 6) {
                lbl_1_bss_D0.flag28 = 1;
            }
            slot->motion = 1;
            slot->speed = 1.0f;
            slot->time = 0.0f;
            slot->attrSet = 0;
            slot->attrReset = 0x40000005;
            fn_1_4020(1);
            lbl_1_bss_C.timerC = 0;
            lbl_1_bss_C.timerE = 0;
            lbl_1_bss_C.phase++;
        }
        break;
    case 1:
        if (lbl_1_bss_D0.flag28 != 0) {
            if (lbl_1_bss_C.timerE++ >= 0x258) {
                lbl_1_bss_80[0].mode = 0;
                lbl_1_bss_80[1].mode = 0;
                lbl_1_bss_D0.plrMask = 0;
                lbl_1_bss_C.phase = 3;
                ret = 1;
            }
            if (lbl_1_bss_C.timerE == 0x12C) {
                fn_1_1A90(lbl_1_bss_D0.timer);
            }
        } else {
            if (lbl_1_bss_C.timerE++ >= lbl_1_bss_8D8[lbl_1_bss_C.idx].b) {
                slot->motion = 1;
                slot->speed = 1.0f;
                slot->time = 11.0f;
                slot->attrSet = 0x40000004;
                slot->attrReset = 0;
                lbl_1_bss_C.phase++;
            }
        }
        break;
    case 2:
        if (slot->time < 1.0f) {
            lbl_1_bss_C.idx++;
            fn_1_6E9C(lbl_1_bss_C.idx);
            slot->motion = 0;
            slot->speed = 1.0f;
            slot->time = 0.0f;
            slot->attrSet = 0x40000001;
            slot->attrReset = 0x40000004;
            lbl_1_bss_C.phase = 0;
        }
        break;
    }
    return ret;
}

void fn_1_6E9C(s16 arg0)
{
    Hu3DModelAttrSet(lbl_1_bss_914->models[lbl_1_bss_8D8[arg0 - 1].c].id, 1);
    Hu3DModelAttrReset(lbl_1_bss_914->models[lbl_1_bss_8D8[arg0].c].id, 1);
}

s32 fn_1_6F38(s16 arg0, s16 arg1)
{
    s32 ret;
    s16 mi;

    ret = 0;
    mi = arg0 + 0xA;
    if (arg1 == 0) {
        switch (lbl_1_bss_80[arg0].stage) {
        case 0:
            Hu3DModelObjPosGet(lbl_1_bss_914->models[arg0 + 0x20].id, lbl_1_bss_80[arg0].hook,
                               &lbl_1_bss_914->models[mi].pos);
            lbl_1_bss_914->models[mi].rot.y = 90.0f - 180.0f * (f32)arg0;
            Hu3DModelHookObjReset(lbl_1_bss_914->models[arg0 + 0x20].id, lbl_1_bss_80[arg0].hook);
            lbl_1_bss_80[arg0].stage++;
            break;
        case 1:
            lbl_1_bss_80[arg0].scale.x -= 0.05f;
            lbl_1_bss_80[arg0].scale.y -= 0.05f;
            lbl_1_bss_80[arg0].scale.z -= 0.05f;
            lbl_1_bss_914->models[mi].rot.x += 20.0f;
            Hu3DModelScaleSetV(lbl_1_bss_914->models[mi].id, &lbl_1_bss_80[arg0].scale);
            if (lbl_1_bss_80[arg0].scale.x <= 0.1f) {
                Hu3DModelAttrSet(lbl_1_bss_914->models[mi].id, 1);
                lbl_1_bss_80[arg0].stage++;
            }
            break;
        case 2:
            ret = 1;
            break;
        }
    } else {
        Hu3DModelAttrSet(lbl_1_bss_914->models[mi].id, 1);
        ret = 1;
    }
    return ret;
}

s32 fn_1_7288(void)
{
    HuVecF sp8;
    s32 ret;

    ret = 0;
    switch (lbl_1_bss_C.phase2) {
    case 0:
        Hu3DModelObjPosGet(lbl_1_bss_914->models[0x22].id, lbl_1_bss_C.name, &sp8);
        if (lbl_1_bss_914->slots[2].motion == 0) {
            fn_1_2834(&sp8, 1);
        } else {
            Hu3DModelHookObjReset(lbl_1_bss_914->models[0x22].id, lbl_1_bss_C.name);
            lbl_1_bss_914->models[lbl_1_bss_8D8[lbl_1_bss_C.idx].c].pos = sp8;
            lbl_1_bss_914->models[lbl_1_bss_8D8[lbl_1_bss_C.idx].c].rot.x = -90.0f;
            lbl_1_bss_914->models[lbl_1_bss_8D8[lbl_1_bss_C.idx].c].rot.z = 0.0f;
            fn_1_2834(&sp8, 0);
        }
        lbl_1_bss_C.phase2++;
        break;
    case 1:
        Hu3DModelTPLvlSet(lbl_1_bss_914->models[lbl_1_bss_8D8[lbl_1_bss_C.idx].c].id, lbl_1_bss_C.tpLvl);
        lbl_1_bss_C.tpLvl = lbl_1_bss_C.tpLvl - 0.05f;
        if (lbl_1_bss_C.tpLvl <= 0.0f) {
            Hu3DModelAttrSet(lbl_1_bss_914->models[lbl_1_bss_8D8[lbl_1_bss_C.idx].c].id, 1);
            lbl_1_bss_C.phase2++;
        }
        break;
    case 2:
        ret = 1;
        break;
    }
    return ret;
}

s16 fn_1_753C(void)
{
    M544Slot *slot;
    s16 i;
    s16 ret;
    s32 sub;
    s32 r;

    slot = &lbl_1_bss_914->slots[0];
    ret = 0;
    switch (lbl_1_bss_D0.phase2) {
    case 0:
        slot[0].shiftState = 1;
        slot[0].shiftMotion = 0;
        slot[0].shiftStart = 0.0f;
        slot[0].shiftEnd = 20.0f;
        slot[0].shiftAttr = 0x40000001;
        slot[0].attrReset = 0;
        slot[1].shiftState = 1;
        slot[1].shiftMotion = 0;
        slot[1].shiftStart = 0.0f;
        slot[1].shiftEnd = 20.0f;
        slot[1].shiftAttr = 0x40000001;
        slot[1].attrReset = 0;
        slot[2].shiftState = 1;
        slot[2].shiftMotion = 0;
        slot[2].shiftStart = 0.0f;
        slot[2].shiftEnd = 20.0f;
        slot[2].shiftAttr = 0x40000001;
        slot[2].attrReset = 0;
        lbl_1_bss_C.flag16 = 1;
        fn_1_7288();
        lbl_1_bss_D0.phase2++;
        break;
    case 1:
        sub = fn_1_7288();
        ret = sub;
        for (i = 0; i < 2; i++) {
            if (lbl_1_bss_80[i].mode == 0 || lbl_1_bss_80[i].mode == 1) {
                r = fn_1_6F38(i, 0);
                ret *= r;
            }
        }
        if (ret != 0) {
            lbl_1_bss_D0.phase2++;
        }
        break;
    case 2:
        ret = 1;
        break;
    }
    return ret;
}

s32 fn_1_7F30(void)
{
    f32 spd;
    s32 ret = 0;
    s32 t = 0;

    switch (lbl_1_bss_D0.phase) {
    case 0:
        fn_1_3EF8();
        fn_1_B36C();
        lbl_1_bss_D0.phase++;
        break;
    case 1:
        if (lbl_1_bss_D0.timer6++ > 0xF0) {
            lbl_1_bss_D0.phase++;
        }
        break;
    case 2:
        spd = 60.0f;
        t += fn_1_18E8(&lbl_1_bss_8->center.x, spd, -12.0f, -211.0f);
        t += fn_1_18E8(&lbl_1_bss_8->center.y, spd, 208.0f, 218.0f);
        t += fn_1_18E8(&lbl_1_bss_8->center.z, spd, -120.0f, 0.0f);
        t += fn_1_18E8(&lbl_1_bss_8->rotX, spd, -11.0f, -11.0f);
        t += fn_1_18E8(&lbl_1_bss_8->rotY, spd, 0.0f, -29.0f);
        if ((s16)(t + fn_1_18E8(&lbl_1_bss_8->zoom, spd, 141.0f, 310.0f)) == 0) {
            lbl_1_bss_D0.phase++;
        }
        break;
    }
    return ret;
}

s16 fn_1_817C(void)
{
    M544Slot *slot;
    s16 ret;
    s32 t;

    slot = &lbl_1_bss_914->slots[2];
    ret = 0;
    switch (lbl_1_bss_D0.phase) {
    case 0:
        if (lbl_1_bss_D0.timer6++ > 0x3C) {
            if (lbl_1_bss_D0.plrMask != 0) {
                slot->shiftState = 1;
                slot->shiftMotion = (lbl_1_bss_D0.plrMask & 1) + 2;
                slot->shiftStart = 0.0f;
                slot->shiftEnd = 14.0f;
                slot->shiftAttr = 0;
                slot->attrReset = 0x40000001;
                fn_1_4020(5);
            }
            lbl_1_bss_D0.timer6 = 0;
            lbl_1_bss_D0.phase++;
        }
        break;
    case 1:
        if (lbl_1_bss_D0.plrMask != 0) {
            if (slot->shiftState == 3) {
                lbl_1_bss_D0.phase++;
            }
        } else {
            lbl_1_bss_D0.phase++;
        }
        break;
    case 2:
        if (lbl_1_bss_D0.plrMask != 0) {
            if (lbl_1_bss_D0.timer6++ >= 0x1E) {
                lbl_1_bss_D0.phase++;
            }
        } else {
            lbl_1_bss_D0.phase++;
        }
        break;
    case 3:
        if (lbl_1_bss_D0.plrMask == 0) {
            t = fn_1_18E8(&lbl_1_bss_8->center.x, 30.0f, -211.0f, 0.0f);
            t += fn_1_18E8(&lbl_1_bss_8->rotY, 30.0f, -29.0f, 0.0f);
            if ((s16)(t + fn_1_18E8(&lbl_1_bss_8->zoom, 30.0f, 310.0f, 560.0f)) == 0) {
                lbl_1_bss_D0.phase++;
            }
        } else if ((lbl_1_bss_D0.plrMask & 1) == 0) {
            t = fn_1_18E8(&lbl_1_bss_8->center.x, 30.0f, -211.0f, -217.0f);
            t += fn_1_18E8(&lbl_1_bss_8->center.z, 30.0f, 0.0f, -120.0f);
            t += fn_1_18E8(&lbl_1_bss_8->rotY, 30.0f, -29.0f, 32.0f);
            if ((s16)(t + fn_1_18E8(&lbl_1_bss_8->zoom, 30.0f, 310.0f, 304.0f)) == 0) {
                lbl_1_bss_D0.phase++;
            }
        } else {
            t = fn_1_18E8(&lbl_1_bss_8->center.x, 30.0f, -211.0f, 229.0f);
            t += fn_1_18E8(&lbl_1_bss_8->center.z, 30.0f, 0.0f, -115.0f);
            t += fn_1_18E8(&lbl_1_bss_8->rotY, 30.0f, -29.0f, -28.0f);
            if ((s16)(t + fn_1_18E8(&lbl_1_bss_8->zoom, 30.0f, 310.0f, 314.0f)) == 0) {
                lbl_1_bss_D0.phase++;
            }
        }
        break;
    case 4:
        ret = 1;
        lbl_1_bss_D0.flag38++;
        break;
    }
    return ret;
}

void fn_1_86BC(void)
{
    s32 i;

    for (i = 0; i < 4; i++) {
        memset(&lbl_1_bss_828[i], 0, sizeof(M544Item));
    }
}

void fn_1_8710(s32 arg0, s16 arg1)
{
    if (arg1 < 4) {
        memset(&lbl_1_bss_828[arg1], 0, sizeof(M544Item));
        lbl_1_bss_828[arg1].active = 1;
        lbl_1_bss_828[arg1].owner = arg0;
        lbl_1_bss_828[arg1].alpha = 1.0f;
        Hu3DModelObjPosGet(lbl_1_bss_914->models[arg0 + 0x20].id,
                           CharModelItemHookGet(lbl_1_bss_914->chars[arg0].charNo, 1, 0), &lbl_1_bss_828[arg1].a);
        Hu3DModelObjPosGet(lbl_1_bss_914->models[(arg0 ^ 1) + 0x20].id,
                           CharModelItemHookGet(lbl_1_bss_914->chars[arg0 ^ 1].charNo, 1, 4), &lbl_1_bss_828[arg1].b);
        if (lbl_1_bss_914->chars[arg0 ^ 1].charNo == 6) {
            lbl_1_bss_828[arg1].b.y = 180.0f;
            if ((arg0 ^ 1) != 0) {
                lbl_1_bss_828[arg1].b.x -= 30.0f;
            } else {
                lbl_1_bss_828[arg1].b.x += 30.0f;
            }
        }
        lbl_1_bss_828[arg1].b.y += 20.0f;
        if ((arg0 ^ 1) != 0) {
            lbl_1_bss_828[arg1].b.x -= 50.0f;
            return;
        }
        lbl_1_bss_828[arg1].b.x += 45.0f;
    }
}

void fn_1_89AC(OMOBJ *obj)
{
    M544Slot *slot;
    s32 i;

    for (i = 0; i < 4; i++) {
        if (lbl_1_bss_828[i].active != 0) {
            switch (lbl_1_bss_828[i].state) {
            case 0:
                fn_1_8E98(i, 0);
                lbl_1_bss_828[i].state++;
                break;
            case 1:
                slot = &lbl_1_bss_914->slots[lbl_1_bss_828[i].owner ^ 1];
                lbl_1_bss_828[i].timer++;
                fn_1_91B4(i, lbl_1_bss_828[i].timer);
                if (lbl_1_bss_828[i].timer >= 0xA) {
                    fn_1_8E98(i, 1);
                    lbl_1_bss_828[i].state++;
                    lbl_1_bss_828[i].timer = 0;
                    slot->shiftState = 1;
                    slot->shiftMotion = 1;
                    slot->shiftStart = 0.0f;
                    slot->shiftEnd = 10.0f;
                    slot->shiftAttr = 0;
                    slot->attrReset = 0;
                    lbl_1_bss_828[i].timer = 0;
                    Hu3DModelAttrSet(lbl_1_bss_914->models[(s16)((s16)(lbl_1_bss_828[i].owner ^ 1) + 0xA)].id, 1);
                    omVibrate(lbl_1_bss_914->chars[lbl_1_bss_828[i].owner ^ 1].playerNo, 0xC, 0xC, 0);
                }
                break;
            case 2:
                slot = &lbl_1_bss_914->slots[lbl_1_bss_828[i].owner ^ 1];
                if (lbl_1_bss_828[i].timer++ == 0x2D) {
                    slot->shiftState = 1;
                    slot->shiftMotion = 0;
                    slot->shiftStart = 0.0f;
                    slot->shiftEnd = 10.0f;
                    slot->shiftAttr = 0x40000001;
                    slot->attrReset = 0;
                    lbl_1_bss_828[i].active = 0;
                }
                fn_1_9364(i, 0);
                break;
            case 3:
                slot = &lbl_1_bss_914->slots[lbl_1_bss_828[i].owner];
                fn_1_8E98(i, 2);
                slot->shiftState = 1;
                slot->shiftMotion = 1;
                slot->shiftStart = 0.0f;
                slot->shiftEnd = 10.0f;
                slot->shiftAttr = 0;
                slot->attrReset = 0;
                Hu3DModelAttrSet(lbl_1_bss_914->models[(s16)((s16)lbl_1_bss_828[i].owner + 0xA)].id, 1);
                lbl_1_bss_828[i].timer = 0;
                lbl_1_bss_828[i].state++;
                break;
            case 4:
                slot = &lbl_1_bss_914->slots[lbl_1_bss_828[i].owner];
                if (lbl_1_bss_828[i].timer++ == 0x2D) {
                    slot->shiftState = 1;
                    slot->shiftMotion = 0;
                    slot->shiftStart = 0.0f;
                    slot->shiftEnd = 10.0f;
                    slot->shiftAttr = 0x40000001;
                    slot->attrReset = 0;
                    lbl_1_bss_828[i].active = 0;
                }
                fn_1_9364(i, 1);
                break;
            }
        }
    }
}

void fn_1_8E98(s32 arg0, s32 arg1)
{
    s32 mode;
    M544Mdl *m;

    mode = arg1;
    switch (mode) {
    case 0:
        m = &lbl_1_bss_914->models[arg0 + 0xC];
        Hu3DMotionSpeedSet(m->id, 10.0f);
        Hu3DMotionTimeSet(m->id, 0.0f);
        Hu3DModelAttrReset(m->id, 1);
        m->pos.x = lbl_1_bss_828[arg0].a.x;
        m->pos.y = lbl_1_bss_828[arg0].a.y;
        m->pos.z = lbl_1_bss_828[arg0].a.z;
        return;
    case 1:
        Hu3DModelAttrSet(lbl_1_bss_914->models[arg0 + 0xC].id, 1);
        m = &lbl_1_bss_914->models[arg0 + 0x10];
        Hu3DModelAttrReset(m->id, 1);
        Hu3DMotionSpeedSet(m->id, 6.0f);
        Hu3DMotionTimeSet(m->id, 0.0f);
        m->pos.x = lbl_1_bss_828[arg0].b.x;
        m->pos.y = lbl_1_bss_828[arg0].b.y;
        m->pos.z = lbl_1_bss_828[arg0].b.z;
        Hu3DModelPosSetV(m->id, &m->pos);
        if (lbl_1_bss_828[arg0].owner != 0) {
            m->rot.y = -45.0f;
        } else {
            m->rot.y = 90.0f;
        }
        Hu3DModelRotSetV(m->id, &m->rot);
        return;
    case 2:
        m = &lbl_1_bss_914->models[arg0 + 0x10];
        Hu3DModelAttrReset(m->id, 1);
        Hu3DMotionSpeedSet(m->id, 8.0f);
        Hu3DMotionTimeSet(m->id, 0.0f);
        m->pos.x = lbl_1_bss_828[arg0].a.x;
        m->pos.y = lbl_1_bss_828[arg0].a.y;
        m->pos.z = lbl_1_bss_828[arg0].a.z;
        m->pos.y += 20.0f;
        Hu3DModelPosSetV(m->id, &m->pos);
        if (lbl_1_bss_828[arg0].owner != 0) {
            m->rot.y = 90.0f;
        } else {
            m->rot.y = -45.0f;
        }
        Hu3DModelRotSetV(m->id, &m->rot);
        return;
    }
}

void fn_1_91B4(s32 arg0, s32 arg1)
{
    M544Mdl *m;

    m = &lbl_1_bss_914->models[arg0 + 0xC];
    m->pos.x = lbl_1_bss_828[arg0].a.x + (((f32)arg1 * (lbl_1_bss_828[arg0].b.x - lbl_1_bss_828[arg0].a.x)) / 10.0f);
    m->pos.y = lbl_1_bss_828[arg0].a.y + (((f32)arg1 * (lbl_1_bss_828[arg0].b.y - lbl_1_bss_828[arg0].a.y)) / 10.0f);
    m->pos.z = lbl_1_bss_828[arg0].a.z + (((f32)arg1 * (lbl_1_bss_828[arg0].b.z - lbl_1_bss_828[arg0].a.z)) / 10.0f);
}

s32 fn_1_9364(s32 arg0, s32 arg1)
{
    s32 sp8;
    f32 a;
    s32 i;
    HU3DMODEL *model;
    HSFMATERIAL *mat;

    sp8 = 1;
    if (arg1 != 0) {
        model = &Hu3DData[lbl_1_bss_914->models[lbl_1_bss_828[arg0].owner + 0x20].id];
    } else {
        model = &Hu3DData[lbl_1_bss_914->models[(lbl_1_bss_828[arg0].owner ^ 1) + 0x20].id];
    }
    mat = model->hsf->material;
    lbl_1_bss_828[arg0].alpha = lbl_1_bss_828[arg0].alpha * 0.99;
    if (lbl_1_bss_828[arg0].alpha < 0.05) {
        lbl_1_bss_828[arg0].alpha = 0.0f;
    }
    a = lbl_1_bss_828[arg0].alpha;
    if (a > 0.865f) {
        for (i = 0; i < model->hsf->materialNum; i++) {
            mat->color[0] = (s32)((f32)mat->color[0] * a);
            mat->color[1] = (s32)((f32)mat->color[1] * a);
            mat->color[2] = (s32)((f32)mat->color[2] * a);
            mat++;
        }
    }
    if (lbl_1_bss_828[arg0].alpha == 0.0f) {
        return 1;
    }
    return 0;
}

void fn_1_95D8(s32 arg0, s32 arg1)
{
    s32 list[2];
    s32 neg;
    s32 i;

    list[0] = arg0;
    list[1] = arg1;
    i = 0;
    neg = -1;
    for (; i < 2; i++) {
        if (list[i] >= 0) {
            memset(&lbl_1_bss_828[list[i]], 0, sizeof(M544Item));
            lbl_1_bss_828[list[i]].active = 1;
            lbl_1_bss_828[list[i]].state = 3;
            lbl_1_bss_828[list[i]].owner = i;
            lbl_1_bss_828[list[i]].alpha = 1.0f;
            Hu3DModelObjPosGet(lbl_1_bss_914->models[i + 0x20].id,
                               CharModelItemHookGet(lbl_1_bss_914->chars[i].charNo, 1, 0), &lbl_1_bss_828[list[i]].a);
            omVibrate(lbl_1_bss_914->chars[i].playerNo, 0xC, 0xC, 0);
        }
    }
}

void fn_1_9798(s16 arg0)
{
    s16 lvl;
    s16 i;
    s16 cnt;
    s16 lo;
    s16 t;

    lvl = lbl_1_bss_914->chars[arg0].cpuLevel - 1;
    i = 0;
    cnt = 0;
    while (i < 10 && lbl_1_bss_8D8[i].c >= 6) {
        cnt++;
        i++;
    }
    if (frandmod(100) > (s32)(lbl_1_data_3EC[lvl][3] - cnt * 5) && lbl_1_bss_80[arg0].aiMode == 0 && cnt != 0) {
        lbl_1_bss_80[arg0].aiMode = 2;
    }
    if (frandmod(100) > lbl_1_data_3EC[lvl][2] && lbl_1_bss_80[arg0].aiMode == 0) {
        lbl_1_bss_80[arg0].aiMode = 1;
    }
    if (frandmod(100) > lbl_1_data_3EC[lvl][4] && lbl_1_bss_80[arg0].aiMode == 0) {
        lbl_1_bss_80[arg0].aiMode = 3;
    }
    switch (lbl_1_bss_80[arg0].aiMode) {
    default:
        lo = lbl_1_data_3EC[lvl][0];
        lbl_1_bss_80[arg0].delay = lo + frandmod(lbl_1_data_3EC[lvl][1] - lo);
        return;
    case 2:
        if (cnt != 0) {
            lbl_1_bss_80[arg0].target = frandmod(cnt);
        } else {
            lbl_1_bss_80[arg0].target = 0;
        }
        t = lbl_1_bss_8D8[lbl_1_bss_80[arg0].target].b - 0x1D;
        if (t > 0) {
            lbl_1_bss_80[arg0].delay = frandmod(t) + 0x1E;
        } else {
            lbl_1_bss_80[arg0].delay = 0x1E;
        }
        if (lbl_1_data_3EC[lvl][1] < lbl_1_bss_80[arg0].delay) {
            lo = lbl_1_data_3EC[lvl][0];
            lbl_1_bss_80[arg0].delay = lo + frandmod(lbl_1_data_3EC[lvl][1] - lo);
            return;
        }
        return;
    case 3:
        if (cnt != 0) {
            lbl_1_bss_80[arg0].target = frandmod(cnt);
        } else {
            lbl_1_bss_80[arg0].target = 0;
        }
        t = lbl_1_bss_8D8[lbl_1_bss_80[arg0].target].a - 0x1D;
        if (t > 0) {
            lbl_1_bss_80[arg0].delay = frandmod(t) + 0x1E;
            return;
        }
        lbl_1_bss_80[arg0].delay = 0x1E;
        break;
    }
}

s16 fn_1_9C84(s16 arg0)
{
    s16 sp8;
    s16 j;
    s16 ret;

    ret = 0;
    sp8 = lbl_1_bss_914->chars[arg0].cpuLevel - 1;
    switch (lbl_1_bss_80[arg0].aiMode) {
    case 0:
        if (lbl_1_bss_D0.flag28 != 0) {
            if (lbl_1_bss_80[arg0].delay-- <= 0) {
                ret = lbl_1_bss_8D8[lbl_1_bss_C.idx].c | 0x80;
            }
        }
        break;
    case 1:
        if (lbl_1_bss_D0.flag28 != 0) {
            if (lbl_1_bss_80[arg0].delay-- <= 0) {
                j = 0;
                while (j <= 5 && j == lbl_1_bss_8D8[lbl_1_bss_C.idx].c) {
                    j++;
                }
                ret = j | 0x80;
            }
        }
        break;
    case 2:
        if (lbl_1_bss_C.idx == lbl_1_bss_80[arg0].target && lbl_1_bss_C.phase == 1) {
            if (lbl_1_bss_80[arg0].delay-- <= 0) {
                ret = 0x80;
            }
        }
        break;
    case 3:
        if (lbl_1_bss_C.idx == lbl_1_bss_80[arg0].target && lbl_1_bss_C.phase == 0) {
            if (lbl_1_bss_80[arg0].delay-- <= 0) {
                ret = 0x80;
            }
        }
        break;
    }
    return ret;
}
