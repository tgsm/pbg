#include "engine/display/CLight.h"
#include "engine/display/CTexture.h"
#include "engine/display/CWarp.h"
#include "engine/sound/DkSound.h"
#include "engine/wrap/DKW_RGBAReal.h"
#include "entities/CEntityEscapingObject.h"
#include "entities/CEntityScaryFaceBox.h"
#include "entities/CEntityTimer.h"
#include "entities/CEntityWarp.h"
#include "CGame.h"
#include "CGamePartIngame.h"
#include "CGameRoomManager.h"
#include <cstdlib>
#include <iostream>

CGameRoomManager::CGameRoomManager(CGamePartIngame* game_part, CGame* game) {
    m_game_part = game_part;
    m_game = game;
    m_batch44 = NULL;
    m_batch48 = NULL;
    m_batch124 = NULL;
    m_pigspline108 = NULL;
    m_screen_effect = NULL;

    Init();
}

CGameRoomManager::~CGameRoomManager() {
    if (m_screen_effect != NULL) {
        delete m_screen_effect;
    }

    if (m_batch148 != NULL) {
        m_game->m_display_engine->GetImmediate()->RemoveBatch2D(m_batch148);
        m_batch148 = NULL;
    }
    if (m_batch44 != NULL) {
        m_game->m_display_engine->GetImmediate()->RemoveBatch2D(m_batch44);
        m_batch44 = NULL;
    }
    if (m_batch48 != NULL) {
        m_game->m_display_engine->GetImmediate()->RemoveBatch2D(m_batch48);
        m_batch48 = NULL;
    }
    if (m_batch124 != NULL) {
        m_game->m_display_engine->GetImmediate()->RemoveBatch2D(m_batch124);
        m_batch124 = NULL;
    }
    if (m_batch130 != NULL) {
        m_game->m_display_engine->GetImmediate()->RemoveBatch2D(m_batch130);
        m_batch130 = NULL;
        m_game->m_display_engine->GetImmediate()->RemoveBatch2D(m_batch134);
        m_batch134 = NULL;
        m_game->m_display_engine->GetImmediate()->RemoveBatch2D(m_batch138);
        m_batch138 = NULL;
        m_game->m_display_engine->GetImmediate()->RemoveBatch2D(m_batch13C);
        m_batch13C = NULL;
    }

    if (m_soundFC != NULL) {
        m_soundFC->Stop();
        m_game->m_sound_engine->RemoveSound(m_soundFC);
        m_soundFC = NULL;
    }
    if (m_sound104 != NULL) {
        m_sound104->Stop();
        m_game->m_sound_engine->RemoveSound(m_sound104);
        m_sound104 = NULL;
    }
    if (m_soundF8 != NULL) {
        m_soundF8->Stop();
        m_game->m_sound_engine->RemoveSound(m_soundF8);
        m_soundF8 = NULL;
    }
    if (m_sound100 != NULL) {
        m_sound100->Stop();
        m_game->m_sound_engine->RemoveSound(m_sound100);
        m_sound100 = NULL;
    }

    if (m_pigspline108 != NULL) {
        delete m_pigspline108;
        m_pigspline108 = NULL;
    }

    if (m_light != NULL) {
        m_game->GetScene()->RemoveLight(m_light);
        m_light = NULL;
    }

    m_game->m_sound_engine->SetGlobalVolume(m_game->m_unk504C);
}

void CGameRoomManager::Init() {
    SetState(ROOM_STATE_0, 1);
    m_flags = 0;
    m_unk4C = NULL;
    m_key_sequence_entity = NULL;
    m_hero = NULL;

    m_unk54 = CDKW_V3d(0.0f, 0.0f, 0.0f);
    m_unk6C = CDKW_V3d(0.0f, 0.0f, 0.0f);
    m_unk60 = CDKW_V3d(0.0f, 0.0f, 0.0f);
    m_unk78 = CDKW_V3d(0.0f, 0.0f, 0.0f);

    m_unk9C = 0.0f;
    m_unkA0 = 0.0f;
    m_unkC8 = 1.5f;
    m_unkCC = 15.0f;
    m_unkD0 = 5.0f;
    m_unkD4 = 20.0f;
    m_unkA4 = 0.0f;
    m_unkA8 = 0.0f;
    m_unkD8 = 0.0f;
    m_unkDC = 3.0f;
    m_pathfinder_entity = NULL;
    m_unk128 = 0.0f;
    m_unk12C = 0.0f;
    m_unk140 = 0.0f;
    m_unk144 = -1;
    m_batch148 = NULL;
    m_batch130 = NULL;
    m_batch134 = NULL;
    m_batch138 = NULL;
    m_batch13C = NULL;
    m_soundFC = NULL;
    m_sound100 = NULL;
    m_soundF8 = NULL;
    m_sound104 = NULL;
    m_clump160 = NULL;
    m_unk164 = 0;
    m_unk154 = 0;
    m_unk10C = 1.0f;
    m_light = NULL;
    m_unk170 = 0.0f;

    m_game->m_entity_manager->m_path_finder->m_unk4B6C.clear();

    if (m_game->GetCurrentMission().m_rooms[m_game->m_unk4F58] & (1 << 2)) {
        m_flags |= (1 << 16);
        m_flags &= ~(1 << 17);
    }

    if (m_game->GetCurrentMission().IsSecondPassCompleted() != 0u) {
        m_flags |= (1 << 21);
    }
}

