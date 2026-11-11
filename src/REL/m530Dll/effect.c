#include "game/charman.h"
#include "game/data.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/object.h"
#include "game/pad.h"
#include "math.h"

#include "game/mg/seqman.h"

#include "REL/m530Dll.h"

s32 lbl_1_data_390[3] = { 3, 4, 5 };

void fn_1_88FC(OMOBJ *obj);
void fn_1_8C30(OMOBJ *obj);
s32 fn_1_8E2C(M530_PLAYER_WORK *work, Vec *pos);
s32 fn_1_8ED8(Vec *vec);
void fn_1_8F64(OMOBJ *obj);
void fn_1_916C(OMOBJ *obj);
void fn_1_91F8(OMOBJ *obj);
void fn_1_92E8(OMOBJ *obj);
void fn_1_9384(void);
void fn_1_9388(OMOBJ *obj);
void fn_1_9460(OMOBJ *obj);
void fn_1_95B0(OMOBJ *obj);
void fn_1_96B0(OMOBJ *obj);
s32 fn_1_96F8(OMOBJ *obj);

void fn_1_8848(void) {
    HU3DMODELID modelId;
    OMOBJ *obj;

    modelId = Hu3DModelCreate(HuDataSelHeapReadNum(0x580007, HU_MEMNUM_OVL, HEAP_MODEL));
    obj = omAddObjEx(lbl_1_bss_B8, 0x3000, 1, 0, -1, fn_1_88FC);
    obj->mdlId[0] = modelId;
    Hu3DModelLayerSet(modelId, 1);
    omSetTra(obj, 0.0f, 100.0f, lbl_1_data_348[0].z);
}

void fn_1_88FC(OMOBJ *obj) {
    if (Hu3DMotionEndCheck(obj->mdlId[0]) != 0) {
        Hu3DModelKill(obj->mdlId[0]);
        omDelObjEx(lbl_1_bss_B8, obj);
    }
}

void fn_1_8958(OMOBJ *obj) {
    M530_PLAYER_WORK *work = obj->data;
    s16 vol;
    s16 pan;

    fn_1_1A50(&work->pos.x, &vol, &pan);
    CharModelVoicePanSet(work->charNo, pan);
    CharModelVoiceVolSet(work->charNo, vol);
    if (MgSeqModeGet() == 6) {
        CharModelAttrReset(work->charNo, 1);
        work->timer = 0;
        work->state = 3;
        obj->objFunc = fn_1_916C;
        return;
    }
    if (MgSeqModeGet() == 5) {
        switch (work->state) {
            case 1:
                if (work->timer == 0) {
                    omVibrate(work->playerNo, 12, 12, 0);
                    fn_1_10E8(obj, 8);
                }
                work->timer++;
                if (CharMotionEndCheck(work->charNo) != 0) {
                    work->timer = 0;
                    fn_1_10E8(obj, 3);
                    work->state = 0;
                }
                break;
            case 2:
                if (work->timer == 0) {
                    fn_1_10E8(obj, 3);
                }
                work->timer++;
                if (work->timer % 2 != 0) {
                    CharModelAttrSet(work->charNo, 1);
                } else {
                    CharModelAttrReset(work->charNo, 1);
                }
                if (work->timer == 30) {
                    work->timer = 0;
                    work->state = 0;
                }
                break;
        }
        work->vel.x = 0.0f;
        work->vel.y = 0.0f;
        work->vel.z = 0.0f;
        if (work->flag != 0) {
            fn_1_9388(obj);
        } else if (fn_1_342C(work->padNo) >= 10.0f) {
            work->vel.x = 0.26785713f * HuPadStkX[work->padNo];
            work->vel.z = 0.26785713f * -HuPadStkY[work->padNo];
        }
        fn_1_8C30(obj);
        omSetRot(obj, work->rot.x, work->rot.y, work->rot.z);
        omSetTra(obj, work->pos.x, work->pos.y, work->pos.z);
    }
}

