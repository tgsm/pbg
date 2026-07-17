#ifndef ENGINE_WRAP_DKW_CHARSET_H
#define ENGINE_WRAP_DKW_CHARSET_H

#include <rwsdk/baimage.h>
#include <rwsdk/baraster.h>
#include <stdarg.h>

// FIXME: This needs a home.
extern "C" {
typedef RwRaster RtCharset;
RtCharset* RtCharsetCreate(RwRGBA*, RwRGBA*);
}

// TODO
enum DKW_PrintPos {

};

class CDKW_Charset {
public:
    CDKW_Charset();
    ~CDKW_Charset();

    void PrintAbsolute(int, int, char*);
    void Print(int, int, DKW_PrintPos, char*, ...);
    void Print(int, int, DKW_PrintPos, int, char*, va_list* args);
    void PrintAbsolute(int, int, int, char*, va_list* args);

public:
    RtCharset* m_rw_charset;
};

#endif
