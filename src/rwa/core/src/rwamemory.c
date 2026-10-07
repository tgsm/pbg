#include <stddef.h>
#include <string.h>
#include <rwa/core/rwamemory.h>

void* _rwaMalloc(size_t size) {
    return RwMalloc(size);
}

void* _rwaMallocAligned(size_t size) {
    void* aligned;
    void* ptr;
    size_t alignedSize = size + 32;
    ptr = RwMalloc(alignedSize);
    if (ptr != NULL) {
        aligned = (void*)(((RwInt32)ptr + 32) & ~0x1F);
        ((RwInt32*)aligned)[-1] = (RwInt32)ptr;
    } else {
        return NULL;
    }

    return aligned;
}

void _rwaFreeAligned(void* ptr) {
    _rwaFree((void*)(((RwInt32*)ptr)[-1]));
}

void _rwaFree(void* ptr) {
    RwFree(ptr);
}

void* _rwaCalloc(size_t n, size_t size) {
    return RwCalloc(n, size);
}

static void* FakeCalloc(size_t n, size_t size) {
    void* ptr = RwMalloc(n * size);
    if (ptr != NULL) {
        memset(ptr, 0, n * size);
    }
    return ptr;
}

RwBool _rwaMemoryOpen(RwMemoryFunctions* funcs) {
    if (funcs != NULL) {
        memcpy(&RwEngineInstance->memoryFuncs, funcs, sizeof(RwMemoryFunctions));
    } else {
        RwEngineInstance->memoryFuncs.rwmalloc = malloc;
        RwEngineInstance->memoryFuncs.rwfree = free;
        RwEngineInstance->memoryFuncs.rwrealloc = realloc;
        RwEngineInstance->memoryFuncs.rwcalloc = FakeCalloc;
    }

    return TRUE;
}

void _rwaMemoryClose(void) {

}
