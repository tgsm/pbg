#include <dolphin/pad.h>
#include <dolphin/vi.h>
#include "engine/display/DkDisplay.h"
#include "engine/input/IInputEngine.h"
#include "engine/sound/CSound2D.h"
#include "engine/sound/DkSound.h"
#include "CEventGuiHandlers.h"
#include "CGame.h"
#include "CGamePartIngame.h"
#include <iostream>

static F32 gs_TimeBeforeMemCardCheck;

CGuiBaseEventHandler::CGuiBaseEventHandler(const std::string& type) : m_type(type) {

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
        m_game->FadeInit(1.0f, CGame::FADE_TYPE_4, fade_color.red, fade_color.green, fade_color.blue, 0.0f);

        m_game->m_screen_effect->SetSequenceByIndex(0);

        ((CGamePartIngame*)m_game->GetGamePartPointer())->m_game_room_manager->m_flags |= (1 << 5);

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
        m_game->GetGuiManager()->GetGuiPtr("PAUSE_PAUSE_GAME")->menu->Reset();
        m_game->GetGuiManager()->SetActive("PAUSE_PAUSE_GAME", 0);
        m_game->GetGuiManager()->SetVisible("PAUSE_PAUSE_GAME", 0);
    } else {
        m_game->GetGuiManager()->GetGuiPtr("PAUSE_PAUSE_DREAM")->menu->Reset();
        m_game->GetGuiManager()->SetActive("PAUSE_PAUSE_DREAM", 0);
        m_game->GetGuiManager()->SetVisible("PAUSE_PAUSE_DREAM", 0);
    }
}

CGuiLoadCheckingMemoryCardEventHandler::CGuiLoadCheckingMemoryCardEventHandler() : CGuiBaseEventHandler("GuiLoadCheckingMemoryCardEventHandler") {

}

void CGuiLoadCheckingMemoryCardEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    CGuiBaseEventHandler::OnEvent(menu, event, unk);

    if (event == DKGUI::EVENT_3) {
        std::string str = (char*)unk;
        std::string str2;
        if (str == "exit") {
            m_game->m_timer->Pause();
            U32 backup_state = m_game->GetBackupEngine()->GetState();
            m_game->m_timer->Resume();

            m_game->GetGuiManager()->GetGuiPtr("LOAD_CHECK_MMC")->menu->Reset();
            m_game->GetGuiManager()->SetActive("LOAD_CHECK_MMC", 0);
            m_game->GetGuiManager()->SetVisible("LOAD_CHECK_MMC", 0);

            if (backup_state == 17) {
                CGuiChooseGameEventHandler* handler = (CGuiChooseGameEventHandler*)m_game->GetGuiManager()->IsEventCallbackRegistered("GuiChooseGameEventHandler");
                if (handler != NULL) {
                    handler->m_unk10.clear();
                }

                str2.assign(m_game->GetBackupEngine()->FindFirst("GPLE9G", ""), 0);
                if (str2 == "") {
                    m_game->GetGuiManager()->GetGuiPtr("LOAD_NO_DATA")->menu->Reset();
                    m_game->GetGuiManager()->SetActive("LOAD_NO_DATA", 1);
                    m_game->GetGuiManager()->SetVisible("LOAD_NO_DATA", 1);
                } else {
                    handler->AddToList(str2);

                    str2.assign(m_game->GetBackupEngine()->FindNext(), 0);
                    while (str2 != "") {
                        handler->AddToList(str2);
                        str2.assign(m_game->GetBackupEngine()->FindNext(), 0);
                    }

                    handler->m_unk1C = 0;
                    handler->m_unk20 = 0;

                    m_game->GetGuiManager()->GetGuiPtr("LOAD_CHOOSE_GAME")->menu->Reset();
                    m_game->GetGuiManager()->SetActive("LOAD_CHOOSE_GAME", 1);
                    m_game->GetGuiManager()->SetVisible("LOAD_CHOOSE_GAME", 1);
                }
            } else if (backup_state & (1 << 2)) {
                m_game->GetGuiManager()->GetGuiPtr("LOAD_MMC_CORRUPT")->menu->Reset();
                m_game->GetGuiManager()->SetActive("LOAD_MMC_CORRUPT", 1);
                m_game->GetGuiManager()->SetVisible("LOAD_MMC_CORRUPT", 1);
            } else if (backup_state & (1 << 3)) {
                m_game->GetGuiManager()->GetGuiPtr("LOAD_MMC_UNUSEABLE")->menu->Reset();
                m_game->GetGuiManager()->SetActive("LOAD_MMC_UNUSEABLE", 1);
                m_game->GetGuiManager()->SetVisible("LOAD_MMC_UNUSEABLE", 1);
            } else if (backup_state & (1 << 1)) {
                m_game->GetGuiManager()->GetGuiPtr("LOAD_WRONG_DEVICE")->menu->Reset();
                m_game->GetGuiManager()->SetActive("LOAD_WRONG_DEVICE", 1);
                m_game->GetGuiManager()->SetVisible("LOAD_WRONG_DEVICE", 1);
            } else if (!(backup_state & (1 << 0))) {
                m_game->GetGuiManager()->GetGuiPtr("LOAD_NO_MMC")->menu->Reset();
                m_game->GetGuiManager()->SetActive("LOAD_NO_MMC", 1);
                m_game->GetGuiManager()->SetVisible("LOAD_NO_MMC", 1);
            } else if (!(backup_state & (1 << 4))) {
                U32 flags = m_game->m_unk8;
                flags |= (1 << 9);
                m_game->m_unk8 |= flags;

                m_game->GetGuiManager()->GetGuiPtr("SAVE_UNFORMAT")->menu->Reset();
                m_game->GetGuiManager()->SetActive("SAVE_UNFORMAT", 1);
                m_game->GetGuiManager()->SetVisible("SAVE_UNFORMAT", 1);
            }
        }
    }
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
            m_game->GetGuiManager()->GetGuiPtr("LOAD_NO_DATA")->menu->Reset();
            m_game->GetGuiManager()->SetActive("LOAD_NO_DATA", 0);
            m_game->GetGuiManager()->SetVisible("LOAD_NO_DATA", 0);
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
        m_game->GetGuiManager()->GetGuiPtr("LOAD_NO_MMC")->menu->Reset();
        m_game->GetGuiManager()->SetActive("LOAD_NO_MMC", 0);
        m_game->GetGuiManager()->SetVisible("LOAD_NO_MMC", 0);
    } else if (str == "yes") {
        m_game->GetGuiManager()->GetGuiPtr("LOAD_NO_MMC")->menu->Reset();
        m_game->GetGuiManager()->SetActive("LOAD_NO_MMC", 0);
        m_game->GetGuiManager()->SetVisible("LOAD_NO_MMC", 0);

        m_game->GetGuiManager()->GetGuiPtr("LOAD_CHECK_MMC")->menu->Reset();
        m_game->GetGuiManager()->SetActive("LOAD_CHECK_MMC", 1);
        m_game->GetGuiManager()->SetVisible("LOAD_CHECK_MMC", 1);
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
        m_game->GetGuiManager()->GetGuiPtr("LOAD_MMC_UNFORMAT")->menu->Reset();
        m_game->GetGuiManager()->SetActive("LOAD_MMC_UNFORMAT", 0);
        m_game->GetGuiManager()->SetVisible("LOAD_MMC_UNFORMAT", 0);
    } else if (str == "yes") {
        m_game->GetGuiManager()->GetGuiPtr("LOAD_MMC_UNFORMAT")->menu->Reset();
        m_game->GetGuiManager()->SetActive("LOAD_MMC_UNFORMAT", 0);
        m_game->GetGuiManager()->SetVisible("LOAD_MMC_UNFORMAT", 0);

        m_game->GetGuiManager()->GetGuiPtr("LOAD_CHECK_MMC")->menu->Reset();
        m_game->GetGuiManager()->SetActive("LOAD_CHECK_MMC", 1);
        m_game->GetGuiManager()->SetVisible("LOAD_CHECK_MMC", 1);
    }
}

CGuiChooseGameEventHandler::CGuiChooseGameEventHandler() : CGuiBaseEventHandler("GuiChooseGameEventHandler") {
    m_unk10.reserve(8);
}

void CGuiChooseGameEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    if (!m_unkC) {
        CGuiBaseEventHandler::OnEvent(menu, event, unk);
    }

    if (event == DKGUI::EVENT_2) {
        m_unkC = FALSE;
        m_unk24 = 1.0f;
    } else if (event == DKGUI::EVENT_0) {
        if (m_unk24 < 0.0f) {
            m_game->m_timer->Pause();
            U32 backup_state = m_game->GetBackupEngine()->GetState();
            m_game->m_timer->Resume();

            if (backup_state != 17) {
                m_game->GetGuiManager()->GetGuiPtr("LOAD_CHOOSE_GAME")->menu->Reset();
                m_game->GetGuiManager()->SetActive("LOAD_CHOOSE_GAME", 0);
                m_game->GetGuiManager()->SetVisible("LOAD_CHOOSE_GAME", 0);

                m_game->GetGuiManager()->GetGuiPtr("LOAD_CHECK_MMC")->menu->Reset();
                m_game->GetGuiManager()->SetActive("LOAD_CHECK_MMC", 1);
                m_game->GetGuiManager()->SetVisible("LOAD_CHECK_MMC", 1);
                return;
            }

            m_unk24 = 1.0f;
        } else {
            m_unk24 -= m_game->GetDeltaTime();
        }

        int iVar2 = m_unk1C - m_unk20;
        for (int i = 0; i < 4; i++) {
            std::string str;
            if (i + iVar2 < (int)m_unk10.size()) {
                str.assign(m_unk10[i + iVar2], 0);
                if (DKI::IInputEngine::GetDevice(0)->IsConnected() == TRUE) {
                    F32 text_width = m_game->GetGuiEngine()->GetTextWidth(str.c_str(), 0.07f, NULL);
                    if (m_unk1C == iVar2 + i) {
                        m_game->GetGuiEngine()->SetTextColor(0xFF, 0xFF, 0x99, 0xFF);
                    } else {
                        m_game->GetGuiEngine()->SetTextColor(0x99, 0x66, 0xCC, 0xFF);
                    }
                    m_game->GetGuiEngine()->AddText(0.5f - (text_width / 2), 0.48f + (0.0745f * i), str.c_str(), 0.07f, NULL, 0.0f);
                }
            }
        }

        char buf[32];
        sprintf(buf, "CHOOSE_GAME_%02d", m_unk20 + 1);
        menu->GetAnim()->GotoFrameLabel(buf);
    } else if (event == DKGUI::EVENT_13 && m_unkC == TRUE) {
        m_game->GetGuiManager()->GetGuiPtr("LOAD_CHOOSE_GAME")->menu->Reset();
        m_game->GetGuiManager()->SetActive("LOAD_CHOOSE_GAME", 0);
        m_game->GetGuiManager()->SetVisible("LOAD_CHOOSE_GAME", 0);

        PlaySoundInline("playsound SND_004_2C");
    } else if (event == DKGUI::EVENT_12 && m_unkC == TRUE) {
        if (m_unk1C >= 0 && m_unk1C < (int)m_unk10.size()) {
            m_game->GetGuiManager()->GetGuiPtr("LOAD_CHOOSE_GAME")->menu->Reset();
            m_game->GetGuiManager()->SetActive("LOAD_CHOOSE_GAME", 0);
            m_game->GetGuiManager()->SetVisible("LOAD_CHOOSE_GAME", 0);

            CGuiLoadingMemoryCardEventHandler* handler = (CGuiLoadingMemoryCardEventHandler*)m_game->GetGuiManager()->IsEventCallbackRegistered("GuiLoadingMemoryCardEventHandler");
            if (handler == NULL) {
                return;
            }
            handler->SetFilename(m_unk10[m_unk1C]);

            PlaySoundInline("playsound SND_003_2D");

            m_game->GetGuiManager()->GetGuiPtr("LOAD_LOAD_DATA")->menu->Reset();
            m_game->GetGuiManager()->SetActive("LOAD_LOAD_DATA", 1);
            m_game->GetGuiManager()->SetVisible("LOAD_LOAD_DATA", 1);
        }
    } else if (event == DKGUI::EVENT_8 && m_unkC == TRUE) {
        if (m_unk1C > 0) {
            PlaySoundInline("playsound SND_002_2D");

            m_unk1C--;
            if (m_unk20 > 0) {
                m_unk20--;
            }
        }
    } else if (event == DKGUI::EVENT_9 && m_unkC == TRUE) {
        if (m_unk1C < (int)m_unk10.size() - 1) {
            m_unk1C++;
            PlaySoundInline("playsound SND_002_2D");

            if (m_unk20 < 3) {
                if (m_unk1C % 4 >= m_unk20 + 1) {
                    m_unk20++;
                }
            }
        }
    } else if (event == DKGUI::EVENT_3) {
        std::string str = (char*)unk;
        if (str == "exit") {
            m_unkC = TRUE;
        }
    }
}

CGuiLoadingMemoryCardEventHandler::CGuiLoadingMemoryCardEventHandler() : CGuiBaseEventHandler("GuiLoadingMemoryCardEventHandler") {

}

