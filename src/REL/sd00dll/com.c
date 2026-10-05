#include <dolphin/types.h>
#include <dolphin/mtx.h>

/* Declared locally: sd00dll.h covers none of the CPU state below, and
   game/hu3d.h is deliberately not included - it pulls humath.h -> MSL
   math.h, whose `extern inline sqrtf` emits 16 bytes of rodata this unit
   does not have. */
typedef struct SdCom {
    s32 unk00;
    u8 pad04[0xC4];
    Vec refPos;
    u8 padD4[0x10];
    s32 unkE4;
    u8 padE8[0x128];
    s32 unk210;
    u8 pad214[0x74];
    s32 unk288;
    s32 unk28C;
    u8 pad290[0x14];
    struct SdCom *unk2A4;
    u8 pad2A8[0x4];
    Vec unk2AC;
    u8 pad2B8[0x78];
s32 unk330;
    u8 pad334[0x10];
    s32 unk344;
    u8 pad348[0x10];
    f32 unk358;
    u8 pad35C[0x5C];
    s32 unk3B8;
    s32 unk3BC;
    s32 unk3C0;
    s32 unk3C4;
    s32 velX;
    s32 velY;
    s32 velZ;
    s32 mode3D4;
    s32 unk3D8;
} SdCom;

/* `unk64` points at the record the object is measured against: word 0x28 is
   the reach used to rank candidates, word 0 is a flag, and a vector starts
   at 0x4. */
typedef struct SdObjTarget {
    s32 unk00;
    Vec offset4;
    u8 pad10[0x18];
    s32 reach;
} SdObjTarget;

/* 0x70-byte stage object record. */
typedef struct SdObj {
    u8 pad00[0x10];
    s32 unk10;
    s16 unk14;
    u8 pad16[0x12];
    Vec pos;
    f32 unk34;
    u8 pad38[0x24];
    s32 unk5C;
    u8 pad60[0x4];
    SdObjTarget *unk64;
    u8 pad68[0x8];
} SdObj;

typedef struct SdSys {
    u8 pad00[0xB64];
    SdObj *objTbl;
} SdSys;

extern void SdMachineParamSet(SdCom *com, s32 a, s32 b, s32 c, s32 d);
extern SdSys *sys;

/* `unk64` points at a per-object record whose word 0x28 is the reach used to
   rank candidates; only that word is touched here. */
#define SdObjRef SdObjTarget

/* Second view over the block at com + 0x2AC. */
typedef struct SdComStage {
    u8 pad00[0x34];
    s32 timer;
    s32 nextMode;
    u8 pad3C[0x4];
    s32 mode;
} SdComStage;

/* 10-byte player configuration record; word 8 is the "human controlled" flag. */
typedef struct {
    u8 pad00[0x8];
    s16 unk8;
} GwPlayerConfEntry;

extern GwPlayerConfEntry GwPlayerConf[];
extern s32 rand8(void);
static const Vec comUnitVecA = {1.0f, 1.0f, 1.0f};
static const Vec comUnitVecB = {1.0f, 1.0f, 1.0f};

extern f64 cos(f64 x);
extern f64 atan2(f64 y, f64 x);

