#include <cstddef>
#include <rwsdk/plcore/bamatrix.h>
#include <rwsdk/plcore/bamemory.h>
#include <rwsdk/badevice.h>
#include <rwsdk/baframe.h>

static RwModuleInfo frameModule;
static RwInt32 _rwFrameFreeListBlockSize = 50;
static RwInt32 _rwFrameFreeListPreallocBlocks = 1;

struct RwPluginRegistry frameTKList = {
    sizeof(RwFrame),
    sizeof(RwFrame),
    0,
    0,
    NULL,
    NULL,
};

extern void _rwFrameSyncHierarchyLTM(RwFrame*);

void* _rwFrameOpen(void* object, RwInt32 offset, RwInt32) {
    static RwFreeList frameFreeList;
    frameModule.globalsOffset = offset;
    *(RwFreeList**)((RwInt32)RwEngineInstance + frameModule.globalsOffset) = RwFreeListCreateAndPreallocateSpace(frameTKList.sizeOfStruct, _rwFrameFreeListBlockSize, 4, _rwFrameFreeListPreallocBlocks, &frameFreeList);
    if (*(RwFreeList**)((RwInt32)RwEngineInstance + frameModule.globalsOffset) == NULL) {
        return NULL;
    }

    RwEngineInstance->dirtyFrameListMaybe.link.next = &RwEngineInstance->dirtyFrameListMaybe.link;
    RwEngineInstance->dirtyFrameListMaybe.link.prev = &RwEngineInstance->dirtyFrameListMaybe.link;

    frameModule.numInstances++;
    return object;
}

void* _rwFrameClose(void* object, RwInt32, RwInt32) {
    if (*(RwFreeList**)((RwInt32)RwEngineInstance + frameModule.globalsOffset) != NULL) {
        RwFreeListDestroy(*(RwFreeList**)((RwInt32)RwEngineInstance + frameModule.globalsOffset));
        *(RwFreeList**)((RwInt32)RwEngineInstance + frameModule.globalsOffset) = NULL;
    }

    frameModule.numInstances--;
    return object;
}

// Equivalent: regalloc
static void rwSetHierarchyRoot(RwFrame* frame, RwFrame* root) {
    RwFrame* current;

    frame->root = root;
    for (current = frame->child; current != NULL; current = current->next) {
        rwSetHierarchyRoot(current, root);
    }
}

static RwFrame* rwFrameCloneRecurse(RwFrame* frame, RwFrame* parent) {
    RwFrame* clone = RwEngineInstance->memoryAlloc(*(RwFreeList**)((RwInt32)RwEngineInstance + frameModule.globalsOffset));
    RwFrame* child;
    if (clone == NULL) {
        return NULL;
    }

    if (parent == NULL) {
        parent = clone;
    }
    clone->object.type = frame->object.type;
    clone->object.subType = frame->object.subType;
    clone->object.flags = frame->object.flags;
    clone->object.privateFlags = frame->object.privateFlags;
    clone->object.parent = NULL;
    clone->objectList.link.next = &clone->objectList.link;
    clone->objectList.link.prev = &clone->objectList.link;
    clone->object.parent = NULL;
    clone->modelling = frame->modelling;
    clone->child = NULL;
    clone->next = NULL;
    clone->root = parent;
    frame->root = clone;

    for (child = frame->child; child != NULL; child = child->next) {
        RwFrame* childClone = rwFrameCloneRecurse(child, parent);
        if (childClone == NULL) {
            RwFrameDestroyHierarchy(clone);
            return NULL;
        }

        childClone->next = clone->child;
        clone->child = childClone;
        childClone->object.parent = clone;
    }

    _rwPluginRegistryInitObject(&frameTKList, clone);
    _rwPluginRegistryCopyObject(&frameTKList, clone, frame);

    return clone;
}

RwFrame* _rwFrameCloneAndLinkClones(RwFrame* frame) {
    RwFrame* clone = rwFrameCloneRecurse(frame, NULL);
    if (clone != NULL) {
        clone->object.privateFlags &= ~((1 << 1) | (1 << 0));
        RwFrameUpdateObjects(clone);
    }
    return clone;
}

RwFrame* _rwFramePurgeClone(RwFrame* frame) {
    RwFrame* parent = (RwFrame*)frame->object.parent;
    if (parent != NULL) {
        rwSetHierarchyRoot(frame, parent->root);
    } else {
        rwSetHierarchyRoot(frame, frame);
    }
    return frame;
}

