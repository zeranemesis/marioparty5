/* gun part c: effect updates/creates for KoopaFire, Aikun, Laser, KoopaArm, Star, flame/dust helpers, SdBulletDraw */

extern s32 rand8(void);

/* part B / part C particle data updaters (called from effect update/create) */
static void PakkunEffectDataUpdate(HU3DPARTICLEDATA *data, s32 count);
static void PakkunEffectData2Update(HU3DPARTICLEDATA *data, s32 count);
static void KoopaFireEffectDataUpdate(HU3DPARTICLEDATA *data, s32 count);
static void AikunEffectDataUpdate(HU3DPARTICLEDATA *data, s32 count);
static void LaserEffectDataUpdate(HU3DPARTICLEDATA *data, s32 count);
static void StarEffectDataUpdate(HU3DPARTICLEDATA *data, s32 count);

/* scale a particle colour channel as the original (f32 -> u8 truncation) */
#define SD_COLSCALE(ch, s) ((u8)((f32)(ch) * (s)))

static void PakkunEffectDataUpdate(HU3DPARTICLEDATA *data, s32 count)
{
    s32 i;

    for (i = 0; i < count; i++) {
        HU3DPARTICLEDATA *p = &data[i];
        p->pos.x = p->pos.x + p->vel.x;
        p->pos.y = p->pos.y + (p->vel.y + p->accel.y);
        p->pos.z = p->pos.z + p->vel.z;
        PSVECScale(&p->vel, &p->vel, 0.98f);
        p->accel.y = p->accel.y * 1.08f;
        p->scale = p->scale * 0.98f;
        if (data[0].parManId % 2 == 0) {
            p->zRot = p->zRot + 45.0f;
        }
    }
    data[0].parManId++;
    if (data[0].parManId >= 60) {
        data[0].parManId = 0;
        data[0].time = 0;
        for (i = 0; i < count; i++) {
            data[i].scale = 0.0f;
        }
    }
}

static void PakkunEffectData2Update(HU3DPARTICLEDATA *data, s32 count)
{
    s32 i;

    for (i = 0; i < count; i++) {
        HU3DPARTICLEDATA *p = &data[i];
        if (p->time == 0) {
            continue;
        }
        p->pos.x = p->pos.x + p->vel.x;
        p->pos.y = p->pos.y + (p->vel.y + p->accel.y);
        p->pos.z = p->pos.z + p->vel.z;
        PSVECScale(&p->vel, &p->vel, 0.9f);
        p->accel.y = p->accel.y * 1.1f;
        p->scale = p->scale * 0.98f;
        if (p->parManId % 2 == 0) {
            p->zRot = p->zRot + 45.0f;
        }
        p->parManId++;
        if (p->parManId >= 36) {
            p->parManId = 0;
            p->time = 0;
            p->scale = 0.0f;
        }
    }
}

static void BulletExplodeUpdate(SdBullet *bullet)
{
    KoopaFireEffectDataUpdate(BulletPartBase(bullet, 5), 30);
}

static void KoopaFireEffectCreate(SdBullet *bullet, Mtx mtx)
{
    HU3DPARTICLEDATA *data = BulletPartBase(bullet, 5);
    Mtx base, rot, m;
    Vec dir;
    s32 i, r;

    dir.x = 0.0f;
    dir.y = 0.0f;
    dir.z = 1.0f;
    PSMTXCopy(mtx, base);
    base[0][3] = 0.0f;
    base[1][3] = 0.0f;
    base[2][3] = 0.0f;
    PSVECNormalize(&dir, &dir);
    for (i = 0; i < 30; i++) {
        HU3DPARTICLEDATA *p = &data[i];
        if (p->time != 0) {
            continue;
        }
        r = rand8() % 31 - 15;
        PSMTXRotRad(rot, 'y', 0.017453292f * (f32)r);
        PSMTXConcat(base, rot, m);
        r = rand8() % 15 - 7;
        PSMTXRotRad(rot, 'x', 0.017453292f * (f32)r);
        PSMTXConcat(m, rot, m);
        p->time = 1;
        p->parManId = 0;
        p->pos.x = mtx[0][3];
        p->pos.y = mtx[1][3];
        p->pos.z = mtx[2][3];
        PSMTXMultVec(m, &dir, &p->vel);
        PSVECScale(&p->vel, &p->vel, 25.0f + bullet->prevPos.y);
        p->accel.y = 1.0f;
        p->color.r = 0xFF;
        p->color.g = 0xFF;
        p->color.b = 0xFF;
        p->color.a = 0xFF;
        p->scale = 45.0f + (f32)(rand8() % 15);
        break;
    }
}

