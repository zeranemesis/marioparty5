// m517 unit: small helper functions

typedef signed int s32;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned char u8;
typedef float f32;

extern "C" {
    extern unsigned short omSysExitReq;
    extern u32 Hu3DData[];
    extern u32 lbl_1_data_234[];
    extern void fn_1_632C(void);
    extern void fn_1_6378(void);
    extern void fn_1_63EC(void);
    s32 OSGetCurrentThread(void);
    s32 OSGetThreadPriority(s32 thread);
}

#pragma dont_inline on
extern "C" void fn_1_2E20(void *r3)
{
    u32 *p = (u32 *)r3;
    u32 r0 = p[1];
    u32 r4 = p[0];
    ((u32 *)r4)[1] = r0;
    r0 = p[0];
    r4 = p[1];
    ((u32 *)r4)[0] = r0;
    p[0] = (u32)r3;
    p[1] = (u32)r3;
}
// NONMATCHING: MWCC allocates r31/r30/r29 and emits stack frame; original uses r0/r4 only
#pragma dont_inline reset

#pragma dont_inline on
extern "C" void fn_1_46FC(void)
{
    omSysExitReq = 1;
}
#pragma dont_inline reset

#pragma dont_inline on
extern "C" void fn_1_4B74(s16 *r3, u32 r4)
{
    s16 r0 = *r3;
    u32 r6 = r0 * 0x134;
    u32 r5 = (u32)Hu3DData;
    u32 r5_new = r5 + r6;
    *(u32 *)((u8 *)r5_new + 0x120) = r4;
}
// NONMATCHING: MWCC allocates r31/r30/r29/r28 and emits stack frame; original uses r0/r5/r6
#pragma dont_inline reset

#pragma dont_inline on
extern "C" u32 fn_1_4B90(s16 *r3)
{
    s16 r0 = *r3;
    u32 r5 = r0 * 0x134;
    u32 r4 = (u32)Hu3DData;
    u32 r4_new = r4 + r5;
    return r4_new + 0xF0;
}
// NONMATCHING: MWCC allocates r31/r30/r29/r28 and emits stack frame; original uses r0/r4/r5
#pragma dont_inline reset

#pragma dont_inline on
extern "C" void fn_1_630C(u32 *r3, u32 *r4)
{
    *r3 = (u32)lbl_1_data_234;
    r4[0x5C / 4] = (u32)r3;
    r4[0x14 / 4] = (u32)fn_1_632C;
}
#pragma dont_inline reset

#pragma dont_inline on
extern "C" s32 fn_1_6CFC(void)
{
    return OSGetThreadPriority(OSGetCurrentThread());
}
#pragma dont_inline reset