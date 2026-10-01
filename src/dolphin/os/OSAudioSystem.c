#include "dolphin/os.h"

#include "dolphin/os/OSCache.h"
#include "dolphin/os/OSArena.h"

/* DSP mailbox / AI registers live in the PI block */
#define PI 0xCC003000
#define DSP_MAILBOX (*(volatile u16*)(PI + 0x5000))
#define DSP_IMEM0 (*(volatile u16*)(PI + 0x5004))
#define DSP_IMEM1 (*(volatile u16*)(PI + 0x5006))
#define DSP_IMEM24 (*(volatile u16*)(PI + 0x500A))
#define DSP_DMEM0P (*(volatile u32*)(PI + 0x5020))
#define DSP_DMEM1P (*(volatile u32*)(PI + 0x5024))
#define DSP_DMEM8P (*(volatile u32*)(PI + 0x5028))
#define DSP_CPURUN (*(volatile u16*)(PI + 0x5036))

#define DSP_CODE 0x81000000

/* The DSP boot stub, 0x80 bytes, copied to 0x81000000 at reset. */
static const u16 DSPInitCode[] = {
    0x0010, 0x0033, 0x0034, 0x0035, 0x0036, 0x0037, 0x0038, 0x0039, 0x1203, 0x1205, 0x8000, 0xFFFF,
    0x1000, 0x001D, 0x0000, 0x1C1E, 0x1B1E, 0x0800, 0x0027, 0x0000, 0xFFFC, 0x8000, 0x0028, 0x0054,
    0x4348, 0x02FF, 0x02FF, 0x02FF, 0x02FF, 0x0000, 0x0000, 0x0000,
};

void __OSInitAudioSystem(void)
{
    register u32 tick;
    register volatile u16* imem0 = (volatile u16*)(PI + 0x5004);
    register volatile u16* imem1 = (volatile u16*)(PI + 0x5006);
    register volatile u16* mailbox = (volatile u16*)(PI + 0x5000);
    register volatile u32* dmem0p = (volatile u32*)(PI + 0x5020);
    register volatile u32* dmem1p = (volatile u32*)(PI + 0x5024);
    register volatile u32* dmem8p = (volatile u32*)(PI + 0x5028);
    register volatile u16* cputype = (volatile u16*)(PI + 0x500A);

    memcpy((void*)DSP_CODE, (void*)((u32)OSGetArenaHi() - 0x80), 0x80);
    memcpy((void*)DSP_CODE, DSPInitCode, 0x80);
    DCFlushRange((void*)DSP_CODE, 0x80);

    /* Reset and configure the DSP */
    *cputype = 0x8AC;
    *cputype |= 0x1;

    while (*mailbox & 0x8000) {
    }
    *mailbox = 0;

    while ((*imem0 | ((*imem1 & 0xFFFF) << 16)) & 0x80000000) {
    }

    *dmem0p = 0x100;
    *dmem1p = 0;
    *dmem8p = 0x20;

    /* Wait for the reset to complete */
    while (!(*imem0 & (1 << 6))) {
    }
    *imem0 = *imem0;
    tick = OSGetTick();
    while (OSGetTick() - tick < 0x892) {
    }

    *dmem0p = 0x100;
    *dmem1p = 0;
    *dmem8p = 0x20;

    /* Acknowledge the boot handshake */
    while (!(*imem0 & (1 << 6))) {
    }
    *imem0 = *imem0;
    *imem0 &= ~((7) << 19);

    while (*imem0 & (1 << 21)) {
    }
    *imem0 &= ~((7) << 2);

    while (*mailbox & (1 << 16)) {
    }
    *mailbox |= 0x4;

    *cputype = 0x8AC;
    *cputype |= 0x1;
    while (*cputype & 0x8000) {
    }

    memcpy((void*)DSP_CODE, (void*)((u32)OSGetArenaHi() - 0x80), 0x80);
}

void __OSStopAudioSystem(void)
{
    register u32 tick;
    register volatile u16* imem0 = (volatile u16*)(PI + 0x5004);
    register volatile u16* mailbox = (volatile u16*)(PI + 0x5000);
    register volatile u16* imem1 = (volatile u16*)(PI + 0x5006);
    register volatile u16* cpurun = (volatile u16*)(PI + 0x5036);
    register volatile u16* cputype = (volatile u16*)(PI + 0x500A);

    *cputype = 0x804;
    *cpurun &= ~(1 << 15);

    while (*imem0 & (1 << 21)) {
    }
    while (*imem0 & (1 << 22)) {
    }

    *imem0 = 0x8AC;
    *mailbox = 0;

    while ((*imem0 | ((*imem1 & 0xFFFF) << 16)) & 0x80000000) {
    }

    tick = OSGetTick();
    while (OSGetTick() - tick < 0x2C) {
    }

    *imem0 |= 0x1;
    while (*imem0 & 0x8000) {
    }
}