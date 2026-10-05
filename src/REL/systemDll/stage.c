#include "game/hu3d.h"

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
} MdRing;

typedef struct MdFuki_s {
    int mode;
    int grp;
    int memberNo;
    int unkC;
    int unk10;
    int unk14;
    int unk18;
    void *hook;
} MdFuki;

typedef struct Sys_s {
    u8 pad[0x19C];
    MdRing *ring;
    u8 pad2[0x4];
    MdFuki *fuki;
} Sys;

extern Sys *sysP;

void MdRingModeSet(int no, int mode)
{
    MdRing *ring = &sysP->ring[no];
    ring->mode = mode;
}

void MdRingMdlSet(int no, int mdlNo)
{
    MdRing *ring = &sysP->ring[no];
    ring->baseMdlNo = mdlNo;
}

void MdFukiModeSet(int no, int mode)
{
    MdFuki *fuki = &sysP->fuki[no];
    fuki->mode = mode;
}

void MdFukiGrpSet(int no, int grp)
{
    MdFuki *fuki = &sysP->fuki[no];
    fuki->grp = grp;
}

void MdFukiMemberNoSet(int no, int memberNo)
{
    MdFuki *fuki = &sysP->fuki[no];
    fuki->memberNo = memberNo;
}

void MdFukiHookSet(int no, void *hook)
{
    MdFuki *fuki = &sysP->fuki[no];
    fuki->hook = hook;
}