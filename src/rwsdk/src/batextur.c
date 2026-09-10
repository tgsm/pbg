#include "dont_inline_hack.h"
#include <rwsdk/badevice.h>
#include <rwsdk/batextur.h>
#include <rwsdk/plcore/baerr.h>
#include <rwsdk/plcore/bamemory.h>

static RwFreeList _rwTexDictionaryFreeList;
static RwFreeList _rwTextureFreeList;

RwPluginRegistry textureTKList = {
    sizeof(RwTexture),
    sizeof(RwTexture),
    0,
    0,
    NULL,
    NULL,
};

RwPluginRegistry texDictTKList = {
    sizeof(RwTexDictionary),
    sizeof(RwTexDictionary),
    0,
    0,
    NULL,
    NULL,
};

static RwModuleInfo textureModule;
static RwTexDictionary* dummyTexDict;

struct UnkTextureModule {
    RwLLLink link;
    RwFreeList* textureFreeList;
    RwFreeList* texDictFreeList;
    RwTexDictionary* currentTexDictionary;
    RwTextureCallBackRead textureReadCB;
    RwBool mipmapping;
    RwBool autoMipmapping;
    void* unk20;
    RwUInt16 unk24;
    RwTextureCallBackMipmapGeneration rasterBuildMipmapsCB;
    RwTextureCallBackMipmapName mipmapNameCB;
};

#define GET_TEXTURE_MODULE() \
    ((struct UnkTextureModule*)((int)RwEngineInstance + textureModule.globalsOffset))

static RwBool TextureDefaultMipmapName(RwChar* a0, RwChar* a1, RwUInt8 a2, RwInt32) {
    static const RwChar character[] = "0123456789abcdef";

    RwChar name[3];
    name[0] = 'm';
    name[1] = (a2 != 0 && a2 < sizeof(character) - 1) ? character[a2] : '\0';
    name[2] = '\0';
    if (name[1] != '\0') {
        RwStrcat(a0, name);
        if (a1 != NULL && *a1 != '\0') {
            RwStrcat(a1, name);
        }
    }

    return TRUE;
}

static RwTexture* TextureDefaultRead(const char*, const char*);
static RwRaster* TextureRasterDefaultBuildMipmaps(RwRaster*, RwImage*);

static RwBool StringCompare(const RwChar* a, const RwChar* b) {
    DONT_INLINE_HACK();

    while (*a != '\0' && *b != '\0') {
        RwChar a_ = *a;
        RwChar b_ = *b;

        if (a_ >= 'a' && a_ <= 'z') {
            a_ -= 0x20;
        }
        if (b_ >= 'a' && b_ <= 'z') {
            b_ -= 0x20;
        }

        if (a_ != b_) {
            return FALSE;
        }

        a++;
        b++;
    }

    if (*a == *b) {
        return TRUE;
    }

    return FALSE;
}

RwBool RwTextureSetMipmapping(RwBool mipmap) {
    GET_TEXTURE_MODULE()->mipmapping = mipmap;
    return TRUE;
}

RwBool RwTextureGetMipmapping(void) {
    return GET_TEXTURE_MODULE()->mipmapping;
}

RwBool RwTextureSetAutoMipmapping(RwBool autoMipmap) {
    GET_TEXTURE_MODULE()->autoMipmapping = autoMipmap;
    return TRUE;
}

RwBool RwTextureGetAutoMipmapping(void) {
    return GET_TEXTURE_MODULE()->autoMipmapping;
}

RwTexture* RwTextureSetRaster(RwTexture* texture, RwRaster* raster) {
    if (raster != NULL) {
        if (RwEngineInstance->stdFunc[8](texture, raster, 0)) {
            return texture;
        } else {
            return NULL;
        }
    } else {
        texture->raster = NULL;
    }
    return texture;
}

RwTexDictionary* RwTexDictionaryCreate(void) {
    RwTexDictionary* dict = RwEngineInstance->memoryAlloc(GET_TEXTURE_MODULE()->texDictFreeList);
    if (dict == NULL) {
        return NULL;
    }

    dict->object.type = 6;
    dict->object.subType = 0;
    // FIXME: This was probably two separate statements, but this function won't be inlined in _rwTextureOpen if they are separate.
    dict->object.privateFlags = dict->object.flags = 0;
    dict->object.parent = NULL;
    dict->lInInstance.next = GET_TEXTURE_MODULE()->link.next;
    dict->lInInstance.prev = &GET_TEXTURE_MODULE()->link;

    GET_TEXTURE_MODULE()->link.next->prev = &dict->lInInstance;
    GET_TEXTURE_MODULE()->link.next = &dict->lInInstance;

    dict->texturesInDict.link.next = &dict->texturesInDict.link;
    dict->texturesInDict.link.prev = &dict->texturesInDict.link;
    _rwPluginRegistryInitObject(&texDictTKList, dict);

    return dict;
}

