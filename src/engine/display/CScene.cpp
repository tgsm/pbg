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
    m_unk98 = 0;
    m_unkD4 = 0;
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
    m_unk98 = 0;

    AS_ULONG_VECTOR_HACK(m_unk6C).reserve(16);
    m_unk78.reserve(16);
    AS_ULONG_VECTOR_HACK(m_unk84).reserve(16);
    AS_ULONG_VECTOR_HACK(m_unk18).reserve(16);
    AS_ULONG_VECTOR_HACK(m_lights).reserve(16);
    AS_ULONG_VECTOR_HACK(m_cameras).reserve(16);
    AS_ULONG_VECTOR_HACK(m_shadow_maps).reserve(16);
    AS_ULONG_VECTOR_HACK(m_unk48).reserve(16);
    AS_ULONG_VECTOR_HACK(m_unk54).reserve(16);
    AS_ULONG_VECTOR_HACK(m_unk60).reserve(16);
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

    for (int i = 0; i < (int)m_unk78.size(); i++) {
        SBATCH3DENTRY& entry = m_unk78[i];
        if (entry.matrix != NULL) {
            RwMatrixDestroy(entry.matrix);
        }
        entry.matrix = NULL;
    }

    m_unk78.clear();
    m_unk6C.__unknown_reset_function();
    m_unk84.__unknown_reset_function();

    ReleaseCollisionBSP();
    if (m_world != NULL) {
        delete m_world;
    }
    m_world = NULL;

    m_object_dictionary = NULL;
    m_selected_camera = NULL;
    m_unk98 = 0;
    m_unkD4 = 0;
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

int CScene::CreateShadowMapGetIndex(ILight* light, U32 a2, U32 a3, U32 a4) {
    CShadowMap* shadow_map = new CShadowMap;
    unsigned long shadow_map_ul = reinterpret_cast<unsigned long>(shadow_map);

    shadow_map->Init(m_engine, this, light, a2, a3, a4);

    for (int i = 0; i < (int)AS_ULONG_VECTOR_HACK(m_shadow_maps).size(); i++) {
        if (m_shadow_maps[i] == NULL) {
            AS_ULONG_VECTOR_HACK(m_shadow_maps)[i] = shadow_map_ul;
            return i;
        }
    }

    AS_ULONG_VECTOR_HACK(m_shadow_maps).push_back(shadow_map_ul);
    return m_shadow_maps.size() - 1;
}

CShadowMap* CScene::CreateShadowMap(ILight* light, U32 a2, U32 a3, U32 a4) {
    int index = CreateShadowMapGetIndex(light, a2, a3, a4);
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
        m_colorC0 = m_colorB0;
    }

    ((CShadowMap*)shadow_map)->Render();

    if (m_unk98) {
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
        m_colorC0 = m_colorB0;
    }

    for (int i = 0; i < (int)m_shadow_maps.size(); i++) {
        CShadowMap* shadow_map = m_shadow_maps[i];
        if (shadow_map != NULL && shadow_map->IsEnabled() == TRUE && shadow_map->IsDisplayEnabled() == TRUE) {
            SetAmbient(shadow_map->GetShadowColorRed(), shadow_map->GetShadowColorGreen(), shadow_map->GetShadowColorBlue());
            shadow_map->Prepare();
            shadow_map->Render();
        }
    }

    if (m_unk98) {
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
