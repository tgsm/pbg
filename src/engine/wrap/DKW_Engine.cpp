#include "engine/wrap/DKW_Charset.h"
#include "engine/wrap/DKW_Engine.h"
#include "engine/wrap/DKW_ImageTGA.h"

RwVideoMode CDKW_Engine::ms_VideoMode;

CDKW_Engine* CDKW_Engine::ms_CurrentEngine = NULL;
U32 CDKW_Engine::ms_FrameCounter = 0;
U32 CDKW_Engine::ms_FramesPerSecond = 0;
F32 CDKW_Engine::ms_DeltaTime = 0.0f;
F32 CDKW_Engine::ms_Time = 0.0f;
CDKW_Charset* CDKW_Engine::ms_pCharset = NULL;
CDKW_RGBA CDKW_Engine::ms_ForegroundColor(0xC8, 0xC8, 0xC8, 0xFF);
CDKW_RGBA CDKW_Engine::ms_BackgroundColor(0x40, 0x40, 0x40, 0x00);
BOOL CDKW_Engine::m_AllowEscape = FALSE;
CDKW_Camera* CDKW_Engine::ms_pCurrentCamera = NULL;

BOOL CDKW_Engine::ms_bAllowBufferization = TRUE;
BOOL CDKW_Engine::m_AllowPreclip = TRUE;
BOOL CDKW_Engine::ms_OrthoNormalizeCamera = TRUE;
BOOL CDKW_Engine::ms_bAllowCharset = TRUE;

extern BOOL AllPluginAttach();

static void ResizeAllCamera(void* camera_ptr, void* rect_ptr) {
    CDKW_Camera* camera = (CDKW_Camera*)camera_ptr;
    RwRect* rect = (RwRect*)rect_ptr;

    camera->Resize(rect);
}

BOOL CDKW_Engine::PS_Init() {
    return TRUE;
}

RwMemoryFunctions* CDKW_Engine::MemorySetting() {
    return NULL;
}

void CDKW_Engine::InstallFileSystem() {

}

BOOL CDKW_Engine::DisplayVideo(char*) {
    return TRUE;
}

void CDKW_Engine::InstallDebugMessage() {

}

DKW_DeviceSetting CDKW_Engine::DeviceSetting() {
    DKW_DeviceSetting setting;
    setting.sub_system = 0;
    setting.video_mode = 0;
    return setting;
}

BOOL CDKW_Engine::NativeTextureSupport() {
    return TRUE;
}

BOOL CDKW_Engine::OnPlugAttach() {
    return TRUE;
}

BOOL CDKW_Engine::OnResize(RwRect*) {
    return TRUE;
}

BOOL CDKW_Engine::OnIdle() {
    return TRUE;
}

BOOL CDKW_Engine::OnTerm() {
    return TRUE;
}

BOOL CDKW_Engine::OnInit(int, char**) {
    return TRUE;
}

U32 CDKW_Engine::Timer() {
    return 0;
}

F64 CDKW_Engine::GetTimer() {
    return 0.0;
}

F64 CDKW_Engine::GetDeltaTimer() {
    return 0.0;
}

void CDKW_Engine::SaveTimer() {

}

F64 CDKW_Engine::GetDeltaTimerSinceSaved() {
    return 0.0;
}

CDKW_Engine::CDKW_Engine(RwEngineOpenParams* params, U32 arena_size) {
    m_DisplayID = params->displayID;
    m_ArenaSize = arena_size;
    m_unk4 = FALSE;
    m_AllowEscape = FALSE;
    ms_CurrentEngine = this;
}

CDKW_Engine::~CDKW_Engine() {
    ms_CurrentEngine = NULL;
}

RwVideoMode CDKW_Engine::GetVideoMode() {
    RwEngineGetVideoModeInfo(&CDKW_Engine::ms_VideoMode, RwEngineGetCurrentVideoMode());
    return CDKW_Engine::ms_VideoMode;
}

BOOL CDKW_Engine::EventHandler(DKW_Event event, void *data) {
    if (event == DKW_EVENT_21) {
        if (m_AllowEscape == TRUE) {
            ms_CurrentEngine->m_unk4 = FALSE;
        }
        return TRUE;
    }

    switch (event) {
        case DKW_EVENT_PLAY_VIDEO: {
            char* filename = (char*)data;
            if (ms_CurrentEngine->DisplayVideo(filename)) {
                return TRUE;
            }
            return TRUE;
        }

        case DKW_EVENT_RESIZE: {
            RwRect* rect = (RwRect*)data;

            if (!ms_CurrentEngine->OnResize(rect)) {
                return FALSE;
            }

            if (ms_CurrentEngine->m_unk4) {
                RwFreeListForAllUsed(CDKW_Camera::ms_pMemEntry, ResizeAllCamera, rect);
            }

            return TRUE;
        }

        case DKW_EVENT_18: {
            static BOOL firstCall = TRUE;
            static U32 lastFrameTime;
            static U32 lastAnimTime;

            if (firstCall) {
                lastFrameTime = ms_CurrentEngine->Timer();
                lastAnimTime = lastFrameTime;
                firstCall = FALSE;
            }

            U32 currentTime = ms_CurrentEngine->Timer();
            if (currentTime >= lastFrameTime + 1000) {
                ms_FramesPerSecond = ms_FrameCounter;
                ms_FrameCounter = 0;
                lastFrameTime += 1000;
                if (currentTime >= lastFrameTime + 1000) {
                    lastFrameTime = currentTime;
                }
            }

            ms_DeltaTime = (currentTime - lastAnimTime) * 0.001f;
            ms_Time += ms_DeltaTime;
            lastAnimTime = currentTime;

            return ms_CurrentEngine->OnIdle() != FALSE;
        }

        default:
            break;
    }

    return TRUE;
}

