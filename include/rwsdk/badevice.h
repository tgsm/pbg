#ifndef RWSDK_BADEVICE_H
#define RWSDK_BADEVICE_H

#include <stdarg.h>
#include <rwsdk/plcore/bafsys.h>
#include <rwsdk/plcore/baimmedi.h>
#include <rwsdk/plcore/bamemory.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RwEngineOpenParams {
    void* displayID;

    // These may be unique to PBG, they're not present in BfBB DWARF
    void* unk4;
    int unk8;
    int unkC;
} RwEngineOpenParams;

typedef struct RwSubSystemInfo {
    char name[0x50];
} RwSubSystemInfo;

typedef struct RwVideoMode {
    unsigned int unk0;
    unsigned int unk4;
    unsigned int unk8;
    unsigned int unkC;
} RwVideoMode;

typedef struct RwStringFunctions {
    int (*rwsprintf)(char* str, const char* format, ...); // 0xF0
    int (*rwvsprintf)(char* str, const char* format, va_list ap);
    char* (*rwstrcpy)(char* dst, const char* src);
    char* (*rwstrncpy)(char* dst, const char* src, unsigned long len);
    char* (*rwstrcat)(char* dst, const char* src);
    char* (*rwstrncat)(char* dst, const char* src, unsigned long len);
    char* (*rwstrrchr)(const char* str, int c);
    char* (*rwstrchr)(const char* str, int c);
    char* (*rwstrstr)(const char* haystack, const char* needle);
    int (*rwstrcmp)(const char* a, const char* b);
    int (*rwstricmp)(const char* a, const char* b);
    int (*rwstrlen)(const char* str);
    char* (*rwstrupr)(char* str);
    char* (*rwstrlwr)(char* str);
    char* (*rwstrtok)(char* str, const char* delim);
    int (*rwsscanf)(const char* str, const char* format, ...);
} RwStringFunctions; // size: 0x40

typedef enum RwPrimitiveType {
    rwPRIMTYPENAPRIMTYPE = 0,
    rwPRIMTYPELINELIST = 1,
    rwPRIMTYPEPOLYLINE = 2,
    rwPRIMTYPETRILIST = 3,
    rwPRIMTYPETRISTRIP = 4,
    rwPRIMTYPETRIFAN = 5,
    rwPRIMTYPEPOINTLIST = 6,
    rwPRIMITIVETYPEFORCEENUMSIZEINT = 0x7FFFFFFF,
} RwPrimitiveType;

typedef struct rwGameCube2DVertex {
    float x;
    float y;
    float z;
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
    float u;
    float v;
} RwIm2DVertex;

typedef struct RwDevice {
    float gammaCorrection;
    int (*fpSystem)(int, void*, void*, int);
    float zBufferNear;
    float zBufferFar;
    int (*fpRenderStateSet)(RwRenderState, void*);
    int (*fpRenderStateGet)(RwRenderState, void*);
    int (*fpIm2DRenderLine)(RwIm2DVertex*, int, int, int);
    int (*fpIm2DRenderTriangle)(RwIm2DVertex*, int, int, int, int);
    int (*fpIm2DRenderPrimitive)(RwPrimitiveType, RwIm2DVertex*, int);
    int (*fpIm2DRenderIndexedPrimitive)(RwPrimitiveType, RwIm2DVertex*, int, unsigned short*, int);
    int (*fpIm3DRenderLine)(int, int);
    int (*fpIm3DRenderTriangle)(int, int, int);
    int (*fpIm3DRenderPrimitive)(RwPrimitiveType);
    int (*fpIm3DRenderIndexedPrimitive)(RwPrimitiveType, unsigned short*, int);
} RwDevice; // size: 0x38

