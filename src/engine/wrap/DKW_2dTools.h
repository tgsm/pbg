#ifndef ENGINE_WRAP_DKW_2DTOOLS_H
#define ENGINE_WRAP_DKW_2DTOOLS_H

#include <rwsdk/tool/2d/brush.h>
#include <rwsdk/tool/2d/fill.h>
#include <rwsdk/tool/2d/path.h>
#include "engine/display/CTexture.h"
#include "engine/wrap/DKW_RGBA.h"
#include "engine/wrap/DKW_V2d.h"

class CDKW_2dBrush {
public:
    Rt2dBrush* m_rw_brush;

public:
    CDKW_2dBrush();
    ~CDKW_2dBrush();

    BOOL Create();
    BOOL SetRGBA(CDKW_RGBA*, CDKW_RGBA*, CDKW_RGBA*, CDKW_RGBA*);
    BOOL SetUV(CDKW_V2d*, CDKW_V2d*, CDKW_V2d*, CDKW_V2d*);
    BOOL SetTexture(CDKW_Texture*);
};
REQUIRE_SIZE(CDKW_2dBrush, 0x4);

// Nothing here is guaranteed to be official. Basically all of this class is inlined (and does not show up in the ELF).
class CDKW_2dPath {
public:
    Rt2dPath* m_rw_path;

public:
    CDKW_2dPath() {
        m_rw_path = NULL;
        if (m_rw_path != NULL && m_rw_path != NULL) {
            if (Rt2dPathDestroy(m_rw_path)) {
                m_rw_path = NULL;
            }
        }

        Create();
    }
    ~CDKW_2dPath() {
        if (m_rw_path != NULL) {
            if (Rt2dPathDestroy(m_rw_path)) {
                m_rw_path = NULL;
            }
        }
    }

    void Create() {
        Rt2dPath* path = Rt2dPathCreate();
        if (path != NULL) {
            m_rw_path = path;
        }
    }

    void Lock() {
        Rt2dPath* path = Rt2dPathLock(m_rw_path);
        if (path != NULL) {
            m_rw_path = path;
        }
    }

    void Unlock() {
        Rt2dPath* path = Rt2dPathUnlock(m_rw_path);
        if (path != NULL) {
            m_rw_path = path;
        }
    }

    void Rect(F32 a1, F32 a2, F32 a3, F32 a4) {
        Rt2dPath* path = Rt2dPathRect(m_rw_path, a1, a2, a3, a4);
        if (path != NULL) {
            m_rw_path = path;
        }
    }

    void Fill(CDKW_2dBrush* brush) {
        Rt2dPath* path = Rt2dPathFill(m_rw_path, brush->m_rw_brush);
        if (path != NULL) {
            m_rw_path = path;
        }
    }
};
REQUIRE_SIZE(CDKW_2dPath, 0x4);

#endif