// Fakematching?: branching
void CGuiLoadingMemoryCardEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    CGuiBaseEventHandler::OnEvent(menu, event, unk);

    if (event == DKGUI::EVENT_3) {
        std::string str = (char*)unk;
        if (str == "exit") {
            m_game->GetGuiManager()->GetGuiPtr("LOAD_LOAD_DATA")->menu->Reset();
            m_game->GetGuiManager()->SetActive("LOAD_LOAD_DATA", 0);
            m_game->GetGuiManager()->SetVisible("LOAD_LOAD_DATA", 0);

            m_game->m_timer->Pause();
            U32 backup_state = m_game->GetBackupEngine()->GetState();
            m_game->m_timer->Resume();
            if (backup_state == 17) {
                m_game->GetGameBackup()->CreateNewGame();

                CGuiEnterNameEventHandler* handler = (CGuiEnterNameEventHandler*)m_game->GetGuiManager()->IsEventCallbackRegistered("GuiEnterNameEventHandler");
                if (handler != NULL) {
                    handler->m_unk10 = 0;
                    handler->SetText(m_unk10);
                }

                int load_result = m_game->GetBackupEngine()->Load(m_game->GetGameBackup()->GetCurrentContainer()->m_unk4, 0x18000, "GPLE9G", m_unk10);
                switch (load_result) {
                    case -98:
                        m_game->GetGuiManager()->GetGuiPtr("LOAD_MMC_UNUSEABLE")->menu->Reset();
                        m_game->GetGuiManager()->SetActive("LOAD_MMC_UNUSEABLE", 1);
                        m_game->GetGuiManager()->SetVisible("LOAD_MMC_UNUSEABLE", 1);
                        return;
                    case -96:
                        m_game->GetGuiManager()->GetGuiPtr("LOAD_MMC_UNUSEABLE")->menu->Reset();
                        m_game->GetGuiManager()->SetActive("LOAD_MMC_UNUSEABLE", 1);
                        m_game->GetGuiManager()->SetVisible("LOAD_MMC_UNUSEABLE", 1);
                        return;
                    case -97:
                        m_game->GetGuiManager()->GetGuiPtr("LOAD_FILE_CORRUPT")->menu->Reset();
                        m_game->GetGuiManager()->SetActive("LOAD_FILE_CORRUPT", 1);
                        m_game->GetGuiManager()->SetVisible("LOAD_FILE_CORRUPT", 1);
                        return;
                    case -95:
                        m_game->GetGuiManager()->GetGuiPtr("LOAD_LOAD_FAIL")->menu->Reset();
                        m_game->GetGuiManager()->SetActive("LOAD_LOAD_FAIL", 1);
                        m_game->GetGuiManager()->SetVisible("LOAD_LOAD_FAIL", 1);
                        return;
                    default:
                        m_game->GetGuiManager()->GetGuiPtr("LOAD_LOAD_FAIL")->menu->Reset();
                        m_game->GetGuiManager()->SetActive("LOAD_LOAD_FAIL", 1);
                        m_game->GetGuiManager()->SetVisible("LOAD_LOAD_FAIL", 1);
                        return;
                    case 0:
                        goto case_0;
                }
            } else if (backup_state & (1 << 2)) {
                m_game->GetGuiManager()->GetGuiPtr("LOAD_MMC_CORRUPT")->menu->Reset();
                m_game->GetGuiManager()->SetActive("LOAD_MMC_CORRUPT", 1);
                m_game->GetGuiManager()->SetVisible("LOAD_MMC_CORRUPT", 1);
            } else if (backup_state & (1 << 3)) {
                m_game->GetGuiManager()->GetGuiPtr("LOAD_MMC_UNUSEABLE")->menu->Reset();
                m_game->GetGuiManager()->SetActive("LOAD_MMC_UNUSEABLE", 1);
                m_game->GetGuiManager()->SetVisible("LOAD_MMC_UNUSEABLE", 1);
            } else if (backup_state & (1 << 1)) {
                m_game->GetGuiManager()->GetGuiPtr("LOAD_WRONG_DEVICE")->menu->Reset();
                m_game->GetGuiManager()->SetActive("LOAD_WRONG_DEVICE", 1);
                m_game->GetGuiManager()->SetVisible("LOAD_WRONG_DEVICE", 1);
            } else if (!(backup_state & (1 << 0))) {
                m_game->GetGuiManager()->GetGuiPtr("LOAD_NO_MMC")->menu->Reset();
                m_game->GetGuiManager()->SetActive("LOAD_NO_MMC", 1);
                m_game->GetGuiManager()->SetVisible("LOAD_NO_MMC", 1);
                return;
            }
            return;
case_0:
            DKBAK::DKBAK_DATE date;
            if (m_game->GetBackupEngine()->GetDate("GPLE9G", m_unk10, date) == 0) {
                m_game->SetBackupDate(date);
            }

            m_game->GetGameBackup()->Backup();
            m_game->GetGameBackup()->ApplyToGameData();

            if (m_unk10 == "CHEATER") {
                m_game->GetMission(0).m_unk30 = 0;
                m_game->GetMission(1).m_unk30 = 0;
                m_game->GetMission(2).m_unk30 = 0;
                m_game->GetMission(3).m_unk30 = 0;
                m_game->GetMission(4).m_unk30 = 0;
                m_game->GetMission(5).m_unk30 = 0;
                m_game->GetMission(6).m_unk30 = 0;
                m_game->GetGameBackup()->GetFromGameData(1);
            }

            m_game->GetGuiManager()->GetGuiPtr("LOAD_LOAD_OK")->menu->Reset();
            m_game->GetGuiManager()->SetActive("LOAD_LOAD_OK", 1);
            m_game->GetGuiManager()->SetVisible("LOAD_LOAD_OK", 1);
        }
    }
}

CGuiLoadLoadSuccessEventHandler::CGuiLoadLoadSuccessEventHandler() : CGuiBaseEventHandler("GuiLoadLoadSuccessEventHandler") {

}

void CGuiLoadLoadSuccessEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    CGuiBaseEventHandler::OnEvent(menu, event, unk);

    if (event == DKGUI::EVENT_2) {
        m_unkC = FALSE;
        return;
    }

    if (m_unkC == TRUE) {
        // BUG: Redundant double-check
        if (m_unkC == TRUE) {
            m_game->GetGuiManager()->GetGuiPtr("LOAD_LOAD_OK")->menu->Reset();
            m_game->GetGuiManager()->SetActive("LOAD_LOAD_OK", 0);
            m_game->GetGuiManager()->SetVisible("LOAD_LOAD_OK", 0);

            CDKW_RGBA fade_color = m_game->ComputeGameFadeColor();
            m_game->FadeInit(1.0f, CGame::FADE_TYPE_4, fade_color.red, fade_color.green, fade_color.blue, 0.0f);
            ((CGamePartIngame*)m_game->GetGamePartPointer())->m_game_room_manager->m_flags |= (1 << 5);
            m_game->FadeIn(-1.0f);

            m_game->ResetOpcodeBuffer();
            m_game->PushOpcodeValue(1);
            m_game->PushOpcodeValue(3);
            m_game->PushOpcodeValue(0);
            m_game->PushOpcodeValue(0);
            m_game->PushOpcodeValue(0);
            m_game->PushOpcodeValue(25);
            m_game->PushOpcodeValue(0);
            m_game->PushOpcodeValue(0);
            m_game->PushOpcodeValue(0);
            m_game->PushOpcodeValue(1);

            UnkGamePartAndReturnTypeInline();
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

CGuiLoadLoadFailEventHandler::CGuiLoadLoadFailEventHandler() : CGuiBaseEventHandler("GuiLoadLoadFailEventHandler") {

}

void CGuiLoadLoadFailEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    CGuiBaseEventHandler::OnEvent(menu, event, unk);

    if (event == DKGUI::EVENT_2) {
        gs_TimeBeforeMemCardCheck = 1.0f;
    } else if (event == DKGUI::EVENT_0) {
        if (gs_TimeBeforeMemCardCheck < 0.0f) {
            m_game->m_timer->Pause();
            U32 backup_state = m_game->GetBackupEngine()->GetState();
            m_game->m_timer->Resume();

            if (!(backup_state & (1 << 0))) {
                m_game->GetGuiManager()->GetGuiPtr("LOAD_LOAD_FAIL")->menu->Reset();
                m_game->GetGuiManager()->SetActive("LOAD_LOAD_FAIL", 0);
                m_game->GetGuiManager()->SetVisible("LOAD_LOAD_FAIL", 0);

                m_game->GetGuiManager()->GetGuiPtr("LOAD_CHECK_MMC")->menu->Reset();
                m_game->GetGuiManager()->SetActive("LOAD_CHECK_MMC", 1);
                m_game->GetGuiManager()->SetVisible("LOAD_CHECK_MMC", 1);
            } else {
                gs_TimeBeforeMemCardCheck = 1.0f;
            }
        } else {
            gs_TimeBeforeMemCardCheck -= m_game->GetDeltaTime();
        }
    } else if (event == DKGUI::EVENT_3) {
        std::string str = (char*)unk;
        if (str == "yes") {
            m_game->GetGuiManager()->GetGuiPtr("LOAD_LOAD_FAIL")->menu->Reset();
            m_game->GetGuiManager()->SetActive("LOAD_LOAD_FAIL", 0);
            m_game->GetGuiManager()->SetVisible("LOAD_LOAD_FAIL", 0);

            m_game->GetGuiManager()->GetGuiPtr("LOAD_CHECK_MMC")->menu->Reset();
            m_game->GetGuiManager()->SetActive("LOAD_CHECK_MMC", 1);
            m_game->GetGuiManager()->SetVisible("LOAD_CHECK_MMC", 1);
        } else if (str == "no") {
            m_game->GetGuiManager()->GetGuiPtr("LOAD_LOAD_FAIL")->menu->Reset();
            m_game->GetGuiManager()->SetActive("LOAD_LOAD_FAIL", 0);
            m_game->GetGuiManager()->SetVisible("LOAD_LOAD_FAIL", 0);
        }
    }
}

CGuiLoadMemoryCardUnuseableEventHandler::CGuiLoadMemoryCardUnuseableEventHandler() : CGuiBaseEventHandler("GuiLoadMemoryCardUnuseableEventHandler") {

}

void CGuiLoadMemoryCardUnuseableEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    CGuiBaseEventHandler::OnEvent(menu, event, unk);

    if (event == DKGUI::EVENT_2) {
        gs_TimeBeforeMemCardCheck = 1.0f;
    } else if (event == DKGUI::EVENT_0) {
        if (gs_TimeBeforeMemCardCheck < 0.0f) {
            m_game->m_timer->Pause();
            U32 backup_state = m_game->GetBackupEngine()->GetState();
            m_game->m_timer->Resume();

            if (!(backup_state & (1 << 0))) {
                m_game->GetGuiManager()->GetGuiPtr("LOAD_MMC_UNUSEABLE")->menu->Reset();
                m_game->GetGuiManager()->SetActive("LOAD_MMC_UNUSEABLE", 0);
                m_game->GetGuiManager()->SetVisible("LOAD_MMC_UNUSEABLE", 0);

                m_game->GetGuiManager()->GetGuiPtr("LOAD_CHECK_MMC")->menu->Reset();
                m_game->GetGuiManager()->SetActive("LOAD_CHECK_MMC", 1);
                m_game->GetGuiManager()->SetVisible("LOAD_CHECK_MMC", 1);
            } else {
                gs_TimeBeforeMemCardCheck = 1.0f;
            }
        } else {
            gs_TimeBeforeMemCardCheck -= m_game->GetDeltaTime();
        }
    } else if (event == DKGUI::EVENT_3) {
        std::string str = (char*)unk;
        if (str == "yes") {
            m_game->GetGuiManager()->GetGuiPtr("LOAD_MMC_UNUSEABLE")->menu->Reset();
            m_game->GetGuiManager()->SetActive("LOAD_MMC_UNUSEABLE", 0);
            m_game->GetGuiManager()->SetVisible("LOAD_MMC_UNUSEABLE", 0);

            m_game->GetGuiManager()->GetGuiPtr("LOAD_CHECK_MMC")->menu->Reset();
            m_game->GetGuiManager()->SetActive("LOAD_CHECK_MMC", 1);
            m_game->GetGuiManager()->SetVisible("LOAD_CHECK_MMC", 1);
        } else if (str == "no") {
            m_game->GetGuiManager()->GetGuiPtr("LOAD_MMC_UNUSEABLE")->menu->Reset();
            m_game->GetGuiManager()->SetActive("LOAD_MMC_UNUSEABLE", 0);
            m_game->GetGuiManager()->SetVisible("LOAD_MMC_UNUSEABLE", 0);
        }
    }
}

CGuiCreateNewGameEventHandler::CGuiCreateNewGameEventHandler() : CGuiBaseEventHandler("GuiCreateNewGameEventHandler") {

}

void CGuiCreateNewGameEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    CGuiBaseEventHandler::OnEvent(menu, event, unk);

    if (event == DKGUI::EVENT_3) {
        std::string str = (char*)unk;
        if (str == "yes") {
            m_game->GetGuiManager()->GetGuiPtr("CREATE_CREATE_GAME")->menu->Reset();
            m_game->GetGuiManager()->SetActive("CREATE_CREATE_GAME", 0);
            m_game->GetGuiManager()->SetVisible("CREATE_CREATE_GAME", 0);

            m_game->GetGuiManager()->GetGuiPtr("CREATE_ENTER_NAME")->menu->Reset();
            m_game->GetGuiManager()->SetActive("CREATE_ENTER_NAME", 1);
            m_game->GetGuiManager()->SetVisible("CREATE_ENTER_NAME", 1);

            CGuiEnterNameEventHandler* handler = (CGuiEnterNameEventHandler*)m_game->GetGuiManager()->IsEventCallbackRegistered("GuiEnterNameEventHandler");
            if (handler != NULL) {
                handler->m_unk10 = 0;
                handler->SetText("");
            }
        } else if (str == "no") {
            m_game->GetGuiManager()->GetGuiPtr("CREATE_CREATE_GAME")->menu->Reset();
            m_game->GetGuiManager()->SetActive("CREATE_CREATE_GAME", 0);
            m_game->GetGuiManager()->SetVisible("CREATE_CREATE_GAME", 0);
        }
    }
}

CGuiEnterNameEventHandler::CGuiEnterNameEventHandler() : CGuiBaseEventHandler("GuiEnterNameEventHandler") {

}

void CGuiEnterNameEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    CGuiBaseEventHandler::OnEvent(menu, event, unk);

    int mod, div;

    div = m_unk10 / 7;
    mod = m_unk10 % 7;
    if (event == DKGUI::EVENT_0) {
        if (DKI::IInputEngine::GetDevice(0)->IsConnected() == TRUE) {
            F32 text_width = m_game->GetGuiEngine()->GetTextWidth(m_unk14.c_str(), 0.07f, NULL);
            m_game->GetGuiEngine()->SetTextColor(0xC4, 0xE1, 0xFF, 0xFF);
            m_game->GetGuiEngine()->AddText(0.5f - (text_width / 2), 0.35f, m_unk14.c_str(), 0.07f, NULL, 0.0f);
        }
    } else if (event == DKGUI::EVENT_12) {
        if (m_unk10 == 26) {
            if (m_unk14.size() == 0) {
                return;
            }
            m_unk14.resize(m_unk14.size() - 1, '\0');
            PlaySoundInline("playsound SND_004_2C");
        } else if (m_unk10 == 27) {
            if (m_unk14.size() != 0) {
                PlaySoundInline("playsound SND_003_2D");

                m_game->GetGameBackup()->CreateNewGame();

                // Unlock all levels if the entered name is "CHEATER".
                if (m_unk14 == "CHEATER") {
                    m_game->GetMission(0).m_unk30 = 0;
                    m_game->GetMission(1).m_unk30 = 0;
                    m_game->GetMission(2).m_unk30 = 0;
                    m_game->GetMission(3).m_unk30 = 0;
                    m_game->GetMission(4).m_unk30 = 0;
                    m_game->GetMission(5).m_unk30 = 0;
                    m_game->GetMission(6).m_unk30 = 0;
                    m_game->GetGameBackup()->GetFromGameData(1);
                }
                m_game->GetGameBackup()->Backup();

                m_game->GetGuiManager()->GetGuiPtr("CREATE_ENTER_NAME")->menu->Reset();
                m_game->GetGuiManager()->SetActive("CREATE_ENTER_NAME", 0);
                m_game->GetGuiManager()->SetVisible("CREATE_ENTER_NAME", 0);

                m_game->GetGuiManager()->GetGuiPtr("SAVE_CHECK_MMC")->menu->Reset();
                m_game->GetGuiManager()->SetActive("SAVE_CHECK_MMC", 1);
                m_game->GetGuiManager()->SetVisible("SAVE_CHECK_MMC", 1);
            }
        } else if (m_unk14.size() < 8) {
            m_unk14.append(1, 'A' + m_unk10);
            if (m_unk14.size() == 8) {
                m_unk10 = 27;
                FrameLabelInline(menu);
            }
            PlaySoundInline("playsound SND_003_2D");
        } else {
            FrameLabelInline(menu);
        }
    } else if (event == DKGUI::EVENT_13) {
        m_game->GetGuiManager()->GetGuiPtr("CREATE_ENTER_NAME")->menu->Reset();
        m_game->GetGuiManager()->SetActive("CREATE_ENTER_NAME", 0);
        m_game->GetGuiManager()->SetVisible("CREATE_ENTER_NAME", 0);

        PlaySoundInline("playsound SND_004_2C");
    } else if (event == DKGUI::EVENT_11) {
        if (m_unk14.size() == 8) {
            if (m_unk10 == 26) {
                m_unk10 = 27;
                FrameLabelInline(menu);
            } else {
                m_unk10 = 26;
                FrameLabelInline(menu);
            }
        } else {
            mod++;
            if (mod > 6) {
                mod = 0;
            }
            m_unk10 = mod + div * 7;
            FrameLabelInline(menu);
        }
    } else if (event == DKGUI::EVENT_10) {
        if (m_unk14.size() == 8) {
            if (m_unk10 == 26) {
                m_unk10 = 27;
                FrameLabelInline(menu);
            } else {
                m_unk10 = 26;
                FrameLabelInline(menu);
            }
        } else {
            mod--;
            if (mod < 0) {
                mod = 6;
            }
            m_unk10 = mod + div * 7;
            FrameLabelInline(menu);
        }
    } else if (event == DKGUI::EVENT_8) {
        if (m_unk14.size() != 8) {
            div--;
            if (div < 0) {
                div = 3;
            }
            m_unk10 = mod + div * 7;
            FrameLabelInline(menu);
        } else {
            FrameLabelInline(menu);
        }
    } else if (event == DKGUI::EVENT_9) {
        if (m_unk14.size() != 8) {
            div++;
            if (div > 3) {
                div = 0;
            }
            m_unk10 = mod + div * 7;
            FrameLabelInline(menu);
        } else {
            FrameLabelInline(menu);
        }
    }
}

CGuiSaveCheckingMemorycardEventHandler::CGuiSaveCheckingMemorycardEventHandler() : CGuiBaseEventHandler("GuiSaveCheckingMemorycardEventHandler") {
    m_unk10 = 0;
}

// Incomplete
void CGuiSaveCheckingMemorycardEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    CGuiBaseEventHandler::OnEvent(menu, event, unk);

    if (event == DKGUI::EVENT_3) {
        std::string str = (char*)unk;
        std::string str2;
        if (str == "exit") {
            m_game->m_timer->Pause();
            U32 backup_state = m_game->GetBackupEngine()->GetState();
            m_game->m_timer->Resume();

            m_game->m_timer->Pause();
            m_game->GetGuiManager()->GetGuiPtr("SAVE_CHECK_MMC")->menu->Reset();
            m_game->GetGuiManager()->SetActive("SAVE_CHECK_MMC", 0);
            m_game->GetGuiManager()->SetVisible("SAVE_CHECK_MMC", 0);

            if (backup_state == 17) {
                CGuiEnterNameEventHandler* handler = (CGuiEnterNameEventHandler*)m_game->GetGuiManager()->IsEventCallbackRegistered("GuiEnterNameEventHandler");

                str2.assign(m_game->GetBackupEngine()->FindFirst("GPLE9G", handler->GetText()), 0);

                if (str2 == handler->GetText() && m_game->m_unk4F54 == 8 && m_game->GetUnk4F58() == 1) {
                    m_game->GetGuiManager()->GetGuiPtr("CREATE_OVERWRITE")->menu->Reset();
                    m_game->GetGuiManager()->SetActive("CREATE_OVERWRITE", 1);
                    m_game->GetGuiManager()->SetVisible("CREATE_OVERWRITE", 1);
                    m_unk10 = 0;
                } else if (str2 != handler->GetText()) {
                    if (m_game->GetBackupEngine()->IsSpaceAvailable("GPLE9G", handler->GetText(), 0x18000) == FALSE) {
                        m_game->GetGuiManager()->GetGuiPtr("SAVE_NO_SPACE")->menu->Reset();
                        m_game->GetGuiManager()->SetActive("SAVE_NO_SPACE", 1);
                        m_game->GetGuiManager()->SetVisible("SAVE_NO_SPACE", 1);
                        m_unk10 = 0;
                    }
                } else {
                    DKBAK::DKBAK_DATE date;
                    if (m_game->GetBackupEngine()->GetDate("GPLE9G", handler->GetText(), date) == 0) {
                        m_game->SetBackupDate(date);
                        if (TRUE) {
                        // if (date == m_game->GetBackupDate()) {
                            m_game->GetGuiManager()->GetGuiPtr("CREATE_OVERWRITE")->menu->Reset();
                            m_game->GetGuiManager()->SetActive("CREATE_OVERWRITE", 1);
                            m_game->GetGuiManager()->SetVisible("CREATE_OVERWRITE", 1);
                            m_unk10 = 0;

                            m_game->m_timer->Resume();
                            m_game->GetGameBackup()->GetFromGameData(1);
                            m_game->GetGameBackup()->Backup();

                            return;
                        }
                    }

                    m_game->GetGuiManager()->GetGuiPtr("SAVE_SAVE_DATA")->menu->Reset();
                    m_game->GetGuiManager()->SetActive("SAVE_SAVE_DATA", 1);
                    m_game->GetGuiManager()->SetVisible("SAVE_SAVE_DATA", 1);

                    m_game->GetGameBackup()->GetFromGameData(1);
                    m_game->GetGameBackup()->Backup();
                }
            } else if (backup_state & (1 << 2)) {
                m_game->GetGuiManager()->GetGuiPtr("SAVE_MMC_CORRUPT")->menu->Reset();
                m_game->GetGuiManager()->SetActive("SAVE_MMC_CORRUPT", 1);
                m_game->GetGuiManager()->SetVisible("SAVE_MMC_CORRUPT", 1);
                m_unk10 = 0;
            } else if (backup_state & (1 << 3)) {
                m_game->GetGuiManager()->GetGuiPtr("SAVE_MMC_UNUSEABLE")->menu->Reset();
                m_game->GetGuiManager()->SetActive("SAVE_MMC_UNUSEABLE", 1);
                m_game->GetGuiManager()->SetVisible("SAVE_MMC_UNUSEABLE", 1);
                m_unk10 = 0;
            } else if (backup_state & (1 << 1)) {
                m_game->GetGuiManager()->GetGuiPtr("SAVE_WRONG_DEVICE")->menu->Reset();
                m_game->GetGuiManager()->SetActive("SAVE_WRONG_DEVICE", 1);
                m_game->GetGuiManager()->SetVisible("SAVE_WRONG_DEVICE", 1);
                m_unk10 = 0;
            } else if (!(backup_state & (1 << 0))) {
                m_game->GetGuiManager()->GetGuiPtr("SAVE_NO_MMC")->menu->Reset();
                m_game->GetGuiManager()->SetActive("SAVE_NO_MMC", 1);
                m_game->GetGuiManager()->SetVisible("SAVE_NO_MMC", 1);
                m_unk10 = 0;
            } else if (!(backup_state & (1 << 4))) {
                m_game->GetGuiManager()->GetGuiPtr("SAVE_UNFORMAT")->menu->Reset();
                m_game->GetGuiManager()->SetActive("SAVE_UNFORMAT", 1);
                m_game->GetGuiManager()->SetVisible("SAVE_UNFORMAT", 1);
                m_unk10 = 0;
            }

            m_game->m_timer->Resume();
        }
    }
}

CGuiSaveNoSpaceEventHandler::CGuiSaveNoSpaceEventHandler() : CGuiBaseEventHandler("GuiSaveNoSpaceEventHandler") {

}

// Equivalent: regalloc
void CGuiSaveNoSpaceEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    CGuiBaseEventHandler::OnEvent(menu, event, unk);

    if (event == DKGUI::EVENT_2) {
        if (m_game->m_unk4F54 != 8) {
            menu->GetAnim()->GotoFrameLabel("NO_SPACE_INGAME");
            menu->GetAnim()->Play();
        }
        m_unkC = FALSE;
        gs_TimeBeforeMemCardCheck = 1.0f;
    } else if (event == DKGUI::EVENT_0) {
        if (gs_TimeBeforeMemCardCheck < 0.0f) {
            m_game->m_timer->Pause();
            U32 backup_state = m_game->GetBackupEngine()->GetState();
            m_game->m_timer->Resume();

            if (!(backup_state & (1 << 0))) {
                m_game->GetGuiManager()->GetGuiPtr("SAVE_NO_SPACE")->menu->Reset();
                m_game->GetGuiManager()->SetActive("SAVE_NO_SPACE", 0);
                m_game->GetGuiManager()->SetVisible("SAVE_NO_SPACE", 0);

                m_game->GetGuiManager()->GetGuiPtr("SAVE_CHECK_MMC")->menu->Reset();
                m_game->GetGuiManager()->SetActive("SAVE_CHECK_MMC", 1);
                m_game->GetGuiManager()->SetVisible("SAVE_CHECK_MMC", 1);
            } else {
                gs_TimeBeforeMemCardCheck = 1.0f;
            }
        } else {
            gs_TimeBeforeMemCardCheck -= m_game->GetDeltaTime();
        }
    } else if (event == DKGUI::EVENT_3) {
        std::string str = (char*)unk;
        if (str == "yes") {
            m_game->GetGuiManager()->GetGuiPtr("SAVE_NO_SPACE")->menu->Reset();
            m_game->GetGuiManager()->SetActive("SAVE_NO_SPACE", 0);
            m_game->GetGuiManager()->SetVisible("SAVE_NO_SPACE", 0);

            m_game->GetGuiManager()->GetGuiPtr("SAVE_CHECK_MMC")->menu->Reset();
            m_game->GetGuiManager()->SetActive("SAVE_CHECK_MMC", 1);
            m_game->GetGuiManager()->SetVisible("SAVE_CHECK_MMC", 1);
        } else if (str == "no") {
            m_game->GetGuiManager()->GetGuiPtr("SAVE_NO_SPACE")->menu->Reset();
            m_game->GetGuiManager()->SetActive("SAVE_NO_SPACE", 0);
            m_game->GetGuiManager()->SetVisible("SAVE_NO_SPACE", 0);

            if (m_game->m_unk4F54 == 8 && m_game->m_unk4F58 == 1) {
                CDKW_RGBA fade_color = m_game->ComputeGameFadeColor();
                m_game->FadeInit(1.0f, CGame::FADE_TYPE_4, fade_color.red, fade_color.green, fade_color.blue, 0.0f);
                ((CGamePartIngame*)m_game->GetGamePartPointer())->m_game_room_manager->m_flags |= (1 << 5);
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
        } else if (str == "MMC") {
            DKI::IInputEngine::GetDevice(0)->StopVibration();
            DKI::IInputEngine::Update(1.0f/30.0f);

            while (!PADRecalibrate(PAD_CHAN0_BIT)) {
                PADStatus status[4];
                do {
                    PADReset(PAD_CHAN0_BIT);
                    PADRead(&status[0]);
                } while (status[0].err == PAD_ERR_NO_CONTROLLER);
            }

            if (DkSoundGetEngine() != NULL) {
                DkSoundGetEngine()->StopStreamedSound();
            }

            if (DkDisplayGetEngine() != NULL) {
                DKDSP::CScene* scene = DkDisplayGetEngine()->GetScene(0);
                if (scene != NULL) {
                    scene->Clear(3, 0.0f, 0.0f, 0.0f);
                    scene->BeginRender();
                    scene->EndRender();
                    scene->Flip(0);

                    scene->Clear(3, 0.0f, 0.0f, 0.0f);
                    scene->BeginRender();
                    scene->EndRender();
                    scene->Flip(0);
                }
            }

            GXDrawDone();
            VISetBlack(TRUE);
            VIFlush();
            VIWaitForRetrace();

            OSResetSystem(TRUE, 0, TRUE);
        }
    }
}

CGuiSavingEventHandler::CGuiSavingEventHandler() : CGuiBaseEventHandler("GuiSavingEventHandler") {

}

