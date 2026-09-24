#include "REL/m540Dll.h"

#include "REL/m540Dll/math.c"
#include "REL/m540Dll/lib.c"

#include "messdir_enum.h"
#include "game/gxarray.h"

/* Work of the stage objects (lbl_1_bss_F8) */
typedef struct M540StageWork_s {
    int no; /* 0x00 */
} M540_STAGEWORK;

/* Work of the course map object (lbl_1_bss_F4) */
typedef struct M540MapWork_s {
    HU3DMODELID *mdlId;    /* 0x00 */
    HuVecF *mdlOfs;        /* 0x04 */
    int animType;          /* 0x08 */
    int dispMode;          /* 0x0C */
    HU3DMODELID copyMdlId; /* 0x10 */
    HU3DMODELID drawMdlId; /* 0x12 */
    int texW;              /* 0x14 */
    int texH;              /* 0x18 */
    u32 texSize;           /* 0x1C */
    BOOL texReadyF;        /* 0x20 */
    void *texBuf;          /* 0x24 */
} M540_MAPWORK;

/* Work of the cart objects (lbl_1_bss_E0) */
typedef struct M540CartWork_s {
    M540_OBJ *rider;          /* 0x00 */
    int unk4;                 /* 0x04 */
    BOOL effectF;             /* 0x08 */
    BOOL effectDispF;         /* 0x0C */
    int unk10;                /* 0x10 */
    HuVecF velStart;          /* 0x14 */
    HuVecF vel;               /* 0x20 */
    int no;                   /* 0x2C */
    float ofsY;               /* 0x30 */
    HuVecF efcOfs[4];         /* 0x34 */
    HU3DMODELID efcMdlId[4];  /* 0x64 */
    int playerIdx;            /* 0x6C */
    int unk70;                /* 0x70 */
    int unk74;                /* 0x74 */
    int lane;                 /* 0x78 */
    int unk7C;                /* 0x7C */
    int unk80;                /* 0x80 */
    int unk84;                /* 0x84 */
    int unk88;                /* 0x88 */
    s16 unk8C;                /* 0x8C */
    int seNo;                 /* 0x90 */
    s16 unk94;                /* 0x94 */
} M540_CARTWORK;

/* Work of the player objects (lbl_1_bss_CC) */
typedef struct M540PlayerWork_s {
    HuVecF target;  /* 0x00 */
    int time;       /* 0x0C */
    int order;      /* 0x10 */
    HuVecF vel;     /* 0x14 */
    M540_OBJ *cart; /* 0x20 */
} M540_PLAYERWORK;

typedef struct M540LightParam_s {
    HuVecF pos;
    HuVecF dir;
    GXColor color;
    HuVecF shadowPos;
    HuVecF shadowUp;
    HuVecF shadowTarget;
} M540_LIGHTPARAM;

static const float lbl_1_rodata_90[5] = { 40, 30, 20, 10, 0 };
static const float lbl_1_rodata_A4[5] = { -675, -225, 225, 675, 0 };
static const float lbl_1_rodata_B8[5] = { 450, 200, -50, -300, -1000 };
static const M540_LIGHTPARAM lbl_1_rodata_CC = {
    { 0, 1000, 3000 },
    { 0, 0, -1000 },
    { 255, 255, 255, 255 },
    { -13250, 3000, 3000 },
    { 0, 1, 0 },
    { -13250, 0, -1000 }
};
static const int lbl_1_rodata_10C[5] = { 0x7FC, 0x7FE, 0x7FF, 0x800, 0x801 };
static const u32 lbl_1_rodata_120[CHARNO_MAX][7] = {
    { DATANUM(DATA_mariomot, 0x00), DATANUM(DATA_mariomot, 0x01), DATANUM(DATA_mariomot, 0x02), DATANUM(DATA_mariomot, 0x03), DATANUM(DATA_mariomot, 0x04), DATANUM(DATA_mariomot, 0x30), DATANUM(DATA_mariomot, 0x52) },
    { DATANUM(DATA_luigimot, 0x00), DATANUM(DATA_luigimot, 0x01), DATANUM(DATA_luigimot, 0x02), DATANUM(DATA_luigimot, 0x03), DATANUM(DATA_luigimot, 0x04), DATANUM(DATA_luigimot, 0x30), DATANUM(DATA_luigimot, 0x52) },
    { DATANUM(DATA_peachmot, 0x00), DATANUM(DATA_peachmot, 0x01), DATANUM(DATA_peachmot, 0x02), DATANUM(DATA_peachmot, 0x03), DATANUM(DATA_peachmot, 0x04), DATANUM(DATA_peachmot, 0x30), DATANUM(DATA_peachmot, 0x52) },
    { DATANUM(DATA_yoshimot, 0x00), DATANUM(DATA_yoshimot, 0x01), DATANUM(DATA_yoshimot, 0x02), DATANUM(DATA_yoshimot, 0x03), DATANUM(DATA_yoshimot, 0x04), DATANUM(DATA_yoshimot, 0x30), DATANUM(DATA_yoshimot, 0x52) },
    { DATANUM(DATA_wariomot, 0x00), DATANUM(DATA_wariomot, 0x01), DATANUM(DATA_wariomot, 0x02), DATANUM(DATA_wariomot, 0x03), DATANUM(DATA_wariomot, 0x04), DATANUM(DATA_wariomot, 0x30), DATANUM(DATA_wariomot, 0x52) },
    { DATANUM(DATA_daisymot, 0x00), DATANUM(DATA_daisymot, 0x01), DATANUM(DATA_daisymot, 0x02), DATANUM(DATA_daisymot, 0x03), DATANUM(DATA_daisymot, 0x04), DATANUM(DATA_daisymot, 0x30), DATANUM(DATA_daisymot, 0x52) },
    { DATANUM(DATA_waluigimot, 0x00), DATANUM(DATA_waluigimot, 0x01), DATANUM(DATA_waluigimot, 0x02), DATANUM(DATA_waluigimot, 0x03), DATANUM(DATA_waluigimot, 0x04), DATANUM(DATA_waluigimot, 0x30), DATANUM(DATA_waluigimot, 0x52) },
    { DATANUM(DATA_kinopiomot, 0x00), DATANUM(DATA_kinopiomot, 0x01), DATANUM(DATA_kinopiomot, 0x02), DATANUM(DATA_kinopiomot, 0x03), DATANUM(DATA_kinopiomot, 0x04), DATANUM(DATA_kinopiomot, 0x30), DATANUM(DATA_kinopiomot, 0x52) },
    { DATANUM(DATA_teresamot, 0x00), DATANUM(DATA_teresamot, 0x01), DATANUM(DATA_teresamot, 0x02), DATANUM(DATA_teresamot, 0x03), DATANUM(DATA_teresamot, 0x04), DATANUM(DATA_teresamot, 0x30), DATANUM(DATA_teresamot, 0x52) },
    { DATANUM(DATA_minikoopamot, 0x00), DATANUM(DATA_minikoopamot, 0x01), DATANUM(DATA_minikoopamot, 0x02), DATANUM(DATA_minikoopamot, 0x03), DATANUM(DATA_minikoopamot, 0x04), DATANUM(DATA_minikoopamot, 0x30), DATANUM(DATA_minikoopamot, 0x52) },
    { DATANUM(DATA_minikoopamot, 0x00), DATANUM(DATA_minikoopamot, 0x01), DATANUM(DATA_minikoopamot, 0x02), DATANUM(DATA_minikoopamot, 0x03), DATANUM(DATA_minikoopamot, 0x04), DATANUM(DATA_minikoopamot, 0x30), DATANUM(DATA_minikoopamot, 0x52) },
    { DATANUM(DATA_minikoopamot, 0x00), DATANUM(DATA_minikoopamot, 0x01), DATANUM(DATA_minikoopamot, 0x02), DATANUM(DATA_minikoopamot, 0x03), DATANUM(DATA_minikoopamot, 0x04), DATANUM(DATA_minikoopamot, 0x30), DATANUM(DATA_minikoopamot, 0x52) },
    { DATANUM(DATA_minikoopamot, 0x00), DATANUM(DATA_minikoopamot, 0x01), DATANUM(DATA_minikoopamot, 0x02), DATANUM(DATA_minikoopamot, 0x03), DATANUM(DATA_minikoopamot, 0x04), DATANUM(DATA_minikoopamot, 0x30), DATANUM(DATA_minikoopamot, 0x52) },
};

static char *lbl_1_data_44[5] = {
    "itemhook_A",
    "itemhook_B",
    "itemhook_C",
    "itemhook_D",
    "itemhook_E"
};

/* .bss, defined in reverse order of address */
static M540_CAMERA *lbl_1_bss_138;
static M540_CAMERA *lbl_1_bss_134;
static M540_CAMERA *lbl_1_bss_130;
static M540_OBJ *lbl_1_bss_108[10];
static M540_OBJ *lbl_1_bss_104;
static M540_OBJ *lbl_1_bss_F8[3];
static M540_OBJ *lbl_1_bss_F4;
static M540_OBJ *lbl_1_bss_E0[5];
static M540_OBJ *lbl_1_bss_DC;
static M540_OBJ *lbl_1_bss_CC[4];
static M540_OBJ *lbl_1_bss_C8;
static M540_OBJ *lbl_1_bss_C4;
static M540_OBJ *lbl_1_bss_A4[8];
static M540_OBJ *lbl_1_bss_80[9];
static M540_OBJ *lbl_1_bss_70[4];
static M540_OBJ *lbl_1_bss_60[4];
static M540_OBJ *lbl_1_bss_5C;
static int lbl_1_bss_48[5];
static int lbl_1_bss_44;
static M540_OBJ *lbl_1_bss_40;
static M540_OBJ *lbl_1_bss_3C;
static float lbl_1_bss_38;
static float lbl_1_bss_34;
static int lbl_1_bss_30;
static int lbl_1_bss_20[4];
static int lbl_1_bss_1C;
static HU3DLIGHTID lbl_1_bss_1A;
static s16 lbl_1_bss_18;
static int lbl_1_bss_14;
static int lbl_1_bss_10;
static M540_OBJ *lbl_1_bss_0[4];

