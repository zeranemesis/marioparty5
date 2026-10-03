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
s32 fn_1_8E78(s32 idx);
void omVibrate(s16 player, s16 kind, s16 strength, s16 duration);
void CharModelLandDustCreate(s16 charNo, void *pos);
void fn_1_3D40(OMOBJ *obj);
s32 fn_1_1850(void);
s32 fn_1_1940(void);
s32 fn_1_6DEC(s32 idx, Vec *pos, Vec *out);
void PSVECSubtract(const Vec *a, const Vec *b, Vec *out);
void PSVECAdd(const Vec *a, const Vec *b, Vec *out);
f64 PSVECMag(const Vec *v);
void fn_1_4558(OMOBJ *obj, s32 arg);
void fn_1_796C(void *arg);
void fn_1_8D88(void *arg);
void fn_1_19E0(s32 player, s32 value1, s32 value2);
void CharModelVoicePanSet(s16 charNo, s16 pan);
void CharModelKill(s16 charNo);
HU3DMODELID CharModelCreate(s16 charNo, s16 model);
HU3DMOTID CharMotionCreate(s16 charNo, unsigned int dataNum, s32 mode);
void CharMotionDataClose(s16 charNo);
void CharEffectLayerSet(s16 layerNo);
float CharModelHeightGet(s16 charNo);

static const unsigned int lbl_1_data_98[8] = {
    0x008C0000, 0x008C0001, 0x008C0002, 0x008C0006,
    0x008C0031, 0x008C0003, 0x008C0004, 0x008C0016
};

typedef struct {
    s32 f00;
    s32 f04;
    s32 idx;
    s32 f0C;
    OMOBJ *obj;
    OMOBJFUNC func;
    f32 *p4C;
    f32 *p58;
    f32 height;
    f32 f24;
} PLAYERANIM;
s32 fn_1_1AF8(Vec *pos, s32 useScreen);
s32 fn_1_4650(OMOBJ *obj);

static OMOBJ *lbl_1_bss_28[4];

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
    u32 *work;
    PLAYERANIM *anim;
    s32 idx;
    s32 i;
    s32 charNo;
    s32 modelId;
    s32 motionId;

    obj->data = HuMemDirectMallocNum(HEAP_HEAP, 0xDC, HU_MEMNUM_OVL);
    work = (u32 *)obj->data;
    memset(work, 0, 0xDC);

    obj->stat |= 0x100;

    idx = obj->work[0];
    work[0x00 / 4] = idx;
    work[0x04 / 4] = obj->work[1];
    work[0x08 / 4] = obj->work[2];

    charNo = GwPlayerConf[idx].charNo;
    work[0x0C / 4] = charNo;

    fn_1_19E0(idx, work[0x04 / 4], work[0x0C / 4]);

    work[0x28 / 4] = (u32)GwPlayerConf[idx].dif << 30 >> 30;
    work[0x24 / 4] = GwPlayerConf[idx].type;

    work[0x34 / 4] = 0;
    work[0x38 / 4] = 0;
    work[0x3C / 4] = 0;

    modelId = CharModelCreate(charNo, 2);
    obj->mdlId[0] = modelId;
    Hu3DModelLayerSet(modelId, 5);
    CharEffectLayerSet(6);

    for (i = 0; i < 8; i++) {
        motionId = CharMotionCreate(charNo, lbl_1_data_98[i], 2);
        obj->mtnId[i] = motionId;
    }

    CharMotionDataClose(charNo);

    work[0x10 / 4] = -1;

    fn_1_4558(obj, 0);

    work[0x54 / 4] = 0.0f;
    obj->trans.z = 0.0f;
    work[0x50 / 4] = 0.0f;
    obj->trans.y = 0.0f;
    work[0x4C / 4] = 0.0f;
    obj->trans.x = 0.0f;

    work[0x60 / 4] = 0.0f;
    work[0x5C / 4] = 0.0f;
    work[0x58 / 4] = 0.0f;

    work[0x70 / 4] = 0.0f;
    work[0x88 / 4] = 0;
    work[0x8C / 4] = 0;
    work[0x94 / 4] = 1.0f;
    work[0x98 / 4] = 1.0f;
    work[0x90 / 4] = 0;

    anim = (PLAYERANIM *)((u8 *)work + 0x9C);
    anim->f00 = 0;
    anim->f04 = 0;
    anim->idx = idx;
    anim->f0C = 0;
    anim->obj = obj;
    anim->func = fn_1_3BBC;
    anim->p4C = (f32 *)((u8 *)work + 0x4C);
    anim->p58 = (f32 *)((u8 *)work + 0x58);

    work[0x74 / 4] = (f32)(0.8 * CharModelHeightGet(charNo));
    anim->height = work[0x74 / 4];
    anim->f24 = 40.0f;

    fn_1_8D88(anim);

    work[0xD0 / 4] = 1;
    work[0xD4 / 4] = 70.0f;
    work[0xD8 / 4] = (u32)((u8 *)work + 0x64);
    fn_1_796C((void *)((u8 *)work + 0xD0));

    work[0x40 / 4] = 0;
    obj->objFunc = fn_1_28E0;
}

