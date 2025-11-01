#ifndef PIGLET_CEVENTGUIHANDLERS_H
#define PIGLET_CEVENTGUIHANDLERS_H

#include <string>
#include <vector>
#include "engine/wrap/DKW_V2d.h"
#include "engine/gui/IEventGUIMenuCallBack.h"
#include "CGame.h"
#include "CGamePart.h"

class CGuiBaseEventHandler : public DKGUI::IEventGUIMenuCallBack {
protected:
    std::string m_unk4;
    CGame* m_game;
    BOOL m_unkC;

    void UnkGamePartAndReturnTypeInline() {
        CGamePart* game_part = m_game->GetGamePartPointer();
        game_part->m_unk4 = 0;
        m_game->SetCurrentRoomReturnType(CGame::RETURN_TYPE_0, -1);
    }

public:
    CGuiBaseEventHandler(const std::string& unk);
    virtual ~CGuiBaseEventHandler();

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);

    void SetGame(CGame* game) { m_game = game; }
};

class CGuiNullEventHandler : public CGuiBaseEventHandler {
public:
    CGuiNullEventHandler();
    virtual ~CGuiNullEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);
};

class CGuiPauseEventHandler : public CGuiBaseEventHandler {
public:
    CGuiPauseEventHandler();
    virtual ~CGuiPauseEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);
};

class CGuiLoadCheckingMemoryCardEventHandler : public CGuiBaseEventHandler {
public:
    CGuiLoadCheckingMemoryCardEventHandler();
    virtual ~CGuiLoadCheckingMemoryCardEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);
};

class CGuiLoadNoDataEventHandler : public CGuiBaseEventHandler {
public:
    CGuiLoadNoDataEventHandler();
    virtual ~CGuiLoadNoDataEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);
};

class CGuiLoadNoMemCardEventHandler : public CGuiBaseEventHandler {
public:
    CGuiLoadNoMemCardEventHandler();
    virtual ~CGuiLoadNoMemCardEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);
};

class CGuiLoadNotFormattedMemCardEventHandler : public CGuiBaseEventHandler {
public:
    CGuiLoadNotFormattedMemCardEventHandler();
    virtual ~CGuiLoadNotFormattedMemCardEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);
};

class CGuiChooseGameEventHandler : public CGuiBaseEventHandler {
private:
    std::vector<std::string> m_unk10;
    U8 m_unk1C[0xC];

public:
    CGuiChooseGameEventHandler();
    virtual ~CGuiChooseGameEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);
};
REQUIRE_SIZE(CGuiChooseGameEventHandler, 0x28);

class CGuiLoadingMemoryCardEventHandler : public CGuiBaseEventHandler {
private:
    std::string m_unk10;

public:
    CGuiLoadingMemoryCardEventHandler();
    virtual ~CGuiLoadingMemoryCardEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);

    std::string GetFilename() {
        return m_unk10;
    }
};

class CGuiLoadLoadSuccessEventHandler : public CGuiBaseEventHandler {
public:
    CGuiLoadLoadSuccessEventHandler();
    virtual ~CGuiLoadLoadSuccessEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);
};

class CGuiLoadLoadFailEventHandler : public CGuiBaseEventHandler {
public:
    CGuiLoadLoadFailEventHandler();
    virtual ~CGuiLoadLoadFailEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);
};

class CGuiLoadMemoryCardUnuseableEventHandler : public CGuiBaseEventHandler {
public:
    CGuiLoadMemoryCardUnuseableEventHandler();
    virtual ~CGuiLoadMemoryCardUnuseableEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);
};

class CGuiCreateNewGameEventHandler : public CGuiBaseEventHandler {
public:
    CGuiCreateNewGameEventHandler();
    virtual ~CGuiCreateNewGameEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);
};

class CGuiEnterNameEventHandler : public CGuiBaseEventHandler {
public:
    U32 m_unk10;
    std::string m_unk14;

public:
    CGuiEnterNameEventHandler();
    virtual ~CGuiEnterNameEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);

    std::string GetText() {
        return m_unk14;
    }

    void SetText(std::string text) {
        m_unk14.assign(text, 0);
    }
};

class CGuiSaveCheckingMemorycardEventHandler : public CGuiBaseEventHandler {
private:
    U32 m_unk10;

public:
    CGuiSaveCheckingMemorycardEventHandler();
    virtual ~CGuiSaveCheckingMemorycardEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);
};

class CGuiSaveNoSpaceEventHandler : public CGuiBaseEventHandler {
public:
    CGuiSaveNoSpaceEventHandler();
    virtual ~CGuiSaveNoSpaceEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);
};

class CGuiSavingEventHandler : public CGuiBaseEventHandler {
public:
    CGuiSavingEventHandler();
    virtual ~CGuiSavingEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);
};

class CGuiSaveFailedEventHandler : public CGuiBaseEventHandler {
public:
    CGuiSaveFailedEventHandler();
    virtual ~CGuiSaveFailedEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);
};