static void fn_1_15C(void);
static void fn_1_1D4(s16 mode, s16 frameNo);
static void fn_1_FFC(s16 mode, s16 frameNo);
static void fn_1_2C20(s16 mode, s16 frameNo);
static void fn_1_2C48(s16 mode, s16 frameNo);
static void fn_1_566C(s16 mode, s16 frameNo);
static void fn_1_618C(s16 mode, s16 frameNo);
static void fn_1_636C(M540_OBJ *obj);
static void fn_1_6820(HU3DMODEL *modelP, Mtx *mtx);
static void fn_1_6920(HU3DMODEL *modelP, Mtx *mtx);
static void fn_1_6C3C(void);
static void fn_1_73DC(M540_OBJ *obj);
static void fn_1_7490(M540_OBJ *obj);
static void fn_1_766C(M540_OBJ *obj);
static void fn_1_7A48(M540_OBJ *obj);
static void fn_1_84C8(M540_OBJ *obj);
static void fn_1_857C(M540_OBJ *obj);
static void fn_1_8B70(M540_OBJ *obj);
static void fn_1_9008(M540_OBJ *obj, int arg1, int arg2);
static void fn_1_9080(M540_OBJ *obj);
static void fn_1_A19C(M540_OBJ *obj);
static void fn_1_AA30(M540_OBJ *obj);
static void fn_1_AC14(M540_OBJ *obj);
static M540_OBJ *fn_1_CB34(M540_OBJ *obj);
static int fn_1_8468(M540_OBJ *obj);
static void fn_1_FF14(float arg0, float arg1);
static void fn_1_103B0(HU3DMODELID modelId);
static void fn_1_104B4(HU3DMODELID modelId);

typedef void (*VoidFunc)(void);
extern const VoidFunc _ctors[];
extern const VoidFunc _dtors[];

void ObjectSetup(void);

int _prolog(void)
{
    const VoidFunc *ctors = _ctors;
    while(*ctors != 0) {
        (**ctors)();
        ctors++;
    }
    ObjectSetup();
    return 0;
}

void _epilog(void)
{
    const VoidFunc *dtors = _dtors;
    while(*dtors != 0) {
        (**dtors)();
        dtors++;
    }
}

void ObjectSetup(void)
{
    OMOBJMAN *objman;
    MGSEQ_PARAM seqParam;
    objman = omInitObjMan(1, 8192);
    omGameSysInit(objman);
    HuWinInit(0);
    seqParam.maxTime = 0;
    seqParam.timerPos = 0;
    seqParam.initHook = fn_1_1D4;
    seqParam.fadeInHook = fn_1_FFC;
    seqParam.startHook = fn_1_2C20;
    seqParam.mainHook = fn_1_2C48;
    seqParam.finishHook = fn_1_566C;
    seqParam.preWinnerHook = fn_1_618C;
    seqParam.winnerHook = NULL;
    seqParam.fadeOutHook = NULL;
    seqParam.closeHook = NULL;
    MgSeqCreate(&seqParam);
}

static void fn_1_15C(void)
{
    while(!omSysExitReq) {
        HuPrcVSleep();
    }
    HuAudFadeOut(1000);
    WipeCreate(WIPE_MODE_OUT, WIPE_TYPE_WHITEFADE, 60);
    while(WipeCheckEnd()) {
        HuPrcVSleep();
    }
    omOvlReturnEx(1, 1);
    HuPrcEnd();
}

static void fn_1_5510(int no)
{
    M540_OBJ *obj = lbl_1_bss_E0[no];
    if(fn_1_8468(obj) == 0) {
        fn_1_9008(obj, 0, 1);
    } else {
        M540_CARTWORK *work = M540_OBJWORK(M540_CARTWORK, obj);
        work->unk10 = 1;
    }
}

static void fn_1_5588(int no)
{
    M540_OBJ *obj = lbl_1_bss_E0[no];
    M540_CARTWORK *work = M540_OBJWORK(M540_CARTWORK, obj);
    HuVecF pos;
    if(fn_1_8468(obj) == 0) {
        fn_1_134EC(obj, &pos.x, &pos.y, &pos.z);
        fn_1_FF14(lbl_1_rodata_B8[work->unk7C], lbl_1_rodata_A4[work->lane]);
    } else {
        work->unk10 = 1;
    }
}

static M540_OBJ *fn_1_61C4(void)
{
    M540_OBJ *obj = fn_1_13174(0x1000, 0x2000, DATANUM(DATA_m540, 6), 0, NULL, 1, NULL);
    Hu3DModelCameraSet(obj->modelId, HU3D_CAM0|HU3D_CAM2);
    fn_1_13698(obj);
    fn_1_13774(obj, TRUE);
    return obj;
}

static M540_OBJFUNC lbl_1_data_98[] = {
    fn_1_636C
};

static M540_OBJ *fn_1_6260(int no)
{
    static const u32 lbl_1_rodata_564[3] = {
        DATANUM(DATA_m540, 2),
        DATANUM(DATA_m540, 3),
        DATANUM(DATA_m540, 4)
    };
    M540_OBJ *obj = fn_1_13174(0x1000, 0x2000, lbl_1_rodata_564[no], 0, NULL, 1, lbl_1_data_98);
    M540_STAGEWORK *work;
    Hu3DModelAttrSet(obj->modelId, HU3D_ATTR_NOCULL);
    Hu3DModelCameraSet(obj->modelId, HU3D_CAM0|HU3D_CAM2);
    work = M540_OBJWORK(M540_STAGEWORK, obj);
    work->no = no;
    fn_1_13698(obj);
    switch(no) {
        case 1:
        case 2:
            fn_1_13934(obj);
            break;
        case 0:
            fn_1_13994(obj);
            break;
    }
    fn_1_13774(obj, TRUE);
    return obj;
}

static void fn_1_6360(M540_OBJ *obj)
{
    obj->mode = 0;
}

static void fn_1_636C(M540_OBJ *obj)
{
    static const float lbl_1_rodata_570[4] = { 0, -2600, 0, 0 };
    M540_STAGEWORK *work = M540_OBJWORK(M540_STAGEWORK, obj);
    while(1) {
        float centerX = lbl_1_bss_138->view[0].center.x;
        if(centerX > 0.0f) {
            fn_1_13498(obj, 13250.0+lbl_1_rodata_570[work->no], 0, 0);
        } else {
            fn_1_13498(obj, -13250.0+lbl_1_rodata_570[work->no], 0, 0);
        }
        M540_OBJ_SLEEP(1);
    }
}

static M540_OBJFUNC lbl_1_data_9C[] = {
    fn_1_73DC,
    fn_1_7490,
    fn_1_766C,
    fn_1_7A48
};

static M540_OBJ *fn_1_6524(void)
{
    static const u32 lbl_1_rodata_590[18] = {
        DATANUM(DATA_m540, 0x08),
        DATANUM(DATA_m540, 0x0A),
        DATANUM(DATA_m540, 0x09),
        DATANUM(DATA_m540, 0x00),
        DATANUM(DATA_m540, 0x01),
        DATANUM(DATA_m540, 0x87),
        DATANUM(DATA_m540, 0x88),
        DATANUM(DATA_m540, 0x89),
        DATANUM(DATA_m540, 0x8A),
        DATANUM(DATA_m540, 0x8B),
        DATANUM(DATA_m540, 0x8C),
        DATANUM(DATA_m540, 0x8D),
        DATANUM(DATA_m540, 0x8E),
        DATANUM(DATA_m540, 0x8F),
        DATANUM(DATA_m540, 0x90),
        DATANUM(DATA_m540, 0x90),
        DATANUM(DATA_m540, 0x90),
        DATANUM(DATA_m540, 0x90)
    };
    M540_MAPWORK *work;
    M540_OBJ *obj;
    int i;
    HU3DMODEL *modelP;
    obj = fn_1_13174(0x1000, 0x4000, DATANUM(DATA_m540, 5), 0, NULL, 4, lbl_1_data_9C);
    Hu3DModelCameraSet(obj->modelId, HU3D_CAM2);
    Hu3DModelAttrSet(obj->modelId, HU3D_ATTR_ZWRITE_OFF);
    Hu3DModelAttrSet(obj->modelId, HU3D_MOTATTR_LOOP);
    work = M540_OBJWORK(M540_MAPWORK, obj);
    work->mdlId = HuMemDirectMallocNum(HEAP_HEAP, 18*sizeof(HU3DMODELID), HU_MEMNUM_OVL);
    work->mdlOfs = HuMemDirectMallocNum(HEAP_HEAP, 18*sizeof(HuVecF), HU_MEMNUM_OVL);
    for(i=0; i<18; i++) {
        work->mdlOfs[i].x = 0;
        work->mdlOfs[i].y = 0;
        work->mdlOfs[i].z = 0;
    }
    fn_1_13698(obj);
    fn_1_13934(obj);
    fn_1_13774(obj, TRUE);
    for(i=0; i<18; i++) {
        work->mdlId[i] = Hu3DModelCreateData(lbl_1_rodata_590[i]);
        Hu3DModelCameraSet(work->mdlId[i], HU3D_CAM2);
        Hu3DModelAttrSet(work->mdlId[i], HU3D_ATTR_ZWRITE_OFF);
    }
    Hu3DModelAttrSet(work->mdlId[3], HU3D_ATTR_DISPOFF);
    Hu3DModelAttrSet(work->mdlId[4], HU3D_ATTR_DISPOFF);
    work->mdlOfs[0].z = 0.15f;
    work->mdlOfs[1].z = 0.15f;
    work->mdlOfs[2].z = 1.0f;
    work->mdlOfs[3].z = 0.8f;
    work->mdlOfs[4].z = 0.8f;
    work->copyMdlId = Hu3DHookFuncCreate((HU3DMODELHOOK)fn_1_6820);
    Hu3DModelCameraSet(work->copyMdlId, HU3D_CAM1);
    modelP = &Hu3DData[work->copyMdlId];
    modelP->hookData = work;
    work->texW = 160;
    work->texH = 120;
    work->texSize = GXGetTexBufferSize(work->texW, work->texH, GX_TF_RGB5A3, GX_FALSE, 0);
    work->texBuf = HuMemDirectMallocNum(HEAP_MODEL, work->texSize, modelP->mallocNo);
    work->texReadyF = FALSE;
    work->drawMdlId = Hu3DHookFuncCreate((HU3DMODELHOOK)fn_1_6920);
    Hu3DModelCameraSet(work->drawMdlId, HU3D_CAM2);
    modelP = &Hu3DData[work->drawMdlId];
    modelP->hookData = work;
    return obj;
}

