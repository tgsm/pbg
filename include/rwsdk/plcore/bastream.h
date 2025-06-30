#ifndef RWSDK_PLCORE_BASTREAM_H
#define RWSDK_PLCORE_BASTREAM_H

enum RwStreamType {
    rwNASTREAM = 0,
    rwSTREAMFILE = 1,
    rwSTREAMFILENAME = 2,
    rwSTREAMMEMORY = 3,
    rwSTREAMCUSTOM = 4,
    rwSTREAMTYPEFORCEENUMSIZEINT = 0x7FFFFFFF,
};

enum RwStreamAccessType {
    rwNASTREAMACCESS = 0,
    rwSTREAMREAD = 1,
    rwSTREAMWRITE = 2,
    rwSTREAMAPPEND = 3,
    rwSTREAMACCESSTYPEFORCEENUMSIZEINT = 0x7FFFFFFF,
};

typedef struct RwStreamMemory {
    // TODO
} RwStreamMemory;

typedef struct RwStreamFile {
    // TODO
} RwStreamFile;

typedef struct RwStreamCustom {
    int (*sfnclose)(void*);
    unsigned int (*sfnread)(void*, void*, unsigned int);
    int (*sfnwrite)(void*, void*, unsigned int);
    int (*sfnskip)(void*, unsigned int);
    void* data;
} RwStreamCustom;

union RwStreamUnion {
    RwStreamMemory memory;
    RwStreamFile file;
    RwStreamCustom custom;
};

typedef struct RwStream {
    enum RwStreamType type;
    enum RwStreamAccessType accessType;
    int position;
    union RwStreamUnion Type;
    int rwOwned;
} RwStream; // size: 0x24

#endif
