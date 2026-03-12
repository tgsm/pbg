#include "engine/wrap/DKW_Material.h"
#include "entities/CEntityWorld.h"
#include "CGame.h"
#include <math.h>
#include <cstring>
#include <iostream>

CEntityWorldShadowMapValidationCallback ShadowMapValidationCallback;

U32 nAlphaRefValues[] = {
    0x00, 0x1F, 0x2F, 0x5F, 0x7F, 0x85, 0x89, 0x8D, 0x90, 0x95, 0x99, 0x9D, 0xA0, 0xA9, 0xAF, 0xB9,
    0xBF, 0xC4, 0xC8, 0xCA, 0xCD, 0xD0, 0xD4, 0xD8, 0xDA, 0xDD, 0xDE, 0xE0, 0xE1, 0xE4, 0xE5, 0xE8,
};
static BOOL bAllowShadow = TRUE;
static BOOL bAllowDisableAlpha = TRUE;

BOOL CEntityWorldShadowMapValidationCallback::ValidateObjectDisplay(CDKW_Atomic*) {
    if (bAllowDisableAlpha == TRUE) {
        DKDSP::CEngine::ms_pEngine->DisableAlphaTest();
    }
    return bAllowDisableAlpha;
}

BOOL CEntityWorldShadowMapValidationCallback::ValidateDisplay(DKDSP::IShadowMap*, DKDSP::ICamera*, CDKW_Atomic*, CDKW_Sphere*) {
    if (bAllowDisableAlpha == TRUE) {
        DKDSP::CEngine::ms_pEngine->DisableAlphaTest();
    }
    return !bAllowDisableAlpha ? FALSE : TRUE;
}

CEntityWorld::CEntityWorld(CEntityManager* entity_manager, std::string name) : CEntityObject(entity_manager, name) {
    m_clump = NULL;
    m_controller = NULL;
    m_unk48 = FALSE;
    m_unk4C = 0.0f;
}

CEntityWorld::~CEntityWorld() {
    if (m_clump != NULL) {
        m_entity_manager->GetGame()->GetScene()->RemoveClump(m_clump);
    }

    if (m_controller != NULL) {
        m_entity_manager->GetGame()->m_anim_dictionary->RemoveController(m_controller->GetName());
    }

    m_clump = NULL;
    m_controller = NULL;
}

void CEntityWorld::Reset() {
    CEntityObject::Reset();
}

void CEntityWorld::Update(F32 dt) {
    if (IsFlagged(ENTITY_FLAG_ACTIVE) && !IsFlagged(ENTITY_FLAG_UNK7)) {
        m_controller->Update(dt);
        m_clump->Update(dt);
        m_unk4C += dt;
    }
}

void CEntityWorld::Render(F32 dt) {
    if (IsFlagged(ENTITY_FLAG_VISIBLE) && !IsFlagged(ENTITY_FLAG_UNK8)) {
        if (m_unk48 == TRUE) {
            CDKW_Clump* wrap_clump = m_clump->GetDkWrapClump();
            CDKW_Atomic* atomic = wrap_clump->GetFirstAtomic();
            while (atomic != NULL) {
                CDKW_Geometry* wrap_geo = CDKW_Geometry::GetInstance(atomic->m_rw_atomic->geometry);
                if (wrap_geo != NULL) {
                    for (int i = 0; i < wrap_geo->GetNumberOfMaterials(); i++) {
                        CDKW_Material* wrap_material = CDKW_Material::GetInstance(wrap_geo->GetMaterial(i));
                        if (wrap_material != NULL && wrap_material->m_rw_material->texture != NULL) {
                            wrap_material->m_rw_material->texture->filterAddressing = (wrap_material->m_rw_material->texture->filterAddressing & ~0xFF) | (1 << 1);
                        }
                    }
                }

                atomic = wrap_clump->GetNextAtomic();
            }

            m_entity_manager->GetGame()->m_display_engine->AlphaAtomicBufferization(FALSE);
            m_entity_manager->GetGame()->m_display_engine->DisableAlphaTest();
            m_entity_manager->GetGame()->m_display_engine->RegisterShadowMapValidationCallback(&ShadowMapValidationCallback);
        } else {
            m_entity_manager->GetGame()->m_display_engine->RegisterShadowMapValidationCallback(NULL);
        }

        bAllowShadow = TRUE;
        bAllowDisableAlpha = TRUE;

        m_entity_manager->GetGame()->GetScene()->RenderClump(m_clump);

        if (m_unk48 == TRUE) {
            bAllowShadow = FALSE;
            bAllowDisableAlpha = FALSE;
            m_entity_manager->GetGame()->m_display_engine->PrepareAlphaTest();

            for (int i = 0; i < 4; i++) {
                if (i == 3) {
                    bAllowShadow = TRUE;
                }

                m_entity_manager->GetGame()->m_display_engine->SetAlphaTest(nAlphaRefValues[i + 1]);

                CDKW_Clump* wrap_clump = m_clump->GetDkWrapClump();
                CDKW_Atomic* atomic = wrap_clump->GetFirstAtomic();
                F32 dVar20 = 0.015f * i;
                while (atomic != NULL) {
                    RwFrameGetLTM(atomic->GetRwFrame())->pos.y += 0.055f;
                    F32 dVar15 = sin((3.14159f * m_unk4C) / 3.0f);
                    RwFrameGetLTM(atomic->GetRwFrame())->pos.x += dVar20 * dVar15;
                    dVar15 = cos((3.14159f * m_unk4C) / 4.0f);
                    RwFrameGetLTM(atomic->GetRwFrame())->pos.z += dVar20 * dVar15;

                    atomic = wrap_clump->GetNextAtomic();
                }

                m_entity_manager->GetGame()->GetScene()->RenderClump(m_clump);
            }

            m_entity_manager->GetGame()->m_display_engine->RestoreAlphaTest();
            m_entity_manager->GetGame()->m_display_engine->AlphaAtomicBufferization(TRUE);
            m_clump->Update(0.0f);
        }

        bAllowShadow = TRUE;
        m_entity_manager->GetGame()->m_display_engine->RegisterShadowMapValidationCallback(NULL);
    }
}