class CGuiSaveReadyEventHandler : public CGuiBaseEventHandler {
public:
    CGuiSaveReadyEventHandler();
    virtual ~CGuiSaveReadyEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);
};

class CGuiLoadFileCorruptEventHandler : public CGuiBaseEventHandler {
public:
    CGuiLoadFileCorruptEventHandler();
    virtual ~CGuiLoadFileCorruptEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);
};

class CGuiLoadFileDeletingEventHandler : public CGuiBaseEventHandler {
public:
    CGuiLoadFileDeletingEventHandler();
    virtual ~CGuiLoadFileDeletingEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);
};

class CGuiLoadFileDeletingSuccessEventHandler : public CGuiBaseEventHandler {
public:
    CGuiLoadFileDeletingSuccessEventHandler();
    virtual ~CGuiLoadFileDeletingSuccessEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);
};

class CGuiLoadFileDeletingFailedEventHandler : public CGuiBaseEventHandler {
public:
    CGuiLoadFileDeletingFailedEventHandler();
    virtual ~CGuiLoadFileDeletingFailedEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);
};

class CGuiLoadWrongDeviceEventHandler : public CGuiBaseEventHandler {
public:
    CGuiLoadWrongDeviceEventHandler();
    virtual ~CGuiLoadWrongDeviceEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);
};

class CGuiSaveWrongDeviceEventHandler : public CGuiBaseEventHandler {
public:
    CGuiSaveWrongDeviceEventHandler();
    virtual ~CGuiSaveWrongDeviceEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);
};

class CGuiLoadCorruptMemcardEventHandler : public CGuiBaseEventHandler {
public:
    CGuiLoadCorruptMemcardEventHandler();
    virtual ~CGuiLoadCorruptMemcardEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);
};

class CGuiSaveCorruptMemcardEventHandler : public CGuiBaseEventHandler {
public:
    CGuiSaveCorruptMemcardEventHandler();
    virtual ~CGuiSaveCorruptMemcardEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);
};

class CGuiSaveMemoryCardUnuseableEventHandler : public CGuiBaseEventHandler {
public:
    CGuiSaveMemoryCardUnuseableEventHandler();
    virtual ~CGuiSaveMemoryCardUnuseableEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);
};

class CGuiSaveOkEventHandler : public CGuiBaseEventHandler {
public:
    CGuiSaveOkEventHandler();
    virtual ~CGuiSaveOkEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);
};

class CGuiSaveNoMemCardEventHandler : public CGuiBaseEventHandler {
public:
    CGuiSaveNoMemCardEventHandler();
    virtual ~CGuiSaveNoMemCardEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);
};

class CGuiSaveFormatEventHandler : public CGuiBaseEventHandler {
public:
    CGuiSaveFormatEventHandler();
    virtual ~CGuiSaveFormatEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);
};

class CGuiSaveFormatSureEventHandler : public CGuiBaseEventHandler {
public:
    CGuiSaveFormatSureEventHandler();
    virtual ~CGuiSaveFormatSureEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);
};

class CGuiSaveFormattingEventHandler : public CGuiBaseEventHandler {
public:
    CGuiSaveFormattingEventHandler();
    virtual ~CGuiSaveFormattingEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);
};

class CGuiFormatOkEventHandler : public CGuiBaseEventHandler {
public:
    CGuiFormatOkEventHandler();
    virtual ~CGuiFormatOkEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);
};

class CGuiFormatFailedEventHandler : public CGuiBaseEventHandler {
public:
    CGuiFormatFailedEventHandler();
    virtual ~CGuiFormatFailedEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);
};

class CGuiOverwriteEventHandler : public CGuiBaseEventHandler {
public:
    CGuiOverwriteEventHandler();
    virtual ~CGuiOverwriteEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);
};

class CGuiOverwriteSureEventHandler : public CGuiBaseEventHandler {
public:
    CGuiOverwriteSureEventHandler();
    virtual ~CGuiOverwriteSureEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);
};

class CGuiDreamSelectEventHandler : public CGuiBaseEventHandler {
private:
    U32 m_unk10;
    U32 m_unk14;
    U32 m_unk18;

public:
    CGuiDreamSelectEventHandler();
    virtual ~CGuiDreamSelectEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);

    void GotoMission(int);
    void PlayNarrator(int mission_no);
};

class CGuiScreenSettingEventHandler : public CGuiBaseEventHandler {
private:
    CDKW_V2d m_unk10;
    U8 m_unk18[8];

public:
    CGuiScreenSettingEventHandler();
    virtual ~CGuiScreenSettingEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);
};
REQUIRE_SIZE(CGuiScreenSettingEventHandler, 0x20);

class CGuiNoPadEventHandler : public CGuiBaseEventHandler {
public:
    CGuiNoPadEventHandler();
    virtual ~CGuiNoPadEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);
};

class CGuiStartScreenEventHandler : public CGuiBaseEventHandler {
public:
    CGuiStartScreenEventHandler();
    virtual ~CGuiStartScreenEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);
};

#endif
