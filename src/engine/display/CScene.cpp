#include <new>
#include <rwsdk/badevice.h>
#include "engine/display/CScene.h"
#include "engine/display/CEngine.h"

namespace DKDSP {

// Equivalent: CAtomic temporary
CScene::CScene() {
    m_engine = NULL;
    m_world = NULL;
    m_collision_world = NULL;
    m_object_dictionary = NULL;
    m_selected_camera = NULL;
    m_rendering = FALSE;
    m_unk94 = FALSE;
    m_unk98 = FALSE;
    m_unkD4 = NULL;
}

// Equivalent: std::vector
CScene::~CScene() {
    Release();
}

void CScene::Initialize(CObjectDictionary* object_dictionary, CEngine* engine) {
    if (m_world == NULL) {
        m_world = new (RwEngineInstance->unk140(CDKW_World::ms_pMemEntry)) CDKW_World(1000.0f);
    }
    m_object_dictionary = object_dictionary;
    m_engine = engine;
    m_unk98 = FALSE;

    AS_ULONG_VECTOR_HACK(m_unk6C).reserve(16);
    m_3d_batches.reserve(16);
    AS_ULONG_VECTOR_HACK(m_2d_batches).reserve(16);
    AS_ULONG_VECTOR_HACK(m_clumps).reserve(16);
    AS_ULONG_VECTOR_HACK(m_lights).reserve(16);
    AS_ULONG_VECTOR_HACK(m_cameras).reserve(16);
    AS_ULONG_VECTOR_HACK(m_shadow_maps).reserve(16);
    AS_ULONG_VECTOR_HACK(m_mirrors).reserve(16);
    AS_ULONG_VECTOR_HACK(m_warps).reserve(16);
    AS_ULONG_VECTOR_HACK(m_particle_emitters).reserve(16);
}

void CScene::Release() {
    RemoveAllParticleEmitters();
    RemoveAllWarps();
    RemoveAllClumps();
    RemoveAllAtomics();
    RemoveAllShadowMaps();
    RemoveAllMirrors();
    RemoveAllLights();
    RemoveAllCamera();

    for (int i = 0; i < (int)m_3d_batches.size(); i++) {
        SBATCH3DENTRY& entry = m_3d_batches[i];
        if (entry.matrix != NULL) {
            RwMatrixDestroy(entry.matrix);
        }
        entry.matrix = NULL;
    }

    m_3d_batches.clear();
    m_unk6C.__unknown_reset_function();
    m_2d_batches.__unknown_reset_function();

    ReleaseCollisionBSP();
    if (m_world != NULL) {
        delete m_world;
    }
    m_world = NULL;

    m_object_dictionary = NULL;
    m_selected_camera = NULL;
    m_unk98 = FALSE;
    m_unkD4 = NULL;
}

U32 CScene::GetAtomicSize() {
    return 0;
}

CDKW_World* CScene::GetWorld() {
    return m_world;
}

CDKW_World* CScene::GetCollisionWorld() {
    return m_collision_world;
}

void CScene::ReleaseCollisionBSP() {
    if (m_collision_world != NULL) {
        delete m_collision_world;
    }
    m_collision_world = NULL;
}

int CScene::AddCamera(CCamera* camera) {
    unsigned long camera_ul = reinterpret_cast<unsigned long>(camera);
    m_world->AddCamera(camera->m_wrap_camera);

    for (int i = 0; i < (int)AS_ULONG_VECTOR_HACK(m_cameras).size(); i++) {
        if (AS_ULONG_VECTOR_HACK(m_cameras)[i] == NULL) {
            AS_ULONG_VECTOR_HACK(m_cameras)[i] = camera_ul;
            return i;
        }
    }

    AS_ULONG_VECTOR_HACK(m_cameras).push_back(camera_ul);
    return AS_ULONG_VECTOR_HACK(m_cameras).size() - 1;
}

int CScene::CreateCameraGetIndex() {
    return AddCamera(new CCamera);
}

CCamera* CScene::CreateCamera() {
    int index = CreateCameraGetIndex();
    if (index < 0) {
        return NULL;
    }

    return m_cameras[index];
}

int CScene::GetNumberOfCamera() {
    return m_cameras.size();
}

CCamera* CScene::GetCamera(int id) {
    if (id < 0 || id >= (int)m_cameras.size()) {
        return NULL;
    }
    return m_cameras[id];
}

int CScene::GetCameraIndex(ICamera* camera) {
    if (camera == NULL) {
        return -1;
    }

    for (int i = 0; i < (int)m_cameras.size(); i++) {
        if (m_cameras[i] == camera) {
            return i;
        }
    }
    return -1;
}

void CScene::RemoveCamera(ICamera* camera) {
    for (int i = 0; i < (int)m_cameras.size(); i++) {
        if (m_cameras[i] == camera) {
            RemoveCamera(i);
            return;
        }
    }
}

void CScene::RemoveAllCamera() {
    for (int i = 0; i < (int)m_cameras.size(); i++) {
        RemoveCamera(i);
    }

    m_cameras.__unknown_reset_function();
}

int CScene::CreateLightGetIndex(ELIGHTTYPE type) {
    CLight* light = new CLight(type);
    unsigned long light_ul = reinterpret_cast<unsigned long>(light);

    for (int i = 0; i < (int)AS_ULONG_VECTOR_HACK(m_lights).size(); i++) {
        if (m_lights[i] == NULL) {
            m_lights[i] = light;
            return i;
        }
    }

    AS_ULONG_VECTOR_HACK(m_lights).push_back(light_ul);
    reinterpret_cast<CLight*>(light_ul)->SetEnabled(TRUE);
    m_world->AddLight(reinterpret_cast<CLight*>(light_ul)->m_wrap_light);

    return AS_ULONG_VECTOR_HACK(m_lights).size() - 1;
}

CLight* CScene::CreateLight(ELIGHTTYPE type) {
    int index = CreateLightGetIndex(type);
    if (index < 0) {
        return NULL;
    }

    return m_lights[index];
}

CLight* CScene::CopyLight(ILight* light_) {
    CLight* copy;

    CLight* light = (CLight*)light_;
    if (light == NULL) {
        return NULL;
    }

    copy = CreateLight(light->GetLightType());
    if (copy == NULL) {
        return NULL;
    }

    copy->GetSettingsFromLight(light);
    return copy;
}

int CScene::GetNumberOfLights() {
    return m_lights.size();
}

CLight* CScene::GetLight(int id) {
    if (id < 0 || id >= (int)m_lights.size()) {
        return NULL;
    }
    return m_lights[id];
}

int CScene::GetLightIndex(ILight* light) {
    if (light == NULL) {
        return -1;
    }

    for (int i = 0; i < (int)m_lights.size(); i++) {
        if (m_lights[i] == light) {
            return i;
        }
    }
    return -1;
}

void CScene::EnableLight(ILight* light, BOOL enable) {
    if (light == NULL) {
        return;
    }

    for (int i = 0; i < (int)m_lights.size(); i++) {
        if (m_lights[i] == light) {
            EnableLight(i, enable);
            return;
        }
    }
}

void CScene::EnableLight(int id, BOOL enable) {
    if (id < 0 || id >= (int)m_lights.size()) {
        return;
    }

    if (enable == TRUE) {
        if (m_lights[id]->IsEnabled() == FALSE) {
            m_lights[id]->SetEnabled(TRUE);
            m_unk94 = TRUE;
        }
    } else if (m_lights[id]->IsEnabled() == TRUE) {
        m_lights[id]->SetEnabled(FALSE);
    }
}

void CScene::RemoveLight(ILight* light) {
    for (int i = 0; i < (int)m_lights.size(); i++) {
        if (m_lights[i] == light) {
            RemoveLight(i);
            return;
        }
    }
}

void CScene::RemoveAllLights() {
    for (int i = 0; i < (int)m_lights.size(); i++) {
        RemoveLight(i);
    }

    m_lights.__unknown_reset_function();
}

int CScene::CreateShadowMapGetIndex(ILight* light, U32 width, U32 height, U32 a4) {
    CShadowMap* shadow_map = new CShadowMap;
    unsigned long shadow_map_ul = reinterpret_cast<unsigned long>(shadow_map);

    shadow_map->Init(m_engine, this, light, width, height, a4);

    for (int i = 0; i < (int)AS_ULONG_VECTOR_HACK(m_shadow_maps).size(); i++) {
        if (m_shadow_maps[i] == NULL) {
            AS_ULONG_VECTOR_HACK(m_shadow_maps)[i] = shadow_map_ul;
            return i;
        }
    }

    AS_ULONG_VECTOR_HACK(m_shadow_maps).push_back(shadow_map_ul);
    return m_shadow_maps.size() - 1;
}

CShadowMap* CScene::CreateShadowMap(ILight* light, U32 width, U32 height, U32 a4) {
    int index = CreateShadowMapGetIndex(light, width, height, a4);
    if (index < 0) {
        return NULL;
    }

    return m_shadow_maps[index];
}

int CScene::GetNumberOfShadowMaps() {
    return m_shadow_maps.size();
}

CShadowMap* CScene::GetShadowMap(int id) {
    if (id < 0 || id >= (int)m_shadow_maps.size()) {
        return NULL;
    }
    return m_shadow_maps[id];
}

int CScene::GetShadowMapIndex(IShadowMap* shadow_map) {
    if (shadow_map == NULL) {
        return -1;
    }

    for (int i = 0; i < (int)m_shadow_maps.size(); i++) {
        if (m_shadow_maps[i] == shadow_map) {
            return i;
        }
    }
    return -1;
}

void CScene::RemoveShadowMap(IShadowMap* shadow_map) {
    for (int i = 0; i < (int)m_shadow_maps.size(); i++) {
        if (m_shadow_maps[i] == shadow_map) {
            RemoveShadowMap(i);
            return;
        }
    }
}

void CScene::RemoveAllShadowMaps() {
    for (int i = 0; i < (int)m_shadow_maps.size(); i++) {
        RemoveShadowMap(i);
    }

    m_shadow_maps.__unknown_reset_function();
}

void CScene::SetupClumpToReceiveShadowMap(IClump* clump, int a2) {
    if (clump != NULL) {
        m_engine->GetShadowMapPipeline()->SetupClump(clump, a2);
    }
}

void CScene::SetupAtomicToReceiveShadowMap(IClump* clump, int a2, int a3) {
    if (clump != NULL) {
        m_engine->GetShadowMapPipeline()->SetupAtomic(clump, a2, a3);
    }
}

void CScene::SetupAtomicToReceiveShadowMap(IAtomic* atomic, int a2) {
    if (atomic != NULL) {
        m_engine->GetShadowMapPipeline()->SetupAtomic(atomic, a2);
    }
}

void CScene::EnableShadowMapRendering(int id, BOOL enable) {
    if (id < 0 || id >= (int)m_shadow_maps.size()) {
        return;
    }

    if (m_shadow_maps[id] != NULL) {
        EnableShadowMapRendering(m_shadow_maps[id], enable);
    }
}

void CScene::EnableShadowMapRendering(IShadowMap* shadow_map_, BOOL enable) {
    CShadowMap* shadow_map = (CShadowMap*)shadow_map_;
    if (shadow_map != NULL) {
        shadow_map->SetEnabled(enable);
    }
}

void CScene::RenderShadowMap(int id) {
    if (id < 0 || id >= (int)m_shadow_maps.size()) {
        return;
    }

    if (m_shadow_maps[id] != NULL) {
        RenderShadowMap(m_shadow_maps[id]);
    }
}

void CScene::RenderShadowMap(IShadowMap* shadow_map) {
    if (shadow_map == NULL) {
        return;
    }

    if (m_unk98 != TRUE) {
        m_unkD4 = CEngine::ms_pShadowMapValidationCallback;
        m_engine->RegisterShadowMapValidationCallback(&m_shadow_map_deny_callback);
        m_unk98 = TRUE;

        AffectAllLightsZero();
        m_colorC0 = m_ambient_color;
    }

    ((CShadowMap*)shadow_map)->Render();

    if (m_unk98 != FALSE) {
        m_engine->RegisterShadowMapValidationCallback(m_unkD4);
        m_unkD4 = NULL;
        m_unk98 = FALSE;

        ReenableLightsIGuess();
        SetAmbient(m_colorC0.m_r, m_colorC0.m_g, m_colorC0.m_b);
    }
}

void CScene::UpdateShadowMaps() {
    BOOL bVar1 = FALSE;
    for (int i = 0; i < (int)m_shadow_maps.size(); i++) {
        CShadowMap* shadow_map = m_shadow_maps[i];
        if (shadow_map != NULL && shadow_map->IsEnabled() == TRUE && shadow_map->IsDisplayEnabled() == TRUE) {
            bVar1 = TRUE;
            break;
        }
    }
    if (!bVar1) {
        return;
    }

    if (m_unk98 != TRUE) {
        m_unkD4 = CEngine::ms_pShadowMapValidationCallback;
        m_engine->RegisterShadowMapValidationCallback(&m_shadow_map_deny_callback);
        m_unk98 = TRUE;

        AffectAllLightsZero();
        m_colorC0 = m_ambient_color;
    }

    for (int i = 0; i < (int)m_shadow_maps.size(); i++) {
        CShadowMap* shadow_map = m_shadow_maps[i];
        if (shadow_map != NULL && shadow_map->IsEnabled() == TRUE && shadow_map->IsDisplayEnabled() == TRUE) {
            SetAmbient(shadow_map->GetShadowColorRed(), shadow_map->GetShadowColorGreen(), shadow_map->GetShadowColorBlue());
            shadow_map->Prepare();
            shadow_map->Render();
        }
    }

    if (m_unk98 != FALSE) {
        m_engine->RegisterShadowMapValidationCallback(m_unkD4);
        m_unkD4 = NULL;
        m_unk98 = FALSE;

        ReenableLightsIGuess();
        SetAmbient(m_colorC0.m_r, m_colorC0.m_g, m_colorC0.m_b);
    }
}

void CScene::PrepareShadowMaps() {
    for (int i = 0; i < (int)m_shadow_maps.size(); i++) {
        CShadowMap* shadow_map = m_shadow_maps[i];
        if (shadow_map != NULL && shadow_map->IsEnabled() == TRUE && shadow_map->IsDisplayEnabled() == TRUE) {
            shadow_map->Prepare();
        }
    }
}

void CScene::UnprepareShadowMaps() {
    for (int i = 0; i < (int)m_shadow_maps.size(); i++) {
        CShadowMap* shadow_map = m_shadow_maps[i];
        if (shadow_map != NULL && shadow_map->IsEnabled() == TRUE && shadow_map->IsDisplayEnabled() == TRUE) {
            shadow_map->Unprepare();
        }
    }
}

int CScene::CreateMirrorGetIndex(U32 width, U32 height, U32 a3) {
    CMirror* mirror = new CMirror;
    unsigned long mirror_ul = reinterpret_cast<unsigned long>(mirror);
    mirror->Init(m_engine, this, width, height, a3);

    for (int i = 0; i < (int)AS_ULONG_VECTOR_HACK(m_mirrors).size(); i++) {
        if (m_mirrors[i] == NULL) {
            AS_ULONG_VECTOR_HACK(m_mirrors)[i] = mirror_ul;
            return i;
        }
    }

    AS_ULONG_VECTOR_HACK(m_mirrors).push_back(mirror_ul);
    return AS_ULONG_VECTOR_HACK(m_mirrors).size() - 1;
}

CMirror* CScene::CreateMirror(U32 width, U32 height, U32 a3) {
    int index = CreateMirrorGetIndex(width, height, a3);
    if (index < 0) {
        return NULL;
    }

    return m_mirrors[index];
}

int CScene::GetNumberOfMirrors() {
    return m_mirrors.size();
}

CMirror* CScene::GetMirror(int id) {
    if (id < 0 || id >= (int)m_mirrors.size()) {
        return NULL;
    }
    return m_mirrors[id];
}

int CScene::GetMirrorIndex(IMirror* mirror) {
    if (mirror == NULL) {
        return -1;
    }

    for (int i = 0; i < (int)m_mirrors.size(); i++) {
        if (m_mirrors[i] == mirror) {
            return i;
        }
    }
    return -1;
}

void CScene::RemoveMirror(IMirror* mirror) {
    for (int i = 0; i < (int)m_mirrors.size(); i++) {
        if (m_mirrors[i] == mirror) {
            RemoveMirror(i);
            return;
        }
    }
}

void CScene::RemoveAllMirrors() {
    for (int i = 0; i < (int)m_mirrors.size(); i++) {
        RemoveMirror(i);
    }

    m_mirrors.__unknown_reset_function();
}

void CScene::EnableMirrorRendering(int id, BOOL enable) {
    if (id < 0 || id >= (int)m_mirrors.size()) {
        return;
    }

    if (m_mirrors[id] != NULL) {
        EnableMirrorRendering(m_mirrors[id], enable);
    }
}

void CScene::EnableMirrorRendering(IMirror* mirror_, BOOL enable) {
    CMirror* mirror = (CMirror*)mirror_;
    if (mirror != NULL) {
        mirror->SetEnabled(enable);
    }
}

void CScene::RenderMirror(int id) {
    if (id < 0 || id >= (int)m_mirrors.size()) {
        return;
    }

    if (m_mirrors[id] != NULL) {
        RenderMirror(m_mirrors[id]);
    }
}

void CScene::RenderMirror(IMirror* mirror_) {
    CMirror* mirror = (CMirror*)mirror_;
    if (mirror != NULL) {
        mirror->Render();
    }
}

void CScene::AssociateClumpToMirror(IMirror* mirror_, IClump* clump_) {
    CMirror* mirror = (CMirror*)mirror_;
    CClump* clump = (CClump*)clump_;
    if (mirror != NULL && clump != NULL) {
        mirror->AssociateClump(clump);
    }
}

void CScene::UpdateMirrors() {
    BOOL bVar1 = FALSE;
    for (int i = 0; i < (int)m_mirrors.size(); i++) {
        CMirror* mirror = m_mirrors[i];
        if (mirror != NULL && mirror->IsEnabled() == TRUE) {
            bVar1 = TRUE;
            break;
        }
    }
    if (!bVar1) {
        return;
    }

    for (int i = 0; i < (int)m_mirrors.size(); i++) {
        CMirror* mirror = m_mirrors[i];
        if (mirror != NULL && mirror->IsEnabled() == TRUE) {
            mirror->Render();
        }
    }
}
void CScene::SetupAtomicForMaterialAnimation(IClump* clump, int a2) {
    if (clump != NULL) {
        m_engine->GetMaterialAnimationPipeline()->SetupAtomic(clump, a2);
    }
}


void CScene::SetupAtomicForMaterialAnimation(IAtomic* atomic) {
    if (atomic != NULL) {
        m_engine->GetMaterialAnimationPipeline()->SetupAtomic(atomic);
    }
}

int CScene::CreateWarpGetIndex() {
    CWarp* warp = new CWarp;
    unsigned long warp_ul = reinterpret_cast<unsigned long>(warp);
    warp->Init(m_engine, this);

    for (int i = 0; i < (int)AS_ULONG_VECTOR_HACK(m_warps).size(); i++) {
        if (m_warps[i] == NULL) {
            AS_ULONG_VECTOR_HACK(m_warps)[i] = warp_ul;
            return i;
        }
    }

    AS_ULONG_VECTOR_HACK(m_warps).push_back(warp_ul);
    return AS_ULONG_VECTOR_HACK(m_warps).size() - 1;
}

CWarp* CScene::CreateWarp() {
    int index = CreateWarpGetIndex();
    if (index < 0) {
        return NULL;
    }

    return m_warps[index];
}

int CScene::GetNumberOfWarps() {
    return m_warps.size();
}

CWarp* CScene::GetWarp(int id) {
    if (id < 0 || id >= (int)m_warps.size()) {
        return NULL;
    }
    return m_warps[id];
}

int CScene::GetWarpIndex(IWarp* warp) {
    if (warp == NULL) {
        return -1;
    }

    for (int i = 0; i < (int)m_warps.size(); i++) {
        if (m_warps[i] == warp) {
            return i;
        }
    }
    return -1;
}

void CScene::RemoveWarp(IWarp* warp) {
    for (int i = 0; i < (int)m_warps.size(); i++) {
        if (m_warps[i] == warp) {
            RemoveWarp(i);
            return;
        }
    }
}

void CScene::RemoveAllWarps() {
    for (int i = 0; i < (int)m_warps.size(); i++) {
        RemoveWarp(i);
    }

    m_warps.__unknown_reset_function();
}

void CScene::EnableWarpRendering(int id, BOOL enable) {
    if (id < 0 || id >= (int)m_warps.size()) {
        return;
    }

    if (m_warps[id] != NULL) {
        EnableWarpRendering(m_warps[id], enable);
    }
}

void CScene::EnableWarpRendering(IWarp* warp_, BOOL enable) {
    CWarp* warp = (CWarp*)warp_;
    if (warp != NULL) {
        warp->SetEnabled(enable);
    }
}

void CScene::RenderWarp(int id) {
    if (id < 0 || id >= (int)m_warps.size()) {
        return;
    }

    if (m_warps[id] != NULL) {
        RenderWarp(m_warps[id]);
    }
}

void CScene::RenderWarp(IWarp* warp_) {
    CWarp* warp = (CWarp*)warp_;
    if (warp != NULL && m_rendering) {
        warp->Render();
    }
}

void CScene::RenderWarps() {
    if (!m_rendering) {
        return;
    }

    for (int i = 0; i < (int)m_warps.size(); i++) {
        CWarp* warp = m_warps[i];
        if (warp != NULL && warp->IsEnabled() == TRUE) {
            warp->Render();
        }
    }
}

int CScene::CloneClumpGetIndex(std::string name, IObjectDictionary* object_dictionary_) {
    CObjectDictionary* object_dictionary = m_object_dictionary;
    if (object_dictionary_ != NULL) {
        object_dictionary = (CObjectDictionary*)object_dictionary_;
    }

    CClump* clump = object_dictionary->FindClump(name);
    if (clump == NULL) {
        return -1;
    }

    return AddClumpGetIndex(clump->Clone());
}

CClump* CScene::CloneClump(std::string name, IObjectDictionary* object_dictionary) {
    int index = CloneClumpGetIndex(name, object_dictionary);
    if (index < 0) {
        return NULL;
    }
    return m_clumps[index];
}

CClump* CScene::CloneClump(IClump* clump) {
    int index = CloneClumpGetIndex(clump);
    if (index < 0) {
        return NULL;
    }
    return m_clumps[index];
}

int CScene::CloneClumpGetIndex(IClump* clump_) {
    CClump* clump = (CClump*)clump_;
    if (clump == NULL) {
        return -1;
    }
    return AddClumpGetIndex(clump->Clone());
}

int CScene::GetNumberOfClumps() {
    return m_clumps.size();
}

CClump* CScene::GetClump(int id) {
    if (id < 0 || id >= (int)m_clumps.size()) {
        return NULL;
    }
    return m_clumps[id];
}

int CScene::GetClumpIndex(IClump* clump) {
    if (clump == NULL) {
        return -1;
    }

    for (int i = 0; i < (int)m_clumps.size(); i++) {
        if (m_clumps[i] == clump) {
            return i;
        }
    }
    return -1;
}

void CScene::RemoveClump(IClump* clump) {
    for (int i = 0; i < (int)m_clumps.size(); i++) {
        if (m_clumps[i] == clump) {
            RemoveClump(i);
            return;
        }
    }
}

void CScene::RemoveAllClumps() {
    for (int i = 0; i < (int)m_clumps.size(); i++) {
        RemoveClump(i);
    }

    m_clumps.__unknown_reset_function();
}

int CScene::CloneAtomicGetIndex(std::string name, IObjectDictionary* object_dictionary) {
    return -1;
}

CAtomic* CScene::CloneAtomic(std::string name, IObjectDictionary* object_dictionary) {
    return NULL;
}

int CScene::GetNumberOfAtomics() {
    return 0;
}

CAtomic* CScene::GetAtomic(int id) {
    return NULL;
}

int CScene::GetAtomicIndex(IAtomic* atomic) {
    return -1;
}

void CScene::RemoveAtomic(int id) {

}

void CScene::RemoveAtomic(IAtomic* atomic) {

}

void CScene::RemoveAllAtomics() {

}

CParticleEmitter* CScene::AddParticleEmitter(std::string name) {
    unsigned long emitter_ul = reinterpret_cast<unsigned long>(new CParticleEmitter);
    reinterpret_cast<CParticleEmitter*>(emitter_ul)->SetTextureDictionary(m_engine->GetTextureDictionary());
    reinterpret_cast<CParticleEmitter*>(emitter_ul)->SetName(name);
    reinterpret_cast<CParticleEmitter*>(emitter_ul)->m_scene = this;

    for (int i = 0; i < (int)AS_ULONG_VECTOR_HACK(m_particle_emitters).size(); i++) {
        if (m_particle_emitters[i] == NULL) {
            AS_ULONG_VECTOR_HACK(m_particle_emitters)[i] = emitter_ul;
            return reinterpret_cast<CParticleEmitter*>(emitter_ul);
        }
    }

    AS_ULONG_VECTOR_HACK(m_particle_emitters).push_back(emitter_ul);
    return reinterpret_cast<CParticleEmitter*>(emitter_ul);
}

CParticleEmitter* CScene::CreateParticleEmitter(std::string name, U32 a2, U32 a3) {
    CParticleEmitter* emitter = AddParticleEmitter(name);
    if (emitter == NULL) {
        return NULL;
    }

    emitter->Create(a2, a3);
    return emitter;
}

CParticleEmitter* CScene::CreateParticleEmitterFromDefinition(std::string name, IParticleEmitterDefinition* definition) {
    if (definition == NULL) {
        return NULL;
    }

    CParticleEmitter* emitter = AddParticleEmitter(name);
    if (emitter == NULL) {
        return NULL;
    }

    emitter->Load(definition);
    return emitter;
}

CParticleEmitter* CScene::LoadParticleEmitter(std::string name, DkXmd::CChunkIterator* iter) {
    if (iter == NULL) {
        return NULL;
    }

    CParticleEmitter* emitter = AddParticleEmitter(name);
    if (emitter == NULL) {
        return NULL;
    }

    emitter->Load(iter);
    return emitter;
}

CParticleEmitter* CScene::LoadParticleEmitter(std::string name, IRWStream* stream) {
    if (stream == NULL) {
        return NULL;
    }

    CParticleEmitter* emitter = AddParticleEmitter(name);
    if (emitter == NULL) {
        return NULL;
    }

    emitter->Load(stream);
    return emitter;
}

CParticleEmitter* CScene::LoadParticleEmitterFromFile(std::string name, char* filename) {
    if (filename == NULL) {
        return NULL;
    }

    CRWStream* stream = m_engine->OpenStream(rwSTREAMFILENAME, rwSTREAMREAD, filename);
    if (stream == NULL) {
        return NULL;
    }

    CParticleEmitter* emitter = LoadParticleEmitter(name, stream);
    m_engine->DestroyStream(stream);
    if (emitter == NULL) {
        return NULL;
    }

    return emitter;
}

CParticleEmitter* CScene::FindParticleEmitter(std::string name) {
    for (int i = 0; i < (int)m_particle_emitters.size(); i++) {
        CParticleEmitter* emitter = m_particle_emitters[i];
        if (*emitter->GetName() == name) {
            return emitter;
        }
    }
    return NULL;
}

int CScene::GetNumberOfParticleEmitters() {
    return m_particle_emitters.size();
}

CParticleEmitter* CScene::GetParticleEmitter(int id) {
    if (id < 0 || id >= (int)m_particle_emitters.size()) {
        return NULL;
    }
    return m_particle_emitters[id];
}

int CScene::GetParticleEmitterIndex(IParticleEmitter* emitter) {
    if (emitter == NULL) {
        return -1;
    }

    for (int i = 0; i < (int)m_particle_emitters.size(); i++) {
        if (m_particle_emitters[i] == emitter) {
            return i;
        }
    }
    return -1;
}

void CScene::RemoveParticleEmitter(IParticleEmitter* emitter) {
    for (int i = 0; i < (int)m_particle_emitters.size(); i++) {
        if (m_particle_emitters[i] == emitter) {
            RemoveParticleEmitter(i);
            return;
        }
    }
}

void CScene::RemoveAllParticleEmitters() {
    for (int i = 0; i < (int)m_particle_emitters.size(); i++) {
        RemoveParticleEmitter(i);
    }

    m_particle_emitters.__unknown_reset_function();
}

extern "C" {
extern RwCamera* RwCameraCalcPixelRay(RwCamera* camera, void* a1, RwV2d*);
}

// Equivalent: CDKW_V3d's RwV3d constructor should not be inlined
BOOL CScene::PickComputePixelRay(ICamera* camera_, F32 a2, F32 a3, CDKW_V3d& a4, CDKW_V3d& a5) {
    RwV3d RStack_3c;
    RwV3d RStack_48;

    if (camera_ == NULL) {
        return FALSE;
    }
    CCamera* camera = (CCamera*)camera_;
    RwCamera* rw_camera = camera->m_wrap_camera->m_rw_camera;

    CDKW_V2d vec;
    vec.x = a2;
    vec.y = a3;

    if (!RwCameraCalcPixelRay(rw_camera, &RStack_48, &vec)) {
        return FALSE;
    }

    a4 = CDKW_V3d(RStack_48);
    a5 = CDKW_V3d(RStack_3c);

    return TRUE;
}

extern "C" {
extern RpLight* RpLightSetColor(RpLight* light, RwRGBAReal* color);
}

void CScene::SetAmbient(F32 red, F32 green, F32 blue) {
    if (m_world != NULL) {
        // FIXME: Make CDKW_RGBAReal inherit RwRGBAReal
        CDKW_RGBAReal color(red, green, blue, 1.0f);
        RpLightSetColor(m_world->m_rw_light, (RwRGBAReal*)&color);
    }

    m_ambient_color.m_r = red;
    m_ambient_color.m_g = green;
    m_ambient_color.m_b = blue;
    m_ambient_color.m_a = 1.0f;
}

F32 CScene::GetAmbientRed() {
    return m_ambient_color.m_r;
}

F32 CScene::GetAmbientGreen() {
    return m_ambient_color.m_g;
}

F32 CScene::GetAmbientBlue() {
    return m_ambient_color.m_b;
}

void CScene::SelectCamera(int id) {
    if (id < 0 || id >= (int)m_cameras.size()) {
        return;
    }

    m_selected_camera = m_cameras[id];
    if (m_selected_camera != NULL) {
        RwRect rect = m_engine->GetRect();
        m_selected_camera->m_wrap_camera->Resize(&rect);
    }
    m_engine->SetCurrentCamera(m_selected_camera);
}

void CScene::SelectCamera(ICamera* camera) {
    m_selected_camera = (CCamera*)camera;
    m_engine->SetCurrentCamera(m_selected_camera);
}

CCamera* CScene::GetSelectedCamera() {
    return m_selected_camera;
}

void CScene::Clear(int a1, F32 red, F32 green, F32 blue) {
    CDKW_Camera* cam = m_selected_camera->m_wrap_camera;
    CDKW_RGBA clear_color;
    clear_color.red = red * 255.0f;
    clear_color.green = green * 255.0f;
    clear_color.blue = blue * 255.0f;
    clear_color.alpha = 0xFF;
    if (a1 != 0) {
        RwCameraClear(cam->m_rw_camera, &clear_color, a1);
    }
}

BOOL CScene::IsRendering() {
    return m_rendering;
}

void CScene::RenderAtomic(int id) {
    if (!m_rendering) {
        return;
    }

    // Empty
}

void CScene::RenderAtomic(IAtomic* atomic) {
    if (!m_rendering) {
        return;
    }

    if (atomic == NULL) {
        return;
    }

    if (m_rendering && m_unk94 == TRUE) {
        m_unk94 = FALSE;
    }

    ((CAtomic*)atomic)->Render();
}

void CScene::RenderAtomicRange(int starting_id, int n) {

}

void CScene::RenderClump(int id) {
    if (!m_rendering || id < 0 || id >= (int)m_clumps.size()) {
        return;
    }

    if (m_clumps[id] != NULL) {
        RenderClump(m_clumps[id]);
    }
}

void CScene::RenderClump(IClump* clump) {
    if (clump == NULL) {
        return;
    }

    if (!m_rendering) {
        return;
    }

    if (m_rendering && m_unk94 == TRUE) {
        m_unk94 = FALSE;
    }

    ((CClump*)clump)->Render();
}

void CScene::RenderClumpFast(int id) {
    if (!m_rendering || id < 0 || id >= (int)m_clumps.size()) {
        return;
    }

    if (m_clumps[id] != NULL) {
        RenderClumpFast(m_clumps[id]);
    }
}

void CScene::RenderClumpFast(IClump* clump) {
    if (clump == NULL) {
        return;
    }

    if (!m_rendering) {
        return;
    }

    if (m_rendering && m_unk94 == TRUE) {
        m_unk94 = FALSE;
    }

    ((CClump*)clump)->RenderFast();
}

void CScene::RenderClumpRange(int starting_id, int n) {
    if (!m_rendering || starting_id < 0 || starting_id >= (int)m_clumps.size()) {
        return;
    }

    if (n <= 0) {
        return;
    }

    if (starting_id + n > (int)m_clumps.size()) {
        n = m_clumps.size() - starting_id;
    }

    for (int i = 0; i < n; i++) {
        if (m_clumps[i + starting_id] != NULL) {
            RenderClump(i + starting_id);
        }
    }
}

// Equivalent: scheduling
void CScene::RenderParticleEmitter(IParticleEmitter* emitter, BOOL a2) {
    unsigned long emitter_ul = reinterpret_cast<unsigned long>(emitter);

    if (a2 || m_rendering) {
        if (emitter_ul != NULL && reinterpret_cast<CParticleEmitter*>(emitter_ul)->GetNumberOfActiveParticles() != 0) {
            if (!a2) {
                reinterpret_cast<CParticleEmitter*>(emitter_ul)->Render();
                return;
            }

            for (int i = 0; i < (int)AS_ULONG_VECTOR_HACK(m_unk6C).size(); i++) {
                if (m_unk6C[i] == NULL) {
                    AS_ULONG_VECTOR_HACK(m_unk6C)[i] = emitter_ul;
                    return;
                }
            }

            AS_ULONG_VECTOR_HACK(m_unk6C).push_back(emitter_ul);
        }
    }
}

void CScene::RenderBatch2D(IIm2DBatch* batch_, RwPrimitiveType type, BOOL a3) {
    if ((a3 || m_rendering) && batch_ != NULL) {
        CIm2DBatch* batch = (CIm2DBatch*)batch_;
        if (!a3) {
            batch->Render(type, NULL);
            return;
        }

        unsigned long batch_ul = reinterpret_cast<unsigned long>(batch);
        batch->m_prim_type = type;

        for (int i = 0; i < (int)AS_ULONG_VECTOR_HACK(m_2d_batches).size(); i++) {
            if (m_2d_batches[i] == NULL) {
                AS_ULONG_VECTOR_HACK(m_2d_batches)[i] = batch_ul;
                return;
            }
        }

        AS_ULONG_VECTOR_HACK(m_2d_batches).push_back(batch_ul);
    }
}

void CScene::RenderLightHelper(ILight* light_) {
    if (!m_rendering) {
        return;
    }

    if (light_ == NULL) {
        return;
    }
    CLight* light = (CLight*)light_;

    switch (light->GetLightType()) {
        case LIGHT_TYPE_DIRECTIONAL:
            light->RenderLightDirectionnal();
            break;
        case LIGHT_TYPE_POINT:
            light->RenderLightOmni();
            break;
        case LIGHT_TYPE_SPOT:
        case LIGHT_TYPE_SOFTSPOT:
            light->RenderLightSpot();
            break;
    }
}

void CScene::Flush() {
    if (m_rendering) {
        m_selected_camera->Flush();
        FlushImmediate3D();
        FlushParticleEmitters();
        FlushImmediate2D();
    }
}

void CScene::FlushParticleEmitters() {
    if (!m_rendering) {
        return;
    }

    for (int i = 0; i < (int)m_unk6C.size(); i++) {
        if (m_unk6C[i] != NULL) {
            m_unk6C[i]->Render();
        }
        m_unk6C[i] = NULL;
    }
}

void CScene::FlushImmediate3D() {
    if (!m_rendering) {
        return;
    }

    for (int i = 0; i < (int)m_3d_batches.size(); i++) {
        SBATCH3DENTRY& entry = m_3d_batches[i];
        if (entry.batch != NULL) {
            entry.batch->Render(entry.prim_type, entry.matrix);
        }
        entry.batch = NULL;
    }
}

void CScene::FlushImmediate2D() {
    if (!m_rendering) {
        return;
    }

    for (int i = 0; i < (int)m_2d_batches.size(); i++) {
        if (m_2d_batches[i] != NULL) {
            m_2d_batches[i]->Render(m_2d_batches[i]->m_prim_type, NULL);
        }
        m_2d_batches[i] = NULL;
    }
}

void CScene::Flip(int a1) {
    if (m_rendering == TRUE) {
        EndRender();
    }
    m_selected_camera->m_wrap_camera->Show(a1);
}

void CScene::Screenshot(char* a1) {
    m_engine->Screenshot(a1, m_selected_camera->GetBuffer());
}

}
