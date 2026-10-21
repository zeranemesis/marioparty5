#include "types.h"

typedef s16 HU3DMODELID;

void Hu3DModelPosSet(HU3DMODELID modelId, f32 posX, f32 posY, f32 posZ);
void Hu3DModelRotSet(HU3DMODELID modelId, f32 rotX, f32 rotY, f32 rotZ);
void Hu3DModelScaleSet(HU3DMODELID modelId, f32 scaleX, f32 scaleY, f32 scaleZ);
void Hu3DModelAttrSet(HU3DMODELID modelId, s32 a1);
void Hu3DModelAttrReset(HU3DMODELID modelId, s32 a1);
s32 rand8(void);

s32 MgTimerCreate(s32 mode);
void MgTimerParamSet(s32 timerId, s32 param, s32 a2, s32 a3);
void MgTimerPosSet(s32 timerId, f32 x, f32 y);
void MgTimerModeOnSet(s32 timerId, s32 mode);

double cos(double x);
double sin(double x);

s16 GameMesCreate(s32 a0, s32 a1, ...);
u32 GameMesStatGet(s16 mesId);
void GameMesKill(s16 mesId);
void HuAudJinglePlay(s32 jingleNo);
void CharFXPlayPan(s16 charNo, s32 pan, s32 volume);
void MgSeqModeNext(void);
void omCameraViewMoveSimpleMulti(s32 mode, void *p1, s32 a2);
s32 HuAudFXPlay(s32 fxNo);
void HuAudFXPanning(s32 handle, s16 pan);
s32 HuAudFXPitchSet(s32 handle, s16 pitch);
s32 MgTimerValueGet(s32 timerId);
s32 MgTimerDoneCheck(s32 timerId);
void Hu3DCameraPosGet(s32 camNo, void *pos, void *up, void *target);
void fn_1_2648(s32 i);
s32 HuAudBGMPlay(s32 bgmNo);
u32 WipeCheckEnd(void);
void Hu3DTexScrollPosSet(s16 id, f32 a, f32 b, f32 c);
void GWMgRecordSet(s32 a, s32 b);
s32 _CheckFlag(s32 flag);
void Hu3DModelCameraSet(HU3DMODELID modelId, s32 a1);
void Hu3DModelLayerSet(HU3DMODELID modelId, s32 a1);
void PSVECSubtract(void *dst, void *a, void *b);
void PSVECNormalize(void *v, void *w);
void HuAudFXListnerSet(void *a, void *b, f32 c, f32 d);
s32 HuAudFXEmiterPlay(s32 fxNo, void *slot);
void HuAudFXEmiterUpDate(s32 handle, void *slot);
void CharFXPlay(s16 charNo, s32 a1);
void Hu3DMotionShiftSet(HU3DMODELID modelId, s16 motId, f32 start, f32 end, u32 attr);
void Hu3DMotionSpeedSet(HU3DMODELID modelId, f32 speed);
void Hu3DCameraPosSetV(s32 camNo, void *pos, void *target, void *up);
void HuAudSStreamFadeOut(s32 handle, s32 fadeTime);

typedef struct M547Vec2_s {
    f32 x;
    f32 y;
} M547Vec2;

#define M547_RATE 1.05

extern u8 lbl_1_bss_38[];

typedef struct M547ModelProbe_s {
    u8 pad_00[0x16C];
    s16 probe;
} M547ModelProbe;

/* s16 read at element + 0x2F4 through the model pointer */
typedef struct M547ModelIdProbe_s {
    u8 pad_00[0x2F4];
    s16 id;
} M547ModelIdProbe;

/* the three s16 fields of the model array this unit reads through a cast */
typedef struct M547ModelProbes_s {
    u8 pad_00[0x16C];
    s16 id16C;
    u8 pad_172[0x36];
    s16 id1A4;
    u8 pad_1A6[0xDE];
    s16 id284;
} M547ModelProbes;

typedef union M547F32S16_s {
    f32 f;
    s16 s;
} M547F32S16;

/* model entry reached through M547GameState.models, 0x1C bytes */
typedef struct M547ModelEntry_s {
    s16 id;
    s16 pad_02;
    f32 x;
    M547F32S16 y;
    f32 z;
    f32 rotX;
    f32 rotY;
    f32 rotZ;
} M547ModelEntry;

/* per-character effect slot, 0x24 bytes */
typedef struct M547FxSlot_s {
    s16 kind;
    s16 state;
    f32 fade;
    u8 pad_08[0x18];
    s32 flags;
} M547FxSlot;

/* motion table reached through M547GameState.motion, s16 fields at 0, 2, 6 and 0xA */
typedef struct M547Motion_s {
    s16 id;
    s16 field02;
    u8 pad_04[2];
    s16 field06;
    s16 field08;
    s16 fieldA;
} M547Motion;

/* 0xC stride slot at +0xC0 of M547GameState */
typedef struct M547Slot_s {
    f32 f0;
    f32 f1;
    f32 f2;
} M547Slot;

/* M547GameState is defined in the parent object (lbl_1_bss_488 is undefined here) */
typedef struct M547GameState_s {
    u8 pad_00[6];
    s16 fxId0;
    u8 pad_08[0x1A];
    s16 fxId1;
    u8 pad_24[0x18];
    M547ModelEntry *models;
    u8 pad_40[8];
    M547Motion *motion[2];
    M547FxSlot fxSlots[2];
    u8 pad_98[8];
    s32 counterA0;
    s32 counterA4;
    s32 fieldA8;
    s32 fieldAC;
    s32 counters[4];
    M547Slot slots[3];
    u8 pad_E4[8];
    f32 fieldEC;
} M547GameState;

extern M547GameState lbl_1_bss_488;
extern f32 lbl_1_bss_458[4];

extern f32 lbl_1_bss_464[4];
extern s32 lbl_1_bss_78;

/* 0x1C-stride entry reached as ((T *)&lbl_1_bss_488)[i] */
typedef struct M547IdxEntry_s {
    u8 pad_00[4];
    s16 a;
    s16 b;
    u8 pad_08[0x14];
} M547IdxEntry;

