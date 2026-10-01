#include "dolphin/os.h"

#include "dolphin/os/OSError.h"
#include "dolphin/os/OSInterrupt.h"

#define HW_CPU 0xCC000000
#define HW_PI 0xCC003000
#define HW_EXI 0xCC004000
#define HW_SI 0xCC006800
#define HW_DSP 0xCC006C00

#define CPU_SRR0 (*(volatile u32*)(HW_CPU + 0x3000))
#define CPU_SRR1 (*(volatile u32*)(HW_CPU + 0x3004))
#define CPU_SRR (*(volatile u32*)(HW_CPU + 0x3010))

#define PI_STATUS (*(volatile u32*)(HW_PI + 0x0004))
#define PI_IMASK (*(volatile u32*)(HW_PI + 0x000C))

#define EXI0_STAT (*(volatile u16*)(HW_EXI + 0x001E))
#define EXI0_MASK (*(volatile u16*)(HW_EXI + 0x001C))
#define EXI1_STAT (*(volatile u16*)(HW_EXI + 0x012E))
#define EXI1_MASK (*(volatile u16*)(HW_EXI + 0x012C))
#define EXI2_STAT (*(volatile u16*)(HW_EXI + 0x023E))
#define EXI2_MASK (*(volatile u16*)(HW_EXI + 0x023C))

#define CP_STATUS (*(volatile u32*)(HW_CPU + 0x3010))

#define SI_STATUS (*(volatile u32*)(HW_SI + 0x0000))

extern void __OSSetExceptionHandler(u32 code, OSErrorHandler handler);
extern void __OSReschedule(void);
extern s32 OSDisableScheduler(void);
extern s32 OSEnableScheduler(void);
extern OSTime OSGetTime(void);

void OSLoadContext(OSContext* context);

static void ExternalInterruptHandler(u16 exception, OSContext* context);

/* Index of the highest set bit: interrupts are dispatched most significant
   first. Declared with `register` so MWCC substitutes the caller's register
   and never materialises a real call. */
#define __OSInterruptBit(value, out) asm { cntlzw out, value }

static const OSInterruptMask InterruptPrioTable[] = {
    0x00000100u, 0x00000040u, 0xF8000000u, 0x00000200u, 0x00000080u, 0x00003000u,
    0x00000020u, 0x03FF8C00u, 0x04000000u, 0x00004000u, 0xFFFFFFFFu,
};

static void** InterruptHandlerTable;

asm BOOL OSDisableInterrupts(void)
{
    nofralloc
    mfmsr r3
    rlwinm r4, r3, 0, 17, 15
    mtmsr r4
    extrwi r3, r3, 1, 16
    blr
}

asm BOOL OSEnableInterrupts(void)
{
    nofralloc
    mfmsr r3
    ori r4, r3, 0x8000
    mtmsr r4
    extrwi r3, r3, 1, 16
    blr
}

asm BOOL OSRestoreInterrupts(BOOL flag)
{
    nofralloc
    cmpwi r3, 0
    mfmsr r4
    beq _disable
    ori r5, r4, 0x8000
    b _set
_disable:
    rlwinm r5, r4, 0, 17, 15
_set:
    mtmsr r5
    extrwi r3, r4, 1, 16
    blr
}

__OSInterruptHandler __OSSetInterruptHandler(__OSInterrupt interrupt, __OSInterruptHandler handler)
{
    void** slot;
    __OSInterruptHandler old;

    slot = (void**)&InterruptHandlerTable[interrupt];
    old = (__OSInterruptHandler)*slot;
    *slot = (void*)handler;

    return old;
}

__OSInterruptHandler __OSGetInterruptHandler(__OSInterrupt interrupt)
{
    return (__OSInterruptHandler)InterruptHandlerTable[interrupt];
}

