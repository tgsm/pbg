#include "engine/Allocator.h"

// This file is basically OSAlloc from the Dolphin SDK, but with some modifications and some new functions.

struct Cell {
    Cell* prev;
    Cell* next;
    s32 size;
};

struct HeapDesc {
    s32 size;
    Cell* free;
    Cell* allocated;
};

static HeapDesc* HeapArray;
static int NumHeaps;
static void* ArenaStart;
static void* ArenaEnd;
volatile OSHeapHandle __OSCurrHeap = -1;

static BOOL bUseAutoHeap;
static u32 nQualificationSize;
static u32 nNumberOfVerySmallEntries;
static u32 nNumberOfVerySmallEntriesFree;
static u32 nMinimumNumberOfVerySmallEntriesFree;
static u8* pVerySmallBufferMemory;
static u8* pVerySmallBufferMemoryHandler;
static u32 nFirstAvailableVerySmallBlock;

void (*__OSAllocConfigurator)(int*);

static u8* OSGetVerySmallEntry() {
    if (nNumberOfVerySmallEntriesFree == 0) {
        return 0;
    }

    u8* entry;
    if (nFirstAvailableVerySmallBlock < nNumberOfVerySmallEntries && pVerySmallBufferMemoryHandler[nFirstAvailableVerySmallBlock] == 0) {
        entry = &pVerySmallBufferMemory[nFirstAvailableVerySmallBlock * 32];
        pVerySmallBufferMemoryHandler[nFirstAvailableVerySmallBlock] = 1;
        BOOL bVar1 = FALSE;
        for (u32 i = nFirstAvailableVerySmallBlock + 1; i < nNumberOfVerySmallEntries; i++) {
            if (pVerySmallBufferMemoryHandler[i] == 0) {
                bVar1 = TRUE;
                nFirstAvailableVerySmallBlock = i;
                break;
            }
        }

        if (!bVar1) {
            nFirstAvailableVerySmallBlock = nNumberOfVerySmallEntries;
        }
        nNumberOfVerySmallEntriesFree--;
        if (nNumberOfVerySmallEntriesFree < nMinimumNumberOfVerySmallEntriesFree) {
            nMinimumNumberOfVerySmallEntriesFree = nNumberOfVerySmallEntriesFree;
        }
    } else {
        for (u32 i = 0; i < nNumberOfVerySmallEntries; i++) {
            if (pVerySmallBufferMemoryHandler[i] == 0) {
                entry = &pVerySmallBufferMemory[i * 32];
                nNumberOfVerySmallEntriesFree--;
                if (nNumberOfVerySmallEntriesFree < nMinimumNumberOfVerySmallEntriesFree) {
                    nMinimumNumberOfVerySmallEntriesFree = nNumberOfVerySmallEntriesFree;
                }
                pVerySmallBufferMemoryHandler[i] = 1;
                return entry;
            }
        }

        return 0;
    }

    return entry;
}

static Cell* DLAddFront(Cell* list, Cell* cell) {
    cell->next = list;
    cell->prev = 0;
    if (list != NULL) {
        list->prev = cell;
    }
    return cell;
}

static Cell* DLLookup(Cell* list, Cell* cell) {
    for (; list != NULL; list = list->next) {
        if (list == cell) {
            return list;
        }
    }
    return 0;
}

static Cell* DLExtract(Cell* list, Cell* cell) {
    if (cell->next != NULL) {
        cell->next->prev = cell->prev;
    }
    if (cell->prev == NULL) {
        return cell->next;
    }
    cell->prev->next = cell->next;
    return list;
}

static Cell* DLInsert(Cell* list, Cell* cell) {
    Cell* prev;
    Cell* next;

    for (next = list, prev = 0; next != NULL; prev = next, next = next->next) {
        if (cell <= next) {
            break;
        }
    }

    cell->next = next;
    cell->prev = prev;

    if (next != NULL) {
        next->prev = cell;
        if ((u8*)cell + cell->size == (u8*)next) {
            cell->size += next->size;
            next = next->next;
            cell->next = next;
            if (next != NULL) {
                next->prev = cell;
            }
        }
    }

    if (prev != NULL) {
        prev->next = cell;
        if ((u8*)prev + prev->size == (u8*)cell) {
            prev->size += cell->size;
            prev->next = next;
            if (next != NULL) {
                next->prev = prev;
            }
        }
        return list;
    }

    return cell;
}