typedef enum RwCoreDeviceSystemFn {
    rwDEVICESYSTEMOPEN = 0,
    rwDEVICESYSTEMCLOSE = 1,
    rwDEVICESYSTEMSTART = 2,
    rwDEVICESYSTEMSTOP = 3,
    rwDEVICESYSTEMREGISTER = 4,
    rwDEVICESYSTEMGETNUMMODES = 5,
    rwDEVICESYSTEMGETMODEINFO = 6,
    rwDEVICESYSTEMUSEMODE = 7,
    rwDEVICESYSTEMGETMODE = 10,
    rwDEVICESYSTEMSTANDARDS = 11,
    rwDEVICESYSTEMGETTEXMEMSIZE = 12,
    rwDEVICESYSTEMGETNUMSUBSYSTEMS = 13,
    rwDEVICESYSTEMGETSUBSYSTEMINFO = 14,
    rwDEVICESYSTEMGETCURRENTSUBSYSTEM = 15,
    rwDEVICESYSTEMSETSUBSYSTEM = 16,
    rwDEVICESYSTEMFINALIZESTART = 17,
    rwDEVICESYSTEMINITIATESTOP = 18,
    rwDEVICESYSTEMGETMAXTEXTURESIZE = 19,
    rwCOREDEVICESYSTEMFNFORCEENUMSIZEINT = 0x7FFFFFFF,
} RwCoreDeviceSystemFn;

typedef int (*RwStandardFunc)(void*, void*, int);

typedef enum RwEngineStatus {
    rwENGINESTATUSIDLE = 0,
    rwENGINESTATUSINITED = 1,
    rwENGINESTATUSOPENED = 2,
    rwENGINESTATUSSTARTED = 3,
    rwENGINESTATUSFORCEENUMSIZEINT = 0x7FFFFFFF,
} RwEngineStatus;

typedef enum RwEngineInitFlag {
    rwENGINEINITFREELISTS = 0,
    rwENGINEINITNOFREELISTS = (1 << 0),
    rwENGINEINITFLAGFORCEENUMSIZEINT = 0x7FFFFFFF,
} RwEngineInitFlag;

typedef struct RwGlobals {
    void* curCamera;
    char unk4[0x6];
    unsigned short lightFrame;
    unsigned short pad[2];
    RwDevice dOpenDevice;
    char unk48[0x58 - 0x48];
    RwStandardFunc unk58;
    RwStandardFunc unk5C;
    char unk60[0x84 - 0x60];
    RwStandardFunc unk84;
    RwStandardFunc unk88;
    RwStandardFunc unk8C;
    char unk90[4];
    RwStandardFunc unk94;
    RwStandardFunc unk98;
    char unk9C[0xA4 - 0x9C];
    RwStandardFunc unkA4;
    RwStandardFunc unkA8;
    char unkAC[0xB8 - 0xAC];
    RwStandardFunc unkB8;
    RwLinkList dirtyFrameListMaybe;
    RwFileFunctions fileFuncs;
    RwStringFunctions stringFuncs;
    RwMemoryFunctions memoryFuncs;
    void* (*memoryAlloc)(RwFreeList*);
    RwFreeList* (*memoryFree)(RwFreeList*, void*);
    char unk148[4];
    RwEngineStatus engineStatus;
    unsigned int resArenaInitSize;
} RwGlobals; // size: 0x154

typedef struct RwModuleInfo {
    int globalsOffset;
    int numInstances;
} RwModuleInfo;

// FIXME: DWARFs say this is a void*
extern RwGlobals* RwEngineInstance;

int _rwDeviceSystemRequest(RwDevice* device, int systemFn, void* dest, void*, int);
int _rwGetNumEngineInstances(void);
unsigned int RwEngineGetVersion(void);
int RwEngineRegisterPlugin(int, int pluginID, void* openFunc, void* closeFunc); // FIXME: RwPluginObjectConstructor and RwPluginObjectDestructor?
unsigned int RwEngineGetNumSubSystems(void);
RwSubSystemInfo* RwEngineGetSubSystemInfo(RwSubSystemInfo* subSystem, int);
int RwEngineGetCurrentSubSystem(void);
int RwEngineSetSubSystem(int);
unsigned int RwEngineGetNumVideoModes(void);
RwVideoMode* RwEngineGetVideoModeInfo(RwVideoMode* videoMode, int);
int RwEngineGetCurrentVideoMode(void);
int RwEngineSetVideoMode(int);
int RwEngineGetTextureMemorySize(void);
int RwEngineGetMaxTextureSize(void);
int RwEngineStop(void);
int RwEngineStart(void);
int RwEngineClose(void);
int RwEngineOpen(void*);

#ifdef __cplusplus
}
#endif

#endif
