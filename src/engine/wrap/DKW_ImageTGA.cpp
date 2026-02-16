#include <cstdlib>
#include <cstring>
#include <rwsdk/badevice.h>
#include "engine/wrap/DKW_ImageTGA.h"
#include "types.h"

struct TGAHeader {
    U8 id_length;
    U8 color_map_type;
    U8 image_type;
    U16 color_map_first_entry_index;
    U16 color_map_length;
    U8 color_map_entry_size;
    U16 image_x_origin;
    U16 image_y_origin;
    U16 image_width;
    U16 image_height;
    U8 image_pixel_depth;
    U8 image_descriptor;
} tgaHeader;

BOOL CDKW_ImageTGA::CheckTGA(CDKW_Stream *stream) {
#define READ_FROM_STREAM(value) \
    (RwStreamRead(stream, &value, sizeof(value)) >= sizeof(value))

    if (!READ_FROM_STREAM(tgaHeader.id_length)) {
        return FALSE;
    }
    if (!READ_FROM_STREAM(tgaHeader.color_map_type)) {
        return FALSE;
    }
    if (!READ_FROM_STREAM(tgaHeader.image_type)) {
        return FALSE;
    }
    if (!READ_FROM_STREAM(tgaHeader.color_map_first_entry_index)) {
        return FALSE;
    }
    if (!READ_FROM_STREAM(tgaHeader.color_map_length)) {
        return FALSE;
    }
    if (!READ_FROM_STREAM(tgaHeader.color_map_entry_size)) {
        return FALSE;
    }
    if (!READ_FROM_STREAM(tgaHeader.image_x_origin)) {
        return FALSE;
    }
    if (!READ_FROM_STREAM(tgaHeader.image_y_origin)) {
        return FALSE;
    }
    if (!READ_FROM_STREAM(tgaHeader.image_width)) {
        return FALSE;
    }
    if (!READ_FROM_STREAM(tgaHeader.image_height)) {
        return FALSE;
    }
    if (!READ_FROM_STREAM(tgaHeader.image_pixel_depth)) {
        return FALSE;
    }
    if (!READ_FROM_STREAM(tgaHeader.image_descriptor)) {
        return FALSE;
    }

    for (int i = 0; i < tgaHeader.id_length; i++) {
        char id_char;
        if (!READ_FROM_STREAM(id_char)) {
            return FALSE;
        }
    }

#define BYTESWAP_U16(value) \
    value = (S16)(*((U8*)(&value) + 0)) | ((S16)(*((U8*)(&value) + 1)) << 8)

    BYTESWAP_U16(tgaHeader.color_map_first_entry_index);
    BYTESWAP_U16(tgaHeader.color_map_length);
    BYTESWAP_U16(tgaHeader.image_x_origin);
    BYTESWAP_U16(tgaHeader.image_y_origin);
    BYTESWAP_U16(tgaHeader.image_width);
    BYTESWAP_U16(tgaHeader.image_height);

    switch (tgaHeader.image_type) {
        case 1:
            if (tgaHeader.color_map_type != 1) {
                return FALSE;
            }
            if (tgaHeader.color_map_entry_size != 16 && tgaHeader.color_map_entry_size != 24 && tgaHeader.color_map_entry_size != 32) {
                return FALSE;
            }
            if ((tgaHeader.image_descriptor & 0x10) != 0) {
                return FALSE;
            }
            if ((tgaHeader.image_descriptor & 0xC0) == 0xC0) {
                return FALSE;
            }
            break;
        case 3:
            if (tgaHeader.color_map_type != 0) {
                return FALSE;
            }
            if (tgaHeader.color_map_entry_size != 0) {
                return FALSE;
            }
            if ((tgaHeader.image_descriptor & 0x10) != 0) {
                return FALSE;
            }
            if ((tgaHeader.image_descriptor & 0xC0) == 0xC0) {
                return FALSE;
            }
            break;
        case 2:
            if (tgaHeader.image_pixel_depth != 16 && tgaHeader.image_pixel_depth != 24 && tgaHeader.image_pixel_depth != 32) {
                return FALSE;
            }
            if ((tgaHeader.image_descriptor & 0x10) != 0) {
                return FALSE;
            }
            if ((tgaHeader.image_descriptor & 0xC0) == 0xC0) {
                return FALSE;
            }
            if (tgaHeader.image_pixel_depth == 24 && (tgaHeader.image_descriptor & 0xF) != 0) {
                return FALSE;
            }
            if (tgaHeader.image_pixel_depth == 32 && (tgaHeader.image_descriptor & 0xF) != 8) {
                return FALSE;
            }
            break;
        case 9:
            if (tgaHeader.color_map_type != 1) {
                return FALSE;
            }
            if (tgaHeader.color_map_entry_size != 16 && tgaHeader.color_map_entry_size != 24 && tgaHeader.color_map_entry_size != 32) {
                return FALSE;
            }
            if ((tgaHeader.image_descriptor & 0x10) != 0) {
                return FALSE;
            }
            if ((tgaHeader.image_descriptor & 0xC0) != 0) {
                return FALSE;
            }
            break;
        case 10:
            if (tgaHeader.image_pixel_depth != 16 && tgaHeader.image_pixel_depth != 24 && tgaHeader.image_pixel_depth != 32) {
                return FALSE;
            }
            if ((tgaHeader.image_descriptor & 0x10) != 0) {
                return FALSE;
            }
            if ((tgaHeader.image_descriptor & 0xC0) != 0) {
                return FALSE;
            }
            break;
        default:
            return FALSE;
    }

    return TRUE;
}

