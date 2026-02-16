#ifndef ENGINE_DISPLAY_CENGINE_H
#define ENGINE_DISPLAY_CENGINE_H

#include "engine/display/IEngine.h"
#include "engine/display/CDisplacementPipelineGCN.h"
#include "engine/display/CMaterialAnimationPipelineGCN.h"
#include "engine/display/CShadowMapPipelineGCN.h"

namespace DKDSP {

struct IInitCallback {
    virtual void func8(CEngine* engine);
};
// Unknown size

struct SEVENTANIMATIONCALLBACKENTRY {
    std::string name;
    int id;
    IEventAnimationCallback* callback;

    SEVENTANIMATIONCALLBACKENTRY() : name("") {

    }
};
REQUIRE_SIZE(SEVENTANIMATIONCALLBACKENTRY, 0xC);

// Unknown
struct SRenderState {
    RwRenderState state;
    int unk4;
};

class CEngine : public IEngine {
public:
    std::vector<CScene*> m_scenes;
    CObjectDictionary m_object_dictionary;
    CAnimDictionary m_anim_dictionary;
    CTextureDictionary m_texture_dictionary;
    CImmediate m_immediate;
    CShadowMapPipelineGCN m_shadow_map_pipeline;
    CMaterialAnimationPipelineGCN m_material_animation_pipeline;
    CDisplacementPipelineGCN m_displacement_pipeline;
    CDkWrapEngine* m_wrap_engine;
    CCharset m_charset;
    std::vector<CTimer*> m_timers;
    BOOL m_rendering;
    U32 m_unk130;
    SRenderState m_render_states[29];
    U32 m_unk21C;
    void* m_window_handle;
    std::vector<SEVENTANIMATIONCALLBACKENTRY> m_event_animation_callback_entries;
    BOOL m_adl_enabled;
    CLight* m_adl_directional_light;
    BOOL m_unk238;
    U32 m_unk23C;
    U32 m_shadow_map_processing_width;
    U32 m_shadow_map_processing_height;
    void* m_gcn_video_mode;

public:
    CEngine() {
        m_wrap_engine = NULL;
        m_rendering = FALSE;
        m_unk21C = 0;
        m_adl_enabled = FALSE;
        m_adl_directional_light = NULL;
        m_unk238 = FALSE;

        ms_nUniquenessCounter = 0;

        m_shadow_map_processing_width = 128;
        m_shadow_map_processing_height = 128;

        m_gcn_video_mode = NULL;

        for (int i = 0; i < 29; i++) {
            m_render_states[i].state = (RwRenderState)0;
            m_render_states[i].unk4 = 0;
        }

        m_window_handle = NULL;
        m_unk130 = 0;

        ms_pGlobalTextureDictionary = &m_texture_dictionary;
        ms_pEngine = this;

        AS_ULONG_VECTOR_HACK(m_scenes).reserve(16);
        AS_ULONG_VECTOR_HACK(m_timers).reserve(16);
        m_event_animation_callback_entries.reserve(16);

        m_unk23C = 0;
    }
    ~CEngine();

