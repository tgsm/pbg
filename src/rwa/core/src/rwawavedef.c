#include "dont_inline_hack.h"
#include <rwa/core/rwamemory.h>
#include <rwa/core/rwawavedef.h>
#include <stddef.h>
#include <string.h>

static RwaFreeList _waveDefFreeList;
static RwBool _moduleOpen = FALSE;

const RwaUUID rwaWAVEFORMAT_VAG = {
    0xD9EA9798, 0xBBBC, 0x447B, { 0x96, 0xB2, 0x65, 0x47, 0x59, 0x10, 0x2E, 0x16 }
};

const RwaUUID rwaWAVEFORMAT_PCM = {
    0xD01BD217, 0x3587, 0x4EED, { 0xB9, 0xD9, 0xB8, 0xE8, 0x6E, 0xA9, 0xB9, 0x95 }
};

const RwaUUID rwaWAVEFORMAT_FLOAT = {
    0xDA1E4382, 0x2C99, 0x4C61, { 0xAD, 0x99, 0x7F, 0x36, 0x4B, 0x21, 0x15, 0x37 }
};

const RwaUUID rwaWAVEFORMAT_GCNADPCM = {
    0xF86215B0, 0x31D5, 0x4C29, { 0xBD, 0x37, 0xCD, 0xBF, 0x9B, 0xD1, 0x0C, 0x53 }
};

const RwaUUID rwaWAVEFORMAT_XADPCM = {
    0x632FA22B, 0x11DD, 0x458F, { 0xAA, 0x27, 0xA5, 0xC3, 0x46, 0xE9, 0x79, 0x0E }
};

const RwaUUID rwaWAVEFORMAT_WMA = {
    0x3F1D8147, 0xB7C4, 0x41E6, { 0xA6, 0x9B, 0x3C, 0xC0, 0x02, 0x5B, 0x33, 0xC7 }
};

const RwaUUID rwaWAVEFORMAT_MP3 = {
    0xBACFB36E, 0x529D, 0x4692, { 0xBF, 0x53, 0x32, 0x42, 0x56, 0xB0, 0x73, 0x4F }
};

const RwaUUID rwaWAVEFORMAT_MP2 = {
    0x34D09A54, 0x57D3, 0x409E, { 0xA6, 0xAD, 0x2B, 0xC8, 0x45, 0xAE, 0xC3, 0x39 }
};

const RwaUUID rwaWAVEFORMAT_MPG = {
    0x04C15BA7, 0xF907, 0x40AB, { 0xA4, 0x9F, 0xEE, 0xFE, 0xF8, 0xC4, 0xD2, 0x96 }
};

const RwaUUID rwaWAVEFORMAT_AC3 = {
    0xA30DB390, 0x58A9, 0x43C4, { 0xB9, 0xD2, 0x55, 0xD8, 0x4D, 0x3A, 0xE7, 0x54 }
};

const RwaUUID* _rwaGwaveFormats[] = {
    &rwaWAVEFORMAT_VAG,
    &rwaWAVEFORMAT_PCM,
    &rwaWAVEFORMAT_FLOAT,
    &rwaWAVEFORMAT_GCNADPCM,
    &rwaWAVEFORMAT_XADPCM,
    &rwaWAVEFORMAT_AC3,
    &rwaWAVEFORMAT_WMA,
    &rwaWAVEFORMAT_MP3,
};

RwBool _rwaWaveDefOpenModule(void) {
    _moduleOpen = TRUE;
    if (RwaFreeListCreate(sizeof(RwaWaveDef) + sizeof(RwaUUID), 8, 16, 0, &_waveDefFreeList)) {
        return TRUE;
    } else {
        _moduleOpen = FALSE;
        return FALSE;
    }
}

static RwBool _RwaWaveDefDestroyCallback(RwaObjDef*, RwaObjDef* objDef, void*);
void _rwaWaveDefCloseModule(void) {
    _rwaObjDefEnum(NULL, NULL, _RwaWaveDefDestroyCallback, NULL, TRUE);
    RwaFreeListDestroy(&_waveDefFreeList);
    _moduleOpen = FALSE;
}

// Unknown param types
static RwBool _RwaWaveDefDestroyCallback(RwaObjDef*, RwaObjDef* objDef, void*) {
    RwLLLink* temp;

    RwLLLink* link = objDef->waveDefList.link.next;
    RwLLLink* end = &objDef->waveDefList.link;
    while (link != end) {
        temp = link->next;
        RwaWaveDefDestroy((RwaWaveDef*)((int)link - offsetof(RwaWaveDef, link)));
        link = temp;
    }
    return FALSE;
}

