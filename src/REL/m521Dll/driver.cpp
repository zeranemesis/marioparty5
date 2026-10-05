#include "dolphin/types.h"

extern "C" {
typedef struct PTMF {
    long this_delta;
    long v_offset;
    union {
        void *f_addr;
        long ve_offset;
    } f_data;
} PTMF;

s32 __ptmf_test(const PTMF *ptmf);
void __ptmf_scall(void *self);
void __dl__FPv(void *ptr);

// singleton.h assertion
void fn_1_4C3C(const char *cond, const char *file, s32 line);

struct CListNode {
    CListNode *m_prev;
    CListNode *m_next;
};

extern CListNode lbl_1_bss_1C;

void fn_1_5CCC(CListNode *self);
void fn_1_5D98(CListNode *self, CListNode *head);
void fn_1_5CD8(CListNode *self, s32 freeIt);

void fn_1_DDEC(void *);
void fn_1_E784(void *, s32);

extern s32 lbl_1_bss_A00;
extern s32 lbl_1_bss_A04;
extern s32 lbl_1_bss_A08;
extern s32 lbl_1_bss_A0C;
}

extern "C" {
struct Driver {
    u32 m_unk0;
    u32 m_unk4;
    u32 m_unk8;
    u32 m_unkC;
    u8 m_unk10[0x90 - 0x10];
    s32 m_field90;
};

void fn_1_D298(Driver *self, s32 val) {
    if (self->m_field90 != val) {
        self->m_field90 = val;
    }
}

void fn_1_D26C(Driver *self) {
    fn_1_DDEC((u8 *)self + 0x1C);
}

u32 *fn_1_DB10(Driver *self, u32 idx) {
    idx += self->m_unk8;
    if (idx >= self->m_unk0) {
        idx -= self->m_unk0;
    }
    return &((u32 *)self->m_unkC)[idx];
}

void fn_1_F59C(Driver *self, s32 arg) {
    fn_1_E784(self, arg);
}

void fn_1_E748(Driver *self) {
    self->m_unk4--;
    self->m_unk8++;
    if (self->m_unk8 >= self->m_unk0) {
        self->m_unk8 -= self->m_unk0;
    }
}

void fn_1_114DC() {
    lbl_1_bss_A00 = 0x78;
    lbl_1_bss_A04 = 0xD2;
    lbl_1_bss_A08 = 0xD2;
    lbl_1_bss_A0C = 0x96;
}
}