    virtual void EnableADLPipelines();
    virtual BOOL IsADLEnabled();
    virtual void SetADLDirectionalLight(ILight*);
    virtual void SetCharsetCreation(BOOL allow) { CDKW_Engine::ms_bAllowCharset = allow; }
    virtual void* Init(void*, void*, U32 screen_width, U32 screen_height);
    virtual void* Init(void* a1) { return Init(a1, NULL, 640, 480); }
    virtual void* InitManaged() { return Init(NULL, NULL, 640, 480); }
    virtual void* InitManaged(U32 screen_width, U32 screen_height) { return Init(NULL, NULL, screen_width, screen_height); }
    virtual void SetGCNVideoMode(void* video_mode);
    virtual void* Open(void*, void*, U32 screen_width, U32 screen_height); // FIXME: Figure out what these return.
    virtual void* Open(void* a1, void* a2) { return Open(a1, a2, 640, 480); }
    virtual void* Open(void* a1) { return Open(a1, NULL, 640, 480); }
    virtual void* Open() { return Open(NULL, NULL, 640, 480); }
    virtual void* Open(U32 screen_width, U32 screen_height) { return Open(NULL, NULL, 640, 480); }
    virtual void Setup(int subsystem, int video_mode);
    virtual void Start();
    virtual void* GetWindowHandle(); // Not sure what this returns, the window handle is always NULL
    virtual BOOL Update();
    virtual void Release();
    virtual void EnableDMACheck();
    virtual void ResetDMACheck();
    virtual RwRect GetRect();
    virtual CCharset* GetCharset();
    virtual BOOL IsRendering();
    virtual U32 GetFPS();
    virtual F64 GetTime();
    virtual void AllowEscape();
    virtual void Screenshot(char*, IRaster*);
    virtual void RegisterWarningCollector(IWarningCollector*);
    virtual void RegisterTextureCallback(ITextureCallback*);
    virtual void RegisterInitCallback(IInitCallback*);
    virtual void RegisterShadowMapValidationCallback(IShadowMapValidationCallback*);
    virtual U32 GetSize();
    virtual U32 GetResourceArenaUsed();
    virtual U32 GetVectorMemory();
    virtual U32 GetNumberOfAlphaAtomicBufferized();
    virtual void AlphaEnable();
    virtual void AlphaAtomicBufferization(BOOL enable);
    virtual BOOL IsAlphaAtomicBufferizationEnabled();
    virtual void EnableAtomicPreclip(BOOL enable);
    virtual BOOL IsAlphaPreclippingEnabled();
    virtual void EnableCameraOrthoNormalization(BOOL enable);
    virtual BOOL IsCameraOrthoNormalizationEnabled();
    virtual void EnableShowWindow(BOOL enable);
    virtual BOOL IsForeground();
    virtual void DisableAlphaTest();
    virtual void PrepareAlphaTest();
    virtual void SetAlphaTest(U32);
    virtual void RestoreAlphaTest();
    virtual U32 GetShadowMapImageProcessingRasterWidth() { return m_shadow_map_processing_width; }
    virtual U32 GetShadowMapImageProcessingRasterHeight() { return m_shadow_map_processing_height; }
    virtual void SetShadowMapImageProcessingResolution(U32 width, U32 height) {
        m_shadow_map_processing_height = height;
        m_shadow_map_processing_width = width;
    }
    virtual U32 GetNumberOfAdapters();
    virtual void SetAdapter(int);
    virtual int GetCurrentAdapter();
    virtual void GetAdapterInfo(int, SRWAdapterInfo*);
    virtual U32 GetNumberOfVideoModes();
    virtual void SetVideoMode(int);
    virtual int GetCurrentVideoMode();
    virtual void GetVideoModeInfo(int, SRWVideoModeInfo*);
    virtual U32 GetTextureMemorySize();
    virtual U32 GetMaxTextureSize();
    virtual void DisplayMetrics();
    virtual void DumpDictionaryInConsole();
    virtual CScene* CreateScene();
    virtual U32 GetSceneIndex(IScene* scene);
    virtual CScene* GetScene(int);
    virtual U32 GetNumberOfScenes();
    virtual void DestroyScene(IScene* scene);
    virtual void SetImagePath(char* path);
    virtual CRWStream* OpenStream(RwStreamType type, RwStreamAccessType access_type, const void*);
    virtual CRWStream* OpenStreamFile(RwStreamAccessType access_type, const std::FILE* file);
    virtual CRWStream* OpenStreamFileName(RwStreamAccessType access_type, char* filename);
    virtual CRWStream* OpenStreamMemory(RwStreamAccessType access_type, CDKW_Memory* memory);
    virtual CRWStream* OpenStreamCustom(RwStreamAccessType access_type, const void*);
    virtual void DestroyStream(IRWStream*);
    virtual CObjectDictionary* GetObjectDictionary();
    virtual CAnimDictionary* GetAnimDictionary();
    virtual CTextureDictionary* GetTextureDictionary();
    virtual CImmediate* GetImmediate();
    virtual CTimerGCN* CreateTimer();
    virtual void DestroyTimer(ITimer* timer);
    virtual void SetRenderState(RwRenderState, void*);
    virtual BOOL SetRaster(IRaster* raster);
    virtual BOOL RegisterEvent(U32, std::string name, IEventAnimationCallback*);
    virtual BOOL ChangeEventCallback(U32, IEventAnimationCallback*);
    virtual BOOL ChangeEventCallback(std::string name, IEventAnimationCallback*);
    virtual IEventAnimationCallback* GetEventCallback(U32);
    virtual IEventAnimationCallback* GetEventCallback(std::string name);
    virtual U32 GetNumberOfEvents();
    virtual int FindEvent(U32);
    virtual int FindEvent(std::string name);

    CRWStream* OpenStreamInline(CDKW_Stream* wrap_stream, RwStreamType type, RwStreamAccessType access_type, void* a4) {
        CRWStream* stream = new CRWStream(wrap_stream, type, access_type, a4);
        if (stream == NULL) {
            RwStreamClose(wrap_stream, a4);
            return NULL;
        }
        return stream;
    }

    void SetCurrentCamera(ICamera* camera);
    void SetRendering(BOOL, CScene* scene);
    CShadowMapPipelineGCN* GetShadowMapPipeline();
    CMaterialAnimationPipelineGCN* GetMaterialAnimationPipeline();
    void UpdateRenderState();
    SEVENTANIMATIONCALLBACKENTRY* GetEvent(int id);

    static IWarningCollector* ms_pWarningCollector;
    static IInitCallback* ms_pInitCallback;
    static IShadowMapValidationCallback* ms_pShadowMapValidationCallback;
    static CTextureDictionary* ms_pGlobalTextureDictionary;
    static CEngine* ms_pEngine;
    static CScene* ms_pCurrentRenderingScene;
    static U32 ms_nUniquenessCounter;
};
REQUIRE_SIZE(CEngine, 0x24C);

}

#endif
