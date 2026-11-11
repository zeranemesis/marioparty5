#define _MATH_H
#include "game/frand.h"
#include "game/hsfex.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/object.h"

#include "game/mg/seqman.h"

#include "REL/m530Dll.h"

#include "datadir_enum.h"

char lbl_1_data_150[12] = "itemhook_f1";
char lbl_1_data_15C[12] = "itemhook_f2";
char lbl_1_data_168[12] = "itemhook_f3";
char lbl_1_data_174[12] = "itemhook_f4";
char lbl_1_data_180[12] = "itemhook_f5";
char lbl_1_data_18C[12] = "itemhook_f6";
char lbl_1_data_198[12] = "itemhook_f7";
char lbl_1_data_1A4[12] = "itemhook_f8";
char lbl_1_data_1B0[12] = "itemhook_f9";
char lbl_1_data_1BC[13] = "itemhook_f10";
char lbl_1_data_1C9[13] = "itemhook_f11";
char lbl_1_data_1D6[13] = "itemhook_f12";
char lbl_1_data_1E3[13] = "itemhook_f13";
char lbl_1_data_1F0[13] = "itemhook_f14";
char lbl_1_data_1FD[13] = "itemhook_f15";
char lbl_1_data_20A[13] = "itemhook_f16";
char lbl_1_data_217[13] = "itemhook_f17";
char lbl_1_data_224[13] = "itemhook_f18";
char lbl_1_data_231[13] = "itemhook_f19";
char lbl_1_data_23E[13] = "itemhook_f20";
char lbl_1_data_24B[13] = "itemhook_f21";

char *lbl_1_data_258[21] = {
    lbl_1_data_150, lbl_1_data_15C, lbl_1_data_168, lbl_1_data_174, lbl_1_data_180, lbl_1_data_18C, lbl_1_data_198,
    lbl_1_data_1A4, lbl_1_data_1B0, lbl_1_data_1BC, lbl_1_data_1C9, lbl_1_data_1D6, lbl_1_data_1E3, lbl_1_data_1F0,
    lbl_1_data_1FD, lbl_1_data_20A, lbl_1_data_217, lbl_1_data_224, lbl_1_data_231, lbl_1_data_23E, lbl_1_data_24B,
};

char lbl_1_data_2AC[12] = "itemhook_ef";

s32 lbl_1_bss_CC;
s32 lbl_1_bss_C4[2];
s32 lbl_1_bss_C0;

void fn_1_4980(s32 cellNo);
void fn_1_5360(OMOBJ *obj);
void fn_1_57A4(OMOBJ *obj);
void fn_1_5BD4(OMOBJ *obj);
void fn_1_5E50(OMOBJ *obj);
void fn_1_5FE8(OMOBJ *obj);
void fn_1_61A8(OMOBJ *obj);
void fn_1_6280(OMOBJ *obj);
void fn_1_6358(M530_CELL *cell);
void fn_1_6400(OMOBJ *obj);

