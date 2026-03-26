#ifndef ENGINE_DISPLAY_CSCENE_H
#define ENGINE_DISPLAY_CSCENE_H

#include <vector>
#include "engine/display/IScene.h"
#include "engine/display/IShadowMapValidationCallback.h"
#include "engine/display/IParticleEmitterDefinition.h"
#include "engine/display/CObjectDictionary.h"
#include "engine/wrap/DKW_RGBAReal.h"

namespace DKDSP {

class CEngine;

// FIXME: This needs a home.
class CShadowMapDenyCallback : public IShadowMapValidationCallback {
public:
    CShadowMapDenyCallback();
    ~CShadowMapDenyCallback();

    virtual BOOL ValidateDisplay(IShadowMap*, ICamera*, CDKW_Atomic*, CDKW_Sphere*);
    virtual BOOL ValidateObjectDisplay(CDKW_Atomic*);
};

// TODO
struct SBATCH3DENTRY {
    U8 unk0[4];
    CDKW_Matrix* matrix;
    U8 unk8[4];
};
REQUIRE_SIZE(SBATCH3DENTRY, 0xC);

class CScene : public IScene {
public:
    CEngine* m_engine;
    CDKW_World* m_world;
    CDKW_World* m_collision_world;
    CCamera* m_selected_camera;
    CObjectDictionary* m_object_dictionary;
    std::vector<void*> m_unk18;
    std::vector<CLight*> m_lights;
    std::vector<CCamera*> m_cameras;
    std::vector<CShadowMap*> m_shadow_maps;
    std::vector<void*> m_unk48;
    std::vector<void*> m_unk54;
    std::vector<void*> m_unk60;
    std::vector<void*> m_unk6C;
    std::vector<SBATCH3DENTRY> m_unk78;
    std::vector<void*> m_unk84;
    BOOL m_rendering;
    BOOL m_unk94;
    BOOL m_unk98;
    CAtomic m_atomic9C;
    CDKW_RGBAReal m_colorB0;
    CDKW_RGBAReal m_colorC0;
    CShadowMapDenyCallback m_shadow_map_deny_callback;
    IShadowMapValidationCallback* m_unkD4;

public:
    CScene();
    ~CScene();

