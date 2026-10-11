#include "types.h"

typedef s16 HU3DMODELID;
typedef s16 HU3DMOTID;

typedef struct omObj_s {
    u8 pad00[0x40];
    /* 0x40 */ void *mdl;
    /* 0x44 */ u8 pad44[0x04];
    /* 0x48 */ void *mot;
    /* 0x4C */ s32 idx;
    u8 pad50[0x0C];
    /* 0x5C */ void *data;
} OMOBJ;

extern OMOBJ **lbl_1_bss_F8;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
} Vec3;

extern u16 HuPadBtnDown[4];
extern u8 HuPadDStk[4];
extern Vec3 lbl_1_data_1A4[];
extern s32 lbl_1_data_170[8];

typedef struct {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ s32 unk04[3];
} TBL;

extern TBL lbl_1_bss_D8[2];
extern void *lbl_1_bss_D4;
extern s32 fn_1_848C(s32 arg0);
extern s32 fn_1_3378(Vec3 *arg0, Vec3 *arg1, Vec3 *arg2, f32 arg3);
extern s32 fn_1_360C(Vec3 *arg0, Vec3 *arg1, Vec3 *arg2, f32 arg3);
extern f32 fn_1_349C(f32 arg0, f32 arg1, f32 arg2);
extern s32 fn_1_1014(Vec3 *arg0);
extern s32 CharMotionTotalTimeGet(s16 charNo, s32 motNo);
extern s32 CharMotionExtraTimeGet(s16 charNo, s32 motNo);
extern f32 Hu3DMotionTimeGet(s16 mdlId);
extern s32 Hu3DMotionShiftIDGet(s16 mdlId);
extern void CharFXPlayPan(s16 charNo, s32 seId, s32 pan);
extern void omDelObjEx(void *objman, OMOBJ *obj);
extern void fn_1_4F14(s32 arg0);
extern void fn_1_5138(s32 arg0);
extern void fn_1_4B8C(OMOBJ *arg0, s32 arg1);
extern void fn_1_52F8(s32 arg0);
extern void fn_1_54A8(s32 arg0);

typedef struct playerObj_s {
    /* 0x00 */ u8 pad00[0x04];
    /* 0x04 */ s16 unk04;
    /* 0x06 */ s16 unk06;
    /* 0x08 */ u8 pad08[0x04];
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ u8 pad10[0x36];
    /* 0x46 */ s16 unk46;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ u8 pad4C[0x1C];
    /* 0x68 */ s16 unk68;
    /* 0x6A */ s16 unk6A;
} PLAYEROBJ;

void fn_1_4EB4(s32 arg0, s32 arg1)
{
    u8 *work = *(u8 **)((u8 *)lbl_1_bss_F8[arg0] + 0x5C);
    *(u32 *)(work + 0x28) = arg1;
}

s32 fn_1_4EE4(s32 arg0)
{
    u8 *work = *(u8 **)((u8 *)lbl_1_bss_F8[arg0] + 0x5C);
    return *(s32 *)(work + 0x28);
}

// NONMATCHING: only the table-reset half is written; the second pass that walks
// lbl_1_bss_F8 and seeds the tables from each player's work area is not.
// Reset both assignment tables: wipe each 16-byte entry, then fill its three
// id slots with -1 so the first real assignment always lands.
void fn_1_4D74(void)
{
    s32 i;
    s32 j;

    for (i = 0; i < 2; i++) {
        memset(&lbl_1_bss_D8[i], 0, 0x10);
        for (j = 0; j < 3; j++) {
            lbl_1_bss_D8[i].unk04[j] = -1;
        }
    }
}

extern void CharModelAttrSet(s16 charNo, u32 attr);
extern s32 CharMotionTotalTimeGet(s16 charNo, s32 motNo);
extern s32 CharMotionExtraTimeGet(s16 charNo, s32 motNo);
extern void CharMotionShiftSet(s16 charNo, s32 motNo, f32 start, f32 end, u32 attr);
extern void CharModelAttrReset(s16 charNo, u32 attr);
extern void CharMotionSet(s16 charNo, s32 motNo);
extern void Hu3DMotionShiftSet(s16 mdlId, s16 motId, f32 start, f32 end, u32 attr);
extern void Hu3DModelAttrReset(s16 mdlId, u32 attr);
extern void Hu3DMotionSet(s16 mdlId, s16 motId);
extern Vec3 lbl_1_data_204[];
extern Vec3 lbl_1_data_32C[][3];

// Per-frame player state: 0 seeds the spawn position and arms the spawn motion,
// 1 drives the approach and hands off to the arrive or wait path.
void fn_1_3E68(OMOBJ *obj)
{
    u8 *work = *(u8 **)((u8 *)obj + 0x5C);

    switch (obj->idx) {
    case 0:
        obj->idx = 1;
        *(Vec3 *)(work + 0x50) = lbl_1_data_1A4[*(s32 *)(work + 0x2C)];
        CharModelAttrSet(*(s16 *)(work + 0x02), 0x40000001);
        fn_1_4B8C(obj, 2);
        break;

    case 1:
        if (fn_1_3378((Vec3 *)(work + 0x10), (Vec3 *)(work + 0x50), (Vec3 *)(work + 0x1C), 10.0f) != 0) {
            if (*(s32 *)(work + 0x2C) == 0) {
                fn_1_5138(*(s16 *)work);
            } else {
                fn_1_4F14(*(s16 *)work);
            }
        }
        break;
    }
}

