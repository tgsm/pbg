#ifndef RWSDK_BADEVICE_H
#define RWSDK_BADEVICE_H

typedef struct RwGlobals {
    char unk0[0x134];
    void (*unk134)(void* ptr);
    void* (*unk138)(void* ptr, unsigned int);
    char unk13C[0x154 - 0x13C];
} RwGlobals; // size: 0x154

extern RwGlobals* RwEngineInstance;

unsigned int RwEngineGetVersion(void);

#endif
