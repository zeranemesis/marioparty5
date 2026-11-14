/* stage part D: mushroom/spike objects, object hit checks, rabbit push, dust/fire, stage 7 smoke, particle camera, aux.
 * Shared data used here and defined by part A in stage.c:
 *   SdStageColTri *stageColMesh; s32 stageColFloorNum; SdStageData stageDataTbl[9];
 *   SdStageWork stageWork (only .lavaY is read); SdStageSmoke stage07Smoke[20]; s32 auxATbl[8];
 */
extern s32 MgSeqModeGet(void);
extern s32 rand8(void);

static void StageObjCol(SdStageObj *obj);
static s32 StageKinokoCheck(Vec *pos, f32 radius);

/* Stage 7 smoke base positions (lbl_1_data_18AC). */
static Vec stage07SmokeBase[20] = {
    { -250.0f, -0.0f, 3750.0f }, { -230.0f, -0.0f, 3710.0f }, { -210.0f, -0.0f, 3680.0f },
    { -195.0f, -0.0f, 3655.0f }, { -180.0f, -0.0f, 3630.0f }, { -150.0f, -0.0f, 3590.0f },
    { -120.0f, -0.0f, 3550.0f }, { -80.0f, -0.0f, 3550.0f },  { -45.0f, -0.0f, 3550.0f },
    { -20.0f, -0.0f, 3550.0f },  { 250.0f, -0.0f, 3750.0f },  { 230.0f, -0.0f, 3710.0f },
    { 210.0f, -0.0f, 3680.0f },  { 195.0f, -0.0f, 3655.0f },  { 180.0f, -0.0f, 3630.0f },
    { 150.0f, -0.0f, 3590.0f },  { 120.0f, -0.0f, 3550.0f },  { 80.0f, -0.0f, 3550.0f },
    { 45.0f, -0.0f, 3550.0f },   { 20.0f, -0.0f, 3550.0f },
};

/* Height of the collision floor under pos (inlined twice in the original). */
static inline f32 StageKinokoFloorYGet(Vec *pos)
{
    Vec p;
    Vec up;
    Vec local;
    Vec dirLocal;
    Vec step;
    Vec toTri;
    Vec flat;
    Vec edge;
    SdStageColTri *tri;
    SdStageColTri *hit;
    f32 best;
    f32 dist;
    f32 dot;
    s32 col;
    s32 row;
    s32 k;
    s32 c;
    s32 inside;
    s32 i;

    p = *pos;
    p.y = -5000.0f;
    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;
    if (sys->stageNo == 0) {
        col = (s32)(48.0f + p.x / 50.0f);
        row = (s32)(24.0f + p.z / 100.0f);
        if (row > 0x2F) {
            row = 0x2F;
        } else if (row < 0) {
            row = 0;
        }
        best = 10000.0f;
        hit = NULL;
        for (k = 0; k < 3; k++) {
            c = col + k - 1;
            if (c > 0x5F) {
                c = 0x5F;
            } else if (c < 0) {
                c = 0;
            }
            tri = &stageColMesh[row * 0x60 + c];
            PSVECSubtract(&p, &tri->pos, &toTri);
            dist = PSVECMag(&toTri);
            if (best > dist) {
                best = dist;
                local = toTri;
                hit = tri;
            }
        }
        if (best == 10000.0f) {
            return -10000.0f;
        }
        PSMTXMultVec(hit->invMtx, &local, &local);
        PSMTXMultVec(hit->invMtx, &up, &dirLocal);
        PSVECScale(&up, &step, -local.y / dirLocal.y);
        PSVECAdd(&p, &step, &p);
        return p.y;
    }
    for (i = 0; i < stageColFloorNum; i++) {
        tri = &stageColMesh[i];
        PSVECSubtract(&p, &tri->pos, &local);
        flat = local;
        flat.y = 0.0f;
        if (PSVECMag(&flat) > tri->radius) {
            continue;
        }
        inside = 1;
        for (k = 0; k < 3; k++) {
            PSVECSubtract(&local, &tri->vtx[k], &edge);
            edge.y = 0.0f;
            if (PSVECMag(&edge) == 0.0f) {
                edge.x = 1.0f;
                edge.y = 1.0f;
                edge.z = 1.0f;
            }
            PSVECNormalize(&edge, &edge);
            dot = PSVECDotProduct(&tri->dir[k], &edge);
            if (dot < tri->dot[k]) {
                inside = 0;
                break;
            }
        }
        if (!inside) {
            continue;
        }
        PSMTXMultVec(tri->invMtx, &local, &local);
        PSMTXMultVec(tri->invMtx, &up, &dirLocal);
        PSVECScale(&up, &step, -local.y / dirLocal.y);
        PSVECAdd(&p, &step, &p);
        return p.y;
    }
    return -10000.0f;
}

