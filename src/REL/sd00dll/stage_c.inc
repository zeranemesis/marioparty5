/* stage part C: SdStageUpdate .. StageBomheiBigUpdate, MakeStageLight .. StageHamonMatHook.
   Shared data owned by part A (declared before this file is included):
   stageWork (SdStageWork: frame, lavaY, uv[4]), stageLavaModelId, stageTrialMap (u8 *),
   stageColMesh (SdStageColTri *), objModelFile[], objDataTbl[] (SdStageObjType),
   objBomheiMotTbl[], objBomheiBigMotTbl[]. */

/* ---- forward declarations of statics defined in other parts ---- */
static void StageObjCol(SdStageObj *obj);                   /* part D */
static void StageKinokoUpdate(SdStageObj *obj);             /* part D */
static void StageNKinokoUpdate(SdStageObj *obj);            /* part D */
static void StageRabbitUpdate(void);                        /* part D */
static void StageCreateDust(SdStageObj *obj, f32 scale);    /* part D */
static void StageUpdateFire(SdStageObj *obj);               /* part D */
static void Stage07SmokeMain(void);                         /* part D */

/* ---- part C statics ---- */
static void StageTrialObjCreate(void);
static void StageMainObjCreate(void);
static void StageDataClose(void);
static void StageObjUpdate(void);
static void StageDokanUpdate(SdStageObj *obj);
static void StageTogezoUpdate(SdStageObj *obj);
static void StageBomheiUpdate(SdStageObj *obj);
static void StageBomheiBigUpdate(SdStageObj *obj);
static void MakeStageLight(void);
static void StageHamonCreate(void);
static void StageHamonKill(void);
static void StageHamonLayerHook(void);
static void StageHamonPosSet(Vec *pos, Vec *dir);
static void StageHamonColorSet(GXColor *color);
static void StageHamonSpeedSet(f32 speed);
static void StageHamonMatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material);

static void (*const objUpdateFuncTbl[])(SdStageObj *obj) = {
    StageDokanUpdate, StageBomheiUpdate, StageBomheiBigUpdate,
    StageTogezoUpdate, StageKinokoUpdate, StageNKinokoUpdate,
};

static Vec shadowCamPos = { 0.0f, 6000.0f, 0.0f };
static Vec shadowCamUp = { 0.0f, 0.0f, -1.0f };
static Vec shadowCamTarget = { 0.0f, 0.0f, 0.0f };

void SdStageUpdate(void)
{
    f32 t;
    f32 s;

    if (stageLavaModelId != -1) {
        stageWork.frame++;
        t = (f32)stageWork.frame;
        s = (f32)((1.0 + sin(3.141592653589793 * (4.0f * t) / 180.0)) * 0.5);
        Hu3DModelAmbSet(stageLavaModelId, s, s, s);
        t *= 0.25f;
        s = (f32)((1.0 + sin(3.141592653589793 * t / 180.0)) * 0.5);
        stageWork.uv[0][0] = 0.25f * s;
        stageWork.uv[0][1] = 0.25f * s;
        stageWork.uv[1][0] = 1.0f - 0.25f * s;
        stageWork.uv[1][1] = 0.25f * s;
        stageWork.uv[2][0] = 0.25f * s;
        stageWork.uv[2][1] = 1.0f - 0.25f * s;
        stageWork.uv[3][0] = 1.0f - 0.25f * s;
        stageWork.uv[3][1] = 1.0f - 0.25f * s;
        DCStoreRangeNoSync(stageWork.uv[0], 8);
        DCStoreRangeNoSync(stageWork.uv[1], 8);
        DCStoreRangeNoSync(stageWork.uv[2], 8);
        DCStoreRangeNoSync(stageWork.uv[3], 8);
    }
    if ((u32)MgSeqModeGet() >= 5) {
        StageRabbitUpdate();
        StageObjUpdate();
    }
    sys->frameCount++;
    if (sys->stageNo == 7) {
        Stage07SmokeMain();
    }
}

