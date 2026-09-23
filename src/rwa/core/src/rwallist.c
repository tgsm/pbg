#include <rwa/core/rwafreelist.h>
#include <rwa/core/rwallist.h>

RwaFreeList _rwaGfreeListLLNodes;
static RwBool _linkListOpen = FALSE;

RwBool _rwaLListModuleOpen(void) {
    if (RwaFreeListCreate(sizeof(RwaLLNode), 32, 0, 0, &_rwaGfreeListLLNodes) != NULL) {
        _linkListOpen = TRUE;
        return TRUE;
    } else {
        return FALSE;
    }
}

void _rwaLListModuleClose(void) {
    RwaFreeListDestroy(&_rwaGfreeListLLNodes);
    _linkListOpen = FALSE;
}

RwaLLNode* RwaLListAddData(RwaLLNode* list, void* data) {
    RwaLLNode* node = RwaFreeListAlloc(&_rwaGfreeListLLNodes);
    if (node == NULL) {
        return NULL;
    }

    node->data = data;
    node->prev = list;
    node->next = (list->next == list) ? list : list->next;
    list->next->prev = node;
    list->next = node;

    return node;
}

RwaLLNode* RwaLListRemoveData(RwaLLNode* list, void* data) {
    RwaLLNode* current;
    for (current = list->next; data != current->data; current = current->next) {
        if (current == list) {
            return NULL;
        }
    }

    current->prev->next = current->next;
    current->next->prev = current->prev;
    RwaFreeListFree(&_rwaGfreeListLLNodes, current);
    return list;
}

RwInt32 RwaLListFindDataIndex(RwaLLNode* list, void* data) {
    RwInt32 index = 0;
    RwaLLNode* current;
    for (current = list->next; current != list; current = current->next, index++) {
        if (current->data == data) {
            return index;
        }
    }
    return -1;
}

void RwaLListEmpty(RwaLLNode* list) {
    RwaLLNode* current, *temp;
    for (current = list->next, temp = list; current != temp; current = list->next, temp = list) {
        current->prev->next = current->next;
        current->next->prev = current->prev;
        RwaFreeListFree(&_rwaGfreeListLLNodes, current);
    }
}
