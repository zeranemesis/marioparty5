/*
 * Message numbers that are really string pointers.
 *
 * The game passes either a message number (file << 16 | index, always below
 * 0x80000000) or a pointer to a string it built, and tells them apart with
 * `messNum > 0x80000000`, which held on GameCube because every RAM address
 * started with 0x80. A 64-bit pointer does not fit in those 32 bits, so
 * MESSNUM_PTR hands out a handle above 0x80000000 instead, and the few places
 * that turn a message number back into a string resolve it here.
 *
 * Handles are direct-mapped by pointer: registering the same string twice
 * gives the same handle, and a collision only costs an older entry, never a
 * wrong string for a live one.
 */

#include "port/messnum.h"

#include <stddef.h>

#include "dolphin/os.h"

#define MESSNUM_SLOT_COUNT 4096u
#define MESSNUM_HANDLE_BASE 0x80000001u

static const char *sSlots[MESSNUM_SLOT_COUNT];

static u32 slot_of(const char *ptr)
{
    uintptr_t value = (uintptr_t)ptr;
    /* mix the whole address so that pointers 8 or 32 bytes apart differ */
    value ^= value >> 32;
    value *= 0x9E3779B97F4A7C15ull;
    return (u32)((value >> 20) & (MESSNUM_SLOT_COUNT - 1));
}

u32 HuWinMesPtrToNum(const char *ptr)
{
    u32 slot;

    if (ptr == NULL) {
        return 0;
    }
    slot = slot_of(ptr);
    sSlots[slot] = ptr;
    return MESSNUM_HANDLE_BASE + slot;
}

char *HuWinMesNumToPtr(u32 messNum)
{
    u32 slot;

    if (messNum < MESSNUM_HANDLE_BASE || messNum >= MESSNUM_HANDLE_BASE + MESSNUM_SLOT_COUNT) {
        OSReport("messnum> %08x is not a string handle\n", messNum);
        return NULL;
    }
    slot = messNum - MESSNUM_HANDLE_BASE;
    if (sSlots[slot] == NULL) {
        OSReport("messnum> handle %08x has been reused\n", messNum);
    }
    return (char *)sSlots[slot];
}
