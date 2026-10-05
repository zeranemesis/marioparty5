#include "game/hu3d.h"
#include "game/sprite.h"
#include "game/object.h"
#include "game/process.h"

typedef struct Sys_s {
    char unk_00[0x9C];
    void *ring;
} SYS;

extern SYS *sysP;

typedef struct MdRing_s {
    BOOL disp;
    int mode;
    GXColor color;
    GXColor endColor;
    int unk10;
    float time;
    float ofsTime;
    float t;
    float ofsT;
    int mdlNo;
    HuVecF **posP;
    HuVecF **rotP;
    HuVecF **scaleP;
    int baseMdlNo;
    HuVecF baseOfs;
} MDRING;

void MdRingModeSet(int no, int mode) {
    void *ring = sysP->ring;
    ring = (char *)ring + no * 0x44;
    *(int *)((char *)ring + 0x4) = mode;
} // NONMATCHING: regalloc r5 vs r31 for ring pointer

void MdRingMdlSet(int no, int mdlNo) {
    void *ring = sysP->ring;
    ring = (char *)ring + no * 0x44;
    *(int *)((char *)ring + 0x34) = mdlNo;
} // NONMATCHING: regalloc r5 vs r31 for ring pointer

HuVec2f *MdSpriteGrpPosGet(HUSPRGRPID *sprite, int grpNo) {
    int byteOffset = grpNo * 2 + 0x46;
    HUSPRGRPID grpId = *(HUSPRGRPID *)((char *)sprite + byteOffset);
    return &HuSprGrpData[grpId].pos;
} // NONMATCHING: regalloc differs (r30/r31 vs r0/r31)

HuVec2f *MdSpriteGrpScaleGet(HUSPRGRPID *sprite, int grpNo) {
    int byteOffset = grpNo * 2 + 0x46;
    HUSPRGRPID grpId = *(HUSPRGRPID *)((char *)sprite + byteOffset);
    return &HuSprGrpData[grpId].scale;
} // NONMATCHING: regalloc differs (r30/r31 vs r0/r31)

static void LayerPostHook(s16 layerNo) {
    if (Hu3DCameraNo == 0) {
        Hu3DZClear();
    }
}

static void Layer2Hook(s16 layerNo) {
    if (Hu3DCameraNo == 1) {
        Hu3DZClear();
    }
}