// Equivalent?
void* OSAllocFromHeap(int heap, u32 size) {
    Cell* cell;
    Cell* newCell;
    HeapDesc* hd;
    s32 leftoverSize;

    if (size == 0) {
        size = 1;
    }

    if (bUseAutoHeap == TRUE) {
        if (size <= 0x20) {
            void* smallEntry = OSGetVerySmallEntry();
            if (smallEntry != NULL) {
                return smallEntry;
            }
        }

        if (size <= nQualificationSize) {
            heap = 1;
            if (OSGetSizeOfLargestFreeBlock(heap) < (s32)((size + 0x3F) & ~0x1F)) {
                heap = 0;
            }
        } else {
            heap = 0;
            if (OSGetSizeOfLargestFreeBlock(heap) < (s32)((size + 0x3F) & ~0x1F)) {
                heap = 1;
            }
        }
    }

    hd = &HeapArray[heap];
    size += 0x20;
    size = (size + 0x1F) & ~0x1F;
    cell = 0;

    for (newCell = hd->free; newCell != NULL; newCell = newCell->next) {
        if (size <= newCell->size) {
            if (cell != NULL) {
                if ((u32)newCell->size < (u32)cell->size && ((u32)((u8*)newCell + 0x20) & 0x1F) == 0) {
                    cell = newCell;
                }
            } else {
                if (((u32)((u8*)newCell + 0x20) & 0x1F) == 0) {
                    cell = newCell;
                }
            }
        }
    }

    if (cell == NULL) {
        return NULL;
    }

    leftoverSize = cell->size - size;
    if (leftoverSize < 0x40U) {
        hd->free = DLExtract(hd->free, cell);
    } else {
        cell->size = size;
        newCell = (Cell*)((u8*)cell + size);
        newCell->size = leftoverSize;
        newCell->prev = cell->prev;
        newCell->next = cell->next;
        if (newCell->next != NULL) {
            newCell->next->prev = newCell;
        }
        if (newCell->prev != NULL) {
            newCell->prev->next = newCell;
        } else {
            hd->free = newCell;
        }
    }

    hd->allocated = DLAddFront(hd->allocated, cell);
    return (u8*)cell + 0x20;
}

// Incomplete
void* OSAllocFromHeapAligned(int heap, u32 size, u32 align) {
    Cell* cell;
    Cell* newCell;
    HeapDesc* hd;
    s32 leftoverSize;

    if (size == 0) {
        size = 1;
    }

    hd = &HeapArray[heap];

    if (align < 32) {
        align = 32;
    }
    if (align == 32) {
        return OSAllocFromHeap(heap, size);
    }

    size += 0x20;
    size = (size + 0x1F) & ~0x1F;
    BOOL bVar1 = FALSE;
    cell = 0;

    for (newCell = hd->free; newCell != NULL; newCell = newCell->next) {
        if (size <= newCell->size) {
            if (cell != NULL) {
                if ((u32)newCell->size < (u32)cell->size && newCell->size - (((u32)((u8*)newCell + 0x20) / align) * align) == 0) {
                    cell = newCell;
                }
            } else {
                if (newCell->size - (((u32)((u8*)newCell + 0x20) / align) * align) == 0) {
                    cell = newCell;
                }
            }
        }
    }

    if (cell == NULL) {
        cell = 0; // amazing
        size += 0x40;
        for (newCell = hd->free; newCell != NULL; newCell = newCell->next) {
            if (((size + align + 0x1F) & ~0x1F) <= newCell->size) {
                if (cell != NULL) {
                    if (newCell->size < cell->size && ((u32)((u8*)newCell + 0x20) & 0x1F) == 0) {
                        cell = newCell;
                    }
                } else {
                    if (((u32)((u8*)newCell + 0x20) & 0x1F) == 0) {
                        cell = newCell;
                    }
                }
            }
        }

        bVar1 = TRUE;
    }


    if (cell == NULL) {
        return NULL;
    }

    if (!bVar1) {
        leftoverSize = cell->size - size;
        if (leftoverSize < 0x40U) {
            hd->free = DLExtract(hd->free, cell);
        } else {
            cell->size = size;
            newCell = (Cell*)((u8*)cell + size);
            newCell->size = leftoverSize;
            newCell->prev = cell->prev;
            newCell->next = cell->next;
            if (newCell->next != NULL) {
                newCell->next->prev = newCell;
            }
            if (newCell->prev != NULL) {
                newCell->prev->next = newCell;
            } else {
                hd->free = newCell;
            }
        }
    } else {

    }

    hd->allocated = DLAddFront(hd->allocated, cell);
    return (u8*)cell + 0x20;
}

