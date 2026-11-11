/* This unit treats PSVECMag's result as a double (fcmpo against a rodata
   double), so hide the SDK's f32 prototype before it is pulled in. */
#define PSVECMag PSVECMag__f32
#include "game/hu3d.h"
#include "game/memory.h"
#include "game/pad.h"
#include "math.h"
#include "string.h"
#include "game/gamework.h"
#include "game/object.h"
#include "REL/m516Dll/m516types.h"
#undef PSVECMag

void fn_1_2624(OMOBJ *obj);
void fn_1_28E0(OMOBJ *obj);
void fn_1_3BBC(OMOBJ *obj);
void fn_1_2C68(OMOBJ *obj);
void fn_1_3110(OMOBJ *obj);
void fn_1_2F08(OMOBJ *obj);
void fn_1_2E7C(OMOBJ *obj);
void fn_1_18E0(s32 player);
s32 fn_1_1868(void);
s32 fn_1_18B0(s32 player);
void fn_1_1A20(s32 player);
void fn_1_1910(s32 player);
f32 fn_1_1D34(f32 start, f32 end, f32 t);
s32 fn_1_46BC(OMOBJ *obj);
void Hu3DMotionSpeedSet(s16 modelId, f32 speed);
void fn_1_3734(OMOBJ *obj);
s32 fn_1_89EC(Vec *pos, f32 radius);
void omVibrate(s16 player, s16 kind, s16 strength, s16 duration);
void CharModelLandDustCreate(s16 charNo, void *pos);
void fn_1_3D40(OMOBJ *obj);
s32 fn_1_1850(void);
s32 fn_1_1940(void);
s32 fn_1_6DEC(s32 idx, Vec *pos, f32 *rotY);
void PSVECSubtract(const Vec *a, const Vec *b, Vec *out);
void PSVECAdd(const Vec *a, const Vec *b, Vec *out);
f64 PSVECMag(const Vec *v);
void fn_1_4558(OMOBJ *obj, u32 arg);
void fn_1_19E0(s32 player, s32 value1, s32 value2);
void CharModelVoicePanSet(s16 charNo, s16 pan);
void CharModelKill(s16 charNo);
HU3DMODELID CharModelCreate(s16 charNo, s16 model);
HU3DMOTID CharMotionCreate(s16 charNo, unsigned int dataNum, s32 mode);
void CharMotionDataClose(s16 charNo);
void CharEffectLayerSet(s16 layerNo);
float CharModelHeightGet(s16 charNo);

unsigned int lbl_1_data_98[8] = {
    0x008C0000, 0x008C0001, 0x008C0002, 0x008C0006,
    0x008C0031, 0x008C0003, 0x008C0004, 0x008C0016
};

typedef struct {
    s32 idx;
    s32 grp;
    s32 f08;
    s32 charNo;
    s32 motion;
    u32 f14;
    u32 f18;
    f32 f1C;
    f32 f20;
    s32 type;
    u32 dif;
    f32 f2C;
    f32 f30;
    s32 f34;
    s32 f38;
    s32 f3C;
    u32 state;
    u32 timer;
    u32 f48;
    Vec pos;
    f32 f58;
    f32 f5C;
    f32 f60;
    Vec pos64;
    f32 rotY;
    f32 f74;
    f32 f78;
    u32 f7C;
    u32 f80;
    s32 f84;
    s32 f88;
    s32 f8C;
    s32 f90;
    f32 f94;
    f32 f98;
    ACTHIT hit; /* radius = hit.f24, height = hit.f20, standing flag = hit.f28, other = hit.f30 */
    EFFRING ring;
} PLAYERWORK;

typedef struct {
    u16 motion;
    f32 speed;
    u32 attr;
} PLAYERMOTDEF;

s32 fn_1_1AF8(Vec *pos, s32 useScreen);
s32 fn_1_4650(OMOBJ *obj);

OMOBJ *lbl_1_bss_28[4];

void fn_1_254C(OMOBJMAN *objman)
{
    s32 i;
    OMOBJ *obj;

    omMakeGroupEx(objman, 0, 4);

    for (i = 0; i < 4; i++) {
        obj = omAddObjEx(objman, 0x1E, 1, 8, 0, fn_1_2624);
        lbl_1_bss_28[i] = obj;
        obj->work[0] = i;
        obj->work[1] = GwPlayerConf[i].grp;
        obj->work[2] = i;
    }
}

