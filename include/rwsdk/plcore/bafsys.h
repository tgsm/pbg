#ifndef RWSDK_PLCORE_BAFSYS_H
#define RWSDK_PLCORE_BAFSYS_H

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

RwFileFunctions* RwOsGetFileInterface(void);
int _rwFileSystemOpen(void);
void _rwFileSystemClose(void);

#ifdef __cplusplus
}
#endif

#endif
