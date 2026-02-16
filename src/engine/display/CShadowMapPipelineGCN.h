#ifndef ENGINE_DISPLAY_CSHADOWMAPPIPELINEGCN_H
#define ENGINE_DISPLAY_CSHADOWMAPPIPELINEGCN_H

#include "engine/display/CRaster.h"
#include "engine/display/IShadowMapPipeline.h"

namespace DKDSP {

// TODO
class CShadowMapPipelineGCN : public IShadowMapPipeline {
public:
    CDKW_Raster* m_image_processing_raster;

public:
    CShadowMapPipelineGCN() {
        m_image_processing_raster = NULL;
    }
    ~CShadowMapPipelineGCN();

    virtual BOOL Create();
    virtual void Release();
    virtual void SetupClump(IClump* clump, int);
};
REQUIRE_SIZE(CShadowMapPipelineGCN, 0x8);

}

#endif
