#include "dolphin/os.h"

#define RoundUpPtr(p, align)   ((void*)(((u32)(p) + ((align) - 1)) & ~((align) - 1)))
#define RoundDownPtr(p, align) ((void*)((u32)(p) & ~((align) - 1)))

inline static void InitDefaultHeap(void) {
	void* arenaLo;
	void* arenaHi;

	OSReport("GCN_Mem_Alloc.c : InitDefaultHeap. No Heap Available\n");
	OSReport("Metrowerks CW runtime library initializing default heap\n");

	arenaLo = OSGetArenaLo();
	arenaHi = OSGetArenaHi();

	arenaLo = OSInitAlloc(arenaLo, arenaHi, 1);
	OSSetArenaLo(arenaLo);

	arenaLo = RoundUpPtr(arenaLo, 0x20);
	arenaHi = RoundDownPtr(arenaHi, 0x20);

	OSSetCurrentHeap(OSCreateHeap(arenaLo, arenaHi));
	OSSetArenaLo(arenaLo = arenaHi);
}

/* Deferred inlining emits functions in reverse order: __sys_free comes first in the binary. */
void* __sys_alloc(u32 size) {
    if (__OSCurrHeap == -1) {
        InitDefaultHeap();
    }

    return OSAllocFromHeap(__OSCurrHeap, size);
}

void __sys_free(void* p) {
    if (__OSCurrHeap == -1) {
        InitDefaultHeap();
    }

    OSFreeToHeap(__OSCurrHeap, p);
}
