#include "CGamePartCredits.h"
#include "CGame.h"
#include "CIcon.h"
#include "entities/CEntityManager.h"
#include <string>
#include <iostream>

extern "C" {
extern void Rt2dCTMSetIdentity(void);
}

CGamePartCredits::CGamePartCredits(CGame* game, int a2) {
    m_type = GAME_PART_TYPE_CREDITS;
    m_unk4 = m_type;
    m_game = NULL;
    m_credits_xmd_data = NULL;
    m_credits_xmd = NULL;
    m_credits_xmd_iter = NULL;
    m_unk34 = FALSE;
    m_input_quit_credits_A = NULL;
    m_input_quit_credits_B = NULL;
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

    m_colors.__unknown_reset_function();
    AS_ULONG_VECTOR_HACK(m_colors).reserve(6);
    m_styles.__unknown_reset_function();
    AS_ULONG_VECTOR_HACK(m_styles).reserve(6);
    m_entries.__unknown_reset_function();
    AS_ULONG_VECTOR_HACK(m_entries).reserve(100);

    m_game = game;

    U32 rf_unkC = m_game->GetResourceFactory()->m_unkC;
    m_game->GetResourceFactory()->m_unkC = 0;

    m_input_quit_credits_A = DKI::IInputEngine::CreateInput("QUITCREDITSA", 0, 19);
    m_input_quit_credits_B = DKI::IInputEngine::CreateInput("QUITCREDITSB", 0, 12);

    SetCreditsFile("MENUS/CREDITS/Credits.XMD");
    Parse(*m_credits_xmd_iter);

    m_raster_width = m_game->GetCamera()->GetBuffer()->GetWidth();
    m_raster_height = m_game->GetCamera()->GetBuffer()->GetHeight();

    m_unk48 = 1;

    for (std::vector<Entry*>::iterator iter = m_entries.begin(); iter < m_entries.end(); iter++) {
        m_unk48 += GetEntrySize(**iter);
        m_unk48 += (U32)m_interline;
    }

    m_game->GetCamera()->SetViewWindow(0.5f, 0.5f);

    Rt2dCTMSetIdentity();

    for (int i = 0; i < 4; i++) {
        m_batches[i] = m_game->GetDisplayEngine()->GetImmediate()->CreateBatch2D(4, 0);
    }

    m_game->GetTimer()->Reset();

    m_game->ComputeDeltaTime();
    m_game->ComputeDeltaTime();
    m_game->ComputeDeltaTime();
    m_game->ComputeDeltaTime();
    m_game->ComputeDeltaTime();
    m_game->ComputeDeltaTime();
    m_game->ComputeDeltaTime();
    m_game->ComputeDeltaTime();

    m_game->GetResourceFactory()->m_unkC = rf_unkC;

    m_game->GetGuiManager()->Reset();
}