/* GwPlayer entry, 0x108 stride */
typedef struct M547GwPlayer_s {
    u8 pad_00[0x2C];
    s16 field2C;
    u8 pad_2E[0xDA];
} M547GwPlayer;

extern M547GwPlayer GwPlayer[];
extern s32 lbl_1_bss_430[4];

/* 0xC-stride entry of lbl_1_bss_440 */
typedef struct M547Vec3_s {
    f32 f0;
    f32 f1;
    f32 f2;
} M547Vec3;

extern M547Vec3 lbl_1_bss_440[2];

/* unit-owned .bss, declared in reverse so .bss offsets match the original */
typedef struct M547Fx4_s {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} M547Fx4;

/* Unit-owned BSS, in reverse declaration order to preserve original offsets. */
s32 lbl_1_bss_6D4[5];
s32 lbl_1_bss_6C4[4];
s16 lbl_1_bss_6BC[4];
f32 lbl_1_bss_6AC[4];
s32 lbl_1_bss_6A8;
s16 lbl_1_bss_6A4[1];
s32 lbl_1_bss_6A0;
f32 lbl_1_bss_69C;
f32 lbl_1_bss_698;

/* Unit-owned .data, in original address order (0x280..0x350). */
f32 lbl_1_data_280[3] = {0.0f, 200.0f, 200.0f};
f32 lbl_1_data_28C[3] = {0.0f, 1.0f, 0.0f};
f32 lbl_1_data_298[3] = {0.0f, 200.0f, 0.0f};
s32 lbl_1_data_2A4[2] = {-1, -1};
s32 lbl_1_data_2AC[4] = {0, 0, 0, 0};
f32 lbl_1_data_2BC[26] = {
    0.0f, 0.0f, 0.25f, 0.0f, 0.5f, 0.0f, 0.75f, 0.0f,
    0.0f, 0.25f, 0.25f, 0.25f, 0.5f, 0.25f, 0.75f, 0.25f,
    0.0f, 0.5f, 0.25f, 0.5f, 0.25f, 0.5f, 0.25f, 0.5f,
    0.25f, 0.5f,
};
s32 lbl_1_data_324 = -1;
s32 lbl_1_data_328[2] = {0, 0};
s16 lbl_1_data_330[2] = {0, 0};
s32 lbl_1_data_334[2] = {0, 0};
f32 lbl_1_data_33C = 5.0f;
s32 lbl_1_data_340 = -1;
f32 lbl_1_data_344[3] = {0.0f, 0.0f, 0.0f};

extern s16 lbl_1_bss_480;
extern s16 lbl_1_bss_2;
extern s32 lbl_1_bss_24[2];
extern s32 lbl_1_bss_34[1];
extern void *lbl_1_bss_484;

void fn_1_4094(void)
{
    s16 i;

    for (i = 1; i < 9; i++) {
        Hu3DModelPosSet(lbl_1_bss_488.models[i].id, lbl_1_bss_488.models[i].x,
                        lbl_1_bss_488.models[i].y.f, lbl_1_bss_488.models[i].z);
        Hu3DModelRotSet(lbl_1_bss_488.models[i].id, lbl_1_bss_488.models[i].rotX,
                        lbl_1_bss_488.models[i].rotY, lbl_1_bss_488.models[i].rotZ);
    }
}

void fn_1_41B0(void)
{
    s32 i;
    s32 j;

    lbl_1_bss_488.counters[0]++;
    for (i = 0; i < 2; i++) {
        lbl_1_bss_488.slots[i].f1 = 800.0f - 600.0f * lbl_1_bss_488.counters[0] / 30.0f;
        if (lbl_1_bss_488.counters[0] == 30) {
            lbl_1_bss_488.slots[i].f1 = 200.0f;
            lbl_1_bss_488.fxSlots[i].fade = 1.0f;
            if (i != 0) {
                lbl_1_bss_488.counterA0++;
                lbl_1_bss_488.counterA4 = 0;
                lbl_1_bss_488.counters[0] = 0;
                omCameraViewMoveSimpleMulti(1, &lbl_1_bss_78, 0x3c);
                HuAudFXPlay(0x835);
                for (j = 0; j < 4; j++) {
                    lbl_1_bss_6C4[j] = 0;
                    lbl_1_bss_6D4[j] = 0;
                }
            }
        }
    }
    if ((WipeCheckEnd() & 0xFF) == 0) {
        if (lbl_1_data_324 < 0) {
            lbl_1_data_324 = HuAudBGMPlay(0x3b);
        }
    }
}

void fn_1_43A0(void)
{
    s32 i;

    lbl_1_bss_488.counters[0]++;
    for (i = 0; i < 2; i++) {
        if (lbl_1_bss_488.counters[0] <= 15) {
            lbl_1_bss_488.slots[i].f1 =
                180.0 + 20.0 *
                cos(3.141592653589793 * (lbl_1_bss_488.counters[0] * 360 / 15) / 180.0);
        } else if (lbl_1_bss_488.counters[0] <= 30) {
            lbl_1_bss_488.slots[i].f1 =
                185.0 + 15.0 *
                cos(3.141592653589793 * ((lbl_1_bss_488.counters[0] - 15) * 360 / 15) / 180.0);
        } else if (lbl_1_bss_488.counters[0] <= 45) {
            lbl_1_bss_488.slots[i].f1 =
                195.0 + 5.0 *
                cos(3.141592653589793 * ((lbl_1_bss_488.counters[0] - 30) * 360 / 15) / 180.0);
        } else if (lbl_1_bss_488.counters[0] <= 60) {
            lbl_1_bss_488.slots[i].f1 =
                198.0 + 2.0 *
                cos(3.141592653589793 * ((lbl_1_bss_488.counters[0] - 45) * 360 / 15) / 180.0);
        }
        if (lbl_1_bss_488.counters[0] == 60) {
            Hu3DMotionShiftSet(lbl_1_bss_488.models[i + 13].id,
                               lbl_1_bss_488.motion[i]->id, 0.0f, 20.0f, 0x40000001);
            if (i != 0) {
                lbl_1_bss_488.counterA0 = 0;
                lbl_1_bss_488.counterA4 = 0;
                lbl_1_bss_488.counters[0] = 0;
                MgSeqModeNext();
            }
        }
    }
    if ((WipeCheckEnd() & 0xFF) == 0) {
        if (lbl_1_data_324 < 0) {
            lbl_1_data_324 = HuAudBGMPlay(0x3b);
        }
    }
}

