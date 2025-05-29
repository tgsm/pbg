#ifndef ENGINE_WRAP_DKW_GCNENGINE_H
#define ENGINE_WRAP_DKW_GCNENGINE_H

#include "engine/wrap/DKW_Engine.h"
#include "engine/wrap/DKW_V2d.h"

class DKW_PadButtonStatus;

class CDKW_GCNEngine : public CDKW_Engine {
private:
    u8 m_unk10[8];
    s64 m_saved_time;

    static s64 ms_InitialTime;
public:
    virtual s32 Timer();
    virtual f32 GetTimer();
    virtual f32 GetDeltaTimer();
    virtual void SaveTimer();
    virtual f32 GetDeltaTimerSinceSaved();
    virtual BOOL PS_Init();
    virtual MemoryFunctionsStruct* MemorySetting();
    virtual void InstallDebugMessage();
    virtual void InstallFileSystem();
    virtual void OnPadButtonDown(DKW_PadButtonStatus*);
    virtual void OnPadButtonUp(DKW_PadButtonStatus*);
    virtual void OnPadAnalogRight(CDKW_V2d*);
    virtual void OnPadAnalogLeft(CDKW_V2d*);
    
    void PreInit(RwEngineOpenParams*);
    void EngineInit(int argc, char** argv);
    void GCNInit();
    s64 OSGetTime();
};

#endif
