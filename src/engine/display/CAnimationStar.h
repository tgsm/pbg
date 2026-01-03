#ifndef ENGINE_DISPLAY_CANIMATIONSTAR_H
#define ENGINE_DISPLAY_CANIMATIONSTAR_H

#include "engine/display/IAnimationStar.h"

namespace DKDSP {

class CAnimationStar : public IAnimationStar {
public:
    SAnimationStarValues* GetDefaultAnimationValues();
};

}

#endif