RwBool RwFrameDirty(RwFrame* frame) {
    return frame->root->object.privateFlags & ((1 << 1) | (1 << 0));
}

void _rwFrameInit(RwFrame* frame) {
    frame->object.type = 0;
    frame->object.subType = 0;
    frame->object.flags = 0;
    frame->object.privateFlags = 0;
    frame->object.parent = NULL;
    frame->objectList.link.next = &frame->objectList.link;
    frame->objectList.link.prev = &frame->objectList.link;
    frame->modelling.flags = (1 << 1) | (1 << 0);
    frame->modelling.at.z = 1.0f;
    frame->modelling.up.y = 1.0f;
    frame->modelling.right.x = 1.0f;
    frame->modelling.up.x = 0.0f;
    frame->modelling.right.z = 0.0f;
    frame->modelling.right.y = 0.0f;
    frame->modelling.at.y = 0.0f;
    frame->modelling.at.x = 0.0f;
    frame->modelling.up.z = 0.0f;
    frame->modelling.pos.z = 0.0f;
    frame->modelling.pos.y = 0.0f;
    frame->modelling.pos.x = 0.0f;
    frame->modelling.flags |= (1 << 17) | (1 << 1) | (1 << 0);
    frame->ltm.flags = (1 << 1) | (1 << 0);
    frame->ltm.at.z = 1.0f;
    frame->ltm.up.y = 1.0f;
    frame->ltm.right.x = 1.0f;
    frame->ltm.up.x = 0.0f;
    frame->ltm.right.z = 0.0f;
    frame->ltm.right.y = 0.0f;
    frame->ltm.at.y = 0.0f;
    frame->ltm.at.x = 0.0f;
    frame->ltm.up.z = 0.0f;
    frame->ltm.pos.z = 0.0f;
    frame->ltm.pos.y = 0.0f;
    frame->ltm.pos.x = 0.0f;
    frame->ltm.flags |= (1 << 17) | (1 << 1) | (1 << 0);
    frame->child = NULL;
    frame->next = NULL;
    frame->root = frame;

    _rwPluginRegistryInitObject(&frameTKList, frame);
}

RwFrame* RwFrameCreate(void) {
    RwFrame* frame = RwEngineInstance->memoryAlloc(*(RwFreeList**)((RwInt32)RwEngineInstance + frameModule.globalsOffset));
    if (frame == NULL) {
        return NULL;
    }

    _rwFrameInit(frame);
    return frame;
}

RwBool RwFrameDestroy(RwFrame* frame) {
    RwFrame* current;
    RwFreeList* list;

    _rwPluginRegistryDeInitObject(&frameTKList, frame);

    if (frame->object.parent != NULL) {
        RwFrameRemoveChild(frame);
    }

    if (frame->object.privateFlags & ((1 << 1) | (1 << 0))) {
        frame->inDirtyListLink.prev->next = frame->inDirtyListLink.next;
        frame->inDirtyListLink.next->prev = frame->inDirtyListLink.prev;
    }

    for (current = frame->child; current != NULL; current = current->next) {
        current->object.parent = NULL;
    }

    list = *(RwFreeList**)((RwInt32)RwEngineInstance + frameModule.globalsOffset);
    RwEngineInstance->memoryFree(list, frame);

    return TRUE;
}

static void FrameDestroyRecurseDeInitLeaf(RwFrame* frame) {
    _rwPluginRegistryDeInitObject(&frameTKList, frame);
    if (frame->object.privateFlags & ((1 << 1) | (1 << 0))) {
        frame->inDirtyListLink.prev->next = frame->inDirtyListLink.next;
        frame->inDirtyListLink.next->prev = frame->inDirtyListLink.prev;
    }
}

static void rwFrameDestroyRecurseDestroyLeaf(RwFrame* frame) {
    FrameDestroyRecurseDeInitLeaf(frame);
    RwEngineInstance->memoryFree(*(RwFreeList**)((RwInt32)RwEngineInstance + frameModule.globalsOffset), frame);
}

static void rwFrameDestroyRecurse(RwFrame* frame) {
    if (frame != NULL) {
        RwFrame* child = frame->child;
        while (child != NULL) {
            RwFrame* next = child->next;
            rwFrameDestroyRecurse(child);
            child = next;
        }
        rwFrameDestroyRecurseDestroyLeaf(frame);
    }
}

RwBool RwFrameDestroyHierarchy(RwFrame* frame) {
    rwFrameDestroyRecurse(frame);
    return TRUE;
}

