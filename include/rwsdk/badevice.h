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
    RwInt32 unk8;
    RwInt32 unkC;
} RwEngineOpenParams;

typedef struct RwSubSystemInfo {
    RwChar name[0x50];
} RwSubSystemInfo;

typedef enum RwVideoModeFlag {
    rwVIDEOMODEEXCLUSIVE = (1 << 0),
    rwVIDEOMODEFLAGFORCEENUMSIZEINT = 0x7FFFFFFF,
} RwVideoModeFlag;

typedef struct RwVideoMode {
    RwUInt32 width;
    RwUInt32 height;
    RwUInt32 depth;
    RwVideoModeFlag flags;
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

// Unofficial macros?
#define RwSprintf(str, format, ...) RwEngineInstance->stringFuncs.rwsprintf(str, format, ##__VA_ARGS__)
#define RwVsprintf(str, format, ap) RwEngineInstance->stringFuncs.rwvsprintf(str, format, ap)
#define RwStrcpy(dst, src) RwEngineInstance->stringFuncs.rwstrcpy(dst, src)
#define RwStrncpy(dst, src, len) RwEngineInstance->stringFuncs.rwstrncpy(dst, src, len)
#define RwStrcat(dst, src) RwEngineInstance->stringFuncs.rwstrcat(dst, src)
#define RwStrncat(dst, src, len) RwEngineInstance->stringFuncs.rwstrncat(dst, src, len)
#define RwStrrchr(str, c) RwEngineInstance->stringFuncs.rwstrrchr(str, c)
#define RwStrchr(str, c) RwEngineInstance->stringFuncs.rwstrchr(str, c)
#define RwStrstr(haystack, needle) RwEngineInstance->stringFuncs.rwstrstr(haystack, needle)
#define RwStrcmp(a, b) RwEngineInstance->stringFuncs.rwstrcmp(a, b)
#define RwStricmp(a, b) RwEngineInstance->stringFuncs.rwstricmp(a, b)
#define RwStrlen(str) RwEngineInstance->stringFuncs.rwstrlen(str)
#define RwStrupr(str) RwEngineInstance->stringFuncs.rwstrupr(str)
#define RwStrlwr(str) RwEngineInstance->stringFuncs.rwstrlwr(str)
#define RwStrtok(str, delim) RwEngineInstance->stringFuncs.rwstrtok(str, delim)
#define RwSscanf(str, format, ...) RwEngineInstance->stringFuncs.rwsscanf(str, format, __VA_ARGS__)

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
    RwReal x;
    RwReal y;
    RwReal z;
    RwUInt8 r;
    RwUInt8 g;
    RwUInt8 b;
    RwUInt8 a;
    RwReal u;
    RwReal v;
} RwIm2DVertex;

typedef struct RwDevice {
    RwReal gammaCorrection;
    RwBool (*fpSystem)(RwInt32, void*, void*, RwInt32);
    RwReal zBufferNear;
    RwReal zBufferFar;
    RwBool (*fpRenderStateSet)(RwRenderState, void*);
    RwBool (*fpRenderStateGet)(RwRenderState, void*);
    RwBool (*fpIm2DRenderLine)(RwIm2DVertex*, RwInt32, RwInt32, RwInt32);
    RwBool (*fpIm2DRenderTriangle)(RwIm2DVertex*, RwInt32, RwInt32, RwInt32, RwInt32);
    RwBool (*fpIm2DRenderPrimitive)(RwPrimitiveType, RwIm2DVertex*, RwInt32);
    RwBool (*fpIm2DRenderIndexedPrimitive)(RwPrimitiveType, RwIm2DVertex*, RwInt32, RwUInt16*, RwInt32);
    RwBool (*fpIm3DRenderLine)(RwInt32, RwInt32);
    RwBool (*fpIm3DRenderTriangle)(RwInt32, RwInt32, RwInt32);
    RwBool (*fpIm3DRenderPrimitive)(RwPrimitiveType);
    RwBool (*fpIm3DRenderIndexedPrimitive)(RwPrimitiveType, RwUInt16*, RwInt32);
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
    rwDEVICESYSTEMFOCUS = 8,
    rwDEVICESYSTEMINITPIPELINE = 9,
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

typedef RwBool (*RwStandardFunc)(void*, void*, RwInt32);

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
    RwUInt8 unk4[0x6];
    RwUInt16 lightFrame;
    RwUInt16 pad[2];
    RwDevice dOpenDevice;
    RwStandardFunc stdFunc[29];
    RwLinkList dirtyFrameListMaybe;
    RwFileFunctions fileFuncs;
    RwStringFunctions stringFuncs;
    RwMemoryFunctions memoryFuncs;
    void* (*memoryAlloc)(RwFreeList*);
    RwFreeList* (*memoryFree)(RwFreeList*, void*);
    RwUInt8 unk148[4]; // likely RwMetrics*, unused in PBG
    RwEngineStatus engineStatus;
    RwUInt32 resArenaInitSize;
} RwGlobals; // size: 0x154

typedef struct RwModuleInfo {
    RwInt32 globalsOffset;
    RwInt32 numInstances;
} RwModuleInfo;

// FIXME: DWARFs say this is a void*
extern RwGlobals* RwEngineInstance;

RwBool _rwDeviceSystemRequest(RwDevice* device, RwInt32 systemFn, void* dest, void*, RwInt32);
RwUInt32 _rwGetNumEngineInstances(void);
RwUInt32 RwEngineGetVersion(void);
RwInt32 RwEngineRegisterPlugin(RwInt32, RwInt32 pluginID, RwPluginObjectConstructor constructCB, RwPluginObjectDestructor destructCB);
RwUInt32 RwEngineGetNumSubSystems(void);
RwSubSystemInfo* RwEngineGetSubSystemInfo(RwSubSystemInfo* subSystem, RwInt32);
RwInt32 RwEngineGetCurrentSubSystem(void);
RwBool RwEngineSetSubSystem(RwInt32);
RwUInt32 RwEngineGetNumVideoModes(void);
RwVideoMode* RwEngineGetVideoModeInfo(RwVideoMode* videoMode, RwInt32);
RwInt32 RwEngineGetCurrentVideoMode(void);
RwBool RwEngineSetVideoMode(RwInt32);
RwInt32 RwEngineGetTextureMemorySize(void);
RwInt32 RwEngineGetMaxTextureSize(void);
RwBool RwEngineStop(void);
RwBool RwEngineStart(void);
RwBool RwEngineClose(void);
RwBool RwEngineOpen(void*);

#ifdef __cplusplus
}
#endif

#endif
