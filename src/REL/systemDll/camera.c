#include "types.h"

typedef struct MdCamera_s {
    u8 pad0[0x40];
    float posX;
    float posY;
    float posZ;
    u8 pad4C[0x4];
    float upX;
    float upY;
    float upZ;
} MdCamera;

extern void MdDebugFontPrint(const char *arg0, ...);

typedef struct MdLight_s {
    u8 pad0[0x8];
    float hvrX;
    float hvrY;
    float hvrZ;
    u8 pad14[0x3C];
    u8 r;
    u8 g;
    u8 b;
} MdLight;

static const char lbl_1_data_1781[] = "lhvr(%.1f / %.1f / %.1f)";
static const char lbl_1_data_179A[] = "lrgb(%d / %d / %d)";

void MdLightPrint(MdLight *arg0)
{
    MdDebugFontPrint(lbl_1_data_1781, arg0->hvrX, arg0->hvrY, arg0->hvrZ);
    MdDebugFontPrint(lbl_1_data_179A, arg0->r, arg0->g, arg0->b);
}

static const char lbl_1_data_1708[] = "camera(HU3D_CAM%d))";
static const char lbl_1_data_171C[] = "   hvr(%.1f / %.1f / %.1f / %.1f)";
static const char lbl_1_data_173E[] = "   pos(%.1f / %.1f / %.1f)";
static const char lbl_1_data_1759[] = "target(%.1f / %.1f / %.1f)";
static const char lbl_1_data_1774[] = "  fovy(%.1f)";

typedef struct MdCamEntry_s {
    u8 pad0[0x4];
    float f04;
    float f08;
    float f0C;
    float targetX;
    float targetY;
    float targetZ;
    u8 pad1C[0x24];
    float hvrX;
    float hvrY;
    float hvrZ;
    u8 pad28[0x2E];
    float f7C;
    float f80;
} MdCamEntry;                 /* 0x84 bytes */


typedef struct MdCam_s {
    u8 pad0[0x190];
    s32 curIdx;
    u8 pad194[0x8];
} MdCam;

void MdCameraPrint(MdCam *work)
{
    MdCamEntry *e = (MdCamEntry *)((u8 *)work + work->curIdx * 0x84 + 4);

    MdDebugFontPrint(lbl_1_data_1708, work->curIdx);
    MdDebugFontPrint(lbl_1_data_171C, e->hvrX, e->hvrY, e->hvrZ);
    MdDebugFontPrint(lbl_1_data_173E, e->f04, e->f08, e->f0C);
    MdDebugFontPrint(lbl_1_data_1759, e->targetX, e->targetY, e->targetZ);
    MdDebugFontPrint(lbl_1_data_1774, e->f80);
}

void MdCameraPosSet(MdCamera *arg0, float posX, float posY, float posZ, float upX, float upY, float upZ)
{
    arg0->posX = posX;
    arg0->posY = posY;
    arg0->posZ = posZ;
    arg0->upX = upX;
    arg0->upY = upY;
    arg0->upZ = upZ;
}
