#ifndef ENGINE_DISPLAY_IANIMATIONSTARENTRY_H
#define ENGINE_DISPLAY_IANIMATIONSTARENTRY_H

#include <string>
#include "engine/display/CAnimation.h"

namespace DKDSP {

class IAnimationStarEntry {
public:
    IAnimationStarEntry();

    virtual int GetNumberOfTargetAnimations() = 0;
    virtual std::string* GetTargetAnimationName() = 0;
    virtual int GetSize() = 0;
    virtual std::string* GetName() = 0;
    virtual void SetName(std::string name) = 0;
    virtual int GetNumberOfAnimations() = 0;
    virtual CAnimation* GetAnimation(int) = 0;
    virtual int GetAnimationNode(int) = 0;
    virtual F32 GetAnimationDuration(EANIMATIONTYPE type) = 0;
};

}

#endif
