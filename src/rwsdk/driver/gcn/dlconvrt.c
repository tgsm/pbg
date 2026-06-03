#include <cstddef>
#include "dont_inline_hack.h"
#include <rwsdk/baimage.h>
#include <rwsdk/baraster.h>
#include <rwsdk/driver/common/palquant.h>
#include <rwsdk/plcore/baerr.h>

typedef void (*UnconvertFn)(RwRGBA* x, unsigned int y);
typedef unsigned int (*ConvertFn)(RwRGBA* x);

unsigned int _rwDlFindMSB(int x) {
    unsigned int msb = -1;
    while (x != 0) {
        x >>= 1;
        msb++;
    }
    return msb;
}

unsigned int _rwDlConv8888To555(RwRGBA* x) {
    return (((x->red & 0xF8) << 7) | 0x8000) | ((x->green & 0xF8) << 2) | ((x->blue >> 3) & 0x1F);
}

unsigned int _rwDlConv8888To565(RwRGBA* x) {
    return ((x->red & 0xF8) << 8) | ((x->green & 0xFC) << 3) | ((x->blue >> 3) & 0x1F);
}

unsigned int _rwDlConv8888To555or3444(RwRGBA* x) {
    if (x->alpha != 0xFF) {
        return ((x->alpha & 0xE0) << 7) | ((x->red & 0xF0) << 4) | (x->green & 0xF0) | ((x->blue >> 4) & 0xF);
    } else {
        return _rwDlConv8888To555(x);
    }
}

unsigned int _rwDlConv8888ToDl888(RwRGBA* x) {
    return 0xFF000000 | (x->red << 16) | (x->green << 8) | x->blue;
}

unsigned int _rwDlConv8888ToDl8888(RwRGBA* x) {
    return (x->alpha << 24) | (x->red << 16) | (x->green << 8) | x->blue;
}

int _rwDlRGBToPixel(unsigned int* a0, RwRGBA* a1, RwRasterFormat rasterFormat) {
    unsigned int ret;
    DONT_INLINE_HACK();

    switch (rasterFormat & rwRASTERFORMATPIXELFORMATMASK) {
        case rwRASTERFORMATDEFAULT:
            _rwDlRGBToPixel(&ret, a1, rwRASTERFORMAT565);
            break;
        case rwRASTERFORMATLUM8: {
            RwThrowErrorParams(1, 2, "rwRASTERFORMATLUM8 not yet supported");
            break;
        }
        case rwRASTERFORMAT1555:
        case rwRASTERFORMAT4444:
            ret = _rwDlConv8888To555or3444(a1);
            break;
        case rwRASTERFORMAT555:
            ret = _rwDlConv8888To555(a1);
            break;
        case rwRASTERFORMAT565:
            ret = _rwDlConv8888To565(a1);
            break;
        case rwRASTERFORMAT8888:
            ret = _rwDlConv8888ToDl8888(a1);
            break;
        case rwRASTERFORMAT888:
            ret = _rwDlConv8888ToDl888(a1);
            break;
        default: {
            RwThrowError(1, E_RW_INVRASTERFORMAT);
            break;
        }
    }

    *a0 = ret;
    return 1;
}

void _rwDlConv555To8888(RwRGBA* x, unsigned int y) {
    x->red = (y >> 7) & 0xF8;
    x->green = (y >> 2) & 0xF8;
    x->blue = (y << 3);
    x->alpha = 0xFF;
}

void _rwDlConv565To8888(RwRGBA* x, unsigned int y) {
    x->red = (y >> 8) & 0xF8;
    x->green = (y >> 3) & 0xFC;
    x->blue = (y << 3);
    x->alpha = 0xFF;
}

void _rwDlConv1555To8888(RwRGBA* x, unsigned int y) {
    if (y & (1 << 15)) {
        _rwDlConv555To8888(x, y);
    } else {
        x->red = (y >> 4) & 0xF0;
        x->green = (y >> 0) & 0xF0;
        x->blue = (y << 4);
        x->alpha = 0x00;
    }
}