RwBool _rwaWaveDefRegister(RwaWaveDefRegisterFunc* registerFuncs, RwUInt32 noRegisterFuncs) {
    RwInt32 i;
    for (i = 0; i < noRegisterFuncs; i++) {
        if (registerFuncs[i]() == NULL) {
            return FALSE;
        }
    }
    return TRUE;
}

RwBool RwaWaveFormatCompare(RwaWaveFormat* a, RwaWaveFormat* b) {
    if (RwaUUIDCompare(a->dataType, b->dataType) == 0 &&
        a->sampleRate == b->sampleRate &&
        a->bitDepth == b->bitDepth &&
        a->noChannels == b->noChannels &&
        (a->flags & ~(1 << 1)) == (b->flags & ~(1 << 1))) {
        return TRUE;
    } else {
        return FALSE;
    }
}

RwUInt32 RwaWaveFormatGetFrameSize(RwaWaveFormat* format) {
    DONT_INLINE_HACK();
    if (RwaUUIDCompare(format->dataType, &rwaWAVEFORMAT_PCM) == 0) {
        return format->bitDepth >> 3;
    } else if (RwaUUIDCompare(format->dataType, &rwaWAVEFORMAT_VAG) == 0) {
        return 16;
    } else if (RwaUUIDCompare(format->dataType, &rwaWAVEFORMAT_FLOAT) == 0) {
        return 4;
    } else if (RwaUUIDCompare(format->dataType, &rwaWAVEFORMAT_GCNADPCM) == 0) {
        return 8;
    } else if (RwaUUIDCompare(format->dataType, &rwaWAVEFORMAT_XADPCM) == 0) {
        return (format->noChannels == 1) ? 36 : 4;
    } else if (RwaUUIDCompare(format->dataType, &rwaWAVEFORMAT_MP3) == 0) {
        RwUInt32 unk, sampleRate;
        if (format->miscData != NULL) {
            unk = *(RwUInt32*)format->miscData;
        } else {
            unk = 128000;
        }
        sampleRate = (format->sampleRate != 0) ? format->sampleRate : 48000;
        return (unk * 144) / sampleRate;
    } else {
        return 0;
    }
}

void _rwaWaveFormatFreeMiscData(RwaWaveFormat* format) {
    if (!(format->flags & (1 << 1)) && format->miscData != NULL) {
        _rwaFree(format->miscData);
    }
    format->miscData = NULL;
    format->miscDataSize = 0;
}

RwaWaveDef* RwaWaveDefAssignID(RwaWaveDef* def, const RwaUUID* uuid, const char* name) {
    if (_rwaUniqueIDAssignUUID(&def->uniqueID, uuid) == NULL) {
        return NULL;
    }

    if (_rwaUniqueIDAssignName(&def->uniqueID, name) == NULL) {
        return NULL;
    }

    return def;
}

extern RwaObjDef* _rwaGnullObjDef;

RwaWaveDef* RwaWaveDefCreate(RwaObjDef* objDef, RwaWaveDef* def) {
    if (def == NULL) {
        def = RwaFreeListAlloc(&_waveDefFreeList);
        if (def == NULL) {
            return NULL;
        }
        def->flags = (1 << 1);
    } else {
        def->flags = 0;
    }

    _rwaUniqueIDInitialize(&def->uniqueID);
    def->createFunction = NULL;
    def->destroyFunction = NULL;
    def->uploadFunction = NULL;
    def->uploadBusyFunction = NULL;
    def->downloadFunction = NULL;
    def->unregisterCallback = NULL;
    def->stateSize = 0;
    def->stateAlign = 16;
    def->freeList = NULL;
    RwaWaveDefAssignSupportedFormats(def, NULL, 0);
    def->link.prev = NULL;
    def->link.next = NULL;
    if (objDef == NULL) {
        objDef = _rwaGnullObjDef;
    }
    def->objDef = objDef;
    def->link.next = objDef->waveDefList.link.next;
    def->link.prev = &objDef->waveDefList.link;
    objDef->waveDefList.link.next->prev = &def->link;
    objDef->waveDefList.link.next = &def->link;

    return def;
}

extern RwaWaveDef* RwaWaveDictUsingAllWaveDef(RwaWaveDef*, RwInt32);

