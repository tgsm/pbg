#include "engine/input/IInputEngine.h"
#include "engine/sound/CSound2D.h"
#include "CEventGuiHandlers.h"
#include "CGame.h"
#include "CGamePart.h"
#include "CGamePartIngame.h"
#include <iostream>

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
        m_game->m_gui_manager->GetGuiPtr("PAUSE_PAUSE_DREAM")->menu->Reset();
        m_game->m_gui_manager->SetActive("PAUSE_PAUSE_DREAM", 0);
        m_game->m_gui_manager->SetVisible("PAUSE_PAUSE_DREAM", 0);
    }
}

CGuiLoadCheckingMemoryCardEventHandler::CGuiLoadCheckingMemoryCardEventHandler() : CGuiBaseEventHandler("GuiLoadCheckingMemoryCardEventHandler") {

}

CGuiLoadNoDataEventHandler::CGuiLoadNoDataEventHandler() : CGuiBaseEventHandler("GuiLoadNoDataEventHandler") {

}

// Equivalent: regalloc
void CGuiLoadNoDataEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    CGuiBaseEventHandler::OnEvent(menu, event, unk);

    if (event == DKGUI::EVENT_2) {
        m_unkC = FALSE;
        return;
    }

    if (m_unkC == TRUE) {
        if (m_unkC == TRUE) {
            m_game->m_gui_manager->GetGuiPtr("LOAD_NO_DATA")->menu->Reset();
            m_game->m_gui_manager->SetActive("LOAD_NO_DATA", 0);
            m_game->m_gui_manager->SetVisible("LOAD_NO_DATA", 0);
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

CGuiLoadNoMemCardEventHandler::CGuiLoadNoMemCardEventHandler() : CGuiBaseEventHandler("GuiLoadNoMemCardEventHandler") {

}

void CGuiLoadNoMemCardEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    CGuiBaseEventHandler::OnEvent(menu, event, unk);

    if (event == DKGUI::EVENT_2) {
        m_unkC = FALSE;
        return;
    }

    if (event != DKGUI::EVENT_3) {
        return;
    }

    std::string str = (char*)unk;
    if (str == "no") {
        m_game->m_gui_manager->GetGuiPtr("LOAD_NO_MMC")->menu->Reset();
        m_game->m_gui_manager->SetActive("LOAD_NO_MMC", 0);
        m_game->m_gui_manager->SetVisible("LOAD_NO_MMC", 0);
    } else if (str == "yes") {
        m_game->m_gui_manager->GetGuiPtr("LOAD_NO_MMC")->menu->Reset();
        m_game->m_gui_manager->SetActive("LOAD_NO_MMC", 0);
        m_game->m_gui_manager->SetVisible("LOAD_NO_MMC", 0);

        m_game->m_gui_manager->GetGuiPtr("LOAD_CHECK_MMC")->menu->Reset();
        m_game->m_gui_manager->SetActive("LOAD_CHECK_MMC", 1);
        m_game->m_gui_manager->SetVisible("LOAD_CHECK_MMC", 1);
    }
}

CGuiLoadNotFormattedMemCardEventHandler::CGuiLoadNotFormattedMemCardEventHandler() : CGuiBaseEventHandler("GuiLoadNotFormattedMemCardEventHandler") {

}

void CGuiLoadNotFormattedMemCardEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    CGuiBaseEventHandler::OnEvent(menu, event, unk);

    if (event == DKGUI::EVENT_2) {
        m_unkC = FALSE;
        return;
    }

    if (event != DKGUI::EVENT_3) {
        return;
    }

    std::string str = (char*)unk;
    if (str == "no") {
        m_game->m_gui_manager->GetGuiPtr("LOAD_MMC_UNFORMAT")->menu->Reset();
        m_game->m_gui_manager->SetActive("LOAD_MMC_UNFORMAT", 0);
        m_game->m_gui_manager->SetVisible("LOAD_MMC_UNFORMAT", 0);
    } else if (str == "yes") {
        m_game->m_gui_manager->GetGuiPtr("LOAD_MMC_UNFORMAT")->menu->Reset();
        m_game->m_gui_manager->SetActive("LOAD_MMC_UNFORMAT", 0);
        m_game->m_gui_manager->SetVisible("LOAD_MMC_UNFORMAT", 0);

        m_game->m_gui_manager->GetGuiPtr("LOAD_CHECK_MMC")->menu->Reset();
        m_game->m_gui_manager->SetActive("LOAD_CHECK_MMC", 1);
        m_game->m_gui_manager->SetVisible("LOAD_CHECK_MMC", 1);
    }
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

void CGuiSaveFailedEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    CGuiBaseEventHandler::OnEvent(menu, event, unk);

    if (event == DKGUI::EVENT_2) {
        gs_TimeBeforeMemCardCheck = 1.0f;
    } else if (event == DKGUI::EVENT_0) {
        if (gs_TimeBeforeMemCardCheck < 0.0f) {
            m_game->m_timer->Pause();
            u32 backup_state = m_game->m_backup_engine->GetState();
            m_game->m_timer->Resume();

            if (!(backup_state & (1 << 0))) {
                m_game->m_gui_manager->GetGuiPtr("SAVE_SAVE_FAILED")->menu->Reset();
                m_game->m_gui_manager->SetActive("SAVE_SAVE_FAILED", 0);
                m_game->m_gui_manager->SetVisible("SAVE_SAVE_FAILED", 0);

                m_game->m_gui_manager->GetGuiPtr("SAVE_CHECK_MMC")->menu->Reset();
                m_game->m_gui_manager->SetActive("SAVE_CHECK_MMC", 1);
                m_game->m_gui_manager->SetVisible("SAVE_CHECK_MMC", 1);
            } else {
                gs_TimeBeforeMemCardCheck = 1.0f;
            }
        } else {
            gs_TimeBeforeMemCardCheck -= m_game->GetDeltaTime();
        }
    } else if (event == DKGUI::EVENT_3) {
        std::string str = (char*)unk;
        if (str == "yes") {
            m_game->m_gui_manager->GetGuiPtr("SAVE_SAVE_FAILED")->menu->Reset();
            m_game->m_gui_manager->SetActive("SAVE_SAVE_FAILED", 0);
            m_game->m_gui_manager->SetVisible("SAVE_SAVE_FAILED", 0);

            m_game->m_gui_manager->GetGuiPtr("SAVE_CHECK_MMC")->menu->Reset();
            m_game->m_gui_manager->SetActive("SAVE_CHECK_MMC", 1);
            m_game->m_gui_manager->SetVisible("SAVE_CHECK_MMC", 1);
        } else if (str == "no") {
            m_game->m_gui_manager->GetGuiPtr("SAVE_SAVE_FAILED")->menu->Reset();
            m_game->m_gui_manager->SetActive("SAVE_SAVE_FAILED", 0);
            m_game->m_gui_manager->SetVisible("SAVE_SAVE_FAILED", 0);

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
    }
}

CGuiSaveReadyEventHandler::CGuiSaveReadyEventHandler() : CGuiBaseEventHandler("GuiSaveReadyEventHandler") {

}

void CGuiSaveReadyEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    CGuiBaseEventHandler::OnEvent(menu, event, unk);

    if (event != DKGUI::EVENT_3) {
        return;
    }

    std::string str = (char*)unk;
    if (str == "no") {
        m_game->m_gui_manager->GetGuiPtr("SAVE_SAVE_READY")->menu->Reset();
        m_game->m_gui_manager->SetActive("SAVE_SAVE_READY", 0);
        m_game->m_gui_manager->SetVisible("SAVE_SAVE_READY", 0);
    } else if (str == "yes") {
        m_game->m_gui_manager->GetGuiPtr("SAVE_SAVE_READY")->menu->Reset();
        m_game->m_gui_manager->SetActive("SAVE_SAVE_READY", 0);
        m_game->m_gui_manager->SetVisible("SAVE_SAVE_READY", 0);

        m_game->m_gui_manager->GetGuiPtr("SAVE_CHECK_MMC")->menu->Reset();
        m_game->m_gui_manager->SetActive("SAVE_CHECK_MMC", 1);
        m_game->m_gui_manager->SetVisible("SAVE_CHECK_MMC", 1);
    }
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

void CGuiLoadFileDeletingSuccessEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    CGuiBaseEventHandler::OnEvent(menu, event, unk);

    if (event != DKGUI::EVENT_3) {
        return;
    }

    std::string str = (char*)unk;
    if (str == "exit") {
        m_game->m_gui_manager->GetGuiPtr("LOAD_FILE_DELETEOK")->menu->Reset();
        m_game->m_gui_manager->SetActive("LOAD_FILE_DELETEOK", 0);
        m_game->m_gui_manager->SetVisible("LOAD_FILE_DELETEOK", 0);

        m_game->m_gui_manager->GetGuiPtr("LOAD_CHECK_MMC")->menu->Reset();
        m_game->m_gui_manager->SetActive("LOAD_CHECK_MMC", 1);
        m_game->m_gui_manager->SetVisible("LOAD_CHECK_MMC", 1);
    }
}

