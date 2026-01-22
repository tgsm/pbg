#include "CGame.h"
#include "CLoadingCatchThemAll.h"
#include "engine/wrap/DKW_Geometry.h"
#include "engine/wrap/DKW_Material.h"
#include "entities/CEntityManager.h"
#include <iostream>
#include <math.h>

CLoadingCatchThemAll::CLoadingCatchThemAll(CGame* game) : CLoadingCallback(game) {
    m_clump44 = NULL;
}

CLoadingCatchThemAll::~CLoadingCatchThemAll() {
    Destroy();
}

// Incomplete: stack
void CLoadingCatchThemAll::Create() {
    m_unk58 = 0;

    CLoadingCallback::Create();

    if (m_game != NULL && m_scene != NULL && m_camera != NULL && m_light != NULL && m_clump24 != NULL && m_light2 != NULL) {
        if (m_clump1C == NULL) {
            BOOL failed = (m_clump1C = m_scene->CloneClump("Models/100_PIGLET/CAR_100.dff", NULL)) == NULL;
            if (failed) {
                return;
            }
        }

        if (m_controller == NULL) {
            BOOL failed = (m_controller = m_game->m_anim_dictionary->CreateController("Models/100_PIGLET/CAR_100.dff", m_clump1C, 6)) == NULL;
            if (failed) {
                return;
            }
        }

        if (m_clump44 == NULL) {
            BOOL failed = (m_clump44 = m_scene->CloneClump("Models/650_FIOLE/CAR_650.dff", NULL)) == NULL;
            if (failed) {
                return;
            }
        }

        m_game->m_screen_effect->SetSequenceByIndex(0);

        CDKW_RGBA color;
        F32 scalar = 1.1086956f;
        m_unk30 = CDKW_RGBA(78, 83, 149, 0xFF);
        color.red = m_unk30.red * scalar;
        color.green = m_unk30.green * scalar;
        color.blue = m_unk30.blue * scalar;

        CDKW_V3d local_9c;

        CDKW_V3d local_a8(0.0f, 3.2f, 7.2f);
        CDKW_V3d copy_a8 = local_a8;
        CDKW_V3d local_b4(0.0f, 1.05f, 0.0f);
        local_9c = local_b4;
        LookAtInline(m_camera, copy_a8, local_9c, CDKW_V3d::YAXIS);
        m_camera->SetFOV(60.0f, 4.0f/3.0f);

        CDKW_V3d local_c0(0.0f, 3.0f, 3.0f);
        CDKW_V3d copy_c0 = local_c0;
        CDKW_V3d local_cc(0.0f, 1.05f, 0.0f);
        local_9c = local_cc;
        LookAtInline(m_light, copy_c0, local_9c, CDKW_V3d::YAXIS);

        m_light->SetColor(0.7f, 0.7f, 0.7f);
        m_light->SetConeAngle(90.0f);
        m_light->SetRadius(20.0f);
        m_scene->EnableLight(m_light, 1);

        CDKW_V3d model_pos_ = CDKW_V3d::ZERO;
        CDKW_V3d local_d8(4.0f, 4.0f, 4.0f);
        local_9c = local_d8;

        UpdateModelPos(m_clump24, model_pos_);
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

        UpdateModelPos(m_clump1C, CDKW_V3d::ZERO);

        m_anims48[0] = m_game->m_anim_dictionary->FindAnimation("Models/100_PIGLET/ANM_100_240.anm");
        m_anims48[1] = m_game->m_anim_dictionary->FindAnimation("Models/100_PIGLET/ANM_100_280.anm");
        m_anims50[0] = m_game->m_anim_dictionary->FindDMorphAnimation("Models/100_PIGLET/ANM_100_020_1.dma");
        m_anims50[1] = m_game->m_anim_dictionary->FindDMorphAnimation("Models/100_PIGLET/ANM_100_080_1.dma");

        RwFrameRotate(90.0f, m_clump1C->GetFrame()->m_rwframe, &CDKW_V3d::YAXIS, 0);

        m_controller->StopAllAnimations(DKDSP::ANIMATION_TYPE_ANM, 0);
        m_controller->StopAllAnimations(DKDSP::ANIMATION_TYPE_DMA, 0);
        m_controller->PlayAnimation(m_anims48[0], 0);
        m_controller->PlayAnimation(m_anims50[0], 1);
        m_controller->SetAnimationTime(m_anims48[0], m_unk2C);
        m_controller->SetAnimationTime(m_anims50[0], m_unk2C);
        m_clump1C->SetController(m_controller);
        m_controller->Play(0.0f);

        m_clump1C->SetAnimation(m_game->m_anim_dictionary->FindAnimation("Models/650_FIOLE/CAR_650.anm"));
        UpdateModelPos(m_clump1C, CDKW_V3d(0.0f, 0.0f, 0.0f));

        CDKW_V3d local_e4_(23.7f, 38.8f, 12.0f);
        CDKW_V3d copy_e4_ = local_e4_;
        LookAtInline(m_light2, copy_e4_, CDKW_V3d::YAXIS);

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
        m_shadow_map->AddShadowEmitter(m_clump44);
        m_scene->SetupClumpToReceiveShadowMap(m_clump24, 1);

        m_game->GetResourceFactory()->m_unk4 = 1;
    }
}

