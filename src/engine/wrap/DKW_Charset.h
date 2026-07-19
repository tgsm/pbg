#ifndef ENGINE_WRAP_DKW_CHARSET_H
#define ENGINE_WRAP_DKW_CHARSET_H

#include <rwsdk/tool/charset/rtcharse.h>
#include <stdarg.h>

// TODO
enum DKW_PrintPos {
    PRINT_POS_UNK0 = (1 << 0),
    PRINT_POS_UNK1 = (1 << 1),
    PRINT_POS_UNK2 = (1 << 2),
    PRINT_POS_UNK3 = (1 << 3),
};

class CDKW_Charset {
public:
    CDKW_Charset();
    ~CDKW_Charset();

    int PrintAbsolute(int, int, char*);
    int Print(int, int, DKW_PrintPos, char*, ...);
    int Print(int, int, DKW_PrintPos, int, char*, va_list* args);
    int PrintAbsolute(int, int, int, char*, va_list* args);

public:
    RtCharset* m_rw_charset;
};

#endif
