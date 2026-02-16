#ifndef ENGINE_DISPLAY_CTEXTUREDICTIONARY_H
#define ENGINE_DISPLAY_CTEXTUREDICTIONARY_H

#include <vector>
#include "engine/display/ITextureCallback.h"
#include "engine/display/ITextureDictionary.h"

namespace DKDSP {

class CEngine;
class CDKW_TexDictionary;

// TODO
struct SIMAGEENTRY {};
struct SRASTERENTRY {};

class CTextureDictionary : ITextureDictionary {
public:
    CEngine* m_engine;
    ITextureCallback* m_texture_callback;
    std::vector<SIMAGEENTRY> m_image_entries;
    std::vector<SRASTERENTRY> m_raster_entries;
    std::vector<CTexture*> m_textures24;
    CDKW_TexDictionary* m_wrap_texdictionary;
    std::vector<CTexture*> m_textures34;
    BOOL m_unk40;

public:
    CTextureDictionary() {
        m_engine = NULL;
        m_wrap_texdictionary = NULL;
        m_texture_callback = NULL;
        m_unk40 = FALSE;
    }
    ~CTextureDictionary();

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
    virtual void func58();
    virtual void func5C();
    virtual void func60();
    virtual void func64();
    virtual void func68();
    virtual void func6C();
    virtual void func70();
    virtual void func74();
    virtual void func78();
    virtual void func7C();
    virtual void func80();
    virtual void func84();
    virtual void func88();
    virtual CTexture* LoadTextureFromFile(char* name, U32, U32);
    virtual void func90();
    virtual void func94();
    virtual void func98();
    virtual CTexture* FindTexture(char* name);
    // FIXME: There are more virtuals after this, but this is all I need for now

    void Init();

    static void RegisterCallbacks();
};
REQUIRE_SIZE(CTextureDictionary, 0x44);

}

#endif
