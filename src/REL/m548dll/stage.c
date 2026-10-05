#include "types.h"

#include "game/charman.h"
#include "game/hu3d.h"

typedef struct {
    s32 unk0;
    u16 unk4;
    s16 unk6;
    u8 unk8[0x14];
} STAGE_CHAR;

typedef struct {
    s16 unk0;
    f32 unk4;
    u8 unk8[0xBC];
} STAGE_MOTION;

typedef struct {
    u8 pad0[0xC4];
    STAGE_MOTION speed;
} STAGE_MOTION_ROOT;

#pragma pack(2)
typedef struct {
    s16 unk0;
    f32 unk4;
    f32 unk8;
    u8 unkA[0x1A];
} STAGE_ENTRY;

typedef struct {
    STAGE_CHAR unk0[2];
    u8 unk38[4];
    STAGE_MOTION_ROOT *unk3C;
    s32 *unk40[2];
    s16 *unk48[2];
    u8 unk50[2];
    STAGE_ENTRY unk52[2];
    u8 unk9A[0x62];
    s32 unkFC;
    u8 unk100[0xB4];
    s32 unk1B4;
    u8 unk1B8[4];
    s32 unk1BC;
} STAGE_WORK;

extern STAGE_WORK *lbl_1_bss_5F8;

void fn_1_7104(void);
void fn_1_773C(void);
void fn_1_7C88(void);
void fn_1_856C(void);
void fn_1_8F78(void);

static void (*lbl_1_data_5C0[5])(void) = {
    fn_1_7104,
    fn_1_773C,
    fn_1_7C88,
    fn_1_856C,
    fn_1_8F78,
};

void fn_1_65E8(s32 *arg0, s32 *arg1)
{
    lbl_1_bss_5F8->unk40[0] = arg0;
    lbl_1_bss_5F8->unk40[1] = arg1;
}

void fn_1_660C(s16 *arg0, s16 *arg1)
{
    lbl_1_bss_5F8->unk48[0] = arg0;
    lbl_1_bss_5F8->unk48[1] = arg1;
}

void fn_1_6D4C(void)
{
    s16 i;
    s16 j;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 5; j++) {
            lbl_1_bss_5F8->unk48[i][j] =
                CharMotionCreate(lbl_1_bss_5F8->unk0[i].unk6, lbl_1_bss_5F8->unk40[i][j]);
        }
    }

    for (i = 0; i < 2; i++) {
        lbl_1_bss_5F8->unk52[i].unk0 = -1;
        lbl_1_bss_5F8->unk52[i].unk4 = 1.0f;
        lbl_1_bss_5F8->unk52[i].unk8 = -1.0f;
    }
}

void fn_1_70B0(void)
{
    s32 i;

    for (i = 0; i < 5; i++) {
        lbl_1_data_5C0[i]();
    }
}

void fn_1_8F78(void)
{
    STAGE_MOTION *mot = &lbl_1_bss_5F8->unk3C->speed;

    switch (lbl_1_bss_5F8->unk1BC) {
    case 0:
        if (lbl_1_bss_5F8->unkFC != 2) {
            break;
        }
        lbl_1_bss_5F8->unk1BC++;
        lbl_1_bss_5F8->unk1B4 = 0;
        Hu3DMotionSpeedSet(mot->unk0, 0.5f);
        Hu3DMotionTimeSet(mot->unk0, 0.0f);
        break;
    case 1:
        lbl_1_bss_5F8->unk1B4++;
        mot->unk4 = lbl_1_bss_5F8->unk1B4 * 4000 / 200 - 2000;
        if (lbl_1_bss_5F8->unk1B4 == 200) {
            lbl_1_bss_5F8->unk1BC++;
        }
        break;
    case 2:
        if (lbl_1_bss_5F8->unkFC != 1) {
            break;
        }
        lbl_1_bss_5F8->unk1BC = 0;
        break;
    }
}