static void fn_1_6820(HU3DMODEL *modelP, Mtx *mtx)
{
    static GXColor lbl_1_data_AC = { 0, 0, 0, 255 };
    M540_MAPWORK *work;
    HU3DCAMERA *camera;
    work = modelP->hookData;
    camera = &Hu3DCamera[1];
    if(camera->fov <= 0.0f) {
        return;
    }
    GXSetTexCopySrc(0, 0, 320, 240);
    GXSetTexCopyDst(work->texW, work->texH, GX_TF_RGB5A3, GX_TRUE);
    GXSetCopyClear(lbl_1_data_AC, 0xFFFFFF);
    GXCopyTex(work->texBuf, GX_TRUE);
    DCFlushRange(work->texBuf, work->texSize);
    GXPixModeSync();
    work->texReadyF = TRUE;
    fn_1_6C3C();
}

static void fn_1_6920(HU3DMODEL *modelP, Mtx *mtx)
{
    static const HuVecF lbl_1_rodata_5E0[4] = {
        { -593, 356, 0 },
        { 593, 356, 0 },
        { 593, -356, 0 },
        { -593, -356, 0 }
    };
    static const float lbl_1_rodata_610[4][2] = {
        { 0, 0 },
        { 1, 0 },
        { 1, 1 },
        { 0, 1 }
    };
    static const GXColor lbl_1_rodata_630[1] = {
        { 0, 0, 80, 16 }
    };
    M540_MAPWORK *work;
    int i;
    HU3DCAMERA *camera;
    GXTexObj texObj;
    Mtx mtxInv;
    camera = &Hu3DCamera[2];
    work = modelP->hookData;
    if(!work->texReadyF) {
        return;
    }
    if(camera->fov <= 0.0f) {
        return;
    }
    GXLoadPosMtxImm(*mtx, GX_PNMTX0);
    PSMTXInvXpose(*mtx, mtxInv);
    GXLoadNrmMtxImm(mtxInv, GX_PNMTX0);
    GXSetNumTevStages(1);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_KONST, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    GXInitTexObj(&texObj, work->texBuf, work->texW, work->texH, GX_TF_RGB5A3, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(&texObj, GX_NEAR, GX_NEAR, 0, 0, 0, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&texObj, GX_TEXMAP0);
    GXSetAlphaCompare(GX_GEQUAL, 1, GX_AOP_AND, GX_GEQUAL, 1);
    GXSetZCompLoc(GX_FALSE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX16);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSETARRAY(GX_VA_POS, lbl_1_rodata_5E0, sizeof(lbl_1_rodata_5E0), sizeof(HuVecF), TRUE);
    GXSetVtxDesc(GX_VA_CLR0, GX_INDEX16);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSETARRAY(GX_VA_CLR0, lbl_1_rodata_630, sizeof(lbl_1_rodata_630), sizeof(GXColor), TRUE);
    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX16);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSETARRAY(GX_VA_TEX0, lbl_1_rodata_610, sizeof(lbl_1_rodata_610), sizeof(lbl_1_rodata_610[0]), TRUE);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    for(i=0; i<4; i++) {
        GXPosition1x16(i);
        GXColor1x16(0);
        GXTexCoord1x16(i);
    }
    GXEnd();
}

static void fn_1_6C3C(void)
{
    HU3DCAMERA *camera;
    Mtx44 proj;
    Mtx lookAt;
    HuVecF eye;
    HuVecF target;
    HuVecF up;
    GXColor color;
    camera = &Hu3DCamera[0];
    if(camera->fov <= 0.0f) {
        return;
    }
    C_MTXPerspective(proj, camera->fov, camera->aspect, camera->near, camera->far);
    GXSetProjection(proj, GX_PERSPECTIVE);
    GXSetViewport(0, 0, 640, 480, 0, 1);
    GXSetScissor(0, 0, 640, 480);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    color.r = color.g = color.b = 0;
    color.a = 255;
    GXSetTevColor(GX_TEVREG0, color);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetCullMode(GX_CULL_NONE);
    GXSetZMode(GX_TRUE, GX_ALWAYS, GX_TRUE);
    eye.x = eye.y = eye.z = 0;
    target.x = target.y = 0;
    target.z = -100;
    up.x = up.z = 0;
    up.y = 1;
    C_MTXLookAt(lookAt, &eye, &up, &target);
    GXLoadPosMtxImm(lookAt, GX_PNMTX0);
    eye.x = 1.2f*(80000.0*-sin(0.5f*camera->fov));
    eye.y = 0.8333333f*(80000.0*-sin(0.5f*camera->fov));
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(-eye.x, -eye.y, -23000);
    GXPosition3f32(eye.x, -eye.y, -23000);
    GXPosition3f32(eye.x, eye.y, -23000);
    GXPosition3f32(-eye.x, eye.y, -23000);
    GXEnd();
}

static void fn_1_707C(M540_OBJ *obj, float x, float y, float z)
{
    M540_MAPWORK *work = M540_OBJWORK(M540_MAPWORK, obj);
    int i;
    M540_OBJ *cart;
    HuVecF *ofs;
    M540_CARTWORK *cartWork;
    s16 no;
    float scale;
    HuVecF pos;
    for(i=0; i<4; i++) {
        cart = fn_1_CB34(lbl_1_bss_CC[i]);
        if(cart != NULL) {
            cartWork = M540_OBJWORK(M540_CARTWORK, cart);
            fn_1_134EC(cart, &pos.x, &pos.y, &pos.z);
            no = lbl_1_bss_CC[i]->charNo+5;
            ofs = &work->mdlOfs[no];
            ofs->x = -45.0f+((905.0f*pos.x)/21700.0f);
            ofs->y = 560.0f-(110.0f*i);
            ofs->z = 35.0f;
            if(cartWork->unk10) {
                scale = 1.4f+(0.002f*pos.y);
            } else {
                scale = 1.4f;
            }
            if(scale > 1.4f) {
                scale = 1.4f;
            }
            if(scale < 0.0f) {
                scale = 0.0f;
            }
            Hu3DModelScaleSet(work->mdlId[no], scale, scale, 1.0f);
        }
    }
    fn_1_13498(obj, x, y, z);
    for(i=0; i<18; i++) {
        Hu3DModelPosSet(work->mdlId[i], x+work->mdlOfs[i].x, y+work->mdlOfs[i].y, z+work->mdlOfs[i].z);
    }
    if(work->dispMode == 3) {
        Hu3DModelPosSet(work->drawMdlId, x, 400.0f+y, 30.5f+z);
    } else {
        Hu3DModelPosSet(work->drawMdlId, x, 400.0f+y, 9000.5f+z);
    }
}

static void fn_1_73D0(M540_OBJ *obj)
{
    obj->mode = 0;
}

static void fn_1_73DC(M540_OBJ *obj)
{
    while(1) {
        M540_OBJ_SLEEP(1);
    }
}

static void fn_1_7484(M540_OBJ *obj)
{
    obj->mode = 1;
}

static void fn_1_7490(M540_OBJ *obj)
{
    HuVecF pos;
    float t;
    float y;
    float step;
    fn_1_134EC(obj, &pos.x, &pos.y, &pos.z);
    step = 0.008333334f;
    t = 0;
    while(1) {
        y = pos.y+(10.0*HuSin(360.0f*t));
        fn_1_707C(obj, pos.x, y, pos.z);
        t += step;
        if(t >= 1.0f) {
            t = 0;
        }
        M540_OBJ_SLEEP(1);
    }
}

static void fn_1_7660(M540_OBJ *obj)
{
    obj->mode = 2;
}

static void fn_1_766C(M540_OBJ *obj)
{
    HuVecF pos;
    float t;
    float baseY;
    float y;
    float step;
    fn_1_134EC(obj, &pos.x, &pos.y, &pos.z);
    baseY = pos.y;
    step = 0.008333334f;
    t = 0;
    while(1) {
        fn_1_134EC(obj, &pos.x, &pos.y, &pos.z);
        y = baseY+(18.0*HuSin(360.0f*t));
        t += step;
        if(t >= 1.0f) {
            t = 0;
        }
        if(pos.x < 300.0f+lbl_1_bss_34) {
            pos.x = 300.0f+lbl_1_bss_34;
        }
        fn_1_707C(obj, pos.x, y, pos.z);
        M540_OBJ_SLEEP(1);
    }
}

static void fn_1_78DC(M540_OBJ *obj, int dispMode)
{
    M540_MAPWORK *work = M540_OBJWORK(M540_MAPWORK, obj);
    int i;
    HuVecF pos;
    for(i=0; i<18; i++) {
        if(i != 3 && i != 4) {
            Hu3DModelAttrSet(work->mdlId[i], HU3D_ATTR_DISPOFF);
        }
    }
    Hu3DModelAttrReset(work->mdlId[2], HU3D_ATTR_DISPOFF);
    switch(work->dispMode = dispMode) {
        case 0:
            break;
        case 1:
            Hu3DModelAttrReset(work->mdlId[0], HU3D_ATTR_DISPOFF);
            break;
        case 2:
            Hu3DModelAttrReset(work->mdlId[1], HU3D_ATTR_DISPOFF);
            for(i=0; i<4; i++) {
                Hu3DModelAttrReset(work->mdlId[lbl_1_bss_CC[i]->charNo+5], HU3D_ATTR_DISPOFF);
            }
            break;
        case 3:
            break;
    }
    pos.x = obj->pos.x;
    pos.y = obj->pos.y;
    pos.z = obj->pos.z;
    fn_1_707C(obj, pos.x, pos.y, pos.z);
}