void _rwDlConv4444To8888(RwRGBA* x, unsigned int y) {
    if (y & (1 << 15)) {
        _rwDlConv555To8888(x, y);
    } else {
        x->red = (y >> 4) & 0xF0;
        x->green = (y >> 0) & 0xF0;
        x->blue = (y << 4);
        x->alpha = (y >> 7) & 0xE0;
    }
}

void _rwDlConvDl888To8888(RwRGBA* x, unsigned int y) {
    x->alpha = 0xFF;
    x->red = (y >> 16) & 0xFF;
    x->green = (y >> 8) & 0xFF;
    x->blue = y & 0xFF;
}

void _rwDlConvDl8888To8888(RwRGBA* x, unsigned int y) {
    x->alpha = (y >> 24) & 0xFF;
    x->red = (y >> 16) & 0xFF;
    x->green = (y >> 8) & 0xFF;
    x->blue = y & 0xFF;
}

int _rwDlPixelToRGB(RwRGBA* a0, unsigned int* a1, RwRasterFormat rasterFormat) {
    int a1_ = *a1;
    DONT_INLINE_HACK();

    switch (rasterFormat & rwRASTERFORMATPIXELFORMATMASK) {
        case rwRASTERFORMATDEFAULT:
            _rwDlPixelToRGB(a0, a1, rwRASTERFORMAT565);
            break;
        case rwRASTERFORMATLUM8: {
            RwThrowErrorParams(1, 2, "rwRASTERFORMATLUM8 not yet supported");
            break;
        }
        case rwRASTERFORMAT555:
            _rwDlConv555To8888(a0, a1_);
            break;
        case rwRASTERFORMAT1555:
            _rwDlConv1555To8888(a0, a1_);
            break;
        case rwRASTERFORMAT565:
            _rwDlConv565To8888(a0, a1_);
            break;
        case rwRASTERFORMAT4444:
            _rwDlConv4444To8888(a0, a1_);
            break;
        case rwRASTERFORMAT888:
            _rwDlConvDl888To8888(a0, a1_);
            break;
        case rwRASTERFORMAT8888:
            _rwDlConvDl8888To8888(a0, a1_);
            break;
        default: {
            RwThrowError(1, E_RW_INVRASTERFORMAT);
            break;
        }
    }

    return 1;
}

UnconvertFn _rwDlSelectUnconvertFn(RwRasterFormat rasterFormat) {
    UnconvertFn fn = NULL;
    DONT_INLINE_HACK();

    switch (rasterFormat & rwRASTERFORMATPIXELFORMATMASK) {
        case rwRASTERFORMATLUM8: {
            RwThrowErrorParams(1, 2, "rwRASTERFORMATLUM8 not yet supported");
            break;
        }
        case rwRASTERFORMAT555:
            fn = _rwDlConv555To8888;
            break;
        case rwRASTERFORMAT565:
            fn = _rwDlConv565To8888;
            break;
        case rwRASTERFORMAT1555:
            fn = _rwDlConv1555To8888;
            break;
        case rwRASTERFORMAT4444:
            fn = _rwDlConv4444To8888;
            break;
        case rwRASTERFORMAT8888:
            fn = _rwDlConvDl8888To8888;
            break;
        case rwRASTERFORMAT888:
            fn = _rwDlConvDl888To8888;
            break;
        default: {
            RwThrowError(1, E_RW_INVRASTERFORMAT);
            break;
        }
    }

    return fn;
}

