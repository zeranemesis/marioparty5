#include <string.h>

#include <dolphin/gx.h>
#include <dolphin/os.h>

// GXFifoObj private data
struct __GXFifoObj {
    u8 *base;
    u8 *top;
    u32 size;
    u32 hiWatermark;
    u32 loWatermark;
    void *rdPtr;
    void *wrPtr;
    s32 count;
    u8 bind_cpu;
    u8 bind_gp;
};

// GX state as laid out by the SDK revision linked in MP5 (0x5B0 bytes). It
// differs from the older GXData in <dolphin/gx/GXPriv.h> (0x4F8 bytes): three
// banks of eight texture regions replace TexRegions/TexRegionsCI/nextTexRgn*.
typedef struct __GXData_struct {
    u16 vNumNot;
    u16 bpSentNot;
    u16 vNum;
    u16 vLim;
    u32 cpEnable;
    u32 cpStatus;
    u32 cpClr;
    u32 vcdLo;
    u32 vcdHi;
    u32 vatA[8];
    u32 vatB[8];
    u32 vatC[8];
    u32 lpSize;
    u32 matIdxA;
    u32 matIdxB;
    u32 indexBase[4];
    u32 indexStride[4];
    u32 ambColor[2];
    u32 matColor[2];
    u32 suTs0[8];
    u32 suTs1[8];
    u32 suScis0;
    u32 suScis1;
    u32 tref[8];
    u32 iref;
    u32 bpMask;
    u32 IndTexScale0;
    u32 IndTexScale1;
    u32 tevc[16];
    u32 teva[16];
    u32 tevKsel[8];
    u32 cmode0;
    u32 cmode1;
    u32 zmode;
    u32 peCtrl;
    u32 cpDispSrc;
    u32 cpDispSize;
    u32 cpDispStride;
    u32 cpDisp;
    u32 cpTexSrc;
    u32 cpTexSize;
    u32 cpTexStride;
    u32 cpTex;
    GXBool cpTexZ;
    u32 genMode;
    GXTexRegion TexRegions0[8];
    GXTexRegion TexRegions1[8];
    GXTexRegion TexRegions2[8];
    GXTlutRegion TlutRegions[20];
    GXTexRegion *(*texRegionCallback)(GXTexObj *, GXTexMapID);
    GXTlutRegion *(*tlutRegionCallback)(u32);
    GXAttrType nrmType;
    GXBool hasNrms;
    GXBool hasBiNrms;
    u32 projType;
    f32 projMtx[6];
    f32 vpLeft;
    f32 vpTop;
    f32 vpWd;
    f32 vpHt;
    f32 vpNearz;
    f32 vpFarz;
    f32 zOffset;
    f32 zScale;
    u32 tImage0[8];
    u32 tMode0[8];
    u32 texmapId[16];
    u32 tcsManEnab;
    u32 tevTcEnab;
    GXPerf0 perf0;
    GXPerf1 perf1;
    u32 perfSel;
    GXBool inDispList;
    GXBool dlSaveContext;
    GXBool abtWaitPECopy;
    u8 dirtyVAT;
    u32 dirtyState;
} GXData;

extern GXData *const __GXData;
#define gx __GXData

extern volatile u32 *__piReg;

void __GXSaveCPUFifoAux(struct __GXFifoObj *realFifo);
void __GXSetDirtyState(void);
void __GXSendFlushPrim(void);

#define GX_WRITE_U8(v) (GXWGFifo.u8 = (v))
#define GX_WRITE_U32(v) (GXWGFifo.u32 = (v))

static struct __GXFifoObj DisplayListFifo;
static volatile struct __GXFifoObj *OldCPUFifo;
static GXData __savedGXdata;

void GXBeginDisplayList(void *list, u32 size)
{
    struct __GXFifoObj *CPUFifo = (struct __GXFifoObj *)GXGetCPUFifo();

    if (gx->dirtyState != 0) {
        __GXSetDirtyState();
    }
    if (gx->dlSaveContext != 0) {
        memcpy(&__savedGXdata, gx, sizeof(__savedGXdata));
    }
    DisplayListFifo.base = (u8 *)list;
    DisplayListFifo.top = (u8 *)list + size - 4;
    DisplayListFifo.size = size;
    DisplayListFifo.count = 0;
    DisplayListFifo.rdPtr = list;
    DisplayListFifo.wrPtr = list;
    gx->inDispList = 1;
    GXSaveCPUFifo((GXFifoObj *)CPUFifo);
    OldCPUFifo = CPUFifo;
    GXSetCPUFifo((GXFifoObj *)&DisplayListFifo);
}

u32 GXEndDisplayList(void)
{
    u32 ov;
    BOOL enabled;
    u32 cpenable;

    if (gx->dirtyState != 0) {
        __GXSetDirtyState();
    }
    ov = (__piReg[5] >> 26) & 1;
    __GXSaveCPUFifoAux(&DisplayListFifo);
    GXSetCPUFifo((GXFifoObj *)OldCPUFifo);
    if (gx->dlSaveContext != 0) {
        enabled = OSDisableInterrupts();
        cpenable = gx->cpEnable;
        memcpy(gx, &__savedGXdata, sizeof(*gx));
        gx->cpEnable = cpenable;
        OSRestoreInterrupts(enabled);
    }
    gx->inDispList = 0;
    if (!ov) {
        return DisplayListFifo.count;
    }
    return 0;
}

void GXCallDisplayList(const void *list, u32 nbytes)
{
    if (gx->dirtyState != 0) {
        __GXSetDirtyState();
    }
    if (*(u32 *)&gx->vNumNot == 0) { // checks both vNum and bpSent
        __GXSendFlushPrim();
    }
    GX_WRITE_U8(0x40);
    GX_WRITE_U32((u32)list);
    GX_WRITE_U32(nbytes);
}
