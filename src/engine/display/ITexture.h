#ifndef ENGINE_DISPLAY_ITEXTURE_H
#define ENGINE_DISPLAY_ITEXTURE_H

#include "engine/display/CRaster.h"

// FIXME: Move these to a RW header.
enum RwTextureAddressMode {
    rwTEXTUREADDRESSNATEXTUREADDRESS = 0,
    rwTEXTUREADDRESSWRAP = 1,
    rwTEXTUREADDRESSMIRROR = 2,
    rwTEXTUREADDRESSCLAMP = 3,
    rwTEXTUREADDRESSBORDER = 4,
    rwTEXTUREADDRESSMODEFORCEENUMSIZEINT = 0x7FFFFFFF,
};
enum RwTextureFilterMode {
    rwFILTERNAFILTERMODE = 0,
    rwFILTERNEAREST = 1,
    rwFILTERLINEAR = 2,
    rwFILTERMIPNEAREST = 3,
    rwFILTERMIPLINEAR = 4,
    rwFILTERLINEARMIPNEAREST = 5,
    rwFILTERLINEARMIPLINEAR = 6,
    rwTEXTUREFILTERMODEFORCEENUMSIZEINT = 0x7FFFFFFF,
};

namespace DKDSP {

class ITexture {
public:
    virtual U32 GetSize() = 0;
    virtual RwTextureAddressMode GetAddressing() = 0;
    virtual RwTextureAddressMode GetAddressingU() = 0;
    virtual RwTextureAddressMode GetAddressingV() = 0;
    virtual void SetAddressing(RwTextureAddressMode) = 0;
    virtual void SetAddressingU(RwTextureAddressMode) = 0;
    virtual void SetAddressingV(RwTextureAddressMode) = 0;
    virtual RwTextureFilterMode GetFilterMode() = 0;
    virtual void SetFilterMode(RwTextureFilterMode) = 0;
    virtual CRaster* GetRaster() = 0;
    virtual void SetRaster(IRaster* raster) = 0;
    virtual void GenerateMipMap() = 0;
    virtual char* GetName() = 0;
    virtual void SetName(char* name) = 0;
    virtual char* GetMaskName() = 0;
    virtual void SetMaskName(char* mask) = 0;
    virtual int GetReferenceCount() = 0;
};

}

#endif
