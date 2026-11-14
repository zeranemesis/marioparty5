#include "game/object.h"
#include "game/mg/timer.h"
#include "REL/sd00dll_sys.h"
#include "game/pad.h"
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/gamework.h"
#include "game/audio.h"
#include <math.h>
#include <string.h>

/* ---- shared (parts A/B/C) types ---- */
typedef void (*SdBulletFunc)(SdBullet *bullet);
typedef void (*SdBulletMtxFunc)(SdBullet *bullet, Mtx mtx);

/* ---- cross-unit externs (owner unit in comment) ---- */
extern s32 MgSeqModeGet(void);                    /* game */

/* ---- part A (this file) static functions ---- */
static void BulletKinokoUpdate(SdBullet *bullet);
static void BulletSuikaUpdate(SdBullet *bullet);
static void BulletBombUpdate(SdBullet *bullet);
static void BulletKillerUpdate(SdBullet *bullet);
static void BulletSanboUpdate(SdBullet *bullet);
static void BulletPakkunUpdate(SdBullet *bullet);
static void BulletKoopaFireUpdate(SdBullet *bullet);
static void BulletAikunUpdate(SdBullet *bullet);
static void BulletBananaUpdate(SdBullet *bullet);
static void BulletLaserUpdate(SdBullet *bullet);
static void BulletHaruUpdate(SdBullet *bullet);
static void BulletKoopaArmUpdate(SdBullet *bullet);
static void BulletBombSmallUpdate(SdBullet *bullet);
static s32 BulletMachineCol(SdBullet *bullet);
static s32 BulletParticleCol(SdBullet *bullet, s32 idx, s32 damage);
static s32 BulletObjHitCheck(SdBullet *bullet);
static s32 BulletParticleObjHitCheck(SdBullet *bullet, s32 idx);
static void BulletParticleObjHitCreate(SdBullet *bullet, s32 idx);
static s32 BulletRabbitHitCheck(SdBullet *bullet);
static s32 BulletParticleRabbitHitCheck(SdBullet *bullet, s32 idx);
static void BulletLaserCheck(SdBullet *bullet);
static HU3DPARTICLEDATA *BulletPartBase(SdBullet *bullet, s32 idx);

/* ---- part B functions (gun_b.inc): signatures are fixed by the tables below ---- */
static void BulletEffectUpdate(SdBullet *bullet);
static void BulletEffectCreate(SdBullet *bullet, Mtx mtx);
static void BulletAirEffectCreate(SdBullet *bullet, Mtx mtx);
static void BulletImpactCreate(SdBullet *bullet, Mtx mtx);
static void KinokoEffectUpdate(SdBullet *bullet);
static void ShootEffectCreate(SdBullet *bullet, Mtx mtx);
static void DustCloudImpactCreate(SdBullet *bullet, Mtx mtx);
static void SuikaEffectUpdate(SdBullet *bullet);
static void SuikaEffectCreate(SdBullet *bullet, Mtx mtx);
static void SuikaImpactCreate(SdBullet *bullet, Mtx mtx);
static void FlameEffectUpdate(SdBullet *bullet);
static void FlameEffectUpdateData(SdBullet *bullet);
static void BomheiAirEffectUpdate(SdBullet *bullet, Mtx mtx);
static void BulletDustImpactCreate(SdBullet *bullet, Mtx mtx);
static void KillerEffectUpdate(SdBullet *bullet);
static void SanboEffectCreate(SdBullet *bullet, Mtx mtx);
static void SanboImpactCreate(SdBullet *bullet, Mtx mtx);
static void PakkunEffectUpdate(SdBullet *bullet);
static void PakkunEffectCreate(SdBullet *bullet, Mtx mtx);
static void PakkunAirEffectUpdate(SdBullet *bullet, Mtx mtx);
static void PakkunImpactCreate(SdBullet *bullet, Mtx mtx);

/* ---- part C functions (gun_c.inc) ---- */
static void BulletExplodeUpdate(SdBullet *bullet);
static void KoopaFireEffectCreate(SdBullet *bullet, Mtx mtx);
static void AikunEffectUpdate(SdBullet *bullet);
static void AikunEffectCreate(SdBullet *bullet, Mtx mtx);
static void AikunImpactCreate(SdBullet *bullet, Mtx mtx);
static void LaserAirEffectUpdate(SdBullet *bullet, Mtx mtx);
static void LaserEffectUpdate(SdBullet *bullet);
static void KoopaArmEffectUpdate(SdBullet *bullet);
static void KoopaArmImpactCreate(SdBullet *bullet, Mtx mtx);
static void StarEffectUpdate(SdBullet *bullet);

/* ---- data (.data), shared by parts A/B/C ---- */
static SdBulletFunc bulletUpdateFuncTbl[13] = {
    BulletKinokoUpdate, BulletSuikaUpdate, BulletBombUpdate, BulletKillerUpdate,
    BulletSanboUpdate, BulletPakkunUpdate, BulletKoopaFireUpdate, BulletAikunUpdate,
    BulletBananaUpdate, BulletLaserUpdate, BulletHaruUpdate, BulletKoopaArmUpdate,
    BulletBombSmallUpdate,
};

static SdBulletMtxFunc effectCreateFuncTbl[13] = {
    ShootEffectCreate, SuikaEffectCreate, ShootEffectCreate, ShootEffectCreate,
    SanboEffectCreate, PakkunEffectCreate, KoopaFireEffectCreate, AikunEffectCreate,
    ShootEffectCreate, ShootEffectCreate, ShootEffectCreate, ShootEffectCreate,
    ShootEffectCreate,
};

static SdBulletMtxFunc airEffectUpdateFuncTbl[13] = {
    NULL, NULL, BomheiAirEffectUpdate, NULL,
    NULL, PakkunAirEffectUpdate, NULL, NULL,
    NULL, LaserAirEffectUpdate, NULL, NULL,
    BomheiAirEffectUpdate,
};

static SdBulletMtxFunc impactCreateFuncTbl[13] = {
    DustCloudImpactCreate, SuikaImpactCreate, BulletDustImpactCreate, BulletDustImpactCreate,
    SanboImpactCreate, PakkunImpactCreate, DustCloudImpactCreate, AikunImpactCreate,
    BulletDustImpactCreate, BulletDustImpactCreate, BulletDustImpactCreate, KoopaArmImpactCreate,
    BulletDustImpactCreate,
};

static SdBulletFunc effectUpdateFuncTbl[13] = {
    KinokoEffectUpdate, NULL, FlameEffectUpdate, FlameEffectUpdate,
    KillerEffectUpdate, NULL, NULL, NULL,
    FlameEffectUpdate, FlameEffectUpdate, FlameEffectUpdate, KoopaArmEffectUpdate,
    FlameEffectUpdate,
};

static SdBulletFunc flameEffectFuncTbl[13] = {
    NULL, NULL, FlameEffectUpdateData, FlameEffectUpdateData,
    NULL, NULL, NULL, NULL,
    FlameEffectUpdateData, FlameEffectUpdateData, FlameEffectUpdateData, NULL,
    FlameEffectUpdateData,
};

static SdBulletFunc bulletExplodeFuncTbl[13] = {
    NULL, NULL, NULL, NULL,
    NULL, NULL, BulletExplodeUpdate, NULL,
    NULL, NULL, NULL, NULL,
    NULL,
};

static SdBulletFunc aikunEffectFuncTbl[13] = {
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, AikunEffectUpdate,
    NULL, NULL, NULL, NULL,
    NULL,
};

static SdBulletFunc pakkunEffectFuncTbl[13] = {
    NULL, NULL, NULL, NULL,
    NULL, PakkunEffectUpdate, NULL, NULL,
    NULL, NULL, NULL, NULL,
    NULL,
};

static SdBulletFunc suikaEffectFuncTbl[13] = {
    NULL, SuikaEffectUpdate, NULL, NULL,
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL,
    NULL,
};

static SdBulletFunc starEffectFuncTbl[13] = {
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, StarEffectUpdate,
    NULL,
};

