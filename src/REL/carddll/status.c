// carddll/status.c - decompilation of the carddll/status unit (104648 B).
// Real code is being added function by function (smallest first); each function is
// verified with tools/fndiff.py before the next one is started. Functions are emitted
// in the order the target disassembly lists them (i.e. by original address).

#include "types.h"

void *memset(void *dst, s32 c, u32 n);

/* ---- type definitions ---- */
typedef struct {
    s32 f_0;
    s32 f_4;
    s32 f_8;
    s32 f_C;
    s32 f_10;
    s32 f_14;
} StatusSlot;

typedef struct {
    u8 a, b, c, d;
} Byte4;

typedef struct {
    s32 f_0;
    s32 f_4;
    s32 f_8;
    s32 f_C;
    f32 f_10;
    s32 f_14;
    s32 f_18;
    s32 f_1C;
    s32 f_20;
    s32 f_24;
    s32 f_28;
    Byte4 f_2C;
} Status1944;

typedef struct {
    s32 f_0;
    s32 f_4;
    s32 f_8;
    StatusSlot *f_C;
} StatusSlotRef;

/* ---- BSS symbols in REVERSE target address order (highest first)
     MWCC -sdata 0 lays out BSS in reverse declaration order:
     last declared = lowest address.
     Target BSS addresses from config/GP5E01_00/rels/carddll/symbols.txt: */
static u8 lbl_1_bss_21D8[0xAC];
static u8 lbl_1_bss_2034[0x1A4];
static StatusSlot lbl_1_bss_201C[1];
static StatusSlot lbl_1_bss_1FD4[3];
static StatusSlot lbl_1_bss_1FBC[1];
static StatusSlot lbl_1_bss_1FA4[1];
static StatusSlot lbl_1_bss_1F44[5];
static StatusSlot lbl_1_bss_1F2C[1];
static StatusSlot lbl_1_bss_1F14[1];
static StatusSlot lbl_1_bss_1EFC[1];
static StatusSlot lbl_1_bss_1ECC[2];
static StatusSlot lbl_1_bss_1E9C[2];
static StatusSlot lbl_1_bss_1E6C[2];
static StatusSlot lbl_1_bss_1E54[1];
static StatusSlot lbl_1_bss_1E3C[1];
static StatusSlot lbl_1_bss_1E24[1];
static StatusSlot lbl_1_bss_1E0C[1];
static StatusSlot lbl_1_bss_1B3C[25];
static StatusSlot lbl_1_bss_1AF4[3];
static StatusSlot lbl_1_bss_1A7C[6];
static StatusSlot lbl_1_bss_1A1C[5];
static StatusSlot lbl_1_bss_1A04[1];
static StatusSlot lbl_1_bss_19EC[1];
static StatusSlot lbl_1_bss_19D4[1];
static StatusSlot lbl_1_bss_19BC[1];
static StatusSlot lbl_1_bss_19A4[1];
static StatusSlot lbl_1_bss_198C[1];
static StatusSlot lbl_1_bss_1974[1];
static Status1944 lbl_1_bss_1944;
static s32 lbl_1_bss_18DC[0x68 / 4];
static u8 lbl_1_bss_1848[0x94];
static u8 lbl_1_bss_17AC[0x9C];
static u8 lbl_1_bss_12FC[0x4B0];

/* ---- lbl_1_data_8D8: 26 records of {s32, s32, s32, StatusSlot *}, 0x10 each.
     DATA symbols use forward declaration order (first declared = lowest address).
     Forward references to BSS require (u32)&sym casts. ---- */