void fn_1_8C30(OMOBJ *obj) {
    M530_PLAYER_WORK *work = obj->data;
    s32 anim;
    Vec sp8;

    if (work->state != 1) {
        if (fabs(work->vel.x) > 7.5 || fabs(work->vel.z) > 7.5) {
            anim = 2;
        } else if (PSVECMag(&work->vel) == 0.0f) {
            anim = 0;
        } else {
            anim = 1;
        }
        if (anim != work->unk74) {
            fn_1_10E8(obj, lbl_1_data_390[anim]);
        }
        if (PSVECMag(&work->vel) != 0.0f) {
            work->unk84 = 180.0 * (atan2(work->vel.x, work->vel.z) / 3.141592653589793);
            PSVECAdd(&work->pos, &work->vel, &sp8);
            fn_1_8E2C(work, &sp8);
            fn_1_8ED8(&sp8);
            work->pos = sp8;
        }
        if (work->state != 2) {
            fn_1_8F64(obj);
        }
        work->rot.y = fn_1_3FF4(work->rot.y, work->unk84, 30.0f);
    }
}

s32 fn_1_8E2C(M530_PLAYER_WORK *work, Vec *pos) {
    s32 i;
    M530_PLAYER_WORK *other;

    for (i = 0; i < 4; i++) {
        other = lbl_1_bss_40[i]->data;
        if (work != other && other->slot != 0 && fn_1_4164(&work->pos, pos, &other->pos, 120.0f) != 0) {
            return 1;
        }
    }
    return 0;
}

s32 fn_1_8ED8(Vec *vec) {
    f32 mag = PSVECMag(vec);
    if (mag <= 550.0f) {
        return 0;
    }
    PSVECNormalize(vec, vec);
    PSVECScale(vec, vec, 550.0f);
    return 1;
}

void fn_1_8F64(OMOBJ *obj) {
    M530_PLAYER_WORK *work = obj->data;
    Vec sp8;
    s32 cell;

    cell = fn_1_65D4(&work->pos, 100.0f);
    if (cell != -1 && lbl_1_bss_DC[cell].state == 5) {
        if (lbl_1_bss_DC[cell].kind == 4) {
            if (lbl_1_bss_DC[cell].variant == 0) {
                fn_1_1B78(&work->pos.x, 0x79D);
                lbl_1_bss_4C[work->side].score += 1;
            } else {
                fn_1_1B78(&work->pos.x, 0x79E);
                lbl_1_bss_4C[work->side].score += 3;
            }
            omVibrate(work->playerNo, 12, 6, 6);
            Hu3DModelObjPosGet(obj->mdlId[0], work->itemHook, &sp8);
            CharEffectCoinGlowCreate((s16)lbl_1_data_6C.camNo, &sp8);
            fn_1_1648(work->side);
            fn_1_694C(cell);
            return;
        }
        if (lbl_1_bss_DC[cell].kind == 6) {
            fn_1_1B78(&work->pos.x, 0x79F);
            work->state = 1;
            work->timer = 0;
            fn_1_694C(cell);
            return;
        }
    }
    cell = fn_1_645C(&work->pos, 60.0f);
    if (cell != -1 && lbl_1_bss_DC[cell].state == 7) {
        fn_1_1B78(&work->pos.x, 0x79F);
        work->state = 1;
        work->timer = 0;
        fn_1_6980(cell);
        return;
    }
}

void fn_1_916C(OMOBJ *obj) {
    M530_PLAYER_WORK *work = obj->data;

    if (work->timer == 0) {
        CharModelHookDustCreate(work->charNo, work->itemHook);
        fn_1_10E8(obj, 0);
    }
    work->timer++;
    if (work->timer == 60) {
        work->timer = 0;
        work->state = 4;
        obj->objFunc = fn_1_91F8;
    }
}

void fn_1_91F8(OMOBJ *obj) {
    M530_PLAYER_WORK *work = obj->data;

    if (work->timer++ == 0) {
        fn_1_10E8(obj, 2);
        work->unk88 = 1;
        work->goal = lbl_1_data_360[work->side];
        work->goalSpeed = 7.5f;
        return;
    }
    if (fn_1_96F8(obj) != 0) {
        work->timer = 0;
        work->state = 5;
        obj->objFunc = fn_1_92E8;
    }
    omSetRot(obj, work->rot.x, work->rot.y, work->rot.z);
    omSetTra(obj, work->pos.x, work->pos.y, work->pos.z);
}

