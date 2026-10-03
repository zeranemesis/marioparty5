#include "dolphin/os.h"

#include "string.h"

/* The DSP's CPU side register block at 0xCC005000. Only the words the init
 * and stop sequences touch are named; the rest is padding. Every field is
 * volatile because the DSP reads and writes the block asynchronously. */
typedef struct DSPInitRegs_s {
    /* 0x00 */ volatile u16 init_vector;
    /* 0x02 */ u16 pad02;
    /* 0x04 */ volatile s16 mmem_hi;
    /* 0x06 */ volatile s16 mmem_lo;
    /* 0x08 */ u16 pad08;
    /* 0x0A */ volatile u16 init_vector2;
    /* 0x0C */ u16 pad0C[3];
    /* 0x12 */ volatile u16 cpucode;
    /* 0x14 */ u16 pad14[0x0C / 2];
    /* 0x20 */ volatile u32 dsp_mail;
    /* 0x24 */ volatile u32 pad24;
    /* 0x28 */ volatile u32 pad28;
    /* 0x2C */ u16 pad2C[0xA / 2];
    /* 0x36 */ volatile u16 resume_vector;
} DSPINITREGS;

#define DSP_INIT_ADDRESS 0xCC005000
#define ARENA_SAVE_ADDRESS 0x81000000
#define ARENA_SAVE_SIZE 0x80

/* Copied to ARENA_SAVE_ADDRESS and run by the DSP: a 32 bit word of DSP code
 * interleaved with immediate data. */
static u32 DSPInitCode[] = {
    0x029F0010, 0x029F0033, 0x029F0034, 0x029F0035, 0x029F0036, 0x029F0037,
    0x029F0038, 0x029F0039, 0x12061203, 0x12041205, 0x00808000, 0x0088FFFF,
    0x00841000, 0x0064001D, 0x02180000, 0x81001C1E, 0x00441B1E, 0x00840800,
    0x00640027, 0x191E0000, 0x00DEFFFC, 0x02A08000, 0x029C0028, 0x16FC0054,
    0x16FD4348, 0x002102FF, 0x02FF02FF, 0x02FF02FF, 0x02FF02FF, 0x00000000,
    0x00000000, 0x00000000,
};

void __OSInitAudioSystem(void)
{
    // NONMATCHING: register allocation. The target keeps the full base
    // 0xCC005000 live in r3 and derives vec as addi r31,r3,0xa, so every
    // access is a small displacement off one register. Here MWCC folds the
    // base to 0xcc00 and re-materialises a displacement on each access
    // (sth r0, 0x500a(r3)), which needs 3 callee-saved registers instead of
    // 7. Same instruction count (125), same semantics, different registers.
    DSPINITREGS *regs;
    volatile u16 *vec;
    volatile s16 *mmem;
    u32 count;
    u32 start;
    u8 *arenaHi;

    arenaHi = OSGetArenaHi();
    memcpy(arenaHi - ARENA_SAVE_SIZE, (void *)ARENA_SAVE_ADDRESS, ARENA_SAVE_SIZE);
    memcpy((void *)ARENA_SAVE_ADDRESS, DSPInitCode, ARENA_SAVE_SIZE);
    DCFlushRange((void *)ARENA_SAVE_ADDRESS, ARENA_SAVE_SIZE);

    regs = (DSPINITREGS *)DSP_INIT_ADDRESS;
    regs->cpucode = 0x43;

    vec = &regs->init_vector2;
    mmem = &regs->mmem_hi;

    *vec = 0x8AC;
    *vec |= 1;
    while (*vec & 1)
        ;

    regs->init_vector = 0;
    do {
        count = ((u32)mmem[0] << 16) | (u32)mmem[1];
    } while (count & 0x80000000);

    regs->dsp_mail = 0x100;
    regs->pad24 = 0;
    regs->pad28 = 0x20;

    while (!(*vec & 0x4000000))
        ;
    *vec = *vec;

    start = (u32)OSGetTick();
    while ((s32)((u32)OSGetTick() - start) < 0x892)
        ;

    regs->dsp_mail = 0x100;
    regs->pad24 = 0;
    regs->pad28 = 0x20;

    while (!(*vec & 0x4000000))
        ;
    *vec = *vec;

    *vec &= 0xFFEFFFFF;
    while (*vec & 0x200000)
        ;
    *vec &= 0xDFFFFFFF;

    while (!(*(volatile u32 *)mmem & 0x10000))
        ;

    *vec |= 4;
    *vec = 0x8AC;
    *vec |= 1;
    while (*vec & 1)
        ;

    arenaHi = OSGetArenaHi();
    memcpy((void *)ARENA_SAVE_ADDRESS, arenaHi - ARENA_SAVE_SIZE, ARENA_SAVE_SIZE);
}

void __OSStopAudioSystem(void)
{
    // NONMATCHING: as above, the base pointer is not materialised in a
    // register, so the address is rebuilt from 0xcc00 on each use.
    DSPINITREGS *regs;
    volatile u16 *vec;
    u32 count;
    u32 start;

    regs = (DSPINITREGS *)DSP_INIT_ADDRESS;
    vec = &regs->init_vector2;

    *vec = 0x804;
    regs->resume_vector &= 0xFFFEFFFF;

    while (*vec & 0x200000)
        ;
    while (*vec & 0x400000)
        ;

    *vec = 0x8AC;
    regs->init_vector = 0;
    do {
        count = ((u32)regs->mmem_hi << 16) | (u32)regs->mmem_lo;
    } while (count & 0x80000000);

    start = (u32)OSGetTick();
    while ((s32)((u32)OSGetTick() - start) < 0x2C)
        ;

    *vec |= 1;
    while (*vec & 1)
        ;
}
