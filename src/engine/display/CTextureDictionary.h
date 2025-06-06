#ifndef ENGINE_DISPLAY_CTEXTUREDICTIONARY_H
#define ENGINE_DISPLAY_CTEXTUREDICTIONARY_H

#include "engine/display/ITextureDictionary.h"

namespace DKDSP {

class CTextureDictionary : ITextureDictionary {
public:
    virtual void Release() = 0;
    virtual void Update() = 0;
    virtual U32 GetImageSize() = 0;
    virtual U32 GetRasterSize() = 0;
    virtual U32 GetTextureSize() = 0;
    virtual U32 GetSize() = 0;
    virtual U32 GetVectorSize() = 0;
    virtual void RegisterTextureCallback(ITextureCallback*) = 0;
    virtual CImage* LoadImageFromFile(char*, char*) = 0;
    virtual CImage* LoadImage(std::string, IRWStream*) = 0;
    virtual CImage* LoadImageFromTGA(std::string, IRWStream*) = 0;
    virtual U32 CopyImage(std::string, std::string) = 0; // what does this return?
    virtual CImage* CreateImage(std::string, int, int, int) = 0;
    virtual BOOL WriteImage(IImage*, char*) = 0;
    virtual BOOL WriteImage(IImage*, IRWStream*) = 0;
    virtual CImage* FindImage(std::string) = 0;
    virtual U32 GetNumberOfImages() = 0;
    virtual CImage* GetImage(int) = 0;
    virtual void RemoveImage(std::string) = 0;
    virtual void RemoveImage(IImage*) = 0;
    // FIXME: There are more virtuals after this, but this is all I need for now
};

}

#endif