void fn_1_2600(void)
{
    CharModelKill(-1);
}

void fn_1_2624(OMOBJ *obj)
{
    PLAYERWORK *work;
    ACTHIT *anim;
    s32 idx;
    s32 i;
    s32 charNo;
    s32 modelId;
    s32 motionId;

    obj->data = HuMemDirectMallocNum(HEAP_HEAP, sizeof(PLAYERWORK), HU_MEMNUM_OVL);
    work = (PLAYERWORK *)obj->data;
    memset(work, 0, sizeof(PLAYERWORK));

    obj->stat |= 0x100;

    idx = obj->work[0];
    work->idx = idx;
    work->grp = obj->work[1];
    work->f08 = obj->work[2];

    charNo = GwPlayerConf[idx].charNo;
    work->charNo = charNo;

    fn_1_19E0(idx, work->grp, work->charNo);

    work->dif = (u32)GwPlayerConf[idx].dif << 30 >> 30;
    work->type = GwPlayerConf[idx].type;

    work->f34 = 0;
    work->f38 = 0;
    work->f3C = 0;

    modelId = CharModelCreate(charNo, 2);
    obj->mdlId[0] = modelId;
    Hu3DModelLayerSet(modelId, 5);
    CharEffectLayerSet(6);

    for (i = 0; i < 8; i++) {
        motionId = CharMotionCreate(charNo, lbl_1_data_98[i], 2);
        obj->mtnId[i] = motionId;
    }

    CharMotionDataClose(charNo);

    work->motion = -1;

    fn_1_4558(obj, 0);

    work->pos.z = 0.0f;
    obj->trans.z = 0.0f;
    work->pos.y = 0.0f;
    obj->trans.y = 0.0f;
    work->pos.x = 0.0f;
    obj->trans.x = 0.0f;

    work->f60 = 0.0f;
    work->f5C = 0.0f;
    work->f58 = 0.0f;

    work->rotY = 0.0f;
    work->f88 = 0;
    work->f8C = 0;
    work->f94 = 1.0f;
    work->f98 = 1.0f;
    work->f90 = 0;

    anim = &work->hit;
    anim->flags = 0;
    anim->kind = 0;
    anim->id = idx;
    anim->disabled = 0;
    anim->arg = obj;
    anim->callback = (void (*)(void *))fn_1_3BBC;
    anim->pos = &work->pos;
    anim->vel = (Vec *)&work->f58;

    work->f74 = (f32)(0.8 * CharModelHeightGet(charNo));
    anim->f20 = work->f74;
    anim->f24 = 40.0f;

    fn_1_8D88(anim);

    work->ring.active = 1;
    work->ring.size = 70.0f;
    work->ring.pos = &work->pos64;
    fn_1_796C(&work->ring);

    work->state = 0;
    obj->objFunc = fn_1_28E0;
}

void fn_1_28E0(OMOBJ *obj)
{
    PLAYERWORK *work = (PLAYERWORK *)obj->data;
    Vec save;
    s32 padNo;
    double stickX;
    double stickY;

    work->f14 = 0;
    work->f18 = 0;
    work->f1C = 0.0f;
    work->f20 = 0.0f;

    switch (work->state) {
    case 0:
        save = *&work->pos;

        if (fn_1_6DEC(work->idx, &work->pos,
                      &work->rotY) != 0) {
            PSVECSubtract(&work->pos, &save,
                          (Vec *)&work->f58);

            if ((s32)work->f48 == 0 ||
                PSVECMag((Vec *)&work->f58) < 1.0) {
                fn_1_4558(obj, 0);
            } else {
                fn_1_4558(obj, 2);
            }

            fn_1_2E7C(obj);
            return;
        }

        work->hit.flags |= 1;
        work->state++;
    case 1:
        if ((u32)fn_1_1850() != 2) {
            break;
        }

        work->state++;
        /* fall through */
    case 2:
        if ((u32)fn_1_1850() != 2) {
            work->state++;
            work->timer = 0;
            work->f7C = 1;
            break;
        }

        if ((s32)work->type != 0) {
            fn_1_3D40(obj);
            break;
        }

        padNo = GwPlayerConf[work->idx].padNo;

        // NONMATCHING: the original sets the OE bit (fsubs) on the int->double
        // conversions below; this compiler emits fsub for the same expression.
        stickX = (double)HuPadStkX[padNo];
        stickY = (double)(-HuPadStkY[padNo]);

        work->f18 = HuPadBtn[padNo];
        work->f14 = HuPadBtnDown[padNo];

        work->f1C =
            (f32)(180.0 * (atan2(stickX, stickY) / 3.141592653589793));
        work->f20 =
            (f32)(0.01785714365541935 * fabs(stickX));

        if (work->f20 > 1.0f) {
            work->f20 = 1.0f;
        }
        break;
    case 3:
        if ((s32)++work->timer < 0x14) {
            break;
        }

        work->state++;
        fn_1_4558(obj, 0);
        /* fall through */
    case 4:
        if ((u32)fn_1_1850() == 3 && fn_1_1940() != 0) {
            work->state = 0;
            obj->objFunc = fn_1_2C68;
        }
        break;
    }

    fn_1_3110(obj);
    fn_1_2F08(obj);
    PSVECAdd(&work->pos, (Vec *)&work->f58,
             &work->pos);
    fn_1_2E7C(obj);
}