static void StageKinokoUpdate(SdStageObj *obj)
{
    Vec target;
    Vec tmp;
    Vec mid;
    Vec axis;
    Vec a;
    Vec b;
    Vec copy;
    Mtx mtx;
    f32 t;
    f32 y;
    s32 who;
    s32 cnt;
    s32 own;

    if (MgSeqModeGet() != 5) {
        return;
    }
    switch (obj->mode) {
    case 1:
        who = 0;
        if (sys->gameType == 0) {
            if (sys->machine[0].hp == sys->machine[1].hp) {
                who = 0;
            } else if (sys->machine[0].hp < sys->machine[1].hp) {
                who = 1;
            } else {
                who = 2;
            }
        } else if (sys->gameType == 1) {
            who = SdFlagDispTimerGet(0);
            who -= SdFlagDispTimerGet(1);
            if (who == -1) {
                who = 1;
            } else if (who == 1) {
                who = 2;
            }
        } else if (sys->gameType == 2) {
            who = SdRabbitDispTimerGet(0);
            who -= SdRabbitDispTimerGet(1);
            if (who == -1) {
                who = 1;
            } else if (who == 1) {
                who = 2;
            }
        }
        if (who == 0) {
            a = sys->machine[0].curPos;
            b = sys->machine[1].curPos;
            b.y = 0.0f;
            a.y = 0.0f;
            mid.x = (a.x + b.x) * 0.5f;
            mid.y = 0.0f;
            mid.z = (a.z + b.z) * 0.5f;
            target = mid;
            PSVECSubtract(&a, &b, &a);
            if (PSVECMag(&a) == 0.0f) {
                a.x = 1.0f;
                a.y = 1.0f;
                a.z = 1.0f;
            }
            PSVECNormalize(&a, &a);
            MathPlaneRotMtxGet(&a, NULL, 0.0f, mtx);
            axis.x = mtx[0][0];
            axis.y = mtx[1][0];
            axis.z = mtx[2][0];
            a = axis;
            cnt = 0;
            do {
                t = 200.0f * (f32)(rand8() % 0x48 - 0x24);
                PSVECScale(&a, &tmp, t);
                PSVECAdd(&tmp, &target, &tmp);
                cnt++;
                if (cnt > 0x19) {
                    break;
                }
                copy = tmp;
                if (!StageKinokoCheck(&copy, 100.0f)) {
                    continue;
                }
                if (PSVECDistance(&tmp, &sys->machine[0].curPos) < 300.0f) {
                    continue;
                }
                target = tmp;
                break;
            } while (1);
        } else {
            own = who - 1;
            cnt = 0;
            do {
                copy = sys->machine[own].curPos;
                SdRandomPosGet(&target, 0, &copy, 0, 1000.0f, 300.0f);
                cnt++;
                if (cnt > 0x19) {
                    break;
                }
                tmp = target;
                if (!StageKinokoCheck(&tmp, 100.0f)) {
                    continue;
                }
                t = PSVECDistance(&target, &sys->machine[own ^ 1].curPos);
                if (PSVECDistance(&target, &sys->machine[own].curPos) > t) {
                    continue;
                }
                break;
            } while (1);
        }
        if (cnt > 0x19) {
            return;
        }
        target.y = StageKinokoFloorYGet(&target);
        obj->basePos = target;
        copy = target;
        SdImpactSoundStart(0x579, &copy, 1.0f);
        target.y -= 150.0f;
        Hu3DModelAttrReset(obj->mdlId, 1);
        Hu3DModelPosSetV(obj->mdlId, &target);
        if (sys->stageNo != 3) {
            target.y += 150.0f;
        } else {
            target.y += 180.0f;
        }
        Hu3DModelPosSetV(obj->shadowMdlId, &target);
        obj->timer = 0;
        obj->mode++;
        break;
    case 2:
        obj->timer++;
        t = MathLerp(1, (f32)obj->timer / 60.0f);
        y = 150.0f - 150.0f * t;
        Hu3DData[obj->mdlId].pos.y = obj->basePos.y - y;
        if ((f32)obj->timer >= 12.0f) {
            Hu3DModelAttrReset(obj->shadowMdlId, 1);
        }
        if ((f32)obj->timer >= 60.0f) {
            obj->dispF = 1;
            obj->pos = Hu3DData[obj->mdlId].pos;
            obj->timer = 0;
            obj->mode++;
        }
        break;
    case 3:
        obj->timer++;
        if ((f32)obj->timer >= 660.0f) {
            if (obj->timer % 2 != 0) {
                Hu3DModelAttrReset(obj->mdlId, 1);
                Hu3DModelAttrReset(obj->shadowMdlId, 1);
            } else {
                Hu3DModelAttrSet(obj->mdlId, 1);
                Hu3DModelAttrSet(obj->shadowMdlId, 1);
            }
        }
        if ((f32)obj->timer >= 900.0f) {
            Hu3DModelAttrSet(obj->mdlId, 1);
            Hu3DModelAttrSet(obj->shadowMdlId, 1);
            obj->timer = 0;
            obj->mode = 0;
            obj->dispF = 0;
        }
        StageObjCol(obj);
        break;
    }
}