static void KoopaFireEffectDataUpdate(HU3DPARTICLEDATA *data, s32 count)
{
    s32 i;

    for (i = 0; i < count; i++) {
        HU3DPARTICLEDATA *p = &data[i];
        if (p->time == 0) {
            continue;
        }
        if (p->scale > 0.0f) {
            p->pos.x = p->pos.x + p->vel.x;
            p->pos.y = p->pos.y + (p->vel.y + p->accel.y);
            p->pos.z = p->pos.z + p->vel.z;
            PSVECScale(&p->vel, &p->vel, 0.95f);
            p->accel.y = p->accel.y * 1.02f;
            p->scale = p->scale * 1.08f;
            if (p->scale > 200.0f) {
                p->scale = 200.0f;
            }
            p->color.r = SD_COLSCALE(p->color.r, 0.995f);
            p->color.g = SD_COLSCALE(p->color.g, 0.98f);
            p->color.b = SD_COLSCALE(p->color.b, 0.9f);
            p->color.a = SD_COLSCALE(p->color.a, 0.92f);
            if (p->color.a < 2) {
                p->scale = 0.0f;
            }
        }
        p->parManId++;
        if (p->parManId >= 60) {
            p->parManId = 0;
            p->time = 0;
            p->scale = 0.0f;
        }
    }
}

static void AikunEffectUpdate(SdBullet *bullet)
{
    HU3DPARTICLE *eff = (HU3DPARTICLE *)Hu3DData[sys->partMdlId[6]].hookData;
    HU3DPARTICLEDATA *data = eff->data + bullet->partIdx[6];

    if (data[0].time != 0) {
        AikunEffectDataUpdate(data, 3);
    }
    if (data[3].time != 0) {
        AikunEffectDataUpdate(data + 3, 3);
    }
}

static void AikunEffectCreate(SdBullet *bullet, Mtx mtx)
{
    HU3DPARTICLEDATA *data = BulletPartBase(bullet, 6);
    s32 i;

    data[0].time = 1;
    data[0].parManId = 0;
    for (i = 0; i < 3; i++) {
        HU3DPARTICLEDATA *p = &data[i];
        p->pos.x = mtx[0][3];
        p->pos.y = mtx[1][3];
        p->pos.z = mtx[2][3];
        p->color.r = 0xFF;
        p->color.g = 0xFF;
        p->color.b = 0xFF;
        p->color.a = 0xFF;
        p->scale = 50.0f;
    }
}

static void AikunImpactCreate(SdBullet *bullet, Mtx mtx)
{
    HU3DPARTICLEDATA *data = BulletPartBase(bullet, 6) + 3;
    s32 i;

    data[0].time = 1;
    data[0].parManId = 0;
    for (i = 0; i < 3; i++) {
        HU3DPARTICLEDATA *p = &data[i];
        p->pos.x = mtx[0][3];
        p->pos.y = mtx[1][3];
        p->pos.z = mtx[2][3];
        p->color.r = 0xFF;
        p->color.g = 0xFF;
        p->color.b = 0xFF;
        p->color.a = 0xFF;
        p->scale = 50.0f;
    }
}

