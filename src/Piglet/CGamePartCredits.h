#ifndef PIGLET_CGAMEPARTCREDITS_H
#define PIGLET_CGAMEPARTCREDITS_H

#include <string>
#include <vector>
#include "engine/display/CEngine.h"
#include "engine/input/CInput.h"
#include "engine/sound/CSound2D.h"
#include "engine/xmd/CChunkIterator.h"
#include "engine/xmd/CXmdFile.h"
#include "engine/wrap/DKW_RGBA.h"
#include "CGamePart.h"

class CGame;

class CGamePartCredits : public CGamePart {
public:
    struct Color {
        Color() {
            id = -1;
            color = DKW_RGBA_WHITE;
        }

        int id;
        CDKW_RGBA color;
    };

    struct Entry {
        Entry() {
            id = -1;
            size = -1;
            color_id = -1;
        }

        int id;
        int size;
        int color_id;
    };

    CGame* m_game;
    int m_unk10;
    void* m_credits_xmd_data;
    DkXmd::CXmdFile* m_credits_xmd;
    DkXmd::CChunkIterator* m_credits_xmd_iter;
    DKI::IInput* m_unk20;
    DKI::IInput* m_unk24;
    F32 m_start_time;
    int m_raster_width;
    int m_raster_height;
    BOOL m_unk34;
    F32 m_speed;
    F32 m_interline;
    F32 m_black_strip_height;
    F32 m_fade_strip_height;
    U32 m_unk48;
    DKSND::CSound2D* m_unk4C;
    U32 m_size;
    U32 m_color_id;
    DKDSP::CIm2DBatch* m_batches[4];
    std::vector<Color*> m_unk68;
    std::vector<Entry*> m_unk74;
    std::vector<std::string*> m_unk80;

public:
    CGamePartCredits(CGame* game, int);
    virtual ~CGamePartCredits();

    virtual U32 NextFrame();
    virtual BOOL SetCreditsFile(std::string credits_xmd_filename);
    virtual void Parse(DkXmd::CChunkIterator iter);
    virtual void ParseDefaults(DkXmd::CChunkIterator iter);
    virtual void ParseCreditsList(DkXmd::CChunkIterator iter);
    virtual U32 NextFrameExit();
    virtual BOOL TestForExit();
    virtual void Update(F32 dt);
    virtual void Render(F32 dt);
    virtual void RenderStrip(F32 dt);
    virtual void RenderBackGround(F32 dt);
    virtual Entry* GetFontStyle(int);
    virtual int* GetFontColor(int);
    virtual int GetEntrySize(Entry& entry);
    virtual int GetEntryColorId(Entry& entry);

    void ParseColor(Color* color, DkXmd::CChunkIterator iter);
    void ParseEntry(Entry* entry, DkXmd::CChunkIterator iter);
};
REQUIRE_SIZE(CGamePartCredits, 0x8C);

#endif