void fn_1_47E0(void)
{
    lbl_1_bss_6A8 = MgTimerCreate(0);
    MgTimerParamSet(lbl_1_bss_6A8, 600, 0, 0);
    MgTimerPosSet(lbl_1_bss_6A8, 288.0f, 416.0f);
    MgTimerModeOnSet(lbl_1_bss_6A8, 1);
    lbl_1_bss_488.counters[0] = 1;
    lbl_1_bss_488.counterA0++;
    lbl_1_bss_6BC[0] = 0;
    lbl_1_bss_6BC[1] = 0;
    lbl_1_bss_6AC[0] = 0.0f;
    lbl_1_bss_6AC[1] = 3.0f;
    lbl_1_bss_6AC[2] = 600.0f;
    lbl_1_bss_6AC[3] = 200.0f;
}

void fn_1_490C(void)
{
    lbl_1_bss_6AC[0] += lbl_1_bss_6AC[1];
    lbl_1_bss_6AC[1] *= 0.985;
    lbl_1_bss_6AC[2] *= 0.985;
    lbl_1_bss_6AC[3] *= 0.985;
    lbl_1_bss_464[0] = -100.0 + lbl_1_bss_6AC[2] * cos(3.141592653589793 * lbl_1_bss_6AC[0] / 180.0);
    lbl_1_bss_464[1] = 300.0 - lbl_1_bss_6AC[3] * sin(3.141592653589793 * lbl_1_bss_6AC[0] / 180.0);
    lbl_1_bss_464[2] = -500.0f;
}

void fn_1_4A94(void)
{
    s32 i;
    s32 n;
    s32 handle;
    s32 step;

    for (i = 0; i < 2; i++) {
        if (lbl_1_bss_488.counters[i + 1] == 1) {
            lbl_1_bss_488.counters[i + 1] = 2;
            Hu3DMotionShiftSet(((M547ModelProbes *)&lbl_1_bss_488.models[i])->id16C,
                               lbl_1_bss_488.motion[i]->field06, 0.0f, 20.0f, 0x40000001);
        }
        if (((M547ModelEntry *)&lbl_1_bss_488)[i].y.s != 0) {
            continue;
        }
        if (lbl_1_bss_488.counters[i + 1] < 2) {
            continue;
        }
        if (lbl_1_data_2AC[i * 2 + 1] < 0) {
            lbl_1_data_2AC[i * 2 + 1]--;
        }
        if (lbl_1_bss_488.counters[i + 1] == lbl_1_data_2AC[i * 2]) {
            if (lbl_1_data_2AC[i * 2 + 1] >= 0) {
                lbl_1_data_2AC[i * 2 + 1]++;
            }
            if (lbl_1_data_2AC[i * 2 + 1] < 30) {
                continue;
            }
            if (MgTimerValueGet(lbl_1_bss_6A8) < 45) {
                continue;
            }
            Hu3DMotionShiftSet(((M547ModelProbes *)&lbl_1_bss_488.models[i])->id16C,
                               lbl_1_bss_488.motion[i]->id, 0.0f, 20.0f, 0x40000001);
            lbl_1_data_2AC[i * 2 + 1] = -1;
            lbl_1_data_2AC[i * 2] = lbl_1_bss_488.counters[i + 1];
        } else {
            if (lbl_1_data_2AC[i * 2 + 1] >= 0) {
                lbl_1_data_2AC[i * 2] = lbl_1_bss_488.counters[i + 1];
                lbl_1_data_2AC[i * 2 + 1] = 0;
            } else if (lbl_1_data_2AC[i * 2 + 1] < -20) {
                Hu3DMotionShiftSet(((M547ModelProbes *)&lbl_1_bss_488.models[i])->id16C,
                                   lbl_1_bss_488.motion[i]->field06, 0.0f, 20.0f, 0x40000001);
                lbl_1_data_2AC[i * 2] = lbl_1_bss_488.counters[i + 1];
                lbl_1_data_2AC[i * 2 + 1] = 0;
            }
        }
    }
    for (i = 0; i < 2; i++) {
        if (lbl_1_bss_488.counters[i + 1] >= 80) {
            lbl_1_bss_488.slots[i].f1 = 150.0f;
            lbl_1_data_330[i] = 0x2000;
            step = 3;
        } else {
            lbl_1_bss_488.slots[i].f1 = 200 - lbl_1_bss_488.counters[i + 1] * 50 / 80;
            lbl_1_data_330[i] = (lbl_1_bss_488.counters[i + 1] << 13) / 80;
            step = 3;
        }
        if (lbl_1_bss_488.counters[i + 1] - lbl_1_data_328[i] >= step) {
            lbl_1_data_328[i] = lbl_1_bss_488.counters[i + 1];
            handle = HuAudFXPlay(0x83a);
            HuAudFXPanning(handle, i * 32 + 0x30);
            HuAudFXPitchSet(handle, lbl_1_data_330[i]);
            lbl_1_data_334[i] = 0;
        } else {
            lbl_1_data_334[i]++;
            if (((M547ModelEntry *)&lbl_1_bss_488)[i].y.s != 0) {
                if (lbl_1_data_334[i] >= 15) {
                    lbl_1_data_328[i] = lbl_1_bss_488.counters[i + 1];
                    handle = HuAudFXPlay(0x83a);
                    HuAudFXPanning(handle, i * 32 + 0x30);
                    HuAudFXPitchSet(handle, lbl_1_data_330[i]);
                    lbl_1_data_334[i] = 0;
                }
            }
        }
    }
    if (lbl_1_bss_6BC[0] < 180) {
        lbl_1_bss_6BC[0]++;
    } else {
        lbl_1_bss_6AC[0] += lbl_1_bss_6AC[1];
        lbl_1_bss_6AC[1] *= 0.985;
        lbl_1_bss_6AC[2] *= 0.985;
        lbl_1_bss_6AC[3] *= 0.985;
        lbl_1_bss_464[0] = -100.0 + lbl_1_bss_6AC[2] * cos(3.141592653589793 * lbl_1_bss_6AC[0] / 180.0);
        lbl_1_bss_464[1] = 300.0 - lbl_1_bss_6AC[3] * sin(3.141592653589793 * lbl_1_bss_6AC[0] / 180.0);
        lbl_1_bss_464[2] = -500.0f;
    }
    if (MgTimerDoneCheck(lbl_1_bss_6A8) != 0) {
        for (i = 0; i < 2; i++) {
            if (lbl_1_bss_488.counters[i + 1] != 0) {
                lbl_1_bss_488.counters[i + 1]--;
            }
        }
        for (i = 0, n = 0; i < 2; i++) {
            if (((M547ModelEntry *)&lbl_1_bss_488)[i].y.s != 0) {
                fn_1_2648(i);
            }
        }
        lbl_1_bss_488.counters[0] = 0;
        if (lbl_1_bss_488.counters[1] + lbl_1_bss_488.counters[1] != 0) {
            if (lbl_1_bss_488.counters[1] == lbl_1_bss_488.counters[2]) {
                lbl_1_bss_488.counters[rand8() % 2 + 1]--;
            }
        }
        lbl_1_bss_488.counterA0++;
        lbl_1_bss_488.counterA4 = 0;
        lbl_1_bss_488.fieldEC = 30.0f;
        for (i = 0, n = 0; i < 2; i++) {
            if (lbl_1_bss_488.counters[i + 1] != 0) {
                lbl_1_bss_488.slots[i + 2].f1 = 10000.0f;
            } else {
                lbl_1_bss_488.slots[i + 2].f1 = 0.0f;
                n++;
            }
            Hu3DMotionShiftSet(((M547ModelProbes *)&lbl_1_bss_488.models[i])->id16C,
                               lbl_1_bss_488.motion[i]->field02, 0.0f, 10.0f, 0);
            lbl_1_bss_488.fxSlots[i].flags = 0x40000001;
        }
        HuAudSStreamFadeOut(lbl_1_data_324, 100);
        if (n == 2) {
            lbl_1_bss_488.counterA0 = 4;
            lbl_1_bss_488.counterA4 = 0;
            lbl_1_bss_488.counters[1] = 0;
            lbl_1_bss_488.counters[2] = 0;
        } else {
            HuAudFXPlay(0x836);
            lbl_1_bss_24[0] = 1;
            *(s32 *)((u8 *)lbl_1_bss_484 + 0x4C) = 0;
            Hu3DCameraPosGet(1, &lbl_1_bss_38[0], &lbl_1_bss_38[0xC], &lbl_1_bss_38[0x18]);
            for (i = 0; i < 4; i++) {
                lbl_1_bss_6BC[i] = 0;
                lbl_1_bss_6AC[i] = 0.0f;
            }
            lbl_1_bss_6AC[1] = 9.0f;
            if (n == 1) {
                if (lbl_1_bss_488.counters[1] != 0) {
                    lbl_1_bss_488.slots[3].f1 = 10000.0f;
                } else {
                    lbl_1_bss_488.slots[2].f1 = 10000.0f;
                }
            }
        }
    }
}