static void AikunEffectDataUpdate(HU3DPARTICLEDATA *data, s32 count)
{
    s32 i;

    for (i = 0; i < count; i++) {
        HU3DPARTICLEDATA *p = &data[i];
        if (data[0].parManId >= i * 10 + 60) {
            p->scale = 0.0f;
        } else if (data[0].parManId >= i * 10) {
            PSVECScale(&p->vel, &p->vel, 0.9f);
            p->scale = p->scale * 1.04f;
            p->color.a = SD_COLSCALE(p->color.a, 0.9f);
        }
    }
    data[0].parManId++;
    if (data[0].parManId >= 80) {
        data[0].parManId = 0;
        data[0].time = 0;
        for (i = 0; i < count; i++) {
            data[i].scale = 0.0f;
        }
    }
}

static void LaserAirEffectUpdate(SdBullet *bullet, Mtx mtx)
{
    HU3DPARTICLEDATA *data = BulletPartBase(bullet, 10);
    GXColor color = { 0xFF, 0xFF, 0xFF, 0xFF };
    Vec dir, pos;
    s32 i;

    dir.x = mtx[0][2];
    dir.y = mtx[1][2];
    dir.z = mtx[2][2];
    pos = dir;
    PSVECScale(&pos, &pos, -80.0f);
    PSVECAdd(&bullet->pos, &pos, &pos);
    for (i = 0; i < 20; i++) {
        HU3DPARTICLEDATA *p = &data[i];
        if (p->time != 0) {
            continue;
        }
        p->time = 1;
        p->parManId = 0;
        p->vel.x = 0.0f;
        p->vel.y = 0.0f;
        p->vel.z = 0.0f;
        p->accel.x = 0.0f;
        p->accel.y = 0.0f;
        p->accel.z = 0.0f;
        p->pos = pos;
        p->scale = 45.0f;
        p->color = color;
        break;
    }
}

static void LaserEffectUpdate(SdBullet *bullet)
{
    LaserEffectDataUpdate(BulletPartBase(bullet, 10), 20);
}

static void LaserEffectDataUpdate(HU3DPARTICLEDATA *data, s32 count)
{
    GXColor base = { 0x00, 0xFF, 0xFF, 0xB4 };
    s32 i;

    for (i = 0; i < count; i++) {
        HU3DPARTICLEDATA *p = &data[i];
        if (p->time == 0) {
            continue;
        }
        p->scale = p->scale * 0.98f;
        if (p->parManId <= 6) {
            p->color.r = 255 - (255 - base.r) * p->parManId / 6;
            p->color.g = 255 - (255 - base.g) * p->parManId / 6;
            p->color.b = 255 - (255 - base.b) * p->parManId / 6;
        }
        p->color.a = SD_COLSCALE(p->color.a, 0.85f);
        p->parManId++;
        if (p->parManId >= 20) {
            p->parManId = 0;
            p->time = 0;
            p->scale = 0.0f;
        }
    }
}

static void KoopaArmEffectUpdate(SdBullet *bullet)
{
    HU3DPARTICLEDATA *data = BulletPartBase(bullet, 3);
    s32 i;

    if (data[0].time == 0) {
        return;
    }
    for (i = 0; i < 8; i++) {
        HU3DPARTICLEDATA *p = &data[i];
        p->pos.x = p->pos.x + p->vel.x;
        p->pos.y = p->pos.y + p->vel.y;
        p->pos.z = p->pos.z + p->vel.z;
        PSVECScale(&p->vel, &p->vel, 0.9f);
        p->scale = p->scale * 1.04f;
        p->color.a = SD_COLSCALE(p->color.a, 0.9f);
    }
    data[0].parManId++;
    if (data[0].parManId >= 60) {
        data[0].parManId = 0;
        data[0].time = 0;
        for (i = 0; i < 8; i++) {
            data[i].scale = 0.0f;
        }
    }
}

static void StarEffectUpdate(SdBullet *bullet)
{
    HU3DPARTICLEDATA *data = BulletPartBase(bullet, 8);

    if (data[0].time != 0) {
        StarEffectDataUpdate(data, 12);
    }
}