static SdBulletFunc laserEffectFuncTbl[13] = {
    NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL,
    NULL, LaserEffectUpdate, NULL, NULL,
    NULL,
};

static char *bulletObjTbl[13] = {
    "", "bul02", "", "",
    "bul05", "", "", "",
    "", "", "", "",
    "",
};

/* 1 = fire on button press (down edge), 0 = fire while held */
static const s8 gunPressTbl[13] = { 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1 };

/* ---- part A functions ---- */
void SdGunUpdate(SdMachine *machine)
{
    Mtx m;
    Vec d1, d2, tgt, f;
    SdGun *gun;
    SdBullet *b;
    SdMachine *target;
    s32 i, k, played, flag;
    f32 mag1, mag2, dot1, dot2, mag, yaw1, yaw2;

    target = machine->target;
    played = 0;

    if (gunPressTbl[machine->bulletType] == 0) {
        if ((HuPadBtn[machine->padNo] & 0x200) == 0) {
            return;
        }
    } else if ((HuPadBtnDown[machine->padNo] & 0x200) == 0) {
        return;
    }

    for (i = 0; i < machine->gunNum; i++) {
        gun = &machine->gun[i];
        if (gun->timer > 0) {
            continue;
        }
        for (k = 0; k < machine->gunBulletNum; k++) {
            if (gun->bullet[k].mode != 0) {
                continue;
            }
            b = &gun->bullet[k];
            b->mode = 1;
            b->unk18 = 0;
            b->unk14 = 0;
            b->unk4 = 0;
            b->unk120 = 0.0f;
            b->unk64 = b->param->unk14;
            PSMTXTrans(m, machine->curPos.x, machine->curPos.y, machine->curPos.z);
            PSMTXConcat(m, machine->mtx, m);
            PSMTXConcat(m, gun->mtx, b->mtx);
            {
                Vec t = { 0.0f, 0.0f, 0.0f };
                t.x = b->mtx[0][3];
                t.y = b->mtx[1][3];
                t.z = b->mtx[2][3];
                b->pos = t;
            }
            b->prevPos = b->pos;
            b->startPos = b->prevPos;
            if (sys->gameType != 2) {
                tgt = target->curPos;
                tgt.y += target->hitHeight * 0.5f;
            } else {
                flag = 0;
                SdRabbitPosGet(&tgt);
                PSVECSubtract(&tgt, &machine->curPos, &d1);
                PSVECSubtract(&target->curPos, &machine->curPos, &d2);
                d2.y = 0.0f;
                d1.y = 0.0f;
                mag1 = PSVECMag(&d1);
                mag2 = PSVECMag(&d2);
                if (SdRabbitDoneCheck()) {
                    flag = 1;
                }
                if (!SdRabbitCheck()) {
                    flag = 1;
                } else if (target->mode >= 2 || target->unk210 != 0) {
                    flag = 0;
                } else if (mag1 < 0.5f || mag2 < 0.5f) {
                    flag = 1;
                } else {
                    Vec t = { 0.0f, 0.0f, 0.0f };
                    t.x = machine->mtx[0][2];
                    t.y = machine->mtx[1][2];
                    t.z = machine->mtx[2][2];
                    f = t;
                    PSVECNormalize(&d1, &d1);
                    dot1 = PSVECDotProduct(&d1, &f);
                    PSVECNormalize(&d2, &d2);
                    dot2 = PSVECDotProduct(&d2, &f);
                    if (dot1 < dot2) {
                        flag = 1;
                    }
                }
                if (flag == 0) {
                    SdRabbitPosGet(&tgt);
                    tgt.y += 45.0f;
                } else {
                    tgt = target->curPos;
                    tgt.y += target->hitHeight * 0.5f;
                }
            }
            PSVECSubtract(&tgt, &b->startPos, &d1);
            if (b->param->unk0 == 2) {
                PSVECNormalize(&d1, &d1);
                PSVECScale(&d1, &d1, 150.0f);
                b->unk124 = 22.5f;
            }
            mag = PSVECMag(&d1);
            if (mag < b->param->unk20) {
                Vec t = { 0.0f, 0.0f, 0.0f };
                t.x = d1.x;
                t.y = 0.0f;
                t.z = d1.z;
                d2 = t;
                PSVECNormalize(&d2, &d2);
                PSVECScale(&d2, &d2, b->param->unk20);
                {
                    Vec t2 = { 0.0f, 0.0f, 0.0f };
                    t2.x = d2.x;
                    t2.y = d1.y;
                    t2.z = d2.z;
                    d1 = t2;
                }
            } else if (0.0f == b->param->unk1C && mag > b->param->range) {
                Vec t = { 0.0f, 0.0f, 0.0f };
                t.x = d1.x;
                t.y = 0.0f;
                t.z = d1.z;
                d2 = t;
                PSVECNormalize(&d2, &d2);
                PSVECScale(&d2, &d2, b->param->range);
                {
                    Vec t2 = { 0.0f, 0.0f, 0.0f };
                    t2.x = d2.x;
                    t2.y = d1.y;
                    t2.z = d2.z;
                    d1 = t2;
                }
            }
            b->speed = PSVECMag(&d1);
            yaw1 = HuAtan(b->mtx[0][2], b->mtx[2][2]);
            PSVECNormalize(&d1, &d1);
            yaw2 = HuAtan(d1.x, d1.z);
            if (yaw1 - yaw2 < 7.5f && yaw1 - yaw2 > -7.5f) {
                PSMTXIdentity(m);
            } else {
                PSMTXRotRad(m, 'y', 0.017453292f * (yaw1 - yaw2));
            }
            PSMTXMultVec(m, &d1, &d1);
            MathPlaneRotMtxGet(&d1, NULL, 0.0f, b->mtx);
            MathMtxRotGet(b->mtx, &b->rot);
            PSVECScale(&d1, &d1, b->speed);
            PSVECAdd(&d1, &b->startPos, &b->pos110);
            if (machine->bulletType == 12) {
                b->pos110.y = b->startPos.y;
            }
            b->gun->lastBullet = b;
            b->unkFC = 5.0f * (f32)b->param->unk8;
            if (b->mdlId != -1 && machine->bulletType != 4 && machine->bulletType != 1) {
                Hu3DModelAttrReset(b->mdlId, 1);
            }
            if (b->mdlId2 != -1) {
                Hu3DModelAttrReset(b->mdlId2, 1);
            }
            gun->timer = machine->gunTimerInit;
            if (!played) {
                SdMachineFxGunPlay(b);
                played = 1;
            }
            if (b->speed < 1.0f) {
                b->speed = 1.0f;
            }
            break;
        }
    }
}

void SdBulletUpdate(SdMachine *machine)
{
    s32 i, k;
    SdGun *gun;
    SdBullet *b;

    for (i = 0; i < machine->gunNum; i++) {
        gun = &machine->gun[i];
        if (gun->timer != 0) {
            gun->timer--;
        }
        for (k = 0; k < machine->gunBulletNum; k++) {
            b = &gun->bullet[k];
            bulletUpdateFuncTbl[machine->bulletType](b);
            BulletEffectUpdate(b);
            if (b->mdlId != -1) {
                Hu3DModelPosSetV(b->mdlId, &b->pos);
                Hu3DModelRotSetV(b->mdlId, &b->rot);
            }
            if (b->mdlId2 != -1) {
                Hu3DModelPosSetV(b->mdlId2, &b->pos);
            }
        }
    }
}