void CGameRoomManager::Update(F32 dt) {
    if (m_screen_effect != NULL) {
        m_screen_effect->Update(dt);
    }

    if (m_flags & (1 << 5)) {
        switch (GetState()) {
            case 3:
                OnPlayRTCInit();
                break;
            case 4:
                OnPlayRTCUpdate();
                break;
            case 5:
                switch (m_unkC) {
                    case 0:
                        m_unkC = 1;
                        break;
                    case 1:
                        if (m_flags & (1 << 6)) {
                            SetState(ROOM_STATE_10, 1);
                            m_unkC = m_unk14;
                        } else {
                            SetState(ROOM_STATE_2, 1);
                        }
                        break;
                }
                break;
        }
    } else {
        m_delta_time = dt;
        m_game->m_unk8 &= ~(1 << 6);

        switch (GetState()) {
            case 0:
                InitHeroEntityMember();

                m_game->m_unk504C = m_game->m_sound_engine->GetGlobalVolume();
                m_delta_time = 0.0f;
                m_game->SetCurrentRoomReturnType(CGame::RETURN_TYPE_0, -1);

                if (m_unk154 & (1 << 31)) {
                    m_game->FadeInit(1.0f, CGame::FADE_TYPE_4, ((m_unk154 & 0xFF) >> 0), ((m_unk154 & 0xFF00) >> 8), ((m_unk154 & 0xFF0000) >> 16), 0.0f);
                } else {
                    m_game->FadeInit(1.0f, CGame::FADE_TYPE_2, 0, 0, 0, 0.0f);
                }
                m_game->m_unk503C &= ~(1 << 3);
                m_game->m_unk503C |= (1 << 0);
                m_game->FadeOut(-1.0f);
                m_game->m_unk5038 = 1;

                SetState(ROOM_STATE_1, 1);

                CountEnemies();

                if (m_flags & (1 << 6)) {
                    SetState(ROOM_STATE_10, 1);
                    m_unkC = 0;
                }

                if (m_game->GetCurrentMission().m_rooms[m_game->m_unk4F58] & (1 << 2)) {
                    m_flags |= (1 << 25);
                }

                break;
            case 1:
                FadeInRoom();
                break;
            case 2:
                m_game->m_entity_manager->m_path_finder->Update(m_delta_time);
                OnPlayNormal();
                break;
            case 3:
                OnPlayRTCInit();
                break;
            case 4:
                OnPlayRTCUpdate();
                break;
            case 5:
                switch (m_unkC) {
                    case 0:
                        m_unkC = 1;
                        break;
                    case 1:
                        if (m_flags & (1 << 6)) {
                            SetState(ROOM_STATE_10, 1);
                            m_unkC = m_unk14;
                        } else {
                            SetState(ROOM_STATE_2, 1);
                        }
                        break;
                }
                break;
            case 6:
                OnPlayFightInit();
                break;
            case 7:
                OnPlayFightUpdate();
                break;
            case 8:
                OnPlayFightClose();
                break;
            case 9:
                if (!(m_flags & (1 << 3))) {
                    U32 unkC = m_unkC;
                    SetState((ERoomState)(m_unk8 >> 16), 1);
                    m_unkC = unkC;
                }
                break;
            case 10:
                SecondPass();
                break;
            case 11:
                for (U32 i = 0; i < m_game->m_entity_manager->GetEntityCount(); i++) {
                    CEntity* entity = m_game->m_entity_manager->GetEntity(i);
                    if (entity != NULL) {
                        U32 type = entity->GetType();
                        // unused
                    }
                }
                SetState(ROOM_STATE_12, 1);
                break;
            case 12:
                SetState(ROOM_STATE_13, 1);
                break;
            case 13:
                if (m_flags & (1 << 1)) {
                    m_game->SetCurrentRoomReturnType(CGame::RETURN_TYPE_2, -1);
                } else {
                    m_game->SetCurrentRoomReturnType(CGame::RETURN_TYPE_1, -1);
                }
                break;
            case 14:
                Victory();
                break;
            case 15:
                DisplayCatchThemAll();
                break;
        }

        if (m_game->m_unk5038 != 2 && !(m_game->m_unk8 & (1 << 6))) {
            m_game->FadeUpdate(m_delta_time);
        }
    }
}

