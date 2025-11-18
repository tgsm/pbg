#ifndef ENGINE_WRAP_DKW_GCNENGINE_H
#define ENGINE_WRAP_DKW_GCNENGINE_H

#include "engine/wrap/DKW_Engine.h"
#include "engine/wrap/DKW_V2d.h"

class DKW_PadButtonStatus;

class CDKW_GCNEngine : public CDKW_Engine {
private:
    U8 m_unk10[8];
    S64 m_saved_time;

    static S64 ms_InitialTime;
public:
    virtual S32 Timer();
    virtual F32 GetTimer();
    virtual F32 GetDeltaTimer();
    virtual void SaveTimer();
    virtual F32 GetDeltaTimerSinceSaved();
    virtual BOOL PS_Init();
    virtual RwMemoryFunctions* MemorySetting();
    virtual void InstallDebugMessage();
    virtual void InstallFileSystem();
    virtual void OnPadButtonDown(DKW_PadButtonStatus*);
    virtual void OnPadButtonUp(DKW_PadButtonStatus*);
    virtual void OnPadAnalogRight(CDKW_V2d*);
    virtual void OnPadAnalogLeft(CDKW_V2d*);
    
    void PreInit(RwEngineOpenParams*);
    void EngineInit(int argc, char** argv);
    void GCNInit();

    static S64 OSGetTime();
};

#endif
