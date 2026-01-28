#ifndef ENGINE_INPUT_IINPUTENGINE_H
#define ENGINE_INPUT_IINPUTENGINE_H

#include <vector>
#include "engine/input/CInput.h"
#include "engine/input/CCombo.h"

namespace DKI {

class IInputEngine {
public:
    static void Open(void*, void*);
    static void Close();
    static void Clear();
    static BOOL Update(F32 dt);
    static IDevice* GetDevice(int id);
    static CCombo* CreateCombo(char* name, char* input1_name, char* input2_name, int a4);
    static CInput* CreateInput(char* name, int device_id, int);
    static void DestroyInput(IInput* input);

    static inline IDevice* GetDeviceInline(int id);

    static inline IInput* GetInput(char* name) {
        IInput* input = NULL;
        for (U32 i = 0; i < m_VInputs.size() && input == NULL; i++) {
            if (strcmp(m_VInputs[i]->GetName(), name) == 0) {
                input = m_VInputs[i];
            }
        }
        return input;
    }

    static DkXmd::CChunkIterator m_ChunkSetting;
    static std::vector<IInput*> m_VInputs;
    static std::vector<IDevice*> m_VDevices;
};

}

#endif
