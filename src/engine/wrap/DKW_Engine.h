#ifndef ENGINE_WRAP_DKW_ENGINE_H
#define ENGINE_WRAP_DKW_ENGINE_H

#include <rwsdk/badevice.h>
#include "types.h"

extern "C" {
extern void DolphinInitMemorySystem(RwMemoryFunctions*);
struct RwRect;
}

class CDKW_Engine {
private:
    U8 m_unk0[0xC];

    static S64 ms_InitialTime;
public:
    virtual S32 Timer();
    virtual F32 GetTimer();
    virtual F32 GetDeltaTimer();
    virtual void SaveTimer();
    virtual F32 GetDeltaTimerSinceSaved();
    virtual BOOL PS_Init();
    virtual RwMemoryFunctions* MemorySetting();
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
    S64 OSGetTime();
};

#endif
