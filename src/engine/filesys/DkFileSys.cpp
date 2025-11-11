#include "engine/filesys/DkFileSys.h"

std::vector<DkFileSysDescriptor*> CDkFileSys::m_pUnCompressedMemory;
DkXmd::CChunkIterator CDkFileSys::m_OnFatPosition;
RwFileFunctions RWFileInterface;
