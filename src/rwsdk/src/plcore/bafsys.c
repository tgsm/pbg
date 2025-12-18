#include <rwsdk/badevice.h>
#include <rwsdk/plcore/bafsys.h>
#include <stddef.h>
#include <stdio.h>

RwFileFunctions* RwOsGetFileInterface(void) {
    return &RwEngineInstance->fileFuncs;
}

static int rwfexist(const char* path) {
    void* file = RwEngineInstance->fileFuncs.rwfopen(path, "rb");
    int result = file != NULL;
    if (file != NULL) {
        RwEngineInstance->fileFuncs.rwfclose(file);
    }
    return result;
}

int _rwFileSystemOpen(void) {
    RwEngineInstance->fileFuncs.rwfexist = rwfexist;
    RwEngineInstance->fileFuncs.rwfopen = fopen;
    RwEngineInstance->fileFuncs.rwfclose = fclose;
    RwEngineInstance->fileFuncs.rwfread = fread;
    RwEngineInstance->fileFuncs.rwfwrite = fwrite;
    RwEngineInstance->fileFuncs.rwfgets = fgets;
    RwEngineInstance->fileFuncs.rwfputs = fputs;
    RwEngineInstance->fileFuncs.rwfeof = feof;
    RwEngineInstance->fileFuncs.rwfseek = fseek;
    RwEngineInstance->fileFuncs.rwfflush = fflush;
    RwEngineInstance->fileFuncs.rwftell = ftell;
    return 1;
}

void _rwFileSystemClose() {

}