void fn_1_2C68(OMOBJ *obj)
{
    PLAYERWORK *work = (PLAYERWORK *)obj->data;

    work->f14 = 0;
    work->f18 = 0;
    work->f1C = 0.0f;
    work->f20 = 0.0f;
    work->f58 =
        work->f5C =
        work->f60 = 0.0f;

    switch (work->state) {
    case 0:
        fn_1_18E0(work->idx);
        work->f7C = 0;
        work->f80 = 0;
        work->state++;
        /* fall through */
    case 1:
        fn_1_2F08(obj);

        if ((u32)fn_1_1868() == 0) {
            break;
        }

        work->state++;
        work->timer = 0;
        break;
    case 2:
        fn_1_2F08(obj);
        work->rotY =
            fn_1_1D34(work->rotY, 0.0f, 0.1f);

        if ((s32)++work->timer < 0xA) {
            break;
        }

        work->state++;
        work->timer = 0;
        break;
    case 3:
        fn_1_2F08(obj);
        work->rotY =
            fn_1_1D34(work->rotY, 0.0f, 0.1f);

        if ((s32)++work->timer < 0x14) {
            break;
        }

        if (fn_1_18B0(work->idx) >= 0) {
            fn_1_4558(obj, 3);
        } else {
            fn_1_4558(obj, 4);
            fn_1_1A20(work->idx);
        }

        work->state++;
        break;
    case 5:
        if (fn_1_4650(obj) != 0) {
            fn_1_1910(work->idx);
        }
        break;
    }

    fn_1_3110(obj);
    PSVECAdd(&work->pos, (Vec *)&work->f58,
             &work->pos);
    fn_1_2E7C(obj);
}

void fn_1_2E7C(OMOBJ *obj)
{
    PLAYERWORK *work = (PLAYERWORK *)obj->data;
    s32 pan;

    work->f48++;

    pan = fn_1_1AF8(&work->pos, 1);
    CharModelVoicePanSet((s16)work->charNo, (s16)pan);

    if ((s32)work->f80 != 0 && fn_1_4650(obj) != 0) {
        work->f80 = 0;
    }
}

void fn_1_2F08(OMOBJ *obj)
{
    PLAYERWORK *work = (PLAYERWORK *)obj->data;
    s32 n2;
    s32 flag;
    s32 n0;
    s32 n1;
    f32 speed;
    f32 limit;
    f32 sp;

    speed = work->f20 * work->f94;
    n0 = 0;
    n1 = 1;
    n2 = 2;

    if ((s32)work->f84 != 0) {
        work->rotY =
            fn_1_1D34(work->rotY,
                      work->f1C, 0.3f);
        return;
    }

    flag = (s32)work->f80;

    if ((s32)work->motion == 6) {
        flag = 0;
    }

    if (speed > 0.05f && (s32)work->f7C == 0 && flag == 0) {
        work->f80 = 0;

        work->rotY =
            fn_1_1D34(work->rotY,
                      work->f1C, 0.3f);

        work->f58 =
            (f32)(0.01666666753590107 *
                  (550.0 *
                   (speed *
                    sin(3.141592653589793 * work->f1C /
                        180.0))));

        if (fn_1_46BC(obj) != 0) {
            limit = 0.5f;

            if ((s32)work->motion == n2) {
                limit = 0.45f;
            }

            if (speed > limit) {
                fn_1_4558(obj, n2);
                sp = speed;
            } else {
                fn_1_4558(obj, n1);
                sp = speed / limit;
            }

            Hu3DMotionSpeedSet(obj->mdlId[0], sp);
        }
    } else {
        if ((s32)work->f80 == 0) {
            fn_1_4558(obj, n0);
        }

        work->f58 = 0.0f;
    }

    work->f58 =
        work->f58 + work->f78;
}

