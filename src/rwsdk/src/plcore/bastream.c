#include <stddef.h>
#include <string.h>
#include <rwsdk/badevice.h>
#include <rwsdk/plcore/baerr.h>
#include <rwsdk/plcore/bamemory.h>
#include <rwsdk/plcore/bastream.h>

static RwFreeList _rwStreamFreeList;
static RwModuleInfo streamModule;
static int _rwStreamFreeListBlockSize = 16;
static int _rwStreamFreeListPreallocBlocks = 1;

// Unknown name
#define GET_MODULE_FREELIST(module) \
    *(RwFreeList**)((int)RwEngineInstance + (module).globalsOffset)

void* _rwStreamModuleOpen(void* a0, int offset, int) {
    streamModule.globalsOffset = offset;
    GET_MODULE_FREELIST(streamModule) = RwFreeListCreateAndPreallocateSpace(sizeof(RwStream), _rwStreamFreeListBlockSize, 4, _rwStreamFreeListPreallocBlocks, &_rwStreamFreeList);
    if (GET_MODULE_FREELIST(streamModule) == NULL) {
        return NULL;
    }

    streamModule.numInstances++;
    return a0;
}

void* _rwStreamModuleClose(void* a0, int, int) {
    if (GET_MODULE_FREELIST(streamModule) != NULL) {
        RwFreeListDestroy(GET_MODULE_FREELIST(streamModule));
    }
    streamModule.numInstances--;
    return a0;
}

static RwStream* StreamFileNameInitialize(RwStream* stream, RwStreamAccessType accessType, char* fileName) {
    RwStream* ret = NULL;
    void* file = NULL;

    switch (accessType) {
        case rwSTREAMREAD:
            file = RwEngineInstance->fileFuncs.rwfopen(fileName, "rb");
            break;
        case rwSTREAMWRITE:
            file = RwEngineInstance->fileFuncs.rwfopen(fileName, "wb");
            break;
        case rwSTREAMAPPEND:
            file = RwEngineInstance->fileFuncs.rwfopen(fileName, "ab");
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

RwStream* _rwStreamInitialize(RwStream* stream, int rwOwned, RwStreamType type, RwStreamAccessType accessType, void* data) {
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
            if (RwEngineInstance->fileFuncs.rwftell(data) == -1) {
                retret = NULL;
            } else {
                stream->Type.file.fpFile = data;
                retret = stream;
            }
            ret = retret;
            break;
        }
        case rwSTREAMFILENAME:
            ret = StreamFileNameInitialize(stream, accessType, (char*)data);
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

unsigned int RwStreamRead(RwStream* stream, void* dest, unsigned int size) {
    switch (stream->type) {
        case rwSTREAMFILE:
        case rwSTREAMFILENAME: {
            void* file = stream->Type.file.fpFile;
            unsigned int nRead = RwEngineInstance->fileFuncs.rwfread(dest, 1, size, file);
            if (nRead != size) {
                if (RwEngineInstance->fileFuncs.rwfeof(file)) {
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

RwStream* RwStreamWrite(RwStream* stream, const void* src, unsigned int size) {
    switch (stream->type) {
        case rwSTREAMFILE:
        case rwSTREAMFILENAME: {
            void* file = stream->Type.file.fpFile;
            unsigned int nWrote = RwEngineInstance->fileFuncs.rwfwrite(src, 1, size, file);
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
                int newSize = size + stream->Type.memory.nSize;
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

RwStream* RwStreamSkip(RwStream* stream, unsigned int size) {
    if (size == 0) {
        return stream;
    }

    switch (stream->type) {
        case rwSTREAMFILE:
        case rwSTREAMFILENAME: {
            void* file = stream->Type.file.fpFile;
            if (RwEngineInstance->fileFuncs.rwfseek(file, size, 1) != 0) {
                if (RwEngineInstance->fileFuncs.rwfeof(file)) {
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

int RwStreamClose(RwStream* stream, void* data) {
    int ret;

    switch (stream->type) {
        case rwSTREAMFILE:
            ret = 1;
            break;
        case rwSTREAMFILENAME:
            ret = RwEngineInstance->fileFuncs.rwfclose(stream->Type.file.fpFile) == 0 ? 1 : 0;
            break;
        case rwSTREAMMEMORY:
            if (stream->accessType != rwSTREAMREAD && (RwMemory*)data != NULL) {
                ((RwMemory*)data)->start = stream->Type.memory.memBlock;
                ((RwMemory*)data)->length = stream->Type.memory.position;
            }
            ret = 1;
            break;
        case rwSTREAMCUSTOM:
            if (stream->Type.custom.sfnclose != NULL) {
                stream->Type.custom.sfnclose(stream->Type.custom.data);
            }
            ret = 1;
            break;
        default:
            RwThrowError(1, E_RW_INVSTREAMTYPE);
            return 0;
    }

    if (stream->rwOwned) {
        RwEngineInstance->memoryFree(GET_MODULE_FREELIST(streamModule), stream);
    }

    return ret;
}

RwStream* RwStreamOpen(RwStreamType type, RwStreamAccessType accessType, void* data) {
    RwStream* stream = RwEngineInstance->memoryAlloc(GET_MODULE_FREELIST(streamModule));
    if (!_rwStreamInitialize(stream, 1, type, accessType, data)) {
        RwEngineInstance->memoryFree(GET_MODULE_FREELIST(streamModule), stream);
        stream = NULL;
    }
    return stream;
}
