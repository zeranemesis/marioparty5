/* gun part B: bullet effect create/update dispatchers and per-bullet particle effects. */
extern s32 rand8(void);

/* part C helpers used by PakkunEffectUpdate */
static void PakkunEffectDataUpdate(HU3DPARTICLEDATA *pd, s32 num);
static void PakkunEffectData2Update(HU3DPARTICLEDATA *pd, s32 num);

static void KinokoEffectDataUpdate(HU3DPARTICLEDATA *pd, s32 num);
static void SuikaEffectDataUpdate(HU3DPARTICLEDATA *pd, s32 num);

static void BulletEffectUpdate(SdBullet *bullet)
{
    s32 type = bullet->machine->bulletType;

    if (effectUpdateFuncTbl[type] != NULL) {
        effectUpdateFuncTbl[type](bullet);
    }
    if (flameEffectFuncTbl[type] != NULL) {
        flameEffectFuncTbl[type](bullet);
    }
    if (bulletExplodeFuncTbl[type] != NULL) {
        bulletExplodeFuncTbl[type](bullet);
    }
    if (aikunEffectFuncTbl[type] != NULL) {
        aikunEffectFuncTbl[type](bullet);
    }
    if (pakkunEffectFuncTbl[type] != NULL) {
        pakkunEffectFuncTbl[type](bullet);
    }
    if (suikaEffectFuncTbl[type] != NULL) {
        suikaEffectFuncTbl[type](bullet);
    }
    if (starEffectFuncTbl[type] != NULL) {
        starEffectFuncTbl[type](bullet);
    }
    if (laserEffectFuncTbl[type] != NULL) {
        laserEffectFuncTbl[type](bullet);
    }
}

static void BulletEffectCreate(SdBullet *bullet, Mtx mtx)
{
    s32 type = bullet->machine->bulletType;

    if (effectCreateFuncTbl[type] != NULL) {
        effectCreateFuncTbl[type](bullet, mtx);
    }
}

static void BulletAirEffectCreate(SdBullet *bullet, Mtx mtx)
{
    s32 type = bullet->machine->bulletType;

    if (airEffectUpdateFuncTbl[type] != NULL) {
        airEffectUpdateFuncTbl[type](bullet, mtx);
    }
}

static void BulletImpactCreate(SdBullet *bullet, Mtx mtx)
{
    s32 type = bullet->machine->bulletType;

    if (impactCreateFuncTbl[type] != NULL) {
        impactCreateFuncTbl[type](bullet, mtx);
    }
}

/* Common particle decay: move, damp velocity, grow, fade; kill the bank after 60 frames. */
static void KinokoEffectDataUpdate(HU3DPARTICLEDATA *pd, s32 num)
{
    s32 i;

    for (i = 0; i < num; i++) {
        pd[i].pos.x += pd[i].vel.x;
        pd[i].pos.y += pd[i].vel.y;
        pd[i].pos.z += pd[i].vel.z;
        PSVECScale(&pd[i].vel, &pd[i].vel, 0.9f);
        pd[i].scale *= 1.04f;
        pd[i].color.a = (u8)((f32)pd[i].color.a * 0.9f);
    }
    if (++pd->parManId >= 60) {
        pd->parManId = 0;
        pd->time = 0;
        for (i = 0; i < num; i++) {
            pd[i].scale = 0.0f;
        }
    }
}

static void SuikaEffectDataUpdate(HU3DPARTICLEDATA *pd, s32 num)
{
    KinokoEffectDataUpdate(pd, num);
}

static void KinokoEffectUpdate(SdBullet *bullet)
{
    HU3DPARTICLEDATA *pd = BulletPartBase(bullet, 3);

    if (pd[0].time != 0) {
        KinokoEffectDataUpdate(pd, 8);
    }
    if (pd[8].time != 0) {
        KinokoEffectDataUpdate(pd + 8, 8);
    }
}