void CEntityWorld::Parse(DkXmd::CChunkIterator iter) {
    DkXmd::CChunkIterator dest;

    CEntityObject::Parse(iter);

    char buf1[128];
    char buf2[128];

    if (iter.GetChunk("TexDict", dest)) {
        strcpy(buf1, dest.GetStringValue());
        m_entity_manager->GetGame()->GetResourceFactory()->LoadResource(RESOURCE_TYPE_TEXTURE_DICTIONARY, buf1);
    }

    int iVar11 = 24;
    if (iter.GetChunk("SubAnimsNumber", dest)) {
        iVar11 = dest.GetS32Value();
    }

    if (iter.GetChunk("Mesh", dest)) {
        strcpy(buf2, dest.GetStringValue());
        m_entity_manager->GetGame()->GetResourceFactory()->LoadResource(RESOURCE_TYPE_CLUMP, buf2);
        m_clump = m_entity_manager->GetGame()->GetScene()->CloneClump(buf2, NULL);
        if (m_clump != NULL) {
            m_controller = m_entity_manager->GetGame()->m_anim_dictionary->CreateController(m_unk0, m_clump, iVar11 + 1);
            m_controller->SetEventUserData(this);
            m_clump->SetController(m_controller);
            m_entity_manager->GetGame()->GetScene()->SetupClumpToReceiveShadowMap(m_clump, 0);
        }
    }

    if (iter.GetFirstChildChunk(dest) == TRUE) {
        do {
            strcpy(buf1, dest.GetName());

            if (strcmp(buf1, "Grass") == 0) {
                m_unk48 = TRUE;
            }

            if (strcmp(buf1, "AtomicRenderStates") == 0) {
                if (m_clump != NULL) {
                    m_clump->LoadRenderStates(&dest);
                }
            }

            if (strcmp(buf1, "DefaultAnimation") == 0) {
                strcpy(buf2, dest.GetStringValue());
                m_entity_manager->GetGame()->GetResourceFactory()->LoadResource(RESOURCE_TYPE_ANIMATION, buf2);

                if (m_controller != NULL) {
                    m_controller->PlayAnimation(m_entity_manager->GetGame()->m_anim_dictionary->FindAnimation(buf2), -1, 0.0f, 0.0f, 0.0f, 0.0f);
                    m_controller->Play(0.0f);
                }
            } else if (strcmp(buf1, "Animation") == 0) {
                DkXmd::CChunkIterator dest2;
                if (dest.GetFirstChildChunk(dest2)) {
                    F32 frame = 0.0f;
                    int node = -1;
                    DKDSP::CMaterialAnimation* material_anim = NULL;
                    DKDSP::CAnimation* anim = NULL;
                    DKDSP::CDMorphAnimation* dmorph_anim = NULL;

                    do {
                        strcpy(buf1, dest2.GetName());

                        if (strcmp(buf1, "Frame") == 0) {
                            frame = dest2.GetFloatValue();
                        } else if (strcmp(buf1, "TAN") == 0) {
                            strcpy(buf2, dest2.GetStringValue());
                            m_entity_manager->GetGame()->GetResourceFactory()->LoadResource(RESOURCE_TYPE_MATERIAL_ANIMATION, buf2);
                            material_anim = m_entity_manager->GetGame()->m_anim_dictionary->FindMaterialAnimation(buf2);
                        } else if (strcmp(buf1, "DMA") == 0) {
                            strcpy(buf2, dest2.GetStringValue());
                            m_entity_manager->GetGame()->GetResourceFactory()->LoadResource(RESOURCE_TYPE_DMORPH_ANIMATION, buf2);
                            dmorph_anim = m_entity_manager->GetGame()->m_anim_dictionary->FindDMorphAnimation(buf2);
                        } else if (strcmp(buf1, "ANM") == 0) {
                            strcpy(buf2, dest2.GetStringValue());
                            m_entity_manager->GetGame()->GetResourceFactory()->LoadResource(RESOURCE_TYPE_ANIMATION, buf2);
                            anim = m_entity_manager->GetGame()->m_anim_dictionary->FindAnimation(buf2);
                        } else if (strcmp(buf1, "Node") == 0) {
                            node = dest2.GetS32Value();
                        }
                    } while (dest2.GetNextSiblingChunk(dest2));

                    if (m_controller != NULL) {
                        if (material_anim != NULL) {
                            m_controller->PlayAnimation(material_anim, node, 0.0f, frame, 0.0f, 0.0f);
                        }
                        if (dmorph_anim != NULL) {
                            m_controller->PlayAnimation(dmorph_anim, node, 0.0f, frame, 0.0f, 0.0f);
                        }
                        if (anim != NULL) {
                            m_controller->PlayAnimation(anim, node, 0.0f, frame, 0.0f, 0.0f);
                        }
                        m_controller->Play(0.0f);
                    }
                }
            }
        } while (dest.GetNextSiblingChunk(dest) == TRUE);
    }
}

void CEntityWorld::ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior) {
    CEntityObject::ParseBehavior(iter, behavior);
}