void fn_1_28E0(OMOBJ *obj)
{
    u32 *work = (u32 *)obj->data;
    Vec save;
    s32 padNo;
    double stickX;
    double stickY;

    work[0x14 / 4] = 0;
    work[0x18 / 4] = 0;
    *(f32 *)((u8 *)work + 0x1C) = 0.0f;
    *(f32 *)((u8 *)work + 0x20) = 0.0f;

    switch (work[0x40 / 4]) {
    case 0:
        save = *(Vec *)((u8 *)work + 0x4C);

        if (fn_1_6DEC(work[0x00 / 4], (Vec *)((u8 *)work + 0x4C),
                      (Vec *)((u8 *)work + 0x70)) != 0) {
            PSVECSubtract((Vec *)((u8 *)work + 0x4C), &save,
                          (Vec *)((u8 *)work + 0x58));

            if ((s32)work[0x48 / 4] == 0 ||
                PSVECMag((Vec *)((u8 *)work + 0x58)) < 1.0) {
                fn_1_4558(obj, 0);
            } else {
                fn_1_4558(obj, 2);
            }

            fn_1_2E7C(obj);
            return;
        }

        work[0x9C / 4] |= 1;
        work[0x40 / 4]++;
    case 1:
        if ((u32)fn_1_1850() != 2) {
            break;
        }

        work[0x40 / 4]++;
        /* fall through */
    case 2:
        if ((u32)fn_1_1850() != 2) {
            work[0x40 / 4]++;
            work[0x44 / 4] = 0;
            work[0x7C / 4] = 1;
            break;
        }

        if ((s32)work[0x24 / 4] != 0) {
            fn_1_3D40(obj);
            break;
        }

        padNo = GwPlayerConf[work[0x00 / 4]].padNo;

        // NONMATCHING: the original sets the OE bit (fsubs) on the int->double
        // conversions below; this compiler emits fsub for the same expression.
        stickX = (double)HuPadStkX[padNo];
        stickY = (double)(-HuPadStkY[padNo]);

        work[0x18 / 4] = HuPadBtn[padNo];
        work[0x14 / 4] = HuPadBtnDown[padNo];

        *(f32 *)((u8 *)work + 0x1C) =
            (f32)(180.0 * (atan2(stickX, stickY) / 3.141592653589793));
        *(f32 *)((u8 *)work + 0x20) =
            (f32)(0.01785714365541935 * fabs(stickX));

        if (*(f32 *)((u8 *)work + 0x20) > 1.0f) {
            *(f32 *)((u8 *)work + 0x20) = 1.0f;
        }
        break;
    case 3:
        if ((s32)++work[0x44 / 4] < 0x14) {
            break;
        }

        work[0x40 / 4]++;
        fn_1_4558(obj, 0);
        /* fall through */
    case 4:
        if ((u32)fn_1_1850() == 3 && fn_1_1940() != 0) {
            work[0x40 / 4] = 0;
            obj->objFunc = fn_1_2C68;
        }
        break;
    }

    fn_1_3110(obj);
    fn_1_2F08(obj);
    PSVECAdd((Vec *)((u8 *)work + 0x4C), (Vec *)((u8 *)work + 0x58),
             (Vec *)((u8 *)work + 0x4C));
    fn_1_2E7C(obj);
}

