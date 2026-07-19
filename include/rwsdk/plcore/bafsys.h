#ifndef RWSDK_PLCORE_BAFSYS_H
#define RWSDK_PLCORE_BAFSYS_H

#include <rwsdk/rwtypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RwFileFunctions {
    int (*rwfexist)(const char* path);
    void* (*rwfopen)(const char* path, const char* mode);
    int (*rwfclose)(void* stream);
    unsigned long (*rwfread)(void* ptr, unsigned long size, unsigned long n, void* stream);
    unsigned long (*rwfwrite)(const void* ptr, unsigned long size, unsigned long n, void* stream);
    char* (*rwfgets)(char* s, int size, void* stream);
    int (*rwfputs)(const char* s, void* stream);
    int (*rwfeof)(void* stream);
    int (*rwfseek)(void* stream, long offset, int whence);
    int (*rwfflush)(void* stream);
    long (*rwftell)(void* stream);
} RwFileFunctions; // size: 0x2C

#define RwFexist(path) RwEngineInstance->fileFuncs.rwfexist(path)
#define RwFopen(path, mode) RwEngineInstance->fileFuncs.rwfopen(path, mode)
#define RwFclose(stream) RwEngineInstance->fileFuncs.rwfclose(stream)
#define RwFread(ptr, size, n, stream) RwEngineInstance->fileFuncs.rwfread(ptr, size, n, stream)
#define RwFwrite(ptr, size, n, stream) RwEngineInstance->fileFuncs.rwfwrite(ptr, size, n, stream)
#define RwFgets(s, size, stream) RwEngineInstance->fileFuncs.rwfgets(s, size, stream)
#define RwFputs(s, stream) RwEngineInstance->fileFuncs.rwfputs(s, stream)
#define RwFeof(stream) RwEngineInstance->fileFuncs.rwfeof(stream)
#define RwFseek(stream, offset, whence) RwEngineInstance->fileFuncs.rwfseek(stream, offset, whence)
#define RwFflush(stream) RwEngineInstance->fileFuncs.rwfflush(stream)
#define RwFtell(stream) RwEngineInstance->fileFuncs.rwftell(stream)

RwFileFunctions* RwOsGetFileInterface(void);
RwBool _rwFileSystemOpen(void);
void _rwFileSystemClose(void);

#ifdef __cplusplus
}
#endif

#endif