void _rwDlImage4GetFromRaster(RwImage* image, RwRaster* raster) {
    UnconvertFn fn = _rwDlSelectUnconvertFn((raster->cFormat & 0xFF) << 8);

    switch (raster->depth) {
        case 4: {
            int i, j;
            for (i = 0; i < 16; i++) {
                fn(&image->palette[i], ((unsigned short*)raster->palette)[i]);
            }
            for (i = 0; i < raster->height; i++) {
                unsigned char* imageBuf = image->cpPixels + image->stride * i;
                unsigned char* rasterBuf = raster->cpPixels + raster->stride * i;
                // This feels fake
                for (j = 0; j < raster->width; j += 2) {
                    imageBuf[0] = (rasterBuf[0] >> 4) & 0xF;
                    imageBuf[1] = rasterBuf[0] & 0xF;
                    imageBuf += 2;
                    rasterBuf += 1;
                }
            }
            break;
        }
        case 8:
        case 16:
        case 32: {
            RwThrowErrorParams(1, 2, "Conversion from 8/16/32bit rasters to 4bit images is not supported");
            break;
        }
        case 24:
        default: {
            RwThrowError(1, E_RW_INVRASTERDEPTH);
            break;
        }
    }
}

void _rwDlImage8GetFromRaster(RwImage* image, RwRaster* raster) {
    UnconvertFn fn = _rwDlSelectUnconvertFn((raster->cFormat & 0xFF) << 8);

    switch (raster->depth) {
        case 4: {
            int i, j;
            for (i = 0; i < 16; i++) {
                fn(&image->palette[i], ((unsigned short*)raster->palette)[i]);
            }
            for (i = 0; i < raster->height; i++) {
                unsigned char* imageBuf = image->cpPixels + image->stride * i;
                unsigned char* rasterBuf = raster->cpPixels + raster->stride * i;
                for (j = 0; j < raster->width; j += 2) {
                    imageBuf[0] = (rasterBuf[0] >> 4) & 0xF;
                    imageBuf[1] = rasterBuf[0] & 0xF;
                    imageBuf += 2;
                    rasterBuf += 1;
                }
            }
            break;
        }
        case 8: {
            int i, j;
            for (i = 0; i < 256; i++) {
                fn(&image->palette[i], ((unsigned short*)raster->palette)[i]);
            }
            for (i = 0; i < raster->height; i++) {
                unsigned char* rasterBuf = raster->cpPixels + raster->stride * i;
                unsigned char* imageBuf = image->cpPixels + image->stride * i;
                for (j = 0; j < raster->width; j++) {
                    *imageBuf = *rasterBuf;
                    imageBuf += 1;
                    rasterBuf += 1;
                }
            }
            break;
        }
        case 16:
        case 32: {
            RwThrowErrorParams(1, 2, "Conversion from 16/32bit rasters to 8bit images is not supported");
            break;
        }
        case 24:
        default: {
            RwThrowError(1, E_RW_INVRASTERDEPTH);
            break;
        }
    }
}

