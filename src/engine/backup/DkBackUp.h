#ifndef ENGINE_BACKUP_DKBACKUP_H
#define ENGINE_BACKUP_DKBACKUP_H

#include "engine/backup/CGCNBAKEngine.h"

extern DKBAK::CGCNBAKEngine* gs_TheEngine;

#ifdef __cplusplus
extern "C" {
#endif

DKBAK::CGCNBAKEngine* DkBakUpGetEngine(void);
void DkBakUpRelease(void);

#ifdef __cplusplus
}
#endif

#endif
