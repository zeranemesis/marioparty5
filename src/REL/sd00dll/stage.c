#include "game/object.h"
#include "game/mg/timer.h"
#include "REL/sd00dll_sys.h"
#include "game/audio.h"
#include "game/data.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/mg/seqman.h"
#include <string.h>

extern s32 rand8(void);

/* Parts of this unit (stage_b..e.inc) are included at the end of this file, so every
   static below (data + function prototypes) is shared by all parts. */

#define SD_COLMESH_W 0x60  /* stageColMesh cells: 96 (x, 50 units) * 48 (z, 100 units) */
#define SD_COLMESH_H 0x30
#define SD_COLMESH_NUM (SD_COLMESH_W * SD_COLMESH_H)
#define SD_TRIALMAP_W 0x31 /* stageTrialMapHeight / stageTrialMapNum: 49 x 49 cells */

/* stageWork (0x20 bytes on GC): lava/hamon texture state */
typedef struct SdStageWork_s {
    s32 frame;             // 0x00
    f32 lavaY;             // 0x04 (trial stage: lava model height, 90.0)
    f32 *uv[4];            // 0x08 (lava model ST coords)
    HSFMATERIAL *material; // 0x18 (lava model material)
    s32 pad1C;             // 0x1C
} SdStageWork;

/* ---- functions of the other parts of this unit (all static in the original) ---- */
static void StageDokanUpdate(SdStageObj *obj);
static void StageBomheiUpdate(SdStageObj *obj);
static void StageBomheiBigUpdate(SdStageObj *obj);
static void StageTogezoUpdate(SdStageObj *obj);
static void StageKinokoUpdate(SdStageObj *obj);
static void StageNKinokoUpdate(SdStageObj *obj);
static void StageDataClose(void);
static void MakeStageLight(void);
static void StageHamonCreate(void);
static void StageHamonLayerHook(s16 layerNo);
static void StageHamonPosSet(Vec *a, Vec *b);
static void StageHamonSpeedSet(f32 speed);
static void StageHamonMatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material);
static void StageTrialMakeGround(void);
static void MakeTrialStageMesh(void);

/* ---- part A functions ---- */
static void StageBackObjInit(void);
static void StageShadowInit(void);
static void ReadFace(s32 idx, HSFFACE *face, HSFBUFFER *vertex, s8 *normal, Vec *vtx[3], s8 *nrm[3]);
static void MakeStage(s32 stageNo, s32 smooth, f32 height);
static void MakeTrialStage(s32 smooth, f32 height);
static void MakeMainStage(void);

/* ===================== data ===================== */

/* .data */
static HU3DMODELID trialModelId[4] = { -1, -1, -1, -1 };

static SdStageData stageDataTbl[8] = {
    { 0x00BC0000, 0x00BC000A, 0, 2350.0f, 2350.0f },
    { 0x00BC0002, 0x00BC000B, 1, 3500.0f, 3500.0f },
    { 0x00BC0003, 0x00BC000C, 1, 3000.0f, 3000.0f },
    { 0x00BC0004, 0x00BC000D, 1, 4200.0f, 4200.0f },
    { 0x00BC0005, 0x00BC000E, 1, 3000.0f, 3000.0f },
    { 0x00BC0006, 0x00BC000F, 1, 2950.0f, 2950.0f },
    { 0x00BC0007, 0x00BC0010, 1, 2500.0f, 2500.0f },
    { 0x00BC0008, 0x00BC0011, 2, 3000.0f, 3000.0f },
};

static s32 objModelFile[6] = { 0x00BC0060, 0x00BC0061, 0x00BC0064, 0x00BC0067, 0x00BC006A, 0x00BC006B };
static s32 objBomheiMotTbl[2] = { 0x00BC0062, 0x00BC0063 };
static s32 objBomheiBigMotTbl[2] = { 0x00BC0065, 0x00BC0066 };

static SdStageObjType objDataTbl[6] = {
    { 1, 0, 0.0f, 0, 150.0f, 300.0f, 150.0f, 0, 0, 0.0f, 0 },
    { 0, 0, 80.0f, 0, 60.0f, 60.0f, 60.0f, 300, 300, 450.0f, 2 },
    { 0, 0, 100.0f, 0, 70.0f, 70.0f, 70.0f, 600, 600, 750.0f, 3 },
    { 0, 0, 0.0f, 0, 68.0f, 68.0f, 68.0f, 0, 0, 0.0f, 1 },
    { 1, 0, 0.0f, 0, 100.0f, 160.0f, 100.0f, 0, 0, 0.0f, 0 },
    { 1, 0, 0.0f, 0, 75.0f, 120.0f, 75.0f, 0, 0, 0.0f, 0 },
};

