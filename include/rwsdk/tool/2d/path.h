#ifndef RWSDK_TOOL_2D_PATH_H
#define RWSDK_TOOL_2D_PATH_H

#include "rwsdk/baimage.h"
#include "rwsdk/batextur.h"
#include "rwsdk/plcore/bavector.h"

#ifdef __cplusplus
extern "C" {
#endif

// TODO
typedef struct Rt2dPath {
    char unk0;
} Rt2dPath; // unknown size

Rt2dPath* Rt2dPathRect(Rt2dPath* path, float, float, float, float);
int Rt2dPathDestroy(Rt2dPath* brush);
Rt2dPath* Rt2dPathCreate(void);
Rt2dPath* Rt2dPathLock(Rt2dPath* path);
Rt2dPath* Rt2dPathUnlock(Rt2dPath* path);

#ifdef __cplusplus
}
#endif

#endif