void CGameRoomManager::Render() {
    DKDSP::CEngine* display_engine = m_game->m_display_engine;

    if (IsPlayingRTC() && m_unk170 > 0.5f) {
        m_unk170 = 0.5f;
    }

    if (m_unk170 > 0.0f) {
        int iVar8;
        if (m_unk170 > 2.5f) {
            iVar8 = 200.0f * (1.0f - (0.5f + (m_unk170 - 3.0f)) / 0.5f);
        } else if (m_unk170 < 0.5f) {
            iVar8 = 200.0f * (m_unk170 / 0.5f);
        } else {
            iVar8 = 200;
        }
        CEntityScaryFaceBox::RenderCookiesNbIconDirect(m_game, m_game->GetCurrentMission().GetMaxCollectedNbCookies(), 0.17250001f, 0.11750001f, iVar8, 0.27f, 0.135f, 1, 0);
    }

    m_unk170 -= m_delta_time;
    if (m_unk170 < 0.0f) {
        m_unk170 = 0.0f;
    }

    if (m_batch124 != NULL) {
        DKDSP::CTexture* texture;
        if (m_flags & (1 << 17)) {
            texture = m_game->m_texture_dictionary->FindTexture("BMP_202");
        } else if (m_flags & (1 << 18)) {
            texture = m_game->m_texture_dictionary->FindTexture("BMP_204");
        } else {
            texture = m_game->m_texture_dictionary->FindTexture("BMP_101");
        }

        display_engine->SetRenderState((RwRenderState)12, (void*)1);
        display_engine->SetRenderState((RwRenderState)10, (void*)5);
        display_engine->SetRenderState((RwRenderState)11, (void*)6);
        if (texture != NULL) {
            display_engine->SetRenderState((RwRenderState)1, (void*)texture->GetRaster());
        } else {
            display_engine->SetRenderState((RwRenderState)1, (void*)NULL);
        }

        m_batch124->Render(rwPRIMTYPETRIFAN);
    }

    if (m_batch130 != NULL && !IsPlayingRTC()) {
        display_engine->SetRenderState((RwRenderState)12, (void*)1);
        display_engine->SetRenderState((RwRenderState)10, (void*)5);
        display_engine->SetRenderState((RwRenderState)11, (void*)6);

        DKDSP::CTexture* texture = m_game->m_texture_dictionary->FindTexture("BMP_306");
        if (texture != NULL) {
            display_engine->SetRenderState((RwRenderState)1, (void*)texture->GetRaster());
        } else {
            display_engine->SetRenderState((RwRenderState)1, (void*)NULL);
        }
        m_batch130->Render(rwPRIMTYPETRIFAN);

        texture = m_game->m_texture_dictionary->FindTexture("BMP_305");
        if (texture != NULL) {
            display_engine->SetRenderState((RwRenderState)1, (void*)texture->GetRaster());
        } else {
            display_engine->SetRenderState((RwRenderState)1, (void*)NULL);
        }
        m_batch134->Render(rwPRIMTYPETRIFAN);

        texture = m_game->m_texture_dictionary->FindTexture("BMP_304");
        if (texture != NULL) {
            display_engine->SetRenderState((RwRenderState)1, (void*)texture->GetRaster());
        } else {
            display_engine->SetRenderState((RwRenderState)1, (void*)NULL);
        }
        m_batch138->Render(rwPRIMTYPETRIFAN);

        texture = m_game->m_texture_dictionary->FindTexture("BMP_303");
        if (texture != NULL) {
            display_engine->SetRenderState((RwRenderState)1, (void*)texture->GetRaster());
        } else {
            display_engine->SetRenderState((RwRenderState)1, (void*)NULL);
        }
        m_batch13C->Render(rwPRIMTYPETRIFAN);
    }

    if (m_key_sequence_entity != NULL) {
        m_key_sequence_entity->Render(m_delta_time);
    }

    if (!IsPlayingRTC()) {
        if (m_batch44 != NULL) {
            DKDSP::CTexture* texture = m_game->m_texture_dictionary->FindTexture("BMP_301");
            display_engine->SetRenderState((RwRenderState)12, (void*)1);
            display_engine->SetRenderState((RwRenderState)10, (void*)5);
            display_engine->SetRenderState((RwRenderState)11, (void*)6);
            if (texture != NULL) {
                display_engine->SetRenderState((RwRenderState)1, (void*)texture->GetRaster());
            } else {
                display_engine->SetRenderState((RwRenderState)1, (void*)NULL);
            }
            m_batch44->Render(rwPRIMTYPETRIFAN);
        }
        if (m_batch48 != NULL) {
            DKDSP::CTexture* texture = m_game->m_texture_dictionary->FindTexture("BMP_302");
            display_engine->SetRenderState((RwRenderState)12, (void*)1);
            display_engine->SetRenderState((RwRenderState)10, (void*)5);
            display_engine->SetRenderState((RwRenderState)11, (void*)6);
            if (texture != NULL) {
                display_engine->SetRenderState((RwRenderState)1, (void*)texture->GetRaster());
            } else {
                display_engine->SetRenderState((RwRenderState)1, (void*)NULL);
            }
            m_batch48->Render(rwPRIMTYPETRIFAN);
        }
    }

    if (m_flags & (1 << 22) && !(m_flags & (1 << 10))) {
        m_game->RenderFade();
    }

    if (m_batch148 != NULL) {
        display_engine->SetRenderState((RwRenderState)12, (void*)1);
        display_engine->SetRenderState((RwRenderState)10, (void*)5);
        display_engine->SetRenderState((RwRenderState)11, (void*)6);
        display_engine->SetRenderState((RwRenderState)1, (void*)NULL);
        display_engine->SetRenderState((RwRenderState)8, (void*)0);
        display_engine->AlphaEnable();
        m_batch148->Render(rwPRIMTYPETRIFAN);
        display_engine->SetRenderState((RwRenderState)8, (void*)1);
    }

    U32 state = GetState();
    if (((state == 6 && m_unkC == 3) || (state == 8 && m_unkC == 2)) || (m_flags & (1 << 12) && !(m_flags & (1 << 3)))) {
        RenderFadeWarpForFight(display_engine);
    } else if (m_flags & (1 << 9)) {
        m_hero->AddFlag(ENTITY_FLAG_VISIBLE);
        m_hero->Render(m_delta_time);
        m_game->GetScene()->Flush();
        m_hero->DelFlag(ENTITY_FLAG_VISIBLE);
    }

    if (m_flags & (1 << 22) && m_flags & (1 << 10)) {
        m_game->RenderFade();
    }

    if (m_clump160 != NULL) {
        m_game->GetScene()->RenderClump(m_clump160);
    }

    RenderVictoryElement(display_engine);

    if (m_screen_effect != NULL) {
        m_screen_effect->Render(NULL);
    }
}