CGamePartCredits::~CGamePartCredits() {
    m_game->GetFxManager()->Clear();
    m_game->GetSoundEngine()->DeleteAllSounds();

    for (int i = 0; i < 4; i++) {
        m_game->GetDisplayEngine()->GetImmediate()->RemoveBatch2D(m_batches[i]);
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
    if (m_input_quit_credits_A != NULL) {
        DKI::IInputEngine::DestroyInput(m_input_quit_credits_A);
        m_input_quit_credits_A = NULL;
    }
    if (m_input_quit_credits_B != NULL) {
        DKI::IInputEngine::DestroyInput(m_input_quit_credits_B);
        m_input_quit_credits_B = NULL;
    }

    for (std::vector<Color*>::iterator iter = m_colors.end() - 1; iter >= m_colors.begin(); iter--) {
        delete *iter;
        AS_ULONG_VECTOR_HACK(m_colors).erase(reinterpret_cast<unsigned long*>(iter));
    }
    m_colors.__unknown_reset_function();

    for (std::vector<Style*>::iterator iter = m_styles.end() - 1; iter >= m_styles.begin(); iter--) {
        delete *iter;
        AS_ULONG_VECTOR_HACK(m_styles).erase(reinterpret_cast<unsigned long*>(iter));
    }
    m_styles.__unknown_reset_function();

    for (std::vector<Entry*>::iterator iter = m_entries.end() - 1; iter >= m_entries.begin(); iter--) {
        delete *iter;
        AS_ULONG_VECTOR_HACK(m_entries).erase(reinterpret_cast<unsigned long*>(iter));
    }
    m_entries.__unknown_reset_function();

    m_game->GetGuiManager()->UnLoadLevel(0);
    m_game->GetResourceFactory()->UnloadResources(0);
}

BOOL CGamePartCredits::SetCreditsFile(std::string credits_xmd) {
    m_credits_xmd_data = m_game->GetResourceFactory()->LoadPureFile(credits_xmd, NULL);
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
                m_game->GetResourceFactory()->LoadResource(RESOURCE_TYPE_SAMPLE_BANK1, dest.GetStringValue());
            } else if (tmp == "SoundId") {
                tmp = dest.GetStringValue();
                m_unk4C = m_game->GetSoundEngine()->PlaySound2D(tmp, 0);
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
                m_black_strip_height = dest.GetFloatValue() * m_game->GetCamera()->GetBuffer()->GetHeight();
            } else if (tmp == "FadeStripHeight") {
                m_fade_strip_height = dest.GetFloatValue() * m_game->GetCamera()->GetBuffer()->GetHeight();
            } else if (tmp == "StartTime") {
                m_start_time = -dest.GetFloatValue();
            } else if (tmp == "Defaults") {
                ParseDefaults(dest);
            } else if (tmp == "Color") {
                Color* color = new Color;
                unsigned long c = reinterpret_cast<unsigned long>(color);
                ParseColor(color, dest);
                AS_ULONG_VECTOR_HACK(m_colors).push_back(c);
            } else if (tmp == "Style") {
                Style* style = new Style;
                unsigned long s = reinterpret_cast<unsigned long>(style);
                ParseStyle(style, dest);
                AS_ULONG_VECTOR_HACK(m_styles).push_back(s);
            } else if (tmp == "CreditsList") {
                ParseCreditsList(dest);
            }
        } while (dest.GetNextSiblingChunk(dest) == TRUE);
    }
}

void CGamePartCredits::ParseStyle(Style* style, DkXmd::CChunkIterator iter) {
    std::string tmp;
    DkXmd::CChunkIterator dest;
    if (iter.GetFirstChildChunk(dest) == TRUE) {
        do {
            tmp = dest.GetName();

            if (tmp == "Id") {
                style->id = dest.GetS32Value();
            } else if (tmp == "Size") {
                style->size = dest.GetS32Value();
            } else if (tmp == "ColorId") {
                style->color_id = dest.GetS32Value();
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
                color->color.red = dest.GetS32Value();
            } else if (tmp == "G") {
                color->color.green = dest.GetS32Value();
            } else if (tmp == "B") {
                color->color.blue = dest.GetS32Value();
            } else if (tmp == "A") {
                color->color.alpha = dest.GetS32Value();
            }
        } while (dest.GetNextSiblingChunk(dest) == TRUE);
    }
}

CGamePartCredits::Color* CGamePartCredits::GetFontColor(int id) {
    for (std::vector<Color*>::iterator iter = m_colors.begin(); iter < m_colors.end(); iter++) {
        Color* color = *iter;
        if (color->id == id) {
            return color;
        }
    }

    return NULL;
}

CGamePartCredits::Style* CGamePartCredits::GetFontStyle(int id) {
    for (std::vector<Style*>::iterator iter = m_styles.begin(); iter < m_styles.end(); iter++) {
        Style* style = *iter;
        if (style->id == id) {
            return style;
        }
    }

    return NULL;
}

int CGamePartCredits::GetEntrySize(Entry& entry) {
    if (entry.id != -1) {
        Style* style = GetFontStyle(entry.id);
        if (style != NULL && style->size != -1) {
            return style->size;
        }
    }
    return m_size;
}

int CGamePartCredits::GetEntryColorId(Entry& entry) {
    if (entry.id != -1) {
        Style* style = GetFontStyle(entry.id);
        if (style != NULL && style->color_id != -1) {
            return style->color_id;
        }
    }
    return m_color_id;
}

U32 CGamePartCredits::NextFrame() {
    if (!m_game->GetDisplayEngine()->Update()) {
        return GAME_PART_TYPE_DM_ROOM_LAUNCHER;
    }

    F32 dt = m_game->GetDeltaTime();
    Update(dt);
    Render(dt);

    if (TestForExit() && m_start_time > 2.0f) {
        if (!m_game->IsUnk5038Not2()) {
            m_game->FadeInit(2.0f, CGame::FADE_TYPE_0, 0, 0, 0, 0.0f);
            m_game->m_unk5038 = 0;
        }
    }

    if (m_game->m_unk5038 == 0 && !m_game->FadeIn(dt)) {
        m_game->m_unk5038 = 2;
        return NextFrameExit();
    }

    if (m_unk4 != m_type || m_unk34 == TRUE) {
        return NextFrameExit();
    }

    return m_type;
}

