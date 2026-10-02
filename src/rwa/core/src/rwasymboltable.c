#include <rwa/core/rwasymboltable.h>
#include <stddef.h>

RwaSymbolTable* _rwaSymbolTableInit(RwaSymbolTable* table) {
    table->noParamTypes = 0;
    table->paramTypeList.link.next = &table->paramTypeList.link;
    table->paramTypeList.link.prev = &table->paramTypeList.link;
    return table;
}

RwaSymbolTable* _rwaSymbolTableDestroy(RwaSymbolTable* table) {
    return _rwaSymbolTableInit(table);
}

RwaSymbolTable* _rwaSymbolTableAddParamType(RwaSymbolTable* table, RwaParamType* type) {
    type->link.prev = NULL;
    type->link.next = NULL;
    type->link.next = table->paramTypeList.link.next;
    type->link.prev = &table->paramTypeList.link;

    table->paramTypeList.link.next->prev = &type->link;
    table->paramTypeList.link.next = &type->link;
    table->noParamTypes++;

    return table;
}

RwaSymbolTable* _rwaSymbolTableRemoveParamType(RwaSymbolTable* table, RwaParamType* type) {
    RwLLLink* current = table->paramTypeList.link.next;
    RwLLLink* end = &table->paramTypeList.link;
    for (; current != end; current = current->next) {
        RwaParamType* linkType = (RwaParamType*)((RwInt32)current - offsetof(RwaParamType, link));
        if (linkType == type) {
            break;
        }
    }

    table->noParamTypes--;
    type->link.prev->next = type->link.next;
    type->link.next->prev = type->link.prev;

    return table;
}

RwaParamType* _rwaSymbolTableFindParamTypeByUUID(RwaSymbolTable* table, RwaUUID* uuid) {
    RwLLLink* current = table->paramTypeList.link.next;
    RwLLLink* end = &table->paramTypeList.link;
    RwaParamType* type = (RwaParamType*)((RwInt32)current - offsetof(RwaParamType, link));
    while (RwaUUIDCompare(type->uniqueID.uuid.uuid, uuid) != 0) {
        current = current->next;
        if (current == end) {
            return NULL;
        }

        type = (RwaParamType*)((RwInt32)(current) - offsetof(RwaParamType, link));
    }
    return type;
}

RwaParamType* _rwaSymbolTableFindParamTypeFirstDepend(RwaSymbolTable* table, RwaParamType* type) {
    RwLLLink* current = table->paramTypeList.link.next;
    RwLLLink* end = &table->paramTypeList.link;
    RwaParamType* linkType = (RwaParamType*)((RwInt32)current - offsetof(RwaParamType, link));
    while (linkType->paramType != type) {
        current = current->next;
        if (current == end) {
            return NULL;
        }

        linkType = (RwaParamType*)((RwInt32)(current) - offsetof(RwaParamType, link));
    }
    return linkType;
}

RwaParamType* _rwaSymbolTableFindParamTypeByIndex(RwaSymbolTable* table, RwInt32 index) {
    RwInt32 i;
    RwLLLink* current;
    RwLLLink* end;
    RwaParamType* linkType;

    if (index > table->noParamTypes || table->noParamTypes == 0) {
        return NULL;
    }

    current = table->paramTypeList.link.next;
    end = &table->paramTypeList.link;
    linkType = (RwaParamType*)((RwInt32)current - offsetof(RwaParamType, link));
    for (i = 0; i < index; i++) {
        current = current->next;
        if (current == end) {
            return NULL;
        }

        linkType = (RwaParamType*)((RwInt32)(current) - offsetof(RwaParamType, link));
    }
    return linkType;
}