RwBool RwTexDictionaryDestroy(RwTexDictionary* dict) {
    if (GET_TEXTURE_MODULE()->currentTexDictionary == dict) {
        GET_TEXTURE_MODULE()->currentTexDictionary = NULL;
    }

    RwTexDictionaryForAllTextures(dict, (RwTextureCallBack)RwTextureDestroy, 0);
    _rwPluginRegistryDeInitObject(&texDictTKList, dict);

    dict->lInInstance.prev->next = dict->lInInstance.next;
    dict->lInInstance.next->prev = dict->lInInstance.prev;

    RwEngineInstance->memoryFree(GET_TEXTURE_MODULE()->texDictFreeList, dict);
    return TRUE;
}

// Equivalent: regalloc
RwTexDictionary* RwTexDictionaryForAllTextures(RwTexDictionary* dict, RwTextureCallBack callback, void* data) {
    RwLLLink* link = dict->texturesInDict.link.next;
    while (link != &dict->texturesInDict.link) {
        RwTexture* texture = (RwTexture*)(&link[-1]);
        RwLLLink* next = link->next;
        if (!callback(texture, data)) {
            break;
        }
        link = next;
    }

    return dict;
}

RwBool RwTexDictionaryForAllTexDictionaries(RwTexDictionaryCallBack callback, void* data) {
    RwLLLink* link = GET_TEXTURE_MODULE()->link.next;
    RwLLLink* end = &GET_TEXTURE_MODULE()->link;
    while (link != end) {
        RwTexDictionary* dict = (RwTexDictionary*)(&link[-2]); // is this real?
        RwLLLink* next = link->next;
        if (!callback(dict, data)) {
            break;
        }
        link = next;
    }

    return TRUE;
}

RwTexture* RwTextureCreate(RwRaster* raster) {
    RwTexture* texture = RwEngineInstance->memoryAlloc(GET_TEXTURE_MODULE()->textureFreeList);
    if (texture != NULL) {
        texture->dict = NULL;
        texture->name[0] = '\0';
        texture->mask[0] = '\0';
        texture->raster = raster;
        texture->refCount = 1;
        texture->filterAddressing = 0;
        texture->filterAddressing = (texture->filterAddressing & 0xFFFF00FF) | (0x11 << 8);
        texture->filterAddressing = (texture->filterAddressing & 0xFFFFFF00) | 0x1;

        _rwPluginRegistryInitObject(&textureTKList, texture);
    }

    return texture;
}

RwBool RwTextureDestroy(RwTexture* texture) {
    RwBool ret = TRUE;
    texture->refCount--;
    if (texture->refCount <= 0) {
        texture->refCount++;
        _rwPluginRegistryDeInitObject(&textureTKList, texture);
        if (texture->dict != NULL) {
            texture->lInDictionary.prev->next = texture->lInDictionary.next;
            texture->lInDictionary.next->prev = texture->lInDictionary.prev;
        }
        if (texture->raster != NULL) {
            RwRasterDestroy(texture->raster);
            texture->raster = NULL;
        }
        texture->refCount--;
        RwEngineInstance->memoryFree(GET_TEXTURE_MODULE()->textureFreeList, texture);
        ret = TRUE;
    }

    return ret;
}

RwTexture* RwTextureSetName(RwTexture* texture, char* name) {
    RwStrncpy(texture->name, name, sizeof(texture->name));
    if (RwStrlen(name) >= sizeof(texture->name)) {
        RwThrowErrorParams(1, E_RW_STRING_TRUNCATION, name, sizeof(texture->name), sizeof(texture->name) - 1, name[sizeof(texture->name) - 1]);
        texture->name[sizeof(texture->name) - 1] = '\0';
    }

    return texture;
}

RwTexture* RwTextureSetMaskName(RwTexture* texture, char* mask) {
    RwStrncpy(texture->mask, mask, sizeof(texture->mask));
    if (RwStrlen(mask) >= sizeof(texture->mask)) {
        RwThrowErrorParams(1, E_RW_STRING_TRUNCATION, mask, sizeof(texture->mask), sizeof(texture->mask) - 1, mask[sizeof(texture->mask) - 1]);
        texture->mask[sizeof(texture->mask) - 1] = '\0';
    }

    return texture;
}