void CGameRoomManager::RenderRoomNormalFade() {
    if (!(m_flags & (1 << 22))) {
        m_game->RenderFade();
    }
}

void CGameRoomManager::RenderFadeWarpForFight(DKDSP::IEngine* display_engine) {
    CDKW_RGBAReal ambient;
    CEntityPhial* phial;
    DKDSP::CScene* scene = m_game->GetScene();
    int affects[32];
    int nb_warps;
    int nb_lights;
    int nb_clumps;
    int j;
    DKDSP::CLight* light;
    CEntityWarp* warp_entity;
    DKDSP::CWarp* warp;
    int i;

    F32 new_ambient = m_game->m_unk502C / m_game->m_fade_duration;
    if (GetState() == 8 || (m_flags & (1 << 14))) {
        new_ambient = 1.0f - new_ambient;
    }

    ambient.m_r = m_game->GetScene()->GetAmbientRed();
    ambient.m_g = m_game->GetScene()->GetAmbientGreen();
    ambient.m_b = m_game->GetScene()->GetAmbientBlue();
    scene->SetAmbient(new_ambient, new_ambient, new_ambient);

    nb_warps = m_game->m_entity_manager->GetEntityTypeCount(ENTITY_WARP);

    scene->Flush();
    nb_lights = scene->GetNumberOfLights();

    for (i = 0; i < nb_lights; i++) {
        light = scene->GetLight(i);
        affects[i] = light->GetAffected();
        light->Affect(0);
    }

    for (i = 0; i < nb_warps; i++) {
        warp_entity = (CEntityWarp*)m_game->m_entity_manager->GetEntityType(ENTITY_WARP, i);
        if (warp_entity != NULL && warp_entity->m_unk0 == "FIGHTWARP") {
            warp = warp_entity->GetWarp();
            nb_clumps = warp->GetNumberOfClumps();
            for (j = 0; j < nb_clumps; j++) {
                warp->GetClump(j);
                warp->SetRenderState(j, DKDSP::WARP_RENDER_STATE_6, (void*)2);
                warp->SetRenderState(j, DKDSP::WARP_RENDER_STATE_7, (void*)2);
            }
            warp->Render();
        }
    }

    scene->Flush();

    for (i = 0; i < nb_lights; i++) {
        scene->GetLight(i)->Affect(affects[i]);
    }

    scene->SetAmbient(ambient.m_r, ambient.m_g, ambient.m_b);

    if (m_unk4C != NULL) {
        phial = m_unk4C->m_phial;
        if (phial != NULL) {
            phial->AddFlag(ENTITY_FLAG_VISIBLE);
            phial->Render(m_delta_time);
            phial->DelFlag(ENTITY_FLAG_VISIBLE);
        } else {
            display_engine->SetRenderState((RwRenderState)8, (void*)1);
            m_unk4C->AddFlag(ENTITY_FLAG_VISIBLE);
            m_unk4C->Render(m_delta_time);
            m_unk4C->DelFlag(ENTITY_FLAG_VISIBLE);
        }
    }
    scene->Flush();

    m_hero->AddFlag(ENTITY_FLAG_VISIBLE);
    m_hero->Render(m_delta_time);
    m_hero->DelFlag(ENTITY_FLAG_VISIBLE);
    scene->Flush();

    if (m_pathfinder_entity != NULL) {
        m_pathfinder_entity->m_unkF4 |= (1 << 2);
        m_pathfinder_entity->Render(m_delta_time);
        scene->Flush();
        m_pathfinder_entity->m_unkF4 &= ~(1 << 2);
    }
}

U32 CGameRoomManager::AddFlag(U32 flag) {
    U32 state = GetState();
    if ((flag & (1 << 0) || flag & (1 << 1)) && (state >= 6 && state <= 8)) {
        flag &= ~(1 << 0);
    }
    m_flags |= flag;
    return m_flags;
}

U32 CGameRoomManager::RemFlag(U32 flag) {
    m_flags &= ~flag;
    return m_flags;
}

U32 CGameRoomManager::GetFlag() {
    return m_flags;
}