void __OSInterruptInit(void)
{
    memset(InterruptHandlerTable, 0, 32 * sizeof(void*));

    *(volatile u32*)0xCC003004 = 0;
    *(volatile u32*)0x800000C4 = 0;
    *(volatile u32*)0x800000C8 = 0;

    PI_STATUS = 0xF0;
    __OSMaskInterrupts(0xFFFFFFE0);

    __OSSetExceptionHandler(OS_ERROR_EXTERNAL_INTERRUPT, (OSErrorHandler)ExternalInterruptHandler);
}

static void SetInterruptMask(register OSInterruptMask mask);

static void SetInterruptMask(register OSInterruptMask mask)
{
    /* The hardware registers keep the *disabled* interrupts, so the caller
       mask has to be inverted bit by bit. */
    register s32 bit;

    __OSInterruptBit(mask, bit);
    switch (bit) {
    case __OS_INTERRUPT_MEM_0:
        EXI0_MASK = ((~(mask >> 0)) & OS_INTERRUPTMASK_MEM_0) |
                    ((~(mask >> 1)) & (OS_INTERRUPTMASK_MEM_0 >> 1)) |
                    ((~(mask >> 2)) & (OS_INTERRUPTMASK_MEM_0 >> 2)) |
                    ((~(mask >> 3)) & (OS_INTERRUPTMASK_MEM_0 >> 3)) |
                    ((~(mask >> 4)) & (OS_INTERRUPTMASK_MEM_0 >> 4));
        mask &= ~(OS_INTERRUPTMASK_MEM);
        break;
    case __OS_INTERRUPT_DSP_AI:
        PI_IMASK = ((PI_IMASK & 0xFFC00000u) | ((~(mask >> 5)) & 0x10u) |
                    ((~(mask >> 6)) & 0x40u) | ((~(mask >> 7)) & 0x100u));
        mask &= ~(OS_INTERRUPTMASK_DSP);
        break;
    case __OS_INTERRUPT_AI_AI:
        *(volatile u32*)(HW_DSP) = (*(volatile u32*)(HW_DSP) & ~0x2Cu) | (~(mask >> 8) & 0x4u);
        mask &= ~(1 << 8);
        break;
    case __OS_INTERRUPT_EXI_0_EXI:
        *(volatile u32*)(HW_SI + 0x00) =
            (*(volatile u32*)(HW_SI + 0x00) & 0xFFFFD3F0u) | (~(mask >> 9) & 0x1u) |
            (~(mask >> 10) & 0x4u) | (~(mask >> 11) & 0x400u);
        mask &= ~0xE00u;
        break;
    case __OS_INTERRUPT_EXI_1_EXI:
        *(volatile u32*)(HW_SI + 0x14) =
            (*(volatile u32*)(HW_SI + 0x14) & 0xFFFFF3F0u) | (~(mask >> 12) & 0x1u) |
            (~(mask >> 13) & 0x4u) | (~(mask >> 14) & 0x400u);
        mask &= ~0x7000u;
        break;
    case __OS_INTERRUPT_EXI_2_EXI:
        *(volatile u32*)(HW_SI + 0x28) =
            (*(volatile u32*)(HW_SI + 0x28) & 0xF0000000u) | (~(mask >> 15) & 0x1u) |
            (~(mask >> 16) & 0x4u);
        mask &= ~0x18000u;
        break;
    case __OS_INTERRUPT_PI_CP:
        CPU_SRR1 = (~(mask >> 17) & 0x800u) | 0xF0u | (~(mask >> 20) & 0x8u) |
                   (~(mask >> 21) & 0x4u) | (~(mask >> 22) & 0x2u) | (~(mask >> 23) & 0x1u) |
                   (~(mask >> 24) & 0x100u) | (~(mask >> 25) & 0x1000u) |
                   (~(mask >> 18) & 0x200u) | (~(mask >> 19) & 0x400u) |
                   (~(mask >> 26) & 0x2000u);
        mask &= ~0x7FE0000u;
        break;
    }
}

