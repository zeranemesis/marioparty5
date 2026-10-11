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
    MdFuki *fuki;
} Sys;

extern s16 Hu3DCameraNo;
extern void Hu3DZClear(void);
extern Sys *sysP;
extern u8 sysWork[];
extern void TexBufFBCopy(void *arg0, s32 arg1);
extern void ToueiZClear(void);

void MdRingModeSet(Sys *arg0, int no, int mode)
{
    MdRing *r = arg0->ring + no;

    r->mode = mode;
}

void MdRingMdlSet(Sys *arg0, int no, int mdlNo)
{
    MdRing *r = arg0->ring + no;

    r->baseMdlNo = mdlNo;
}

void MdFukiModeSet(Sys *arg0, int no, int mode)
{
    MdFuki *r = arg0->fuki + no;

    r->mode = mode;
}

void MdFukiGrpSet(Sys *arg0, int no, int grp)
{
    MdFuki *r = arg0->fuki + no;

    r->grp = grp;
}

void MdFukiMemberNoSet(Sys *arg0, int no, int memberNo)
{
    MdFuki *r = arg0->fuki + no;

    r->memberNo = memberNo;
}

void MdFukiHookSet(Sys *arg0, int no, void *hook)
{
    MdFuki *r = arg0->fuki + no;

    r->hook = hook;
}

int MdFukiModeGet(Sys *arg0, int no)
{
    MdFuki *r = arg0->fuki + no;

    return r->mode;
}

void MdRingOfsSet(Sys *arg0, int no, float x, float y, float z)
{
    MdRing *r = arg0->ring + no;

    r->baseOfs.x = x;
    r->baseOfs.y = y;
    r->baseOfs.z = z;
}

void MdRingScaleSet(Sys *arg0, int no, float scale)
{
    MdRing *r = arg0->ring + no;

    r->scaleP[0]->x = scale;
    r->scaleP[0]->y = scale;
    r->scaleP[0]->z = scale;
}

void MdLayer1Hook(void)
{
    if (Hu3DCameraNo == 2) {
        Hu3DZClear();
    }
}

void MdCopyWarpFilter(void)
{
    u8 *work = sysWork;

    if (Hu3DCameraNo == 0) {
        TexBufFBCopy(work, 0xa);
        ToueiZClear();
    }
}

void MdLayer2Hook(void)
{
    u8 *work = sysWork;

    if (Hu3DCameraNo == 0) {
        TexBufFBCopy(work, 0xa);
        ToueiZClear();
    }
}

void MdRingColorReset(Sys *arg0, int no)
{
    MdRing *r = arg0->ring + no;

    r->unk10 = 0;
    r->time = 0;
    r->ofsTime = 0;
    r->t = 0;
    r->ofsT = 0;
}