#ifndef RWSDK_BAIMRAS_H
#define RWSDK_BAIMRAS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <rwsdk/baimage.h>
#include <rwsdk/baraster.h>

RwImage* RwImageSetFromRaster(RwImage* image, RwRaster* raster);
RwRaster* RwRasterSetFromImage(RwRaster* raster, RwImage* image);

#ifdef __cplusplus
}
#endif

#endif