static void BulletKinokoUpdate(SdBullet *bullet)
{
    Vec up, a, s, d;
    Vec out;
    Mtx m;
    s32 hit;

    switch (bullet->mode) {
    case 1:
        bullet->prevPos = bullet->pos;
        bullet->unk120 += bullet->unkFC / bullet->speed;
        a = bullet->pos110;
        s = bullet->startPos;
        MathVecInterpolate(&out, &s, &a, 0.15f * bullet->speed, bullet->unk120);
        bullet->pos = out;
        PSVECSubtract(&bullet->pos, &bullet->prevPos, &d);
        {
            Vec t = { 0.0f, 0.0f, 0.0f };
            t.x = bullet->mtx[0][1];
            t.y = bullet->mtx[1][1];
            t.z = bullet->mtx[2][1];
            up = t;
        }
        MathPlaneRotMtxGet(&d, &up, 0.0f, m);
        if (bullet->unk14 == 0) {
            m[0][3] = bullet->pos.x;
            m[1][3] = bullet->pos.y;
            m[2][3] = bullet->pos.z;
            BulletEffectCreate(bullet, m);
            Hu3DModelAttrReset(bullet->machine->gunMdlId, 0x40000080);
            Hu3DMotionShapeTimeSet(bullet->machine->gunMdlId, 0.0f);
            bullet->unk14 = 1;
        }
        MathMtxRotGet(m, &bullet->rot);
        hit = 0;
        if (BulletMachineCol(bullet)) {
            hit = 1;
        } else if (MgSeqModeGet() > 5) {
            hit = 1;
        } else if (BulletRabbitHitCheck(bullet)) {
            hit = 1;
        } else if (SdStageWallCheck(&bullet->pos, &d)) {
            hit = 2;
        } else if (BulletObjHitCheck(bullet)) {
            hit = 2;
        } else if (SdStageFloorCheck(&bullet->pos, &d)) {
            hit = 2;
        }
        if (hit != 0) {
            SdMachineFxImpactPlay(bullet);
            if (hit == 2) {
                MathPlaneRotMtxGet(&d, NULL, 0.0f, m);
            }
            m[0][3] = bullet->pos.x;
            m[1][3] = bullet->pos.y;
            m[2][3] = bullet->pos.z;
            BulletImpactCreate(bullet, m);
            Hu3DModelAttrSet(bullet->mdlId, 1);
            if (bullet->mdlId2 != -1) {
                Hu3DModelAttrSet(bullet->mdlId2, 1);
            }
            bullet->mode = 0;
        }
        break;
    }
}

static void BulletSuikaUpdate(SdBullet *bullet)
{
    Vec up, a, s, d, d2;
    Vec out;
    Mtx m;
    s32 hit;
    f32 mag;

    switch (bullet->mode) {
    case 1: {
        GXColor color = { 0xFF, 0x00, 0x20, 0x80 };
        bullet->gun->color = color;
        bullet->mode++;
    }
        /* fallthrough */
    case 2:
        bullet->prevPos = bullet->pos;
        bullet->unk120 += bullet->unkFC / bullet->speed;
        a = bullet->pos110;
        s = bullet->startPos;
        MathVecInterpolate(&out, &s, &a, 0.0f, bullet->unk120);
        bullet->pos = out;
        PSVECSubtract(&bullet->pos, &bullet->prevPos, &d);
        {
            Vec t = { 0.0f, 0.0f, 0.0f };
            t.x = bullet->mtx[0][1];
            t.y = bullet->mtx[1][1];
            t.z = bullet->mtx[2][1];
            up = t;
        }
        MathPlaneRotMtxGet(&d, &up, 0.0f, m);
        if (bullet->unk14 == 0) {
            if (bullet->machine->bulletType != 4) {
                Hu3DModelAttrReset(bullet->machine->gunMdlId, 0x40000080);
                Hu3DMotionShapeTimeSet(bullet->machine->gunMdlId, 0.0f);
            }
            m[0][3] = bullet->pos.x;
            m[1][3] = bullet->pos.y;
            m[2][3] = bullet->pos.z;
            BulletEffectCreate(bullet, m);
            bullet->unk14 = 1;
        }
        MathMtxRotGet(m, &bullet->rot);
        hit = 0;
        if (BulletMachineCol(bullet)) {
            hit = 1;
        } else if (MgSeqModeGet() > 5) {
            hit = 1;
        } else if (BulletRabbitHitCheck(bullet)) {
            hit = 1;
        } else if (SdStageWallCheck(&bullet->pos, &d)) {
            hit = 2;
        } else if (BulletObjHitCheck(bullet)) {
            hit = 2;
        } else if (SdStageFloorCheck(&bullet->pos, &d)) {
            hit = 2;
        } else {
            PSVECSubtract(&bullet->pos, &bullet->startPos, &d2);
            mag = PSVECMag(&d2);
            if (mag > bullet->param->range) {
                hit = 1;
            }
        }
        if (hit != 0) {
            SdMachineFxImpactPlay(bullet);
            if (hit == 2) {
                MathPlaneRotMtxGet(&d, NULL, 0.0f, m);
            }
            m[0][3] = bullet->pos.x;
            m[1][3] = bullet->pos.y;
            m[2][3] = bullet->pos.z;
            BulletImpactCreate(bullet, m);
            if (bullet->mdlId != -1) {
                Hu3DModelAttrSet(bullet->mdlId, 1);
            }
            if (bullet->mdlId2 != -1) {
                Hu3DModelAttrSet(bullet->mdlId2, 1);
            }
            bullet->mode = 0;
        }
        break;
    }
}

static void BulletBombUpdate(SdBullet *bullet)
{
    Vec up, a, s, d, p;
    Vec out;
    Mtx m;
    s32 hit;

    switch (bullet->mode) {
    case 1:
        BulletLaserCheck(bullet);
        bullet->prevPos = bullet->pos;
        bullet->unk120 += bullet->unkFC / bullet->speed;
        a = bullet->pos110;
        s = bullet->startPos;
        MathVecInterpolate(&out, &s, &a, 2.0f * bullet->unk124, bullet->unk120);
        bullet->pos = out;
        PSVECSubtract(&bullet->pos, &bullet->prevPos, &d);
        {
            Vec t = { 0.0f, 0.0f, 0.0f };
            t.x = bullet->mtx[0][1];
            t.y = bullet->mtx[1][1];
            t.z = bullet->mtx[2][1];
            up = t;
        }
        MathPlaneRotMtxGet(&d, &up, 0.0f, m);
        if (bullet->unk14 == 0) {
            m[0][3] = bullet->pos.x;
            m[1][3] = bullet->pos.y;
            m[2][3] = bullet->pos.z;
            BulletEffectCreate(bullet, m);
            Hu3DModelAttrReset(bullet->machine->gunMdlId, 0x40000002);
            Hu3DMotionTimeSet(bullet->machine->gunMdlId, 0.0f);
            bullet->unk14 = 1;
        }
        MathMtxRotGet(m, &bullet->rot);
        hit = 0;
        {
            Vec n = { 0.0f, 1.0f, 0.0f };
            d = n;
        }
        if (BulletMachineCol(bullet)) {
            hit = 1;
        } else if (MgSeqModeGet() > 5) {
            hit = 1;
        } else if (BulletRabbitHitCheck(bullet)) {
            hit = 1;
        } else if (SdStageWallCheck(&bullet->pos, NULL)) {
            hit = 2;
        } else if (BulletObjHitCheck(bullet)) {
            hit = 2;
        } else if (SdStageFloorCheck(&bullet->pos, &d)) {
            hit = 3;
        }
        if (hit != 0) {
            if (hit != 3) {
                SdMachineFxImpactPlay(bullet);
            } else {
                p = bullet->pos;
                SdImpactSoundStart(0x555, &p, 0.75f);
            }
            MathPlaneRotMtxGet(&d, NULL, 0.0f, m);
            m[0][3] = bullet->pos.x;
            m[1][3] = bullet->pos.y;
            m[2][3] = bullet->pos.z;
            bullet->unk14 = 0;
            if (hit < 3) {
                BulletImpactCreate(bullet, m);
                if (hit == 2) {
                    bullet->unk64 = bullet->param->unk18;
                    PSVECSubtract(&bullet->machine->target->curPos, &bullet->pos, &d);
                    MathPlaneRotMtxGet(&d, NULL, 0.0f, bullet->mtx);
                    BulletMachineCol(bullet);
                    BulletRabbitHitCheck(bullet);
                    BulletObjHitCheck(bullet);
                }
                Hu3DModelAttrSet(bullet->mdlId, 1);
                if (bullet->mdlId2 != -1) {
                    Hu3DModelAttrSet(bullet->mdlId2, 1);
                }
                bullet->mode = 0;
            } else {
                Hu3DMotionSet(bullet->mdlId, bullet->mdlId5);
                Hu3DModelAttrSet(bullet->mdlId, 0x40000001);
                Hu3DMotionTimeSet(bullet->mdlId, 0.0f);
                Hu3DModelAttrReset(bullet->mdlId3, 1);
                Hu3DMotionTimeSet(bullet->mdlId3, 0.0f);
                BulletAirEffectCreate(bullet, m);
                bullet->mode++;
            }
        }
        break;
    case 2:
        if (MgSeqModeGet() == 5) {
            bullet->unk14++;
            if ((f32)bullet->unk14 < bullet->param->unk10) {
                break;
            }
        }
        {
            Vec n = { 0.0f, 1.0f, 0.0f };
            d = n;
        }
        MathPlaneRotMtxGet(&d, NULL, 0.0f, m);
        m[0][3] = bullet->pos.x;
        m[1][3] = bullet->pos.y;
        m[2][3] = bullet->pos.z;
        BulletImpactCreate(bullet, m);
        bullet->unk64 = bullet->param->unk18;
        PSVECSubtract(&bullet->machine->target->curPos, &bullet->pos, &d);
        MathPlaneRotMtxGet(&d, NULL, 0.0f, bullet->mtx);
        SdMachineFxImpactPlay(bullet);
        if (MgSeqModeGet() == 5) {
            BulletMachineCol(bullet);
            BulletRabbitHitCheck(bullet);
            BulletObjHitCheck(bullet);
        }
        Hu3DModelAttrSet(bullet->mdlId, 1);
        Hu3DModelAttrSet(bullet->mdlId2, 1);
        Hu3DModelAttrSet(bullet->mdlId3, 1);
        Hu3DMotionSet(bullet->mdlId, bullet->mdlId4);
        Hu3DMotionTimeSet(bullet->mdlId, 0.0f);
        SdModelGlowSet(bullet->mdlId);
        bullet->mode = 0;
        bullet->unk14 = 0;
        break;
    }
}

