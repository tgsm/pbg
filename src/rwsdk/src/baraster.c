#include <cstring>
#include <rwsdk/plcore/bamemory.h>
#include <rwsdk/badevice.h>
#include <rwsdk/baraster.h>

static RwFreeList _rwRasterFreeList;
static RwModuleInfo rasterModule;
static int _rwRasterFreeListBlockSize = 128;
static int _rwRasterFreeListPreallocBlocks = 1;

// TODO
struct UnkRwPluginRegistryStruct {
    int unk0;
    int unk4;
    char unk8[0x10];
};
struct UnkRwPluginRegistryStruct rasterTKList = {
    sizeof(RwRaster),
    sizeof(RwRaster),
};
extern void _rwPluginRegistryInitObject(struct UnkRwPluginRegistryStruct*, void* object);
extern void _rwPluginRegistryDeInitObject(struct UnkRwPluginRegistryStruct*, void* object);

typedef struct UnkRasterStruct {
    RwRaster* rasterPtrMaybe;
    char unk4[0x24];
    unsigned int unk28;
    RwRaster raster;
    RwFreeList* rasterFreeList;
} UnkRasterStruct; // size: 0x64

void* _rwRasterOpen(void* a0, int offset) {
    rasterModule.globalsOffset = offset;

    memset(&((UnkRasterStruct*)((int)RwEngineInstance + rasterModule.globalsOffset))->raster, 0, sizeof(RwRaster));
    ((UnkRasterStruct*)((int)RwEngineInstance + rasterModule.globalsOffset))->raster.width = 0;
    ((UnkRasterStruct*)((int)RwEngineInstance + rasterModule.globalsOffset))->raster.height = 0;
    ((UnkRasterStruct*)((int)RwEngineInstance + rasterModule.globalsOffset))->raster.depth = 0;
    ((UnkRasterStruct*)((int)RwEngineInstance + rasterModule.globalsOffset))->raster.cFlags = (1 << 7);
    ((UnkRasterStruct*)((int)RwEngineInstance + rasterModule.globalsOffset))->raster.cpPixels = NULL;
    ((UnkRasterStruct*)((int)RwEngineInstance + rasterModule.globalsOffset))->raster.palette = NULL;
    ((UnkRasterStruct*)((int)RwEngineInstance + rasterModule.globalsOffset))->raster.cType = 0;
    ((UnkRasterStruct*)((int)RwEngineInstance + rasterModule.globalsOffset))->unk28 = 0;
    ((UnkRasterStruct*)((int)RwEngineInstance + rasterModule.globalsOffset))->rasterPtrMaybe = &((UnkRasterStruct*)((int)RwEngineInstance + rasterModule.globalsOffset))->raster;

    ((UnkRasterStruct*)((int)RwEngineInstance + rasterModule.globalsOffset))->rasterFreeList = RwFreeListCreateAndPreallocateSpace(rasterTKList.unk0, _rwRasterFreeListBlockSize, 4, _rwRasterFreeListPreallocBlocks, &_rwRasterFreeList);
    if (((UnkRasterStruct*)((int)RwEngineInstance + rasterModule.globalsOffset))->rasterFreeList == NULL) {
        return NULL;
    } else {
        rasterModule.numInstances++;
    }
    return a0;
}

void* _rwRasterClose(void* a0) {
    if (((UnkRasterStruct*)((int)RwEngineInstance + rasterModule.globalsOffset))->rasterFreeList != NULL) {
        RwFreeListDestroy(((UnkRasterStruct*)((int)RwEngineInstance + rasterModule.globalsOffset))->rasterFreeList);
        ((UnkRasterStruct*)((int)RwEngineInstance + rasterModule.globalsOffset))->rasterFreeList = NULL;
    }
    rasterModule.numInstances--;

    return a0;
}

void* RwRasterLock(RwRaster* raster, int a1, int a3) {
    void* ret;
    if (RwEngineInstance->unk84(&ret, raster, a3 + ((a1 & 0xFF) << 8)) != 0) {
        return ret;
    }
    return NULL;
}

RwRaster* RwRasterCreate(int width, int height, int depth, int a3) {
    int (*unk58)(void*, void*, int);
    RwRaster* raster = RwEngineInstance->unk140(((UnkRasterStruct*)((int)RwEngineInstance + rasterModule.globalsOffset))->rasterFreeList);
    if (raster != NULL) {
        unk58 = RwEngineInstance->unk58;
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
        if (unk58(NULL, raster, a3) == 0) {
            RwEngineInstance->unk144(((UnkRasterStruct*)((int)RwEngineInstance + rasterModule.globalsOffset))->rasterFreeList, raster);
            return NULL;
        } else {
            _rwPluginRegistryInitObject(&rasterTKList, raster);
        }

        return raster;
    }

    return NULL;
}

extern void _rwResourcesPurge(void);

RwRaster* RwRasterShowRaster(RwRaster* raster, void* a1, unsigned int a2) {
    int (*unk98)(void*, void*, int) = RwEngineInstance->unk98;
    _rwResourcesPurge();
    if (unk98(raster, a1, a2) != 0) {
        return raster;
    }
    return NULL;
}

int RwRasterGetNumLevels(RwRaster* raster) {
    int ret;
    if (!((raster->cFormat << 8) & (1 << 15))) {
       return 1;
    }

    if (RwEngineInstance->unkB8(&ret, raster, 0) != 0) {
        return ret;
    }
    return -1;
}

void* RwRasterLockPalette(RwRaster* raster, int a1) {
    void* ret;
    if (RwEngineInstance->unkA4(&ret, raster, a1) != 0) {
        return ret;
    }
    return NULL;
}

// FIXME: Stubbed param types
extern int _rwPluginRegistryAddPlugin(struct UnkRwPluginRegistryStruct*, unsigned int size, unsigned int, void*, void*, void*);

int RwRasterRegisterPlugin(unsigned int size, unsigned int a1, void* a2, void* a3, void* a4) {
    return _rwPluginRegistryAddPlugin(&rasterTKList, size, a1, a2, a3, a4);
}

int RwRasterDestroy(RwRaster* raster) {
    _rwPluginRegistryDeInitObject(&rasterTKList, raster);
    RwEngineInstance->unk5C(NULL, raster, 0);
    RwEngineInstance->unk144(((UnkRasterStruct*)((int)RwEngineInstance + rasterModule.globalsOffset))->rasterFreeList, raster);

    return 1;
}

RwRaster* RwRasterUnlockPalette(RwRaster* raster) {
    RwEngineInstance->unkA8(NULL, raster, 0);
    raster->privateFlags &= ~((1 << 4) | (1 << 3));
    return raster;
}

RwRaster* RwRasterRender(RwRaster* raster, int a1, int a2) {
    struct {
        int unk0;
        int unk4;
    } local_18;
    local_18.unk0 = a1;
    local_18.unk4 = a2;

    RwEngineInstance->unk8C(raster, &local_18, 0);
    return raster;
}

RwRaster* RwRasterRenderFast(RwRaster* raster, int a1, int a2) {
    struct {
        int unk0;
        int unk4;
    } local_18;
    local_18.unk0 = a1;
    local_18.unk4 = a2;

    RwEngineInstance->unk94(raster, &local_18, 0);
    return raster;
}

RwRaster* RwRasterUnlock(RwRaster* raster) {
    RwEngineInstance->unk88(NULL, raster, 0);
    return raster;
}
