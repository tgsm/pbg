#include "CGame.h"
#include "CLoadingTigger.h"
#include "engine/wrap/DKW_Geometry.h"
#include "engine/wrap/DKW_Material.h"
#include <iostream>

CLoadingTigger::CLoadingTigger(CGame* game) : CLoadingCallback(game) {

}

CLoadingTigger::~CLoadingTigger() {
    Destroy();
}

void CLoadingTigger::Create() {
    CLoadingCallback::Create();

    if (m_game != NULL && m_scene != NULL && m_camera != NULL && m_light != NULL && m_clump24 != NULL && m_light2 != NULL) {
        if (m_clump1C == NULL) {
            BOOL failed = (m_clump1C = m_scene->CloneClump("Models/120_TIGGER/CAR_120.dff", NULL)) == NULL;
            if (failed) {
                return;
            }
        }

        if (m_controller == NULL) {
            BOOL failed = (m_controller = m_game->m_anim_dictionary->CreateController("Models/120_TIGGER/CAR_120.dff", m_clump1C, 6)) == NULL;
            if (failed) {
                return;
            }
        }

        m_game->m_screen_effect->SetSequenceByIndex(4);

        CDKW_RGBA color;
        F32 scalar = 1.1086956f;
        m_unk30 = CDKW_RGBA(229, 120, 15, 0xFF);
        color.red = m_unk30.red * scalar;
        color.green = m_unk30.green * scalar;
        color.blue = m_unk30.blue * scalar;

        CDKW_V3d local_9c;

        CDKW_V3d local_a8(0.0f, 5.1f, 12.5f);
        CDKW_V3d copy_a8 = local_a8;
        CDKW_V3d local_b4(0.0f, 1.8f, 0.0f);
        local_9c = local_b4;
        m_camera->LookAtInline(copy_a8, local_9c, CDKW_V3d::YAXIS);

        CDKW_V3d local_c0(4.0f, 7.0f, 6.0f);
        CDKW_V3d copy_c0 = local_c0;
        CDKW_V3d local_cc(0.0f, 1.8f, 0.0f);
        local_9c = local_cc;
        m_light->LookAtInline(copy_c0, local_9c, CDKW_V3d::YAXIS);

        m_light->SetColor(0.7f, 0.7f, 0.7f);
        m_light->SetConeAngle(90.0f);
        m_light->SetRadius(20.0f);
        m_scene->EnableLight(m_light, 1);

        CDKW_V3d model_pos_ = CDKW_V3d::ZERO;
        CDKW_V3d local_d8(4.0f, 4.0f, 4.0f);
        local_9c = local_d8;

        m_clump24->UpdateModelPos(model_pos_);
        RwFrameScale(m_clump24->GetFrame()->m_rwframe, &local_9c, 2);

        int nb_atomics = m_clump24->GetNumberOfAtomics();
        for (int i = 0; i < nb_atomics; i++) {
            DKDSP::CAtomic* atomic = m_clump24->GetAtomic(i);
            atomic->GetNumberOfMaterials();
            CDKW_Geometry* geo = atomic->GetGeometry()->GetDkWrapGeometry();

            if (geo->Lock()) {
                geo->m_rw_geometry->flags |= (1 << 6);

                int nb_materials = geo->GetNumberOfMaterials();
                for (int j = 0; j < nb_materials; j++) {
                    CDKW_Material* material = CDKW_Material::GetInstance(geo->m_rw_geometry->matList.materials[j]);
                    m_unk34 = (CDKW_RGBA&)(material->m_rw_material->color);
                    m_unk38 = material->m_rw_material->texture;
                    material->m_rw_material->color = color;
                    RpMaterialSetTexture(material->m_rw_material, NULL);
                }

                RpGeometryUnlock(geo->m_rw_geometry);
            }
        }

        m_clump1C->UpdateModelPos(CDKW_V3d::ZERO);

        DKDSP::CAnimation* anim = m_game->m_anim_dictionary->FindAnimation("Models/120_TIGGER/ANM_120_090.anm");

        m_controller->StopAllAnimations(DKDSP::ANIMATION_TYPE_ANM, 0);
        m_controller->StopAllAnimations(DKDSP::ANIMATION_TYPE_DMA, 0);
        m_controller->PlayAnimation(anim, 0);
        m_controller->SetAnimationTime(anim, m_unk2C);
        m_clump1C->SetController(m_controller);
        m_controller->Play(0.0f);

        CDKW_V3d local_e4(23.7f, 38.8f, 12.0f);
        CDKW_V3d copy_e4 = local_e4;
        CDKW_V3d local_f0(0.0f, 1.0f, 0.0f);
        local_9c = local_f0;
        m_light2->LookAtInline(copy_e4, local_9c, CDKW_V3d::YAXIS);

        m_light2->SetColor(0.2f, 0.2f, 0.2f);
        m_light2->SetRadius(20.0f);
        m_scene->EnableLight(m_light2, 0);

        m_game->GetResourceFactory()->m_unk4 = 0;
        if (m_shadow_map == NULL) {
            DKDSP::CShadowMap* shadow_map = m_scene->CreateShadowMap(m_light2, 128, 128, 16);
            m_shadow_map = shadow_map;
            if (shadow_map == NULL) {
                m_game->GetResourceFactory()->m_unk4 = 1;
                return;
            }
        }

        m_shadow_map->SetShadowColor(0.75f, 0.75f, 0.75f);
        m_shadow_map->GetCamera()->SetViewWindow(4.0f, 4.0f);
        m_shadow_map->SetBlurLevel(4);
        m_shadow_map->EnableDisplay(TRUE);
        m_shadow_map->AddShadowEmitter(m_clump1C);
        m_scene->SetupClumpToReceiveShadowMap(m_clump24, 1);

        m_game->GetResourceFactory()->m_unk4 = 1;
    }
}

void CLoadingTigger::Destroy() {
    CLoadingCallback::Destroy();
}

void CLoadingTigger::Update() {
    CLoadingCallback::Update();
}
