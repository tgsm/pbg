#include <cstring>
#include <rwsdk/plcore/bamemory.h>
#include <rwsdk/badevice.h>
#include <rwsdk/baraster.h>

static RwFreeList _rwRasterFreeList;
static RwModuleInfo rasterModule;
static RwInt32 _rwRasterFreeListBlockSize = 128;
static RwInt32 _rwRasterFreeListPreallocBlocks = 1;

struct RwPluginRegistry rasterTKList = {
    sizeof(RwRaster),
    sizeof(RwRaster),
    0,
    0,
    NULL,
    NULL,
};

typedef struct UnkRasterStruct {
    RwRaster* rasterPtrMaybe;
    RwUInt8 unk4[0x24];
    RwUInt32 unk28;
    RwRaster raster;
    RwFreeList* rasterFreeList;
} UnkRasterStruct; // size: 0x64

void* _rwRasterOpen(void* a0, RwInt32 offset, RwInt32) {
    rasterModule.globalsOffset = offset;

    memset(&((UnkRasterStruct*)((RwInt32)RwEngineInstance + rasterModule.globalsOffset))->raster, 0, sizeof(RwRaster));
    ((UnkRasterStruct*)((RwInt32)RwEngineInstance + rasterModule.globalsOffset))->raster.width = 0;
    ((UnkRasterStruct*)((RwInt32)RwEngineInstance + rasterModule.globalsOffset))->raster.height = 0;
    ((UnkRasterStruct*)((RwInt32)RwEngineInstance + rasterModule.globalsOffset))->raster.depth = 0;
    ((UnkRasterStruct*)((RwInt32)RwEngineInstance + rasterModule.globalsOffset))->raster.cFlags = (1 << 7);
    ((UnkRasterStruct*)((RwInt32)RwEngineInstance + rasterModule.globalsOffset))->raster.cpPixels = NULL;
    ((UnkRasterStruct*)((RwInt32)RwEngineInstance + rasterModule.globalsOffset))->raster.palette = NULL;
    ((UnkRasterStruct*)((RwInt32)RwEngineInstance + rasterModule.globalsOffset))->raster.cType = 0;
    ((UnkRasterStruct*)((RwInt32)RwEngineInstance + rasterModule.globalsOffset))->unk28 = 0;
    ((UnkRasterStruct*)((RwInt32)RwEngineInstance + rasterModule.globalsOffset))->rasterPtrMaybe = &((UnkRasterStruct*)((RwInt32)RwEngineInstance + rasterModule.globalsOffset))->raster;

    ((UnkRasterStruct*)((RwInt32)RwEngineInstance + rasterModule.globalsOffset))->rasterFreeList = RwFreeListCreateAndPreallocateSpace(rasterTKList.sizeOfStruct, _rwRasterFreeListBlockSize, 4, _rwRasterFreeListPreallocBlocks, &_rwRasterFreeList);
    if (((UnkRasterStruct*)((RwInt32)RwEngineInstance + rasterModule.globalsOffset))->rasterFreeList == NULL) {
        return NULL;
    } else {
        rasterModule.numInstances++;
    }
    return a0;
}

void* _rwRasterClose(void* a0, RwInt32, RwInt32) {
    if (((UnkRasterStruct*)((RwInt32)RwEngineInstance + rasterModule.globalsOffset))->rasterFreeList != NULL) {
        RwFreeListDestroy(((UnkRasterStruct*)((RwInt32)RwEngineInstance + rasterModule.globalsOffset))->rasterFreeList);
        ((UnkRasterStruct*)((RwInt32)RwEngineInstance + rasterModule.globalsOffset))->rasterFreeList = NULL;
    }
    rasterModule.numInstances--;

    return a0;
}

void* RwRasterLock(RwRaster* raster, RwInt32 a1, RwInt32 a3) {
    void* ret;
    if (RwEngineInstance->stdFunc[15](&ret, raster, a3 + ((a1 & 0xFF) << 8)) != 0) {
        return ret;
    }
    return NULL;
}