static void fn_1_7A24(M540_OBJ *obj, int animType)
{
    M540_MAPWORK *work = M540_OBJWORK(M540_MAPWORK, obj);
    work->animType = animType;
    obj->mode = 3;
}

static void fn_1_7A48(M540_OBJ *obj)
{
    M540_MAPWORK *work = M540_OBJWORK(M540_MAPWORK, obj);
    HU3DMODELID modelId;
    int i;
    switch(work->animType) {
        case 0:
            Hu3DModelAttrSet(work->mdlId[4], HU3D_ATTR_DISPOFF);
            modelId = work->mdlId[3];
            Hu3DMotionTimeSet(modelId, 0);
            Hu3DModelAttrReset(modelId, HU3D_ATTR_DISPOFF);
            Hu3DModelAttrReset(modelId, HU3D_MOTATTR_PAUSE);
            M540_OBJ_SLEEP(30);
            break;
        case 1:
            for(i=0; i<13; i++) {
                Hu3DModelAttrSet(work->mdlId[i+5], HU3D_ATTR_DISPOFF);
            }
            Hu3DModelAttrSet(work->mdlId[3], HU3D_ATTR_DISPOFF);
            modelId = work->mdlId[4];
            Hu3DMotionTimeSet(modelId, 0);
            Hu3DModelAttrReset(modelId, HU3D_ATTR_DISPOFF);
            Hu3DModelAttrReset(modelId, HU3D_MOTATTR_PAUSE);
            M540_OBJ_SLEEP(30);
            break;
    }
    obj->mode = 0;
}

static M540_OBJFUNC lbl_1_data_B0[] = {
    fn_1_84C8,
    fn_1_857C,
    fn_1_A19C,
    fn_1_8B70,
    fn_1_9080,
    fn_1_AA30
};

static M540_OBJ *fn_1_7C40(int no)
{
    static const u32 lbl_1_rodata_690[5] = {
        DATANUM(DATA_m540, 0x68),
        DATANUM(DATA_m540, 0x61),
        DATANUM(DATA_m540, 0x62),
        DATANUM(DATA_m540, 0x69),
        DATANUM(DATA_m540, 0x63)
    };
    static const u32 lbl_1_rodata_6A4[5] = {
        DATANUM(DATA_m540, 0x16),
        DATANUM(DATA_m540, 0x25),
        DATANUM(DATA_m540, 0x34),
        DATANUM(DATA_m540, 0x43),
        DATANUM(DATA_m540, 0x52)
    };
    static const float lbl_1_rodata_6B8[5] = { -150, -90, -109, -10, -10 };
    static const HuVecF lbl_1_rodata_6CC[5] = {
        { -380, -180, 0 },
        { -220, -100, 0 },
        { -190, 0, 0 },
        { -180, 0, 0 },
        { -150, 0, 0 }
    };
    static const HuVecF lbl_1_rodata_708[5] = {
        { -50, -60, 120 },
        { -150, 60, 0 },
        { -100, -40, 0 },
        { 100, 0, 0 },
        { -160, -0.0f, 0 }
    };
    static const u32 lbl_1_rodata_744[5][14] = {
        {
            DATANUM(DATA_m540, 0x17), DATANUM(DATA_m540, 0x18), DATANUM(DATA_m540, 0x19), DATANUM(DATA_m540, 0x1A),
            DATANUM(DATA_m540, 0x1B), DATANUM(DATA_m540, 0x1C), DATANUM(DATA_m540, 0x1D), DATANUM(DATA_m540, 0x1E),
            DATANUM(DATA_m540, 0x1F), DATANUM(DATA_m540, 0x20), DATANUM(DATA_m540, 0x21), DATANUM(DATA_m540, 0x22),
            DATANUM(DATA_m540, 0x23), DATANUM(DATA_m540, 0x24)
        },
        {
            DATANUM(DATA_m540, 0x26), DATANUM(DATA_m540, 0x27), DATANUM(DATA_m540, 0x28), DATANUM(DATA_m540, 0x29),
            DATANUM(DATA_m540, 0x2A), DATANUM(DATA_m540, 0x2B), DATANUM(DATA_m540, 0x2C), DATANUM(DATA_m540, 0x2D),
            DATANUM(DATA_m540, 0x2E), DATANUM(DATA_m540, 0x2F), DATANUM(DATA_m540, 0x30), DATANUM(DATA_m540, 0x31),
            DATANUM(DATA_m540, 0x32), DATANUM(DATA_m540, 0x33)
        },
        {
            DATANUM(DATA_m540, 0x35), DATANUM(DATA_m540, 0x36), DATANUM(DATA_m540, 0x37), DATANUM(DATA_m540, 0x38),
            DATANUM(DATA_m540, 0x39), DATANUM(DATA_m540, 0x3A), DATANUM(DATA_m540, 0x3B), DATANUM(DATA_m540, 0x3C),
            DATANUM(DATA_m540, 0x3D), DATANUM(DATA_m540, 0x3E), DATANUM(DATA_m540, 0x3F), DATANUM(DATA_m540, 0x40),
            DATANUM(DATA_m540, 0x41), DATANUM(DATA_m540, 0x42)
        },
        {
            DATANUM(DATA_m540, 0x44), DATANUM(DATA_m540, 0x45), DATANUM(DATA_m540, 0x46), DATANUM(DATA_m540, 0x47),
            DATANUM(DATA_m540, 0x48), DATANUM(DATA_m540, 0x49), DATANUM(DATA_m540, 0x4A), DATANUM(DATA_m540, 0x4B),
            DATANUM(DATA_m540, 0x4C), DATANUM(DATA_m540, 0x4D), DATANUM(DATA_m540, 0x4E), DATANUM(DATA_m540, 0x4F),
            DATANUM(DATA_m540, 0x50), DATANUM(DATA_m540, 0x51)
        },
        {
            DATANUM(DATA_m540, 0x53), DATANUM(DATA_m540, 0x54), DATANUM(DATA_m540, 0x55), DATANUM(DATA_m540, 0x56),
            DATANUM(DATA_m540, 0x57), DATANUM(DATA_m540, 0x58), DATANUM(DATA_m540, 0x59), DATANUM(DATA_m540, 0x5A),
            DATANUM(DATA_m540, 0x5B), DATANUM(DATA_m540, 0x5C), DATANUM(DATA_m540, 0x5D), DATANUM(DATA_m540, 0x5E),
            DATANUM(DATA_m540, 0x5F), DATANUM(DATA_m540, 0x60)
        }
    };
    M540_CARTWORK *work;
    M540_OBJ *obj;
    int i;
    HU3DMODELID modelId;
    obj = fn_1_13174(0x1000, 0x4000, lbl_1_rodata_6A4[no], 14, (u32 *)lbl_1_rodata_744[no], 6, lbl_1_data_B0);
    Hu3DModelCameraSet(obj->modelId, HU3D_CAM0|HU3D_CAM2);
    Hu3DModelAttrSet(obj->modelId, HU3D_ATTR_ZWRITE_OFF);
    work = M540_OBJWORK(M540_CARTWORK, obj);
    work->rider = NULL;
    work->unk4 = 0;
    work->ofsY = lbl_1_rodata_6B8[no];
    work->no = no;
    work->vel.x = 0;
    work->vel.y = 0;
    work->vel.z = 0;
    work->playerIdx = 0;
    work->effectF = FALSE;
    work->effectDispF = FALSE;
    work->unk10 = 0;
    work->unk8C = 0;
    work->seNo = -1;
    work->unk94 = 0;
    for(i=0; i<4; i++) {
        work->efcMdlId[i] = HU3D_MODELID_NONE;
        work->efcOfs[i].x = 0;
        work->efcOfs[i].y = 0;
        work->efcOfs[i].z = 0;
    }
    work->efcOfs[0] = lbl_1_rodata_6CC[no];
    for(i=0; i<3; i++) {
        work->efcOfs[i+1] = lbl_1_rodata_708[no];
    }
    fn_1_13698(obj);
    fn_1_13934(obj);
    fn_1_13874(obj, 1, 0, 8, HU3D_MOTATTR_LOOP);
    fn_1_13774(obj, TRUE);
    work->efcMdlId[0] = Hu3DModelCreateData(lbl_1_rodata_690[no]);
    Hu3DModelAttrSet(work->efcMdlId[0], HU3D_MOTATTR_LOOP);
    Hu3DModelAttrSet(work->efcMdlId[0], HU3D_ATTR_DISPOFF);
    for(i=0; i<3; i++) {
        modelId = Hu3DModelCreateData(DATANUM(DATA_m540, 0x66));
        work->efcMdlId[i+1] = modelId;
        Hu3DModelCameraSet(modelId, HU3D_CAM0|HU3D_CAM2);
        Hu3DModelAttrSet(modelId, HU3D_MOTATTR_LOOP);
        Hu3DModelAttrSet(modelId, HU3D_ATTR_DISPOFF);
        Hu3DMotionTimeSet(modelId, i*15);
    }
    return obj;
}

static void fn_1_8094(M540_OBJ *obj, float x, float y, float z);

static void fn_1_7F9C(M540_OBJ *obj, int lane)
{
    M540_CARTWORK *work = M540_OBJWORK(M540_CARTWORK, obj);
    work->unk7C = 1;
    work->lane = lane;
    fn_1_8094(obj, -11250.0f, 0.0f, lbl_1_rodata_A4[lane]);
}