f32 ComObjDistGet(SdCom *com, Vec *out)
{
    Vec tmp;
    Vec tmp2;
    Vec refPos = com->refPos;
    Vec one[1];
    Vec one2[1];
    SdObj *obj;
    s32 i;
    s32 rest;
    SdComStage *stage = (SdComStage *)&com->unk2AC;
    f32 deg;
    f32 angleCos;
    f32 distLimit;
    f32 dotLimit;
    f32 refDeg;
    f32 cosLimit;

    rest = 7 - stage->mode;

    angleCos = 12.0f * rest;
    if (sys->objTbl == 0) {
        return 0.0f;
    }
    angleCos = (f32)cos(3.141592653589793 * angleCos / 180.0);
    cosLimit = (f32)cos(1.3089969389957472);
    out->y = 0.0f;
    if (PSVECMag(out) == 0.0f) {
        one[0] = comUnitVecA;
        *out = one[0];
    }
    PSVECNormalize(out, out);
    for (i = 0; i < 8; i++) {
        obj = &sys->objTbl[i];
        if (obj->unk14 == -1) {
            continue;
        }
        if (obj->unk5C == 0) {
            continue;
        }
        if (obj->unk10 == 4) {
            continue;
        }
        if (obj->unk10 == 5) {
            continue;
        }
        if (obj->unk10 == 0) {
            distLimit = 150.0f + com->unk358;
            dotLimit = cosLimit;
        } else {
            distLimit = com->unk358 + 100.0f * rest;
            dotLimit = angleCos;
        }
        PSVECAdd(&obj->pos, &obj->unk64->offset4, &tmp);
        PSVECSubtract(&tmp, &refPos, &tmp);
        tmp2 = tmp;
        if (obj->unk64->unk00 == 1) {
            tmp.y = 0.0f;
        }
        deg = PSVECMag(&tmp);
        if (deg > distLimit + obj->unk34) {
            continue;
        }
        if (PSVECMag(&tmp) == 0.0f) {
            one2[0] = comUnitVecB;
            tmp = one2[0];
        }
        PSVECNormalize(&tmp, &tmp);
        if (PSVECDotProduct(out, &tmp) < dotLimit) {
            continue;
        }
        deg = (f32)(180.0 * (atan2(tmp.y, tmp.z) / 3.141592653589793));
        refDeg = (f32)(180.0 * (atan2(out->x, out->z) / 3.141592653589793));
        deg = deg - refDeg;
        if (deg > 180.0f) {
            deg = deg - 360.0f;
        }
        if (deg < -180.0f) {
            deg = 360.0f + deg;
        }
        return deg;
    }
    return -10000.0f;
}

s32 ComNextModeFarGet(SdCom *com)
{
    SdComStage *stage = (SdComStage *)&com->unk2AC;
    SdCom *cur = com->unk2A4;
    s32 rnd;
    s32 otherQuot;
    s32 selfQuot;

    otherQuot = com->unkE4 / cur->mode3D4;
    selfQuot = cur->unkE4 / cur->mode3D4;

    if (GwPlayerConf[cur->unk344].unk8 == 0) {
        if (stage->mode == 5) {
            if (cur->unk330 != 0) {
                return 4;
            }
            if (stage->timer % 180 == 0) {
                rnd = rand8() % 100;
                if (rnd < 15) {
                    return 1;
                }
                if (rnd < 45) {
                    return 0;
                }
                if (rnd < 90) {
                    return 4;
                }
                return 3;
            }
            return stage->nextMode;
        }
        if (stage->mode == 3) {
            if (cur->unk330 != 0) {
                return 4;
            }
            if (stage->timer % 180 == 0) {
                rnd = rand8() % 100;
                if (rnd < 20) {
                    return 4;
                }
                if (rnd < 25) {
                    return 3;
                }
                goto mid;
            }
            if (stage->nextMode == 4) {
                goto retnext;
            }
            if (stage->nextMode == 3) {
                goto retnext;
            }
            goto mid;
        retnext:
            return stage->nextMode;
        }
        if (stage->mode == 2) {
            if (stage->timer % 180 == 0) {
                rnd = rand8() % 100;
                if (rnd < 10) {
                    return 4;
                }
                goto mid;
            }
            if (stage->nextMode != 4) {
                goto mid;
            }
            return stage->nextMode;
        }
        goto mid;
    } else {
        if (stage->nextMode == 4) {
            if (stage->timer % 30 == 0) {
                goto mid;
            }
            return stage->nextMode;
        }
        if (stage->timer % 240 == 0) {
            return 4;
        }
        goto mid;
    }
mid:
    if (otherQuot > selfQuot) {
        return 1;
    }
    if (otherQuot != selfQuot) {
        goto rank;
    }
    if (cur->unk210 != 0) {
        goto rank;
    }
    if (com->unk210 == 0) {
        goto rank;
    }
    if (stage->mode < 3) {
        return 1;
    }
    return 2;
rank:
    if (cur->unk00 >= 2) {
        goto rankA;
    }
    if (cur->unk210 == 0) {
        goto rankB;
    }
rankA:
    if (stage->mode < 3) {
        return 2;
    }
    return 1;
rankB:
    if (com->unk210 == 0) {
        goto bonus;
    }
    if (stage->mode <= 3) {
        return 1;
    }
    return 2;
bonus:
    if (com->unk3C4 == 6 || com->unk3C4 == 11) {
        if (cur->unk3C4 != 6 && cur->unk3C4 != 11 && com->unkE4 > 4) {
            return 7;
        }
    }
    if (com->unk28C > cur->unk28C && com->unk288 < cur->unk288) {
        if (cur->unk3C4 == 12) {
            return 5;
        }
        return 6;
    }
    return 1;
}