static void BulletKillerUpdate(SdBullet *bullet)
{
    Vec fwd, tp, v50, d, up, n;
    Mtx m;
    s32 hit;

    switch (bullet->mode) {
    case 1:
        bullet->prevPos = bullet->pos;
        {
            Vec t = { 0.0f, 0.0f, 0.0f };
            t.x = bullet->mtx[0][2];
            t.y = bullet->mtx[1][2];
            t.z = bullet->mtx[2][2];
            fwd = t;
        }
        PSVECScale(&fwd, &v50, bullet->param->unk1C - 1.0f);
        if (bullet->machine->target->camSingleF == 0) {
            tp = bullet->machine->target->curPos;
            tp.y += bullet->machine->target->hitRadius;
        } else {
            PSVECAdd(&bullet->pos, &fwd, &tp);
        }
        PSVECSubtract(&tp, &bullet->pos, &fwd);
        PSVECNormalize(&fwd, &fwd);
        PSVECAdd(&v50, &fwd, &v50);
        PSVECScale(&v50, &v50, bullet->unkFC / bullet->param->unk1C);
        PSVECAdd(&bullet->prevPos, &v50, &bullet->pos);
        {
            Vec t = { 0.0f, 0.0f, 0.0f };
            t.x = bullet->mtx[0][1];
            t.y = bullet->mtx[1][1];
            t.z = bullet->mtx[2][1];
            up = t;
        }
        MathPlaneRotMtxGet(&v50, &up, 0.0f, bullet->mtx);
        if (bullet->unk14 == 0) {
            Hu3DModelAttrReset(bullet->machine->gunMdlId, 0x40000080);
            Hu3DMotionShapeTimeSet(bullet->machine->gunMdlId, 0.0f);
            bullet->mtx[0][3] = bullet->pos.x;
            bullet->mtx[1][3] = bullet->pos.y;
            bullet->mtx[2][3] = bullet->pos.z;
            BulletEffectCreate(bullet, bullet->mtx);
        }
        if (bullet->machine->bulletType == 9) {
            BulletAirEffectCreate(bullet, bullet->mtx);
        }
        bullet->unk14++;
        MathMtxRotGet(bullet->mtx, &bullet->rot);
        hit = 0;
        if (BulletMachineCol(bullet)) {
            hit = 1;
        } else if (MgSeqModeGet() > 5) {
            hit = 1;
        } else if (BulletRabbitHitCheck(bullet)) {
            hit = 1;
        } else if (SdStageWallCheck(&bullet->pos, NULL)) {
            hit = 2;
        } else if (BulletObjHitCheck(bullet)) {
            hit = 2;
        } else if ((f32)bullet->unk14 >= bullet->param->range) {
            hit = 2;
        } else if (SdStageFloorCheck(&bullet->pos, NULL)) {
            hit = 2;
        }
        if (hit != 0) {
            SdMachineFxImpactPlay(bullet);
            {
                Vec nn = { 0.0f, 1.0f, 0.0f };
                n = nn;
            }
            MathPlaneRotMtxGet(&n, NULL, 0.0f, m);
            m[0][3] = bullet->pos.x;
            m[1][3] = bullet->pos.y;
            m[2][3] = bullet->pos.z;
            BulletImpactCreate(bullet, m);
            if (hit == 2) {
                bullet->unk64 = bullet->param->unk18;
                PSVECSubtract(&bullet->machine->target->curPos, &bullet->pos, &n);
                MathPlaneRotMtxGet(&n, NULL, 0.0f, bullet->mtx);
                BulletMachineCol(bullet);
                BulletRabbitHitCheck(bullet);
                BulletObjHitCheck(bullet);
            }
            Hu3DModelAttrSet(bullet->mdlId, 1);
            if (bullet->mdlId2 != -1) {
                Hu3DModelAttrSet(bullet->mdlId2, 1);
            }
            bullet->mode = 0;
        }
        break;
    }
}

static void BulletSanboUpdate(SdBullet *bullet)
{
    BulletSuikaUpdate(bullet);
}