void OSFreeToHeap(int heap, void* ptr) {
    HeapDesc* hd;
    Cell* cell;

    if (ptr == NULL) {
        return;
    }

    BOOL bVar1;
    if (ptr == NULL) {
        bVar1 = FALSE;
    } else {
        void* begin = pVerySmallBufferMemory;
        void* end = (pVerySmallBufferMemory + (nNumberOfVerySmallEntries * 32));
        if (ptr >= begin && ptr < end) {
            bVar1 = TRUE;
        } else {
            bVar1 = FALSE;
        }
    }

    if (bVar1 == TRUE) {
        if (ptr != NULL) {
            u32 uVar2 = ((u32)ptr - (u32)pVerySmallBufferMemory) / 32;
            pVerySmallBufferMemoryHandler[uVar2] = 0;
            if (nFirstAvailableVerySmallBlock > uVar2) {
                nFirstAvailableVerySmallBlock = uVar2;
            }

            nNumberOfVerySmallEntriesFree++;
        }
        return;
    }

    cell = (Cell*)((u32)ptr - 0x20);
    if (bUseAutoHeap == TRUE) {
        if (DLLookup(HeapArray->allocated, cell) != NULL) {
            heap = 0;
        } else {
            heap = 1;
        }
    }

    hd = &HeapArray[heap];

    hd->allocated = DLExtract(hd->allocated, cell);
    hd->free = DLInsert(hd->free, cell);
}

OSHeapHandle OSSetCurrentHeap(OSHeapHandle heap) {
    OSHeapHandle oldHeap = __OSCurrHeap;
    __OSCurrHeap = heap;
    return oldHeap;
}

void* OSInitAlloc(void* arenaStart, void* arenaEnd, int maxHeaps) {
    u32 arraySize = maxHeaps * sizeof(HeapDesc);
    HeapArray = (HeapDesc*)arenaStart;
    NumHeaps = maxHeaps;

    for (int i = 0; i < NumHeaps; i++) {
        HeapDesc* hd = &HeapArray[i];
        hd->size = -1;
        hd->free = hd->allocated = 0;
    }

    __OSCurrHeap = -1;

    arenaStart = (void*)((u32)((char*)HeapArray + arraySize));
    arenaStart = (void*)(((u32)arenaStart + 0x1F) & ~0x1F);
    ArenaStart = arenaStart;
    ArenaEnd = (void*)((u32)arenaEnd & ~0x1F);
    return arenaStart;
}

int OSCreateHeap(void* start, void* end) {
    start = (void*)(((u32)start + 0x1FU) & ~0x1F);
    end = (void*)(((u32)end) & ~0x1F);

    for (int heap = 0; heap < NumHeaps; heap++) {
        HeapDesc* hd = &HeapArray[heap];
        if (hd->size < 0) {
            hd->size = (u32)end - (u32)start;

            Cell* cell = (Cell*)start;
            cell->prev = 0;
            cell->next = 0;
            cell->size = hd->size;
            hd->free = cell;
            hd->allocated = 0;

            return heap;
        }
    }

    return -1;
}

int OSGetSizeOfLargestFreeBlock(int heap) {
    u32 size = 0;
    if (heap > NumHeaps) {
        return 0;
    }

    if (heap == -1) {
        for (int heap_ = 0; heap_ < NumHeaps; heap_++) {
            size = OSGetSizeOfLargestFreeBlock(heap_);
        }
    } else {
        HeapDesc* hd = &HeapArray[heap];
        for (Cell* cell = hd->free; cell != NULL; cell = cell->next) {
            if (cell->size > size) {
                size = cell->size;
            }
        }
    }

    return size;
}

void OSSetConfigurator(void (*configurator)(int*), BOOL useAutoHeap) {
    __OSAllocConfigurator = configurator;
    bUseAutoHeap = useAutoHeap;
}

void OSSetQualificationSize(int size) {
    nQualificationSize = size;
}

void OSSetNumberOfVerySmallBuffers(int num) {
    nNumberOfVerySmallEntries = num;
    nNumberOfVerySmallEntriesFree = num;
    nMinimumNumberOfVerySmallEntriesFree = num;
}
