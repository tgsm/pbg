#include "CGamePartCredits.h"
#include "CGame.h"
#include <string>
#include <iostream>

extern "C" {
extern void Rt2dCTMSetIdentity(void);
}

// FIXME: Get rid of this
#define CLEAR_VECTOR(vec) \
    vec.m_size = 0; \
    if (vec.m_data != NULL) { \
        delete vec.m_data; \
    } \
    vec.m_data = NULL; \
    vec.m_capacity = 0;

CGamePartCredits::CGamePartCredits(CGame* game, int a2) {
    m_unk0 = 1;
    m_unk4 = m_unk0;
    m_game = NULL;
    m_credits_xmd_data = NULL;
    m_credits_xmd = NULL;
    m_credits_xmd_iter = NULL;
    m_unk34 = FALSE;
    m_unk20 = NULL;
    m_unk24 = NULL;
    m_unk4C = NULL;
    m_start_time = 0.0f;
    m_raster_width = 0;
    m_raster_height = 0;
    m_speed = 0.0f;
    m_interline = 0.0f;
    m_size = 0;
    m_color_id = 0;
    m_unk48 = 0;
    m_fade_strip_height = 0.0f;
    m_black_strip_height = 0.0f;
    m_unk10 = a2;

    CLEAR_VECTOR(m_unk68);
    m_unk68.reserve(6);
    CLEAR_VECTOR(m_unk74);
    m_unk74.reserve(6);
    CLEAR_VECTOR(m_unk80);
    m_unk80.reserve(100);

    m_game = game;

    u32 rf_unkC = m_game->m_resource_factory->m_unkC;
    m_game->m_resource_factory->m_unkC = 0;

    m_unk20 = DKI::IInputEngine::CreateInput("QUITCREDITSA", 0, 19);
    m_unk24 = DKI::IInputEngine::CreateInput("QUITCREDITSB", 0, 12);

    SetCreditsFile("MENUS/CREDITS/Credits.XMD");
    Parse(*m_credits_xmd_iter);

    m_raster_width = m_game->m_camera->GetBuffer()->GetWidth();
    m_raster_height = m_game->m_camera->GetBuffer()->GetHeight();

    m_unk48 = 1;

    for (size_t** iter = m_unk80.begin(); iter < m_unk80.end(); iter++) {
        u32 size = GetEntrySize(reinterpret_cast<Entry&>(**iter));
        m_unk48 += size;
        u32 interline = m_interline;
        m_unk48 += interline;
    }

    m_game->m_camera->SetViewWindow(0.5f, 0.5f);

    Rt2dCTMSetIdentity();

    for (int i = 0; i < 4; i++) {
        m_batches[i] = m_game->m_display_engine->GetImmediate()->CreateBatch2D(4, 0);
    }

    m_game->m_timer->Reset();

    m_game->ComputeDeltaTime();
    m_game->ComputeDeltaTime();
    m_game->ComputeDeltaTime();
    m_game->ComputeDeltaTime();
    m_game->ComputeDeltaTime();
    m_game->ComputeDeltaTime();
    m_game->ComputeDeltaTime();
    m_game->ComputeDeltaTime();

    m_game->m_resource_factory->m_unkC = rf_unkC;

    m_game->m_gui_manager->Reset();
}

// Equivalent: std::vector shenanigans
CGamePartCredits::~CGamePartCredits() {
    m_game->m_fx_manager->Clear();
    m_game->m_sound_engine->DeleteAllSounds();

    for (int i = 0; i < 4; i++) {
        // FIXME: Remove this cast
        m_game->m_display_engine->GetImmediate()->RemoveBatch2D((DKDSP::IIm2DBatch*)m_batches[i]);
        m_batches[i] = NULL;
    }

    if (m_credits_xmd_data != NULL) {
        delete m_credits_xmd_data;
        m_credits_xmd_data = NULL;
    }
    if (m_credits_xmd != NULL) {
        delete m_credits_xmd;
        m_credits_xmd = NULL;
    }
    if (m_unk20 != NULL) {
        DKI::IInputEngine::DestroyInput(m_unk20);
        m_unk20 = NULL;
    }
    if (m_unk24 != NULL) {
        DKI::IInputEngine::DestroyInput(m_unk24);
        m_unk24 = NULL;
    }

    // mr. hax

    for (size_t** iter = m_unk68.end() - 1; iter >= m_unk68.begin(); iter--) {
        if (*iter != NULL) {
            delete *iter;
        }
        m_unk68.erase(iter);
    }
    CLEAR_VECTOR(m_unk68);

    for (size_t** iter = m_unk74.end() - 1; iter >= m_unk74.begin(); iter--) {
        delete *iter;
        m_unk74.erase(iter);
    }
    CLEAR_VECTOR(m_unk74);

    // Freeing std::string*s from a vector of size_ts.
    // Why would you do this
    for (size_t** iter = m_unk80.end() - 1; iter >= m_unk80.begin(); iter--) {
        std::string* str = reinterpret_cast<std::string*>(*iter);
        if (str != NULL) {
            str->~basic_string();
            delete *iter;
        }
        m_unk80.erase(iter);
    }
    CLEAR_VECTOR(m_unk80);

    m_game->m_gui_manager->UnLoadLevel(0);
    m_game->m_resource_factory->UnloadResources(0);
}

