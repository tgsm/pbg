#ifndef ENGINE_DISPLAY_CDISPLACEMENTPIPELINEGCN_H
#define ENGINE_DISPLAY_CDISPLACEMENTPIPELINEGCN_H

#include "engine/display/IDisplacementPipeline.h"

namespace DKDSP {

// TODO
class CDisplacementPipelineGCN : public IDisplacementPipeline {
public:
    ~CDisplacementPipelineGCN();

    virtual BOOL Create();
    virtual void Release();
};
REQUIRE_SIZE(CDisplacementPipelineGCN, 0x4);

}

#endif