void SdStageObjCreate(void)
{
    StageDataClose();
    sys->objCreateF = 1;
    sys->obj = HuMemDirectMallocNum(HEAP_HEAP, 8 * sizeof(SdStageObj), HU_MEMNUM_OVL);
    memset(sys->obj, 0, 8 * sizeof(SdStageObj));
    if (sys->stageNo == 0) {
        StageTrialObjCreate();
    } else {
        StageMainObjCreate();
    }
    SdMachineParticleKill();
    SdMachineParticleCreate();
}

/* Shared tail of both ObjCreate functions: creates the object model (+ fire
   model for bomhei), the shadow, and fills the obj fields. */
static void StageObjModelCreate(SdStageObj *obj, s32 type, Vec *pos)
{
    f32 scale;

    obj->mdlId = Hu3DModelCreateData(objModelFile[type]);
    if (type == 1 || type == 2) {
        Hu3DModelAttrSet(obj->mdlId, 0x40000001);
        Hu3DMotionSpeedSet(obj->mdlId, 1.0f);
        obj->fireMdlId = Hu3DModelCreateData(0xBC006C);
        Hu3DModelHookSet(obj->mdlId, "itemhook_c", obj->fireMdlId);
        Hu3DModelAttrSet(obj->fireMdlId, 1);
        Hu3DModelAttrSet(obj->fireMdlId, 0x40000001);
        Hu3DModelLayerSet(obj->fireMdlId, 3);
        Hu3DModelCameraSet(obj->fireMdlId, 3);
        Hu3DMotionSpeedSet(obj->fireMdlId, 1.0f);
        if (type == 1) {
            Hu3DModelScaleSet(obj->fireMdlId, 1.5f, 1.5f, 1.5f);
            obj->motId1 = Hu3DJointMotionData(obj->mdlId, objBomheiMotTbl[0]);
            obj->motId2 = Hu3DJointMotionData(obj->mdlId, objBomheiMotTbl[1]);
        } else {
            Hu3DModelScaleSet(obj->fireMdlId, 2.0f, 2.0f, 2.0f);
            obj->motId1 = Hu3DJointMotionData(obj->mdlId, objBomheiBigMotTbl[0]);
            obj->motId2 = Hu3DJointMotionData(obj->mdlId, objBomheiBigMotTbl[1]);
        }
        Hu3DMotionSet(obj->mdlId, obj->motId1);
    }
    obj->type = type;
    obj->typeP = &objDataTbl[type];
    obj->pos = *pos;
    obj->dispF = 1;
    obj->radius = obj->typeP->radius;
    obj->height = obj->typeP->height;
    obj->height2 = obj->typeP->height2;
    obj->endF = obj->typeP->damage;
    Hu3DModelPosSetV(obj->mdlId, pos);
    Hu3DModelLayerSet(obj->mdlId, 0);
    Hu3DModelCameraSet(obj->mdlId, 3);
    if (obj->type == 0) {
        Hu3DModelScaleSet(obj->mdlId, 3.0f, 3.0f, 3.0f);
    }
    obj->shadowMdlId = Hu3DModelLink(sys->baseShadowMdlId);
    scale = 2.0f * obj->radius / 100.0f;
    Hu3DModelShadowSet(obj->shadowMdlId);
    Hu3DModelPosSetV(obj->shadowMdlId, pos);
    Hu3DModelScaleSet(obj->shadowMdlId, scale, 1.0f, scale);
    Hu3DModelLayerSet(obj->shadowMdlId, 3);
    Hu3DModelCameraSet(obj->shadowMdlId, 3);
    Hu3DModelAttrReset(obj->shadowMdlId, 1);
    Hu3DModelAttrSet(obj->shadowMdlId, 8);
}

