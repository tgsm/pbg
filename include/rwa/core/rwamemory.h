#ifndef RWA_CORE_RWAMEMORY_H
#define RWA_CORE_RWAMEMORY_H

#include <stdlib.h>
#include <rwsdk/badevice.h>

#ifdef __cplusplus
extern "C" {
#endif

void* _rwaMalloc(size_t size);
void* _rwaMallocAligned(size_t size);
void _rwaFreeAligned(void* ptr);
void _rwaFree(void* ptr);
void* _rwaCalloc(size_t n, size_t size);
RwBool _rwaMemoryOpen(RwMemoryFunctions* funcs);
void _rwaMemoryClose(void);

#ifdef __cplusplus
}
#endif

#endif