/* background object names (NULL terminated) + per-object rotation scale */
static char *stage1Obj[] = { "sky", NULL };
static f32 stage1ObjScale[] = { 1.25f };
static char *stage2Obj[] = { "sky", "cloud", "hill", "mt", NULL };
static f32 stage2ObjScale[] = { 1.25f, 1.1f, 0.025f, 0.0125f };
static char *stage3Obj[] = { "sky", "moon", "s01", "s02", "s03", "s04", "s05", "woods01", "woods02", "woods03", NULL };
static f32 stage3ObjScale[] = { 1.25f, 1.25f, 1.25f, 1.25f, 1.25f, 1.25f, 1.25f, 0.1f, 0.05f, 0.025f };
static char *stage4Obj[] = { "sky", "cloud", "mt1", "mt2", NULL };
static f32 stage4ObjScale[] = { 1.25f, 1.0f, 0.3f, 0.2f };
static char *stage5Obj[] = { "sky", "mt", "cloud", NULL };
static f32 stage5ObjScale[] = { 1.15f, 1.0f, 0.5f };
static char *stage6Obj[] = { "sky", "palm01", "palm02", NULL };
static f32 stage6ObjScale[] = { 0.5f, 0.15f, 0.1f };
static char *stage7Obj[] = { "cosmos", "gus", "earth_add", "earth_mat", "moonbase", "antena1", "antena2", "antena3", "antena4", "pipeline", NULL };
static f32 stage7ObjScale[] = { 1.25f, 1.15f, 1.0f, 1.0f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f };
static char *stage8Obj[] = { "sky", "mt", "fp1", "fp2", "cloud", NULL };
static f32 stage8ObjScale[] = { 1.25f, 1.1f, 1.0f, 0.75f, 1.15f };

static char **stageObjTbl[8] = { stage1Obj, stage2Obj, stage3Obj, stage4Obj, stage5Obj, stage6Obj, stage7Obj, stage8Obj };
static f32 *stageObjScaleTbl[8] = { stage1ObjScale, stage2ObjScale, stage3ObjScale, stage4ObjScale,
                                    stage5ObjScale, stage6ObjScale, stage7ObjScale, stage8ObjScale };

/* shadow-casting object names */
static char *stage1ShadowObj[] = { "grid1", NULL };
static char *stage2ShadowObj[] = { "y01", NULL };
static char *stage3ShadowObj[] = { "y01", "y02", "y03", NULL };
static char *stage4ShadowObj[] = { "y01", "y02", "y03", "y04", NULL };
static char *stage5ShadowObj[] = { "y01", NULL };
static char *stage6ShadowObj[] = { "y01", "y02", "y03", NULL };
static char *stage7ShadowObj[] = { "y01", "y02", NULL };
static char *stage8ShadowObj[] = { "y01", NULL };

static char **stageShadowObjTbl[8] = { stage1ShadowObj, stage2ShadowObj, stage3ShadowObj, stage4ShadowObj,
                                       stage5ShadowObj, stage6ShadowObj, stage7ShadowObj, stage8ShadowObj };

/* (lbl_1_data_1864 = "itemhook_c" is a plain string literal in its user) */
static void (*objUpdateFuncTbl[6])(SdStageObj *) = {
    StageDokanUpdate, StageBomheiUpdate, StageBomheiBigUpdate, StageTogezoUpdate, StageKinokoUpdate, StageNKinokoUpdate,
};

static Vec shadowCamPos = { 0.0f, 6000.0f, 0.0f };
static Vec shadowCamUp = { 0.0f, 0.0f, -1.0f };
static Vec shadowCamTarget = { 0.0f, 0.0f, 0.0f };

/* original symbol lbl_1_data_18AC: stage 7 smoke emitter positions */
static Vec stage07SmokePosTbl[20] = {
    { -250.0f, -0.0f, 3750.0f }, { -230.0f, -0.0f, 3710.0f }, { -210.0f, -0.0f, 3680.0f }, { -195.0f, -0.0f, 3655.0f },
    { -180.0f, -0.0f, 3630.0f }, { -150.0f, -0.0f, 3590.0f }, { -120.0f, -0.0f, 3550.0f }, { -80.0f, -0.0f, 3550.0f },
    { -45.0f, -0.0f, 3550.0f },  { -20.0f, -0.0f, 3550.0f },  { 250.0f, -0.0f, 3750.0f },  { 230.0f, -0.0f, 3710.0f },
    { 210.0f, -0.0f, 3680.0f },  { 195.0f, -0.0f, 3655.0f },  { 180.0f, -0.0f, 3630.0f },  { 150.0f, -0.0f, 3590.0f },
    { 120.0f, -0.0f, 3550.0f },  { 80.0f, -0.0f, 3550.0f },   { 45.0f, -0.0f, 3550.0f },   { 20.0f, -0.0f, 3550.0f },
};

static s32 auxATbl[8] = { 0, 0, 0, 0, 3, 3, 0, 4 };

/* .bss */
static u8 *textureBackupPtr;
static SdStageSmoke stage07Smoke[20];
static u8 *texturePtr;
static s32 stageTrialLavaFloorOnF;
static s32 stageTrialClearOff;
static s32 stageTrialNoiseDec;
static s32 stageTrialNoiseY;
static s32 stageTrialNoiseX;
static s32 stageTrialNoiseNum;
static s32 stageTrialSmooth;
static f32 stageTrialHeight;
static HU3DMODELID stageLavaModelId;
static s32 stageColWallNum;
static s32 stageColFloorNum;
static SdStageColTri *stageColCurTriangle;
static SdStageColTri *stageColWall;
static SdStageColTri *stageColMesh;
static SdStageTrialCell *stageTrialMapNum;
static SdStageTrialCell *stageTrialMapHeight;
static u8 *stageTrialMapColType;
static u8 *stageTrialMap;
static SdStageWork stageWork;