static void fn_1_801C(M540_OBJ *obj)
{
    M540_CARTWORK *work = M540_OBJWORK(M540_CARTWORK, obj);
    int i;
    fn_1_136D4(obj);
    for(i=0; i<4; i++) {
        Hu3DModelAttrSet(work->efcMdlId[i], HU3D_ATTR_DISPOFF);
    }
}

static char *fn_1_8438(M540_OBJ *obj);

static void fn_1_8094(M540_OBJ *obj, float x, float y, float z)
{
    M540_CARTWORK *work = M540_OBJWORK(M540_CARTWORK, obj);
    int i;
    float rotZ;
    HuVecF pos;
    Mtx mtx;
    obj->pos.x = x;
    obj->pos.y = y;
    obj->pos.z = z;
    Hu3DModelPosSet(obj->modelId, x, y-work->ofsY, z);
    Hu3DModelObjMtxGet(obj->modelId, fn_1_8438(obj), mtx);
    pos.x = mtx[0][3];
    pos.y = mtx[1][3];
    pos.z = mtx[2][3];
    Hu3DModelPosSet(work->efcMdlId[0], pos.x+work->efcOfs[0].x, (pos.y+work->efcOfs[0].y)-work->ofsY, pos.z+work->efcOfs[0].z);
    rotZ = 3.0f*work->vel.x;
    for(i=0; i<3; i++) {
        Hu3DModelPosSet(work->efcMdlId[i+1], pos.x+work->efcOfs[i+1].x, (pos.y+work->efcOfs[i+1].y)+(i*8), (pos.z+work->efcOfs[i+1].z)-(i*4));
        Hu3DModelRotSet(work->efcMdlId[i+1], 0, 0, rotZ);
    }
    if(lbl_1_bss_DC == obj) {
        lbl_1_bss_34 = x;
    }
}

static void fn_1_82D0(M540_OBJ *obj)
{
    M540_CARTWORK *work = M540_OBJWORK(M540_CARTWORK, obj);
    int i;
    if(lbl_1_bss_DC != obj) {
        fn_1_104B4(work->efcMdlId[0]);
        work->effectF = TRUE;
        for(i=0; i<3; i++) {
            Hu3DModelAttrSet(work->efcMdlId[i+1], HU3D_ATTR_DISPOFF);
        }
        work->effectDispF = FALSE;
    }
}

static void fn_1_8364(M540_OBJ *obj)
{
    M540_CARTWORK *work = M540_OBJWORK(M540_CARTWORK, obj);
    fn_1_103B0(work->efcMdlId[0]);
    work->effectF = FALSE;
}

static void fn_1_83A4(M540_OBJ *obj)
{
    M540_CARTWORK *work = M540_OBJWORK(M540_CARTWORK, obj);
    int i;
    if(lbl_1_bss_DC != obj) {
        for(i=0; i<3; i++) {
            Hu3DModelAttrReset(work->efcMdlId[i+1], HU3D_ATTR_DISPOFF);
        }
        work->effectDispF = TRUE;
        fn_1_103B0(work->efcMdlId[0]);
        work->effectF = FALSE;
    }
}

static char *fn_1_8438(M540_OBJ *obj)
{
    M540_CARTWORK *work = M540_OBJWORK(M540_CARTWORK, obj);
    return lbl_1_data_44[work->no];
}

static int fn_1_8468(M540_OBJ *obj)
{
    M540_CARTWORK *work = M540_OBJWORK(M540_CARTWORK, obj);
    return work->unk10;
}

static void fn_1_8484(M540_OBJ *obj, int playerIdx)
{
    M540_CARTWORK *work = M540_OBJWORK(M540_CARTWORK, obj);
    work->playerIdx = playerIdx;
}

static int fn_1_84A0(M540_OBJ *obj)
{
    M540_CARTWORK *work = M540_OBJWORK(M540_CARTWORK, obj);
    return work->playerIdx;
}

static void fn_1_84BC(M540_OBJ *obj)
{
    obj->mode = 0;
}

static void fn_1_84C8(M540_OBJ *obj)
{
    while(1) {
        M540_OBJ_SLEEP(1);
    }
}

static void fn_1_8570(M540_OBJ *obj)
{
    obj->mode = 1;
}

static BOOL fn_1_10E90(HuVecF pos, float scale);
static void fn_1_8B64(M540_OBJ *obj);

static void fn_1_857C(M540_OBJ *obj)
{
    M540_CARTWORK *work = M540_OBJWORK(M540_CARTWORK, obj);
    int i;
    int time;
    float step;
    HuVecF startPos;
    HuVecF pos;
    fn_1_134EC(obj, &startPos.x, &startPos.y, &startPos.z);
    fn_1_13874(obj, 0, 44.0f, 0.0f, HU3D_MOTATTR_NONE);
    time = 30;
    work->velStart.x = 8.0f;
    work->velStart.y = 4.0f;
    work->velStart.z = 0.0f;
    step = (30.0f-work->velStart.x)/time;
    for(i=0; i<time; i++) {
        work->vel.x = work->velStart.x+(step*i);
        work->vel.y = 0;
        work->vel.z = 0;
        fn_1_AC14(obj);
        M540_OBJ_SLEEP(1);
    }
    if(lbl_1_bss_DC != obj) {
        work->seNo = HuAudFXPlay(lbl_1_rodata_10C[work->no]);
    }
    for(i=0; ; i++) {
        fn_1_134EC(obj, &pos.x, &pos.y, &pos.z);
        if(pos.x-startPos.x >= 1000.0f) {
            break;
        }
        fn_1_AC14(obj);
        M540_OBJ_SLEEP(1);
    }
    if(lbl_1_bss_DC != obj) {
        fn_1_134EC(obj, &pos.x, &pos.y, &pos.z);
        pos.y += 10.0f;
        fn_1_10E90(pos, 100.0f);
    }
    fn_1_13874(obj, 1, 0, 0, HU3D_MOTATTR_LOOP);
    fn_1_82D0(obj);
    time = 60;
    for(i=0; i<time; i++) {
        work->vel.y = 4.0*HuSin((180.0f*(1.0f/time))*i);
        fn_1_AC14(obj);
        M540_OBJ_SLEEP(1);
    }
    work->vel.y = 0;
    work->unk74 = 2;
    fn_1_13964(obj);
    if(work->rider != NULL) {
        fn_1_13964(work->rider);
    }
    fn_1_8B64(obj);
}

static void fn_1_8B64(M540_OBJ *obj)
{
    obj->mode = 3;
}

static void fn_1_A190(M540_OBJ *obj);

static void fn_1_8B70(M540_OBJ *obj)
{
    M540_CARTWORK *work = M540_OBJWORK(M540_CARTWORK, obj);
    int timeA;
    int timeB;
    int timeC;
    int dirA;
    int dirB;
    int rnd;
    HuVecF pos;
    timeA = frandmod(60)+20;
    timeB = frandmod(60)+20;
    timeC = frandmod(60)+20;
    work->unk88 = 0;
    work->unk80 = 0;
    work->unk84 = 0;
    if(work->rider != NULL) {
        CharModelFxFlagSet(work->rider->charNo, FALSE);
        fn_1_13874(work->rider, 6, 0, 0, HU3D_MOTATTR_PAUSE);
    }
    do {
        fn_1_134EC(obj, &pos.x, &pos.y, &pos.z);
        if(lbl_1_bss_DC == obj) {
            work->unk74 = 2;
        } else {
            if(pos.x >= 3950.0f) {
                if(lbl_1_bss_E0[lbl_1_bss_20[3]] == obj) {
                    work->unk74 = 2;
                    work->unk7C = 1;
                    work->lane = 4;
                } else {
                    work->unk74 = 2;
                    work->unk7C = 2;
                    work->lane = 4;
                }
            } else if(pos.x < lbl_1_bss_34-900.0f) {
                work->unk74 = 0;
            } else if(pos.x < lbl_1_bss_34-250.0f) {
                work->unk74 = 1;
            } else if(pos.x > 300.0f+lbl_1_bss_34) {
                work->unk74 = 3;
            } else if(work->unk88++ > timeA) {
                rnd = frandmod(20);
                if(rnd > 10) {
                    work->unk74 = 2;
                } else if((rnd%2) == 0) {
                    work->unk74 = 1;
                } else {
                    work->unk74 = 3;
                }
                timeA = frandmod(60)+20;
                work->unk88 = 0;
            }
            if(lbl_1_bss_30 < 2) {
                if(work->unk84++ > timeC) {
                    if((int)frandmod(4) == 0) {
                        if((int)frandmod(2) == 0) {
                            dirA = 1;
                        } else {
                            dirA = -1;
                        }
                        work->unk7C += dirA;
                        if(work->unk7C < 0) {
                            work->unk7C = 0;
                        }
                        if(work->unk7C >= 2) {
                            work->unk7C = 2;
                        }
                    }
                    timeC = frandmod(60)+20;
                    work->unk84 = 0;
                }
                if(work->unk80++ > timeB && lbl_1_bss_30 < 2) {
                    if((int)frandmod(20) == 0) {
                        if((int)frandmod(2) == 0) {
                            dirB = 1;
                        } else {
                            dirB = -1;
                        }
                        work->lane += dirB;
                        if(work->lane < 0) {
                            work->lane = 0;
                        }
                        if(work->lane > 4) {
                            work->lane = 4;
                        }
                    }
                    timeB = frandmod(60)+30;
                    work->unk80 = 0;
                }
            }
        }
        fn_1_AC14(obj);
        M540_OBJ_SLEEP(1);
    } while(!(pos.x >= 7450.0f));
    fn_1_A190(obj);
}

static void fn_1_9008(M540_OBJ *obj, int arg1, int arg2)
{
    M540_CARTWORK *work = M540_OBJWORK(M540_CARTWORK, obj);
    work->unk70 = arg1;
    if(lbl_1_bss_E0[lbl_1_bss_20[3]] == obj) {
        arg2 = 0;
    }
    work->unk10 = arg2;
    if(arg2 == 1) {
        lbl_1_bss_1C--;
    }
    obj->mode = 4;
}

