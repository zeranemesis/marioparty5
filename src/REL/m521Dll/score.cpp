#include "dolphin/types.h"
#include "game/data.h"
#include "game/sprite.h"

extern "C" {
void __dl__FPv(void *ptr);

// singleton.h assertion
void fn_1_4C3C(const char *cond, const char *file, s32 line);

// The intrusive list the score registers itself in; lbl_1_bss_1C is its head.
struct CListNode {
    CListNode *m_prev;
    CListNode *m_next;
};

extern CListNode lbl_1_bss_1C;

void fn_1_5CCC(CListNode *self);
void fn_1_5D98(CListNode *self, CListNode *head);
void fn_1_5CD8(CListNode *self, s32 freeIt);

// Virtual table of the base class (lives in m521.o).
extern void *lbl_1_data_21C[5];

// One on-screen score panel: a sprite group plus three fading digit slots.
struct ScoreEntry {
    s16 m_grp;     /* 0x00 */
    s32 m_count;   /* 0x04 */
    s32 m_fade[3]; /* 0x08 */
    f32 m_x;       /* 0x14 */
    f32 m_y;       /* 0x18 */
};

class CScore {
public:
    CListNode m_node;      /* 0x00 */
    void **m_vtable;       /* 0x08 */
    s32 m_bitFlags;        /* 0x0C */
    u8 m_active;           /* 0x10 */
    ScoreEntry m_entry[2]; /* 0x14 */
};

extern CScore *lbl_1_bss_A74;

CScore *fn_1_17F78(CScore *self);
CScore *fn_1_182D4(CScore *self, s16 freeIt);
static void fn_1_18360(CScore *self, s32 idx);
void fn_1_1844C(CScore *self, s32 idx, s32 value);
void fn_1_1845C(ScoreEntry *entry);
void fn_1_1851C(ScoreEntry *entry);
void fn_1_186F0(CScore *self);
void fn_1_18A10(CScore *self, s32 bit);
CScore *fn_1_18A4C(CScore *self, s16 freeIt);

#ifdef __MWERKS__
#define SCORE_RODATA __declspec(section ".rodata")
#else
#define SCORE_RODATA
#endif

void *lbl_1_data_7E4[5] = { 0, 0, (void *)fn_1_18A4C, (void *)fn_1_18A10, (void *)fn_1_186F0 };

struct ScorePos {
    f32 x;
    f32 y;
};

SCORE_RODATA const ScorePos lbl_1_rodata_3DC[2] = { { 58.0f, 54.0f }, { 520.0f, 54.0f } };

// singleton.h register/unregister pair: the derived pointer is built from the
// subobject address and only then biased back, so the null check stays.
static CScore *singletonPtr(u8 *subObj)
{
    return (subObj != 0) ? (CScore *)(subObj - 0x10) : 0;
}

CScore *fn_1_17F78(CScore *self)
{
    ANIMDATA *animA;
    ANIMDATA *animB;
    ANIMDATA *animC;
    s32 i;
    s16 j;
    HUSPRGRPID grp;
    s32 fileNum;

    fn_1_5CCC(&self->m_node);
    self->m_vtable = lbl_1_data_21C;
    self->m_bitFlags = 0;
    fn_1_5D98(&self->m_node, &lbl_1_bss_1C);
    if (lbl_1_bss_A74 != 0) {
        fn_1_4C3C("ms_pointer == 0", "singleton.h", 0x35);
    }
    lbl_1_bss_A74 = singletonPtr((u8 *)self + 0x10);
    self->m_vtable = lbl_1_data_7E4;
    self->m_active = 0;
    animA = HuSprAnimDataRead(0x4F0011);
    animB = HuSprAnimDataRead(0x4F0012);
    for (i = 0; i < 2; i++) {
        if (i == 0) {
            fileNum = 0x4F000F;
        } else {
            fileNum = 0x4F0010;
        }
        animC = HuSprAnimDataRead(fileNum);
        grp = HuSprGrpCreate(8);
        self->m_entry[i].m_grp = grp;
        HuSprGrpMemberSet(grp, 0, HuSprCreate(animC, 0x190, 0));
        HuSprGrpMemberSet(grp, 2, HuSprCreate(animA, 0x12C, 0));
        HuSprGrpMemberSet(grp, 3, HuSprCreate(animA, 0x12C, 0));
        HuSprGrpMemberSet(grp, 4, HuSprCreate(animA, 0x12C, 0));
        HuSprPosSet(grp, 2, -22.0f, 0.0f);
        HuSprPosSet(grp, 3, 0.0f, 0.0f);
        HuSprPosSet(grp, 4, 22.0f, 0.0f);
        HuSprGrpMemberSet(grp, 5, HuSprCreate(animB, 0x64, 0));
        HuSprGrpMemberSet(grp, 6, HuSprCreate(animB, 0x64, 0));
        HuSprGrpMemberSet(grp, 7, HuSprCreate(animB, 0x64, 0));
        HuSprPosSet(grp, 5, -22.0f, 0.0f);
        HuSprPosSet(grp, 6, 0.0f, 0.0f);
        HuSprPosSet(grp, 7, 22.0f, 0.0f);
        for (j = 0; j < 8; j++) {
            HuSprAttrSet(grp, j, 1);
            HuSprAttrSet(grp, j, 4);
        }
        fn_1_18360(self, i);
    }
    return self;
}

CScore *fn_1_182D4(CScore *self, s16 freeIt)
{
    if (self != 0) {
        if (lbl_1_bss_A74 == 0) {
            fn_1_4C3C("ms_pointer != 0", "singleton.h", 0x3D);
        }
        lbl_1_bss_A74 = 0;
        if (freeIt > 0) {
            delete self;
        }
    }
    return self;
}

static void fn_1_18360(CScore *self, s32 idx)
{
    ScoreEntry *entry;
    s32 i;

    entry = &self->m_entry[idx];
    entry->m_count = 0;
    entry->m_x = lbl_1_rodata_3DC[idx].x;
    entry->m_y = lbl_1_rodata_3DC[idx].y;
    HuSprPosSet(entry->m_grp, 1, -36.0f, 0.0f);
    HuSprAttrSet(entry->m_grp, 1, 4);
    HuSprGrpPosSet(entry->m_grp, entry->m_x, entry->m_y);
    for (i = 0; i < 3; i++) {
        entry->m_fade[i] = 0x12;
    }
}

void fn_1_1844C(CScore *self, s32 idx, s32 value)
{
    self->m_entry[idx].m_count = value;
}

void fn_1_1845C(ScoreEntry *entry)
{
    s32 i;

    for (i = 0; i < 3; i++) {
        if (i < entry->m_count) {
            if (--entry->m_fade[i] < 0) {
                entry->m_fade[i] = 0;
            }
        } else {
            if ((f64)++entry->m_fade[i] > 18.0) {
                entry->m_fade[i] = 0x12;
            }
        }
    }
}

void fn_1_1851C(ScoreEntry *entry)
{
    s16 i;
    s32 j;
    f32 scale;

    for (i = 0; i < 5; i++) {
        HuSprAttrReset(entry->m_grp, i, 4);
    }
#ifdef TARGET_PC
    // The original loops to 3 inclusive and uses member 8, past the group's 8 members.
    for (j = 0; j < 3; j++) {
#else
    for (j = 0; j <= 3; j++) {
#endif
        if ((f64)entry->m_fade[j] < 18.0) {
            HuSprAttrReset(entry->m_grp, (s16)(j + 5), 4);
            scale = ((f64)entry->m_fade[j] + 18.0) / 18.0;
            HuSprScaleSet(entry->m_grp, (s16)(j + 5), scale, scale);
            HuSprTPLvlSet(entry->m_grp, (s16)(j + 5), (18.0 - (f64)entry->m_fade[j]) / 18.0);
        } else {
            HuSprAttrSet(entry->m_grp, (s16)(j + 5), 4);
        }
    }
}

void fn_1_186F0(CScore *self)
{
    s32 i;
    s32 j;
    s32 m;
    s16 k;
    f32 scale;

    if (self->m_active != 0) {
        for (i = 0; i < 2; i++) {
            for (j = 0; j < 3; j++) {
                if (j < self->m_entry[i].m_count) {
                    if (--self->m_entry[i].m_fade[j] < 0) {
                        self->m_entry[i].m_fade[j] = 0;
                    }
                } else {
                    if ((f64)++self->m_entry[i].m_fade[j] > 18.0) {
                        self->m_entry[i].m_fade[j] = 0x12;
                    }
                }
            }
            for (k = 0; k < 5; k++) {
                HuSprAttrReset(self->m_entry[i].m_grp, k, 4);
            }
#ifdef TARGET_PC
            for (m = 0; m < 3; m++) {
#else
            for (m = 0; m <= 3; m++) {
#endif
                if ((f64)self->m_entry[i].m_fade[m] < 18.0) {
                    HuSprAttrReset(self->m_entry[i].m_grp, (s16)(m + 5), 4);
                    scale = ((f64)self->m_entry[i].m_fade[m] + 18.0) / 18.0;
                    HuSprScaleSet(self->m_entry[i].m_grp, (s16)(m + 5), scale, scale);
                    HuSprTPLvlSet(self->m_entry[i].m_grp, (s16)(m + 5), (18.0 - (f64)self->m_entry[i].m_fade[m]) / 18.0);
                } else {
                    HuSprAttrSet(self->m_entry[i].m_grp, (s16)(m + 5), 4);
                }
            }
        }
    }
}

void fn_1_18A10(CScore *self, s32 bit)
{
    self->m_bitFlags &= ~(1 << bit);
    switch (bit) {
    case 0:
        break;
    case 2:
        self->m_active = 1;
        break;
    case 5:
        break;
    }
}

CScore *fn_1_18A4C(CScore *self, s16 freeIt)
{
    if (self != 0) {
        self->m_vtable = lbl_1_data_7E4;
        if (&self->m_node != 0) {
            if (lbl_1_bss_A74 == 0) {
                fn_1_4C3C("ms_pointer != 0", "singleton.h", 0x3D);
            }
            lbl_1_bss_A74 = 0;
        }
        if (self != 0) {
            self->m_vtable = lbl_1_data_21C;
            if (self != 0) {
                fn_1_5CD8(&self->m_node, 0);
            }
        }
        if (freeIt > 0) {
            delete self;
        }
    }
    return self;
}
}
