#ifndef ENGINE_DISPLAY_CDISPLACEMENTPIPELINEGCN_H
#define ENGINE_DISPLAY_CDISPLACEMENTPIPELINEGCN_H

#include "types.h"

namespace DKDSP {

// TODO
class CDisplacementPipelineGCN {
public:
    ~CDisplacementPipelineGCN();

    virtual BOOL Create();
    virtual void Release();
};
REQUIRE_SIZE(CDisplacementPipelineGCN, 0x4);

}

#endif
