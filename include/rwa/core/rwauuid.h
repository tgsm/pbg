#ifndef RWA_CORE_RWAUUID_H
#define RWA_CORE_RWAUUID_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RwaUUID {
    unsigned int time_low;
    unsigned short time_mid;
    unsigned short time_hi_and_version;
    unsigned char node[8];
} RwaUUID;

typedef struct RwaUniqueID {
    union {
        RwaUUID* uuid;
        const RwaUUID* copyUUID;
    } uuid;
    union {
        char* uniqueName;
        const char* copyName;
    } name;
    unsigned int flags;
} RwaUniqueID;

int RwaUUIDCompare(const RwaUUID* a, const RwaUUID* b);
int _rwaUniqueIDModuleOpen(void);
void _rwaUniqueIDModuleClose(void);
RwaUniqueID* _rwaUniqueIDAssignName(RwaUniqueID* id, const char* name);
RwaUniqueID* _rwaUniqueIDAssignUUID(RwaUniqueID* id, const RwaUUID* uuid);
RwaUniqueID* _rwaUniqueIDFreeData(RwaUniqueID* id);
RwaUniqueID* _rwaUniqueIDFreeName(RwaUniqueID* id);
RwaUniqueID* _rwaUniqueIDFreeUUID(RwaUniqueID* id);
RwaUniqueID* _rwaUniqueIDInitialize(RwaUniqueID* id);
const RwaUUID* _rwaUUIDSerialize(const RwaUUID* src, RwaUUID* dest, int endianness);
int _rwamemicmp(const char* a, const char* b, int len);

#ifdef __cplusplus
}
#endif

#endif