void fn_1_56D0(void)
{
    lbl_1_bss_6AC[0] += lbl_1_bss_6AC[1];
    if (lbl_1_bss_6AC[0] > 180.0f) {
        lbl_1_bss_6AC[0] = 180.0f;
    }
    lbl_1_bss_464[0] = -100.0 + 500.0 * sin(3.141592653589793 * lbl_1_bss_6AC[0] / 180.0);
    if (lbl_1_bss_6AC[0] >= 90.0f) {
        if (lbl_1_bss_464[0] < 0.0f) {
            lbl_1_bss_464[0] = 0.0f;
        }
    }
    lbl_1_bss_464[1] += lbl_1_bss_488.fieldEC;
    lbl_1_bss_464[2] = -500.0 * cos(3.141592653589793 * lbl_1_bss_6AC[0] / 180.0);
    lbl_1_bss_458[1] = -lbl_1_bss_6AC[0];
}

void fn_1_5894(void)
{
    s32 i = 0;
    s32 cnt = 0;

    for (; i < 2; i++) {
        if (lbl_1_bss_488.slots[i + 2].f1 > lbl_1_bss_488.slots[i].f1 + lbl_1_bss_488.fieldEC) {
            lbl_1_bss_488.slots[i].f1 += lbl_1_bss_488.fieldEC;
        } else {
            lbl_1_bss_488.slots[i].f1 = lbl_1_bss_488.slots[i + 2].f1;
            cnt++;
        }
    }
    lbl_1_bss_488.fieldEC *= 1.06;
    lbl_1_bss_6AC[0] += lbl_1_bss_6AC[1];
    if (lbl_1_bss_6AC[0] > 180.0f) {
        lbl_1_bss_6AC[0] = 180.0f;
    }
    lbl_1_bss_464[0] = -100.0 + 500.0 * sin(3.141592653589793 * lbl_1_bss_6AC[0] / 180.0);
    if (lbl_1_bss_6AC[0] >= 90.0f) {
        if (lbl_1_bss_464[0] < 0.0f) {
            lbl_1_bss_464[0] = 0.0f;
        }
    }
    lbl_1_bss_464[1] += lbl_1_bss_488.fieldEC;
    lbl_1_bss_464[2] = -500.0 * cos(3.141592653589793 * lbl_1_bss_6AC[0] / 180.0);
    lbl_1_bss_458[1] = -lbl_1_bss_6AC[0];
    if (lbl_1_bss_488.counterA4++ >= 5) {
        if (*(f32 *)&lbl_1_bss_38[0x1c] < 4000.0f) {
            *(f32 *)&lbl_1_bss_38[0x1c] += 25.0f;
        }
    }
    if (lbl_1_bss_488.counterA4 >= 10) {
        *(f32 *)&lbl_1_bss_38[0x8] -= 20.0f;
        if (*(f32 *)&lbl_1_bss_38[0x8] < 0.0f) {
            *(f32 *)&lbl_1_bss_38[0x8] = 0.1f;
        }
        *(f32 *)&lbl_1_bss_38[0x4] -= 4.0f;
        if (*(f32 *)&lbl_1_bss_38[0x4] < 250.0f) {
            *(f32 *)&lbl_1_bss_38[0x4] = 250.0f;
        }
    }
    *(f32 *)&lbl_1_bss_38[0x10] = *(f32 *)&lbl_1_bss_38[0x1c];
    *(f32 *)&lbl_1_bss_38[0x14] = *(f32 *)&lbl_1_bss_38[0x20];
    Hu3DCameraPosSetV(1, &lbl_1_bss_38[0], &lbl_1_bss_38[0xc], &lbl_1_bss_38[0x18]);
    if (cnt == 2) {
        memcpy(&lbl_1_bss_488.slots[2], &lbl_1_bss_488.slots[0], 0xC);
        memcpy(&lbl_1_bss_488.slots[3], &lbl_1_bss_488.slots[1], 0xC);
        lbl_1_bss_488.counterA0++;
        lbl_1_bss_488.counterA4 = 0;
        Hu3DModelAttrSet(((M547ModelProbe *)&lbl_1_bss_488.models[0])->probe, 1);
        Hu3DModelAttrSet(lbl_1_bss_488.models[14].id, 1);
        Hu3DCameraPosSetV(2, &lbl_1_data_280[0], &lbl_1_data_28C[0], &lbl_1_data_298[0]);
        lbl_1_bss_488.counters[3] = 0;
        lbl_1_bss_488.fieldA8 = 0;
        lbl_1_bss_488.fieldAC = 3;
        HuAudFXPlay(0x839);
        lbl_1_data_324 = HuAudBGMPlay(0x4b);
    }
}

