#ifndef PIGLET_CEVENTGUIHANDLERS_H
#define PIGLET_CEVENTGUIHANDLERS_H

#include <string>
#include <vector>
#include "engine/wrap/DKW_V2d.h"
#include "engine/gui/IEventGUIMenuCallBack.h"
#include "entities/CEntityMesh.h"
#include "CGame.h"
#include "CGamePart.h"

class CGuiBaseEventHandler : public DKGUI::IEventGUIMenuCallBack {
protected:
    std::string m_type;
    CGame* m_game;
    BOOL m_unkC;

    std::string GetType() {
        return m_type;
    }

    void UnkGamePartAndReturnTypeInline() {
        CGamePart* game_part = m_game->GetGamePartPointer();
        game_part->m_unk4 = 0;
        m_game->SetCurrentRoomReturnType(CGame::RETURN_TYPE_0, -1);
    }

    void PlaySoundInline(const char* command) {
        std::string str = command;
        if (str.find("playsound") >= 0) {
            std::string str2;
            str2 = &command[strlen("playsound ")];
            DKSND::CSound2D* sound = m_game->m_sound_engine->PlaySound2D(str2, 1);
            if (sound != NULL) {
                sound->SetVolume(1.0f);
                sound->SetLayer(2);
            }
        }
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
public:
    std::vector<std::string> m_unk10;
    int m_unk1C;
    int m_unk20;
    F32 m_unk24;

public:
    CGuiChooseGameEventHandler();
    virtual ~CGuiChooseGameEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);

    void AddToList(std::string unk) {
        m_unk10.push_back(unk);
    }
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

    void SetFilename(std::string filename) {
        m_unk10.assign(filename, 0);
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
    int m_unk10;
    std::string m_unk14;

    void FrameLabelInline(DKGUI::IGUIMenu* menu) {
        char buf[32];
        sprintf(buf, "ENTER_NAME_%02d", m_unk10 + 1);
        menu->GetAnim()->GotoFrameLabel(buf);
    }

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
    int m_unk10;

public:
    CGuiSaveCheckingMemorycardEventHandler();
    virtual ~CGuiSaveCheckingMemorycardEventHandler() {
        m_unk10 = 0;
    }

    virtual void OnEvent(DKGUI::IGUIMenu*, DKGUI::EMENU_EVENT, void*);

    int GetUnk10() {
        return m_unk10;
    }

    void SetUnk10(int value) {
        m_unk10 = value;
    }
};
REQUIRE_SIZE(CGuiSaveCheckingMemorycardEventHandler, 0x14);

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
    int m_unk10;
    int m_unk14;
    CEntityMesh* m_unk18;

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
    int m_unk18;
    F32 m_unk1C;

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
