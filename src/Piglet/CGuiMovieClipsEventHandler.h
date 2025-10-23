#ifndef PIGLET_CGUIMOVIECLIPSEVENTHANDLER_H
#define PIGLET_CGUIMOVIECLIPSEVENTHANDLER_H

#include "CEventGuiHandlers.h"

class CGuiMovieClipsEventHandler : public CGuiBaseEventHandler {
public:
    int m_unk10;
    U8 m_unk14[0x8];

public:
    CGuiMovieClipsEventHandler();
    virtual ~CGuiMovieClipsEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void*);

    int GetUnk10() { return m_unk10; }
};
REQUIRE_SIZE(CGuiMovieClipsEventHandler, 0x1C);

#endif
