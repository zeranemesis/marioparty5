#define _MATH_H

#include "REL/m504dll.h"
#include "game/data.h"
#include "string.h"

#ifdef __MWERKS__
#define WW_RODATA __declspec(section ".rodata")
#else
#define WW_RODATA
#endif

#define WW_ABS(x) (((x) >= 0.0f) ? (x) : -(x))

extern double atan2();
extern double sin();
extern double cos();
extern double acos();
extern double fmod();
extern double sqrt();
extern s32 rand8(void);

extern const Vec lbl_1_rodata_1D0;
extern const Vec lbl_1_rodata_1DC;

extern s32 fn_1_4030(M504STAGE *stage, Vec *pos, Vec *out, s32 flag, s16 *wallIdx, s16 *cornerIdx, f32 range);
extern void fn_1_5870(M504STAGE *stage, s16 wallIdx);
extern void fn_1_5EC(s32 seId);
extern void fn_1_C99C(M504CAMERA *cam, s16 type, s32 cnt, f32 amp, f32 decay, f32 speed);
extern void fn_1_D1F0(s16 camMask, Vec *pos);

WW_RODATA const Vec lbl_1_rodata_1FC = { 0.0f, 0.0f, 0.0f };
WW_RODATA const Vec lbl_1_rodata_208 = { 0.0f, 0.0f, 0.0f };

Vec lbl_1_data_4C4[8] = {
    { 0.0f, 0.0f, -1.0f },
    { 1.0f, 0.0f, -1.0f },
    { 1.0f, 0.0f, 0.0f },
    { 1.0f, 0.0f, 1.0f },
    { 0.0f, 0.0f, 1.0f },
    { -1.0f, 0.0f, 1.0f },
    { -1.0f, 0.0f, 0.0f },
    { -1.0f, 0.0f, -1.0f }
};

#define WANWAN_HEADING (180.0 * (atan2(dir.x, dir.z) / 3.141592653589793))
#define WANWAN_HEADING_V (180.0 * (atan2(v.x, v.z) / 3.141592653589793))

