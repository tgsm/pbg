#include "CGamePartDMRoomLauncher.h"
#include "CGame.h"
#include <iostream>

extern "C" {
extern void Rt2dCTMSetIdentity(void);
}

CGamePartDMRoomLauncher::CGamePartDMRoomLauncher(CGame* game) {
    m_game = game;
    m_unk0 = 9;
    m_debug_menu = new CDebugMenu(m_game, "DMRoom/RoomLoader.xmd");

    m_debug_menu->GetControl("Mission")->SetRange(0, 7);
    if (m_game->m_unk4F54 == 0) {
        m_debug_menu->GetControl("Mission")->SetUnk1C(0);
    } else {
        m_debug_menu->GetControl("Mission")->SetUnk1CMinus1(m_game->m_unk4F54);
    }
    m_debug_menu->GetControl("Room")->SetUnk1CMinus1(m_game->m_unk4F58);
    m_debug_menu->GetControl("Player")->SetUnk1C(m_game->m_unk4F5C);

    m_game->GetCamera()->SetViewWindow(0.5f, 0.5f);

    Rt2dCTMSetIdentity();
}

CGamePartDMRoomLauncher::~CGamePartDMRoomLauncher() {
    delete m_debug_menu;
    m_debug_menu = NULL;

    m_game->GetResourceFactory()->UnloadResources(0);
}

U32 CGamePartDMRoomLauncher::NextFrame() {
    U32 ret;

    if (m_game->m_display_engine->Update()) {
        switch (m_debug_menu->UpdateAndDisplay()) {
            case 0: {
                CMenuControl* mission_control = m_debug_menu->GetControl("Mission");
                switch (reinterpret_cast<CControlValue*>(mission_control->m_control_values[mission_control->m_unk1C])->GetS32Value()) {
                    case 1:
                        m_debug_menu->GetControl("Room")->SetRange(0, 5);
                        break;
                    case 2:
                        m_debug_menu->GetControl("Room")->SetRange(0, 8);
                        break;
                    case 3:
                        m_debug_menu->GetControl("Room")->SetRange(0, 8);
                        break;
                    case 4:
                        m_debug_menu->GetControl("Room")->SetRange(0, 10);
                        break;
                    case 5:
                        m_debug_menu->GetControl("Room")->SetRange(0, 10);
                        break;
                    case 6:
                        m_debug_menu->GetControl("Room")->SetRange(0, 12);
                        break;
                    case 7:
                        m_debug_menu->GetControl("Room")->SetRange(0, 2);
                        break;
                    case 8:
                        m_debug_menu->GetControl("Room")->SetRange(0, 2);
                        break;
                }
                ret = m_unk0;
                break;
            }
            case 1: {
                CMenuControl* mission_control = m_debug_menu->GetControl("Mission");
                int value = reinterpret_cast<CControlValue*>(mission_control->m_control_values[mission_control->m_unk1C])->GetS32Value();

                m_game->ResetOpcodeBuffer();
                m_game->PushOpcodeValue(2);
                m_game->PushOpcodeValue(value);

                CMenuControl* room_control = m_debug_menu->GetControl("Room");
                m_game->m_unk4F58 = reinterpret_cast<CControlValue*>(room_control->m_control_values[room_control->m_unk1C])->GetS32Value();
                CMenuControl* player_control = m_debug_menu->GetControl("Player");
                m_game->m_unk4F5C = reinterpret_cast<CControlValue*>(player_control->m_control_values[player_control->m_unk1C])->GetS32Value();

                m_game->PushOpcodeValue(1);

                room_control = m_debug_menu->GetControl("Room");
                m_game->PushOpcodeValue(reinterpret_cast<CControlValue*>(room_control->m_control_values[room_control->m_unk1C])->GetS32Value());
                player_control = m_debug_menu->GetControl("Player");
                m_game->PushOpcodeValue(reinterpret_cast<CControlValue*>(player_control->m_control_values[player_control->m_unk1C])->GetS32Value());

                m_game->PushOpcodeValue(0);
                m_game->PushOpcodeValue(0);
                m_game->PushOpcodeValue(0);
                m_game->PushOpcodeValue(0);
                m_game->PushOpcodeValue(0);
                m_game->PushOpcodeValue(0);
                m_game->PushOpcodeValue(1);

                if (m_game->GetMission(value - 1).GetUnk34() == 0) {
                    room_control = m_debug_menu->GetControl("Room");
                    if (m_game->GetMission(value - 1).GetUnkC() == reinterpret_cast<CControlValue*>(room_control->m_control_values[room_control->m_unk1C])->GetS32Value()) {
                        m_game->PushOpcodeValue(6);
                    }
                }

                mission_control = m_debug_menu->GetControl("Mission");
                m_debug_menu->RemoveControl("Room");
                m_debug_menu->RemoveControl("Player");
                mission_control->DestroyValueList();

                CControlValue* new_control = new CControlValue;
                new_control->SetS32Value(1);
                new_control->SetTexture("loading");
                mission_control->m_control_values.push_back(reinterpret_cast<size_t>(new_control));
                mission_control->m_unkC++;

                m_debug_menu->m_unk24 = 0;
                m_game->m_display_engine->Update();

                m_debug_menu->UpdateAndDisplay();

                ret = 7;
                break;
            }
            case 2:
                ret = m_unk0;
                break;
            default:
                ret = m_unk0;
                break;
        }
    } else {
        ret = 10;
    }

    return ret;
}