s32 ComNextModeNearGet(SdCom *com)
{
    SdComStage *stage = (SdComStage *)&com->unk2AC;
    s32 rnd;
    SdCom *cur = com->unk2A4;

    if (GwPlayerConf[cur->unk344].unk8 == 0) {
        if (stage->timer % 60 == 0) {
            rnd = rand8() % 100;
            if (stage->mode == 5) {
                if (rnd < 80) {
                    return 4;
                } else {
                    return 8;
                }
            }
            if (stage->mode == 3) {
                if (rnd < 20) {
                    return 4;
                } else {
                    return 8;
                }
            }
            if (stage->mode == 2) {
                if (rnd < 10) {
                    return 4;
                } else {
                    return 8;
                }
            } else {
                goto mode8;
            }
        } else {
            if (stage->mode == 1) {
                goto mode8;
            }
            return stage->nextMode;
        }
    } else {
        if (stage->nextMode == 4) {
            if (stage->timer % 30 == 0) {
                goto mode8;
            }
            return stage->nextMode;
        }
        if (stage->timer % 240 == 0) {
            return 4;
        } else {
            goto mode8;
        }
    }
mode8:
    /* The original's CodeWarrior build merged the four `return 8` tails of
       this function into a single block; the gotos reproduce that shape. */
    return 8;
}

void ComStageObjPosGet(Vec *out, SdCom *com)
{
    SdObj *obj;
    Vec *stagePos = &com->unk2AC;
    s32 i;
    SdCom *cur = com->unk2A4;
    SdObj *best = 0;
    Vec fallback = {-10000.0f, 0.0f, 0.0f};
    Vec tmp;
    Vec unused;
    s32 bestReach = 10000;
    f32 bestDist = 10000.0f;
    f32 dist;

    if (cur->mode3D4 == 1) {
        *out = cur->refPos;
    } else {
        if (sys->objTbl != 0) {
            for (i = 0; i < 8; i++) {
                obj = &sys->objTbl[i];
                if (obj->unk14 == -1) {
                    continue;
                }
                if (obj->unk5C == 0) {
                    continue;
                }
                if (obj->unk10 >= 4) {
                    continue;
                }
                if (((SdObjTarget *)obj->unk64)->reach == 0) {
                    continue;
                }
                if (bestReach < ((SdObjTarget *)obj->unk64)->reach) {
                    continue;
                }
                PSVECSubtract(&com->refPos, &obj->pos, &tmp);
                dist = PSVECMag(&tmp);
                if (bestDist < dist) {
                    continue;
                }
                bestDist = dist;
                bestReach = ((SdObjTarget *)obj->unk64)->reach;
                best = obj;
            }
        }
        if (best != 0) {
            *out = best->pos;
        } else {
            *out = fallback;
        }
    }
}

void ComRandomizeMachine(SdCom *com, s32 vx, s32 vy, s32 vz)
{
    SdMachineParamSet(com, com->unk3B8, com->unk3BC, com->unk3C0, com->unk3C4);
    com->velY += vx;
    com->velX += vy;
    com->velZ += vz;
    com->velX = (com->velX > 8) ? 8 : ((com->velX < 1) ? 1 : com->velX);
    com->velY = (com->velY > 8) ? 8 : ((com->velY < 1) ? 1 : com->velY);
    com->velZ = (com->velZ > 8) ? 8 : ((com->velZ < 1) ? 1 : com->velZ);
    com->mode3D4 = (com->mode3D4 > 8) ? 8 : ((com->mode3D4 < 1) ? 1 : com->mode3D4);
    com->unk3D8 = (com->unk3D8 > 8) ? 8 : ((com->unk3D8 < 1) ? 1 : com->unk3D8);
}