static void StageTrialObjCreate(void)
{
    s32 *occ;
    SdStageObj *obj;
    SdStageColTri *tri;
    Vec tmp;
    Vec diff;
    s32 i;
    s32 tries;
    s32 x;
    s32 z;
    s32 ix;
    s32 iz;
    s32 cellType;
    s32 type;

    x = 0;
    z = 0;
    /* original allocates 0x400 and clears 0x380 although (z << 4) + x can reach 0x110 */
    occ = HuMemDirectMallocNum(HEAP_HEAP, 0x440, HU_MEMNUM_OVL);
    memset(occ, 0, 0x440);
    for (i = 0; i < 8; i++) {
        tries = 0;
        obj = &sys->obj[i];
        obj->shadowMdlId = -1;
        obj->fireMdlId = -1;
        obj->mdlId = -1;
        obj->motId2 = -1;
        obj->motId1 = -1;
        obj->flameIdx = -1;
        obj->fireIdx = -1;
        if (sys->gameType != 0 && i >= 1 && i < 3) {
            continue;
        }
        if (i < 6) {
            for (;;) {
                tries++;
                if (tries >= 10) {
                    break;
                }
                x = rand8() % 8;
                x += (i < 4) ? 2 : 6;
                x = (x > 16) ? 16 : (x < 0 ? 0 : x);
                z = rand8() % 8;
                z += (i < 4) ? 2 : 6;
                z = (z > 16) ? 16 : (z < 0 ? 0 : z);
                cellType = ((s8)stageTrialMap[(z << 4) + x] >> 4) & 0xF;
                tmp.x = 150.0f + 3.0f * (100.0f * (f32)x) - 2400.0f;
                tmp.y = 0.0f;
                tmp.z = 150.0f + 3.0f * (100.0f * (f32)z) - 2400.0f;
                ix = (s32)(48.0f + tmp.x / 50.0f);
                iz = (s32)(24.0f + tmp.z / 100.0f);
                tri = &stageColMesh[ix + iz * 0x60];
                tmp.y = tri->pos.y;
                if (cellType >= 3) {
                    continue;
                }
                if (occ[(z << 4) + x] != 0) {
                    continue;
                }
                if (tmp.y < 100.0f) {
                    continue;
                }
                PSVECSubtract(&sys->machine[0].curPos, &tmp, &diff);
                diff.y = 0.0f;
                if (PSVECMag(&diff) < 300.0f + sys->machine[0].hitRadius) {
                    continue;
                }
                PSVECSubtract(&sys->machine[1].curPos, &tmp, &diff);
                diff.y = 0.0f;
                if (PSVECMag(&diff) < 300.0f + sys->machine[1].hitRadius) {
                    continue;
                }
                break;
            }
        } else {
            tmp.x = 0.0f;
            tmp.y = 0.0f;
            tmp.z = 0.0f;
        }
        if (tries >= 10) {
            continue;
        }
        if (i < 6) {
            type = rand8() % 4;
        } else if (i == 6) {
            type = 4;
        } else {
            type = 5;
        }
        StageObjModelCreate(obj, type, &tmp);
        occ[(z << 4) + x] = 1;
        if (i >= 6) {
            Hu3DModelScaleSet(obj->mdlId, 1.5f, 1.5f, 1.5f);
            Hu3DModelAttrSet(obj->mdlId, 1);
            Hu3DModelAttrSet(obj->shadowMdlId, 1);
        }
    }
    HuMemDirectFree(occ);
}

