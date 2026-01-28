#include "engine/input/IInputEngine.h"
#include "engine/input/CGCNPad.h"
#include "engine/input/CInputHalfAxe.h"

namespace DKI {

DkXmd::CChunkIterator IInputEngine::m_ChunkSetting;
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

IDevice* IInputEngine::GetDeviceInline(int id) {
    unsigned long device = NULL;
    DkXmd::CChunkIterator unused;

    switch (id) {
        case 0:
            device = reinterpret_cast<unsigned long>(CGCNPad::ms_Pad1);
            if (device == NULL) {
                if (CGCNPad::ms_Pad1 == NULL) {
                    CGCNPad::ms_Pad1 = new CGCNPad(0);
                    CGCNPad::ms_Pad1->Init(NULL);
                }
                device = reinterpret_cast<unsigned long>(CGCNPad::ms_Pad1);
                AS_ULONG_VECTOR_HACK(m_VDevices).push_back(device);
            }
            return reinterpret_cast<IDevice*>(device);
        case 1:
            device = reinterpret_cast<unsigned long>(CGCNPad::ms_Pad2);
            if (device == NULL) {
                if (CGCNPad::ms_Pad2 == NULL) {
                    CGCNPad::ms_Pad2 = new CGCNPad(1);
                    CGCNPad::ms_Pad2->Init(NULL);
                }
                device = reinterpret_cast<unsigned long>(CGCNPad::ms_Pad2);
                AS_ULONG_VECTOR_HACK(m_VDevices).push_back(device);
            }
            return reinterpret_cast<IDevice*>(device);
        case 2:
            device = reinterpret_cast<unsigned long>(CGCNPad::ms_Pad3);
            if (device == NULL) {
                if (CGCNPad::ms_Pad3 == NULL) {
                    CGCNPad::ms_Pad3 = new CGCNPad(2);
                    CGCNPad::ms_Pad3->Init(NULL);
                }
                device = reinterpret_cast<unsigned long>(CGCNPad::ms_Pad3);
                AS_ULONG_VECTOR_HACK(m_VDevices).push_back(device);
            }
            return reinterpret_cast<IDevice*>(device);
        case 3:
            device = reinterpret_cast<unsigned long>(CGCNPad::ms_Pad4);
            if (device == NULL) {
                if (CGCNPad::ms_Pad4 == NULL) {
                    CGCNPad::ms_Pad4 = new CGCNPad(3);
                    CGCNPad::ms_Pad4->Init(NULL);
                }
                device = reinterpret_cast<unsigned long>(CGCNPad::ms_Pad4);
                AS_ULONG_VECTOR_HACK(m_VDevices).push_back(device);
            }
            return reinterpret_cast<IDevice*>(device);
        case 4:
            return reinterpret_cast<IDevice*>(device);
        case 5:
            return reinterpret_cast<IDevice*>(device);
    }

    return reinterpret_cast<IDevice*>(device);
}

IDevice* IInputEngine::GetDevice(int id) {
    return GetDeviceInline(id);
}

void __deadstripped_function() {
    IDevice* device;
    device->SetRumble(0, 0);
}

void IInputEngine::DestroyInput(IInput *input) {
    for (std::vector<IInput*>::iterator iter = m_VInputs.begin(); iter != m_VInputs.end(); iter++) {
        if (*iter == input) {
            delete *iter;
            *iter = NULL;
            AS_ULONG_VECTOR_HACK(m_VInputs).erase(reinterpret_cast<unsigned long*>(iter));
            return;
        }
    }
}

CCombo* IInputEngine::CreateCombo(char* name, char* input1_name, char* input2_name, int a4) {
    IInput* input1;
    IInput* input2;

    input1 = GetInput(input1_name);
    input2 = GetInput(input2_name);

    if (input1 == NULL || input2 == NULL) {
        return NULL;
    }

    unsigned long combo = reinterpret_cast<unsigned long>(new CCombo(name, input1, input2, a4));
    AS_ULONG_VECTOR_HACK(m_VInputs).insert(reinterpret_cast<unsigned long*>(m_VInputs.data() + m_VInputs.size()), 1, combo);
    // AS_ULONG_VECTOR_HACK(m_VInputs).push_back(combo);
    return reinterpret_cast<CCombo*>(combo);
}

CInput* IInputEngine::CreateInput(char* name, int device_id, int a2) {
    unsigned long input;

    if (device_id >= 0 && device_id <= 3) {
        switch (a2) {
            case 0x12345:
                input = reinterpret_cast<unsigned long>(new CInputHalfAxe(name, GetDevice(device_id), 14, 0));
                break;
            case 0x12346:
                input = reinterpret_cast<unsigned long>(new CInputHalfAxe(name, GetDevice(device_id), 14, 1));
                break;
            case 0x12347:
                input = reinterpret_cast<unsigned long>(new CInputHalfAxe(name, GetDevice(device_id), 15, 1));
                break;
            case 0x12348:
                input = reinterpret_cast<unsigned long>(new CInputHalfAxe(name, GetDevice(device_id), 15, 0));
                break;
            case 0x12349:
                input = reinterpret_cast<unsigned long>(new CInputHalfAxe(name, GetDevice(device_id), 16, 0));
                break;
            case 0x1234A:
                input = reinterpret_cast<unsigned long>(new CInputHalfAxe(name, GetDevice(device_id), 16, 1));
                break;
            case 0x1234B:
                input = reinterpret_cast<unsigned long>(new CInputHalfAxe(name, GetDevice(device_id), 17, 1));
                break;
            case 0x1234C:
                input = reinterpret_cast<unsigned long>(new CInputHalfAxe(name, GetDevice(device_id), 17, 0));
                break;
            default:
                input = reinterpret_cast<unsigned long>(new CInput(name, GetDevice(device_id), a2));
                break;
        }
    } else {
        input = reinterpret_cast<unsigned long>(new CInput(name, GetDevice(device_id), a2));
    }

    AS_ULONG_VECTOR_HACK(m_VInputs).push_back(input);
    return reinterpret_cast<CInput*>(input);
}

void IInputEngine::Open(void*, void*) {
    AS_ULONG_VECTOR_HACK(m_VDevices).reserve(2);
    AS_ULONG_VECTOR_HACK(m_VInputs).reserve(25);
}

}
