#ifndef ENGINE_DISPLAY_CMATERIALANIMATIONPIPELINEGCN_H
#define ENGINE_DISPLAY_CMATERIALANIMATIONPIPELINEGCN_H

#include "engine/display/IMaterialAnimationPipeline.h"

namespace DKDSP {

// TODO
class CMaterialAnimationPipelineGCN : public IMaterialAnimationPipeline {
public:
    ~CMaterialAnimationPipelineGCN();

    virtual BOOL Create();

    void Release();
    void SetupAtomic(IClump* clump, int);
    void SetupAtomic(IAtomic* atomic);
};
REQUIRE_SIZE(CMaterialAnimationPipelineGCN, 0x4);

}

#endif
