#ifndef ENGINE_DISPLAY_CSCENE_H
#define ENGINE_DISPLAY_CSCENE_H

#include "engine/display/IScene.h"

namespace DKDSP {

class CScene : public IScene {
public:
    virtual CMirror* CreateMirror(U32 texture_width, U32 texture_height, U32 texture_depth_maybe);
    virtual void EnableMirrorRendering(IMirror* mirror, BOOL enable);
    virtual void AssociateClumpToMirror(IMirror* mirror, IClump* clump);
    virtual IWarp* CreateWarp();
    virtual void RemoveWarp(IWarp*);
    virtual void RenderWarp(IWarp*);
    virtual CClump* CloneClump(std::string, IObjectDictionary*);
    virtual void SelectCamera(ICamera*);
    virtual void Clear(int, F32, F32, F32);
    virtual void BeginRender();
    virtual void RenderClump(IClump* clump);
    virtual void Flush();
    virtual void EndRender();
    virtual void Flip(int);
};

}

#endif
