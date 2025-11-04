#ifndef RWSDK_BADEVICE_H
#define RWSDK_BADEVICE_H

#include <stdarg.h>

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
    char unk0[0xF0];
    RwStringFunctions stringFuncs;
    RwMemoryFunctions memoryFuncs;
    char unk140[0x154 - 0x140];
} RwGlobals; // size: 0x154

extern RwGlobals* RwEngineInstance;

unsigned int RwEngineGetVersion(void);

#endif
