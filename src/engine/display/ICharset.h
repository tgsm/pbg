#ifndef ENGINE_DISPLAY_ICHARSET_H
#define ENGINE_DISPLAY_ICHARSET_H

#include "engine/wrap/DKW_RGBA.h"
#include "engine/wrap/DKW_Charset.h"

namespace DKDSP {

class ICharset {
public:
    ICharset();

    virtual void Release() = 0;
    virtual void SetColors(const CDKW_RGBA& foreground, const CDKW_RGBA& background) = 0;
    virtual void SetForeGroundColor(F32 r, F32 g, F32 b, F32 a) = 0;
    virtual void SetForeGroundColorInt(U8 r, U8 g, U8 b, U8 a) = 0;
    virtual void SetBackGroundColor(F32 r, F32 g, F32 b, F32 a) = 0;
    virtual void SetBackGroundColorInt(U8 r, U8 g, U8 b, U8 a) = 0;
    virtual BOOL IsValid() = 0;
    virtual void PrintTextAbsolute(int, int, char* text) = 0;
    virtual void PrintText(int, int, DKW_PrintPos, char* text) = 0;
    virtual void Print(int, int, DKW_PrintPos, char*, ...) = 0;
    virtual void PrintAbsolute(int, int, char*, ...) = 0;
};

}

#endif