void fn_1_670C(s32 i);
void fn_1_6A88(s32 i);

void fn_1_5DC4(void)
{
    s32 i;

    if (lbl_1_bss_2 < 100) {
        lbl_1_bss_2++;
    }

    switch (lbl_1_bss_488.counterA4) {
    case 0:
        if (lbl_1_bss_2 == 10) {
            for (i = 0; i < 2; i++) {
                Hu3DModelCameraSet(*(s16 *)((u8 *)lbl_1_bss_488.models + i * 0x1c + 0x16c), 2);
                Hu3DModelAttrReset(*(s16 *)((u8 *)lbl_1_bss_488.models + i * 0x1c + 0x16c), 1);
                lbl_1_bss_488.slots[i].f0 = (float)(i * 240 - 120);
                lbl_1_bss_488.slots[i].f1 = 80.0f;
                lbl_1_bss_488.slots[i].f2 = -400.0f;
            }
            lbl_1_bss_488.counterA4++;
        }
        break;
    case 1:
        lbl_1_bss_488.counters[3]++;
        if (lbl_1_bss_488.counters[3] % 4 == 0) {
            for (i = 0; i < 2; i++) {
                lbl_1_bss_488.counters[i + 1]--;
                if (lbl_1_bss_488.counters[i + 1] < 0) {
                    lbl_1_bss_488.fieldAC -= (i + 1);
                    lbl_1_bss_488.counterA4++;
                    lbl_1_data_2A4[1] = i;
                    Hu3DMotionShiftSet(*(s16 *)((u8 *)lbl_1_bss_488.models + i * 0x1c + 0x16c),
                                       lbl_1_bss_488.motion[i]->field08,
                                       0.0f, 10.0f, 0x40000001);
                }
            }
        }
        break;
    case 2:
        lbl_1_bss_488.counters[3]++;
        if (lbl_1_bss_488.counters[3] % 4 == 0) {
            for (i = 0; i < 2; i++) {
                if (lbl_1_bss_488.counters[i + 1] >= 0) {
                    lbl_1_bss_488.counters[i + 1]--;
                    if (lbl_1_bss_488.counters[i + 1] <= 0) {
                        lbl_1_bss_488.fieldAC -= (i + 1);
                    lbl_1_data_2A4[0] = i;
                    Hu3DTexScrollPosSet(*(s16 *)((u8 *)&lbl_1_bss_480 + 2),
                                        ((M547Vec2 *)lbl_1_data_2BC)[*(s16 *)((u8 *)&lbl_1_bss_488 + i * 0x1c + 6)].x,
                                        ((M547Vec2 *)lbl_1_data_2BC)[*(s16 *)((u8 *)&lbl_1_bss_488 + i * 0x1c + 6)].y,
                                        0.0f);
                    lbl_1_bss_488.fieldA8 = 1;
                    lbl_1_bss_488.counterA4++;
                    ((s32 *)&lbl_1_bss_24)[3] = 1;
                    }
                }
            }
        }
        break;
    case 3:
        break;
    default:
        break;
    }

    if (lbl_1_bss_488.fieldA8 == 0) {
        f32 f31 = (float)(lbl_1_bss_488.counters[3] % 25);
        f31 = 0.5f - (0.5f * f31) / 25.0f;
        Hu3DTexScrollPosSet(lbl_1_bss_480, 0.0f, f31, 0.0f);
    }

    for (i = 0; i < 2; i++) {
        if (i != lbl_1_data_2A4[1] && lbl_1_bss_488.slots[i + 2].f1 < 23000.0f) {
            lbl_1_bss_488.slots[i + 2].f1 += 80.0f;
        }
    }

    if (lbl_1_bss_488.counterA4 < 3 && lbl_1_bss_464[1] < 23000.0f) {
        lbl_1_bss_464[1] += 80.0f;
    }

    if (lbl_1_bss_488.fieldAC & 1) {
        lbl_1_bss_488.slots[0].f1 = (float)(80.0 + 15.0 * sin(3.141592653589793 * (double)(((lbl_1_bss_488.counters[3] % 80) * 360) / 80) / 180.0));
        lbl_1_bss_488.slots[0].f2 = (float)(-400.0 + 35.0 * sin(3.141592653589793 * (double)(((lbl_1_bss_488.counters[3] % 160) * 360) / 160) / 180.0));
    }
    if (lbl_1_bss_488.fieldAC & 2) {
        lbl_1_bss_488.slots[1].f1 = (float)(80.0 - 15.0 * sin(3.141592653589793 * (double)(((lbl_1_bss_488.counters[3] % 80) * 360) / 80) / 180.0));
        lbl_1_bss_488.slots[1].f2 = (float)(-400.0 - 35.0 * sin(3.141592653589793 * (double)(((lbl_1_bss_488.counters[3] % 160) * 360) / 160) / 180.0));
    }

    for (i = 0; i < 2; i++) {
        if (lbl_1_data_2A4[i] >= 0) {
            if (i != 0) {
                fn_1_670C(lbl_1_data_2A4[1]);
            } else {
                fn_1_6A88(lbl_1_data_2A4[0]);
            }
        }
    }
}