void _rwDlImage32GetFromRaster(RwImage* image, RwRaster* raster) {
    UnconvertFn fn = _rwDlSelectUnconvertFn((raster->cFormat & 0xFF) << 8);

    switch (raster->depth) {
        case 4: {
            RwRGBA buf[16];
            int i, j;
            for (i = 0; i < 16; i++) {
                fn(&buf[i], ((unsigned short*)raster->palette)[i]);
            }
            for (i = 0; i < raster->height; i++) {
                RwRGBA* imageBuf = (RwRGBA*)(image->cpPixels + image->stride * i);
                unsigned char* rasterBuf = (raster->cpPixels + raster->stride * i);
                for (j = 0; j < raster->width; j += 2) {
                    *(imageBuf++) = buf[(*rasterBuf >> 4) & 0xF];
                    *(imageBuf++) = buf[*(rasterBuf++) & 0xF];
                }
            }
            break;
        }
        case 8: {
            RwRGBA buf[256];
            int i, j;
            for (i = 0; i < 256; i++) {
                fn(&buf[i], ((unsigned short*)raster->palette)[i]);
            }
            for (i = 0; i < raster->height; i++) {
                unsigned char* rasterBuf = (raster->cpPixels + raster->stride * i);
                RwRGBA* imageBuf = (RwRGBA*)(image->cpPixels + image->stride * i);
                for (j = 0; j < raster->width; rasterBuf++, j++) {
                    *(imageBuf++) = buf[*(rasterBuf)];
                }
            }
            break;
        }
        case 16: {
            int i, j;
            for (i = 0; i < raster->height; i++) {
                unsigned short* rasterBuf = (unsigned short*)(raster->cpPixels + raster->stride * i);
                RwRGBA* imageBuf = (RwRGBA*)(image->cpPixels + image->stride * i);
                for (j = 0; j < raster->width; j++) {
                    fn(imageBuf, *rasterBuf);
                    imageBuf += 1;
                    rasterBuf += 1;
                }
            }
            break;
        }
        case 24: {
            RwThrowError(1, E_RW_INVRASTERDEPTH);
            break;
        }
        case 32: {
            int i, j;
            for (i = 0; i < raster->height; i++) {
                unsigned int* rasterBuf = (unsigned int*)(raster->cpPixels + raster->stride * i);
                RwRGBA* imageBuf = (RwRGBA*)(image->cpPixels + image->stride * i);
                for (j = 0; j < raster->width; j++) {
                    fn(imageBuf, *rasterBuf);
                    imageBuf += 1;
                    rasterBuf += 1;
                }
            }
            break;
        }
        default: {
            RwThrowError(1, E_RW_INVRASTERDEPTH);
            break;
        }
    }
}

int _rwDlImageGetFromRaster(void* imagePtr, void* rasterPtr) {
    RwImage* image = imagePtr;
    RwRaster* raster = rasterPtr;
    int lockedRaster = 0;
    int lockedPalette = 0;

    if (!(raster->privateFlags & rwRASTERPIXELLOCKEDREAD)) {
        RwRasterLock(raster, 0, 2);
        lockedRaster = 1;
    }

    if (((raster->cFormat & 0x60) << 8) && !(raster->privateFlags & rwRASTERPALETTELOCKEDREAD)) {
        RwRasterLockPalette(raster, 2);
        lockedPalette = 1;
    }

    switch (image->depth) {
        case 4:
            _rwDlImage4GetFromRaster(image, raster);
            break;
        case 8:
            _rwDlImage8GetFromRaster(image, raster);
            break;
        case 32:
            _rwDlImage32GetFromRaster(image, raster);
            break;
        default: {
            RwThrowError(1, E_RW_INVIMAGEDEPTH);
            break;
        }
    }

    if (lockedPalette == 1) {
        RwRasterUnlockPalette(raster);
    }
    if (lockedRaster == 1) {
        RwRasterUnlock(raster);
    }

    return 1;
}

ConvertFn _rwDlSelectConvertFn(RwRaster* raster) {
    ConvertFn fn = NULL;
    DONT_INLINE_HACK();

    switch ((raster->cFormat << 8) & rwRASTERFORMATPIXELFORMATMASK) {
        case rwRASTERFORMATLUM8: {
            RwThrowErrorParams(1, 2, "rwRASTERFORMATLUM8 not yet supported");
            break;
        }
        case rwRASTERFORMAT555:
            fn = _rwDlConv8888To555;
            break;
        case rwRASTERFORMAT565:
            fn = _rwDlConv8888To565;
            break;
        case rwRASTERFORMAT1555:
        case rwRASTERFORMAT4444:
            fn = _rwDlConv8888To555or3444;
            break;
        case rwRASTERFORMAT888:
            fn = _rwDlConv8888ToDl888;
            break;
        case rwRASTERFORMAT8888:
            fn = _rwDlConv8888ToDl8888;
            break;
        default: {
            RwThrowError(1, E_RW_INVRASTERFORMAT);
            break;
        }
    }

    return fn;
}