void fn_1_2C68(OMOBJ *obj)
{
    u32 *work = (u32 *)obj->data;

    work[0x14 / 4] = 0;
    work[0x18 / 4] = 0;
    *(f32 *)((u8 *)work + 0x1C) = 0.0f;
    *(f32 *)((u8 *)work + 0x20) = 0.0f;
    *(f32 *)((u8 *)work + 0x58) =
        *(f32 *)((u8 *)work + 0x5C) =
        *(f32 *)((u8 *)work + 0x60) = 0.0f;

    switch (work[0x40 / 4]) {
    case 0:
        fn_1_18E0(work[0x00 / 4]);
        work[0x7C / 4] = 0;
        work[0x80 / 4] = 0;
        work[0x40 / 4]++;
        /* fall through */
    case 1:
        fn_1_2F08(obj);

        if ((u32)fn_1_1868() == 0) {
            break;
        }

        work[0x40 / 4]++;
        work[0x44 / 4] = 0;
        break;
    case 2:
        fn_1_2F08(obj);
        *(f32 *)((u8 *)work + 0x70) =
            fn_1_1D34(*(f32 *)((u8 *)work + 0x70), 0.0f, 0.1f);

        if ((s32)++work[0x44 / 4] < 0xA) {
            break;
        }

        work[0x40 / 4]++;
        work[0x44 / 4] = 0;
        break;
    case 3:
        fn_1_2F08(obj);
        *(f32 *)((u8 *)work + 0x70) =
            fn_1_1D34(*(f32 *)((u8 *)work + 0x70), 0.0f, 0.1f);

        if ((s32)++work[0x44 / 4] < 0x14) {
            break;
        }

        if (fn_1_18B0(work[0x00 / 4]) >= 0) {
            fn_1_4558(obj, 3);
        } else {
            fn_1_4558(obj, 4);
            fn_1_1A20(work[0x00 / 4]);
        }

        work[0x40 / 4]++;
        break;
    case 5:
        if (fn_1_4650(obj) != 0) {
            fn_1_1910(work[0x00 / 4]);
        }
        break;
    }

    fn_1_3110(obj);
    PSVECAdd((Vec *)((u8 *)work + 0x4C), (Vec *)((u8 *)work + 0x58),
             (Vec *)((u8 *)work + 0x4C));
    fn_1_2E7C(obj);
}

void fn_1_2E7C(OMOBJ *obj)
{
    u32 *work = (u32 *)obj->data;
    s32 pan;

    work[0x48 / 4]++;

    pan = fn_1_1AF8((Vec *)((u8 *)work + 0x4C), 1);
    CharModelVoicePanSet((s16)work[0x0C / 4], (s16)pan);

    if ((s32)work[0x80 / 4] != 0 && fn_1_4650(obj) != 0) {
        work[0x80 / 4] = 0;
    }
}