static void BulletPakkunUpdate(SdBullet *bullet)
{
    Vec up, a, s, d, out;
    Mtx m;
    s32 hit, cnt;

    switch (bullet->mode) {
    case 1:
        BulletLaserCheck(bullet);
        bullet->prevPos = bullet->pos;
        bullet->unk120 += bullet->unkFC / bullet->speed;
        a = bullet->pos110;
        s = bullet->startPos;
        MathVecInterpolate(&out, &s, &a, bullet->unk124, bullet->unk120);
        bullet->pos = out;
        PSVECSubtract(&bullet->pos, &bullet->prevPos, &d);
        {
            Vec t = { 0.0f, 0.0f, 0.0f };
            t.x = bullet->mtx[0][1];
            t.y = bullet->mtx[1][1];
            t.z = bullet->mtx[2][1];
            up = t;
        }
        MathPlaneRotMtxGet(&d, &up, 0.0f, m);
        if (bullet->unk14 == 0) {
            Hu3DModelAttrReset(bullet->machine->gunMdlId, 0x40000080);
            Hu3DMotionShapeTimeSet(bullet->machine->gunMdlId, 0.0f);
            m[0][3] = bullet->pos.x;
            m[1][3] = bullet->pos.y;
            m[2][3] = bullet->pos.z;
            BulletEffectCreate(bullet, m);
        }
        bullet->unk14++;
        if ((bullet->unk14 & 3) == 0) {
            m[0][3] = bullet->pos.x;
            m[1][3] = bullet->pos.y;
            m[2][3] = bullet->pos.z;
            BulletAirEffectCreate(bullet, m);
        }
        MathMtxRotGet(m, &bullet->rot);
        hit = 0;
        {
            Vec n = { 0.0f, 1.0f, 0.0f };
            d = n;
        }
        if (BulletMachineCol(bullet)) {
            hit = 1;
        } else if (MgSeqModeGet() > 5) {
            hit = 1;
        } else if (BulletRabbitHitCheck(bullet)) {
            hit = 1;
        } else if (SdStageFloorCheck(&bullet->pos, &d)) {
            hit = 2;
        } else if (SdStageWallCheck(&bullet->pos, &d)) {
            hit = 2;
            d.y = -0.5f;
        } else if (BulletObjHitCheck(bullet)) {
            hit = 2;
        }
        if (hit == 0) {
            cnt = bullet->machine->stat[3] / 3;
            if (cnt == 0) {
                cnt = 1;
            }
            BulletParticleCol(bullet, 1, cnt);
            BulletParticleRabbitHitCheck(bullet, 1);
            BulletParticleObjHitCheck(bullet, 1);
        } else {
            SdMachineFxImpactPlay(bullet);
            if (bullet->mdlId2 != -1) {
                Hu3DModelAttrSet(bullet->mdlId2, 1);
            }
            MathPlaneRotMtxGet(&d, NULL, 0.0f, m);
            m[0][3] = bullet->pos.x;
            m[1][3] = bullet->pos.y;
            m[2][3] = bullet->pos.z;
            BulletImpactCreate(bullet, m);
            if (hit == 1) {
                bullet->mode = 0;
            } else {
                bullet->unk14 = 0;
                bullet->mode = 2;
            }
        }
        break;
    case 2:
        cnt = bullet->machine->stat[3] / 3;
        if (cnt == 0) {
            cnt = 1;
        }
        if (MgSeqModeGet() == 5) {
            BulletParticleCol(bullet, 1, cnt);
            BulletParticleRabbitHitCheck(bullet, 1);
            BulletParticleObjHitCheck(bullet, 1);
        }
        bullet->unk14++;
        if ((f32)bullet->unk14 >= 60.0f) {
            bullet->mode = 0;
        }
        break;
    }
}

static void BulletKoopaFireUpdate(SdBullet *bullet)
{
    SdMachine *machine;
    SdGun *gun;
    Mtx m, r;
    Vec t;

    switch (bullet->mode) {
    case 1:
    case 2:
        machine = bullet->machine;
        gun = bullet->gun;
        bullet->unk14++;
        if (bullet->unk14 == 1) {
            HuAudFXVolSet(bullet->machine->gunSeId, 0x7F);
            Hu3DModelAttrReset(bullet->machine->gunMdlId, 0x40000002);
            Hu3DMotionTimeSet(bullet->machine->gunMdlId, 0.0f);
        }
        if (bullet->unk14 == 6) {
            Hu3DModelAttrSet(bullet->machine->gunMdlId, 0x40000002);
        }
        if (bullet->unk14 % 2 == 0) {
            bullet->prevPos.y = PSVECMag(&machine->rotVel);
            PSMTXTrans(m, machine->curPos.x, machine->curPos.y, machine->curPos.z);
            PSMTXConcat(m, machine->mtx, m);
            PSMTXConcat(m, gun->mtx, m);
            PSMTXRotRad(r, 'x', 0.34906584f);
            t.x = m[0][3];
            t.y = m[1][3];
            t.z = m[2][3];
            PSMTXConcat(m, r, m);
            m[0][3] = t.x;
            m[1][3] = t.y;
            m[2][3] = t.z;
            BulletEffectCreate(bullet, m);
        }
        BulletParticleObjHitCreate(bullet, 5);
        if (MgSeqModeGet() == 5) {
            BulletParticleCol(bullet, 5, bullet->machine->stat[3]);
            BulletParticleRabbitHitCheck(bullet, 5);
            BulletParticleObjHitCheck(bullet, 5);
        }
        if (bullet->mode == 1) {
            if ((HuPadBtn[bullet->machine->padNo] & 0x200) && MgSeqModeGet() == 5) {
                if (!((f32)bullet->unk14 >= 240.0f)) {
                    break;
                }
            }
            if ((f32)bullet->unk14 >= 240.0f) {
                bullet->gun->timer = 60;
            }
            HuAudFXVolSet(bullet->machine->gunSeId, 0);
            bullet->mode = 2;
        } else if ((f32)bullet->unk14 >= 15.0f) {
            Hu3DModelAttrReset(bullet->machine->gunMdlId, 0x40000002);
            bullet->mode = 0;
        }
        break;
    }
}

static void BulletAikunUpdate(SdBullet *bullet)
{
    Vec up, a, s, d, d2, d3, out, p;
    Mtx m;
    s32 hit;
    f32 mag, sz;
    SdMachine *machine;
    SdGun *gun;
    HU3DPARTICLEDATA *pd;
    HU3DPARTICLE *eff;

    switch (bullet->mode) {
    case 1:
        bullet->prevPos = bullet->pos;
        bullet->unk120 += bullet->unkFC / bullet->speed;
        a = bullet->pos110;
        s = bullet->startPos;
        MathVecInterpolate(&out, &s, &a, 0.0f, bullet->unk120);
        bullet->pos = out;
        PSVECSubtract(&bullet->pos, &bullet->startPos, &d);
        sz = 0.8f * PSVECMag(&d);
        if ((f32)bullet->unk14 < 30.0f) {
            Hu3DModelScaleSet(bullet->mdlId, 0.75f, 0.75f, sz / 40.0f);
        }
        PSVECSubtract(&bullet->pos, &bullet->prevPos, &d);
        {
            Vec t = { 0.0f, 0.0f, 0.0f };
            t.x = bullet->mtx[0][1];
            t.y = bullet->mtx[1][1];
            t.z = bullet->mtx[2][1];
            up = t;
        }
        MathPlaneRotMtxGet(&d, &up, 0.0f, m);
        if (bullet->unk14 == 0) {
            m[0][3] = bullet->pos.x;
            m[1][3] = bullet->pos.y;
            m[2][3] = bullet->pos.z;
            BulletEffectCreate(bullet, m);
            Hu3DModelAttrReset(bullet->machine->gunMdlId, 0x40000080);
            Hu3DMotionShapeTimeSet(bullet->machine->gunMdlId, 0.0f);
            bullet->unk14 = 1;
        }
        MathMtxRotGet(m, &bullet->rot);
        p = bullet->pos;
        hit = 0;
        if (BulletMachineCol(bullet)) {
            hit = 1;
        } else if (MgSeqModeGet() > 5) {
            hit = 1;
        } else if (BulletRabbitHitCheck(bullet)) {
            hit = 1;
        } else if (SdStageWallCheck(&p, NULL)) {
            hit = 1;
        } else if (BulletObjHitCheck(bullet)) {
            hit = 1;
        } else if (SdStageFloorCheck(&p, &d)) {
            hit = 1;
        } else {
            PSVECSubtract(&bullet->pos, &bullet->startPos, &d2);
            mag = PSVECMag(&d2);
            if (mag > bullet->param->range) {
                hit = 1;
            }
        }
        if (hit != 0) {
            SdMachineFxImpactPlay(bullet);
            {
                Vec n = { 0.0f, 1.0f, 0.0f };
                d = n;
            }
            MathPlaneRotMtxGet(&d, NULL, 0.0f, m);
            m[0][3] = bullet->pos.x;
            m[1][3] = bullet->pos.y;
            m[2][3] = bullet->pos.z;
            BulletImpactCreate(bullet, m);
            bullet->mode++;
        }
        break;
    case 2:
        Hu3DData[bullet->mdlId].scale.z -= bullet->unkFC / 40.0f;
        if (Hu3DData[bullet->mdlId].scale.z <= 0.1f) {
            Hu3DData[bullet->mdlId].scale.z = 0.1f;
            Hu3DModelAttrSet(bullet->mdlId, 1);
            eff = (HU3DPARTICLE *)Hu3DData[sys->partMdlId[6]].hookData;
            if (eff->data->time == 0) {
                bullet->mode = 0;
            }
        }
        break;
    }
    if (bullet->mode != 0) {
        pd = BulletPartBase(bullet, 6);
        if (pd->time != 0) {
            machine = bullet->machine;
            gun = bullet->gun;
            PSMTXTrans(m, machine->curPos.x, machine->curPos.y, machine->curPos.z);
            PSMTXConcat(m, machine->mtx, m);
            PSMTXConcat(m, gun->mtx, m);
            {
                Vec t = { 0.0f, 0.0f, 0.0f };
                t.x = m[0][3];
                t.y = m[1][3];
                t.z = m[2][3];
                d3 = t;
            }
            pd[2].pos = d3;
            pd[1].pos = pd[2].pos;
            pd[0].pos = pd[1].pos;
        }
    }
}

