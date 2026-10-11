// m555Dll/REL/m555Dll/stage - decompilation of the m555Dll/REL/m555Dll/stage unit
// (3808 B of text, 15 functions).  Real code is added function by function,
// smallest first, each verified with tools/fndiff.py before the next one.
// Only the smallest helpers are written so far, so the unit stays
// NonMatching and does not link into the module yet.

#include "dolphin/types.h"

/* The 0x2E8-byte stage control record.  fn_1_1A9A4 stores its caller's work
   pointer into lbl_1_bss_3038 after memset-ing the whole record, and it lays
   out two 0x1C-stride model entries at 0x000, so every helper below reaches
   the live record through that pointer.  Only the fields the helpers written
   so far touch are named; the padding keeps the named offsets where the
   original has them. */
typedef struct {
    u8 pad_0[0x5C];       /* 0x000 */
    s32 unk5C[3];         /* 0x05C */
    s32 unk68[3];         /* 0x068 */
    u8 pad_74[0x100];     /* 0x074 */
    s16 unk174;           /* 0x174 */
    u8 pad_176[0x172];    /* 0x176 */
} S555_CTRL;

/* The unit's own pointer to the live control record; stage.o's only .bss
   symbol. */
S555_CTRL *lbl_1_bss_3038;

// 0001ABF0, 20 bytes.  Stores the given halfword at control+0x174.
void fn_1_1ABF0(s16 value)
{
    lbl_1_bss_3038->unk174 = value;
}

// 0001AC04, 32 bytes.  Stores the word at control+0x5C, indexed by the s16
// argument (idx*4).
void fn_1_1AC04(s16 idx, s32 value)
{
    lbl_1_bss_3038->unk5C[idx] = value;
}

// 0001AC24, 32 bytes.  Stores the word at control+0x68, indexed by the s16
// argument (idx*4).
void fn_1_1AC24(s16 idx, s32 value)
{
    lbl_1_bss_3038->unk68[idx] = value;
}

// 0001B868, 4 bytes.  The original body is a single `blr`, i.e. a leaf stub
// with no frame, no locals and no parameters.
void fn_1_1B868(void)
{
}

// 0001B86C, 4 bytes.  Same shape as fn_1_1B868: a bare `blr`.
void fn_1_1B86C(void)
{
}

// 0001B870, 4 bytes.  Same shape as fn_1_1B868: a bare `blr`.
void fn_1_1B870(void)
{
}

// 0001B874, 4 bytes.  Same shape as fn_1_1B868: a bare `blr`.
void fn_1_1B874(void)
{
}

// 0001B878, 4 bytes.  Same shape as fn_1_1B868: a bare `blr`.
void fn_1_1B878(void)
{
}

// 0001B87C, 4 bytes.  Same shape as fn_1_1B868: a bare `blr`.
void fn_1_1B87C(void)
{
}

// 0001B880, 4 bytes.  Same shape as fn_1_1B868: a bare `blr`.
void fn_1_1B880(void)
{
}
