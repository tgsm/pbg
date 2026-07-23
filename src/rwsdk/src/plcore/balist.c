#include <rwsdk/badevice.h>
#include <rwsdk/plcore/baerr.h>
#include <rwsdk/plcore/balist.h>

RwSList* _rwSListCreate(RwInt32 entrySize) {
    RwSList* list = RwMalloc(sizeof(RwSList));
    if (list == NULL) {
        RwThrowErrorParams(1, E_RW_NOMEM, sizeof(RwSList));
        return NULL;
    }

    list->numElementsFilled = 0;
    list->numElementsAlloced = 20;
    list->entrySize = entrySize;
    list->listElements = RwMalloc(list->numElementsAlloced * list->entrySize);
    if (list->listElements == NULL) {
        RwThrowErrorParams(1, E_RW_NOMEM, list->numElementsAlloced * list->entrySize);
        RwFree(list);
        return NULL;
    }

    return list;
}

// NOTE: This function empties the list. It *does not* check if the list is empty.
// Maybe returns an RwSList?
void _rwSListEmpty(RwSList* list) {
    list->numElementsFilled = 0;
}

RwUInt8* _rwSListGetNewEntry(RwSList* list) {
    RwUInt8* entry;
    if (list->numElementsFilled >= list->numElementsAlloced) {
        RwInt32 numAlloced;
        RwUInt8* newListPtr = RwRealloc(list->listElements, list->entrySize * (list->numElementsAlloced + (list->numElementsAlloced / 4)));
        if (newListPtr == NULL) {
            RwThrowErrorParams(1, E_RW_NOMEM, list->entrySize * (list->numElementsAlloced + (list->numElementsAlloced / 4)));
            return NULL;
        }

        list->listElements = newListPtr;
        numAlloced = list->numElementsAlloced;
        list->numElementsAlloced = numAlloced + (numAlloced / 4);
    }

    entry = &list->listElements[list->numElementsFilled * list->entrySize];
    list->numElementsFilled++;
    return entry;
}

RwUInt8* _rwSListGetNewEntries(RwSList* list, int numEntries) {
    RwUInt8* entries;
    if (list->numElementsFilled + numEntries >= list->numElementsAlloced) {
        RwInt32 numAlloced;
        RwUInt8* newListPtr = RwRealloc(list->listElements, list->entrySize * (list->numElementsAlloced + numEntries + (list->numElementsAlloced / 4)));
        if (newListPtr == NULL) {
            RwThrowErrorParams(1, E_RW_NOMEM, list->entrySize * (list->numElementsAlloced + numEntries + (list->numElementsAlloced / 4)));
            return NULL;
        }

        list->listElements = newListPtr;
        numAlloced = list->numElementsAlloced;
        list->numElementsAlloced = numAlloced + numEntries + (numAlloced / 4);
    }

    entries = &list->listElements[list->numElementsFilled * list->entrySize];
    list->numElementsFilled += numEntries;
    return entries;
}

RwUInt8* _rwSListGetArray(RwSList* list) {
    if (list->numElementsFilled != 0) {
        return list->listElements;
    }
    return NULL;
}

RwBool _rwSListDestroy(RwSList* list) {
    if (list->listElements != NULL && list->numElementsAlloced != 0) {
        RwFree(list->listElements);
        list->listElements = NULL;
        list->numElementsAlloced = 0;
    }
    RwFree(list);

    return TRUE;
}

RwInt32 _rwSListGetNumEntries(RwSList* list) {
    return list->numElementsFilled;
}

RwUInt8* _rwSListToArray(RwSList* list) {
    RwUInt8* array = NULL;
    if (list->numElementsFilled != 0) {
        array = list->listElements;
    } else if (list->listElements != NULL) {
        RwFree(list->listElements);
        list->listElements = NULL;
        array = NULL;
    }

    RwFree(list);
    return array;
}

RwUInt8* _rwSListGetEntry(RwSList* list, int entryNum) {
    return &list->listElements[list->entrySize * entryNum];
}