static void BulletBananaUpdate(SdBullet *bullet)
{
    Vec up, a, s, d, d2, out;
    Mtx m, r;
    s32 hit;
    f32 mag;

    switch (bullet->mode) {
    case 1:
        bullet->prevPos = bullet->pos;
        bullet->unk120 += bullet->unkFC / bullet->speed;
        a = bullet->pos110;
        s = bullet->startPos;
        MathVecInterpolate(&out, &s, &a, 0.0f, bullet->unk120);
        bullet->pos = out;
        PSVECSubtract(&bullet->pos, &bullet->prevPos, &d);
        {
            Vec t = { 0.0f, 0.0f, 0.0f };
            t.x = bullet->mtx[0][1];
            t.y = bullet->mtx[1][1];
            t.z = bullet->mtx[2][1];
            up = t;
        }
        MathPlaneRotMtxGet(&d, &up, 0.0f, m);
        if (bullet->unk14 == 0) {
            m[0][3] = bullet->pos.x;
            m[1][3] = bullet->pos.y;
            m[2][3] = bullet->pos.z;
            BulletEffectCreate(bullet, m);
            Hu3DModelAttrReset(bullet->machine->gunMdlId, 0x40000080);
            Hu3DMotionShapeTimeSet(bullet->machine->gunMdlId, 0.0f);
        }
        bullet->unk14++;
        PSMTXRotRad(r, 'z', 0.017453292f * (2.5f * (f32)bullet->unk14));
        PSMTXConcat(m, r, m);
        MathMtxRotGet(m, &bullet->rot);
        hit = 0;
        if (BulletMachineCol(bullet)) {
            hit = 1;
        } else if (MgSeqModeGet() > 5) {
            hit = 1;
        } else if (BulletRabbitHitCheck(bullet)) {
            hit = 1;
        } else if (SdStageWallCheck(&bullet->pos, NULL)) {
            hit = 2;
        } else if (BulletObjHitCheck(bullet)) {
            hit = 2;
        } else if (SdStageFloorCheck(&bullet->pos, &d)) {
            hit = 2;
        } else {
            PSVECSubtract(&bullet->pos, &bullet->startPos, &d2);
            mag = PSVECMag(&d2);
            if (mag > bullet->param->range) {
                hit = 2;
            }
        }
        if (hit != 0) {
            SdMachineFxImpactPlay(bullet);
            {
                Vec n = { 0.0f, 1.0f, 0.0f };
                d = n;
            }
            MathPlaneRotMtxGet(&d, NULL, 0.0f, m);
            m[0][3] = bullet->pos.x;
            m[1][3] = bullet->pos.y;
            m[2][3] = bullet->pos.z;
            BulletImpactCreate(bullet, m);
            if (hit == 2) {
                bullet->unk64 = bullet->param->unk18;
                PSVECSubtract(&bullet->machine->target->curPos, &bullet->pos, &d);
                MathPlaneRotMtxGet(&d, NULL, 0.0f, bullet->mtx);
                BulletMachineCol(bullet);
                BulletRabbitHitCheck(bullet);
                BulletObjHitCheck(bullet);
            }
            Hu3DModelAttrSet(bullet->mdlId, 1);
            if (bullet->mdlId2 != -1) {
                Hu3DModelAttrSet(bullet->mdlId2, 1);
            }
            bullet->mode = 0;
        }
        break;
    }
}

static void BulletLaserUpdate(SdBullet *bullet)
{
    BulletKillerUpdate(bullet);
}

static void BulletHaruUpdate(SdBullet *bullet)
{
    Vec up, a, s, d, d2, out;
    Mtx m, r;
    s32 hit;
    f32 mag;

    switch (bullet->mode) {
    case 1:
        bullet->prevPos = bullet->pos;
        bullet->unk120 += bullet->unkFC / bullet->speed;
        a = bullet->pos110;
        s = bullet->startPos;
        MathVecInterpolate(&out, &s, &a, 0.0f, bullet->unk120);
        bullet->pos = out;
        PSVECSubtract(&bullet->pos, &bullet->prevPos, &d);
        {
            Vec t = { 0.0f, 0.0f, 0.0f };
            t.x = bullet->mtx[0][1];
            t.y = bullet->mtx[1][1];
            t.z = bullet->mtx[2][1];
            up = t;
        }
        MathPlaneRotMtxGet(&d, &up, 0.0f, m);
        if (bullet->unk14 == 0) {
            m[0][3] = bullet->pos.x;
            m[1][3] = bullet->pos.y;
            m[2][3] = bullet->pos.z;
            BulletEffectCreate(bullet, m);
        }
        bullet->unk14++;
        PSMTXRotRad(r, 'z', 0.017453292f * (5.0f * (f32)bullet->unk14));
        PSMTXConcat(m, r, m);
        MathMtxRotGet(m, &bullet->rot);
        hit = 0;
        if (BulletMachineCol(bullet)) {
            hit = 1;
        } else if (MgSeqModeGet() > 5) {
            hit = 1;
        } else if (BulletRabbitHitCheck(bullet)) {
            hit = 1;
        } else if ((HuPadBtn[bullet->machine->padNo] & 0x200) == 0) {
            hit = 2;
        } else if (SdStageWallCheck(&bullet->pos, NULL)) {
            hit = 2;
        } else if (BulletObjHitCheck(bullet)) {
            hit = 2;
        } else if (SdStageFloorCheck(&bullet->pos, &d)) {
            hit = 2;
        } else {
            PSVECSubtract(&bullet->pos, &bullet->startPos, &d2);
            mag = PSVECMag(&d2);
            if (mag > bullet->param->range) {
                hit = 2;
            }
        }
        if (hit != 0) {
            SdMachineFxImpactPlay(bullet);
            {
                Vec n = { 0.0f, 1.0f, 0.0f };
                d = n;
            }
            MathPlaneRotMtxGet(&d, NULL, 0.0f, m);
            m[0][3] = bullet->pos.x;
            m[1][3] = bullet->pos.y;
            m[2][3] = bullet->pos.z;
            BulletImpactCreate(bullet, m);
            if (hit == 2) {
                bullet->unk64 = bullet->param->unk18;
                PSVECSubtract(&bullet->machine->target->curPos, &bullet->pos, &d);
                MathPlaneRotMtxGet(&d, NULL, 0.0f, bullet->mtx);
                BulletMachineCol(bullet);
                BulletRabbitHitCheck(bullet);
                BulletObjHitCheck(bullet);
            }
            Hu3DModelAttrSet(bullet->mdlId, 1);
            if (bullet->mdlId2 != -1) {
                Hu3DModelAttrSet(bullet->mdlId2, 1);
            }
            bullet->gun->timer = bullet->machine->gunTimerInit;
            bullet->mode = 0;
        }
        break;
    }
}

