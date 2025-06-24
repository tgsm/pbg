#ifndef ENGINE_GUI_CGUIANIM_H
#define ENGINE_GUI_CGUIANIM_H

#include "engine/gui/IGUIAnim.h"

class CGUIAnim : public IGUIAnim {
public:
    U8 m_unk4[0x3C - 0x4];

public:
    virtual ~CGUIAnim();

    virtual Rt2dObject* GetScene();
    virtual void GotoFrameLabel(const std::string&);
    virtual void GotoFrame(const int&);
    virtual void Play();
    virtual void Stop();
    virtual void PressButton(int);
    virtual void PressButton(const std::string&);
};
REQUIRE_SIZE(CGUIAnim, 0x3C);

#endif