static s32 StageKinokoCheck(Vec *pos, f32 radius)
{
    Vec p;
    Vec lim;
    Vec q;
    f32 top;
    f32 margin;
    f32 mx;
    f32 mz;
    f32 dist;
    f32 floorY;
    s32 type;

    p = *pos;
    type = stageDataTbl[sys->stageNo].type;
    lim.x = stageDataTbl[sys->stageNo].radius;
    lim.z = stageDataTbl[sys->stageNo].radius2;
    if (sys->stageNo == 0) {
        top = stageWork.lavaY - 25.0f;
    } else if (sys->stageNo == 2 || sys->stageNo == 7) {
        top = -25.0f;
    } else {
        top = -10000.0f;
    }
    lim.y = top;
    lim.x += -(200.0f + radius);
    lim.z += -(200.0f + radius);
    p.y = 0.0f;
    if (type == 0) {
        if (p.x < 0.0f) {
            p.x *= -1.0f;
        }
        if (p.z < 0.0f) {
            p.z *= -1.0f;
        }
        mx = lim.x - p.x;
        mz = lim.z - p.z;
        if (mx <= mz) {
            margin = mx;
        } else {
            margin = mz;
        }
    } else {
        dist = PSVECMag(&p);
        margin = lim.x - dist;
    }
    if (margin <= 0.0f) {
        return 0;
    }
    q = *pos;
    if (SdStageBaseObjHitCheck(&q, radius)) {
        return 0;
    }
    if (sys->stageNo == 7) {
        pos->y = 0.0f;
        dist = PSVECMag(pos);
        if (dist < 1000.0f + radius || dist > 2450.0f - radius) {
            return 0;
        }
        return 1;
    }
    floorY = StageKinokoFloorYGet(pos);
    if (sys->stageNo == 0) {
        top = stageWork.lavaY - 25.0f;
    } else if (sys->stageNo == 2 || sys->stageNo == 7) {
        top = -25.0f;
    } else {
        top = -10000.0f;
    }
    if (floorY <= 24.0f + top) {
        return 0;
    }
    return 1;
}

