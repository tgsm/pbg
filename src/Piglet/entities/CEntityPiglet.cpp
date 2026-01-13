#include "engine/sound/DkSound.h"
#include "CGame.h"
#include "CGamePartIngame.h"
#include "CIcon.h"
#include "entities/CEntityManager.h"
#include "entities/CEntityPiglet.h"
#include "entities/CEntityPushingBox.h"
#include "entities/CEntityZone.h"
#include <math.h>
#include <iostream>

CEntityPiglet::~CEntityPiglet() {
    for (int i = 0; i < 6; i++) {
        m_entity_manager->GetGame()->m_display_engine->GetImmediate()->RemoveBatch2D(m_batches[i]);
        m_batches[i] = NULL;
    }
}

U32 CEntityPiglet::GetSaveSize() {
    return 0x10;
}

void CEntityPiglet::SetStateEnvironement() {
    std::string unused;

    switch (m_state) {
        case 0:
            m_entity_manager->GetGame()->m_screen_effect->SetSequenceByIndex(0);
            break;
        case 1:
            m_entity_manager->GetGame()->m_screen_effect->SetSequenceByIndex(1);
            break;
        case 2:
            m_entity_manager->GetGame()->m_screen_effect->SetSequenceByIndex(2);
            break;
    }
}

void CEntityPiglet::AddFlag(U32 flag) {
    switch (flag) {
        case ENTITY_FLAG_UNK3:
            m_unk284 = 0;
            m_unk280 = 0.0f;
            break;
        case ENTITY_FLAG_UNK4:
            ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->UnblockFightMode();
            break;
    }

    HideOSD();
    CEntityHero::AddFlag(flag);
}

void CEntityPiglet::DelFlag(U32 flag) {
    switch (flag) {
        case ENTITY_FLAG_UNK3:
            SetMode(0);
            UpdateAnimations_Wait(0.0f, 1);
            break;
    }

    CEntityHero::DelFlag(flag);
}

void CEntityPiglet::AddItem(char* item) {
    // Don't add the item if we're already carrying it.
    for (int i = 0; i < 3; i++) {
        char* current_item = m_entity_manager->GetGame()->GetCurrentMission()->GetItem(i);
        if (current_item != NULL && RwEngineInstance->stringFuncs.rwstrcmp(item, current_item) == 0) {
            return;
        }
    }

    int result = m_entity_manager->GetGame()->GetCurrentMission()->AddItem((S8*)item);
    if (result != -1) {
        std::string name = "SND_204_2D";
        DKSND::CSound2D* sound = DkSoundGetEngine()->PlaySound2D(&name, 1);
        if (sound != NULL) {
            sound->SetLayer(1);
        }
        UnHideOSD(result);
    }
}

void CEntityPiglet::RemoveItem(char* item) {
    m_entity_manager->GetGame()->GetCurrentMission()->RemoveItem((S8*)item);

    std::string name = "SND_205_2D";
    DKSND::CSound2D* sound = DkSoundGetEngine()->PlaySound2D(&name, 1);
    if (sound != NULL) {
        sound->SetLayer(1);
    }
}

void CEntityPiglet::RemoveItem(int item) {
    m_entity_manager->GetGame()->GetCurrentMission()->RemoveItem(item);

    std::string name = "SND_205_2D";
    DKSND::CSound2D* sound = DkSoundGetEngine()->PlaySound2D(&name, 1);
    if (sound != NULL) {
        sound->SetLayer(1);
    }
}

void CEntityPiglet::PushStart(CEntityPushingBox* box) {
    m_unk270 = 0;
    m_pushing_box = box;
    SetMode(16);
}

void CEntityPiglet::PushPush(CEntityPushingBox* box) {
    if (m_unk270 == 0) {
        m_unk270 = 1;
    }
}

void CEntityPiglet::PushBloc(CEntityPushingBox* box) {
    m_unk270 = 0;
    m_pushing_box = box;
    SetMode(17);
}

void CEntityPiglet::PushStop(CEntityPushingBox* box) {
    m_unk270 = 0;
    m_pushing_box = NULL;
    SetMode(0);
}

