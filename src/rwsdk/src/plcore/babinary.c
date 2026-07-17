#include "rwsdk/plcore/baerr.h"
#include <rwsdk/plcore/babinary.h>
#include <string.h>

typedef struct UnkRwChunkHeader {
    RwUInt32 type;
    RwUInt32 length;
    RwUInt32 version;
} UnkRwChunkHeader;

static RwBool ChunkIsComplex(RwChunkHeaderInfo* info) {
    switch (info->type) {
        case 1:
            return FALSE;
        case 2:
            return FALSE;
        case 3:
            return FALSE;
        case 5:
            return TRUE;
        case 6:
            return TRUE;
        case 7:
            return TRUE;
        case 8:
            return TRUE;
        case 9:
            return TRUE;
        case 10:
            return TRUE;
        case 11:
            return TRUE;
        case 13:
            return FALSE;
        case 14:
            return TRUE;
        case 15:
            return TRUE;
        case 16:
            return TRUE;
        case 18:
            return TRUE;
        case 19:
            return FALSE;
        case 20:
            return TRUE;
        case 26:
            return TRUE;
        default:
            return FALSE;
    }
}

RwBool _rwStreamReadChunkHeader(RwStream* stream, RwUInt32* type, RwUInt32* length, RwUInt32* version, RwUInt32* buildNum) {
    UnkRwChunkHeader header;

    RwBool success = RwStreamRead(stream, &header, sizeof(UnkRwChunkHeader)) == sizeof(UnkRwChunkHeader);
    if (!success) {
        RwThrowError(1, E_RW_READ);
        return FALSE;
    }

    RwMemNative32((RwUInt32*)&header, sizeof(UnkRwChunkHeader));

    if (type != NULL) {
        *type = header.type;
    }

    if (length != NULL) {
        *length = header.length;
    }

    if ((header.version & 0xFFFF0000) == 0) {
        if (version != NULL) {
            *version = header.version << 8;
        }
        if (buildNum != NULL) {
            *buildNum = 0;
        }
    } else {
        if (version != NULL) {
            *version = ((header.version >> 14) & 0x3FF00) + 0x30000 | ((header.version >> 16) & 0x3F);
        }
        if (buildNum != NULL) {
            *buildNum = header.version & 0xFFFF;
        }
    }

    return TRUE;
}

void _rwStreamWriteVersionedChunkHeader(RwStream* stream, RwUInt32 type, RwUInt32 length, RwUInt32 version, RwUInt32 buildNum) {
    UnkRwChunkHeader header;
    header.type = type;
    header.length = length;
    header.version = ((version - 0x30000) << 14) & 0xFFC00000 | (version & 0x3F) << 16 | buildNum & 0xFFFF;

    RwMemLittleEndian32((RwUInt32*)&header, sizeof(header));
    RwStreamWrite(stream, &header, sizeof(header));
}

RwBool RwStreamFindChunk(RwStream* stream, RwUInt32 type, RwUInt32* length, RwUInt32* version) {
    RwUInt32 chunkType;
    RwUInt32 chunkLength;
    RwUInt32 chunkVersion;

    while (_rwStreamReadChunkHeader(stream, &chunkType, &chunkLength, &chunkVersion, NULL)) {
        if (chunkType == type) {
            if (chunkVersion < 0x31000) { // 3.1.0.0
                RwThrowError(1, E_RW_BADVERSION);
                return FALSE;
            }
            if (chunkVersion > 0x34003) { // 3.4.0.3
                RwThrowError(1, E_RW_BADVERSION);
                return FALSE;
            }

            if (length != NULL) {
                *length = chunkLength;
            }

            if (version != NULL) {
                *version = chunkVersion;
            }

            return TRUE;
        }

        if (!RwStreamSkip(stream, chunkLength)) {
            return FALSE;
        }
    }

    return FALSE;
}

