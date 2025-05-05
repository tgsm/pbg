#pragma once on

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
    virtual u32 GetImageSize();
    virtual u32 GetRasterSize();
    virtual u32 GetTextureSize();
    virtual u32 GetSize();
    virtual u32 GetVectorSize();
    virtual void RegisterTextureCallback(ITextureCallback*);
    virtual CImage* LoadImageFromFile(char*, char*);
    virtual CImage* LoadImage(std::string, IRWStream*);
    virtual CImage* LoadImageFromTGA(std::string, IRWStream*);
    virtual u32 CopyImage(std::string, std::string); // what does this return?
    virtual CImage* CreateImage(std::string, int, int, int);
    virtual BOOL WriteImage(IImage*, char*);
    virtual BOOL WriteImage(IImage*, IRWStream*);
    virtual CImage* FindImage(std::string);
    virtual u32 GetNumberOfImages();
    virtual CImage* GetImage(int);
    virtual void RemoveImage(std::string);
    virtual void RemoveImage(IImage*);
    // FIXME: There are more virtuals after this, but this is all I need for now
};

}
