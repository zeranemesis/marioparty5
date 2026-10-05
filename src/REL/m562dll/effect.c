#include "types.h"

/* library */
extern void *memset(void *dest, int val, u32 len);
extern void HuMemDirectFree(void *ptr);

/* other functions of this translation unit */
extern void fn_1_2CEF4(void *arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4);
extern void fn_1_2D434(void *arg0, s32 arg1, s32 arg2);

/* 0x10: list header, u16 counter at 0x0, entry array at 0x4, s16 cap at 0xc */
typedef struct EffectList {
    u16 m_count;
    u16 m_pad2;
    void *m_entries;
    s32 m_unk8;
    s16 m_max;
    s16 m_padE;
} EffectList;

/* 0x8: two raw words moved in one go */
typedef struct IntVec2 {
    u32 m_x;
    u32 m_y;
} IntVec2;

/* 0xc: two raw words copied into the entry, plus a third one */
typedef struct IntVec3 {
    IntVec2 m_xy;
    u32 m_z;
} IntVec3;

/* 0x1c: one entry of the array pointed to by m_entries */
typedef struct EffectEntry {
    s16 m_unk0;
    s16 m_unk2;
    u32 m_unk4;
    u32 m_unk8;
    IntVec2 m_xy;
    u32 m_unk14;
    f32 m_unk18;
} EffectEntry;

/* .bss of this translation unit */
s16 lbl_1_bss_79F8[0x10];
s32 lbl_1_bss_7A18;

void fn_1_2CDE8(EffectList *arg0) {
    if (arg0->m_entries != 0) {
        HuMemDirectFree(arg0->m_entries);
        arg0->m_count = 0;
    }
    memset(arg0, 0, 0x10);
}

void fn_1_2CE44(EffectList *arg0) {
    fn_1_2CEF4(arg0, arg0, 2, 1, 0);
    arg0->m_count = 0;
}

void fn_1_2CE8C(EffectList *arg0, s16 arg1, u32 arg2, IntVec3 *arg3,
                u32 arg4, f32 arg5) {
    EffectEntry *temp_r31;

    if (arg0->m_count < arg0->m_max) {
        temp_r31 = (EffectEntry *) ((char *) arg0->m_entries +
                                    arg0->m_count * 0x1C);
        temp_r31->m_unk0 = arg1;
        temp_r31->m_unk8 = arg4;
        temp_r31->m_xy = arg3->m_xy;
        temp_r31->m_unk14 = arg3->m_z;
        temp_r31->m_unk18 = arg5;
        temp_r31->m_unk4 = arg2;
        arg0->m_count++;
    }
}

void fn_1_2D3F0(EffectList *arg0) {
    fn_1_2D434(arg0->m_entries, 0, (s16) ((s16) arg0->m_count - 1));
}

void fn_1_2DB24(void) {
    s32 i;

    for (i = 0; i < 0x10; i++) {
        lbl_1_bss_79F8[i] = -1;
    }
    lbl_1_bss_7A18 = 0;
}