#ifndef RWA_CORE_RWAUUID_H
#define RWA_CORE_RWAUUID_H

#include <rwsdk/rwtypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RwaUUID {
    RwUInt32 time_low;
    RwUInt16 time_mid;
    RwUInt16 time_hi_and_version;
    RwUInt8 node[8];
} RwaUUID;

typedef struct RwaUniqueID {
    union {
        RwaUUID* uuid;
        const RwaUUID* copyUUID;
    } uuid;
    union {
        RwChar* uniqueName;
        const RwChar* copyName;
    } name;
    RwUInt32 flags;
} RwaUniqueID;

RwInt32 RwaUUIDCompare(const RwaUUID* a, const RwaUUID* b);
RwBool _rwaUniqueIDModuleOpen(void);
void _rwaUniqueIDModuleClose(void);
RwaUniqueID* _rwaUniqueIDAssignName(RwaUniqueID* id, const RwChar* name);
RwaUniqueID* _rwaUniqueIDAssignUUID(RwaUniqueID* id, const RwaUUID* uuid);
RwaUniqueID* _rwaUniqueIDFreeData(RwaUniqueID* id);
RwaUniqueID* _rwaUniqueIDFreeName(RwaUniqueID* id);
RwaUniqueID* _rwaUniqueIDFreeUUID(RwaUniqueID* id);
RwaUniqueID* _rwaUniqueIDInitialize(RwaUniqueID* id);
const RwaUUID* _rwaUUIDSerialize(const RwaUUID* src, RwaUUID* dest, RwInt32 endianness);
RwInt32 _rwamemicmp(const RwChar* a, const RwChar* b, RwInt32 len);

#ifdef __cplusplus
}
#endif

#endif
