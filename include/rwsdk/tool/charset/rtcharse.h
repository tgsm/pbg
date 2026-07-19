#ifndef RWSDK_TOOL_CHARSET_RTCHARSE_H
#define RWSDK_TOOL_CHARSET_RTCHARSE_H

#include <rwsdk/baraster.h>
#include <rwsdk/baimras.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef RwRaster RtCharset;

typedef struct RtCharsetDesc {
    RwInt32 width;
    RwInt32 height;
    RwInt32 width_internal;
    RwInt32 height_internal;
    RwInt32 count;
    RwInt32 tileWidth;
    RwInt32 tileHeight;
} RtCharsetDesc;

RtCharset* RtCharsetCreate(RwRGBA*, RwRGBA*);
RwBool RtCharsetDestroy(RtCharset*);
RtCharset* RtCharsetPrintBuffered(RtCharset*, char*, int, int, int);
RtCharset* RtCharsetGetDesc(RtCharset*, RtCharsetDesc*);

#ifdef __cplusplus
}
#endif

#endif
