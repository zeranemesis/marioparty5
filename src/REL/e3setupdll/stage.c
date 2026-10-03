#include "game/hu3d.h"
#include "game/data.h"
#include "game/memory.h"

#include "REL/e3setupdll.h"

typedef struct StageMdlTblEntry {
    s32 dataNum;
    s16 motNum;
    s16 pad;
    HuVecF pos;
    HuVecF rot;
    HuVecF scale;
    s16 cameraBit;
    s16 layer;
} StageMdlTblEntry;

s16 DummyMdlId;
s16 StageMotId[0x100];
s16 StageMdlId[0x100];

StageMdlTblEntry StageMdlTbl[5] = {
    { 0x00200002, -1, 0, { 0, 0, 0 }, { 0, 0, 0 }, { 1, 2, 2 }, 1, 0 },
    { 0x00200006, -1, 0, { -356, -240, 0 }, { 0, 5, 0 }, { 2, 2, 2 }, 2, 0 },
    { 0x00200007, 1, 0, { -356, -240, 0 }, { 0, 5, 0 }, { 2, 2, 2 }, 2, 0 },
    { 0x00200004, -1, 0, { 0, 0, 0 }, { 0, 0, 0 }, { 1, 1, 1.5 }, 2, 0 },
    { 0xFFFFFFFF, -1, 0, { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 }, 2, 0 },
};

void StageModelCreate(void *tbl) {
    StageMdlTblEntry *entry = (StageMdlTblEntry *)tbl;
    s16 i;
    for (i = 0; entry->dataNum != 0xFFFFFFFF; i++) {
        if (entry->motNum == -1) {
            StageMdlId[i] = Hu3DModelCreate(HuDataSelHeapReadNum(entry->dataNum, HU_MEMNUM_OVL, HEAP_MODEL));
            Hu3DModelPosSetV(StageMdlId[i], &entry->pos);
            Hu3DModelRotSetV(StageMdlId[i], &entry->rot);
            Hu3DModelScaleSetV(StageMdlId[i], &entry->scale);
            Hu3DModelCameraSet(StageMdlId[i], entry->cameraBit);
            Hu3DModelLayerSet(StageMdlId[i], 1);
        } else {
            StageMotId[i] = Hu3DJointMotion(StageMdlId[entry->motNum], HuDataSelHeapReadNum(entry->dataNum, HU_MEMNUM_OVL, HEAP_MODEL));
        }
        entry++;
    }
}
