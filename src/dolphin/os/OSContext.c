#include "dolphin/os.h"

#include "dolphin/db.h"
#include "dolphin/os/OSContext.h"
#include "dolphin/os/OSError.h"

#define OS_CONTEXT_SRR1_FPU 0x2000
#define OS_CONTEXT_SRR1_FPU_MASK 0xA0000

/* Low-memory globals driving the context switcher */
#define OS_CTX_SAVE_AREA (*(OSContext**)0x800000C0)
#define OS_CTX_CURRENT (*(OSContext**)0x800000D4)
#define OS_CTX_LAST_FPU (*(OSContext**)0x800000D8)

extern void __OSSetExceptionHandler(u32 code, OSErrorHandler handler);

static void __OSLoadFPUContext(OSContext* context)
{
    register OSContext* ctx = context;
    register u32 psf;

    if (context->state & OS_CONTEXT_STATE_FPSAVED) {
        asm {
            lfd f0, 0x190(ctx)
            mtfsf 255, f0
            mfspr psf, HID2
        }
        if (psf & 2) {
            asm {
                psq_l f0, 0x1c8(ctx), 0, 0
                psq_l f1, 0x1d0(ctx), 0, 0
                psq_l f2, 0x1d8(ctx), 0, 0
                psq_l f3, 0x1e0(ctx), 0, 0
                psq_l f4, 0x1e8(ctx), 0, 0
                psq_l f5, 0x1f0(ctx), 0, 0
                psq_l f6, 0x1f8(ctx), 0, 0
                psq_l f7, 0x200(ctx), 0, 0
                psq_l f8, 0x208(ctx), 0, 0
                psq_l f9, 0x210(ctx), 0, 0
                psq_l f10, 0x218(ctx), 0, 0
                psq_l f11, 0x220(ctx), 0, 0
                psq_l f12, 0x228(ctx), 0, 0
                psq_l f13, 0x230(ctx), 0, 0
                psq_l f14, 0x238(ctx), 0, 0
                psq_l f15, 0x240(ctx), 0, 0
                psq_l f16, 0x248(ctx), 0, 0
                psq_l f17, 0x250(ctx), 0, 0
                psq_l f18, 0x258(ctx), 0, 0
                psq_l f19, 0x260(ctx), 0, 0
                psq_l f20, 0x268(ctx), 0, 0
                psq_l f21, 0x270(ctx), 0, 0
                psq_l f22, 0x278(ctx), 0, 0
                psq_l f23, 0x280(ctx), 0, 0
                psq_l f24, 0x288(ctx), 0, 0
                psq_l f25, 0x290(ctx), 0, 0
                psq_l f26, 0x298(ctx), 0, 0
                psq_l f27, 0x2a0(ctx), 0, 0
                psq_l f28, 0x2a8(ctx), 0, 0
                psq_l f29, 0x2b0(ctx), 0, 0
                psq_l f30, 0x2b8(ctx), 0, 0
                psq_l f31, 0x2c0(ctx), 0, 0
            }
        }
    }

    asm {
        lfd f0, 0x90(ctx)
        lfd f1, 0x98(ctx)
        lfd f2, 0xa0(ctx)
        lfd f3, 0xa8(ctx)
        lfd f4, 0xb0(ctx)
        lfd f5, 0xb8(ctx)
        lfd f6, 0xc0(ctx)
        lfd f7, 0xc8(ctx)
        lfd f8, 0xd0(ctx)
        lfd f9, 0xd8(ctx)
        lfd f10, 0xe0(ctx)
        lfd f11, 0xe8(ctx)
        lfd f12, 0xf0(ctx)
        lfd f13, 0xf8(ctx)
        lfd f14, 0x100(ctx)
        lfd f15, 0x108(ctx)
        lfd f16, 0x110(ctx)
        lfd f17, 0x118(ctx)
        lfd f18, 0x120(ctx)
        lfd f19, 0x128(ctx)
        lfd f20, 0x130(ctx)
        lfd f21, 0x138(ctx)
        lfd f22, 0x140(ctx)
        lfd f23, 0x148(ctx)
        lfd f24, 0x150(ctx)
        lfd f25, 0x158(ctx)
        lfd f26, 0x160(ctx)
        lfd f27, 0x168(ctx)
        lfd f28, 0x170(ctx)
        lfd f29, 0x178(ctx)
        lfd f30, 0x180(ctx)
        lfd f31, 0x188(ctx)
    }
}