/* Eight dust puffs fanned around the muzzle in the xy plane. */
static void DustRingCreate(HU3DPARTICLEDATA *pd, Mtx mtx)
{
    Mtx rot, copy;
    Vec dir = { 0.0f, 1.0f, 1.5f };
    GXColor color = { 0xDA, 0xCB, 0x9E, 0xFF };
    s32 i;

    PSMTXCopy(mtx, copy);
    copy[0][3] = copy[1][3] = copy[2][3] = 0.0f;
    PSVECNormalize(&dir, &dir);
    pd[0].time = 1;
    pd[0].parManId = 0;
    for (i = 0; i < 8; i++) {
        Vec p = { 0.0f, 0.0f, 0.0f };
        p.x = mtx[0][3];
        p.y = mtx[1][3];
        p.z = mtx[2][3];
        pd[i].pos = p;
        PSMTXRotRad(rot, 'z', 0.017453292f * (45.0f * i));
        PSMTXConcat(copy, rot, rot);
        PSMTXMultVec(rot, &dir, &pd[i].vel);
        PSVECScale(&pd[i].vel, &pd[i].vel, 15.0f);
        pd[i].color = color;
        pd[i].scale = 50.0f;
    }
}

static void ShootEffectCreate(SdBullet *bullet, Mtx mtx)
{
    DustRingCreate(BulletPartBase(bullet, 3), mtx);
}

static void DustCloudImpactCreate(SdBullet *bullet, Mtx mtx)
{
    DustRingCreate(BulletPartBase(bullet, 3) + 8, mtx);
}

static void SuikaEffectUpdate(SdBullet *bullet)
{
    HU3DPARTICLEDATA *pd = BulletPartBase(bullet, 7);

    if (pd[0].time != 0) {
        SuikaEffectDataUpdate(pd, 4);
    }
    if (pd[4].time != 0) {
        SuikaEffectDataUpdate(pd + 4, 4);
    }
}

/* Four spinning particles using the gun colour; rotation phase is random. */
static void SuikaBurstCreate(SdBullet *bullet, HU3DPARTICLEDATA *pd, Mtx mtx)
{
    Mtx rot, copy;
    Vec dir = { 0.0f, 1.0f, 1.5f };
    f32 base, ang;
    s32 i;

    PSMTXCopy(mtx, copy);
    copy[0][3] = copy[1][3] = copy[2][3] = 0.0f;
    PSVECNormalize(&dir, &dir);
    pd[0].time = 1;
    pd[0].parManId = 0;
    base = (f32)(rand8() % 60);
    for (i = 0; i < 4; i++) {
        Vec p = { 0.0f, 0.0f, 0.0f };
        p.x = mtx[0][3];
        p.y = mtx[1][3];
        p.z = mtx[2][3];
        pd[i].pos = p;
        ang = base + 90.0f * i;
        PSMTXRotRad(rot, 'z', 0.017453292f * ang);
        pd[i].zRot = 0.017453292f * -ang;
        PSMTXConcat(copy, rot, rot);
        PSMTXMultVec(rot, &dir, &pd[i].vel);
        PSVECScale(&pd[i].vel, &pd[i].vel, 15.0f);
        pd[i].color = bullet->gun->color;
        pd[i].scale = 40.0f;
    }
}

static void SuikaEffectCreate(SdBullet *bullet, Mtx mtx)
{
    SuikaBurstCreate(bullet, BulletPartBase(bullet, 7), mtx);
}

static void SuikaImpactCreate(SdBullet *bullet, Mtx mtx)
{
    SuikaBurstCreate(bullet, BulletPartBase(bullet, 7) + 4, mtx);
}

static void FlameEffectUpdate(SdBullet *bullet)
{
    HU3DPARTICLEDATA *pd = BulletPartBase(bullet, 3);

    if (pd[0].time != 0) {
        KinokoEffectDataUpdate(pd, 8);
    }
    if (pd[8].time != 0) {
        KinokoEffectDataUpdate(pd + 8, 8);
    }
    if (pd[16].time != 0) {
        SdEffectFlameUpdate(pd + 16, 0x30);
    }
}

