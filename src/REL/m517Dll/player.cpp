// m517Dll player unit: player-related functions.

typedef signed int s32;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef float f32;

extern "C" {
void fn_1_15DC0(void *ptr, s32 value);
void fn_1_1566C(void *ptr);
void fn_1_1A4A0(void);
void fn_1_158FC(void *ptr);
void fn_1_19560(void *ptr);
void fn_1_158A8(void *ptr, s32 a, s32 b);
void fn_1_15614(void);
void fn_1_15A78(void *ptr, f32 a, f32 b);
void fn_1_1877C(void *ptr);
void fn_1_18EDC(void *ptr);
s32 fn_1_6AD4(s32 a, s32 b);
}

// BSS symbols used by player functions
extern s32 lbl_1_bss_A48;
extern s32 lbl_1_bss_A4C;
extern s32 lbl_1_bss_A50;

// rodata constants
extern const f32 lbl_1_rodata_5A4; // 250.0f
extern const f32 lbl_1_rodata_5A8; // -300.0f

// MWCC inlines a definition into its callers inside this translation unit,
// which deletes the call the original keeps. Keep every definition out of line.
#pragma dont_inline on
extern "C" void fn_1_1566C(void *ptr)
{
    fn_1_15DC0(ptr, 0);
}

extern "C" void fn_1_1A4A0(void)
{
    lbl_1_bss_A48 = 0x78;
    lbl_1_bss_A4C = 0xD2;
    lbl_1_bss_A50 = 0xD2;
}

extern "C" void fn_1_158FC(void *ptr)
{
    fn_1_15A78(ptr, lbl_1_rodata_5A4, lbl_1_rodata_5A8);
}

extern "C" void fn_1_19560(void *ptr)
{
    if (((unsigned char *)ptr)[0x2C] != 0) {
        fn_1_1877C(ptr);
    } else {
        fn_1_18EDC(ptr);
    }
}

extern "C" void fn_1_158A8(void *ptr, s32 a, s32 b)
{
    void *p = ptr;
    s32 r4_save = a;
    s32 r5_save = b;
    s32 result = fn_1_6AD4(r4_save, r5_save);
    if ((result & 0xFFFFFF) != 0) {
        *(s32 *)p += 1;
    }
}
#pragma dont_inline reset