#include "dolphin/os.h"

#include "dolphin/os/OSCache.h"
#include "dolphin/os/OSError.h"
#include "dolphin/PPCArch.h"
#include "dolphin/db.h"

/* clang-format off */

/* The cache primitives are written with the 60x instructions directly: there is
   no way to express a counted block loop over them in C. */

#define SYNC() asm { sync }

asm void DCEnable(void)
{
    nofralloc
    sync
    mfspr r3, HID0
    ori r3, r3, 0x4000
    mtspr HID0, r3
    blr
}

asm void DCInvalidateRange(register void* address, register u32 nBytes)
{
    nofralloc
    cmplwi nBytes, 0
    blelr
    clrlwi r5, address, 27
    add r4, nBytes, r5
    addi r4, r4, 0x1f
    srwi r4, r4, 5
    mtctr r4
_block:
    dcbi r0, address
    addi address, address, 0x20
    bdnz _block
    blr
}

asm void DCFlushRange(register void* address, register u32 nBytes)
{
    nofralloc
    cmplwi nBytes, 0
    blelr
    clrlwi r5, address, 27
    add r4, nBytes, r5
    addi r4, r4, 0x1f
    srwi r4, r4, 5
    mtctr r4
_block:
    dcbf r0, address
    addi address, address, 0x20
    bdnz _block
    sc
    blr
}

asm void DCStoreRange(register void* address, register u32 nBytes)
{
    nofralloc
    cmplwi nBytes, 0
    blelr
    clrlwi r5, address, 27
    add r4, nBytes, r5
    addi r4, r4, 0x1f
    srwi r4, r4, 5
    mtctr r4
_block:
    dcbst r0, address
    addi address, address, 0x20
    bdnz _block
    sc
    blr
}

asm void DCFlushRangeNoSync(register void* address, register u32 nBytes)
{
    nofralloc
    cmplwi nBytes, 0
    blelr
    clrlwi r5, address, 27
    add r4, nBytes, r5
    addi r4, r4, 0x1f
    srwi r4, r4, 5
    mtctr r4
_block:
    dcbf r0, address
    addi address, address, 0x20
    bdnz _block
    blr
}

asm void DCStoreRangeNoSync(register void* address, register u32 nBytes)
{
    nofralloc
    cmplwi nBytes, 0
    blelr
    clrlwi r5, address, 27
    add r4, nBytes, r5
    addi r4, r4, 0x1f
    srwi r4, r4, 5
    mtctr r4
_block:
    dcbst r0, address
    addi address, address, 0x20
    bdnz _block
    blr
}

asm void DCZeroRange(register void* address, register u32 nBytes)
{
    nofralloc
    cmplwi nBytes, 0
    blelr
    clrlwi r5, address, 27
    add r4, nBytes, r5
    addi r4, r4, 0x1f
    srwi r4, r4, 5
    mtctr r4
_block:
    dcbz r0, address
    addi address, address, 0x20
    bdnz _block
    blr
}

asm void ICInvalidateRange(register void* address, register u32 nBytes)
{
    nofralloc
    cmplwi nBytes, 0
    blelr
    clrlwi r5, address, 27
    add r4, nBytes, r5
    addi r4, r4, 0x1f
    srwi r4, r4, 5
    mtctr r4
_block:
    icbi r0, address
    addi address, address, 0x20
    bdnz _block
    sync
    isync
    blr
}

asm void ICFlashInvalidate(void)
{
    nofralloc
    mfspr r3, HID0
    ori r3, r3, 0x800
    mtspr HID0, r3
    blr
}

asm void ICEnable(void)
{
    nofralloc
    isync
    mfspr r3, HID0
    ori r3, r3, 0x8000
    mtspr HID0, r3
    blr
}

/* Bring the locked cache up: block in the whole 1MB main memory through the
   normal cache, then switch the DMA address translation registers over to the
   locked cache window and zero it. */
static asm void __LCEnable(void)
{
    nofralloc
    mfmsr r5
    ori r5, r5, 0x1000
    mtmsr r5

    lis r3, 0x8000
    li r4, 0x400
    mtctr r4
_block:
    dcbt r0, r3
    dcbst r0, r3
    addi r3, r3, 0x20
    bdnz _block

    mfspr r4, HID2
    oris r4, r4, 0x100f
    mtspr HID2, r4

    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop

    lis r3, 0xe000
    ori r3, r3, 0x2
    mtdbatl 3, r3
    ori r3, r3, 0x1fe
    mtdbatu 3, r3
    isync

    lis r3, 0xe000
    li r6, 0x200
    mtctr r6
    li r6, 0x0
_block2:
    dcbz_l r6, r3
    addi r3, r3, 0x20
    bdnz _block2

    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop

    blr
}

void LCEnable(void)
{
    BOOL flag;

    flag = OSDisableInterrupts();
    __LCEnable();
    OSRestoreInterrupts(flag);
}

