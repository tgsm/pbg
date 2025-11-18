#include "entities/CEntityLight.h"
#include "entities/CEntityMesh.h"
#include "CGame.h"
#include <cstring>
#include <iostream>

CEntityLight::CEntityLight(CEntityManager* entity_manager, std::string name) : CEntityObject(entity_manager, name) {
    m_light4C = NULL;
    m_shadow_params.reserve(8);
}

// Equivalent: std::vector
CEntityLight::~CEntityLight() {
    for (U32 i = 0; i < m_shadow_params.size(); i++) {
        m_entity_manager->GetGame()->GetScene()->RemoveShadowMap(m_shadow_params[i].shadow_map);
        m_entity_manager->GetGame()->GetScene()->RemoveLight(m_shadow_params[i].light);
    }
    m_shadow_params.clear();

    if (m_light4C != NULL) {
        m_entity_manager->GetGame()->GetScene()->RemoveLight(m_light4C);
    }
    m_light4C = NULL;
}

void CEntityLight::Reset() {
    CEntityObject::Reset();
}

BOOL CEntityLight::Create(std::string type) {
    if (m_light4C != NULL) {
        return FALSE;
    }

    if (type == "Directional") {
        m_light4C = m_entity_manager->GetGame()->GetScene()->CreateLight(DKDSP::LIGHT_TYPE_DIRECTIONAL);
    } else if (type == "Point") {
        m_light4C = m_entity_manager->GetGame()->GetScene()->CreateLight(DKDSP::LIGHT_TYPE_POINT);
    } else if (type == "Spot") {
        m_light4C = m_entity_manager->GetGame()->GetScene()->CreateLight(DKDSP::LIGHT_TYPE_SPOT);
    } else if (type == "SoftSpot") {
        m_light4C = m_entity_manager->GetGame()->GetScene()->CreateLight(DKDSP::LIGHT_TYPE_SOFTSPOT);
    } else {
        return FALSE;
    }

    return TRUE;
}

void CEntityLight::Render(F32 dt) {

}

void CEntityLight::Parse(DkXmd::CChunkIterator iter) {
    char buf[128];
    DkXmd::CChunkIterator dest1, dest2;

    int shadowmap_width = 128;
    int shadowmap_height = 128;

    F32 red;
    F32 green;
    F32 blue;
    F32 view_height;
    F32 view_width;

    red = blue = green = 0.5f;
    view_width = view_height = 1.0f;

    CEntityObject::Parse(iter);

    if (iter.GetChunk("LightType", dest1)) {
        Create(dest1.GetStringValue());
    }

    if (iter.GetChunk("CastShadow", dest1)) {
        strcpy(buf, dest1.GetStringValue());

        if (strcmp(buf, "On") == 0) {
            if (m_light4C->GetLightType() == DKDSP::LIGHT_TYPE_DIRECTIONAL) {
                view_height = view_width = 3.0f;
            } else {
                view_height = view_width = 1.0f;
            }

            if (iter.GetChunk("ShadowMap", dest2)) {
                DkXmd::CChunkIterator shadowmap_iter;
                if (dest2.GetChunk("W", shadowmap_iter)) {
                    shadowmap_width = shadowmap_iter.GetS32Value();
                }
                if (dest2.GetChunk("H", shadowmap_iter)) {
                    shadowmap_height = shadowmap_iter.GetS32Value();
                }
            }

            if (iter.GetChunk("ShadowColor", dest2)) {
                DkXmd::CChunkIterator shadowcolor_iter;
                if (dest2.GetChunk("R", shadowcolor_iter)) {
                    red = shadowcolor_iter.GetFloatValue();
                }
                if (dest2.GetChunk("G", shadowcolor_iter)) {
                    green = shadowcolor_iter.GetFloatValue();
                }
                if (dest2.GetChunk("B", shadowcolor_iter)) {
                    blue = shadowcolor_iter.GetFloatValue();
                }
            }

            if (iter.GetChunk("ShadowMapViewWindow", dest2)) {
                DkXmd::CChunkIterator smvw_iter;
                if (dest2.GetChunk("W", smvw_iter)) {
                    view_width = smvw_iter.GetFloatValue();
                }
                if (dest2.GetChunk("H", smvw_iter)) {
                    view_height = smvw_iter.GetFloatValue();
                }
            }
        }
    }

    if (iter.GetFirstChildChunk(dest1) == TRUE) {
        do {
            strcpy(buf, dest1.GetName());

            if (strcmp(buf, "Color") == 0) {
                if (dest1.GetFirstChildChunk(dest2) == TRUE) {
                    do {
                        strcpy(buf, dest2.GetName());

                        if (strcmp(buf, "R") == 0) {
                            F32 value = dest2.GetFloatValue();
                            m_light4C->SetColor(value, m_light4C->GetColorGreen(), m_light4C->GetColorBlue());
                        } else if (strcmp(buf, "G") == 0) {
                            F32 value = dest2.GetFloatValue();
                            m_light4C->SetColor(m_light4C->GetColorRed(), value, m_light4C->GetColorBlue());
                        } else if (strcmp(buf, "B") == 0) {
                            F32 value = dest2.GetFloatValue();
                            m_light4C->SetColor(m_light4C->GetColorRed(), m_light4C->GetColorGreen(), value);
                        }
                    } while (dest2.GetNextSiblingChunk(dest2) == TRUE);
                }
            } else if (strcmp(buf, "Radius") == 0) {
                m_light4C->SetRadius(dest1.GetFloatValue());
            } else if (strcmp(buf, "ConeAngle") == 0) {
                m_light4C->SetConeAngle(dest1.GetFloatValue());
            } else if (strcmp(buf, "ShadowEmitter") == 0) {
                strcpy(buf, dest1.GetStringValue());
                CEntityMesh* mesh = (CEntityMesh*)m_entity_manager->GetEntity(buf);
                if (mesh != NULL) {
                    SShadowParams params;
                    params.unk8 = mesh;
                    params.light = m_entity_manager->GetGame()->GetScene()->CopyLight(m_light4C);
                    m_entity_manager->GetGame()->GetScene()->EnableLight(params.light, 0);
                    params.shadow_map = m_entity_manager->GetGame()->GetScene()->CreateShadowMap(params.light, shadowmap_width, shadowmap_height, 16);
                    params.color.m_r = red;
                    params.color.m_g = green;
                    params.color.m_b = blue;
                    params.shadow_map->SetShadowColor(red, green, blue);
                    params.shadow_map->GetCamera()->SetViewWindow(view_width, view_height);
                    params.shadow_map->SetBlurLevel(2);
                    params.shadow_map->AddShadowEmitter(params.unk8->m_clump);

                    m_shadow_params.push_back(params);
                }
            }
        } while (dest1.GetNextSiblingChunk(dest1) == TRUE);
    }
}

void CEntityLight::ParseBehavior(DkXmd::CChunkIterator iter, CEntityBhvTagBehavior* behavior) {
    CEntityObject::ParseBehavior(iter, behavior);
}

void CEntityLight::DelFlag(U32 flag) {
    if (flag & ENTITY_FLAG_ACTIVE) {
        m_entity_manager->GetGame()->GetScene()->EnableLight(m_light4C, 0);
        flag |= ENTITY_FLAG_VISIBLE;
    }

    CEntityObject::DelFlag(flag);
}

void CEntityLight::AddFlag(U32 flag) {
    if (flag & ENTITY_FLAG_ACTIVE) {
        m_entity_manager->GetGame()->GetScene()->EnableLight(m_light4C, 1);
        flag |= ENTITY_FLAG_VISIBLE;
    }

    CEntityObject::AddFlag(flag);
}
