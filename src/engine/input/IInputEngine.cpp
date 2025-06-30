#include "engine/input/IInputEngine.h"
#include "engine/input/CGCNPad.h"

namespace DKI {

DkXmd::CChunkIterator IInputEngine::m_ChunkSetting;
// TODO: These are also vectors of size_t when we insert stuff into them, how do we make that work?
std::vector<IInput*> IInputEngine::m_VInputs;
std::vector<IDevice*> IInputEngine::m_VDevices;

// FIXME: Get rid of this
#define CLEAR_VECTOR(vec) \
    vec.m_size = 0; \
    if (vec.m_data != NULL) { \
        delete vec.m_data; \
    } \
    vec.m_data = NULL; \
    vec.m_capacity = 0;

void IInputEngine::Close() {
    for (size_t i = 0; i < m_VDevices.size(); i++) {
        delete m_VDevices[i];
    }

    CLEAR_VECTOR(m_VDevices);
    CLEAR_VECTOR(m_VInputs);
}

void IInputEngine::Clear() {
    for (size_t i = 0; i < m_VInputs.size(); i++) {
        delete m_VInputs[i];
    }

    CLEAR_VECTOR(m_VInputs);
}

BOOL IInputEngine::Update(F32 dt) {
    U32 i;
    BOOL ret = TRUE;

    CGCNPad::Update();

    for (i = 0; i < m_VDevices.size(); i++) {
        m_VDevices[i]->Update(dt);
        if (!m_VDevices[i]->IsConnected()) {
            ret = FALSE;
        }
    }

    return ret;
}

}
