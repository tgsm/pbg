#ifndef RWSDK_BADEVICE_H
#define RWSDK_BADEVICE_H

#include <stdarg.h>
#include <rwsdk/plcore/bafsys.h>

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

typedef struct RwSubSystem {
    char unk0[0x50];
} RwSubSystem;

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

typedef struct RwMemoryFunctions {
    void* (*rwmalloc)(unsigned long size); // 0x130
    void (*rwfree)(void* ptr);
    void* (*rwrealloc)(void* ptr, unsigned long size);
    void* (*rwcalloc)(unsigned long n, unsigned long size);
} RwMemoryFunctions; // size: 0x10

typedef struct RwGlobals {
    char unk0[0xC4];
    RwFileFunctions fileFuncs;
    RwStringFunctions stringFuncs;
    RwMemoryFunctions memoryFuncs;
    void* (*unk140)(void*);
    void* (*unk144)(void*, void*);
    char unk148[0x154 - 0x148];
} RwGlobals; // size: 0x154

extern RwGlobals* RwEngineInstance;

unsigned int RwEngineGetVersion(void);
unsigned int RwEngineGetNumSubSystems(void);
RwSubSystem* RwEngineGetSubSystemInfo(RwSubSystem* subsystem, int);
int RwEngineGetCurrentSubSystem(void);
int RwEngineSetSubSystem(int);
unsigned int RwEngineGetNumVideoModes(void);
RwVideoMode* RwEngineGetVideoModeInfo(RwVideoMode* videoMode, int);
int RwEngineGetCurrentVideoMode(void);
int RwEngineSetVideoMode(int);
unsigned int RwEngineGetTextureMemorySize(void);
unsigned int RwEngineGetMaxTextureSize(void);

#ifdef __cplusplus
}
#endif

#endif