void fn_1_9680(M504WANWAN *self, M504SLOT *slot)
{
    s16 *mdlTbl;
    s32 slotNo;
    s16 mdl;

    mdlTbl = slot->work->mdl;
    slotNo = slot->slotNo;
    memset(self, 0, sizeof(M504WANWAN));
    self->slot = slot;
    self->active = 0;
    self->step = 0;
    self->count = 0;
    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0x3E0000, 0x10000000, HEAP_MODEL));
    self->mdl = mdl;
    Hu3DModelReflectTypeSet(mdl, 4);
    self->mtn[0] = Hu3DJointMotion(mdl, HuDataSelHeapReadNum(0x3E003D, 0x10000000, HEAP_MODEL));
    self->mtn[1] = Hu3DJointMotion(mdl, HuDataSelHeapReadNum(0x3E003E, 0x10000000, HEAP_MODEL));
    self->mtn[2] = Hu3DJointMotion(mdl, HuDataSelHeapReadNum(0x3E003F, 0x10000000, HEAP_MODEL));
    self->mtn[3] = Hu3DJointMotion(mdl, HuDataSelHeapReadNum(0x3E0040, 0x10000000, HEAP_MODEL));
    self->mtn[4] = Hu3DJointMotion(mdl, HuDataSelHeapReadNum(0x3E0041, 0x10000000, HEAP_MODEL));
    self->mtn[5] = Hu3DJointMotion(mdl, HuDataSelHeapReadNum(0x3E0042, 0x10000000, HEAP_MODEL));
    self->mtn[6] = Hu3DJointMotion(mdl, HuDataSelHeapReadNum(0x3E0043, 0x10000000, HEAP_MODEL));
    self->mtn[7] = Hu3DJointMotion(mdl, HuDataSelHeapReadNum(0x3E0044, 0x10000000, HEAP_MODEL));
    self->mtn[8] = Hu3DJointMotion(mdl, HuDataSelHeapReadNum(0x3E0045, 0x10000000, HEAP_MODEL));
    self->mtn[9] = Hu3DJointMotion(mdl, HuDataSelHeapReadNum(0x3E0046, 0x10000000, HEAP_MODEL));
    Hu3DMotionSet(mdl, self->mtn[0]);
    Hu3DMotionSpeedSet(mdl, 2.0f);
    Hu3DModelAttrSet(mdl, 0x40000001);
    Hu3DModelShadowSet(mdl);
    Hu3DModelCameraSet(mdl, (u16)lbl_1_data_45E0[slot->slotNo]);
    Hu3DModelAttrReset(mdl, 1);
    self->pos = lbl_1_rodata_1D0;
    self->rot = lbl_1_rodata_1DC;
    Hu3DModelPosSet(mdl, self->pos.x, self->pos.y, self->pos.z);
    Hu3DModelRotSet(mdl, self->rot.x, self->rot.y, self->rot.z);
    self->state = 5;
    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0x3E0024, 0x10000000, HEAP_MODEL));
    self->mdl28 = mdl;
    Hu3DModelPosSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(mdl, 4.0f, 4.0f, 4.0f);
    Hu3DModelCameraSet(mdl, (u16)lbl_1_data_45E0[slotNo]);
    Hu3DModelLayerSet(mdl, 5);
    Hu3DModelAttrSet(mdl, 1);
    self->mot2A = Hu3DJointMotion(mdl, HuDataSelHeapReadNum(0x3E0047, 0x10000000, HEAP_MODEL));
    Hu3DMotionSet(mdl, self->mot2A);
    Hu3DMotionSpeedSet(mdl, 1.0f);
    Hu3DModelAttrReset(mdl, 0x40000001);
    self->on2C = 0;
    mdl = Hu3DModelCreate(HuDataSelHeapReadNum(0x3E0025, 0x10000000, HEAP_MODEL));
    self->mdl30 = mdl;
    Hu3DModelPosSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelRotSet(mdl, 0.0f, 0.0f, 0.0f);
    Hu3DModelScaleSet(mdl, 2.5f, 2.5f, 2.5f);
    Hu3DModelCameraSet(mdl, (u16)lbl_1_data_45E0[slotNo]);
    Hu3DModelLayerSet(mdl, 5);
    Hu3DModelAttrSet(mdl, 1);
    self->mot32 = Hu3DJointMotion(mdl, HuDataSelHeapReadNum(0x3E0048, 0x10000000, HEAP_MODEL));
    Hu3DMotionSet(mdl, self->mot32);
    Hu3DMotionSpeedSet(mdl, 2.0f);
    Hu3DModelAttrReset(mdl, 0x40000001);
    self->on34 = 0;
}

void fn_1_9C10(M504WANWAN *self)
{
    s32 i;

    Hu3DMotionKill(self->mot32);
    Hu3DMotionKill(self->mot2A);
    Hu3DModelKill(self->mdl30);
    Hu3DModelKill(self->mdl28);
    /* NONMATCHING: CodeWarrior materialises the 0x4002 attr mask as lis+addi, MWCC folds it into one li. */
    Hu3DModelAttrSet(self->mdl, 0x4000 + HU3D_ATTR_ZWRITE_OFF);

    for (i = 0; i < 10; i++) {
        Hu3DMotionKill(self->mtn[i]);
    }

    Hu3DModelKill(self->mdl);
    memset(self, 0, sizeof(M504WANWAN));
}

