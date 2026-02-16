#ifndef ENGINE_WRAP_DKW_ENGINE_H
#define ENGINE_WRAP_DKW_ENGINE_H

#include <rwsdk/badevice.h>
#include "engine/wrap/DKW_Camera.h"
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

class CDKW_Charset;

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

    void Stop();
    void EngineInit(int argc, char** argv);
    void PreInit(RwEngineOpenParams*);
    void PostInit();
    S64 OSGetTime();

    static U32 GetFPS() {
        return ms_FramesPerSecond;
    }

    static CDKW_Charset* GetCharset() {
        DONT_INLINE_HACK();
        return ms_pCharset;
    }

    static void SetCurrentCamera(CDKW_Camera* camera) {
        ms_pCurrentCamera = camera;
    }

    static int GetCurrentVideoMode() {
        return RwEngineGetCurrentVideoMode();
    }

    static void GetVideoModeInfo(RwVideoMode* video_mode, int a2) {
        DONT_INLINE_HACK();
        RwEngineGetVideoModeInfo(video_mode, a2);
    }

    static BOOL EventHandler(DKW_Event event, void* a1);

    static U32 ms_FrameCounter;
    static U32 ms_FramesPerSecond;
    static F32 ms_DeltaTime;
    static CDKW_Charset* ms_pCharset;
    static BOOL ms_bAllowCharset;
    static BOOL m_AllowEscape;
    static CDKW_Camera* ms_pCurrentCamera;
    static BOOL ms_bAllowBufferization;
    static BOOL m_AllowPreclip;
    static BOOL ms_OrthoNormalizeCamera;
};

#endif