CGuiLoadFileDeletingFailedEventHandler::CGuiLoadFileDeletingFailedEventHandler() : CGuiBaseEventHandler("GuiLoadFileDeletingFailedEventHandler") {

}

CGuiLoadWrongDeviceEventHandler::CGuiLoadWrongDeviceEventHandler() : CGuiBaseEventHandler("GuiLoadWrongDeviceEventHandler") {

}

void CGuiLoadWrongDeviceEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    CGuiBaseEventHandler::OnEvent(menu, event, unk);

    if (event == DKGUI::EVENT_2) {
        gs_TimeBeforeMemCardCheck = 1.0f;
    } else if (event == DKGUI::EVENT_0) {
        if (gs_TimeBeforeMemCardCheck < 0.0f) {
            m_game->m_timer->Pause();
            u32 backup_state = m_game->m_backup_engine->GetState();
            m_game->m_timer->Resume();

            if (!(backup_state & (1 << 1))) {
                m_game->m_gui_manager->GetGuiPtr("LOAD_WRONG_DEVICE")->menu->Reset();
                m_game->m_gui_manager->SetActive("LOAD_WRONG_DEVICE", 0);
                m_game->m_gui_manager->SetVisible("LOAD_WRONG_DEVICE", 0);

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
            m_game->m_gui_manager->GetGuiPtr("LOAD_WRONG_DEVICE")->menu->Reset();
            m_game->m_gui_manager->SetActive("LOAD_WRONG_DEVICE", 0);
            m_game->m_gui_manager->SetVisible("LOAD_WRONG_DEVICE", 0);

            m_game->m_gui_manager->GetGuiPtr("LOAD_CHECK_MMC")->menu->Reset();
            m_game->m_gui_manager->SetActive("LOAD_CHECK_MMC", 1);
            m_game->m_gui_manager->SetVisible("LOAD_CHECK_MMC", 1);
        } else if (str == "no") {
            m_game->m_gui_manager->GetGuiPtr("LOAD_WRONG_DEVICE")->menu->Reset();
            m_game->m_gui_manager->SetActive("LOAD_WRONG_DEVICE", 0);
            m_game->m_gui_manager->SetVisible("LOAD_WRONG_DEVICE", 0);
        }
    }
}

CGuiSaveWrongDeviceEventHandler::CGuiSaveWrongDeviceEventHandler() : CGuiBaseEventHandler("GuiSaveWrongDeviceEventHandler") {

}

void CGuiSaveWrongDeviceEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    CGuiBaseEventHandler::OnEvent(menu, event, unk);

    if (event == DKGUI::EVENT_2) {
        gs_TimeBeforeMemCardCheck = 1.0f;
    } else if (event == DKGUI::EVENT_0) {
        if (gs_TimeBeforeMemCardCheck < 0.0f) {
            m_game->m_timer->Pause();
            u32 backup_state = m_game->m_backup_engine->GetState();
            m_game->m_timer->Resume();

            if (!(backup_state & (1 << 1))) {
                m_game->m_gui_manager->GetGuiPtr("SAVE_WRONG_DEVICE")->menu->Reset();
                m_game->m_gui_manager->SetActive("SAVE_WRONG_DEVICE", 0);
                m_game->m_gui_manager->SetVisible("SAVE_WRONG_DEVICE", 0);

                m_game->m_gui_manager->GetGuiPtr("SAVE_CHECK_MMC")->menu->Reset();
                m_game->m_gui_manager->SetActive("SAVE_CHECK_MMC", 1);
                m_game->m_gui_manager->SetVisible("SAVE_CHECK_MMC", 1);
            } else {
                gs_TimeBeforeMemCardCheck = 1.0f;
            }
        } else {
            gs_TimeBeforeMemCardCheck -= m_game->GetDeltaTime();
        }
    } else if (event == DKGUI::EVENT_3) {
        std::string str = (char*)unk;
        if (str == "yes") {
            m_game->m_gui_manager->GetGuiPtr("SAVE_WRONG_DEVICE")->menu->Reset();
            m_game->m_gui_manager->SetActive("SAVE_WRONG_DEVICE", 0);
            m_game->m_gui_manager->SetVisible("SAVE_WRONG_DEVICE", 0);

            m_game->m_gui_manager->GetGuiPtr("SAVE_CHECK_MMC")->menu->Reset();
            m_game->m_gui_manager->SetActive("SAVE_CHECK_MMC", 1);
            m_game->m_gui_manager->SetVisible("SAVE_CHECK_MMC", 1);
        } else if (str == "no") {
            m_game->m_gui_manager->GetGuiPtr("SAVE_WRONG_DEVICE")->menu->Reset();
            m_game->m_gui_manager->SetActive("SAVE_WRONG_DEVICE", 0);
            m_game->m_gui_manager->SetVisible("SAVE_WRONG_DEVICE", 0);

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
    }
}

CGuiLoadCorruptMemcardEventHandler::CGuiLoadCorruptMemcardEventHandler() : CGuiBaseEventHandler("GuiLoadCorruptMemcardEventHandler") {

}

// Equivalent: regalloc
void CGuiLoadCorruptMemcardEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    CGuiBaseEventHandler::OnEvent(menu, event, unk);

    if (event == DKGUI::EVENT_0) {
        m_game->m_timer->Pause();
        u32 backup_state = m_game->m_backup_engine->GetState();
        m_game->m_timer->Resume();

        if (!(backup_state & (1 << 0))) {
            m_game->m_gui_manager->GetGuiPtr("LOAD_MMC_CORRUPT")->menu->Reset();
            m_game->m_gui_manager->SetActive("LOAD_MMC_CORRUPT", 0);
            m_game->m_gui_manager->SetVisible("LOAD_MMC_CORRUPT", 0);

            m_game->m_gui_manager->GetGuiPtr("LOAD_CHECK_MMC")->menu->Reset();
            m_game->m_gui_manager->SetActive("LOAD_CHECK_MMC", 1);
            m_game->m_gui_manager->SetVisible("LOAD_CHECK_MMC", 1);
        }
    } else if (event == DKGUI::EVENT_3) {
        std::string str = (char*)unk;
        if (str == "yes") {
            m_game->m_gui_manager->GetGuiPtr("LOAD_MMC_CORRUPT")->menu->Reset();
            m_game->m_gui_manager->SetActive("LOAD_MMC_CORRUPT", 0);
            m_game->m_gui_manager->SetVisible("LOAD_MMC_CORRUPT", 0);

            // nice
            m_game->m_unk8 |= m_game->m_unk8 | (1 << 9);

            m_game->m_gui_manager->GetGuiPtr("SAVE_FORMAT_SURE")->menu->Reset();
            m_game->m_gui_manager->SetActive("SAVE_FORMAT_SURE", 1);
            m_game->m_gui_manager->SetVisible("SAVE_FORMAT_SURE", 1);
        } else if (str == "no") {
            m_game->m_gui_manager->GetGuiPtr("LOAD_MMC_CORRUPT")->menu->Reset();
            m_game->m_gui_manager->SetActive("LOAD_MMC_CORRUPT", 0);
            m_game->m_gui_manager->SetVisible("LOAD_MMC_CORRUPT", 0);
        }
    }
}

CGuiSaveCorruptMemcardEventHandler::CGuiSaveCorruptMemcardEventHandler() : CGuiBaseEventHandler("GuiSaveCorruptMemcardEventHandler") {

}

void CGuiSaveCorruptMemcardEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    CGuiBaseEventHandler::OnEvent(menu, event, unk);

    if (event == DKGUI::EVENT_0) {
        m_game->m_timer->Pause();
        u32 backup_state = m_game->m_backup_engine->GetState();
        m_game->m_timer->Resume();

        if (!(backup_state & (1 << 0))) {
            m_game->m_gui_manager->GetGuiPtr("SAVE_MMC_UNUSEABLE")->menu->Reset();
            m_game->m_gui_manager->SetActive("SAVE_MMC_UNUSEABLE", 0);
            m_game->m_gui_manager->SetVisible("SAVE_MMC_UNUSEABLE", 0);

            m_game->m_gui_manager->GetGuiPtr("SAVE_CHECK_MMC")->menu->Reset();
            m_game->m_gui_manager->SetActive("SAVE_CHECK_MMC", 1);
            m_game->m_gui_manager->SetVisible("SAVE_CHECK_MMC", 1);
        }
    } else if (event == DKGUI::EVENT_3) {
        std::string str = (char*)unk;
        if (str == "yes") {
            m_game->m_gui_manager->GetGuiPtr("SAVE_MMC_UNUSEABLE")->menu->Reset();
            m_game->m_gui_manager->SetActive("SAVE_MMC_UNUSEABLE", 0);
            m_game->m_gui_manager->SetVisible("SAVE_MMC_UNUSEABLE", 0);

            m_game->m_gui_manager->GetGuiPtr("SAVE_CHECK_MMC")->menu->Reset();
            m_game->m_gui_manager->SetActive("SAVE_CHECK_MMC", 1);
            m_game->m_gui_manager->SetVisible("SAVE_CHECK_MMC", 1);
        } else if (str == "no") {
            m_game->m_gui_manager->GetGuiPtr("SAVE_MMC_UNUSEABLE")->menu->Reset();
            m_game->m_gui_manager->SetActive("SAVE_MMC_UNUSEABLE", 0);
            m_game->m_gui_manager->SetVisible("SAVE_MMC_UNUSEABLE", 0);

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
    }
}

