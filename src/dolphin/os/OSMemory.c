#include <dolphin/os.h>

extern void __OSUnhandledException(u32 exception);
extern void OSRegisterResetFunction(OSResetFunctionInfo* info);
extern OSErrorHandler __OSErrorTable[];

#define MEM_BASE 0xCC004000

/* Layout of the memory-protection register block at 0xCC004000. The volatile
   belongs on the fields, not on the pointer: with `volatile MEMRegs *` MWCC
   re-materialises the address at every access instead of keeping the whole
   0xCC004000 in a callee-saved register, which is what the original does
   (r28 holds 0xCC004000 and the stores use 0x20(r28), 0x10(r28), 0x28(r28)). */
typedef struct {
    u8 pad10[0x10];
    volatile u16 unk10;   /* 0x10 */
    u8 pad12[0x0C];
    volatile u16 unk1E;   /* 0x1E */
    volatile u16 unk20;   /* 0x20 */
    volatile u16 unk22;   /* 0x22 */
    volatile u16 unk24;   /* 0x24 */
    u8 pad26[0x02];
    volatile u16 unk28;   /* 0x28 */
} MEMRegs;

u32 OSGetPhysicalMemSize(void)
{
    return *(u32*)0x80000028;
}

u32 OSGetConsoleSimulatedMemSize(void)
{
    return *(u32*)0x800000F0;
}

static BOOL OnReset(BOOL final)
{
    if (final) {
        *(volatile u16*)(MEM_BASE + 0x10) = 0xFF;
        __OSMaskInterrupts(0xF0000000);
    }
    return TRUE;
}

static void MEMIntrruptHandler(s16 interrupt, OSContext* context)
{
    u32 flags;

    flags = *(volatile u16*)(MEM_BASE + 0x22);
    flags |= *(volatile u16*)(MEM_BASE + 0x24) << 16;
    *(volatile u16*)(MEM_BASE + 0x20) = 0;

    if (__OSErrorTable[OS_ERROR_PROTECTION] != NULL) {
        __OSErrorTable[OS_ERROR_PROTECTION](OS_ERROR_PROTECTION, context, *(volatile u16*)(MEM_BASE + 0x1E), flags);
        return;
    }
    __OSUnhandledException(OS_ERROR_PROTECTION);
}

static OSResetFunctionInfo ResetFunctionInfo = { OnReset, 0x7F, NULL, NULL };

/* clang-format off */
static asm void Config24MB(void)
{
    nofralloc
    li      r7, 0x0
    lis     r4, 0x0
    addi    r4, r4, 0x2
    lis     r3, 0x8000
    addi    r3, r3, 0x1ff
    lis     r6, 0x100
    addi    r6, r6, 0x2
    lis     r5, 0x8100
    addi    r5, r5, 0xff
    isync
    mtdbatu 0, r7
    mtdbatl 0, r4
    mtdbatu 0, r3
    isync
    mtibatu 0, r7
    mtibatl 0, r4
    mtibatu 0, r3
    isync
    mtdbatu 2, r7
    mtdbatl 2, r6
    mtdbatu 2, r5
    isync
    mtibatu 2, r7
    mtibatl 2, r6
    mtibatu 2, r5
    isync
    mfmsr   r3
    ori     r3, r3, 0x30
    mtsrr1  r3
    mflr    r3
    mtsrr0  r3
    rfi
}

static asm void Config48MB(void)
{
    nofralloc
    li      r7, 0x0
    lis     r4, 0x0
    addi    r4, r4, 0x2
    lis     r3, 0x8000
    addi    r3, r3, 0x3ff
    lis     r6, 0x200
    addi    r6, r6, 0x2
    lis     r5, 0x8200
    addi    r5, r5, 0x1ff
    isync
    mtdbatu 0, r7
    mtdbatl 0, r4
    mtdbatu 0, r3
    isync
    mtibatu 0, r7
    mtibatl 0, r4
    mtibatu 0, r3
    isync
    mtdbatu 2, r7
    mtdbatl 2, r6
    mtdbatu 2, r5
    isync
    mtibatu 2, r7
    mtibatl 2, r6
    mtibatu 2, r5
    isync
    mfmsr   r3
    ori     r3, r3, 0x30
    mtsrr1  r3
    mflr    r3
    mtsrr0  r3
    rfi
}

static asm void RealMode(register u32 addr)
{
    nofralloc
    clrlwi  r3, r3, 2
    mtsrr0  r3
    mfmsr   r3
    rlwinm  r3, r3, 0, 28, 25
    mtsrr1  r3
    rfi
}
/* clang-format on */

void __OSInitMemoryProtection(void)
{
    u32 memSize;
    BOOL enabled;
    MEMRegs* regs;

    memSize = *(u32*)0x800000F0;
    enabled = OSDisableInterrupts();
    regs = (MEMRegs*)MEM_BASE;

    regs->unk20 = 0;
    regs->unk10 = 0xFF;
    __OSMaskInterrupts(0xF0000000);

    __OSSetInterruptHandler(OS_PROTECT_CHAN0, MEMIntrruptHandler);
    __OSSetInterruptHandler(OS_PROTECT_CHAN1, MEMIntrruptHandler);
    __OSSetInterruptHandler(OS_PROTECT_CHAN2, MEMIntrruptHandler);
    __OSSetInterruptHandler(OS_PROTECT_CHAN3, MEMIntrruptHandler);
    __OSSetInterruptHandler(OS_PROTECT_CHAN4, MEMIntrruptHandler);
    OSRegisterResetFunction(&ResetFunctionInfo);

    if (*(u32*)0x800000F0 < *(u32*)0x80000028 && *(u32*)0x800000F0 == 0x01800000) {
        DCInvalidateRange((void*)0x81800000, 0x01800000);
        regs->unk28 = 2;
    }

    if (memSize <= 0x01800000) {
        RealMode((u32)Config24MB);
    } else if (memSize <= 0x03000000) {
        RealMode((u32)Config48MB);
    }

    __OSUnmaskInterrupts(OS_INTERRUPTMASK_MEM_ADDRESS);
    OSRestoreInterrupts(enabled);
}
