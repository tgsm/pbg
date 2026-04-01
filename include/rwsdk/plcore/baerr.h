#ifndef RWSDK_PLCORE_BAERR_H
#define RWSDK_PLCORE_BAERR_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RwError {
    int pluginID; // RwCorePluginID?
    int errorCode;
} RwError;

typedef enum RwErrorCodeCommon {
    E_RW_NOERROR = 0x80000000,
} RwErrorCodeCommon;

#ifdef __cplusplus
}
#endif

#endif
