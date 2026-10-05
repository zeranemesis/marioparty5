#include "game/object.h"
#include "game/gamework.h"
#include "game/hu3d.h"
#include "game/hsfex.h"
#include "game/gamemes.h"
#include "game/wipe.h"
#include "game/audio.h"
#include "game/memory.h"
#include "game/pad.h"
#include "game/mg/seqman.h"
#include "game/mg/timer.h"

#include "humath.h"
#include "msm.h"

#include "datanum/m523.h"

extern void fn_1_992C(void);
extern void fn_1_4CCC(void *);
extern void fn_1_6294(void *);
extern s32 fn_1_90EC(void *);
extern OMOBJMAN *lbl_1_bss_E4;

typedef struct PlayerWork3_s {
    u8 pad[0x28];
    s32 unk28;
    u8 pad2[0x54 - 0x2C];
    s32 unk54;
} PlayerWork3;

extern PlayerWork3 *lbl_1_bss_140[4];

typedef struct PlayerWork_s {
    struct PlayerWork_s *unk0;
    struct PlayerWork_s *unk4;
} PlayerWork;

void fn_1_98E8(PlayerWork *work)
{
    work->unk0->unk4 = work->unk4;
    work->unk4->unk0 = work->unk0;

    fn_1_992C();
}

typedef struct PlayerNode_s {
    struct PlayerNode_s *next;
    u8 pad[0x18 - 4];
    u16 unk18;
} PlayerNode;

extern PlayerNode lbl_1_bss_10C;

void fn_1_992C(void)
{
    PlayerNode *node;
    u16 idx;

    idx = 0;
    node = &lbl_1_bss_10C;
    goto cond;
loop:
    node->unk18 = idx++;
    node = node->next;
cond:
    if (node->next != NULL) {
        goto loop;
    }
}

typedef struct PlayerSubWork_s {
    u8 pad[0x30];
    s16 unk30;
} PlayerSubWork;

typedef struct PlayerMainWork_s {
    u8 pad[0x5C];
    PlayerSubWork *unk5C;
} PlayerMainWork;

void fn_1_40BC(PlayerMainWork *work)
{
    PlayerSubWork *sub;

    sub = work->unk5C;
    if (sub->unk30 == 0) {
        fn_1_4CCC(work);
    } else {
        fn_1_6294(work);
    }
}

typedef struct PlayerWork2_s {
    u8 pad[0x40];
    s16 *unk40;
} PlayerWork2;

void fn_1_4534(PlayerWork2 *work)
{
    if (Hu3DMotionEndCheck(*work->unk40)) {
        Hu3DModelKill(*work->unk40);
        omDelObjEx(lbl_1_bss_E4, (OMOBJ *)work);
    }
}

void fn_1_405C(void)
{
    int i;

    for (i = 0; i < 4; i++) {
        lbl_1_bss_140[i]->unk28 = 1;
        lbl_1_bss_140[i]->unk54 = 0;
    }
}

typedef struct PlayerWork4_s {
    u8 pad[0x4];
    s16 unk4;
    u8 pad2[0xC - 0x6];
    void *unkC;
} PlayerWork4;

s32 fn_1_7358(PlayerWork4 *work)
{
    s32 result;

    result = 0;
    if ((s32)work->unkC) {
        result = fn_1_90EC(work);
    } else if (HuPadBtnDown[work->unk4] & 0x100) {
        result = 1;
    }
    return result;
}