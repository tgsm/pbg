#ifndef ENGINE_DISPLAY_IANIMATIONSTARENTRY_H
#define ENGINE_DISPLAY_IANIMATIONSTARENTRY_H

#include <string>
#include "engine/display/CAnimation.h"

namespace DKDSP {

class IAnimationStarEntry {
public:
    virtual int GetNumberOfTargetAnimations();
    virtual std::string* GetTargetAnimationName();
    virtual int GetSize();
    virtual std::string* GetName();
    virtual void SetName(std::string name);
    virtual int GetNumberOfAnimations();
    virtual CAnimation* GetAnimation(int);
    virtual int GetAnimationNode(int);
    virtual F32 GetAnimationDuration(EANIMATIONTYPE type);
};

}

#endif