static void StageNKinokoUpdate(SdStageObj *obj)
{
    StageKinokoUpdate(obj);
}

static void StageObjCol(SdStageObj *obj)
{
    SdStageMachine *m;
    Vec v;
    Vec d;
    Vec n;
    Vec refl;
    Vec nv;
    Vec c;
    f32 dist;
    f32 push;
    s32 i;
    s32 mdlId;

    if (obj->dispF == 0) {
        return;
    }
    for (i = 0; i < 2; i++) {
        m = &sys->machine[i];
        v = m->curPos;
        if (m->attr & 2) {
            v.y -= m->wheelBase;
        }
        if (v.y - m->wheelBase > obj->pos.y + obj->height) {
            continue;
        }
        if (v.y + m->hitHeight < obj->pos.y) {
            continue;
        }
        PSVECSubtract(&v, &obj->pos, &v);
        v.y = 0.0f;
        dist = PSVECMag(&v);
        if (dist > m->hitRadius + obj->radius) {
            continue;
        }
        if (obj->type == 4 || obj->type == 5) {
            Hu3DModelAttrSet(obj->mdlId, 1);
            Hu3DModelAttrSet(obj->shadowMdlId, 1);
            obj->dispF = 0;
            obj->timer = 0;
            obj->mode = 0;
            m->effTimer = 0;
            if (obj->type == 4) {
                m->effType = 1;
                mdlId = sys->kinokoMdlId;
                SdPlayerSoundPlay(m, 2);
                HuAudFXPlayVolPan(m->playerNo + 0x575, 0x7F, (s16)(m->playerNo * 32 + 0x30));
            } else {
                m->effType = 2;
                mdlId = sys->togezoMdlId;
                SdPlayerSoundPlay(m, 3);
                HuAudFXPlayVolPan(m->playerNo + 0x577, 0x7F, (s16)(m->playerNo * 32 + 0x30));
            }
            Hu3DMotionTimeSet(mdlId, 0.0f);
            SdVibrate(m->playerNo, 1, 15);
            return;
        }
        obj->hitF = 1;
        PSVECSubtract(&m->basePos, &obj->pos, &d);
        push = PSVECMag(&d);
        PSVECSubtract(&m->curPos, &obj->pos, &d);
        push -= PSVECMag(&d);
        if (PSVECMag(&v) == 0.0f) {
            v.x = 1.0f;
            v.y = 1.0f;
            v.z = 1.0f;
        }
        PSVECNormalize(&v, &v);
        PSVECScale(&v, &v, (m->hitRadius + obj->radius) - dist);
        if (obj->typeP->bounceF == 0.0f || obj->mode < 2 || obj->mode >= 4) {
            if (m->mode == 2) {
                n.x = m->planeMtx[0][2];
                n.y = m->planeMtx[1][2];
                n.z = m->planeMtx[2][2];
                refl = n;
                if (PSVECMag(&d) == 0.0f) {
                    d.x = 1.0f;
                    d.y = 1.0f;
                    d.z = 1.0f;
                }
                PSVECNormalize(&d, &d);
                PSVECScale(&d, &d, -1.0f);
                C_VECReflect(&refl, &d, &refl);
                refl.y = 0.0f;
                MathPlaneRotMtxGet(&refl, NULL, 0.0f, m->planeMtx);
            }
            if (PSVECMag(&m->rotVel) > 1.5f + (f32)m->speedLv && m->contactF == 0) {
                if (sys->timeUpF != 0 || sys->allDoneF != 0) {
                    m->contactF = 1;
                }
                HuAudFXPlayVolPan(m->playerNo + 0x563, 0x7F, (s16)(m->playerNo * 32 + 0x30));
                SdVibrate(m->playerNo, 0, 5);
            }
            m->curPos.x += v.x;
            m->curPos.z += v.z;
        }
        if (PSVECMag(&v) == 0.0f) {
            v.x = 1.0f;
            v.y = 1.0f;
            v.z = 1.0f;
        }
        PSVECNormalize(&v, &v);
        if (push < 5.0f) {
            push = 5.0f;
        }
        PSVECScale(&v, &v, 0.5f * push);
        nv.x = v.x;
        nv.y = 0.0f;
        nv.z = v.z;
        m->vel = nv;
        if (m->unk210 == 0 && obj->endF != 0) {
            PSVECSubtract(&m->curPos, &obj->pos, &v);
            if (m->bodyType == 10) {
                c = v;
                SdMachineDamage(m, &c, (s32)((f32)obj->typeP->damage * 0.5f));
            } else {
                c = v;
                SdMachineDamage(m, &c, obj->typeP->damage);
            }
        }
    }
}