void fn_1_670C(s32 i)
{
    M547Vec2 v;
    f32 t;

    switch (lbl_1_bss_6D4[0]) {
    case 0:
        lbl_1_bss_488.slots[i].f1 -= lbl_1_data_33C;
        lbl_1_data_33C *= M547_RATE;
        if (lbl_1_bss_488.slots[i].f1 < -250.0f) {
            lbl_1_bss_6D4[0]++;
            t = (lbl_1_bss_488.slots[i + 2].f1 - 11040.0f) / 12000.0f;
            Hu3DModelCameraSet(((M547ModelProbe *)&lbl_1_bss_488.models[i])->probe, 1);
            Hu3DModelLayerSet(((M547ModelProbe *)&lbl_1_bss_488.models[i])->probe, 6);
            memcpy(&lbl_1_bss_488.slots[i], &lbl_1_bss_488.slots[i + 2], 0xC);
            lbl_1_bss_6D4[1] = 0;
            lbl_1_data_344[0] = 0.0f;
            lbl_1_data_344[1] = 0.5 + 0.5 * t;
            lbl_1_data_344[2] = 75.0f + 75.0f * t;
            PSVECSubtract(&lbl_1_bss_38[0x18], &lbl_1_bss_38[0], &v);
            PSVECNormalize(&v, &v);
            HuAudFXListnerSet(&lbl_1_bss_38[0], &v, 30000.0f, 1000.0f);
            lbl_1_data_340 = HuAudFXEmiterPlay(0x837, &lbl_1_bss_488.slots[i]);
        }
        break;
    case 1:
        if (lbl_1_bss_488.slots[i].f1 > -2000.0f) {
            lbl_1_data_344[0] = lbl_1_data_344[0] + lbl_1_data_344[1];
            lbl_1_bss_488.slots[i].f1 -= lbl_1_data_344[2];
            lbl_1_bss_488.slots[i].f2 = lbl_1_data_344[0];
            HuAudFXEmiterUpDate(lbl_1_data_340, &lbl_1_bss_488.slots[i]);
        } else {
            lbl_1_bss_6D4[0]++;
        }
        break;
    }
}

void fn_1_6A88(s32 i)
{
    f32 t;

    switch (lbl_1_bss_6C4[0]) {
    case 0:
        Hu3DModelAttrReset(lbl_1_bss_488.models[22].id, 1);
        lbl_1_bss_6C4[0]++;
        lbl_1_bss_698 = lbl_1_bss_488.counters[3] % 25;
        lbl_1_bss_698 += 1.0f;
        lbl_1_bss_69C = 25.0f;
        lbl_1_bss_6C4[1] = 0;
        lbl_1_bss_6C4[2] = 0;
        break;
    case 1:
        lbl_1_bss_6A0++;
        lbl_1_bss_698 += cos(3.141592653589793 * (lbl_1_bss_6A0 * 90 / 60) / 180.0);
        if (lbl_1_bss_6A0 >= 60) {
            Hu3DMotionSpeedSet(lbl_1_bss_488.models[22].id, 2.0f);
            Hu3DMotionShiftSet(((M547ModelProbe *)&lbl_1_bss_488.models[i])->probe,
                               lbl_1_bss_488.motion[i]->fieldA, 0.0f, 10.0f, 0x40000001);
            lbl_1_bss_6C4[0]++;
            HuAudSStreamFadeOut(lbl_1_data_324, 0x64);
        }
        break;
    case 2:
        lbl_1_bss_698 -= 0.046875004f;
        if (lbl_1_bss_698 < 0.0f) {
            lbl_1_bss_698 += lbl_1_bss_69C;
        }
        if (lbl_1_bss_6C4[2]++ == 25) {
            Hu3DMotionSpeedSet(lbl_1_bss_488.models[22].id, 6.0f);
            HuAudFXPlay(0x838);
        }
        if (lbl_1_bss_6D4[0] >= 2) {
            lbl_1_bss_6C4[1]++;
            if (lbl_1_bss_6C4[1] >= 100) {
                MgSeqModeNext();
            }
        }
        break;
    }
    lbl_1_bss_488.counters[3]++;
    if (lbl_1_bss_698 >= lbl_1_bss_69C) {
        lbl_1_bss_698 -= lbl_1_bss_69C;
    }
    t = 0.5f - 0.5f * lbl_1_bss_698 / lbl_1_bss_69C;
    Hu3DTexScrollPosSet(lbl_1_bss_480, 0.0f, t, 0.0f);
    lbl_1_bss_488.slots[i].f2 =
        -400.0 + (35 - i * 70) *
        sin(3.141592653589793 * (lbl_1_bss_488.counters[3] % 160 * 360 / 160) / 180.0);
}

