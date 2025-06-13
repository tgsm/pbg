#ifndef PIGLET_CGUIMOVIECLIPSEVENTHANDLER_H
#define PIGLET_CGUIMOVIECLIPSEVENTHANDLER_H

#include "CEventGuiHandlers.h"

class CGuiMovieClipsEventHandler : public CGuiBaseEventHandler {
private:
    U8 m_unk10[0xC];

public:
    CGuiMovieClipsEventHandler();
    virtual ~CGuiMovieClipsEventHandler() {}

    virtual void OnEvent(DKGUI::IGUIMenu* menu, DKGUI::EMENU_EVENT event, void*);
};
REQUIRE_SIZE(CGuiMovieClipsEventHandler, 0x1C);

#endif