RwBool RwaWaveDefDestroy(RwaWaveDef* def) {
    if (RwaWaveDictUsingAllWaveDef(def, 1) != NULL) {
        return FALSE;
    }

    if (def->unregisterCallback != NULL) {
        def->unregisterCallback(def);
    }
    if (def->freeList != NULL) {
        RwaFreeListDestroy(def->freeList);
    }
    if (def->flags & (1 << 0)) {
        _rwaFree(def->supportedFormats.formats);
    }
    def->link.prev->next = def->link.next;
    def->link.next->prev = def->link.prev;
    _rwaUniqueIDFreeData(&def->uniqueID);
    if (def->flags & (1 << 1)) {
        RwaFreeListFree(&_waveDefFreeList, def);
    }

    return TRUE;
}

void _rwaWaveDefFormatCopyToWaveFormat(RwaWaveDefFormat* defFormat, RwaWaveFormat* format, RwBool initialize) {
    if (initialize) {
        format->sampleRate = 0;
        format->dataType = NULL;
        format->length = 0;
        format->bitDepth = 0;
        format->noChannels = 0;
        format->flags = 0;
        format->reserved = 0;
        format->miscData = NULL;
        format->miscDataSize = 0;
    }

    format->bitDepth = defFormat->bitDepth;
    format->dataType = defFormat->dataType;
    format->noChannels = defFormat->noChannels;
    format->flags = 0;
    if (defFormat->flags & (1 << 0)) {
        format->flags |= (1 << 0);
    }
    if (defFormat->flags & (1 << 2)) {
        format->flags |= (1 << 2);
    }
}

RwBool RwaWaveDefFormatWaveFormatCompare(RwaWaveDefFormat* defFormat, RwaWaveFormat* format) {
    return (format->noChannels == defFormat->noChannels &&
            format->bitDepth == defFormat->bitDepth &&
            format->sampleRate >= defFormat->minSampleRate &&
            format->sampleRate <= defFormat->maxSampleRate &&
            RwaUUIDCompare(format->dataType, defFormat->dataType) == 0 &&
            (format->flags & (1 << 0)) == (defFormat->flags & (1 << 0)));
}

void RwaWaveDefAssignSupportedFormats(RwaWaveDef* def, const RwaWaveDefFormat** supportedFormats, RwUInt32 noSupportedFormats) {
    if (def->flags & (1 << 0)) {
        if (def->supportedFormats.formats != NULL) {
            _rwaFree(def->supportedFormats.formats);
        }
        def->flags &= ~(1 << 0);
    }
    def->supportedFormats.copyFormats = supportedFormats;
    def->noSupportedFormats = noSupportedFormats;
    def->defaultFormatIndex = 0;
}

RwUInt32 _rwaWaveFormatGetSize(RwaWaveFormat* format) {
    RwUInt32 size = sizeof(RwaWaveFormat);
    if (format->dataType != NULL) {
        size += sizeof(RwaUUID);
    }
    if (format->miscData != NULL) {
        size += format->miscDataSize;
    }
    return size;
}

extern void RwaEndianCopy(void*, void*, RwInt32);

RwaWaveFormat* _rwaWaveFormatSerialize(RwaWaveFormat* a, RwaWaveFormat* b, RwBool a2, RwInt32 endianness) {
    if (a2) {
        void* dest = b;
        if (endianness != 0) {
            RwBool hasUUID = a->dataType ? TRUE : FALSE;
            RwaEndianCopy(&dest, &a->sampleRate, sizeof(a->sampleRate));
            RwaEndianCopy(&dest, &hasUUID, sizeof(hasUUID));
            RwaEndianCopy(&dest, &a->length, sizeof(a->length));
            RwaEndianCopy(&dest, &a->bitDepth, sizeof(a->bitDepth));
            RwaEndianCopy(&dest, &a->noChannels, sizeof(a->noChannels));
            // FIXME: What the heck is here that is size 2?
            memset(dest, 0, 2);
            dest = (void*)((int)dest + 2);
            RwaEndianCopy(&dest, &a->miscData, sizeof(a->miscData));
            RwaEndianCopy(&dest, &a->miscDataSize, sizeof(a->miscDataSize));
            RwaEndianCopy(&dest, &a->flags, sizeof(a->flags));
            RwaEndianCopy(&dest, &a->reserved, sizeof(a->reserved));
            memset(dest, 0, 2);
            dest = (void*)((int)dest + 2);
        } else {
            memcpy(dest, a, sizeof(RwaWaveFormat));
            dest = (void*)((int)dest + sizeof(RwaWaveFormat));
        }

        if (a->dataType != NULL) {
            _rwaUUIDSerialize(a->dataType, (RwaUUID*)dest, endianness);
            dest = (void*)((int)dest + sizeof(RwaUUID));
        }

        if (a->miscData != NULL) {
            memcpy(dest, a->miscData, a->miscDataSize);
            dest = (void*)((int)dest + a->miscDataSize);
            a->flags |= (1 << 1);
        }
    } else {
        void* dest;
        RwaUUID* uuid;
        if (endianness) {
            dest = a;
            RwaEndianCopy(&dest, &b->sampleRate, sizeof(b->sampleRate));
            RwaEndianCopy(&dest, &b->dataType, sizeof(RwaWaveFormat*));
            RwaEndianCopy(&dest, &b->length, sizeof(b->length));
            RwaEndianCopy(&dest, &b->bitDepth, sizeof(b->bitDepth));
            RwaEndianCopy(&dest, &b->noChannels, sizeof(b->noChannels));
            memset(dest, 0, 2);
            dest = (void*)((int)dest + 2);
            RwaEndianCopy(&dest, &b->miscData, sizeof(b->miscData));
            RwaEndianCopy(&dest, &b->miscDataSize, sizeof(b->miscDataSize));
            RwaEndianCopy(&dest, &b->flags, sizeof(b->flags));
            RwaEndianCopy(&dest, &b->reserved, sizeof(b->reserved));
            memset(dest, 0, 2);
            dest = (void*)((int)dest + 2);
            uuid = (RwaUUID*)(b + 1);
        } else {
            memcpy(a, b, sizeof(RwaWaveFormat));
            uuid = (RwaUUID*)(b + 1);
        }

        if (a->dataType != NULL) {
            _rwaUUIDSerialize(uuid, uuid, endianness);
            a->dataType = _rwaWaveFormatFindUUID(uuid);
            if (a->dataType == NULL) {
                return NULL;
            }
            uuid++;
        }

        if (a->miscData != NULL) {
            a->miscData = uuid;
        }
    }

    return a;
}