static void StageMainObjCreate(void)
{
    SdStageObj *obj;
    Vec pos;
    Vec tmp;
    Vec diff;
    f32 minY;
    f32 lowY;
    f32 rad;
    f32 mag;
    s32 i;
    s32 tries;
    s32 type;

    if (sys->stageNo == 0) {
        lowY = stageWork.lavaY - 25.0f;
    } else if (sys->stageNo == 2 || sys->stageNo == 7) {
        lowY = -25.0f;
    } else {
        lowY = -10000.0f;
    }
    minY = lowY + 24.0f;
    for (i = 0; i < 8; i++) {
        obj = &sys->obj[i];
        obj->shadowMdlId = -1;
        obj->fireMdlId = -1;
        obj->mdlId = -1;
        obj->motId2 = -1;
        obj->motId1 = -1;
        obj->flameIdx = -1;
        obj->fireIdx = -1;
    }
    if (sys->machine[0].hitRadius > sys->machine[1].hitRadius) {
        rad = 2.0f * sys->machine[0].hitRadius;
    } else {
        rad = 2.0f * sys->machine[1].hitRadius;
    }
    for (i = 0; i < 8; i++) {
        if ((sys->gameType != 0 || sys->stageNo == 7) && i >= 3 && i < 6) {
            continue;
        }
        tries = 0;
        obj = &sys->obj[i];
        if (i < 6) {
            for (;;) {
                tries++;
                if (tries >= 10) {
                    break;
                }
                SdStageRandomPosGet(&pos, -(150.0f + rad));
                if (pos.y == -10000.0f) {
                    continue;
                }
                if (pos.y < minY) {
                    continue;
                }
                PSVECSubtract(&sys->machine[0].curPos, &pos, &diff);
                diff.y = 0.0f;
                if (PSVECMag(&diff) < 300.0f + sys->machine[0].hitRadius) {
                    continue;
                }
                PSVECSubtract(&sys->machine[1].curPos, &pos, &diff);
                diff.y = 0.0f;
                if (PSVECMag(&diff) < 300.0f + sys->machine[1].hitRadius) {
                    continue;
                }
                if (sys->stageNo == 7) {
                    diff = pos;
                    diff.y = 0.0f;
                    mag = PSVECMag(&diff);
                    if (mag < 1000.0f + obj->radius) {
                        continue;
                    }
                    if (mag > 2450.0f - obj->radius) {
                        continue;
                    }
                }
                tmp = pos;
                if (!SdStageObjHitCheck(&tmp, 150.0f + rad)) {
                    break;
                }
            }
        } else {
            pos.x = 0.0f;
            pos.y = 0.0f;
            pos.z = 0.0f;
        }
        if (tries >= 10) {
            continue;
        }
        if (i < 6) {
            if (sys->stageNo == 2 || sys->stageNo == 5 || sys->stageNo == 7) {
                type = (rand8() % 2) * 3;
            } else {
                type = rand8() % 4;
            }
        } else if (i == 6) {
            type = 4;
        } else {
            type = 5;
        }
        StageObjModelCreate(obj, type, &pos);
        if (i >= 6) {
            obj->dispF = 0;
            Hu3DModelAttrSet(obj->mdlId, 1);
            Hu3DModelScaleSet(obj->mdlId, 1.5f, 1.5f, 1.5f);
            Hu3DModelAttrSet(obj->shadowMdlId, 1);
        }
    }
}

static void StageDataClose(void)
{
    SdStageObj *obj;
    s32 i;

    if (sys->objCreateF) {
        for (i = 0; i < 8; i++) {
            obj = &sys->obj[i];
            if (obj->motId1 != -1) {
                Hu3DMotionSet(obj->mdlId, -1);
                Hu3DMotionKill(obj->motId1);
                Hu3DMotionKill(obj->motId2);
                obj->motId2 = -1;
                obj->motId1 = -1;
            }
            if (obj->fireMdlId != -1) {
                Hu3DModelHookReset(obj->mdlId);
                Hu3DModelKill(obj->fireMdlId);
            }
            if (obj->mdlId != -1) {
                Hu3DModelKill(obj->mdlId);
            }
            if (obj->shadowMdlId != -1) {
                Hu3DModelKill(obj->shadowMdlId);
            }
            obj->fireMdlId = -1;
            obj->shadowMdlId = -1;
            obj->mdlId = -1;
        }
        HuMemDirectFree(sys->obj);
        sys->obj = NULL;
    }
    sys->objCreateF = 0;
}

static void StageObjUpdate(void)
{
    SdStageObj *obj;
    f32 time;
    s32 i;

    if (sys->obj == NULL) {
        return;
    }
    time = (f32)MgTimerValueGet(sys->timer);
    if (time == 3600.0f || (f32)MgTimerValueGet(sys->timer) == 1800.0f) {
        if (rand8() % 10 == 0) {
            sys->obj[7].mode = 1;
        } else {
            sys->obj[6].mode = 1;
        }
    }
    for (i = 0; i < 8; i++) {
        obj = &sys->obj[i];
        if (obj->mdlId != -1) {
            objUpdateFuncTbl[obj->type](obj);
            StageUpdateFire(obj);
        }
    }
}

