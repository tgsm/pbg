#include "engine/display/CTextureDictionary.h"
#include "CGame.h"
#include "CShadowZone.h"
#include <math.h>
#include <iostream>

CShadowZone::CShadowZone(CGame* game) {
    m_image = NULL;
    m_game = game;
}

CShadowZone::~CShadowZone() {
    UnloadShadowZone();
}

BOOL CShadowZone::LoadShadowZone(DkXmd::CChunkIterator* iter) {
    std::string tmp;
    DkXmd::CChunkIterator dest;
    DkXmd::CChunkIterator dest2;

    F32 z_width;
    F32 x_width;
    DKDSP::CEngine* display_engine;
    DKDSP::CTextureDictionary* texture_dictionary;
    void* file;
    DKDSP::IRWStream* stream;

    if (iter == NULL) {
        return FALSE;
    }

    tmp = iter->GetName();
    if (tmp == "ShadowZone") {
        if (iter->GetFirstChildChunk(dest) == TRUE) {
            do {
                tmp = dest.GetName();
                if (tmp == "File") {
                    std::string maybe_filename;
                    maybe_filename = dest.GetStringValue();

                    if (m_game == NULL) {
                        return FALSE;
                    }

                    U32 local_78;
                    file = m_game->GetResourceFactory()->LoadPureFile(maybe_filename, &local_78);
                    if (file == NULL) {
                        return FALSE;
                    }

                    display_engine = m_game->m_display_engine;
                    if (display_engine == NULL) {
                        return FALSE;
                    }

                    CDKW_Memory memory;
                    memory.m_unk4 = local_78;
                    memory.m_file = file;
                    stream = display_engine->OpenStreamMemory(rwSTREAMREAD, &memory);
                    if (stream == NULL) {
                        return FALSE;
                    }

                    texture_dictionary = m_game->m_texture_dictionary;
                    if (texture_dictionary == NULL) {
                        return FALSE;
                    }
                    m_image = texture_dictionary->LoadImageFromTGA("SHADOWZONEIMAGE", stream);
                    if (m_image == NULL) {
                        return FALSE;
                    }

                    display_engine->DestroyStream(stream);
                    delete file;
                } else if (tmp == "LeftUpCorner") {
                    if (dest.GetChunk("X", dest2)) {
                        m_topleft_x = dest2.GetFloatValue() / 100.0f;
                    }
                    if (dest.GetChunk("Y", dest2)) {
                        m_topleft_y = dest2.GetFloatValue() / 100.0f;
                    }
                    if (dest.GetChunk("Z", dest2)) {
                        m_topleft_z = dest2.GetFloatValue() / 100.0f;
                    }
                } else if (tmp == "RightDownCorner") {
                    if (dest.GetChunk("X", dest2)) {
                        m_downright_x = dest2.GetFloatValue() / 100.0f;
                    }
                    if (dest.GetChunk("Y", dest2)) {
                        m_downright_y = dest2.GetFloatValue() / 100.0f;
                    }
                    if (dest.GetChunk("Z", dest2)) {
                        m_downright_z = dest2.GetFloatValue() / 100.0f;
                    }
                }
            } while (dest.GetNextSiblingChunk(dest) == TRUE);
        }
    } else {
        return FALSE;
    }

    m_unk28 = m_image->GetDepth();
    x_width = m_downright_x - m_topleft_x;
    z_width = m_downright_z - m_topleft_z;
    if (m_image != NULL) {
        m_unk1C = x_width / (m_image->GetWidth() - 1.0f);
        m_unk20 = z_width / (m_image->GetHeight() - 1.0f);
    }

    return TRUE;
}

void CShadowZone::UnloadShadowZone() {
    if (m_game == NULL || m_image == NULL) {
        return;
    }

    if (m_game->m_texture_dictionary != NULL) {
        m_game->m_texture_dictionary->RemoveImage(m_image);
        m_image = NULL;
    }
}

CDKW_RGBAReal CShadowZone::RGBToHSV(CDKW_RGBAReal rgb) {
    F32 f1;
    f32 min_component;
    F32 yeah;
    F32 max_component;
    F32 red;
    F32 green;
    F32 blue;

    CDKW_RGBAReal ret;
    f1 = -1.0f;
    red = rgb.m_r;
    green = rgb.m_g;
    blue = rgb.m_b;
    max_component = red;
    if (green > red) {
        max_component = green;
    }
    if (blue > max_component) {
        max_component = blue;
    }
    min_component = red;
    if (green < min_component) {
        min_component = green;
    }
    if (blue < min_component) {
        min_component = blue;
    }
    min_component = max_component - min_component;
    if (min_component >= 0.05f) {
        yeah = min_component / (max_component + 0.5f);
        if (max_component == red) {
            f1 = 0.0f + (green - blue) / (min_component + 0.5f);
        } else if (max_component == green) {
            f1 = 2.0f + (blue - red) / (min_component + 0.5f);
        } else if (max_component == blue) {
            f1 = 4.0f + (red - green) / (min_component + 0.5f);
        }
        if (f1 < 0.0f) {
            f1 += 6.0f;
        } else if (f1 >= 6.0f) {
            f1 -= 6.0f;
        }

        ret.m_r = f1 * (1.0f/6.0f);
        ret.m_g = yeah;
        ret.m_b = max_component;
    } else {
        ret.m_r = f1;
        ret.m_g = 0.0f;
        ret.m_b = max_component;
    }

    return ret;
}

// Incomplete
CDKW_RGBAReal CShadowZone::HSVToRGB(CDKW_RGBAReal rgb) {
    CDKW_RGBAReal ret;

    F32 red;
    F32 green;
    F32 blue;
    F32 src_a;

    red = rgb.m_r;
    green = rgb.m_g;
    blue = rgb.m_b;

    src_a = 0.0f;
    if (red < 0.0f) {
        red = 0.0f;
    } else if (red > 1.0f) {
        red = floor(red);
        src_a -= red;
    }
    src_a *= 6.0f;
    red = src_a;
    int uVar1 = red;
    F32 funny = red - uVar1;
    F32 fVar2 = blue * (1.0f - green);
    F32 fVar3 = blue * (1.0f - green * funny);
    F32 fVar4 = blue * (1.0f - green * (1.0f - funny));

    switch (uVar1) {
        case 0:
            ret.m_r = blue;
            ret.m_g = fVar4;
            ret.m_b = fVar2;
            break;
        case 1:
            ret.m_r = fVar3;
            ret.m_g = blue;
            ret.m_b = fVar2;
            break;
        case 2:
            ret.m_r = fVar2;
            ret.m_g = blue;
            ret.m_b = fVar4;
            break;
        case 3:
            ret.m_r = fVar2;
            ret.m_g = fVar3;
            ret.m_b = blue;
            break;
        case 4:
            ret.m_r = fVar4;
            ret.m_g = fVar2;
            ret.m_b = blue;
            break;
        case 5:
            ret.m_r = blue;
            ret.m_g = fVar2;
            ret.m_b = fVar3;
            break;
    }

    return ret;
}
