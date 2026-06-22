#ifndef RWSDK_PLCORE_BATKREG_H
#define RWSDK_PLCORE_BATKREG_H

#include <rwsdk/plcore/bastream.h>

#ifdef __cplusplus
extern "C" {
#endif

struct RwPluginRegEntry;

typedef struct RwPluginRegistry {
    RwInt32 sizeOfStruct;
    RwInt32 origSizeOfStruct;
    RwInt32 maxSizeOfStruct;
    RwInt32 staticAlloc;
    struct RwPluginRegEntry* firstRegEntry;
    struct RwPluginRegEntry* lastRegEntry;
} RwPluginRegistry; // size: 0x18

typedef RwStream* (*RwPluginDataChunkWriteCallBack)(RwStream*, RwInt32, const void*, RwInt32, RwInt32);
typedef RwStream* (*RwPluginDataChunkReadCallBack)(RwStream*, RwInt32, void*, RwInt32, RwInt32);
typedef RwInt32 (*RwPluginDataChunkGetSizeCallBack)(const void*, RwInt32, RwInt32);
typedef RwInt32 (*RwPluginDataChunkAlwaysCallBack)(void*, RwInt32, RwInt32);
typedef RwInt32 (*RwPluginDataChunkRightsCallBack)(void*, RwInt32, RwInt32, RwUInt32);
typedef void* (*RwPluginObjectConstructor)(void*, RwInt32, RwInt32);
typedef void* (*RwPluginObjectCopy)(void*, const void*, RwInt32, RwInt32);
typedef void* (*RwPluginObjectDestructor)(void*, RwInt32, RwInt32);
typedef void* (*RwPluginErrorStrCallBack)(void*);

typedef struct RwPluginRegEntry {
    RwInt32 offset;
    RwInt32 size;
    RwUInt32 pluginID;
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

RwBool _rwPluginRegistryOpen(void);
RwBool _rwPluginRegistryClose(void);
RwInt32 _rwPluginRegistryGetPluginOffset(RwPluginRegistry* registry, RwUInt32 pluginID);
RwInt32 _rwPluginRegistryAddPlugin(RwPluginRegistry* registry, RwInt32 size, RwUInt32 pluginID, RwPluginObjectConstructor constructCB, RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB);
RwPluginRegistry* _rwPluginRegistryInitObject(RwPluginRegistry* registry, void* ptr);
RwPluginRegistry* _rwPluginRegistryDeInitObject(RwPluginRegistry* registry, void* ptr);
RwPluginRegistry* _rwPluginRegistryCopyObject(RwPluginRegistry* registry, void* ptr, const void* a2);

#ifdef __cplusplus
}
#endif

#endif
