#ifndef ENGINE_DISPLAY_CMATERIALANIMATIONPIPELINEGCN_H
#define ENGINE_DISPLAY_CMATERIALANIMATIONPIPELINEGCN_H

#include "types.h"

namespace DKDSP {

// TODO
class CMaterialAnimationPipelineGCN {
public:
    ~CMaterialAnimationPipelineGCN();

    virtual BOOL Create();

    void Release();
};
REQUIRE_SIZE(CMaterialAnimationPipelineGCN, 0x4);

}

#endif
