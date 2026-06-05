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
    E_RW_BADENGINESTATE = 0x80000001,
    E_RW_INVIMAGEDEPTH = 0x80000008,
    E_RW_INVRASTERDEPTH = 0x8000000C,
    E_RW_INVRASTERFORMAT = 0x8000000D,
    E_RW_NOMEM = 0x80000013,
    E_RW_NULLP = 0x80000016,
} RwErrorCodeCommon;

// Unofficial names
#define RwThrowError(_pluginID, _errorCode) \
    { \
        RwError error; \
        error.pluginID = (_pluginID); \
        error.errorCode = _rwerror(_errorCode); \
        RwErrorSet(&error); \
    }
#define RwThrowErrorParams(_pluginID, _errorCode, ...) \
    { \
        RwError error; \
        error.pluginID = (_pluginID); \
        error.errorCode = _rwerror(_errorCode, ##__VA_ARGS__); \
        RwErrorSet(&error); \
    }

void* _rwErrorOpen(void* unused, int offset);
void* _rwErrorClose(void*);
RwError* RwErrorSet(RwError* error);
int _rwerror(int errorCode, ...);

#ifdef __cplusplus
}
#endif

#endif