// NONMATCHING: register/stack allocation still differs (two extra FPRs
// spilled for the fabs and fmod temporaries).
void fn_1_3110(OMOBJ *obj)
{
    PLAYERWORK *work = (PLAYERWORK *)obj->data;
    ACTHITEX *tile;
    s32 idx;
    ACTHIT *tileIdx;
    f64 absSpeed;
    f32 wrap;
    f32 speed;
    f32 ang;
    f32 t;
    f32 t2;
    Vec v;

    fn_1_3734(obj);

    work->f78 = 0.0f;

    speed = (f32)(0.01666666753590107 *
                  (550.0 * (work->f20 *
                           sin(3.141592653589793 *
                                   work->f1C /
                               180.0))));

    speed = speed * work->f94;

    if ((s32)work->f84 == 0) {
        if (0.0f != work->f94 &&
            (work->f14 & 0x100) != 0) {
            work->f58 = speed;
            work->f5C =
                (f32)(26.66666805744171 * work->f94);
            work->f84 = 1;
            fn_1_4558(obj, 5);
            return;
        }

        v.x = work->pos.x;
        v.y = work->pos.y - 10.0;
        v.z = work->pos.z;

        idx = fn_1_89EC(&v, work->hit.f24);

        if (idx >= 0) {
            tileIdx = fn_1_8E78(idx);
            tile = (ACTHITEX *)tileIdx;

            if (tile->base.vel != NULL &&
                tile->base.vel->y > 0.0f) {
                if (work->pos.x >
                    tile->base.pos->x) {
                    work->f58 += 5.000000260770321;
                } else {
                    work->f58 -= 5.000000260770321;
                }

                work->f5C += 16.66666753590107;
                work->f84 = 1;
                fn_1_4558(obj, 5);
                omVibrate((s16)work->idx, 0xC, 6, 6);
            }
        }

        if (idx < 0) {
            absSpeed = fabs(speed);

            if (absSpeed > 5.000000260770321) {
                ang = work->f1C;
                t = (f32)fmod(ang, 360.0);
                t2 = t;
                wrap = t2;

                if (wrap < 0.0f) {
                    wrap += 360.0f;
                }

                if (wrap > 45.0 && wrap < 135.0) {
                    v.x += 120.0;
                }

                if (wrap > 225.0 && wrap < 315.0) {
                    v.x -= 120.0;
                }

                idx = fn_1_89EC(&v, work->hit.f24);
            }

            work->f84 = 1;

            if (0.0f != work->f94) {
                fn_1_4558(obj, 5);
            }
        }

        if (idx >= 0) {
            work->pos.y = v.y;
            tileIdx = fn_1_8E78(idx);
            tile = (ACTHITEX *)tileIdx;

            if (tile->base.vel != NULL) {
                work->f78 =
                    tile->base.vel->x;
            }
        }
    } else {
        work->f58 =
            work->f58 + 0.05 * speed;
        work->f58 =
            work->f58 * 0.98;

        if ((work->f18 & 0x100) != 0 &&
            work->f5C >= 0.0f) {
            work->f5C =
                work->f5C - 0.9166667144745589;
        } else {
            work->f5C =
                work->f5C - 1.6333334185183048;
        }

        v.x = work->pos.x;
        v.y = work->pos.y - 10.0;
        v.z = work->pos.z;

        idx = fn_1_89EC(&v, work->hit.f24);

        if (work->f5C < 0.0f && idx >= 0) {
            work->pos.y = v.y;
            work->f5C = 0.0f;
            work->f58 = 0.0f;
            work->f84 = 0;

            CharModelLandDustCreate((s16)work->charNo,
                                   &work->pos);

            if (0.0f != work->f94) {
                fn_1_4558(obj, 6);
                work->f80 = 1;
            }
        }

        v.x = work->pos.x;
        v.y = 10.0 + (work->pos.y +
                      work->hit.f20);
        v.z = work->pos.z;

        idx = fn_1_89EC(&v, work->hit.f24);

        if (work->f5C > 0.0f && idx >= 0) {
            tileIdx = fn_1_8E78(idx);
            tile = (ACTHITEX *)tileIdx;

            if (tile->f38 == 0) {
                tile->f38 = 1;
                tile->hitObj = obj;
                tile->f34 = work->idx;
            }

            if (work->f5C > 0.0f) {
                work->f5C = 0.0f;
            }
        }
    }
}