void CEntityPiglet::ManageMessage(SDkMessage& message) {
    if (strcmp(message.unk20, "ENTER") == 0) {
        SetMode(14);
    }

    if (strcmp(message.unk20, "EXIT") == 0) {
        CEntity* entity = m_entity_manager->GetEntity(message.unk0);
        if (entity != NULL) {
            int type = entity->GetType();
            if (type > ENTITY_UNK5 && type < ENTITY_UNK9) {
                m_unk260 = ((CEntityZone*)entity)->GetPosition();
                SetMode(10);
            }
        }
    }

    if (strcmp(message.unk20, "PUSH_START") == 0) {
        PushStart((CEntityPushingBox*)m_entity_manager->GetEntity(message.unk0));
    } else if (strcmp(message.unk20, "PUSH_PUSH") == 0) {
        PushPush((CEntityPushingBox*)m_entity_manager->GetEntity(message.unk0));
    } else if (strcmp(message.unk20, "PUSH_BLOC") == 0) {
        PushBloc((CEntityPushingBox*)m_entity_manager->GetEntity(message.unk0));
    } else if (RwEngineInstance->stringFuncs.rwstrcmp(message.unk20, "PUSH_STOP") == 0) {
        PushStop((CEntityPushingBox*)m_entity_manager->GetEntity(message.unk0));
    }

    if (GetMode() != 2) {
        char buf[32];
        if (strcmp(message.unk20, "GIVE_FULL_LIFE") == 0) {
            m_state = 0;
            m_unk13C = 3;
            SetStateEnvironement();
        } else if (strncmp(message.unk20, "DEL", RwEngineInstance->stringFuncs.rwstrlen("DEL")) == 0) {
            int name_index = RwEngineInstance->stringFuncs.rwstrlen("DEL");
            strncpy(buf, &message.unk20[name_index], RwEngineInstance->stringFuncs.rwstrlen(message.unk20) - name_index + 1);
            RemoveItem(buf);
        } else if (strncmp(message.unk20, "ADD", RwEngineInstance->stringFuncs.rwstrlen("ADD")) == 0) {
            int index = RwEngineInstance->stringFuncs.rwstrlen("ADD");
            strncpy(buf, &message.unk20[index], RwEngineInstance->stringFuncs.rwstrlen(message.unk20) - index + 1);
            AddItem(buf);

            int name_index = RwEngineInstance->stringFuncs.rwstrlen("ITEM_") - 1;
            strncpy(m_unk2B4, &buf[name_index], RwEngineInstance->stringFuncs.rwstrlen(buf) - name_index + 1);
            m_unk2B4[0] = 'O';
        }
    } else {
        if (strncmp(message.unk20, "GRIMACE", RwEngineInstance->stringFuncs.rwstrlen("GRIMACE")) == 0) {
            if (strcmp(message.unk20, "GRIMACE01") == 0) {
                SetGrimace(1);
            } else if (strcmp(message.unk20, "GRIMACE02") == 0) {
                SetGrimace(2);
            } else if (strcmp(message.unk20, "GRIMACE03") == 0) {
                SetGrimace(3);
            } else if (strcmp(message.unk20, "GRIMACE04") == 0) {
                SetGrimace(4);
            } else if (strcmp(message.unk20, "GRIMACE05") == 0) {
                SetGrimace(5);
            } else if (strcmp(message.unk20, "GRIMACE06") == 0) {
                SetGrimace(6);
            } else if (strcmp(message.unk20, "GRIMACE08") == 0) {
                SetGrimace(7);
            } else if (strcmp(message.unk20, "GRIMACE09") == 0) {
                SetGrimace(8);
            } else if (strcmp(message.unk20, "GRIMACE10") == 0) {
                SetGrimace(9);
            } else if (strcmp(message.unk20, "GRIMACE11") == 0) {
                SetGrimace(10);
            }

            SetMode(13);
        }
    }

    CEntityHero::ManageMessage(message);
}