RwRaster* RwRasterCreate(RwInt32 width, RwInt32 height, RwInt32 depth, RwInt32 a3) {
    RwStandardFunc func;
    RwRaster* raster = RwEngineInstance->memoryAlloc(((UnkRasterStruct*)((RwInt32)RwEngineInstance + rasterModule.globalsOffset))->rasterFreeList);
    if (raster != NULL) {
        func = RwEngineInstance->stdFunc[4];
        raster->privateFlags = 0;
        raster->cFlags = 0;
        raster->width = width;
        raster->height = height;
        raster->nOffsetX = 0;
        raster->nOffsetY = 0;
        raster->depth = depth;
        raster->parent = raster;
        raster->cpPixels = NULL;
        raster->palette = NULL;
        if (func(NULL, raster, a3) == 0) {
            RwEngineInstance->memoryFree(((UnkRasterStruct*)((RwInt32)RwEngineInstance + rasterModule.globalsOffset))->rasterFreeList, raster);
            return NULL;
        } else {
            _rwPluginRegistryInitObject(&rasterTKList, raster);
        }

        return raster;
    }

    return NULL;
}

extern void _rwResourcesPurge(void);

RwRaster* RwRasterShowRaster(RwRaster* raster, void* a1, RwUInt32 a2) {
    RwStandardFunc func = RwEngineInstance->stdFunc[20];
    _rwResourcesPurge();
    if (func(raster, a1, a2) != 0) {
        return raster;
    }
    return NULL;
}

RwInt32 RwRasterGetNumLevels(RwRaster* raster) {
    RwInt32 ret;
    if (!((raster->cFormat << 8) & (1 << 15))) {
       return 1;
    }

    if (RwEngineInstance->stdFunc[28](&ret, raster, 0) != 0) {
        return ret;
    }
    return -1;
}

void* RwRasterLockPalette(RwRaster* raster, RwInt32 a1) {
    void* ret;
    if (RwEngineInstance->stdFunc[23](&ret, raster, a1) != 0) {
        return ret;
    }
    return NULL;
}

RwInt32 RwRasterRegisterPlugin(RwInt32 size, RwInt32 pluginID, RwPluginObjectConstructor constructCB, RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB) {
    return _rwPluginRegistryAddPlugin(&rasterTKList, size, pluginID, constructCB, destructCB, copyCB);
}

RwBool RwRasterDestroy(RwRaster* raster) {
    _rwPluginRegistryDeInitObject(&rasterTKList, raster);
    RwEngineInstance->stdFunc[5](NULL, raster, 0);
    RwEngineInstance->memoryFree(((UnkRasterStruct*)((RwInt32)RwEngineInstance + rasterModule.globalsOffset))->rasterFreeList, raster);

    return TRUE;
}

RwRaster* RwRasterUnlockPalette(RwRaster* raster) {
    RwEngineInstance->stdFunc[24](NULL, raster, 0);
    raster->privateFlags &= ~((1 << 4) | (1 << 3));
    return raster;
}

RwRaster* RwRasterRender(RwRaster* raster, RwInt32 a1, RwInt32 a2) {
    struct {
        RwInt32 unk0;
        RwInt32 unk4;
    } local_18;
    local_18.unk0 = a1;
    local_18.unk4 = a2;

    RwEngineInstance->stdFunc[17](raster, &local_18, 0);
    return raster;
}

RwRaster* RwRasterRenderFast(RwRaster* raster, RwInt32 a1, RwInt32 a2) {
    struct {
        RwInt32 unk0;
        RwInt32 unk4;
    } local_18;
    local_18.unk0 = a1;
    local_18.unk4 = a2;

    RwEngineInstance->stdFunc[19](raster, &local_18, 0);
    return raster;
}

RwRaster* RwRasterUnlock(RwRaster* raster) {
    RwEngineInstance->stdFunc[16](NULL, raster, 0);
    return raster;
}
