#include <stddef.h>
#include <rwsdk/badevice.h>
#include <rwsdk/plcore/baerr.h>
#include <rwsdk/plcore/bamemory.h>
#include <rwsdk/plcore/batkreg.h>

#ifdef __cplusplus
extern "C" {
#endif

static RwPluginRegistry** toolkitNonFLRegList;
static RwFreeList toolkitRegEntriesSpace;
static RwUInt32 numRegToolkits;
static RwFreeList* toolkitRegEntries;
static RwInt32 _rwPluginRegFreeListBlockSize = 64;
static RwInt32 _rwPluginRegListPreallocBlocks = 1;

RwBool _rwPluginRegistryOpen(void) {
    toolkitRegEntries = RwFreeListCreateAndPreallocateSpace(sizeof(RwPluginRegEntry), _rwPluginRegFreeListBlockSize, 4, _rwPluginRegListPreallocBlocks, &toolkitRegEntriesSpace);
    if (toolkitRegEntries == NULL) {
        return FALSE;
    }

    numRegToolkits = 0;
    return TRUE;
}

static void rwDestroyEntry(void* entry_, void* freeList_) {
    RwPluginRegEntry* entry = (RwPluginRegEntry*)entry_;
    RwFreeList* freeList = (RwFreeList*)freeList_;
    RwPluginRegistry* registry = entry->parentRegistry;
    if (registry->firstRegEntry != NULL) {
        registry->sizeOfStruct = registry->origSizeOfStruct;
        entry->parentRegistry->firstRegEntry = NULL;
        entry->parentRegistry->lastRegEntry = NULL;
    }
    RwEngineInstance->memoryFree(freeList, entry);
}

RwBool _rwPluginRegistryClose(void) {
    if (toolkitRegEntries != NULL) {
        RwInt32 i;
        RwFreeListForAllUsed(toolkitRegEntries, rwDestroyEntry, toolkitRegEntries);

        if (RwEngineInstance->memoryAlloc != _rwFreeListAllocReal) {
            for (i = 0; i < numRegToolkits; i++) {
                RwPluginRegistry* registry = NULL;
                RwPluginRegEntry* entry = toolkitNonFLRegList[i]->firstRegEntry;

                if (entry != NULL) {
                    registry = entry->parentRegistry;
                }

                while (entry != NULL) {
                    RwPluginRegEntry* temp = entry->nextRegEntry;
                    RwEngineInstance->memoryFree(NULL, entry);
                    entry = temp;
                }

                if (registry != NULL && registry->firstRegEntry != NULL) {
                    registry->sizeOfStruct = registry->origSizeOfStruct;
                    registry->firstRegEntry = NULL;
                    registry->lastRegEntry = NULL;
                }
            }

            if (toolkitNonFLRegList != NULL) {
                RwFree(toolkitNonFLRegList);
                toolkitNonFLRegList = NULL;
            }
        }

        RwFreeListDestroy(toolkitRegEntries);
        toolkitRegEntries = 0;
    }

    return TRUE;
}

static void* PluginDefaultConstructor(void* a0, RwInt32, RwInt32) {
    return a0;
}

static void* PluginDefaultDestructor(void* a0, RwInt32, RwInt32) {
    return a0;
}

static void* PluginDefaultCopy(void* a0, const void*, RwInt32, RwInt32) {
    return a0;
}

RwInt32 _rwPluginRegistryGetPluginOffset(RwPluginRegistry* registry, RwUInt32 pluginID) {
    RwPluginRegEntry* entry;
    for (entry = registry->firstRegEntry; entry != NULL; entry = entry->nextRegEntry) {
        if (entry->pluginID == pluginID) {
            return entry->offset;
        }
    }

    return -1;
}

RwInt32 _rwPluginRegistryAddPlugin(RwPluginRegistry* registry, RwInt32 size, RwUInt32 pluginID, RwPluginObjectConstructor constructCB, RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB) {
    RwPluginRegEntry* entry;
    RwInt32 sizeAligned;

    if (toolkitRegEntries == NULL) {
        return -1;
    }

    if (_rwGetNumEngineInstances() != 0) {
        RwThrowError(1, E_RW_PLUGININIT);
        return -1;
    }

    if (RwEngineInstance->memoryAlloc != _rwFreeListAllocReal) {
        RwInt32 i;
        for (i = 0; i < numRegToolkits; i++) {
            if (registry == toolkitNonFLRegList[i]) {
                break;
            }
        }

        if (numRegToolkits == i) {
            RwPluginRegistry** registries = (RwPluginRegistry**)RwMalloc((numRegToolkits + 1) * sizeof(RwPluginRegistry*));
            RwInt32 j = 0;
            if (toolkitNonFLRegList != NULL) {
                for (; j < numRegToolkits; j++) {
                    registries[j] = toolkitNonFLRegList[j];
                }
                RwFree(toolkitNonFLRegList);
                toolkitNonFLRegList = NULL;
            }
            registries[j] = registry;
            numRegToolkits++;
            toolkitNonFLRegList = registries;
        }
    }

    for (entry = registry->firstRegEntry; entry != NULL; entry = entry->nextRegEntry) {
        if (entry->pluginID == pluginID) {
            RwThrowError(1, E_RW_PLUGININIT);
            return entry->offset;
        }
    }

    sizeAligned = registry->sizeOfStruct + ((size + 3) & ~3);
    if (registry->maxSizeOfStruct != 0 && sizeAligned > registry->maxSizeOfStruct) {
        return -1;
    }

    entry = (RwPluginRegEntry*)RwEngineInstance->memoryAlloc(toolkitRegEntries);
    if (entry != NULL) {
        entry->offset = registry->sizeOfStruct;
        registry->sizeOfStruct = sizeAligned;
        entry->size = size;
        entry->pluginID = pluginID;
        entry->readCB = NULL;
        entry->writeCB = NULL;
        entry->getSizeCB = NULL;
        entry->alwaysCB = NULL;
        entry->rightsCB = NULL;
        entry->constructCB = (constructCB != NULL) ? constructCB : PluginDefaultConstructor;
        entry->destructCB = (destructCB != NULL) ? destructCB : PluginDefaultDestructor;
        entry->copyCB = (copyCB != NULL) ? copyCB : PluginDefaultCopy;
        entry->errorStrCB = NULL;
        entry->nextRegEntry = NULL;
        entry->prevRegEntry = NULL;
        entry->parentRegistry = registry;

        if (registry->firstRegEntry == NULL) {
            registry->firstRegEntry = entry;
            registry->lastRegEntry = entry;
        } else {
            registry->lastRegEntry->nextRegEntry = entry;
            entry->prevRegEntry = registry->lastRegEntry;
            registry->lastRegEntry = entry;
        }
        return entry->offset;
    }

    return -1;
}

RwPluginRegistry* _rwPluginRegistryInitObject(RwPluginRegistry* registry, void* ptr) {
    RwPluginRegEntry* entry;

    for (entry = registry->firstRegEntry; entry != NULL; entry = entry->nextRegEntry) {
        if (entry->constructCB(ptr, entry->offset, entry->size) == NULL) {
            for (entry = entry->prevRegEntry; entry != NULL; entry = entry->prevRegEntry) {
                entry->destructCB(ptr, entry->offset, entry->size);
            }
            return NULL;
        }
    }

    return registry;
}

RwPluginRegistry* _rwPluginRegistryDeInitObject(RwPluginRegistry* registry, void* ptr) {
    RwPluginRegEntry* entry;
    for (entry = registry->lastRegEntry; entry != NULL; entry = entry->prevRegEntry) {
        entry->destructCB(ptr, entry->offset, entry->size);
    }

    return registry;
}

RwPluginRegistry* _rwPluginRegistryCopyObject(RwPluginRegistry* registry, void* ptr, const void* a2) {
    RwPluginRegEntry* entry;
    for (entry = registry->firstRegEntry; entry != NULL; entry = entry->nextRegEntry) {
        entry->copyCB(ptr, a2, entry->offset, entry->size);
    }

    return registry;
}

#ifdef __cplusplus
}
#endif
