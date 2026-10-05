#define _MATH_H
#include "game/hu3d.h"
#include "game/object.h"

#include "REL/m530Dll.h"

#include "datadir_enum.h"

typedef struct m530hookwork_s {
    u8 pad00[0x1C];
    s16 hookMdlId; //0x1C
    s16 mdlId; //0x1E
    s16 mdlId2; //0x20
} M530_HOOK_WORK;

typedef struct m530kinomicell_s {
    s32 unk0; //0x00
    s32 unk4; //0x04
    s32 unk8; //0x08
    s32 unkC; //0x0C
    Vec pos; //0x10
    u8 pad1C[0x20];
    s32 unk3C; //0x3C
    s32 unk40; //0x40
    s32 unk44; //0x44
    s32 unk48; //0x48
    s32 unk4C; //0x4C
} M530_KINOMI_CELL;

extern M530_KINOMI_CELL lbl_1_bss_DC[21];

char lbl_1_data_2AC[12] = "itemhook_ef";

void fn_1_525C(M530_HOOK_WORK *work);
void fn_1_531C(M530_HOOK_WORK *work);
void fn_1_6400(OMOBJ *obj);
void fn_1_694C(s32 cellNo);
void fn_1_6980(s32 cellNo);

void fn_1_525C(M530_HOOK_WORK *work) {
    if (work->mdlId != -1) {
        Hu3DModelHookObjReset(work->hookMdlId, lbl_1_data_2AC);
        Hu3DModelKill(work->mdlId);
        work->mdlId = -1;
    }
}

void fn_1_531C(M530_HOOK_WORK *work) {
    if (work->mdlId2 != -1) {
        Hu3DModelKill(work->mdlId2);
        work->mdlId2 = -1;
    }
}

void fn_1_6400(OMOBJ *obj) {
    if (Hu3DMotionEndCheck(obj->mdlId[0]) != 0) {
        Hu3DModelKill(obj->mdlId[0]);
        omDelObjEx(lbl_1_bss_B8, obj);
    }
}

void fn_1_694C(s32 cellNo) {
    lbl_1_bss_DC[cellNo].unk4C = 0;
    lbl_1_bss_DC[cellNo].unk3C = 9;
}

void fn_1_6980(s32 cellNo) {
    lbl_1_bss_DC[cellNo].unk4C = 0;
    lbl_1_bss_DC[cellNo].unk3C = 8;
}