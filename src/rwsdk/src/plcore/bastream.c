#include <stddef.h>
#include <string.h>
#include <rwsdk/badevice.h>
#include <rwsdk/plcore/baerr.h>
#include <rwsdk/plcore/bamemory.h>
#include <rwsdk/plcore/bastream.h>

static RwFreeList _rwStreamFreeList;
static RwModuleInfo streamModule;
static RwInt32 _rwStreamFreeListBlockSize = 16;
static RwInt32 _rwStreamFreeListPreallocBlocks = 1;

// Unknown name
#define GET_MODULE_FREELIST(module) \
    *(RwFreeList**)((RwInt32)RwEngineInstance + (module).globalsOffset)

void* _rwStreamModuleOpen(void* a0, RwInt32 offset, RwInt32) {
    streamModule.globalsOffset = offset;
    GET_MODULE_FREELIST(streamModule) = RwFreeListCreateAndPreallocateSpace(sizeof(RwStream), _rwStreamFreeListBlockSize, 4, _rwStreamFreeListPreallocBlocks, &_rwStreamFreeList);
    if (GET_MODULE_FREELIST(streamModule) == NULL) {
        return NULL;
    }

    streamModule.numInstances++;
    return a0;
}

void* _rwStreamModuleClose(void* a0, RwInt32, RwInt32) {
    if (GET_MODULE_FREELIST(streamModule) != NULL) {
        RwFreeListDestroy(GET_MODULE_FREELIST(streamModule));
    }
    streamModule.numInstances--;
    return a0;
}

static RwStream* StreamFileNameInitialize(RwStream* stream, RwStreamAccessType accessType, RwChar* fileName) {
    RwStream* ret = NULL;
    void* file = NULL;

    switch (accessType) {
        case rwSTREAMREAD:
            file = RwFopen(fileName, "rb");
            break;
        case rwSTREAMWRITE:
            file = RwFopen(fileName, "wb");
            break;
        case rwSTREAMAPPEND:
            file = RwFopen(fileName, "ab");
            break;
        default:
            RwThrowError(1, E_RW_INVSTREAMACCESSTYPE);
            break;
    }

    if (file != NULL) {
        stream->Type.file.fpFile = file;
        ret = stream;
    } else {
        RwThrowErrorParams(1, E_RW_BADOPEN, fileName);
    }

    return ret;
}

static RwStream* StreamMemoryInitialize(RwStream* stream, RwStreamAccessType accessType, RwMemory* memory) {
    RwStream* ret = NULL;

    switch (accessType) {
        case rwSTREAMREAD:
            stream->Type.memory.position = 0;
            stream->Type.memory.nSize = memory->length;
            stream->Type.memory.memBlock = memory->start;
            ret = stream;
            break;
        case rwSTREAMWRITE:
            stream->Type.memory.position = 0;
            stream->Type.memory.nSize = 0;
            stream->Type.memory.memBlock = NULL;
            ret = stream;
            break;
        case rwSTREAMAPPEND:
            stream->Type.memory.position = memory->length;
            stream->Type.memory.nSize = memory->length;
            stream->Type.memory.memBlock = memory->start;
            ret = stream;
            break;
        default:
            RwThrowError(1, E_RW_INVSTREAMACCESSTYPE);
            break;
    }

    return ret;
}

RwStream* _rwStreamInitialize(RwStream* stream, RwBool rwOwned, RwStreamType type, RwStreamAccessType accessType, void* data) {
    RwStream* ret = NULL;

    if (stream == NULL) {
        return NULL;
    }

    stream->type = type;
    stream->accessType = accessType;
    stream->rwOwned = rwOwned;

    switch (type) {
        case rwSTREAMFILE: {
            RwStream* retret;
            if (RwFtell(data) == -1) {
                retret = NULL;
            } else {
                stream->Type.file.fpFile = data;
                retret = stream;
            }
            ret = retret;
            break;
        }
        case rwSTREAMFILENAME:
            ret = StreamFileNameInitialize(stream, accessType, (RwChar*)data);
            break;
        case rwSTREAMMEMORY:
            ret = StreamMemoryInitialize(stream, accessType, (RwMemory*)data);
            break;
        case rwSTREAMCUSTOM:
            memcpy(&stream->Type.custom, (RwStreamCustom*)data, sizeof(RwStreamCustom));
            ret = stream;
            break;
        default:
            RwThrowError(1, E_RW_INVSTREAMTYPE);
            break;
    }

    return ret;
}

RwUInt32 RwStreamRead(RwStream* stream, void* dest, RwUInt32 size) {
    switch (stream->type) {
        case rwSTREAMFILE:
        case rwSTREAMFILENAME: {
            void* file = stream->Type.file.fpFile;
            RwUInt32 nRead = RwFread(dest, 1, size, file);
            if (nRead != size) {
                if (RwFeof(file)) {
                    RwThrowError(1, E_RW_ENDOFSTREAM);
                } else {
                    RwThrowError(1, E_RW_READ);
                }
            }

            return nRead;
        }
        case rwSTREAMMEMORY:
            if (size > stream->Type.memory.nSize - stream->Type.memory.position) {
                size = stream->Type.memory.nSize - stream->Type.memory.position;
                RwThrowError(1, E_RW_ENDOFSTREAM);
            }

            memcpy(dest, stream->Type.memory.memBlock + stream->Type.memory.position, size);
            stream->Type.memory.position += size;
            return size;
        case rwSTREAMCUSTOM:
            return stream->Type.custom.sfnread(stream->Type.custom.data, dest, size);
        default:
            RwThrowError(1, E_RW_INVSTREAMTYPE);
            return 0;
    }
}

