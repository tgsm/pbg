#include "CGame.h"
#include "CLoadingAdventure.h"
#include "engine/wrap/DKW_Geometry.h"
#include "engine/wrap/DKW_Material.h"
#include <iostream>

CLoadingAdventure::CLoadingAdventure(CGame* game) : CLoadingCallback(game) {
    m_hero_state = 0;
}

CLoadingAdventure::~CLoadingAdventure() {
    Destroy();
}

void CLoadingAdventure::Create() {
    BOOL failed;
    CDKW_Material* material;
    DKDSP::CAnimation* anim;
    DKDSP::CDMorphAnimation* anim2;
    DKDSP::CAtomic* atomic;
    int iVar16;
    int nb_materials;
    int nb_atomics;
    CDKW_Geometry* geo;
    RpMaterial* rp_material;
    int i;
    int j;

    CLoadingCallback::Create();

    if (m_game != NULL && m_scene != NULL && m_camera != NULL && m_light != NULL && m_clump24 != NULL && m_light2 != NULL) {
        if (m_clump1C == NULL) {
            failed = (m_clump1C = m_scene->CloneClump("Models/100_PIGLET/CAR_100.dff", NULL)) == NULL;
            if (failed) {
                return;
            }
        }

        if (m_controller == NULL) {
            failed = (m_controller = m_game->m_anim_dictionary->CreateController("Models/100_PIGLET/CAR_100.dff", m_clump1C, 6)) == NULL;
            if (failed) {
                return;
            }
        }

        std::string anm_filename, dma_filename;
        CDKW_RGBA tmp_color;

        iVar16 = m_hero_state;
        if (m_game->m_unk4F5C == 3 || m_game->m_unk4F54 == 8) {
            iVar16 = 0;
        }
        switch (iVar16) {
            case 2:
                anm_filename = "Models/100_PIGLET/ANM_100_280.anm";
                dma_filename = "Models/100_PIGLET/ANM_100_070_1.dma";
                m_game->m_screen_effect->SetSequenceByIndex(2);
                tmp_color = CDKW_RGBA(0, 0, 0, 0xFF);
                break;
            case 1:
                anm_filename = "Models/100_PIGLET/ANM_100_210.anm";
                dma_filename = "Models/100_PIGLET/ANM_100_040_1.dma";
                m_game->m_screen_effect->SetSequenceByIndex(1);
                tmp_color = CDKW_RGBA(26, 26, 67, 0xFF);
                break;
            default:
                anm_filename = "Models/100_PIGLET/ANM_100_220.anm";
                dma_filename = "Models/100_PIGLET/ANM_100_010_2.dma";
                m_game->m_screen_effect->SetSequenceByIndex(0);
                tmp_color = CDKW_RGBA(199, 226, 222, 0xFF);
                break;
        }

        CDKW_RGBA color = tmp_color;
        color.red *= 1.1086956f;
        color.green *= 1.1086956f;
        color.blue *= 1.1086956f;
        m_unk30 = tmp_color;

        CDKW_V3d local_9c;

        CDKW_V3d local_a8(0.0f, 3.6f, 8.8f);
        CDKW_V3d copy_a8 = local_a8;
        CDKW_V3d local_b4(0.0f, 1.3f, 0.0f);
        local_9c = local_b4;
        LookAtInline(m_camera, copy_a8, local_9c, CDKW_V3d::YAXIS);

        CDKW_V3d local_c0(2.0f, 6.0f, 6.0f);
        CDKW_V3d copy_c0 = local_c0;
        CDKW_V3d local_cc(0.0f, 1.3f, 0.0f);
        local_9c = local_cc;
        LookAtInline(m_light, copy_c0, local_9c, CDKW_V3d::YAXIS);

        m_light->SetColor(0.7f, 0.7f, 0.7f);
        m_light->SetConeAngle(90.0f);
        m_light->SetRadius(20.0f);
        m_scene->EnableLight(m_light, 1);

        CDKW_V3d model_pos_ = CDKW_V3d::ZERO;
        CDKW_V3d local_d8(3.0f, 3.0f, 3.0f);
        local_9c = local_d8;

        UpdateModelPos(m_clump24, model_pos_);
        RwFrameScale(m_clump24->GetFrame()->m_rwframe, &local_9c, 2);

        nb_atomics = m_clump24->GetNumberOfAtomics();
        for (i = 0; i < nb_atomics; i++) {
            atomic = m_clump24->GetAtomic(i);
            atomic->GetNumberOfMaterials();
            geo = atomic->GetGeometry()->wrap_geometry;

            if (geo->Lock()) {
                geo->rw_geometry->flags |= (1 << 6);

                nb_materials = geo->rw_geometry->matList.numMaterials;
                for (j = 0; j < nb_materials; j++) {
                    rp_material = geo->rw_geometry->matList.materials[j];
                    material = CDKW_Material::GetInstance(rp_material);
                    m_unk34 = (CDKW_RGBA&)(material->rw_material->color);
                    m_unk38 = material->rw_material->texture;
                    material->rw_material->color = color;
                    RpMaterialSetTexture(material->rw_material, NULL);
                }

                RpGeometryUnlock(geo->rw_geometry);
            }
        }

        UpdateModelPos(m_clump1C, CDKW_V3d::ZERO);

        anim = m_game->m_anim_dictionary->FindAnimation(anm_filename);
        anim2 = m_game->m_anim_dictionary->FindDMorphAnimation(dma_filename);

        m_controller->StopAllAnimations(DKDSP::ANIMATION_TYPE_ANM, 0);
        m_controller->StopAllAnimations(DKDSP::ANIMATION_TYPE_DMA, 0);
        m_controller->PlayAnimation(anim, 0);
        m_controller->PlayAnimation(anim2, 1);
        m_controller->SetAnimationTime(anim, m_unk2C);
        m_controller->SetAnimationTime(anim2, m_unk2C);
        m_clump1C->SetController(m_controller);
        m_controller->Play(0.0f);

        CDKW_V3d local_e4(23.7f, 38.8f, 12.0f);
        CDKW_V3d copy_e4 = local_e4;
        CDKW_V3d local_f0(0.0f, 1.0f, 0.0f);
        local_9c = local_f0;
        LookAtInline(m_light2, copy_e4, local_9c, CDKW_V3d::YAXIS);

        m_light2->SetColor(0.2f, 0.2f, 0.2f);
        m_light2->SetRadius(20.0f);
        m_scene->EnableLight(m_light2, 0);

        m_game->GetResourceFactory()->m_unk4 = 0;
        if (m_shadow_map == NULL) {
            m_shadow_map = m_scene->CreateShadowMap(m_light2, 128, 128, 16);
            if (m_shadow_map == NULL) {
                m_game->GetResourceFactory()->m_unk4 = 1;
                return;
            }
        }

        color = m_unk30;

        m_shadow_map->SetShadowColor(0.75f, 0.75f, 0.75f);
        m_shadow_map->GetCamera()->SetViewWindow(2.0f, 2.0f);
        m_shadow_map->SetBlurLevel(4);
        m_shadow_map->EnableDisplay(TRUE);
        m_shadow_map->AddShadowEmitter(m_clump1C);
        m_scene->SetupClumpToReceiveShadowMap(m_clump24, 1);

        m_game->GetResourceFactory()->m_unk4 = 1;
    }
}

void CLoadingAdventure::Destroy() {
    CLoadingCallback::Destroy();
}

void CLoadingAdventure::Update() {
    CLoadingCallback::Update();
}