extern "C" {
extern RwImage* RtBMPImageRead(const char* filename);
extern RwImage* RtBMPImageWrite(RwImage* image, const char* filename);
extern RwImage* RtPNGImageRead(const char* filename);
extern RwImage* RtPNGImageWrite(RwImage* image, const char* filename);
extern RwBool RwImageRegisterImageFormat(const char* extenstion, RwImageCallBackRead readCB, RwImageCallBackWrite writeCB);
}

BOOL CDKW_Engine::EngineInit(int argc, char** argv) {
    m_ArenaSize = 0x90000;
    if (!RwEngineInit(MemorySetting(), rwENGINEINITFREELISTS, m_ArenaSize)) {
        return FALSE;
    }

    InstallFileSystem();
    InstallDebugMessage();

    if (!AllPluginAttach() || !OnPlugAttach()) {
        return FALSE;
    }

    if (!RwEngineOpen(this)) {
        RwEngineTerm();
        return FALSE;
    }

    DKW_DeviceSetting device_setting = DeviceSetting();
    if (!RwEngineSetSubSystem(device_setting.sub_system) || !RwEngineSetVideoMode(device_setting.video_mode)) {
        RwEngineClose();
        RwEngineTerm();
        return FALSE;
    }

    if (!RwEngineStart()) {
        RwEngineClose();
        RwEngineTerm();
        return FALSE;
    }

    GetVideoMode();

    if (!RwImageRegisterImageFormat("bmp", RtBMPImageRead, RtBMPImageWrite)) {
        return FALSE;
    }
    if (!RwImageRegisterImageFormat("png", RtPNGImageRead, RtPNGImageWrite)) {
        return FALSE;
    }
    // Should I just make the callbacks return a RwImage* instead of a CDKW_Image*?
    if (!RwImageRegisterImageFormat("tga", (RwImageCallBackRead)CDKW_ImageTGA::Read, (RwImageCallBackWrite)CDKW_ImageTGA::Write)) {
        return FALSE;
    }

    if (!NativeTextureSupport()) {
        return FALSE;
    }

    if (ms_bAllowCharset == TRUE && !RtCharsetOpen()) {
        RwEngineStop();
        RwEngineClose();
        RwEngineTerm();
        return FALSE;
    }

    if (!PS_Init()) {
        RwEngineStop();
        RwEngineClose();
        RwEngineTerm();
        return FALSE;
    }

    if (!OnInit(argc, argv)) {
        RwEngineStop();
        RwEngineClose();
        RwEngineTerm();
        return FALSE;
    }

    if (ms_bAllowCharset == TRUE) {
        ms_pCharset = new CDKW_Charset;
        if (ms_pCharset->m_rw_charset == NULL) {
            RwEngineStop();
            RwEngineClose();
            RwEngineTerm();
            return FALSE;
        }
    }

    m_unk4 = TRUE;

    return TRUE;
}

BOOL CDKW_Engine::PreInit(RwEngineOpenParams* params) {
    m_ArenaSize = 0x90000;
    if (!RwEngineInit(MemorySetting(), rwENGINEINITFREELISTS, m_ArenaSize)) {
        return FALSE;
    }

    InstallFileSystem();
    InstallDebugMessage();

    if (!AllPluginAttach() || !OnPlugAttach()) {
        return FALSE;
    }

    if (!RwEngineOpen(this)) {
        RwEngineTerm();
        return FALSE;
    }

    return TRUE;
}

BOOL CDKW_Engine::PostInit() {
    if (!RwEngineStart()) {
        RwEngineClose();
        RwEngineTerm();
        return FALSE;
    }

    GetVideoMode();

    if (!RwImageRegisterImageFormat("bmp", RtBMPImageRead, RtBMPImageWrite)) {
        return FALSE;
    }
    if (!RwImageRegisterImageFormat("tga", (RwImageCallBackRead)CDKW_ImageTGA::Read, (RwImageCallBackWrite)CDKW_ImageTGA::Write)) {
        return FALSE;
    }
    if (!RwImageRegisterImageFormat("png", RtPNGImageRead, RtPNGImageWrite)) {
        return FALSE;
    }

    if (!NativeTextureSupport()) {
        return FALSE;
    }

    if (ms_bAllowCharset == TRUE && !RtCharsetOpen()) {
        RwEngineStop();
        RwEngineClose();
        RwEngineTerm();
        return FALSE;
    }

    if (!PS_Init()) {
        RwEngineStop();
        RwEngineClose();
        RwEngineTerm();
        return FALSE;
    }

    if (!OnInit(0, NULL)) {
        RwEngineStop();
        RwEngineClose();
        RwEngineTerm();
        return FALSE;
    }

    if (ms_bAllowCharset == TRUE) {
        ms_pCharset = new CDKW_Charset;
        if (ms_pCharset->m_rw_charset == NULL) {
            RwEngineStop();
            RwEngineClose();
            RwEngineTerm();
            return FALSE;
        }
    }

    m_unk4 = TRUE;

    return TRUE;
}