PLAYERMOTDEF lbl_1_data_B8[8] = {
    { 0, 0.2f, 0x40000001 },
    { 1, 0.2f, 0x40000001 },
    { 2, 0.2f, 0x40000001 },
    { 3, 0.2f, 0 },
    { 4, 0.2f, 0 },
    { 5, 0.2f, 0 },
    { 6, 0.2f, 0 },
    { 7, 0.2f, 0x40000001 },
};
f32 lbl_1_data_118[4] = { 0.0f, -0.02f, 0.02f, 0.05f };
s32 lbl_1_data_128[4] = { 0x1B, 0x12, 0xC, 6 };

extern u32 frand(void);
extern f32 fn_1_6E94(f64 x);
extern f32 fn_1_1E50(f32 start, f32 end);
extern void fn_1_8BF0(void *a, void *b);
extern s32 CharFXPlayPan(s16 charNo, s16 seId, s16 pan);
extern s32 CharMotionEndCheck(s16 charNo);
extern s16 CharMotionShiftIDGet(s16 charNo);
extern void CharMotionShiftSet(s16 charNo, s16 motId, f32 start, f32 end, u32 attr);
s32 fn_1_4878(OMOBJ *obj, Vec *pos, f32 *rotY);
ACTHIT *fn_1_4298(OMOBJ *obj, f32 dx, f32 dy);
ACTHIT *fn_1_42E8(OMOBJ *obj, f32 dx, f32 dy, f32 radius);
void fn_1_43B8(OMOBJ *obj, f32 dir, f32 speed);

// Remaining difference: float temporaries in switch cases 2 and 4.
void fn_1_3734(OMOBJ *obj)
{
    PLAYERWORK *work = (PLAYERWORK *)obj->data;
    Vec v;
    f32 sx = 1.0f;
    f32 sy = 1.0f;

    if (work->hit.f28.i != 0) {
        fn_1_4878((OMOBJ *)work->hit.f30.other, &v, NULL);
        if (work->pos.x > v.x) {
            work->f58 += 5.000000260770321;
        } else {
            work->f58 -= 5.000000260770321;
        }
        work->f5C += 16.66666753590107;
        work->f84 = 1;
        fn_1_4558(obj, 5);
        omVibrate((s16)work->idx, 0xC, 6, 6);
    }
    if (work->hit.f2C.i != 0) {
        if (work->f88 == 0 && work->f90 == 0) {
            if (work->f5C <= 0.0f) {
                work->f88 = 1;
                CharFXPlayPan((s16)work->charNo, 0x202, fn_1_1AF8(&work->pos, 1));
            }
            omVibrate((s16)work->idx, 0xC, 6, 6);
        }
        work->f58 *= 0.5;
    }
    switch (work->f88) {
    case 0:
        break;
    case 1:
        work->f88++;
        work->f8C = 0;
    case 2:
        sx = 0.5f;
        sy = (f32)(1.0 - (0.5 * (f32)(0.1282051282051282 * (f64)work->f8C)));
        if (!((f32)(f64)(++work->f8C) < 7.800000000000001)) {
            work->f88++;
            work->f8C = 0;
        }
        break;
    case 3:
        sx = 0.5f;
        sy = 0.5f;
        if (!((f32)(f64)(++work->f8C) < 90.0)) {
            work->f88++;
            work->f8C = 0;
        }
        break;
    case 4:
        sx = 0.5f;
        sy = (f32)(0.5 + (0.5 * (f32)(0.5 * (1.0 + -cos(6.283185307179586 * (1.5 * (f32)(0.050505050505050504 * (f64)work->f8C)))))));
        if (!((f32)(f64)(++work->f8C) < 19.8)) {
            work->f88 = 0;
            work->f90 = 0x5A;
            sy = 1.0f;
        }
        break;
    }
    work->f94 = work->f94 + 0.2 * (sx - work->f94);
    work->f98 = sy;
    work->hit.f20 = work->f74 * sy;
}

