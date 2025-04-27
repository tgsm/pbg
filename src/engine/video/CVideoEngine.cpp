#include "video/CVideoEngine.h"

namespace DKVIDEO {

CVideoEngine::CVideoEngine() {
    m_callback = NULL;
    m_volume = 1.0f;
}

CVideoEngine::~CVideoEngine() {

}

void CVideoEngine::SetCallBack(VideoEngineCallback callback) {
    m_callback = callback;
}

}
