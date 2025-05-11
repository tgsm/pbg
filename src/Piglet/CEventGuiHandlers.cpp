#include "engine/input/IInputEngine.h"
#include "engine/sound/CSound2D.h"
#include "CEventGuiHandlers.h"
#include "CGame.h"
#include "CGamePart.h"
#include "CGamePartIngame.h"
#include <iostream>
#include <vector>

static f32 gs_TimeBeforeMemCardCheck;

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
        UnkGamePartAndReturnTypeInline();

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

            UnkGamePartAndReturnTypeInline();
        } else {
            m_game->ResetOpcodeBuffer();
            m_game->PushOpcodeValue(5);

            UnkGamePartAndReturnTypeInline();
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

CGuiLoadNoDataEventHandler::CGuiLoadNoDataEventHandler() : CGuiBaseEventHandler("GuiLoadNoDataEventHandler") {

}

CGuiLoadNoMemCardEventHandler::CGuiLoadNoMemCardEventHandler() : CGuiBaseEventHandler("GuiLoadNoMemCardEventHandler") {

}

CGuiLoadNotFormattedMemCardEventHandler::CGuiLoadNotFormattedMemCardEventHandler() : CGuiBaseEventHandler("GuiLoadNotFormattedMemCardEventHandler") {

}

// Equivalent: std::vector stuff
CGuiChooseGameEventHandler::CGuiChooseGameEventHandler() : CGuiBaseEventHandler("GuiChooseGameEventHandler") {
    m_unk10.reserve(8);
}

CGuiLoadingMemoryCardEventHandler::CGuiLoadingMemoryCardEventHandler() : CGuiBaseEventHandler("GuiLoadingMemoryCardEventHandler") {

}

CGuiLoadLoadSuccessEventHandler::CGuiLoadLoadSuccessEventHandler() : CGuiBaseEventHandler("GuiLoadLoadSuccessEventHandler") {

}

CGuiLoadLoadFailEventHandler::CGuiLoadLoadFailEventHandler() : CGuiBaseEventHandler("GuiLoadLoadFailEventHandler") {

}

CGuiLoadMemoryCardUnuseableEventHandler::CGuiLoadMemoryCardUnuseableEventHandler() : CGuiBaseEventHandler("GuiLoadMemoryCardUnuseableEventHandler") {

}

CGuiCreateNewGameEventHandler::CGuiCreateNewGameEventHandler() : CGuiBaseEventHandler("GuiCreateNewGameEventHandler") {

}

CGuiEnterNameEventHandler::CGuiEnterNameEventHandler() : CGuiBaseEventHandler("GuiEnterNameEventHandler") {

}

CGuiSaveCheckingMemorycardEventHandler::CGuiSaveCheckingMemorycardEventHandler() : CGuiBaseEventHandler("GuiSaveCheckingMemorycardEventHandler") {
    m_unk10 = 0;
}

CGuiSaveNoSpaceEventHandler::CGuiSaveNoSpaceEventHandler() : CGuiBaseEventHandler("GuiSaveNoSpaceEventHandler") {

}

CGuiSavingEventHandler::CGuiSavingEventHandler() : CGuiBaseEventHandler("GuiSavingEventHandler") {

}

CGuiSaveFailedEventHandler::CGuiSaveFailedEventHandler() : CGuiBaseEventHandler("GuiSaveFailedEventHandler") {

}

CGuiSaveReadyEventHandler::CGuiSaveReadyEventHandler() : CGuiBaseEventHandler("GuiSaveReadyEventHandler") {

}

CGuiLoadFileCorruptEventHandler::CGuiLoadFileCorruptEventHandler() : CGuiBaseEventHandler("GuiLoadFileCorruptEventHandler") {

}

// Equivalent
void CGuiLoadFileCorruptEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    CGuiBaseEventHandler::OnEvent(menu, event, unk);

    if (event == DKGUI::EVENT_2) {
        gs_TimeBeforeMemCardCheck = 1.0f;
    } else if (event == DKGUI::EVENT_0) {
        if (gs_TimeBeforeMemCardCheck < 0.0f) {
            m_game->m_timer->Pause();
            u32 backup_state = m_game->m_backup_engine->GetState();
            m_game->m_timer->Resume();

            if (!(backup_state & (1 << 0))) {
                m_game->m_gui_manager->GetGuiPtr("LOAD_FILE_CORRUPT")->menu->Reset();
                m_game->m_gui_manager->SetActive("LOAD_FILE_CORRUPT", 0);
                m_game->m_gui_manager->SetVisible("LOAD_FILE_CORRUPT", 0);

                m_game->m_gui_manager->GetGuiPtr("LOAD_CHECK_MMC")->menu->Reset();
                m_game->m_gui_manager->SetActive("LOAD_CHECK_MMC", 1);
                m_game->m_gui_manager->SetVisible("LOAD_CHECK_MMC", 1);
            } else {
                gs_TimeBeforeMemCardCheck = 1.0f;
            }
        } else {
            gs_TimeBeforeMemCardCheck -= m_game->GetDeltaTime();
        }
    } else if (event == DKGUI::EVENT_3) {
        std::string str = (char*)unk;
        if (str == "yes") {
            m_game->m_gui_manager->GetGuiPtr("LOAD_FILE_CORRUPT")->menu->Reset();
            m_game->m_gui_manager->SetActive("LOAD_FILE_CORRUPT", 0);
            m_game->m_gui_manager->SetVisible("LOAD_FILE_CORRUPT", 0);

            m_game->m_gui_manager->GetGuiPtr("LOAD_LOAD_DATA")->menu->Reset();
            m_game->m_gui_manager->SetActive("LOAD_LOAD_DATA", 1);
            m_game->m_gui_manager->SetVisible("LOAD_LOAD_DATA", 1);
        } else if (str == "no") {
            m_game->m_gui_manager->GetGuiPtr("LOAD_FILE_CORRUPT")->menu->Reset();
            m_game->m_gui_manager->SetActive("LOAD_FILE_CORRUPT", 0);
            m_game->m_gui_manager->SetVisible("LOAD_FILE_CORRUPT", 0);
        } else if (str == "delete") {
            m_game->m_gui_manager->GetGuiPtr("LOAD_FILE_CORRUPT")->menu->Reset();
            m_game->m_gui_manager->SetActive("LOAD_FILE_CORRUPT", 0);
            m_game->m_gui_manager->SetVisible("LOAD_FILE_CORRUPT", 0);

            CGuiLoadingMemoryCardEventHandler* handler = (CGuiLoadingMemoryCardEventHandler*)m_game->m_gui_manager->IsEventCallbackRegistered("GuiLoadingMemoryCardEventHandler");
            if (handler != NULL) {
                // Weird negate-or-rightshift going on here. Should just move.
                if (handler->GetFilename() != "") {
                    if (m_game->m_backup_engine->Delete("GPLE9G", handler->GetFilename())) {
                        m_game->m_timer->Pause();

                        if (m_game->m_backup_engine->GetState() == 8) {
                            m_game->m_timer->Resume();

                            m_game->m_gui_manager->GetGuiPtr("LOAD_MMC_UNUSEABLE")->menu->Reset();
                            m_game->m_gui_manager->SetActive("LOAD_MMC_UNUSEABLE", 1);
                            m_game->m_gui_manager->SetVisible("LOAD_MMC_UNUSEABLE", 1);
                        } else {
                            m_game->m_timer->Resume();

                            m_game->m_gui_manager->GetGuiPtr("LOAD_FILE_DELETEFAIL")->menu->Reset();
                            m_game->m_gui_manager->SetActive("LOAD_FILE_DELETEFAIL", 1);
                            m_game->m_gui_manager->SetVisible("LOAD_FILE_DELETEFAIL", 1);
                        }
                    } else {
                        m_game->m_gui_manager->GetGuiPtr("LOAD_CHECK_MMC")->menu->Reset();
                        m_game->m_gui_manager->SetActive("LOAD_CHECK_MMC", 1);
                        m_game->m_gui_manager->SetVisible("LOAD_CHECK_MMC", 1);
                    }
                }
            }
        }
    }
}