extern int _RwGameCubeRasterExtOffset;
// Equivalent?
void _rwDlRasterPalletized4SetFromImage(RwRaster* raster, RwImage* image) {
    int offset = _RwGameCubeRasterExtOffset;
    unsigned char* unkPtr = (unsigned char*)(raster->parent) + offset;
    ConvertFn fn = _rwDlSelectConvertFn(raster);

    switch (image->depth) {
        case 4: {
            int i;
            unsigned short* rPalette;
            int j;
            for (i = 0; i < raster->height; i++) {
                unsigned char* imageBuf = (unsigned char*)(image->cpPixels + image->stride * i);
                unsigned char* rasterBuf = (unsigned char*)(raster->cpPixels + raster->stride * i);
                for (j = 0; j < raster->width; j += 2) {
                    int value = (*(imageBuf++) & 0xF) << 4;
                    value |= *(imageBuf++) & 0xF;
                    *(rasterBuf++) = value;
                }
            }

            if (*(unkPtr + 0x33) != 0) {
                return;
            }

            rPalette = (unsigned short*)raster->palette;
            for (i = 0; i < 16; i++) {
                (rPalette[i]) = fn(&image->palette[i]);
            }
            break;
        }
        case 8:
        case 32: {
            int depth = raster->depth;
            RwImage* temp = RwImageCreate(image->width, image->height, depth);
            if (temp == NULL) {
                temp = NULL;
            } else {
                RwPalQuant palquant;
                RwImageAllocatePixels(temp);
                if (!_rwPalQuantInit(&palquant)) {
                    temp = NULL;
                } else {
                    _rwPalQuantAddImage(1.0f, &palquant, image);
                    _rwPalQuantResolvePalette(temp->palette, 1 << depth, &palquant);
                    _rwPalQuantMatchImage(temp->cpPixels, temp->stride, temp->depth, 0, &palquant, image);
                    _rwPalQuantTerm(&palquant);
                }
            }

            if (temp != NULL) {
                _rwDlRasterPalletized4SetFromImage(raster, temp);
                RwImageDestroy(temp);
            }
            break;
        }
        default: {
            RwThrowError(1, E_RW_INVIMAGEDEPTH);
            break;
        }
    }
}

// Equivalent?
void _rwDlRasterPalletized8SetFromImage(RwRaster* raster, RwImage* image) {
    ConvertFn fn;
    fn = _rwDlSelectConvertFn(raster);

    switch (image->depth) {
        case 4:
        case 8: {
            int i;
            int offset = _RwGameCubeRasterExtOffset;
            unsigned char* unkPtr = (unsigned char*)(raster->parent) + offset;
            int j;
            unsigned short* rPalette;
            for (i = 0; i < raster->height; i++) {
                unsigned char* imageBuf = (unsigned char*)(image->cpPixels + image->stride * i);
                unsigned char* rasterBuf = (unsigned char*)(raster->cpPixels + raster->stride * i);
                for (j = 0; j < raster->width; j++) {
                    *rasterBuf = *imageBuf;
                    rasterBuf++;
                    imageBuf++;
                }
            }

            if (*(unkPtr + 0x33) != 0) {
                return;
            }

            rPalette = (unsigned short*)raster->palette;
            for (i = 0; i < 1 << image->depth; i++) {
                (rPalette[i]) = fn(&image->palette[i]);
            }
            break;
        }
        case 32: {
            int depth = raster->depth;
            RwImage* temp = RwImageCreate(image->width, image->height, depth);
            if (temp == NULL) {
                temp = NULL;
            } else {
                RwPalQuant palquant;
                RwImageAllocatePixels(temp);
                if (!_rwPalQuantInit(&palquant)) {
                    temp = NULL;
                } else {
                    _rwPalQuantAddImage(1.0f, &palquant, image);
                    _rwPalQuantResolvePalette(temp->palette, 1 << depth, &palquant);
                    _rwPalQuantMatchImage(temp->cpPixels, temp->stride, temp->depth, 0, &palquant, image);
                    _rwPalQuantTerm(&palquant);
                }
            }

            if (temp != NULL) {
                _rwDlRasterPalletized4SetFromImage(raster, temp);
                RwImageDestroy(temp);
            }
            break;
        }
        default: {
            RwThrowError(1, E_RW_INVIMAGEDEPTH);
            break;
        }
    }
}