/* ===================== functions ===================== */

static s32 StageClamp(s32 v, s32 lo, s32 hi)
{
    if (v > hi) {
        return hi;
    }
    if (v < lo) {
        return lo;
    }
    return v;
}

/* index of the face buffer called name (hsf->faceNum if missing) */
static s32 StageFaceFind(HSFDATA *hsf, char *name)
{
    s32 i;

    for (i = 0; i < hsf->faceNum; i++) {
        if (strlen(hsf->face[i].name) == strlen(name) && memcmp(name, hsf->face[i].name, strlen(name)) == 0) {
            break;
        }
    }
    return i;
}

/* index of name inside the NULL terminated list (index of the terminator if missing) */
static s32 StageNameFind(char **names, char *name)
{
    s32 i;

    for (i = 0; names[i] != NULL; i++) {
        if (strlen(names[i]) == strlen(name) && memcmp(names[i], name, strlen(names[i])) == 0) {
            break;
        }
    }
    return i;
}

/* normalize; a zero vector becomes (1, 1, 1) first */
static void StageVecNormalize(Vec *v)
{
    if (PSVECMag(v) == 0.0f) {
        v->x = 1.0f;
        v->y = 1.0f;
        v->z = 1.0f;
    }
    PSVECNormalize(v, v);
}

void SdStageInit(s32 stageNo)
{
    stageTrialMap = NULL;
    stageTrialMapColType = NULL;
    stageColWall = NULL;
    stageColMesh = HuMemDirectMallocNum(HEAP_MODEL, SD_COLMESH_NUM * sizeof(SdStageColTri), HU_MEMNUM_OVL);
    SdStageCreate(stageNo);
    MakeStage(stageNo, 5, 75.0f);
}

void SdStageCreate(s32 stageNo)
{
    Vec hamonPosA = { 0.0f, 0.0f, 0.03f };
    Vec hamonPosB = { 0.0f, 0.0f, 0.03f };
    Vec trialPos = { -10000.0f, 0.0f, 0.0f };
    HSFDATA *hsf;
    s32 i;

    if (stageColWall != NULL) {
        HuMemDirectFree(stageColWall);
        stageColWall = NULL;
    }
    if (stageTrialMapColType != NULL) {
        HuMemDirectFree(stageTrialMapColType);
        stageTrialMapColType = NULL;
    }
    if (stageTrialMap != NULL) {
        HuMemDirectFree(stageTrialMap);
        stageTrialMap = NULL;
    }
    texturePtr = NULL;
    for (i = 0; i < 5; i++) {
        sys->trialPos[i] = trialPos;
    }
    stageLavaModelId = -1;
    sys->hamonMdlId[1] = -1;
    sys->hamonMdlId[0] = -1;
    sys->backMdlId[1] = -1;
    sys->backMdlId[0] = -1;
    sys->shadowMdlId = -1;
    sys->stageMdlId = -1;
    sys->stageNo = stageNo;
    sys->stageMdlId = Hu3DModelCreateData(stageDataTbl[sys->stageNo].groundFile);
    if (sys->stageNo == 2) {
        sys->hamonMdlId[0] = Hu3DModelCreateData(0x00BC0009);
        sys->hamonMdlId[1] = Hu3DModelLink(sys->hamonMdlId[0]);
        StageHamonCreate();
        Hu3DLayerHookSet(14, StageHamonLayerHook);
        StageHamonPosSet(&hamonPosB, &hamonPosA);
        StageHamonSpeedSet(-0.005f);
        for (i = 0; i < 2; i++) {
            Hu3DCameraCreate(sdCameraBitTbl[i + 2]);
            Hu3DCameraViewportSet(sdCameraBitTbl[i + 2], 320.0f * i, 0.0f, 320.0f, 480.0f, 0.0f, 1.0f);
            Hu3DCameraScissorSet(sdCameraBitTbl[i + 2], (u32)(320.0f * i), 0, 320, 480);
            Hu3DCameraPerspectiveSet(sdCameraBitTbl[i + 2], 60.0f, 20.0f, 35000.0f, 0.6f);
            Hu3DModelCameraSet(sys->hamonMdlId[i], (u16)sdCameraBitTbl[i + 2]);
            Hu3DModelLayerSet(sys->hamonMdlId[i], 7);
            Hu3DModelMatHookSet(sys->hamonMdlId[i], StageHamonMatHook);
        }
    }
    sys->backMdlId[0] = Hu3DModelCreateData(stageDataTbl[sys->stageNo].objFile);
    sys->backMdlId[1] = Hu3DModelLink(sys->backMdlId[0]);
    if (sys->stageNo == 1) {
        sys->shadowMdlId = Hu3DModelCreateData(0x00BC0001);
        Hu3DModelAttrSet(sys->shadowMdlId, 0x40000001);
        Hu3DModelLayerSet(sys->shadowMdlId, 0);
        Hu3DModelLayerSet(sys->stageMdlId, 1);
    } else {
        Hu3DModelLayerSet(sys->stageMdlId, 0);
    }
    for (i = 0; i < 2; i++) {
        Hu3DModelCameraSet(sys->backMdlId[i], (u16)sdCameraBitTbl[i]);
        Hu3DModelLayerSet(sys->backMdlId[i], 0);
    }
    StageShadowInit();
    StageBackObjInit();
    if (sys->stageNo == 2 || sys->stageNo == 7) {
        Hu3DModelAmbSet(sys->stageMdlId, 0.1f, 0.1f, 0.1f);
    } else if (sys->stageNo == 6) {
        Hu3DModelAmbSet(sys->stageMdlId, 1.0f, 1.0f, 1.0f);
    } else {
        Hu3DModelAmbSet(sys->stageMdlId, 1.0f, 1.0f, 1.0f);
    }
    Hu3DMotionTimeSet(sys->stageMdlId, 0.0f);
    if (sys->stageNo == 1) {
        Hu3DMotionSpeedSet(sys->backMdlId[0], 0.04f);
        Hu3DMotionSpeedSet(sys->backMdlId[1], 0.04f);
    } else if (sys->stageNo == 2 || sys->stageNo == 6) {
        Hu3DMotionSpeedSet(sys->backMdlId[0], 1.0f);
        Hu3DMotionSpeedSet(sys->backMdlId[1], 1.0f);
        Hu3DModelAttrSet(sys->backMdlId[0], 0x40000001);
        Hu3DModelAttrSet(sys->backMdlId[1], 0x40000001);
    } else {
        Hu3DMotionSpeedSet(sys->stageMdlId, 1.0f);
    }
    Hu3DModelAttrSet(sys->stageMdlId, 0x40000041);
    Hu3DModelCameraSet(sys->stageMdlId, 3);
    if (sys->stageNo == 0) {
        HSFBITMAP *bitmap;

        /* trial stage ground texture: shuffle which half of the atlas is used */
        textureBackupPtr = HuMemDirectMallocNum(HEAP_HEAP, 0x40000, HU_MEMNUM_OVL);
        hsf = Hu3DData[sys->stageMdlId].hsf;
        bitmap = hsf->attribute->bitmap;
        for (i = 0; i < hsf->bitmapNum; i++) {
            if (strlen("sd00_g00") == strlen(bitmap[i].name) && memcmp("sd00_g00", bitmap[i].name, strlen("sd00_g00")) == 0) {
                break;
            }
        }
        texturePtr = bitmap[i].data;
        memcpy(textureBackupPtr, texturePtr + (rand8() % 2) * 0x40000, 0x40000);
        memcpy(texturePtr, textureBackupPtr, 0x40000);
        memcpy(texturePtr + 0x40000, textureBackupPtr, 0x40000);
        HuMemDirectFree(textureBackupPtr);
    }
    if (sys->fxId != -1) {
        HuAudFXStop(sys->fxId);
        sys->fxId = -1;
    }
    if (sys->stageNo == 5) {
        sys->fxId = HuAudFXPlay(0x3A3);
    } else if (sys->stageNo == 7) {
        sys->fxId = HuAudFXPlay(0x3A2);
    }
}

