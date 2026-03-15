#include <dolphin/gx.h>
#include "entities/CEntityBhvTagReceiveShadow.h"
#include "entities/CEntityMesh.h"
#include "entities/CEntityWorld.h"
#include "CGame.h"
#include <cstring>
#include <iostream>

CEntityMesh::CEntityMesh(CEntityManager* entity_manager, std::string name) : CEntityObject(entity_manager, name) {
    m_unkBC = (1.0f/90.0f);

    m_unkDC = 3.4028235e38f;
    m_unkE0 = 3.4028235e38f;
    m_unkE4 = 3.4028235e38f;
    m_unkE8 = 3.4028235e38f;

    m_mirror = NULL;
    m_unkF0 = FALSE;
    m_controller = NULL;
    m_animation_star = NULL;
    m_animation_star_controller = NULL;
    m_clump = NULL;

    m_cone_angle = 0.0f;
    m_cone_distance = 0.0f;
    m_cone_mv_ratio = 1.0f;
    m_cone_offset = 0.0f;

    m_attached_fxs.clear();
    m_attached_fxs.reserve(4);
    m_attached_snds.clear();
    m_attached_snds.reserve(4);

    m_sound_emitter = NULL;
    m_look_at_cam = FALSE;
    m_entity_light_01 = NULL;
}

// Equivalent: std::vector
CEntityMesh::~CEntityMesh() {
    if (m_mirror != NULL) {
        m_mirror->RemoveAllClumps();
        m_entity_manager->GetGame()->GetScene()->RemoveMirror(m_mirror);
        m_mirror = NULL;
    }

    if (m_controller != NULL) {
        m_entity_manager->GetGame()->m_anim_dictionary->RemoveController(m_controller->GetName());
        m_controller = NULL;
    }

    if (m_animation_star != NULL && *m_animation_star->GetName() == m_unk0) {
        m_entity_manager->GetGame()->m_anim_dictionary->RemoveAnimationStar(*m_animation_star->GetName());
        m_animation_star = NULL;
    }

    if (m_animation_star_controller != NULL) {
        m_entity_manager->GetGame()->m_anim_dictionary->RemoveAnimationStarController(*m_animation_star_controller->GetName());
        m_animation_star_controller = NULL;
    }

    if (m_clump != NULL) {
        m_entity_manager->GetGame()->GetScene()->RemoveClump(m_clump);
        m_clump = NULL;
    }

    if (m_sound_emitter != NULL) {
        m_entity_manager->GetGame()->m_sound_engine->RemoveEmitter(m_sound_emitter);
        m_sound_emitter = NULL;
    }

    for (int i = 0; i < (int)m_attached_fxs.size(); i++) {
        if (m_attached_fxs[i].emitter != NULL) {
            m_entity_manager->GetGame()->GetScene()->RemoveParticleEmitter(m_attached_fxs[i].emitter);
            m_attached_fxs[i].emitter = NULL;
        }
    }
}

void CEntityMesh::Reset() {
    CEntityObject::Reset();
}

void CEntityMesh::UpdateAnimations(F32 dt) {
    if (m_animation_star_controller != NULL) {
        m_animation_star_controller->Update(dt);
    }
    if (m_controller != NULL) {
        m_controller->Update(dt);
    }
}

void CEntityMesh::Update(F32 dt) {
    CEntityObject::Update(dt);

    if (m_look_at_cam == TRUE) {
        CDKW_V3d camera_pos = m_entity_manager->GetGame()->GetCamera()->GetPosition();
        m_clump->LookAtInline(camera_pos, CDKW_V3d::YAXIS);
    }

    if (IsFlagged(ENTITY_FLAG_ACTIVE) != FALSE && IsFlagged(ENTITY_FLAG_UNK7) != TRUE) {
        UpdateAnimations(dt);
        m_clump->Update(dt);
        if (GetType() == ENTITY_ESCAPING_OBJECT) {
            UpdateAttachedFX(dt, 0);
        } else {
            UpdateAttachedFX(dt, 1);
        }

        DKSND::CSoundEmitter* sound_emitter = m_sound_emitter;
        if (sound_emitter != NULL) {
            sound_emitter->SetPosition(GetPosition());
        }
    }
}