static void BulletKoopaArmUpdate(SdBullet *bullet)
{
    SdMachine *machine;
    SdGun *gun;
    Mtx m;
    Vec t;
    f32 time, maxTime;

    switch (bullet->mode) {
    case 1:
        machine = bullet->machine;
        gun = bullet->gun;
        if (bullet->unk14 == 0) {
            bullet->pos.y += 50.0f;
            bullet->mtx[0][3] = bullet->pos.x;
            bullet->mtx[1][3] = bullet->pos.y;
            bullet->mtx[2][3] = bullet->pos.z;
            BulletEffectCreate(bullet, bullet->mtx);
            Hu3DModelAttrReset(bullet->machine->gunMdlId, 0x40000080);
            Hu3DMotionShapeTimeSet(bullet->machine->gunMdlId, 0.0f);
            Hu3DMotionShapeSpeedSet(bullet->machine->gunMdlId, 1.0f);
            bullet->unk14 = 1;
        }
        time = Hu3DData[bullet->machine->gunMdlId].motShapeWork.time;
        maxTime = Hu3DMotionShapeMaxTimeGet(bullet->machine->gunMdlId);
        if (time < 10.0f) {
            PSMTXTrans(m, machine->curPos.x, machine->curPos.y, machine->curPos.z);
            PSMTXConcat(m, machine->mtx, m);
            PSMTXConcat(m, gun->mtx, m);
            {
                Vec v = { 0.0f, 0.0f, 0.0f };
                v.x = 0.0f;
                v.y = 20.0f - 18.0f * time;
                v.z = 184.0f + 51.25f * time;
                t = v;
            }
            PSMTXMultVec(m, &t, &bullet->pos);
            if (BulletObjHitCheck(bullet)
                && Hu3DData[bullet->machine->gunMdlId].motShapeWork.time >= 1.0f) {
                bullet->mtx[0][3] = bullet->pos.x;
                bullet->mtx[1][3] = bullet->pos.y;
                bullet->mtx[2][3] = bullet->pos.z;
                BulletImpactCreate(bullet, bullet->mtx);
                Hu3DMotionShapeSpeedSet(bullet->machine->gunMdlId, -0.15f);
                HuAudFXPlayVolPan(bullet->machine->playerNo + 0x563, 0x7F,
                                  bullet->machine->playerNo * 32 + 0x30);
                bullet->mode = 2;
                break;
            }
            if (MgSeqModeGet() == 5) {
                if (BulletMachineCol(bullet) || BulletRabbitHitCheck(bullet)) {
                    bullet->mtx[0][3] = bullet->pos.x;
                    bullet->mtx[1][3] = bullet->pos.y;
                    bullet->mtx[2][3] = bullet->pos.z;
                    BulletImpactCreate(bullet, bullet->mtx);
                }
            }
        }
        if (time >= maxTime) {
            bullet->mode = 0;
        }
        break;
    case 2:
        if (Hu3DData[bullet->machine->gunMdlId].motShapeWork.time < 0.1f) {
            Hu3DMotionShapeTimeSet(bullet->machine->gunMdlId, 0.0f);
            bullet->mode = 0;
        }
        break;
    }
}

static void BulletBombSmallUpdate(SdBullet *bullet)
{
    Vec up, a, s, d, p, out;
    Mtx m;
    s32 hit;

    switch (bullet->mode) {
    case 1:
        bullet->prevPos = bullet->pos;
        bullet->unk120 += bullet->unkFC / bullet->speed;
        a = bullet->pos110;
        s = bullet->startPos;
        MathVecInterpolate(&out, &s, &a, 25.0f, bullet->unk120);
        bullet->pos = out;
        PSVECSubtract(&bullet->pos, &bullet->prevPos, &d);
        {
            Vec t = { 0.0f, 0.0f, 0.0f };
            t.x = bullet->mtx[0][1];
            t.y = bullet->mtx[1][1];
            t.z = bullet->mtx[2][1];
            up = t;
        }
        MathPlaneRotMtxGet(&d, &up, 0.0f, m);
        if (bullet->unk14 == 0) {
            m[0][3] = bullet->pos.x;
            m[1][3] = bullet->pos.y;
            m[2][3] = bullet->pos.z;
            BulletEffectCreate(bullet, m);
            Hu3DModelAttrReset(bullet->machine->gunMdlId, 0x40000080);
            Hu3DMotionShapeTimeSet(bullet->machine->gunMdlId, 0.0f);
            Hu3DModelAttrReset(bullet->machine->gunMdlId, 0x40000002);
            Hu3DMotionTimeSet(bullet->machine->gunMdlId, 0.0f);
            bullet->unk14 = 1;
        }
        MathMtxRotGet(m, &bullet->rot);
        hit = 0;
        {
            Vec n = { 0.0f, 1.0f, 0.0f };
            d = n;
        }
        if (BulletMachineCol(bullet)) {
            hit = 1;
        } else if (MgSeqModeGet() > 5) {
            hit = 1;
        } else if (BulletRabbitHitCheck(bullet)) {
            hit = 1;
        } else if (SdStageWallCheck(&bullet->pos, NULL)) {
            hit = 2;
        } else if (BulletObjHitCheck(bullet)) {
            hit = 2;
        } else if (SdStageFloorCheck(&bullet->pos, &d)) {
            hit = 3;
        }
        if (hit != 0) {
            if (hit != 3) {
                SdMachineFxImpactPlay(bullet);
            } else {
                p = bullet->pos;
                SdImpactSoundStart(0x555, &p, 0.75f);
            }
            MathPlaneRotMtxGet(&d, NULL, 0.0f, m);
            m[0][3] = bullet->pos.x;
            m[1][3] = bullet->pos.y;
            m[2][3] = bullet->pos.z;
            bullet->unk14 = 0;
            if (hit < 3) {
                BulletImpactCreate(bullet, m);
                if (hit == 2) {
                    bullet->unk64 = bullet->param->unk18;
                    PSVECSubtract(&bullet->machine->target->curPos, &bullet->pos, &d);
                    MathPlaneRotMtxGet(&d, NULL, 0.0f, bullet->mtx);
                    BulletRabbitHitCheck(bullet);
                    BulletMachineCol(bullet);
                    BulletObjHitCheck(bullet);
                }
                Hu3DModelAttrSet(bullet->mdlId, 1);
                if (bullet->mdlId2 != -1) {
                    Hu3DModelAttrSet(bullet->mdlId2, 1);
                }
                bullet->mode = 0;
            } else {
                Hu3DMotionSet(bullet->mdlId, bullet->mdlId5);
                Hu3DModelAttrSet(bullet->mdlId, 0x40000041);
                Hu3DMotionTimeSet(bullet->mdlId, 0.0f);
                Hu3DModelAttrReset(bullet->mdlId3, 1);
                Hu3DMotionTimeSet(bullet->mdlId3, 0.0f);
                BulletAirEffectCreate(bullet, m);
                bullet->mode++;
            }
        }
        break;
    case 2:
        if (MgSeqModeGet() == 5) {
            bullet->unk14++;
            if ((f32)bullet->unk14 < bullet->param->unk10) {
                break;
            }
        }
        SdMachineFxImpactPlay(bullet);
        {
            Vec n = { 0.0f, 1.0f, 0.0f };
            d = n;
        }
        MathPlaneRotMtxGet(&d, NULL, 0.0f, m);
        m[0][3] = bullet->pos.x;
        m[1][3] = bullet->pos.y;
        m[2][3] = bullet->pos.z;
        BulletImpactCreate(bullet, m);
        bullet->unk64 = bullet->param->unk18;
        PSVECSubtract(&bullet->machine->target->curPos, &bullet->pos, &d);
        MathPlaneRotMtxGet(&d, NULL, 0.0f, bullet->mtx);
        if (MgSeqModeGet() == 5) {
            BulletMachineCol(bullet);
            BulletRabbitHitCheck(bullet);
            BulletObjHitCheck(bullet);
        }
        Hu3DModelAttrSet(bullet->mdlId, 1);
        Hu3DModelAttrSet(bullet->mdlId2, 1);
        Hu3DModelAttrSet(bullet->mdlId3, 1);
        Hu3DMotionSet(bullet->mdlId, bullet->mdlId4);
        Hu3DMotionTimeSet(bullet->mdlId, 0.0f);
        SdModelGlowSet(bullet->mdlId);
        bullet->mode = 0;
        bullet->unk14 = 0;
        break;
    }
}