static void StageBackObjInit(void)
{
    s32 i;
    s32 j;
    char **names;

    memset(sys->backObj, 0, sizeof(sys->backObj));
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 20; j++) {
            names = stageObjTbl[sys->stageNo];
            if (names[j] == NULL) {
                break;
            }
            sys->backObj[i][j] = Hu3DModelObjPtrGet(sys->backMdlId[i], names[j]);
        }
    }
}

void SdStageBackRotYSet(s32 machineNo, f32 rotY, f32 rotX)
{
    s32 j;

    if (MgSeqModeGet() < 2) {
        return;
    }
    rotY -= rotX;
    if (rotY > 180.0f) {
        rotY -= 360.0f;
    }
    if (rotY < -180.0f) {
        rotY += 360.0f;
    }
    for (j = 0; j < 20 && sys->backObj[machineNo][j] != NULL; j++) {
        sys->backObj[machineNo][j]->mesh.base.rot.y -= rotY * stageObjScaleTbl[sys->stageNo][j];
    }
    if (sys->stageNo == 7) {
        sys->backObj[machineNo][4]->mesh.base.rot.y += 0.1f;
    } else if (sys->stageNo == 1) {
        sys->backObj[machineNo][0]->mesh.base.rot.y -= 0.0125f;
        sys->backObj[machineNo][1]->mesh.base.rot.y -= 0.025f;
    } else if (sys->stageNo == 4) {
        sys->backObj[machineNo][0]->mesh.base.rot.y += 0.1f;
        sys->backObj[machineNo][2]->mesh.base.rot.y += 2.0f;
    }
}

static void StageShadowInit(void)
{
    s32 i;
    char **names = stageShadowObjTbl[sys->stageNo];

    for (i = 0; names[i] != NULL; i++) {
        Hu3DModelShadowMapObjSet(sys->stageMdlId, names[i]);
    }
}

