#ifndef RWA_RWAMEMORY
#define RWA_RWAMEMORY

#include <stdlib.h>
#include <rwsdk/badevice.h>

void* _rwaMalloc(size_t size);
void* _rwaMallocAligned(size_t size);
void _rwaFreeAligned(void* ptr);
void _rwaFree(void* ptr);
void* _rwaCalloc(size_t n, size_t size);
int _rwaMemoryOpen(RwMemoryFunctions* funcs_ptr);
void _rwaMemoryClose(void);

#endif