void fn_1_3BBC(OMOBJ *obj)
{
    PLAYERWORK *work = (PLAYERWORK *)obj->data;
    s16 mdl = obj->mdlId[0];
    f32 scale;

    if (work->pos.x < -1100.0) {
        work->pos.x += 2200.0;
    }
    if (work->pos.x > 1100.0) {
        work->pos.x -= 2200.0;
    }
    Hu3DModelPosSetV(mdl, &work->pos);
    Hu3DModelRotSet(mdl, 0.0f, work->rotY, 0.0f);
    scale = (f32)(1.0 + (0.5 * (1.0 - work->f98)));
    Hu3DModelScaleSet(mdl, scale, work->f98, scale);
    if (work->f90 > 0) {
        if (((work->f90 -= 1) & 1) != 0) {
            Hu3DModelAttrSet(mdl, 1);
        } else {
            Hu3DModelAttrReset(mdl, 1);
        }
    }
    fn_1_8BF0(&work->pos, &work->pos64);
}

void fn_1_3D40(OMOBJ *obj)
{
    PLAYERWORK *work = (PLAYERWORK *)obj->data;
    ACTHIT *hit;
    f32 speed;
    f32 v;

    if (work->f3C != work->f84) {
        work->f3C = work->f84;
        work->f34 = 0;
    }
    if (work->f84 != 0) {
        work->f18 |= 0x100;
    } else if (work->f38 > 0) {
        work->f38 -= 1;
        if (work->f38 == 0 && (s32)(frand() & 3) != 0) {
            work->f18 |= 0x100;
        }
    } else {
        speed = fn_1_6E94(work->pos.y);
        hit = fn_1_4298(obj, 0.0f, 350.0f);
        if (hit != NULL) {
            if (hit->f30.id != 0) {
                if (work->f34 > lbl_1_data_128[work->dif]) {
                    work->f18 |= 0x100;
                }
                if (fabs(speed) < 1.666666753590107) {
                    work->f2C = 0.0f;
                    work->f30 = 0.0f;
                } else {
                    v = speed * lbl_1_data_118[work->dif];
                    work->f30 = (f32)fabs(v);
                    if (v < 0.0f) {
                        work->f2C = 270.0f;
                    } else {
                        work->f2C = 90.0f;
                    }
                }
            } else {
                fn_1_43B8(obj, speed, 0.6f);
            }
        } else if (speed >= 0.0f && fn_1_4298(obj, -100.0f, 350.0f) == NULL) {
            work->f2C = 270.0f;
            work->f30 = 0.8f;
            if (!(fabs(fn_1_1E50(work->f2C, work->rotY)) > 60.0)) {
                if (work->dif == 0) {
                    if (work->pos.y < 1000.0) {
                        work->f18 |= 0x100;
                    }
                } else if (work->pos.y < 600.0 && fn_1_4298(obj, -200.0f, 350.0f) != NULL) {
                    work->f18 |= 0x100;
                    work->f30 = 0.4f;
                }
            }
        } else if (speed <= 0.0f && fn_1_4298(obj, 100.0f, 350.0f) == NULL) {
            work->f2C = 90.0f;
            work->f30 = 0.8f;
            if (!(fabs(fn_1_1E50(work->f2C, work->rotY)) > 60.0)) {
                if (work->dif == 0) {
                    if (work->pos.y < 1000.0) {
                        work->f18 |= 0x100;
                    }
                } else if (work->pos.y < 600.0 && fn_1_4298(obj, 200.0f, 350.0f) != NULL) {
                    work->f18 |= 0x100;
                    work->f30 = 0.4f;
                }
            }
        } else {
            fn_1_43B8(obj, speed, 0.8f);
        }
    }
    work->f1C = work->f2C;
    work->f20 = work->f30;
    if (work->f20 > 1.0) {
        work->f20 = 1.0f;
    }
    if ((work->f18 & 0x100) != 0) {
        work->f14 |= 0x100;
    }
    work->f34++;
}

