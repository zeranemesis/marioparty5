#include "dolphin/__start.h"

#include "dolphin/os.h"
#include "dolphin/__ppc_eabi_init.h"
#include "PowerPC_EABI_Support/Runtime/__ppc_eabi_linker.h"

/* Boot info word 2 selects the debug mode. */
#define DEBUGFLAG2 (*(volatile u32*)BOOTINFO2_ADDR)

__declspec(weak) void InitMetroTRK_BBA(void);

void __check_pad3(void)
{
    if ((Pad3Button & 0x0EEF) == 0x0EEF) {
        OSResetSystem(FALSE, 0, FALSE);
    }
}

__declspec(section ".init") static void __set_debug_bba(void)
{
    Debug_BBA = 1;
}

__declspec(section ".init") static u8 __get_debug_bba(void)
{
    return Debug_BBA;
}

__declspec(weak) __declspec(section ".init") void __start(void)
{
    u32 debugflag;
    u32 bootinfo;
    u32 count;
    u8* arenaHi;

    __init_registers();
    __init_hardware();

    asm {
        li r0, -1
        stwu r1, -8(r1)
        stw r0, 4(r1)
        stw r0, 0(r1)
    }

    __init_data();

    *(volatile u32*)EXCEPTIONMASK_ADDR = 0;

    bootinfo = *(volatile u32*)BOOTINFO2_ADDR;
    if (bootinfo != 0) {
        debugflag = *(volatile u32*)(bootinfo + OS_BI2_DEBUGFLAG_OFFSET);

        if (debugflag == 2) {
            InitMetroTRK();
        } else if (debugflag == 3) {
            InitMetroTRK();
        } else if (debugflag == 4) {
            __set_debug_bba();
        }
    } else if (*(volatile u32*)ARENAHI_ADDR != 0) {
        debugflag = *(volatile u32*)DEBUGFLAG_ADDR;

        if (debugflag == 2) {
            InitMetroTRK();
        } else if (debugflag == 3) {
            InitMetroTRK();
        } else if (debugflag == 4) {
            __set_debug_bba();
        }
    }

    bootinfo = *(volatile u32*)BOOTINFO2_ADDR;
    if (bootinfo != 0 && *(volatile u32*)(bootinfo + 8) != 0) {
        u32* table = (u32*)(bootinfo + *(volatile u32*)(bootinfo + 8));
        count = *table;
        if (count != 0) {
            arenaHi = (u8*)(table + 1);
            for (; count != 0; count--) {
                table++;
                *table += (u32)bootinfo;
            }
            *(u32*)ARENAHI_ADDR = (u32)arenaHi & ~0x1F;
        } else {
            count = 0;
            arenaHi = NULL;
        }
    } else {
        count = 0;
        arenaHi = NULL;
    }

    DBInit();
    OSInit();

    if ((*(volatile u16*)DVD_DEVICECODE_ADDR & 0x8000) ||
        ((*(volatile u16*)DVD_DEVICECODE_ADDR & 0x7FFF) == 1)) {
        __check_pad3();
    }

    if (__get_debug_bba() == 1) {
        InitMetroTRK_BBA();
    }

    __init_user();

    main((int)count, (char**)arenaHi);
    exit(0);
}

__declspec(section ".init") static asm void __init_registers(void)
{
    nofralloc
    li r0, 0
    li r3, 0
    li r4, 0
    li r5, 0
    li r6, 0
    li r7, 0
    li r8, 0
    li r9, 0
    li r10, 0
    li r11, 0
    li r12, 0
    li r14, 0
    li r15, 0
    li r16, 0
    li r17, 0
    li r18, 0
    li r19, 0
    li r20, 0
    li r21, 0
    li r22, 0
    li r23, 0
    li r24, 0
    li r25, 0
    li r26, 0
    li r27, 0
    li r28, 0
    li r29, 0
    li r30, 0
    li r31, 0
    lis r1, _stack_addr@h
    ori r1, r1, _stack_addr@l
    lis r2, _SDA2_BASE_@h
    ori r2, r2, _SDA2_BASE_@l
    lis r13, _SDA_BASE_@h
    ori r13, r13, _SDA_BASE_@l
    blr
}

__declspec(section ".init") static void __init_data(void)
{
    __rom_copy_info* copy;
    __bss_init_info* bss;

    for (copy = _rom_copy_info; copy->size != 0; copy++) {
        if (copy->addr != NULL && copy->addr != copy->rom) {
            memcpy(copy->addr, copy->rom, copy->size);
            __flush_cache(copy->addr, copy->size);
        }
    }

    for (bss = _bss_init_info; bss->size != 0; bss++) {
        if (bss->addr != NULL) {
            memset(bss->addr, 0, bss->size);
        }
    }
}

__declspec(weak) void InitMetroTRK_BBA(void)
{
}