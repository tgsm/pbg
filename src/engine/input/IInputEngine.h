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
    static IDevice* GetDevice(int);
    static CCombo* CreateCombo(char*, char*, char*, int);
    static CInput* CreateInput(char*, int, int);
    static void DestroyInput(IInput* input);

    static DkXmd::CChunkIterator m_ChunkSetting;
    static std::vector<IInput*> m_VInputs;
    static std::vector<IDevice*> m_VDevices;
};

}

#endif