// Equivalent: loop unrolling (does 2 iterations per optimized iteration, but we need it to do 4)
RwUInt32* RwMemLittleEndian32(RwUInt32* data, RwUInt32 size) {
    RwUInt32* iter = data;

    size >>= 2;
    while (size != 0) {
        *iter = ((*iter & 0x000000FF) << 24) | ((*iter & 0x0000FF00) << 8) | ((*iter & 0x00FF0000) >> 8) | ((*iter & 0xFF000000) >> 24);
        iter++;
        size--;
    }

    return data;
}

RwUInt16* RwMemLittleEndian16(RwUInt16* data, RwUInt32 size) {
    RwUInt16* iter = data;

    size >>= 1;
    while (size != 0) {
        *iter = (*iter << 8) | (*iter >> 8);
        iter++;
        size--;
    }

    return data;
}

// Equivalent
// Just RwMemLittleEndian32?
RwUInt32* RwMemNative32(RwUInt32* data, RwUInt32 size) {
    RwUInt32* iter = data;

    size >>= 2;
    while (size != 0) {
        *iter = ((*iter & 0x000000FF) << 24) | ((*iter & 0x0000FF00) << 8) | ((*iter & 0x00FF0000) >> 8) | ((*iter & 0xFF000000) >> 24);
        iter++;
        size--;
    }

    return data;
}

// Just RwMemLittleEndian16?
RwUInt16* RwMemNative16(RwUInt16* data, RwUInt32 size) {
    RwUInt16* iter = data;

    size >>= 1;
    while (size != 0) {
        *iter = (*iter << 8) | (*iter >> 8);
        iter++;
        size--;
    }

    return data;
}

// Equivalent
RwStream* RwStreamWriteInt32(RwStream* stream, RwUInt32* data, RwUInt32 size) {
    RwUInt32 buf[64];
    RwUInt32 n;

    while (size > 0) {
        n = 64 * sizeof(RwUInt32);
        if (size < 64 * sizeof(RwUInt32)) {
            n = size;
        }

        memcpy(buf, data, n);
        RwMemNative32(buf, n);

        if (!RwStreamWrite(stream, buf, n)) {
            return NULL;
        }

        size -= n;
        data = (RwUInt32*)((int)data + n);
    }
    return stream;
}

RwStream* RwStreamWriteInt16(RwStream* stream, RwUInt16* data, RwUInt32 size) {
    RwUInt16 buf[128];
    RwUInt32 n;

    while (size > 0) {
        n = 128 * sizeof(RwUInt16);
        if (size < 128 * sizeof(RwUInt16)) {
            n = size;
        }

        memcpy(buf, data, n);
        RwMemNative16(buf, n);

        if (!RwStreamWrite(stream, buf, n)) {
            return NULL;
        }

        size -= n;
        data = (RwUInt16*)((int)data + n);
    }
    return stream;
}

RwStream* RwStreamReadInt16(RwStream* stream, RwUInt16* data, RwUInt32 size) {
    if (!RwStreamRead(stream, data, size)) {
        RwThrowError(1, E_RW_READ);
        return NULL;
    }

    RwMemNative16(data, size);
    return stream;
}

// Equivalent
RwStream* RwStreamReadInt32(RwStream* stream, RwUInt32* data, RwUInt32 size) {
    if (!RwStreamRead(stream, data, size)) {
        RwThrowError(1, E_RW_READ);
        return NULL;
    }

    RwMemNative32(data, size);
    return stream;
}

RwStream* RwStreamReadChunkHeaderInfo(RwStream* stream, RwChunkHeaderInfo* info) {
    RwUInt32 type;
    RwUInt32 length;
    RwUInt32 version;
    RwUInt32 buildNum;

    if (!_rwStreamReadChunkHeader(stream, &type, &length, &version, &buildNum)) {
        return NULL;
    }

    info->type = type;
    info->length = length;
    info->version = version;
    info->buildNum = buildNum;
    info->isComplex = ChunkIsComplex(info);

    return stream;
}