void CEntityMesh::AddFlag(U32 flag) {
    switch (flag) {
        case ENTITY_FLAG_UNK8:
            if (m_mirror != NULL) {
                m_entity_manager->GetGame()->GetScene()->EnableMirrorRendering(m_mirror, FALSE);
            }
            break;
        case ENTITY_FLAG_VISIBLE:
            if (m_mirror != NULL) {
                m_entity_manager->GetGame()->GetScene()->EnableMirrorRendering(m_mirror, TRUE);
            }
            break;
    }

    CEntityObject::AddFlag(flag);
}

void CEntityMesh::DelFlag(U32 flag) {
    switch (flag) {
        case ENTITY_FLAG_UNK8:
            if (m_mirror != NULL) {
                m_entity_manager->GetGame()->GetScene()->EnableMirrorRendering(m_mirror, TRUE);
            }
            break;
        case ENTITY_FLAG_VISIBLE:
            if (m_mirror != NULL) {
                m_entity_manager->GetGame()->GetScene()->EnableMirrorRendering(m_mirror, FALSE);
            }
            break;
    }

    CEntityObject::DelFlag(flag);
}

extern "C" {
extern void RwRenderStateSet(int, int);
extern void _rwDlRenderStateSetAlphaComp(int);
}

void CEntityMesh::Render(f32 dt) {
    if (!IsFlagged(ENTITY_FLAG_VISIBLE)) {
        return;
    }

    if (IsFlagged(ENTITY_FLAG_UNK8)) {
        return;
    }

    BeginLighting();
    BOOL aab_enabled = m_entity_manager->GetGame()->m_display_engine->IsAlphaAtomicBufferizationEnabled();

    if (m_unkF0 == TRUE) {
        m_entity_manager->GetGame()->m_display_engine->AlphaAtomicBufferization(TRUE);
    }

    m_entity_manager->GetGame()->m_display_engine->RegisterShadowMapValidationCallback(&m_smv_callback);

    if (m_mirror != NULL) {
        RwRenderStateSet(10, 2);
        RwRenderStateSet(11, 1);
        _rwDlRenderStateSetAlphaComp(3);
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
        m_entity_manager->GetGame()->m_display_engine->AlphaAtomicBufferization(FALSE);
    }

    m_entity_manager->GetGame()->GetScene()->RenderClump(m_clump);
    if (m_mirror != NULL) {
        m_entity_manager->GetGame()->m_display_engine->AlphaAtomicBufferization(TRUE);
        _rwDlRenderStateSetAlphaComp(2);
        _rwDlRenderStateSetAlphaComp(3);
        RwRenderStateSet(10, 5);
        RwRenderStateSet(11, 6);
    }

    m_entity_manager->GetGame()->m_display_engine->RegisterShadowMapValidationCallback(NULL);
    if (m_unkF0 == TRUE) {
        m_entity_manager->GetGame()->m_display_engine->AlphaAtomicBufferization(aab_enabled);
    }

    EndLighting();
    RenderAttachedFX(dt);

    CEntityObject::Render(dt);
}

void CEntityMesh::PlayAnimAnm(DKDSP::IGenericAnimation* animation, F32 a2) {
    m_controller->StopAllAnimations(DKDSP::ANIMATION_TYPE_ANM, 0);
    if (animation != NULL) {
        int unk1 = m_controller->PlayAnimation(animation, -1, 0.0f, 0.0f, a2, a2);
        if (unk1 >= 0) {
            int unk2 = m_controller->FindAnimationFromUID(DKDSP::ANIMATION_TYPE_ANM, unk1);
            if (unk2 >= 0) {
                m_controller->SetAnimationLoopMode(DKDSP::ANIMATION_TYPE_ANM, unk2, DKDSP::ANIMATION_LOOP_3, 0);
            }
        }
    }
}

