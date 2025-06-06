#ifndef ENGINE_DISPLAY_ITEXTUREDICTIONARY_H
#define ENGINE_DISPLAY_ITEXTUREDICTIONARY_H

#include <string>
#include "engine/display/CImage.h"
#include "types.h"

namespace DKDSP {

class ITextureCallback;
class IRWStream;

class ITextureDictionary {
public:
    virtual void Release();
    virtual void Update();
    virtual U32 GetImageSize();
    virtual U32 GetRasterSize();
    virtual U32 GetTextureSize();
    virtual U32 GetSize();
    virtual U32 GetVectorSize();
    virtual void RegisterTextureCallback(ITextureCallback*);
    virtual CImage* LoadImageFromFile(char*, char*);
    virtual CImage* LoadImage(std::string, IRWStream*);
    virtual CImage* LoadImageFromTGA(std::string, IRWStream*);
    virtual U32 CopyImage(std::string, std::string); // what does this return?
    virtual CImage* CreateImage(std::string, int, int, int);
    virtual BOOL WriteImage(IImage*, char*);
    virtual BOOL WriteImage(IImage*, IRWStream*);
    virtual CImage* FindImage(std::string);
    virtual U32 GetNumberOfImages();
    virtual CImage* GetImage(int);
    virtual void RemoveImage(std::string);
    virtual void RemoveImage(IImage*);
    // FIXME: There are more virtuals after this, but this is all I need for now
};

}

#endif