static void __OSSaveFPUContext(OSContext* context)
{
    register OSContext* ctx = context;
    register u32 psf;

    ctx->state |= OS_CONTEXT_STATE_FPSAVED;

    asm {
        stfd f0, 0x90(ctx)
        stfd f1, 0x98(ctx)
        stfd f2, 0xa0(ctx)
        stfd f3, 0xa8(ctx)
        stfd f4, 0xb0(ctx)
        stfd f5, 0xb8(ctx)
        stfd f6, 0xc0(ctx)
        stfd f7, 0xc8(ctx)
        stfd f8, 0xd0(ctx)
        stfd f9, 0xd8(ctx)
        stfd f10, 0xe0(ctx)
        stfd f11, 0xe8(ctx)
        stfd f12, 0xf0(ctx)
        stfd f13, 0xf8(ctx)
        stfd f14, 0x100(ctx)
        stfd f15, 0x108(ctx)
        stfd f16, 0x110(ctx)
        stfd f17, 0x118(ctx)
        stfd f18, 0x120(ctx)
        stfd f19, 0x128(ctx)
        stfd f20, 0x130(ctx)
        stfd f21, 0x138(ctx)
        stfd f22, 0x140(ctx)
        stfd f23, 0x148(ctx)
        stfd f24, 0x150(ctx)
        stfd f25, 0x158(ctx)
        stfd f26, 0x160(ctx)
        stfd f27, 0x168(ctx)
        stfd f28, 0x170(ctx)
        stfd f29, 0x178(ctx)
        stfd f30, 0x180(ctx)
        stfd f31, 0x188(ctx)
        mffs f0
        stfd f0, 0x190(ctx)
        lfd f0, 0x90(ctx)
        mfspr psf, HID2
    }
    if (psf & 2) {
        asm {
            psq_st f0, 0x1c8(ctx), 0, 0
            psq_st f1, 0x1d0(ctx), 0, 0
            psq_st f2, 0x1d8(ctx), 0, 0
            psq_st f3, 0x1e0(ctx), 0, 0
            psq_st f4, 0x1e8(ctx), 0, 0
            psq_st f5, 0x1f0(ctx), 0, 0
            psq_st f6, 0x1f8(ctx), 0, 0
            psq_st f7, 0x200(ctx), 0, 0
            psq_st f8, 0x208(ctx), 0, 0
            psq_st f9, 0x210(ctx), 0, 0
            psq_st f10, 0x218(ctx), 0, 0
            psq_st f11, 0x220(ctx), 0, 0
            psq_st f12, 0x228(ctx), 0, 0
            psq_st f13, 0x230(ctx), 0, 0
            psq_st f14, 0x238(ctx), 0, 0
            psq_st f15, 0x240(ctx), 0, 0
            psq_st f16, 0x248(ctx), 0, 0
            psq_st f17, 0x250(ctx), 0, 0
            psq_st f18, 0x258(ctx), 0, 0
            psq_st f19, 0x260(ctx), 0, 0
            psq_st f20, 0x268(ctx), 0, 0
            psq_st f21, 0x270(ctx), 0, 0
            psq_st f22, 0x278(ctx), 0, 0
            psq_st f23, 0x280(ctx), 0, 0
            psq_st f24, 0x288(ctx), 0, 0
            psq_st f25, 0x290(ctx), 0, 0
            psq_st f26, 0x298(ctx), 0, 0
            psq_st f27, 0x2a0(ctx), 0, 0
            psq_st f28, 0x2a8(ctx), 0, 0
            psq_st f29, 0x2b0(ctx), 0, 0
            psq_st f30, 0x2b8(ctx), 0, 0
            psq_st f31, 0x2c0(ctx), 0, 0
        }
    }
}

void OSSaveFPUContext(OSContext* context)
{
    __OSSaveFPUContext(context);
}

void OSSetCurrentContext(OSContext* context)
{
    register u32 msr;

    OS_CTX_CURRENT = context;
    OS_CTX_SAVE_AREA = (OSContext*)((u32)context & ~3);

    if ((u32)OS_CTX_LAST_FPU == (u32)context) {
        context->srr1 |= OS_CONTEXT_SRR1_FPU;
        asm {
            mfmsr msr
            ori msr, msr, 2
            mtmsr msr
        }
    } else {
        context->srr1 &= ~OS_CONTEXT_SRR1_FPU_MASK;
        asm {
            mfmsr msr
            rlwinm msr, msr, 0, 19, 17
            ori msr, msr, 2
            mtmsr msr
            isync
        }
    }
}

OSContext* OSGetCurrentContext(void)
{
    return OS_CTX_CURRENT;
}