static void FlameEffectUpdateData(SdBullet *bullet)
{
    HU3DPARTICLEDATA *pd = BulletPartBase(bullet, 4);

    if (pd[0].time != 0) {
        SdEffectFlameUpdateData(pd, 0x19);
    }
}

static void BomheiAirEffectUpdate(SdBullet *bullet, Mtx mtx)
{
    DustRingCreate(BulletPartBase(bullet, 3) + 8, mtx);
}

static void BulletDustImpactCreate(SdBullet *bullet, Mtx mtx)
{
    HU3DPARTICLEDATA *dust = BulletPartBase(bullet, 3);
    HU3DPARTICLEDATA *flame = BulletPartBase(bullet, 4);
    Vec pos = bullet->pos;

    SdEffectDustCreateData(&pos, dust + 16, flame, bullet->param->unk18 / 300.0f);
}

static void KillerEffectUpdate(SdBullet *bullet)
{
    HU3DPARTICLEDATA *pd = BulletPartBase(bullet, 3);

    if (pd[0].time != 0) {
        KinokoEffectDataUpdate(pd, 1);
    }
    if (pd[1].time != 0) {
        KinokoEffectDataUpdate(pd + 1, 1);
    }
}

/* Single smoke puff flying along the muzzle z axis. */
static void SanboPuffCreate(HU3DPARTICLEDATA *pd, Mtx mtx, f32 scale)
{
    GXColor color = { 0xDA, 0xCB, 0x9E, 0xFF };
    Vec p = { 0.0f, 0.0f, 0.0f };
    Vec v = { 0.0f, 0.0f, 0.0f };

    pd->time = 1;
    pd->parManId = 0;
    p.x = mtx[0][3];
    p.y = mtx[1][3];
    p.z = mtx[2][3];
    pd->pos = p;
    v.x = mtx[0][2];
    v.y = mtx[1][2];
    v.z = mtx[2][2];
    pd->vel = v;
    PSVECScale(&pd->vel, &pd->vel, 15.0f);
    pd->color = color;
    pd->scale = scale;
}

static void SanboEffectCreate(SdBullet *bullet, Mtx mtx)
{
    SanboPuffCreate(BulletPartBase(bullet, 3), mtx, 75.0f);
}

static void SanboImpactCreate(SdBullet *bullet, Mtx mtx)
{
    SanboPuffCreate(BulletPartBase(bullet, 3) + 1, mtx, 50.0f);
}

static void PakkunEffectUpdate(SdBullet *bullet)
{
    HU3DPARTICLEDATA *pd = BulletPartBase(bullet, 1);

    if (pd[0].time != 0) {
        PakkunEffectDataUpdate(pd, 4);
    }
    PakkunEffectData2Update(pd + 4, 12);
    if (pd[16].time != 0) {
        PakkunEffectDataUpdate(pd + 16, 8);
    }
}

/* Random light tint: rgb in 0x80..0xFF, alpha 0xB4. */
static GXColor PakkunColorGet(void)
{
    GXColor color = { 0x00, 0x00, 0x00, 0xB4 };

    color.r = (u8)(rand8() % 0x80 + 0x80);
    color.g = (u8)(rand8() % 0x80 + 0x80);
    color.b = (u8)(rand8() % 0x80 + 0x80);
    return color;
}