void CGamePartCredits::Update(F32 dt) {
    F32 entry_bottom_y;
    F32 entry_size;

#ifdef VERSION_GPLE9G
    if (DKI::IInputEngine::GetDevice(0)->IsConnected() == TRUE) {
#endif
        m_start_time += dt;
#ifdef VERSION_GPLE9G
    }
#endif

    m_game->GetSoundEngine()->BeginUpdate();
    m_game->GetSoundEngine()->EndUpdate();

    if (!m_game->IsUnk5038Not2()) {
        m_game->GetGuiManager()->Update(dt);
    }
    F32 y = 0.0f;
    F32 dVar13 = (m_raster_height + m_unk48) / m_speed;

    for (std::vector<Entry*>::iterator iter = m_entries.begin(); iter < m_entries.end(); iter++) {
        Entry& entry = **iter;
        entry_size = GetEntrySize(entry);
        entry_bottom_y = y + entry_size;
        y = InterpolValue(m_raster_height + entry_bottom_y, entry_bottom_y - m_unk48, m_start_time, dVar13);
        if (y >= 0.0f && y < m_raster_height + entry_size) {
            F32 text_width = m_raster_width * m_game->GetGuiEngine()->GetTextWidth(entry.text.c_str(), entry_size / m_raster_height, NULL);
            int rw = m_raster_width;

            CDKW_RGBA color = GetFontColor(GetEntryColorId(entry))->color;
            color.alpha = 0xFF;
            m_game->GetGuiEngine()->SetTextColor(color.red, color.green, color.blue, color.alpha);

            F32 x = (rw - text_width) / 2;
            m_game->GetGuiEngine()->AddText(x / m_raster_width, y / m_raster_height, entry.text.c_str(), entry_size / m_raster_height, NULL, 0.0f);
        }

        y = entry_bottom_y + m_interline;
    }

    if (m_start_time >= dVar13) {
        m_unk34 = TRUE;
    }
}

void CGamePartCredits::Render(F32 dt) {
    m_game->GetScene()->SelectCamera(m_game->GetCamera());
    m_game->GetScene()->Clear(3, 0.0f, 0.0f, 0.0f);

    m_game->GetScene()->BeginRender();

    m_game->GetFxManager()->Render();
    m_game->GetGuiEngine()->UpdateAndRenderOnlyTexts(m_game->GetCamera()->m_wrap_camera->m_rw_camera);
    if (!m_game->IsUnk5038Not2()) {
        m_game->GetGuiManager()->Render(dt);
    }
    m_game->GetScene()->Flush();
    m_game->RenderFade();
    m_game->RenderFade(); // why did they do this twice?
    RenderStrip(dt);

    m_game->GetScene()->EndRender();

    m_game->GetScene()->Flip(0);
}

void CGamePartCredits::RenderBackGround(F32 dt) {
    CIcon::BeginRender(m_game->GetDisplayEngine(), m_game->GetCamera(), m_game->GetScene());

    CIcon icon;
    icon.m_width = 1.0f;
    icon.m_height = 1.0f;
    icon.m_x = 0.5f;
    icon.m_y = 0.5f;

    char texture_name[256] = {};
    RwEngineInstance->stringFuncs.rwsprintf(texture_name, "hiver02b_D");
    icon.m_texture = m_game->GetTextureDictionary()->FindTexture(texture_name);

    icon.Render(m_batches[0], 0);

    CIcon::EndRender();
}

