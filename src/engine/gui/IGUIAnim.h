#ifndef ENGINE_GUI_IGUIANIM_H
#define ENGINE_GUI_IGUIANIM_H

#include <string>

extern struct Rt2dObject;

class IGUIAnim {
public:
    virtual ~IGUIAnim();

    virtual Rt2dObject* GetScene() = 0;
    virtual void GotoFrameLabel(const std::string&) = 0;
    virtual void GotoFrame(const int&) = 0;
    virtual void Play() = 0;
    virtual void Stop() = 0;
    virtual void PressButton(int) = 0;
    virtual void PressButton(const std::string&) = 0;
};

#endif
