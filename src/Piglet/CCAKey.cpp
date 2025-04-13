#include <cstring>
#include "CCAKey.h"
#include "engine/DKW_V3d.h"

extern CDKW_V3d ReadVector(DkXmd::CChunkIterator*);

// Equivalent: stack offsets
CCAKey::CCAKey(DkXmd::CChunkIterator* iter, char* name) {
    DkXmd::CChunkIterator dest;

    volatile BOOL unk6;
    volatile BOOL unk5;
    volatile BOOL unk4;
    volatile BOOL unk3;
    volatile BOOL unk2;
    volatile BOOL unk1;

    m_name = NULL;

    int iVar2 = iter->GetFirstChildChunk(dest);

    unk1 = FALSE;
    unk2 = FALSE;
    unk3 = FALSE;
    unk4 = FALSE;
    unk5 = FALSE;
    unk6 = FALSE;

    do {
        if (strcmp("ID", dest.GetName()) == 0) {
            const u8* value = (const u8*)dest.GetStringValue();
            u32 i;
            for (i = 0; value[i] != 0u && i < 0x400; i++) {

            }
            m_name = name;
            memcpy(m_name, value, i + 1);

            unk1 = TRUE;
        } else if (strcmp("POSITION", dest.GetName()) == 0) {
            m_unk4 = ReadVector(&dest);
            unk2 = TRUE;
        } else if (strcmp("CAMERA", dest.GetName()) == 0) {
            m_unk10 = ReadVector(&dest);
            unk3 = TRUE;
        } else if (strcmp("TARGET", dest.GetName()) == 0) {
            m_unk1C = ReadVector(&dest);
            unk4 = TRUE;
        } else if (strcmp("FOV", dest.GetName()) == 0) {
            if (dest.GetType() == 2) {
                m_fov = dest.GetFloatValue();
            } else if (dest.GetType() == 3) {
                m_fov = dest.GetS32Value();
            }
            unk5 = TRUE;
        } else if (strcmp("ROLL", dest.GetName()) == 0) {
            if (dest.GetType() == 2) {
                m_roll = dest.GetFloatValue();
            } else if (dest.GetType() == 3) {
                m_roll = dest.GetS32Value();
            }
            unk6 = TRUE;
        }

        iVar2 = dest.GetNextSiblingChunk(dest);
    } while (iVar2 != 0);
}

CCAKey::~CCAKey() {
    m_name = NULL;
}

char* CCAKey::GetName() {
    return m_name;
}
