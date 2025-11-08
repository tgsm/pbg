#include "engine/filesys/DkFileSys.h"

BOOL CDkFileSys::loadfat(char* path, DkXmd::CXmdFile* xmd) {
    DkFile* file = (DkFile*)RWFileInterface.rwfopen(path, "rb");
    if (file == NULL) {
        return FALSE;
    }

    RWFileInterface.rwfseek(file, 0, 2); // SEEK_END
    m_FatSize = RWFileInterface.rwftell(file);
    m_FatMemEntry = new U8[m_FatSize];
    if (m_FatMemEntry == NULL) {
        return FALSE;
    }

    RWFileInterface.rwfseek(file, 0, 0); // SEEK_SET
    RWFileInterface.rwfread(m_FatMemEntry, 1, m_FatSize, file);
    RWFileInterface.rwfclose(file);

    return xmd->Parse(m_FatMemEntry) != FALSE;
}

unsigned char* CDkFileSys::KeepOnlyFileName(char* path) {
    unsigned int ch;
    unsigned char* ret = (U8*)path;
    for (; path != NULL && (ch = *(U8*)path, ch != '\0'); path++) {
        if (ch == '/' || ch == '\\') {
            ret = (U8*)path + 1;
        }
    }
    return ret;
}
