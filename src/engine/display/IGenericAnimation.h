#ifndef ENGINE_DISPLAY_IGENERICANIMATION_H
#define ENGINE_DISPLAY_IGENERICANIMATION_H

#include <string>
#include "types.h"

namespace DKDSP {

enum EANIMATIONTYPE {
    ANIMATION_TYPE_ANM,
    ANIMATION_TYPE_DMA,
    ANIMATION_TYPE_TAN,
    ANIMATION_TYPE_EVN,
    ANIMATION_TYPE_4,
};

class IGenericAnimation {
public:
    IGenericAnimation();

    virtual EANIMATIONTYPE GetType() = 0;
    virtual F32 GetDuration() = 0;
    virtual std::string* GetName() = 0;
    virtual U32 GetSize() = 0;
};

}

#endif
