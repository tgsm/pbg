#ifndef ENGINE_WRAP_DKW_ENGINE_H
#define ENGINE_WRAP_DKW_ENGINE_H

#include "types.h"

// FIXME: Put this in badevice?
struct RwEngineOpenParams {

};

extern "C" {
// TODO: Used by RenderWare
struct MemoryFunctionsStruct {
    void* (*alloc)(size_t);
    void (*free)(void*);
    void* (*realloc)(void*, size_t);
    void* (*calloc)(size_t, size_t);
};
extern void DolphinInitMemorySystem(MemoryFunctionsStruct*);

struct RwRect;
}

class CDKW_Engine {
private:
    u8 m_unk0[0xC];

    static s64 ms_InitialTime;
public:
    virtual s32 Timer();
    virtual f32 GetTimer();
    virtual f32 GetDeltaTimer();
    virtual void SaveTimer();
    virtual f32 GetDeltaTimerSinceSaved();
    virtual BOOL PS_Init();
    virtual MemoryFunctionsStruct* MemorySetting();
    virtual void* DeviceSetting();
    virtual void InstallDebugMessage();
    virtual BOOL NativeTextureSupport();
    virtual void InstallFileSystem();
    virtual BOOL DisplayVideo(char*);
    virtual BOOL OnPlugAttach();
    virtual BOOL OnResize(RwRect*);
    virtual BOOL OnInit(int argc, char** argv);
    virtual BOOL OnIdle();
    virtual BOOL OnTerm();

    void PreInit(RwEngineOpenParams*);
    void EngineInit(int argc, char** argv);
    void GCNInit();
    s64 OSGetTime();
};

#endif
