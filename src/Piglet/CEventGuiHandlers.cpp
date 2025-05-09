#include "engine/sound/CSound2D.h"
#include "CEventGuiHandlers.h"
#include "CGame.h"
#include "CGamePart.h"
#include "CGamePartIngame.h"
#include <iostream>

CGuiBaseEventHandler::CGuiBaseEventHandler(const std::string& unk) : m_unk4(unk) {

}

CGuiBaseEventHandler::~CGuiBaseEventHandler() {

}

void CGuiBaseEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    if (event != DKGUI::EVENT_3) {
        return;
    }

    std::string str = (char*)unk;
    if (str.find("playsound", 0, strlen("playsound")) >= 0) {
        std::string name_maybe;
        name_maybe = (char*)unk + strlen("playsound ");
        DKSND::CSound2D* sound = m_game->m_sound_engine->PlaySound2D(name_maybe, 1);
        if (sound != NULL) {
            sound->SetVolume(1.0f);
            sound->SetLayer(2);
        }
    }
}

CGuiNullEventHandler::CGuiNullEventHandler() : CGuiBaseEventHandler("GuiNullEventHandler") {

}

void CGuiNullEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    CGuiBaseEventHandler::OnEvent(menu, event, unk);
}

CGuiPauseEventHandler::CGuiPauseEventHandler() : CGuiBaseEventHandler("GuiPauseEventHandler") {

}

void CGuiPauseEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    CGuiBaseEventHandler::OnEvent(menu, event, unk);

    char* str_ptr = (char*)unk;
    if (event != DKGUI::EVENT_3) {
        return;
    }

    std::string str = str_ptr;
    if (str == "yes") {
        // FIXME: This pattern pops up a few times, is this an inlined function?
        CGamePart* game_part = m_game->GetGamePartPointer();
        game_part->m_unk4 = 0;
        m_game->SetCurrentRoomReturnType(CGame::RETURN_TYPE_0, -1);

        CDKW_RGBA fade_color = CDKW_RGBA(199, 226, 222, 0xFF);
        m_game->FadeInit(1.0f, CGame::FADE_TYPE_4, fade_color.m_r, fade_color.m_g, fade_color.m_b, 0.0f);

        m_game->m_screen_effect->SetSequenceByIndex(0);

        ((CGamePartIngame*)m_game->GetGamePartPointer())->m_game_room_manager->m_unk0 |= (1 << 5);

        m_game->FadeIn(-1.0f);

        if (m_game->m_unk4F54 == 8) {
            m_game->ResetOpcodeBuffer();
            if (m_game->m_unk4F58 == 2) {
                m_game->PushOpcodeValue(1);
                m_game->PushOpcodeValue(1);
                m_game->PushOpcodeValue(0);
                m_game->PushOpcodeValue(0);
                m_game->PushOpcodeValue(0);
                m_game->PushOpcodeValue(25);
                m_game->PushOpcodeValue(0);
                m_game->PushOpcodeValue(0);
                m_game->PushOpcodeValue(0);
                m_game->PushOpcodeValue(1);
            } else if (m_game->m_unk4F58 == 3) {
                m_game->PushOpcodeValue(1);
                m_game->PushOpcodeValue(1);
                m_game->PushOpcodeValue(0);
                m_game->PushOpcodeValue(0);
                m_game->PushOpcodeValue(0);
                m_game->PushOpcodeValue(25);
                m_game->PushOpcodeValue(0);
                m_game->PushOpcodeValue(0);
                m_game->PushOpcodeValue(0);
                m_game->PushOpcodeValue(1);
            } else if (m_game->m_unk4F58 == 1) {
                m_game->PushOpcodeValue(11);
            }

            game_part = m_game->GetGamePartPointer();
            game_part->m_unk4 = 0;
            m_game->SetCurrentRoomReturnType(CGame::RETURN_TYPE_0, -1);
        } else {
            m_game->ResetOpcodeBuffer();
            m_game->PushOpcodeValue(5);

            game_part = m_game->GetGamePartPointer();
            game_part->m_unk4 = 0;
            m_game->SetCurrentRoomReturnType(CGame::RETURN_TYPE_0, -1);
        }
    } else if (str != "no" && str != "resume") {
        return;
    }

    if (m_game->m_unk4F54 == 8) {
        m_game->m_gui_manager->GetGuiPtr("PAUSE_PAUSE_GAME")->menu->Reset();
        m_game->m_gui_manager->SetActive("PAUSE_PAUSE_GAME", 0);
        m_game->m_gui_manager->SetVisible("PAUSE_PAUSE_GAME", 0);
    } else {
        m_game->m_gui_manager->GetGuiPtr("PAUSE_PAUSE_GAME")->menu->Reset();
        m_game->m_gui_manager->SetActive("PAUSE_PAUSE_DREAM", 0);
        m_game->m_gui_manager->SetVisible("PAUSE_PAUSE_DREAM", 0);
    }
}

CGuiLoadCheckingMemoryCardEventHandler::CGuiLoadCheckingMemoryCardEventHandler() : CGuiBaseEventHandler("GuiLoadCheckingMemoryCardEventHandler") {

}
