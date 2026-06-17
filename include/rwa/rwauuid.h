#ifndef RWA_RWAUUID_H
#define RWA_RWAUUID_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RwaUUID {
    unsigned char unk0[4];
    unsigned char unk4[2];
    unsigned char unk6[2];
    unsigned char unk8[8];
} RwaUUID;

typedef struct RwaUniqueID {
    RwaUUID* uuid;
    char* name;
    unsigned int flags;
} RwaUniqueID;

int RwaUUIDCompare(RwaUUID* a, RwaUUID* b);
int _rwaUniqueIDModuleOpen(void);
void _rwaUniqueIDModuleClose(void);
RwaUniqueID* _rwaUniqueIDAssignName(RwaUniqueID* id, char* name);
RwaUniqueID* _rwaUniqueIDAssignUUID(RwaUniqueID* id, RwaUUID* uuid);
RwaUniqueID* _rwaUniqueIDFreeData(RwaUniqueID* id);
RwaUniqueID* _rwaUniqueIDFreeName(RwaUniqueID* id);
RwaUniqueID* _rwaUniqueIDFreeUUID(RwaUniqueID* id);
RwaUniqueID* _rwaUniqueIDInitialize(RwaUniqueID* id);
RwaUUID* _rwaUUIDSerialize(RwaUUID* src, RwaUUID* dest, int endianness);
int _rwamemicmp(char* a, char* b, int len);

#ifdef __cplusplus
}
#endif

#endif