void fn_1_2F08(OMOBJ *obj)
{
    u32 *work = (u32 *)obj->data;
    s32 n2;
    s32 flag;
    s32 n0;
    s32 n1;
    f32 speed;
    f32 limit;
    f32 sp;

    speed = *(f32 *)((u8 *)work + 0x20) * *(f32 *)((u8 *)work + 0x94);
    n0 = 0;
    n1 = 1;
    n2 = 2;

    if ((s32)work[0x84 / 4] != 0) {
        *(f32 *)((u8 *)work + 0x70) =
            fn_1_1D34(*(f32 *)((u8 *)work + 0x70),
                      *(f32 *)((u8 *)work + 0x1C), 0.3f);
        return;
    }

    flag = (s32)work[0x80 / 4];

    if ((s32)work[0x10 / 4] == 6) {
        flag = 0;
    }

    if (speed > 0.05f && (s32)work[0x7C / 4] == 0 && flag == 0) {
        work[0x80 / 4] = 0;

        *(f32 *)((u8 *)work + 0x70) =
            fn_1_1D34(*(f32 *)((u8 *)work + 0x70),
                      *(f32 *)((u8 *)work + 0x1C), 0.3f);

        *(f32 *)((u8 *)work + 0x58) =
            (f32)(0.01666666753590107 *
                  (550.0 *
                   (speed *
                    sin(3.141592653589793 * *(f32 *)((u8 *)work + 0x1C) /
                        180.0))));

        if (fn_1_46BC(obj) != 0) {
            limit = 0.5f;

            if ((s32)work[0x10 / 4] == n2) {
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
        if ((s32)work[0x80 / 4] == 0) {
            fn_1_4558(obj, n0);
        }

        *(f32 *)((u8 *)work + 0x58) = 0.0f;
    }

    *(f32 *)((u8 *)work + 0x58) =
        *(f32 *)((u8 *)work + 0x58) + *(f32 *)((u8 *)work + 0x78);
}

// NONMATCHING: register/stack allocation still differs (two extra FPRs
// spilled for the fabs and fmod temporaries).
void fn_1_3110(OMOBJ *obj)
{
    u32 *work = (u32 *)obj->data;
    u8 *tile;
    s32 idx;
    s32 tileIdx;
    f64 absSpeed;
    f32 wrap;
    f32 speed;
    f32 ang;
    f32 t;
    f32 t2;
    Vec v;

    fn_1_3734(obj);

    *(f32 *)((u8 *)work + 0x78) = 0.0f;

    speed = (f32)(0.01666666753590107 *
                  (550.0 * (*(f32 *)((u8 *)work + 0x20) *
                           sin(3.141592653589793 *
                                   *(f32 *)((u8 *)work + 0x1C) /
                               180.0))));

    speed = speed * *(f32 *)((u8 *)work + 0x94);

    if ((s32)work[0x84 / 4] == 0) {
        if (0.0f != *(f32 *)((u8 *)work + 0x94) &&
            (work[0x14 / 4] & 0x100) != 0) {
            *(f32 *)((u8 *)work + 0x58) = speed;
            *(f32 *)((u8 *)work + 0x5C) =
                (f32)(26.66666805744171 * *(f32 *)((u8 *)work + 0x94));
            work[0x84 / 4] = 1;
            fn_1_4558(obj, 5);
            return;
        }

        v.x = *(f32 *)((u8 *)work + 0x4C);
        v.y = *(f32 *)((u8 *)work + 0x50) - 10.0;
        v.z = *(f32 *)((u8 *)work + 0x54);

        idx = fn_1_89EC(&v, *(f32 *)((u8 *)work + 0xC0));

        if (idx >= 0) {
            tileIdx = fn_1_8E78(idx);
            tile = (u8 *)tileIdx;

            if (*(f32 **)((u8 *)tile + 0x1C) != NULL &&
                *(f32 *)((u8 *)*(f32 **)((u8 *)tile + 0x1C) + 4) > 0.0f) {
                if (*(f32 *)((u8 *)work + 0x4C) >
                    *(f32 *)(*(u32 *)((u8 *)tile + 0x18) + 0)) {
                    *(f32 *)((u8 *)work + 0x58) += 5.000000260770321;
                } else {
                    *(f32 *)((u8 *)work + 0x58) -= 5.000000260770321;
                }

                *(f32 *)((u8 *)work + 0x5C) += 16.66666753590107;
                work[0x84 / 4] = 1;
                fn_1_4558(obj, 5);
                omVibrate((s16)work[0x00 / 4], 0xC, 6, 6);
            }
        }

        if (idx < 0) {
            absSpeed = fabs(speed);

            if (absSpeed > 5.000000260770321) {
                ang = *(f32 *)((u8 *)work + 0x1C);
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

                idx = fn_1_89EC(&v, *(f32 *)((u8 *)work + 0xC0));
            }

            work[0x84 / 4] = 1;

            if (0.0f != *(f32 *)((u8 *)work + 0x94)) {
                fn_1_4558(obj, 5);
            }
        }

        if (idx >= 0) {
            *(f32 *)((u8 *)work + 0x50) = v.y;
            tileIdx = fn_1_8E78(idx);
            tile = (u8 *)tileIdx;

            if (*(f32 **)((u8 *)tile + 0x1C) != NULL) {
                *(f32 *)((u8 *)work + 0x78) =
                    *(f32 *)((u8 *)*(f32 **)((u8 *)tile + 0x1C) + 0);
            }
        }
    } else {
        *(f32 *)((u8 *)work + 0x58) =
            *(f32 *)((u8 *)work + 0x58) + 0.05 * speed;
        *(f32 *)((u8 *)work + 0x58) =
            *(f32 *)((u8 *)work + 0x58) * 0.98;

        if ((work[0x18 / 4] & 0x100) != 0 &&
            *(f32 *)((u8 *)work + 0x5C) >= 0.0f) {
            *(f32 *)((u8 *)work + 0x5C) =
                *(f32 *)((u8 *)work + 0x5C) - 0.9166667144745589;
        } else {
            *(f32 *)((u8 *)work + 0x5C) =
                *(f32 *)((u8 *)work + 0x5C) - 1.6333334185183048;
        }

        v.x = *(f32 *)((u8 *)work + 0x4C);
        v.y = *(f32 *)((u8 *)work + 0x50) - 10.0;
        v.z = *(f32 *)((u8 *)work + 0x54);

        idx = fn_1_89EC(&v, *(f32 *)((u8 *)work + 0xC0));

        if (*(f32 *)((u8 *)work + 0x5C) < 0.0f && idx >= 0) {
            *(f32 *)((u8 *)work + 0x50) = v.y;
            *(f32 *)((u8 *)work + 0x5C) = 0.0f;
            *(f32 *)((u8 *)work + 0x58) = 0.0f;
            work[0x84 / 4] = 0;

            CharModelLandDustCreate((s16)work[0x0C / 4],
                                   (void *)((u8 *)work + 0x4C));

            if (0.0f != *(f32 *)((u8 *)work + 0x94)) {
                fn_1_4558(obj, 6);
                work[0x80 / 4] = 1;
            }
        }

        v.x = *(f32 *)((u8 *)work + 0x4C);
        v.y = 10.0 + (*(f32 *)((u8 *)work + 0x50) +
                      *(f32 *)((u8 *)work + 0xBC));
        v.z = *(f32 *)((u8 *)work + 0x54);

        idx = fn_1_89EC(&v, *(f32 *)((u8 *)work + 0xC0));

        if (*(f32 *)((u8 *)work + 0x5C) > 0.0f && idx >= 0) {
            tileIdx = fn_1_8E78(idx);
            tile = (u8 *)tileIdx;

            if (*(s32 *)((u8 *)tile + 0x38) == 0) {
                *(u32 *)((u8 *)tile + 0x38) = 1;
                *(u32 *)((u8 *)tile + 0x3C) = (u32)obj;
                *(u32 *)((u8 *)tile + 0x34) = work[0x00 / 4];
            }

            if (*(f32 *)((u8 *)work + 0x5C) > 0.0f) {
                *(f32 *)((u8 *)work + 0x5C) = 0.0f;
            }
        }
    }
}