s32 SdStageObjHitCheck(Vec *pos, f32 radius)
{
    Vec p = *pos;

    return SdStageObjHitCheck2(&p, radius, 0);
}

s32 SdStageBaseObjHitCheck(Vec *pos, f32 radius)
{
    Vec p = *pos;

    return SdStageBaseObjHitCheck2(&p, radius, 0);
}

s32 SdStageObjHitCheck2(Vec *pos, f32 radius, s32 type)
{
    SdStageObj *obj;
    Vec c;
    Vec save;
    f32 dist;
    s32 i;

    if (sys->obj == NULL) {
        return 0;
    }
    for (i = 0; i < 8; i++) {
        obj = &sys->obj[i];
        if (obj->mdlId == -1) {
            continue;
        }
        if (obj->dispF == 0) {
            continue;
        }
        PSVECAdd(&obj->pos, &obj->typeP->offset, &c);
        PSVECSubtract(pos, &c, &c);
        save = c;
        if (obj->typeP->flags == 1) {
            c.y = 0.0f;
        }
        dist = PSVECMag(&c);
        if (dist > radius + obj->radius) {
            continue;
        }
        if (type == 1) {
            if (obj->typeP->flags == 1) {
                if (save.y < 0.0f) {
                    continue;
                }
                if (save.y > obj->height) {
                    continue;
                }
            }
            obj->hitF = 1;
        }
        return 1;
    }
    return 0;
}

s32 SdStageBaseObjHitCheck2(Vec *pos, f32 radius, s32 type)
{
    SdStageObj *obj;
    Vec c;
    Vec save;
    f32 dist;
    s32 i;

    if (sys->obj == NULL) {
        return 0;
    }
    for (i = 0; i < 8; i++) {
        obj = &sys->obj[i];
        if (obj->mdlId == -1) {
            continue;
        }
        PSVECAdd(&obj->pos, &obj->typeP->offset, &c);
        PSVECSubtract(pos, &c, &c);
        save = c;
        if (obj->typeP->flags == 1) {
            c.y = 0.0f;
        }
        dist = PSVECMag(&c);
        if (dist > radius + obj->typeP->radius) {
            continue;
        }
        if (type == 1) {
            if (obj->typeP->flags == 1) {
                if (save.y < 0.0f) {
                    continue;
                }
                if (save.y > obj->typeP->height) {
                    continue;
                }
            }
            obj->hitF = 1;
        }
        return 1;
    }
    return 0;
}