void CEntityPiglet::ResolveContact(const DkPh::Collider::Body& body, int a2, int a3) {
    CEntity* entity = body.entity4;
    CEntityObject::ResolveContact(body, a2, a3);

    int type = entity->GetType();

    BOOL iVar8 = TRUE;
    if (type > ENTITY_UNK5 && type < ENTITY_UNK9) {
        iVar8 = ((CEntityZone*)entity)->ShouldKick();
        if (!iVar8 && (GetMode() == 19 || GetMode() == 20)) {
            SetMode(0);
        }

        switch (type) {
            case ENTITY_SEARCHABLE_ZONE: {
                F32 time = m_animation_star_controller->GetPlayingAnimationTime();
                if (m_unk26C == 0) {
                    if (m_animation_star_controller->IsPlayingAnimation("SEARCH") == TRUE) {
                        if (time > 0.35f) {
                            m_entity_manager->GetGame()->GetMailbox()->SendMessage(m_unk0, entity->m_unk0, "ACTION", 0);
                            m_unk26C = 1;
                        }
                    } else if (m_animation_star_controller->IsPlayingAnimation("SEARCH_RUN") == TRUE) {
                        if (time > 0.18f) {
                            m_entity_manager->GetGame()->GetMailbox()->SendMessage(m_unk0, entity->m_unk0, "ACTION", 0);
                            m_unk26C = 1;
                        }
                    }
                }
                break;
            }
            default:
                if (!iVar8) {
                    m_unk2A8 = 1;
                }
                if (((CEntityZone*)entity)->m_unk1B8 == 2) {
                    m_unk2AC = 1;
                }

                if (GetMode() == 0 || GetMode() == 1) {
                    if (m_unk124 & (1 << 0)) {
                        m_entity_manager->GetGame()->GetMailbox()->SendMessage(m_unk0, entity->m_unk0, "ACTION", 0);
                    }
                }
                break;
        }
    }

    if (GetMode() == 18) {
        m_animation_star_controller->GetPlayingAnimationTime();
        if (m_unk26C == 0 && m_animation_star_controller->IsPlayingAnimation("USE_OBJECT") == TRUE) {
            if ((m_animation_star_controller->GetPlayingAnimationTime() > 0.85f * m_animation_star_controller->GetPlayingAnimationDuration()) && !iVar8) {
                char* item = m_entity_manager->GetGame()->GetCurrentMission()->GetItem(m_unk2A0);
                m_unk26C = 1;

                char buf[32];
                RwEngineInstance->stringFuncs.rwsprintf(buf, "%s%s", "USE", item);

                if (item != NULL) {
                    m_entity_manager->GetGame()->GetMailbox()->SendMessage(m_unk0, entity->m_unk0, buf, 0);

                    std::string name = "SND_206_2D";
                    DKSND::CSound2D* sound = DkSoundGetEngine()->PlaySound2D(&name, 1);
                    if (sound != NULL) {
                        sound->SetLayer(1);
                    }
                }
            }
        }
    }
}

void CEntityPiglet::SetGrimace(int grimace) {
    switch (grimace) {
        case 1:
            m_animation_star_controller->Play("GRIMACE01", 1);
            break;
        case 2:
            m_animation_star_controller->Play("GRIMACE02", 1);
            break;
        case 3:
            m_animation_star_controller->Play("GRIMACE03", 1);
            break;
        case 4:
            m_animation_star_controller->Play("GRIMACE04", 1);
            break;
        case 5:
            m_animation_star_controller->Play("GRIMACE05", 1);
            break;
        case 6:
            m_animation_star_controller->Play("GRIMACE06", 1);
            break;
        case 7:
            m_animation_star_controller->Play("GRIMACE08", 1);
            break;
        case 8:
            m_animation_star_controller->Play("GRIMACE09", 1);
            break;
        case 9:
            m_animation_star_controller->Play("GRIMACE10", 1);
            break;
        case 10:
            m_animation_star_controller->Play("GRIMACE11", 1);
            break;
    }
}

void CEntityPiglet::DicreaseLife(int life) {
    m_unk13C -= life;

    if (life == 0) {
        SetMode(5);
        return;
    }

    SetMode(4);

    if (m_unk13C <= 0) {
        m_state = 2;
        SetMode(7);
    } else if (m_unk13C <= 1) {
        m_animation_star_controller->Play("WORRIED_TO_PANIC", 1);
        m_state = 2;
        SetStateEnvironement();
    } else if (m_unk13C <= 2) {
        m_animation_star_controller->Play("BASE_TO_WORRIED", 1);
        m_state = 1;
        SetStateEnvironement();
    }
}