void CEntityMesh::PlayAnimEvn(DKDSP::IGenericAnimation* animation, F32 a2) {
    m_controller->StopAllAnimations(DKDSP::ANIMATION_TYPE_EVN, 0);
    if (animation != NULL) {
        int unk1 = m_controller->PlayAnimation(animation, 0, 0.0f, 0.0f, a2, a2);
        if (unk1 >= 0) {
            int unk2 = m_controller->FindAnimationFromUID(DKDSP::ANIMATION_TYPE_EVN, unk1);
            if (unk2 >= 0) {
                m_controller->SetAnimationLoopMode(DKDSP::ANIMATION_TYPE_EVN, unk2, DKDSP::ANIMATION_LOOP_3, 0);
            }
        }
    }
}

void CEntityMesh::PlayAnimDma(DKDSP::IGenericAnimation* animation, U32 a2) {
    if (a2 == -1) {
        m_controller->StopAllAnimations(DKDSP::ANIMATION_TYPE_DMA, 0);
    }

    if (animation != NULL) {
        int unk1 = m_controller->PlayAnimation(animation, a2, 0.0f, 0.0f, 0.0f, 0.0f);
        if (unk1 >= 0) {
            int unk2 = m_controller->FindAnimationFromUID(DKDSP::ANIMATION_TYPE_DMA, unk1);
            if (unk2 >= 0) {
                m_controller->SetAnimationLoopMode(DKDSP::ANIMATION_TYPE_DMA, unk2, DKDSP::ANIMATION_LOOP_3, 0);
            }
        }
    }
}

void CEntityMesh::PlayAnimTan(DKDSP::IGenericAnimation* animation, U32 a2) {
    m_controller->StopAllAnimations(DKDSP::ANIMATION_TYPE_TAN, 0);
    if (animation != NULL) {
        int unk1 = m_controller->PlayAnimation(animation, a2, 0.0f, 0.0f, 0.0f, 0.0f);
        if (unk1 >= 0) {
            int unk2 = m_controller->FindAnimationFromUID(DKDSP::ANIMATION_TYPE_TAN, unk1);
            if (unk2 >= 0) {
                m_controller->SetAnimationLoopMode(DKDSP::ANIMATION_TYPE_TAN, unk2, DKDSP::ANIMATION_LOOP_3, 0);
            }
        }
    }
}

U32 CEntityMesh::Create(std::string mesh_filename) {
    if (m_clump != NULL) {
        return 0;
    }

    m_entity_manager->GetGame()->GetResourceFactory()->LoadResource(RESOURCE_TYPE_CLUMP, mesh_filename);
    BOOL created = (m_clump = m_entity_manager->GetGame()->GetScene()->CloneClump(mesh_filename, NULL)) != NULL;
    return created != FALSE;
}

void CEntityMesh::LoadAnimations(DkXmd::CChunkIterator iter) {
    char buf[128];
    DkXmd::CChunkIterator dest;

    if (iter.GetFirstChildChunk(dest) == TRUE) {
        do {
            strcpy(buf, dest.GetName());

            if (strcmp(buf, "ANM") == 0) {
                strcpy(buf, dest.GetStringValue());
                m_entity_manager->GetGame()->GetResourceFactory()->LoadResource(RESOURCE_TYPE_ANIMATION, buf);
            } else if (strcmp(buf, "DMA") == 0) {
                strcpy(buf, dest.GetStringValue());
                m_entity_manager->GetGame()->GetResourceFactory()->LoadResource(RESOURCE_TYPE_DMORPH_ANIMATION, buf);
            } else if (strcmp(buf, "TAN") == 0) {
                strcpy(buf, dest.GetStringValue());
                m_entity_manager->GetGame()->GetResourceFactory()->LoadResource(RESOURCE_TYPE_MATERIAL_ANIMATION, buf);
            } else if (strcmp(buf, "EVN") == 0) {
                strcpy(buf, dest.GetStringValue());
                m_entity_manager->GetGame()->GetResourceFactory()->LoadResource(RESOURCE_TYPE_EVENT_ANIMATION, buf);
            }

            LoadAnimations(dest); // Interesting play...
        } while (dest.GetNextSiblingChunk(dest) == TRUE);
    }
}

