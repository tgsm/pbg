#ifndef ENGINE_INPUT_IINPUTENGINE_H
#define ENGINE_INPUT_IINPUTENGINE_H

#include "engine/input/CInput.h"
#include "engine/input/CCombo.h"

namespace DKI {

// TODO
class IInputEngine {
public:
    static void Close();
    static void Clear();
    static IDevice* GetDevice(int);
    static CCombo* CreateCombo(char*, char*, char*, int);
    static CInput* CreateInput(char*, int, int);
    static void DestroyInput(IInput* input);
};

}

#endif