static void StageRabbitUpdate(void)
{
    SdStageMachine *m;
    Vec rpos;
    Vec v;
    Vec d;
    Vec nv;
    f32 dist;
    f32 push;
    s32 i;

    if (sys->gameType != 2) {
        return;
    }
    if (SdRabbitDoneCheck() != 0) {
        return;
    }
    if (SdRabbitCheck() == 0) {
        return;
    }
    SdRabbitPosGet(&rpos);
    for (i = 0; i < 2; i++) {
        m = &sys->machine[i];
        v = m->curPos;
        if (v.y - m->wheelBase > 90.0f + rpos.y) {
            continue;
        }
        if (v.y + m->hitHeight < rpos.y) {
            continue;
        }
        PSVECSubtract(&v, &rpos, &v);
        v.y = 0.0f;
        dist = PSVECMag(&v);
        if (dist > 100.0f + m->hitRadius) {
            continue;
        }
        PSVECSubtract(&m->basePos, &rpos, &d);
        push = PSVECMag(&d);
        PSVECSubtract(&m->curPos, &rpos, &d);
        push -= PSVECMag(&d);
        if (PSVECMag(&v) == 0.0f) {
            v.x = 1.0f;
            v.y = 1.0f;
            v.z = 1.0f;
        }
        PSVECNormalize(&v, &v);
        PSVECScale(&v, &v, (100.0f + m->hitRadius) - dist);
        m->curPos.x += v.x;
        m->curPos.z += v.z;
        if (PSVECMag(&v) == 0.0f) {
            v.x = 1.0f;
            v.y = 1.0f;
            v.z = 1.0f;
        }
        PSVECNormalize(&v, &v);
        if (push < 5.0f) {
            push = 5.0f;
        }
        PSVECScale(&v, &v, 0.5f * push);
        nv.x = v.x;
        nv.y = 0.0f;
        nv.z = v.z;
        m->vel = nv;
    }
}

static void StageCreateDust(SdStageObj *obj, f32 scale)
{
    HU3DPARTICLE *dust = Hu3DData[sys->dustMdlId].hookData;
    HU3DPARTICLEDATA *dustData = dust->data + obj->fireIdx;
    HU3DPARTICLE *flame = Hu3DData[sys->flameMdlId].hookData;
    HU3DPARTICLEDATA *flameData = flame->data + obj->flameIdx;
    Vec pos = obj->pos;

    SdEffectDustCreateData(&pos, dustData, flameData, scale);
}

static void StageUpdateFire(SdStageObj *obj)
{
    HU3DPARTICLE *dust;
    HU3DPARTICLE *flame;
    HU3DPARTICLEDATA *dustData;
    HU3DPARTICLEDATA *flameData;

    if (obj->fireIdx == -1) {
        return;
    }
    dust = Hu3DData[sys->dustMdlId].hookData;
    dustData = dust->data + obj->fireIdx;
    flame = Hu3DData[sys->flameMdlId].hookData;
    flameData = flame->data + obj->flameIdx;
    SdEffectFlameUpdate(dustData, 0x30);
    SdEffectFlameUpdateData(flameData, 0x19);
}

static void Stage07SmokeCreate(void);
static void Stage07SmokeUpdate(void);

static void Stage07SmokeMain(void)
{
    if (sys->frameCount % 5 == 0) {
        Stage07SmokeCreate();
    }
    Stage07SmokeUpdate();
}

