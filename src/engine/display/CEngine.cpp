#include <dolphin/gx.h>
#include <dolphin/vi.h>
#include "engine/display/CEngine.h"

#include "engine/wrap/DKW_Camera.h" // FIXME: For CDKW_AlphaSortedAtomicList. Remove

namespace DKDSP {

IWarningCollector* CEngine::ms_pWarningCollector;
IInitCallback* CEngine::ms_pInitCallback;
IShadowMapValidationCallback* CEngine::ms_pShadowMapValidationCallback;
CTextureDictionary* CEngine::ms_pGlobalTextureDictionary;
CEngine* CEngine::ms_pEngine;
CScene* CEngine::ms_pCurrentRenderingScene;
U32 CEngine::ms_nUniquenessCounter;

// Equivalent: std::vector
CEngine::~CEngine() {
    m_charset.SetDkWrapCharset(NULL, 0);
    Release();
}

void CEngine::EnableADLPipelines() {
    m_adl_enabled = TRUE;
}

BOOL CEngine::IsADLEnabled() {
    return m_adl_enabled;
}

void CEngine::SetADLDirectionalLight(ILight* light) {
    m_adl_directional_light = (CLight*)light;
}

void* CEngine::Init(void* a1, void* a2, U32 screen_width, U32 screen_height) {
    RwEngineOpenParams params;
    if (m_gcn_video_mode != NULL) {
        params.unk4 = m_gcn_video_mode;
    } else if (VIGetTvFormat() == VI_NTSC || VIGetTvFormat() == VI_EURGB60) {
        params.unk4 = &GXNtsc480IntDf;
    } else if (VIGetTvFormat() == VI_PAL) {
        params.unk4 = &GXPal528IntDf;
    } else if (VIGetTvFormat() == VI_MPAL) {
        params.unk4 = &GXMpal480IntDf;
    }
    params.unk8 = 0;
    params.unkC = 0x40000;
    params.displayID = &params.unk4;

    m_wrap_engine = new CDkWrapEngine(&params, a1);
    m_wrap_engine->m_unk48 = m_adl_enabled;

    m_wrap_engine->EngineInit(0, NULL);

    m_charset.SetDkWrapCharset(CDKW_Engine::GetCharset(), 0);
    m_texture_dictionary.m_engine = this;
    m_anim_dictionary.m_engine = this;
    m_object_dictionary.m_engine = this;
    m_texture_dictionary.Init();
    m_shadow_map_pipeline.Create();
    m_material_animation_pipeline.Create();
    m_displacement_pipeline.Create();
    ms_pCurrentRenderingScene = NULL;

    RwTextureSetMipmapping(TRUE);
    RwTextureSetAutoMipmapping(TRUE);

    m_window_handle = NULL;
    return NULL;
}

void CEngine::SetGCNVideoMode(void* video_mode) {
    m_gcn_video_mode = video_mode;
}

void* CEngine::Open(void* a1, void*, U32 screen_width, U32 screen_height) {
    RwEngineOpenParams params;
    if (m_gcn_video_mode != NULL) {
        params.unk4 = m_gcn_video_mode;
    } else if (VIGetTvFormat() == VI_NTSC || VIGetTvFormat() == VI_EURGB60) {
        params.unk4 = &GXNtsc480IntDf;
    } else if (VIGetTvFormat() == VI_PAL) {
        params.unk4 = &GXPal528IntDf;
    } else if (VIGetTvFormat() == VI_MPAL) {
        params.unk4 = &GXMpal480IntDf;
    }
    params.unk8 = 0;
    params.unkC = 0x40000;
    params.displayID = &params.unk4;

    m_wrap_engine = new CDkWrapEngine(&params, a1);
    m_wrap_engine->m_unk48 = m_adl_enabled;
    m_wrap_engine->PreInit(&params);

    m_window_handle = NULL;
    return NULL;
}

void CEngine::Setup(int a1, int a2) {
    if (m_wrap_engine != NULL) {
        RwEngineSetSubSystem(a1);
        RwEngineSetVideoMode(a2);
    }
}

void CEngine::Start() {
    if (m_wrap_engine != NULL) {
        m_wrap_engine->PostInit();
        m_charset.SetDkWrapCharset(CDKW_Engine::GetCharset(), 0);
        m_texture_dictionary.m_engine = this;
        m_anim_dictionary.m_engine = this;
        m_object_dictionary.m_engine = this;
        m_texture_dictionary.Init();
        m_shadow_map_pipeline.Create();
        m_material_animation_pipeline.Create();
        m_displacement_pipeline.Create();
        ms_pCurrentRenderingScene = NULL;

        RwTextureSetMipmapping(TRUE);
        RwTextureSetAutoMipmapping(TRUE);
    }
}

void* CEngine::GetWindowHandle() {
    return m_window_handle;
}

BOOL CEngine::Update() {
    ResetDMACheck();

    if (m_wrap_engine != NULL) {
        return m_wrap_engine->Update();
    }

    return FALSE;
}

// FIXME: Get rid of this
#define CLEAR_VECTOR(vec) \
    (vec).m_size = 0; \
    if ((vec).m_data != NULL) { \
        delete (vec).m_data; \
    } \
    (vec).m_data = NULL; \
    (vec).m_capacity = 0


void CEngine::Release() {
    int i;

    m_adl_enabled = FALSE;
    m_adl_directional_light = NULL;

    for (i = 0; i < (int)m_scenes.size(); i++) {
        if (m_scenes[i] != NULL) {
            m_scenes[i]->Release();
            delete m_scenes[i];
        }
        m_scenes[i] = NULL;
    }
    CLEAR_VECTOR(m_scenes);

    for (i = 0; i < (int)m_timers.size(); i++) {
        if (m_timers[i] != NULL) {
            delete m_timers[i];
        }
        m_timers[i] = NULL;
    }
    CLEAR_VECTOR(m_timers);

    m_event_animation_callback_entries.clear();

    m_object_dictionary.Release();
    m_anim_dictionary.Release();
    m_texture_dictionary.Release();
    m_displacement_pipeline.Release();
    m_material_animation_pipeline.Release();
    m_shadow_map_pipeline.Release();
    m_immediate.Release();

    if (m_wrap_engine != NULL) {
        m_wrap_engine->Stop();
        delete m_wrap_engine;
    }
    m_wrap_engine = NULL;

    ms_pCurrentRenderingScene = NULL;
}

void CEngine::EnableDMACheck() {

}

void CEngine::ResetDMACheck() {

}

RwRect CEngine::GetRect() {
    if (m_wrap_engine != NULL) {
        return m_wrap_engine->GetRect();
    }

    RwRect rect;
    memset(&rect, 0, sizeof(RwRect));
    return rect;
}

CCharset* CEngine::GetCharset() {
    return &m_charset;
}

BOOL CEngine::IsRendering() {
    return m_rendering;
}

U32 CEngine::GetFPS() {
    if (m_wrap_engine == NULL) {
        return 0;
    }

    return m_wrap_engine->GetFPS();
}

F64 CEngine::GetTime() {
    if (m_wrap_engine == NULL) {
        return 0.0;
    }

    return m_wrap_engine->GetTimer();
}

void CEngine::AllowEscape() {
    if (m_wrap_engine != NULL) {
        CDKW_Engine::m_AllowEscape = TRUE;
    }
}

void CEngine::Screenshot(char* a1, IRaster* raster) {
    if (a1 == NULL) {
        return;
    }

    if (raster == NULL) {
        return;
    }

    CImage* image = m_texture_dictionary.CreateImage(a1, raster->GetWidth(), raster->GetHeight(), 32);
    if (image == NULL) {
        return;
    }

    image->AllocatePixels();
    image->SetFromRaster(raster);
    m_texture_dictionary.WriteImage(image, a1);
    m_texture_dictionary.RemoveImage(a1);
}

void CEngine::RegisterWarningCollector(IWarningCollector* collector) {
    ms_pWarningCollector = collector;
}

void CEngine::RegisterTextureCallback(ITextureCallback* callback) {
    m_texture_dictionary.RegisterTextureCallback(callback);
}

void CEngine::RegisterInitCallback(IInitCallback* callback) {
    ms_pInitCallback = callback;
}

void CEngine::RegisterShadowMapValidationCallback(IShadowMapValidationCallback* callback) {
    ms_pShadowMapValidationCallback = callback;
}

extern "C" {
extern unsigned int RwResourcesGetArenaSize(void);
extern unsigned int RwResourcesGetArenaUsage(void);
}

U32 CEngine::GetSize() {
    U32 size = m_anim_dictionary.GetSize() + m_texture_dictionary.GetSize() + m_object_dictionary.GetSize() + m_immediate.GetSize();

    for (int i = 0; i < (int)m_scenes.size(); i++) {
        if (m_scenes[i] != NULL) {
            size += m_scenes[i]->GetSize();
        }
    }

    size += RwResourcesGetArenaSize();
    size += (int)GetVectorMemory();

    return size;
}

U32 CEngine::GetResourceArenaUsed() {
    return RwResourcesGetArenaUsage();
}

U32 CEngine::GetVectorMemory() {
    return (m_scenes.m_capacity + m_timers.m_capacity + m_event_animation_callback_entries.m_capacity) * sizeof(unsigned long);
}

U32 CEngine::GetNumberOfAlphaAtomicBufferized() {
    return CDKW_AlphaSortedAtomicList::ms_nNumberOfEntries;
}

void CEngine::AlphaEnable() {

}

void CEngine::AlphaAtomicBufferization(BOOL allow) {
    CDKW_Engine::ms_bAllowBufferization = allow;
}

BOOL CEngine::IsAlphaAtomicBufferizationEnabled() {
    return CDKW_Engine::ms_bAllowBufferization;
}

void CEngine::EnableAtomicPreclip(BOOL enable) {
    CDKW_Engine::m_AllowPreclip = enable;
}

BOOL CEngine::IsAlphaPreclippingEnabled() {
    return CDKW_Engine::m_AllowPreclip;
}

void CEngine::EnableCameraOrthoNormalization(BOOL enable) {
    CDKW_Engine::ms_OrthoNormalizeCamera = enable;
}

BOOL CEngine::IsCameraOrthoNormalizationEnabled() {
    return CDKW_Engine::ms_OrthoNormalizeCamera;
}

void CEngine::EnableShowWindow(BOOL enable) {
    CDkWrapEngine::m_bEnableShowWindow = enable;
}

void CEngine::SetCurrentCamera(ICamera* camera_) {
    CCamera* camera = (CCamera*)camera_;
    if (m_wrap_engine != NULL) {
        if (camera != NULL) {
            m_wrap_engine->SetCurrentCamera(camera->m_wrap_camera);
        } else {
            m_wrap_engine->SetCurrentCamera(NULL);
        }

        m_unk238 = FALSE;
        if (camera != NULL && camera->GetProjection() != rwPERSPECTIVE) {
            m_unk238 = TRUE;
        }
    }
}

void CEngine::SetRendering(BOOL a1, CScene* scene) {
    m_rendering = a1;
    ms_pCurrentRenderingScene = scene;
}

CShadowMapPipelineGCN* CEngine::GetShadowMapPipeline() {
    return &m_shadow_map_pipeline;
}

CMaterialAnimationPipelineGCN* CEngine::GetMaterialAnimationPipeline() {
    return &m_material_animation_pipeline;
}

U32 CEngine::GetNumberOfAdapters() {
    if (m_wrap_engine != NULL) {
        return RwEngineGetNumSubSystems();
    }

    return 0;
}

void CEngine::SetAdapter(int adapter) {
    if (m_wrap_engine != NULL) {
        RwEngineSetSubSystem(adapter);
    }
}

int CEngine::GetCurrentAdapter() {
    if (m_wrap_engine != NULL) {
        return RwEngineGetCurrentSubSystem();
    }

    return 0;
}

void CEngine::GetAdapterInfo(int a1, SRWAdapterInfo* adapter_info) {
    if (m_wrap_engine != NULL && adapter_info != NULL) {
        RwSubSystem subsystem;
        RwEngineGetSubSystemInfo(&subsystem, a1);
        for (int i = 0; i < 0x50; i++) {
            adapter_info->subsystem.unk0[i] = subsystem.unk0[i];
        }
    }
}

U32 CEngine::GetNumberOfVideoModes() {
    if (m_wrap_engine != NULL) {
        return RwEngineGetNumVideoModes();
    }

    return 0;
}

void CEngine::SetVideoMode(int video_mode) {
    if (m_wrap_engine != NULL) {
        RwEngineSetSubSystem(video_mode);
    }
}

int CEngine::GetCurrentVideoMode() {
    if (m_wrap_engine != NULL) {
        return m_wrap_engine->GetCurrentVideoMode();
    }

    return 0;
}

void CEngine::GetVideoModeInfo(int a1, SRWVideoModeInfo* info) {
    if (m_wrap_engine != NULL && info != NULL) {
        RwVideoMode video_mode;
        CDKW_Engine::GetVideoModeInfo(&video_mode, a1);
        info->video_mode = video_mode;
    }
}

U32 CEngine::GetTextureMemorySize() {
    if (m_wrap_engine != NULL) {
        return RwEngineGetTextureMemorySize();
    }

    return 0;
}

U32 CEngine::GetMaxTextureSize() {
    if (m_wrap_engine != NULL) {
        return RwEngineGetMaxTextureSize();
    }

    return 0;
}

void CEngine::DisplayMetrics() {

}

void CEngine::DumpDictionaryInConsole() {

}

CScene* CEngine::CreateScene() {
    int index = -1;
    for (int i = 0; i < (int)m_scenes.size(); i++) {
        if (m_scenes[i] == NULL) {
            index = i;
            break;
        }
    }

    unsigned long scene = reinterpret_cast<unsigned long>(new CScene);
    if (scene == NULL) {
        return NULL;
    }

    reinterpret_cast<CScene*>(scene)->Initialize(&m_object_dictionary, this);
    if (index >= 0) {
        AS_ULONG_VECTOR_HACK(m_scenes)[index] = scene;
    } else {
        AS_ULONG_VECTOR_HACK(m_scenes).push_back(scene);
    }

    return reinterpret_cast<CScene*>(scene);
}

U32 CEngine::GetSceneIndex(IScene* scene) {
    if (scene == NULL) {
        return -1;
    }

    for (int i = 0; i < (int)m_scenes.size(); i++) {
        if (m_scenes[i] == scene) {
            return i;
        }
    }

    return -1;
}

CScene* CEngine::GetScene(int id) {
    if (id < 0 || id >= (int)m_scenes.size()) {
        return 0;
    }

    return m_scenes[id];
}

U32 CEngine::GetNumberOfScenes() {
    return m_scenes.size();
}

void CEngine::SetImagePath(char* path) {
    if (path != NULL) {
        RwImageSetPath(path);
    }
}

CRWStream* CEngine::OpenStream(RwStreamType type, RwStreamAccessType access_type, const void* a3) {
    CDKW_Stream* wrap_stream = (CDKW_Stream*)RwStreamOpen(type, access_type, (void*)a3);
    if (wrap_stream != NULL) {
        return OpenStreamInline(wrap_stream, type, access_type, (void*)a3);
    }

    return NULL;
}

CRWStream* CEngine::OpenStreamFile(RwStreamAccessType access_type, const std::FILE* file) {
    CDKW_Stream* wrap_stream = (CDKW_Stream*)RwStreamOpen(rwSTREAMFILE, access_type, (void*)file);
    if (wrap_stream != NULL) {
        return OpenStreamInline(wrap_stream, rwSTREAMFILE, access_type, (void*)file);
    }

    return NULL;
}

CRWStream* CEngine::OpenStreamFileName(RwStreamAccessType access_type, char* filename) {
    CDKW_Stream* wrap_stream = (CDKW_Stream*)RwStreamOpen(rwSTREAMFILENAME, access_type, (void*)filename);
    if (wrap_stream != NULL) {
        return OpenStreamInline(wrap_stream, rwSTREAMFILENAME, access_type, (void*)filename);
    }

    return NULL;
}

CRWStream* CEngine::OpenStreamMemory(RwStreamAccessType access_type, CDKW_Memory* memory) {
    CDKW_Stream* wrap_stream = (CDKW_Stream*)RwStreamOpen(rwSTREAMMEMORY, access_type, (void*)memory);
    if (wrap_stream != NULL) {
        return OpenStreamInline(wrap_stream, rwSTREAMMEMORY, access_type, (void*)memory);
    }

    return NULL;
}

CRWStream* CEngine::OpenStreamCustom(RwStreamAccessType access_type, const void* a2) {
    CDKW_Stream* wrap_stream = (CDKW_Stream*)RwStreamOpen(rwSTREAMCUSTOM, access_type, (void*)a2);
    if (wrap_stream != NULL) {
        return OpenStreamInline(wrap_stream, rwSTREAMCUSTOM, access_type, (void*)a2);
    }

    return NULL;
}

void CEngine::DestroyStream(IRWStream* stream_) {
    CRWStream* stream = (CRWStream*)stream_;
    if (stream != NULL) {
        stream->Release();
        delete stream;
    }
}

CObjectDictionary* CEngine::GetObjectDictionary() {
    return &m_object_dictionary;
}

CAnimDictionary* CEngine::GetAnimDictionary() {
    return &m_anim_dictionary;
}

CTextureDictionary* CEngine::GetTextureDictionary() {
    return &m_texture_dictionary;
}

CImmediate* CEngine::GetImmediate() {
    return &m_immediate;
}

CTimerGCN* CEngine::CreateTimer() {
    unsigned long timer = reinterpret_cast<unsigned long>(new CTimerGCN(m_wrap_engine->GetTimer()));
    if (timer == NULL) {
        return NULL;
    }

    reinterpret_cast<CTimerGCN*>(timer)->SetEngine(this);
    for (int i = 0; i < (int)AS_ULONG_VECTOR_HACK(m_timers).size(); i++) {
        if (AS_ULONG_VECTOR_HACK(m_timers)[i] == NULL) {
            AS_ULONG_VECTOR_HACK(m_timers)[i] = timer;
            return reinterpret_cast<CTimerGCN*>(timer);
        }
    }

    AS_ULONG_VECTOR_HACK(m_timers).push_back(timer);

    return reinterpret_cast<CTimerGCN*>(timer);
}

extern "C" {
extern int RwRenderStateSet(RwRenderState, void*);
}

BOOL CEngine::SetRaster(IRaster* raster_) {
    CRaster* raster = (CRaster*)raster_;
    if (raster == NULL) {
        this->SetRenderState((RwRenderState)1, NULL);
    } else if (!m_rendering) {
        return FALSE;
    } else {
        void* raster_ptr = NULL;
        if (raster != NULL) {
            raster_ptr = raster->m_wrap_raster;
        }
        return RwRenderStateSet((RwRenderState)1, raster_ptr);
    }

    return TRUE;
}

BOOL CEngine::RegisterEvent(U32 id, std::string name, IEventAnimationCallback* callback) {
    // Return early if the event is already registered.
    int location = FindEvent(id);
    if (location < 0) {
        location = FindEvent(name);
    }
    if (location >= 0) {
        return FALSE;
    }

    SEVENTANIMATIONCALLBACKENTRY event;
    event.callback = callback;
    event.id = id;
    event.name.assign(name, 0);
    m_event_animation_callback_entries.push_back(event);
    return TRUE;
}

BOOL CEngine::ChangeEventCallback(U32 id, IEventAnimationCallback* callback) {
    int location = FindEvent(id);
    if (location < 0) {
        return FALSE;
    }

    m_event_animation_callback_entries[location].callback = callback;
    return TRUE;
}

BOOL CEngine::ChangeEventCallback(std::string name, IEventAnimationCallback* callback) {
    int location = FindEvent(name);
    if (location < 0) {
        return FALSE;
    }

    m_event_animation_callback_entries[location].callback = callback;
    return TRUE;
}

IEventAnimationCallback* CEngine::GetEventCallback(U32 id) {
    int location = FindEvent(id);
    if (location < 0) {
        return NULL;
    }

    return m_event_animation_callback_entries[location].callback;
}

IEventAnimationCallback* CEngine::GetEventCallback(std::string name) {
    int location = FindEvent(name);
    if (location < 0) {
        return NULL;
    }

    return m_event_animation_callback_entries[location].callback;
}

U32 CEngine::GetNumberOfEvents() {
    return m_event_animation_callback_entries.size();
}

int CEngine::FindEvent(U32 id) {
    for (int i = 0; i < (int)m_event_animation_callback_entries.size(); i++) {
        SEVENTANIMATIONCALLBACKENTRY& entry = m_event_animation_callback_entries[i];
        if (entry.id == id) {
            return i;
        }
    }
    return -1;
}

int CEngine::FindEvent(std::string name) {
    for (int i = 0; i < (int)m_event_animation_callback_entries.size(); i++) {
        SEVENTANIMATIONCALLBACKENTRY& entry = m_event_animation_callback_entries[i];
        if (entry.name == name) {
            return i;
        }
    }
    return -1;
}

SEVENTANIMATIONCALLBACKENTRY* CEngine::GetEvent(int id) {
    if (id < 0 || id >= (int)GetNumberOfEvents()) {
        return NULL;
    }

    return &m_event_animation_callback_entries[id];
}

BOOL CEngine::IsForeground() {
    return TRUE;
}

}