static void StageDokanUpdate(SdStageObj *obj)
{
    StageObjCol(obj);
}

static void StageTogezoUpdate(SdStageObj *obj)
{
    StageObjCol(obj);
}

static void StageBomheiUpdate(SdStageObj *obj)
{
    Vec pos;
    f32 size;

    switch (obj->mode) {
        case 0:
            obj->endF = 0;
            if ((u32)MgSeqModeGet() == 5) {
                if (obj->hitF != 0 || ++obj->timer >= obj->typeP->waitTime) {
                    Hu3DMotionSet(obj->mdlId, obj->motId2);
                    Hu3DMotionTimeSet(obj->mdlId, 0.0f);
                    Hu3DModelAttrReset(obj->fireMdlId, 1);
                    Hu3DMotionTimeSet(obj->fireMdlId, 0.0f);
                    obj->mode++;
                    obj->timer = 0;
                }
            }
            StageObjCol(obj);
            break;
        case 1:
            if ((u32)MgSeqModeGet() == 5) {
                obj->timer++;
                if ((f32)obj->timer >= 60.0f) {
                    pos = obj->pos;
                    if (obj->type == 1) {
                        SdImpactStart(0x557, &pos, 1, 1.0f, obj->typeP->bounceF);
                    } else {
                        SdImpactStart(0x559, &pos, 1, 1.0f, obj->typeP->bounceF);
                    }
                    Hu3DModelAttrSet(obj->mdlId, 1);
                    Hu3DModelAttrSet(obj->shadowMdlId, 1);
                    Hu3DModelAttrSet(obj->fireMdlId, 1);
                    Hu3DMotionSet(obj->mdlId, obj->motId1);
                    Hu3DMotionTimeSet(obj->mdlId, 0.0f);
                    Hu3DModelScaleSet(obj->mdlId, 0.0f, 0.0f, 0.0f);
                    Hu3DModelScaleSet(obj->shadowMdlId, 0.0f, 0.0f, 0.0f);
                    SdModelGlowSet(obj->mdlId);
                    obj->endF = 1;
                    StageCreateDust(obj, obj->typeP->bounceF / 450.0f);
                    obj->timer = 0;
                    obj->mode++;
                }
            }
            StageObjCol(obj);
            break;
        case 2:
            if ((u32)MgSeqModeGet() == 5) {
                size = obj->typeP->radius;
                size += ((f32)obj->timer * (obj->typeP->bounceF - obj->typeP->radius)) / 15.0f;
                obj->radius = size;
                obj->height = size;
                obj->height2 = size;
                StageObjCol(obj);
                obj->timer++;
                if ((f32)obj->timer >= 15.0f) {
                    obj->dispF = 0;
                    obj->timer = 0;
                    obj->mode++;
                }
            }
            break;
        case 3:
            if ((u32)MgSeqModeGet() == 5) {
                obj->endF = 0;
                obj->hitF = 0;
                obj->radius = obj->typeP->radius;
                obj->height = obj->typeP->height;
                obj->height2 = obj->typeP->height2;
                if (++obj->timer >= obj->typeP->hideTime) {
                    Hu3DModelAttrReset(obj->mdlId, 1);
                    Hu3DModelAttrReset(obj->shadowMdlId, 1);
                    obj->dispF = 1;
                    obj->timer = 0;
                    obj->mode++;
                }
            }
            break;
        case 4:
            if ((u32)MgSeqModeGet() == 5) {
                obj->timer++;
                size = (f32)obj->timer / 60.0f;
                Hu3DModelScaleSet(obj->mdlId, size, size, size);
                size *= (2.0f * obj->typeP->radius) / 100.0f;
                Hu3DModelScaleSet(obj->shadowMdlId, size, size, size);
                if (size <= 0.0f) {
                    size = 0.01f;
                }
                obj->radius = obj->typeP->radius * size;
                obj->height = obj->typeP->height * size;
                obj->height2 = obj->typeP->height2 * size;
            }
            StageObjCol(obj);
            if ((u32)MgSeqModeGet() == 5) {
                if ((f32)obj->timer >= 60.0f) {
                    Hu3DModelScaleSet(obj->mdlId, 1.0f, 1.0f, 1.0f);
                    size = (2.0f * obj->typeP->radius) / 100.0f;
                    Hu3DModelScaleSet(obj->shadowMdlId, size, size, size);
                    obj->hitF = 0;
                    obj->timer = 0;
                    obj->mode = 0;
                }
            }
            break;
    }
}

