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
    virtual void func58() = 0;
    virtual void func5C() = 0;
    virtual void func60() = 0;
    virtual void func64() = 0;
    virtual void func68() = 0;
    virtual void func6C() = 0;
    virtual void func70() = 0;
    virtual void func74() = 0;
    virtual void func78() = 0;
    virtual void func7C() = 0;
    virtual void func80() = 0;
    virtual void func84() = 0;
    virtual void func88() = 0;
    virtual void func8C() = 0;
    virtual void func90() = 0;
    virtual void func94() = 0;
    virtual void func98() = 0;
    virtual CTexture* FindTexture(char* name) = 0;
    // FIXME: There are more virtuals after this, but this is all I need for now
};

}

#endif