s32 fn_1_9CAC(M504WANWAN *self)
{
    M504SLOT *slot;
    M504STAGE *stage;
    M504CAMERA *cam;
    s32 ret;
    Vec hit;
    Vec dirA;
    Vec dirB;
    s16 wallIdx;
    s16 cornerIdx;
    f32 a;
    f32 b;
    f32 c;
    f32 spd;
    f32 mag;
    f32 ang;
    f32 maxTime;
    s32 hitF;
    u16 t;

    slot = self->slot;
    cam = &slot->cam;
    stage = &slot->stage;
    if (slot->work->unk1218 != 0) {
        return 0;
    }
    self->off = lbl_1_rodata_1FC;
    ret = 0;
    switch (self->state) {
        case 0:
            if (self->active != 0) {
                self->state = 1;
            }
            break;
        case 1:
            Hu3DMotionShiftSet(self->mdl, self->mtn[4], 0.0f, 8.0f, 0);
            self->state = 2;
            break;
        case 2:
            self->rot.y += 24.0f;
            if (self->rot.y > 180.0f) {
                a = self->rot.y - 360.0f;
            } else {
                if (self->rot.y <= -180.0f) {
                    b = 360.0f + self->rot.y;
                } else {
                    b = self->rot.y;
                }
                a = b;
            }
            self->rot.y = a;
            if (!(self->rot.y < 0.0f) && !(self->rot.y > 24.0f)) {
                if (self->step != 0) {
                    self->step--;
                }
                if (self->step == 0 && slot->player.mode != 3) {
                    self->vel.y = 20.0f;
                    self->f74 = 6.0 + (0.8 * (f64)(f32)self->count);
                    self->rot.y = self->face;
                    self->f68 = 20.0f;
                    self->f6C = 0.0f;
                    self->timer = 30;
                    Hu3DModelAttrReset(self->mdl, 0x40000001);
                    Hu3DMotionShiftSet(self->mdl, self->mtn[2], 0.0f, 8.0f, 0);
                    if (self->active != 0) {
                        fn_1_5EC(0x2D8);
                    }
                    self->state = 3;
                }
            }
            break;
        case 3:
            self->f6C += 10.0f;
            if (self->f6C > 90.0f) {
                self->f6C = 0.0f;
            }
            c = cos(3.141592653589793 * self->f6C / 180.0);
            self->rot.x = self->f68 * c;
            self->off.x = (50.0f * c) * sin(3.141592653589793 * self->rot.y / 180.0);
            self->off.z = (50.0f * c) * cos(3.141592653589793 * self->rot.y / 180.0);
            if (self->timer != 0) {
                self->timer--;
            }
            if (self->timer == 0) {
                Hu3DMotionShiftSet(self->mdl, self->mtn[3], 0.0f, 8.0f, 0x40000001);
                self->rot.x = 0.0f;
                self->state = 4;
            }
            break;
        case 4:
            self->f74 = self->f74 + 0.8;
            self->vel.y += -3.75f;
            spd = self->f74 + (0.8f * self->vel.y);
            self->vel.x = spd * sin(3.141592653589793 * self->face / 180.0);
            self->vel.z = spd * cos(3.141592653589793 * self->face / 180.0);
            self->pos.x += self->vel.x;
            self->pos.y += self->vel.y;
            self->pos.z += self->vel.z;
            if (self->pos.y < 0.0f) {
                fn_1_D1F0((s16)lbl_1_data_45E0[slot->slotNo], &self->pos);
                if (self->active != 0) {
                    if (slot->unk46C != 0) {
                        fn_1_5EC(0x646);
                    } else {
                        fn_1_5EC(0x647);
                    }
                }
                self->pos.y = 0.0f;
                self->vel.y = 20.0f;
            }
            self->timer = 1;
            if (fn_1_4030(stage, &self->pos, &hit, 1, &wallIdx, &cornerIdx, 100.0f) != 0 && (mag = PSVECMag(&hit), (cornerIdx < 0) || !(mag < spd))) {
                dirA = self->vel;
                dirA.y = 0.0f;
                PSVECNormalize(&dirA, &dirA);
                dirB.x = -hit.x;
                dirB.z = -hit.z;
                dirB.y = 0.0f;
                PSVECNormalize(&dirB, &dirB);
                ang = 180.0 * ((f32)acos(PSVECDotProduct(&dirA, &dirB)) / 3.141592653589793);
                hitF = 0;
                if (wallIdx >= 0) {
                    if (stage->obj[wallIdx].type == 1) {
                        if (!(ang > 45.0f)) {
                            hitF = 1;
                        }
                    } else if (!(ang > 75.0f)) {
                        hitF = 1;
                    }
                } else if (!(ang > 75.0f)) {
                    hitF = 1;
                }
                if (hitF != 0) {
                    self->timer = 0;
                    Hu3DModelPosSet(self->mdl28, self->pos.x, 200.0f + self->pos.y, self->pos.z);
                    Hu3DMotionTimeSet(self->mdl28, 0.0f);
                    Hu3DModelAttrReset(self->mdl28, 1);
                    self->on2C = 1;
                    if (self->active != 0) {
                        if (slot->unk46C != 0) {
                            fn_1_5EC(0x649);
                        } else {
                            fn_1_5EC(0x64A);
                        }
                    }
                    omVibrate((s16)slot->slotNo, 20, 6, 6);
                    Hu3DModelAttrReset(self->mdl, 0x40000001);
                    Hu3DMotionShiftSet(self->mdl, self->mtn[7], 0.0f, 2.0f, 0);
                    fn_1_C99C(cam, 0, 60, 25.0f, 0.95f, 90.0f);
                    fn_1_5870(stage, wallIdx);
                    self->state = 5;
                }
            }
            break;
        case 5:
            if (Hu3DMotionShiftIDGet(self->mdl) < 0) {
                Hu3DMotionShiftSet(self->mdl, self->mtn[6], 0.0f, 8.0f, 0x40000001);
                self->state = 6;
            }
            /* fallthrough */
        case 6:
            self->vel.y += -3.75f;
            self->pos.y += self->vel.y;
            if (!(self->pos.y > 0.0f)) {
                self->pos.y = 0.0f;
                t = self->timer;
                self->timer = t + 1;
                if (t >= 120) {
                }
            }
            break;
        case 7:
            t = self->timer;
            self->timer = t + 1;
            if (t >= 30) {
                self->timer = 0;
                self->state = 1;
            }
            break;
        case 8:
            self->f74 = self->f74 + 0.6;
            self->vel.y += -3.75f;
            self->rot.y *= 0.9f;
            self->vel.x = 0.1f * -self->pos.x;
            self->vel.z = self->f74 + (0.8f * self->vel.y);
            self->pos.x += self->vel.x;
            self->pos.y += self->vel.y;
            self->pos.z += self->vel.z;
            if (self->pos.y < 0.0f) {
                self->pos.y = 0.0f;
                self->vel.y = 40.0f;
            }
            if (self->pos.z > 3700.0f) {
                self->pos.z = 3700.0f;
                a = WW_ABS(self->rot.y);
                if (a < 3.0f) {
                    if (self->pos.y <= 0.0f) {
                        self->rot.y = 0.0f;
                        Hu3DMotionShiftSet(self->mdl, self->mtn[8], 0.0f, 8.0f, 0x40000001);
                        self->state = 10;
                    }
                }
            }
            break;
        case 9:
            self->pos.y = 0.0f;
            a = WW_ABS(self->rot.y);
            if (a > 24.0f) {
                self->rot.y += 24.0f;
                if (self->rot.y > 180.0f) {
                    b = self->rot.y - 360.0f;
                } else {
                    if (self->rot.y <= -180.0f) {
                        c = 360.0f + self->rot.y;
                    } else {
                        c = self->rot.y;
                    }
                    b = c;
                }
                self->rot.y = b;
            } else {
                self->rot.y = 0.0f;
            }
            break;
    }
    if (self->state < 8 && self->pos.z > 3550.0f) {
        self->active = 0;
        ret = 1;
    }
    if (fn_1_4030(stage, &self->pos, &hit, 1, &wallIdx, &cornerIdx, 100.0f) != 0) {
        self->pos.x += hit.x;
        self->pos.z += hit.z;
    }
    Hu3DModelPosSet(self->mdl, self->pos.x + self->off.x, self->pos.y + self->off.y, self->pos.z + self->off.z);
    Hu3DModelRotSet(self->mdl, self->rot.x, self->rot.y, self->rot.z);
    if (self->unk24 != 0 && self->state != 9) {
        Hu3DMotionShiftSet(self->mdl, self->mtn[8], 0.0f, 8.0f, 0x40000001);
        self->off = lbl_1_rodata_208;
        self->pos.y = 0.0f;
        self->rot.x = 0.0f;
        self->state = 9;
    }
    if (self->on2C != 0) {
        maxTime = Hu3DMotionMaxTimeGet(self->mdl28);
        if (Hu3DMotionTimeGet(self->mdl28) >= maxTime) {
            Hu3DModelAttrSet(self->mdl28, 1);
            self->on2C = 0;
        }
    }
    if (self->on34 != 0) {
        maxTime = Hu3DMotionMaxTimeGet(self->mdl30);
        if (Hu3DMotionTimeGet(self->mdl30) >= maxTime) {
            Hu3DModelAttrSet(self->mdl30, 1);
            self->on34 = 0;
        }
    }
    return ret;
}

