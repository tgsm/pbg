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

// Unofficial names
#define RwThrowError(_pluginID, _errorCode) \
    RwError error; \
    error.pluginID = (_pluginID); \
    error.errorCode = _rwerror(_errorCode); \
    RwErrorSet(&error);
#define RwThrowErrorParams(_pluginID, _errorCode, ...) \
    RwError error; \
    error.pluginID = (_pluginID); \
    error.errorCode = _rwerror(_errorCode, ##__VA_ARGS__); \
    RwErrorSet(&error);

RwError* RwErrorSet(RwError* error);
int _rwerror(int errorCode, ...);

#ifdef __cplusplus
}
#endif

#endif
