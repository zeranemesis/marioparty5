#include "REL/sd00dll.h"
#include <dolphin/mtx.h>

/* Forward declarations for bullet update functions */
void BulletSuikaUpdate(void *);
void BulletKillerUpdate(void *);
s32 SdStageObjHitCheck2(Vec *, s32);
void KoopaFireEffectDataUpdate(void *, s32);
void LaserEffectDataUpdate(void *, s32);

/* Bullet state structure (partial, based on accessed offsets) */
typedef struct {
    s32 x;
    s32 y;
    s32 z;
} VecInt;

typedef struct {
    u8 pad00[0x3C4];
    s32 modelType;
} BulletModel;

typedef struct {
    u8 pad00[0x48];
    s32 unk48;
    u8 pad4C[0x10];  /* to 0x5C */
    s32 unk5C;
    u8 pad60[0xC];   /* to 0x6C */
    void *unk68;
    u8 pad6C[0x60];  /* to 0xCC */
    VecInt pos;        /* at 0xCC */
    VecInt prevPos;    /* at 0xD8 */
    u8 padE4[0x44];    /* to 0x128 */
    BulletModel *model;     /* at 0x128 */
} Bullet;

/* External globals */
extern void *sys;
extern void *Hu3DData;
extern void (*effectCreateFuncTbl[])(Bullet *, Vec *);

/* 0x16A80 - 40 bytes */
void BulletSanboUpdate(Bullet *bullet)
{
    BulletSuikaUpdate(bullet);
}

/* 0x17CB4 - 40 bytes */
void BulletLaserUpdate(Bullet *bullet)
{
    BulletKillerUpdate(bullet);
}

/* 0x18E68 - 92 bytes */
s32 BulletObjHitCheck(Bullet *bullet)
{
    Bullet *b = bullet;
    s32 pos[3];
    s32 ret;

    pos[0] = b->pos.x;
    pos[1] = b->pos.y;
    pos[2] = b->pos.z;

    ret = SdStageObjHitCheck2((Vec *)pos, 1);
    return ret;
}

/* 0x19944 - 124 bytes */
void BulletEffectCreate(Bullet *bullet, Vec *arg1)
{
    s32 modelType;
    void (*func)(Bullet *, Vec *);

    modelType = bullet->model->modelType;
    func = effectCreateFuncTbl[modelType];
    if (func != NULL) {
        func(bullet, arg1);
    }
}

/* 0x1C62C - 120 bytes */
void BulletExplodeUpdate(Bullet *bullet)
{
    s32 idx = (*(s16 **)sys)[0x1A6 / 2];
    void *data = (u8 *)Hu3DData + idx * 0x134;
    void *effectData = *(void **)((u8 *)data + 0x120);
    void *target = (u8 *)effectData + *(s32 *)((u8 *)effectData + 0x48) + bullet->unk48 * 0x44;

    KoopaFireEffectDataUpdate(target, 0x1E);
}

/* 0x1D524 - 120 bytes */
void LaserEffectUpdate(Bullet *bullet)
{
    s32 idx = (*(s16 **)sys)[0x1B0 / 2];
    void *data = (u8 *)Hu3DData + idx * 0x134;
    void *effectData = *(void **)((u8 *)data + 0x120);
    void *target = (u8 *)effectData + *(s32 *)((u8 *)effectData + 0x48) + bullet->unk5C * 0x44;

    LaserEffectDataUpdate(target, 0x14);
}