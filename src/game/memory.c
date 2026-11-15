#include "game/memory.h"
#include "dolphin/os.h"
#ifdef TARGET_PC
#include <stdlib.h>
#endif

#define DATA_GET_BLOCK(ptr) ((MEMORY_BLOCK *)(((char *)(ptr))-32))
#define BLOCK_GET_DATA(block) (((char *)(block))+32)

#define MEM_ALLOC_SIZE(size) (((size)+63) & 0xFFFFFFE0)

typedef struct MemoryBlock_s MEMORY_BLOCK;

struct MemoryBlock_s {
    s32 size;
    u8 magic;
    u8 flag;
    MEMORY_BLOCK *prev;
    MEMORY_BLOCK *next;
    u32 num;
    u32 retAddr;
};

static void *HuMemMemoryAlloc2(void *heap, s32 size, u32 num, u32 retAddr);
static void *HuMemTailMemoryAlloc2(void *heap, s32 size, u32 num, u32 retAddr);

#ifdef TARGET_PC
/* First-fit scans walk every block from the heap start, so loading a model (hundreds of allocations in a heap of
 * thousands of blocks) stalled frames for tens of ms. Each heap remembers where its leading run of allocated blocks
 * ends; a scan starting there finds the same block. Allocations never free a block before that point, so the hint
 * stays right until the next free or heap init anywhere (HuMemFreeGen), which drops every hint. */
#define HUMEM_HINT_NUM 32
static struct {
    void *heap;
    MEMORY_BLOCK *hint;
    u32 gen;
} HuMemHint[HUMEM_HINT_NUM];
static u32 HuMemFreeGen = 1;

static MEMORY_BLOCK *HuMemHintGet(void *heap)
{
    u32 i = ((uintptr_t)heap >> 5) % HUMEM_HINT_NUM;
    if(HuMemHint[i].heap == heap && HuMemHint[i].gen == HuMemFreeGen) {
        return HuMemHint[i].hint;
    }
    return heap;
}

static void HuMemHintSet(void *heap, MEMORY_BLOCK *hint)
{
    u32 i = ((uintptr_t)heap >> 5) % HUMEM_HINT_NUM;
    HuMemHint[i].heap = heap;
    HuMemHint[i].hint = hint;
    HuMemHint[i].gen = HuMemFreeGen;
}
#endif

void *HuMemHeapInit(void *heap, s32 size)
{
    MEMORY_BLOCK *block = heap;
#ifdef TARGET_PC
    HuMemFreeGen++;
#endif
    block->size = size;
    block->magic = 205;
    block->flag = 0;
    block->prev = block;
    block->next = block;
    block->num = -256;
    block->retAddr = 0xCDCDCDCD;
    return block;
}

void *HuMemMemoryAllocNum(void *heap, s32 size, u32 num, u32 retAddr)
{
    return HuMemMemoryAlloc2(heap, size, num, retAddr);
}

void *HuMemMemoryAlloc(void *heap, s32 size, u32 retAddr)
{
    return HuMemMemoryAlloc2(heap, size, -256, retAddr);
}

static void *HuMemMemoryAlloc2(void *heap, s32 size, u32 num, u32 retAddr)
{
    s32 blockSize = MEM_ALLOC_SIZE(size);
    MEMORY_BLOCK *block = heap;
#ifdef TARGET_PC
    MEMORY_BLOCK *firstFree = NULL;
    MEMORY_BLOCK *ref = NULL;
    static int check = -1;
    if(check < 0) {
        check = getenv("MP5_TEST_HEAPCHECK") != NULL; /* test bench: full scan too, report any other pick */
    }
    if(check) {
        for(ref = heap; ref->flag || ref->size < blockSize; ) {
            ref = ref->next;
            if(ref == heap) {
                ref = NULL;
                break;
            }
        }
    }
    block = HuMemHintGet(heap);
    if(check && (ref ? ref : heap) != block && ref != NULL) {
        MEMORY_BLOCK *b = block;
        while(b != ref && b->next != heap) {
            b = b->next;
        }
        if(b != ref) {
            OSReport("HEAPCHECK: hint %p past first fit %p\n", block, ref);
        }
    }
#endif
    do {
#ifdef TARGET_PC
        if(!block->flag && block->size < blockSize && !firstFree) {
            firstFree = block;
        }
#endif
        if(!block->flag && block->size >= blockSize) {
            if(block->size-blockSize > 32u) {
                MEMORY_BLOCK *new_block = (MEMORY_BLOCK *)(((uintptr_t)block)+blockSize);
                new_block->size = block->size-blockSize;
                new_block->magic = 205;
                new_block->flag = 0;
                new_block->retAddr = retAddr;
                block->next->prev = new_block;
                new_block->next = block->next;
                block->next = new_block;
                new_block->prev = block;
                block->size = blockSize;
            }
            block->flag = 1;
            block->magic = 165;
            block->num = num;
            block->retAddr = retAddr;
#ifdef TARGET_PC
            HuMemHintSet(heap, firstFree ? firstFree : block->next);
#endif
            return BLOCK_GET_DATA(block);
        }
        block = block->next;
    } while(block != heap);
    OSReport("HuMem>memory alloc error %08x(%08X): Call %08x\n", size, num, retAddr);
    HuMemHeapDump(heap, -1);
    return NULL;
}