BOOL CEntityMesh::ParseStar(std::string str) {
    DkXmd::CChunkIterator iter;

    m_animation_star = m_entity_manager->GetGame()->m_anim_dictionary->FindAnimationStar(str);
    if (m_animation_star != NULL) {
        m_animation_star_controller = m_entity_manager->GetGame()->m_anim_dictionary->CreateAnimationStarController(m_unk0, m_animation_star, m_controller);
        if (m_animation_star_controller == NULL) {
            return FALSE;
        }

        return TRUE;
    } else {
        void* xmd_data = m_entity_manager->GetGame()->GetResourceFactory()->LoadPureFile(str, NULL);
        if (xmd_data == NULL) {
            return FALSE;
        }

        DkXmd::CXmdFile* xmd = new DkXmd::CXmdFile;
        if (!xmd->Parse(xmd_data)) {
            return FALSE;
        }

        iter = xmd->m_chunk_iterator;
        LoadAnimations(iter);
        delete xmd;
        delete xmd_data;

        m_entity_manager->GetGame()->GetResourceFactory()->LoadResource(RESOURCE_TYPE_ANIMATION_STAR, str);
        m_animation_star = m_entity_manager->GetGame()->m_anim_dictionary->FindAnimationStar(str);
        m_animation_star_controller = m_entity_manager->GetGame()->m_anim_dictionary->CreateAnimationStarController(m_unk0, m_animation_star, m_controller);
        if (m_animation_star_controller == NULL) {
            return FALSE;
        }

        return TRUE;
    }
}

BOOL CEntityMesh::ParseStar(DkXmd::CChunkIterator iter) {
    LoadAnimations(iter);

    m_animation_star = m_entity_manager->GetGame()->m_anim_dictionary->LoadAnimationStarFromChunk(m_unk0, &iter);
    if (m_animation_star == NULL) {
        return FALSE;
    }

    m_animation_star_controller = m_entity_manager->GetGame()->m_anim_dictionary->CreateAnimationStarController(m_unk0, m_animation_star, m_controller);
    if (m_animation_star_controller == NULL) {
        return FALSE;
    }

    return TRUE;
}