void CGameRoomManager::SetState(ERoomState state, int a2) {
    if (a2 != 0) {
        m_unk8 <<= 16;
    }

    m_unk8 &= ~0xFFFF;
    m_unk8 |= (state & 0xFFFF);

    switch (GetState()) {
        case 0:
            m_game_part->m_unk18 &= ~(1 << 0);
            m_game_part->m_unk18 &= ~(1 << 1);
            break;
        case 1:
            m_game_part->m_unk18 |= (1 << 0);
            m_game_part->m_unk18 |= (1 << 1);
            break;
        case 2:
            m_game_part->m_unk18 |= (1 << 0);
            m_game_part->m_unk18 |= (1 << 1);
            break;
        case 3:
            m_game_part->m_unk18 |= (1 << 0);
            m_game_part->m_unk18 |= (1 << 1);
            break;
        case 4:
            m_game_part->m_unk18 |= (1 << 0);
            m_game_part->m_unk18 |= (1 << 1);
            break;
        case 5:
            m_game_part->m_unk18 |= (1 << 0);
            m_game_part->m_unk18 |= (1 << 1);
            break;
        case 6:
            m_unkC = 0;
            m_game_part->m_unk18 |= (1 << 0);
            m_game_part->m_unk18 |= (1 << 1);
            break;
        case 7:
            m_unkC = 0;
            m_game_part->m_unk18 |= (1 << 0);
            m_game_part->m_unk18 |= (1 << 1);
            break;
        case 8:
            m_unkC = 0;
            m_game_part->m_unk18 |= (1 << 0);
            m_game_part->m_unk18 |= (1 << 1);
            break;
        case 9:
            m_game_part->m_unk18 &= ~(1 << 0);
            m_game_part->m_unk18 |= (1 << 1);
            break;
        case 10:
            m_game_part->m_unk18 |= (1 << 0);
            m_game_part->m_unk18 |= (1 << 1);
            break;
        case 11:
            m_game_part->m_unk18 &= ~(1 << 0);
            m_game_part->m_unk18 |= (1 << 1);
            break;
        case 12:
            m_game_part->m_unk18 &= ~(1 << 0);
            m_game_part->m_unk18 |= (1 << 1);
            break;
        case 13:
            m_game_part->m_unk18 &= ~(1 << 0);
            m_game_part->m_unk18 &= ~(1 << 1);
            break;
        case 14:
            m_game_part->m_unk18 |= (1 << 0);
            m_game_part->m_unk18 |= (1 << 1);
            m_unkC = 0;
            break;
        case 15:
            m_game_part->m_unk18 |= (1 << 0);
            m_game_part->m_unk18 |= (1 << 1);
            m_unkC = 0;
            break;
    }
}

U32 CGameRoomManager::GetState() {
    return m_unk8 & 0xFFFF;
}

BOOL CGameRoomManager::IsPlayingRTC() {
    if (GetState() >= 3 && GetState() <= 5) {
        return TRUE;
    }
    return FALSE;
}

BOOL CGameRoomManager::IsOnFight() {
    if (m_flags & (1 << 13)) {
        return TRUE;
    }

    U32 state = GetState();
    if (state == 7 || (state == 6 && m_unkC >= 4) || (state == 8 && m_unkC < 2)) {
        return TRUE;
    }

    U32 unk;
    U32 unkC;

    unkC = m_unkC;
    unk = m_unk8 >> 16;
    if (state == 9 && (unk == 7 || (unk == 6 && unkC == 5) || (unk == 8 && unkC < 2))) {
        return TRUE;
    }

    return FALSE;
}

#pragma dont_inline on
void CGameRoomManager::FadeInRoom() {
    UpdateAdventureCam();
    SetState(ROOM_STATE_2, 1);
    if (m_flags & (1 << 6)) {
        SetState(ROOM_STATE_10, 1);
        m_unkC = 0;
    }
}
#pragma dont_inline reset

