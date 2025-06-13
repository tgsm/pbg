#include "engine/input/IInputEngine.h"
#include "CEventGuiHandlers.h"
#include "CGuiScrapBookEventHandler.h"
#include "CGuiMovieClipsEventHandler.h"
#include "CGame.h"
#include "CGuiManager.h"
#include <iostream>

// Probably a fakematch, this is just here to be inlined in the constructor
void CGuiManager::PushEnterNameEvent(size_t& event_as_size) {
    event_as_size = reinterpret_cast<size_t>(new CGuiEnterNameEventHandler);
    if (event_as_size != 0) {
        reinterpret_cast<CGuiEnterNameEventHandler*>(event_as_size)->SetGame(m_game);
        reinterpret_cast<CGuiEnterNameEventHandler*>(event_as_size)->SetText("TESTSAVE");
        m_unkC.push_back(event_as_size);
    }
}

// Equivalent
CGuiManager::CGuiManager(CGame* game) {
    m_game = game;

    m_unk58 = 0;

    m_unk1C = DKI::IInputEngine::CreateInput("GUISTART", 0, 12);
    m_game->m_gui_engine->SetButtonStartAction(m_unk1C);
    m_unk50 = DKI::IInputEngine::CreateInput("GUIOK", 0, 19);
    m_game->m_gui_engine->SetButtonOkAction(m_unk50);
    m_unk54 = DKI::IInputEngine::CreateInput("GUICANCEL", 0, 20);
    m_game->m_gui_engine->SetButtonCancelAction(m_unk54);

    m_unk20 = DKI::IInputEngine::CreateInput("GUIUP", 0, 2);
    m_unk24 = DKI::IInputEngine::CreateInput("GUIDOWN", 0, 3);
    m_unk28 = DKI::IInputEngine::CreateInput("GUILEFT", 0, 1);
    m_unk2C = DKI::IInputEngine::CreateInput("GUIRIGHT", 0, 0);
    m_unk30 = DKI::IInputEngine::CreateInput("GUIUPAXE", 0, 0x12347);
    m_unk34 = DKI::IInputEngine::CreateInput("GUIDOWNAXE", 0, 0x12348);
    m_unk38 = DKI::IInputEngine::CreateInput("GUILEFTAXE", 0, 0x12345);
    m_unk3C = DKI::IInputEngine::CreateInput("GUIRIGHTAXE", 0, 0x12346);

    m_unk40 = DKI::IInputEngine::CreateCombo("GUIUPCOMBO", "GUIUP", "GUIUPAXE", 1);
    m_unk44 = DKI::IInputEngine::CreateCombo("GUIDOWNCOMBO", "GUIDOWN", "GUIDOWNAXE", 1);
    m_unk48 = DKI::IInputEngine::CreateCombo("GUILEFTCOMBO", "GUILEFT", "GUILEFTAXE", 1);
    m_unk4C = DKI::IInputEngine::CreateCombo("GUIRIGHTCOMBO", "GUIRIGHT", "GUIRIGHTAXE", 1);
    m_game->m_gui_engine->SetButtonDownAction(m_unk44);
    m_game->m_gui_engine->SetButtonUpAction(m_unk40);
    m_game->m_gui_engine->SetButtonLeftAction(m_unk48);
    m_game->m_gui_engine->SetButtonRightAction(m_unk4C);

    size_t event_as_size = 0;

    PushEvent<CGuiNullEventHandler>(event_as_size);
    PushEvent<CGuiPauseEventHandler>(event_as_size);
    PushEvent<CGuiMovieClipsEventHandler>(event_as_size);
    PushEvent<CGuiScrapBookEventHandler>(event_as_size);
    PushEvent<CGuiLoadCheckingMemoryCardEventHandler>(event_as_size);
    PushEvent<CGuiLoadingMemoryCardEventHandler>(event_as_size);
    PushEvent<CGuiLoadLoadSuccessEventHandler>(event_as_size);
    PushEvent<CGuiLoadLoadFailEventHandler>(event_as_size);
    PushEvent<CGuiCreateNewGameEventHandler>(event_as_size);
    PushEvent<CGuiLoadNoDataEventHandler>(event_as_size);
    PushEvent<CGuiLoadNoMemCardEventHandler>(event_as_size);
    PushEvent<CGuiChooseGameEventHandler>(event_as_size);
    PushEnterNameEvent(event_as_size);
    PushEvent<CGuiSaveCheckingMemorycardEventHandler>(event_as_size);
    PushEvent<CGuiSaveNoSpaceEventHandler>(event_as_size);
    PushEvent<CGuiSavingEventHandler>(event_as_size);
    PushEvent<CGuiSaveFailedEventHandler>(event_as_size);
    PushEvent<CGuiSaveOkEventHandler>(event_as_size);
    PushEvent<CGuiSaveNoMemCardEventHandler>(event_as_size);
    PushEvent<CGuiSaveFormatEventHandler>(event_as_size);
    PushEvent<CGuiSaveFormatSureEventHandler>(event_as_size);
    PushEvent<CGuiFormatOkEventHandler>(event_as_size);
    PushEvent<CGuiFormatFailedEventHandler>(event_as_size);
    PushEvent<CGuiSaveFormattingEventHandler>(event_as_size);
    PushEvent<CGuiOverwriteEventHandler>(event_as_size);
    PushEvent<CGuiOverwriteSureEventHandler>(event_as_size);
    PushEvent<CGuiDreamSelectEventHandler>(event_as_size);
    PushEvent<CGuiScreenSettingEventHandler>(event_as_size);
    PushEvent<CGuiNoPadEventHandler>(event_as_size);
    PushEvent<CGuiStartScreenEventHandler>(event_as_size);
    PushEvent<CGuiLoadNotFormattedMemCardEventHandler>(event_as_size);
    PushEvent<CGuiLoadMemoryCardUnuseableEventHandler>(event_as_size);
    PushEvent<CGuiSaveMemoryCardUnuseableEventHandler>(event_as_size);
    PushEvent<CGuiSaveReadyEventHandler>(event_as_size);
    PushEvent<CGuiLoadFileCorruptEventHandler>(event_as_size);
    PushEvent<CGuiLoadFileDeletingEventHandler>(event_as_size);
    PushEvent<CGuiLoadFileDeletingSuccessEventHandler>(event_as_size);
    PushEvent<CGuiLoadFileDeletingFailedEventHandler>(event_as_size);
    PushEvent<CGuiLoadWrongDeviceEventHandler>(event_as_size);
    PushEvent<CGuiSaveWrongDeviceEventHandler>(event_as_size);
    PushEvent<CGuiLoadCorruptMemcardEventHandler>(event_as_size);
    PushEvent<CGuiSaveCorruptMemcardEventHandler>(event_as_size);
}

