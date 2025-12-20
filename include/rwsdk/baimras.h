#ifndef RWSDK_BAIMRAS_H
#define RWSDK_BAIMRAS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <rwsdk/baraster.h>

typedef struct RwImage RwImage;

RwImage* RwImageSetFromRaster(RwImage* image, RwRaster* raster);
RwRaster* RwRasterSetFromImage(RwRaster* raster, RwImage* image);

#ifdef __cplusplus
}
#endif

#endif