void CGameRoomManager::OnPlayNormal() {
    m_flags &= ~(1 << 11);
    m_flags &= ~(1 << 19);
    m_game->m_unk503C &= ~(1 << 3);

    if (m_flags & (1 << 3)) {
        SetState(ROOM_STATE_9, 1);
        return;
    } else if (m_flags & (1 << 6)) {
        SetState(ROOM_STATE_10, 1);
        m_unkC = 0;
        return;
    } else if (m_flags & (1 << 0)) {
        m_flags &= ~(1 << 0);
        m_flags &= ~(1 << 1);

        if (m_unk4C != NULL) {
            SetState(ROOM_STATE_6, 1);
            OnPlayFightInit();
            return;
        }
    }

    if (m_flags & (1 << 1)) {
        SetState(ROOM_STATE_6, 1);
        OnPlayFightInit();
        return;
    } else if (m_flags & (1 << 2)) {
        m_flags &= ~(1 << 2);
        SetState(ROOM_STATE_3, 1);
        m_unkC = 0;
        OnPlayRTCInit();
        return;
    } else if (m_hero != NULL && m_hero->GetType() == ENTITY_TIGGER && m_hero->GetState() == 1) {
        if (m_unk144 == -1) {
            m_unk144 = 3;
            m_unk140 = 0.0f;
            std::string tmp;

            tmp = "SCAR_120_08_2C";

            if (m_unk4C != NULL) {
                DKSND::CSound2D* sound = DkSoundGetEngine()->PlaySound2D(&tmp, 1);
                if (sound != NULL) {
                    sound->SetLoopMode(0);
                    sound->SetLayer(1);
                }
            } else if (m_hero != NULL) {
                DKSND::CSound2D* sound = DkSoundGetEngine()->PlaySound2D(&tmp, 1);
                if (sound != NULL) {
                    sound->SetLoopMode(0);
                    sound->SetLayer(1);
                }
            }
        }

        if (m_unk144 > 0) {
            DisplayExclamation();
        } else {
            if (m_batch130 != NULL) {
                m_game->m_display_engine->GetImmediate()->RemoveBatch2D(m_batch130);
                m_batch130 = NULL;
                m_game->m_display_engine->GetImmediate()->RemoveBatch2D(m_batch134);
                m_batch134 = NULL;
                m_game->m_display_engine->GetImmediate()->RemoveBatch2D(m_batch138);
                m_batch138 = NULL;
                m_game->m_display_engine->GetImmediate()->RemoveBatch2D(m_batch13C);
                m_batch13C = NULL;
            }
        }
    } else {
        m_unk144 = -1;
        m_unk140 = 0.0f;
        if (m_batch130 != NULL) {
            m_game->m_display_engine->GetImmediate()->RemoveBatch2D(m_batch130);
            m_batch130 = NULL;
            m_game->m_display_engine->GetImmediate()->RemoveBatch2D(m_batch134);
            m_batch134 = NULL;
            m_game->m_display_engine->GetImmediate()->RemoveBatch2D(m_batch138);
            m_batch138 = NULL;
            m_game->m_display_engine->GetImmediate()->RemoveBatch2D(m_batch13C);
            m_batch13C = NULL;
        }
    }

    CheckIfFightPossible();
    CheckIfHeroIsPushing();

    if (!(m_flags & (1 << 25))) {
        if (m_game->GetCurrentMission().m_rooms[m_game->m_unk4F58] & (1 << 2)) {
            if (m_flags & (1 << 16)) {
                m_flags &= ~(1 << 17);
            } else {
                m_flags |= (1 << 16);
                m_flags |= (1 << 17);
                m_unk14 = 4;
                m_unk10 = 2;
                SetState(ROOM_STATE_14, 1);
            }
        }
    }

    m_game->m_unk503C &= ~(1 << 0);

    UpdateAdventureCam();
}

void CGameRoomManager::OnPlayRTCInit() {
    switch (m_unkC) {
        case 0:
            if (m_batch124 != NULL) {
                m_game->m_display_engine->GetImmediate()->RemoveBatch2D(m_batch124);
                m_batch124 = NULL;
            }

            if (m_batch130 != NULL) {
                m_game->m_display_engine->GetImmediate()->RemoveBatch2D(m_batch130);
                m_game->m_display_engine->GetImmediate()->RemoveBatch2D(m_batch134);
                m_game->m_display_engine->GetImmediate()->RemoveBatch2D(m_batch138);
                m_game->m_display_engine->GetImmediate()->RemoveBatch2D(m_batch13C);
                m_batch130 = NULL;
                m_batch134 = NULL;
                m_batch138 = NULL;
                m_batch13C = NULL;
            }

            if (m_sound100 != NULL) {
                m_sound100->Stop();
                m_game->m_sound_engine->RemoveSound(m_sound100);
                m_sound100 = NULL;
            }
            if (m_soundFC != NULL) {
                m_soundFC->Stop();
                m_game->m_sound_engine->RemoveSound(m_soundFC);
                m_soundFC = NULL;
            }
            if (m_sound104 != NULL) {
                m_sound104->Stop();
                m_game->m_sound_engine->RemoveSound(m_sound104);
                DKI::IInputEngine::GetDevice(0)->StopVibration();
                m_sound104 = NULL;
            }

            m_flags &= ~(1 << 20);
            m_unkC = 2;

            if (m_flags & (1 << 24)) {
                CDKW_RGBA fade_color = m_game->ComputeGameFadeColor();
                m_game->FadeInit(0.5f, CGame::FADE_TYPE_4, fade_color.m_r, fade_color.m_g, fade_color.m_b, 0.0f);
                m_game->m_unk503C &= ~(1 << 3);
                m_unkC = 1;
            }

            m_game->GetGuiManager()->HideAllUnPauseGUI();
            break;
        case 1:
            if (m_game->FadeIn(m_delta_time)) {
                break;
            }

            m_unkC = 2;
            // fallthrough
        case 2:
            if (m_game->m_unk4F54 != 7) {
                CEntityManager* manager = m_game->m_entity_manager;
                for (U32 i = 0; i < m_game->m_entity_manager->GetEntityCount(); i++) {
                    CEntity* entity = manager->GetEntity(i);
                    if (entity == NULL) {
                        continue;
                    }

                    U32 type = entity->GetType();
                    if (type > ENTITY_NPC23 && type < ENTITY_UNK36) {
                        CEntityNPC* npc = (CEntityNPC*)entity;
                        npc->AddFlag(ENTITY_FLAG_UNK8);
                        if (npc->GetGenericBehaviour() != GENERIC_BEHAVIOUR_6) {
                            npc->SaveCurrentAnim();
                            npc->Update(0.001f);
                        }
                    } else if (type == ENTITY_PIGLET) {
                        ((CEntityPiglet*)entity)->AddFlag(ENTITY_FLAG_UNK8);
                    } else if (type == ENTITY_ESCAPING_OBJECT) {
                        ((CEntityEscapingObject*)entity)->AddFlag(ENTITY_FLAG_UNK8);
                    } else if (type == ENTITY_TIMER) {
                        ((CEntityTimer*)entity)->AddFlag(ENTITY_FLAG_UNK7);
                    }
                }
            }

            m_player_entity->Start();
            m_game->m_unk503C &= ~(1 << 0);
            SetState(ROOM_STATE_4, 1);
            m_unkC = 0;
            break;
    }
}