void *HuMemTailMemoryAlloc(void *heap, s32 size, u32 retAddr)
{
#ifdef TARGET_PC
    // the GC code returns this only because the result is still in r3
    return HuMemTailMemoryAlloc2(heap, size, -256, retAddr);
#else
    HuMemTailMemoryAlloc2(heap, size, -256, retAddr);
#endif
}

static void *HuMemTailMemoryAlloc2(void *heap, s32 size, u32 num, u32 retAddr)
{
    s32 blockSize = MEM_ALLOC_SIZE(size);
    MEMORY_BLOCK *block = heap;
    while(block->next != heap) {
        block = block->next;
    }
    do {
        if(!block->flag && block->size >= blockSize) {
            if(block->size-blockSize > 32u) {
                MEMORY_BLOCK *old_block = block;
                block = (MEMORY_BLOCK *)(((char *)old_block)+(old_block->size-blockSize));
                block->size = blockSize;
                block->magic = 165;
                block->prev = old_block;
                block->next = old_block->next;
                old_block->size = old_block->size-blockSize;
                old_block->next = old_block->next->prev = block;
                old_block->retAddr = retAddr;
            }
            block->flag = 1;
            block->num = num;
            block->retAddr = retAddr;
            return BLOCK_GET_DATA(block);
        }
        block = block->prev;
    } while(block != heap);
    printf("memory allocation(tail) error.\n");
    return NULL;
}

#ifdef TARGET_PC
u32 HuMemNumGet(void *ptr)
{
    return ptr ? DATA_GET_BLOCK(ptr)->num : 0;
}
#endif

void HuMemMemoryFreeNum(void *heap, u32 num, u32 retAddr)
{
    MEMORY_BLOCK *block = heap;
    do {
        MEMORY_BLOCK *block_next = block->next;
        if(block->flag && block->num == num) {
            HuMemMemoryFree(BLOCK_GET_DATA(block), retAddr);
        }
        block = block_next;
    } while(block != heap);
    
}

void HuMemMemoryFree(void *ptr, u32 retAddr)
{
    MEMORY_BLOCK *block;
    if(!ptr) {
        return;
    }
    block = DATA_GET_BLOCK(ptr);
    if(block->magic != 165) {
        OSReport("HuMem>memory free error. %08x( call %08x)\n", ptr, retAddr);
        return;
    }
#ifdef TARGET_PC
    HuMemFreeGen++;
#endif
    if(block->prev < block && !block->prev->flag) {
        block->flag  = 0;
        block->magic = 205;
        block->next->prev = block->prev;
        block->prev->next = block->next;
        block->prev->size += block->size;
        block = block->prev;
    }
    if(block->next > block && !block->next->flag) {
        block->next->next->prev = block;
        block->size += block->next->size;
        block->next = block->next->next;
    }
    block->flag = 0;
    block->magic = 205;
    block->retAddr = retAddr;
}

s32 HuMemUsedMemorySizeGet(void *heap)
{
    MEMORY_BLOCK *block = heap;
    s32 size = 0;
    do {
        if(block->flag == 1) {
            size += block->size;
        }
        block = block->next;
    } while(block != heap);
    return size;
}

s32 HuMemUsedMemoryBlockGet(void *heap)
{
    MEMORY_BLOCK *block = heap;
    s32 num_blocks = 0;
    do {
        if(block->flag == 1) {
            num_blocks++;
        }
        block = block->next;
    } while(block != heap);
    return num_blocks;
}

s32 HuMemMemoryAllocSizeGet(s32 size)
{
    return MEM_ALLOC_SIZE(size);
}

void HuMemHeapDump(void *heap, s16 status)
{
    MEMORY_BLOCK *block = heap;
    s32 size = 0;
    s32 inactive_size = 0;
    s32 num_blocks = 0;
    s32 num_unused_blocks = 0;
    u8 dump_type;

    if(status < 0) {
        dump_type = 10;
    } else if(status == 0) {
        dump_type = 0;
    } else {
        dump_type = 1;
    }
    OSReport("======== HuMem heap dump %08x ========\n", heap);
    OSReport("MCB-----+Size----+MG+FL+Prev----+Next----+UNum----+Body----+Call----\n");
    do {
        if(dump_type == 10 || block->flag == dump_type) {
            OSReport("%08x %08x %02x %02x %08x %08x %08x %08x %08x\n", block, block->size, block->magic, block->flag,
                block->prev, block->next, block->num, BLOCK_GET_DATA(block), block->retAddr);
        }
        if(block->flag == 1) {
            size += block->size;
            num_blocks++;
        } else {
            inactive_size += block->size;
            num_unused_blocks++;
        }
        
        block = block->next;
    } while(block != heap);
    OSReport("MCB:%d(%d/%d) MEM:%08x(%08x/%08x)\n", num_blocks+num_unused_blocks, num_blocks, num_unused_blocks, 
        size+inactive_size, size, inactive_size);
    OSReport("======== HuMem heap dump %08x end =====\n", heap);
}

s32 HuMemMemorySizeGet(void *ptr)
{
    MEMORY_BLOCK *block;
    if(!ptr) {
        return 0;
    }
    block = DATA_GET_BLOCK(ptr);
    if(block->flag == 1 && block->magic == 165) {
        return block->size-32;
    } else {
        return 0;
    }
}