// Equivalent?: regalloc
void CGuiSavingEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    CGuiBaseEventHandler::OnEvent(menu, event, unk);

    if (event == DKGUI::EVENT_2) {
        CGuiSaveCheckingMemorycardEventHandler* handler = (CGuiSaveCheckingMemorycardEventHandler*)m_game->GetGuiManager()->IsEventCallbackRegistered("GuiSaveCheckingMemorycardEventHandler");
        if (handler == NULL) {
            return;
        }
        if (handler->GetUnk10() == 0) {
            return;
        }

        menu->GetAnim()->GotoFrameLabel("AUTOSAVE_DATA");
        menu->GetAnim()->Play();
        handler->SetUnk10(0);
    } else if (event == DKGUI::EVENT_3) {
        std::string str = (char*)unk;
        std::string unused;
        if (str == "exit") {
            m_game->m_timer->Pause();
            CGuiEnterNameEventHandler* handler = (CGuiEnterNameEventHandler*)m_game->GetGuiManager()->IsEventCallbackRegistered("GuiEnterNameEventHandler");

            int save_result = m_game->GetBackupEngine()->Save(m_game->GetGameBackup()->GetCurrentContainer()->m_unk4, 0x18000, "GPLE9G", handler->GetText());
            m_game->m_timer->Resume();

            m_game->GetGuiManager()->GetGuiPtr("SAVE_SAVE_DATA")->menu->Reset();
            m_game->GetGuiManager()->SetActive("SAVE_SAVE_DATA", 0);
            m_game->GetGuiManager()->SetVisible("SAVE_SAVE_DATA", 0);

            switch (save_result) {
                case -98:
                    m_game->GetGuiManager()->GetGuiPtr("SAVE_MMC_UNUSEABLE")->menu->Reset();
                    m_game->GetGuiManager()->SetActive("SAVE_MMC_UNUSEABLE", 1);
                    m_game->GetGuiManager()->SetVisible("SAVE_MMC_UNUSEABLE", 1);
                    return;
                case -96:
                    m_game->GetGuiManager()->GetGuiPtr("SAVE_MMC_UNUSEABLE")->menu->Reset();
                    m_game->GetGuiManager()->SetActive("SAVE_MMC_UNUSEABLE", 1);
                    m_game->GetGuiManager()->SetVisible("SAVE_MMC_UNUSEABLE", 1);
                    return;
                case -97:
                    m_game->GetGuiManager()->GetGuiPtr("SAVE_SAVE_FAILED")->menu->Reset();
                    m_game->GetGuiManager()->SetActive("SAVE_SAVE_FAILED", 1);
                    m_game->GetGuiManager()->SetVisible("SAVE_SAVE_FAILED", 1);
                    return;
                case -95:
                    m_game->GetGuiManager()->GetGuiPtr("SAVE_SAVE_FAILED")->menu->Reset();
                    m_game->GetGuiManager()->SetActive("SAVE_SAVE_FAILED", 1);
                    m_game->GetGuiManager()->SetVisible("SAVE_SAVE_FAILED", 1);
                    return;
                default: {
                    m_game->m_timer->Pause();
                    U32 backup_state = m_game->GetBackupEngine()->GetState();
                    // why
                    if (m_game->GetBackupEngine()->GetState() == 8) {
                        m_game->m_timer->Resume();
                        m_game->GetGuiManager()->GetGuiPtr("SAVE_MMC_UNUSEABLE")->menu->Reset();
                        m_game->GetGuiManager()->SetActive("SAVE_MMC_UNUSEABLE", 1);
                        m_game->GetGuiManager()->SetVisible("SAVE_MMC_UNUSEABLE", 1);
                        return;
                    } else {
                        m_game->m_timer->Resume();
                        m_game->GetGuiManager()->GetGuiPtr("SAVE_SAVE_FAILED")->menu->Reset();
                        m_game->GetGuiManager()->SetActive("SAVE_SAVE_FAILED", 1);
                        m_game->GetGuiManager()->SetVisible("SAVE_SAVE_FAILED", 1);
                        return;
                    }
                    break;
                }
                case 0:
                    m_game->m_timer->Pause();
                    DKBAK::DKBAK_DATE date;
                    if (m_game->GetBackupEngine()->GetDate("GPLE9G", handler->GetText(), date) == 0) {
                        m_game->SetBackupDate(date);
                    }
                    m_game->m_timer->Resume();

                    m_game->GetGuiManager()->GetGuiPtr("SAVE_SAVE_OK")->menu->Reset();
                    m_game->GetGuiManager()->SetActive("SAVE_SAVE_OK", 1);
                    m_game->GetGuiManager()->SetVisible("SAVE_SAVE_OK", 1);
                    break;
            }
        }
    }
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
            U32 backup_state = m_game->GetBackupEngine()->GetState();
            m_game->m_timer->Resume();

            if (!(backup_state & (1 << 0))) {
                m_game->GetGuiManager()->GetGuiPtr("SAVE_SAVE_FAILED")->menu->Reset();
                m_game->GetGuiManager()->SetActive("SAVE_SAVE_FAILED", 0);
                m_game->GetGuiManager()->SetVisible("SAVE_SAVE_FAILED", 0);

                m_game->GetGuiManager()->GetGuiPtr("SAVE_CHECK_MMC")->menu->Reset();
                m_game->GetGuiManager()->SetActive("SAVE_CHECK_MMC", 1);
                m_game->GetGuiManager()->SetVisible("SAVE_CHECK_MMC", 1);
            } else {
                gs_TimeBeforeMemCardCheck = 1.0f;
            }
        } else {
            gs_TimeBeforeMemCardCheck -= m_game->GetDeltaTime();
        }
    } else if (event == DKGUI::EVENT_3) {
        std::string str = (char*)unk;
        if (str == "yes") {
            m_game->GetGuiManager()->GetGuiPtr("SAVE_SAVE_FAILED")->menu->Reset();
            m_game->GetGuiManager()->SetActive("SAVE_SAVE_FAILED", 0);
            m_game->GetGuiManager()->SetVisible("SAVE_SAVE_FAILED", 0);

            m_game->GetGuiManager()->GetGuiPtr("SAVE_CHECK_MMC")->menu->Reset();
            m_game->GetGuiManager()->SetActive("SAVE_CHECK_MMC", 1);
            m_game->GetGuiManager()->SetVisible("SAVE_CHECK_MMC", 1);
        } else if (str == "no") {
            m_game->GetGuiManager()->GetGuiPtr("SAVE_SAVE_FAILED")->menu->Reset();
            m_game->GetGuiManager()->SetActive("SAVE_SAVE_FAILED", 0);
            m_game->GetGuiManager()->SetVisible("SAVE_SAVE_FAILED", 0);

            if (m_game->m_unk4F54 == 8 && m_game->m_unk4F58 == 1) {
                CDKW_RGBA fade_color = m_game->ComputeGameFadeColor();
                m_game->FadeInit(1.0f, CGame::FADE_TYPE_4, fade_color.red, fade_color.green, fade_color.blue, 0.0f);
                ((CGamePartIngame*)m_game->GetGamePartPointer())->m_game_room_manager->m_flags |= (1 << 5);
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
        m_game->GetGuiManager()->GetGuiPtr("SAVE_SAVE_READY")->menu->Reset();
        m_game->GetGuiManager()->SetActive("SAVE_SAVE_READY", 0);
        m_game->GetGuiManager()->SetVisible("SAVE_SAVE_READY", 0);
    } else if (str == "yes") {
        m_game->GetGuiManager()->GetGuiPtr("SAVE_SAVE_READY")->menu->Reset();
        m_game->GetGuiManager()->SetActive("SAVE_SAVE_READY", 0);
        m_game->GetGuiManager()->SetVisible("SAVE_SAVE_READY", 0);

        m_game->GetGuiManager()->GetGuiPtr("SAVE_CHECK_MMC")->menu->Reset();
        m_game->GetGuiManager()->SetActive("SAVE_CHECK_MMC", 1);
        m_game->GetGuiManager()->SetVisible("SAVE_CHECK_MMC", 1);
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
            U32 backup_state = m_game->GetBackupEngine()->GetState();
            m_game->m_timer->Resume();

            if (!(backup_state & (1 << 0))) {
                m_game->GetGuiManager()->GetGuiPtr("LOAD_FILE_CORRUPT")->menu->Reset();
                m_game->GetGuiManager()->SetActive("LOAD_FILE_CORRUPT", 0);
                m_game->GetGuiManager()->SetVisible("LOAD_FILE_CORRUPT", 0);

                m_game->GetGuiManager()->GetGuiPtr("LOAD_CHECK_MMC")->menu->Reset();
                m_game->GetGuiManager()->SetActive("LOAD_CHECK_MMC", 1);
                m_game->GetGuiManager()->SetVisible("LOAD_CHECK_MMC", 1);
            } else {
                gs_TimeBeforeMemCardCheck = 1.0f;
            }
        } else {
            gs_TimeBeforeMemCardCheck -= m_game->GetDeltaTime();
        }
    } else if (event == DKGUI::EVENT_3) {
        std::string str = (char*)unk;
        if (str == "yes") {
            m_game->GetGuiManager()->GetGuiPtr("LOAD_FILE_CORRUPT")->menu->Reset();
            m_game->GetGuiManager()->SetActive("LOAD_FILE_CORRUPT", 0);
            m_game->GetGuiManager()->SetVisible("LOAD_FILE_CORRUPT", 0);

            m_game->GetGuiManager()->GetGuiPtr("LOAD_LOAD_DATA")->menu->Reset();
            m_game->GetGuiManager()->SetActive("LOAD_LOAD_DATA", 1);
            m_game->GetGuiManager()->SetVisible("LOAD_LOAD_DATA", 1);
        } else if (str == "no") {
            m_game->GetGuiManager()->GetGuiPtr("LOAD_FILE_CORRUPT")->menu->Reset();
            m_game->GetGuiManager()->SetActive("LOAD_FILE_CORRUPT", 0);
            m_game->GetGuiManager()->SetVisible("LOAD_FILE_CORRUPT", 0);
        } else if (str == "delete") {
            m_game->GetGuiManager()->GetGuiPtr("LOAD_FILE_CORRUPT")->menu->Reset();
            m_game->GetGuiManager()->SetActive("LOAD_FILE_CORRUPT", 0);
            m_game->GetGuiManager()->SetVisible("LOAD_FILE_CORRUPT", 0);

            CGuiLoadingMemoryCardEventHandler* handler = (CGuiLoadingMemoryCardEventHandler*)m_game->GetGuiManager()->IsEventCallbackRegistered("GuiLoadingMemoryCardEventHandler");
            if (handler != NULL) {
                // Weird negate-or-rightshift going on here. Should just move.
                if (handler->GetFilename() != "") {
                    if (m_game->GetBackupEngine()->Delete("GPLE9G", handler->GetFilename())) {
                        m_game->m_timer->Pause();

                        if (m_game->GetBackupEngine()->GetState() == 8) {
                            m_game->m_timer->Resume();

                            m_game->GetGuiManager()->GetGuiPtr("LOAD_MMC_UNUSEABLE")->menu->Reset();
                            m_game->GetGuiManager()->SetActive("LOAD_MMC_UNUSEABLE", 1);
                            m_game->GetGuiManager()->SetVisible("LOAD_MMC_UNUSEABLE", 1);
                        } else {
                            m_game->m_timer->Resume();

                            m_game->GetGuiManager()->GetGuiPtr("LOAD_FILE_DELETEFAIL")->menu->Reset();
                            m_game->GetGuiManager()->SetActive("LOAD_FILE_DELETEFAIL", 1);
                            m_game->GetGuiManager()->SetVisible("LOAD_FILE_DELETEFAIL", 1);
                        }
                    } else {
                        m_game->GetGuiManager()->GetGuiPtr("LOAD_CHECK_MMC")->menu->Reset();
                        m_game->GetGuiManager()->SetActive("LOAD_CHECK_MMC", 1);
                        m_game->GetGuiManager()->SetVisible("LOAD_CHECK_MMC", 1);
                    }
                }
            }
        }
    }
}

CGuiLoadFileDeletingEventHandler::CGuiLoadFileDeletingEventHandler() : CGuiBaseEventHandler("GuiLoadFileDeletingEventHandler") {

}

void CGuiLoadFileDeletingEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    CGuiBaseEventHandler::OnEvent(menu, event, unk);

    if (event == DKGUI::EVENT_0) {
        m_game->m_timer->Pause();
        U32 state = m_game->GetBackupEngine()->GetState();
        m_game->m_timer->Resume();

        if (state != 17) {
            m_game->GetGuiManager()->GetGuiPtr("LOAD_FILE_DELETING")->menu->Reset();
            m_game->GetGuiManager()->SetActive("LOAD_FILE_DELETING", 0);
            m_game->GetGuiManager()->SetVisible("LOAD_FILE_DELETING", 0);

            m_game->GetGuiManager()->GetGuiPtr("LOAD_CHECK_MMC")->menu->Reset();
            m_game->GetGuiManager()->SetActive("LOAD_CHECK_MMC", 1);
            m_game->GetGuiManager()->SetVisible("LOAD_CHECK_MMC", 1);
        }
    } else if (event == DKGUI::EVENT_3) {
        std::string str = (char*)unk;
        if (str == "exit") {
            m_game->GetGuiManager()->GetGuiPtr("LOAD_FILE_DELETING")->menu->Reset();
            m_game->GetGuiManager()->SetActive("LOAD_FILE_DELETING", 0);
            m_game->GetGuiManager()->SetVisible("LOAD_FILE_DELETING", 0);

            CGuiLoadingMemoryCardEventHandler* handler = (CGuiLoadingMemoryCardEventHandler*)m_game->GetGuiManager()->IsEventCallbackRegistered("GuiLoadingMemoryCardEventHandler");
            if (handler && handler->GetFilename() != "") {
                if (m_game->GetBackupEngine()->Delete("GPLE9G", handler->GetFilename())) {
                    m_game->m_timer->Pause();
                    if (m_game->GetBackupEngine()->GetState() == 8) {
                        m_game->m_timer->Resume();
                        m_game->GetGuiManager()->GetGuiPtr("LOAD_MMC_UNUSEABLE")->menu->Reset();
                        m_game->GetGuiManager()->SetActive("LOAD_MMC_UNUSEABLE", 1);
                        m_game->GetGuiManager()->SetVisible("LOAD_MMC_UNUSEABLE", 1);
                    } else {
                        m_game->m_timer->Resume();
                        m_game->GetGuiManager()->GetGuiPtr("LOAD_FILE_DELETEFAIL")->menu->Reset();
                        m_game->GetGuiManager()->SetActive("LOAD_FILE_DELETEFAIL", 1);
                        m_game->GetGuiManager()->SetVisible("LOAD_FILE_DELETEFAIL", 1);
                    }
                } else {
                    m_game->GetGuiManager()->GetGuiPtr("LOAD_CHECK_MMC")->menu->Reset();
                    m_game->GetGuiManager()->SetActive("LOAD_CHECK_MMC", 1);
                    m_game->GetGuiManager()->SetVisible("LOAD_CHECK_MMC", 1);
                }
            }
        }
    }
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
        m_game->GetGuiManager()->GetGuiPtr("LOAD_FILE_DELETEOK")->menu->Reset();
        m_game->GetGuiManager()->SetActive("LOAD_FILE_DELETEOK", 0);
        m_game->GetGuiManager()->SetVisible("LOAD_FILE_DELETEOK", 0);

        m_game->GetGuiManager()->GetGuiPtr("LOAD_CHECK_MMC")->menu->Reset();
        m_game->GetGuiManager()->SetActive("LOAD_CHECK_MMC", 1);
        m_game->GetGuiManager()->SetVisible("LOAD_CHECK_MMC", 1);
    }
}

CGuiLoadFileDeletingFailedEventHandler::CGuiLoadFileDeletingFailedEventHandler() : CGuiBaseEventHandler("GuiLoadFileDeletingFailedEventHandler") {

}

void CGuiLoadFileDeletingFailedEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    CGuiBaseEventHandler::OnEvent(menu, event, unk);

    if (event != DKGUI::EVENT_3) {
        return;
    }

    std::string str = (char*)unk;
    if (str == "exit") {
        m_game->GetGuiManager()->GetGuiPtr("LOAD_FILE_DELETEFAIL")->menu->Reset();
        m_game->GetGuiManager()->SetActive("LOAD_FILE_DELETEFAIL", 0);
        m_game->GetGuiManager()->SetVisible("LOAD_FILE_DELETEFAIL", 0);

        m_game->GetGuiManager()->GetGuiPtr("LOAD_CHECK_MMC")->menu->Reset();
        m_game->GetGuiManager()->SetActive("LOAD_CHECK_MMC", 1);
        m_game->GetGuiManager()->SetVisible("LOAD_CHECK_MMC", 1);
    }
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
            U32 backup_state = m_game->GetBackupEngine()->GetState();
            m_game->m_timer->Resume();

            if (!(backup_state & (1 << 1))) {
                m_game->GetGuiManager()->GetGuiPtr("LOAD_WRONG_DEVICE")->menu->Reset();
                m_game->GetGuiManager()->SetActive("LOAD_WRONG_DEVICE", 0);
                m_game->GetGuiManager()->SetVisible("LOAD_WRONG_DEVICE", 0);

                m_game->GetGuiManager()->GetGuiPtr("LOAD_CHECK_MMC")->menu->Reset();
                m_game->GetGuiManager()->SetActive("LOAD_CHECK_MMC", 1);
                m_game->GetGuiManager()->SetVisible("LOAD_CHECK_MMC", 1);
            } else {
                gs_TimeBeforeMemCardCheck = 1.0f;
            }
        } else {
            gs_TimeBeforeMemCardCheck -= m_game->GetDeltaTime();
        }
    } else if (event == DKGUI::EVENT_3) {
        std::string str = (char*)unk;
        if (str == "yes") {
            m_game->GetGuiManager()->GetGuiPtr("LOAD_WRONG_DEVICE")->menu->Reset();
            m_game->GetGuiManager()->SetActive("LOAD_WRONG_DEVICE", 0);
            m_game->GetGuiManager()->SetVisible("LOAD_WRONG_DEVICE", 0);

            m_game->GetGuiManager()->GetGuiPtr("LOAD_CHECK_MMC")->menu->Reset();
            m_game->GetGuiManager()->SetActive("LOAD_CHECK_MMC", 1);
            m_game->GetGuiManager()->SetVisible("LOAD_CHECK_MMC", 1);
        } else if (str == "no") {
            m_game->GetGuiManager()->GetGuiPtr("LOAD_WRONG_DEVICE")->menu->Reset();
            m_game->GetGuiManager()->SetActive("LOAD_WRONG_DEVICE", 0);
            m_game->GetGuiManager()->SetVisible("LOAD_WRONG_DEVICE", 0);
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
            U32 backup_state = m_game->GetBackupEngine()->GetState();
            m_game->m_timer->Resume();

            if (!(backup_state & (1 << 1))) {
                m_game->GetGuiManager()->GetGuiPtr("SAVE_WRONG_DEVICE")->menu->Reset();
                m_game->GetGuiManager()->SetActive("SAVE_WRONG_DEVICE", 0);
                m_game->GetGuiManager()->SetVisible("SAVE_WRONG_DEVICE", 0);

                m_game->GetGuiManager()->GetGuiPtr("SAVE_CHECK_MMC")->menu->Reset();
                m_game->GetGuiManager()->SetActive("SAVE_CHECK_MMC", 1);
                m_game->GetGuiManager()->SetVisible("SAVE_CHECK_MMC", 1);
            } else {
                gs_TimeBeforeMemCardCheck = 1.0f;
            }
        } else {
            gs_TimeBeforeMemCardCheck -= m_game->GetDeltaTime();
        }
    } else if (event == DKGUI::EVENT_3) {
        std::string str = (char*)unk;
        if (str == "yes") {
            m_game->GetGuiManager()->GetGuiPtr("SAVE_WRONG_DEVICE")->menu->Reset();
            m_game->GetGuiManager()->SetActive("SAVE_WRONG_DEVICE", 0);
            m_game->GetGuiManager()->SetVisible("SAVE_WRONG_DEVICE", 0);

            m_game->GetGuiManager()->GetGuiPtr("SAVE_CHECK_MMC")->menu->Reset();
            m_game->GetGuiManager()->SetActive("SAVE_CHECK_MMC", 1);
            m_game->GetGuiManager()->SetVisible("SAVE_CHECK_MMC", 1);
        } else if (str == "no") {
            m_game->GetGuiManager()->GetGuiPtr("SAVE_WRONG_DEVICE")->menu->Reset();
            m_game->GetGuiManager()->SetActive("SAVE_WRONG_DEVICE", 0);
            m_game->GetGuiManager()->SetVisible("SAVE_WRONG_DEVICE", 0);

            if (m_game->m_unk4F54 == 8 && m_game->m_unk4F58 == 1) {
                CDKW_RGBA fade_color = m_game->ComputeGameFadeColor();
                m_game->FadeInit(1.0f, CGame::FADE_TYPE_4, fade_color.red, fade_color.green, fade_color.blue, 0.0f);
                ((CGamePartIngame*)m_game->GetGamePartPointer())->m_game_room_manager->m_flags |= (1 << 5);
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
        U32 backup_state = m_game->GetBackupEngine()->GetState();
        m_game->m_timer->Resume();

        if (!(backup_state & (1 << 0))) {
            m_game->GetGuiManager()->GetGuiPtr("LOAD_MMC_CORRUPT")->menu->Reset();
            m_game->GetGuiManager()->SetActive("LOAD_MMC_CORRUPT", 0);
            m_game->GetGuiManager()->SetVisible("LOAD_MMC_CORRUPT", 0);

            m_game->GetGuiManager()->GetGuiPtr("LOAD_CHECK_MMC")->menu->Reset();
            m_game->GetGuiManager()->SetActive("LOAD_CHECK_MMC", 1);
            m_game->GetGuiManager()->SetVisible("LOAD_CHECK_MMC", 1);
        }
    } else if (event == DKGUI::EVENT_3) {
        std::string str = (char*)unk;
        if (str == "yes") {
            m_game->GetGuiManager()->GetGuiPtr("LOAD_MMC_CORRUPT")->menu->Reset();
            m_game->GetGuiManager()->SetActive("LOAD_MMC_CORRUPT", 0);
            m_game->GetGuiManager()->SetVisible("LOAD_MMC_CORRUPT", 0);

            // nice
            U32 flags = m_game->m_unk8;
            flags |= m_game->m_unk8 | (1 << 9);
            m_game->m_unk8 = flags;

            m_game->GetGuiManager()->GetGuiPtr("SAVE_FORMAT_SURE")->menu->Reset();
            m_game->GetGuiManager()->SetActive("SAVE_FORMAT_SURE", 1);
            m_game->GetGuiManager()->SetVisible("SAVE_FORMAT_SURE", 1);
        } else if (str == "no") {
            m_game->GetGuiManager()->GetGuiPtr("LOAD_MMC_CORRUPT")->menu->Reset();
            m_game->GetGuiManager()->SetActive("LOAD_MMC_CORRUPT", 0);
            m_game->GetGuiManager()->SetVisible("LOAD_MMC_CORRUPT", 0);
        }
    }
}

CGuiSaveCorruptMemcardEventHandler::CGuiSaveCorruptMemcardEventHandler() : CGuiBaseEventHandler("GuiSaveCorruptMemcardEventHandler") {

}

void CGuiSaveCorruptMemcardEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    CGuiBaseEventHandler::OnEvent(menu, event, unk);

    if (event == DKGUI::EVENT_0) {
        m_game->m_timer->Pause();
        U32 backup_state = m_game->GetBackupEngine()->GetState();
        m_game->m_timer->Resume();

        if (!(backup_state & (1 << 0))) {
            m_game->GetGuiManager()->GetGuiPtr("SAVE_MMC_CORRUPT")->menu->Reset();
            m_game->GetGuiManager()->SetActive("SAVE_MMC_CORRUPT", 0);
            m_game->GetGuiManager()->SetVisible("SAVE_MMC_CORRUPT", 0);

            m_game->GetGuiManager()->GetGuiPtr("SAVE_CHECK_MMC")->menu->Reset();
            m_game->GetGuiManager()->SetActive("SAVE_CHECK_MMC", 1);
            m_game->GetGuiManager()->SetVisible("SAVE_CHECK_MMC", 1);
        }
    } else if (event == DKGUI::EVENT_3) {
        std::string str = (char*)unk;
        if (str == "yes") {
            m_game->GetGuiManager()->GetGuiPtr("SAVE_MMC_CORRUPT")->menu->Reset();
            m_game->GetGuiManager()->SetActive("SAVE_MMC_CORRUPT", 0);
            m_game->GetGuiManager()->SetVisible("SAVE_MMC_CORRUPT", 0);

            m_game->GetGuiManager()->GetGuiPtr("SAVE_FORMAT_SURE")->menu->Reset();
            m_game->GetGuiManager()->SetActive("SAVE_FORMAT_SURE", 1);
            m_game->GetGuiManager()->SetVisible("SAVE_FORMAT_SURE", 1);
        } else if (str == "no") {
            m_game->GetGuiManager()->GetGuiPtr("SAVE_MMC_CORRUPT")->menu->Reset();
            m_game->GetGuiManager()->SetActive("SAVE_MMC_CORRUPT", 0);
            m_game->GetGuiManager()->SetVisible("SAVE_MMC_CORRUPT", 0);

            if (m_game->m_unk4F54 == 8 && m_game->m_unk4F58 == 1) {
                CDKW_RGBA fade_color = m_game->ComputeGameFadeColor();
                m_game->FadeInit(1.0f, CGame::FADE_TYPE_4, fade_color.red, fade_color.green, fade_color.blue, 0.0f);
                ((CGamePartIngame*)m_game->GetGamePartPointer())->m_game_room_manager->m_flags |= (1 << 5);
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
            U32 backup_state = m_game->GetBackupEngine()->GetState();
            m_game->m_timer->Resume();

            if (!(backup_state & (1 << 0))) {
                m_game->GetGuiManager()->GetGuiPtr("SAVE_MMC_UNUSEABLE")->menu->Reset();
                m_game->GetGuiManager()->SetActive("SAVE_MMC_UNUSEABLE", 0);
                m_game->GetGuiManager()->SetVisible("SAVE_MMC_UNUSEABLE", 0);

                m_game->GetGuiManager()->GetGuiPtr("SAVE_CHECK_MMC")->menu->Reset();
                m_game->GetGuiManager()->SetActive("SAVE_CHECK_MMC", 1);
                m_game->GetGuiManager()->SetVisible("SAVE_CHECK_MMC", 1);
            } else {
                gs_TimeBeforeMemCardCheck = 1.0f;
            }
        } else {
            gs_TimeBeforeMemCardCheck -= m_game->GetDeltaTime();
        }
    } else if (event == DKGUI::EVENT_3) {
        std::string str = (char*)unk;
        if (str == "yes") {
            m_game->GetGuiManager()->GetGuiPtr("SAVE_MMC_UNUSEABLE")->menu->Reset();
            m_game->GetGuiManager()->SetActive("SAVE_MMC_UNUSEABLE", 0);
            m_game->GetGuiManager()->SetVisible("SAVE_MMC_UNUSEABLE", 0);

            m_game->GetGuiManager()->GetGuiPtr("SAVE_CHECK_MMC")->menu->Reset();
            m_game->GetGuiManager()->SetActive("SAVE_CHECK_MMC", 1);
            m_game->GetGuiManager()->SetVisible("SAVE_CHECK_MMC", 1);
        } else if (str == "no") {
            m_game->GetGuiManager()->GetGuiPtr("SAVE_MMC_UNUSEABLE")->menu->Reset();
            m_game->GetGuiManager()->SetActive("SAVE_MMC_UNUSEABLE", 0);
            m_game->GetGuiManager()->SetVisible("SAVE_MMC_UNUSEABLE", 0);

            if (m_game->m_unk4F54 == 8 && m_game->m_unk4F58 == 1) {
                CDKW_RGBA fade_color = m_game->ComputeGameFadeColor();
                m_game->FadeInit(1.0f, CGame::FADE_TYPE_4, fade_color.red, fade_color.green, fade_color.blue, 0.0f);
                ((CGamePartIngame*)m_game->GetGamePartPointer())->m_game_room_manager->m_flags |= (1 << 5);
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
            m_game->GetGuiManager()->GetGuiPtr("SAVE_SAVE_OK")->menu->Reset();
            m_game->GetGuiManager()->SetActive("SAVE_SAVE_OK", 0);
            m_game->GetGuiManager()->SetVisible("SAVE_SAVE_OK", 0);

            if (m_game->m_unk4F54 == 8 && m_game->m_unk4F58 == 1) {
                CDKW_RGBA fade_color = m_game->ComputeGameFadeColor();
                m_game->FadeInit(1.0f, CGame::FADE_TYPE_4, fade_color.red, fade_color.green, fade_color.blue, 0.0f);
                ((CGamePartIngame*)m_game->GetGamePartPointer())->m_game_room_manager->m_flags |= (1 << 5);
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
        m_game->GetGuiManager()->GetGuiPtr("SAVE_NO_MMC")->menu->Reset();
        m_game->GetGuiManager()->SetActive("SAVE_NO_MMC", 0);
        m_game->GetGuiManager()->SetVisible("SAVE_NO_MMC", 0);

        m_game->GetGuiManager()->GetGuiPtr("SAVE_CHECK_MMC")->menu->Reset();
        m_game->GetGuiManager()->SetActive("SAVE_CHECK_MMC", 1);
        m_game->GetGuiManager()->SetVisible("SAVE_CHECK_MMC", 1);
    } else if (str == "no") {
        m_game->GetGuiManager()->GetGuiPtr("SAVE_NO_MMC")->menu->Reset();
        m_game->GetGuiManager()->SetActive("SAVE_NO_MMC", 0);
        m_game->GetGuiManager()->SetVisible("SAVE_NO_MMC", 0);

        if (m_game->m_unk4F54 == 8 && m_game->m_unk4F58 == 1) {
            CDKW_RGBA fade_color = m_game->ComputeGameFadeColor();
            m_game->FadeInit(1.0f, CGame::FADE_TYPE_4, fade_color.red, fade_color.green, fade_color.blue, 0.0f);
            ((CGamePartIngame*)m_game->GetGamePartPointer())->m_game_room_manager->m_flags |= (1 << 5);
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
            U32 backup_state = m_game->GetBackupEngine()->GetState();
            m_game->m_timer->Resume();

            if (!(backup_state & (1 << 0))) {
                m_game->GetGuiManager()->GetGuiPtr("SAVE_UNFORMAT")->menu->Reset();
                m_game->GetGuiManager()->SetActive("SAVE_UNFORMAT", 0);
                m_game->GetGuiManager()->SetVisible("SAVE_UNFORMAT", 0);

                if (m_game->m_unk8 & (1 << 9)) {
                    m_game->GetGuiManager()->GetGuiPtr("LOAD_CHECK_MMC")->menu->Reset();
                    m_game->GetGuiManager()->SetActive("LOAD_CHECK_MMC", 1);
                    m_game->GetGuiManager()->SetVisible("LOAD_CHECK_MMC", 1);
                    m_game->m_unk8 &= ~(1 << 9);
                } else {
                    m_game->GetGuiManager()->GetGuiPtr("SAVE_CHECK_MMC")->menu->Reset();
                    m_game->GetGuiManager()->SetActive("SAVE_CHECK_MMC", 1);
                    m_game->GetGuiManager()->SetVisible("SAVE_CHECK_MMC", 1);
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
            m_game->GetGuiManager()->GetGuiPtr("SAVE_UNFORMAT")->menu->Reset();
            m_game->GetGuiManager()->SetActive("SAVE_UNFORMAT", 0);
            m_game->GetGuiManager()->SetVisible("SAVE_UNFORMAT", 0);

            m_game->GetGuiManager()->GetGuiPtr("SAVE_FORMAT_SURE")->menu->Reset();
            m_game->GetGuiManager()->SetActive("SAVE_FORMAT_SURE", 1);
            m_game->GetGuiManager()->SetVisible("SAVE_FORMAT_SURE", 1);

        } else if (str == "no") {
            m_game->GetGuiManager()->GetGuiPtr("SAVE_UNFORMAT")->menu->Reset();
            m_game->GetGuiManager()->SetActive("SAVE_UNFORMAT", 0);
            m_game->GetGuiManager()->SetVisible("SAVE_UNFORMAT", 0);

            if (m_game->m_unk8 & (1 << 9)) {
                m_game->m_unk8 &= ~(1 << 9);
                return;
            }

            if (m_game->m_unk4F54 == 8 && m_game->m_unk4F58 == 1) {
                CDKW_RGBA fade_color = m_game->ComputeGameFadeColor();
                m_game->FadeInit(1.0f, CGame::FADE_TYPE_4, fade_color.red, fade_color.green, fade_color.blue, 0.0f);
                ((CGamePartIngame*)m_game->GetGamePartPointer())->m_game_room_manager->m_flags |= (1 << 5);
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
            U32 backup_state = m_game->GetBackupEngine()->GetState();
            m_game->m_timer->Resume();

            if (!(backup_state & (1 << 0))) {
                m_game->GetGuiManager()->GetGuiPtr("SAVE_FORMAT_SURE")->menu->Reset();
                m_game->GetGuiManager()->SetActive("SAVE_FORMAT_SURE", 0);
                m_game->GetGuiManager()->SetVisible("SAVE_FORMAT_SURE", 0);

                if (m_game->m_unk8 & (1 << 9)) {
                    m_game->GetGuiManager()->GetGuiPtr("LOAD_CHECK_MMC")->menu->Reset();
                    m_game->GetGuiManager()->SetActive("LOAD_CHECK_MMC", 1);
                    m_game->GetGuiManager()->SetVisible("LOAD_CHECK_MMC", 1);
                    m_game->m_unk8 &= ~(1 << 9);
                } else {
                    m_game->GetGuiManager()->GetGuiPtr("SAVE_CHECK_MMC")->menu->Reset();
                    m_game->GetGuiManager()->SetActive("SAVE_CHECK_MMC", 1);
                    m_game->GetGuiManager()->SetVisible("SAVE_CHECK_MMC", 1);
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
            m_game->GetGuiManager()->GetGuiPtr("SAVE_FORMAT_SURE")->menu->Reset();
            m_game->GetGuiManager()->SetActive("SAVE_FORMAT_SURE", 0);
            m_game->GetGuiManager()->SetVisible("SAVE_FORMAT_SURE", 0);

            m_game->GetGuiManager()->GetGuiPtr("SAVE_FORMAT")->menu->Reset();
            m_game->GetGuiManager()->SetActive("SAVE_FORMAT", 1);
            m_game->GetGuiManager()->SetVisible("SAVE_FORMAT", 1);
        } else if (str == "no") {
            m_game->GetGuiManager()->GetGuiPtr("SAVE_FORMAT_SURE")->menu->Reset();
            m_game->GetGuiManager()->SetActive("SAVE_FORMAT_SURE", 0);
            m_game->GetGuiManager()->SetVisible("SAVE_FORMAT_SURE", 0);

            if (m_game->m_unk8 & (1 << 9)) {
                m_game->m_unk8 &= ~(1 << 9);
                return;
            }

            if (m_game->m_unk4F54 == 8 && m_game->m_unk4F58 == 1) {
                CDKW_RGBA fade_color = m_game->ComputeGameFadeColor();
                m_game->FadeInit(1.0f, CGame::FADE_TYPE_4, fade_color.red, fade_color.green, fade_color.blue, 0.0f);
                ((CGamePartIngame*)m_game->GetGamePartPointer())->m_game_room_manager->m_flags |= (1 << 5);
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
    if (str == "exit") {
        m_game->GetGuiManager()->GetGuiPtr("SAVE_FORMAT")->menu->Reset();
        m_game->GetGuiManager()->SetActive("SAVE_FORMAT", 0);
        m_game->GetGuiManager()->SetVisible("SAVE_FORMAT", 0);

        m_game->m_timer->Pause();
        U32 backup_status = m_game->GetBackupEngine()->GetState();
        m_game->m_timer->Resume();

        if (backup_status == 17) {
            U32 flags = m_game->m_unk8;
            if (flags & (1 << 9)) {
                m_game->m_unk8 = flags & ~(1 << 9);
                m_game->GetGuiManager()->GetGuiPtr("LOAD_CHECK_MMC")->menu->Reset();
                m_game->GetGuiManager()->SetActive("LOAD_CHECK_MMC", 1);
                m_game->GetGuiManager()->SetVisible("LOAD_CHECK_MMC", 1);
                return;
            } else {
                m_game->GetGuiManager()->GetGuiPtr("SAVE_CHECK_MMC")->menu->Reset();
                m_game->GetGuiManager()->SetActive("SAVE_CHECK_MMC", 1);
                m_game->GetGuiManager()->SetVisible("SAVE_CHECK_MMC", 1);
                return;
            }
        } else if (backup_status == 0) {
            U32 flags = m_game->m_unk8;
            if (flags & (1 << 9)) {
                m_game->m_unk8 = flags & ~(1 << 9);
                m_game->GetGuiManager()->GetGuiPtr("LOAD_CHECK_MMC")->menu->Reset();
                m_game->GetGuiManager()->SetActive("LOAD_CHECK_MMC", 1);
                m_game->GetGuiManager()->SetVisible("LOAD_CHECK_MMC", 1);
                return;
            } else {
                m_game->GetGuiManager()->GetGuiPtr("SAVE_CHECK_MMC")->menu->Reset();
                m_game->GetGuiManager()->SetActive("SAVE_CHECK_MMC", 1);
                m_game->GetGuiManager()->SetVisible("SAVE_CHECK_MMC", 1);
                return;
            }
        }

        if (m_game->GetBackupEngine()->Format() != 0) {
            m_game->GetGuiManager()->GetGuiPtr("SAVE_FORMAT_FAILED")->menu->Reset();
            m_game->GetGuiManager()->SetActive("SAVE_FORMAT_FAILED", 1);
            m_game->GetGuiManager()->SetVisible("SAVE_FORMAT_FAILED", 1);
        } else {
            m_game->GetGuiManager()->GetGuiPtr("SAVE_FORMAT_OK")->menu->Reset();
            m_game->GetGuiManager()->SetActive("SAVE_FORMAT_OK", 1);
            m_game->GetGuiManager()->SetVisible("SAVE_FORMAT_OK", 1);
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
            m_game->GetGuiManager()->GetGuiPtr("SAVE_FORMAT_OK")->menu->Reset();
            m_game->GetGuiManager()->SetActive("SAVE_FORMAT_OK", 0);
            m_game->GetGuiManager()->SetVisible("SAVE_FORMAT_OK", 0);

            U32 flags = m_game->m_unk8;
            if (flags & (1 << 9)) {
                m_game->m_unk8 = flags & ~(1 << 9);
            } else {
                m_game->GetGuiManager()->GetGuiPtr("SAVE_CHECK_MMC")->menu->Reset();
                m_game->GetGuiManager()->SetActive("SAVE_CHECK_MMC", 1);
                m_game->GetGuiManager()->SetVisible("SAVE_CHECK_MMC", 1);
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
            U32 backup_state = m_game->GetBackupEngine()->GetState();
            m_game->m_timer->Resume();

            if (!(backup_state & (1 << 0))) {
                m_game->GetGuiManager()->GetGuiPtr("SAVE_FORMAT_FAILED")->menu->Reset();
                m_game->GetGuiManager()->SetActive("SAVE_FORMAT_FAILED", 0);
                m_game->GetGuiManager()->SetVisible("SAVE_FORMAT_FAILED", 0);

                U32 flags = m_game->m_unk8;
                if (flags & (1 << 9)) {
                    m_game->m_unk8 = flags & ~(1 << 9);
                    m_game->GetGuiManager()->GetGuiPtr("LOAD_CHECK_MMC")->menu->Reset();
                    m_game->GetGuiManager()->SetActive("LOAD_CHECK_MMC", 1);
                    m_game->GetGuiManager()->SetVisible("LOAD_CHECK_MMC", 1);
                } else {
                    m_game->GetGuiManager()->GetGuiPtr("SAVE_CHECK_MMC")->menu->Reset();
                    m_game->GetGuiManager()->SetActive("SAVE_CHECK_MMC", 1);
                    m_game->GetGuiManager()->SetVisible("SAVE_CHECK_MMC", 1);
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
            m_game->GetGuiManager()->GetGuiPtr("SAVE_FORMAT_FAILED")->menu->Reset();
            m_game->GetGuiManager()->SetActive("SAVE_FORMAT_FAILED", 0);
            m_game->GetGuiManager()->SetVisible("SAVE_FORMAT_FAILED", 0);

            U32 flags = m_game->m_unk8;
            if (flags & (1 << 9)) {
                m_game->m_unk8 = flags & ~(1 << 9);
                m_game->GetGuiManager()->GetGuiPtr("LOAD_CHECK_MMC")->menu->Reset();
                m_game->GetGuiManager()->SetActive("LOAD_CHECK_MMC", 1);
                m_game->GetGuiManager()->SetVisible("LOAD_CHECK_MMC", 1);
                return;
            } else {
                m_game->GetGuiManager()->GetGuiPtr("SAVE_CHECK_MMC")->menu->Reset();
                m_game->GetGuiManager()->SetActive("SAVE_CHECK_MMC", 1);
                m_game->GetGuiManager()->SetVisible("SAVE_CHECK_MMC", 1);
            }
        } else if (str == "no") {
            m_game->GetGuiManager()->GetGuiPtr("SAVE_FORMAT_FAILED")->menu->Reset();
            m_game->GetGuiManager()->SetActive("SAVE_FORMAT_FAILED", 0);
            m_game->GetGuiManager()->SetVisible("SAVE_FORMAT_FAILED", 0);

            if (m_game->m_unk8 & (1 << 9)) {
                m_game->m_unk8 &= ~(1 << 9);
                return;
            } else if (m_game->m_unk4F54 == 8 && m_game->m_unk4F58 == 1) {
                CDKW_RGBA fade_color = m_game->ComputeGameFadeColor();
                m_game->FadeInit(1.0f, CGame::FADE_TYPE_4, fade_color.red, fade_color.green, fade_color.blue, 0.0f);
                ((CGamePartIngame*)m_game->GetGamePartPointer())->m_game_room_manager->m_flags |= (1 << 5);
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
            U32 backup_state = m_game->GetBackupEngine()->GetState();
            m_game->m_timer->Resume();

            if (!(backup_state & (1 << 0))) {
                m_game->GetGuiManager()->GetGuiPtr("CREATE_OVERWRITE")->menu->Reset();
                m_game->GetGuiManager()->SetActive("CREATE_OVERWRITE", 0);
                m_game->GetGuiManager()->SetVisible("CREATE_OVERWRITE", 0);

                m_game->GetGuiManager()->GetGuiPtr("SAVE_CHECK_MMC")->menu->Reset();
                m_game->GetGuiManager()->SetActive("SAVE_CHECK_MMC", 1);
                m_game->GetGuiManager()->SetVisible("SAVE_CHECK_MMC", 1);
            } else {
                gs_TimeBeforeMemCardCheck = 1.0f;
            }
        } else {
            gs_TimeBeforeMemCardCheck -= m_game->GetDeltaTime();
        }
    } else if (event == DKGUI::EVENT_3) {
        std::string str = (char*)unk;
        if (str == "yes") {
            m_game->GetGuiManager()->GetGuiPtr("CREATE_OVERWRITE")->menu->Reset();
            m_game->GetGuiManager()->SetActive("CREATE_OVERWRITE", 0);
            m_game->GetGuiManager()->SetVisible("CREATE_OVERWRITE", 0);

            m_game->GetGuiManager()->GetGuiPtr("CREATE_OVERWRITE_SURE")->menu->Reset();
            m_game->GetGuiManager()->SetActive("CREATE_OVERWRITE_SURE", 1);
            m_game->GetGuiManager()->SetVisible("CREATE_OVERWRITE_SURE", 1);
        } else if (str == "no") {
            m_game->GetGuiManager()->GetGuiPtr("CREATE_OVERWRITE")->menu->Reset();
            m_game->GetGuiManager()->SetActive("CREATE_OVERWRITE", 0);
            m_game->GetGuiManager()->SetVisible("CREATE_OVERWRITE", 0);
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
            U32 backup_state = m_game->GetBackupEngine()->GetState();
            m_game->m_timer->Resume();

            if (!(backup_state & (1 << 0))) {
                m_game->GetGuiManager()->GetGuiPtr("CREATE_OVERWRITE_SURE")->menu->Reset();
                m_game->GetGuiManager()->SetActive("CREATE_OVERWRITE_SURE", 0);
                m_game->GetGuiManager()->SetVisible("CREATE_OVERWRITE_SURE", 0);

                m_game->GetGuiManager()->GetGuiPtr("SAVE_CHECK_MMC")->menu->Reset();
                m_game->GetGuiManager()->SetActive("SAVE_CHECK_MMC", 1);
                m_game->GetGuiManager()->SetVisible("SAVE_CHECK_MMC", 1);
            } else {
                gs_TimeBeforeMemCardCheck = 1.0f;
            }
        } else {
            gs_TimeBeforeMemCardCheck -= m_game->GetDeltaTime();
        }
    } else if (event == DKGUI::EVENT_3) {
        std::string str = (char*)unk;
        if (str == "yes") {
            m_game->GetGuiManager()->GetGuiPtr("CREATE_OVERWRITE_SURE")->menu->Reset();
            m_game->GetGuiManager()->SetActive("CREATE_OVERWRITE_SURE", 0);
            m_game->GetGuiManager()->SetVisible("CREATE_OVERWRITE_SURE", 0);

            m_game->m_timer->Pause();
            U32 backup_state = m_game->GetBackupEngine()->GetState();
            m_game->m_timer->Resume();

            if (backup_state == 17) {
                CGuiEnterNameEventHandler* handler = (CGuiEnterNameEventHandler*)m_game->GetGuiManager()->IsEventCallbackRegistered("GuiEnterNameEventHandler");
                std::string temp = m_game->GetBackupEngine()->FindFirst("GPLE9G", handler->GetText());

                if (temp != handler->GetText() && m_game->GetBackupEngine()->IsSpaceAvailable("GPLE9G", handler->GetText(), 0x18000) == FALSE) {
                    m_game->GetGuiManager()->GetGuiPtr("SAVE_NO_SPACE")->menu->Reset();
                    m_game->GetGuiManager()->SetActive("SAVE_NO_SPACE", 1);
                    m_game->GetGuiManager()->SetVisible("SAVE_NO_SPACE", 1);
                } else {
                    m_game->GetGuiManager()->GetGuiPtr("SAVE_SAVE_DATA")->menu->Reset();
                    m_game->GetGuiManager()->SetActive("SAVE_SAVE_DATA", 1);
                    m_game->GetGuiManager()->SetVisible("SAVE_SAVE_DATA", 1);
                }

            } else if (backup_state & (1 << 2)) {
                m_game->GetGuiManager()->GetGuiPtr("SAVE_MMC_CORRUPT")->menu->Reset();
                m_game->GetGuiManager()->SetActive("SAVE_MMC_CORRUPT", 1);
                m_game->GetGuiManager()->SetVisible("SAVE_MMC_CORRUPT", 1);
            } else if (backup_state & (1 << 3)) {
                m_game->GetGuiManager()->GetGuiPtr("SAVE_MMC_UNUSEABLE")->menu->Reset();
                m_game->GetGuiManager()->SetActive("SAVE_MMC_UNUSEABLE", 1);
                m_game->GetGuiManager()->SetVisible("SAVE_MMC_UNUSEABLE", 1);
            } else if (backup_state & (1 << 1)) {
                m_game->GetGuiManager()->GetGuiPtr("SAVE_WRONG_DEVICE")->menu->Reset();
                m_game->GetGuiManager()->SetActive("SAVE_WRONG_DEVICE", 1);
                m_game->GetGuiManager()->SetVisible("SAVE_WRONG_DEVICE", 1);
            } else if (!(backup_state & (1 << 0))) {
                m_game->GetGuiManager()->GetGuiPtr("SAVE_NO_MMC")->menu->Reset();
                m_game->GetGuiManager()->SetActive("SAVE_NO_MMC", 1);
                m_game->GetGuiManager()->SetVisible("SAVE_NO_MMC", 1);
            } else if (!(backup_state & (1 << 4))) {
                m_game->GetGuiManager()->GetGuiPtr("SAVE_UNFORMAT")->menu->Reset();
                m_game->GetGuiManager()->SetActive("SAVE_UNFORMAT", 1);
                m_game->GetGuiManager()->SetVisible("SAVE_UNFORMAT", 1);
            }
        } else if (str == "no") {
            m_game->GetGuiManager()->GetGuiPtr("CREATE_OVERWRITE_SURE")->menu->Reset();
            m_game->GetGuiManager()->SetActive("CREATE_OVERWRITE_SURE", 0);
            m_game->GetGuiManager()->SetVisible("CREATE_OVERWRITE_SURE", 0);
        }
    }
}

CGuiDreamSelectEventHandler::CGuiDreamSelectEventHandler() : CGuiBaseEventHandler("GuiDreamSelectEventHandler") {
    m_unk10 = 0;
    m_unk18 = 0;
}

static RwV3d gs_WarpsCameras[21][2] = {
    {},
    {{6155.0f, -1162.0f, -4336.0f}, {7451.0f, -516.0f, -5172.0f}},
    {{6692.0f, -894.0f, -4684.0f}, {7451.0f, -529.0f, -5208.0f}},
    {},
    {{4628.0f, -1195.0f, -3801.0f}, {4670.0f, -788.0f, -4165.0f}},
    {{4628.0f, -1195.0f, -3801.0f}, {4670.0f, -788.0f, -4165.0f}},
    {{2573.0f, -1167.0f, -4360.0f}, {2522.0f, -190.0f, -4637.0f}},
    {{2408.0f, -1237.0f, -4343.0f}, {2353.0f, -190.0f, -4681.0f}},
    {{2408.0f, -1237.0f, -4343.0f}, {2353.0f, -190.0f, -4681.0f}},
    {{-2627.0f, -1378.0f, -4002.0f}, {-1937.0f, 99.0f, -5371.0f}},
    {{-2626.0f, -1206.0f, -4111.0f}, {-2006.0f, 145.0f, -5371.0f}},
    {{-2626.0f, -1206.0f, -4111.0f}, {-2006.0f, 145.0f, -5371.0f}},
    {{-4612.0f, -741.0f, -3811.0f}, {-4447.0f, -195.0f, -4549.0f}},
    {{-4727.0f, -870.0f, -3646.0f}, {-4375.0f, -264.0f, -4670.0f}},
    {{-4727.0f, -870.0f, -3646.0f}, {-4375.0f, -264.0f, -4670.0f}},
    {{-7284.0f, -2230.0f, -3777.0f}, {-7895.0f, -754.0f, -4703.0f}},
    {{-7498.0f, -1845.0f, -4102.0f}, {-7902.0f, -795.0f, -5015.0f}},
    {{-7501.0f, -1759.0f, -4025.0f}, {-7861.0f, -744.0f, -4944.0f}},
    {{671.0f, -1605.0f, -3880.0f}, {212.0f, -375.0f, -4669.0f}},
    {{671.0f, -1605.0f, -3880.0f}, {212.0f, -375.0f, -4669.0f}},
};

// Incomplete
void CGuiDreamSelectEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    CGuiBaseEventHandler::OnEvent(menu, event, unk);

    if (event == DKGUI::EVENT_0) {
        if (m_unk14 == 1 && m_unk10 != 7) {
            CMission& mission = m_game->GetMission(m_unk10 - 1);
            char buf[32];
            sprintf(buf, "%03d/%03d", mission.GetCurrentNbCookies(), mission.GetNbTotalCookies());
            m_game->GetGuiEngine()->AddText(0.1f, 0.25f, buf, 0.05f, NULL, 0.0f);
        }

        if (m_unk10 > 0 && m_unk10 < 8) {
            if (m_unk18 != NULL) {
                m_unk18->UpdateAnimations(m_game->GetDeltaTime() / 2);
                DKDSP::CClump* clump = m_unk18->m_clump;
                clump->Update(m_game->GetDeltaTime() / 2);
            }

            if (m_unk10 == 6 && m_game->GetMission(6 - 1).m_unk30 != 0u) {
                CEntity* entity = m_game->m_entity_manager->GetEntity("FX_803_00");
                if (entity != NULL) {
                    entity->Update(m_game->GetDeltaTime() / 2);
                }
            }

            int iVar4 = m_unk10 - 1;
            int iVar15 = iVar4 * 18;
            CMission& mission = m_game->GetMission(iVar4);
            if (mission.m_unk30 == 0u) {
                if (mission.m_unk2C == 0) {
                    iVar15 += 6;
                } else if (mission.m_unk2C != 0) {
                    iVar15 += 12;
                }
            }
            CDKW_V3d vec1;
            vec1.x = gs_WarpsCameras[iVar15][0].x;
            vec1.z = gs_WarpsCameras[iVar15][0].z;
            vec1.y = -gs_WarpsCameras[iVar15][0].y;
            CDKW_V3d vec2;
            vec2.x = gs_WarpsCameras[iVar15][1].x;
            vec2.z = gs_WarpsCameras[iVar15][1].z;
            vec2.y = -gs_WarpsCameras[iVar15][1].y;
            vec1.x /= 100.0f;
            vec1.z /= 100.0f;
            vec1.y /= 100.0f;
            vec2.x /= 100.0f;
            vec2.z /= 100.0f;
            vec2.y /= 100.0f;
            m_game->GetCamera()->SetFOV(50.0f, 4.0f/3.0f);
            m_game->GetCamera()->LookAtInline(vec1, vec2, CDKW_V3d::YAXIS);
        }
    } else if (event == DKGUI::EVENT_2) {
        if (m_game->GetGuiManager()->IsActive("DREAM_ARROW_LEFT") == FALSE) {
            m_game->GetGuiManager()->SetActive("DREAM_ARROW_LEFT", 1);
            m_game->GetGuiManager()->SetVisible("DREAM_ARROW_LEFT", 1);
        }

        m_unk14 = 0;

        // int iVar5 = m_game->GetMission(0).Unk30Check() != 0 || m_game->GetMission(0).m_unk2C != 0 || m_game->GetMission(1).Unk30Check() != 0 || m_game->GetMission(1).m_unk2C != 0;
        // // iVar5 = 0;
        if (m_game->MissionUnk30Inline(0) == 1) {
            m_game->GetMission(2).m_unk30 = 0;
            m_game->GetMission(3).m_unk30 = 0;
        }

        if (m_game->MissionUnk30Inline(2) == 1) {
            m_game->GetMission(4).m_unk30 = 0;
            m_game->GetMission(5).m_unk30 = 0;
        }

        int iVar3;
        for (int i = 0; i < 8; i++) {
            CMission& mission = m_game->GetMission(i);
            if ((int)mission.m_unk30 < 0) {
                iVar3 = mission.m_unk30;
            }
        }
        // ...
        if (iVar3 == 1) {
            m_game->GetMission(6).m_unk30 = 0;
        }

        if (m_game->m_unk8 & (1 << 4)) {
            for (int i = 0; i < 7; i++) {
                m_game->GetMission(i).m_unk30 = 0;
            }
        }

        GotoMission(1);
    } else if (event == DKGUI::EVENT_3) {
        std::string str = (char*)unk;

        if (str == "cookie") {
            m_unk14 = 1;
            PlayNarrator(m_unk10);
        } else if (str != "exit") {
            if (str == "cancel") {
                PlaySoundInline("playsound SND_004_2C");
                GotoMission(0);
            } else if (str == "yes") {
                if (m_game->GetMission(m_unk10 - 1).m_unk30 == 0u) {
                    m_game->ResetOpcodeBuffer();
                    if (m_game->GetMission(m_unk10 - 1).m_unk2C == 0) {
                        switch (m_unk10) {
                            case 1:
                                m_game->PushOpcodeValue(13);
                                m_game->PushOpcodeValue(15);
                                break;
                            case 2:
                                m_game->PushOpcodeValue(13);
                                m_game->PushOpcodeValue(17);
                                break;
                            case 3:
                                m_game->PushOpcodeValue(13);
                                m_game->PushOpcodeValue(20);
                                break;
                            case 4:
                                m_game->PushOpcodeValue(13);
                                m_game->PushOpcodeValue(22);
                                break;
                            case 5:
                                m_game->PushOpcodeValue(13);
                                m_game->PushOpcodeValue(25);
                                break;
                            case 6:
                                m_game->PushOpcodeValue(13);
                                m_game->PushOpcodeValue(27);
                                break;
                        }
                    }

                    m_game->PushOpcodeValue(2);
                    m_game->PushOpcodeValue(m_unk10);
                    m_game->StopNarratorLine();

                    CDKW_RGBA color;

                    if (m_game->GetMission(m_unk10 - 1).m_unk2C == 0) {
                        m_game->PushOpcodeValue(1);
                        m_game->PushOpcodeValue(m_game->GetMission(m_unk10 - 1).GetUnkC());
                        m_game->PushOpcodeValue(m_game->GetMission(m_unk10 - 1).m_unk10);

                        m_game->PushOpcodeValue(m_game->GetMission(m_unk10 - 1).GetStartPosition().x);
                        m_game->PushOpcodeValue(m_game->GetMission(m_unk10 - 1).GetStartPosition().y);
                        m_game->PushOpcodeValue(m_game->GetMission(m_unk10 - 1).GetStartPosition().z);
                        m_game->PushOpcodeValue(m_game->GetMission(m_unk10 - 1).GetStartRotation().x);
                        m_game->PushOpcodeValue(m_game->GetMission(m_unk10 - 1).GetStartRotation().y);
                        m_game->PushOpcodeValue(m_game->GetMission(m_unk10 - 1).GetStartRotation().z);

                        m_game->PushOpcodeValue(0);
                        if (m_game->GetMission(m_unk10 - 1).m_unk34 == 0) {
                            m_game->PushOpcodeValue(6);
                        }

                        color = m_game->ComputeGameFadeColor();
                    } else {
                        m_game->PushOpcodeValue(1);
                        m_game->PushOpcodeValue(m_game->GetMission(m_unk10 - 1).GetUnkC());
                        m_game->PushOpcodeValue(3);

                        m_game->PushOpcodeValue(m_game->GetMission(m_unk10 - 1).GetStartPosition().x);
                        m_game->PushOpcodeValue(m_game->GetMission(m_unk10 - 1).GetStartPosition().y);
                        m_game->PushOpcodeValue(m_game->GetMission(m_unk10 - 1).GetStartPosition().z);
                        m_game->PushOpcodeValue(m_game->GetMission(m_unk10 - 1).GetStartRotation().x);
                        m_game->PushOpcodeValue(m_game->GetMission(m_unk10 - 1).GetStartRotation().y);
                        m_game->PushOpcodeValue(m_game->GetMission(m_unk10 - 1).GetStartRotation().z);

                        m_game->PushOpcodeValue(0);

                        color = CDKW_RGBA(78, 83, 149, 0xFF);
                    }

                    UnkGamePartAndReturnTypeInline();
                    m_game->FadeInit(1.0f, CGame::FADE_TYPE_4, color.red, color.green, color.blue, 0.0f);
                    m_game->GetIngameGamePart()->GetGameRoomManager()->m_flags |= (1 << 5);
                    m_game->FadeIn(-1.0f);

                    m_unk18 = NULL;

                    m_game->GetGuiManager()->GetGuiPtr("DREAM_DREAM")->menu->GetAnim()->GotoFrameLabel("DREAM_NOCLOUD");
                    m_game->GetGuiManager()->GetGuiPtr("DREAM_DREAM")->menu->GetAnim()->Play();
                    m_game->GetGuiManager()->SetActive("DREAM_DREAM", 0);
                    m_game->GetGuiManager()->SetActive("DREAM_ARROW_LEFT", 0);
                }
            } else if (str == "left") {
                if (m_unk10 - 1 > 0) {
                    GotoMission(m_unk10 - 1);
                }
            } else if (str == "right") {
                if (m_unk10 + 1 < 8) {
                    if (m_unk10 + 1 == 7 && m_game->GetMission(7 - 1).m_unk30 != 0) {
                        return;
                    }

                    GotoMission(m_unk10 + 1);
                }
            }
        }
    }
}

void CGuiDreamSelectEventHandler::GotoMission(int mission_no) {
    if (mission_no == 0) {
        m_game->GetGuiManager()->GetGuiPtr("DREAM_DREAM")->menu->Reset();
        m_game->GetGuiManager()->SetActive("DREAM_DREAM", 0);
        m_game->GetGuiManager()->SetVisible("DREAM_DREAM", 0);

        m_game->GetGuiManager()->GetGuiPtr("DREAM_ARROW_LEFT")->menu->Reset();
        m_game->GetGuiManager()->SetActive("DREAM_ARROW_LEFT", 0);
        m_game->GetGuiManager()->SetVisible("DREAM_ARROW_LEFT", 0);

        m_game->GetGuiManager()->GetGuiPtr("COOKIE_COOKIE")->menu->Reset();
        m_game->GetGuiManager()->SetActive("COOKIE_COOKIE", 0);
        m_game->GetGuiManager()->SetVisible("COOKIE_COOKIE", 0);

        m_unk10 = 0;
        m_unk14 = 0;

        CDKW_V3d hero_pos = m_game->m_entity_manager->GetHero()->GetPosition();

        CGamePartIngame* game_part = m_game->GetIngameGamePart();
        if (game_part != NULL) {
            CCACamera* camera = game_part->m_camera;
            camera->Update(hero_pos, m_game->GetDeltaTime());

            CDKW_V3d position = game_part->m_camera->GetCameraPosition();
            CDKW_V3d target = game_part->m_camera->GetCameraTarget();
            F32 roll = game_part->m_camera->GetCameraRoll();

            m_game->GetCamera()->SetFOV(roll, 4.0f/3.0f);
            m_game->GetCamera()->LookAtInline(position, target, CDKW_V3d::YAXIS);
        }

        m_game->StopNarratorLine();
    } else {
        m_game->StopNarratorLine();

        m_unk14 = 0;
        m_unk10 = mission_no;

        if (m_game->GetMission(mission_no - 1).m_unk30 == 0u) {
            m_game->GetGuiManager()->GetGuiPtr("DREAM_DREAM")->menu->GetAnim()->GotoFrameLabel("DREAM_CLOUD");
            m_game->GetGuiManager()->GetGuiPtr("DREAM_DREAM")->menu->GetAnim()->Play();
            if (m_game->GetGuiManager()->IsActive("COOKIE_COOKIE") == FALSE && mission_no != 7) {
                m_game->GetGuiManager()->GetGuiPtr("COOKIE_COOKIE")->menu->Reset();
                m_game->GetGuiManager()->SetActive("COOKIE_COOKIE", 1);
                m_game->GetGuiManager()->SetVisible("COOKIE_COOKIE", 1);
            }

            if (mission_no == 7) {
                m_unk14 = 0;

                m_game->GetGuiManager()->GetGuiPtr("COOKIE_COOKIE")->menu->Reset();
                m_game->GetGuiManager()->SetActive("COOKIE_COOKIE", 0);
                m_game->GetGuiManager()->SetVisible("COOKIE_COOKIE", 0);
            }

            if (mission_no != 7) {
                CMission& mission = m_game->GetMission(mission_no - 1);
                if (mission.m_unk2C != 0) {
                    if (mission.IsSecondPassCompleted() != 0u) {
                        m_game->GetGuiManager()->GetGuiPtr("COOKIE_COOKIE")->menu->GetAnim()->GotoFrameLabel("COOKIE_NONPC");
                        m_game->GetGuiManager()->GetGuiPtr("COOKIE_COOKIE")->menu->GetAnim()->Play();
                    } else {
                        m_game->GetGuiManager()->GetGuiPtr("COOKIE_COOKIE")->menu->GetAnim()->GotoFrameLabel("COOKIE_NPC");
                        m_game->GetGuiManager()->GetGuiPtr("COOKIE_COOKIE")->menu->GetAnim()->Play();
                    }
                } else {
                    m_game->GetGuiManager()->GetGuiPtr("COOKIE_COOKIE")->menu->GetAnim()->GotoFrameLabel("COOKIE");
                    m_game->GetGuiManager()->GetGuiPtr("COOKIE_COOKIE")->menu->GetAnim()->Play();
                }
            }
        } else {
            m_game->GetGuiManager()->GetGuiPtr("DREAM_DREAM")->menu->GetAnim()->GotoFrameLabel("DREAM_NOCLOUD");
            m_game->GetGuiManager()->GetGuiPtr("DREAM_DREAM")->menu->GetAnim()->Play();

            if (m_game->GetGuiManager()->IsActive("COOKIE_COOKIE")) {
                m_game->GetGuiManager()->SetActive("COOKIE_COOKIE", 0);
                m_game->GetGuiManager()->SetVisible("COOKIE_COOKIE", 0);
            }
        }

        if (mission_no > 1 && mission_no < 7) {
            if (mission_no == 6 && m_game->GetMission(6).m_unk30 != 0u && !(m_game->m_unk8 & (1 << 4))) {
                m_game->GetGuiManager()->GetGuiPtr("DREAM_ARROW_LEFT")->menu->GetAnim()->GotoFrameLabel("L");
                m_game->GetGuiManager()->GetGuiPtr("DREAM_ARROW_LEFT")->menu->GetAnim()->Play();
            } else {
                m_game->GetGuiManager()->GetGuiPtr("DREAM_ARROW_LEFT")->menu->GetAnim()->GotoFrameLabel("LR");
                m_game->GetGuiManager()->GetGuiPtr("DREAM_ARROW_LEFT")->menu->GetAnim()->Play();
            }
        } else if (mission_no == 1) {
            m_game->GetGuiManager()->GetGuiPtr("DREAM_ARROW_LEFT")->menu->GetAnim()->GotoFrameLabel("R");
            m_game->GetGuiManager()->GetGuiPtr("DREAM_ARROW_LEFT")->menu->GetAnim()->Play();
        } else if (mission_no == 7) {
            m_game->GetGuiManager()->GetGuiPtr("DREAM_ARROW_LEFT")->menu->GetAnim()->GotoFrameLabel("L");
            m_game->GetGuiManager()->GetGuiPtr("DREAM_ARROW_LEFT")->menu->GetAnim()->Play();
        }

        switch (mission_no) {
            case 1:
                m_unk18 = (CEntityMesh*)m_game->m_entity_manager->GetEntity("CAR_710_803");
                break;
            case 2:
                m_unk18 = (CEntityMesh*)m_game->m_entity_manager->GetEntity("CAR_750_803");
                break;
            case 3:
                m_unk18 = (CEntityMesh*)m_game->m_entity_manager->GetEntity("CAR_730_803");
                break;
            case 4:
                m_unk18 = (CEntityMesh*)m_game->m_entity_manager->GetEntity("CAR_740_803");
                break;
            case 5:
                m_unk18 = (CEntityMesh*)m_game->m_entity_manager->GetEntity("CAR_760_803");
                break;
            case 6:
                m_unk18 = (CEntityMesh*)m_game->m_entity_manager->GetEntity("CAR_720_803");
                break;
            case 7:
                m_unk18 = (CEntityMesh*)m_game->m_entity_manager->GetEntity("CAR_770_803");
                break;
        }

        CMission& mission = m_game->GetMission(mission_no - 1);
        if (mission.m_unk30 != 0u) {
            PlayNarrator(mission_no);
            if (m_unk18 != NULL) {
                m_unk18->m_animation_star_controller->Play("ANIM3", 1, 1);
            }
        } else if (mission.m_unk2C == 0) {
            if (m_unk18 != NULL) {
                m_unk18->m_animation_star_controller->Play("ANIM2", 1, 1);
            }
        } else if (mission.m_unk2C != 0) {
            if (m_unk18 != NULL) {
                m_unk18->m_animation_star_controller->Play("ANIM1", 1, 1);
            }
        }

        if (m_unk18 != NULL) {
            m_unk18->SetPosition(CDKW_V3d::ZERO);
        }
    }
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
            if (m_game->m_unk210[2].m_unk30 != 0u) {
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
            if (m_game->m_unk210[3].m_unk30 != 0u) {
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
            if (m_game->m_unk210[4].m_unk30 != 0u) {
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
            if (m_game->m_unk210[5].m_unk30 != 0u) {
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

void CGuiScreenSettingEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    CDKW_V2d offset;
    offset = m_game->GetScreenOffset();

    CGuiBaseEventHandler::OnEvent(menu, event, unk);

    if (event == DKGUI::EVENT_0) {
        if (m_game->GetGuiManager()->m_unk20->GetState().m_unk0 == FALSE &&
            m_game->GetGuiManager()->m_unk24->GetState().m_unk0 == FALSE &&
            m_game->GetGuiManager()->m_unk28->GetState().m_unk0 == FALSE &&
            m_game->GetGuiManager()->m_unk2C->GetState().m_unk0 == FALSE &&
            m_unk18 == 1) {
            if (m_unk1C > 0.5f) {
                m_game->GetGuiManager()->GetGuiPtr("SCREEN_ADJUST_SCREEN")->menu->GetAnim()->GotoFrameLabel("ADJUST_NONE");
                m_game->GetGuiManager()->GetGuiPtr("SCREEN_ADJUST_SCREEN")->menu->GetAnim()->Play();
                m_unk18 = 0;
                m_unk1C = 0.0f;
            } else {
                m_unk1C += m_game->GetDeltaTime();
            }
        }
    } else if (event == DKGUI::EVENT_2 || event == DKGUI::EVENT_1) {
        if (!m_unkC) {
            m_unk10 = m_game->GetScreenOffset();
            if (event == DKGUI::EVENT_2) {
                PlaySoundInline("playsound SND_003_2D");
            }
        }
        m_unkC = TRUE;
        m_unk18 = 1;
        m_unk1C = 0.0f;
    }
    if (event == DKGUI::EVENT_12) {
        PlaySoundInline("playsound SND_003_2D");

        m_game->GetGuiManager()->GetGuiPtr("SCREEN_ADJUST_SCREEN")->menu->Reset();
        m_game->GetGuiManager()->SetActive("SCREEN_ADJUST_SCREEN", 0);
        m_game->GetGuiManager()->SetVisible("SCREEN_ADJUST_SCREEN", 0);

        m_unkC = FALSE;
    } else if (event == DKGUI::EVENT_13) {
        PlaySoundInline("playsound SND_004_2C");

        m_game->GetGuiManager()->GetGuiPtr("SCREEN_ADJUST_SCREEN")->menu->Reset();
        m_game->GetGuiManager()->SetActive("SCREEN_ADJUST_SCREEN", 0);
        m_game->GetGuiManager()->SetVisible("SCREEN_ADJUST_SCREEN", 0);

        m_unkC = FALSE;
        m_game->SetScreenOffset(m_unk10);
    } else if (event == DKGUI::EVENT_8) {
        if (m_unk18 == 0) {
            m_game->GetGuiManager()->GetGuiPtr("SCREEN_ADJUST_SCREEN")->menu->GetAnim()->GotoFrameLabel("ADJUST_UP");
            m_game->GetGuiManager()->GetGuiPtr("SCREEN_ADJUST_SCREEN")->menu->GetAnim()->Play();

            m_unk18 = 1;
            m_unk1C = 0.0f;
        }

        m_game->GetCamera()->GetBuffer()->GetHeight();
        CDKW_V2d offset_copy(offset);
        offset_copy.y -= 64.0f * m_game->GetDeltaTime();
        if (offset_copy.y >= -32.0f) {
            m_game->SetScreenOffset(offset_copy);
        }
    } else if (event == DKGUI::EVENT_9) {
        if (m_unk18 == 0) {
            m_game->GetGuiManager()->GetGuiPtr("SCREEN_ADJUST_SCREEN")->menu->GetAnim()->GotoFrameLabel("ADJUST_DOWN");
            m_game->GetGuiManager()->GetGuiPtr("SCREEN_ADJUST_SCREEN")->menu->GetAnim()->Play();

            m_unk18 = 1;
            m_unk1C = 0.0f;
        }

        m_game->GetCamera()->GetBuffer()->GetHeight();
        CDKW_V2d offset_copy(offset);
        offset_copy.y += 64.0f * m_game->GetDeltaTime();
        if (offset_copy.y <= 31.0f) {
            m_game->SetScreenOffset(offset_copy);
        }
    } else if (event == DKGUI::EVENT_10) {
        if (m_unk18 == 0) {
            m_game->GetGuiManager()->GetGuiPtr("SCREEN_ADJUST_SCREEN")->menu->GetAnim()->GotoFrameLabel("ADJUST_LEFT");
            m_game->GetGuiManager()->GetGuiPtr("SCREEN_ADJUST_SCREEN")->menu->GetAnim()->Play();

            m_unk18 = 1;
            m_unk1C = 0.0f;
        }

        m_game->GetCamera()->GetBuffer()->GetWidth();
        CDKW_V2d offset_copy(offset);
        offset_copy.x -= 64.0f * m_game->GetDeltaTime();
        if (offset_copy.x >= -32.0f) {
            m_game->SetScreenOffset(offset_copy);
        }
    } else if (event == DKGUI::EVENT_11) {
        if (m_unk18 == 0) {
            m_game->GetGuiManager()->GetGuiPtr("SCREEN_ADJUST_SCREEN")->menu->GetAnim()->GotoFrameLabel("ADJUST_RIGHT");
            m_game->GetGuiManager()->GetGuiPtr("SCREEN_ADJUST_SCREEN")->menu->GetAnim()->Play();

            m_unk18 = 1;
            m_unk1C = 0.0f;
        }

        m_game->GetCamera()->GetBuffer()->GetWidth();
        CDKW_V2d offset_copy(offset);
        offset_copy.x += 64.0f * m_game->GetDeltaTime();
        if (offset_copy.x <= 31.0f) {
            m_game->SetScreenOffset(offset_copy);
        }
    } else if (event == DKGUI::EVENT_3) {
        std::string str = (char*)unk;
        if (str == "exit") {
            // empty
        }
    }
}

CGuiNoPadEventHandler::CGuiNoPadEventHandler() : CGuiBaseEventHandler("GuiNoPadEventHandler") {

}

void CGuiNoPadEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    CGuiBaseEventHandler::OnEvent(menu, event, unk);

    if (event != DKGUI::EVENT_0) {
        return;
    }

    if (DKI::IInputEngine::GetDevice(0)->IsConnected() == TRUE) {
        m_game->GetGuiManager()->GetGuiPtr("NOPAD_NOPAD")->menu->Reset();
        m_game->GetGuiManager()->SetActive("NOPAD_NOPAD", 0);
        m_game->GetGuiManager()->SetVisible("NOPAD_NOPAD", 0);
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
        m_game->GetGuiManager()->SetActive("TITLE_TITLE", 0);
        m_game->FadeInit(1.0f, CGame::FADE_TYPE_3, 0, 0, 0, 0.0f);
        m_game->m_unk5038 = 0;

        m_game->ResetOpcodeBuffer();
        m_game->PushOpcodeValue(5);

        UnkGamePartAndReturnTypeInline();
    }
}