void fn_1_AB40(Mtx m, Vec *rot)
{
    f32 s;
    f32 c;
    f64 v;
    f64 y;
    struct {
        f32 a;
    } tmp;

    rot->x = 180.0 * (atan2(m[1][2], m[2][2]) / 3.141592653589793);
    if (rot->x < 0.0f) {
        rot->x += 360.0f;
    }
    rot->z = 180.0 * (atan2(m[0][1], m[0][0]) / 3.141592653589793);
    if (rot->z < 0.0f) {
        rot->z += 360.0f;
    }
    s = -m[0][2];
    if ((f32)(1.0 - (f64)(s * s)) >= 0.0) {
        v = 1.0 - (f64)(s * s);
    } else {
        v = -(1.0 - (f64)(s * s));
    }
    if ((f32)v > 0.0f) {
        /* CodeWarrior inlines sqrt as frsqrte + three Newton steps; MWCC calls sqrt for the seed. */
        y = 1.0 / sqrt((f64)(f32)v);
        y = (0.5 * y) * (3.0 - (f64)(f32)v * (y * y));
        y = (0.5 * y) * (3.0 - (f64)(f32)v * (y * y));
        tmp.a = (f32)((f64)(f32)v * ((0.5 * y) * (3.0 - (f64)(f32)v * (y * y))));
        c = tmp.a;
    } else {
        c = (f32)v;
    }
    if (rot->x > 90.0f && rot->x < 270.0f && rot->z > 90.0f && rot->z < 270.0f) {
        rot->x = fmod(180.0f + rot->x, 360.0);
        rot->z = fmod(180.0f + rot->z, 360.0);
        c = -c;
    }
    rot->y = 180.0 * (atan2(s, c) / 3.141592653589793);
    if (rot->y < 0.0f) {
        rot->y += 360.0f;
    }
}