// NONMATCHING: the position reset half is written; the motion block differs (the target// re-issues the same shift/set choice as fn_1_52F8, my version re-arms a hook object).// Send a player home: copy the entry position for its mode, clear the velocity,
// arm the return state, then re-run the same motion selection fn_1_52F8 uses.
void fn_1_5138(s32 idx)
{
    OMOBJ *obj = lbl_1_bss_F8[idx];
    u8 *work = obj->data;

    *(Vec3 *)(work + 0x50) = lbl_1_data_32C[*(s16 *)(work + 0x46)][0];
    *(f32 *)(work + 0x54) = 0.0f;
    *(f32 *)(work + 0x5C) = 0.0f;
    *(f32 *)(work + 0x60) = 0.0f;
    *(f32 *)(work + 0x64) = 0.0f;
    *(s32 *)(work + 0x28) = 4;

    {
        OMOBJ *obj2 = lbl_1_bss_F8[idx];
        u8 *work2 = obj2->data;

        if (*(s32 *)(work2 + 0x30) != 2) {
            *(s32 *)(work2 + 0x30) = 2;
            if (work2[0x6C] != 0) {
                omAddObjEx(lbl_1_bss_D4, 0x2000, 0, 0, -1, fn_1_4B8C);
            }
        }
    }
}

// NONMATCHING: the position half and both motion paths are right, but the original keeps// the re-read object in r30 with its work in r29 while MWCC shifts the whole set.// NONMATCHING: MWCC keeps obj in r28 where the original uses r30; the switch, both// time getters and the motion-time gate are otherwise identical.
// Fire a motion-end voice: pick the cue and its total length from the motion
// index, then once the motion has actually run past that length, play it panned
// and drop the hook object.
void fn_1_49DC(OMOBJ *obj)
{
    u8 *work = *(u8 **)((u8 *)lbl_1_bss_F8[obj->idx] + 0x5C);
    s32 motion;
    s32 time;
    s32 pan;

    switch (*(s32 *)(work + 0x30)) {
    case 4:
        motion = 0x1F8;
        time = CharMotionTotalTimeGet(*(s16 *)(work + 0x02), lbl_1_data_170[*(s32 *)(work + 0x30)]);
        break;
    case 6:
        motion = 0x1FB;
        time = CharMotionTotalTimeGet(*(s16 *)(work + 0x02), lbl_1_data_170[*(s32 *)(work + 0x30)]);
        break;
    default:
        motion = 0x200;
        time = CharMotionExtraTimeGet(*(s16 *)(work + 0x02), lbl_1_data_170[*(s32 *)(work + 0x30)]);
        break;
    }
    pan = fn_1_1014((Vec3 *)(work + 0x10));
    if (Hu3DMotionTimeGet(*(s16 *)(lbl_1_bss_F8[obj->idx]->mdl)) >= (f64)(s16)time) {
        if (Hu3DMotionShiftIDGet(*(s16 *)(lbl_1_bss_F8[obj->idx]->mdl)) < 0) {
            CharFXPlayPan(*(s16 *)(work + 0x02), motion, pan);
            omDelObjEx(lbl_1_bss_D4, obj);
        }
    }
}

// Park a player at the idle slot: copy the idle position for its state, clear// the velocity, arm the idle state, then re-issue the idle motion.
void fn_1_52F8(s32 idx)
{
    OMOBJ *obj = lbl_1_bss_F8[idx];
    u8 *work = obj->data;

    *(Vec3 *)(work + 0x50) = lbl_1_data_204[*(s32 *)(work + 0x2C)];
    *(f32 *)(work + 0x54) = 0.0f;
    *(f32 *)(work + 0x5C) = 0.0f;
    *(f32 *)(work + 0x60) = 0.0f;
    *(f32 *)(work + 0x64) = 0.0f;
    *(s32 *)(work + 0x28) = 4;

    {
        OMOBJ *obj2 = lbl_1_bss_F8[idx];
        u8 *work2 = obj2->data;
        s16 charNo = *(s16 *)(work2 + 0x02);
        f32 shift = 0.0f;
        f32 time = 10.0f;
        u32 attr = 0x40000001;

        if (*(s32 *)(work2 + 0x30) != 2) {
            *(s32 *)(work2 + 0x30) = 2;
            if (time != 0.0f) {
                CharMotionShiftSet(charNo, *(s16 *)((u8 *)obj2->mot + 4), shift, time, attr);
                Hu3DMotionShiftSet(*(s16 *)((u8 *)obj2->mdl + 0x02), *(s16 *)(work2 + 0x3A), shift, time, attr);
            } else {
                CharModelAttrReset(charNo, 0x40000003);
                CharMotionSet(charNo, *(s16 *)((u8 *)obj2->mot + 4));
                Hu3DModelAttrReset(*(s16 *)((u8 *)obj2->mdl + 0x02), 0x40000003);
                Hu3DMotionSet(*(s16 *)((u8 *)obj2->mdl + 0x02), *(s16 *)(work2 + 0x3A));
            }
        }
    }
}

