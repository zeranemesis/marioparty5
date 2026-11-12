#include "REL/m536Dll.h"

extern "C" {

extern void *lbl_1_bss_9FC;
extern void *lbl_1_data_1FC[];

// One score panel per player, 0x20 bytes each starting at 0x14.
struct M536ScoreSlot {
    s16 m_grp;        /* 0x00 */
    s32 m_charNo;     /* 0x04 */
    s32 m_count;      /* 0x08 */
    s32 m_anim[3];    /* 0x0C */
    f32 m_x;          /* 0x18 */
    f32 m_y;          /* 0x1C */
};

struct M536Score {
    M536_NODE m_node;        /* 0x00 */
    void **m_vptr;           /* 0x08 */
    s32 m_flag;              /* 0x0C */
    u8 m_active;             /* 0x10 */
    M536ScoreSlot m_slot[4]; /* 0x14 */
};

struct M536ScorePos {
    f32 x;
    f32 y;
};

M536Score *fn_1_E180(M536Score *self, s16 freeIt);
void fn_1_E144(M536Score *self, s32 bit);
void fn_1_DE98(M536Score *self);

s32 lbl_1_data_564[15] = {
    0x960020, 0x960021, 0x960022, 0x960023, 0x960024,
    0x960026, 0x960027, 0x960028, 0x960029, 0x96002A,
    0x96002B, 0x96002C, 0x96002D, 0x96002A, 0x96002A,
};
void *lbl_1_data_5A0[6] = { 0, 0, (void *)fn_1_E180, (void *)fn_1_E144, (void *)fn_1_DE98, 0 };

M536_RODATA const M536ScorePos lbl_1_rodata_268[4] = {
    { 70.0f, 54.0f }, { 508.0f, 54.0f }, { 70.0f, 424.0f }, { 508.0f, 424.0f },
};

M536Score *fn_1_D6FC(M536Score *self)
{
    M536Score *ms;
    ANIMDATA *bg;
    ANIMDATA *digit;
    ANIMDATA *ring;
    s16 grp;
    s32 i;
    s16 j;

    fn_1_1264(self);
    self->m_vptr = lbl_1_data_1FC;
    self->m_flag = 0;
    fn_1_1330(self, &lbl_1_bss_C);
    if (lbl_1_bss_9FC != NULL) {
        fn_1_1D4("ms_pointer == 0", "singleton.h", 0x35);
    }
    ms = (M536Score *)((u8 *)self + 0x10);
    if (ms != NULL) {
        ms = (M536Score *)((u8 *)ms - 0x10);
    }
    lbl_1_bss_9FC = ms;
    self->m_vptr = lbl_1_data_5A0;
    self->m_active = 0;
    bg = HuSprAnimRead(HuDataSelHeapReadNum(0x5E000B, 0x10000000, HEAP_MODEL));
    digit = HuSprAnimRead(HuDataSelHeapReadNum(0x5E000C, 0x10000000, HEAP_MODEL));
    ring = HuSprAnimRead(HuDataSelHeapReadNum(0x5E000D, 0x10000000, HEAP_MODEL));
    for (i = 0; i < 4; i++) {
        grp = HuSprGrpCreate(8);
        self->m_slot[i].m_grp = grp;
        HuSprGrpMemberSet(grp, 0, HuSprCreate(bg, 0x190, 0));
        HuSprGrpMemberSet(grp, 2, HuSprCreate(digit, 0x12C, 0));
        HuSprGrpMemberSet(grp, 3, HuSprCreate(digit, 0x12C, 0));
        HuSprGrpMemberSet(grp, 4, HuSprCreate(digit, 0x12C, 0));
        HuSprPosSet(grp, 2, -8.0f, 0.0f);
        HuSprPosSet(grp, 3, 14.0f, 0.0f);
        HuSprPosSet(grp, 4, 36.0f, 0.0f);
        HuSprGrpMemberSet(grp, 5, HuSprCreate(ring, 0x64, 0));
        HuSprGrpMemberSet(grp, 6, HuSprCreate(ring, 0x64, 0));
        HuSprGrpMemberSet(grp, 7, HuSprCreate(ring, 0x64, 0));
        HuSprPosSet(grp, 5, -8.0f, 0.0f);
        HuSprPosSet(grp, 6, 14.0f, 0.0f);
        HuSprPosSet(grp, 7, 36.0f, 0.0f);
        for (j = 0; j < 8; j++) {
            HuSprAttrSet(grp, j, 1);
            HuSprAttrSet(grp, j, 4);
        }
    }
    return self;
}

M536Score *fn_1_DA34(M536Score *self, s16 freeIt)
{
    if (self != NULL) {
        if (lbl_1_bss_9FC == NULL) {
            fn_1_1D4("ms_pointer != 0", "singleton.h", 0x3D);
        }
        lbl_1_bss_9FC = NULL;
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

void fn_1_DAC0(M536Score *self, s32 idx, s32 charNo)
{
    M536ScoreSlot *slot;
    s32 i;

    slot = &self->m_slot[idx];
    slot->m_charNo = charNo;
    slot->m_count = 0;
    slot->m_x = lbl_1_rodata_268[idx].x;
    slot->m_y = lbl_1_rodata_268[idx].y;
    HuSprGrpMemberSet(slot->m_grp, 1,
        HuSprCreate(HuSprAnimRead(HuDataSelHeapReadNum(lbl_1_data_564[charNo], 0x10000000, HEAP_MODEL)), 0x12C, 0));
    HuSprPosSet(slot->m_grp, 1, -36.0f, 0.0f);
    HuSprAttrSet(slot->m_grp, 1, 4);
    HuSprGrpPosSet(slot->m_grp, slot->m_x, slot->m_y);
    for (i = 0; i < 3; i++) {
        slot->m_anim[i] = 0x12;
    }
}

void fn_1_DBF4(M536Score *self, s32 idx, s32 count)
{
    self->m_slot[idx].m_count = count;
}

void fn_1_DC04(M536Score *self, M536ScoreSlot *slot)
{
    s32 i;

    for (i = 0; i < 3; i++) {
        if (i < slot->m_count) {
            if (--slot->m_anim[i] < 0) {
                slot->m_anim[i] = 0;
            }
        } else {
            if ((f32)++slot->m_anim[i] > 18.0f) {
                slot->m_anim[i] = 0x12;
            }
        }
    }
}

void fn_1_DCC4(M536Score *self, M536ScoreSlot *slot)
{
    {
        s16 i;
        for (i = 0; i < 5; i++) {
            HuSprAttrReset(slot->m_grp, i, 4);
        }
    }
    {
        s32 k;
        for (k = 0; k <= 3; k++) {
            if ((f32)slot->m_anim[k] < 18.0f) {
                f32 scale;
                HuSprAttrReset(slot->m_grp, (s16)(k + 5), 4);
                scale = ((f32)slot->m_anim[k] + 18.0) / 18.0;
                HuSprScaleSet(slot->m_grp, (s16)(k + 5), scale, scale);
                HuSprTPLvlSet(slot->m_grp, (s16)(k + 5), (18.0 - slot->m_anim[k]) / 18.0);
            } else {
                HuSprAttrSet(slot->m_grp, (s16)(k + 5), 4);
            }
        }
    }
}

void fn_1_DE98(M536Score *self)
{
    M536ScoreSlot *slot;
    s32 i;

    if (self->m_active != 0) {
        for (i = 0; i < 4; i++) {
            slot = &self->m_slot[i];
            fn_1_DC04(self, slot);
            fn_1_DCC4(self, slot);
        }
    }
}

void fn_1_E144(M536Score *self, s32 bit)
{
    self->m_flag &= ~(1 << bit);
    switch (bit) {
    case 0:
    case 1:
        break;
    case 2:
        self->m_active = 1;
        break;
    }
}

M536Score *fn_1_E180(M536Score *self, s16 freeIt)
{
    if (self != NULL) {
        self->m_vptr = lbl_1_data_5A0;
        if ((M536Score *)((u8 *)self + 0x10) != NULL) {
            if (lbl_1_bss_9FC == NULL) {
                fn_1_1D4("ms_pointer != 0", "singleton.h", 0x3D);
            }
            lbl_1_bss_9FC = NULL;
        }
        if (self != NULL) {
            self->m_vptr = lbl_1_data_1FC;
            if (self != NULL) {
                fn_1_1270(self, 0);
            }
        }
        if (freeIt > 0) {
            fn_1_C8(self);
        }
    }
    return self;
}

}