static void StageBomheiBigUpdate(SdStageObj *obj)
{
    StageBomheiUpdate(obj);
}

static void MakeStageLight(void)
{
    Vec lightPos = { 0.0f, 1000.0f, 0.0f };
    Vec lightDir = { 1.0f, -1.0f, 1.0f };
    Vec camPos;
    Vec camUp;
    Vec camTarget;
    GXColor color = { 255, 255, 255, 128 };

    if (sys->lightId != -1) {
        Hu3DGLightKill(sys->lightId);
    }
    sys->lightId = Hu3DGLightCreateV(&lightPos, &lightDir, &color);
    Hu3DGLightStaticSet(sys->lightId, 1);
    if (sys->stageNo == 3) {
        lightPos.x = 0.0f;
        lightPos.y = 1000.0f;
        lightPos.z = 0.0f;
        lightDir.x = 0.0f;
        lightDir.y = -1.0f;
        lightDir.z = 1.0f;
        color.r = 255;
        color.g = 128;
        color.b = 0;
        color.a = 255;
        camPos.x = 0.0f;
        camPos.y = 3000.0f;
        camPos.z = -7000.0f;
        camUp.x = 0.0f;
        camUp.y = 1.0f;
        camUp.z = 0.0f;
        camTarget.x = 0.0f;
        camTarget.y = -3000.0f;
        camTarget.z = 7000.0f;
        Hu3DGLightInfinitytSet(sys->lightId);
    } else if (sys->stageNo == 7) {
        lightPos.x = 0.0f;
        lightPos.y = 3000.0f;
        lightPos.z = 0.0f;
        camPos = shadowCamPos;
        camUp = shadowCamUp;
        camTarget = shadowCamTarget;
        Hu3DGLightPointSet(sys->lightId, 3000.0f, 0.95f, GX_DA_MEDIUM);
    } else {
        lightPos.x = 0.0f;
        lightPos.y = 1000.0f;
        lightPos.z = 0.0f;
        camPos = shadowCamPos;
        camUp = shadowCamUp;
        camTarget = shadowCamTarget;
        Hu3DGLightInfinitytSet(sys->lightId);
    }
    Hu3DGLightPosSetV(sys->lightId, &lightPos, &lightDir);
    Hu3DGLightColorSet(sys->lightId, color.r, color.g, color.b, color.a);
    Hu3DShadowMultiPosSet(&camPos, &camUp, &camTarget, 3);
}

static void StageHamonCreate(void)
{
    GXColor color = { 0, 0, 0, 0 };

    sys->hamonTex = HuMemDirectMallocNum(HEAP_MODEL, GXGetTexBufferSize(320, 240, GX_TF_RGB565, GX_FALSE, 0), HU_MEMNUM_OVL);
    sys->hamonAnim[0] = HuSprAnimDataRead(0xBC009D);
    sys->hamonAnim[1] = HuSprAnimDataRead(0xBC009E);
    sys->hamonPos[1].z = 0.0f;
    sys->hamonPos[1].y = 0.0f;
    sys->hamonPos[1].x = 0.0f;
    sys->hamonPos[0].z = 0.0f;
    sys->hamonPos[0].y = 0.0f;
    sys->hamonPos[0].x = 0.0f;
    sys->hamonColor = color;
    sys->hamonTime = 0.0f;
    sys->hamonSpeed = -0.02f;
}

