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
    float radius;
    RwRGBAReal color;
    float minusCosAngle;
    RwLinkList WorldSectorsInLight;
    RwLLLink inWorld;
    unsigned short lightFrame;
    unsigned short pad;
} RpLight; // size: 0x40

RpLight* RpLightSetRadius(RpLight* light, float radius);
RpLight* RpLightSetColor(RpLight* light, RwRGBAReal* color);
float RpLightGetConeAngle(RpLight* light);
RpLight* RpLightSetConeAngle(RpLight* light, float coneAngle);
void RpLightRegisterPlugin(unsigned int size, unsigned int a1, void* a2, void* a3, void* a4);
int RpLightDestroy(RpLight* light);
RpLight* RpLightCreate(RpLightType type);
void* _rpLightClose(void* a0);
void* _rpLightOpen(void* a0, unsigned int globalsOffset);

#ifdef __cplusplus
}
#endif

#endif
