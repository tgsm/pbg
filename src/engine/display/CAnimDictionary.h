#ifndef ENGINE_DISPLAY_CANIMDICTIONARY_H
#define ENGINE_DISPLAY_CANIMDICTIONARY_H

#include "engine/display/IAnimDictionary.h"

namespace DKDSP {

// TODO
class CAnimDictionary : public IAnimDictionary {
public:
    virtual CAnimation* FindAnimation(std::string name);
};

}

#endif