ACTHIT *fn_1_4298(OMOBJ *obj, f32 dx, f32 dy)
{
    PLAYERWORK *work = (PLAYERWORK *)obj->data;

    return fn_1_42E8(obj, dx, dy, work->hit.f24);
}

ACTHIT *fn_1_42E8(OMOBJ *obj, f32 dx, f32 dy, f32 radius)
{
    PLAYERWORK *work = (PLAYERWORK *)obj->data;
    ACTHIT *hit;
    Vec v;
    s32 idx;

    v.x = work->pos.x + dx;
    v.y = work->pos.y + dy;
    v.z = work->pos.z;
    idx = fn_1_89EC(&v, radius);
    if (idx < 0) {
        return NULL;
    }
    hit = fn_1_8E78(idx);
    if (hit == NULL) {
        return NULL;
    }
    if (hit->disabled != 0) {
        return NULL;
    }
    return hit;
}

// Remaining difference: operand load order of the -50.0f add.
void fn_1_43B8(OMOBJ *obj, f32 dir, f32 speed)
{
    PLAYERWORK *work = (PLAYERWORK *)obj->data;
    PLAYERWORK *work2;
    ACTHIT *found;
    ACTHIT *hit;
    Vec v;
    f64 off;
    f32 ox;
    s32 idx;

    if (dir >= 0.0f) {
        off = -100.0;
    } else {
        off = 100.0;
    }
    ox = (f32)off;
    work2 = (PLAYERWORK *)obj->data;
    v.x = work2->pos.x + ox;
    v.y = -50.0f + work2->pos.y;
    v.z = work2->pos.z;
    idx = fn_1_89EC(&v, 150.0f);
    if (idx < 0) {
        found = NULL;
    } else {
        hit = fn_1_8E78(idx);
        if (hit == NULL) {
            found = NULL;
        } else if (hit->disabled != 0) {
            found = NULL;
        } else {
            found = hit;
        }
    }
    if (found == NULL) {
        dir = -dir;
        work->f38 = 0xC;
    }
    if (dir >= 0.0f) {
        work->f2C = 270.0f;
        work->f30 = speed;
        return;
    }
    work->f2C = 90.0f;
    work->f30 = speed;
}

void fn_1_4558(OMOBJ *obj, u32 arg)
{
    PLAYERWORK *work = (PLAYERWORK *)obj->data;
    f32 shift;

    if ((u32)work->motion != arg && arg < 8) {
        if (work->motion < 0) {
            shift = 0.0f;
        } else {
            shift = 60.0f * lbl_1_data_B8[arg].speed;
        }
        work->motion = arg;
        CharMotionShiftSet((s16)work->charNo, obj->mtnId[lbl_1_data_B8[arg].motion], 0.0f, shift, lbl_1_data_B8[arg].attr);
    }
}

s32 fn_1_4650(OMOBJ *obj)
{
    PLAYERWORK *work = (PLAYERWORK *)obj->data;

    if (CharMotionEndCheck((s16)work->charNo) != 0 && CharMotionShiftIDGet((s16)work->charNo) < 0) {
        return 1;
    }
    return 0;
}

s32 fn_1_46BC(OMOBJ *obj)
{
    PLAYERWORK *work = (PLAYERWORK *)obj->data;
    s32 ret = 0;

    if (CharMotionShiftIDGet((s16)work->charNo) < 0) {
        ret = 1;
    }
    return ret;
}

s32 fn_1_471C(OMOBJ *obj, u32 arg)
{
    s32 ret = 0;

    if (fn_1_4650(obj) != 0) {
        fn_1_4558(obj, arg);
        ret = 1;
    }
    return ret;
}

s32 fn_1_485C(OMOBJ *obj)
{
    PLAYERWORK *work = (PLAYERWORK *)obj->data;

    return work->idx;
}

s32 fn_1_4878(OMOBJ *obj, Vec *pos, f32 *rotY)
{
    PLAYERWORK *work = (PLAYERWORK *)obj->data;

    if (pos != NULL) {
        *pos = work->pos;
    }
    if (rotY != NULL) {
        *rotY = work->rotY;
    }
    return 1;
}