static s16 fn_1_10F44(HuVecF pos);
static s16 fn_1_110A8(HuVecF pos);
static int fn_1_111B8(int seId, HuVecF pos);
static void fn_1_A070(M540_OBJ *obj);
static void fn_1_AA24(M540_OBJ *obj);

static void fn_1_9080(M540_OBJ *obj)
{
    static const int lbl_1_rodata_888[CHARNO_MAX] = {
        0x1F9, 0x1E5, 0x25D, 0x2AD, 0x285, 0x1BD, 0x299, 0x1D1, 0x271, 0x235, 0x249, 0x221, 0x20D
    };
    M540_CARTWORK *work = M540_OBJWORK(M540_CARTWORK, obj);
    int i;
    int seNo;
    HuVecF pos;
    if(work->unk4 == 0) {
        fn_1_13874(obj, 2, 0, 0, HU3D_MOTATTR_NONE);
        fn_1_83A4(obj);
    }
    switch(work->unk70) {
        case 0:
        case 1:
            fn_1_134EC(obj, &pos.x, &pos.y, &pos.z);
            seNo = fn_1_111B8(0x802, pos);
            work->unk74 = 3;
            if(work->rider != NULL) {
                omVibrate(work->rider->playerNo, 90, 6, 6);
            }
            for(i=0; i<90; i++) {
                fn_1_134EC(obj, &pos.x, &pos.y, &pos.z);
                HuAudFXPanning(seNo, fn_1_110A8(pos));
                HuAudFXVolSet(seNo, fn_1_10F44(pos));
                if(pos.x < lbl_1_bss_34-250.0f && work->unk10 == 0) {
                    work->unk74 = 2;
                }
                fn_1_AC14(obj);
                M540_OBJ_SLEEP(1);
            }
            HuAudFXStop(seNo);
            break;
        case 2:
            if(work->rider != NULL) {
                omVibrate(work->rider->playerNo, 20, 4, 2);
            }
            break;
        case 3:
            if(work->rider != NULL) {
                omVibrate(work->rider->playerNo, 20, 12, 0);
            }
            break;
    }
    if(work->rider != NULL) {
        CharModelFxFlagSet(work->rider->charNo, FALSE);
        fn_1_13874(work->rider, 6, 0, 0, HU3D_MOTATTR_LOOP);
        fn_1_134EC(obj, &pos.x, &pos.y, &pos.z);
        fn_1_111B8(lbl_1_rodata_888[work->rider->charNo], pos);
    }
    switch(work->unk70) {
        case 0:
            fn_1_13874(obj, 3, 0, 0, HU3D_MOTATTR_NONE);
            work->unk4 = 1;
            break;
        case 1:
            fn_1_13874(obj, 5, 0, 0, HU3D_MOTATTR_NONE);
            break;
        case 2:
            fn_1_13874(obj, 7, 0, 0, HU3D_MOTATTR_NONE);
            break;
        case 3:
            fn_1_13874(obj, 9, 0, 0, HU3D_MOTATTR_NONE);
            break;
    }
    switch(work->unk70) {
        case 0:
            if(++work->unk7C > 3) {
                work->unk7C = 3;
            }
            if(work->rider != NULL) {
                omVibrate(work->rider->playerNo, 90, 4, 2);
            }
            seNo = HuAudFXPlay(0x804);
            for(i=0; i<90; i++) {
                fn_1_A070(obj);
                fn_1_134EC(obj, &pos.x, &pos.y, &pos.z);
                HuAudFXPanning(seNo, fn_1_110A8(pos));
                HuAudFXVolSet(seNo, fn_1_10F44(pos));
                M540_OBJ_SLEEP(1);
            }
            HuAudFXStop(seNo);
            if(work->unk10 == 0) {
                fn_1_13874(obj, 4, 0, 0, HU3D_MOTATTR_LOOP);
                if(--work->unk7C < 0) {
                    work->unk7C = 0;
                }
            }
            break;
        case 1:
            if((work->unk7C += 2) > 3) {
                work->unk7C = 3;
            }
            if(work->rider != NULL) {
                omVibrate(work->rider->playerNo, 120, 4, 2);
            }
            for(i=0; i<120; i++) {
                fn_1_A070(obj);
                M540_OBJ_SLEEP(1);
            }
            if(work->unk10 == 0) {
                fn_1_13874(obj, 6, 0, 0, HU3D_MOTATTR_NONE);
                if((work->unk7C -= 2) < 0) {
                    work->unk7C = 0;
                }
                seNo = HuAudFXPlay(0x804);
                for(i=0; i<60; i++) {
                    fn_1_A070(obj);
                    fn_1_134EC(obj, &pos.x, &pos.y, &pos.z);
                    HuAudFXPanning(seNo, fn_1_110A8(pos));
                    HuAudFXVolSet(seNo, fn_1_10F44(pos));
                    M540_OBJ_SLEEP(1);
                }
                HuAudFXStop(seNo);
                fn_1_13874(obj, 2, 0, 0, HU3D_MOTATTR_LOOP);
            }
            break;
        case 2:
            work->vel.x = 0;
            work->unk74 = 4;
            for(i=0; i<90; i++) {
                fn_1_A070(obj);
                M540_OBJ_SLEEP(1);
            }
            fn_1_13874(obj, 8, 0, 0, HU3D_MOTATTR_NONE);
            if(work->unk10 == 0) {
                for(i=0; i<60; i++) {
                    if(i == 20 && work->rider != NULL) {
                        omVibrate(work->rider->playerNo, 40, 4, 2);
                    }
                    fn_1_A070(obj);
                    M540_OBJ_SLEEP(1);
                }
                if((work->unk7C -= 2) < 0) {
                    work->unk7C = 0;
                }
                fn_1_13874(obj, 2, 0, 0, HU3D_MOTATTR_LOOP);
            }
            break;
        case 3:
            work->vel.x *= 0.25f;
            work->unk74 = 4;
            for(i=0; i<90; i++) {
                if(i == 20 && work->rider != NULL) {
                    omVibrate(work->rider->playerNo, 70, 4, 2);
                }
                fn_1_A070(obj);
                M540_OBJ_SLEEP(1);
            }
            if(work->unk10 == 0) {
                fn_1_13874(obj, 8, 0, 0, HU3D_MOTATTR_NONE);
                for(i=0; i<60; i++) {
                    fn_1_A070(obj);
                    M540_OBJ_SLEEP(1);
                }
                if((work->unk7C -= 2) < 0) {
                    work->unk7C = 0;
                }
                fn_1_13874(obj, 2, 0, 0, HU3D_MOTATTR_LOOP);
            }
            break;
    }
    if(work->unk10 != 0) {
        work->unk8C = --lbl_1_bss_18;
        OSReport("[RANK] %d st\n", work->unk8C);
        work->unk7C = 4;
        work->unk74 = 3;
        for(i=0; i<300; i++) {
            fn_1_A070(obj);
            M540_OBJ_SLEEP(1);
        }
        fn_1_136D4(obj);
        fn_1_AA24(obj);
    } else {
        obj->mode = 3;
    }
}

static void fn_1_A070(M540_OBJ *obj)
{
    M540_CARTWORK *work = M540_OBJWORK(M540_CARTWORK, obj);
    HuVecF pos;
    fn_1_134EC(obj, &pos.x, &pos.y, &pos.z);
    if(work->unk10 == 0) {
        switch(work->unk70) {
            case 0:
            case 1:
                if(pos.x < lbl_1_bss_34-250.0f) {
                    work->unk74 = 2;
                }
                break;
            case 2:
                if(pos.x < lbl_1_bss_34-750.0f) {
                    work->vel.x = lbl_1_rodata_90[2];
                    work->unk74 = 2;
                }
                break;
        }
    }
    fn_1_AC14(obj);
}

static void fn_1_A190(M540_OBJ *obj)
{
    obj->mode = 2;
}

static void fn_1_A19C(M540_OBJ *obj)
{
    M540_CARTWORK *work = M540_OBJWORK(M540_CARTWORK, obj);
    int i;
    int time;
    float step;
    HuVecF pos;
    time = 84;
    work->velStart = work->vel;
    step = (15.0f-work->velStart.x)/time;
    for(i=0; i<time; i++) {
        work->vel.x = work->velStart.x+(step*i);
        work->vel.y = 0;
        work->vel.z = 0;
        fn_1_AC14(obj);
        M540_OBJ_SLEEP(1);
    }
    if(obj != lbl_1_bss_DC) {
        fn_1_13934(obj);
        if(work->rider != NULL) {
            fn_1_13934(work->rider);
        }
    }
    switch(work->no) {
        case 0:
        case 1:
        case 3:
        case 4:
            fn_1_13874(obj, (work->unk4 != 0) ? 11 : 10, 0, 0, HU3D_MOTATTR_NONE);
            break;
    }
    time = 90;
    fn_1_134EC(obj, &pos.x, &pos.y, &pos.z);
    work->vel.y = -pos.y/time;
    for(i=0; i<time; i++) {
        fn_1_AC14(obj);
        M540_OBJ_SLEEP(1);
    }
    if(work->seNo != -1) {
        HuAudFXStop(work->seNo);
        work->seNo = -1;
    }
    if(lbl_1_bss_DC != obj) {
        fn_1_134EC(obj, &pos.x, &pos.y, &pos.z);
        pos.y += 10.0f;
        fn_1_10E90(pos, 150.0f);
    }
    if(work->rider != NULL) {
        omVibrate(work->rider->playerNo, 20, 4, 2);
    }
    fn_1_8364(obj);
    time = 60;
    work->velStart.x = work->vel.x;
    step = (6.0f-work->velStart.x)/time;
    for(i=0; i<time; i++) {
        work->vel.x = work->velStart.x+(step*i);
        work->vel.y = 6.0*HuSin((360.0f*(1.0f/time))*i);
        fn_1_AC14(obj);
        if(i == time/2 && lbl_1_bss_DC != obj) {
            lbl_1_bss_44 = 1;
            if(work->rider != NULL) {
                lbl_1_bss_40 = work->rider;
            }
        }
        M540_OBJ_SLEEP(1);
    }
    work->vel.y = 0;
    if(lbl_1_bss_DC != obj) {
        fn_1_134EC(obj, &pos.x, &pos.y, &pos.z);
        pos.y += 10.0f;
        fn_1_10E90(pos, 200.0f);
    }
    if(work->rider != NULL) {
        omVibrate(work->rider->playerNo, 60, 6, 6);
    }
    time = 60;
    work->velStart.x = work->vel.x;
    step = -work->velStart.x/time;
    for(i=0; i<time; i++) {
        work->vel.x = work->velStart.x+(step*i);
        fn_1_AC14(obj);
        M540_OBJ_SLEEP(1);
    }
    work->vel.x = 0;
    fn_1_137EC(obj);
    fn_1_8364(obj);
    obj->mode = 0;
}

