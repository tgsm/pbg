#ifndef ENGINE_GUI_CGUIMENU_H
#define ENGINE_GUI_CGUIMENU_H

#include "engine/gui/IGUIMenu.h"

namespace DKGUI {

class CGUIMenu : public IGUIMenu {
public:
    virtual ~CGUIMenu();

    virtual void Load();
    virtual void Unload();
    virtual void SetActive(BOOL active);
    virtual BOOL IsActive();
    virtual void SetVisible(BOOL visible);
    virtual BOOL IsVisible();
    virtual void SetPriority(int priority);
    virtual void Reset();
    virtual void Update(f32 dt_maybe);
    virtual void Render();
    virtual IEventGUIMenuCallBack* SetEventCallback(IEventGUIMenuCallBack* callback); // Returns the old callback
    virtual void* GetAnim(); // unknown return type
};

}

#endif