void CGameRoomManager::OnPlayRTCUpdate() {
    switch (m_unkC) {
        case 0:
            if (m_player_entity->IsFinished() || m_flags & (1 << 20)) {
                m_flags &= ~(1 << 20);
                m_unkC = 1;

                if (m_game->m_unk4F54 != 7) {
                    CEntityManager* manager = m_game->m_entity_manager;
                    for (U32 i = 0; i < m_game->m_entity_manager->GetEntityCount(); i++) {
                        CEntity* entity = manager->GetEntity(i);
                        if (entity == NULL) {
                            continue;
                        }

                        U32 type = entity->GetType();
                        if (type > ENTITY_NPC23 && type < ENTITY_UNK36) {
                            CEntityNPC* npc = (CEntityNPC*)entity;
                            npc->Reset();
                            npc->UpdateVolumes();
                            npc->DelFlag(ENTITY_FLAG_UNK8);

                            if (!npc->IsFlagged(ENTITY_FLAG_UNK6) && npc->IsFlagged(ENTITY_FLAG_VISIBLE)) {
                                npc->RestoreSavedAnim();
                                npc->UpdateAnimations(5.0f * rand() / 32767.0f);
                            }
                        } else if (type == ENTITY_PIGLET) {
                            ((CEntityPiglet*)entity)->DelFlag(ENTITY_FLAG_UNK8);
                        } else if (type == ENTITY_ESCAPING_OBJECT) {
                            ((CEntityEscapingObject*)entity)->DelFlag(ENTITY_FLAG_UNK8);
                        } else if (type == ENTITY_TIMER) {
                            ((CEntityTimer*)entity)->DelFlag(ENTITY_FLAG_UNK7);
                        }
                    }

                    m_flags &= ~(1 << 0);
                    m_flags &= ~(1 << 1);
                }
            }
            break;
        case 1:
            SetState(ROOM_STATE_5, 1);
            break;
    }
}