CGuiSaveMemoryCardUnuseableEventHandler::CGuiSaveMemoryCardUnuseableEventHandler() : CGuiBaseEventHandler("GuiSaveMemoryCardUnuseableEventHandler") {

}

void CGuiSaveMemoryCardUnuseableEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    CGuiBaseEventHandler::OnEvent(menu, event, unk);

    if (event == DKGUI::EVENT_2) {
        gs_TimeBeforeMemCardCheck = 1.0f;
    } else if (event == DKGUI::EVENT_0) {
        if (gs_TimeBeforeMemCardCheck < 0.0f) {
            m_game->m_timer->Pause();
            u32 backup_state = m_game->m_backup_engine->GetState();
            m_game->m_timer->Resume();

            if (!(backup_state & (1 << 0))) {
                m_game->m_gui_manager->GetGuiPtr("SAVE_MMC_UNUSEABLE")->menu->Reset();
                m_game->m_gui_manager->SetActive("SAVE_MMC_UNUSEABLE", 0);
                m_game->m_gui_manager->SetVisible("SAVE_MMC_UNUSEABLE", 0);

                m_game->m_gui_manager->GetGuiPtr("SAVE_CHECK_MMC")->menu->Reset();
                m_game->m_gui_manager->SetActive("SAVE_CHECK_MMC", 1);
                m_game->m_gui_manager->SetVisible("SAVE_CHECK_MMC", 1);
            } else {
                gs_TimeBeforeMemCardCheck = 1.0f;
            }
        } else {
            gs_TimeBeforeMemCardCheck -= m_game->GetDeltaTime();
        }
    } else if (event == DKGUI::EVENT_3) {
        std::string str = (char*)unk;
        if (str == "yes") {
            m_game->m_gui_manager->GetGuiPtr("SAVE_MMC_UNUSEABLE")->menu->Reset();
            m_game->m_gui_manager->SetActive("SAVE_MMC_UNUSEABLE", 0);
            m_game->m_gui_manager->SetVisible("SAVE_MMC_UNUSEABLE", 0);

            m_game->m_gui_manager->GetGuiPtr("SAVE_CHECK_MMC")->menu->Reset();
            m_game->m_gui_manager->SetActive("SAVE_CHECK_MMC", 1);
            m_game->m_gui_manager->SetVisible("SAVE_CHECK_MMC", 1);
        } else if (str == "no") {
            m_game->m_gui_manager->GetGuiPtr("SAVE_MMC_UNUSEABLE")->menu->Reset();
            m_game->m_gui_manager->SetActive("SAVE_MMC_UNUSEABLE", 0);
            m_game->m_gui_manager->SetVisible("SAVE_MMC_UNUSEABLE", 0);

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
    }
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

void CGuiSaveNoMemCardEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    CGuiBaseEventHandler::OnEvent(menu, event, unk);

    if (event != DKGUI::EVENT_3) {
        return;
    }

    std::string str = (char*)unk;
    if (str == "yes") {
        m_game->m_gui_manager->GetGuiPtr("SAVE_NO_MMC")->menu->Reset();
        m_game->m_gui_manager->SetActive("SAVE_NO_MMC", 0);
        m_game->m_gui_manager->SetVisible("SAVE_NO_MMC", 0);

        m_game->m_gui_manager->GetGuiPtr("SAVE_CHECK_MMC")->menu->Reset();
        m_game->m_gui_manager->SetActive("SAVE_CHECK_MMC", 1);
        m_game->m_gui_manager->SetVisible("SAVE_CHECK_MMC", 1);
    } else if (str == "no") {
        m_game->m_gui_manager->GetGuiPtr("SAVE_NO_MMC")->menu->Reset();
        m_game->m_gui_manager->SetActive("SAVE_NO_MMC", 0);
        m_game->m_gui_manager->SetVisible("SAVE_NO_MMC", 0);

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
}

CGuiSaveFormatEventHandler::CGuiSaveFormatEventHandler() : CGuiBaseEventHandler("GuiSaveFormatEventHandler") {

}

void CGuiSaveFormatEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    CGuiBaseEventHandler::OnEvent(menu, event, unk);

    if (event == DKGUI::EVENT_2) {
        gs_TimeBeforeMemCardCheck = 1.0f;
    } else if (event == DKGUI::EVENT_0) {
        if (gs_TimeBeforeMemCardCheck < 0.0f) {
            m_game->m_timer->Pause();
            u32 backup_state = m_game->m_backup_engine->GetState();
            m_game->m_timer->Resume();

            if (!(backup_state & (1 << 0))) {
                m_game->m_gui_manager->GetGuiPtr("SAVE_UNFORMAT")->menu->Reset();
                m_game->m_gui_manager->SetActive("SAVE_UNFORMAT", 0);
                m_game->m_gui_manager->SetVisible("SAVE_UNFORMAT", 0);

                if (m_game->m_unk8 & (1 << 9)) {
                    m_game->m_gui_manager->GetGuiPtr("LOAD_CHECK_MMC")->menu->Reset();
                    m_game->m_gui_manager->SetActive("LOAD_CHECK_MMC", 1);
                    m_game->m_gui_manager->SetVisible("LOAD_CHECK_MMC", 1);
                    m_game->m_unk8 &= ~(1 << 9);
                } else {
                    m_game->m_gui_manager->GetGuiPtr("SAVE_CHECK_MMC")->menu->Reset();
                    m_game->m_gui_manager->SetActive("SAVE_CHECK_MMC", 1);
                    m_game->m_gui_manager->SetVisible("SAVE_CHECK_MMC", 1);
                }
            } else {
                gs_TimeBeforeMemCardCheck = 1.0f;
            }
        } else {
            gs_TimeBeforeMemCardCheck -= m_game->GetDeltaTime();
        }
    } else if (event == DKGUI::EVENT_3) {
        std::string str = (char*)unk;
        if (str == "yes") {
            m_game->m_gui_manager->GetGuiPtr("SAVE_UNFORMAT")->menu->Reset();
            m_game->m_gui_manager->SetActive("SAVE_UNFORMAT", 0);
            m_game->m_gui_manager->SetVisible("SAVE_UNFORMAT", 0);

            m_game->m_gui_manager->GetGuiPtr("SAVE_FORMAT_SURE")->menu->Reset();
            m_game->m_gui_manager->SetActive("SAVE_FORMAT_SURE", 1);
            m_game->m_gui_manager->SetVisible("SAVE_FORMAT_SURE", 1);

        } else if (str == "no") {
            m_game->m_gui_manager->GetGuiPtr("SAVE_UNFORMAT")->menu->Reset();
            m_game->m_gui_manager->SetActive("SAVE_UNFORMAT", 0);
            m_game->m_gui_manager->SetVisible("SAVE_UNFORMAT", 0);

            if (m_game->m_unk8 & (1 << 9)) {
                m_game->m_unk8 &= ~(1 << 9);
                return;
            }

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
    }
}

CGuiSaveFormatSureEventHandler::CGuiSaveFormatSureEventHandler() : CGuiBaseEventHandler("GuiSaveFormatSureEventHandler") {

}

void CGuiSaveFormatSureEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    CGuiBaseEventHandler::OnEvent(menu, event, unk);

    if (event == DKGUI::EVENT_2) {
        gs_TimeBeforeMemCardCheck = 1.0f;
    } else if (event == DKGUI::EVENT_0) {
        if (gs_TimeBeforeMemCardCheck < 0.0f) {
            m_game->m_timer->Pause();
            u32 backup_state = m_game->m_backup_engine->GetState();
            m_game->m_timer->Resume();

            if (!(backup_state & (1 << 0))) {
                m_game->m_gui_manager->GetGuiPtr("SAVE_FORMAT_SURE")->menu->Reset();
                m_game->m_gui_manager->SetActive("SAVE_FORMAT_SURE", 0);
                m_game->m_gui_manager->SetVisible("SAVE_FORMAT_SURE", 0);

                if (m_game->m_unk8 & (1 << 9)) {
                    m_game->m_gui_manager->GetGuiPtr("LOAD_CHECK_MMC")->menu->Reset();
                    m_game->m_gui_manager->SetActive("LOAD_CHECK_MMC", 1);
                    m_game->m_gui_manager->SetVisible("LOAD_CHECK_MMC", 1);
                    m_game->m_unk8 &= ~(1 << 9);
                } else {
                    m_game->m_gui_manager->GetGuiPtr("SAVE_CHECK_MMC")->menu->Reset();
                    m_game->m_gui_manager->SetActive("SAVE_CHECK_MMC", 1);
                    m_game->m_gui_manager->SetVisible("SAVE_CHECK_MMC", 1);
                }
            } else {
                gs_TimeBeforeMemCardCheck = 1.0f;
            }
        } else {
            gs_TimeBeforeMemCardCheck -= m_game->GetDeltaTime();
        }
    } else if (event == DKGUI::EVENT_3) {
        std::string str = (char*)unk;
        if (str == "yes") {
            m_game->m_gui_manager->GetGuiPtr("SAVE_FORMAT_SURE")->menu->Reset();
            m_game->m_gui_manager->SetActive("SAVE_FORMAT_SURE", 0);
            m_game->m_gui_manager->SetVisible("SAVE_FORMAT_SURE", 0);

            m_game->m_gui_manager->GetGuiPtr("SAVE_FORMAT")->menu->Reset();
            m_game->m_gui_manager->SetActive("SAVE_FORMAT", 1);
            m_game->m_gui_manager->SetVisible("SAVE_FORMAT", 1);
        } else if (str == "no") {
            m_game->m_gui_manager->GetGuiPtr("SAVE_FORMAT_SURE")->menu->Reset();
            m_game->m_gui_manager->SetActive("SAVE_FORMAT_SURE", 0);
            m_game->m_gui_manager->SetVisible("SAVE_FORMAT_SURE", 0);

            if (m_game->m_unk8 & (1 << 9)) {
                m_game->m_unk8 &= ~(1 << 9);
                return;
            }

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
    }
}

CGuiSaveFormattingEventHandler::CGuiSaveFormattingEventHandler() : CGuiBaseEventHandler("GuiSaveFormattingEventHandler") {

}

// Equivalent: regalloc
void CGuiSaveFormattingEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    CGuiBaseEventHandler::OnEvent(menu, event, unk);

    if (event != DKGUI::EVENT_3) {
        return;
    }

    std::string str = (char*)unk;
    if (str == "yes") {
        m_game->m_gui_manager->GetGuiPtr("SAVE_FORMAT_FAILED")->menu->Reset();
        m_game->m_gui_manager->SetActive("SAVE_FORMAT_FAILED", 0);
        m_game->m_gui_manager->SetVisible("SAVE_FORMAT_FAILED", 0);

        m_game->m_timer->Pause();
        u32 backup_status = m_game->m_backup_engine->GetState();
        m_game->m_timer->Resume();

        if (backup_status == 17) {
            if (m_game->m_unk8 & (1 << 9)) {
                m_game->m_unk8 &= ~(1 << 9);
                m_game->m_gui_manager->GetGuiPtr("LOAD_CHECK_MMC")->menu->Reset();
                m_game->m_gui_manager->SetActive("LOAD_CHECK_MMC", 1);
                m_game->m_gui_manager->SetVisible("LOAD_CHECK_MMC", 1);
                return;
            } else {
                m_game->m_gui_manager->GetGuiPtr("SAVE_CHECK_MMC")->menu->Reset();
                m_game->m_gui_manager->SetActive("SAVE_CHECK_MMC", 1);
                m_game->m_gui_manager->SetVisible("SAVE_CHECK_MMC", 1);
                return;
            }
        } else if (backup_status == 0) {
            if (m_game->m_unk8 & (1 << 9)) {
                m_game->m_unk8 &= ~(1 << 9);
                m_game->m_gui_manager->GetGuiPtr("LOAD_CHECK_MMC")->menu->Reset();
                m_game->m_gui_manager->SetActive("LOAD_CHECK_MMC", 1);
                m_game->m_gui_manager->SetVisible("LOAD_CHECK_MMC", 1);
                return;
            } else {
                m_game->m_gui_manager->GetGuiPtr("SAVE_CHECK_MMC")->menu->Reset();
                m_game->m_gui_manager->SetActive("SAVE_CHECK_MMC", 1);
                m_game->m_gui_manager->SetVisible("SAVE_CHECK_MMC", 1);
                return;
            }
        }

        if (m_game->m_backup_engine->Format() != 0) {
            m_game->m_gui_manager->GetGuiPtr("SAVE_FORMAT_OK")->menu->Reset();
            m_game->m_gui_manager->SetActive("SAVE_FORMAT_OK", 1);
            m_game->m_gui_manager->SetVisible("SAVE_FORMAT_OK", 1);
        } else {
            m_game->m_gui_manager->GetGuiPtr("SAVE_FORMAT_FAILED")->menu->Reset();
            m_game->m_gui_manager->SetActive("SAVE_FORMAT_FAILED", 1);
            m_game->m_gui_manager->SetVisible("SAVE_FORMAT_FAILED", 1);
        }
    }
}