void SdStageClose(void)
{
    HSFDATA *hsf;
    HSFBUFFER *vertex;
    HSFBUFFER *normal;
    s8 *normalData;
    s32 i;

    if (sys->stageNo == 0) {
        /* flatten the trial ground mesh again for the next round */
        hsf = Hu3DData[sys->stageMdlId].hsf;
        i = StageFaceFind(hsf, "grid1");
        vertex = &hsf->vertex[i];
        normal = &hsf->normal[i];
        normalData = normal->data;
        for (i = 0; i < vertex->count; i++) {
            ((Vec *)vertex->data)[i].y = 0.0f;
        }
        for (i = 0; i < normal->count; i++) {
            normalData[i * 3] = 0;
            normalData[i * 3 + 1] = 127;
            normalData[i * 3 + 2] = 0;
        }
        if (stageLavaModelId != -1) {
            Hu3DModelAttrSet(stageLavaModelId, 1);
        }
    }
    StageDataClose();
    if (stageColMesh != NULL) {
        HuMemDirectFree(stageColMesh);
        stageColMesh = NULL;
    }
    if (stageColWall != NULL) {
        HuMemDirectFree(stageColWall);
        stageColWall = NULL;
    }
    if (stageTrialMapColType != NULL) {
        HuMemDirectFree(stageTrialMapColType);
        stageTrialMapColType = NULL;
    }
    if (stageTrialMap != NULL) {
        HuMemDirectFree(stageTrialMap);
        stageTrialMap = NULL;
    }
}

/* resolves triangle idx of a face to its 3 vertices (and, if normal != NULL, 3 normal triplets) */
static void ReadFace(s32 idx, HSFFACE *face, HSFBUFFER *vertex, s8 *normal, Vec *vtx[3], s8 *nrm[3])
{
    HSFFACEINDEX *p[3];

    p[0] = &face->index[0];
    p[1] = &face->index[1];
    p[2] = &face->index[2];
    switch (face->type) {
        case 2:
            break;
        case 3:
            if (idx != 0) {
                p[0] = &face->index[1];
                p[1] = &face->index[3];
                p[2] = &face->index[2];
            }
            break;
        case 4:
            if (idx == 0) {
            } else if (idx == 1) {
                p[0] = &face->index[2];
                p[1] = &face->index[1];
                p[2] = &face->strip.data[0];
            } else if (idx == 2) {
                p[0] = &face->index[1];
                p[1] = &face->strip.data[1];
                p[2] = &face->strip.data[0];
            } else if (idx % 2 != 0) {
                p[0] = &face->strip.data[idx - 3];
                p[1] = &face->strip.data[idx - 2];
                p[2] = &face->strip.data[idx - 1];
            } else {
                p[0] = &face->strip.data[idx - 3];
                p[1] = &face->strip.data[idx - 1];
                p[2] = &face->strip.data[idx - 2];
            }
            break;
        default:
            break;
    }
    vtx[0] = &((Vec *)vertex->data)[p[0]->vertex];
    vtx[1] = &((Vec *)vertex->data)[p[1]->vertex];
    vtx[2] = &((Vec *)vertex->data)[p[2]->vertex];
    if (normal != NULL) {
        nrm[0] = normal + p[0]->normal * 3;
        nrm[1] = normal + p[1]->normal * 3;
        nrm[2] = normal + p[2]->normal * 3;
    }
}

/* number of triangles of a face */
static s32 StageFaceTriNum(HSFFACE *face)
{
    s32 num = 0;

    if (face->type == 2) {
        num = 1;
    } else if (face->type == 3) {
        num = 2;
    } else if (face->type == 4) {
        num = face->strip.count + 1;
    }
    return num;
}

static void MakeStage(s32 stageNo, s32 smooth, f32 height)
{
    if (sys->stageNo == 0) {
        MakeTrialStage(smooth, height);
    } else {
        MakeMainStage();
    }
    MakeStageLight();
}

/* local frame of a collision triangle: mtx = [dir | normal | side], invMtx, unkC = foot point */
static void StageColTriFrameSet(SdStageColTri *tri, Vec *dir)
{
    Mtx m;
    Vec side;
    Vec tmp;

    PSVECCrossProduct(dir, &tri->normal, &side);
    PSVECNormalize(&side, &side);
    PSMTXIdentity(m);
    m[0][0] = dir->x;
    m[1][0] = dir->y;
    m[2][0] = dir->z;
    m[0][1] = tri->normal.x;
    m[1][1] = tri->normal.y;
    m[2][1] = tri->normal.z;
    m[0][2] = side.x;
    m[1][2] = side.y;
    m[2][2] = side.z;
    PSMTXCopy(m, tri->mtx);
    PSMTXInverse(tri->mtx, tri->invMtx);
    tmp.x = 0.0f;
    tmp.y = -1.0f;
    tmp.z = 0.0f;
    PSMTXMultVec(tri->invMtx, &tmp, &tmp);
    tmp.y = 0.0f;
    PSMTXMultVec(tri->mtx, &tmp, &tri->unkC);
}

