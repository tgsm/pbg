#ifndef RWSDK_TOOL_2D_FILL_H
#define RWSDK_TOOL_2D_FILL_H

#include "rwsdk/tool/2d/brush.h"
#include "rwsdk/tool/2d/path.h"

#ifdef __cplusplus
extern "C" {
#endif

Rt2dPath* Rt2dPathFill(Rt2dPath* path, Rt2dBrush* brush);

#ifdef __cplusplus
}
#endif

#endif