RwTexture* RwTexDictionaryAddTexture(RwTexDictionary* dict, RwTexture* texture) {
    if (texture->dict != NULL) {
        texture->lInDictionary.prev->next = texture->lInDictionary.next;
        texture->lInDictionary.next->prev = texture->lInDictionary.prev;
    }
    texture->dict = dict;
    texture->lInDictionary.next = dict->texturesInDict.link.next;
    texture->lInDictionary.prev = &dict->texturesInDict.link;
    dict->texturesInDict.link.next->prev = &texture->lInDictionary;
    dict->texturesInDict.link.next = &texture->lInDictionary;

    return texture;
}

RwTexture* RwTexDictionaryFindNamedTexture(RwTexDictionary* dict, char* name) {
    RwTexture* texture;
    RwLLLink* link;
    RwLinkList* list = &dict->texturesInDict;
    for (link = list->link.next; link != &list->link; link = link->next) {
        texture = (RwTexture*)(&link[-1]); // is this real?

        if (texture->name != NULL && StringCompare(texture->name, name)) {
            return texture;
        }
    }

    return NULL;
}

RwTexDictionary* RwTexDictionaryGetCurrent(void) {
    return GET_TEXTURE_MODULE()->currentTexDictionary;
}

RwBool RwTextureGenerateMipmapName(RwChar* a0, RwChar* a1, RwUInt8 a2, RwInt32 a3) {
    RwTextureCallBackMipmapName callback = GET_TEXTURE_MODULE()->mipmapNameCB;
    if (callback != NULL) {
        return callback(a0, a1, a2, a3);
    }

    return FALSE;
}

RwInt32 RwTextureRegisterPlugin(RwInt32 size, RwInt32 pluginID, RwPluginObjectConstructor constructCB, RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB) {
    return _rwPluginRegistryAddPlugin(&textureTKList, size, pluginID, constructCB, destructCB, copyCB);
}

RwBool RwTextureRasterGenerateMipmaps(RwRaster* raster, RwImage* a1) {
    return GET_TEXTURE_MODULE()->rasterBuildMipmapsCB(raster, a1) != NULL;
}

static RwInt32 _rwTextureFreeListBlockSize = 128;
static RwInt32 _rwTextureFreeListPreallocBlocks = 1;
static RwInt32 _rwTexDictionaryFreeListBlockSize = 5;
static RwInt32 _rwTexDictionaryFreeListPreallocBlocks = 1;

void* _rwTextureOpen(void* object, RwInt32 offset, RwInt32 size) {
    textureModule.globalsOffset = offset;

    GET_TEXTURE_MODULE()->textureFreeList = RwFreeListCreateAndPreallocateSpace(textureTKList.sizeOfStruct, _rwTextureFreeListBlockSize, 4, _rwTextureFreeListPreallocBlocks, &_rwTextureFreeList);
    if (GET_TEXTURE_MODULE()->textureFreeList == NULL) {
        return NULL;
    }

    GET_TEXTURE_MODULE()->texDictFreeList = RwFreeListCreateAndPreallocateSpace(texDictTKList.sizeOfStruct, _rwTexDictionaryFreeListBlockSize, 4, _rwTexDictionaryFreeListPreallocBlocks, &_rwTexDictionaryFreeList);
    if (GET_TEXTURE_MODULE()->texDictFreeList == NULL) {
        RwFreeListDestroy(GET_TEXTURE_MODULE()->textureFreeList);
        GET_TEXTURE_MODULE()->textureFreeList = NULL;
        return NULL;
    }

    GET_TEXTURE_MODULE()->link.next = &GET_TEXTURE_MODULE()->link;
    GET_TEXTURE_MODULE()->link.prev = &GET_TEXTURE_MODULE()->link;

    textureModule.numInstances++;

    dummyTexDict = RwTexDictionaryCreate();
    GET_TEXTURE_MODULE()->currentTexDictionary = dummyTexDict;
    if (GET_TEXTURE_MODULE()->currentTexDictionary == NULL) {
        RwFreeListDestroy(GET_TEXTURE_MODULE()->texDictFreeList);
        GET_TEXTURE_MODULE()->texDictFreeList = NULL;
        RwFreeListDestroy(GET_TEXTURE_MODULE()->textureFreeList);
        GET_TEXTURE_MODULE()->textureFreeList = NULL;
        return NULL;
    }

    GET_TEXTURE_MODULE()->mipmapping = FALSE;
    GET_TEXTURE_MODULE()->autoMipmapping = FALSE;
    GET_TEXTURE_MODULE()->textureReadCB = TextureDefaultRead;
    GET_TEXTURE_MODULE()->rasterBuildMipmapsCB = TextureRasterDefaultBuildMipmaps;
    GET_TEXTURE_MODULE()->mipmapNameCB = TextureDefaultMipmapName;
    GET_TEXTURE_MODULE()->unk20 = NULL;
    GET_TEXTURE_MODULE()->unk24 = 0;

    return object;
}
