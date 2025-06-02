#ifndef ENGINE_INPUT_IINPUTENGINE_H
#define ENGINE_INPUT_IINPUTENGINE_H

#include "engine/input/IInput.h"

namespace DKI {

// TODO
class IInputEngine {
public:
    static void Close();
    static void Clear();
    static IDevice* GetDevice(int);
    static IInput* CreateInput(char*, int, int);
    static void DestroyInput(IInput* input);
};

}

#endif