    virtual void Release();
    virtual U32 GetClumpSize();
    virtual U32 GetAtomicSize();
    virtual U32 GetParticleEmitterSize();
    virtual U32 GetSize();
    virtual BOOL LoadBSP(IRWStream* stream);
    virtual BOOL LoadCollisionBSP(IRWStream* stream);
    virtual CDKW_World* GetWorld();
    virtual CDKW_World* GetCollisionWorld();
    virtual void ReleaseCollisionBSP();
    virtual int CreateCameraGetIndex();
    virtual CCamera* CreateCamera();
    virtual int GetNumberOfCamera();
    virtual CCamera* GetCamera(int id);
    virtual int GetCameraIndex(ICamera* camera);
    virtual void RemoveCamera(int id);
    virtual void RemoveCamera(ICamera* camera);
    virtual void RemoveAllCamera();
    virtual int CreateLightGetIndex(ELIGHTTYPE);
    virtual CLight* CreateLight(ELIGHTTYPE);
    virtual CLight* CopyLight(ILight*);
    virtual int GetNumberOfLights();
    virtual CLight* GetLight(int);
    virtual int GetLightIndex(ILight* light);
    virtual void EnableLight(ILight* light, BOOL enable);
    virtual void EnableLight(int id, BOOL enable);
    virtual void RemoveLight(int id);
    virtual void RemoveLight(ILight* light);
    virtual void RemoveAllLights();
    virtual int CreateShadowMapGetIndex(ILight* light, U32, U32, U32);
    virtual CShadowMap* CreateShadowMap(ILight* light, U32, U32, U32);
    virtual int GetNumberOfShadowMaps();
    virtual CShadowMap* GetShadowMap(int id);
    virtual int GetShadowMapIndex(IShadowMap* shadow_map);
    virtual void RemoveShadowMap(int id);
    virtual void RemoveShadowMap(IShadowMap* shadow_map);
    virtual void RemoveAllShadowMaps();
    virtual void SetupClumpToReceiveShadowMap(IClump* clump, int);
    virtual void SetupAtomicToReceiveShadowMap(IClump* clump, int, int);
    virtual void SetupAtomicToReceiveShadowMap(IAtomic* atomic, int);
    virtual void EnableShadowMapRendering(int shadow_map_id, int);
    virtual void EnableShadowMapRendering(IShadowMap* shadow_map, int);
    virtual void RenderShadowMap(int id);
    virtual void RenderShadowMap(IShadowMap* shadow_map);
    virtual void UpdateShadowMaps();
    virtual void PrepareShadowMaps();
    virtual void UnprepareShadowMaps();
    virtual int CreateMirrorGetIndex(U32 texture_width, U32 texture_height, U32 texture_depth_maybe);
    virtual CMirror* CreateMirror(U32 texture_width, U32 texture_height, U32 texture_depth_maybe);
    virtual int GetNumberOfMirrors();
    virtual CMirror* GetMirror(int id);
    virtual int GetMirrorIndex(IMirror* mirror);
    virtual void RemoveMirror(int id);
    virtual void RemoveMirror(IMirror* mirror);
    virtual void RemoveAllMirrors();
    virtual void EnableMirrorRendering(int id, BOOL enable);
    virtual void EnableMirrorRendering(IMirror* mirror, BOOL enable);
    virtual void RenderMirror(int id);
    virtual void RenderMirror(IMirror* mirror);
    virtual void AssociateClumpToMirror(IMirror* mirror, IClump* clump);
    virtual void UpdateMirrors();
    virtual void SetupAtomicForMaterialAnimation(IClump* clump, int);
    virtual void SetupAtomicForMaterialAnimation(IAtomic* atomic);
    virtual int CreateWarpGetIndex();
    virtual CWarp* CreateWarp();
    virtual int GetNumberOfWarps();
    virtual CWarp* GetWarp(int id);
    virtual int GetWarpIndex(IWarp* warp);
    virtual void RemoveWarp(int id);
    virtual void RemoveWarp(IWarp*);
    virtual void RemoveAllWarps();
    virtual void EnableWarpRendering(int id, BOOL enable);
    virtual void EnableWarpRendering(IWarp* warp, BOOL enable);
    virtual void RenderWarp(int id);
    virtual void RenderWarp(IWarp*);
    virtual void RenderWarps();
    virtual int CloneClumpGetIndex(std::string, IObjectDictionary*);
    virtual CClump* CloneClump(std::string, IObjectDictionary*);
    virtual CClump* CloneClump(IClump* clump);
    virtual int CloneClumpGetIndex(IClump* clump);
    virtual int GetNumberOfClumps();
    virtual CClump* GetClump(int id);
    virtual int GetClumpIndex(IClump* clump);
    virtual void RemoveClump(int id);
    virtual void RemoveClump(IClump* clump);
    virtual void RemoveAllClumps();
    virtual int CloneAtomicGetIndex(std::string, IObjectDictionary*);
    virtual CAtomic* CloneAtomic(std::string, IObjectDictionary*);
    virtual int GetNumberOfAtomics();
    virtual CAtomic* GetAtomic(int id);
    virtual int GetAtomicIndex(IAtomic* atomic);
    virtual void RemoveAtomic(int id);
    virtual void RemoveAtomic(IAtomic* atomic);
    virtual void RemoveAllAtomics();
    virtual CParticleEmitter* CreateParticleEmitter(std::string, U32, U32);
    virtual CParticleEmitter* CreateParticleEmitterFromDefinition(std::string, IParticleEmitterDefinition*);
    virtual CParticleEmitter* LoadParticleEmitter(std::string, DkXmd::CChunkIterator*);
    virtual CParticleEmitter* LoadParticleEmitter(std::string, IRWStream* stream);
    virtual CParticleEmitter* LoadParticleEmitterFromFile(std::string, char* filename);
    virtual int GetNumberOfParticleEmitters();
    virtual CParticleEmitter* FindParticleEmitter(std::string name);
    virtual CParticleEmitter* GetParticleEmitter(int id);
    virtual int GetParticleEmitterIndex(IParticleEmitter* emitter);
    virtual void RemoveParticleEmitter(int id);
    virtual void RemoveParticleEmitter(IParticleEmitter* emitter);
    virtual void RemoveAllParticleEmitters();
    virtual BOOL PickComputePixelRay(ICamera* camera, F32, F32, CDKW_V3d&, CDKW_V3d&);
    virtual CAtomic* PickAtomicOnLine(CDKW_V3d&, CDKW_V3d&);
    virtual CAtomic* PickAtomicOnPixel(F32, F32);
    virtual void SetAmbient(F32 red, F32 green, F32 blue);
    virtual F32 GetAmbientRed();
    virtual F32 GetAmbientGreen();
    virtual F32 GetAmbientBlue();
    virtual void SelectCamera(int id);
    virtual void SelectCamera(ICamera* camera);
    virtual CCamera* GetSelectedCamera();
    virtual void Clear(int);
    virtual void Clear(int, F32 red, F32 green, F32 blue);
    virtual BOOL BeginRender();
    virtual BOOL IsRendering();
    virtual void RenderBSP();
    virtual void RenderCollisionBSP();
    virtual void RenderAtomic(int id);
    virtual void RenderAtomic(IAtomic* atomic);
    virtual void RenderAtomicRange(int, int);
    virtual void RenderClump(int id);
    virtual void RenderClump(IClump* clump);
    virtual void RenderClumpFast(int id);
    virtual void RenderClumpFast(IClump* clump);
    virtual void RenderClumpRange(int, int);
    virtual void RenderParticleEmitter(IParticleEmitter* emitter, int);
    virtual void RenderBatch3D(IIm3DBatch* batch, RwPrimitiveType prim_type, CDKW_Matrix* matrix, int);
    virtual void RenderBatch2D(IIm2DBatch* batch, RwPrimitiveType prim_type, int);
    virtual void RenderCameraFrustum(ICamera* camera);
    virtual void RenderLightHelper(ILight* light);
    virtual void Render2DLine(CDKW_V3d*, CDKW_V3d*, F32, F32, F32, F32);
    virtual void Render3DLine(CDKW_V3d*, CDKW_V3d*, F32, F32, F32, F32);
    virtual void Flush();
    virtual void FlushParticleEmitters();
    virtual void FlushImmediate3D();
    virtual void FlushImmediate2D();
    virtual void EndRender();
    virtual void Flip(int);
    virtual void Screenshot(char*);

    void AffectAllLightsZero() {
        for (int i = 0; i < (int)m_lights.size(); i++) {
            if (m_lights[i] != NULL) {
                m_lights[i]->Affect(0);
            }
        }
    }

    void ReenableLightsIGuess() {
        for (int i = 0; i < (int)m_lights.size(); i++) {
            CLight* light = m_lights[i];
            if (light != NULL) {
                light->SetEnabled(light->IsEnabled());
            }
        }
    }

    void Initialize(CObjectDictionary* object_dictionary, CEngine* engine);
    int AddCamera(CCamera* camera);
};
REQUIRE_SIZE(CScene, 0xD8);

}

#endif
