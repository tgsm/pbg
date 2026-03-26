#ifndef ENGINE_DISPLAY_ISCENE_H
#define ENGINE_DISPLAY_ISCENE_H

#include "engine/display/CCamera.h"
#include "engine/display/CLight.h"
#include "engine/display/CMirror.h"
#include "engine/display/CParticleEmitter.h"
#include "engine/display/CParticleEmitterDefinition.h"
#include "engine/display/CShadowMap.h"
#include "engine/display/CWarp.h"
#include "engine/display/ICamera.h"
#include "engine/display/IIm2DBatch.h"
#include "engine/display/IIm3DBatch.h"
#include "engine/wrap/DKW_World.h"

namespace DKDSP {

class IObjectDictionary;

// TODO
class IScene {
public:
    IScene();

    virtual void Release() = 0;
    virtual U32 GetClumpSize() = 0;
    virtual U32 GetAtomicSize() = 0;
    virtual U32 GetParticleEmitterSize() = 0;
    virtual U32 GetSize() = 0;
    virtual BOOL LoadBSP(IRWStream* stream) = 0;
    virtual BOOL LoadCollisionBSP(IRWStream* stream) = 0;
    virtual CDKW_World* GetWorld() = 0;
    virtual CDKW_World* GetCollisionWorld() = 0;
    virtual void ReleaseCollisionBSP() = 0;
    virtual int CreateCameraGetIndex() = 0;
    virtual CCamera* CreateCamera() = 0;
    virtual int GetNumberOfCamera() = 0;
    virtual CCamera* GetCamera(int id) = 0;
    virtual int GetCameraIndex(ICamera* camera) = 0;
    virtual void RemoveCamera(int id) = 0;
    virtual void RemoveCamera(ICamera* camera) = 0;
    virtual void RemoveAllCamera() = 0;
    virtual int CreateLightGetIndex(ELIGHTTYPE) = 0;
    virtual CLight* CreateLight(ELIGHTTYPE) = 0;
    virtual CLight* CopyLight(ILight*) = 0;
    virtual int GetNumberOfLights() = 0;
    virtual CLight* GetLight(int) = 0;
    virtual int GetLightIndex(ILight* light) = 0;
    virtual void EnableLight(ILight* light, BOOL) = 0;
    virtual void EnableLight(int id, BOOL) = 0;
    virtual void RemoveLight(int id) = 0;
    virtual void RemoveLight(ILight* light) = 0;
    virtual void RemoveAllLights() = 0;
    virtual int CreateShadowMapGetIndex(ILight* light, U32, U32, U32) = 0;
    virtual CShadowMap* CreateShadowMap(ILight* light, U32, U32, U32) = 0;
    virtual int GetNumberOfShadowMaps() = 0;
    virtual CShadowMap* GetShadowMap(int id) = 0;
    virtual int GetShadowMapIndex(IShadowMap* shadow_map) = 0;
    virtual void RemoveShadowMap(int id) = 0;
    virtual void RemoveShadowMap(IShadowMap* shadow_map) = 0;
    virtual void RemoveAllShadowMaps() = 0;
    virtual void SetupClumpToReceiveShadowMap(IClump* clump, int) = 0;
    virtual void SetupAtomicToReceiveShadowMap(IClump* clump, int, int) = 0;
    virtual void SetupAtomicToReceiveShadowMap(IAtomic* atomic, int) = 0;
    virtual void EnableShadowMapRendering(int shadow_map_id, int) = 0;
    virtual void EnableShadowMapRendering(IShadowMap* shadow_map, int) = 0;
    virtual void RenderShadowMap(int id) = 0;
    virtual void RenderShadowMap(IShadowMap* shadow_map) = 0;
    virtual void UpdateShadowMaps() = 0;
    virtual void PrepareShadowMaps() = 0;
    virtual void UnprepareShadowMaps() = 0;
    virtual int CreateMirrorGetIndex(U32 texture_width, U32 texture_height, U32 texture_depth_maybe) = 0;
    virtual CMirror* CreateMirror(U32 texture_width, U32 texture_height, U32 texture_depth_maybe) = 0;
    virtual int GetNumberOfMirrors() = 0;
    virtual CMirror* GetMirror(int id) = 0;
    virtual int GetMirrorIndex(IMirror* mirror) = 0;
    virtual void RemoveMirror(int id) = 0;
    virtual void RemoveMirror(IMirror* mirror) = 0;
    virtual void RemoveAllMirrors() = 0;
    virtual void EnableMirrorRendering(int id, BOOL enable) = 0;
    virtual void EnableMirrorRendering(IMirror* mirror, BOOL enable) = 0;
    virtual void RenderMirror(int id) = 0;
    virtual void RenderMirror(IMirror* mirror) = 0;
    virtual void AssociateClumpToMirror(IMirror* mirror, IClump* clump) = 0;
    virtual void UpdateMirrors() = 0;
    virtual void SetupAtomicForMaterialAnimation(IClump* clump, int) = 0;
    virtual void SetupAtomicForMaterialAnimation(IAtomic* atomic) = 0;
    virtual int CreateWarpGetIndex() = 0;
    virtual CWarp* CreateWarp() = 0;
    virtual int GetNumberOfWarps() = 0;
    virtual CWarp* GetWarp(int id) = 0;
    virtual int GetWarpIndex(IWarp* warp) = 0;
    virtual void RemoveWarp(int id) = 0;
    virtual void RemoveWarp(IWarp*) = 0;
    virtual void RemoveAllWarps() = 0;
    virtual void EnableWarpRendering(int id, BOOL enable) = 0;
    virtual void EnableWarpRendering(IWarp* warp, BOOL enable) = 0;
    virtual void RenderWarp(int id) = 0;
    virtual void RenderWarp(IWarp*) = 0;
    virtual void RenderWarps() = 0;
    virtual int CloneClumpGetIndex(std::string, IObjectDictionary*) = 0;
    virtual CClump* CloneClump(std::string, IObjectDictionary*) = 0;
    virtual CClump* CloneClump(IClump* clump) = 0;
    virtual int CloneClumpGetIndex(IClump* clump) = 0;
    virtual int GetNumberOfClumps() = 0;
    virtual CClump* GetClump(int id) = 0;
    virtual int GetClumpIndex(IClump* clump) = 0;
    virtual void RemoveClump(int id) = 0;
    virtual void RemoveClump(IClump* clump) = 0;
    virtual void RemoveAllClumps() = 0;
    virtual int CloneAtomicGetIndex(std::string, IObjectDictionary*) = 0;
    virtual CAtomic* CloneAtomic(std::string, IObjectDictionary*) = 0;
    virtual int GetNumberOfAtomics() = 0;
    virtual CAtomic* GetAtomic(int id) = 0;
    virtual int GetAtomicIndex(IAtomic* atomic) = 0;
    virtual void RemoveAtomic(int id) = 0;
    virtual void RemoveAtomic(IAtomic* atomic) = 0;
    virtual void RemoveAllAtomics() = 0;
    virtual CParticleEmitter* CreateParticleEmitter(std::string, U32, U32) = 0;
    virtual CParticleEmitter* CreateParticleEmitterFromDefinition(std::string, IParticleEmitterDefinition*) = 0;
    virtual CParticleEmitter* LoadParticleEmitter(std::string, DkXmd::CChunkIterator*) = 0;
    virtual CParticleEmitter* LoadParticleEmitter(std::string, IRWStream* stream) = 0;
    virtual CParticleEmitter* LoadParticleEmitterFromFile(std::string, char* filename) = 0;
    virtual int GetNumberOfParticleEmitters() = 0;
    virtual CParticleEmitter* FindParticleEmitter(std::string name) = 0;
    virtual CParticleEmitter* GetParticleEmitter(int id) = 0;
    virtual int GetParticleEmitterIndex(IParticleEmitter* emitter) = 0;
    virtual void RemoveParticleEmitter(int id) = 0;
    virtual void RemoveParticleEmitter(IParticleEmitter* emitter) = 0;
    virtual void RemoveAllParticleEmitters() = 0;
    virtual BOOL PickComputePixelRay(ICamera* camera, F32, F32, CDKW_V3d&, CDKW_V3d&) = 0;
    virtual CAtomic* PickAtomicOnLine(CDKW_V3d&, CDKW_V3d&) = 0;
    virtual CAtomic* PickAtomicOnPixel(F32, F32) = 0;
    virtual void SetAmbient(F32 red, F32 green, F32 blue) = 0;
    virtual F32 GetAmbientRed() = 0;
    virtual F32 GetAmbientGreen() = 0;
    virtual F32 GetAmbientBlue() = 0;
    virtual void SelectCamera(int id) = 0;
    virtual void SelectCamera(ICamera* camera) = 0;
    virtual CCamera* GetSelectedCamera() = 0;
    virtual void Clear(int) = 0;
    virtual void Clear(int, F32 red, F32 green, F32 blue) = 0;
    virtual BOOL BeginRender() = 0;
    virtual BOOL IsRendering() = 0;
    virtual void RenderBSP() = 0;
    virtual void RenderCollisionBSP() = 0;
    virtual void RenderAtomic(int id) = 0;
    virtual void RenderAtomic(IAtomic* atomic) = 0;
    virtual void RenderAtomicRange(int, int) = 0;
    virtual void RenderClump(int id) = 0;
    virtual void RenderClump(IClump* clump) = 0;
    virtual void RenderClumpFast(int id) = 0;
    virtual void RenderClumpFast(IClump* clump) = 0;
    virtual void RenderClumpRange(int, int) = 0;
    virtual void RenderParticleEmitter(IParticleEmitter* emitter, int) = 0;
    virtual void RenderBatch3D(IIm3DBatch* batch, RwPrimitiveType prim_type, CDKW_Matrix* matrix, int) = 0;
    virtual void RenderBatch2D(IIm2DBatch* batch, RwPrimitiveType prim_type, int) = 0;
    virtual void RenderCameraFrustum(ICamera* camera) = 0;
    virtual void RenderLightHelper(ILight* light) = 0;
    virtual void Render2DLine(CDKW_V3d*, CDKW_V3d*, F32, F32, F32, F32) = 0;
    virtual void Render3DLine(CDKW_V3d*, CDKW_V3d*, F32, F32, F32, F32) = 0;
    virtual void Flush() = 0;
    virtual void FlushParticleEmitters() = 0;
    virtual void FlushImmediate3D() = 0;
    virtual void FlushImmediate2D() = 0;
    virtual void EndRender() = 0;
    virtual void Flip(int) = 0;
    virtual void Screenshot(char*) = 0;
};

}

#endif
