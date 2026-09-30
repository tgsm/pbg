#include <rwa/core/rwafreelist.h>
#include <rwa/core/rwamemory.h>
#include <rwa/core/rwawavedict.h>
#include <stddef.h>

static RwaFreeList _waveDictFreeList;

const RwaUUID rwaWAVEDICT_WORK = {
    0x81271D58, 0xC9D7, 0x43A9, { 0x91, 0x17, 0x75, 0x87, 0x57, 0xC3, 0x1E, 0x04 }
};

static RwBool _moduleOpen = FALSE;
static RwBool _moduleClosing = FALSE;
static RwaWaveDict* _workingDict = NULL;
static RwaWaveDict* _currentDict = NULL;
static RwLinkList _waveDictList;

static void _RwaWaveDictFree(RwaWaveDict* dict);

RwBool _rwaWaveDictOpenModule(void) {
    static RwaWaveDict _staticWorkingDict;

    _waveDictList.link.next = &_waveDictList.link;
    _waveDictList.link.prev = &_waveDictList.link;
    _moduleOpen = TRUE;

    if (RwaFreeListCreate(sizeof(RwaWaveDict), 8, 16, 0, &_waveDictFreeList) != NULL) {
        _workingDict = _currentDict = _rwaWaveDictCreate(&_staticWorkingDict);
        if (_workingDict != NULL) {
            RwaWaveDictAssignID(_workingDict, &rwaWAVEDICT_WORK, "");
            return TRUE;
        }

        RwaFreeListDestroy(&_waveDictFreeList);
    }

    _workingDict = NULL;
    _currentDict = NULL;
    _moduleOpen = FALSE;

    return FALSE;
}

void _rwaWaveDictCloseModule(void) {
    RwLLLink* current, *end;
    _moduleClosing = TRUE;

    current = _waveDictList.link.next;
    end = &_waveDictList.link;
    while (current != end) {
        RwaWaveDictDestroy((RwaWaveDict*)((RwInt32)current - offsetof(RwaWaveDict, link)));

        current = _waveDictList.link.next;
        end = &_waveDictList.link;
    }

    RwaFreeListDestroy(&_waveDictFreeList);

    _waveDictList.link.next = &_waveDictList.link;
    _waveDictList.link.prev = &_waveDictList.link;
    _workingDict = NULL;
    _currentDict = NULL;
    _moduleOpen = FALSE;

    _moduleClosing = FALSE;
}

RwaWaveDict* _rwaWaveDictCreate(RwaWaveDict* dict) {
    if (dict != NULL) {
        dict->flags = rwaWAVEDICTFLAGNOTOWNED;
    } else {
        dict = RwaFreeListAlloc(&_waveDictFreeList);
        if (dict == NULL) {
            return NULL;
        }
        dict->flags = rwaWAVEDICTFLAGUSEFREELIST;
    }

    _rwaUniqueIDInitialize(&dict->uniqueID);
    dict->link.prev = NULL;
    dict->link.next = NULL;
    dict->link.next = _waveDictList.link.next;
    dict->link.prev = &_waveDictList.link;
    _waveDictList.link.next->prev = &dict->link;
    _waveDictList.link.next = &dict->link;

    dict->waveListHead.prev = &dict->waveListHead;
    dict->waveListHead.next = &dict->waveListHead;
    dict->waveListHead.data = NULL;

    return dict;
}

void RwaWaveDictDestroy(RwaWaveDict* dict) {
    RwaWave* wave;
    RwLLLink* link;
    RwLLLink* end;
    RwaLLNode* node;
    RwaLLNode* endNode;
    RwBool foundDict;

    dict->link.prev->next = dict->link.next;
    dict->link.next->prev = dict->link.prev;

    node = dict->waveListHead.next;
    endNode = &dict->waveListHead;
    for (; node != endNode; node = node->next) {
        foundDict = FALSE;
        wave = node->data;
        link = _waveDictList.link.next;
        end = &_waveDictList.link;

        if (link == end) {
            RwaWaveDestroy(wave);
        }

        for (; link != end; link = link->next) {
            if (RwaWaveDictContainsWave((RwaWaveDict*)((RwInt32)link - offsetof(RwaWaveDict, link)), wave)) {
                foundDict = TRUE;
                break;
            }
        }

        if (!foundDict) {
            RwaWaveDestroy(wave);
        }
    }

    RwaLListEmpty(&dict->waveListHead);
    if (dict == _currentDict) {
        _currentDict = _workingDict;
    }
    _RwaWaveDictFree(dict);
}

RwaWaveDict* RwaWaveDictAssignID(RwaWaveDict* dict, const RwaUUID* uuid, const RwChar* name) {
    _rwaUniqueIDAssignUUID(&dict->uniqueID, uuid);
    _rwaUniqueIDAssignName(&dict->uniqueID, name);
    return dict;
}

RwaWaveDict* RwaWaveDictContainsWave(RwaWaveDict* dict, RwaWave* wave) {
    if (RwaLListFindDataIndex(&dict->waveListHead, wave) == -1) {
        return NULL;
    }
    return dict;
}

// Iterates through each wave in each wave dictionary. If there is a wave that uses `def`,
// `def` will be returned. Otherwise, this function will return NULL.
RwaWaveDef* RwaWaveDictUsingAllWaveDef(RwaWaveDef* def, RwBool) {
    RwLLLink* current = _waveDictList.link.next;
    RwLLLink* end = &_waveDictList.link;
    for (; current != end; current = current->next) {
        RwaWaveDict* dict = (RwaWaveDict*)((RwInt32)current - offsetof(RwaWaveDict, link));
        RwaLLNode* node = dict->waveListHead.next;
        RwaLLNode* endNode = &dict->waveListHead;
        for (; node != endNode; node = node->next) {
            RwaWave* wave = (RwaWave*)(node->data);
            if (wave->waveDef == def) {
                return def;
            }
        }
    }

    return NULL;
}

RwaWaveDict* RwaWaveDictAddWave(RwaWaveDict* dict, RwaWave* wave) {
    if (RwaWaveDictContainsWave(dict, wave) != NULL) {
        return dict;
    }

    if (RwaLListAddData(&dict->waveListHead, wave) == NULL) {
        return NULL;
    }

    return dict;
}

RwaWaveDict* RwaWaveDictGetCurrent(void) {
    return _currentDict;
}

void _rwaWaveDictRemoveAllWave(RwaWaveDict* dict) {
    RwLLLink* current = _waveDictList.link.next;
    RwLLLink* end = &_waveDictList.link;
    for (; current != end; current = current->next) {
        RwaWaveDict* nodeDict = (RwaWaveDict*)((RwInt32)current - offsetof(RwaWaveDict, link));
        RwaLListRemoveData(&nodeDict->waveListHead, dict);
    }
}

static void _RwaWaveDictFree(RwaWaveDict* dict) {
    _rwaUniqueIDFreeData(&dict->uniqueID);
    if (dict->flags & rwaWAVEDICTFLAGUSEFREELIST) {
        RwaFreeListFree(&_waveDictFreeList, dict);
    } else if (!(dict->flags & rwaWAVEDICTFLAGNOTOWNED)) {
        _rwaFree(dict);
    }
}
