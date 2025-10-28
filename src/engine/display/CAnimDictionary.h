#ifndef ENGINE_DISPLAY_CANIMDICTIONARY_H
#define ENGINE_DISPLAY_CANIMDICTIONARY_H

#include "engine/display/IAnimDictionary.h"

namespace DKDSP {

// TODO
class CAnimDictionary : public IAnimDictionary {
public:
    virtual void Release();
    virtual CAnimation* FindAnimation(std::string name);
    virtual CController* CreateController(std::string, IClump*, int);
    virtual void RemoveController(IController*);
};

}

#endif