u32 OSSaveContext(OSContext* context)
{
    register OSContext* ctx = context;

    asm {
        stmw r13, 0x34(ctx)
        mfspr r0, GQR1
        stw r0, 0x1a8(ctx)
        mfspr r0, GQR2
        stw r0, 0x1ac(ctx)
        mfspr r0, GQR3
        stw r0, 0x1b0(ctx)
        mfspr r0, GQR4
        stw r0, 0x1b4(ctx)
        mfspr r0, GQR5
        stw r0, 0x1b8(ctx)
        mfspr r0, GQR6
        stw r0, 0x1bc(ctx)
        mfspr r0, GQR7
        stw r0, 0x1c0(ctx)
        mfcr r0
        stw r0, 0x80(ctx)
        mflr r0
        stw r0, 0x84(ctx)
        stw r0, 0x198(ctx)
        mfmsr r0
        stw r0, 0x19c(ctx)
        mfctr r0
        stw r0, 0x88(ctx)
        mfxer r0
        stw r0, 0x8c(ctx)
        stw r1, 0x4(ctx)
        stw r2, 0x8(ctx)
        li r0, 1
        stw r0, 0xc(ctx)
        li r3, 0
    }

    return 0;
}

void OSLoadContext(OSContext* context)
{
    register OSContext* ctx = context;

    if ((u32)ctx->srr0 > (u32)OSDisableInterrupts) {
        if ((u32)ctx->srr0 >= (u32)OSDisableInterrupts + 0xC) {
            ctx->srr0 = (u32)OSDisableInterrupts;
        }
    }

    asm {
        lwz r0, 0x0(ctx)
        lwz r1, 0x4(ctx)
        lwz r2, 0x8(ctx)
        lhz r4, 0x1a2(ctx)
        rlwinm. r5, r4, 0, 30, 30
        beq _no_fpu
        rlwinm r4, r4, 0, 31, 29
        sth r4, 0x1a2(ctx)
        lmw r5, 0x14(ctx)
        b _gprs_done
    _no_fpu:
        lmw r13, 0x34(ctx)
    _gprs_done:
        lwz r4, 0x1a8(ctx)
        mtspr GQR1, r4
        lwz r4, 0x1ac(ctx)
        mtspr GQR2, r4
        lwz r4, 0x1b0(ctx)
        mtspr GQR3, r4
        lwz r4, 0x1b4(ctx)
        mtspr GQR4, r4
        lwz r4, 0x1b8(ctx)
        mtspr GQR5, r4
        lwz r4, 0x1bc(ctx)
        mtspr GQR6, r4
        lwz r4, 0x1c0(ctx)
        mtspr GQR7, r4
        lwz r4, 0x80(ctx)
        mtcrf 255, r4
        lwz r4, 0x84(ctx)
        mtlr r4
        lwz r4, 0x88(ctx)
        mtctr r4
        lwz r4, 0x8c(ctx)
        mtxer r4
        mfmsr r4
        rlwinm r4, r4, 0, 17, 15
        rlwinm r4, r4, 0, 31, 29
        mtmsr r4
        lwz r4, 0x198(ctx)
        mtsrr0 r4
        lwz r4, 0x19c(ctx)
        mtsrr1 r4
        lwz r4, 0x10(ctx)
        lwz r3, 0xc(ctx)
        rfi
    }
}

u32 OSGetStackPointer(void)
{
    register u32 ptr;

    asm {
        mr ptr, r1
    }

    return ptr;
}

void OSClearContext(OSContext* context)
{
    context->mode = 0;
    context->state = 0;

    if ((u32)context == (u32)OS_CTX_LAST_FPU) {
        OS_CTX_LAST_FPU = 0;
    }
}

void OSInitContext(OSContext* context, u32 pc, u32 stack)
{
    context->srr0 = pc;
    context->gpr[1] = stack;
    context->srr1 = 0x9032;
    context->cr = 0;
    context->xer = 0;
    context->gpr[2] = 0;
    context->gpr[13] = 0;
    context->gpr[3] = 0;
    context->gpr[4] = 0;
    context->gpr[5] = 0;
    context->gpr[6] = 0;
    context->gpr[7] = 0;
    context->gpr[8] = 0;
    context->gpr[9] = 0;
    context->gpr[10] = 0;
    context->gpr[11] = 0;
    context->gpr[12] = 0;
    context->gpr[14] = 0;
    context->gpr[15] = 0;
    context->gpr[16] = 0;
    context->gpr[17] = 0;
    context->gpr[18] = 0;
    context->gpr[19] = 0;
    context->gpr[20] = 0;
    context->gpr[21] = 0;
    context->gpr[22] = 0;
    context->gpr[23] = 0;
    context->gpr[24] = 0;
    context->gpr[25] = 0;
    context->gpr[26] = 0;
    context->gpr[27] = 0;
    context->gpr[28] = 0;
    context->gpr[29] = 0;
    context->gpr[30] = 0;
    context->gpr[31] = 0;
    context->gqr[0] = 0;
    context->gqr[1] = 0;
    context->gqr[2] = 0;
    context->gqr[3] = 0;
    context->gqr[4] = 0;
    context->gqr[5] = 0;
    context->gqr[6] = 0;
    context->gqr[7] = 0;

    OSClearContext(context);
}