s32 fn_1_AEE0(M504WANWAN *self, f32 *outAng)
{
    Vec dir;
    Vec face;
    M504PLAYER *grp;
    f64 angHi;
    f64 angLo;
    f64 dot;
    f32 deg;
    f32 theta;

    grp = &self->slot->player;
    PSVECSubtract(&grp->pos, &self->pos, &dir);
    dir.y = 0.0f;
    PSVECNormalize(&dir, &dir);

    if (WANWAN_HEADING > 180.0) {
        angHi = WANWAN_HEADING - 360.0;
    } else {
        if (WANWAN_HEADING <= -180.0) {
            angLo = 360.0 + WANWAN_HEADING;
        } else {
            angLo = WANWAN_HEADING;
        }
        angHi = angLo;
    }
    *outAng = (f32)angHi;

    face.x = (f32)sin(3.141592653589793 * self->rot.y / 180.0);
    face.y = 0.0f;
    face.z = (f32)cos(3.141592653589793 * self->rot.y / 180.0);

    dot = PSVECDotProduct(&dir, &face);
    theta = (f32)acos(dot);
    deg = (f32)(180.0 * (theta / 3.141592653589793));

    if (deg < 24.0f) {
        return 1;
    }
    return 0;
}

void fn_1_B1AC(M504WANWAN *self)
{
    Vec v;
    f64 angHi;
    f64 angLo;
    M504PLAYER *grp;

    grp = &self->slot->player;

    if (self->active != 0 && (self->state == 5 || self->state == 6)) {
        self->rot.z = self->rot.x = 0.0f;
        self->rot.y = self->face;
        self->vel.y = 0.0f;
        self->pos.y = 0.0f;
        self->step = (u16)(rand8() % 3);
        self->count = 0;
        Hu3DModelAttrReset(self->mdl, 0x4000 + HU3D_ATTR_DISPOFF);
        Hu3DMotionShiftSet(self->mdl, self->mtn[5], 0.0f, 8.0f, 0);

        PSVECSubtract(&grp->pos, &self->pos, &v);
        v.y = 0.0f;
        PSVECNormalize(&v, &v);

        if (WANWAN_HEADING_V > 180.0) {
            angHi = WANWAN_HEADING_V - 360.0;
        } else {
            if (WANWAN_HEADING_V <= -180.0) {
                angLo = 360.0 + WANWAN_HEADING_V;
            } else {
                angLo = WANWAN_HEADING_V;
            }
            angHi = angLo;
        }

        self->face = (f32)angHi;
        self->timer = 0;
        self->state = 7;
    }
}