// Incomplete
CGuiManager::~CGuiManager() {
    m_unk58 = 0;

    DKI::IInputEngine::DestroyInput(m_unk1C);
    DKI::IInputEngine::DestroyInput(m_unk50);
    DKI::IInputEngine::DestroyInput(m_unk54);
    DKI::IInputEngine::DestroyInput(m_unk20);
    DKI::IInputEngine::DestroyInput(m_unk24);
    DKI::IInputEngine::DestroyInput(m_unk2C);
    DKI::IInputEngine::DestroyInput(m_unk28);
    DKI::IInputEngine::DestroyInput(m_unk30);
    DKI::IInputEngine::DestroyInput(m_unk34);
    DKI::IInputEngine::DestroyInput(m_unk3C);
    DKI::IInputEngine::DestroyInput(m_unk38);
    DKI::IInputEngine::DestroyInput(m_unk40);
    DKI::IInputEngine::DestroyInput(m_unk44);
    DKI::IInputEngine::DestroyInput(m_unk4C);
    DKI::IInputEngine::DestroyInput(m_unk48);
    m_unk1C = NULL;
    m_unk50 = NULL;
    m_unk54 = NULL;
    m_unk20 = NULL;
    m_unk24 = NULL;
    m_unk2C = NULL;
    m_unk28 = NULL;
    m_unk30 = NULL;
    m_unk34 = NULL;
    m_unk3C = NULL;
    m_unk38 = NULL;
    m_unk40 = NULL;
    m_unk44 = NULL;
    m_unk4C = NULL;
    m_unk48 = NULL;
}