void fn_1_92E8(OMOBJ *obj) {
    M530_PLAYER_WORK *work = obj->data;

    work->rot.y = fn_1_3FF4(work->rot.y, 0.0f, 30.0f);
    omSetRot(obj, work->rot.x, work->rot.y, work->rot.z);
    if ((s32)work->rot.y == 0) {
        fn_1_10E8(obj, 0);
        obj->objFunc = (OMOBJFUNC)fn_1_9384;
    }
}

void fn_1_9384(void) {
}

void fn_1_9388(OMOBJ *obj) {
    M530_PLAYER_WORK *work = obj->data;

    if (work->targetNo != -1) {
        if (lbl_1_bss_DC[work->targetNo].state < 2) {
            work->mode = 0;
        }
        if (lbl_1_bss_DC[work->targetNo].state >= 6) {
            work->mode = 0;
        }
    }
    switch (work->mode) {
        case 0:
            fn_1_9460(obj);
            break;
        case 1:
            fn_1_95B0(obj);
            break;
        case 2:
            fn_1_96B0(obj);
            break;
    }
}

void fn_1_9460(OMOBJ *obj) {
    M530_PLAYER_WORK *work = obj->data;
    s32 cell;
    M530_PLAYER_WORK *other;
    s32 idx;
    Vec sp18;
    s32 sp8[4] = { 120, 60, 40, 30 };

    if (work->timer2++ > sp8[work->varNo]) {
        idx = lbl_1_bss_4C[work->side ^ 1].member[1];
        other = lbl_1_bss_40[idx]->data;
        cell = fn_1_6C50(work->varNo, other->targetNo);
        if (cell != -1) {
            work->timer2 = 0;
            work->targetNo = cell;
            work->target = lbl_1_bss_DC[cell].start;
            work->target.y = 0.0f;
            PSVECSubtract(&work->target, &work->pos, &sp18);
            if ((s32)PSVECMag(&sp18) == 0) {
                work->mode = 2;
            } else {
                work->mode = 1;
            }
        }
    }
}

void fn_1_95B0(OMOBJ *obj) {
    M530_PLAYER_WORK *work = obj->data;
    Vec sp8;
    f32 mag;

    PSVECSubtract(&work->target, &work->pos, &sp8);
    mag = PSVECMag(&sp8);
    if ((s32)mag == 0) {
        work->mode = 2;
        return;
    }
    PSVECNormalize(&sp8, &sp8);
    PSVECScale(&sp8, &sp8, 15.0f);
    if (PSVECMag(&sp8) > mag) {
        work->vel.x = 0.0f;
        work->vel.y = 0.0f;
        work->vel.z = 0.0f;
        return;
    }
    work->vel = sp8;
}

void fn_1_96B0(OMOBJ *obj) {
    M530_PLAYER_WORK *work = obj->data;

    if (lbl_1_bss_DC[work->targetNo].state == 0) {
        work->targetNo = -1;
        work->mode = 0;
    }
}

s32 fn_1_96F8(OMOBJ *obj) {
    M530_PLAYER_WORK *work = obj->data;
    M530_PLAYER_WORK *other;
    Vec sp14;
    Vec sp8;
    f32 mag;
    s32 i;

    PSVECSubtract(&work->goal, &work->pos, &sp8);
    mag = PSVECMag(&sp8);
    if (mag == 0.0f) {
        return 1;
    }
    PSVECNormalize(&sp8, &sp8);
    PSVECScale(&sp8, &sp8, work->goalSpeed);
    if (PSVECMag(&sp8) > mag) {
        work->pos = work->goal;
        work->unk88 = 0;
        return 1;
    }
    PSVECAdd(&work->pos, &sp8, &sp14);
    for (i = 0; i < 4; i++) {
        other = lbl_1_bss_40[i]->data;
        if (work == other || other->slot == 0 || fn_1_4164(&work->pos, &sp14, &other->pos, 120.0f) == 0) {
            continue;
        }
        break;
    }
    work->pos = sp14;
    work->rot.y = 180.0 * (atan2(sp8.x, sp8.z) / 3.141592653589793);
    return 0;
}