void _rwDlRaster16SetFromImage(RwRaster* raster, RwImage* image) {
    ConvertFn fn;
    fn = _rwDlSelectConvertFn(raster);

    switch (image->depth) {
        case 4:
        case 8: {
            unsigned short buf[256];
            int i, j;
            for (i = 0; i < 1 << image->depth; i++) {
                (buf[i]) = fn(&image->palette[i]);
            }
            for (i = 0; i < raster->height; i++) {
                unsigned char* imageBuf = (unsigned char*)(image->cpPixels + image->stride * i);
                unsigned short* rasterBuf = (unsigned short*)(raster->cpPixels + raster->stride * i);
                for (j = 0; j < raster->width; j++) {
                    *rasterBuf = buf[*imageBuf];
                    imageBuf++;
                    rasterBuf++;
                }
            }

            break;
        }
        case 32: {
            int i, j;
            for (i = 0; i < raster->height; i++) {
                RwRGBA* imageBuf = (RwRGBA*)(image->cpPixels + image->stride * i);
                unsigned short* rasterBuf = (unsigned short*)(raster->cpPixels + raster->stride * i);
                for (j = 0; j < raster->width; j++) {
                    *rasterBuf = fn(imageBuf);
                    rasterBuf++;
                    imageBuf++;
                }
            }

            break;
        }
        default: {
            RwThrowError(1, E_RW_INVIMAGEDEPTH);
            break;
        }
    }
}

void _rwDlRaster32SetFromImage(RwRaster* raster, RwImage* image) {
    ConvertFn fn;
    fn = _rwDlSelectConvertFn(raster);

    switch (image->depth) {
        case 4:
        case 8: {
            unsigned int buf[256];
            int i, j;
            for (i = 0; i < 1 << image->depth; i++) {
                (buf[i]) = fn(&image->palette[i]);
            }
            for (i = 0; i < raster->height; i++) {
                unsigned char* imageBuf = (unsigned char*)(image->cpPixels + image->stride * i);
                unsigned int* rasterBuf = (unsigned int*)(raster->cpPixels + raster->stride * i);
                for (j = 0; j < raster->width; j++) {
                    *rasterBuf = buf[*imageBuf];
                    imageBuf++;
                    rasterBuf++;
                }
            }

            break;
        }
        case 32: {
            int i, j;
            for (i = 0; i < raster->height; i++) {
                RwRGBA* imageBuf = (RwRGBA*)(image->cpPixels + image->stride * i);
                unsigned int* rasterBuf = (unsigned int*)(raster->cpPixels + raster->stride * i);
                for (j = 0; j < raster->width; j++) {
                    *rasterBuf = fn(imageBuf);
                    rasterBuf++;
                    imageBuf++;
                }
            }

            break;
        }
        default: {
            RwThrowError(1, E_RW_INVIMAGEDEPTH);
            break;
        }
    }
}

// Incomplete
int _rwDlRasterSetFromImage(void* rasterPtr, void* imagePtr) {
    RwImage* image = imagePtr;
    RwRaster* raster = rasterPtr;
    int lockedPalette = 0;
    int lockedRaster = 0;
    int cFormat = (raster->cFormat & 0xFF) << 8;
    int cFormatPal;

    lockedRaster = (raster->privateFlags & rwRASTERPIXELLOCKEDWRITE);
    if (!lockedRaster) {
        if (!RwRasterLock(raster, 0, 5)) {
            return 0;
        }
    } else {
        cFormatPal = cFormat & (rwRASTERFORMATPAL4 | rwRASTERFORMATPAL8);
        lockedPalette = (raster->privateFlags & rwRASTERPALETTELOCKEDWRITE);
        if (!(cFormatPal) || lockedPalette) {
            if (!RwRasterLockPalette(raster, 5)) {
                return 0;
            }
        }
    }

    switch (raster->depth) {
        case 4:
            _rwDlRasterPalletized4SetFromImage(raster, image);
            break;
        case 8:
            _rwDlRasterPalletized8SetFromImage(raster, image);
            break;
        case 16:
            _rwDlRaster16SetFromImage(raster, image);
            break;
        case 24: {
            RwThrowError(1, E_RW_INVRASTERDEPTH);
            break;
        }
        case 32:
            _rwDlRaster32SetFromImage(raster, image);
            break;
        default: {
            RwThrowError(1, E_RW_INVRASTERDEPTH);
            break;
        }
    }

    if (cFormatPal && !lockedPalette) {
        RwRasterUnlockPalette(raster);
    }
    if (!lockedRaster) {
        RwRasterUnlock(raster);
    }

    return 1;
}

