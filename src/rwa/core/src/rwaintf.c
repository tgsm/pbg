#include <rwa/core/rwafreelist.h>
#include <rwa/core/rwaintf.h>
#include <rwa/core/rwaobjdef.h>

static RwaFreeList _handleFreeList;
static RwLinkList _handleList;
static RwBool _moduleOpen = FALSE;

RwBool _rwaObjHandleOpenModule(void) {
    _handleList.link.next = &_handleList.link;
    _handleList.link.prev = &_handleList.link;

    if (RwaFreeListCreate(sizeof(RwaObjHandle), 64, 16, 0, &_handleFreeList) == NULL) {
        return FALSE;
    } else {
        _moduleOpen = TRUE;
        return TRUE;
    }
}

void _rwaObjHandleCloseModule(void) {
    RwLLLink* current = _handleList.link.next;
    RwLLLink* end = &_handleList.link;
    while (current != end) {
        RwaObjHandle* handle = (RwaObjHandle*)((RwInt32)current - offsetof(RwaObjHandle, link));
        RwaObjHandleRelease(handle, NULL);

        current = _handleList.link.next;
        end = &_handleList.link;
    }

    RwaFreeListDestroy(&_handleFreeList);
    _moduleOpen = FALSE;
}

struct UnkObjStateStruct {
    RwUInt32 refCountMaybe;
};

static void _RwaObjHandleFree(RwaObjHandle* handle);
RwaObjHandle* RwaObjHandleCreateUsingInterface(RwaObjDef* objDef, RwaObjHandle* a1, RwUInt8* createParamsBuffer, RwaObjInterfaceDef* interfaceDef, UnkRwaInterfaceStruct* a4) {
    UnkRwaInterfaceStruct space;
    RwaObjHandle* ret;
    RwaObj* handleObj = NULL;
    struct UnkObjStateStruct* state;

    if (a4 == NULL) {
        space.unk10 = 0;
        space.unkC = 0;
        space.unk8 = 0;
        space.unk4 = 0;
        space.unk0 = 2;
        space.unk14 = NULL;
        a4 = &space;
    } else {
        a4->unk0 |= (1 << 1);
        a4->unk0 &= (1 << 2) | (1 << 1) | (1 << 0);
    }

    if (a1 != NULL) {
        handleObj = a1->obj;
    }

    if (a4->unk14 != NULL) {
        ret = a4->unk14;
        ret->flags = (1 << 0);
    } else {
        ret = RwaFreeListAlloc(&_handleFreeList);
        ret->flags = 0;
    }
    if (ret == NULL) {
        return NULL;
    }

    ret->obj = RwaObjCreate(objDef, handleObj, a4->unk0, createParamsBuffer, a4->unk4, a4->unk8);
    handleObj = ret->obj;
    if (ret->obj == NULL) {
        _RwaObjHandleFree(ret);
        return NULL;
    }

    ret->objInterface = NULL;
    ret->refCount = 0;

    if (interfaceDef == NULL) {
        interfaceDef = handleObj->definition->interfaceInfo->defaultInterface;
    }
    if (interfaceDef != NULL) {
        ret->objInterface = RwaObjInterfaceAttach(interfaceDef, handleObj->definition, a4->unkC, a4->unk10);
        if (ret->objInterface == NULL) {
            RwaObjDestroy(ret->obj, NULL, NULL);
            _RwaObjHandleFree(ret);
            return NULL;
        }
    }

    if (ret->obj->state != NULL) {
        state = (struct UnkObjStateStruct*)((RwInt32)(ret->obj->state) + (ret->obj->definition->stateSizeAlign & 0x0FFFFFFF));
    } else {
        state = (struct UnkObjStateStruct*)(ret->obj + 1);
    }
    state->refCountMaybe++;

    ret->link.prev = NULL;
    ret->link.next = NULL;
    ret->link.next = _handleList.link.next;
    ret->link.prev = &_handleList.link;
    _handleList.link.next->prev = &ret->link;
    _handleList.link.next = &ret->link;

    return ret;
}

static void _RwaObjHandleFree(RwaObjHandle* handle) {
    if (!(handle->flags & (1 << 0))) {
        RwaFreeListFree(&_handleFreeList, handle);
    }
}

