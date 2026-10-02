#include <rwa/core/rwafreelist.h>
#include <rwa/core/rwamemory.h>
#include <rwa/core/rwaobjdef.h>
#include <rwa/core/rwaobjinterface.h>
#include <string.h>

static RwaFreeList _objInterfaceDefFreeList;
static RwBool _moduleOpen = FALSE;
static RwLinkList _objInterfaceDefList;

RwBool _rwaObjInterfaceOpenModule(void) {
    _objInterfaceDefList.link.next = &_objInterfaceDefList.link;
    _objInterfaceDefList.link.prev = &_objInterfaceDefList.link;

    if (RwaFreeListCreate(sizeof(RwaObjInterfaceDef), 16, 16, 0, &_objInterfaceDefFreeList) != NULL) {
        _moduleOpen = TRUE;
        return TRUE;
    } else {
        return FALSE;
    }
}

void _rwaObjInterfaceCloseModule(void) {
    RwLLLink* current = _objInterfaceDefList.link.next;
    RwLLLink* end = &_objInterfaceDefList.link;
    while (current != end) {
        RwaObjInterfaceDefDestroy((RwaObjInterfaceDef*)((RwInt32)current - offsetof(RwaObjInterfaceDef, link)));

        current = _objInterfaceDefList.link.next;
        end = &_objInterfaceDefList.link;
    }

    _objInterfaceDefList.link.next = &_objInterfaceDefList.link;
    _objInterfaceDefList.link.prev = &_objInterfaceDefList.link;

    RwaFreeListDestroy(&_objInterfaceDefFreeList);
    _moduleOpen = FALSE;
}

RwBool _rwaObjInterfaceRegister(RwaObjInterfaceRegisterFunc* registerFuncs, RwInt32 numRegisterFuncs) {
    RwUInt32 i;
    for (i = 0; i < numRegisterFuncs; i++) {
        if (registerFuncs[i]() == NULL) {
            return FALSE;
        }
    }
    return TRUE;
}

RwaObjInterface* RwaObjInterfaceAttach(RwaObjInterfaceDef* def, RwaObjDef* objDef, UnkInterfaceFunc a2, void* a3) {
    RwaObjInterface* interface;
    RwaObjDefInterfaceInfo* interfaceInfo = objDef->interfaceInfo;

    RwUInt32 size;
    RwUInt16 j;
    RwUInt16 i;
    RwUInt16 noInputs;
    RwUInt16 noInputParams;
    RwUInt16 noOutputParams;
    RwUInt16 noOutputs;

    RwLLLink* current = interfaceInfo->interfaceList.link.next;
    RwLLLink* end = &interfaceInfo->interfaceList.link;
    for (; current != end; current = current->next) {
        interface = (RwaObjInterface*)((RwInt32)current - offsetof(RwaObjInterface, link));
        if (interface->interfaceDef == def) {
            interface->refCount++;
            return interface;
        }
    }

    if (_rwaObjInterfaceDefSupported(def, objDef, 1) == NULL) {
        return NULL;
    }

    size = sizeof(RwaObjInterface) + (def->noInputParams * sizeof(RwaInputParamMap)) + (def->noOutputParams * sizeof(RwaOutputParamMap));
    if (a2 != NULL) {
        interface = a2(size, a3);
        if (interface != NULL) {
            interface->flags = (1 << 0);
        }
    } else {
        interface = _rwaMalloc(size);
        if (interface != NULL) {
            interface->flags = 0;
        }
    }

    if (interface == NULL) {
        return NULL;
    }

    interface->inputParamMaps = (RwaInputParamMap*)(interface + 1);
    interface->outputParamMaps = (RwaOutputParamMap*)(interface->inputParamMaps + def->noInputParams);
    interface->interfaceDef = def;
    interface->refCount = 0;

    noInputParams = def->noInputParams;
    noInputs = objDef->interfaceInfo->noInputs;
    for (i = 0; i < noInputParams; i++) {
        for (j = 0; j < noInputs; j++) {
            if (RwaUUIDCompare(interfaceInfo->inputParams[j].uniqueID.uuid.uuid, def->inputParams[i].uniqueID.uuid.uuid) == 0) {
                interface->inputParamMaps[i].paramIndex = j;
                interface->inputParamMaps[i].setFunc = interfaceInfo->inputParams[j].setFunc;
            }
        }
    }

    noOutputParams = def->noOutputParams;
    noOutputs = objDef->interfaceInfo->noOutputs;
    for (i = 0; i < noOutputParams; i++) {
        for (j = 0; j < noOutputs; j++) {
            if (RwaUUIDCompare(interfaceInfo->outputParams[j].uniqueID.uuid.uuid, def->outputParams[i].uniqueID.uuid.uuid) == 0) {
                interface->outputParamMaps[i].paramIndex = j;
                interface->outputParamMaps[i].getFunc = interfaceInfo->outputParams[j].getFunc;
            }
        }
    }

    interface->link.next = interfaceInfo->interfaceList.link.next;
    interface->link.prev = &interfaceInfo->interfaceList.link;
    interfaceInfo->interfaceList.link.next->prev = &interface->link;
    interfaceInfo->interfaceList.link.next = &interface->link;

    return interface;
}