static void fn_1_AA24(M540_OBJ *obj)
{
    obj->mode = 5;
}

static void fn_1_AA30(M540_OBJ *obj)
{
    M540_CARTWORK *work = M540_OBJWORK(M540_CARTWORK, obj);
    work->vel.x = 0;
    work->vel.z = 0;
    work->unk74 = 4;
    work->unk7C = 4;
    if(work->seNo != -1) {
        HuAudFXStop(work->seNo);
        work->seNo = -1;
    }
    while(1) {
        fn_1_A070(obj);
        M540_OBJ_SLEEP(1);
    }
}

static BOOL fn_1_B600(M540_OBJ *obj);

static void fn_1_AC14(M540_OBJ *obj)
{
    M540_CARTWORK *work = M540_OBJWORK(M540_CARTWORK, obj);
    float targetX;
    float targetY;
    float targetZ;
    HuVecF pos;
    fn_1_134EC(obj, &pos.x, &pos.y, &pos.z);
    pos.x += work->vel.x;
    pos.y += work->vel.y;
    pos.z += work->vel.z;
    if(pos.y < -3000.0f) {
        pos.y = -3000.0f;
    }
    if(pos.x < -12000.0f) {
        pos.x = -12000.0f;
    }
    if(pos.x > 12000.0f) {
        pos.x = 12000.0f;
    }
    if(pos.z < -2000.0f) {
        pos.z = -2000.0f;
    }
    if(pos.z > 2000.0f) {
        pos.z = 2000.0f;
    }
    fn_1_8094(obj, pos.x, pos.y, pos.z);
    switch(obj->mode) {
        case 3:
        case 4:
            if(fn_1_B600(obj)) {
                if(work->unk94 == 0) {
                    fn_1_111B8(0x801, pos);
                    work->unk94 = 60;
                }
                if(work->vel.z > 0.0f) {
                    work->lane++;
                } else if(work->vel.z < 0.0f) {
                    work->lane--;
                } else if(work->vel.y > 0.0f) {
                    work->unk7C--;
                } else if(work->vel.y < 0.0f) {
                    work->unk7C++;
                }
                if(work->lane < 0) {
                    work->lane = 0;
                }
                if(work->lane > 4) {
                    work->lane = 4;
                }
                if(work->unk7C < 0) {
                    work->unk7C = 0;
                }
                if(work->unk7C > 4) {
                    work->unk7C = 4;
                }
                pos.x -= work->vel.x;
                pos.y -= work->vel.y;
                pos.z -= work->vel.z;
                if(lbl_1_bss_30 < 1 || lbl_1_bss_E0[lbl_1_bss_20[3]] != obj) {
                    work->vel.x *= 0.25f;
                    work->vel.y *= 0.25f;
                    work->vel.z *= 0.25f;
                }
                fn_1_8094(obj, pos.x, pos.y, pos.z);
            }
            targetX = lbl_1_rodata_90[work->unk74];
            if(targetX < work->vel.x) {
                if((work->vel.x -= 0.2f) < targetX) {
                    work->vel.x = targetX;
                }
            }
            if(targetX > work->vel.x) {
                if((work->vel.x += 0.2f) > targetX) {
                    work->vel.x = targetX;
                }
            }
            work->vel.y = 0;
            targetY = lbl_1_rodata_B8[work->unk7C];
            if(pos.y-6.0f >= targetY) {
                work->vel.y = -6.0f;
            }
            if(pos.y+6.0f <= targetY) {
                work->vel.y = 6.0f;
            }
            work->vel.z = 0;
            targetZ = lbl_1_rodata_A4[work->lane];
            if(pos.z-4.0f >= targetZ) {
                work->vel.z = -4.0f;
            }
            if(pos.z+4.0f <= targetZ) {
                work->vel.z = 4.0f;
            }
            break;
    }
    if(work->unk94 > 0) {
        work->unk94--;
    }
    if(work->seNo != -1) {
        HuAudFXPanning(work->seNo, fn_1_110A8(pos));
        HuAudFXVolSet(work->seNo, fn_1_10F44(pos));
    }
}

static void fn_1_FCB8(M540_OBJ *obj);

static BOOL fn_1_B600(M540_OBJ *obj)
{
    static const int lbl_1_rodata_8F0[5] = { -20, 0, 0, 0, 0 };
    BOOL result;
    int i;
    M540_OBJ *other;
    M540_CARTWORK *work;
    M540_OBJ *item;
    MathSphere sphereA;
    MathSphere sphereB;
    Mtx mtx;
    result = FALSE;
    if(lbl_1_bss_DC == obj) {
        return FALSE;
    }
    Hu3DModelObjMtxGet(obj->modelId, fn_1_8438(obj), mtx);
    work = M540_OBJWORK(M540_CARTWORK, obj);
    sphereA.center.x = mtx[0][3]+lbl_1_rodata_8F0[work->no];
    sphereA.center.y = mtx[1][3];
    sphereA.center.z = mtx[2][3];
    sphereA.radius = 140.0f;
    if(!(sphereA.center.x > lbl_1_bss_34-1500.0f && sphereA.center.x < 1500.0f+lbl_1_bss_34)) {
        return FALSE;
    }
    for(i=0; i<4; i++) {
        other = fn_1_CB34(lbl_1_bss_CC[i]);
        if(other != obj) {
            Hu3DModelObjMtxGet(other->modelId, fn_1_8438(other), mtx);
            work = M540_OBJWORK(M540_CARTWORK, obj);
            sphereB.center.x = mtx[0][3]+lbl_1_rodata_8F0[work->no];
            sphereB.center.y = mtx[1][3];
            sphereB.center.z = mtx[2][3];
            sphereB.radius = 140.0f;
            if(MathSphereCheck(&sphereA, &sphereB)) {
                if(sphereB.center.y > sphereA.center.y) {
                    if(work->unk7C < 4) {
                        work->unk84 = 100;
                        work->unk7C++;
                    }
                } else if(sphereB.center.y < sphereA.center.y && work->unk7C > 0) {
                    work->unk84 = 100;
                    work->unk7C--;
                }
                if(obj->mode == 3) {
                    if(lbl_1_bss_E0[lbl_1_bss_20[3]] != obj) {
                        fn_1_9008(obj, 1, 0);
                    } else if(lbl_1_bss_30 < 1) {
                        fn_1_9008(obj, 0, 0);
                    }
                }
                result = TRUE;
                return result;
            }
        }
    }
    sphereB.radius = 50.0f;
    for(i=0; i<4; i++) {
        item = lbl_1_bss_70[i];
        if(item->mode == 1) {
            fn_1_134EC(item, &sphereB.center.x, &sphereB.center.y, &sphereB.center.z);
            if(MathSphereCheck(&sphereA, &sphereB)) {
                if(obj->mode == 3) {
                    if(lbl_1_bss_E0[lbl_1_bss_20[3]] == obj) {
                        fn_1_9008(obj, 3, 0);
                    } else {
                        fn_1_9008(obj, 3, 1);
                    }
                }
                fn_1_FCB8(item);
                result = TRUE;
            }
        }
    }
    return result;
}

static s16 fn_1_BCD4(M540_OBJ *obj)
{
    M540_CARTWORK *work = M540_OBJWORK(M540_CARTWORK, obj);
    return work->unk8C;
}

static void fn_1_BE68(M540_OBJ *obj);
static void fn_1_BF4C(M540_OBJ *obj);
static void fn_1_C800(M540_OBJ *obj);
static void fn_1_CB5C(M540_OBJ *obj);
static void fn_1_D0A8(M540_OBJ *obj);
static void fn_1_CA30(M540_OBJ *obj);
static void fn_1_D2E8(M540_OBJ *obj);
static void fn_1_D3F8(M540_OBJ *obj);

static M540_OBJFUNC lbl_1_data_D8[] = {
    fn_1_BE68,
    fn_1_BF4C,
    fn_1_C800,
    fn_1_CB5C,
    fn_1_D0A8,
    fn_1_CA30,
    fn_1_D2E8,
    fn_1_D3F8
};

static M540_OBJ *fn_1_BCF0(int playerNo)
{
    M540_OBJ *obj;
    M540_PLAYERWORK *work;
    obj = fn_1_13A40(0x1001, 0x2000, playerNo, 7, (u32 *)lbl_1_rodata_120, 6, lbl_1_data_D8, 8);
    Hu3DModelCameraSet(obj->modelId, HU3D_CAM0|HU3D_CAM2);
    CharEffectLayerSet(1);
    CharMotionUpdateSet(obj->charNo, ((u32 *)lbl_1_rodata_120)[(obj->charNo*7)+6], FALSE);
    fn_1_13698(obj);
    fn_1_13934(obj);
    fn_1_13774(obj, TRUE);
    work = M540_OBJWORK(M540_PLAYERWORK, obj);
    work->vel.x = 0;
    work->vel.y = 0;
    work->vel.z = 0;
    work->cart = NULL;
    fn_1_13874(obj, 0, 0, 0, HU3D_MOTATTR_LOOP);
    obj->mode = 0;
    return obj;
}

