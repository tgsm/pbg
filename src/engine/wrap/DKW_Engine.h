#ifndef ENGINE_WRAP_DKW_ENGINE_H
#define ENGINE_WRAP_DKW_ENGINE_H

#include <rwsdk/badevice.h>
#include "types.h"

extern "C" {
extern void DolphinInitMemorySystem(RwMemoryFunctions*);
struct RwRect {
    int x;
    int y;
    int w;
    int h;
};
}

enum DKW_Event {
    DKW_EVENT_18 = 18,
};

class CDKW_Engine {
private:
    U8 m_unk0[0xC];

    static S64 ms_InitialTime;
public:
    CDKW_Engine(RwEngineOpenParams* params, U32);
    ~CDKW_Engine();

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

    static BOOL EventHandler(DKW_Event event, void* a1);
};

#endif