RwBool RwaObjInterfaceRemove(RwaObjInterface* interface) {
    if (interface->refCount != 0) {
        interface->refCount--;
        return FALSE;
    }

    interface->link.prev->next = interface->link.next;
    interface->link.next->prev = interface->link.prev;

    if (!(interface->flags & (1 << 0))) {
        _rwaFree(interface);
    }

    return TRUE;
}

// Equivalent: regalloc
// Need r3 to get copied to r23
// Can disable the macro below to get more matching behavior
RwaObjInterfaceDef* _rwaObjInterfaceDefSupported(RwaObjInterfaceDef* def, RwaObjDef* objDef, RwInt32) {
    RwaObjDefInterfaceInfo* interfaceInfo;
    RwUInt32 j;
    RwUInt32 i;
    RwUInt32 noInputs;
    RwUInt32 noInputParams;
    RwBool bVar3;
    RwBool bVar4;
    RwUInt32 noOutputParams;
    RwUInt32 noOutputs;

    interfaceInfo = objDef->interfaceInfo;
    bVar4 = TRUE;
    noInputParams = def->noInputParams;
    noInputs = interfaceInfo->noInputs;
    for (i = 0; i < noInputParams; i++) {
        bVar3 = FALSE;
        for (j = 0; j < noInputs; j++) {
            if (RwaUUIDCompare(interfaceInfo->inputParams[j].uniqueID.uuid.uuid, def->inputParams[i].uniqueID.uuid.uuid) == 0) {
                bVar3 = TRUE;
                break;
            }
        }

        if (!bVar3) {
            bVar4 = FALSE;
        }
    }

    noOutputParams = def->noOutputParams;
#if 1
    noOutputs = /*objDef->*/interfaceInfo->noOutputs;
#else
    noOutputs = objDef->interfaceInfo->noOutputs;
#endif
    for (i = 0; i < noOutputParams; i++) {
        bVar3 = FALSE;
        for (j = 0; j < noOutputs; j++) {
            if (RwaUUIDCompare(interfaceInfo->outputParams[j].uniqueID.uuid.uuid, def->outputParams[i].uniqueID.uuid.uuid) == 0) {
                bVar3 = TRUE;
                break;
            }
        }

        if (!bVar3) {
            bVar4 = FALSE;
        }
    }

    return !bVar4 ? NULL : def;
}

