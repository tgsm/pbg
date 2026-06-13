#ifndef RWSDK_PLCORE_BASTREAM_H
#define RWSDK_PLCORE_BASTREAM_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum RwStreamType {
    rwNASTREAM = 0,
    rwSTREAMFILE = 1,
    rwSTREAMFILENAME = 2,
    rwSTREAMMEMORY = 3,
    rwSTREAMCUSTOM = 4,
    rwSTREAMTYPEFORCEENUMSIZEINT = 0x7FFFFFFF,
} RwStreamType;

typedef enum RwStreamAccessType {
    rwNASTREAMACCESS = 0,
    rwSTREAMREAD = 1,
    rwSTREAMWRITE = 2,
    rwSTREAMAPPEND = 3,
    rwSTREAMACCESSTYPEFORCEENUMSIZEINT = 0x7FFFFFFF,
} RwStreamAccessType;

typedef struct RwStreamMemory {
    unsigned int position;
    unsigned int nSize;
    unsigned char* memBlock;
} RwStreamMemory; // size: 0xC

typedef union RwStreamFile {
    void* fpFile;
    const void* constfpFile;
} RwStreamFile;

typedef struct RwStreamCustom {
    int (*sfnclose)(void*);
    unsigned int (*sfnread)(void*, void*, unsigned int);
    int (*sfnwrite)(void*, const void*, unsigned int);
    int (*sfnskip)(void*, unsigned int);
    void* data;
} RwStreamCustom; // size: 0x14

typedef union RwStreamUnion {
    RwStreamMemory memory;
    RwStreamFile file;
    RwStreamCustom custom;
} RwStreamUnion;

typedef struct RwStream {
    enum RwStreamType type;
    enum RwStreamAccessType accessType;
    int position;
    RwStreamUnion Type;
    int rwOwned;
} RwStream; // size: 0x24

typedef struct RwMemory {
    unsigned char* start;
    unsigned int length;
} RwMemory; // size: 0x8

void* _rwStreamModuleOpen(void* a0, int offset, int);
void* _rwStreamModuleClose(void* a0, int, int);
RwStream* _rwStreamInitialize(RwStream* stream, int rwOwned, RwStreamType type, RwStreamAccessType accessType, void* data);
unsigned int RwStreamRead(RwStream* stream, void* dest, unsigned int size);
RwStream* RwStreamWrite(RwStream* stream, const void* src, unsigned int size);
RwStream* RwStreamSkip(RwStream* stream, unsigned int size);
int RwStreamClose(RwStream* stream, void* data);
RwStream* RwStreamOpen(enum RwStreamType type, enum RwStreamAccessType accessType, void* data);

#ifdef __cplusplus
}
#endif

#endif
