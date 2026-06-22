#include <stdarg.h>
#include <rwsdk/badevice.h>
#include <rwsdk/plcore/bamemory.h>
#include <rwsdk/plcore/baerr.h>

static RwModuleInfo errorModule;

void* _rwErrorOpen(void* unused, RwInt32 offset, RwInt32) {
    errorModule.globalsOffset = offset;
    errorModule.numInstances++;
    (*(RwError*)((RwInt32)RwEngineInstance + errorModule.globalsOffset)).pluginID = 0;
    (*(RwError*)((RwInt32)RwEngineInstance + errorModule.globalsOffset)).errorCode = E_RW_NOERROR;

    return unused;
}

void* _rwErrorClose(void* a0, RwInt32, RwInt32) {
    errorModule.numInstances--;
    return a0;
}

RwError* RwErrorSet(RwError* error) {
    RwError* currentError = (RwError*)((RwInt32)RwEngineInstance + errorModule.globalsOffset);
    if (currentError->pluginID != 0) {
        return error;
    }
    if (currentError->errorCode != E_RW_NOERROR) {
        return error;
    }
    if ((error->errorCode & 0x80000000) != 0) {
        currentError->pluginID = 0;
    } else {
        currentError->pluginID = error->pluginID;
    }

    (*(RwError*)((RwInt32)RwEngineInstance + errorModule.globalsOffset)).errorCode = error->errorCode;
    return error;
}

RwError* RwErrorGet(RwError* error) {
    *error = *((RwError*)((RwInt32)RwEngineInstance + errorModule.globalsOffset));
    (*(RwError*)((RwInt32)RwEngineInstance + errorModule.globalsOffset)).pluginID = 0;
    (*(RwError*)((RwInt32)RwEngineInstance + errorModule.globalsOffset)).errorCode = E_RW_NOERROR;

    return error;
}

RwInt32 _rwerror(RwInt32 errorCode, ...) {
    va_list list;
    va_start(list, errorCode);
    va_end(list);

    return errorCode;
}