CDKW_Image* CDKW_ImageTGA::Read(const char* path) {
    CDKW_Stream* stream = (CDKW_Stream*)RwStreamOpen(rwSTREAMFILENAME, rwSTREAMREAD, (void*)path);
    if (stream == NULL) {
        return NULL;
    }

    CDKW_Image* image = ReadFromStream(stream);
    RwStreamClose(stream, NULL);
    return image;
}

// Incomplete
CDKW_Image* CDKW_ImageTGA::ReadFromStream(const CDKW_Stream* stream) {
    if (stream == NULL) {
        return NULL;
    }

    if (CheckTGA((CDKW_Stream*)stream) <= 0) {
        return NULL;
    }

    int depth;
    if (tgaHeader.image_type == 1 || tgaHeader.image_type == 3 || tgaHeader.image_type == 9) {
        depth = 8;
    } else {
        depth = 32;
    }
    CDKW_Image* image = (CDKW_Image*)RwImageCreate(tgaHeader.image_width, tgaHeader.image_height, depth);
    if (image == NULL) {
        return NULL;
    }

    RwImageAllocatePixels(image);
    U8* pixels = image->cpPixels;
    U8* temp__ = pixels;

    switch (tgaHeader.image_type) {
        case 1:
        case 3: {
            RwRGBA* palette = image->palette;
            if (palette == NULL) {
                return NULL;
            }

            if (tgaHeader.image_type == 3) {
                for (U32 i = 0; i < 256; i += 8) {
                    for (U32 j = 0; j < 8; j++) {
                        palette->red = i + j;
                        palette->green = i + j;
                        palette->blue = i + j;
                        palette->alpha = 0xFF;
                        palette++;
                    }
                }
            } else {
                for (int i = 0; i < tgaHeader.color_map_length; i++) {
                    if (tgaHeader.color_map_entry_size == 16) {
                        U16 color;
                        RwStreamRead((CDKW_Stream*)stream, &color, sizeof(color));
                        palette->red = (color & 0x7C00) >> 7;
                        palette->green = (color & 0x3E0) >> 2;
                        palette->blue = (color & 0x1F) << 3;
                        palette->alpha = (color & 0x8000) >> 8;
                    } else if (tgaHeader.color_map_entry_size == 24) {
                        U8 color[3];
                        RwStreamRead((CDKW_Stream*)stream, &color, sizeof(color));
                        palette->red = color[2];
                        palette->green = color[1];
                        palette->blue = color[0];
                        palette->alpha = 0xFF;
                    } else if (tgaHeader.color_map_entry_size == 32) {
                        U8 color[4];
                        RwStreamRead((CDKW_Stream*)stream, &color, sizeof(color));
                        palette->red = color[3];
                        palette->green = color[2];
                        palette->blue = color[1];
                        palette->alpha = color[0];
                    }
                    palette++;
                }
            }

            if (tgaHeader.image_descriptor & 0x20) {
                for (U32 i = 0; i < tgaHeader.image_height; i++) {
                    RwStreamRead((CDKW_Stream*)stream, pixels + i * tgaHeader.image_width, tgaHeader.image_width);
                }
            } else {
                for (int i = tgaHeader.image_height - 1; i >= 0; i--) {
                    RwStreamRead((CDKW_Stream*)stream, pixels + i * tgaHeader.image_width, tgaHeader.image_width);
                }
            }
            break;
        }
        case 2:
            switch (tgaHeader.image_pixel_depth) {
                case 16: {
                    U16* array = (U16*)RwEngineInstance->memoryFuncs.rwmalloc(tgaHeader.image_width * sizeof(U16));
                    if (tgaHeader.image_descriptor & 0x20) {
                        for (U32 i = 0; i < tgaHeader.image_height; i++) {
                            RwStreamRead((CDKW_Stream*)stream, array, tgaHeader.image_width * sizeof(U16));
                            int j = 0;
                            U8* temp = pixels;
                            for (; j < tgaHeader.image_width; j++, temp += 4) {
                                U16* value = &array[j];
                                U8* piss = &temp[i * tgaHeader.image_width * 4];
                                temp[i * tgaHeader.image_width * 4 + 0] = (*value & 0x7C00) >> 7;
                                temp[i * tgaHeader.image_width * 4 + 1] = (*value & 0x3E0) >> 2;
                                temp[i * tgaHeader.image_width * 4 + 2] = (*value & 0x1F) << 3;
                                temp[i * tgaHeader.image_width * 4 + 3] = 0xFF;
                            }
                        }
                    } else {
                        for (int i = tgaHeader.image_height - 1; i >= 0; i--) {
                            RwStreamRead((CDKW_Stream*)stream, array, tgaHeader.image_width * sizeof(U16));
                            int j = 0;
                            U8* temp = pixels;
                            for (; j < tgaHeader.image_width; j++) {
                                temp[i * tgaHeader.image_width * 4 + 0] = (array[j] & 0x7C00) >> 7;
                                temp[i * tgaHeader.image_width * 4 + 1] = (array[j] & 0x3E0) >> 2;
                                temp[i * tgaHeader.image_width * 4 + 2] = (array[j] & 0x1F) << 3;
                                temp[i * tgaHeader.image_width * 4 + 3] = 0xFF;
                                temp += 4;
                            }
                        }
                    }
                    RwEngineInstance->memoryFuncs.rwfree(array);
                    break;
                }
                case 24: {
                    typedef U8 funnycolor[3];
                    funnycolor* array = (funnycolor*)RwEngineInstance->memoryFuncs.rwmalloc(tgaHeader.image_width * sizeof(funnycolor));
                    if (tgaHeader.image_descriptor & 0x20) {
                        for (int i = 0; i < tgaHeader.image_height; i++) {
                            RwStreamRead((CDKW_Stream*)stream, array, tgaHeader.image_width * sizeof(funnycolor));
                            for (int j = 0; j < tgaHeader.image_width; j++) {
                                pixels[i * tgaHeader.image_width * 4 + 0] = array[j][2];
                                pixels[i * tgaHeader.image_width * 4 + 1] = array[j][1];
                                pixels[i * tgaHeader.image_width * 4 + 2] = array[j][0];
                                pixels[i * tgaHeader.image_width * 4 + 3] = 0xFF;
                                pixels += 4;
                            }
                        }
                    } else {
                        for (int i = tgaHeader.image_height - 1; i >= 0; i--) {
                            RwStreamRead((CDKW_Stream*)stream, array, tgaHeader.image_width * sizeof(funnycolor));
                            int j = 0;
                            U8* temp = pixels;
                            for (; j < tgaHeader.image_width; j++, temp += 4) {
                                temp[i * tgaHeader.image_width * 4 + 0] = array[j][2];
                                temp[i * tgaHeader.image_width * 4 + 1] = array[j][1];
                                temp[i * tgaHeader.image_width * 4 + 2] = array[j][0];
                                temp[i * tgaHeader.image_width * 4 + 3] = 0xFF;
                            }
                        }
                    }
                    RwEngineInstance->memoryFuncs.rwfree(array);
                    break;
                }
                case 32: {
                    typedef U8 funnycolor[4];
                    funnycolor* array = (funnycolor*)RwEngineInstance->memoryFuncs.rwmalloc(tgaHeader.image_width * sizeof(funnycolor));
                    if (tgaHeader.image_descriptor & 0x20) {
                        for (U32 i = 0; i < tgaHeader.image_height; i++) {
                            RwStreamRead((CDKW_Stream*)stream, array, tgaHeader.image_width * sizeof(funnycolor));
                            for (int j = 0; j < tgaHeader.image_width; j++) {
                                pixels[i * tgaHeader.image_width * 4 + 0] = array[j][2];
                                pixels[i * tgaHeader.image_width * 4 + 1] = array[j][1];
                                pixels[i * tgaHeader.image_width * 4 + 2] = array[j][0];
                                pixels[i * tgaHeader.image_width * 4 + 3] = array[j][3];
                                pixels += 4;
                            }
                        }
                    } else {
                        for (int i = tgaHeader.image_height - 1; i >= 0; i--) {
                            RwStreamRead((CDKW_Stream*)stream, array, tgaHeader.image_width * sizeof(funnycolor));
                            U8* temp = pixels;
                            for (int j = 0; j < tgaHeader.image_width; j++) {
                                temp[i * tgaHeader.image_width * 4 + 0] = array[j][2];
                                temp[i * tgaHeader.image_width * 4 + 1] = array[j][1];
                                temp[i * tgaHeader.image_width * 4 + 2] = array[j][0];
                                temp[i * tgaHeader.image_width * 4 + 3] = array[j][3];
                                temp += 4;
                            }
                        }
                    }
                    RwEngineInstance->memoryFuncs.rwfree(array);
                    break;
                }
                default:
                    return NULL;
            }
            break;
        case 9: {
            RwRGBA* palette = image->palette;
            if (palette == NULL) {
                return NULL;
            }

            for (int i = 0; i < tgaHeader.color_map_length; i++) {
                if (tgaHeader.color_map_entry_size == 16) {
                    U16 color;
                    RwStreamRead((CDKW_Stream*)stream, &color, sizeof(color));
                    palette->red = (color & 0x7C00) >> 7;
                    palette->green = (color & 0x3E0) >> 2;
                    palette->blue = (color & 0x1F) << 3;
                    palette->alpha = (color & 0x8000) >> 8;
                } else if (tgaHeader.color_map_entry_size == 24) {
                    U8 color[3];
                    RwStreamRead((CDKW_Stream*)stream, &color, sizeof(color));
                    palette->red = color[2];
                    palette->green = color[1];
                    palette->blue = color[0];
                    palette->alpha = 0xFF;
                } else if (tgaHeader.color_map_entry_size == 32) {
                    U8 color[4];
                    RwStreamRead((CDKW_Stream*)stream, &color, sizeof(color));
                    palette->red = color[3];
                    palette->green = color[2];
                    palette->blue = color[1];
                    palette->alpha = color[0];
                }
                palette++;
            }

            U8 local_54;
            for (U32 i = 0, nb_pixels_maybe = tgaHeader.image_width * tgaHeader.image_height; i < nb_pixels_maybe; i += local_54 + 1) {
                RwStreamRead((CDKW_Stream*)stream, &local_54, sizeof(local_54));
                if (local_54 & 0x80) {
                    local_54 &= 0x7F;
                    U8 local_55;
                    RwStreamRead((CDKW_Stream*)stream, &local_55, sizeof(local_55));
                    for (U32 j = 0; j <= local_54; j++) {
                        pixels[j] = local_55;
                    }
                } else {
                    RwStreamRead((CDKW_Stream*)stream, pixels, local_54 + 1);
                    pixels += local_54 + 1;
                }
            }

            if (!(tgaHeader.image_descriptor & 0x20)) {
                U8* pvVar6 = (U8*)RwEngineInstance->memoryFuncs.rwmalloc(tgaHeader.image_width);
                for (int i = 0; i < tgaHeader.image_height / 2; i++) {
                    memcpy(pvVar6, pixels + i * tgaHeader.image_width, tgaHeader.image_width);
                    memcpy(pixels + i * tgaHeader.image_width, pixels + (tgaHeader.image_height - i - 1) * tgaHeader.image_width, tgaHeader.image_width);
                    memcpy(pixels + (tgaHeader.image_height - i - 1) * tgaHeader.image_width, pvVar6, tgaHeader.image_width);
                }
                RwEngineInstance->memoryFuncs.rwfree(pvVar6);
            }
            break;
        }
        case 10: {
            switch (tgaHeader.image_pixel_depth) {
                case 16: {
                    U32 uVar15 = 0;
                    for (U32 i = 0; i < tgaHeader.image_height; i++) {
                        S8 local_56;
                        for (; uVar15 < tgaHeader.image_width; uVar15 += local_56 + 1) {
                            RwStreamRead((CDKW_Stream*)stream, &local_56, 1);
                            if (local_56 & 0x80) {
                                local_56 &= 0x7F;
                                U16 color;
                                RwStreamRead((CDKW_Stream*)stream, &color, sizeof(color));
                                for (U32 k = 0; k <= local_56; k++, pixels += 4) {
                                    pixels[0] = (color & 0x7C00) >> 7;
                                    pixels[1] = (color & 0x3E0) >> 2;
                                    pixels[2] = (color & 0x1F) << 3;
                                    pixels[3] = 0xFF;
                                }
                            } else {
                                U16 color;
                                for (U32 k = 0; k <= local_56; k++, pixels += 4) {
                                    RwStreamRead((CDKW_Stream*)stream, &color, sizeof(color));
                                    pixels[0] = (color & 0x7C00) >> 7;
                                    pixels[1] = (color & 0x3E0) >> 2;
                                    pixels[2] = (color & 0x1F) << 3;
                                    pixels[3] = 0xFF;
                                }
                            }
                        }
                        uVar15 -= tgaHeader.image_width;
                    }
                    break;
                }
                case 24: {
                    U32 uVar15 = 0;
                    for (U32 i = 0; i < tgaHeader.image_height; i++) {
                        S8 local_57;
                        for (; uVar15 < tgaHeader.image_width; uVar15 += local_57 + 1) {
                            RwStreamRead((CDKW_Stream*)stream, &local_57, 1);
                            if (local_57 & 0x80) {
                                local_57 &= 0x7F;
                                U8 color[3];
                                RwStreamRead((CDKW_Stream*)stream, &color, sizeof(color));
                                for (U32 k = 0; k <= local_57; k++, pixels += 4) {
                                    pixels[0] = color[2];
                                    pixels[1] = color[1];
                                    pixels[2] = color[0];
                                    pixels[3] = 0xFF;
                                }
                            } else {
                                U8 color[3];
                                for (U32 k = 0; k <= local_57; k++, pixels += 4) {
                                RwStreamRead((CDKW_Stream*)stream, &color, sizeof(color));
                                    pixels[0] = color[2];
                                    pixels[1] = color[1];
                                    pixels[2] = color[0];
                                    pixels[3] = 0xFF;
                                }
                            }
                        }
                        uVar15 -= tgaHeader.image_width;
                    }
                    break;
                }
                case 32: {
                    U32 uVar15 = 0;
                    for (U32 i = 0; i < tgaHeader.image_height; i++) {
                        S8 local_58;
                        for (; uVar15 < tgaHeader.image_width; uVar15 += local_58 + 1) {
                            RwStreamRead((CDKW_Stream*)stream, &local_58, 1);
                            if (local_58 & 0x80) {
                                local_58 &= 0x7F;
                                U8 color[4];
                                RwStreamRead((CDKW_Stream*)stream, &color, sizeof(color));
                                for (U32 k = 0; k <= local_58; k++, pixels += 4) {
                                    pixels[0] = color[2];
                                    pixels[1] = color[1];
                                    pixels[2] = color[0];
                                    pixels[3] = color[3];
                                }
                            } else {
                                U8 color[4];
                                for (U32 k = 0; k <= local_58; k++, pixels += 4) {
                                    RwStreamRead((CDKW_Stream*)stream, &color, sizeof(color));
                                    pixels[0] = color[2];
                                    pixels[1] = color[1];
                                    pixels[2] = color[0];
                                    pixels[3] = color[3];
                                }
                            }
                        }
                        uVar15 -= tgaHeader.image_width;
                    }
                    break;
                }
                default:
                    return NULL;
            }

            if (!(tgaHeader.image_descriptor & 0x20)) {
                U8* pvVar6 = (U8*)RwEngineInstance->memoryFuncs.rwmalloc(tgaHeader.image_width * 4);
                for (int i = 0; i < tgaHeader.image_height / 2; i++) {
                    memcpy(pvVar6, pixels + i * tgaHeader.image_width * 4, tgaHeader.image_width * 4);
                    memcpy(pixels + i * tgaHeader.image_width * 4, pixels + (tgaHeader.image_height - i - 1) * tgaHeader.image_width * 4, tgaHeader.image_width * 4);
                    memcpy(pixels + (tgaHeader.image_height - i - 1) * tgaHeader.image_width * 4, pvVar6, tgaHeader.image_width * 4);
                }
                RwEngineInstance->memoryFuncs.rwfree(pvVar6);
            }

            break;
        }
        default:
            return NULL;
    }

    return image;
}