void CGamePartCredits::RenderStrip(F32 dt) {
    CIcon::BeginRender(m_game->GetDisplayEngine(), m_game->GetCamera(), m_game->GetScene());

    m_game->GetDisplayEngine()->SetRenderState(rwRENDERSTATETEXTURERASTER, NULL);

    F32 z = m_game->GetCamera()->GetZNear();
    F32 cam_width = m_game->GetCamera()->GetBuffer()->GetWidth();
    F32 cam_height = m_game->GetCamera()->GetBuffer()->GetHeight();
    F32 strips_height = m_black_strip_height + m_fade_strip_height; // total height of both strips

    for (int i = 0; i < 4; i++) {
        DKDSP::CIm2DBatch* batch = m_batches[i];
        switch (i) {
            case 0:
                batch->SetVertexPosition(0, 0.0f, m_black_strip_height, z, 1.0f / z);
                batch->SetVertexPosition(2, cam_width, m_black_strip_height, z, 1.0f / z);
                batch->SetVertexPosition(1, 0.0f, strips_height, z, 1.0f / z);
                batch->SetVertexPosition(3, cam_width, strips_height, z, 1.0f / z);
                break;
            case 1:
                batch->SetVertexPosition(0, 0.0f, cam_height - strips_height, z, 1.0f / z);
                batch->SetVertexPosition(2, cam_width, cam_height - strips_height, z, 1.0f / z);
                batch->SetVertexPosition(1, 0.0f, cam_height - m_black_strip_height, z, 1.0f / z);
                batch->SetVertexPosition(3, cam_width, cam_height - m_black_strip_height, z, 1.0f / z);
                break;
            case 2:
                batch->SetVertexPosition(0, 0.0f, 0.0f, z, 1.0f / z);
                batch->SetVertexPosition(2, cam_width, 0.0f, z, 1.0f / z);
                batch->SetVertexPosition(1, 0.0f, m_black_strip_height, z, 1.0f / z);
                batch->SetVertexPosition(3, cam_width, m_black_strip_height, z, 1.0f / z);
                break;
            case 3:
                batch->SetVertexPosition(0, 0.0f, cam_height - m_black_strip_height, z, 1.0f / z);
                batch->SetVertexPosition(2, cam_width, cam_height - m_black_strip_height, z, 1.0f / z);
                batch->SetVertexPosition(1, 0.0f, cam_height, z, 1.0f / z);
                batch->SetVertexPosition(3, cam_width, cam_height, z, 1.0f / z);
                break;
        }

        switch (i) {
            case 0:
                batch->SetVertexRGBA(0, 0, 0, 0, 0xFF);
                batch->SetVertexRGBA(2, 0, 0, 0, 0xFF);
                batch->SetVertexRGBA(1, 0, 0, 0, 0x00);
                batch->SetVertexRGBA(3, 0, 0, 0, 0x00);
                break;
            case 1:
                batch->SetVertexRGBA(0, 0, 0, 0, 0x00);
                batch->SetVertexRGBA(2, 0, 0, 0, 0x00);
                batch->SetVertexRGBA(1, 0, 0, 0, 0xFF);
                batch->SetVertexRGBA(3, 0, 0, 0, 0xFF);
                break;
            case 2:
            case 3:
                batch->SetVertexRGBA(0, 0, 0, 0, 0xFF);
                batch->SetVertexRGBA(2, 0, 0, 0, 0xFF);
                batch->SetVertexRGBA(1, 0, 0, 0, 0xFF);
                batch->SetVertexRGBA(3, 0, 0, 0, 0xFF);
                break;
        }

        batch->SetVertexUV(0, 0.0f, 0.0f);
        batch->SetVertexUV(2, 1.0f, 0.0f);
        batch->SetVertexUV(1, 0.0f, 1.0f);
        batch->SetVertexUV(3, 1.0f, 1.0f);

        batch->GrabRenderstates();

        m_game->GetScene()->RenderBatch2D(batch, rwPRIMTYPETRISTRIP, FALSE);
    }

    CIcon::EndRender();
}

BOOL CGamePartCredits::TestForExit() {
    if (m_input_quit_credits_A != NULL && m_input_quit_credits_A->GetState().m_unk0 == TRUE) {
        return TRUE;
    }

    if (m_input_quit_credits_B != NULL && m_input_quit_credits_B->GetState().m_unk0 == TRUE) {
        return TRUE;
    }

    return FALSE;
}

U32 CGamePartCredits::NextFrameExit() {
    m_game->ResetOpcodeBuffer();
    if (m_unk10 != 0) {
        m_game->PushOpcodeValue(SCRIPT_COMMAND_LOAD_START_SCREEN);
    } else {
        m_game->ResetOpcodeBuffer();
        m_game->PushOpcodeValue(SCRIPT_COMMAND_SET_CURRENT_ROOM);
        m_game->PushOpcodeValue(2);
        m_game->PushOpcodeValue(HERO_PIGLET);
        m_game->PushOpcodeValue(25);
        m_game->PushOpcodeValue(0);
        m_game->PushOpcodeValue(15);
        m_game->PushOpcodeValue(0);
        m_game->PushOpcodeValue(156);
        m_game->PushOpcodeValue(0);
        m_game->PushOpcodeValue(0);
    }

    m_game->SetCurrentRoomReturnType(CGame::RETURN_TYPE_0, -1);
    return GAME_PART_TYPE_NONE;
}