// Equivalent?
RwRasterFormat _rwDlImageFindFormat(RwImage* image) {
    RwRasterFormat format;
    int bVar2 = 0;
    if (image->depth == 4 || image->depth == 8) {
        int width = image->width;
        int height = image->height;
        unsigned char* pixels = image->cpPixels;
        RwRGBA* palette = image->palette;
        int i, j;
        for (i = 0; i < height; i++) {
            for (j = 0; j < width; j++, pixels++) {
                unsigned char alpha = palette[*pixels].alpha;
                if (alpha != 0xFF) {
                    bVar2 = 1;
                    if (alpha > 0xF) {
                        format = rwRASTERFORMAT4444;
                        if (image->depth == 4) {
                            format |= rwRASTERFORMATPAL4;
                        } else {
                            format |= rwRASTERFORMATPAL8;
                        }
                        return format;
                    }
                }
            }
            pixels += image->stride;
        }
    } else {
        int width = image->width;
        int height = image->height;
        unsigned char* pixels = image->cpPixels;
        int i, j;
        for (i = 0; i < height; i++) {
            for (j = 0; j < width; j++) {
                unsigned char alpha = pixels[3];
                if (alpha != 0xFF) {
                    bVar2 = 1;
                    if (alpha > 0xF) {
                        return rwRASTERFORMAT4444;
                    }
                }
                pixels += 4;
            }
            pixels += image->stride;
        }
    }

    format = rwRASTERFORMAT565;
    if (bVar2) {
        format = rwRASTERFORMAT1555;
    }

    if (image->depth == 4) {
        format |= rwRASTERFORMATPAL4;
    } else if (image->depth == 8) {
        format |= rwRASTERFORMATPAL8;
    }

    return format;
}

int _rwDlImageFindRasterFormat(void* rasterPtr, void* imagePtr, int a2) {
    RwRaster* raster = rasterPtr;
    RwImage* image = imagePtr;

    raster->cType = a2 & rwRASTERTYPEMASK;
    raster->depth = 0;
    switch (a2 & rwRASTERTYPEMASK) {
        case rwRASTERTYPENORMAL:
        case rwRASTERTYPETEXTURE:
        case rwRASTERTYPECAMERATEXTURE: {
            int format;
            raster->width = (image->width <= 1024) ? image->width : 1024;
            raster->height = (image->height <= 1024) ? image->height : 1024;

            if (a2 & 0x8000) {
                raster->width = 1 << _rwDlFindMSB(raster->width);
                raster->height = 1 << _rwDlFindMSB(raster->height);
            }

            format = _rwDlImageFindFormat(image);
            raster->cFormat = (format | (a2 & 0x9000)) >> 8;
            return 1;
        }
        case rwRASTERTYPEZBUFFER:
        case rwRASTERTYPECAMERA:
            raster->cFormat = 0;
            raster->width = image->width;
            raster->height = image->height;
            return 1;
        default:
        case rwRASTERTYPEUNK3: {
            RwThrowError(1, E_RW_INVRASTERFORMAT);
            return 0;
        }
    }
}