void CGameRoomManager::SecondPass() {
    if (m_flags & (1 << 3)) {
        SetState(ROOM_STATE_9, 1);
        return;
    } else if (m_flags & (1 << 2)) {
        m_flags &= ~(1 << 2);
        SetState(ROOM_STATE_3, 1);
        m_unk14 = m_unkC;
        m_unkC = 0;
        OnPlayRTCInit();
        return;
    } else if (!(m_flags & (1 << 25))) {
        if (m_game->GetCurrentMission().m_rooms[m_game->m_unk4F58] & (1 << 2)) {
            if (m_flags & (1 << 16)) {
                m_flags &= ~(1 << 17);
            } else {
                m_flags |= (1 << 16);
                m_flags |= (1 << 17);
                m_unk14 = m_unkC;
                m_unk10 = 10;
                SetState(ROOM_STATE_14, 1);
                return;
            }
        }
    }

    switch (m_unkC) {
        case 0:
            m_game->m_unk503C &= ~(1 << 0);

            if (!m_game->GetCurrentMission().IsRoomCompleted(m_game->GetUnk4F58())) {
                for (U32 i = 0; i < m_game->m_entity_manager->GetEntityCount(); i++) {
                    CEntity* entity = m_game->m_entity_manager->GetEntity(i);
                    U32 type = entity->GetType();
                    if (type >= ENTITY_NPC23 && type <= ENTITY_UNK36) {
                        CEntityNPC* npc = (CEntityNPC*)entity;
                        if (type == ENTITY_NPC_DOOR || type == ENTITY_NPC_TREE) {
                            m_unk120--;
                            npc->AddFlag(ENTITY_FLAG_UNK6);
                            npc->DelFlag(ENTITY_FLAG_VISIBLE);
                            npc->DelFlag(ENTITY_FLAG_ACTIVE);
                            npc->DelFlag(ENTITY_FLAG_HAS_COLLISION);
                        } else {
                            DkPh::Collider::Body& body = npc->m_entity_manager->m_unk1C->GetBodyRef(npc->m_unk90.unk0);
                            body.unk34 = -1;
                            npc->m_unkF4 |= (1 << 12);
                            npc->m_unk1B0 = npc->m_flags;
                            npc->AddFlag(ENTITY_FLAG_ACTIVE);
                            npc->AddFlag(ENTITY_FLAG_VISIBLE);
                            npc->AddFlag(ENTITY_FLAG_HAS_COLLISION);
                            npc->SetGenericBehaviour(GENERIC_BEHAVIOUR_0);
                            npc->SetPosition(npc->m_unk118);
                            npc->SetBehavior(0xFFFF);
                            npc->m_unk14 = 0xFFFF;
                        }
                    }
                }

                if (m_sound100 != NULL) {
                    m_sound100->Stop();
                    m_game->m_sound_engine->RemoveSound(m_sound100);
                    m_sound100 = NULL;
                }

                if (m_unk120 <= 0) {
                    m_game->GetCurrentMission().m_rooms[m_game->GetUnk4F58()] |= (1 << 1);
                    m_unkC = 4;
                    m_hero->SetMode(0);
                } else {
                    m_unkC = 9;
                    m_unkE0 = 1.5f;
                }
            } else {
                for (U32 i = 0; i < m_game->m_entity_manager->GetEntityCount(); i++) {
                    CEntity* entity = m_game->m_entity_manager->GetEntity(i);
                    U32 type = entity->GetType();
                    if (type >= ENTITY_NPC23 && type <= ENTITY_UNK36) {
                        CEntityNPC* npc = (CEntityNPC*)entity;
                        if (type == ENTITY_NPC_DOOR || type == ENTITY_NPC_TREE) {
                            npc->DelFlag(ENTITY_FLAG_VISIBLE);
                            npc->DelFlag(ENTITY_FLAG_ACTIVE);
                            npc->DelFlag(ENTITY_FLAG_HAS_COLLISION);
                        } else {
                            npc->SetDeadInSecondPass();
                        }
                    }
                }

                m_unkC = 8;
                m_hero->SetMode(0);
            }
            break;
        case 8:
            if (!(m_flags & (1 << 21)) && m_game->GetCurrentMission().IsSecondPassCompleted() != 0u) {
                if (m_unk154 & (1 << 31)) {
                    m_game->FadeInit(1.0f, CGame::FADE_TYPE_4, m_unk154 & 0xFF, (m_unk154 & 0xFF00) >> 8, (m_unk154 & 0xFF0000) >> 16, 0.0f);
                } else {
                    m_game->FadeInit(1.0f, CGame::FADE_TYPE_2, 0, 0, 0, 0.0f);
                }

                CGame* game = m_game;
                game->m_unk503C |= (1 << 2);
                game->m_unk5048 = 2.0f;
                m_hero->SetMode(11);
                m_unkC = 12;
            }

            UpdateAdventureCam();
            break;
        case 9:
            SetState(ROOM_STATE_15, 1);
            break;
        case 2:
            m_unkC = 3;
            // fallthrough
        case 3:
            DisplayTicTac();
            UpdateAdventureCam();
            break;
        case 4:
            if (!(m_flags & (1 << 21)) && m_game->GetCurrentMission().IsSecondPassCompleted() != 0u) {
                m_flags |= (1 << 26);
                m_hero->SetMode(11);
                SetState(ROOM_STATE_15, 1);
            }

            m_flags &= ~(1 << 18);
            UpdateAdventureCam();
            break;
        case 5:
            for (U32 i = 0; i < m_game->m_entity_manager->GetEntityCount(); i++) {
                CEntity* entity = m_game->m_entity_manager->GetEntity(i);
                U32 type = entity->GetType();
                if (type >= ENTITY_NPC23 && type <= ENTITY_UNK36) {
                    CEntityNPC* npc = (CEntityNPC*)entity;
                    npc->SetGenericBehaviour(GENERIC_BEHAVIOUR_6);
                    npc->m_unk1A8 = 9;
                    if (npc->m_phial != NULL) {
                        npc->m_phial->m_unkF4 = 6;
                    }
                }
            }

            m_unkC = 6;
            StopTimer();
            UpdateAdventureCam();
            break;
        case 6:
            UpdateAdventureCam();
            break;
        case 7: {
            m_game->ResetOpcodeBuffer();
            m_game->PushOpcodeValue(4);

            CGamePart* game_part = m_game->GetGamePartPointer();
            game_part->m_unk4 = 0;
            m_game->SetCurrentRoomReturnType(CGame::RETURN_TYPE_0, -1);
            break;
        }
        case 12:
            m_game->FadeIn(m_delta_time);
            if (m_game->GetUnk502C() >= m_game->GetFadeDuration()) {
                m_game->ResetOpcodeBuffer();
                m_game->PushOpcodeValue(5);

                CGamePart* game_part = m_game->GetGamePartPointer();
                game_part->m_unk4 = 0;
                m_game->SetCurrentRoomReturnType(CGame::RETURN_TYPE_0, -1);
            }
            break;
    }
}

void CGameRoomManager::StopTimer() {
    m_unk158 = 0.0f;

    if (m_batch44 != NULL) {
        m_game->m_display_engine->GetImmediate()->RemoveBatch2D(m_batch44);
        m_batch44 = NULL;
    }
    if (m_batch48 != NULL) {
        m_game->m_display_engine->GetImmediate()->RemoveBatch2D(m_batch48);
        m_batch48 = NULL;
    }

    if (m_sound104 != NULL) {
        m_sound104->Stop();
        m_game->m_sound_engine->RemoveSound(m_sound104);
        m_sound104 = NULL;

        DKI::IInputEngine::GetDevice(0)->StopVibration();
    }

    m_unk40 = 0.0f;
}

void CGameRoomManager::CreateScreenEffect(DkXmd::CChunkIterator iter) {
    if (m_screen_effect == NULL) {
        m_screen_effect = new CScreenEffect(m_game);
    }
    m_screen_effect->Parse(iter);
    m_screen_effect->SetSequenceByIndex(0);
}