OSInterruptMask __OSMaskInterrupts(OSInterruptMask mask)
{
    OSInterruptMask save;

    OSDisableInterrupts();

    mask &= ~(*(volatile u32*)0x800000C4 | *(volatile u32*)0x800000C8);
    save = *(volatile u32*)0x800000C4;
    save |= mask;
    *(volatile u32*)0x800000C4 = save;
    save |= *(volatile u32*)0x800000C8;

    SetInterruptMask(save);

    OSRestoreInterrupts(0);

    return save;
}

OSInterruptMask __OSUnmaskInterrupts(OSInterruptMask mask)
{
    OSInterruptMask save;

    OSDisableInterrupts();

    mask &= (*(volatile u32*)0x800000C4 | *(volatile u32*)0x800000C8);
    save = *(volatile u32*)0x800000C4;
    save &= ~mask;
    *(volatile u32*)0x800000C4 = save;
    save |= *(volatile u32*)0x800000C8;

    SetInterruptMask(save);

    OSRestoreInterrupts(0);

    return save;
}

void __OSDispatchInterrupt(__OSException exception, OSContext* context)
{
    register OSInterruptMask mask;
    __OSInterrupt interrupt;
    __OSInterruptHandler handler;
    register u32 status;
    register OSInterruptMask sel;

    mask = 0;

    status = CPU_SRR0;
    if ((status & 0x00030000) != 0 && (status & PI_STATUS) == 0) {
        OSLoadContext(context);
    }

    if (status & 0x01000000) {
        mask |= EXI0_STAT & 0xFC00;
    }
    if (status & 0x02000000) {
        mask |= (PI_STATUS & 0x0F00) >> 2;
    }
    if ((status & 0x04000000) && (*(volatile u32*)HW_DSP & 0x10000000)) {
        mask |= 0x80;
    }
    if (status & 0x08000000) {
        mask |= (SI_STATUS & 0x50100000) & 0x7C;
        mask |= (*(volatile u32*)(HW_SI + 0x14) & 0x50100000) & 0xE;
        mask |= (*(volatile u32*)(HW_SI + 0x28) & 0x50000000) & 0x8005;
    }
    mask |= status & 0x0FBC6020u;

    mask &= ~(*(volatile u32*)0x800000C4 | *(volatile u32*)0x800000C8);

    for (interrupt = 0; ; interrupt++) {
        register s32 bit;

        sel = mask & InterruptPrioTable[interrupt];
        if (sel != 0) {
            __OSInterruptBit(sel, bit);
            interrupt = (__OSInterrupt)bit;
            break;
        }
    }

    handler = (__OSInterruptHandler)InterruptHandlerTable[interrupt];
    if (handler != NULL) {
        if (interrupt > 4) {
            __OSLastInterrupt = interrupt;
            __OSLastInterruptTime = OSGetTime();
            __OSLastInterruptSrr0 = context->srr0;
        }

        OSDisableScheduler();
        handler(interrupt, context);
        OSEnableScheduler();
        __OSReschedule();
        OSLoadContext(context);
    }

    OSLoadContext(context);
}

static asm void ExternalInterruptHandler(u16 exception, OSContext* context)
{
    nofralloc
    stw r0, 0x0(r4)
    stw r1, 0x4(r4)
    stw r2, 0x8(r4)
    stmw r6, 0x18(r4)
    mfspr r0, GQR1
    stw r0, 0x1a8(r4)
    mfspr r0, GQR2
    stw r0, 0x1ac(r4)
    mfspr r0, GQR3
    stw r0, 0x1b0(r4)
    mfspr r0, GQR4
    stw r0, 0x1b4(r4)
    mfspr r0, GQR5
    stw r0, 0x1b8(r4)
    mfspr r0, GQR6
    stw r0, 0x1bc(r4)
    mfspr r0, GQR7
    stw r0, 0x1c0(r4)
    stwu r1, -0x8(r1)
    b __OSDispatchInterrupt
}