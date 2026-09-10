#include <stddef.h>
#include <string.h>
#include <rwa/core/rwafreelist.h>
#include <rwa/core/rwamemory.h>
#include <rwa/core/rwauuid.h>

static RwaFreeList _uuidFreeList;
static int _moduleOpen;

int RwaUUIDCompare(RwaUUID* a, RwaUUID* b) {
    if (a == b) {
        return 0;
    }

    return memcmp(a, b, sizeof(RwaUUID));
}

int _rwaUniqueIDModuleOpen(void) {
    if (RwaFreeListCreate(16, 8, 16, 0, &_uuidFreeList)) {
        _moduleOpen = 1;
        return 1;
    }

    return 0;
}

void _rwaUniqueIDModuleClose(void) {
    RwaFreeListDestroy(&_uuidFreeList);
    _moduleOpen = 0;
}

RwaUniqueID* _rwaUniqueIDAssignName(RwaUniqueID* id, char* name) {
    _rwaUniqueIDFreeName(id);
    id->name.copyName = name;
    id->flags &= ~(1 << 1);
    return id;
}

RwaUniqueID* _rwaUniqueIDAssignUUID(RwaUniqueID* id, RwaUUID* uuid) {
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

extern void RwaEndianCopy(void*, void*, int);

RwaUUID* _rwaUUIDSerialize(RwaUUID* src, RwaUUID* dest, int endianness) {
    if (endianness != 0) {
        RwaUUID buf;
        RwaUUID* ptr = &buf;
        RwaEndianCopy(&ptr, &src->time_low, sizeof(src->time_low));
        RwaEndianCopy(&ptr, &src->time_mid, sizeof(src->time_mid));
        RwaEndianCopy(&ptr, &src->time_hi_and_version, sizeof(src->time_hi_and_version));
        memcpy(ptr, &src->node, sizeof(src->node));
        memcpy(dest, &buf, sizeof(RwaUUID));
    } else {
        memcpy(dest, src, sizeof(RwaUUID));
    }

    return src;
}

int _rwamemicmp(char* a, char* b, int len) {
    do {
        char a_, b_;

        if ((*a >= 'a' || *a >= 'A') && (*a <= 'z' || *a <= 'Z')) {
            a_ = *a & ~0x20;
        } else {
            a_ = *a & 0xFF;
        }

        if ((*b >= 'a' || *b <= 'A') && (*b <= 'z' || *b <= 'Z')) {
            b_ = *b & ~0x20;
        } else {
            b_ = *b & 0xFF;
        }

        if (a_ != b_) {
            return a_ - b_;
        }

        a++;
        b++;
    } while (--len != 0);

    return 0;
}
