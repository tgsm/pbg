#ifndef RWSDK_PLCORE_BATKREG_H
#define RWSDK_PLCORE_BATKREG_H

#include <rwsdk/plcore/bastream.h>

#ifdef __cplusplus
extern "C" {
#endif

struct RwPluginRegEntry;

typedef struct RwPluginRegistry {
    int sizeOfStruct;
    int origSizeOfStruct;
    int maxSizeOfStruct;
    int staticAlloc;
    struct RwPluginRegEntry* firstRegEntry;
    struct RwPluginRegEntry* lastRegEntry;
} RwPluginRegistry; // size: 0x18

typedef RwStream* (*RwPluginDataChunkWriteCallBack)(RwStream*, int, const void*, int, int);
typedef RwStream* (*RwPluginDataChunkReadCallBack)(RwStream*, int, void*, int, int);
typedef int (*RwPluginDataChunkGetSizeCallBack)(const void*, int, int);
typedef int (*RwPluginDataChunkAlwaysCallBack)(void*, int, int);
typedef int (*RwPluginDataChunkRightsCallBack)(void*, int, int, unsigned int);
typedef void* (*RwPluginObjectConstructor)(void*, int, int);
typedef void* (*RwPluginObjectCopy)(void*, const void*, int, int);
typedef void* (*RwPluginObjectDestructor)(void*, int, int);
typedef void* (*RwPluginErrorStrCallBack)(void*);

typedef struct RwPluginRegEntry {
    int offset;
    int size;
    unsigned int pluginID;
    RwPluginDataChunkReadCallBack readCB;
    RwPluginDataChunkWriteCallBack writeCB;
    RwPluginDataChunkGetSizeCallBack getSizeCB;
    RwPluginDataChunkAlwaysCallBack alwaysCB;
    RwPluginDataChunkRightsCallBack rightsCB;
    RwPluginObjectConstructor constructCB;
    RwPluginObjectDestructor destructCB;
    RwPluginObjectCopy copyCB;
    RwPluginErrorStrCallBack errorStrCB;
    struct RwPluginRegEntry* nextRegEntry;
    struct RwPluginRegEntry* prevRegEntry;
    RwPluginRegistry* parentRegistry;
} RwPluginRegEntry; // size: 0x3C

int _rwPluginRegistryOpen(void);
int _rwPluginRegistryClose(void);
int _rwPluginRegistryGetPluginOffset(RwPluginRegistry* registry, unsigned int pluginID);
int _rwPluginRegistryAddPlugin(RwPluginRegistry* registry, int size, unsigned int pluginID, RwPluginObjectConstructor constructCB, RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB);
RwPluginRegistry* _rwPluginRegistryInitObject(RwPluginRegistry* registry, void* ptr);
RwPluginRegistry* _rwPluginRegistryDeInitObject(RwPluginRegistry* registry, void* ptr);
RwPluginRegistry* _rwPluginRegistryCopyObject(RwPluginRegistry* registry, void* ptr, const void* a2);

#ifdef __cplusplus
}
#endif

#endif