asm void LCDisable(void)
{
    nofralloc
    lis r3, 0xe000
    li r4, 0x200
    mtctr r4
_block:
    dcbi r0, r3
    addi r3, r3, 0x20
    bdnz _block
    mfspr r4, HID2
    rlwinm r4, r4, 0, 4, 2
    mtspr HID2, r4
    blr
}

asm void LCStoreBlocks(register void* destAddr, register void* srcTag, register u32 numBlocks)
{
    nofralloc
    extrwi r6, numBlocks, 5, 25
    clrlwi r3, destAddr, 4
    or r6, r6, r3
    mtspr DMA_U, r6
    clrlslwi r6, numBlocks, 30, 2
    or r6, r6, srcTag
    ori r6, r6, 0x2
    mtspr DMA_L, r6
    blr
}

u32 LCStoreData(void* destAddr, void* srcAddr, u32 nBytes)
{
    u32 left;
    u32 nPages;

    left = (nBytes + 31) >> 5;
    nPages = (left + 0x7f) >> 7;

    /* The DMA address space covers 4KB at a time, and the locked cache is only
       reachable in 128 block (4KB) pages. */
    while (left != 0) {
        if (left < 0x80) {
            LCStoreBlocks(destAddr, srcAddr, left);
            left = 0;
        } else {
            LCStoreBlocks(destAddr, srcAddr, 0);
            left -= 0x80;
            destAddr = (void*)((u8*)destAddr + 0x1000);
            srcAddr = (void*)((u8*)srcAddr + 0x1000);
        }
    }

    return nPages;
}

asm void LCQueueWait(register u32 len)
{
    nofralloc
_queue:
    mfspr r4, HID2
    extrwi r4, r4, 4, 4
    cmpw r4, len
    bgt _queue
    blr
}

void L2GlobalInvalidate(void)
{
    u32 l2cr;

    SYNC();
    l2cr = PPCMfl2cr() & 0x7fffffffu;
    PPCMtl2cr(l2cr);
    SYNC();

    l2cr = PPCMfl2cr() | 0x00200000;
    PPCMtl2cr(l2cr);

    /* Wait for the invalidate to complete (L2CR[LOAI] in progress). */
    while (PPCMfl2cr() & 0x1) {
    }

    l2cr = PPCMfl2cr() & (u32)(~0x200000);
    PPCMtl2cr(l2cr);

    /* If the invalidate is *still* running we are in trouble. */
    while (PPCMfl2cr() & 0x1) {
        DBPrintf(">>> L2 INVALIDATE : SHOULD NEVER HAPPEN\n");
    }
}

void DMAErrorHandler(OSError error, OSContext* context, ...)
{
    u32 hid2;

    hid2 = PPCMfhid2();

    OSReport("Machine check received\n");
    OSReport("HID2 = 0x%x   SRR1 = 0x%x\n", hid2, context->srr1);

    if ((hid2 & 0xf00000) == 0 || (context->srr1 & 0x200000) == 0) {
        OSReport("Machine check was not DMA/locked cache related\n");
        OSDumpContext(context);
        PPCHalt();
    }

    OSReport("DMAErrorHandler(): An error occurred while processing DMA.\n");
    OSReport("The following errors have been detected and cleared :\n");

    if (hid2 & HID2_DCHERR) {
        OSReport("\t- Requested a locked cache tag that was already in the cache\n");
    }
    if (hid2 & HID2_DNCERR) {
        OSReport("\t- DMA attempted to access normal cache\n");
    }
    if (hid2 & HID2_DCMERR) {
        OSReport("\t- DMA missed in data cache\n");
    }
    if (hid2 & HID2_DQOERR) {
        OSReport("\t- DMA queue overflowed\n");
    }

    PPCMthid2(hid2);
}

void __OSCacheInit(void)
{
    u32 hid0;
    u32 msr;
    u32 flag;

    hid0 = PPCMfhid0();
    if (!(hid0 & (1 << 15))) {
        ICEnable();
        DBPrintf("L1 i-caches initialized\n");
    }

    hid0 = PPCMfhid0();
    if (!(hid0 & (1 << 14))) {
        DCEnable();
        DBPrintf("L1 d-caches initialized\n");
    }

    flag = PPCMfl2cr();
    if (flag == 0) {
        msr = PPCMfmsr();
        SYNC();
        PPCMtmsr(0x30);
        SYNC();
        SYNC();
        PPCMtl2cr(PPCMfl2cr() & 0x7fffffffu);
        SYNC();
        L2GlobalInvalidate();
        PPCMtmsr(msr);
        PPCMtl2cr((PPCMfl2cr() | 0x80000000u) & (u32)(~0x200000));
        DBPrintf("L2 cache initialized\n");
    }

    OSSetErrorHandler(OS_ERROR_MACHINE_CHECK, DMAErrorHandler);
    DBPrintf("Locked cache machine check handler installed\n");
}

/* clang-format on */