static void PakkunEffectCreate(SdBullet *bullet, Mtx mtx)
{
    HU3DPARTICLEDATA *pd = BulletPartBase(bullet, 1);
    Mtx rot, copy;
    Vec dir = { 0.0f, 0.5f, 1.0f };
    s32 i;

    PSMTXCopy(mtx, copy);
    copy[0][3] = copy[1][3] = copy[2][3] = 0.0f;
    PSVECNormalize(&dir, &dir);
    pd[0].time = 1;
    pd[0].parManId = 0;
    for (i = 0; i < 4; i++) {
        Vec p = { 0.0f, 0.0f, 0.0f };
        Vec accel = { 0.0f, -1.0f, 0.0f };
        p.x = mtx[0][3];
        p.y = mtx[1][3];
        p.z = mtx[2][3];
        pd[i].pos = p;
        PSMTXRotRad(rot, 'z', 0.017453292f * (45.0f + 90.0f * i));
        PSMTXConcat(copy, rot, rot);
        PSMTXMultVec(rot, &dir, &pd[i].vel);
        pd[i].accel = accel;
        PSVECScale(&pd[i].vel, &pd[i].vel, 15.0f);
        pd[i].color = PakkunColorGet();
        pd[i].scale = 80.0f;
    }
}

static void PakkunAirEffectUpdate(SdBullet *bullet, Mtx mtx)
{
    HU3DPARTICLEDATA *pd = BulletPartBase(bullet, 1) + 4;
    Mtx rot, copy, m;
    Vec dir = { 0.0f, 1.0f, 0.0f };
    s32 i;

    PSMTXCopy(mtx, copy);
    copy[0][3] = copy[1][3] = copy[2][3] = 0.0f;
    PSVECNormalize(&dir, &dir);
    for (i = 0; i < 12; i++) {
        Vec p = { 0.0f, 0.0f, 0.0f };
        Vec v = { 0.0f, 0.0f, 0.0f };
        Vec accel = { 0.0f, -1.0f, 0.0f };

        if (pd[i].time != 0) {
            continue;
        }
        PSMTXRotRad(rot, 'y', 0.017453292f * (f32)(rand8() % 41 - 20));
        PSMTXConcat(copy, rot, m);
        PSMTXRotRad(rot, 'x', 0.017453292f * (f32)(rand8() % 21 - 10));
        PSMTXConcat(m, rot, m);
        pd[i].time = 1;
        pd[i].parManId = 0;
        p.x = mtx[0][3];
        p.y = mtx[1][3];
        p.z = mtx[2][3];
        pd[i].pos = p;
        v.x = m[0][2];
        v.y = m[1][2];
        v.z = m[2][2];
        pd[i].vel = v;
        PSVECScale(&pd[i].vel, &pd[i].vel, bullet->unkFC);
        pd[i].accel = accel;
        pd[i].color = PakkunColorGet();
        pd[i].scale = 90.0f + (f32)(rand8() % 20);
        pd[i].zRot = (f32)(rand8() % 120);
        break;
    }
}

static void PakkunImpactCreate(SdBullet *bullet, Mtx mtx)
{
    HU3DPARTICLEDATA *pd = BulletPartBase(bullet, 1) + 16;
    Mtx rot, copy;
    Vec dir = { 0.0f, 1.0f, 1.0f };
    s32 i;

    PSMTXCopy(mtx, copy);
    copy[0][3] = copy[1][3] = copy[2][3] = 0.0f;
    PSVECNormalize(&dir, &dir);
    pd[0].time = 1;
    pd[0].parManId = 0;
    for (i = 0; i < 8; i++) {
        Vec p = { 0.0f, 0.0f, 0.0f };
        Vec accel = { 0.0f, -1.5f, 0.0f };
        p.x = mtx[0][3];
        p.y = mtx[1][3];
        p.z = mtx[2][3];
        pd[i].pos = p;
        PSMTXRotRad(rot, 'z', 0.017453292f * (45.0f * i));
        PSMTXConcat(copy, rot, rot);
        PSMTXMultVec(rot, &dir, &pd[i].vel);
        pd[i].accel = accel;
        PSVECScale(&pd[i].vel, &pd[i].vel, 15.0f);
        pd[i].color = PakkunColorGet();
        pd[i].scale = 80.0f;
    }
}