void CEntityPiglet::SetMode(U32 mode) {
    if (mode == GetMode()) {
        return;
    }

    switch (mode) {
        case 0:
            if (m_pushing_box != NULL) {
                return;
            }
            break;
        case 9:
            DelFlag(ENTITY_FLAG_UNK4);
            SetMode(0);
            return;
        case 1: {
            CDKW_Frame* frame = m_clump->GetFrame();
            m_unk260 = CDKW_V3d(frame->m_rwframe->modelling.at);

            if (m_unk25C > 0) {
                m_animation_star_controller->Play("TURN_BACK_LEFT", 1);
            } else {
                m_animation_star_controller->Play("TURN_BACK_RIGHT", 1);
            }
            break;
        }
        case 2:
            if (GetMode() == 16 || GetMode() == 17) {
                ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->InterruptFightMode();
                return;
            }

            m_unk1C8 = 0.0f;
            AddFlag(ENTITY_FLAG_UNK4);
            break;
        case 14:
            m_animation_star_controller->Play("START_FIGHT", 1);
            break;
        case 15: {
            // TODO
            if (!IsFlagged(ENTITY_FLAG_ACTIVE)) {
                SetMode(2);
            }

            m_animation_star_controller->Play("TURN_FIGHT", 1);
            m_unk260 = m_unk1BC.Normalized();
            break;
        }
        case 16:
        case 17:
            ((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->InterruptFightMode();
            m_unk1BC = m_pushing_box->GetDirection();
            m_unk90.unk8 = CDKW_V3d::ZERO;
            m_unk90.unk14 = CDKW_V3d::ZERO;
            break;
        case 18:
            UnHideOSD(m_unk2A0);
            m_animation_star_controller->Play("USE_OBJECT", 1);
            break;
        case 21:
            m_animation_star_controller->Play("VICTORY", 1);
            SetMode(11);
            m_unk1C8 = 0.0f;
            break;
        case 19:
            if (m_unk138 < 0.2f) {
                return;
            }
            if (GetSpeed() <= m_speed_slow) {
                m_animation_star_controller->Play("SEARCH", 1);
                mode = 19;
            } else {
                m_animation_star_controller->Play("SEARCH_RUN", 1);
                mode = 20;
            }
            break;
        case 10:
            HideOSD();
            break;
        case 7:
            m_animation_star_controller->Play("DEAD_0", 1);
            break;
        case 8: {
            CDKW_V3d position;
            m_animation_star_controller->Play("DEAD_1", 1, 1);
            CDKW_Frame* frame = m_clump->GetFrame();
            position = CDKW_V3d(frame->GetRwFrame()->modelling.pos);
            RwFrameTranslate(frame->GetRwFrame(), &(-position), 2);
            RwFrameRotate(180.0f, frame->GetRwFrame(), &CDKW_V3d::YAXIS, 2);
            RwFrameTranslate(frame->GetRwFrame(), &position, 2);

            m_unk1BC = -m_unk1BC;
            m_unk1C8 = m_speed_run;
            m_gravity = 0.0f;
            break;
        }
    }

    CEntityHero::SetMode(mode);
    m_unk26C = 0;
}

void CEntityPiglet::UpdateAnimations(F32 dt) {
    switch (GetMode()) {
        case 0:
            UpdateAnimations_AdventureMode(dt);
            break;
        case 2:
            UpdateAnimations_AdventureMode(dt);
            break;
        case 15:
            if (m_animation_star_controller->IsPlayingAnimation("TURN_FIGHT") == TRUE) {
                if (m_animation_star_controller->GetPlayingAnimationTime() > m_animation_star_controller->GetPlayingAnimationDuration() * 0.75f) {
                    UpdateAnimations_AdventureMode(dt);
                    SetMode(2);
                }
            }
            break;
        case 14:
            if (m_animation_star_controller->IsPlayingAnimation("START_FIGHT") == TRUE && m_animation_star_controller->IsPlayingAnimationLooped()) {
                UpdateAnimations_AdventureMode(dt);
                if (m_entity_manager->GetGame()->m_unk4F5C == 3) {
                    SetMode(0);
                } else {
                    SetMode(2);
                }
            }
            break;
        case 13:
            if (m_animation_star_controller->IsPlayingAnimation("GRIMACE01") == TRUE ||
                m_animation_star_controller->IsPlayingAnimation("GRIMACE02") == TRUE ||
                m_animation_star_controller->IsPlayingAnimation("GRIMACE03") == TRUE ||
                m_animation_star_controller->IsPlayingAnimation("GRIMACE04") == TRUE ||
                m_animation_star_controller->IsPlayingAnimation("GRIMACE05") == TRUE ||
                m_animation_star_controller->IsPlayingAnimation("GRIMACE06") == TRUE ||
                m_animation_star_controller->IsPlayingAnimation("GRIMACE08") == TRUE ||
                m_animation_star_controller->IsPlayingAnimation("GRIMACE09") == TRUE ||
                m_animation_star_controller->IsPlayingAnimation("GRIMACE10") == TRUE ||
                m_animation_star_controller->IsPlayingAnimation("GRIMACE11") == TRUE) {
                if (m_animation_star_controller->IsPlayingAnimationLooped()) {
                    SetMode(2);
                }
            }
            break;
        case 5:
            if (m_animation_star_controller->IsPlayingAnimation("WAIT") == TRUE ||
                m_animation_star_controller->IsPlayingAnimation("WAIT_WORRIED") == TRUE ||
                m_animation_star_controller->IsPlayingAnimation("WAIT_PANIC") == TRUE) {
                if (m_animation_star_controller->GetPlayingAnimationTime() > 0.25f) {
                    SetMode(6);
                    UpdateAnimations_AdventureMode(dt);
                }
            }
            break;
        case 6:
            UpdateAnimations_AdventureMode(dt);
            break;
        case 1:
            if (m_animation_star_controller->IsPlayingAnimation("TURN_BACK_LEFT") == TRUE ||
                m_animation_star_controller->IsPlayingAnimation("TURN_BACK_RIGHT") == TRUE) {
                if (m_animation_star_controller->GetPlayingAnimationTime() > m_animation_star_controller->GetPlayingAnimationDuration() * 0.85f) {
                    SetMode(0);
                    UpdateAnimations_AdventureMode(dt);
                }
            }
            break;
        case 16:
        case 17:
            switch (m_unk270) {
                case 0:
                    m_animation_star_controller->Play("WALK", 1);
                    break;
                case 1:
                    m_animation_star_controller->Play("PUSH_START", 1);
                    m_unk270 = 2;
                    break;
                case 2:
                    if (m_animation_star_controller->IsPlayingAnimation("PUSH_START") == TRUE && m_animation_star_controller->IsPlayingAnimationLooped()) {
                        m_entity_manager->GetGame()->GetMailbox()->SendMessage(m_unk0, m_pushing_box->m_unk0, "START", 0);

                        if (GetMode() == 16) {
                            m_animation_star_controller->Play("PUSH_LOOP", 1);
                        } else {
                            m_animation_star_controller->Play("PUSH_BLOC", 1);
                        }
                    }
                    break;
            }
            break;
        case 18:
            if (m_animation_star_controller->IsPlayingAnimation("USE_OBJECT") == TRUE && m_animation_star_controller->IsPlayingAnimationLooped()) {
                char* item = m_entity_manager->GetGame()->GetCurrentMission()->GetItem(m_unk2A0);
                if (item != NULL) {
                    char buf[32];

                    int name_index = RwEngineInstance->stringFuncs.rwstrlen("ITEM_") - 1;
                    strncpy(buf, &item[name_index], RwEngineInstance->stringFuncs.rwstrlen(item) - name_index + 1);
                    buf[0] = 'O';
                    strcpy(&buf[RwEngineInstance->stringFuncs.rwstrlen(buf)], "B");

                    m_entity_manager->GetGame()->PlayNarratorLine(buf);
                }

                SetMode(0);
                UpdateAnimations_AdventureMode(dt);
            }
            break;
        case 19:
            if (m_animation_star_controller->IsPlayingAnimation("SEARCH") == TRUE && m_animation_star_controller->IsPlayingAnimationLooped()) {
                SetMode(0);
                m_pad_timer = 0.0f;
                UpdateAnimations_AdventureMode(dt);
            }
            break;
        case 20:
            if (m_animation_star_controller->IsPlayingAnimation("SEARCH_RUN") == TRUE && m_animation_star_controller->IsPlayingAnimationLooped()) {
                SetMode(0);
                UpdateAnimations_AdventureMode(dt);
            }
            break;
        case 21:
            if (m_animation_star_controller->IsPlayingAnimation("VICTORY") == TRUE && m_animation_star_controller->IsPlayingAnimationLooped()) {
                SetMode(11);
                UpdateAnimations_Wait(dt, 0);
            }
            break;
        case 4:
            if (m_animation_star_controller->IsPlayingAnimation("BASE_TO_WORRIED") == TRUE ||
                m_animation_star_controller->IsPlayingAnimation("WORRIED_TO_PANIC") == TRUE) {
                if (m_animation_star_controller->IsPlayingAnimationLooped() == TRUE) {
                    SetMode(5);
                    UpdateAnimations_Wait(dt, 0);
                }
            }
            break;
        case 7:
            if (m_animation_star_controller->IsPlayingAnimation("DEAD_0") == TRUE && m_animation_star_controller->IsPlayingAnimationLooped()) {
                SetMode(8);
            }
            break;
        case 10:
            if (m_state == 1) {
                m_pad_timer = 0.0f;
                m_unk134 = 1;
            }
            UpdateAnimations_AdventureMode(dt);
            break;
        case 11:
            UpdateAnimations_AdventureMode(dt);
            break;
    }

    CEntityHero::UpdateAnimations(dt);
}

// Incomplete: extra copies on the stack, maybe other things
void CEntityPiglet::UpdateMoving(F32 dt) {
    F32 dVar11 = 0.0f;
    m_unk134 = 0;

    HeroMoveParams params;
    params.unk8 = m_friction;
    params.unk4 = m_acceleration;
    params.unk10 = m_speed_rotate;

    switch (GetMode()) {
        case 0: {
            CDKW_V3d pad_dir = GetPadDirection();

            if (m_entity_manager->GetGame()->FadeRelatedInline() == 0.0f) {
                ConvertPadToDirection(pad_dir, &m_unk1BC, &m_unk1C8);
            }

            break;
        }
        case 10:
            m_unk1BC = m_unk260 - GetPosition();
            m_unk1BC.Normalize();

            m_unk1C8 = (m_state == 0) ? m_speed_run : m_speed_run_panic;

            break;
        case 1:
            if (m_animation_star_controller->IsPlayingAnimation("TURN_BACK_LEFT") == TRUE ||
                m_animation_star_controller->IsPlayingAnimation("TURN_BACK_RIGHT") == TRUE) {
                dVar11 = m_animation_star_controller->GetPlayingAnimationTime();
            }
            if (dVar11 < 0.46f) {
                m_unk1C8 = 0.0f;
                params.unk10 = m_speed_rotate_turn_back;
                params.unk8 = InterpolValue(m_friction_turn_back, m_friction, dVar11, 0.46f);
            } else {
                CDKW_V3d dir = GetPadDirection();
                ConvertPadToDirection(dir, &m_unk1BC, &m_unk1C8);
                params.unk10 = 0.0f;
            }
            break;
        case 20: {
            CDKW_V3d dir = GetPadDirection();
            ConvertPadToDirection(dir, &m_unk1BC, &m_unk1C8);
            m_unk1C8 = GetSpeed();

            params.unk8 = m_unk278;
            params.unk4 = m_acceleration;
            params.unk10 = m_speed_rotate / 2;
            break;
        }
        case 16:
        case 17:
            m_unk1C8 = 2.0f * m_speed_walk;
            break;
        case 15:
            if (m_animation_star_controller->IsPlayingAnimation("TURN_FIGHT") == TRUE) {
                if (m_animation_star_controller->GetPlayingAnimationTime() - 0.23333333f >= 0.0f) {
                    F32 x = (F32)atan2(m_unk260.x, m_unk260.z) - 3.1241393f;
                    m_unk1BC.x = sin(x);
                    m_unk1BC.z = cos(x);
                }
            }
            m_unk1C8 = 0.0f;
            params.unk10 = 6.5f;
            break;
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 13:
        case 14:
            m_unk1C8 = 0.0f;
            break;
        default:
            m_unk1C8 = 0.0f;
            params.unk10 = 0.0f;
            break;
        case 8:
            break;
    }

    params.unk0 = m_unk1C8;
    if (params.unk0 != 0.0f) {
        params.unkC = params.unk0 / 3.0f;
    } else {
        params.unkC = m_stop_speed;
    }

    Move(params, dt);
}

void CEntityPiglet::Update(F32 dt) {
    m_unk2A8 = 0;
    m_unk2AC = 0;

    CEntityHero::Update(dt);

    UpdateOSD_Impl(dt);
}

void CEntityPiglet::Render(F32 dt_maybe) {
    CEntityHero::Render(dt_maybe);
    CEntityPiglet::RenderOSD();
}

void CEntityPiglet::UpdateActions() {
    if ((GetMode() == 0 || GetMode() == 1) && (int)m_entity_manager->GetGame()->m_unk4F54 != 8) {
        if (m_unk124 & (1 << 1)) {
            m_unk2A0 = 0;
            SetMode(18);
        } else if (m_unk124 & (1 << 2)) {
            m_unk2A0 = 1;
            SetMode(18);
        } else if (m_unk124 & (1 << 3)) {
            m_unk2A0 = 2;
            SetMode(18);
        }
    }

    if (GetMode() == 0 || GetMode() == 1) {
        if (m_unk124 & (1 << 0)) {
            if (((CGamePartIngame*)m_entity_manager->GetGame()->GetGamePartPointer())->HeroAskForFightModeLaunch() != TRUE) {
                if (m_unk2A8 == 0) {
                    SetMode(19);
                }
            }
        }
    }

    CEntityHero::UpdateActions();
}

void CEntityPiglet::Parse(DkXmd::CChunkIterator iter) {
    CEntityHero::Parse(iter);
    m_unk278 = m_friction;

    std::string tmp;
    DkXmd::CChunkIterator dest;

    if (iter.GetFirstChildChunk(dest) == TRUE) {
        do {
            tmp = dest.GetName();

            if (tmp == "FrictionShoot") {
                m_unk278 = dest.GetFloatValue();
            }
        } while (dest.GetNextSiblingChunk(dest) == TRUE);
    }
}

void CEntityPiglet::PlayOSDSound(std::string name, int a2) {
    if (m_unk2D4 != NULL) {
        m_unk2D4->IsLooping();
        m_unk2D4->SetLoopMode(0);
        m_unk2D4 = NULL;
    }

    if (a2 == 0) {
        DKSND::CSound2D* sound = DkSoundGetEngine()->PlaySound2D(&name, 1);
        if (sound != NULL) {
            sound->SetLayer(1);
        }
    } else {
        m_unk2D4 = DkSoundGetEngine()->PlaySound2DLooped(&name, 1);
    }

    if (m_unk2D4 != NULL) {
        m_unk2D4->SetLayer(1);
    }
}

void CEntityPiglet::HideOSD() {
    switch (m_unk284) {
        case 0:
            break;
        case 2:
            PlayOSDSound("SND_202_2D", 0);
            m_unk284 = 3;
            m_unk280 = 0.0f;
            break;
        case 1:
            PlayOSDSound("SND_202_2D", 0);
            m_unk284 = 3;
            m_unk280 = 0.2f - m_unk280;
            break;
        case 3:
            PlayOSDSound("SND_202_2D", 0);
            break;
    }
}

void CEntityPiglet::UnHideOSD(U32 unk) {
    m_unk2A0 = unk;
    PlayOSDSound("SND_201_2D", 0);

    switch (m_unk284) {
        case 0:
            m_unk284 = 1;
            m_unk280 = 0.0f;
            break;
        case 2:
            m_unk280 = 0.2f;
            break;
        case 3:
            m_unk284 = 1;
            m_unk280 = 0.2f - m_unk280;
            break;
    }
}

void CEntityPiglet::UpdateOSD(F32 dt) {
    switch (m_unk284) {
        case 0:
            m_unk280 = 0.0f;
            return;
        case 1:
            if (m_unk280 >= 0.2f) {
                m_unk284 = 2;
            }
            break;
        case 2:
            if ((U32)RwEngineInstance->stringFuncs.rwstrlen(m_unk2B4) != 0) {
                m_entity_manager->GetGame()->PlayNarratorLine(m_unk2B4);
                RwEngineInstance->stringFuncs.rwsprintf(m_unk2B4, "");
            }
            if (m_unk280 >= 2.2f) {
                m_unk284 = 3;
                F32 what = m_unk280 - 2.0f;
                m_unk280 = what - 0.2f;

                PlayOSDSound("SND_202_2D", 0);
            }
            break;
        case 3:
            if (m_unk280 >= 0.2f) {
                m_unk280 = 0.0f;
                m_unk284 = 0;
                return;
            }
            break;
    }

    m_unk280 += dt;
}

void CEntityPiglet::RenderOSD() {
    if ((U32)m_unk284 == 0) {
        return;
    }

    CGame* game = m_entity_manager->GetGame();
    CIcon::BeginRender(game->m_display_engine, game->GetCamera(), game->GetScene());

    CIcon icon1, icon2;
    F32 dVar9 = 0.0f;

    for (int i = 0; i < 3; i++) {
        icon1.m_width = 0.25f;
        icon1.m_height = 1.0f/3.0f;
        icon1.m_alpha = 0xC0;
        icon1.m_unk4 = m_unk280;

        switch (m_unk284) {
            case 2:
                dVar9 = icon1.m_unk4;
                // fallthrough
            case 1:
                icon1.m_height = InterpolValue(0.0f, icon1.m_height, icon1.m_unk4, 0.2f);
                icon1.m_alpha = InterpolValue(0.0f, icon1.m_alpha, icon1.m_unk4, 0.2f);
                break;
            case 3:
                dVar9 = 0.2f + (2.0f + icon1.m_unk4);
                icon1.m_height = InterpolValue(1.0f/3.0f, 0.0f, icon1.m_unk4, 0.2f);
                icon1.m_alpha = InterpolValue(icon1.m_alpha, 0.0f, icon1.m_unk4, 0.2f);
                break;
        }

        F32 offset;
        if (i == m_unk2A0) {
            offset = 0.01875f * (F32)sin(3.1415927f * dVar9 * 6.0f);
        } else {
            offset = 0.00625f * (F32)sin(3.1415927f * dVar9 * 3.0f);
        }
        icon1.m_width += offset;
        icon1.m_height += offset;

        switch (i) {
            case 0:
                icon1.m_x = 0.3f;
                icon1.m_y = 0.4f;
                break;
            case 1:
                icon1.m_x = 0.5f;
                icon1.m_y = 0.2f;
                break;
            case 2:
                icon1.m_x = 0.7f;
                icon1.m_y = 0.4f;
                break;
        }

        icon2.m_unk4 = icon1.m_unk4;
        icon2.m_texture = icon1.m_texture;
        icon2.m_x = icon1.m_x;
        icon2.m_y = icon1.m_y;
        icon2.m_width = icon1.m_width;
        icon2.m_height = icon1.m_height;
        icon2.m_red = icon1.m_red;
        icon2.m_green = icon1.m_green;
        icon2.m_blue = icon1.m_blue;
        icon2.m_alpha = icon1.m_alpha;
        icon2.m_u1 = icon1.m_u1;
        icon2.m_v1 = icon1.m_v1;
        icon2.m_u2 = icon1.m_u2;
        icon2.m_v2 = icon1.m_v2;
        icon2.m_width = icon1.m_width / 2;
        icon2.m_height = icon1.m_height / 2;
        icon2.m_alpha = 0xC0;

        int num;
        switch (i) {
            case 0:
                num = 0;
                break;
            case 1:
                num = 2;
                break;
            case 2:
                num = 1;
                break;
        }

        char buf[32];
        RwEngineInstance->stringFuncs.rwsprintf(buf, "ITEM_000_0%d", num + 1);
        icon2.m_texture = m_entity_manager->GetGame()->m_texture_dictionary->FindTexture(buf);
        icon2.Render(m_batches[i * 2 + 0], 1);

        char* item = m_entity_manager->GetGame()->GetCurrentMission()->GetItem(i);
        if (item != NULL) {
            if (i == m_unk2A0) {
                icon1.m_alpha = 0xFF;
            }
            icon1.m_texture = m_entity_manager->GetGame()->m_texture_dictionary->FindTexture(item);
            icon1.Render(m_batches[i * 2 + 1], 1);
        }
    }

    CIcon::EndRender();
}