CGuiLoadFileDeletingEventHandler::CGuiLoadFileDeletingEventHandler() : CGuiBaseEventHandler("GuiLoadFileDeletingEventHandler") {

}

CGuiLoadFileDeletingSuccessEventHandler::CGuiLoadFileDeletingSuccessEventHandler() : CGuiBaseEventHandler("GuiLoadFileDeletingSuccessEventHandler") {

}

CGuiLoadFileDeletingFailedEventHandler::CGuiLoadFileDeletingFailedEventHandler() : CGuiBaseEventHandler("GuiLoadFileDeletingFailedEventHandler") {

}

CGuiLoadWrongDeviceEventHandler::CGuiLoadWrongDeviceEventHandler() : CGuiBaseEventHandler("GuiLoadWrongDeviceEventHandler") {

}

CGuiSaveWrongDeviceEventHandler::CGuiSaveWrongDeviceEventHandler() : CGuiBaseEventHandler("GuiSaveWrongDeviceEventHandler") {

}

CGuiLoadCorruptMemcardEventHandler::CGuiLoadCorruptMemcardEventHandler() : CGuiBaseEventHandler("GuiLoadCorruptMemcardEventHandler") {

}

CGuiSaveCorruptMemcardEventHandler::CGuiSaveCorruptMemcardEventHandler() : CGuiBaseEventHandler("GuiSaveCorruptMemcardEventHandler") {

}

CGuiSaveMemoryCardUnuseableEventHandler::CGuiSaveMemoryCardUnuseableEventHandler() : CGuiBaseEventHandler("GuiSaveMemoryCardUnuseableEventHandler") {

}

CGuiSaveOkEventHandler::CGuiSaveOkEventHandler() : CGuiBaseEventHandler("GuiSaveOkEventHandler") {

}