static void MakeTrialStage(s32 smooth, f32 height)
{
    Vec *p[3];
    s8 *n[3];
    Vec a;
    Vec b;
    Vec center;
    char *name = "grid1";
    HSFDATA *hsf;
    HSFDATA *lavaHsf;
    HSFBUFFER *vertexBuf;
    HSFBUFFER *normalBuf;
    HSFBUFFER *faceBuf;
    s8 *normalData;
    HSFFACE *face;
    SdStageColTri *tri;
    SdStageTrialCell *cell;
    s32 clearIdx;
    s32 idx;
    s32 i;
    s32 j;
    s32 k;
    s32 col;
    s32 row;
    s32 triNum;
    s32 type;
    s32 half;
    f32 sum;

    stageTrialLavaFloorOnF = 0;
    stageTrialClearOff = 0;
    stageTrialHeight = height;
    stageTrialSmooth = smooth;
    memset(stageColMesh, 0, SD_COLMESH_NUM * sizeof(SdStageColTri));
    stageTrialMapColType = HuMemDirectMallocNum(HEAP_HEAP, 0x4000, HU_MEMNUM_OVL);
    memset(stageTrialMapColType, 0, 0x4000);
    stageTrialMap = HuMemDirectMallocNum(HEAP_HEAP, 0x100, HU_MEMNUM_OVL);
    clearIdx = rand8() % 5;
    if (clearIdx == 1) {
        stageTrialClearOff = 1;
    }
    memset(stageTrialMap, clearIdx * 16 + 2, 0x100);
    hsf = Hu3DData[sys->stageMdlId].hsf;
    idx = StageFaceFind(hsf, name);
    vertexBuf = &hsf->vertex[idx];
    normalBuf = &hsf->normal[idx];
    faceBuf = &hsf->face[idx];
    normalData = normalBuf->data;
    if (stageLavaModelId == -1) {
        stageLavaModelId = Hu3DModelCreateData(0x00BC005F);
        lavaHsf = Hu3DData[stageLavaModelId].hsf;
        for (i = 0; i < lavaHsf->st->count && i < 4; i++) {
            stageWork.uv[i] = (f32 *)lavaHsf->st->data + i * 2;
        }
        stageWork.material = lavaHsf->material;
        Hu3DModelShadowMapSet(stageLavaModelId);
        Hu3DModelCameraSet(stageLavaModelId, 3);
    }
    Hu3DData[stageLavaModelId].pos.y = 90.0f;
    stageWork.lavaY = 90.0f;
    stageTrialMapHeight = HuMemDirectMallocNum(HEAP_HEAP, SD_TRIALMAP_W * SD_TRIALMAP_W * sizeof(SdStageTrialCell), HU_MEMNUM_OVL);
    memset(stageTrialMapHeight, 0, SD_TRIALMAP_W * SD_TRIALMAP_W * sizeof(SdStageTrialCell));
    StageTrialMakeGround();
    /* register every ground vertex in its 100x100 cell and set its height from the map */
    for (i = 0; i < vertexBuf->count; i++) {
        Vec *v = &((Vec *)vertexBuf->data)[i];

        col = (s32)(24.0f + v->x / 100.0f);
        row = (s32)(24.0f + v->z / 100.0f);
        cell = &stageTrialMapHeight[row * SD_TRIALMAP_W + col];
        cell->data[cell->count++] = v;
        type = stageTrialMap[StageClamp(row / 3, 0, 15) * 16 + StageClamp(col / 3, 0, 15)] & 0xF;
        if (height > 0.0f) {
            if (type == 0) {
                v->y = 0.0f;
            } else if (type == 1) {
                v->y = 50.0f;
            } else if (type == 2) {
                v->y = 100.0f;
            } else {
                v->y = 100.0f + (f32)(type - 2) * height;
            }
        } else {
            v->y = 100.0f;
        }
    }
    half = smooth / 2;
    if (smooth >= 3) {
        for (i = 0; i < SD_TRIALMAP_W; i++) {
            for (j = 0; j < SD_TRIALMAP_W; j++) {
                sum = 0.0f;
                for (k = 0; k < smooth * smooth; k++) {
                    row = StageClamp(i + (k / smooth - half), 0, 0x30);
                    col = StageClamp(j + (k % smooth - half), 0, 0x30);
                    sum += ((Vec *)stageTrialMapHeight[row * SD_TRIALMAP_W + col].data[0])->y;
                }
                cell = &stageTrialMapHeight[i * SD_TRIALMAP_W + j];
                for (k = 0; k < cell->count; k++) {
                    ((Vec *)cell->data[k])->y = sum / (f32)(smooth * smooth);
                }
            }
        }
    }
    HuMemDirectFree(stageTrialMapHeight);
    stageTrialMapNum = HuMemDirectMallocNum(HEAP_HEAP, SD_TRIALMAP_W * SD_TRIALMAP_W * sizeof(SdStageTrialCell), HU_MEMNUM_OVL);
    memset(stageTrialMapNum, 0, SD_TRIALMAP_W * SD_TRIALMAP_W * sizeof(SdStageTrialCell));
    for (i = 0; i < faceBuf->count; i++) {
        face = &((HSFFACE *)faceBuf->data)[i];
        triNum = StageFaceTriNum(face);
        if (triNum == 0) {
            continue;
        }
        for (j = 0; j < triNum; j++) {
            ReadFace(j, face, vertexBuf, normalData, p, n);
            PSVECSubtract(p[1], p[0], &a);
            PSVECNormalize(&a, &a);
            PSVECSubtract(p[2], p[1], &b);
            PSVECNormalize(&b, &b);
            PSVECCrossProduct(&a, &b, &b);
            PSVECNormalize(&b, &b);
            for (k = 2; k >= 0; k--) {
                n[k][0] = (s32)(127.0f * b.x);
                n[k][1] = (s32)(127.0f * b.y);
                n[k][2] = (s32)(127.0f * b.z);
            }
            for (k = 0; k < 3; k++) {
                col = (s32)(24.0f + p[k]->x / 100.0f);
                row = (s32)(24.0f + p[k]->z / 100.0f);
                cell = &stageTrialMapNum[row * SD_TRIALMAP_W + col];
                cell->data[cell->count++] = n[k];
            }
            col = (s32)(48.0f + (p[2]->x + (p[0]->x + p[1]->x)) / 150.0f);
            row = (s32)(24.0f + (p[2]->z + (p[0]->z + p[1]->z)) / 300.0f);
            tri = &stageColMesh[row * SD_COLMESH_W + col];
            tri->normal = b;
            center.x = (p[2]->x + (p[0]->x + p[1]->x)) / 3.0f;
            center.y = (p[2]->y + (p[0]->y + p[1]->y)) / 3.0f;
            center.z = (p[2]->z + (p[0]->z + p[1]->z)) / 3.0f;
            tri->pos = center;
            StageColTriFrameSet(tri, &a);
        }
    }
    if (smooth >= 3) {
        for (i = 0; i < SD_TRIALMAP_W; i++) {
            for (j = 0; j < SD_TRIALMAP_W; j++) {
                Vec acc = { 0.0f, 0.0f, 0.0f };

                for (k = 0; k < smooth * smooth; k++) {
                    s8 *nv;
                    Vec tmp;

                    col = StageClamp(i + (k % smooth - half), 0, 0x30);
                    row = StageClamp(j + (k / smooth - half), 0, 0x30);
                    nv = stageTrialMapNum[row * SD_TRIALMAP_W + col].data[0];
                    tmp.x = (f32)nv[0] / 127.0f;
                    tmp.y = (f32)nv[1] / 127.0f;
                    tmp.z = (f32)nv[2] / 127.0f;
                    PSVECAdd(&acc, &tmp, &acc);
                }
                PSVECScale(&acc, &acc, 1.0f / (f32)(smooth * smooth));
                cell = &stageTrialMapNum[i * SD_TRIALMAP_W + j];
                for (k = 0; k < cell->count; k++) {
                    s8 *nv = cell->data[k];

                    nv[0] = (s32)(127.0f * acc.x);
                    nv[1] = (s32)(127.0f * acc.y);
                    nv[2] = (s32)(127.0f * acc.z);
                }
            }
        }
    }
    HuMemDirectFree(stageTrialMapNum);
    trialModelId[0] = Hu3DModelCreateData(0x00BC005B);
    trialModelId[1] = Hu3DModelCreateData(0x00BC005C);
    trialModelId[2] = Hu3DModelCreateData(0x00BC005D);
    trialModelId[3] = Hu3DModelCreateData(0x00BC005E);
    MakeTrialStageMesh();
    Hu3DModelKill(trialModelId[0]);
    Hu3DModelKill(trialModelId[1]);
    Hu3DModelKill(trialModelId[2]);
    Hu3DModelKill(trialModelId[3]);
}

