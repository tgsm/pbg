#ifndef ENGINE_DISPLAY_CSCENE_H
#define ENGINE_DISPLAY_CSCENE_H

#include "engine/display/IScene.h"

namespace DKDSP {

class CScene : public IScene {
public:
    virtual IWarp* CreateWarp();
    virtual void RemoveWarp(IWarp*);
    virtual void RenderWarp(IWarp*);
    virtual CClump* CloneClump(std::string, IObjectDictionary*);
    virtual void SelectCamera(ICamera*);
    virtual void Clear(int, F32, F32, F32);
    virtual void BeginRender();
    virtual void Flush();
    virtual void EndRender();
    virtual void Flip(int);
};

}

#endif