s32 fn_1_B43C(M504WANWAN *self, Vec *from, f32 range, Vec *dst, s32 flag)
{
    Vec v;
    f32 mag;
    f32 scale;

    PSVECSubtract(from, &self->pos, &v);

    if (flag == 0) {
        v.y = 0.0f;
    }

    mag = PSVECMag(&v);

    if (mag < 100.0f + range) {
        PSVECNormalize(&v, &v);
        scale = 100.0f + range - mag;
        PSVECScale(&v, dst, scale);
        return 1;
    }
    return 0;
}

s32 fn_1_B52C(M504WANWAN *self, Vec *outPos)
{
    Vec pts[8];
    s32 ids[8];
    Vec v;
    Vec face;
    Vec hit;
    s16 wallIdx;
    s16 cornerIdx;
    f32 radius;
    f32 ang;
    f32 bestAng;
    f32 dot;
    f32 theta;
    s32 i;
    s32 n;
    s32 ring;
    s32 bestIdx;
    M504SLOT *slot;
    M504PLAYER *grp;
    M504STAGE *stage;

    slot = self->slot;
    grp = &slot->player;
    stage = &slot->stage;
    radius = 250.0f;
    n = ring = 0;

    while (n == 0) {
        n = 0;

        for (i = 0; i < 8; i++) {
            PSVECNormalize(&lbl_1_data_4C4[i], &v);
            PSVECScale(&v, &v, radius);
            PSVECAdd(&v, &self->pos, &v);

            if (fn_1_4030(stage, &v, &hit, 0, &wallIdx, &cornerIdx, 50.0f) == 0) {
                ids[n] = i;
                pts[n] = v;
                n++;
            }
        }

        radius += 100.0f;

        if (ring++ > 4) {
            break;
        }
    }

    if (n == 0) {
        return 0;
    }

    bestAng = 0.0f;
    bestIdx = 0;

    for (i = 0; i < n; i++) {
        PSVECSubtract(&pts[i], &self->pos, &v);
        PSVECNormalize(&v, &v);

        if (self->state != 2) {
            face.x = (f32)sin(3.141592653589793 * self->rot.y / 180.0);
            face.y = 0.0f;
            face.z = (f32)cos(3.141592653589793 * self->rot.y / 180.0);
        } else {
            PSVECSubtract(&self->pos, &grp->pos, &face);
            face.y = 0.0f;
            PSVECNormalize(&face, &face);
        }

        dot = PSVECDotProduct(&v, &face);
        theta = (f32)acos(dot);
        ang = (f32)(180.0 * (theta / 3.141592653589793));

        if (ang > bestAng) {
            bestAng = ang;
            bestIdx = i;
        }
    }

    *outPos = pts[bestIdx];
    return 1;
}