void OSDumpContext(OSContext* context)
{
    OSContext saved;
    u32* address;
    u32 i;
    s32 count;

    OSReport("------------------------- Context 0x%08x "
             "-------------------------\n",
             context);

    for (i = 0; i < 16; i++) {
        OSReport("r%-2d  = 0x%08x (%14d)  r%-2d  = 0x%08x (%14d)\n", i, (s64)context->gpr[i],
                 i + 16, (s64)context->gpr[i + 16]);
    }
    OSReport("LR   = 0x%08x                   CR   = 0x%08x\n", context->lr, context->cr);
    OSReport("SRR0 = 0x%08x                   SRR1 = 0x%08x\n", context->srr0, context->srr1);
    OSReport("\nGQRs----------\n");

    for (i = 0; i < 4; i++) {
        OSReport("gqr%d = 0x%08x \t gqr%d = 0x%08x\n", i, context->gqr[i], i + 4,
                 context->gqr[i + 4]);
    }

    if (context->state & OS_CONTEXT_STATE_FPSAVED) {
        BOOL flags;
        OSContext* old;

        flags = OSDisableInterrupts();
        old = OSGetCurrentContext();
        OSClearContext(&saved);
        OSSetCurrentContext(&saved);

        OSReport("\n\nFPRs----------\n");
        for (i = 0; i < 32; i += 2) {
            OSReport("fr%d \t= %d \t fr%d \t= %d\n", i, (int)context->fpr[i], i + 1,
                     (int)context->fpr[i + 1]);
        }

        OSReport("\n\nPSFs----------\n");
        for (i = 0; i < 32; i += 2) {
            OSReport("ps%d \t= 0x%x \t ps%d \t= 0x%x\n", i, (int)context->psf[i], i + 1,
                     (int)context->psf[i + 1]);
        }

        OSClearContext(&saved);
        OSSetCurrentContext(old);
        OSRestoreInterrupts(flags);
    }

    OSReport("\nAddress:      Back Chain    LR Save\n");

    address = (u32*)context->gpr[1];
    count = 0;
    while (address != NULL && ((u32)address + 0x10000) != 0xFFFF && count++ < 16) {
        OSReport("0x%08x:   0x%08x    0x%08x\n", address, address[0], address[1]);
        address = (u32*)address[0];
    }
}

static void OSSwitchFPUContext(OSContext* context)
{
    register u32 srr1;
    register OSContext* ctx = context;
    OSContext* old;

    asm {
        mfmsr srr1
        ori srr1, srr1, 0x2000
        mtmsr srr1
        isync
        lwz srr1, 0x19c(ctx)
        ori srr1, srr1, 0x2000
        mtsrr1 srr1
    }

    old = OS_CTX_LAST_FPU;
    OS_CTX_LAST_FPU = context;

    if (old != context) {
        if (old != NULL) {
            __OSSaveFPUContext(old);
        }
        __OSLoadFPUContext(context);
    }

    asm {
        lwz r3, 0x80(ctx)
        mtcrf 255, r3
        lwz r3, 0x84(ctx)
        mtlr r3
        lwz r3, 0x198(ctx)
        mtsrr0 r3
        lwz r3, 0x88(ctx)
        mtctr r3
        lwz r3, 0x8c(ctx)
        mtxer r3
        lhz r3, 0x1a2(ctx)
        rlwinm r3, r3, 0, 31, 29
        sth r3, 0x1a2(ctx)
        lwz r5, 0x14(ctx)
        lwz r3, 0xc(ctx)
        lwz r4, 0x10(ctx)
        rfi
    }
}

void __OSContextInit(void)
{
    __OSSetExceptionHandler(OS_ERROR_FPE, (OSErrorHandler)OSSwitchFPUContext);
    OS_CTX_LAST_FPU = 0;
    DBPrintf("FPU-unavailable handler installed\n");
}