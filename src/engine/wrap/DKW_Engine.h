#ifndef ENGINE_WRAP_DKW_ENGINE_H
#define ENGINE_WRAP_DKW_ENGINE_H

#include <rwsdk/badevice.h>
#include "engine/wrap/DKW_Camera.h"
#include "engine/wrap/DKW_RGBA.h"

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
    DKW_EVENT_RESIZE = 0,
    DKW_EVENT_18 = 18, // update?
    DKW_EVENT_21 = 21,
    DKW_EVENT_PLAY_VIDEO = 30,
};

// Unknown name
struct DKW_DeviceSetting {
    U32 video_mode;
    U32 sub_system;
};

class CDKW_Charset;

class CDKW_Engine {
public:
    void* m_DisplayID;
    BOOL m_unk4; // initialized?
    U32 m_ArenaSize;

    static S64 ms_InitialTime;
public:
    CDKW_Engine(RwEngineOpenParams* params, U32 arena_size);
    ~CDKW_Engine();

    virtual U32 Timer();
    virtual F64 GetTimer();
    virtual F64 GetDeltaTimer();
    virtual void SaveTimer();
    virtual F64 GetDeltaTimerSinceSaved();
    virtual BOOL PS_Init();
    virtual RwMemoryFunctions* MemorySetting();
    virtual DKW_DeviceSetting DeviceSetting();
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
    BOOL EngineInit(int argc, char** argv);
    BOOL PreInit(RwEngineOpenParams* params);
    BOOL PostInit();
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

    static RwVideoMode GetVideoMode();

    static CDKW_RGBA GetForegroundColor() {
        return ms_ForegroundColor;
    }
    static CDKW_RGBA GetBackgroundColor() {
        return ms_BackgroundColor;
    }

    static BOOL EventHandler(DKW_Event event, void* a1);

    static RwVideoMode ms_VideoMode;
    static CDKW_Engine* ms_CurrentEngine;
    static U32 ms_FrameCounter;
    static U32 ms_FramesPerSecond;
    static F32 ms_DeltaTime;
    static F32 ms_Time;
    static CDKW_Charset* ms_pCharset;
    static CDKW_RGBA ms_ForegroundColor;
    static CDKW_RGBA ms_BackgroundColor;
    static BOOL m_AllowEscape;
    static CDKW_Camera* ms_pCurrentCamera;
    static BOOL ms_bAllowBufferization;
    static BOOL m_AllowPreclip;
    static BOOL ms_OrthoNormalizeCamera;
    static BOOL ms_bAllowCharset;
};

#endif
