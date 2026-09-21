#include "msm/msmmem.h"

typedef struct MSMBlock_s {
    /* 0x00 */ struct MSMBlock_s *prev;
    /* 0x04 */ struct MSMBlock_s *next;
    /* 0x08 */ u32 freeSize;
    /* 0x0C */ u32 size;
    /* 0x10 */ void *ptr;
    /* 0x14 */ char pad[12];
} MSM_BLOCK; // Size 0x20

typedef struct MSMMem_s {
    /* 0x00 */ void *ptr;
    /* 0x04 */ u32 size;
    /* 0x08 */ MSM_BLOCK *head;
    /* 0x0C */ MSM_BLOCK first;
} MSM_MEM; // Size 0x2C

static MSM_MEM mem;

void msmMemFree(void *ptr)
{
    MSM_BLOCK *block;
    MSM_BLOCK *blockPrev;
    MSM_BLOCK *blockNext;
    MSM_BLOCK *blockHead;
    MSM_BLOCK *base;

    base = ptr;
    block = &base[-1];
    blockPrev = block->prev;
    blockNext = block->next;
    if (mem.ptr > block || ((uintptr_t)mem.ptr + (uintptr_t)mem.size) <= (uintptr_t)block) {
        return;
    }
    if (blockPrev->next != block || block->ptr != ptr || (blockNext && blockNext->prev != block)) {
        return;
    }
    blockPrev->size += block->freeSize + block->size;
    blockPrev->next = blockNext;
    blockHead = mem.head;
    if (blockHead == block || blockHead->size < blockPrev->size) {
        mem.head = blockPrev;
    }
    if (blockNext) {
        blockNext->prev = blockPrev;
        if (mem.head->size < blockNext->size) {
            mem.head = blockNext;
        }
    }
}

void *msmMemAlloc(u32 size)
{
    s32 alignOfs;
    u32 freeSize;
    u32 allocSize;
    MSM_BLOCK *block;
    MSM_BLOCK *blockPrev;
    MSM_BLOCK *blockNext;

    allocSize = size + 0x20;
    alignOfs = allocSize & 0x1F;
    if (alignOfs) {
        allocSize += 0x20 - alignOfs;
    }
    if (mem.head->size >= allocSize) {
        blockPrev = mem.head;
    } else {
        blockPrev = &mem.first;
        do {
            if (blockPrev->size >= allocSize) {
                break;
            }
            blockPrev = blockPrev->next;
        } while (blockPrev);
        if (!blockPrev) {
            return NULL;
        }
    }
    freeSize = blockPrev->freeSize;
    if (freeSize != 0) {
        freeSize -= 0x20;
    }
    block = (MSM_BLOCK *)((uintptr_t)blockPrev->ptr + freeSize);
    blockNext = blockPrev->next;
    if (mem.ptr > block || (void *)((uintptr_t)mem.ptr + (uintptr_t)mem.size) <= block) {
        return NULL;
    }
    block->freeSize = allocSize;
    block->size = blockPrev->size - allocSize;
    block->ptr = block + 1;
    block->prev = blockPrev;
    block->next = blockNext;
    mem.head = block;
    blockPrev->size = 0;
    blockPrev->next = block;
    if (blockNext) {
        blockNext->prev = block;
        if (mem.head->size < blockNext->size) {
            mem.head = blockNext;
        }
    }
    return block->ptr;
}

void msmMemInit(void *ptr, u32 size)
{
    MSM_BLOCK *block;
    s32 ofs;

    ofs = (s32)(uintptr_t)ptr & 0x1F;
    switch (ofs) {
        default:
            ofs = 0x20 - ofs;
            break;
        case 0:
            ofs = 0;
            break;
    }
    mem.ptr = (void *)((uintptr_t)ptr + ofs);
    ofs = (s32)(uintptr_t)ptr + size;
    mem.size = (ofs - (s32)(uintptr_t)mem.ptr) & ~0x1F;
    block = &mem.first;
    block->freeSize = 0;
    block->size = mem.size;
    block->ptr = mem.ptr;
    block->prev = NULL;
    block->next = NULL;
    mem.head = &mem.first;
}
