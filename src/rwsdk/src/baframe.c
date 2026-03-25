#include <cstddef>
#include <rwsdk/plcore/bamatrix.h>
#include <rwsdk/plcore/bamemory.h>
#include <rwsdk/badevice.h>
#include <rwsdk/baframe.h>

static struct {
    unsigned int offset;
    int refCountMaybe;
} frameModule;
static int _rwFrameFreeListBlockSize = 50;
static int _rwFrameFreeListPreallocBlocks = 1;

// TODO
struct UnkRwPluginRegistryStruct {
    int unk0;
    int unk4;
    char unk8[0x10];
};
struct UnkRwPluginRegistryStruct frameTKList = {
    sizeof(RwFrame),
    sizeof(RwFrame),
};
extern void _rwPluginRegistryInitObject(struct UnkRwPluginRegistryStruct*, void* object);
extern void _rwPluginRegistryDeInitObject(struct UnkRwPluginRegistryStruct*, void* object);

extern void _rwFrameSyncHierarchyLTM(RwFrame*);

// FIXME: Unknown return/param type
void* _rwFrameOpen(void* a0, unsigned int offset) {
    static RwFreeList frameFreeList;
    frameModule.offset = offset;
    *(RwFreeList**)((int)RwEngineInstance + frameModule.offset) = RwFreeListCreateAndPreallocateSpace(frameTKList.unk0, _rwFrameFreeListBlockSize, 4, _rwFrameFreeListPreallocBlocks, &frameFreeList);
    if (*(RwFreeList**)((int)RwEngineInstance + frameModule.offset) == NULL) {
        return NULL;
    }

    RwEngineInstance->dirtyFrameListMaybe.link.next = &RwEngineInstance->dirtyFrameListMaybe.link;
    RwEngineInstance->dirtyFrameListMaybe.link.prev = &RwEngineInstance->dirtyFrameListMaybe.link;

    frameModule.refCountMaybe++;
    return a0;
}

void* _rwFrameClose(void* a0) {
    if (*(RwFreeList**)((int)RwEngineInstance + frameModule.offset) != NULL) {
        RwFreeListDestroy(*(RwFreeList**)((int)RwEngineInstance + frameModule.offset));
        *(RwFreeList**)((int)RwEngineInstance + frameModule.offset) = NULL;
    }

    frameModule.refCountMaybe--;
    return a0;
}

// Equivalent: regalloc
static void rwSetHierarchyRoot(RwFrame* frame, RwFrame* root) {
    RwFrame* current;

    frame->root = root;
    for (current = frame->child; current != NULL; current = current->next) {
        rwSetHierarchyRoot(current, root);
    }
}

int RwFrameDirty(RwFrame* frame) {
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
    RwFrame* frame = RwEngineInstance->unk140(*(RwFreeList**)((int)RwEngineInstance + frameModule.offset));
    if (frame == NULL) {
        return NULL;
    }

    _rwFrameInit(frame);
    return frame;
}

int RwFrameDestroy(RwFrame* frame) {
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

    list = *(RwFreeList**)((int)RwEngineInstance + frameModule.offset);
    RwEngineInstance->unk144(list, frame);

    return 1;
}

RwFrame* RwFrameUpdateObjects(RwFrame* frame) {
    unsigned char privateFlags = frame->root->object.privateFlags;
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
    unsigned char flags;

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
int RwFrameCount(RwFrame* frame) {
    RwFrame* current;
    int count;

    count = 1;
    for (current = frame->child; current != NULL; current = current->next) {
        count += RwFrameCount(current);
    }
    return count;
}

RwFrame* RwFrameTranslate(RwFrame* frame, RwV3d* a1, int a2) {
    RwMatrixTranslate(&frame->modelling, a1, a2);
    RwFrameUpdateObjects(frame);
    return frame;
}

RwFrame* RwFrameScale(RwFrame* frame, RwV3d* a1, int a2) {
    RwMatrixScale(&frame->modelling, a1, a2);
    RwFrameUpdateObjects(frame);
    return frame;
}

RwFrame* RwFrameTransform(RwFrame* frame, RwMatrix* a1, int a2) {
    RwMatrixTransform(&frame->modelling, a1, a2);
    RwFrameUpdateObjects(frame);
    return frame;
}

RwFrame* RwFrameRotate(RwFrame* frame, RwV3d* a1, float a2, int a3) {
    RwMatrixRotate(&frame->modelling, a1, a2, a3);
    RwFrameUpdateObjects(frame);
    return frame;
}
