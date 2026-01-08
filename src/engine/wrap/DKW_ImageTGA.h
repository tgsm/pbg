#ifndef ENGINE_WRAP_DKW_IMAGETGA_H
#define ENGINE_WRAP_DKW_IMAGETGA_H

#include <rwsdk/baimage.h>
#include "engine/display/CImage.h"
#include "engine/display/CRWStream.h"

class CDKW_ImageTGA {
public:
    static BOOL CheckTGA(CDKW_Stream* stream);
    static CDKW_Image* Read(const char* path);
    static CDKW_Image* ReadFromStream(const CDKW_Stream* stream);
    static CDKW_Image* Write(CDKW_Image* image, const char* path);
};

#endif