static void KoopaArmImpactCreate(SdBullet *bullet, Mtx mtx)
{
    HU3DPARTICLEDATA *data = BulletPartBase(bullet, 8);
    Mtx base, rot;
    Vec dirA, dirB;
    s32 i;

    dirA.x = 0.0f;
    dirA.y = 1.0f;
    dirA.z = 1.5f;
    dirB.x = 0.0f;
    dirB.y = 1.0f;
    dirB.z = -0.25f;
    PSMTXCopy(mtx, base);
    base[0][3] = 0.0f;
    base[1][3] = 0.0f;
    base[2][3] = 0.0f;
    PSVECNormalize(&dirA, &dirA);
    PSVECNormalize(&dirB, &dirB);
    data[0].time = 1;
    data[0].parManId = 0;
    for (i = 0; i < 8; i++) {
        HU3DPARTICLEDATA *p = &data[i];
        p->pos.x = mtx[0][3];
        p->pos.y = mtx[1][3];
        p->pos.z = mtx[2][3];
        PSMTXRotRad(rot, 'z', 0.017453292f * (45.0f * (f32)i));
        PSMTXConcat(base, rot, rot);
        PSMTXMultVec(rot, &dirA, &p->vel);
        PSVECScale(&p->vel, &p->vel, 25.0f);
        p->color.a = 0xFF;
        p->scale = 30.0f;
    }
    for (i = 8; i < 12; i++) {
        HU3DPARTICLEDATA *p = &data[i];
        p->pos.x = mtx[0][3];
        p->pos.y = mtx[1][3];
        p->pos.z = mtx[2][3];
        PSMTXRotRad(rot, 'z', 0.017453292f * (60.0f + 90.0f * (f32)(i - 8)));
        PSMTXConcat(base, rot, rot);
        PSMTXMultVec(rot, &dirB, &p->vel);
        PSVECScale(&p->vel, &p->vel, 40.0f);
        p->color.a = 0xFF;
        p->scale = 15.0f;
    }
}

static void StarEffectDataUpdate(HU3DPARTICLEDATA *data, s32 count)
{
    s32 i;

    for (i = 0; i < count; i++) {
        HU3DPARTICLEDATA *p = &data[i];
        p->pos.x = p->pos.x + p->vel.x;
        p->pos.y = p->pos.y + p->vel.y;
        p->pos.z = p->pos.z + p->vel.z;
        PSVECScale(&p->vel, &p->vel, 0.9f);
        p->scale = p->scale * 1.04f;
        if (p->scale > 80.0f) {
            p->scale = 80.0f;
        }
        p->color.a = SD_COLSCALE(p->color.a, 0.95f);
    }
    data[0].parManId++;
    if (data[0].parManId >= 60) {
        data[0].parManId = 0;
        data[0].time = 0;
        for (i = 0; i < count; i++) {
            data[i].scale = 0.0f;
        }
    }
}