void CEntityMesh::Parse(DkXmd::CChunkIterator iter) {
    char buf[128];
    DkXmd::CChunkIterator dest;

    if (iter.GetChunk("TexDict", dest)) {
        strcpy(buf, dest.GetStringValue());
        m_entity_manager->GetGame()->GetResourceFactory()->LoadResource(RESOURCE_TYPE_TEXTURE_DICTIONARY, buf);
    }

    if (iter.GetChunk("SndDict", dest)) {
        strcpy(buf, dest.GetStringValue());
        m_entity_manager->GetGame()->GetResourceFactory()->LoadResource(RESOURCE_TYPE_SAMPLE_BANK1, buf);
    }

    if (m_entity_manager->GetGame()->m_unk4F5C == 1 && iter.GetChunk("SndDictTigger", dest)) {
        strcpy(buf, dest.GetStringValue());
        m_entity_manager->GetGame()->GetResourceFactory()->LoadResource(RESOURCE_TYPE_SAMPLE_BANK1, buf);
    }

    if (m_entity_manager->GetGame()->m_unk4F5C == 2 && iter.GetChunk("SndDictWinnie", dest)) {
        strcpy(buf, dest.GetStringValue());
        m_entity_manager->GetGame()->GetResourceFactory()->LoadResource(RESOURCE_TYPE_SAMPLE_BANK1, buf);
    }

    if (m_entity_manager->GetGame()->m_unk4F5C == 3 && iter.GetChunk("SndDictCatch", dest)) {
        strcpy(buf, dest.GetStringValue());
        m_entity_manager->GetGame()->GetResourceFactory()->LoadResource(RESOURCE_TYPE_SAMPLE_BANK1, buf);
    }

    if (iter.GetChunk("Mesh", dest)) {
        Create(dest.GetStringValue());
        if (m_controller == NULL) {
            m_controller = m_entity_manager->GetGame()->m_anim_dictionary->CreateController(m_unk0, m_clump, 4);
            if (m_controller != NULL) {
                m_controller->SetEventUserData(this);
                m_clump->SetController(m_controller);
                m_controller->Play(0.0f);
            }
        }
    }

    CEntityObject::Parse(iter);

    if (iter.GetFirstChildChunk(dest) == TRUE) {
        do {
            strcpy(buf, dest.GetName());

            if (strcmp(buf, "AtomicRenderStates") == 0) {
                if (m_clump != NULL) {
                    m_clump->LoadRenderStates(&dest);
                    m_unkF0 = TRUE;
                }
            }

            if (strcmp(buf, "AnimationStar") == 0 || strcmp(buf, "AnimationStarFile") == 0) {
                if (dest.GetType() != 1) {
                    ParseStar(dest.GetStringValue());
                } else {
                    ParseStar(dest);
                }
            }

            if (strcmp(buf, "MirrorDefinition") == 0) {
                ParseMirror(dest);
            }

            if (strcmp(buf, "ConeAngle") == 0) {
                // Convert from degrees to radians
                m_cone_angle = 3.1415927f * (dest.GetFloatValue() / 180.0f);
            } else if (strcmp(buf, "ConeDistance") == 0) {
                m_cone_distance = dest.GetFloatValue();
            } else if (strcmp(buf, "ConeMvRatio") == 0) {
                m_cone_mv_ratio = dest.GetFloatValue();
            } else if (strcmp(buf, "ConeOffset") == 0) {
                m_cone_offset = dest.GetFloatValue();
            } else if (strcmp(buf, "AttachedFX") == 0) {
                ParseAttachedFX(dest);
            } else if (strcmp(buf, "AttachedSND") == 0) {
                ParseAttachedSND(dest);
            } else if (strcmp(buf, "LookAtCam") == 0) {
                m_look_at_cam = TRUE;
            }
        } while (dest.GetNextSiblingChunk(dest) == TRUE);
    }

    for (std::vector<DkPh::BVolume*>::iterator iter = m_unk24.begin(); iter < m_unk24.end(); iter++) {
        if (m_entity_manager->m_unk1C->GetBodyRef(*iter).unk28 == 1) {
            DkPh::BVolume* volume = *iter;
            m_unk90.unk0 = volume;
            volume->m_unk1C = &m_unk90;
            break;
        }
    }

    m_unk90.unk8 = CDKW_V3d::ZERO;
    m_unk90.unk14 = CDKW_V3d::ZERO;

    UpdateAnimations(1.0f/30.0f);
    if (m_clump != NULL) {
        m_clump->Update(1.0f/30.0f);
    }
}

void CEntityMesh::ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior) {
    char buf[128];
    DkXmd::CChunkIterator dest;

    CEntityObject::ParseBehavior(iter, behavior);

    if (iter.GetFirstChildChunk(dest) == TRUE) {
        do {
            strcpy(buf, dest.GetName());
            if (strcmp(buf, "ReceiveShadow") == 0) {
                CEntityBhvTagReceiveShadow* shadow = new CEntityBhvTagReceiveShadow;
                shadow->Parse(dest);
                behavior->AddData(shadow);
            }
        } while (dest.GetNextSiblingChunk(dest) == TRUE);
    }
}

void CEntityMesh::RenderAttachedFX(F32 dt) {
    for (int i = 0; i < (int)m_attached_fxs.size(); i++) {
        if (m_attached_fxs[i].emitter != NULL) {
            m_entity_manager->GetGame()->GetScene()->RenderParticleEmitter(m_attached_fxs[i].emitter, 1);
        }
    }
}

// Equivalent
void CEntityMesh::StopAttachedFX(int id) {
    for (int i = 0; i < (int)m_attached_fxs.size(); i++) {
        if (id == m_attached_fxs[i].id) {
            m_attached_fxs[i].emitter->Stop();
            return;
        }
    }
}

// Equivalent
void CEntityMesh::StartAttachedFX(int id) {
    for (int i = 0; i < (int)m_attached_fxs.size(); i++) {
        if (id == m_attached_fxs[i].id) {
            m_attached_fxs[i].emitter->Start();
            return;
        }
    }
}

void CEntityMesh::ResumeAllAttachedFX() {
    for (int i = 0; i < (int)m_attached_fxs.size(); i++) {
        if (m_attached_fxs[i].emitter != NULL) {
            m_attached_fxs[i].emitter->Resume();
        }
    }
}