// Equivalent?: regalloc
CDKW_Image* CDKW_ImageTGA::Write(CDKW_Image* image, const char* path) {
    U8* array;
    U8* pixels;
    CDKW_Stream* stream;

    if (path == NULL || image == NULL) {
        return NULL;
    }

    stream = (CDKW_Stream*)RwStreamOpen(rwSTREAMFILENAME, rwSTREAMWRITE, (void*)path);
    if (stream == NULL) {
        return NULL;
    }

    memset(&tgaHeader, 0, sizeof(tgaHeader));
    tgaHeader.image_pixel_depth = 24;
    tgaHeader.image_width = image->width;
    tgaHeader.image_height = image->height;
    tgaHeader.image_type = 2;

#define WRITE_TO_STREAM(value) \
    ((RwStreamWrite(stream, &value, sizeof(value)) == 0 ? FALSE : TRUE) >= 1)

    if (!WRITE_TO_STREAM(tgaHeader.id_length)) {
        return NULL;
    }
    if (!WRITE_TO_STREAM(tgaHeader.color_map_type)) {
        return NULL;
    }
    if (!WRITE_TO_STREAM(tgaHeader.image_type)) {
        return NULL;
    }
    if (!WRITE_TO_STREAM(tgaHeader.color_map_first_entry_index)) {
        return NULL;
    }
    if (!WRITE_TO_STREAM(tgaHeader.color_map_length)) {
        return NULL;
    }
    if (!WRITE_TO_STREAM(tgaHeader.color_map_entry_size)) {
        return NULL;
    }
    if (!WRITE_TO_STREAM(tgaHeader.image_x_origin)) {
        return NULL;
    }
    if (!WRITE_TO_STREAM(tgaHeader.image_y_origin)) {
        return NULL;
    }
    if (!WRITE_TO_STREAM(tgaHeader.image_width)) {
        return NULL;
    }
    if (!WRITE_TO_STREAM(tgaHeader.image_height)) {
        return NULL;
    }
    if (!WRITE_TO_STREAM(tgaHeader.image_pixel_depth)) {
        return NULL;
    }
    if (!WRITE_TO_STREAM(tgaHeader.image_descriptor)) {
        return NULL;
    }

    pixels = image->cpPixels;
    array = (U8*)RwEngineInstance->memoryFuncs.rwmalloc(image->width * image->height * 3);
    int height = image->height - 1;
    int iVar3 = height;
    int width = image->width;

    for (int i = 0; i <= height; i++, iVar3--) {
        int iVar1 = image->height - iVar3 - 1;
        for (int j = 0; j < image->width; j++) {
            array[(j + iVar1 * image->width) * 3 + 0] = pixels[(j + iVar3 * image->width) * 4 + 2];
            array[(j + iVar1 * image->width) * 3 + 1] = pixels[(j + iVar3 * image->width) * 4 + 1];
            array[(j + iVar1 * image->width) * 3 + 2] = pixels[(j + iVar3 * image->width) * 4 + 0];
        }
    }

    RwStreamWrite(stream, array, image->width * image->height * 3);
    free(array);
    RwStreamClose(stream, NULL);

    return image;
}