RwFrame* RwFrameUpdateObjects(RwFrame* frame) {
    RwUInt8 privateFlags = frame->root->object.privateFlags;
    if (!(privateFlags & ((1 << 1) | (1 << 0)))) {
        frame->root->inDirtyListLink.next = RwEngineInstance->dirtyFrameListMaybe.link.next;
        frame->root->inDirtyListLink.prev = &RwEngineInstance->dirtyFrameListMaybe.link;
        RwEngineInstance->dirtyFrameListMaybe.link.next->prev = &frame->root->inDirtyListLink;
        RwEngineInstance->dirtyFrameListMaybe.link.next = &frame->root->inDirtyListLink;
    }
    frame->root->object.privateFlags = privateFlags | ((1 << 1) | (1 << 0));
    frame->object.privateFlags |= ((1 << 3) | (1 << 2));

    return frame;
}

RwMatrix* RwFrameGetLTM(RwFrame* frame) {
    if (frame->root->object.privateFlags & (1 << 0)) {
        _rwFrameSyncHierarchyLTM(frame->root);
    }
    return &frame->ltm;
}

extern void rwSetHierarchyRoot(RwFrame*, RwFrame*);

RwFrame* RwFrameAddChild(RwFrame* frame, RwFrame* child) {
    RwUInt8 flags;

    if (child->object.parent != NULL) {
        RwFrameRemoveChild(child);
    }
    child->next = frame->child;
    frame->child = child;
    child->object.parent = frame;
    rwSetHierarchyRoot(child, frame->root);

    if (child->object.privateFlags & ((1 << 1) | (1 << 0))) {
        child->inDirtyListLink.prev->next = child->inDirtyListLink.next;
        child->inDirtyListLink.next->prev = child->inDirtyListLink.prev;
        child->object.privateFlags &= ~((1 << 1) | (1 << 0));
    }

    RwFrameUpdateObjects(child);

    return frame;
}

RwFrame* RwFrameRemoveChild(RwFrame* frame) {
    RwFrame* current = ((RwFrame*)(frame->object.parent))->child;
    if (current == frame) {
        ((RwFrame*)(frame->object.parent))->child = frame->next;
    } else {
        while (current->next != frame) {
            current = current->next;
        }
        current->next = frame->next;
    }

    frame->object.parent = NULL;
    frame->next = NULL;
    rwSetHierarchyRoot(frame, frame);

    RwFrameUpdateObjects(frame);

    return frame;
}

RwFrame* RwFrameForAllChildren(RwFrame* frame, RwFrameCallBack callback, void* data) {
    RwFrame* current;
    RwFrame* next;
    for (current = frame->child; current != NULL; current = next) {
        next = current->next;
        if (callback(current, data) == NULL) {
            return frame;
        }
    }

    return frame;
}

// Equivalent: regalloc
RwInt32 RwFrameCount(RwFrame* frame) {
    RwFrame* current;
    RwInt32 count;

    count = 1;
    for (current = frame->child; current != NULL; current = current->next) {
        count += RwFrameCount(current);
    }
    return count;
}

RwFrame* RwFrameTranslate(RwFrame* frame, RwV3d* a1, RwInt32 a2) {
    RwMatrixTranslate(&frame->modelling, a1, a2);
    RwFrameUpdateObjects(frame);
    return frame;
}

RwFrame* RwFrameScale(RwFrame* frame, RwV3d* a1, RwInt32 a2) {
    RwMatrixScale(&frame->modelling, a1, a2);
    RwFrameUpdateObjects(frame);
    return frame;
}

RwFrame* RwFrameTransform(RwFrame* frame, RwMatrix* a1, RwInt32 a2) {
    RwMatrixTransform(&frame->modelling, a1, a2);
    RwFrameUpdateObjects(frame);
    return frame;
}

RwFrame* RwFrameRotate(RwFrame* frame, RwV3d* a1, RwReal a2, RwInt32 a3) {
    RwMatrixRotate(&frame->modelling, a1, a2, a3);
    RwFrameUpdateObjects(frame);
    return frame;
}

RwInt32 RwFrameRegisterPlugin(RwInt32 size, RwInt32 pluginID, RwPluginObjectConstructor constructCB, RwPluginObjectDestructor destructCB, RwPluginObjectCopy copyCB) {
    return _rwPluginRegistryAddPlugin(&frameTKList, size, pluginID, constructCB, destructCB, copyCB);
}