void fn_1_6FF4(void)
{
    s32 i;

    switch (lbl_1_bss_488.counterA4) {
    case 0:
        for (i = 0; i < 2; i++) {
            Hu3DMotionSpeedSet(((M547ModelProbes *)&lbl_1_bss_488.models[i])->id284, 1.0f);
            Hu3DModelAttrReset(((M547ModelProbes *)&lbl_1_bss_488.models[i])->id284, 1);
        }
        lbl_1_bss_488.counterA4++;
        break;
    case 1:
        lbl_1_bss_488.counters[0]++;
        lbl_1_bss_488.slots[0].f1 +=
            20.0 * cos(3.141592653589793 * (lbl_1_bss_488.counters[0] * 90 / 12) / 180.0);
        lbl_1_bss_488.slots[1].f1 +=
            20.0 * cos(3.141592653589793 * (lbl_1_bss_488.counters[0] * 90 / 12) / 180.0);
        if (lbl_1_bss_488.counters[0] == 12) {
            for (i = 0; i < 2; i++) {
                Hu3DModelAttrSet(((M547ModelProbes *)&lbl_1_bss_488.models[i])->id1A4, 1);
            }
            lbl_1_bss_488.counterA4++;
        }
        break;
    case 2:
        lbl_1_bss_488.counters[0]++;
        if (lbl_1_bss_488.counters[0] > 24) {
            lbl_1_bss_488.counters[0] = 24;
        }
        lbl_1_bss_488.slots[0].f1 +=
            20.0 * cos(3.141592653589793 * (lbl_1_bss_488.counters[0] * 90 / 12) / 180.0);
        lbl_1_bss_488.slots[1].f1 +=
            20.0 * cos(3.141592653589793 * (lbl_1_bss_488.counters[0] * 90 / 12) / 180.0);
        if (lbl_1_bss_488.slots[0].f1 < 100.0f) {
            for (i = 0; i < 2; i++) {
                lbl_1_bss_488.slots[i].f1 = 100.0f;
                lbl_1_bss_488.fxSlots[i].kind = 2;
                lbl_1_bss_488.fxSlots[i].state = 0;
            }
            lbl_1_bss_488.counters[0] = 0;
            lbl_1_bss_488.counterA4++;
        }
        break;
    case 3:
        lbl_1_bss_488.counters[0]++;
        if (lbl_1_bss_488.counters[0] < 24) {
            break;
        }
        for (i = 0; i < 2; i++) {
            Hu3DMotionShiftSet(((M547ModelProbes *)&lbl_1_bss_488.models[i])->id16C,
                               lbl_1_bss_488.motion[i]->id, 0.0f, 30.0f, 0x40000001);
        }
        MgSeqModeNext();
        break;
    }
}

void fn_1_750C(void)
{
    f32 t;

    lbl_1_bss_698 = lbl_1_bss_698 - 0.046875004f;
    if (lbl_1_bss_698 < 0.0f) {
        lbl_1_bss_698 = lbl_1_bss_698 + lbl_1_bss_69C;
    }
    t = 0.5f - 0.5f * lbl_1_bss_698 / lbl_1_bss_69C;
    Hu3DTexScrollPosSet(lbl_1_bss_480, 0.0f, t, 0.0f);
    lbl_1_bss_488.counters[3]++;
    lbl_1_bss_488.slots[lbl_1_data_2A4[0]].f2 =
        -400.0 + (35 - lbl_1_data_2A4[0] * 70) *
        sin(3.141592653589793 * (lbl_1_bss_488.counters[3] % 160 * 360 / 160) / 180.0);
}

void fn_1_7724(void)
{
    f32 t;

    if (lbl_1_data_2A4[0] >= 0) {
        lbl_1_bss_698 = lbl_1_bss_698 - 0.046875004f;
        if (lbl_1_bss_698 < 0.0f) {
            lbl_1_bss_698 = lbl_1_bss_698 + lbl_1_bss_69C;
        }
        t = 0.5f - 0.5f * lbl_1_bss_698 / lbl_1_bss_69C;
        Hu3DTexScrollPosSet(lbl_1_bss_480, 0.0f, t, 0.0f);
        lbl_1_bss_488.counters[3]++;
        lbl_1_bss_488.slots[lbl_1_data_2A4[0]].f2 =
            -400.0 + (35 - lbl_1_data_2A4[0] * 70) *
            sin(3.141592653589793 * (lbl_1_bss_488.counters[3] % 160 * 360 / 160) / 180.0);
        lbl_1_bss_2++;
        lbl_1_bss_488.counterA0 = 0;
        if (((M547ModelEntry *)&lbl_1_bss_488)[lbl_1_data_2A4[0]].y.s == 0) {
            if (lbl_1_bss_24[1] > lbl_1_bss_34[0]) {
                lbl_1_bss_488.counterA0 = 2;
            }
        }
    } else {
        lbl_1_bss_488.counterA0 = 1;
    }
    lbl_1_bss_488.counterA4 = 0;
    lbl_1_bss_488.counters[0] = 0;
}

