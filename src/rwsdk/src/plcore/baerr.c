#include <cstddef>
#include <rwsdk/badevice.h>
#include <rwsdk/plcore/bamemory.h>
#include <rwsdk/plcore/baerr.h>
#include <stdarg.h>

static volatile RwModuleInfo errorModule;

void* _rwErrorOpen(void* unused, int offset) {
    errorModule.globalsOffset = offset;
    errorModule.numInstances++;
    (*(RwError*)((int)RwEngineInstance + errorModule.globalsOffset)).pluginID = 0;
    (*(RwError*)((int)RwEngineInstance + errorModule.globalsOffset)).errorCode = E_RW_NOERROR;

    return unused;
}

void* _rwErrorClose(void* a0) {
    errorModule.numInstances--;
    return a0;
}

RwError* RwErrorSet(RwError* error) {
    RwError* currentError = (RwError*)((int)RwEngineInstance + errorModule.globalsOffset);
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

    (*(RwError*)((int)RwEngineInstance + errorModule.globalsOffset)).errorCode = error->errorCode;
    return error;
}

RwError* RwErrorGet(RwError* error) {
    *error = *((RwError*)((int)RwEngineInstance + errorModule.globalsOffset));
    (*(RwError*)((int)RwEngineInstance + errorModule.globalsOffset)).pluginID = 0;
    (*(RwError*)((int)RwEngineInstance + errorModule.globalsOffset)).errorCode = E_RW_NOERROR;

    return error;
}

int _rwerror(int errorCode, ...) {
    va_list list;
    va_start(list, errorCode);
    va_end(list);

    return errorCode;
}
