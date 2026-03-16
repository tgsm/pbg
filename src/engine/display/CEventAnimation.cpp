#include <rwsdk/badevice.h>
#include "engine/display/CEngine.h"
#include "engine/display/CEventAnimation.h"

namespace DKDSP {

CEventAnimation::CEventAnimation() : m_name("") {
    m_duration = 0.0f;
    m_key_table = NULL;
    m_nb_events = 0;
    m_unk14 = NULL;
    m_xmd = NULL;
}

CEventAnimation::~CEventAnimation() {
    Release();
}

void CEventAnimation::Release() {
    m_duration = 0.0f;
    m_nb_events = 0;

    if (m_key_table != NULL) {
        delete[] m_key_table;
    }
    m_key_table = NULL;

    if (m_unk14 != NULL) {
        RwEngineInstance->memoryFuncs.rwfree(m_unk14);
    }
    m_unk14 = NULL;

    if (m_xmd != NULL) {
        delete m_xmd;
    }
    m_xmd = NULL;
}

EANIMATIONTYPE CEventAnimation::GetType() {
    return ANIMATION_TYPE_EVN;
}

BOOL CEventAnimation::Create(int nb_events, F32 duration, DkXmd::CXmdFile* xmd, int) {
    m_duration = duration;
    m_nb_events = nb_events;
    m_xmd = xmd;

    if (m_nb_events > 0) {
        m_key_table = new SEVENTANIMKEY[m_nb_events];
    } else {
        m_key_table = NULL;
    }

    return TRUE;
}

void CEventAnimation::SetKey(int key_no, SEVENTANIMKEY& key) {
    if (key_no < 0 || key_no >= m_nb_events) {
        return;
    }

    m_key_table[key_no].unk0 = key.unk0;
    m_key_table[key_no].id = key.id;
    m_key_table[key_no].m_chunk_iterator = key.m_chunk_iterator;
}

F32 CEventAnimation::GetDuration() {
    return m_duration;
}

void CEventAnimation::SetDuration(F32 duration) {
    m_duration = duration;
}

U32 CEventAnimation::GetNumberOfEvents() {
    return m_nb_events;
}

U32 CEventAnimation::GetSize() {
    return sizeof(CEventAnimation) + m_nb_events * sizeof(SEVENTANIMKEY);
}

SEVENTANIMKEY* CEventAnimation::GetKeyTable() {
    return m_key_table;
}

std::string* CEventAnimation::GetName() {
    return &m_name;
}

void CEventAnimation::SetName(std::string name) {
    m_name.assign(name, 0);
}

void CEventAnimation::Started(void* data) {
    for (int i = 0; i < m_nb_events; i++) {
        SEVENTANIMATIONCALLBACKENTRY* entry = NULL;
        if (m_key_table[i].id >= 0) {
            entry = CEngine::ms_pEngine->GetEvent(m_key_table[i].id);
        }
        if (entry != NULL && entry->callback != NULL) {
            entry->callback->Start(data);
        }
    }
}

void CEventAnimation::Stopped(void* data) {
    for (int i = 0; i < m_nb_events; i++) {
        SEVENTANIMATIONCALLBACKENTRY* entry = NULL;
        if (m_key_table[i].id >= 0) {
            entry = CEngine::ms_pEngine->GetEvent(m_key_table[i].id);
        }
        if (entry != NULL && entry->callback != NULL) {
            entry->callback->Stop(data);
        }
    }
}

void CEventAnimation::Paused(void* data) {
    for (int i = 0; i < m_nb_events; i++) {
        SEVENTANIMATIONCALLBACKENTRY* entry = NULL;
        if (m_key_table[i].id >= 0) {
            entry = CEngine::ms_pEngine->GetEvent(m_key_table[i].id);
        }
        if (entry != NULL && entry->callback != NULL) {
            entry->callback->Pause(data);
        }
    }
}

void CEventAnimation::Resumed(void* data) {
    for (int i = 0; i < m_nb_events; i++) {
        SEVENTANIMATIONCALLBACKENTRY* entry = NULL;
        if (m_key_table[i].id >= 0) {
            entry = CEngine::ms_pEngine->GetEvent(m_key_table[i].id);
        }
        if (entry != NULL && entry->callback != NULL) {
            entry->callback->Resume(data);
        }
    }
}

}