static StatusSlotRef lbl_1_data_8D8[26] = {
    { 0, 0, 1, (StatusSlot *)(u32)&lbl_1_bss_201C[0] },
    { 3, 1, 1, (StatusSlot *)(u32)&lbl_1_bss_1FD4[0] },
    { 1, 1, 1, (StatusSlot *)(u32)&lbl_1_bss_1FBC[0] },
    { 1, 0, 0, (StatusSlot *)(u32)&lbl_1_bss_1FA4[0] },
    { 4, 1, 0, (StatusSlot *)(u32)&lbl_1_bss_1F44[0] },
    { 1, 1, 0, (StatusSlot *)(u32)&lbl_1_bss_1F2C[0] },
    { 1, 1, 1, (StatusSlot *)(u32)&lbl_1_bss_1F14[0] },
    { 1, 1, 1, (StatusSlot *)(u32)&lbl_1_bss_1EFC[0] },
    { 1, 2, 0, (StatusSlot *)(u32)&lbl_1_bss_1ECC[0] },
    { 1, 2, 1, (StatusSlot *)(u32)&lbl_1_bss_1E9C[0] },
    { 1, 2, 1, (StatusSlot *)(u32)&lbl_1_bss_1E6C[0] },
    { 1, 1, 0, (StatusSlot *)(u32)&lbl_1_bss_1E54[0] },
    { 0, 0, 1, (StatusSlot *)(u32)&lbl_1_bss_1E3C[0] },
    { 0, 0, 0, (StatusSlot *)(u32)&lbl_1_bss_1E24[0] },
    { 0, 0, 0, (StatusSlot *)(u32)&lbl_1_bss_1E0C[0] },
    { 0x1E, 0, 1, (StatusSlot *)(u32)&lbl_1_bss_1B3C[0] },
    { 3, 1, 1, (StatusSlot *)(u32)&lbl_1_bss_1AF4[0] },
    { 5, 1, 1, (StatusSlot *)(u32)&lbl_1_bss_1A7C[0] },
    { 4, 2, 1, (StatusSlot *)(u32)&lbl_1_bss_1A1C[0] },
    { 1, 0, 1, (StatusSlot *)(u32)&lbl_1_bss_1A04[0] },
    { 1, 0, 1, (StatusSlot *)(u32)&lbl_1_bss_19EC[0] },
    { 1, 0, 1, (StatusSlot *)(u32)&lbl_1_bss_19D4[0] },
    { 1, 1, 1, (StatusSlot *)(u32)&lbl_1_bss_19BC[0] },
    { 1, 1, 1, (StatusSlot *)(u32)&lbl_1_bss_19A4[0] },
    { 1, 1, 1, (StatusSlot *)(u32)&lbl_1_bss_198C[0] },
    { 1, 1, 1, (StatusSlot *)(u32)&lbl_1_bss_1974[0] },
};

/* callees referenced by fn_1_4FBC8 (defined later in this unit) */
void fn_1_4FBFC(s32 arg0, s32 arg1);
void fn_1_4FFEC(void);

/* fn_1_392B8: clear the whole 0x1A4-byte lbl_1_bss_2034 record. */
void fn_1_392B8(void)
{
    memset(lbl_1_bss_2034, 0, 0x1A4);
}

/* fn_1_3F714: "does record <arg0> slot <arg1> still have its flag unset?"
   The flag lives at offset 4 of a 0x18-byte slot, so the answer is its inverse. */
s32 fn_1_3F714(s32 arg0, s32 arg1)
{
    return lbl_1_data_8D8[arg0].f_C[arg1].f_4 ^ 1;
}

/* fn_1_4149C: store an s32 into word 0x5C of the status record. */
void fn_1_4149C(s32 arg0)
{
    lbl_1_bss_18DC[0x5C / 4] = arg0;
}

/* fn_1_4E8AC: copy four bytes from arg0 into lbl_1_bss_1944 + 0x2C. */
void fn_1_4E8AC(u8 *arg0)
{
    lbl_1_bss_1944.f_2C = *(Byte4 *)arg0;
}

/* fn_1_4E8D8: store an f32 into lbl_1_bss_1944 + 0x10. */
void fn_1_4E8D8(f32 arg0)
{
    lbl_1_bss_1944.f_10 = arg0;
}

/* fn_1_4F114: clear the whole 0x4B0-byte lbl_1_bss_12FC record. */
void fn_1_4F114(void)
{
    memset(lbl_1_bss_12FC, 0, 0x4B0);
}

/* fn_1_4FBC8: hand the two incoming words to fn_1_4FBFC, then run fn_1_4FFEC. */
void fn_1_4FBC8(s32 arg0, s32 arg1)
{
    fn_1_4FBFC(arg0, arg1);
    fn_1_4FFEC();
}

/* fn_1_502DC: the target is a bare "blr" - this entry point takes no action. */
void fn_1_502DC(void)
{
}