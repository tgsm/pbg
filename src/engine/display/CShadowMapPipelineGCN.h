#ifndef ENGINE_DISPLAY_CSHADOWMAPPIPELINEGCN_H
#define ENGINE_DISPLAY_CSHADOWMAPPIPELINEGCN_H

#include "engine/display/IShadowMapPipeline.h"

namespace DKDSP {

// TODO
class CShadowMapPipelineGCN : public IShadowMapPipeline {
public:
    U8 m_unk4[4];

public:
    ~CShadowMapPipelineGCN();

    virtual BOOL Create();
    virtual void Release();
    virtual void SetupClump(IClump* clump, int);
};
REQUIRE_SIZE(CShadowMapPipelineGCN, 0x8);

}

#endif
