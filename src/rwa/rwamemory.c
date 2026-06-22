#include <null.h>
#include <string.h>
#include "rwa/rwamemory.h"

void* _rwaMalloc(size_t size) {
    return RwMalloc(size);
}

void* _rwaMallocAligned(size_t size) {
    void* aligned;
    void* ptr = RwMalloc(size + 32);
    if (ptr != NULL) {
        aligned = (void*)(((int)ptr + 32) & ~0x1F);
        ((int*)aligned)[-1] = (int)ptr;
    } else {
        return NULL;
    }

    return aligned;
}

void _rwaFreeAligned(void* ptr) {
    _rwaFree((void*)(((int*)ptr)[-1]));
}

void _rwaFree(void* ptr) {
    RwFree(ptr);
}

void* _rwaCalloc(size_t n, size_t size) {
    return RwCalloc(n, size);
}

// FIXME: hack
#pragma peephole on

static void* FakeCalloc(size_t n, size_t size) {
    void* ptr = RwMalloc(n * size);
    if (ptr != NULL) {
        memset(ptr, 0, n * size);
    }
    return ptr;
}

int _rwaMemoryOpen(RwMemoryFunctions* funcs_ptr) {
    if (funcs_ptr != NULL) {
        memcpy(&RwEngineInstance->memoryFuncs, funcs_ptr, sizeof(RwMemoryFunctions));
    } else {
        RwEngineInstance->memoryFuncs.rwmalloc = malloc;
        RwEngineInstance->memoryFuncs.rwfree = free;
        RwEngineInstance->memoryFuncs.rwrealloc = realloc;
        RwEngineInstance->memoryFuncs.rwcalloc = FakeCalloc;
    }

    return 1;
}

void _rwaMemoryClose(void) {

}