static void fn_1_BE5C(M540_OBJ *obj)
{
    obj->mode = 0;
}

static void fn_1_BE68(M540_OBJ *obj)
{
    while(1) {
        M540_OBJ_SLEEP(1);
    }
}

static void fn_1_BF10(M540_OBJ *obj, HuVecF *target, int time)
{
    M540_PLAYERWORK *work = M540_OBJWORK(M540_PLAYERWORK, obj);
    work->target = *target;
    work->time = time;
    obj->mode = 1;
}

static void fn_1_C2CC(M540_OBJ *obj);

static void fn_1_BF4C(M540_OBJ *obj)
{
    M540_PLAYERWORK *work = M540_OBJWORK(M540_PLAYERWORK, obj);
    HuVecF pos;
    MathCircle circleA;
    MathCircle circleB;
    fn_1_134EC(obj, &pos.x, &pos.y, &pos.z);
    work->vel.x = (work->target.x-pos.x)/work->time;
    work->vel.z = (work->target.z-pos.z)/work->time;
    work->vel.y = 0;
    while(1) {
        fn_1_C2CC(obj);
        fn_1_134EC(obj, &pos.x, &pos.y, &pos.z);
        circleA.center.x = work->target.x;
        circleA.center.y = work->target.z;
        circleA.radius = 10.0f;
        circleB.center.x = pos.x;
        circleB.center.y = pos.z;
        circleB.radius = (work->vel.x > work->vel.z) ? work->vel.x : work->vel.z;
        if(MathCircleCircleCheck(&circleA, &circleB)) {
            break;
        }
        M540_OBJ_SLEEP(1);
    }
    work->vel.x = 0;
    work->vel.y = 0;
    work->vel.z = 0;
    fn_1_13498(obj, work->target.x, work->target.y, work->target.z);
    fn_1_13874(obj, 0, 0, 8.0f, HU3D_MOTATTR_LOOP);
    if(obj->mode == 1) {
        obj->mode = 0;
    }
}

static void fn_1_C2CC(M540_OBJ *obj)
{
    M540_PLAYERWORK *work = M540_OBJWORK(M540_PLAYERWORK, obj);
    int motNo;
    float speed;
    HuVecF pos;
    HuVecF rot;
    fn_1_134EC(obj, &pos.x, &pos.y, &pos.z);
    fn_1_13580(obj, &rot.x, &rot.y, &rot.z);
    motNo = 0;
    speed = sqrtf(HuSquare(work->vel.x)+HuSquare(work->vel.z));
    if(speed >= 0.4f) {
        if(speed >= 10.0f) {
            work->vel.x /= speed;
            work->vel.z /= speed;
            speed = 10.0f;
            work->vel.x *= speed;
            work->vel.z *= speed;
        }
        rot.y = fn_1_15444(rot.y, HuAtan(work->vel.x, work->vel.z), 0.4f);
        if(speed >= 8.0f) {
            motNo = 2;
        } else {
            motNo = 1;
        }
    }
    pos.x += work->vel.x;
    pos.z += work->vel.z;
    pos.y += work->vel.y;
    fn_1_13498(obj, pos.x, pos.y, pos.z);
    fn_1_1352C(obj, rot.x, rot.y, rot.z);
    if(motNo != fn_1_138F0(obj)) {
        fn_1_13874(obj, motNo, 0, 8.0f, HU3D_MOTATTR_LOOP);
    }
    CharModelVoicePanSet(obj->charNo, fn_1_110A8(pos));
}

static void fn_1_C7DC(M540_OBJ *obj, int order)
{
    M540_PLAYERWORK *work = M540_OBJWORK(M540_PLAYERWORK, obj);
    work->order = order;
    obj->mode = 2;
}

static void fn_1_C800(M540_OBJ *obj)
{
    M540_PLAYERWORK *work = M540_OBJWORK(M540_PLAYERWORK, obj);
    work->target.x = -13550.0f+(200.0f*work->order);
    work->target.y = 0;
    work->target.z = 500.0f;
    work->time = 120;
    fn_1_BF4C(obj);
    work->target.z = 470.0f;
    work->time = 10;
    fn_1_BF4C(obj);
    obj->mode = 0;
}

static void fn_1_C8D8(M540_OBJ *obj, M540_OBJ *cart)
{
    M540_CARTWORK *cartWork = M540_OBJWORK(M540_CARTWORK, cart);
    M540_PLAYERWORK *work = M540_OBJWORK(M540_PLAYERWORK, obj);
    fn_1_13498(obj, 0, 0, 0);
    work->cart = cart;
    cartWork->rider = obj;
    CharMotionVoiceOnSet(obj->charNo, ((u32 *)lbl_1_rodata_120)[(obj->charNo*7)+6], FALSE);
    fn_1_13874(obj, 6, 0, 0, HU3D_MOTATTR_PAUSE);
    fn_1_139C8(cart, fn_1_8438(cart), obj);
    fn_1_1352C(obj, 0, 90.0f, 0);
    obj->mode = 5;
}

static void fn_1_CA30(M540_OBJ *obj)
{
    float ofsY;
    M540_OBJ *cart;
    float rotY;
    ofsY = 0;
    rotY = 90.0f;
    if(obj->charNo == CHARNO_TERESA) {
        ofsY = 20.0f;
    }
    cart = fn_1_CB34(obj);
    while(1) {
        M540_OBJ_SLEEP(1);
    }
}

static M540_OBJ *fn_1_CB34(M540_OBJ *obj)
{
    M540_PLAYERWORK *work = M540_OBJWORK(M540_PLAYERWORK, obj);
    return work->cart;
}

static void fn_1_CB50(M540_OBJ *obj)
{
    obj->mode = 3;
}

static void fn_1_CB5C(M540_OBJ *obj)
{
    M540_PLAYERWORK *work;
    M540_OBJ *cart;
    float t;
    float s;
    HuVecF pos;
    HuVecF start;
    HuVecF end;
    float rotY;
    float startRotY;
    float endRotY;
    Mtx mtx;
    CharModelFxFlagSet(obj->charNo, TRUE);
    work = M540_OBJWORK(M540_PLAYERWORK, obj);
    cart = work->cart;
    Hu3DModelObjMtxGet(cart->modelId, fn_1_8438(cart), mtx);
    pos.x = mtx[0][3];
    pos.y = mtx[1][3];
    pos.z = mtx[2][3];
    fn_1_13A10(cart);
    fn_1_13498(obj, pos.x, pos.y, pos.z);
    end.x = lbl_1_bss_38;
    end.z = 220.0f+mtx[2][3];
    end.y = 0;
    endRotY = 0;
    fn_1_13874(obj, 3, 0, 8.0f, HU3D_MOTATTR_NONE);
    fn_1_1352C(obj, 0, 45.0f, 0);
    t = 0;
    start = pos;
    startRotY = 45.0f;
    while(1) {
        s = HuSin(180.0f*t);
        s = s*s;
        pos.x = start.x+(t*(end.x-start.x));
        pos.z = start.z+(t*(end.z-start.z));
        pos.y = start.y+(180.0f*s);
        rotY = startRotY+(t*(endRotY-startRotY));
        if(t < 0.5f && pos.y < start.y) {
            pos.y = start.y;
        }
        fn_1_1352C(obj, 0, rotY, 0);
        fn_1_13498(obj, pos.x, pos.y, pos.z);
        t += 0.04f;
        if(t >= 1.0f) {
            break;
        }
        M540_OBJ_SLEEP(1);
    }
    pos.y = 0;
    fn_1_13498(obj, pos.x, pos.y, pos.z);
    fn_1_13874(obj, 4, 0, 8.0f, HU3D_MOTATTR_NONE);
    M540_OBJ_SLEEP(20);
    fn_1_13874(obj, 0, 0, 8.0f, HU3D_MOTATTR_NONE);
    obj->mode = 0;
}

static void fn_1_D09C(M540_OBJ *obj)
{
    obj->mode = 4;
}

static void fn_1_D0A8(M540_OBJ *obj)
{
    CharModelFxFlagSet(obj->charNo, TRUE);
    fn_1_13874(obj, 5, 0, 8.0f, HU3D_MOTATTR_NONE);
    while(1) {
        M540_OBJ_SLEEP(1);
    }
}

static M540_OBJ *fn_1_D1A0(void)
{
    static const u32 lbl_1_rodata_924[3] = {
        DATANUM(DATA_m540, 0x0D),
        DATANUM(DATA_m540, 0x0E),
        DATANUM(DATA_m540, 0x0F)
    };
    M540_OBJ *obj = fn_1_13174(0x1000, 0x2000, DATANUM(DATA_m540, 0x0C), 3, (u32 *)lbl_1_rodata_924, 1, NULL);
    Hu3DModelCameraSet(obj->modelId, HU3D_CAM2);
    fn_1_13698(obj);
    fn_1_13934(obj);
    fn_1_13774(obj, TRUE);
    fn_1_13874(obj, 0, 0, 0, HU3D_MOTATTR_LOOP);
    return obj;
}

static void fn_1_D280(M540_OBJ *obj)
{
    fn_1_13874(obj, 0, 0, 8.0f, HU3D_MOTATTR_LOOP);
    obj->mode = 0;
}

static void fn_1_D2E8(M540_OBJ *obj)
{
    while(1) {
        M540_OBJ_SLEEP(1);
    }
}

static void fn_1_D390(M540_OBJ *obj)
{
    fn_1_13874(obj, 2, 0, 8.0f, HU3D_MOTATTR_LOOP);
    obj->mode = 1;
}

static void fn_1_D3F8(M540_OBJ *obj)
{
    while(1) {
        M540_OBJ_SLEEP(1);
    }
}

/* M540_APPEND */
