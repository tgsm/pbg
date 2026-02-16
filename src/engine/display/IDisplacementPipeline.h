#ifndef ENGINE_DISPLAY_IDISPLACEMENTPIPELINE_H
#define ENGINE_DISPLAY_IDISPLACEMENTPIPELINE_H

#include "engine/display/IPipeline.h"

namespace DKDSP {

// TODO
class IDisplacementPipeline : public IPipeline {
public:
    IDisplacementPipeline();

    virtual BOOL Create();
    virtual void Release();
};
REQUIRE_SIZE(IDisplacementPipeline, 0x4);

}

#endif
