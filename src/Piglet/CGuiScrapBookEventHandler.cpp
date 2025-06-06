#include "CGuiScrapBookEventHandler.h"
#include "CGamePartScrapBook.h"
#include <cstring>
#include <iostream>

CGuiScrapBookEventHandler::CGuiScrapBookEventHandler() : CGuiBaseEventHandler("GuiScrapBookEventHandler") {
    m_unk10 = 0;
}

// Equivalent: regalloc, stack offsets
void CGuiScrapBookEventHandler::OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void* unk) {
    CGuiBaseEventHandler::OnEvent(menu, event, unk);

    char buf_30[32];
    char buf_50[32];
    char buf_70[32];
    char buf_90[32];
    char buf_b0[32];
    int iVar7;
    int iVar8;
    CGame* game;
    F32 unk_float;

    m_unk18 = 2;
    m_unk14 = 3;

    iVar7 = m_unk10 / m_unk14;
    iVar8 = m_unk10 - (m_unk10 / m_unk14) * m_unk14;
    game = m_game;

    if (m_game->m_unk5038 != 0) {
        unk_float = 0.0f;
    } else {
        unk_float = 0.0f;
        if (unk_float != m_game->m_fade_duration) {
            unk_float = m_game->m_unk502C / m_game->m_fade_duration;
        }
    }

    if (unk_float != 0.0f) {
        return;
    }

    switch (event) {
        case DKGUI::EVENT_1:
        case DKGUI::EVENT_2: {
            sprintf(buf_b0, "SCRAP_BOOK_%02d", m_unk10 + 1);
            menu->GetAnim()->GotoFrameLabel(buf_b0);

            m_game->m_gui_manager->SetActive("SCRAPBOOK_MAIN", 1);
            m_game->m_gui_manager->SetVisible("SCRAPBOOK_MAIN", 1);

            break;
        }
        case DKGUI::EVENT_12: {
            if (game->m_unk5038 != 2) {
                break;
            }

            if (((CGamePartScrapBook*)m_game->GetGamePartPointer())->IsVideoOpen(m_unk10)) {
                CGuiBaseEventHandler::OnEvent(menu, DKGUI::EVENT_3, (void*)"playsound SND_003_2D");

                CDKW_RGBA fade_color;
                fade_color = CDKW_RGBA(199, 226, 222, 0xFF);
                m_game->FadeInit(1.0f, CGame::FADE_TYPE_4, fade_color.m_r, fade_color.m_g, fade_color.m_b, 0.0f);
                m_game->m_unk5038 = 0;
                m_game->m_unk503C |= (1 << 4);

                m_game->ResetOpcodeBuffer();
                m_game->PushOpcodeValue(13);
                m_game->PushOpcodeValue(m_unk10 + 2);
                m_game->PushOpcodeValue(9);

                UnkGamePartAndReturnTypeInline();
            } else {
                CGuiBaseEventHandler::OnEvent(menu, DKGUI::EVENT_3, (void*)"playsound SND_012_2D");
            }

            break;
        }
        case DKGUI::EVENT_13: {
            if (game->m_unk5038 != 2) {
                break;
            }

            CGuiBaseEventHandler::OnEvent(menu, DKGUI::EVENT_3, (void*)"playsound SND_004_2C");

            CDKW_RGBA fade_color;
            fade_color = CDKW_RGBA(199, 226, 222, 0xFF);
            m_game->FadeInit(1.0f, CGame::FADE_TYPE_4, fade_color.m_r, fade_color.m_g, fade_color.m_b, 0.0f);
            m_game->m_unk5038 = 0;
            m_game->m_unk503C |= (1 << 4);

            m_game->ResetOpcodeBuffer();
            m_game->PushOpcodeValue(1);
            m_game->PushOpcodeValue(3);
            m_game->PushOpcodeValue(0);
            m_game->PushOpcodeValue(-11);
            m_game->PushOpcodeValue(0);
            m_game->PushOpcodeValue(-11);
            m_game->PushOpcodeValue(0);
            m_game->PushOpcodeValue(171);
            m_game->PushOpcodeValue(0);
            m_game->PushOpcodeValue(0);

            UnkGamePartAndReturnTypeInline();

            break;
        }
        case DKGUI::EVENT_8: {
            iVar7--;
            if (iVar7 < 0) {
                iVar7 = m_unk18 - 1;
            }
            m_unk10 = iVar8 + iVar7 * m_unk14;
            sprintf(buf_30, "SCRAP_BOOK_%02d", m_unk10 + 1);
            menu->GetAnim()->GotoFrameLabel(buf_30);
            break;
        }
        case DKGUI::EVENT_9: {
            iVar7++;
            if (iVar7 > m_unk18 - 1) {
                iVar7 = 0;
            }
            m_unk10 = iVar8 + iVar7 * m_unk14;
            sprintf(buf_50, "SCRAP_BOOK_%02d", m_unk10 + 1);
            menu->GetAnim()->GotoFrameLabel(buf_50);
            break;
        }
        case DKGUI::EVENT_10: {
            iVar8--;
            if (iVar8 < 0) {
                iVar8 = m_unk14 - 1;
            }
            m_unk10 = iVar8 + iVar7 * m_unk14;
            sprintf(buf_70, "SCRAP_BOOK_%02d", m_unk10 + 1);
            menu->GetAnim()->GotoFrameLabel(buf_70);
            break;
        }
        case DKGUI::EVENT_11: {
            iVar8++;
            if (iVar8 > m_unk14 - 1) {
                iVar8 = 0;
            }
            m_unk10 = iVar8 + iVar7 * m_unk14;
            sprintf(buf_90, "SCRAP_BOOK_%02d", m_unk10 + 1);
            menu->GetAnim()->GotoFrameLabel(buf_90);
            break;
        }
    }
}
