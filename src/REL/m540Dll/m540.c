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
    void *unk4;               /* 0x04 */
    BOOL effectF;             /* 0x08 */
    BOOL effectDispF;         /* 0x0C */
    int unk10;                /* 0x10 */
    float speedMin;           /* 0x14 */
    float unk18;              /* 0x18 */
    float unk1C;              /* 0x1C */
    float speed;              /* 0x20 */
    float unk24;              /* 0x24 */
    float unk28;              /* 0x28 */
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

/* M540_APPEND */
