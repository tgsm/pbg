#ifndef ENGINE_DISPLAY_CSCENE_H
#define ENGINE_DISPLAY_CSCENE_H

#include "engine/display/IScene.h"

namespace DKDSP {

class CScene : public IScene {
public:
    virtual void SelectCamera(ICamera*);
    virtual void Clear(int, f32, f32, f32);
    virtual void BeginRender();
    virtual void Flush();
    virtual void EndRender();
    virtual void Flip(int);
};

}

#endif