CGuiFormatOkEventHandler::CGuiFormatOkEventHandler() : CGuiBaseEventHandler("GuiSaveFormatOkEventHandler") {

}

// Equivalent: regalloc
void CGuiFormatOkEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    CGuiBaseEventHandler::OnEvent(menu, event, unk);

    if (event == DKGUI::EVENT_2) {
        m_unkC = FALSE;
        return;
    }

    if (m_unkC == TRUE) {
        if (m_unkC == TRUE) {
            m_game->m_gui_manager->GetGuiPtr("SAVE_FORMAT_OK")->menu->Reset();
            m_game->m_gui_manager->SetActive("SAVE_FORMAT_OK", 0);
            m_game->m_gui_manager->SetVisible("SAVE_FORMAT_OK", 0);

            if (m_game->m_unk8 & (1 << 9)) {
                m_game->m_unk8 &= ~(1 << 9);
            } else {
                m_game->m_gui_manager->GetGuiPtr("SAVE_CHECK_MMC")->menu->Reset();
                m_game->m_gui_manager->SetActive("SAVE_CHECK_MMC", 1);
                m_game->m_gui_manager->SetVisible("SAVE_CHECK_MMC", 1);
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

CGuiFormatFailedEventHandler::CGuiFormatFailedEventHandler() : CGuiBaseEventHandler("GuiSaveFormatFailedEventHandler") {

}

// Equivalent: regalloc
void CGuiFormatFailedEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    CGuiBaseEventHandler::OnEvent(menu, event, unk);

    if (event == DKGUI::EVENT_2) {
        gs_TimeBeforeMemCardCheck = 1.0f;
    } else if (event == DKGUI::EVENT_0) {
        if (gs_TimeBeforeMemCardCheck < 0.0f) {
            m_game->m_timer->Pause();
            u32 backup_state = m_game->m_backup_engine->GetState();
            m_game->m_timer->Resume();

            if (!(backup_state & (1 << 0))) {
                m_game->m_gui_manager->GetGuiPtr("SAVE_FORMAT_FAILED")->menu->Reset();
                m_game->m_gui_manager->SetActive("SAVE_FORMAT_FAILED", 0);
                m_game->m_gui_manager->SetVisible("SAVE_FORMAT_FAILED", 0);

                if (m_game->m_unk8 & (1 << 9)) {
                    m_game->m_unk8 &= ~(1 << 9);
                    m_game->m_gui_manager->GetGuiPtr("LOAD_CHECK_MMC")->menu->Reset();
                    m_game->m_gui_manager->SetActive("LOAD_CHECK_MMC", 1);
                    m_game->m_gui_manager->SetVisible("LOAD_CHECK_MMC", 1);
                } else {
                    m_game->m_gui_manager->GetGuiPtr("SAVE_CHECK_MMC")->menu->Reset();
                    m_game->m_gui_manager->SetActive("SAVE_CHECK_MMC", 1);
                    m_game->m_gui_manager->SetVisible("SAVE_CHECK_MMC", 1);
                }
            } else {
                gs_TimeBeforeMemCardCheck = 1.0f;
            }
        } else {
            gs_TimeBeforeMemCardCheck -= m_game->GetDeltaTime();
        }
    } else if (event == DKGUI::EVENT_3) {
        std::string str = (char*)unk;
        if (str == "yes") {
            m_game->m_gui_manager->GetGuiPtr("SAVE_FORMAT_FAILED")->menu->Reset();
            m_game->m_gui_manager->SetActive("SAVE_FORMAT_FAILED", 0);
            m_game->m_gui_manager->SetVisible("SAVE_FORMAT_FAILED", 0);

            if (m_game->m_unk8 & (1 << 9)) {
                m_game->m_unk8 &= ~(1 << 9);
                m_game->m_gui_manager->GetGuiPtr("LOAD_CHECK_MMC")->menu->Reset();
                m_game->m_gui_manager->SetActive("LOAD_CHECK_MMC", 1);
                m_game->m_gui_manager->SetVisible("LOAD_CHECK_MMC", 1);
                return;
            } else {
                m_game->m_gui_manager->GetGuiPtr("SAVE_CHECK_MMC")->menu->Reset();
                m_game->m_gui_manager->SetActive("SAVE_CHECK_MMC", 1);
                m_game->m_gui_manager->SetVisible("SAVE_CHECK_MMC", 1);
            }
        } else if (str == "no") {
            m_game->m_gui_manager->GetGuiPtr("SAVE_FORMAT_FAILED")->menu->Reset();
            m_game->m_gui_manager->SetActive("SAVE_FORMAT_FAILED", 0);
            m_game->m_gui_manager->SetVisible("SAVE_FORMAT_FAILED", 0);

            if (m_game->m_unk8 & (1 << 9)) {
                m_game->m_unk8 &= ~(1 << 9);
                return;
            } else if (m_game->m_unk4F54 == 8 && m_game->m_unk4F58 == 1) {
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
    }
}

CGuiOverwriteEventHandler::CGuiOverwriteEventHandler() : CGuiBaseEventHandler("GuiOverwriteEventHandler") {

}

void CGuiOverwriteEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    CGuiBaseEventHandler::OnEvent(menu, event, unk);

    if (event == DKGUI::EVENT_2) {
        gs_TimeBeforeMemCardCheck = 1.0f;
    } else if (event == DKGUI::EVENT_0) {
        if (gs_TimeBeforeMemCardCheck < 0.0f) {
            m_game->m_timer->Pause();
            u32 backup_state = m_game->m_backup_engine->GetState();
            m_game->m_timer->Resume();

            if (!(backup_state & (1 << 0))) {
                m_game->m_gui_manager->GetGuiPtr("CREATE_OVERWRITE")->menu->Reset();
                m_game->m_gui_manager->SetActive("CREATE_OVERWRITE", 0);
                m_game->m_gui_manager->SetVisible("CREATE_OVERWRITE", 0);

                m_game->m_gui_manager->GetGuiPtr("SAVE_CHECK_MMC")->menu->Reset();
                m_game->m_gui_manager->SetActive("SAVE_CHECK_MMC", 1);
                m_game->m_gui_manager->SetVisible("SAVE_CHECK_MMC", 1);
            } else {
                gs_TimeBeforeMemCardCheck = 1.0f;
            }
        } else {
            gs_TimeBeforeMemCardCheck -= m_game->GetDeltaTime();
        }
    } else if (event == DKGUI::EVENT_3) {
        std::string str = (char*)unk;
        if (str == "yes") {
            m_game->m_gui_manager->GetGuiPtr("CREATE_OVERWRITE")->menu->Reset();
            m_game->m_gui_manager->SetActive("CREATE_OVERWRITE", 0);
            m_game->m_gui_manager->SetVisible("CREATE_OVERWRITE", 0);

            m_game->m_gui_manager->GetGuiPtr("CREATE_OVERWRITE_SURE")->menu->Reset();
            m_game->m_gui_manager->SetActive("CREATE_OVERWRITE_SURE", 1);
            m_game->m_gui_manager->SetVisible("CREATE_OVERWRITE_SURE", 1);
        } else if (str == "no") {
            m_game->m_gui_manager->GetGuiPtr("CREATE_OVERWRITE")->menu->Reset();
            m_game->m_gui_manager->SetActive("CREATE_OVERWRITE", 0);
            m_game->m_gui_manager->SetVisible("CREATE_OVERWRITE", 0);
        }
    }
}

CGuiOverwriteSureEventHandler::CGuiOverwriteSureEventHandler() : CGuiBaseEventHandler("GuiOverwriteSureEventHandler") {

}

// Incomplete
void CGuiOverwriteSureEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    CGuiBaseEventHandler::OnEvent(menu, event, unk);

    if (event == DKGUI::EVENT_2) {
        gs_TimeBeforeMemCardCheck = 1.0f;
    } else if (event == DKGUI::EVENT_0) {
        if (gs_TimeBeforeMemCardCheck < 0.0f) {
            m_game->m_timer->Pause();
            u32 backup_state = m_game->m_backup_engine->GetState();
            m_game->m_timer->Resume();

            if (!(backup_state & (1 << 0))) {
                m_game->m_gui_manager->GetGuiPtr("CREATE_OVERWRITE_SURE")->menu->Reset();
                m_game->m_gui_manager->SetActive("CREATE_OVERWRITE_SURE", 0);
                m_game->m_gui_manager->SetVisible("CREATE_OVERWRITE_SURE", 0);

                m_game->m_gui_manager->GetGuiPtr("SAVE_CHECK_MMC")->menu->Reset();
                m_game->m_gui_manager->SetActive("SAVE_CHECK_MMC", 1);
                m_game->m_gui_manager->SetVisible("SAVE_CHECK_MMC", 1);
            } else {
                gs_TimeBeforeMemCardCheck = 1.0f;
            }
        } else {
            gs_TimeBeforeMemCardCheck -= m_game->GetDeltaTime();
        }
    } else if (event == DKGUI::EVENT_3) {
        std::string str = (char*)unk;
        if (str == "yes") {
            m_game->m_gui_manager->GetGuiPtr("CREATE_OVERWRITE_SURE")->menu->Reset();
            m_game->m_gui_manager->SetActive("CREATE_OVERWRITE_SURE", 0);
            m_game->m_gui_manager->SetVisible("CREATE_OVERWRITE_SURE", 0);

            m_game->m_timer->Pause();
            u32 backup_state = m_game->m_backup_engine->GetState();
            m_game->m_timer->Resume();

            if (backup_state == 17) {
                CGuiEnterNameEventHandler* handler = (CGuiEnterNameEventHandler*)m_game->m_gui_manager->IsEventCallbackRegistered("GuiEnterNameEventHandler");
                m_game->m_backup_engine->FindFirst("GPLE9G", handler->GetText());

                if (m_game->m_backup_engine->IsSpaceAvailable("GPLE9G", handler->GetText(), 0x18000) != 0) {
                    m_game->m_gui_manager->GetGuiPtr("SAVE_NO_SPACE")->menu->Reset();
                    m_game->m_gui_manager->SetActive("SAVE_NO_SPACE", 1);
                    m_game->m_gui_manager->SetVisible("SAVE_NO_SPACE", 1);
                } else {
                    m_game->m_gui_manager->GetGuiPtr("SAVE_SAVE_DATA")->menu->Reset();
                    m_game->m_gui_manager->SetActive("SAVE_SAVE_DATA", 1);
                    m_game->m_gui_manager->SetVisible("SAVE_SAVE_DATA", 1);
                    return;
                }
                
            } else if (backup_state & (1 << 2)) {
                m_game->m_gui_manager->GetGuiPtr("SAVE_MMC_CORRUPT")->menu->Reset();
                m_game->m_gui_manager->SetActive("SAVE_MMC_CORRUPT", 1);
                m_game->m_gui_manager->SetVisible("SAVE_MMC_CORRUPT", 1);
            } else if (backup_state & (1 << 3)) {
                m_game->m_gui_manager->GetGuiPtr("SAVE_MMC_UNUSEABLE")->menu->Reset();
                m_game->m_gui_manager->SetActive("SAVE_MMC_UNUSEABLE", 1);
                m_game->m_gui_manager->SetVisible("SAVE_MMC_UNUSEABLE", 1);
            } else if (backup_state & (1 << 1)) {
                m_game->m_gui_manager->GetGuiPtr("SAVE_WRONG_DEVICE")->menu->Reset();
                m_game->m_gui_manager->SetActive("SAVE_WRONG_DEVICE", 1);
                m_game->m_gui_manager->SetVisible("SAVE_WRONG_DEVICE", 1);
            } else if (!(backup_state & (1 << 0))) {
                m_game->m_gui_manager->GetGuiPtr("SAVE_NO_MMC")->menu->Reset();
                m_game->m_gui_manager->SetActive("SAVE_NO_MMC", 1);
                m_game->m_gui_manager->SetVisible("SAVE_NO_MMC", 1);
            } else if (!(backup_state & (1 << 4))) {
                m_game->m_gui_manager->GetGuiPtr("SAVE_UNFORMAT")->menu->Reset();
                m_game->m_gui_manager->SetActive("SAVE_UNFORMAT", 1);
                m_game->m_gui_manager->SetVisible("SAVE_UNFORMAT", 1);
            }
        } else if (str == "no") {
            m_game->m_gui_manager->GetGuiPtr("CREATE_OVERWRITE_SURE")->menu->Reset();
            m_game->m_gui_manager->SetActive("CREATE_OVERWRITE_SURE", 0);
            m_game->m_gui_manager->SetVisible("CREATE_OVERWRITE_SURE", 0);
        }
    }
}

CGuiDreamSelectEventHandler::CGuiDreamSelectEventHandler() : CGuiBaseEventHandler("GuiDreamSelectEventHandler") {
    m_unk10 = 0;
    m_unk18 = 0;
}

void CGuiDreamSelectEventHandler::PlayNarrator(int mission_no) {
    if (m_game->m_unk210[mission_no - 1].IsSecondPassCompleted() != 0u) {
        return;
    }

    switch (mission_no) {
        case 1:
            if (m_game->m_unk210[0].m_unk2C != 0) {
                m_game->PlayNarratorLine("N803_02");
            } else {
                m_game->PlayNarratorLine("N803_01");
            }
            break;
        case 2:
            if (m_game->m_unk210[1].m_unk2C != 0) {
                m_game->PlayNarratorLine("N803_02");
            } else {
                m_game->PlayNarratorLine("N803_03");
            }
            break;
        case 3:
            if (m_game->m_unk210[2].m_unk30 != 0) {
                m_game->PlayNarratorLine("N803_04");
            } else {
                if (m_game->m_unk210[2].m_unk2C != 0) {
                    m_game->PlayNarratorLine("N803_02");
                } else {
                    m_game->PlayNarratorLine("N803_05");
                }
            }
            break;
        case 4:
            if (m_game->m_unk210[3].m_unk30 != 0) {
                m_game->PlayNarratorLine("N803_06");
            } else {
                if (m_game->m_unk210[3].m_unk2C != 0) {
                    m_game->PlayNarratorLine("N803_02");
                } else {
                    m_game->PlayNarratorLine("N803_07");
                }
            }
            break;
        case 5:
            if (m_game->m_unk210[4].m_unk30 != 0) {
                m_game->PlayNarratorLine("N803_08");
            } else {
                if (m_game->m_unk210[4].m_unk2C != 0) {
                    m_game->PlayNarratorLine("N803_02");
                } else {
                    m_game->PlayNarratorLine("N803_09");
                }
            }
            break;
        case 6:
            if (m_game->m_unk210[5].m_unk30 != 0) {
                m_game->PlayNarratorLine("N803_10");
            } else {
                if (m_game->m_unk210[5].m_unk2C != 0) {
                    m_game->PlayNarratorLine("N803_02");
                } else {
                    m_game->PlayNarratorLine("N803_11");
                }
            }
            break;
        case 7:
            m_game->PlayNarratorLine("N803_12");
            break;
    }
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
