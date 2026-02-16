#ifndef ENGINE_DISPLAY_IENGINE_H
#define ENGINE_DISPLAY_IENGINE_H

#include "engine/display/CAnimDictionary.h"
#include "engine/display/CCharset.h"
#include "engine/display/CDkWrapEngine.h"
#include "engine/display/CImmediate.h"
#include "engine/display/CObjectDictionary.h"
#include "engine/display/CRWStream.h"
#include "engine/display/CScene.h"
#include "engine/display/CTextureDictionary.h"
#include "engine/display/CTimerGCN.h"
#include "engine/display/IEventAnimationCallback.h"

// FIXME: This maybe goes in its own header, but it's only used here.
class CDKW_Memory {
public:
    void* m_file;
    U32 m_unk4;
};

// FIXME: Put this in a RW header.
enum RwRenderState {};

namespace DKDSP {

class IInitCallback;
class IShadowMapValidationCallback;
class IWarningCollector;

struct SRWAdapterInfo {
    RwSubSystem subsystem;
};

struct SRWVideoModeInfo {
    RwVideoMode video_mode;
};

class IEngine {
public:
    IEngine();

    virtual void EnableADLPipelines() = 0;
    virtual BOOL IsADLEnabled() = 0;
    virtual void SetADLDirectionalLight(ILight*) = 0;
    virtual void SetCharsetCreation(BOOL allow) = 0;
    virtual void* Init(void*, void*, U32 screen_width, U32 screen_height) = 0;
    virtual void* Init(void*) = 0;
    virtual void* InitManaged() = 0;
    virtual void* InitManaged(U32 screen_width, U32 screen_height) = 0;
    virtual void SetGCNVideoMode(void* video_mode) = 0;
    virtual void* Open(void*, void*, U32 screen_width, U32 screen_height) = 0; // FIXME: Figure out what these return.
    virtual void* Open(void*, void*) = 0;
    virtual void* Open(void*) = 0;
    virtual void* Open() = 0;
    virtual void* Open(U32 screen_width, U32 screen_height) = 0;
    virtual void Setup(int subsystem, int video_mode) = 0;
    virtual void Start() = 0;
    virtual void* GetWindowHandle() = 0; // Not sure what this returns, the window handle is always NULL
    virtual BOOL Update() = 0;
    virtual void Release() = 0;
    virtual void EnableDMACheck() = 0;
    virtual void ResetDMACheck() = 0;
    virtual RwRect GetRect() = 0;
    virtual CCharset* GetCharset() = 0;
    virtual BOOL IsRendering() = 0;
    virtual U32 GetFPS() = 0;
    virtual F64 GetTime() = 0;
    virtual void AllowEscape() = 0;
    virtual void Screenshot(char*, IRaster*) = 0;
    virtual void RegisterWarningCollector(IWarningCollector*) = 0;
    virtual void RegisterTextureCallback(ITextureCallback*) = 0;
    virtual void RegisterInitCallback(IInitCallback*) = 0;
    virtual void RegisterShadowMapValidationCallback(IShadowMapValidationCallback*) = 0;
    virtual U32 GetSize() = 0;
    virtual U32 GetResourceArenaUsed() = 0;
    virtual U32 GetVectorMemory() = 0;
    virtual U32 GetNumberOfAlphaAtomicBufferized() = 0;
    virtual void AlphaEnable() = 0;
    virtual void AlphaAtomicBufferization(BOOL enable) = 0;
    virtual BOOL IsAlphaAtomicBufferizationEnabled() = 0;
    virtual void EnableAtomicPreclip(BOOL enable) = 0;
    virtual BOOL IsAlphaPreclippingEnabled() = 0;
    virtual void EnableCameraOrthoNormalization(BOOL enable) = 0;
    virtual BOOL IsCameraOrthoNormalizationEnabled() = 0;
    virtual void EnableShowWindow(BOOL enable) = 0;
    virtual BOOL IsForeground() = 0;
    virtual void DisableAlphaTest() = 0;
    virtual void PrepareAlphaTest() = 0;
    virtual void SetAlphaTest(U32) = 0;
    virtual void RestoreAlphaTest() = 0;
    virtual U32 GetShadowMapImageProcessingRasterWidth() = 0;
    virtual U32 GetShadowMapImageProcessingRasterHeight() = 0;
    virtual void SetShadowMapImageProcessingResolution(U32 width, U32 height) = 0;
    virtual U32 GetNumberOfAdapters() = 0;
    virtual void SetAdapter(int) = 0;
    virtual int GetCurrentAdapter() = 0;
    virtual void GetAdapterInfo(int, SRWAdapterInfo*) = 0;
    virtual U32 GetNumberOfVideoModes() = 0;
    virtual void SetVideoMode(int) = 0;
    virtual int GetCurrentVideoMode() = 0;
    virtual void GetVideoModeInfo(int, SRWVideoModeInfo*) = 0;
    virtual U32 GetTextureMemorySize() = 0;
    virtual U32 GetMaxTextureSize() = 0;
    virtual void DisplayMetrics() = 0;
    virtual void DumpDictionaryInConsole() = 0;
    virtual CScene* CreateScene() = 0;
    virtual U32 GetSceneIndex(IScene* scene) = 0;
    virtual CScene* GetScene(int) = 0;
    virtual U32 GetNumberOfScenes() = 0;
    virtual void DestroyScene(IScene* scene) = 0;
    virtual void SetImagePath(char* path) = 0;
    virtual CRWStream* OpenStream(RwStreamType type, RwStreamAccessType access_type, const void*) = 0;
    virtual CRWStream* OpenStreamFile(RwStreamAccessType access_type, const std::FILE* file) = 0;
    virtual CRWStream* OpenStreamFileName(RwStreamAccessType access_type, char* filename) = 0;
    virtual CRWStream* OpenStreamMemory(RwStreamAccessType access_type, CDKW_Memory* memory) = 0;
    virtual CRWStream* OpenStreamCustom(RwStreamAccessType access_type, const void*) = 0;
    virtual void DestroyStream(IRWStream*) = 0;
    virtual CObjectDictionary* GetObjectDictionary() = 0;
    virtual CAnimDictionary* GetAnimDictionary() = 0;
    virtual CTextureDictionary* GetTextureDictionary() = 0;
    virtual CImmediate* GetImmediate() = 0;
    virtual CTimerGCN* CreateTimer() = 0;
    virtual void DestroyTimer(ITimer* timer) = 0;
    virtual void SetRenderState(RwRenderState, void*) = 0;
    virtual BOOL SetRaster(IRaster* raster) = 0;
    virtual BOOL RegisterEvent(U32, std::string name, IEventAnimationCallback*) = 0;
    virtual BOOL ChangeEventCallback(U32, IEventAnimationCallback*) = 0;
    virtual BOOL ChangeEventCallback(std::string name, IEventAnimationCallback*) = 0;
    virtual IEventAnimationCallback* GetEventCallback(U32) = 0;
    virtual IEventAnimationCallback* GetEventCallback(std::string name) = 0;
    virtual U32 GetNumberOfEvents() = 0;
    virtual int FindEvent(U32) = 0;
    virtual int FindEvent(std::string name) = 0;
};

}

#endif
