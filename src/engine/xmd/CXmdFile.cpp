#include "xmd/CXmdFile.h"
#include <cstring>

namespace DkXmd {

CXmdFile::CXmdFile() {
    m_unk4 = NULL;
    m_unk14 = 0;
}

CXmdFile::~CXmdFile() {
    if (m_unk4 != NULL) {
        delete[] m_unk4;
    }
}

}