static void Stage07SmokeCreate(void)
{
    HU3DPARTICLE *par = Hu3DData[sys->smokeMdlId].hookData;
    HU3DPARTICLEDATA *base = par->data + sys->smokeSlot;
    HU3DPARTICLEDATA *d;
    Mtx rot;
    Vec vel;
    Vec pos;
    Vec rnd;
    GXColor color;
    s32 i;
    s32 j;
    s32 k;

    for (i = 0; i < 20; i++) {
        stage07Smoke[i].velY = (s32)(6.0f + (f32)(rand8() % 8));
        stage07Smoke[i].x = (s32)(stage07SmokeBase[i].x + (f32)(rand8() % 20) - 10.0f);
        stage07Smoke[i].y = (s32)(stage07SmokeBase[i].y + (f32)(rand8() % 20) - 10.0f);
        stage07Smoke[i].z = (s32)(stage07SmokeBase[i].z + (f32)(rand8() % 20) - 10.0f);
        stage07Smoke[i].scale = (s32)(50.0f + (f32)(rand8() % 120));
        stage07Smoke[i].color = 0x80 + rand8() % 128;
    }
    for (i = 0; i < 8; i++) {
        PSMTXRotRad(rot, 'y', 0.017453292f * (45.0f * (f32)i));
        k = 0;
        for (j = 0; j < 0x3C; j++) {
            d = &base[i * 0x3C + j];
            if (d->time != 0) {
                continue;
            }
            d->time = 1;
            d->parManId = 0;
            vel.x = 0.0f;
            vel.y = (f32)stage07Smoke[k].velY;
            vel.z = 0.0f;
            d->vel = vel;
            pos.x = (f32)stage07Smoke[k].x;
            pos.y = (f32)stage07Smoke[k].y;
            pos.z = (f32)stage07Smoke[k].z;
            d->pos = pos;
            d->scale = (f32)stage07Smoke[k].scale;
            color.r = (u8)stage07Smoke[k].color;
            color.g = (u8)stage07Smoke[k].color;
            color.b = (u8)stage07Smoke[k].color;
            color.a = 0x80;
            d->color = color;
            PSMTXMultVec(rot, &d->pos, &d->pos);
            k++;
            if (k >= 20) {
                break;
            }
        }
    }
    for (i = 0; i < 20; i++) {
        stage07Smoke[i].velY = (s32)(2.0f + (f32)(rand8() % 2));
        stage07Smoke[i].scale = (s32)(30.0f + (f32)(rand8() % 40));
    }
    k = 0;
    for (j = 0; j < 30; j++) {
        d = &base[0x1E0 + j];
        if (d->time != 0) {
            continue;
        }
        d->time = 1;
        d->parManId = 0;
        vel.x = 0.0f;
        vel.y = (f32)stage07Smoke[k].velY;
        vel.z = 0.0f;
        d->vel = vel;
        rnd.x = 0.0f;
        rnd.y = -10.0f;
        rnd.z = 0.0f;
        SdRandomPosGet(&pos, 0, &rnd, 0, 300.0f, 0.0f);
        d->pos = pos;
        d->scale = (f32)stage07Smoke[k].scale;
        color.r = (u8)stage07Smoke[k].color;
        color.g = (u8)stage07Smoke[k].color;
        color.b = (u8)stage07Smoke[k].color;
        color.a = 0xFF;
        d->color = color;
        k++;
        if (k >= 10) {
            break;
        }
    }
}

static void Stage07SmokeUpdate(void)
{
    HU3DPARTICLE *par = Hu3DData[sys->smokeMdlId].hookData;
    HU3DPARTICLEDATA *base = par->data + sys->smokeSlot;
    HU3DPARTICLEDATA *d;
    s32 i;

    for (i = 0; i < 0x1FE; i++) {
        d = &base[i];
        if (d->time == 0) {
            continue;
        }
        if (d->scale > 0.0f) {
            d->pos.y += d->vel.y;
            d->vel.y *= 1.01f;
            d->scale *= 0.95f;
            d->color.r = (u8)((f32)d->color.r * 0.995f);
            d->color.g = (u8)((f32)d->color.g * 0.98f);
            d->color.b = (u8)((f32)d->color.b * 0.9f);
            d->color.a = (u8)((f32)d->color.a * 0.95f);
            if (d->color.a < 2) {
                d->scale = 0.0f;
            }
        }
        d->parManId++;
        if (d->parManId >= 15) {
            d->parManId = 0;
            d->time = 0;
            d->scale = 0.0f;
        }
    }
}

void SdParticleCameraSet(s32 cameraNo)
{
    HU3DPARTICLE *par = Hu3DData[sys->smokeMdlId].hookData;
    HU3DPARTICLEDATA *base = par->data + sys->smokeSlot;
    s32 i;

    for (i = 0; i < 0x1FE; i++) {
        base[i].cameraBit = (s16)sdCameraBitTbl[cameraNo];
    }
}

void SdAuxSet(void)
{
    HuAudAUXSet(auxATbl[sys->stageNo], 1);
}