void fn_1_45A0(void) {
    s32 i;
    s32 k;
    s32 tmp;
    OMOBJ *obj;

    for (i = 0; i < 21; i++) {
        obj = omAddObjEx(lbl_1_bss_B8, 0x3000, 0, 0, -1, fn_1_5360);
        obj->work[0] = i;
        lbl_1_bss_DC[i].hookMdl = Hu3DModelCreate(HuDataSelHeapReadNum(0x580001, HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelLayerSet(lbl_1_bss_DC[i].hookMdl, 2);
        Hu3DModelObjPosGet(lbl_1_bss_44[0], lbl_1_data_258[i], &lbl_1_bss_DC[i].start);
        Hu3DModelHookSet(lbl_1_bss_44[0], lbl_1_data_258[i], lbl_1_bss_DC[i].hookMdl);
        Hu3D3Dto2D(&lbl_1_bss_DC[i].start, (s16)lbl_1_data_6C.camNo, &lbl_1_bss_DC[i].pos);
        lbl_1_bss_DC[i].basePos = lbl_1_bss_DC[i].start;
        lbl_1_bss_DC[i].no = i;
        lbl_1_bss_DC[i].vel.x = lbl_1_bss_DC[i].vel.y = lbl_1_bss_DC[i].vel.z = 0.0f;
        Hu3DModelAttrSet(lbl_1_bss_DC[i].hookMdl, 1);
        lbl_1_bss_DC[i].efMdl = -1;
        lbl_1_bss_DC[i].shadowMdl = -1;
    }
    fn_1_4980(5);
    fn_1_4980(6);
    i = 0;
    while (i < 4) {
        tmp = frandmod(21);
        if (tmp >= 2 && lbl_1_bss_DC[tmp].state == 0) {
            fn_1_4980(tmp);
            i++;
        }
    }
    fn_1_439C(&lbl_1_bss_DC[0].basePos, &lbl_1_bss_DC[1].basePos, &lbl_1_bss_DC[20].basePos, &lbl_1_bss_D0);
    for (i = 0; i < 2; i++) {
        lbl_1_bss_C4[i] = frandmod(0x3FC) + 0xB4;
    }
    for (i = 0; i < 1; i++) {
        for (k = i + 1; k < 2; k++) {
            if (lbl_1_bss_C4[i] < lbl_1_bss_C4[k]) {
                tmp = lbl_1_bss_C4[i];
                lbl_1_bss_C4[i] = lbl_1_bss_C4[k];
                lbl_1_bss_C4[k] = tmp;
            }
        }
    }
    lbl_1_bss_CC = 0;
    lbl_1_bss_C0 = 0;
}

void fn_1_4980(s32 cellNo) {
    M530_CELL *cell = &lbl_1_bss_DC[cellNo];

    cell->variant = 0;
    cell->state = 2;
    cell->kind = 4;
    cell->timer = 0;
    cell->wait = frandmod(80);
    Hu3DModelAttrSet(cell->hookMdl, 0x40000002);
    Hu3DMotionTimeSet(cell->hookMdl, 40.0f);
    Hu3DModelAttrReset(cell->hookMdl, 1);
}

s32 fn_1_4A18(void) {
    s32 i;
    s32 count;
    s32 state;

    i = 0;
    count = 0;
    for (; i < 21; i++) {
        state = lbl_1_bss_DC[i].state;
        if (state >= 2 && state <= 4) {
            count++;
        }
    }
    return count;
}

void fn_1_4A80(M530_CELL *cell, f32 time) {
    Hu3DMotionTimeSet(cell->hookMdl, time + 100.0f * cell->variant);
}

void fn_1_4AF4(M530_CELL *cell, f32 start, f32 end) {
    Hu3DMotionStartEndSet(cell->hookMdl, start + 100.0f * cell->variant, end + 100.0f * cell->variant);
}

void fn_1_4BAC(M530_CELL *cell, s16 anim) {
    Hu3DModelAttrReset(cell->hookMdl, 0x40000001);
    Hu3DModelAttrReset(cell->hookMdl, 0x40000002);
    Hu3DModelAttrReset(cell->hookMdl, 0x40000004);
    switch (anim) {
        case 0:
            fn_1_4AF4(cell, 0.0f, 19.0f);
            fn_1_4A80(cell, 0.0f);
            Hu3DModelAttrReset(cell->hookMdl, 1);
            break;
        case 1:
            if ((s32)frandmod(2) == 0) {
                fn_1_4AF4(cell, 60.0f, 80.0f);
                fn_1_4A80(cell, 60.0f);
            } else {
                Hu3DModelAttrSet(cell->hookMdl, 0x40000004);
                fn_1_4AF4(cell, 60.0f, 80.0f);
                fn_1_4A80(cell, 80.0f);
            }
            break;
        case 2:
            fn_1_4AF4(cell, 81.0f, 99.0f);
            fn_1_4A80(cell, 81.0f);
            break;
        case 3:
            fn_1_4AF4(cell, 20.0f, 39.0f);
            fn_1_4A80(cell, 20.0f);
            break;
        case 4:
            fn_1_4AF4(cell, 40.0f, 59.0f);
            fn_1_4A80(cell, 40.0f);
            break;
    }
}

void fn_1_51E8(M530_CELL *cell) {
    if (cell->variant == 1) {
        cell->efMdl = Hu3DModelCreate(HuDataSelHeapReadNum(0x580005, HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelAttrSet(cell->efMdl, 0x40000001);
        Hu3DModelHookSet(cell->hookMdl, lbl_1_data_2AC, cell->efMdl);
    }
}

void fn_1_525C(M530_CELL *cell) {
    if (cell->efMdl != -1) {
        Hu3DModelHookObjReset(cell->hookMdl, lbl_1_data_2AC);
        Hu3DModelKill(cell->efMdl);
        cell->efMdl = -1;
    }
}

void fn_1_52B0(M530_CELL *cell) {
    if (cell->shadowMdl == -1) {
        cell->shadowMdl = Hu3DModelCreate(HuDataSelHeapReadNum(0x580002, HU_MEMNUM_OVL, HEAP_MODEL));
        Hu3DModelPosSet(cell->shadowMdl, cell->basePos.x, 0.0f, cell->basePos.z);
    }
}

void fn_1_531C(M530_CELL *cell) {
    if (cell->shadowMdl != -1) {
        Hu3DModelKill(cell->shadowMdl);
        cell->shadowMdl = -1;
    }
}

void fn_1_5360(OMOBJ *obj) {
    u32 no = obj->work[0];
    M530_CELL *cell = &lbl_1_bss_DC[obj->work[0]];

    if (cell->state != 0) {
        if (cell->state == 2) {
            fn_1_57A4(obj);
        }
        switch (cell->state) {
            case 1:
                fn_1_5BD4(obj);
                return;
            case 3:
                if (cell->kind == 4) {
                    cell->state = 4;
                } else {
                    cell->state = 5;
                }
                cell->timer = 0;
                return;
            case 4:
                if (cell->timer++ == 0) {
                    Hu3DModelAttrReset(cell->hookMdl, 0x40000001);
                    Hu3DModelAttrReset(cell->hookMdl, 0x40000002);
                    Hu3DModelAttrReset(cell->hookMdl, 0x40000004);
                    if ((s32)frandmod(2) == 0) {
                        fn_1_4AF4(cell, 60.0f, 80.0f);
                        fn_1_4A80(cell, 60.0f);
                    } else {
                        Hu3DModelAttrSet(cell->hookMdl, 0x40000004);
                        fn_1_4AF4(cell, 60.0f, 80.0f);
                        fn_1_4A80(cell, 80.0f);
                    }
                    fn_1_52B0(cell);
                } else if (Hu3DMotionEndCheck(cell->hookMdl) != 0) {
                    cell->timer = 0;
                    cell->state = 5;
                    Hu3DModelAttrReset(cell->hookMdl, 0x40000004);
                    Hu3DModelAttrSet(cell->hookMdl, 0x40000002);
                    return;
                }
                break;
            case 5:
                fn_1_5E50(obj);
                return;
            case 6:
                fn_1_5FE8(obj);
                return;
            case 7:
                if (cell->timer++ >= 120) {
                    cell->state = 8;
                    cell->timer = 0;
                    return;
                }
                break;
            case 8:
                cell->timer++;
                if (cell->timer % 2 != 0) {
                    Hu3DModelAttrSet(cell->hookMdl, 1);
                } else {
                    Hu3DModelAttrReset(cell->hookMdl, 1);
                }
                if (cell->timer >= 20) {
                    cell->state = 9;
                    cell->timer = 0;
                    return;
                }
                break;
            case 9:
                fn_1_61A8(obj);
                return;
            case 10:
                fn_1_6280(obj);
                break;
        }
    }
}

void fn_1_57A4(OMOBJ *obj) {
    u32 no = obj->work[0];
    M530_CELL *cell = &lbl_1_bss_DC[obj->work[0]];

    if (MgSeqModeGet() >= 5) {
        switch (cell->kind) {
            case 1:
                cell->wait++;
                if (Hu3DMotionEndCheck(cell->hookMdl) != 0) {
                    cell->wait = 0;
                    cell->kind = 2;
                    return;
                }
                break;
            case 2:
                if (cell->wait++ >= 10) {
                    Hu3DModelAttrReset(cell->hookMdl, 0x40000001);
                    Hu3DModelAttrReset(cell->hookMdl, 0x40000002);
                    Hu3DModelAttrReset(cell->hookMdl, 0x40000004);
                    fn_1_4AF4(cell, 20.0f, 39.0f);
                    fn_1_4A80(cell, 20.0f);
                    cell->wait = 0;
                    cell->kind = 3;
                    return;
                }
                break;
            case 3:
                if (Hu3DMotionEndCheck(cell->hookMdl) != 0) {
                    cell->wait = 0;
                    cell->kind = 4;
                    fn_1_51E8(cell);
                }
                break;
            case 4:
                if (MgSeqModeGet() != 8) {
                    if (cell->wait++ >= 160) {
                        Hu3DModelAttrReset(cell->hookMdl, 0x40000001);
                        Hu3DModelAttrReset(cell->hookMdl, 0x40000002);
                        Hu3DModelAttrReset(cell->hookMdl, 0x40000004);
                        fn_1_4AF4(cell, 40.0f, 59.0f);
                        fn_1_4A80(cell, 40.0f);
                        cell->wait = 0;
                        cell->kind = 5;
                        fn_1_525C(cell);
                    }
                }
                break;
            case 5:
                if (Hu3DMotionEndCheck(cell->hookMdl) != 0) {
                    cell->wait = 0;
                    cell->kind = 6;
                    return;
                }
                break;
            case 6:
                if (cell->wait++ >= 120) {
                    cell->state = 3;
                    cell->wait = 0;
                }
                break;
        }
    }
}

void fn_1_5BD4(OMOBJ *obj) {
    M530_CELL *cell = &lbl_1_bss_DC[obj->work[0]];

    cell->state = 2;
    cell->kind = 1;
    cell->timer = 0;
    cell->wait = 0;
    cell->basePos = cell->start;
    cell->efMdl = -1;
    cell->shadowMdl = -1;
    cell->variant = 0;
    if (MgSeqTimerValueGet() < (u32)lbl_1_bss_C4[lbl_1_bss_CC]) {
        lbl_1_bss_CC++;
        cell->variant = 1;
    }
    if (MgSeqModeGet() == 8) {
        cell->variant = 1;
    }
    Hu3DModelAttrReset(cell->hookMdl, 0x40000001);
    Hu3DModelAttrReset(cell->hookMdl, 0x40000002);
    Hu3DModelAttrReset(cell->hookMdl, 0x40000004);
    fn_1_4AF4(cell, 0.0f, 19.0f);
    fn_1_4A80(cell, 0.0f);
    Hu3DModelAttrReset(cell->hookMdl, 1);
    Hu3DMotionCalc(cell->hookMdl);
    Hu3DModelPosSet(cell->hookMdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelHookSet(lbl_1_bss_44[0], lbl_1_data_258[cell->no], cell->hookMdl);
}

void fn_1_5E50(OMOBJ *obj) {
    M530_CELL *cell = &lbl_1_bss_DC[obj->work[0]];

    if (cell->timer++ == 0) {
        Hu3DModelHookObjReset(lbl_1_bss_44[0], lbl_1_data_258[cell->no]);
        cell->basePos = cell->start;
        cell->vel.y = 0.0f;
        Hu3DModelPosSetV(cell->hookMdl, &cell->basePos);
        fn_1_52B0(cell);
        fn_1_1B78(&cell->basePos.x, 0x79C);
        return;
    }
    cell->vel.y += 0.4f;
    cell->basePos.y -= cell->vel.y;
    if (cell->basePos.y <= 25.0f) {
        cell->basePos.y = 25.0f;
        cell->timer = 0;
        if (cell->kind == 4) {
            cell->state = 6;
        } else {
            cell->state = 7;
            fn_1_6358(cell);
        }
    }
    Hu3DModelPosSetV(cell->hookMdl, &cell->basePos);
}

void fn_1_5FE8(OMOBJ *obj) {
    M530_CELL *cell = &lbl_1_bss_DC[obj->work[0]];

    if (cell->timer++ == 0) {
        Hu3DModelAttrReset(cell->hookMdl, 0x40000001);
        Hu3DModelAttrReset(cell->hookMdl, 0x40000002);
        Hu3DModelAttrReset(cell->hookMdl, 0x40000004);
        fn_1_4AF4(cell, 81.0f, 99.0f);
        fn_1_4A80(cell, 81.0f);
        fn_1_525C(cell);
    } else if (Hu3DMotionEndCheck(cell->hookMdl) != 0) {
        cell->state = 9;
        cell->timer = 0;
    }
}

void fn_1_61A8(OMOBJ *obj) {
    M530_CELL *cell = &lbl_1_bss_DC[obj->work[0]];

    Hu3DModelHookObjReset(lbl_1_bss_44[0], lbl_1_data_258[cell->no]);
    Hu3DModelAttrSet(cell->hookMdl, 1);
    fn_1_525C(cell);
    fn_1_531C(cell);
    cell->state = 0;
    cell->kind = 0;
    cell->timer = 0;
}

void fn_1_6280(OMOBJ *obj) {
    M530_CELL *cell = &lbl_1_bss_DC[obj->work[0]];

    if (cell->timer % 2 != 0) {
        Hu3DModelAttrSet(cell->hookMdl, 1);
    } else {
        Hu3DModelAttrReset(cell->hookMdl, 1);
    }
    cell->timer++;
    if ((f32)cell->timer >= 20.0f) {
        cell->state = 9;
        cell->timer = 0;
    }
}

void fn_1_6358(M530_CELL *cell) {
    HU3DMODELID modelId;
    OMOBJ *obj;

    modelId = Hu3DModelCreate(HuDataSelHeapReadNum(0x580006, HU_MEMNUM_OVL, HEAP_MODEL));
    obj = omAddObjEx(lbl_1_bss_B8, 0x3000, 1, 0, -1, fn_1_6400);
    obj->mdlId[0] = modelId;
    Hu3DModelLayerSet(modelId, 3);
    omSetTra(obj, cell->basePos.x, cell->basePos.y, cell->basePos.z);
}

void fn_1_6400(OMOBJ *obj) {
    if (Hu3DMotionEndCheck(obj->mdlId[0]) != 0) {
        Hu3DModelKill(obj->mdlId[0]);
        omDelObjEx(lbl_1_bss_B8, obj);
    }
}

s32 fn_1_645C(Vec *pos, f32 radius) {
    s32 i;
    Vec sp10;

    for (i = 0; i < 21; i++) {
        if (lbl_1_bss_DC[i].state >= 2 && lbl_1_bss_DC[i].state <= 7) {
            PSVECSubtract(pos, &lbl_1_bss_DC[i].basePos, &sp10);
            if (PSVECMag(&sp10) <= radius) {
                return i;
            }
        }
    }
    return -1;
}

s32 fn_1_6518(f32 *pos, f32 radius) {
    s32 i;
    Vec sp10;

    for (i = 0; i < 21; i++) {
        if (lbl_1_bss_DC[i].state >= 2 && lbl_1_bss_DC[i].state < 7) {
            PSVECSubtract((Vec *)pos, &lbl_1_bss_DC[i].basePos, &sp10);
            if (PSVECMag(&sp10) <= radius) {
                return i;
            }
        }
    }
    return -1;
}

s32 fn_1_65D4(Vec *pos, f32 radius) {
    s32 i;
    Vec sp0C;

    for (i = 0; i < 21; i++) {
        if (lbl_1_bss_DC[i].state >= 2 && lbl_1_bss_DC[i].state <= 7
            && !(lbl_1_bss_DC[i].basePos.y - pos->y > 120.0f)) {
            PSVECSubtract(pos, &lbl_1_bss_DC[i].basePos, &sp0C);
            sp0C.y = 0.0f;
            if (PSVECMag(&sp0C) <= radius) {
                return i;
            }
        }
    }
    return -1;
}

s32 fn_1_66D8(s32 cellNo) {
    if (lbl_1_bss_DC[cellNo].state != 2) {
        return 0;
    }
    if (lbl_1_bss_DC[cellNo].kind < 4) {
        return 0;
    }
    lbl_1_bss_DC[cellNo].state = 3;
    return 1;
}

void fn_1_6740(void) {
    s32 i;
    s32 n;
    s32 sel;
    s32 list[21];

    if (lbl_1_bss_C0++ >= 10) {
        i = 0;
        n = 0;
        for (; i < 21; i++) {
            if (lbl_1_bss_DC[i].state == 0) {
                list[n++] = i;
            }
        }
        if (n != 0) {
            if (fn_1_4A18() < 7) {
                sel = list[frandmod(n)];
                lbl_1_bss_DC[sel].state = 1;
                lbl_1_bss_C0 = 0;
            }
        }
    }
}

void fn_1_6868(void) {
    s32 i;
    s32 n;
    s32 sel;
    s32 list[21];

    if (lbl_1_bss_C0++ >= 4) {
        i = 0;
        n = 0;
        for (; i < 21; i++) {
            if (lbl_1_bss_DC[i].state == 0) {
                list[n++] = i;
            }
        }
        if (n != 0) {
            sel = list[frandmod(n)];
            lbl_1_bss_DC[sel].state = 1;
            lbl_1_bss_C0 = 0;
        }
    }
}

void fn_1_694C(s32 cellNo) {
    lbl_1_bss_DC[cellNo].timer = 0;
    lbl_1_bss_DC[cellNo].state = 9;
}

void fn_1_6980(s32 cellNo) {
    lbl_1_bss_DC[cellNo].timer = 0;
    lbl_1_bss_DC[cellNo].state = 8;
}

void fn_1_69B4(void) {
    s32 i;

    for (i = 0; i < 21; i++) {
        if (lbl_1_bss_DC[i].state != 0 && lbl_1_bss_DC[i].state != 9) {
            lbl_1_bss_DC[i].timer = 0;
            lbl_1_bss_DC[i].state = 10;
        }
    }
}

s32 fn_1_6A44(Vec *pos) {
    s32 i;
    s32 best;
    f32 dist;
    f32 minDist;
    Vec bestPos;
    Vec diff;

    minDist = 99999.0f;
    best = -1;
    for (i = 0; i < 21; i++) {
        if (lbl_1_bss_DC[i].kind != 0 && lbl_1_bss_DC[i].kind < 5) {
            PSVECSubtract(&lbl_1_bss_DC[i].basePos, pos, &diff);
            dist = PSVECMag(&diff);
            if (lbl_1_bss_DC[i].variant == 1) {
                dist = 0.0f;
            }
            if (dist < minDist) {
                best = i;
                minDist = dist;
                bestPos = lbl_1_bss_DC[i].basePos;
            }
        }
    }
    return best;
}

s32 fn_1_6B80(f32 *pos, s16 side) {
    s32 i;
    s32 n;
    s32 list[21];

    n = 0;
    for (i = 0; i < 21; i++) {
        if (lbl_1_bss_DC[i].kind != 0 && lbl_1_bss_DC[i].kind < 5 && lbl_1_bss_DC[i].state == 2) {
            list[n++] = i;
        }
    }
    if (n == 0) {
        return -1;
    }
    return list[frandmod(n)];
}

s32 fn_1_6C50(volatile s32 varNo, volatile s32 targetNo) {
    s32 i;
    s32 n;
    s32 list[21];

    n = 0;
    for (i = 0; i < 21; i++) {
        if (lbl_1_bss_DC[i].state >= 4 && lbl_1_bss_DC[i].state <= 5 && lbl_1_bss_DC[i].kind >= 4) {
            /* branch order of the state == 4 test differs from the original (2 rows) */
            if (varNo <= 1) {
                if (lbl_1_bss_DC[i].state != 4) {
                    goto add;
                }
            } else if (lbl_1_bss_DC[i].kind == 4) {
add:
                if (i != targetNo) {
                    list[n++] = i;
                }
            }
        }
    }
    if (n == 0) {
        return -1;
    }
    return list[frandmod(n)];
}
