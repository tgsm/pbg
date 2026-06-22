#ifndef RWSDK_WORLD_BALIGHT_H
#define RWSDK_WORLD_BALIGHT_H

#include <rwsdk/baimage.h>
#include <rwsdk/batypehf.h>
#include <rwsdk/plcore/bamemory.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum RpLightType {
    rpNALIGHTTYPE = 0,
    rpLIGHTDIRECTIONAL = 1,
    rpLIGHTAMBIENT = 2,
    rpLIGHTPOINT = 128,
    rpLIGHTSPOT = 129,
    rpLIGHTSPOTSOFT = 130,
    rpLIGHTTYPEFORCEENUMSIZEINT = 0x7FFFFFFF,
} RpLightType;

typedef enum RpLightFlag {
    rpLIGHTLIGHTATOMICS = (1 << 0),
    rpLIGHTLIGHTWORLD = (1 << 1),
    rpLIGHTFLAGFORCEENUMSIZEINT = 0x7FFFFFFF,
} RpLightFlag;

typedef enum rpLightPrivateFlag {
    rpLIGHTPRIVATENOCHROMA = (1 << 0),
    rpLIGHTPRIVATEFLAGFORCEENUMSIZEINT = 0x7FFFFFFF,
} rpLightPrivateFlag;

typedef struct RpLight {
    RwObjectHasFrame object;
    RwReal radius;
    RwRGBAReal color;
    RwReal minusCosAngle;
    RwLinkList WorldSectorsInLight;
    RwLLLink inWorld;
    RwUInt16 lightFrame;
    RwUInt16 pad;
} RpLight; // size: 0x40

RpLight* RpLightSetRadius(RpLight* light, RwReal radius);
RpLight* RpLightSetColor(RpLight* light, RwRGBAReal* color);
RwReal RpLightGetConeAngle(RpLight* light);
RpLight* RpLightSetConeAngle(RpLight* light, RwReal coneAngle);
void RpLightRegisterPlugin(RwUInt32 size, RwUInt32 a1, void* a2, void* a3, void* a4);
RwBool RpLightDestroy(RpLight* light);
RpLight* RpLightCreate(RpLightType type);
void* _rpLightClose(void* a0);
void* _rpLightOpen(void* a0, RwUInt32 globalsOffset);

#ifdef __cplusplus
}
#endif

#endif
