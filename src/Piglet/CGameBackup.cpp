#include "engine/backup/IBAKEngine.h"
#include "CGameBackup.h"
#include "CGame.h"
#include <cstring>
#include <iostream>

void CSlotContainer::StoreS8(S8 value, S8* dest) {
    *dest = value;
}

void CSlotContainer::StoreS32(S32 value, S8* dest) {
    *((S32*)dest) = value;
}

S8 CSlotContainer::GetS8(S8* src) {
    return *src;
}

S32 CSlotContainer::GetS32(S8* src) {
    return *((S32*)src);
}

CGameBackup::CGameBackup(CGame* game) : m_game(game), m_unk4(NULL), m_current_container(NULL) {
    m_current_container = new CSlotContainer();
}

CGameBackup::~CGameBackup() {
    if (m_unk4 != NULL) {
        delete m_unk4;
        m_unk4 = NULL;
    }
    if (m_current_container != NULL) {
        delete m_current_container;
        m_current_container = NULL;
    }
}

// Equivalent
void CGameBackup::CreateNewGame() {
    CSlotContainer* container = m_current_container;
    int i = 0;
    container->m_unk4->unk0 = 0;
    container->m_unk0 = 0;
    for (; i < (U32)NUM_MISSIONS; i++) {
        CMission& mission = m_game->GetMission(i);
        CMission& other = m_game->GetBackupMission(i);

        mission.m_game = other.m_game;
        mission.m_mission_name.assign(other.m_mission_name, 0);
        mission.m_mission_no = other.m_mission_no;
        mission.m_unkC = other.m_unkC;
        mission.m_unk10 = other.m_unk10;
        mission.m_start_position = other.m_start_position;
        mission.m_start_rotation = other.m_start_rotation;
        mission.m_unk2C = other.m_unk2C;
        mission.m_unk30 = other.m_unk30;
        mission.m_unk34 = other.m_unk34;
        mission.m_num_rooms = other.m_num_rooms;
        mission.m_rooms = other.m_rooms;
        mission.m_unk7C = other.m_unk7C;
        mission.m_start_mission_fmv.assign(other.m_start_mission_fmv, 0);
        mission.m_end_mission_fmv.assign(other.m_end_mission_fmv, 0);
        mission.m_mission_fight_warp_model.assign(other.m_mission_fight_warp_model, 0);
        mission.m_start_rtc.assign(other.m_start_rtc, 0);
        mission.m_unk48C = other.m_unk48C;
        mission.m_num_total_cookies = other.m_num_total_cookies;
        mission.m_num_current_cookies = other.m_num_current_cookies;
        mission.m_unk4C4 = other.m_unk4C4;
    }

    m_game->GetMailbox()->ResetMessageList();

    DKBAK::DKBAK_DATE date;
    date.unk3 = 0;
    date.unk2 = 0;
    date.unk1 = 0;
    date.unk4 = 0;
    date.unk0 = 0;
    date.unk6 = 0;
    m_game->SetBackupDate(date);
}

void CGameBackup::Backup() {

}