void RwaObjHandleRelease(RwaObjHandle* handle, UnkReleaseStruct* a1) {
    struct UnkObjStateStruct* state;
    RwaObj* handleObj;
    UnkReleaseStruct space = {};

    if (a1 == NULL) {
        a1 = &space;
    }

    if (handle->refCount != 0) {
        handle->refCount--;
        return;
    }

    if (handle->objInterface != NULL && RwaObjInterfaceRemove(handle->objInterface) == TRUE) {
        a1->interface = handle->objInterface;
    }

    handleObj = handle->obj;
    if (handleObj->state != NULL) {
        state = (struct UnkObjStateStruct*)((RwInt32)(handleObj->state) + (handleObj->definition->stateSizeAlign & 0xFFFFFFF));
    } else {
        state = (struct UnkObjStateStruct*)(handleObj + 1);
    }
    state->refCountMaybe--;
    if (state->refCountMaybe == 0) {
        RwaObjDestroy(handleObj, a1->func0, a1->unk4);
    }

    handle->link.prev->next = handle->link.next;
    handle->link.next->prev = handle->link.prev;
    if (!(handle->flags & (1 << 0))) {
        RwaFreeListFree(&_handleFreeList, handle);
    }
    a1->handle = handle;
}

RwaObjHandle* RwaObjHandleSetParamData(RwaObjHandle* handle, RwInt32 inputID, RwInt32, void* data) {
    RwaObj* handleObj = handle->obj;
    RwaInputParamMap* param = &handle->objInterface->inputParamMaps[inputID];
    if (param->setFunc(handleObj, param->paramIndex, data) == NULL) {
        return NULL;
    }

    return handle;
}

RwaObjHandle* RwaObjHandleSetParamInt32(RwaObjHandle* handle, RwInt32 inputID, RwInt32, RwInt32 data) {
    RwaObj* handleObj;
    RwaInputParamMap* param;

    if (handle == NULL) {
        return NULL;
    }

    handleObj = handle->obj;
    param = &handle->objInterface->inputParamMaps[inputID];
    if (param->setFunc(handleObj, param->paramIndex, &data) == NULL) {
        return NULL;
    }

    return handle;
}

RwaObjHandle* RwaObjHandleSetParamUInt32(RwaObjHandle* handle, RwInt32 inputID, RwInt32, RwUInt32 data) {
    RwaObj* handleObj;
    RwaInputParamMap* param;

    if (handle == NULL) {
        return NULL;
    }

    handleObj = handle->obj;
    param = &handle->objInterface->inputParamMaps[inputID];
    if (param->setFunc(handleObj, param->paramIndex, &data) == NULL) {
        return NULL;
    }

    return handle;
}

RwaObjHandle* RwaObjHandleSetParamReal(RwaObjHandle* handle, RwInt32 inputID, RwInt32, RwReal data) {
    RwaObj* handleObj;
    RwaInputParamMap* param;

    if (handle == NULL) {
        return NULL;
    }

    handleObj = handle->obj;
    param = &handle->objInterface->inputParamMaps[inputID];
    if (param->setFunc(handleObj, param->paramIndex, &data) == NULL) {
        return NULL;
    }

    return handle;
}

RwaObjHandle* RwaObjHandleSetParamPointer(RwaObjHandle* handle, RwInt32 inputID, RwInt32, void* ptr) {
    RwaObj* handleObj;
    RwaInputParamMap* param;

    if (handle == NULL) {
        return NULL;
    }

    handleObj = handle->obj;
    param = &handle->objInterface->inputParamMaps[inputID];
    if (param->setFunc(handleObj, param->paramIndex, ptr) == NULL) {
        return NULL;
    }

    return handle;
}

RwInt32 RwaObjHandleGetParamInt32(RwaObjHandle* handle, RwInt32 outputID, RwInt32, RwBool* success) {
    RwaOutputParamMap* param = &handle->objInterface->outputParamMaps[outputID];
    RwUInt16 index = param->paramIndex;
    RwInt32 data;
    RwaObjHandle* result = (param->getFunc(handle->obj, index, &data) == NULL) ? NULL : handle;

    if (success != NULL) {
        if (result != NULL) {
            *success = TRUE;
        } else {
            *success = FALSE;
        }
    }

    return data;
}

RwUInt32 RwaObjHandleGetParamUInt32(RwaObjHandle* handle, RwInt32 outputID, RwInt32, RwBool* success) {
    RwaOutputParamMap* param = &handle->objInterface->outputParamMaps[outputID];
    RwUInt16 index = param->paramIndex;
    RwUInt32 data;
    RwaObjHandle* result = (param->getFunc(handle->obj, index, &data) == NULL) ? NULL : handle;

    if (success != NULL) {
        if (result != NULL) {
            *success = TRUE;
        } else {
            *success = FALSE;
        }
    }

    return data;
}

void* RwaObjHandleGetParamPointer(RwaObjHandle* handle, RwInt32 outputID, RwInt32, RwBool* success) {
    RwaOutputParamMap* param = &handle->objInterface->outputParamMaps[outputID];
    RwUInt16 index = param->paramIndex;
    void* data;
    RwaObjHandle* result = (param->getFunc(handle->obj, index, &data) == NULL) ? NULL : handle;

    if (success != NULL) {
        if (result != NULL) {
            *success = TRUE;
        } else {
            *success = FALSE;
        }
    }

    return data;
}