void CLoadingCatchThemAll::Destroy() {
    if (m_clump44 != NULL) {
        m_scene->RemoveClump(m_clump44);
        m_clump44 = NULL;
    }

    CLoadingCallback::Destroy();
}

void CLoadingCatchThemAll::Update() {
    CLoadingCallback::Update();
}

// Incomplete: stack, regalloc
void CLoadingCatchThemAll::UpdateObjects(F32 dt) {
    if (m_clump1C != NULL && m_controller != NULL && m_light2 != NULL) {
        F32 unk2C = m_unk2C;
        int uVar6 = 0;
        while (unk2C > 3.0f) {
            unk2C -= 3.0f;
            uVar6++;
        }

        int mod = uVar6 % 2;
        if (m_unk58 != mod) {
            m_unk58 = mod;
            if (m_clump1C != NULL && m_controller != NULL) {
                m_controller->StopAllAnimations(DKDSP::ANIMATION_TYPE_ANM, 0);
                m_controller->StopAllAnimations(DKDSP::ANIMATION_TYPE_DMA, 0);
                m_controller->PlayAnimation(m_anims48[m_unk58], 0);
                m_controller->PlayAnimation(m_anims50[m_unk58], 1);

                RwFrameRotate(180.0f, m_clump1C->GetFrame()->m_rwframe, &CDKW_V3d::YAXIS, 2);
            }
        }

        CDKW_V3d local_38;
        if (m_unk58 == 0) {
            local_38 = CDKW_V3d(m_clump1C->GetFrame()->m_rwframe->modelling.pos);
            local_38.x = InterpolValue(-9.0f, 9.0f, dt, 3.0f);
            UpdateModelPosRef(m_clump1C, local_38);

            local_38 = CDKW_V3d(m_clump44->GetFrame()->m_rwframe->modelling.pos);
            local_38.x = InterpolValue(-6.0f, 12.0f, dt, 3.0f);
            F32 cos_ = fabsf((F32)cos(14.0f * dt));
            local_38.y = 0.6f * (F32)(F64)(cos_);
            UpdateModelPosRef(m_clump44, local_38);
        } else {
            local_38 = CDKW_V3d(m_clump1C->GetFrame()->m_rwframe->modelling.pos);
            local_38.x = InterpolValue(7.0f, -11.0f, dt, 3.0f);
            UpdateModelPosRef(m_clump1C, local_38);

            local_38 = CDKW_V3d(m_clump44->GetFrame()->m_rwframe->modelling.pos);
            local_38.x = InterpolValue(9.0f, -9.0f, dt, 3.0f);
            F32 cos_ = fabsf((F32)cos(14.0f * dt));
            local_38.y = 0.6f * (F32)(F64)(cos_);
            UpdateModelPosRef(m_clump44, local_38);
        }

        local_38 = CDKW_V3d(m_clump1C->GetFrame()->m_rwframe->modelling.pos);
        local_38 += CDKW_V3d(23.7f, 38.8f, 12.0f);
        UpdateModelPosRef(m_light2, local_38);

        local_38 = CDKW_V3d(m_clump1C->GetFrame()->m_rwframe->modelling.pos);
        local_38 += 0.0f;

        LookAtInline(m_light2, local_38, CDKW_V3d::YAXIS);

        CLoadingCallback::UpdateObjects(dt);
    }
}

void CLoadingCatchThemAll::RenderObjects() {
    if (m_clump44 != NULL) {
        m_scene->RenderClump(m_clump44);
    }

    CLoadingCallback::RenderObjects();
}