RwaObjInterfaceDef* RwaObjInterfaceDefSetup(RwaObjInterfaceDef* def, RwaObjInterfaceDefParam* inputParams, RwUInt32 noInputParams, RwaObjInterfaceDefParam* outputParams, RwUInt32 noOutputParams) {
    if (def->inputParams != NULL && def->outputParams != NULL) {
        if (!(def->flags & (1 << 0))) {
            _rwaFree(def->inputParams);
        }
        def->inputParams = NULL;
        def->outputParams = NULL;
    }

    if (inputParams != NULL && outputParams != NULL) {
        RwUInt32 i;

        for (i = 0; i < noInputParams; i++) {
            inputParams[i].uniqueID.flags = 0;
            if (_rwaParamTypeHandleRefreshType(&inputParams[i].paramType) == NULL) {
                return NULL;
            }
        }

        for (i = 0; i < noOutputParams; i++) {
            outputParams[i].uniqueID.flags = 0;
            if (_rwaParamTypeHandleRefreshType(&outputParams[i].paramType) == NULL) {
                return NULL;
            }
        }

        def->inputParams = inputParams;
        def->outputParams = outputParams;
        def->noInputParams = noInputParams;
        def->noOutputParams = noOutputParams;
        def->flags |= (1 << 0);
    } else {
        def->inputParams = _rwaMalloc((noInputParams + noOutputParams) * sizeof(RwaObjInterfaceDefParam));
        if (def->inputParams == NULL) {
            return NULL;
        }

        def->outputParams = def->inputParams + noInputParams;
        memset(def->inputParams, 0, (noInputParams + noOutputParams) * sizeof(RwaObjInterfaceDefParam));
        def->flags &= ~(1 << 0);
    }

    return def;
}

RwaObjInterfaceDef* RwaObjInterfaceDefAssignID(RwaObjInterfaceDef* def, const RwaUUID* uuid, const RwChar* name) {
    _rwaUniqueIDAssignUUID(&def->uniqueID, uuid);
    _rwaUniqueIDAssignName(&def->uniqueID, name);
    return def;
}

RwaObjInterfaceDef* RwaObjInterfaceDefCreate(RwaObjInterfaceDef* def) {
    if (def == NULL) {
        def = RwaFreeListAlloc(&_objInterfaceDefFreeList);
        if (def != NULL) {
            def->flags = 0;
        }
    } else {
        def->flags = (1 << 1);
    }

    if (def == NULL) {
        return NULL;
    }

    def->noInputParams = 0;
    def->noOutputParams = 0;
    def->inputParams = NULL;
    def->outputParams = NULL;
    def->unregFunc = NULL;
    _rwaUniqueIDInitialize(&def->uniqueID);
    def->link.prev = NULL;
    def->link.next = NULL;

    def->link.next = _objInterfaceDefList.link.next;
    def->link.prev = &_objInterfaceDefList.link;
    _objInterfaceDefList.link.next->prev = &def->link;
    _objInterfaceDefList.link.next = &def->link;

    return def;
}

RwBool RwaObjInterfaceDefDestroy(RwaObjInterfaceDef* def) {
    RwUInt32 noInputParams;
    RwInt32 i;
    RwUInt32 noOutputParams;
    RwUInt32 flags;

    if (def->unregFunc != NULL) {
        def->unregFunc();
    }

    _rwaUniqueIDFreeData(&def->uniqueID);

    noInputParams = def->noInputParams;
    for (i = 0; i < noInputParams; i++) {
        _rwaUniqueIDFreeData(&def->inputParams[i].uniqueID);
    }
    noOutputParams = def->noOutputParams;
    for (i = 0; i < noOutputParams; i++) {
        _rwaUniqueIDFreeData(&def->outputParams[i].uniqueID);
    }

    flags = def->flags;
    if (!(flags & (1 << 0))) {
        _rwaFree(def->inputParams);
    }

    def->link.prev->next = def->link.next;
    def->link.next->prev = def->link.prev;

    flags = def->flags;
    if (!(flags & (1 << 1))) {
        RwaFreeListFree(&_objInterfaceDefFreeList, def);
    }

    return TRUE;
}

RwaObjInterfaceDef* RwaObjInterfaceDefFindByUUID(const RwaUUID* uuid) {
    RwaObjInterfaceDef* def;
    RwLLLink* current;
    RwLLLink* end;

    if (uuid == NULL) {
        return NULL;
    }

    current = _objInterfaceDefList.link.next;
    end = &_objInterfaceDefList.link;
    for (; current != end; current = current->next) {
        def = (RwaObjInterfaceDef*)((RwInt32)current - offsetof(RwaObjInterfaceDef, link));
        if (RwaUUIDCompare(def->uniqueID.uuid.uuid, uuid) == 0) {
            return def;
        }
    }

    return NULL;
}