BOOL CGamePartCredits::SetCreditsFile(std::string credits_xmd) {
    m_credits_xmd_data = m_game->m_resource_factory->LoadPureFile(credits_xmd, NULL);
    if (m_credits_xmd_data == NULL) {
        return FALSE;
    }

    m_credits_xmd = new DkXmd::CXmdFile;
    if (!m_credits_xmd->Parse(m_credits_xmd_data)) {
        return FALSE;
    }

    m_credits_xmd_iter = &m_credits_xmd->m_chunk_iterator;

    return TRUE;
}

void CGamePartCredits::ParseDefaults(DkXmd::CChunkIterator iter) {
    std::string tmp;
    DkXmd::CChunkIterator dest;

    if (iter.GetFirstChildChunk(dest) == TRUE) {
        do {
            tmp = dest.GetName();

            if (tmp == "Size") {
                m_size = dest.GetS32Value();
            } else if (tmp == "ColorId") {
                m_color_id = dest.GetS32Value();
            }
        } while (dest.GetNextSiblingChunk(dest) == TRUE);
    }
}

void CGamePartCredits::Parse(DkXmd::CChunkIterator iter) {
    std::string tmp;
    DkXmd::CChunkIterator dest;

    if (iter.GetFirstChildChunk(dest) == TRUE) {
        do {
            tmp = dest.GetName();

            if (tmp == "SndDict") {
                CResourceFactory* resource_factory = m_game->m_resource_factory;
                resource_factory->LoadResource(10, dest.GetStringValue());
            } else if (tmp == "SoundId") {
                tmp = dest.GetStringValue();
                m_unk4C = m_game->m_sound_engine->PlaySound2D(tmp, 0);
                if (m_unk4C != NULL) {
                    m_unk4C->SetLoopMode(1);
                    m_unk4C->SetLayer(2);
                    m_unk4C->SetVolume(1.0f);
                }
            } else if (tmp == "Speed") {
                m_speed = dest.GetFloatValue();
            } else if (tmp == "Interline") {
                m_interline = dest.GetFloatValue();
            } else if (tmp == "BlackStripHeight") {
                m_black_strip_height = dest.GetFloatValue() * m_game->m_camera->GetBuffer()->GetHeight();
            } else if (tmp == "FadeStripHeight") {
                m_fade_strip_height = dest.GetFloatValue() * m_game->m_camera->GetBuffer()->GetHeight();
            } else if (tmp == "StartTime") {
                m_start_time = -dest.GetFloatValue();
            } else if (tmp == "Defaults") {
                ParseDefaults(dest);
            } else if (tmp == "Color") {
                Color* color = new Color;
                size_t c = reinterpret_cast<size_t>(color);
                ParseColor(color, dest);
                m_unk68.push_back(c);
            } else if (tmp == "Style") {
                Entry* entry = new Entry;
                size_t e = reinterpret_cast<size_t>(entry);
                ParseEntry(entry, dest);
                m_unk74.push_back(e);
            } else if (tmp == "CreditsList") {
                ParseCreditsList(dest);
            }
        } while (dest.GetNextSiblingChunk(dest) == TRUE);
    }
}

void CGamePartCredits::ParseEntry(Entry* entry, DkXmd::CChunkIterator iter) {
    std::string tmp;
    DkXmd::CChunkIterator dest;
    if (iter.GetFirstChildChunk(dest) == TRUE) {
        do {
            tmp = dest.GetName();

            if (tmp == "Id") {
                entry->id = dest.GetS32Value();
            } else if (tmp == "Size") {
                entry->size = dest.GetS32Value();
            } else if (tmp == "ColorId") {
                entry->color_id = dest.GetS32Value();
            }
        } while (dest.GetNextSiblingChunk(dest) == TRUE);
    }
}

void CGamePartCredits::ParseColor(Color* color, DkXmd::CChunkIterator iter) {
    std::string tmp;
    DkXmd::CChunkIterator dest;
    if (iter.GetFirstChildChunk(dest) == TRUE) {
        do {
            tmp = dest.GetName();

            if (tmp == "Id") {
                color->id = dest.GetS32Value();
            } else if (tmp == "R") {
                color->color.m_r = dest.GetS32Value();
            } else if (tmp == "G") {
                color->color.m_g = dest.GetS32Value();
            } else if (tmp == "B") {
                color->color.m_b = dest.GetS32Value();
            } else if (tmp == "A") {
                color->color.m_a = dest.GetS32Value();
            }
        } while (dest.GetNextSiblingChunk(dest) == TRUE);
    }
}

// Am I using the right struct here?
int CGamePartCredits::GetEntrySize(Entry& entry) {
    if (entry.size != -1) {
        Entry* style = GetFontStyle(entry.size);
        if (style != NULL && style->size != -1) {
            return style->size;
        }
    }
    return m_size;
}

// Am I using the right struct here?
int CGamePartCredits::GetEntryColorId(Entry& entry) {
    if (entry.size != -1) {
        Entry* style = GetFontStyle(entry.size);
        if (style != NULL && style->color_id != -1) {
            return style->color_id;
        }
    }
    return m_color_id;
}
