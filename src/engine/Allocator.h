#ifndef ENGINE_ALLOCATOR_H
#define ENGINE_ALLOCATOR_H

#include <dolphin/os.h>

#ifdef __cplusplus
extern "C" {
#endif

void* OSAllocFromHeapAligned(int heap, u32 size, u32 align);
int OSGetSizeOfLargestFreeBlock(int heap);
void OSSetConfigurator(void (*configurator)(int*), BOOL useAutoHeap);
void OSSetQualificationSize(int size);
void OSSetNumberOfVerySmallBuffers(int num);

#ifdef __cplusplus
}
#endif

#endif
