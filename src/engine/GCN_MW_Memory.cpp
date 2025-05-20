#include <dolphin/os.h>
#include "engine/Allocator.h"

// This file is a modified version of GCN_Mem_Alloc.c from the Metrowerks runtime.

#ifdef __cplusplus
extern "C" {
#endif

extern void DkMemoryGCNPreSetup(void);

static void InitDefaultHeap(void) {
    DkMemoryGCNPreSetup();

    void* arena_lo = OSGetArenaLo();
    void* arena_hi = OSGetArenaHi();
    arena_lo = OSInitAlloc(arena_lo, arena_hi, 1);
    OSSetArenaLo(arena_lo);

    arena_lo = (void*)OSRoundUp32B(arena_lo);
    arena_hi = (void*)OSRoundDown32B(arena_hi);

    int unk = 0;
    if (__OSAllocConfigurator != NULL) {
        __OSAllocConfigurator(&unk);
    }
    if (unk == 0) {
        arena_lo = OSInitAlloc(arena_lo, arena_hi, 1);
        OSHeapHandle new_heap = OSCreateHeap(arena_lo, arena_hi);
        OSSetCurrentHeap(new_heap);
    } else {
        arena_lo = OSInitAlloc(arena_lo, arena_hi, 2);
        void* new_heap_hi = (void*)((u32)arena_hi - (u32)arena_lo - (u32)unk);
        new_heap_hi = (void*)((u32)arena_lo + (u32)new_heap_hi);
        OSSetCurrentHeap(OSCreateHeap(arena_lo, new_heap_hi));
        OSCreateHeap(new_heap_hi, arena_hi);
        OSPrepareVerySmallBufferMemory();
    }
    OSSetArenaLo(arena_hi);
}

void* __sys_alloc(u32 size) {
    if (__OSCurrHeap == -1) {
        InitDefaultHeap();
    }
    return OSAlloc(size);
}

void __sys_free(void* ptr) {
    if (__OSCurrHeap == -1) {
        InitDefaultHeap();
    }
    OSFree(ptr);
}

#ifdef __cplusplus
}
#endif
