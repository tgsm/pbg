#include <dolphin/gx.h>
#include "entities/CEntityBhvTagReceiveShadow.h"
#include "entities/CEntityMesh.h"
#include "entities/CEntityWorld.h"
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
    m_unkC8 = NULL;
    m_animation_star_controller = NULL;
    m_clump = NULL;

    m_unkCC = 0.0f;
    m_unkD0 = 0.0f;
    m_unkD4 = 1.0f;
    m_unkD8 = 0.0f;

    m_attached_fxs.clear();
    m_attached_fxs.reserve(4);
    m_attached_snds.clear();
    m_attached_snds.reserve(4);

    m_unk88 = NULL;
    m_unkEC = FALSE;
    m_entity_light_01 = NULL;
}

// TODO
CEntityMesh::~CEntityMesh() {

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

void CEntityMesh::AddFlag(U32 flag) {
    switch (flag) {
        case (1 << 8):
            if (m_mirror != NULL) {
                m_entity_manager->GetGame()->GetScene()->EnableMirrorRendering(m_mirror, FALSE);
            }
            break;
        case (1 << 1):
            if (m_mirror != NULL) {
                m_entity_manager->GetGame()->GetScene()->EnableMirrorRendering(m_mirror, TRUE);
            }
            break;
    }

    CEntityObject::AddFlag(flag);
}

void CEntityMesh::DelFlag(U32 flag) {
    switch (flag) {
        case (1 << 8):
            if (m_mirror != NULL) {
                m_entity_manager->GetGame()->GetScene()->EnableMirrorRendering(m_mirror, TRUE);
            }
            break;
        case (1 << 1):
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
    if (!IsFlagged(1 << 1)) {
        return;
    }

    if (IsFlagged(1 << 8)) {
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

    m_entity_manager->GetGame()->GetResourceFactory()->LoadResource(1, mesh_filename);
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
                m_entity_manager->GetGame()->GetResourceFactory()->LoadResource(3, buf);
            } else if (strcmp(buf, "DMA") == 0) {
                strcpy(buf, dest.GetStringValue());
                m_entity_manager->GetGame()->GetResourceFactory()->LoadResource(4, buf);
            } else if (strcmp(buf, "TAN") == 0) {
                strcpy(buf, dest.GetStringValue());
                m_entity_manager->GetGame()->GetResourceFactory()->LoadResource(5, buf);
            } else if (strcmp(buf, "EVN") == 0) {
                strcpy(buf, dest.GetStringValue());
                m_entity_manager->GetGame()->GetResourceFactory()->LoadResource(8, buf);
            }

            LoadAnimations(dest); // Interesting play...
        } while (dest.GetNextSiblingChunk(dest) == TRUE);
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
