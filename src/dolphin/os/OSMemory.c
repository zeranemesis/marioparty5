#include "dolphin/os.h"

#include "dolphin/os/OSCache.h"
#include "dolphin/os/OSError.h"
#include "dolphin/os/OSInterrupt.h"
#include "dolphin/os/OSReset.h"

#define EXI 0xCC004000

extern void OSRegisterResetFunction(OSResetFunctionInfo* info);
extern void __OSUnhandledException(OSError error, OSContext* context, ...);
extern OSErrorHandler __OSErrorTable[];

u32 OSGetPhysicalMemSize(void)
{
    return *(volatile u32*)0x80000028;
}

u32 OSGetConsoleSimulatedMemSize(void)
{
    return *(volatile u32*)0x800000F0;
}

static BOOL OnReset(BOOL final)
{
    if (!final) {
        *(volatile u16*)(EXI + 0x4010) = 0xFF;
        __OSMaskInterrupts(0xF000);
    }
    return TRUE;
}

static void MEMIntrruptHandler(__OSInterrupt interrupt, OSContext* context)
{
    u16 cause;

    cause = *(volatile u16*)(EXI + 0x4024);
    *(volatile u16*)(EXI + 0x4022) |= (u16)((cause << 10) & 0xFFC0);
    *(volatile u16*)(EXI + 0x4020) = 0;

    if (__OSErrorTable[OS_ERROR_SYSTEM_CALL] != NULL) {
        __OSErrorTable[OS_ERROR_SYSTEM_CALL](OS_ERROR_SYSTEM_CALL, context);
    } else {
        __OSUnhandledException(OS_ERROR_SYSTEM_CALL, context);
    }
}

static asm void Config24MB(void)
{
    nofralloc
    li r7, 0
    lis r4, 0
    addi r4, r4, 2
    lis r3, 0x8000
    addi r3, r3, 0x1ff
    lis r6, 0x100
    addi r6, r6, 2
    lis r5, 0x8100
    addi r5, r5, 0xff
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
    mfmsr r3
    ori r3, r3, 0x30
    mtsrr1 r3
    mflr r3
    mtsrr0 r3
    rfi
}

static asm void Config48MB(void)
{
    nofralloc
    li r7, 0
    lis r4, 0
    addi r4, r4, 2
    lis r3, 0x8000
    addi r3, r3, 0x3ff
    lis r6, 0x200
    addi r6, r6, 2
    lis r5, 0x8200
    addi r5, r5, 0x1ff
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
    mfmsr r3
    ori r3, r3, 0x30
    mtsrr1 r3
    mflr r3
    mtsrr0 r3
    rfi
}

static asm void RealMode(register u32 addr)
{
    nofralloc
    clrlwi r3, addr, 2
    mtsrr0 r3
    mfmsr r3
    rlwinm r3, r3, 0, 28, 25
    mtsrr1 r3
    rfi
}

static OSResetFunctionInfo ResetFunctionInfo = {OnReset, 0x7F, NULL, NULL};

void __OSInitMemoryProtection(void)
{
    BOOL flag;
    u32 simulated;

    simulated = OSGetConsoleSimulatedMemSize();

    flag = OSDisableInterrupts();

    *(volatile u16*)(EXI + 0x4020) = 0;
    *(volatile u16*)(EXI + 0x4010) = 0xFF;
    __OSMaskInterrupts(0xF000);

    __OSSetInterruptHandler(__OS_INTERRUPT_MEM_0, MEMIntrruptHandler);
    __OSSetInterruptHandler(__OS_INTERRUPT_MEM_1, MEMIntrruptHandler);
    __OSSetInterruptHandler(__OS_INTERRUPT_MEM_2, MEMIntrruptHandler);
    __OSSetInterruptHandler(__OS_INTERRUPT_MEM_3, MEMIntrruptHandler);
    __OSSetInterruptHandler(__OS_INTERRUPT_MEM_ADDRESS, MEMIntrruptHandler);

    OSRegisterResetFunction(&ResetFunctionInfo);

    if (OSGetConsoleSimulatedMemSize() < OSGetPhysicalMemSize()) {
        if (OSGetConsoleSimulatedMemSize() - 0x180 == 0) {
            DCInvalidateRange((void*)0x81800000, 0x180);
            *(volatile u16*)(EXI + 0x4028) = 2;
        }
    }

    if (simulated <= 0x180) {
        RealMode((u32)Config24MB);
    } else if (simulated <= 0x300) {
        RealMode((u32)Config48MB);
    }

    __OSUnmaskInterrupts(0x800);
    OSRestoreInterrupts(flag);
}