void CEntityMesh::PauseAllAttachedFX() {
    for (int i = 0; i < (int)m_attached_fxs.size(); i++) {
        if (m_attached_fxs[i].emitter != NULL) {
            m_attached_fxs[i].emitter->Pause();
        }
    }
}

void CEntityMesh::StopAllAttachedFX() {
    for (int i = 0; i < (int)m_attached_fxs.size(); i++) {
        if (m_attached_fxs[i].emitter != NULL) {
            m_attached_fxs[i].emitter->Stop();
        }
    }
}

// Equivalent
DKSND::CSample* CEntityMesh::GetSampleById(int id) {
    for (int i = 0; i < (int)m_attached_snds.size(); i++) {
        if (id == m_attached_snds[i].id) {
            return m_attached_snds[i].sample;
        }
    }

    return NULL;
}

// Equivalent
F32 CEntityMesh::GetPitchVariationById(int id) {
    for (int i = 0; i < (int)m_attached_snds.size(); i++) {
        if (id == m_attached_snds[i].id) {
            return m_attached_snds[i].pitch_variation;
        }
    }

    return 0.0f;
}

void CEntityMesh::EndLighting() {
    if (m_entity_manager->GetGame()->GetShadowZone()->GetImage() == NULL) {
        return;
    }

    m_entity_manager->GetGame()->GetScene()->SetAmbient(m_unk60.m_r, m_unk60.m_g, m_unk60.m_b);
    if (m_entity_light_01 != NULL) {
        m_entity_light_01->GetLight()->SetColor(m_unk70.m_r, m_unk70.m_g, m_unk70.m_b);
    }

    m_entity_manager->GetGame()->m_display_engine->AlphaAtomicBufferization(TRUE);
}

BOOL CEntityMesh::ParseMirror(DkXmd::CChunkIterator iter) {
    F32 width = 1.0f;
    F32 height = 1.0f;
    U32 texture_width = 64;
    U32 texture_height = 64;

    DkXmd::CChunkIterator dest1, dest2;

    if (iter.GetChunk("MirrorWidth", dest1)) {
        width = dest1.GetFloatValue();
    }
    if (iter.GetChunk("MirrorHeight", dest1)) {
        height = dest1.GetFloatValue();
    }
    if (iter.GetChunk("MirrorTextureWidth", dest1)) {
        texture_width = dest1.GetS32Value();
    }
    if (iter.GetChunk("MirrorTextureHeight", dest1)) {
        texture_height = dest1.GetS32Value();
    }

    m_mirror = m_entity_manager->GetGame()->GetScene()->CreateMirror(texture_width, texture_height, 16);
    if (m_mirror != NULL) {
        m_mirror->SetSize(width, height);
        m_entity_manager->GetGame()->GetScene()->EnableMirrorRendering(m_mirror, TRUE);
        m_entity_manager->GetGame()->GetScene()->AssociateClumpToMirror(m_mirror, m_clump);

        if (iter.GetChunk("EntityList", dest1)) {
            if (dest1.GetFirstChildChunk(dest2)) {
                do {
                    if (strcmp(dest2.GetName(), "Entity") == 0) {
                        CEntity* entity = m_entity_manager->GetEntity(dest2.GetStringValue());
                        if (entity != NULL) {
                            if (entity->GetType() == ENTITY_WORLD) {
                                m_mirror->AddClump(((CEntityWorld*)entity)->GetClump());
                            } else {
                                m_mirror->AddClump(((CEntityMesh*)entity)->m_clump);
                            }
                        }
                    }
                } while (dest2.GetNextSiblingChunk(dest2));
            }
        }
    }

    return TRUE;
}

BOOL CEntityMeshShadowMapValidationCallback::ValidateObjectDisplay(CDKW_Atomic*) {
    return m_active;
}

BOOL CEntityMeshShadowMapValidationCallback::ValidateDisplay(DKDSP::IShadowMap*, DKDSP::ICamera*, CDKW_Atomic*, CDKW_Sphere*) {
    return m_active;
}
