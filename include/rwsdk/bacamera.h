#ifndef RWSDK_BACAMERA_H
#define RWSDK_BACAMERA_H

#ifdef __cplusplus
extern "C" {
#endif

enum RwCameraProjection {
    rwNACAMERAPROJECTION = 0,
    rwPERSPECTIVE = 1,
    rwPARALLEL = 2,
    rwCAMERAPROJECTIONFORCEENUMSIZEINT = 0x7FFFFFFF,
};

typedef struct RwCamera {
    char unk0[0x198];
} RwCamera;

#ifdef __cplusplus
}
#endif

#endif
