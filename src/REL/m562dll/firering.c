#include "types.h"

extern void DCStoreRangeNoSync(void* addr, u32 size);

void fn_1_1A6E8(void* unused, void* arg0) {
    char dummy[0x10];
    (void)&dummy;
    DCStoreRangeNoSync(*(void**)((u32)arg0 + 0x48), *(u16*)((u32)arg0 + 0x30) * 0x44);
}

void fn_1_1E9C8(void* ptr) {
    memset(ptr, 0, 0x94);
}