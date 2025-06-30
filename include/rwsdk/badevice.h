#ifndef RWSDK_BADEVICE_H
#define RWSDK_BADEVICE_H

typedef struct RwGlobals {
    char unk0[0x154];
} RwGlobals; // size: 0x154

extern RwGlobals* RwEngineInstance;

unsigned int RwEngineGetVersion(void);

#endif
