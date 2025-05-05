#include "engine/display/CTextureDictionary.h"
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

    f32 z_width;
    f32 x_width;
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

                    u32 local_78;
                    file = m_game->m_resource_factory->LoadPureFile(maybe_filename, &local_78);
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
                    stream = display_engine->OpenStreamMemory(1, &memory);
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

// Equivalent: regalloc
CDKW_RGBAReal CShadowZone::RGBToHSV(CDKW_RGBAReal rgb) {
    CDKW_RGBAReal ret;
    f32 f1 = -1.0f;
    f32 red = rgb.m_r;
    f32 green = rgb.m_g;
    f32 blue = rgb.m_b;
    f32 f6 = red;
    if (green > red) {
        f6 = green;
    }
    if (blue > f6) {
        f6 = blue;
    }
    f32 fVar1 = red;
    if (green < fVar1) {
        fVar1 = green;
    }
    if (blue < fVar1) {
        fVar1 = blue;
    }
    fVar1 = f6 - fVar1;
    if (fVar1 >= 0.05f) {
        f32 yeah = fVar1 / (f6 + 0.5f);
        if (f6 == red) {
            f1 = 0.0f + (green - blue) / (fVar1 + 0.5f);
        } else if (f6 == green) {
            f1 = 2.0f + (blue - red) / (fVar1 + 0.5f);
        } else if (f6 == blue) {
            f1 = 4.0f + (red - green) / (fVar1 + 0.5f);
        }
        if (f1 < 0.0f) {
            f1 += 6.0f;
        } else if (f1 >= 6.0f) {
            f1 -= 6.0f;
        }

        ret.m_r = f1 * (1.0f/6.0f);
        ret.m_g = yeah;
        ret.m_b = f6;
    } else {
        ret.m_r = f1;
        ret.m_g = 0.0f;
        ret.m_b = f6;
    }

    return ret;
}

// Equivalent?: regalloc
CDKW_RGBAReal CShadowZone::HSVToRGB(CDKW_RGBAReal rgb) {
    CDKW_RGBAReal ret;

    f32 red;
    f32 green;
    f32 blue;
    f32 src_a;

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
    f32 funny = red - uVar1;
    f32 fVar2 = blue * (1.0f - green);
    f32 fVar3 = blue * (1.0f - green * funny);
    f32 fVar4 = blue * (1.0f - green * (1.0f - funny));

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