void SdEffectDustCreateData(Vec *pos, void *p1, void *p2, f32 scale)
{
    HU3DPARTICLEDATA *d1 = p1;
    HU3DPARTICLEDATA *d2 = p2;
    Mtx base, rot;
    Vec va, vb, vc, vd, ve;
    s32 i;

    va.x = 0.0f; va.y = 5.0f; va.z = 1.0f;
    vb.x = 0.0f; vb.y = 1.5f; vb.z = 1.0f;
    vc.x = 0.0f; vc.y = 1.0f; vc.z = 1.5f;
    vd.x = 0.0f; vd.y = 0.0f; vd.z = 1.0f;
    ve.x = 0.0f; ve.y = 0.15f; ve.z = 1.0f;
    PSMTXIdentity(base);
    PSVECNormalize(&va, &va);
    PSVECNormalize(&vb, &vb);
    PSVECNormalize(&vc, &vc);
    PSVECNormalize(&vd, &vd);
    PSVECNormalize(&ve, &ve);

    d1[0].time = 1;
    for (i = 0; i < 48; i++) {
        d1[i].parManId = 0;
        d1[i].pos = *pos;
        d1[i].color.a = 0xFF;
        d1[i].color.b = 0xFF;
        d1[i].color.g = 0xFF;
        d1[i].color.r = 0xFF;
    }
    for (i = 0; i < 32; i++) {
        PSMTXRotRad(rot, 'y', 0.017453292f * (360.0f * (f32)i * 0.03125f));
        PSMTXConcat(base, rot, rot);
        PSMTXMultVec(rot, &ve, &d1[i].vel);
        PSVECScale(&d1[i].vel, &d1[i].vel, 40.0f * scale);
        d1[i].scale = ((i % 2 != 0) ? 50.0f : 60.0f) * scale;
    }
    for (i = 32; i < 48; i++) {
        PSMTXRotRad(rot, 'y', 0.017453292f * (360.0f * (f32)i * 0.0625f));
        PSMTXConcat(base, rot, rot);
        PSMTXMultVec(rot, &va, &d1[i].vel);
        PSVECScale(&d1[i].vel, &d1[i].vel, 40.0f * scale);
        d1[i].scale = ((i % 2 != 0) ? 50.0f : 40.0f) * scale;
    }

    d2[0].time = 1;
    for (i = 0; i < 25; i++) {
        d2[i].parManId = 0;
        d2[i].pos = *pos;
        d2[i].color.a = 0xFF;
        d2[i].color.b = 0xFF;
        d2[i].color.g = 0xFF;
        d2[i].color.r = 0xFF;
    }
    d2[0].vel.x = 0.0f;
    d2[0].vel.y = 1.0f;
    d2[0].vel.z = 0.0f;
    PSVECScale(&d2[0].vel, &d2[0].vel, 15.0f * scale);
    d2[0].scale = 100.0f * scale;
    for (i = 1; i < 4; i++) {
        PSMTXRotRad(rot, 'y', 0.017453292f * (120.0f * (f32)i));
        PSMTXMultVec(rot, &vb, &d2[i].vel);
        PSVECScale(&d2[i].vel, &d2[i].vel, 15.0f * scale);
        d2[i].scale = 100.0f * scale;
    }
    for (i = 4; i < 9; i++) {
        PSMTXRotRad(rot, 'y', 0.017453292f * (72.0f * (f32)i));
        PSMTXMultVec(rot, &vc, &d2[i].vel);
        PSVECScale(&d2[i].vel, &d2[i].vel, 15.0f * scale);
        d2[i].scale = 100.0f * scale;
    }
    for (i = 9; i < 16; i++) {
        PSMTXRotRad(rot, 'y', 0.017453292f * (51.5f * (f32)i));
        PSMTXMultVec(rot, &vd, &d2[i].vel);
        PSVECScale(&d2[i].vel, &d2[i].vel, 15.0f * scale);
        d2[i].scale = 100.0f * scale;
    }
    for (i = 16; i < 21; i++) {
        PSMTXRotRad(rot, 'y', 0.017453292f * (72.0f * (f32)i));
        PSMTXMultVec(rot, &vc, &d2[i].vel);
        PSVECScale(&d2[i].vel, &d2[i].vel, -15.0f * scale);
        d2[i].scale = 100.0f * scale;
    }
    for (i = 21; i < 24; i++) {
        PSMTXRotRad(rot, 'y', 0.017453292f * (120.0f * (f32)i));
        PSMTXMultVec(rot, &vb, &d2[i].vel);
        PSVECScale(&d2[i].vel, &d2[i].vel, -15.0f * scale);
        d2[i].scale = 100.0f * scale;
    }
    for (i = 24; i < 25; i++) {
        d2[i].vel.x = 0.0f;
        d2[i].vel.y = 1.0f;
        d2[i].vel.z = 0.0f;
        PSVECScale(&d2[i].vel, &d2[i].vel, -12.0f * scale);
        d2[i].scale = 100.0f * scale;
    }
}