void fn_1_7A08(void)
{
    f32 t;
    s32 idx;

    lbl_1_bss_698 = lbl_1_bss_698 - 0.046875004f;
    if (lbl_1_bss_698 < 0.0f) {
        lbl_1_bss_698 = lbl_1_bss_698 + lbl_1_bss_69C;
    }
    t = 0.5f - 0.5f * lbl_1_bss_698 / lbl_1_bss_69C;
    Hu3DTexScrollPosSet(lbl_1_bss_480, 0.0f, t, 0.0f);
    lbl_1_bss_488.counters[3]++;
    lbl_1_bss_488.slots[lbl_1_data_2A4[0]].f2 =
        -400.0 + (35 - lbl_1_data_2A4[0] * 70) *
        sin(3.141592653589793 * (lbl_1_bss_488.counters[3] % 160 * 360 / 160) / 180.0);
    lbl_1_bss_2++;
    lbl_1_bss_488.counters[0]++;
    if (lbl_1_bss_488.counterA4 == 0 && lbl_1_bss_488.counters[0] > 30) {
        lbl_1_bss_6A4[0] = GameMesCreate(4, 3, ((M547IdxEntry *)&lbl_1_bss_488)[lbl_1_data_2A4[0]].b, -1, -1, -1);
        HuAudJinglePlay(0x79);
        CharFXPlay(((M547IdxEntry *)&lbl_1_bss_488)[lbl_1_data_2A4[0]].b, 0x1fa);
        lbl_1_bss_488.counterA4 = 1;
        lbl_1_bss_488.counters[0] = 0;
        idx = ((M547IdxEntry *)&lbl_1_bss_488)[lbl_1_data_2A4[0]].a;
        if (_CheckFlag(0x1000F) == 0) {
            GwPlayer[idx].field2C = 10;
        }
    } else {
        if ((GameMesStatGet(lbl_1_bss_6A4[0]) & 0xFF) == 0) {
            if (lbl_1_bss_488.counters[0] >= 210) {
                GameMesKill(lbl_1_bss_6A4[0]);
                MgSeqModeNext();
            }
        }
    }
}

void fn_1_7DAC(void)
{
    s32 i;

    lbl_1_bss_488.counters[0]++;
    if (lbl_1_bss_488.counterA4 == 0 && lbl_1_bss_488.counters[0] > 30) {
        for (i = 0; i < 2; i++) {
            lbl_1_bss_488.fxSlots[i].kind = 6;
            lbl_1_bss_488.fxSlots[i].state = 0;
            lbl_1_bss_488.fxSlots[i].flags = 0x40000001;
        }
        lbl_1_bss_6A4[0] = GameMesCreate(2, 2);
        HuAudJinglePlay(123);
        lbl_1_bss_488.counterA4++;
        lbl_1_bss_488.counters[0] = 0;
        CharFXPlayPan(lbl_1_bss_488.fxId0, 0x200, 0x30);
        CharFXPlayPan(lbl_1_bss_488.fxId1, 0x200, 0x50);
    } else {
        if ((GameMesStatGet(lbl_1_bss_6A4[0]) & 0xFF) == 0) {
            if (lbl_1_bss_488.counters[0] >= 210) {
                GameMesKill(lbl_1_bss_6A4[0]);
                MgSeqModeNext();
            }
        }
    }
}

void fn_1_7F2C(void)
{
    f32 t;

    lbl_1_bss_488.counters[0]++;
    lbl_1_bss_698 = lbl_1_bss_698 - 0.046875004f;
    if (lbl_1_bss_698 < 0.0f) {
        lbl_1_bss_698 = lbl_1_bss_698 + lbl_1_bss_69C;
    }
    t = 0.5f - 0.5f * lbl_1_bss_698 / lbl_1_bss_69C;
    Hu3DTexScrollPosSet(lbl_1_bss_480, 0.0f, t, 0.0f);
    lbl_1_bss_488.counters[3]++;
    lbl_1_bss_488.slots[lbl_1_data_2A4[0]].f2 =
        -400.0 + (35 - lbl_1_data_2A4[0] * 70) *
        sin(3.141592653589793 * (lbl_1_bss_488.counters[3] % 160 * 360 / 160) / 180.0);
    lbl_1_bss_2++;
    if (lbl_1_bss_488.counterA4 == 0 && lbl_1_bss_488.counters[0] > 30) {
        lbl_1_bss_6A4[0] = GameMesCreate(0xe, lbl_1_bss_24[1]);
        GWMgRecordSet(7, lbl_1_bss_24[1]);
        lbl_1_bss_488.counterA4++;
        lbl_1_bss_24[0]++;
        if (_CheckFlag(0x1000F) == 0) {
            HuAudJinglePlay(0x86);
        } else {
            lbl_1_bss_488.counters[0] = 150;
        }
    } else {
        if ((GameMesStatGet(lbl_1_bss_6A4[0]) & 0xFF) == 0) {
            if (lbl_1_bss_488.counters[0] >= 150) {
                GameMesKill(lbl_1_bss_6A4[0]);
                lbl_1_bss_488.counterA0 = 0;
                lbl_1_bss_488.counterA4 = 0;
            }
        }
    }
}

void fn_1_8298(void)
{
    s32 i = 0;
    s32 cnt = 0;
    f32 scale;

    for (; i < 2; i++) {
        if (lbl_1_bss_488.counters[i + 1] > 10) {
            cnt++;
        }
    }
for (i = 0; i < 2; i++) {
        if (lbl_1_bss_430[i] == 0) {
            if (lbl_1_bss_430[2] <= 0) {
                lbl_1_bss_430[2] = rand8() % 50 + 100;
                lbl_1_bss_440[i].f1 = 1000.0f;
                Hu3DModelAttrReset(((M547ModelIdProbe *)&lbl_1_bss_488.models[i])->id, 1);
                lbl_1_bss_440[i].f2 = -40 - rand8() % 20;
                lbl_1_bss_430[i] = 1;
                scale = rand8() % 10;
                Hu3DModelScaleSet(((M547ModelIdProbe *)&lbl_1_bss_488.models[i])->id,
                                  0.5 + 0.5 * scale / 10.0,
                                  0.5 + 0.5 * scale / 10.0,
                                  0.5 + 0.5 * scale / 10.0);
                if (i != 0) {
                    lbl_1_bss_440[i].f0 = 60 + rand8() % 40;
                } else {
                    lbl_1_bss_440[i].f0 = -60 - rand8() % 40;
                }
            } else if (cnt != 0) {
                lbl_1_bss_430[2]--;
            }
        } else {
            lbl_1_bss_440[i].f1 -= 40.0f;
            if (lbl_1_bss_440[i].f1 < -600.0f) {
                lbl_1_bss_430[i] = 0;
                Hu3DModelAttrSet(((M547ModelIdProbe *)&lbl_1_bss_488.models[i])->id, 1);
            }
        }
    }
}