static HU3DPARTICLEDATA *BulletPartBase(SdBullet *bullet, s32 idx)
{
    HU3DPARTICLE *eff = (HU3DPARTICLE *)Hu3DData[sys->partMdlId[idx]].hookData;
    return eff->data + bullet->partIdx[idx];
}

static s32 BulletMachineCol(SdBullet *bullet)
{
    SdMachine *target = bullet->machine->target;
    Vec tp, d, f, t;
    f32 r, mag;

    if (target->mode != 1) {
        return 0;
    }
    if (target->unk210 != 0) {
        return 0;
    }
    tp = target->curPos;
    if (bullet->machine->bulletType != 11) {
        r = bullet->unk64;
    } else {
        r = 80.0f;
    }
    if (tp.y - target->wheelBase > bullet->pos.y + r) {
        return 0;
    }
    if (tp.y + target->hitHeight < bullet->pos.y - r) {
        return 0;
    }
    PSVECSubtract(&tp, &bullet->pos, &d);
    d.y = 0.0f;
    mag = PSVECMag(&d);
    if (mag <= target->hitRadius + r) {
        Vec v = { 0.0f, 0.0f, 0.0f };
        v.x = bullet->mtx[0][2];
        v.y = bullet->mtx[1][2];
        v.z = bullet->mtx[2][2];
        f = v;
        t = f;
        SdMachineDamage(target, &t, bullet->machine->stat[3]);
        return 1;
    }
    return 0;
}

static s32 BulletParticleCol(SdBullet *bullet, s32 idx, s32 damage)
{
    SdMachine *target = bullet->machine->target;
    HU3DPARTICLEDATA *pd = BulletPartBase(bullet, idx);
    Vec tp, d, t;
    s32 i;
    f32 mag;

    if (target->mode != 1) {
        return 0;
    }
    if (target->unk210 != 0) {
        return 0;
    }
    tp = target->curPos;
    tp.y += target->hitRadius;
    for (i = 0; i < bullet->param->partNum[idx]; i++) {
        if (0.0f == pd[i].scale) {
            continue;
        }
        PSVECSubtract(&tp, &pd[i].pos, &d);
        mag = PSVECMag(&d);
        if (mag <= target->hitRadius + 0.5f * pd[i].scale) {
            PSVECSubtract(&target->curPos, &bullet->machine->curPos, &tp);
            t = tp;
            SdMachineDamage(target, &t, damage);
            return 1;
        }
    }
    return 0;
}

static s32 BulletObjHitCheck(SdBullet *bullet)
{
    Vec p = bullet->pos;
    return SdStageObjHitCheck2(&p, bullet->unk64, 1);
}

static s32 BulletParticleObjHitCheck(SdBullet *bullet, s32 idx)
{
    HU3DPARTICLEDATA *pd = BulletPartBase(bullet, idx);
    Vec p;
    s32 i;

    for (i = 0; i < bullet->param->partNum[idx]; i++) {
        if (0.0f == pd[i].scale) {
            continue;
        }
        p = pd[i].pos;
        if (SdStageObjHitCheck2(&p, 0.5f * pd[i].scale, 1)) {
            return 1;
        }
    }
    return 0;
}

static void BulletParticleObjHitCreate(SdBullet *bullet, s32 idx)
{
    HU3DPARTICLEDATA *pd = BulletPartBase(bullet, idx);
    Vec p;
    s32 i;

    for (i = 0; i < bullet->param->partNum[idx]; i++) {
        if (0.0f == pd[i].scale) {
            continue;
        }
        p = pd[i].pos;
        if (SdStageObjHitCheck2(&p, 0.5f * pd[i].scale, 1)) {
            pd[i].parManId = 0;
            pd[i].time = 0;
            pd[i].scale = 0.0f;
        }
    }
}

static s32 BulletRabbitHitCheck(SdBullet *bullet)
{
    Vec rp, p;
    s32 playerNo;

    if (sys->gameType != 2) {
        return 0;
    }
    if (SdRabbitDoneCheck()) {
        return 0;
    }
    if (!SdRabbitCheck()) {
        return 0;
    }
    SdRabbitPosGet(&rp);
    if (bullet->machine->bulletType != 11) {
        if (bullet->pos.y - bullet->unk64 > 90.0f + rp.y) {
            return 0;
        }
        if (bullet->pos.y + bullet->unk64 < rp.y) {
            return 0;
        }
    }
    p = bullet->pos;
    p.y = 0.0f;
    rp.y = 0.0f;
    if (PSVECDistance(&rp, &p) > 100.0f + bullet->unk64) {
        return 0;
    }
    playerNo = bullet->machine->playerNo;
    SdRabbitPlayerCheck(playerNo);
    HuAudFXPlayVolPan(playerNo + 0x565, 0x7F, playerNo * 32 + 0x30);
    HuAudFXPlayVolPan(playerNo + 0x567, 0x7F, playerNo * 32 + 0x30);
    SdPlayerSoundPlay(bullet->machine, 4);
    return 1;
}

static s32 BulletParticleRabbitHitCheck(SdBullet *bullet, s32 idx)
{
    HU3DPARTICLEDATA *pd = BulletPartBase(bullet, idx);
    Vec rp, p;
    s32 i;

    if (sys->gameType != 2) {
        return 0;
    }
    if (SdRabbitDoneCheck()) {
        return 0;
    }
    if (!SdRabbitCheck()) {
        return 0;
    }
    SdRabbitPosGet(&rp);
    for (i = 0; i < bullet->param->partNum[idx]; i++) {
        if (0.0f == pd[i].scale) {
            continue;
        }
        if (bullet->machine->bulletType != 6) {
            if (pd[i].pos.y - 0.5f * pd[i].scale > 90.0f + rp.y) {
                continue;
            }
            if (pd[i].pos.y + 0.5f * pd[i].scale < rp.y) {
                continue;
            }
        }
        p = pd[i].pos;
        p.y = 0.0f;
        rp.y = 0.0f;
        if (PSVECDistance(&rp, &p) > 100.0f + 0.5f * pd[i].scale) {
            continue;
        }
        HuAudFXPlayVolPan(bullet->machine->playerNo + 0x565, 0x7F, bullet->machine->playerNo * 32 + 0x30);
        HuAudFXPlayVolPan(bullet->machine->playerNo + 0x567, 0x7F, bullet->machine->playerNo * 32 + 0x30);
        SdPlayerSoundPlay(bullet->machine, 4);
        SdRabbitPlayerCheck(bullet->machine->playerNo);
        return 1;
    }
    return 0;
}

static void BulletLaserCheck(SdBullet *bullet)
{
    Vec d;
    f32 y, oldSpeed, spd;

    if (bullet->gun->lastBullet != bullet) {
        return;
    }
    if (HuPadBtn[bullet->machine->padNo] & 0x200) {
        PSVECSubtract(&bullet->pos110, &bullet->startPos, &d);
        y = d.y;
        d.y = 0.0f;
        oldSpeed = bullet->speed;
        bullet->speed = PSVECMag(&d);
        bullet->speed = bullet->speed + bullet->param->range / 45.0f;
        if (bullet->speed > bullet->param->range) {
            spd = bullet->param->range;
        } else {
            spd = (bullet->speed < 1.0f) ? 1.0f : bullet->speed;
        }
        bullet->speed = spd;
        bullet->unk120 = bullet->unk120 * (oldSpeed / bullet->speed);
        bullet->unk124 = bullet->speed * (0.15f - 0.05f * (bullet->speed / bullet->param->range));
        PSVECNormalize(&d, &d);
        PSVECScale(&d, &d, bullet->speed);
        d.y = y;
        PSVECAdd(&d, &bullet->startPos, &bullet->pos110);
    } else {
        bullet->gun->lastBullet = NULL;
    }
}

#include "REL/sd00dll/gun_b.inc"

#include "REL/sd00dll/gun_c.inc"