// Equivalent
void CGameBackup::ApplyToGameData() {
    CSlotContainer* container = GetCurrentContainer();
    for (int i = 0; container->m_unk0 = container->m_unk4->unk0, container->m_unk0 > i; i++) {
        SSubslot* data;
        if (container->m_unk0 > 0) {
            data = container->m_unk4->sslot;
            for (int j = 0; j < i; j++) {
                data = (SSubslot*)((int)data + data->offset);
            }
        } else {
            data = NULL;
        }

        if (data != NULL) {
            if (strcmp("SLOT_VOLUME", data->name) == 0) {
                m_game->GetSoundEngine()->SetGlobalVolume(data->unk18 / 16384.0f);
                m_game->m_unk504C = m_game->GetSoundEngine()->GetGlobalVolume();
            } else if (strcmp("SLOT_VIBRATION", data->name) == 0) {
                if (data->unk18 != 0) {
                    DKI::IInputEngine::GetDevice(0)->VibrationEnable();
                } else {
                    DKI::IInputEngine::GetDevice(0)->StopVibration();
                    DKI::IInputEngine::GetDevice(0)->VibrationDisable();
                }
            } else if (strcmp("SLOT_STEREO", data->name) == 0) {
                if (OSGetSoundMode() == OS_SOUND_MODE_STEREO) {
                    m_game->GetSoundEngine()->SetStereoMode(OS_SOUND_MODE_STEREO);
                } else {
                    m_game->GetSoundEngine()->SetStereoMode(OS_SOUND_MODE_MONO);
                }
            } else if (strcmp("SLOT_SCROFFSET", data->name) == 0) {
                CDKW_V2d offset;
                offset.x = data->unk18;
                offset.y = data->unk1C;
                m_game->SetScreenOffset(offset);
            } else if (strcmp("SLOT_MAILBOX", data->name) == 0) {
                m_game->GetMailbox()->ResetMessageList();
                m_game->GetMailbox()->RestoreMessageList(&data->unk14);
            } else if (strcmp("SLOT_MISSION_1", data->name) == 0 ||
                       strcmp("SLOT_MISSION_2", data->name) == 0 ||
                       strcmp("SLOT_MISSION_3", data->name) == 0 ||
                       strcmp("SLOT_MISSION_4", data->name) == 0 ||
                       strcmp("SLOT_MISSION_5", data->name) == 0 ||
                       strcmp("SLOT_MISSION_6", data->name) == 0) {
                if (strcmp("SLOT_MISSION_1", data->name) == 0) {
                    m_game->GetMission(MISSION_WINNIE - 1).Restore(&data->unk14);
                } else if (strcmp("SLOT_MISSION_2", data->name) == 0) {
                    m_game->GetMission(MISSION_ROO - 1).Restore(&data->unk14);
                } else if (strcmp("SLOT_MISSION_3", data->name) == 0) {
                    m_game->GetMission(MISSION_OWL - 1).Restore(&data->unk14);
                } else if (strcmp("SLOT_MISSION_4", data->name) == 0) {
                    m_game->GetMission(MISSION_EEYORE - 1).Restore(&data->unk14);
                } else if (strcmp("SLOT_MISSION_5", data->name) == 0) {
                    m_game->GetMission(MISSION_RABBIT - 1).Restore(&data->unk14);
                } else if (strcmp("SLOT_MISSION_6", data->name) == 0) {
                    m_game->GetMission(MISSION_TIGGER - 1).Restore(&data->unk14);
                }
            } else {
                CEntity* entity = m_game->GetEntityManager()->GetEntity(data->name);
                if (entity != NULL && m_game->GetCurrentMissionId() < MISSION_FINAL) {
                    entity->Restore(&data->unk14);
                }
                if (entity != NULL && m_game->GetCurrentMissionId() >= MISSION_FINAL) {
                    if (entity->GetName() == "Piglet") {
                        entity->Restore(&data->unk14);
                    }
                }
            }
        }
    }

    std::string str_;
    str_ = "MENU_SCRIPT";
    SDkMessage message;
    while (m_game->GetMailbox()->GetMessage(&message, str_, TRUE)) {
        message.param = 0;
    }

    if (m_game->GetCurrentMissionId() == MISSION_MENUS && m_game->GetCurrentRoomId() == 2) {
        if (!DKI::IInputEngine::GetDevice(0)->IsVibrationModeEnable()) {
            m_game->GetMailbox()->SendMessage("Piglet", "MENU_SCRIPT", "VIBOFF", 0);
        } else {
            m_game->GetMailbox()->SendMessage("Piglet", "MENU_SCRIPT", "VIBON", 0);
        }

        if (m_game->GetSoundEngine()->GetStereoMode() == OS_SOUND_MODE_MONO) {
            m_game->GetMailbox()->SendMessage("Piglet", "MENU_SCRIPT", "MONO", 0);
        } else {
            m_game->GetMailbox()->SendMessage("Piglet", "MENU_SCRIPT", "STEREO", 0);
        }

        if (m_game->GetSoundEngine()->GetGlobalVolume() > 0.0f) {
            if (m_game->GetSoundEngine()->GetGlobalVolume() > 0.25f) {
                if (m_game->GetSoundEngine()->GetGlobalVolume() > 0.5f) {
                    if (m_game->GetSoundEngine()->GetGlobalVolume() > 0.75f) {
                        m_game->GetMailbox()->SendMessage("Piglet", "MENU_SCRIPT", "VOLA", 0);
                    } else {
                        m_game->GetMailbox()->SendMessage("Piglet", "MENU_SCRIPT", "VOLB", 0);
                    }
                } else {
                    m_game->GetMailbox()->SendMessage("Piglet", "MENU_SCRIPT", "VOLC", 0);
                }
            } else {
                m_game->GetMailbox()->SendMessage("Piglet", "MENU_SCRIPT", "VOLD", 0);
            }
        } else {
            m_game->GetMailbox()->SendMessage("Piglet", "MENU_SCRIPT", "VOLE", 0);
        }
    }
}

CSlotContainer* CGameBackup::GetCurrentContainer() {
    return m_current_container;
}
