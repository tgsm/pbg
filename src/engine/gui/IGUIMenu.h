#ifndef ENGINE_GUI_IGUIMENU_H
#define ENGINE_GUI_IGUIMENU_H

#include "engine/gui/IEventGUIMenuCallBack.h"
#include "engine/gui/IGUIAnim.h"

namespace DKGUI {

class IGUIMenu {
public:
    virtual ~IGUIMenu();

    virtual void Load() = 0;
    virtual void Unload() = 0;
    virtual void SetActive(BOOL active) = 0;
    virtual BOOL IsActive() = 0;
    virtual void SetVisible(BOOL visible) = 0;
    virtual BOOL IsVisible() = 0;
    virtual void SetPriority(int priority) = 0;
    virtual void Reset() = 0;
    virtual void Update(F32 dt_maybe) = 0;
    virtual void Render() = 0;
    virtual IEventGUIMenuCallBack* SetEventCallback(IEventGUIMenuCallBack* callback) = 0; // Returns the old callback
    virtual IGUIAnim* GetAnim() = 0;
};

}

#endif