// Equivalent: regalloc
void CGuiSaveOkEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    CGuiBaseEventHandler::OnEvent(menu, event, unk);

    if (event == DKGUI::EVENT_2) {
        m_unkC = FALSE;
        return;
    }

    if (m_unkC == TRUE) {
        // BUG: Redundant double-check
        if (m_unkC == TRUE) {
            m_game->m_gui_manager->GetGuiPtr("SAVE_SAVE_OK")->menu->Reset();
            m_game->m_gui_manager->SetActive("SAVE_SAVE_OK", 0);
            m_game->m_gui_manager->SetVisible("SAVE_SAVE_OK", 0);

            if (m_game->m_unk4F54 == 8 && m_game->m_unk4F58 == 1) {
                CDKW_RGBA fade_color = m_game->ComputeGameFadeColor();
                m_game->FadeInit(1.0f, CGame::FADE_TYPE_4, fade_color.m_r, fade_color.m_g, fade_color.m_b, 0.0f);
                ((CGamePartIngame*)m_game->GetGamePartPointer())->m_game_room_manager->m_unk0 |= (1 << 5);
                m_game->FadeIn(-1.0f);

                m_game->ResetOpcodeBuffer();
                m_game->PushOpcodeValue(1);
                m_game->PushOpcodeValue(2);
                m_game->PushOpcodeValue(0);
                m_game->PushOpcodeValue(0);
                m_game->PushOpcodeValue(0);
                m_game->PushOpcodeValue(25);
                m_game->PushOpcodeValue(0);
                m_game->PushOpcodeValue(0);
                m_game->PushOpcodeValue(0);
                m_game->PushOpcodeValue(1);

                m_game->GetMailbox()->SendMessage("Piglet", "RTC_802_01", "START", 0);

                UnkGamePartAndReturnTypeInline();
            }
        }
    } else {
        if (event == DKGUI::EVENT_3) {
            std::string str = (char*)unk;
            if (str == "exit") {
                m_unkC = TRUE;
            }
        }
    }
}

CGuiSaveNoMemCardEventHandler::CGuiSaveNoMemCardEventHandler() : CGuiBaseEventHandler("GuiSaveNoMemCardEventHandler") {

}

CGuiSaveFormatEventHandler::CGuiSaveFormatEventHandler() : CGuiBaseEventHandler("GuiSaveFormatEventHandler") {

}

CGuiSaveFormatSureEventHandler::CGuiSaveFormatSureEventHandler() : CGuiBaseEventHandler("GuiSaveFormatSureEventHandler") {

}

CGuiSaveFormattingEventHandler::CGuiSaveFormattingEventHandler() : CGuiBaseEventHandler("GuiSaveFormattingEventHandler") {

}

CGuiFormatOkEventHandler::CGuiFormatOkEventHandler() : CGuiBaseEventHandler("GuiFormatOkEventHandler") {

}

CGuiFormatFailedEventHandler::CGuiFormatFailedEventHandler() : CGuiBaseEventHandler("GuiFormatFailedEventHandler") {

}

CGuiOverwriteEventHandler::CGuiOverwriteEventHandler() : CGuiBaseEventHandler("GuiOverwriteEventHandler") {

}

CGuiOverwriteSureEventHandler::CGuiOverwriteSureEventHandler() : CGuiBaseEventHandler("GuiOverwriteSureEventHandler") {

}

CGuiDreamSelectEventHandler::CGuiDreamSelectEventHandler() : CGuiBaseEventHandler("GuiDreamSelectEventHandler") {
    m_unk10 = 0;
    m_unk18 = 0;
}

CGuiScreenSettingEventHandler::CGuiScreenSettingEventHandler() : CGuiBaseEventHandler("GuiScreenSettingEventHandler") {
    m_unkC = FALSE;
}

CGuiNoPadEventHandler::CGuiNoPadEventHandler() : CGuiBaseEventHandler("GuiNoPadEventHandler") {

}

void CGuiNoPadEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    CGuiBaseEventHandler::OnEvent(menu, event, unk);

    if (event != DKGUI::EVENT_0) {
        return;
    }

    if (DKI::IInputEngine::GetDevice(0)->IsConnected() == TRUE) {
        m_game->m_gui_manager->GetGuiPtr("NOPAD_NOPAD")->menu->Reset();
        m_game->m_gui_manager->SetActive("NOPAD_NOPAD", 0);
        m_game->m_gui_manager->SetVisible("NOPAD_NOPAD", 0);
    }
}

CGuiStartScreenEventHandler::CGuiStartScreenEventHandler() : CGuiBaseEventHandler("GuiStartScreenEventHandler") {

}

void CGuiStartScreenEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    CGuiBaseEventHandler::OnEvent(menu, event, unk);

    if (event != DKGUI::EVENT_3) {
        return;
    }

    std::string str = (char*)unk;
    if (str == "exit") {
        m_game->m_gui_manager->SetActive("TITLE_TITLE", 0);
        m_game->FadeInit(1.0f, CGame::FADE_TYPE_3, 0, 0, 0, 0.0f);
        m_game->m_unk5038 = 0;

        m_game->ResetOpcodeBuffer();
        m_game->PushOpcodeValue(5);

        UnkGamePartAndReturnTypeInline();
    }
}