static void StageHamonKill(void)
{
    if (sys->hamonTex != NULL) {
        HuMemDirectFree(sys->hamonTex);
        sys->hamonTex = NULL;
        HuSprAnimKill(sys->hamonAnim[0]);
        HuSprAnimKill(sys->hamonAnim[1]);
    }
}

static void StageHamonLayerHook(void)
{
    if (Hu3DCameraBit & 4) {
        GXSetTexCopySrc(0, 0, 640, 480);
        GXSetTexCopyDst(320, 240, GX_TF_RGB565, GX_TRUE);
        GXCopyTex(sys->hamonTex, GX_FALSE);
        omPauseChk();
    }
}

static void StageHamonPosSet(Vec *pos, Vec *dir)
{
    sys->hamonPos[0] = *pos;
    sys->hamonPos[1] = *dir;
}

static void StageHamonColorSet(GXColor *color)
{
    sys->hamonColor = *color;
}

static void StageHamonSpeedSet(f32 speed)
{
    sys->hamonSpeed = speed;
}

static void StageHamonMatHook(HU3DDRAWOBJ *drawObj, HSFMATERIAL *material)
{
    GXTexObj texObj;
    GXColor color;
    Mtx lightMtx;
    Mtx camInv;
    Mtx mtxA;
    Mtx mtxB;
    Mtx mtxC;
    HU3DCAMERA *cam;
    HU3DCAMERA *cam2;
    f32 transS;

    cam = &Hu3DCamera[Hu3DCameraNo];
    if ((omPauseChk() & 0xFF) + omUPauseFlag == 0) {
        sys->hamonTime += sys->hamonSpeed;
    }
    GXInitTexObj(&texObj, sys->hamonTex, 320, 240, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(&texObj, GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
    GXLoadTexObj(&texObj, GX_TEXMAP1);
    HuSprTexLoad(sys->hamonAnim[0], 0, 2, 1, 1, 1);
    HuSprTexLoad(sys->hamonAnim[1], 0, 3, 1, 1, 1);
    GXSetNumTexGens(3);
    GXSetNumTevStages(3);
    cam2 = &Hu3DCamera[Hu3DCameraNo - 2];
    transS = ((f32)cam2->scissorX + (f32)cam2->scissorW * 0.5f) / 640.0f;
    C_MTXLightPerspective(lightMtx, cam->fov, 1.2f, 0.5f, -0.5f, transS, 0.5f);
    PSMTXInverse(Hu3DCameraMtx, camInv);
    PSMTXConcat(camInv, drawObj->matrix, mtxB);
    PSMTXConcat(lightMtx, Hu3DCameraMtx, mtxA);
    PSMTXConcat(mtxA, mtxB, mtxA);
    GXLoadTexMtxImm(mtxA, GX_TEXMTX1, GX_MTX3x4);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_POS, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
    PSMTXTrans(mtxB, 0.0f, sys->hamonTime, 0.0f);
    PSMTXScale(mtxC, 0.8f, 0.8f, 1.0f);
    PSMTXConcat(mtxC, mtxB, mtxA);
    GXLoadTexMtxImm(mtxA, GX_TEXMTX0, GX_MTX2x4);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD2, GX_TG_MTX2x4, GX_TG_TEX0, (GX_TEXMTX9 + 3), GX_FALSE, GX_PTIDENTITY);
    color = sys->hamonColor;
    GXSetTevColor(GX_TEVREG0, color);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP1, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD2, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_TEXC, GX_CC_C0, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD1, GX_TEXMAP3, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_TEXC, GX_CC_A0, GX_CC_CPREV);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetNumIndStages(1);
    GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD1, GX_TEXMAP2);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_1, GX_ITS_1);
    GXSetTevIndWarp(GX_TEVSTAGE0, GX_INDTEXSTAGE0, GX_TRUE, GX_FALSE, GX_ITM_0);
    GXSetIndTexMtx(GX_ITM_0, (f32 (*)[3])sys->hamonPos, -1);
}
