#ifndef ENGINE_DISPLAY_IMATERIALANIMATIONPIPELINE_H
#define ENGINE_DISPLAY_IMATERIALANIMATIONPIPELINE_H

#include "engine/display/IPipeline.h"

namespace DKDSP {

class IMaterialAnimationPipeline : public IPipeline {
public:
    IMaterialAnimationPipeline();

    virtual BOOL Create();
};
REQUIRE_SIZE(IMaterialAnimationPipeline, 0x4);

}

#endif
