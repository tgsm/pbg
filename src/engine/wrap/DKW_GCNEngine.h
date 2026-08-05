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
    CDKW_GCNEngine(RwEngineOpenParams* params, U32 a1) : CDKW_Engine(params, a1) {}

    virtual U32 Timer();
    virtual F64 GetTimer();
    virtual F64 GetDeltaTimer();
    virtual void SaveTimer();
    virtual F64 GetDeltaTimerSinceSaved();
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

    static S64 OSGetTime();
    static void GCNInit();
};

#endif