static void MakeMainStage(void)
{
    char *names[3] = { "g", "w", NULL };
    Vec *p[3];
    Vec v[3];
    Vec a;
    Vec b;
    Vec objPos;
    Vec center;
    HSFDATA *hsf;
    HSFBUFFER *vertexBuf;
    HSFBUFFER *faceBuf;
    HSFFACE *face;
    HSFMATERIAL *material;
    SdStageColTri *tri;
    s32 floorNum = 0;
    s32 wallNum = 0;
    s32 floorIdx;
    s32 wallIdx;
    s32 i;
    s32 j;
    s32 k;
    s32 m;
    s32 n;
    s32 triNum;
    f32 radius;
    f32 mag;

    hsf = Hu3DData[sys->stageMdlId].hsf;
    /* pass 1: count floor / wall triangles of the "g" and "w" objects */
    for (i = 0; i < hsf->faceNum; i++) {
        faceBuf = &hsf->face[i];
        vertexBuf = &hsf->vertex[i];
        j = StageNameFind(names, faceBuf->name);
        if (names[j] == NULL) {
            continue;
        }
        for (k = 0; k < faceBuf->count; k++) {
            face = &((HSFFACE *)faceBuf->data)[k];
            triNum = StageFaceTriNum(face);
            if (triNum == 0) {
                continue;
            }
            for (m = 0; m < triNum; m++) {
                ReadFace(m, face, vertexBuf, NULL, p, NULL);
                PSVECSubtract(p[1], p[0], &a);
                StageVecNormalize(&a);
                PSVECSubtract(p[2], p[1], &b);
                StageVecNormalize(&b);
                PSVECCrossProduct(&a, &b, &a);
                StageVecNormalize(&a);
                if (a.y >= 0.5f) {
                    floorNum++;
                } else {
                    wallNum++;
                }
            }
        }
    }
    stageColFloorNum = floorNum;
    stageColWallNum = wallNum;
    stageColWall = HuMemDirectMallocNum(HEAP_HEAP, wallNum * sizeof(SdStageColTri), HU_MEMNUM_OVL);
    memset(stageColMesh, 0, floorNum * sizeof(SdStageColTri));
    memset(stageColWall, 0, wallNum * sizeof(SdStageColTri));
    floorIdx = 0;
    wallIdx = 0;
    /* pass 2: build the collision triangles */
    for (i = 0; i < hsf->faceNum; i++) {
        vertexBuf = &hsf->vertex[i];
        faceBuf = &hsf->face[i];
        j = StageNameFind(names, faceBuf->name);
        if (names[j] == NULL) {
            continue;
        }
        Hu3DModelObjPosGet(sys->stageMdlId, names[j], &objPos);
        for (k = 0; k < faceBuf->count; k++) {
            face = &((HSFFACE *)faceBuf->data)[k];
            triNum = StageFaceTriNum(face);
            if (triNum == 0) {
                continue;
            }
            for (m = 0; m < triNum; m++) {
                ReadFace(m, face, vertexBuf, NULL, p, NULL);
                PSVECAdd(p[0], &objPos, &v[0]);
                PSVECAdd(p[1], &objPos, &v[1]);
                PSVECAdd(p[2], &objPos, &v[2]);
                PSVECSubtract(&v[1], &v[0], &a);
                StageVecNormalize(&a);
                PSVECSubtract(&v[2], &v[1], &b);
                StageVecNormalize(&b);
                PSVECCrossProduct(&a, &b, &b);
                StageVecNormalize(&b);
                if (b.y >= 0.5f) {
                    tri = &stageColMesh[floorIdx++];
                } else {
                    tri = &stageColWall[wallIdx++];
                }
                if (j == 0) {
                    tri->attr = 1;
                }
                if (j == 1) {
                    tri->attr = 2;
                }
                tri->normal = b;
                PSVECCrossProduct(&a, &tri->normal, &b);
                StageVecNormalize(&b);
                center.x = (v[2].x + (v[0].x + v[1].x)) / 3.0f;
                center.y = (v[2].y + (v[0].y + v[1].y)) / 3.0f;
                center.z = (v[2].z + (v[0].z + v[1].z)) / 3.0f;
                tri->pos = center;
                MathPlaneRotMtxGet(&a, &tri->normal, 0.0f, tri->mtx);
                PSMTXInverse(tri->mtx, tri->invMtx);
                a.x = 0.0f;
                a.y = -1.0f;
                a.z = 0.0f;
                PSMTXMultVec(tri->invMtx, &a, &a);
                a.y = 0.0f;
                PSMTXMultVec(tri->mtx, &a, &tri->unkC);
                radius = -10000.0f;
                for (n = 0; n < 3; n++) {
                    PSVECSubtract(&v[n], &tri->pos, &tri->vtx2[n]);
                    tri->vtx[n] = tri->vtx2[n];
                    tri->vtx[n].y = 0.0f;
                    mag = PSVECMag(&tri->vtx[n]);
                    if (radius < mag) {
                        radius = mag;
                    }
                    PSMTXMultVec(tri->invMtx, &tri->vtx2[n], &tri->vtx2[n]);
                    tri->vtx2[n].y = 0.0f;
                }
                tri->radius = radius;
                material = &hsf->material[face->mat & 0xFFF];
                if (material->color[0] == 0 && material->color[1] == 0 && material->color[2] == 0xFF) {
                    tri->attr |= 0x10;
                }
                if (material->color[0] == 0xFF && material->color[1] == 0xFF && material->color[2] == 0) {
                    tri->attr |= 0x20;
                }
                if (material->color[0] == 0 && material->color[1] == 0xFF && material->color[2] == 0) {
                    tri->attr |= 0x40;
                }
                for (n = 0; n < 3; n++) {
                    PSVECSubtract(&tri->vtx2[(n + 1) % 3], &tri->vtx2[n], &a);
                    PSVECNormalize(&a, &a);
                    PSVECSubtract(&tri->vtx2[(n + 2) % 3], &tri->vtx2[n], &b);
                    PSVECNormalize(&b, &b);
                    PSVECAdd(&a, &b, &tri->edge[n]);
                    PSVECNormalize(&tri->edge[n], &tri->edge[n]);
                    tri->dot2[n] = PSVECDotProduct(&tri->edge[n], &a);
                    PSVECSubtract(&tri->vtx[(n + 1) % 3], &tri->vtx[n], &a);
                    PSVECNormalize(&a, &a);
                    PSVECSubtract(&tri->vtx[(n + 2) % 3], &tri->vtx[n], &b);
                    PSVECNormalize(&b, &b);
                    PSVECAdd(&a, &b, &tri->dir[n]);
                    PSVECNormalize(&tri->dir[n], &tri->dir[n]);
                    tri->dot[n] = PSVECDotProduct(&tri->dir[n], &a);
                }
            }
        }
    }
}

#include "REL/sd00dll/stage_b.inc"

#include "REL/sd00dll/stage_c.inc"

#include "REL/sd00dll/stage_d.inc"

#include "REL/sd00dll/stage_e.inc"
