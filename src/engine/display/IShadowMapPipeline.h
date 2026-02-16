#ifndef ENGINE_DISPLAY_ISHADOWMAPPIPELINE_H
#define ENGINE_DISPLAY_ISHADOWMAPPIPELINE_H

#include "engine/display/IPipeline.h"
#include "engine/display/CClump.h"

namespace DKDSP {

class IShadowMapPipeline : public IPipeline {
public:
    virtual BOOL Create() = 0;
    virtual void Release() = 0;
    virtual void SetupClump(IClump* clump, int) = 0;
};
REQUIRE_SIZE(IShadowMapPipeline, 0x4);

}

#endif