void SdEffectFlameUpdate(void *p, s32 count)
{
    HU3DPARTICLEDATA *data = p;
    s32 i;

    for (i = 0; i < count; i++) {
        HU3DPARTICLEDATA *q = &data[i];
        if (q->scale == 0.0f) {
            continue;
        }
        q->pos.x = q->pos.x + q->vel.x;
        q->pos.y = q->pos.y + q->vel.y;
        q->pos.z = q->pos.z + q->vel.z;
        PSVECScale(&q->vel, &q->vel, 0.9f);
        if (data[0].parManId % 2 != 0) {
            q->zRot = q->zRot + 45.0f;
        }
        q->scale = q->scale * 1.04f;
        if (q->scale > 320.0f) {
            q->scale = 320.0f;
        }
        q->color.a = SD_COLSCALE(q->color.a, 0.9f);
        if (q->color.a < 2) {
            q->scale = 0.0f;
        }
    }
    data[0].parManId++;
    if (data[0].parManId >= 60) {
        data[0].parManId = 0;
        data[0].time = 0;
        for (i = 0; i < count; i++) {
            data[i].scale = 0.0f;
        }
    }
}

void SdEffectFlameUpdateData(void *p, s32 count)
{
    HU3DPARTICLEDATA *data = p;
    s32 i;

    for (i = 0; i < count; i++) {
        HU3DPARTICLEDATA *q = &data[i];
        if (q->scale == 0.0f) {
            continue;
        }
        q->pos.x = q->pos.x + q->vel.x;
        q->pos.y = q->pos.y + q->vel.y;
        q->pos.z = q->pos.z + q->vel.z;
        PSVECScale(&q->vel, &q->vel, 0.9f);
        q->scale = q->scale * 1.05f;
        if (q->scale > 320.0f) {
            q->scale = 320.0f;
        }
        q->color.r = SD_COLSCALE(q->color.r, 0.995f);
        q->color.g = SD_COLSCALE(q->color.g, 0.98f);
        q->color.b = SD_COLSCALE(q->color.b, 0.9f);
        q->color.a = SD_COLSCALE(q->color.a, 0.92f);
        if (q->color.a < 2) {
            q->scale = 0.0f;
        }
    }
    data[0].parManId++;
    if (data[0].parManId >= 60) {
        data[0].parManId = 0;
        data[0].time = 0;
        for (i = 0; i < count; i++) {
            data[i].scale = 0.0f;
        }
    }
}

void SdBulletDraw(HU3DMODEL *modelP, Mtx *mtx)
{
    Mtx m;
    Vec saved;
    SdMachine *machine;
    SdBullet *bullet;
    HU3DMODELID mdlId;
    s32 p, j, k;

    if (MgSeqModeGet() != 5) {
        return;
    }
    Hu3DModelObjDrawInit();
    for (p = 0; p < 2; p++) {
        machine = &sys->machine[p];
        if (machine->bulletType != 4 && machine->bulletType != 1) {
            continue;
        }
        mdlId = machine->gun[0].bullet[0].mdlId;
        Hu3DModelAttrReset(mdlId, 1);
        for (j = 0; j < machine->gunNum; j++) {
            for (k = 0; k < machine->gunBulletNum; k++) {
                bullet = &machine->gun[j].bullet[k];
                if (bullet->mode == 0) {
                    continue;
                }
                PSMTXTrans(m, bullet->pos.x, bullet->pos.y, bullet->pos.z);
                PSMTXConcat(*mtx, m, m);
                saved.x = m[0][3];
                saved.y = m[1][3];
                saved.z = m[2][3];
                PSMTXConcat(m, bullet->mtx, m);
                m[0][3] = saved.x;
                m[1][3] = saved.y;
                m[2][3] = saved.z;
                Hu3DModelObjDraw(mdlId, bulletObjTbl[machine->bulletType], m);
            }
        }
        Hu3DModelAttrSet(mdlId, 1);
    }
}
