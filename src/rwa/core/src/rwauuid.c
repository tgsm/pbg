#include <stddef.h>
#include <string.h>
#include <rwa/core/rwafreelist.h>
#include <rwa/core/rwamemory.h>
#include <rwa/core/rwauuid.h>

static RwaFreeList _uuidFreeList;
static RwBool _moduleOpen = FALSE;

RwInt32 RwaUUIDCompare(const RwaUUID* a, const RwaUUID* b) {
    if (a == b) {
        return 0;
    }

    return memcmp(a, b, sizeof(RwaUUID));
}

RwBool _rwaUniqueIDModuleOpen(void) {
    if (RwaFreeListCreate(sizeof(RwaUUID), 8, 16, 0, &_uuidFreeList)) {
        _moduleOpen = TRUE;
        return TRUE;
    }

    return FALSE;
}

void _rwaUniqueIDModuleClose(void) {
    RwaFreeListDestroy(&_uuidFreeList);
    _moduleOpen = FALSE;
}

RwaUniqueID* _rwaUniqueIDAssignName(RwaUniqueID* id, const RwChar* name) {
    _rwaUniqueIDFreeName(id);
    id->name.copyName = name;
    id->flags &= ~(1 << 1);
    return id;
}

RwaUniqueID* _rwaUniqueIDAssignUUID(RwaUniqueID* id, const RwaUUID* uuid) {
    _rwaUniqueIDFreeUUID(id);
    id->uuid.copyUUID = uuid;
    id->flags &= ~(1 << 0);
    return id;
}

RwaUniqueID* _rwaUniqueIDFreeData(RwaUniqueID* id) {
    _rwaUniqueIDFreeUUID(id);
    _rwaUniqueIDFreeName(id);
    return id;
}

RwaUniqueID* _rwaUniqueIDFreeName(RwaUniqueID* id) {
    if (id->name.uniqueName != NULL && id->flags & (1 << 1)) {
        _rwaFree(id->name.uniqueName);
        id->flags &= ~(1 << 1);
    }
    return id;
}

RwaUniqueID* _rwaUniqueIDFreeUUID(RwaUniqueID* id) {
    if (id->uuid.uuid != NULL && id->flags & (1 << 0)) {
        RwaFreeListFree(&_uuidFreeList, id->uuid.uuid);
        id->flags &= ~(1 << 0);
    }
    return id;
}

RwaUniqueID* _rwaUniqueIDInitialize(RwaUniqueID* id) {
    id->flags = 0;
    id->name.uniqueName = NULL;
    id->uuid.uuid = NULL;
    return id;
}

extern void RwaEndianCopy(void*, void*, RwInt32);

const RwaUUID* _rwaUUIDSerialize(const RwaUUID* src, RwaUUID* dest, RwInt32 endianness) {
    if (endianness != 0) {
        RwaUUID buf;
        RwaUUID* ptr = &buf;
        RwaEndianCopy(&ptr, (void*)&src->time_low, sizeof(src->time_low));
        RwaEndianCopy(&ptr, (void*)&src->time_mid, sizeof(src->time_mid));
        RwaEndianCopy(&ptr, (void*)&src->time_hi_and_version, sizeof(src->time_hi_and_version));
        memcpy(ptr, &src->node, sizeof(src->node));
        memcpy(dest, &buf, sizeof(RwaUUID));
    } else {
        memcpy(dest, src, sizeof(RwaUUID));
    }

    return src;
}

RwInt32 _rwamemicmp(const RwChar* a, const RwChar* b, RwInt32 len) {
    do {
        RwChar a_, b_;

        if ((*a >= 'a' || *a >= 'A') && (*a <= 'z' || *a <= 'Z')) {
            a_ = *a & ~0x20;
        } else {
            a_ = *a;
        }

        if ((*b >= 'a' || *b <= 'A') && (*b <= 'z' || *b <= 'Z')) {
            b_ = *b & ~0x20;
        } else {
            b_ = *b;
        }

        if (a_ != b_) {
            return a_ - b_;
        }

        a++;
        b++;
    } while (--len != 0);

    return 0;
}