RwStream* RwStreamWrite(RwStream* stream, const void* src, RwUInt32 size) {
    switch (stream->type) {
        case rwSTREAMFILE:
        case rwSTREAMFILENAME: {
            void* file = stream->Type.file.fpFile;
            RwUInt32 nWrote = RwFwrite(src, 1, size, file);
            if (nWrote != size) {
                RwThrowError(1, E_RW_WRITE);
                return NULL;
            }

            return stream;
        }
        case rwSTREAMMEMORY:
            if (stream->Type.memory.memBlock == NULL) {
                stream->Type.memory.memBlock = RwMalloc(512);
                if (stream->Type.memory.memBlock == NULL) {
                    RwThrowErrorParams(1, E_RW_NOMEM, 512);
                    return NULL;
                }
                stream->Type.memory.nSize = 512;
            }

            if (stream->Type.memory.nSize - stream->Type.memory.position < size) {
                void* newPtr;
                RwInt32 newSize = size + stream->Type.memory.nSize;
                if (size < 512) {
                    newSize = stream->Type.memory.nSize + 512;
                }

                newPtr = RwRealloc(stream->Type.memory.memBlock, newSize);
                if (newPtr == NULL) {
                    RwThrowErrorParams(1, E_RW_NOMEM, newSize - stream->Type.memory.nSize);
                    return NULL;
                }

                stream->Type.memory.memBlock = newPtr;
                stream->Type.memory.nSize = newSize;
            }

            memcpy(stream->Type.memory.memBlock + stream->Type.memory.position, src, size);
            stream->Type.memory.position += size;
            return stream;
        case rwSTREAMCUSTOM:
            if (stream->Type.custom.sfnwrite(stream->Type.custom.data, src, size)) {
                return stream;
            }
            return NULL;
        default:
            RwThrowError(1, E_RW_INVSTREAMTYPE);
            return 0;
    }
}

RwStream* RwStreamSkip(RwStream* stream, RwUInt32 size) {
    if (size == 0) {
        return stream;
    }

    switch (stream->type) {
        case rwSTREAMFILE:
        case rwSTREAMFILENAME: {
            void* file = stream->Type.file.fpFile;
            if (RwFseek(file, size, 1) != 0) {
                if (RwFeof(file)) {
                    RwThrowError(1, E_RW_ENDOFSTREAM);
                }
                return NULL;
            }

            return stream;
        }
        case rwSTREAMMEMORY:
            if (stream->Type.memory.position + size > stream->Type.memory.nSize) {
                stream->Type.memory.position = stream->Type.memory.nSize;
                RwThrowError(1, E_RW_ENDOFSTREAM);
                return NULL;
            }

            stream->Type.memory.position += size;
            return stream;
        case rwSTREAMCUSTOM:
            if (stream->Type.custom.sfnskip(stream->Type.custom.data, size)) {
                return stream;
            }
            return NULL;
        default:
            RwThrowError(1, E_RW_INVSTREAMTYPE);
            return NULL;
    }
}

RwBool RwStreamClose(RwStream* stream, void* data) {
    RwBool ret;

    switch (stream->type) {
        case rwSTREAMFILE:
            ret = TRUE;
            break;
        case rwSTREAMFILENAME:
            ret = RwFclose(stream->Type.file.fpFile) == 0 ? TRUE : FALSE;
            break;
        case rwSTREAMMEMORY:
            if (stream->accessType != rwSTREAMREAD && (RwMemory*)data != NULL) {
                ((RwMemory*)data)->start = stream->Type.memory.memBlock;
                ((RwMemory*)data)->length = stream->Type.memory.position;
            }
            ret = TRUE;
            break;
        case rwSTREAMCUSTOM:
            if (stream->Type.custom.sfnclose != NULL) {
                stream->Type.custom.sfnclose(stream->Type.custom.data);
            }
            ret = TRUE;
            break;
        default:
            RwThrowError(1, E_RW_INVSTREAMTYPE);
            return FALSE;
    }

    if (stream->rwOwned) {
        RwEngineInstance->memoryFree(GET_MODULE_FREELIST(streamModule), stream);
    }

    return ret;
}

RwStream* RwStreamOpen(RwStreamType type, RwStreamAccessType accessType, void* data) {
    RwStream* stream = RwEngineInstance->memoryAlloc(GET_MODULE_FREELIST(streamModule));
    if (!_rwStreamInitialize(stream, TRUE, type, accessType, data)) {
        RwEngineInstance->memoryFree(GET_MODULE_FREELIST(streamModule), stream);
        stream = NULL;
    }
    return stream;
}