RwBool _rwaWaveDefIsTargetFormatSupported(RwaWaveDef* def, RwaWaveFormat* format) {
    RwUInt32 noSupportedFormats;
    RwBool supported;
    RwInt32 i;

    supported = FALSE;
    noSupportedFormats = def->noSupportedFormats;

    if (noSupportedFormats != 0) {
        RwaWaveDefFormat** formats = def->supportedFormats.formats;
        for (i = 0; i < noSupportedFormats; i++) {
            if (RwaWaveDefFormatWaveFormatCompare(formats[i], format)) {
                supported = TRUE;
                break;
            }
        }
    } else {
        supported = TRUE; // Shouldn't this be false? We don't support any formats
    }

    return supported;
}

const RwaUUID* _rwaWaveFormatFindUUID(RwaUUID* formatUUID) {
    RwInt32 i;
    for (i = 0; i < 8u; i++) {
        if (RwaUUIDCompare(_rwaGwaveFormats[i], formatUUID) == 0) {
            return _rwaGwaveFormats[i];
        }
    }

    return NULL;
}

RwInt32 RwaWaveFormatGetSamplesPerAudioFrame(RwaWaveFormat* format) {
    if (RwaUUIDCompare(format->dataType, &rwaWAVEFORMAT_PCM) == 0) {
        return 1;
    } else if (RwaUUIDCompare(format->dataType, &rwaWAVEFORMAT_VAG) == 0) {
        return 28;
    } else if (RwaUUIDCompare(format->dataType, &rwaWAVEFORMAT_FLOAT) == 0) {
        return 1;
    } else if (RwaUUIDCompare(format->dataType, &rwaWAVEFORMAT_AC3) == 0) {
        return 1536;
    } else if (RwaUUIDCompare(format->dataType, &rwaWAVEFORMAT_GCNADPCM) == 0) {
        return 14;
    } else if (RwaUUIDCompare(format->dataType, &rwaWAVEFORMAT_XADPCM) == 0) {
        RwUInt32 samples;
        RwaWaveFormat temp;
        if (format->noChannels == 1) {
            return 64;
        }

        memcpy(&temp, format, sizeof(RwaWaveFormat));
        temp.noChannels = 1;
        samples = RwaWaveFormatGetSamplesPerAudioFrame(&temp) / (RwaWaveFormatGetFrameSize(&temp) / RwaWaveFormatGetFrameSize(format));
        return samples;
    } else if (RwaUUIDCompare(format->dataType, &rwaWAVEFORMAT_MP3) == 0 || RwaUUIDCompare(format->dataType, &rwaWAVEFORMAT_MP2) == 0) {
        return 1152;
    } else if (RwaUUIDCompare(format->dataType, &rwaWAVEFORMAT_MPG) != 0) {
        return 384;
    } else {
        return 0;
    }
}
